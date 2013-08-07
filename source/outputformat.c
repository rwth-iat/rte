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

/*
 * returns the format of the output
 * constants are in the config.h file
 */
OV_RESULT extract_response_format(OV_STRING_VEC* args, OV_UINT*response_format){
	OV_STRING_VEC match = {0,NULL};
	//output format
	find_arguments(args, "format", &match);
	if(match.veclen>=1){
		if(ov_string_compare(match.value[0], "ksx") == OV_STRCMP_EQUAL){
			*response_format = RESPONSE_FORMAT_KSX;
		}else if(ov_string_compare(match.value[0], "json") == OV_STRCMP_EQUAL){
			*response_format = RESPONSE_FORMAT_JSON;
		}else if(ov_string_compare(match.value[0], "tcl") == OV_STRCMP_EQUAL){
			*response_format = RESPONSE_FORMAT_TCL;
		}else if(ov_string_compare(match.value[0], "plain") == OV_STRCMP_EQUAL){
			*response_format = RESPONSE_FORMAT_PLAIN;
		}
	}
	Ov_SetDynamicVectorLength(&match,0,STRING);
	return OV_ERR_OK;
}

/**
 * appends the header for communication to the output string
 * in ksx opens the ks service xml element
 * @param output pointer to the string to manipulate
 * @param format UINT for the type of response
 * @param entry_type string for naming the following content (xml node name in KSX)
 * @return return code always ov_err_ok
 */
OV_RESULT printresponseheader(OV_STRING* output, OV_UINT response_format, OV_STRING entry_type){
	if(response_format==RESPONSE_FORMAT_KSX){
		ov_string_setvalue(output, "<response xmlns=\"http://acplt.org/schemas/ksx/2.0\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://acplt.org/schemas/ksx/2.0 ksx.xsd\">\n");
		return begin_response_part(output, response_format, entry_type);
	}else if(response_format==RESPONSE_FORMAT_JSON){
		return begin_response_part(output, response_format, entry_type);
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
OV_RESULT printresponsefooter(OV_STRING* output, OV_UINT response_format, OV_STRING entry_type){
	if(ov_string_compare(entry_type, NULL) == OV_STRCMP_EQUAL){
		return OV_ERR_GENERIC;
	}
	if(response_format==RESPONSE_FORMAT_KSX){
		finalize_response_part(output, response_format, entry_type);
		finalize_response_part(output, response_format, "response");
	}else if(response_format==RESPONSE_FORMAT_JSON){
		finalize_response_part(output, response_format, "response");
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
OV_RESULT init_response_part(OV_STRING* output, OV_UINT response_format, OV_STRING entry_type){
	ov_string_setvalue(output, "");
	return begin_response_part(output, response_format, entry_type);
}

/**
 * appends opening of element to output string
 * @param output pointer to the string to manipulate
 * @param format UINT for the type of response
 * @param entry_type string for naming the following content (xml node name in KSX)
 * @return return code always ov_err_ok
 */
OV_RESULT begin_response_part(OV_STRING* output, OV_UINT response_format, OV_STRING entry_type){
	if(ov_string_compare(entry_type, NULL) == OV_STRCMP_EQUAL){
		return OV_ERR_GENERIC;
	}
	if(response_format==RESPONSE_FORMAT_TCL){
		if(*output == NULL){
			ov_string_setvalue(output, "{");
		}else{
			ov_string_append(output, "{");
		}
	}else if(response_format==RESPONSE_FORMAT_KSX){
		if(*output == NULL){
			ov_string_print(output, "<%s>", entry_type);
		}else{
			ov_string_print(output, "%s<%s>", *output, entry_type);
		}
	}else if(response_format==RESPONSE_FORMAT_JSON){
		if(*output == NULL){
			ov_string_print(output, "{\"%s\": ", entry_type);
		}else{
			ov_string_print(output, "%s{\"%s\": ", *output, entry_type);
		}
	}else if(response_format==RESPONSE_FORMAT_PLAIN){
		//do nothing
	}
	return OV_ERR_OK;
}

/**
 * adds a seperator if needed in the response format
 * @param output pointer to the string to manipulate
 * @param format UINT for the type of response
 * @return return code always ov_err_ok
 */
OV_RESULT seperate_response_parts(OV_STRING* output, OV_UINT response_format){
	if(response_format==RESPONSE_FORMAT_TCL){
		if(*output == NULL){
			ov_string_setvalue(output, " ");
		}else{
			ov_string_append(output, " ");
		}
	}else if(response_format==RESPONSE_FORMAT_PLAIN){
		if(*output == NULL){
			ov_string_setvalue(output, " ");
		}else{
			ov_string_append(output, " ");
		}

	}else if(response_format==RESPONSE_FORMAT_KSX){
		//none
	}else if(response_format==RESPONSE_FORMAT_JSON){
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
OV_RESULT finalize_response_part(OV_STRING* output, OV_UINT response_format, OV_STRING entry_type){
	if(ov_string_compare(entry_type, NULL) == OV_STRCMP_EQUAL){
		return OV_ERR_GENERIC;
	}
	if(response_format==RESPONSE_FORMAT_TCL){
		if(*output == NULL){
			ov_string_setvalue(output, "}");
		}else{
			ov_string_append(output, "}");
		}
	}else if(response_format==RESPONSE_FORMAT_KSX){
		if(*output == NULL){
			ov_string_print(output, "</%s>\n", entry_type);
		}else{
			ov_string_print(output, "%s</%s>\n", *output, entry_type);
		}
	}else if(response_format==RESPONSE_FORMAT_JSON){
		if(*output == NULL){
			ov_string_print(output, "}\n");
		}else{
			ov_string_print(output, "%s}\n", *output);
		}
	}else if(response_format==RESPONSE_FORMAT_PLAIN){
		if(*output == NULL){
			ov_string_setvalue(output, ";");
		}else{
			ov_string_append(output, ";");
		}
	}
	return OV_ERR_OK;
}

/**
 * prints the info of one result or one result array
 * could be <failure> or <success> in ksx
 * clear text in tcl
 * @param output string to append to
 * @param response_format
 * @param results one result, or one array of results
 * @param len length of array, 1 if no array
 * @param explain_text additional text, which will be appended after the error
 * @return last error we had
 */
OV_RESULT print_result_array(OV_STRING *output, OV_UINT response_format, OV_RESULT *results, OV_UINT len, OV_STRING explain_text){
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
		begin_response_part(&temp, response_format, "mixed");
	}
	for (i=0; i< len;i++){
		if(Ov_Fail(results[i])){
			fr = results[i];
			begin_response_part(&temp, response_format, "failure");
			if(response_format == RESPONSE_FORMAT_KSX){
				if(temp == NULL){
					ov_string_print(&temp, "%i", fr);
				}else{
					ov_string_print(&temp, "%s%i", temp, fr);
				}
			}else{
				if(temp == NULL){
					ov_string_print(&temp, "%s%s", ov_result_getresulttext(fr), explain_text);
				}else{
					ov_string_print(&temp, "%s%s%s", temp, ov_result_getresulttext(fr), explain_text);
				}
			}
			finalize_response_part(&temp, response_format, "failure");
		}else{
			begin_response_part(&temp, response_format, "success");
			finalize_response_part(&temp, response_format, "success");
			if(len == 1 && lasterror == OV_ERR_OK){
				//only one success
				break;
			}
		}
	}
	if(len > 1 && lasterror != OV_ERR_OK){
		finalize_response_part(&temp, response_format, "mixed");
	}
	ov_string_append(output, temp);

	return lasterror;
}

/**
 * Convert a timestring into an XML, TCL or plaintext timestring
*/
OV_RESULT kshttp_timetoascii(OV_STRING* timestring, OV_TIME* time, OV_UINT response_format){
	//timetoascii has timeformat 2002/05/30 09:30:10.123456
	//TCL needs                  2002-05-30 09:30:10.123
	//XML needs                  2002-05-30T09:30:10.1
	//id in String               012345678901234567890123

	ov_string_setvalue(timestring, ov_time_timetoascii(time));

	//manipulate string to correct format, timetoascii garantees the correct length of the string
	(*timestring)[4] = '-';
	(*timestring)[7] = '-';
	if(response_format == RESPONSE_FORMAT_KSX){
		(*timestring)[10] = 'T';
		(*timestring)[21] = '\0';
	}else{
		(*timestring)[23] = '\0';
	}
	return OV_ERR_OK;
}

/**
 * Convert a timespan into an XML, TCL or plaintext timestring
 * XML PT42S Period: T time delimiter, 42 Seconds
 */
OV_RESULT kshttp_timespantoascii(OV_STRING* timestring, OV_TIME_SPAN* ptime, OV_UINT response_format){
	/*
	*	local variables
	*/
	OV_INT			secs = ptime->secs;

	if(response_format != RESPONSE_FORMAT_KSX){
		ov_string_print(timestring, "%i.%06i", ptime->secs, ptime->usecs);
		return OV_ERR_OK;
	}
	//XML format requested
	if(ptime-secs < 0){
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

/* escapes a String to make a valid XML/TCL String
possible changes in XML:
"	&quot;
'	&apos;
<	&lt;
>	&gt;
&	&amp;
in TCL:
" -> \"
{ -> \{
} -> \}

 */
OV_RESULT kshttp_escapeString(OV_STRING* resultString, OV_STRING* strIn, OV_UINT response_format){
	OV_STRING heapString;
	OV_STRING	pcIn;
	OV_STRING	pcOut = 0;
	OV_RESULT	fr = OV_ERR_OK;

	if(response_format == RESPONSE_FORMAT_KSX){
		//malloc worstcase stringlength which is &quot;
		heapString = Ov_HeapMalloc(6*ov_string_getlength(*strIn)+1);
	}else if(response_format == RESPONSE_FORMAT_TCL){
		//malloc worstcase stringlength which is \}
		heapString = Ov_HeapMalloc(2*ov_string_getlength(*strIn)+1);
	}else if(response_format == RESPONSE_FORMAT_PLAIN){
		//no manipulation is done here
		heapString = Ov_HeapMalloc(ov_string_getlength(*strIn)+1);
	}else if(response_format == RESPONSE_FORMAT_JSON){
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
			if(response_format == RESPONSE_FORMAT_TCL || response_format == RESPONSE_FORMAT_JSON){
				*pcOut = '\\';
				pcOut++;
				*pcOut = '"';
			}else if(response_format == RESPONSE_FORMAT_KSX){
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
			}else if(response_format == RESPONSE_FORMAT_PLAIN){
				//no escaping here
				*pcOut = *pcIn;
			}else{
				//should not hit
				*pcOut = *pcIn;
			}
		}else if(*pcIn == '\'' && response_format == RESPONSE_FORMAT_KSX){
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
		}else if(*pcIn == '<' && response_format == RESPONSE_FORMAT_KSX){
			*pcOut = '&';
			pcOut++;
			*pcOut = 'l';
			pcOut++;
			*pcOut = 't';
			pcOut++;
			*pcOut = ';';
		}else if(*pcIn == '>' && response_format == RESPONSE_FORMAT_KSX){
			*pcOut = '&';
			pcOut++;
			*pcOut = 'g';
			pcOut++;
			*pcOut = 't';
			pcOut++;
			*pcOut = ';';
		}else if(*pcIn == '&' && response_format == RESPONSE_FORMAT_KSX){
			*pcOut = '&';
			pcOut++;
			*pcOut = 'a';
			pcOut++;
			*pcOut = 'm';
			pcOut++;
			*pcOut = 'p';
			pcOut++;
			*pcOut = ';';
		}else if(*pcIn == '{' && response_format == RESPONSE_FORMAT_TCL){
			*pcOut = '\\';
			pcOut++;
			*pcOut = '{';
		}else if(*pcIn == '}' && response_format == RESPONSE_FORMAT_TCL){
			*pcOut = '\\';
			pcOut++;
			*pcOut = '}';
		}else if(*pcIn == '\\' && response_format == RESPONSE_FORMAT_JSON){
			*pcOut = '\\';
			pcOut++;
			*pcOut = '\\';
		}else if(*pcIn == '\t' && response_format == RESPONSE_FORMAT_JSON){
			*pcOut = '\\';
			pcOut++;
			*pcOut = 't';
		}else if(*pcIn == '\b' && response_format == RESPONSE_FORMAT_JSON){
			*pcOut = '\\';
			pcOut++;
			*pcOut = 'b';
		}else if(*pcIn == '\n' && response_format == RESPONSE_FORMAT_JSON){
			*pcOut = '\\';
			pcOut++;
			*pcOut = 'n';
		}else if(*pcIn == '\r' && response_format == RESPONSE_FORMAT_JSON){
			*pcOut = '\\';
			pcOut++;
			*pcOut = 'r';
		}else if(*pcIn == '/' && response_format == RESPONSE_FORMAT_JSON){
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
