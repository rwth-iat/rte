#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "ksapi_h.h"
#include "libov/ov_scheduler.h"

#include "xdrhandling.h"
#include "config.h"


/*	ksapi_setUInt_senduint_get
 *	returns the current value of the senduint
 */
OV_DLLFNCEXPORT OV_UINT ksapi_setUInt_senduint_get(
	OV_INSTPTR_ksapi_setUInt		pobj
) {
	return pobj->v_senduint;
}

/*	ksapi_setUInt_senduint_set
 *	sets the current senduint to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_setUInt_senduint_set(
	OV_INSTPTR_ksapi_setUInt		pobj,
	const OV_UINT           		value
) {
	pobj->v_senduint=value;
	return OV_ERR_OK;
}

/*	ksapi_setUInt_startup
 *	On startup set all values to default and close all connections
 */
OV_DLLFNCEXPORT void ksapi_setUInt_startup(
	OV_INSTPTR_ov_object	pobj
) {
	OV_INSTPTR_ksapi_setUInt sobj	=	Ov_StaticPtrCast(ksapi_setUInt, pobj);
	//To avoid a double Startup, which occures, because startup is executed while constructing 
	//the object and starting the FB, this check is implemented
	if (pobj->v_objectstate & OV_OS_STARTED) {
		return;
	}
	ksapi_KSCommon_startup(pobj);
	//initialize Values
	ksapi_setUInt_senduint_set(sobj,0);
	
	return;
}

/*	ksapi_setUInt_shutdown
 *	After closing the service and the connection, shutdown the object.
 */
OV_DLLFNCEXPORT void ksapi_setUInt_shutdown(
	OV_INSTPTR_ov_object	pobj
) {
	ksapi_KSCommon_shutdown(pobj);
	return;
}

/*	ksapi_setUInt_submit
 *	If all parameters were manually set, you can use this function to send the string
 */
OV_DLLFNCEXPORT void ksapi_setUInt_submit(
		OV_INSTPTR_ksapi_setUInt	 			pobj
) {
	OV_VTBLPTR_ksapi_setUInt   pvtableop; 
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	OV_INSTPTR_ksapi_Channel channel;
	char *xdr;
	int xdrlength;

	Ov_GetVTablePtr(ksapi_setUInt, pvtableop, pobj);

	channel = (OV_INSTPTR_ksapi_Channel)Ov_SearchChild(ov_containment, pobj, "channel");
	if(channel == NULL) {
	    ov_logfile_error("SETUINT object without Channel to use: %s", pobj->v_identifier);
	    pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "no channel Object", 2);
	    return;
	}

	if((pobj->v_host) && (pobj->v_server) && (pobj->v_path) && (ksapi_KSCommon_status_get(pksc) != STATUS_KSCOMMON_BUSY)){
		//all values are set
	
		
	generatesetuintxdr(&xdr, &xdrlength, pobj->v_path, pobj->v_senduint);
	
	//send
	ksapi_Channel_sendxdr(channel, pksc, xdr, xdrlength);
		
	}
	else if(ksapi_KSCommon_status_get(pksc) == STATUS_KSCOMMON_BUSY)
	{
		log_error("setUInt: connection in use");
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "connection in use", 5);
	}
	else {
		//not all values are set
		log_error("setUInt: Not all values are set. Sending cancelled");
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "Not all values are set", 2);
	}
	return;
}


/*	ksapi_setUInt_setandsubmit
 *	this function sets the necessary values automatically and starts the sending
 */
OV_DLLFNCEXPORT void ksapi_setUInt_setandsubmit(
	OV_INSTPTR_ksapi_setUInt		pobj,
	OV_STRING						host,
	OV_STRING						server,
	OV_STRING						path,
	OV_UINT							senduint
) {
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	//set values:
	if (Ov_Fail(ksapi_setUInt_senduint_set(pobj, senduint))) {
		log_error("setUInt: error setting senduint");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_host_set(pksc, host))) {
		log_error("setUInt: error setting host");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_server_set(pksc, server))) {
		log_error("setUInt: error setting server");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_path_set(pksc, path))) {
		log_error("setUInt: error setting path");
		return;
	}
	ksapi_setUInt_submit(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_setUInt_returnMethodxdr(
		OV_INSTPTR_ksapi_KSCommon			 	kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
	ksapi_KSCommon_analysesetxdr(kscommon, xdr, xdrlength);
	
	return;
}
