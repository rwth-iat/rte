#include "config.h"

/*
 * returns the format of the output
 * constants are in the config.h file
 */
OV_UINT extract_output_format(OV_STRING_VEC* args){
	//todo move format into http accept header
	OV_UINT re = GETVAR_FORMAT_DEFAULT;
	OV_STRING_VEC match = {0,NULL};
	//output format
	find_arguments(args, "format", &match);
	if(match.veclen>=1){
		if(ov_string_compare(match.value[0], "plain") == OV_STRCMP_EQUAL){
			re = GETVAR_FORMAT_PLAIN;
		}
	}
	Ov_SetDynamicVectorLength(&match,0,STRING);
	return re;
}

/*
 * VECTOR OUTPUT BUSINESS
 */
OV_RESULT init_vector_output(OV_STRING* output, OV_UINT format){
	ov_string_setvalue(output, "");
	return begin_vector_output(output, format);
}

OV_RESULT split_vector_output(OV_STRING* output, OV_UINT format){
	if(format==GETVAR_FORMAT_TCL){
		ov_string_append(output, "}{");
	}else{
		ov_string_append(output, ";");
	}
	return OV_ERR_OK;
}

OV_RESULT begin_vector_output(OV_STRING* output, OV_UINT format){
	if(format==GETVAR_FORMAT_TCL){
		ov_string_append(output, "{");
	}else{
		ov_string_append(output, "");
	}
	return OV_ERR_OK;
}

OV_RESULT finalize_vector_output(OV_STRING* output, OV_UINT format){
	if(format==GETVAR_FORMAT_TCL){
		ov_string_append(output, "}");
	}else{
		ov_string_append(output, "");
	}
	return OV_ERR_OK;
}
