
/******************************************************************************
*
*   FILE
*   ----
*   uaServerToInterfaces.c
*
*   History
*   -------
*   2019-01-24   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_opcua
#define OV_COMPILE_LIBRARY_opcua
#endif


#include "opcua.h"
#include "ov_association.h"

#include "ov_macros.h"

OV_DLLFNCEXPORT OV_RESULT opcua_serverToInterfaces_link(
	const OV_PPT_opcua_serverToInterfaces	pparent,
	const OV_CPT_opcua_serverToInterfaces	pchild,
	const OV_PLACEMENT_HINT	parenthint,
	const OV_CPT_opcua_serverToInterfaces	prelparent,
	const OV_PLACEMENT_HINT	childhint,
	const OV_CPT_opcua_serverToInterfaces	prelchild
){
	OV_INSTPTR_opcua_server pServer = Ov_StaticPtrCast(opcua_server, pparent);

	//Don't load interface if server is running
	if(pServer != NULL && pServer->v_isRunning){
		return OV_ERR_GENERIC;
	}

	return ov_association_link(passoc_opcua_serverToInterfaces,
			Ov_PtrUpCast(ov_object, pparent),
			Ov_PtrUpCast(ov_object, pchild),
			parenthint,
			Ov_PtrUpCast(ov_object, prelparent),
			childhint,
			Ov_PtrUpCast(ov_object, prelchild));
}

OV_DLLFNCEXPORT void opcua_serverToInterfaces_unlink(
	const OV_PPT_opcua_serverToInterfaces	pparent,
	const OV_CPT_opcua_serverToInterfaces	pchild
){
	OV_INSTPTR_opcua_server pServer = Ov_StaticPtrCast(opcua_server, pparent);

	//Don't unload interface if server is running
	if(pServer != NULL && pServer->v_isRunning){
		return;
	}

	// remove the link
	ov_association_unlink(passoc_opcua_serverToInterfaces,
			Ov_PtrUpCast(ov_object, pparent),
			Ov_PtrUpCast(ov_object, pchild));
}

OV_IMPL_GETACCESS(opcua_serverToInterfaces)