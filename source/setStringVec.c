#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "ksapi_h.h"
#include "libov/ov_scheduler.h"

#include "xdrhandling.h"

#include "config.h"

/*	ksapi_setStringVec_sendstringvec_get
 *	returns the current value and the length of the sendstringvec
 */
OV_DLLFNCEXPORT OV_STRING * ksapi_setStringVec_sendstringvec_get(
	OV_INSTPTR_ksapi_setStringVec	pobj,
	OV_UINT								*pveclen
) {
	*pveclen = pobj->v_sendstringvec.veclen;
	return pobj->v_sendstringvec.value;
}

/*	ksapi_setStringVec_sendstringvec_set
 *	sets the current sendstringvec to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_setStringVec_sendstringvec_set(
	OV_INSTPTR_ksapi_setStringVec	pobj,
	const OV_STRING 					*pvalue,
	const OV_UINT						veclen
) {
	return Ov_SetDynamicVectorValue(&pobj->v_sendstringvec,pvalue,veclen,STRING);        
}

/*	ksapi_setStringVec_startup
 *	On startup set all values to default and close all connections
 */
OV_DLLFNCEXPORT void ksapi_setStringVec_startup(
	OV_INSTPTR_ov_object		pobj
) {
	OV_INSTPTR_ksapi_setStringVec sobj	=	Ov_StaticPtrCast(ksapi_setStringVec, pobj);
	//To avoid a double Startup, which occures, because startup is executed while constructing 
	//the object and starting the FB, this check is implemented
	if (pobj->v_objectstate & OV_OS_STARTED) {
		return;
	}
	ksapi_KSCommon_startup(pobj);
	//initialize Values
	ksapi_setStringVec_sendstringvec_set(sobj,NULL,0);
	
	return;
}

/*	ksapi_setStringVec_shutdown
 *	After closing the service and the connection, shutdown the object.
 */
OV_DLLFNCEXPORT void ksapi_setStringVec_shutdown(
	OV_INSTPTR_ov_object		pobj
) {
	ksapi_KSCommon_shutdown(pobj);
	return;
}

/*	ksapi_setString_submit
 *	If all parameters were manually set, you can use this function to send the string
 */
OV_DLLFNCEXPORT void ksapi_setStringVec_submit(
	OV_INSTPTR_ksapi_setStringVec	pobj
) {
	OV_VTBLPTR_ksapi_setStringVec   pvtableop;
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	OV_INSTPTR_ksapi_Channel channel;
	char *xdr;
	int xdrlength;

	Ov_GetVTablePtr(ksapi_setStringVec, pvtableop, pobj);

	channel = (OV_INSTPTR_ksapi_Channel)Ov_SearchChild(ov_containment, pobj, "channel");
	if(channel == NULL) {
	    ov_logfile_error("SETSTRINGVEC object without Channel to use: %s", pobj->v_identifier);
		 pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "no channel Object", 2);
	    return;
	}

	if((pobj->v_sendstringvec.value) && (pobj->v_host) && (pobj->v_server) && (pobj->v_path) && (ksapi_KSCommon_status_get(pksc) != STATUS_KSCOMMON_BUSY)){
		//all values are set
	
	generatesetstringvecxdr(&xdr, &xdrlength, pobj->v_path, pobj->v_sendstringvec.value, pobj->v_sendstringvec.veclen);
	
	//send
	ksapi_Channel_sendxdr(channel, pksc, xdr, xdrlength);
		
	free(xdr);
	
	}
	else if(ksapi_KSCommon_status_get(pksc) == STATUS_KSCOMMON_BUSY)
	{
		log_error("setStringVec: connection in use");
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "connection in use", 5);
	}
	else {
		//not all values are set
		log_error("setStringVec: Not all values are set. Sending cancelled");
		pvtableop->m_returnMethod(Ov_StaticPtrCast(ov_object, pobj), "Not all values are set", 2);
	}
	
	
	return;
}


/*	ksapi_setStringVec_setandsubmit
 *	this function sets the necessary values automatically and starts the sending
 */
OV_DLLFNCEXPORT void ksapi_setStringVec_setandsubmit(
		OV_INSTPTR_ksapi_setStringVec			pobj,
		OV_STRING 									host,
		OV_STRING 									server,
		OV_STRING 									path,
		OV_STRING 									*sendstringvec,
		OV_UINT	 									sendstringlength
) {
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	//set values:
	if (Ov_Fail(ksapi_setStringVec_sendstringvec_set(pobj, sendstringvec, sendstringlength))) {
		log_error("setStringVec: error setting sendstringvec");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_host_set(pksc, host))) {
		log_error("setStringVec: error setting host");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_server_set(pksc, server))) {
		log_error("setStringVec: error setting server");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_path_set(pksc, path))) {
		log_error("setStringVec: error setting path");
		return;
	}
	ksapi_setStringVec_submit(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_setStringVec_returnMethodxdr(
		OV_INSTPTR_ksapi_KSCommon			 	kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
	ksapi_KSCommon_analysesetxdr(kscommon, xdr, xdrlength);
	
	return;
}
