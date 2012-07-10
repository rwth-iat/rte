#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "ksapi_h.h"
#include "libov/ov_scheduler.h"

#include "xdrhandling.h"
#include "config.h"


/*	ksapi_setDoubleVec_senddoublevec_get
 *	returns the current value and the length of the senddoublevec
 */
OV_DLLFNCEXPORT OV_DOUBLE * ksapi_setDoubleVec_senddoublevec_get(
	OV_INSTPTR_ksapi_setDoubleVec	pobj,
	OV_UINT								*pveclen
) {
	*pveclen = pobj->v_senddoublevec.veclen;
	return pobj->v_senddoublevec.value;
}

/*	ksapi_setDoubleVec_senddoublevec_set
 *	sets the current senddoublevec to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_setDoubleVec_senddoublevec_set(
	OV_INSTPTR_ksapi_setDoubleVec	pobj,
	const OV_DOUBLE 					*pvalue,
	const OV_UINT						veclen
) {
	return Ov_SetDynamicVectorValue(&pobj->v_senddoublevec,pvalue,veclen,DOUBLE);        
}

/*	ksapi_setDoubleVec_startup
 *	On startup set all values to default and close all connections
 */
OV_DLLFNCEXPORT void ksapi_setDoubleVec_startup(
	OV_INSTPTR_ov_object		pobj
) {
	OV_INSTPTR_ksapi_setDoubleVec sobj	=	Ov_StaticPtrCast(ksapi_setDoubleVec, pobj);
	//To avoid a double Startup, which occures, because startup is executed while constructing 
	//the object and starting the FB, this check is implemented
	if (pobj->v_objectstate & OV_OS_STARTED) {
		return;
	}
	ksapi_KSCommon_startup(pobj);
	//initialize Values
	ksapi_setDoubleVec_senddoublevec_set(sobj,NULL,0);
	
	return;
}

/*	ksapi_setDoubleVec_shutdown
 *	After closing the service and the connection, shutdown the object.
 */
OV_DLLFNCEXPORT void ksapi_setDoubleVec_shutdown(
	OV_INSTPTR_ov_object		pobj
) {
	ksapi_KSCommon_shutdown(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_setDoubleVec_submit(
	OV_INSTPTR_ksapi_setDoubleVec	pobj
) {
	OV_VTBLPTR_ksapi_setDoubleVec   pvtableop;
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	OV_INSTPTR_ksapi_Channel channel;
	char *xdr=NULL;
	int xdrlength;

	Ov_GetVTablePtr(ksapi_setDoubleVec, pvtableop, pobj);

	channel = (OV_INSTPTR_ksapi_Channel)Ov_SearchChild(ov_containment, pobj, "channel");
	if(channel == NULL) {
	    ov_logfile_error("SETDOUBLEVEC object without Channel to use: %s", pobj->v_identifier);
	    pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "no channel Object", 2);
	    return;
	}

	if((pobj->v_senddoublevec.value) && (pobj->v_host) && (pobj->v_server) && (pobj->v_path) && (ksapi_KSCommon_status_get(pksc) != STATUS_KSCOMMON_BUSY)){
		//all values are set
		
	generatesetdoublevecxdr(&xdr, &xdrlength, pobj->v_path, pobj->v_senddoublevec.value, pobj->v_senddoublevec.veclen);
	
	//send
	ksapi_Channel_sendxdr(channel, pksc, xdr, xdrlength);
		
	free(xdr);
	
	}
	else if(ksapi_KSCommon_status_get(pksc) == STATUS_KSCOMMON_BUSY)
	{
		log_error("setDoubleVec: connection in use");
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "connection in use", 5);
	}
	else {
		//not all values are set
		log_error("setDoubleVec: Not all values are set. Sending cancelled");
		pvtableop->m_returnMethod(Ov_StaticPtrCast(ov_object, pobj), "Not all values are set", 2);
	}
	
	
	return;
}


/*	ksapi_setDoubleVec_setandsubmit
 *	this function sets the necessary values automatically and starts the sending
 */
OV_DLLFNCEXPORT void ksapi_setDoubleVec_setandsubmit(
		OV_INSTPTR_ksapi_setDoubleVec			pobj,
		OV_STRING 									host,
		OV_STRING 									server,
		OV_STRING 									path,
		OV_DOUBLE 									*senddoublevec,
		OV_UINT	 									senddoubleveclength
) {
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	//set values:
	if (Ov_Fail(ksapi_setDoubleVec_senddoublevec_set(pobj, senddoublevec, senddoubleveclength))) {
		log_error("setDoubleVec: error setting senddoublevec");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_host_set(pksc, host))) {
		log_error("setDoubleVec: error setting host");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_server_set(pksc, server))) {
		log_error("setDoubleVec: error setting server");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_path_set(pksc, path))) {
		log_error("setDoubleVec: error setting path");
		return;
	}
	ksapi_setDoubleVec_submit(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_setDoubleVec_returnMethodxdr(
		OV_INSTPTR_ksapi_KSCommon			 	kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
	ksapi_KSCommon_analysesetxdr(kscommon, xdr, xdrlength);
	
	return;
}
