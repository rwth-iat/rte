#include "config.h"

/*
 * returns the format of the output
 * constants are in the config.h file
 */
OV_UINT extract_output_format(OV_STRING_VEC* args){
	//todo move format into http accept header
	OV_UINT re = RESPONSE_FORMAT_DEFAULT;
	OV_STRING_VEC match = {0,NULL};
	//output format
	find_arguments(args, "format", &match);
	if(match.veclen>=1){
		if(ov_string_compare(match.value[0], "ksx") == OV_STRCMP_EQUAL){
			re = RESPONSE_FORMAT_KSX;
		}else if(ov_string_compare(match.value[0], "plain") == OV_STRCMP_EQUAL){
			re = RESPONSE_FORMAT_PLAIN;
		}
	}
	Ov_SetDynamicVectorLength(&match,0,STRING);
	return re;
}

OV_RESULT printresponseheader(OV_STRING* output, OV_UINT format, OV_STRING entry_type){
	if(format==RESPONSE_FORMAT_KSX){
		ov_string_setvalue(output, "<result xmlns=\"http://acplt.org/schemas/ksx/2.0\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://acplt.org/schemas/ksx/2.0 ksx.xsd\">");
		begin_vector_output(output, format, entry_type);
	}else if(format==RESPONSE_FORMAT_PLAIN){
		ov_string_append(output, ";");
	}
	return OV_ERR_OK;
}
OV_RESULT printresponsefooter(OV_STRING* output, OV_UINT format, OV_STRING entry_type){
	if(format==RESPONSE_FORMAT_KSX){
		finalize_vector_output(output, format, entry_type);
		finalize_vector_output(output, format, "result");
	}
	return OV_ERR_OK;
}

/*
 * VECTOR OUTPUT BUSINESS
 */
OV_RESULT init_vector_output(OV_STRING* output, OV_UINT format, OV_STRING entry_type){
	ov_string_setvalue(output, "");
	return begin_vector_output(output, format, entry_type);
}

OV_RESULT split_vector_output(OV_STRING* output, OV_UINT format, OV_STRING entry_type){
	if(format==RESPONSE_FORMAT_TCL){
		ov_string_append(output, "} {");
	}else if(format==RESPONSE_FORMAT_KSX){
		if(*output == NULL){
			//should not happen
			ov_string_print(output, "</%s>\n<%s>", entry_type, entry_type);
		}else{
			ov_string_print(output, "%s</%s>\n<%s>", *output, entry_type, entry_type);
		}
	}else if(format==RESPONSE_FORMAT_PLAIN){
		ov_string_append(output, ";");
	}
	return OV_ERR_OK;
}

OV_RESULT begin_vector_output(OV_STRING* output, OV_UINT format, OV_STRING entry_type){
	if(format==RESPONSE_FORMAT_TCL){
		ov_string_append(output, "{");
	}else if(format==RESPONSE_FORMAT_KSX){
		if(*output == NULL){
			ov_string_print(output, "<%s>", entry_type);
		}else{
			ov_string_print(output, "%s<%s>", *output, entry_type);
		}
	}else if(format==RESPONSE_FORMAT_PLAIN){
		ov_string_append(output, "");
	}
	return OV_ERR_OK;
}

OV_RESULT finalize_vector_output(OV_STRING* output, OV_UINT format, OV_STRING entry_type){
	if(format==RESPONSE_FORMAT_TCL){
		ov_string_append(output, "}");
	}else if(format==RESPONSE_FORMAT_KSX){
		if(*output == NULL){
			//should not happen
			ov_string_print(output, "</%s>\n", entry_type);
		}else{
			ov_string_print(output, "%s</%s>\n", *output, entry_type);
		}
	}else if(format==RESPONSE_FORMAT_PLAIN){
		ov_string_append(output, ";");
	}
	return OV_ERR_OK;
}
