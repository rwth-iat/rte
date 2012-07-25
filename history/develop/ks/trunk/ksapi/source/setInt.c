#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "ksapi_h.h"
#include "libov/ov_scheduler.h"

#include "xdrhandling.h"
#include "config.h"


OV_DLLFNCEXPORT OV_TIME* ksapi_setInt_varTimeStamp_get(
    OV_INSTPTR_ksapi_setInt          pobj
) {
    return &pobj->v_varTimeStamp;
}

OV_DLLFNCEXPORT OV_RESULT ksapi_setInt_varTimeStamp_set(
    OV_INSTPTR_ksapi_setInt          pobj,
    const OV_TIME*  value
) {
    pobj->v_varTimeStamp = *value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT ksapi_setInt_varQState_get(
    OV_INSTPTR_ksapi_setInt          pobj
) {
    return pobj->v_varQState;
}

OV_DLLFNCEXPORT OV_RESULT ksapi_setInt_varQState_set(
    OV_INSTPTR_ksapi_setInt          pobj,
    const OV_UINT  value
) {
    pobj->v_varQState = value;
    return OV_ERR_OK;
}

/*	ksapi_setInt_sendint_get
 *	returns the current value of the sendint
 */
OV_DLLFNCEXPORT OV_INT ksapi_setInt_sendint_get(
	OV_INSTPTR_ksapi_setInt		pobj
) {
	return pobj->v_sendint;
}

/*	ksapi_setInt_sendint_set
 *	sets the current sendint to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_setInt_sendint_set(
	OV_INSTPTR_ksapi_setInt		pobj,
	const OV_INT           		value
) {
	pobj->v_sendint=value;
	return OV_ERR_OK;
}

/*	ksapi_setInt_startup
 *	On startup set all values to default and close all connections
 */
OV_DLLFNCEXPORT void ksapi_setInt_startup(
	OV_INSTPTR_ov_object	pobj
) {
	OV_INSTPTR_ksapi_setInt sobj	=	Ov_StaticPtrCast(ksapi_setInt, pobj);
	//To avoid a double Startup, which occures, because startup is executed while constructing 
	//the object and starting the FB, this check is implemented
	if (pobj->v_objectstate & OV_OS_STARTED) {
		return;
	}
	ksapi_KSCommon_startup(pobj);
	//initialize Values
	ksapi_setInt_sendint_set(sobj,0);
	
	return;
}

/*	ksapi_setInt_shutdown
 *	After closing the service and the connection, shutdown the object.
 */
OV_DLLFNCEXPORT void ksapi_setInt_shutdown(
	OV_INSTPTR_ov_object	pobj
) {
	ksapi_KSCommon_shutdown(pobj);
	return;
}

/*	ksapi_setInt_submit
 *	If all parameters were manually set, you can use this function to send the string
 */
OV_DLLFNCEXPORT void ksapi_setInt_submit(
		OV_INSTPTR_ksapi_setInt	 			pobj
) {
	OV_VTBLPTR_ksapi_setInt   pvtableop; 
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	OV_INSTPTR_ksapi_Channel channel;
	char *xdr=NULL;
	int xdrlength;

	Ov_GetVTablePtr(ksapi_setInt, pvtableop, pobj);

	channel = (OV_INSTPTR_ksapi_Channel)Ov_SearchChild(ov_containment, pobj, "channel");
	if(channel == NULL) {
	    ov_logfile_error("SETINT object without Channel to use: %s", pobj->v_identifier);
	    pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "no channel Object", 2);
	    return;
	}

	if((pobj->v_host) && (pobj->v_server) && (pobj->v_path) && (ksapi_KSCommon_status_get(pksc) != STATUS_KSCOMMON_BUSY)){
		//all values are set
		
	generatesetintxdr(&xdr, &xdrlength, pobj->v_path, pobj->v_sendint, pobj->v_varTimeStamp.secs, pobj->v_varTimeStamp.usecs, pobj->v_varQState);
	

	//send
	ksapi_Channel_sendxdr(channel, pksc, xdr, xdrlength);
	
	free(xdr);
		
	}
	else if(ksapi_KSCommon_status_get(pksc) == STATUS_KSCOMMON_BUSY)
	{
		log_error("setInt: connection in use");
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "connection in use", 5);
	}
	else {
		//not all values are set
		log_error("setInt: Not all values are set. Sending cancelled");
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "Not all values are set", 2);
	}
	return;
}


/*	ksapi_setInt_setandsubmit
 *	this function sets the necessary values automatically and starts the sending
 */
OV_DLLFNCEXPORT void ksapi_setInt_setandsubmit(
	OV_INSTPTR_ksapi_setInt		pobj,
	OV_STRING						host,
	OV_STRING						server,
	OV_STRING						path,
	OV_INT							sendint
) {
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	//set values:
	if (Ov_Fail(ksapi_setInt_sendint_set(pobj, sendint))) {
		log_error("setInt: error setting sendint");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_host_set(pksc, host))) {
		log_error("setInt: error setting host");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_server_set(pksc, server))) {
		log_error("setInt: error setting server");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_path_set(pksc, path))) {
		log_error("setInt: error setting path");
		return;
	}
	ksapi_setInt_submit(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_setInt_returnMethodxdr(
		OV_INSTPTR_ksapi_KSCommon			 	kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
	ksapi_KSCommon_analysesetxdr(kscommon, xdr, xdrlength);
	
	return;
}
