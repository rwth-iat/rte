
/******************************************************************************
*
*   FILE
*   ----
*   ServiceInputArguments.c
*
*   History
*   -------
*   2017-05-29   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_openaas
#define OV_COMPILE_LIBRARY_openaas
#endif


#include "openaas.h"
#include "libov/ov_macros.h"


OV_DLLFNCEXPORT OV_ACCESS openaas_ServiceInputArguments_getaccess(
	OV_INSTPTR_ov_object	pobj,
	const OV_ELEMENT		*pelem,
	const OV_TICKET			*pticket
) {
    /*    
    *   local variables
    */
	return (OV_ACCESS)(OV_AC_WRITE | OV_AC_READ | OV_AC_LINKABLE | OV_AC_UNLINKABLE | OV_AC_DELETEABLE | OV_AC_RENAMEABLE);
}

