/*
 *	Copyright (C) 2015
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

#include "config.h"

#if OV_SYSTEM_MC164
#include "mc164/time.h"
#else
//struct tm, but not time_t on unix
#include <time.h>
#endif

#if OV_SYSTEM_UNIX
//time_t
#include <sys/time.h>
#endif



/**
 * appends the header for communication to the output string
 * in ksx opens the ks service xml element
 * @param output pointer to the string to manipulate
 * @param format UINT for the type of response
 * @param entry_type string for naming the following content (xml node name in KSX)
 * @return return code always ov_err_ok
 */
OV_RESULT kshttp_printresponseheader(OV_STRING* output, const HTTP_RESPONSEFORMAT response_format, const OV_STRING entry_type){
	if(response_format==KSX){
		ov_string_setvalue(output, "<response xmlns=\"http://acplt.org/schemas/ksx/2.0\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://acplt.org/schemas/ksx/2.0 ksx.xsd\">\n");
		return kshttp_response_part_begin(output, response_format, entry_type);
	}else if(response_format==KSJSON){
		return kshttp_response_part_begin(output, response_format, entry_type);
	}
	return OV_ERR_OK;
}
/**
 * appends the footer for communication to the output string
 * in ksx closes the ks service xml element
 * @param output pointer to the string to manipulate
 * @param format UINT for the type of response
 * @param entry_type string for naming the following content (xml node name in KSX)
 * @return return code always ov_err_ok
 */
OV_RESULT kshttp_printresponsefooter(OV_STRING* output, const HTTP_RESPONSEFORMAT response_format, const OV_STRING entry_type){
	if(ov_string_compare(entry_type, NULL) == OV_STRCMP_EQUAL){
		return OV_ERR_GENERIC;
	}
	if(response_format==KSX){
		kshttp_response_part_finalize(output, response_format, entry_type);
		kshttp_response_part_finalize(output, response_format, "response");
	}else if(response_format==KSJSON){
		kshttp_response_part_finalize(output, response_format, "response");
	}
	return OV_ERR_OK;
}

/**
 * clears output string and opens element
 * @param output pointer to the string to manipulate
 * @param format UINT for the type of response
 * @param entry_type string for naming the following content (xml node name in KSX)
 * @return return code always ov_err_ok
 */
OV_RESULT kshttp_response_part_init(OV_STRING* output, const HTTP_RESPONSEFORMAT response_format, const OV_STRING entry_type){
	ov_string_setvalue(output, "");
	return kshttp_response_part_begin(output, response_format, entry_type);
}

/**
 * appends opening of element to output string
 * @param output pointer to the string to manipulate
 * @param format UINT for the type of response
 * @param entry_type string for naming the following content (xml node name in KSX)
 * @return return code always ov_err_ok
 */
OV_RESULT kshttp_response_part_begin(OV_STRING* output, const HTTP_RESPONSEFORMAT response_format, const OV_STRING entry_type){
	if(response_format==KSTCL){
		if(*output == NULL){
			ov_string_setvalue(output, "{");
		}else{
			ov_string_append(output, "{");
		}
		return OV_ERR_OK;
	}else if(response_format==PLAIN){
		//do nothing
		return OV_ERR_OK;
	}
	//the following formats needs a entry_type to be valid
	if(ov_string_compare(entry_type, NULL) == OV_STRCMP_EQUAL){
		return OV_ERR_GENERIC;
	}
	if(response_format==KSX){
		if(*output == NULL){
			ov_string_print(output, "<%s>", entry_type);
		}else{
			ov_string_print(output, "%s<%s>", *output, entry_type);
		}
	}else if(response_format==KSJSON){
		if(*output == NULL){
			ov_string_print(output, "{\"%s\": ", entry_type);
		}else{
			ov_string_print(output, "%s{\"%s\": ", *output, entry_type);
		}
	}
	return OV_ERR_OK;
}

/**
 * adds a seperator if needed in the response format
 * @param output pointer to the string to manipulate
 * @param format UINT for the type of response
 * @return return code always ov_err_ok
 */
OV_RESULT kshttp_response_parts_seperate(OV_STRING* output, const HTTP_RESPONSEFORMAT response_format){
	if(response_format==KSTCL){
		if(*output == NULL){
			ov_string_setvalue(output, " ");
		}else{
			ov_string_append(output, " ");
		}
	}else if(response_format==PLAIN){
		if(*output == NULL){
			ov_string_setvalue(output, " ");
		}else{
			ov_string_append(output, " ");
		}

	}else if(response_format==KSX){
		//none
	}else if(response_format==KSJSON){
		if(*output == NULL){
			ov_string_setvalue(output, ", ");
		}else{
			ov_string_append(output, ", ");
		}
	}
	return OV_ERR_OK;
}

/**
 * appends closing of element to output string
 * @param output pointer to the string to manipulate
 * @param format UINT for the type of response
 * @param entry_type string for naming the following content (xml node name in KSX)
 * @return return code always ov_err_ok
 */
OV_RESULT kshttp_response_part_finalize(OV_STRING* output, const HTTP_RESPONSEFORMAT response_format, const OV_STRING entry_type){
	if(response_format==KSTCL){
		if(*output == NULL){
			ov_string_setvalue(output, "}");
		}else{
			ov_string_append(output, "}");
		}
	}else if(response_format==PLAIN){
		if(*output == NULL){
			ov_string_setvalue(output, ";");
		}else{
			ov_string_append(output, ";");
		}
	}
	//the following formats needs a entry_type to be valid
	if(ov_string_compare(entry_type, NULL) == OV_STRCMP_EQUAL){
		return OV_ERR_GENERIC;
	}
	if(response_format==KSX){
		if(*output == NULL){
			ov_string_print(output, "</%s>\n", entry_type);
		}else{
			ov_string_print(output, "%s</%s>\n", *output, entry_type);
		}
	}else if(response_format==KSJSON){
		if(*output == NULL){
			ov_string_print(output, "}\n");
		}else{
			ov_string_print(output, "%s}\n", *output);
		}
	}
	return OV_ERR_OK;
}

/**
 * prints the info of one result or one result array
 * could be <failure> or <success> in ksx
 * clear text in KSTCL
 * @param output string to append to
 * @param response_format
 * @param results one result, or one array of results
 * @param len length of array, 1 if no array
 * @param explain_text additional text, which will be appended after the error
 * @return last error we had
 */
OV_RESULT kshttp_print_result_array(OV_STRING *output, const HTTP_RESPONSEFORMAT response_format, OV_RESULT *results, const OV_UINT len, const OV_STRING explain_text){
	OV_UINT i = 0;
	OV_STRING temp = NULL;
	OV_RESULT fr = OV_ERR_OK;
	OV_RESULT lasterror = OV_ERR_OK;


/*
<response xmlns="http://acplt.org/schemas/ksx/2.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://acplt.org/schemas/ksx/2.0 ksx.xsd">
	<setvar>
		<success/>
	</setvar>
</response>
or
<response xmlns="http://acplt.org/schemas/ksx/2.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://acplt.org/schemas/ksx/2.0 ksx.xsd">
	<setvar>
		<mixed>
			<failure>17</failure>
			<success/>
		</mixed>
	</setvar>
</response>
or
<response xmlns="http://acplt.org/schemas/ksx/2.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://acplt.org/schemas/ksx/2.0 ksx.xsd">
	<setvar>
		<failure>17</failure>
	</setvar>
</response>
*/

	for (i=0; i< len;i++){
		if(Ov_Fail(results[i])){
			lasterror = results[i];
		}
	}

	if(len > 1 && lasterror != OV_ERR_OK){
		//wrap in "mixed" if multiple results were given
		kshttp_response_part_begin(&temp, response_format, "mixed");
	}
	for (i=0; i< len;i++){
		if(Ov_Fail(results[i])){
			fr = results[i];
			kshttp_response_part_begin(&temp, response_format, "failure");
			if(response_format == KSX){
				if(temp == NULL){
					ov_string_print(&temp, "%i", fr);
				}else{
					ov_string_print(&temp, "%s%i", temp, fr);
				}
			}else{
				if(temp == NULL){
					ov_string_print(&temp, "%s: %s%s", kshttp_mapresult2string(fr), ov_result_getresulttext(fr), explain_text);
				}else{
					ov_string_print(&temp, "%s%s: %s%s", temp, kshttp_mapresult2string(fr), ov_result_getresulttext(fr), explain_text);
				}
			}
			kshttp_response_part_finalize(&temp, response_format, "failure");
		}else{
			kshttp_response_part_begin(&temp, response_format, "success");
			kshttp_response_part_finalize(&temp, response_format, "success");
			if(len == 1 && lasterror == OV_ERR_OK){
				//only one success
				break;
			}
		}
	}
	if(len > 1 && lasterror != OV_ERR_OK){
		kshttp_response_part_finalize(&temp, response_format, "mixed");
	}
	ov_string_append(output, temp);
	if(temp){
		ov_string_setvalue(&temp, NULL);
	}

	return lasterror;
}

/**
 * Convert a time into a XML, KSTCL or plaintext timestring (local time)
*/
OV_RESULT kshttp_timetoascii(OV_STRING* timestring, const OV_TIME* time, const HTTP_RESPONSEFORMAT response_format){
	//timetoascii has timeformat 2002/05/30 09:30:10.123456
	//KSTCL needs                2002-05-30 09:30:10.123
	//XML needs                  2002-05-30T09:30:10.123456
	//id in String               01234567890123456789012345

	ov_string_setvalue(timestring, ov_time_timetoascii_local(time));

	//manipulate string to correct format, timetoascii garantees the correct length of the string
	(*timestring)[4] = '-';
	(*timestring)[7] = '-';
	if(response_format == KSX){
		(*timestring)[10] = 'T';
	}else{
		(*timestring)[23] = '\0';
	}
	return OV_ERR_OK;
}

/**
 * Convert a XML, KSTCL or plaintext timestring into a time (local time)
*/
OV_RESULT kshttp_asciitotime(OV_TIME* time, const OV_STRING timestring){
	//asciitotime has timeformat 2002/05/30 09:30:10.123456
	//KSTCL needs                2002-05-30 09:30:10.123
	//XML needs                  2002-05-30T09:30:10.123456
	//id in String               01234567890123456789012345

	OV_STRING timetemp = NULL;
	OV_RESULT fr = OV_ERR_OK;

	if(ov_string_getlength(timestring) < 18){
		return OV_ERR_BADPARAM;
	}
	//work on a copy of the string
	ov_string_setvalue(&timetemp, timestring);
	//manipulate string to correct format
	timetemp[4] = '/';
	timetemp[7] = '/';
	timetemp[10] = ' ';
	fr = ov_time_asciitotime_local(time, timetemp);

	ov_string_setvalue(&timetemp, NULL);
	return fr;
}

/**
 * Convert a timespan into a XML, KSTCL or plaintext timestring
 * XML PT42S Period: T time delimiter, 42 Seconds
 */
OV_RESULT kshttp_timespantoascii(OV_STRING* timestring, const OV_TIME_SPAN* ptime, const HTTP_RESPONSEFORMAT response_format){
	/*
	*	local variables
	*/
	OV_INT			secs = ptime->secs;

	if(response_format != KSX){
		ov_string_print(timestring, "%i.%06i", ptime->secs, ptime->usecs);
		return OV_ERR_OK;
	}
	//XML format requested
	if(ptime->secs < 0){
		//will be checked at the end
		secs = -ptime->secs;
	}
	if(ptime->usecs != 0){
		ov_string_print(timestring, "%i.%06iS", secs, ptime->usecs);
	}else{
		ov_string_print(timestring, "%iS", secs);
	}
	if(ptime->secs < 0){
		ov_string_print(timestring, "-PT%s", *timestring);
	}else{
		ov_string_print(timestring, "PT%s", *timestring);
	}
	return OV_ERR_OK;
}

/** escapes a String to make a valid XML/KSTCL String
	possible changes in XML:
	"	&quot;
	'	&apos;
	<	&lt;
	>	&gt;
	&	&amp;
	in KSTCL:
	" -> \"
	{ -> \{
	} -> \}
 *
 * @param resultString pointer to an OV_STRING as output
 * @param strIn pointer to an OV_STRING to escape
 * @param response_format
 * @return
 */
OV_RESULT kshttp_escapeString(OV_STRING* resultString, OV_STRING *strIn, const HTTP_RESPONSEFORMAT response_format){
	OV_STRING heapString;
	OV_STRING	pcIn;
	OV_STRING	pcOut = 0;
	OV_RESULT	fr = OV_ERR_OK;

	if(*strIn == NULL){
		return ov_string_setvalue(resultString, "");
	}
	
	if(response_format == KSX){
		//malloc worstcase stringlength which is &quot;
		heapString = Ov_HeapMalloc(6*ov_string_getlength(*strIn)+1);
	}else if(response_format == KSTCL){
		//malloc worstcase stringlength which is \}
		heapString = Ov_HeapMalloc(2*ov_string_getlength(*strIn)+1);
	}else if(response_format == PLAIN){
		//no manipulation is done here
		heapString = Ov_HeapMalloc(ov_string_getlength(*strIn)+1);
	}else if(response_format == KSJSON){
		//malloc worstcase stringlength which is \}
		heapString = Ov_HeapMalloc(2*ov_string_getlength(*strIn)+1);
	}else{
		//no manipulation is done here
		heapString = Ov_HeapMalloc(ov_string_getlength(*strIn)+1);
	}

	pcIn = *strIn;
	pcOut = heapString;
	while(*pcIn) {
		if(*pcIn == '"'){
			if(response_format == KSTCL || response_format == KSJSON){
				*pcOut = '\\';
				pcOut++;
				*pcOut = '"';
			}else if(response_format == KSX){
				*pcOut = '&';
				pcOut++;
				*pcOut = 'q';
				pcOut++;
				*pcOut = 'u';
				pcOut++;
				*pcOut = 'o';
				pcOut++;
				*pcOut = 't';
				pcOut++;
				*pcOut = ';';
			}else if(response_format == PLAIN){
				//no escaping here
				*pcOut = *pcIn;
			}else{
				//should not hit
				*pcOut = *pcIn;
			}
		}else if(*pcIn == '\'' && response_format == KSX){
			*pcOut = '&';
			pcOut++;
			*pcOut = 'a';
			pcOut++;
			*pcOut = 'p';
			pcOut++;
			*pcOut = 'o';
			pcOut++;
			*pcOut = 's';
			pcOut++;
			*pcOut = ';';
		}else if(*pcIn == '<' && response_format == KSX){
			*pcOut = '&';
			pcOut++;
			*pcOut = 'l';
			pcOut++;
			*pcOut = 't';
			pcOut++;
			*pcOut = ';';
		}else if(*pcIn == '>' && response_format == KSX){
			*pcOut = '&';
			pcOut++;
			*pcOut = 'g';
			pcOut++;
			*pcOut = 't';
			pcOut++;
			*pcOut = ';';
		}else if(*pcIn == '&' && response_format == KSX){
			*pcOut = '&';
			pcOut++;
			*pcOut = 'a';
			pcOut++;
			*pcOut = 'm';
			pcOut++;
			*pcOut = 'p';
			pcOut++;
			*pcOut = ';';
		}else if(*pcIn == '\n' && response_format == KSTCL){
			*pcOut = '\\';
			pcOut++;
			*pcOut = 'n';
		}else if(*pcIn == '\t' && response_format == KSTCL){
			*pcOut = '\\';
			pcOut++;
			*pcOut = 't';
/*		causes more problems that it solves...
		}else if(*pcIn == ' ' && response_format == KSTCL){
			*pcOut = '\\';
			pcOut++;
			*pcOut = ' ';
*/
		}else if(*pcIn == '$' && response_format == KSTCL){
			*pcOut = '\\';
			pcOut++;
			*pcOut = '$';
		}else if(*pcIn == '{' && response_format == KSTCL){
			*pcOut = '\\';
			pcOut++;
			*pcOut = '{';
		}else if(*pcIn == '}' && response_format == KSTCL){
			*pcOut = '\\';
			pcOut++;
			*pcOut = '}';
		}else if(*pcIn == '[' && response_format == KSTCL){
			*pcOut = '\\';
			pcOut++;
			*pcOut = '[';
		}else if(*pcIn == ']' && response_format == KSTCL){
			*pcOut = '\\';
			pcOut++;
			*pcOut = ']';
		}else if(*pcIn == '\\' && response_format == KSTCL){
			*pcOut = '\\';
			pcOut++;
			*pcOut = '\\';
		}else if(*pcIn == '\\' && response_format == KSJSON){
			*pcOut = '\\';
			pcOut++;
			*pcOut = '\\';
		}else if(*pcIn == '\t' && response_format == KSJSON){
			*pcOut = '\\';
			pcOut++;
			*pcOut = 't';
		}else if(*pcIn == '\b' && response_format == KSJSON){
			*pcOut = '\\';
			pcOut++;
			*pcOut = 'b';
		}else if(*pcIn == '\n' && response_format == KSJSON){
			*pcOut = '\\';
			pcOut++;
			*pcOut = 'n';
		}else if(*pcIn == '\r' && response_format == KSJSON){
			*pcOut = '\\';
			pcOut++;
			*pcOut = 'r';
		}else if(*pcIn == '/' && response_format == KSJSON){
			*pcOut = '\\';
			pcOut++;
			*pcOut = '/';
		}else{
			*pcOut = *pcIn;
		}
		pcIn++;
		pcOut++;
	}
	*pcOut = '\0';		/*append terminating '\0'*/
	fr = ov_string_setvalue(resultString, heapString);
	Ov_HeapFree(heapString);
	return fr;
}
