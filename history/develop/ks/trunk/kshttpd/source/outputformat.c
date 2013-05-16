#include "config.h"

/*
 * returns the format of the output
 * constants are in the config.h file
 */
OV_UINT extract_response_format(OV_STRING_VEC* args){
	OV_UINT re = RESPONSE_FORMAT_NONE;
	OV_STRING_VEC match = {0,NULL};
	//output format
	find_arguments(args, "format", &match);
	if(match.veclen>=1){
		if(ov_string_compare(match.value[0], "ksx") == OV_STRCMP_EQUAL){
			re = RESPONSE_FORMAT_KSX;
		}else if(ov_string_compare(match.value[0], "json") == OV_STRCMP_EQUAL){
			re = RESPONSE_FORMAT_JSON;
		}else if(ov_string_compare(match.value[0], "tcl") == OV_STRCMP_EQUAL){
			re = RESPONSE_FORMAT_TCL;
		}else if(ov_string_compare(match.value[0], "plain") == OV_STRCMP_EQUAL){
			re = RESPONSE_FORMAT_PLAIN;
		}
	}
	Ov_SetDynamicVectorLength(&match,0,STRING);
	return re;
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
		ov_string_setvalue(output, "<result xmlns=\"http://acplt.org/schemas/ksx/2.0\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://acplt.org/schemas/ksx/2.0 ksx.xsd\">\n");
		begin_response_part(output, response_format, entry_type);
	}else if(response_format==RESPONSE_FORMAT_JSON){
		begin_response_part(output, response_format, entry_type);
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
	if(response_format==RESPONSE_FORMAT_KSX){
		finalize_response_part(output, response_format, entry_type);
		finalize_response_part(output, response_format, "result");
	}else if(response_format==RESPONSE_FORMAT_JSON){
		finalize_response_part(output, response_format, "result");
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
 * unused
 * @param output pointer to the string to manipulate
 * @param format UINT for the type of response
 * @param entry_type string for naming the following content (xml node name in KSX)
 * @return return code always ov_err_ok
 */
OV_RESULT split_response_parts(OV_STRING* output, OV_UINT response_format, OV_STRING entry_type){
	if(response_format==RESPONSE_FORMAT_TCL){
		ov_string_append(output, "} {");
	}else if(response_format==RESPONSE_FORMAT_KSX){
		if(*output == NULL){
			//should not happen
			ov_string_print(output, "</%s>\n<%s>", entry_type, entry_type);
		}else{
			ov_string_print(output, "%s</%s>\n<%s>", *output, entry_type, entry_type);
		}
	}else if(response_format==RESPONSE_FORMAT_PLAIN){
		ov_string_append(output, ";");
	}
	return OV_ERR_OK;
}

/**
 * appends opening of element to output string
 * @param output pointer to the string to manipulate
 * @param format UINT for the type of response
 * @param entry_type string for naming the following content (xml node name in KSX)
 * @return return code always ov_err_ok
 */
OV_RESULT begin_response_part(OV_STRING* output, OV_UINT response_format, OV_STRING entry_type){
	if(response_format==RESPONSE_FORMAT_TCL){
		ov_string_append(output, "{");
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
		ov_string_append(output, "");
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
		ov_string_append(output, " ");
	}else if(response_format==RESPONSE_FORMAT_PLAIN){
		ov_string_append(output, " ");
	}else if(response_format==RESPONSE_FORMAT_KSX){
		//none
	}else if(response_format==RESPONSE_FORMAT_JSON){
		ov_string_append(output, ", ");
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
	if(response_format==RESPONSE_FORMAT_TCL){
		ov_string_append(output, "}");
	}else if(response_format==RESPONSE_FORMAT_KSX){
		if(*output == NULL){
			//should not happen
			ov_string_print(output, "</%s>\n", entry_type);
		}else{
			ov_string_print(output, "%s</%s>\n", *output, entry_type);
		}
	}else if(response_format==RESPONSE_FORMAT_JSON){
		if(*output == NULL){
			//should not happen
			ov_string_print(output, "}\n");
		}else{
			ov_string_print(output, "%s}\n", *output);
		}
	}else if(response_format==RESPONSE_FORMAT_PLAIN){
		ov_string_append(output, ";");
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
		}
	}
	ov_string_append(output, temp);
	return fr;
}
