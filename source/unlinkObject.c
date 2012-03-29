#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "ksapi_h.h"
#include "libov/ov_scheduler.h"

#include "xdrhandling.h"
#include "config.h"


/*	ksapi_unlinkObject_linkedpath_get
 *	returns the current value of the linkedpath
 */
OV_DLLFNCEXPORT OV_STRING ksapi_unlinkObject_linkedpath_get(
	OV_INSTPTR_ksapi_unlinkObject	pobj
) {
	return pobj->v_linkedpath;
}

/*	ksapi_unlinkObject_linkedpath_set
 *	sets the current linkedpath to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_unlinkObject_linkedpath_set(
	OV_INSTPTR_ksapi_unlinkObject	pobj,
	const OV_STRING					value
) {
	return ov_string_setvalue(&pobj->v_linkedpath, value);
}

/*	ksapi_unlinkObject_startup
 *	On startup set all values to default and close all connections
 */
OV_DLLFNCEXPORT void ksapi_unlinkObject_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	//OV_INSTPTR_ksapi_unlinkObject sobj = Ov_StaticPtrCast(ksapi_unlinkObject, pobj);
	//To avoid a double Startup, which occures, because startup is executed while constructing 
	//the object and starting the FB, this check is implemented
	if (pobj->v_objectstate & OV_OS_STARTED) {
		return;
	}
	ksapi_KSCommon_startup(pobj);
	//initialize Values
	
	return;
}

/*	ksapi_unlinkObject_shutdown
 *	After closing the service and the connection, shutdown the object.
 */
OV_DLLFNCEXPORT void ksapi_unlinkObject_shutdown(
	OV_INSTPTR_ov_object	pobj
) {
	ksapi_KSCommon_shutdown(pobj);
	return;
}

/*	ksapi_unlinkObject_submit
 *	If all parameters were manually set, you can use this function to read the string
 */
OV_DLLFNCEXPORT void ksapi_unlinkObject_submit(
	OV_INSTPTR_ksapi_unlinkObject	pobj
) {
	OV_VTBLPTR_ksapi_unlinkObject   pvtableop; 
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	char *xdr;
	int xdrlength;
	OV_INSTPTR_ksapi_Channel channel;

	Ov_GetVTablePtr(ksapi_unlinkObject, pvtableop, pobj);
	
	channel = (OV_INSTPTR_ksapi_Channel)Ov_SearchChild(ov_containment, pobj, "channel");
	if(channel == NULL) {
	    ov_logfile_error("UNLINKOBJECT object without Channel to use: %s", pobj->v_identifier);
	    pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "no channel Object", 2);
	    return;
	}
	
	if((pobj->v_host) && (pobj->v_server) && (pobj->v_linkedpath) && (pobj->v_path) && (ksapi_KSCommon_status_get(pksc) != STATUS_KSCOMMON_BUSY)){
		//all values are set
		
		//send
		generateunlinkobjectxdr(&xdr, &xdrlength, pobj->v_path, pobj->v_linkedpath);
		ksapi_Channel_sendxdr(channel, pksc, xdr, xdrlength);
	
	}
	else if(ksapi_KSCommon_status_get(pksc) == STATUS_KSCOMMON_BUSY)
	{
		log_error("unlinkObject: connection in use");
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "connection in use", 5);
	}
	else {
		//not all values are set
		log_error("link: Not all values are set. Reading cancelled");
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "Not all values are set", 2);
	}
	return;
}


/*	ksapi_unlinkObject_setandsubmit
 *	this function sets the necessary values automatically and starts the reading
 */
OV_DLLFNCEXPORT void ksapi_unlinkObject_setandsubmit(
	OV_INSTPTR_ksapi_unlinkObject	pobj,
	OV_STRING						host,
	OV_STRING						server,
	OV_STRING						path,
	OV_STRING						linkedpath
) {
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	//set values:
	if (Ov_Fail(ksapi_KSCommon_host_set(pksc, host))) {
		log_error("link: error setting host");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_server_set(pksc, server))) {
		log_error("link: error setting server");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_path_set(pksc, path))) {
		log_error("link: error setting path");
		return;
	}
	if (Ov_Fail(ksapi_unlinkObject_linkedpath_set(pobj, linkedpath))) {
		log_error("link: error setting linkedpath");
		return;
	}
	ksapi_unlinkObject_submit(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_unlinkObject_returnMethodxdr(
		OV_INSTPTR_ksapi_KSCommon			 	kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
	OV_VTBLPTR_ksapi_KSCommon pvtableop;

	Ov_GetVTablePtr(ksapi_KSCommon, pvtableop, kscommon);

	//Analyse xdr
	analyseresultonlyreply(xdr, xdrlength);
	pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "Reading completed", 1);
	
	return;
}
