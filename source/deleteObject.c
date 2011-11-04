#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "ksapi_h.h"
#include "libov/ov_scheduler.h"

#include "xdrhandling.h"
#include "config.h"


/*	ksapi_deleteObject_startup
 *	On startup set all values to default and close all connections
 */
OV_DLLFNCEXPORT void ksapi_deleteObject_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	//OV_INSTPTR_ksapi_deleteObject sobj = Ov_StaticPtrCast(ksapi_deleteObject, pobj);
	//To avoid a double Startup, which occures, because startup is executed while constructing 
	//the object and starting the FB, this check is implemented
	if (pobj->v_objectstate & OV_OS_STARTED) {
		return;
	}
	ksapi_KSCommon_startup(pobj);
	//initialize Values
	
	return;
}

/*	ksapi_deleteObject_shutdown
 *	After closing the service and the connection, shutdown the object.
 */
OV_DLLFNCEXPORT void ksapi_deleteObject_shutdown(
	OV_INSTPTR_ov_object	pobj
) {
	ksapi_KSCommon_shutdown(pobj);
	return;
}

/*	ksapi_deleteObject_submit
 *	If all parameters were manually set, you can use this function to read the string
 */
OV_DLLFNCEXPORT void ksapi_deleteObject_submit(
	OV_INSTPTR_ksapi_deleteObject	pobj
) {
	OV_VTBLPTR_ksapi_deleteObject   pvtableop; 
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	OV_INSTPTR_ksapi_Channel channel = (OV_INSTPTR_ksapi_Channel)Ov_SearchChild(ov_containment, pobj, "channel");
	char *xdr;
	int xdrlength;

	Ov_GetVTablePtr(ksapi_deleteObject, pvtableop, pobj);
	
	if(channel == NULL) {
	   ov_logfile_error("DELETEOBJECT object without Channel to use: %s", pobj->v_identifier);
	   pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "no channel Object", 2);
	   return;
	}

	if((pobj->v_host) && (pobj->v_server) && (pobj->v_path)
			&& (ksapi_KSCommon_status_get(pksc) == STATUS_KSCOMMON_OK)){
		//all values are set
		
		//send
		generatedeleteobjectxdr(&xdr, &xdrlength, pobj->v_path);
		ksapi_Channel_sendxdr(channel, pksc, xdr, xdrlength);
	}
	else if(ksapi_KSCommon_status_get(pksc) == STATUS_KSCOMMON_BUSY)
	{
		log_error("deleteObject: connection in use");
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "connection in use", 5);
	}
	else {
		//not all values are set
		log_error("deleteObject: Not all values are set. Reading cancelled");
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "Not all values are set", 2);
	}
	return;
}


/*	ksapi_deleteObject_setandsubmit
 *	this function sets the necessary values automatically and starts the reading
 */
OV_DLLFNCEXPORT void ksapi_deleteObject_setandsubmit(
	OV_INSTPTR_ksapi_deleteObject	pobj,
	OV_STRING						host,
	OV_STRING						server,
	OV_STRING						path
) {
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	//set values:
	if (Ov_Fail(ksapi_KSCommon_host_set(pksc, host))) {
		log_error("deleteObject: error setting host");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_server_set(pksc, server))) {
		log_error("deleteObject: error setting server");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_path_set(pksc, path))) {
		log_error("deleteObject: error setting path");
		return;
	}
	ksapi_deleteObject_submit(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_deleteObject_returnMethodxdr(
		OV_INSTPTR_ksapi_KSCommon	 			kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
	OV_VTBLPTR_ksapi_KSCommon pvtableop;
	
	Ov_GetVTablePtr(ksapi_KSCommon, pvtableop, kscommon);

	//Analyse xdr
	analysedeleteobjectreply(xdr, xdrlength);
	pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "Reading completed", 1);
	
	return;
}
