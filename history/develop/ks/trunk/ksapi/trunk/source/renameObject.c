#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "ksapi_h.h"
#include "libov/ov_scheduler.h"

#include "xdrhandling.h"
#include "config.h"



/*	ksapi_renameObject_newpath_get
 *	returns the current value of the newpath
 */
OV_DLLFNCEXPORT OV_STRING ksapi_renameObject_newpath_get(
	OV_INSTPTR_ksapi_renameObject	pobj
) {
	return pobj->v_newpath;
}

/*	ksapi_renameObject_newpath_set
 *	sets the current newpath to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_renameObject_newpath_set(
	OV_INSTPTR_ksapi_renameObject	pobj,
	const OV_STRING					value
) {
	return ov_string_setvalue(&pobj->v_newpath, value);
}

/*	ksapi_renameObject_startup
 *	On startup set all values to default and close all connections
 */
OV_DLLFNCEXPORT void ksapi_renameObject_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	//OV_INSTPTR_ksapi_renameObject sobj = Ov_StaticPtrCast(ksapi_renameObject, pobj);
	//To avoid a double Startup, which occures, because startup is executed while constructing 
	//the object and starting the FB, this check is implemented
	if (pobj->v_objectstate & OV_OS_STARTED) {
		return;
	}
	ksapi_KSCommon_startup(pobj);
	//initialize Values
	
	return;
}

/*	ksapi_renameObject_shutdown
 *	After closing the service and the connection, shutdown the object.
 */
OV_DLLFNCEXPORT void ksapi_renameObject_shutdown(
	OV_INSTPTR_ov_object	pobj
) {
	ksapi_KSCommon_shutdown(pobj);
	return;
}

/*	ksapi_renameObject_submit
 *	If all parameters were manually set, you can use this function to read the string
 */
OV_DLLFNCEXPORT void ksapi_renameObject_submit(
	OV_INSTPTR_ksapi_renameObject	pobj
) {
	OV_VTBLPTR_ksapi_renameObject   pvtableop; 
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	char *xdr;
	int xdrlength;
	OV_INSTPTR_ksapi_Channel channel;

	Ov_GetVTablePtr(ksapi_renameObject, pvtableop, pobj);

	channel = (OV_INSTPTR_ksapi_Channel)Ov_SearchChild(ov_containment, pobj, "channel");
	if(channel == NULL) {
	    ov_logfile_error("RENAMEOBJECT object without Channel to use: %s", pobj->v_identifier);
	    pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "no channel Object", 2);
	    return;
	}

	if((pobj->v_host) && (pobj->v_server) && (pobj->v_path) && (pobj->v_newpath) && (ksapi_KSCommon_status_get(pksc) != STATUS_KSCOMMON_BUSY)){
		//all values are set
		
		//send
		generaterenameobjectxdr(&xdr, &xdrlength, pobj->v_path, pobj->v_newpath);
		ksapi_Channel_sendxdr(channel, pksc, xdr, xdrlength);
	
	}
	else if(ksapi_KSCommon_status_get(pksc) == STATUS_KSCOMMON_BUSY)
	{
		log_error("renameObject: connection in use");
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "connection in use", 5);
	}
	else {
		//not all values are set
		log_error("renameObject: Not all values are set. Reading cancelled");
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "Not all values are set", 2);
	}
	return;
}


/*	ksapi_renameObject_setandsubmit
 *	this function sets the necessary values automatically and starts the reading
 */
OV_DLLFNCEXPORT void ksapi_renameObject_setandsubmit(
	OV_INSTPTR_ksapi_renameObject	pobj,
	OV_STRING						host,
	OV_STRING						server,
	OV_STRING						path,
	OV_STRING						newpath
) {
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	//set values:
	if (Ov_Fail(ksapi_KSCommon_host_set(pksc, host))) {
		log_error("renameObject: error setting host");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_server_set(pksc, server))) {
		log_error("renameObject: error setting server");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_path_set(pksc, path))) {
		log_error("renameObject: error setting path");
		return;
	}
	if (Ov_Fail(ksapi_renameObject_newpath_set(pobj, newpath))) {
		log_error("renameObject: error setting newpath");
		return;
	}
	ksapi_renameObject_submit(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_renameObject_returnMethodxdr(
		OV_INSTPTR_ksapi_KSCommon			 	kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
	OV_VTBLPTR_ksapi_KSCommon pvtableop;
	
	Ov_GetVTablePtr(ksapi_KSCommon, pvtableop, kscommon);

	//Analyse xdr
	analyserenameobjectreply(xdr, xdrlength);
	pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "Reading completed", 1);
	
	return;
}
