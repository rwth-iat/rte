
/******************************************************************************
*
*   FILE
*   ----
*   ProtocolIdentificator.c
*
*   History
*   -------
*   2013-01-15   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_ksbase
#define OV_COMPILE_LIBRARY_ksbase
#endif


#include "ksbase.h"
#include "libov/ov_macros.h"
#include "ks_logfile.h"


OV_DLLFNCEXPORT OV_BOOL ksbase_ProtocolIdentificator_identify(
	OV_INSTPTR_ksbase_ProtocolIdentificator this,
	OV_INSTPTR_ksbase_Channel pchannel
) {
	ks_logfile_warning("This protocol identificator has no functionality: %s", this->v_identifier);
	return FALSE;
}

OV_DLLFNCEXPORT OV_RESULT ksbase_ProtocolIdentificator_createClientHandler(
	OV_INSTPTR_ksbase_ProtocolIdentificator this,
	OV_INSTPTR_ksbase_Channel pchannel
) {
	ks_logfile_warning("This protocol identificator has no functionality: %s", this->v_identifier);
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_ACCESS ksbase_ProtocolIdentificator_getaccess(
	OV_INSTPTR_ov_object		pobj,
	const OV_ELEMENT			*pelem,
	const OV_TICKET				*pticket
) {
	/*
	*	local variables
	*/

	/*
	*	switch based on the element's type
	*/
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
