#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "ksapi_h.h"
#include "libov/ov_scheduler.h"

#include "xdrhandling.h"
#include "config.h"


OV_DLLFNCEXPORT OV_TIME* ksapi_setBool_varTimeStamp_get(
    OV_INSTPTR_ksapi_setBool          pobj
) {
    return &pobj->v_varTimeStamp;
}

OV_DLLFNCEXPORT OV_RESULT ksapi_setBool_varTimeStamp_set(
    OV_INSTPTR_ksapi_setBool          pobj,
    const OV_TIME*  value
) {
    pobj->v_varTimeStamp = *value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT ksapi_setBool_varQState_get(
    OV_INSTPTR_ksapi_setBool          pobj
) {
    return pobj->v_varQState;
}

OV_DLLFNCEXPORT OV_RESULT ksapi_setBool_varQState_set(
    OV_INSTPTR_ksapi_setBool          pobj,
    const OV_UINT  value
) {
    pobj->v_varQState = value;
    return OV_ERR_OK;
}

/*	ksapi_setBool_sendbool_get
 *	returns the current value of the sendbool
 */
OV_DLLFNCEXPORT OV_BOOL ksapi_setBool_sendbool_get(
	OV_INSTPTR_ksapi_setBool	pobj
) {
	return pobj->v_sendbool;
}

/*	ksapi_setBool_sendbool_set
 *	sets the current sendbool to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_setBool_sendbool_set(
	OV_INSTPTR_ksapi_setBool	pobj,
	const OV_BOOL           	value
) {
	pobj->v_sendbool=value;
	return OV_ERR_OK;
}

/*	ksapi_setBool_startup
 *	On startup set all values to default and close all connections
 */
OV_DLLFNCEXPORT void ksapi_setBool_startup(
	OV_INSTPTR_ov_object			pobj
) {
	OV_INSTPTR_ksapi_setBool sobj	=	Ov_StaticPtrCast(ksapi_setBool, pobj);
	//To avoid a double Startup, which occures, because startup is executed while constructing 
	//the object and starting the FB, this check is implemented
	if (pobj->v_objectstate & OV_OS_STARTED) {
		return;
	}
	ksapi_KSCommon_startup(pobj);
	//initialize Values
	ksapi_setBool_sendbool_set(sobj,0);
	
	return;
}

/*	ksapi_setBool_shutdown
 *	After closing the service and the connection, shutdown the object.
 */
OV_DLLFNCEXPORT void ksapi_setBool_shutdown(
	OV_INSTPTR_ov_object	pobj
) {
	ksapi_KSCommon_shutdown(pobj);
	return;
}

/*	ksapi_setBool_submit
 *	If all parameters were manually set, you can use this function to send the string
 */
OV_DLLFNCEXPORT void ksapi_setBool_submit(
	OV_INSTPTR_ksapi_setBool	pobj
) {
	OV_VTBLPTR_ksapi_setBool   pvtableop; 
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	OV_INSTPTR_ksapi_Channel channel;

	Ov_GetVTablePtr(ksapi_setBool, pvtableop, pobj);

	channel = (OV_INSTPTR_ksapi_Channel)Ov_SearchChild(ov_containment, pobj, "channel");
	if(channel == NULL) {
	   ov_logfile_error("SETBOOL object without Channel to use: %s", pobj->v_identifier);
	   pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "no channel Object", 2);
	   return;
	}
	
	if((pobj->v_host) && (pobj->v_server) && (pobj->v_path) && (ksapi_KSCommon_status_get(pksc) != STATUS_KSCOMMON_BUSY)){
		//all values are set
	char *xdr=NULL;
	int xdrlength;

	generatesetboolxdr(&xdr, &xdrlength, pobj->v_path, pobj->v_sendbool, pobj->v_varTimeStamp.secs, pobj->v_varTimeStamp.usecs, pobj->v_varQState);
	
	//send
	ksapi_Channel_sendxdr(channel, pksc, xdr, xdrlength);
	
	free(xdr);
		
	}
	else if(ksapi_KSCommon_status_get(pksc) == STATUS_KSCOMMON_BUSY)
	{
		log_error("setBool: connection in use");
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "connection in use", 5);
	}
	else {
		//not all values are set
		log_error("setBool: Not all values are set. Sending cancelled");
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "Not all values are set", 2);
	}
	return;
}


/*	ksapi_setBool_setandsubmit
 *	this function sets the necessary values automatically and starts the sending
 */
OV_DLLFNCEXPORT void ksapi_setBool_setandsubmit(
	OV_INSTPTR_ksapi_setBool	pobj,
	OV_STRING						host,
	OV_STRING						server,
	OV_STRING						path,
	OV_BOOL							sendbool
) {
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	//set values:
	if (Ov_Fail(ksapi_setBool_sendbool_set(pobj, sendbool))) {
		log_error("setBool: error setting sendbool");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_host_set(pksc, host))) {
		log_error("setBool: error setting host");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_server_set(pksc, server))) {
		log_error("setBool: error setting server");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_path_set(pksc, path))) {
		log_error("setBool: error setting path");
		return;
	}
	ksapi_setBool_submit(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_setBool_returnMethodxdr(
		OV_INSTPTR_ksapi_KSCommon	 			kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
	ksapi_KSCommon_analysesetxdr(kscommon, xdr, xdrlength);
	
	return;
}
