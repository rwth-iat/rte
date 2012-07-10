#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "ksapi_h.h"
#include "libov/ov_scheduler.h"

#include "xdrhandling.h"
#include "config.h"


/*	ksapi_setDouble_senddouble_get
 *	returns the current value of the senddouble
 */
OV_DLLFNCEXPORT OV_DOUBLE ksapi_setDouble_senddouble_get(
	OV_INSTPTR_ksapi_setDouble	pobj
) {
	return pobj->v_senddouble;
}

/*	ksapi_setDouble_senddouble_set
 *	sets the current senddouble to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_setDouble_senddouble_set(
	OV_INSTPTR_ksapi_setDouble	pobj,
	const OV_DOUBLE           	value
) {
	pobj->v_senddouble=value;
	return OV_ERR_OK;
}

/*	ksapi_setDouble_startup
 *	On startup set all values to default and close all connections
 */
OV_DLLFNCEXPORT void ksapi_setDouble_startup(
	OV_INSTPTR_ov_object			pobj
) {
	OV_INSTPTR_ksapi_setDouble sobj	=	Ov_StaticPtrCast(ksapi_setDouble, pobj);
	//To avoid a double Startup, which occures, because startup is executed while constructing 
	//the object and starting the FB, this check is implemented
	if (pobj->v_objectstate & OV_OS_STARTED) {
		return;
	}
	ksapi_KSCommon_startup(pobj);
	//initialize Values
	ksapi_setDouble_senddouble_set(sobj,0);
	
	return;
}

/*	ksapi_setDouble_shutdown
 *	After closing the service and the connection, shutdown the object.
 */
OV_DLLFNCEXPORT void ksapi_setDouble_shutdown(
	OV_INSTPTR_ov_object	pobj
) {
	ksapi_KSCommon_shutdown(pobj);
	return;
}

/*	ksapi_setDouble_submit
 *	If all parameters were manually set, you can use this function to send the string
 */
OV_DLLFNCEXPORT void ksapi_setDouble_submit(
	OV_INSTPTR_ksapi_setDouble	pobj
) {
	OV_VTBLPTR_ksapi_setDouble	pvtableop; 
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	OV_INSTPTR_ksapi_Channel channel;
	char *xdr=NULL;
	int xdrlength;

	Ov_GetVTablePtr(ksapi_setDouble, pvtableop, pobj);

	channel = (OV_INSTPTR_ksapi_Channel)Ov_SearchChild(ov_containment, pobj, "channel");
	if(channel == NULL) {
	    ov_logfile_error("SETDOUBLE object without Channel to use: %s", pobj->v_identifier);
	    pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "no channel Object", 2);
	    return;
	}

	if( (pobj->v_host) &&
        (pobj->v_server) &&
        (pobj->v_path) &&
        (ksapi_KSCommon_status_get(pksc) != STATUS_KSCOMMON_BUSY) ) {
		//all values are set
		
	generatesetdoublexdr(&xdr, &xdrlength, pobj->v_path, pobj->v_senddouble);
	
	//send
	ksapi_Channel_sendxdr(channel, pksc, xdr, xdrlength);
	
	free(xdr);
		
	}
	else if(ksapi_KSCommon_status_get(pksc) == STATUS_KSCOMMON_BUSY)
	{
		log_error("setDouble: connection in use");
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "connection in use", 5);
	}
	else {
		//not all values are set
		log_error("setDouble: Not all values are set. Sending cancelled");
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "Not all values are set", 2);
	}
	return;
}


/*	ksapi_setDouble_setandsubmit
 *	this function sets the necessary values automatically and starts the sending
 */
OV_DLLFNCEXPORT void ksapi_setDouble_setandsubmit(
	OV_INSTPTR_ksapi_setDouble	pobj,
	OV_STRING						host,
	OV_STRING						server,
	OV_STRING						path,
	OV_DOUBLE						senddouble
) {
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	//set values:
	if (Ov_Fail(ksapi_setDouble_senddouble_set(pobj, senddouble))) {
		log_error("setDouble: error setting senddouble");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_host_set(pksc, host))) {
		log_error("setDouble: error setting host");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_server_set(pksc, server))) {
		log_error("setDouble: error setting server");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_path_set(pksc, path))) {
		log_error("setDouble: error setting path");
		return;
	}
	ksapi_setDouble_submit(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_setDouble_returnMethodxdr(
		OV_INSTPTR_ksapi_KSCommon			 	kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
	ksapi_KSCommon_analysesetxdr(kscommon, xdr, xdrlength);
	
	return;
}
