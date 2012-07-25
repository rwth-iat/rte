#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "ksapi_h.h"
#include "libov/ov_scheduler.h"

#include "xdrhandling.h"
#include "config.h"


OV_DLLFNCEXPORT OV_TIME* ksapi_setIntVec_varTimeStamp_get(
    OV_INSTPTR_ksapi_setIntVec          pobj
) {
    return &pobj->v_varTimeStamp;
}

OV_DLLFNCEXPORT OV_RESULT ksapi_setIntVec_varTimeStamp_set(
    OV_INSTPTR_ksapi_setIntVec          pobj,
    const OV_TIME*  value
) {
    pobj->v_varTimeStamp = *value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT ksapi_setIntVec_varQState_get(
    OV_INSTPTR_ksapi_setIntVec          pobj
) {
    return pobj->v_varQState;
}

OV_DLLFNCEXPORT OV_RESULT ksapi_setIntVec_varQState_set(
    OV_INSTPTR_ksapi_setIntVec          pobj,
    const OV_UINT  value
) {
    pobj->v_varQState = value;
    return OV_ERR_OK;
}

/*	ksapi_setIntVec_sendintvec_get
 *	returns the current value and the length of the sendintvec
 */
OV_DLLFNCEXPORT OV_INT * ksapi_setIntVec_sendintvec_get(
	OV_INSTPTR_ksapi_setIntVec	pobj,
	OV_UINT								*pveclen
) {
	*pveclen = pobj->v_sendintvec.veclen;
	return pobj->v_sendintvec.value;
}

/*	ksapi_setIntVec_sendintvec_set
 *	sets the current sendintvec to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_setIntVec_sendintvec_set(
	OV_INSTPTR_ksapi_setIntVec	pobj,
	const OV_INT	 					*pvalue,
	const OV_UINT						veclen
) {
	return Ov_SetDynamicVectorValue(&pobj->v_sendintvec,pvalue,veclen,INT);        
}

/*	ksapi_setIntVec_startup
 *	On startup set all values to default and close all connections
 */
OV_DLLFNCEXPORT void ksapi_setIntVec_startup(
	OV_INSTPTR_ov_object		pobj
) {
	OV_INSTPTR_ksapi_setIntVec sobj	=	Ov_StaticPtrCast(ksapi_setIntVec, pobj);
	//To avoid a double Startup, which occures, because startup is executed while constructing 
	//the object and starting the FB, this check is implemented
	if (pobj->v_objectstate & OV_OS_STARTED) {
		return;
	}
	ksapi_KSCommon_startup(pobj);
	//initialize Values
	ksapi_setIntVec_sendintvec_set(sobj,NULL,0);
	
	return;
}

/*	ksapi_setIntVec_shutdown
 *	After closing the service and the connection, shutdown the object.
 */
OV_DLLFNCEXPORT void ksapi_setIntVec_shutdown(
	OV_INSTPTR_ov_object		pobj
) {
	ksapi_KSCommon_shutdown(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_setIntVec_submit(
	OV_INSTPTR_ksapi_setIntVec	pobj
) {
	OV_VTBLPTR_ksapi_setIntVec   pvtableop;
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	OV_INSTPTR_ksapi_Channel channel;
	char *xdr=NULL;
	int xdrlength;

	Ov_GetVTablePtr(ksapi_setIntVec, pvtableop, pobj);

	channel = (OV_INSTPTR_ksapi_Channel)Ov_SearchChild(ov_containment, pobj, "channel");
	if(channel == NULL) {
	    ov_logfile_error("SETINTVEC object without Channel to use: %s", pobj->v_identifier);
	    pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "no channel Object", 2);
	    return;
	}

	if((pobj->v_sendintvec.value) && (pobj->v_host) && (pobj->v_server) && (pobj->v_path) && (ksapi_KSCommon_status_get(pksc) != STATUS_KSCOMMON_BUSY)){
		//all values are set
	

		
	generatesetintvecxdr(&xdr, &xdrlength, pobj->v_path, (int*)pobj->v_sendintvec.value, pobj->v_sendintvec.veclen,
			pobj->v_varTimeStamp.secs, pobj->v_varTimeStamp.usecs, pobj->v_varQState);
	
	//send
	ksapi_Channel_sendxdr(channel, pksc, xdr, xdrlength);
		
	free(xdr);
	
	}
	else if(ksapi_KSCommon_status_get(pksc) == STATUS_KSCOMMON_BUSY)
	{
		log_error("setIntVec: connection in use");
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "connection in use", 5);
	}
	else {
		//not all values are set
		log_error("setIntVec: Not all values are set. Sending cancelled");
		pvtableop->m_returnMethod(Ov_StaticPtrCast(ov_object, pobj), "Not all values are set", 2);
	}
	
	
	return;
}


/*	ksapi_setIntVec_setandsubmit
 *	this function sets the necessary values automatically and starts the sending
 */
OV_DLLFNCEXPORT void ksapi_setIntVec_setandsubmit(
		OV_INSTPTR_ksapi_setIntVec			pobj,
		OV_STRING 									host,
		OV_STRING 									server,
		OV_STRING 									path,
		OV_INT	 									*sendintvec,
		OV_UINT	 									sendintveclength
) {
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	//set values:
	if (Ov_Fail(ksapi_setIntVec_sendintvec_set(pobj, sendintvec, sendintveclength))) {
		log_error("setIntVec: error setting sendintvec");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_host_set(pksc, host))) {
		log_error("setIntVec: error setting host");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_server_set(pksc, server))) {
		log_error("setIntVec: error setting server");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_path_set(pksc, path))) {
		log_error("setIntVec: error setting path");
		return;
	}
	ksapi_setIntVec_submit(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_setIntVec_returnMethodxdr(
		OV_INSTPTR_ksapi_KSCommon			 	kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
	ksapi_KSCommon_analysesetxdr(kscommon, xdr, xdrlength);
	
	return;
}
