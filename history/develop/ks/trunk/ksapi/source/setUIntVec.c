#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "ksapi_h.h"
#include "libov/ov_scheduler.h"

#include "xdrhandling.h"

#include "config.h"

OV_DLLFNCEXPORT OV_TIME* ksapi_setUIntVec_varTimeStamp_get(
    OV_INSTPTR_ksapi_setUIntVec          pobj
) {
    return &pobj->v_varTimeStamp;
}

OV_DLLFNCEXPORT OV_RESULT ksapi_setUIntVec_varTimeStamp_set(
    OV_INSTPTR_ksapi_setUIntVec          pobj,
    const OV_TIME*  value
) {
    pobj->v_varTimeStamp = *value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT ksapi_setUIntVec_varQState_get(
    OV_INSTPTR_ksapi_setUIntVec          pobj
) {
    return pobj->v_varQState;
}

OV_DLLFNCEXPORT OV_RESULT ksapi_setUIntVec_varQState_set(
    OV_INSTPTR_ksapi_setUIntVec          pobj,
    const OV_UINT  value
) {
    pobj->v_varQState = value;
    return OV_ERR_OK;
}

/*	ksapi_setUIntVec_senduintvec_get
 *	returns the current value and the length of the senduintvec
 */
OV_DLLFNCEXPORT OV_UINT * ksapi_setUIntVec_senduintvec_get(
	OV_INSTPTR_ksapi_setUIntVec	pobj,
	OV_UINT								*pveclen
) {
	*pveclen = pobj->v_senduintvec.veclen;
	return pobj->v_senduintvec.value;
}

/*	ksapi_setUIntVec_senduintvec_set
 *	sets the current senduintvec to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_setUIntVec_senduintvec_set(
	OV_INSTPTR_ksapi_setUIntVec	pobj,
	const OV_UINT	 					*pvalue,
	const OV_UINT						veclen
) {
	return Ov_SetDynamicVectorValue(&pobj->v_senduintvec,pvalue,veclen,UINT);        
}

/*	ksapi_setUIntVec_startup
 *	On startup set all values to default and close all connections
 */
OV_DLLFNCEXPORT void ksapi_setUIntVec_startup(
	OV_INSTPTR_ov_object		pobj
) {
	OV_INSTPTR_ksapi_setUIntVec sobj	=	Ov_StaticPtrCast(ksapi_setUIntVec, pobj);
	//To avoid a double Startup, which occures, because startup is executed while constructing 
	//the object and starting the FB, this check is implemented
	if (pobj->v_objectstate & OV_OS_STARTED) {
		return;
	}
	ksapi_KSCommon_startup(pobj);
	//initialize Values
	ksapi_setUIntVec_senduintvec_set(sobj,NULL,0);
	
	return;
}

/*	ksapi_setUIntVec_shutdown
 *	After closing the service and the connection, shutdown the object.
 */
OV_DLLFNCEXPORT void ksapi_setUIntVec_shutdown(
	OV_INSTPTR_ov_object		pobj
) {
	ksapi_KSCommon_shutdown(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_setUIntVec_submit(
	OV_INSTPTR_ksapi_setUIntVec	pobj
) {
	OV_VTBLPTR_ksapi_setUIntVec   pvtableop;
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	OV_INSTPTR_ksapi_Channel channel;
	char *xdr=NULL;
	int xdrlength;

	Ov_GetVTablePtr(ksapi_setUIntVec, pvtableop, pobj);

	channel = (OV_INSTPTR_ksapi_Channel)Ov_SearchChild(ov_containment, pobj, "channel");
	if(channel == NULL) {
	    ov_logfile_error("SETUINTVEC object without Channel to use: %s", pobj->v_identifier);
	    pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "no channel Object", 2);
	    return;
	}
	
	if((pobj->v_host) && (pobj->v_server) && (pobj->v_path) && (ksapi_KSCommon_status_get(pksc) != STATUS_KSCOMMON_BUSY)){
		//all values are set
	
	
	generatesetuintvecxdr(&xdr, &xdrlength, pobj->v_path, (unsigned int*)pobj->v_senduintvec.value, pobj->v_senduintvec.veclen,
			pobj->v_varTimeStamp.secs, pobj->v_varTimeStamp.usecs, pobj->v_varQState);
	
	//send
	ksapi_Channel_sendxdr(channel, pksc, xdr, xdrlength);
		
	free(xdr);
	
	}
	else if(ksapi_KSCommon_status_get(pksc) == STATUS_KSCOMMON_BUSY)
	{
		log_error("setUIntVex: connection in use");
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "connection in use", 5);
	}
	else {
		//not all values are set
		log_error("setUIntVec: Not all values are set. Sending cancelled");
		pvtableop->m_returnMethod(Ov_StaticPtrCast(ov_object, pobj), "Not all values are set", 2);
	}
	
	
	return;
}


/*	ksapi_setUIntVec_setandsubmit
 *	this function sets the necessary values automatically and starts the sending
 */
OV_DLLFNCEXPORT void ksapi_setUIntVec_setandsubmit(
		OV_INSTPTR_ksapi_setUIntVec			pobj,
		OV_STRING 									host,
		OV_STRING 									server,
		OV_STRING 									path,
		OV_UINT	 									*senduintvec,
		OV_UINT	 									senduintveclength
) {
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	//set values:
	if (Ov_Fail(ksapi_setUIntVec_senduintvec_set(pobj, senduintvec, senduintveclength))) {
		log_error("setUIntVec: error setting senduintvec");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_host_set(pksc, host))) {
		log_error("setUIntVec: error setting host");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_server_set(pksc, server))) {
		log_error("setUIntVec: error setting server");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_path_set(pksc, path))) {
		log_error("setUIntVec: error setting path");
		return;
	}
	ksapi_setUIntVec_submit(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_setUIntVec_returnMethodxdr(
		OV_INSTPTR_ksapi_KSCommon			 	kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
	ksapi_KSCommon_analysesetxdr(kscommon, xdr, xdrlength);
	
	return;
}
