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

#include "libov/ov_ov.h"
#include "libov/ov_macros.h"
#include "libov/ov_result.h"

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
	OV_STRING mask = NULL;
	OV_STRING temp = NULL;
	OV_STRING compare = NULL;
	//initialize the return vector properly
	Ov_SetDynamicVectorLength(re,0,STRING);
	if(args == NULL || varname == NULL)return OV_ERR_OK;
	//iterate over argument names
	for(i = 0;i < args->veclen;i=i+2){
		//simple match -> put value in the output list
		if(ov_string_compare(args->value[i], varname) == OV_STRCMP_EQUAL){
			Ov_SetDynamicVectorLength(re,re->veclen+1,STRING);
			ov_string_setvalue(&(re->value[re->veclen-1]), args->value[i+1]);
		}else{
			//match for varname and an opening bracket
			if(ov_string_getlength(args->value[i]) > ov_string_getlength(varname)){
				//if varname = var search for var[ to allocate a list of variables
				//construct the mask to cut off
				ov_string_append(&mask, "%.");
				ov_string_print(&temp, "%i", ov_string_getlength(varname)+1);
				ov_string_append(&mask, temp);
				ov_string_append(&mask, "s");
				//cut off the first length+1 characters
				ov_string_print(&temp, mask, args->value[i]);
				//compare the cut off with "varname["
				ov_string_setvalue(&compare, "");
				ov_string_print(&compare, "%s[", varname);
				if(ov_string_compare(temp, compare) == OV_STRCMP_EQUAL){
					Ov_SetDynamicVectorLength(re,re->veclen+1,STRING);
					ov_string_setvalue(&(re->value[re->veclen-1]), args->value[i+1]);
				}
				ov_string_setvalue(&mask, NULL);
				ov_string_setvalue(&temp, NULL);
				ov_string_setvalue(&compare, NULL);
			}
		}
	}
	return OV_ERR_OK;
}

/**
 * Parse raw http HTTP request into a get command and a list of arguments
 *
 * @param buffer input raw request
 * @param cmd output parsed get command
 * @param args output string vector of form value content
 */
OV_RESULT parse_http_request(OV_STRING buffer, OV_STRING* cmd, OV_STRING_VEC* args)
{
    OV_STRING *plist, *pelement;
    OV_UINT i, len, len1;
	//initialize return vector properly
	Ov_SetDynamicVectorLength(args,0,STRING);
    //split out the first line containing the GET command
    plist = ov_string_split(buffer, "\n", &len);
    if(len<=0){
    	return OV_ERR_BADPARAM; //400
    }
    ov_string_setvalue(&buffer, plist[0]);
    ov_string_freelist(plist);
    //split out the actual GET request
    plist = ov_string_split(buffer, " ", &len);
    if(len<1){
    	return OV_ERR_BADPARAM; //400
    }
    ov_string_setvalue(&buffer, plist[1]);
    ov_string_freelist(plist);
    //get the command, cmd contains the /-prefexed call now
    //buffer contains the vars and args (raw)
    //check if the command contains an ?
    plist = ov_string_split(buffer, "?", &len);
    //no ? -> return the full command, args are empty
    if(len<0){
    	ov_string_setvalue(cmd, buffer);
    	return OV_ERR_OK;
    }
    //at least one ? -> split up the command
    ov_string_setvalue(cmd, plist[0]);
    //exactly one ? -> we are done
    if(len == 1) {
        return OV_ERR_OK;
    }
    //not yet done, parsing args
    ov_string_setvalue(&buffer, plist[1]);
    ov_string_freelist(plist);

    plist = ov_string_split(buffer, "&", &len);
    if(len <= 0)return OV_ERR_OK;
    Ov_SetDynamicVectorLength(args,2*len,STRING);
    for(i = 0;i < len;i++){
        pelement = ov_string_split(plist[i], "=", &len1);
        //varname=value
        if(len1==2){
        	if(pelement[0][0] == '\0'){
            	return OV_ERR_BADPARAM; //400;
        	}
			ov_string_setvalue(&(*args).value[2 * i], pelement[0]);
			ov_string_setvalue(&(*args).value[2 * i + 1], pelement[1]);
        }else{
        	return OV_ERR_BADPARAM; //400;
        }
    }
    ov_string_freelist(plist);
    return OV_ERR_OK;
}
