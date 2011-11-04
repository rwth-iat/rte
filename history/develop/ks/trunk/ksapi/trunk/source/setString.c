#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "ksapi_h.h"
#include "libov/ov_scheduler.h"

#include "xdrhandling.h"
#include "config.h"


/*	ksapi_setString_sendstring_get
 *	returns the current value of the sendstring
 */
OV_DLLFNCEXPORT OV_STRING ksapi_setString_sendstring_get(
	OV_INSTPTR_ksapi_setString		pobj
) {
	return pobj->v_sendstring;
}

/*	ksapi_setString_sendstring_set
 *	sets the current sendstring to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_setString_sendstring_set(
	OV_INSTPTR_ksapi_setString		pobj,
	const OV_STRING           		value
) {
	return ov_string_setvalue(&pobj->v_sendstring, value);
}

/*	ksapi_setString_startup
 *	On startup set all values to default and close all connections
 */
OV_DLLFNCEXPORT void ksapi_setString_startup(
	OV_INSTPTR_ov_object	pobj
) {
	OV_INSTPTR_ksapi_setString sobj	=	Ov_StaticPtrCast(ksapi_setString, pobj);
	//To avoid a double Startup, which occures, because startup is executed while constructing 
	//the object and starting the FB, this check is implemented
	if (pobj->v_objectstate & OV_OS_STARTED) {
		return;
	}
	ksapi_KSCommon_startup(pobj);
	//initialize Values
	ksapi_setString_sendstring_set(sobj,"");
	
	return;
}

/*	ksapi_setString_shutdown
 *	After closing the service and the connection, shutdown the object.
 */
OV_DLLFNCEXPORT void ksapi_setString_shutdown(
	OV_INSTPTR_ov_object	pobj
) {
	ksapi_KSCommon_shutdown(pobj);
	return;
}

/*	ksapi_setString_submit
 *	If all parameters were manually set, you can use this function to send the string
 */
OV_DLLFNCEXPORT void ksapi_setString_submit(
		OV_INSTPTR_ksapi_setString	 			pobj
) {
	OV_VTBLPTR_ksapi_setString   pvtableop;
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	OV_INSTPTR_ksapi_Channel channel;

	Ov_GetVTablePtr(ksapi_setString, pvtableop, pobj);

	channel = (OV_INSTPTR_ksapi_Channel)Ov_SearchChild(ov_containment, pobj, "channel");
	if(channel == NULL) {
	    ov_logfile_error("SETSTRING object without Channel to use: %s", pobj->v_identifier);
	    pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "no channel Object", 2);
	    return;
	}

	if((pobj->v_sendstring) && (pobj->v_host) && (pobj->v_server) && (pobj->v_path) && (ksapi_KSCommon_status_get(pksc) != STATUS_KSCOMMON_BUSY)){
		//all values are set
	char *xdr;
	char path[4096];
	int xdrlength;
	
		
	strcpy(path, pobj->v_path);
	
	generatesetstringxdr(&xdr, &xdrlength, path, pobj->v_sendstring);
	
	//print xdr
	//KSDEVEL int j;
	//KSDEVEL printf("\n\nxdr:\nlength: %d\n", xdrlength);
	//KSDEVEL printf("%X %X %X %X %X %X    ", xdr[0], xdr[1], xdr[2], xdr[3], xdr[4], xdr[5]);
	//KSDEVEL for (j = 6; j < xdrlength; j=j+4)
		//KSDEVEL printf("%X %X %X %X     ", xdr[j], xdr[j+1], xdr[j+2], xdr[j+3]);
	//KSDEVEL printf("\n\n");
	//KSDEVEL for (j = 5; j <  xdrlength; j=j+4)
		//KSDEVEL printf("%c %c %c %c     ", xdr[j], xdr[j+1], xdr[j+2], xdr[j+3]);
	//KSDEVEL printf("\n\n");
	//KSDEVEL printf("%s\n\n", xdr);
	
	//send
	ksapi_Channel_sendxdr(channel, pksc, xdr, xdrlength);
	
	free(xdr);
		
	}
	else if(ksapi_KSCommon_status_get(pksc) == STATUS_KSCOMMON_BUSY)
	{
		log_error("setString: connection in use");
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "connection in use", 5);
	}
	else {
		//not all values are set
		log_error("setString: Not all values are set. Sending cancelled");
		pvtableop->m_returnMethod(Ov_StaticPtrCast(ov_object, pobj), "Not all values are set", 2);
	}
	
	
	return;
}


/*	ksapi_setString_setandsubmit
 *	this function sets the necessary values automatically and starts the sending
 */
OV_DLLFNCEXPORT void ksapi_setString_setandsubmit(
	OV_INSTPTR_ksapi_setString	pobj,
	OV_STRING						host,
	OV_STRING						server,
	OV_STRING						path,
	OV_STRING						sendstring
) {
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	//set values:
	if (Ov_Fail(ksapi_setString_sendstring_set(pobj, sendstring))) {
		log_error("setString: error setting sendstring");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_host_set(pksc, host))) {
		log_error("setString: error setting host");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_server_set(pksc, server))) {
		log_error("setString: error setting server");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_path_set(pksc, path))) {
		log_error("setString: error setting path");
		return;
	}
	ksapi_setString_submit(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_setString_returnMethodxdr(
		OV_INSTPTR_ksapi_KSCommon			 	kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
	ksapi_KSCommon_analysesetxdr(kscommon, xdr, xdrlength);
	
	return;
}
