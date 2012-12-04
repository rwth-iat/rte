/*
*	Copyright (C) 2012
*	Chair of Process Control Engineering,
*	Aachen University of Technology.
*	All rights reserved.
*
*	Redistribution and use in source and binary forms, with or without
*	modification, are permitted provided that the following conditions
*	are met:
*	1. Redistributions of source code must retain the above copyright
*	   notice, this list of conditions and the following disclaimer.
*	2. Redistributions in binary form must print or display the above
*	   copyright notice either during startup or must have a means for
*	   the user to view the copyright notice.
*	3. Redistributions in binary form must reproduce the above copyright
*	   notice, this list of conditions and the following disclaimer in
*		the documentation and/or other materials provided with the
*		distribution.
*	4. Neither the name of the Chair of Process Control Engineering nor
*		the name of the Aachen University of Technology may be used to
*		endorse or promote products derived from this software without
*		specific prior written permission.
*
*	THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
*	``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE CHAIR OF
*	PROCESS CONTROL ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT,
*	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
*	OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
*	AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
*	WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*	POSSIBILITY OF SUCH DAMAGE.
*
***********************************************************************/

#include <ctype.h>
#include "config.h"

/**
 * This function will searches for matching varname(s) and returns a list of values
 * Note: varnames like var[0], var[1], var[xx], var[ are matched and returned in the list
 *
 * @param args argument pair array returned by parse_http_request
 * @param varname mask to search like "varname"
 * @param re return list
 * @return always success
 */
OV_RESULT find_arguments(OV_STRING_VEC* args, const OV_STRING varname, OV_STRING_VEC* re){
	int i = 0;
	int varname_len = 0;
	Ov_SetDynamicVectorLength(re,0,STRING);	//initialize the return vector properly
	if(args == NULL || varname == NULL)return OV_ERR_OK;
	//iterate over argument names
	for(i = 0;i < args->veclen;i=i+2){
		//simple match -> put value in the output list
		varname_len = ov_string_getlength(varname);
		if(strncmp(args->value[i], varname, varname_len) == OV_STRCMP_EQUAL){
			if((int)ov_string_getlength(args->value[i]) == varname_len){
				//direct variablename in args
				Ov_SetDynamicVectorLength(re,re->veclen+1,STRING);
				ov_string_setvalue(&(re->value[re->veclen-1]), args->value[i+1]);
			}else{
				//match for varname and an opening bracket
				//if varname = var => search for var[ to allocate a list of variables
				if((args->value[i])[varname_len] == '['){
					Ov_SetDynamicVectorLength(re,re->veclen+1,STRING);
					ov_string_setvalue(&(re->value[re->veclen-1]), args->value[i+1]);
				}
			}
		}
	}
	return OV_ERR_OK;
}

#define PARSE_HTTP_HEADER_RETURN ov_string_setvalue(&rawrequest, NULL);\
		ov_string_freelist(pallheaderslist);\
		ov_string_freelist(plist);\
		ov_string_freelist(pelement);\
		return
/**
 * Parse raw http HTTP request into a get command and a list of arguments
 *
 * @param buffer input raw request
 * @param cmd output parsed get command
 * @param args output string vector of form value content
 * @param http_version sets HTTP/1.1 or HTTP/1.0
 */
OV_RESULT parse_http_header(OV_STRING buffer, OV_STRING* cmd, OV_STRING_VEC* args, OV_STRING* http_version, OV_STRING* http_request_type, OV_BOOL *gzip_accepted, OV_BOOL *keep_alive)
{
	OV_STRING* pallheaderslist=NULL;
	OV_UINT allheaderslength = 0;
	OV_STRING* plist=NULL;
	OV_STRING* pelement=NULL;
	OV_STRING rawrequest=NULL;

	OV_UINT i, len, len1;

	//initialize return vector properly
	Ov_SetDynamicVectorLength(args,0,STRING);

	//checking if Accept-Encoding: gzip
	pallheaderslist = ov_string_split(buffer, "\r\n", &allheaderslength);
	if(allheaderslength<=0){
		PARSE_HTTP_HEADER_RETURN OV_ERR_BADPARAM; //400
	}
	for (i=0; i<allheaderslength; i++){
		if(i==0){
			//split out the first line containing the GET command
			ov_string_setvalue(&rawrequest, pallheaderslist[i]);
		}else if(ov_string_match(pallheaderslist[i], "Accept-Encoding:*") == TRUE){
			if(ov_string_match(pallheaderslist[i], "*gzip*") == TRUE){
				*gzip_accepted = TRUE;
			}
		}else if(ov_string_compare(pallheaderslist[i], "Connection: close") == OV_STRCMP_EQUAL){
			//scan header for Connection: close - the default behavior is keep-alive
			*keep_alive = FALSE;
		}else if(ov_string_match(pallheaderslist[i], "Accept:*") == TRUE){
			//fixme define http handler
			//move extract_output_format into this
		}
	}
	ov_string_freelist(pallheaderslist);

	ov_string_freelist(plist);
	//split out the actual GET request
	plist = ov_string_split(rawrequest, " ", &len);
	if(len!=3){
		PARSE_HTTP_HEADER_RETURN OV_ERR_BADPARAM; //400
	}
	ov_string_setvalue(http_request_type, plist[0]);
	ov_string_setvalue(&rawrequest, plist[1]);
	//does the client use HTTP 1.0?
	if(ov_string_compare(plist[2], "HTTP/1.0") == OV_STRCMP_EQUAL){
		//if so, use 1.0, otherwise 1.1 is set
		ov_string_setvalue(http_version, "1.0");
	}else{
		//default HTTP version
		ov_string_setvalue(http_version, "1.1");
	}

	//get the command, cmd contains the /-prefexed call now
	//rawrequest contains the vars and args (raw)
	//check if the command contains an ?
	ov_string_freelist(plist);
	plist = ov_string_split(rawrequest, "?", &len);
	//no ? -> return the full command, args are empty
	if(len<0){
		ov_string_setvalue(cmd, rawrequest);
		PARSE_HTTP_HEADER_RETURN OV_ERR_OK;
	}
	//at least one ? -> split up the command
	ov_string_setvalue(cmd, plist[0]);
	//exactly one ? -> we are done
	if(len == 1) {
		PARSE_HTTP_HEADER_RETURN OV_ERR_OK;
	}
	//not yet done, parsing args
	ov_string_setvalue(&rawrequest, plist[1]);

	ov_string_freelist(plist);
	plist = ov_string_split(rawrequest, "&", &len);
	if(len <= 0){
		PARSE_HTTP_HEADER_RETURN OV_ERR_OK;
	}
	Ov_SetDynamicVectorLength(args,2*len,STRING);
	for(i = 0;i < len;i++){
		ov_string_freelist(pelement);
		pelement = ov_string_split(plist[i], "=", &len1);
		//varname=value
		if(len1==2){
			if(pelement[0][0] == '\0'){
				PARSE_HTTP_HEADER_RETURN OV_ERR_BADPARAM; //400;
			}
			ov_string_setvalue(&(*args).value[2 * i], pelement[0]);
			ov_string_setvalue(&(*args).value[2 * i + 1], pelement[1]);
		}else{
			PARSE_HTTP_HEADER_RETURN OV_ERR_BADPARAM; //400;
		}
	}

	PARSE_HTTP_HEADER_RETURN OV_ERR_OK;
}



//TODO: Merge to OV or rename
/**
*	Converts percent characters in ascii characters, but skips /
*	Note: the memory for the returned string is allocated on the memory
*	stack, use ov_memstack_lock()/unlock() outside of this function
*/
OV_DLLFNCEXPORT OV_STRING ov_path_topercent_noslash (
				OV_STRING org
) {
	OV_STRING newstring;
	unsigned char *s,*d;
	unsigned int upper, lower;

	newstring = (OV_STRING) ov_memstack_alloc(ov_path_percentsize(org)+1);
	d = (unsigned char *)org;
	s = (unsigned char *)newstring;
	while (*d) {
		if (*d == '/'){
			//preserve Slash
			*s = *d;
		}else
		if (!ov_path_isvalidchar(*d)) {
			upper = (*d) >> 4;
			lower = (*d) % 16;
			s[0] = '%';
			s[1] = (unsigned char)upper + (upper < 10 ? '0' : '7');
			s[2] = (unsigned char)lower + (lower < 10 ? '0' : '7');
			s = s + 2;
		}
		else *s = *d;
		s++;
		d++;
	}
	*s = 0;
	return newstring;
}
