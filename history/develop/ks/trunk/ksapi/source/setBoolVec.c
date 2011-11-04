#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "ksapi_h.h"
#include "libov/ov_scheduler.h"

#include "xdrhandling.h"
#include "config.h"


/*	ksapi_setBoolVec_sendboolvec_get
 *	returns the current value and the length of the sendboolvec
 */
OV_DLLFNCEXPORT OV_BOOL * ksapi_setBoolVec_sendboolvec_get(
	OV_INSTPTR_ksapi_setBoolVec	pobj,
	OV_UINT								*pveclen
) {
	*pveclen = pobj->v_sendboolvec.veclen;
	return pobj->v_sendboolvec.value;
}

/*	ksapi_setBoolVec_sendboolvec_set
 *	sets the current sendboolvec to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_setBoolVec_sendboolvec_set(
	OV_INSTPTR_ksapi_setBoolVec	pobj,
	const OV_BOOL 					*pvalue,
	const OV_UINT						veclen
) {
	return Ov_SetDynamicVectorValue(&pobj->v_sendboolvec,pvalue,veclen,BOOL);        
}

/*	ksapi_setBoolVec_startup
 *	On startup set all values to default and close all connections
 */
OV_DLLFNCEXPORT void ksapi_setBoolVec_startup(
	OV_INSTPTR_ov_object		pobj
) {
	OV_INSTPTR_ksapi_setBoolVec sobj	=	Ov_StaticPtrCast(ksapi_setBoolVec, pobj);
	//To avoid a bool Startup, which occures, because startup is executed while constructing 
	//the object and starting the FB, this check is implemented
	if (pobj->v_objectstate & OV_OS_STARTED) {
		return;
	}
	ksapi_KSCommon_startup(pobj);
	//initialize Values
	ksapi_setBoolVec_sendboolvec_set(sobj,NULL,0);
	
	return;
}

/*	ksapi_setBoolVec_shutdown
 *	After closing the service and the connection, shutdown the object.
 */
OV_DLLFNCEXPORT void ksapi_setBoolVec_shutdown(
	OV_INSTPTR_ov_object		pobj
) {
	ksapi_KSCommon_shutdown(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_setBoolVec_submit(
	OV_INSTPTR_ksapi_setBoolVec	pobj
) {
	OV_VTBLPTR_ksapi_setBoolVec   pvtableop;
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	OV_INSTPTR_ksapi_Channel channel;
	
	Ov_GetVTablePtr(ksapi_setBoolVec, pvtableop, pobj);

	channel = (OV_INSTPTR_ksapi_Channel)Ov_SearchChild(ov_containment, pobj, "channel");
	if(channel == NULL) {
	    ov_logfile_error("SETBOOLVEC object without Channel to use: %s", pobj->v_identifier);
	    pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "no channel Object", 2);
	    return;
	}

	if((pobj->v_host) && (pobj->v_server) && (pobj->v_path) && (ksapi_KSCommon_status_get(pksc) != STATUS_KSCOMMON_BUSY)){
		//all values are set
	char *xdr;
	char path[4096];
	int xdrlength;
		
	strcpy(path, pobj->v_path);
	generatesetboolvecxdr(&xdr, &xdrlength, path, pobj->v_sendboolvec.value, pobj->v_sendboolvec.veclen);
	
	//print xdr
	//KSDEVEL int j;
	//KSDEVEL printf("\n\nxdr:\nlength: %d\n", xdrlength);
	//KSDEVEL printf("%X %X %X %X %X %X    ", xdr[0], xdr[1], xdr[2], xdr[3], xdr[4], xdr[5]);
	//KSDEVEL for (j = 6; j < xdrlength; j=j+4)
		//KSDEVEL printf("%X %X %X %X     ", xdr[j], xdr[j+1], xdr[j+2], xdr[j+3]);
	//KSDEVEL printf("\n\n");
	//KSDEVEL for (j = 6; j < xdrlength; j=j+4)
		//KSDEVEL printf("%c %c %c %c     ", xdr[j], xdr[j+1], xdr[j+2], xdr[j+3]);
	//KSDEVEL printf("\n\n");
	//KSDEVEL printf("%s\n\n", xdr);
	
	//send
	ksapi_Channel_sendxdr(channel, pksc, xdr, xdrlength);
		
	free(xdr);
	
	}
	else if(ksapi_KSCommon_status_get(pksc) == STATUS_KSCOMMON_BUSY)
	{
		log_error("setBoolVec: connection in use");
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "connection in use", 5);
	}
	else {
		//not all values are set
		log_error("setBoolVec: Not all values are set. Sending cancelled");
		pvtableop->m_returnMethod(Ov_StaticPtrCast(ov_object, pobj), "Not all values are set", 2);
	}
	
	
	return;
}


/*	ksapi_setBoolVec_setandsubmit
 *	this function sets the necessary values automatically and starts the sending
 */
OV_DLLFNCEXPORT void ksapi_setBoolVec_setandsubmit(
		OV_INSTPTR_ksapi_setBoolVec			pobj,
		OV_STRING 									host,
		OV_STRING 									server,
		OV_STRING 									path,
		OV_BOOL 									*sendboolvec,
		OV_UINT	 									sendboolveclength
) {
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	//set values:
	if (Ov_Fail(ksapi_setBoolVec_sendboolvec_set(pobj, sendboolvec, sendboolveclength))) {
		log_error("setBoolVec: error setting sendstring");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_host_set(pksc, host))) {
		log_error("setBoolVec: error setting host");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_server_set(pksc, server))) {
		log_error("setBoolVec: error setting server");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_path_set(pksc, path))) {
		log_error("setBoolVec: error setting path");
		return;
	}
	ksapi_setBoolVec_submit(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_setBoolVec_returnMethodxdr(
		OV_INSTPTR_ksapi_KSCommon		 		kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
	ksapi_KSCommon_analysesetxdr(kscommon, xdr, xdrlength);
	
	return;
}
