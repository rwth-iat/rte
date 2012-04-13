

#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "ksapi_h.h"
#include "libov/ov_scheduler.h"

#include "xdrhandling.h"
#include "config.h"


/*	ksapi_createObject_librarypath_get
 *	returns the current value of the librarypath
 */
OV_DLLFNCEXPORT OV_STRING ksapi_createObject_librarypath_get(
	OV_INSTPTR_ksapi_createObject	pobj
) {
	return pobj->v_librarypath;
}

/*	ksapi_createObject_librarypath_set
 *	sets the current librarypath to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_createObject_librarypath_set(
	OV_INSTPTR_ksapi_createObject	pobj,
	const OV_STRING					value
) {
	return ov_string_setvalue(&pobj->v_librarypath, value);
}

/*	ksapi_createObject_position_get
 *	returns the current value of the position
 */
OV_DLLFNCEXPORT OV_INT ksapi_createObject_position_get(
	OV_INSTPTR_ksapi_createObject		pobj
) {
	return pobj->v_position;
}

/*	ksapi_createObject_position_set
 *	sets the current position to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_createObject_position_set(
	OV_INSTPTR_ksapi_createObject		pobj,
	const OV_INT           		value
) {
	pobj->v_position=value;
	return OV_ERR_OK;
}

/*	ksapi_createObject_element_get
 *	returns the current value of the element
 */
OV_DLLFNCEXPORT OV_STRING ksapi_createObject_element_get(
	OV_INSTPTR_ksapi_createObject	pobj
) {
	return pobj->v_element;
}

/*	ksapi_createObject_element_set
 *	sets the current element to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_createObject_element_set(
	OV_INSTPTR_ksapi_createObject	pobj,
	const OV_STRING					value
) {
	return ov_string_setvalue(&pobj->v_element, value);
}

/*	ksapi_createObject_startup
 *
 *	On startup set all values to default and close all connections
 *	
 *	
 */
OV_DLLFNCEXPORT void ksapi_createObject_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	//OV_INSTPTR_ksapi_createObject sobj = Ov_StaticPtrCast(ksapi_createObject, pobj);
	//To avoid a double Startup, which occures, because startup is executed while constructing 
	//the object and starting the FB, this check is implemented
	if (pobj->v_objectstate & OV_OS_STARTED) {
		return;
	}
	ksapi_KSCommon_startup(pobj);
	//initialize Values
	
	return;
}

/*	ksapi_createObject_shutdown
 *	After closing the service and the connection, shutdown the object.
 */
OV_DLLFNCEXPORT void ksapi_createObject_shutdown(
	OV_INSTPTR_ov_object	pobj
) {
	ksapi_KSCommon_shutdown(pobj);
	return;
}

/*	ksapi_createObject_submit
 *	If all parameters were manually set, you can use this function to read the string
 */
OV_DLLFNCEXPORT void ksapi_createObject_submit(
	OV_INSTPTR_ksapi_createObject	pobj
) {
	OV_VTBLPTR_ksapi_createObject   pvtableop; 
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	char *xdr;
	int xdrlength;
	OV_INSTPTR_ksapi_Channel channel;
	
	Ov_GetVTablePtr(ksapi_createObject, pvtableop, pobj);
	
	channel = (OV_INSTPTR_ksapi_Channel)Ov_SearchChild(ov_containment, pobj, "channel");
	if(channel == NULL) {
	    ov_logfile_error("CREATEOBJECT object without Channel to use: %s", pobj->v_identifier);
	    pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "no channel Object", 2);
	    return;
	}

	if((pobj->v_host) && (pobj->v_server) && (pobj->v_librarypath) && (pobj->v_path)
			&& (ksapi_KSCommon_status_get(pksc) == STATUS_KSCOMMON_OK)){
		//all values are set
	
		//send
		generatecreateobjectxdr(&xdr, &xdrlength, pobj->v_path, pobj->v_librarypath, pobj->v_position, pobj->v_element);
		ksapi_Channel_sendxdr(channel, pksc, xdr, xdrlength);
	} else if(ksapi_KSCommon_status_get(pksc) == STATUS_KSCOMMON_BUSY) {
		log_error("createObject: connection in use");
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "connection in use", 5); // TODO 5 ?
	} else {
		//not all values are set
		log_error("createObject: Not all values are set. Reading cancelled");
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "Not all values are set", 2); // TODO 2 ?
	}
	return;
}


/*	ksapi_createObject_setandsubmit
 *	this function sets the necessary values automatically and starts the reading
 */
OV_DLLFNCEXPORT void ksapi_createObject_setandsubmit(
	OV_INSTPTR_ksapi_createObject	pobj,
	OV_STRING						host,
	OV_STRING						server,
	OV_STRING						path,
	OV_STRING						librarypath,
	OV_INT							position,
	OV_STRING						element
) {
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	//set values:
	if (Ov_Fail(ksapi_KSCommon_host_set(pksc, host))) {
		log_error("createObject: error setting host");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_server_set(pksc, server))) {
		log_error("createObject: error setting server");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_path_set(pksc, path))) {
		log_error("createObject: error setting path");
		return;
	}
	if (Ov_Fail(ksapi_createObject_librarypath_set(pobj, librarypath))) {
		log_error("createObject: error setting librarypath");
		return;
	}
	if (Ov_Fail(ksapi_createObject_position_set(pobj, position))) {
		log_error("createObject: error setting position");
		return;
	}
	if (Ov_Fail(ksapi_createObject_element_set(pobj, element))) {
		log_error("createObject: error setting element");
		return;
	}
	ksapi_createObject_submit(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_createObject_returnMethodxdr(
		OV_INSTPTR_ksapi_KSCommon			 			kscommon,
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
