
/******************************************************************************
*
*   FILE
*   ----
*   uaBaseNodeType.c
*
*   History
*   -------
*   2014-10-24   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_iec62541
#define OV_COMPILE_LIBRARY_iec62541
#endif


#include "iec62541.h"
#include "libov/ov_macros.h"


OV_DLLFNCEXPORT OV_ACCESS iec62541_uaBaseNodeType_getaccess(
	OV_INSTPTR_ov_object	pobj,
	const OV_ELEMENT		*pelem,
	const OV_TICKET			*pticket
) {
   switch(pelem->elemtype) {
	case OV_ET_VARIABLE:
		if(pelem->elemunion.pvar->v_offset >= offsetof(OV_INST_ov_object,__classinfo)) {
			if(pelem->elemunion.pvar->v_vartype == OV_VT_CTYPE)
				return OV_AC_NONE;
			else
				return OV_AC_READWRITE;
		}
		break;
	default:
		break;
	}

	return ov_object_getaccess(pobj, pelem, pticket);
}

