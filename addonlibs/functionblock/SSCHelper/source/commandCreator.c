/******************************************************************************
 *
 *   FILE
 *   ----
 *   commandCreator.c
 *
 *   History
 *   -------
 *   2015-01-05   File created
 *
 *******************************************************************************
 *
 *   This file is generated by the 'acplt_builder' command
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_SSChelper
#define OV_COMPILE_LIBRARY_SSChelper
#endif

#include "SSChelper.h"
#include "libov/ov_macros.h"

OV_DLLFNCEXPORT OV_RESULT SSChelper_commandCreator_start_set(
		OV_INSTPTR_SSChelper_commandCreator pobj, const OV_BOOL value) {

	if (value == TRUE) {
		ov_string_setvalue(&pobj->v_command, pobj->v_sender);
		ov_string_append(&pobj->v_command,";");
		ov_string_append(&pobj->v_command,pobj->v_order);
		ov_string_append(&pobj->v_command,";");
		ov_string_append(&pobj->v_command,pobj->v_param);
	}
	pobj->v_start = FALSE;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT void SSChelper_commandCreator_typemethod(
		OV_INSTPTR_fb_functionblock pfb, OV_TIME *pltc) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_SSChelper_commandCreator pinst =
			Ov_StaticPtrCast(SSChelper_commandCreator, pfb);

	return;
}

