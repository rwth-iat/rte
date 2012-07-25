#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "ksapi_h.h"
#include "libov/ov_scheduler.h"

#include "xdrhandling.h"

#include "config.h"

OV_DLLFNCEXPORT OV_TIME* ksapi_setSingle_varTimeStamp_get(
    OV_INSTPTR_ksapi_setSingle          pobj
) {
    return &pobj->v_varTimeStamp;
}

OV_DLLFNCEXPORT OV_RESULT ksapi_setSingle_varTimeStamp_set(
    OV_INSTPTR_ksapi_setSingle          pobj,
    const OV_TIME*  value
) {
    pobj->v_varTimeStamp = *value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT ksapi_setSingle_varQState_get(
    OV_INSTPTR_ksapi_setSingle          pobj
) {
    return pobj->v_varQState;
}

OV_DLLFNCEXPORT OV_RESULT ksapi_setSingle_varQState_set(
    OV_INSTPTR_ksapi_setSingle          pobj,
    const OV_UINT  value
) {
    pobj->v_varQState = value;
    return OV_ERR_OK;
}

/*	ksapi_setSingle_sendsingle_get
 *	returns the current value of the sendsingle
 */
OV_DLLFNCEXPORT OV_SINGLE ksapi_setSingle_sendsingle_get(
	OV_INSTPTR_ksapi_setSingle		pobj
) {
	return pobj->v_sendsingle;
}

/*	ksapi_setSingle_sendsingle_set
 *	sets the current sendsingle to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_setSingle_sendsingle_set(
	OV_INSTPTR_ksapi_setSingle		pobj,
	const OV_SINGLE           		value
) {
	pobj->v_sendsingle=value;
	return OV_ERR_OK;
}

/*	ksapi_setSingle_startup
 *	On startup set all values to default and close all connections
 */
OV_DLLFNCEXPORT void ksapi_setSingle_startup(
	OV_INSTPTR_ov_object	pobj
) {
	OV_INSTPTR_ksapi_setSingle sobj	=	Ov_StaticPtrCast(ksapi_setSingle, pobj);
	//To avoid a double Startup, which occures, because startup is executed while constructing 
	//the object and starting the FB, this check is implemented
	if (pobj->v_objectstate & OV_OS_STARTED) {
		return;
	}
	ksapi_KSCommon_startup(pobj);
	//initialize Values
	ksapi_setSingle_sendsingle_set(sobj,0);
	
	return;
}

/*	ksapi_setSingle_shutdown
 *	After closing the service and the connection, shutdown the object.
 */
OV_DLLFNCEXPORT void ksapi_setSingle_shutdown(
	OV_INSTPTR_ov_object	pobj
) {
	ksapi_KSCommon_shutdown(pobj);
	return;
}

/*	ksapi_setSingle_submit
 *	If all parameters were manually set, you can use this function to send the string
 */
OV_DLLFNCEXPORT void ksapi_setSingle_submit(
		OV_INSTPTR_ksapi_setSingle	 			pobj
) {
	OV_VTBLPTR_ksapi_setSingle   pvtableop; 
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	OV_INSTPTR_ksapi_Channel channel;
	char *xdr=NULL;
	int xdrlength;

	Ov_GetVTablePtr(ksapi_setSingle, pvtableop, pobj);

	channel = (OV_INSTPTR_ksapi_Channel)Ov_SearchChild(ov_containment, pobj, "channel");
	if(channel == NULL) {
	    ov_logfile_error("SETSINGLE object without Channel to use: %s", pobj->v_identifier);
	    pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "no channel Object", 2);
	    return;
	}

	if((pobj->v_host) && (pobj->v_server) && (pobj->v_path)
			&& (ksapi_KSCommon_status_get(pksc) == STATUS_KSCOMMON_OK)){
		//all values are set
	
	generatesetsinglexdr(&xdr, &xdrlength, pobj->v_path, pobj->v_sendsingle, pobj->v_varTimeStamp.secs, pobj->v_varTimeStamp.usecs, pobj->v_varQState);
	
	//send
	ksapi_Channel_sendxdr(channel, pksc, xdr, xdrlength);
	
	free(xdr);
		
	}
	else if(ksapi_KSCommon_status_get(pksc) == STATUS_KSCOMMON_BUSY)
	{
		log_error("setSingle: connection in use");
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "connection in use", 5);
	}
	else {
		//not all values are set
		log_error("setSingle: Not all values are set. Sending cancelled");
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "Not all values are set", 2);
	}
	return;
}


/*	ksapi_setSingle_setandsubmit
 *	this function sets the necessary values automatically and starts the sending
 */
OV_DLLFNCEXPORT void ksapi_setSingle_setandsubmit(
	OV_INSTPTR_ksapi_setSingle		pobj,
	OV_STRING						host,
	OV_STRING						server,
	OV_STRING						path,
	OV_SINGLE						sendsingle
) {
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	//set values:
	if (Ov_Fail(ksapi_setSingle_sendsingle_set(pobj, sendsingle))) {
		log_error("setSingle: error setting sendsingle");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_host_set(pksc, host))) {
		log_error("setSingle: error setting host");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_server_set(pksc, server))) {
		log_error("setSingle: error setting server");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_path_set(pksc, path))) {
		log_error("setSingle: error setting path");
		return;
	}
	ksapi_setSingle_submit(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_setSingle_returnMethodxdr(
		OV_INSTPTR_ksapi_KSCommon			 	kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
	ksapi_KSCommon_analysesetxdr(kscommon, xdr, xdrlength);
	
	return;
}
