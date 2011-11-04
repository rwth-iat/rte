#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif

#include "ksapi.h"
#include "ksapi_h.h"
#include "libov/ov_scheduler.h"
#include "ksapi_logfile.h"

#include "xdrhandling.h"
#include "xdrexec.h"
#include "config.h"

#if !OV_SYSTEM_NT
#include <unistd.h>
#endif
/*	Verwendete Fehlercodes fuer TODO
 *
 *	1	:	alles OK
 *	2	:	Fehler bei der Eingabe
 *	3	:	Keine Antwort-Methode gesetzt
 *	4	:	OV-Fehler
 *	5	:	Verbindungsfehler
 *	6	:	Servicefehler
 *	7	:	falscher Pfad
 * 8	:	Receiver belegt
 *
 */
void KSCommon_receivexdr(OV_INSTPTR_ov_object pobj);

/*	ksapi_KSCommon_host_get
 *	returns the current host
 */
OV_DLLFNCEXPORT OV_STRING ksapi_KSCommon_host_get(
	OV_INSTPTR_ksapi_KSCommon	pobj
) {
	return pobj->v_host;
} // end ksapi_KSCommon_host_get

/*	ksapi_KSCommon_host_set
 *	sets the current host to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_KSCommon_host_set(
	OV_INSTPTR_ksapi_KSCommon	pobj,
	const OV_STRING				value
) {

	return ov_string_setvalue(&pobj->v_host, value);
} // end ksapi_KSCommon_host_set


/*	ksapi_KSCommon_server_get
 *	returns the current server
 */
OV_DLLFNCEXPORT OV_STRING ksapi_KSCommon_server_get(
	OV_INSTPTR_ksapi_KSCommon	pobj
) {
	return pobj->v_server;
} // end ksapi_KSCommon_server_get

/*	ksapi_KSCommon_server_set
 *	sets the current server to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_KSCommon_server_set(
	OV_INSTPTR_ksapi_KSCommon	pobj,
	const OV_STRING				value
) {
		return ov_string_setvalue(&pobj->v_server,value);
} // end ksapi_KSCommon_server_set

/*	ksapi_KSCommon_path_get
 *	returns the current path
 */
OV_DLLFNCEXPORT OV_STRING ksapi_KSCommon_path_get(
	OV_INSTPTR_ksapi_KSCommon	pobj
) {
	return pobj->v_path;
} // end ksapi_KSCommon_path_get


/*	ksapi_KSCommon_path_set
 *	sets the current path to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_KSCommon_path_set(
	OV_INSTPTR_ksapi_KSCommon	pobj,
	const OV_STRING				value
) {
	return ov_string_setvalue(&pobj->v_path, value);
} // end ksapi_KSCommon_path_set


/*	ksapi_KSCommon_status_get
 * Status of the object
 */
OV_DLLFNCEXPORT OV_INT ksapi_KSCommon_status_get(
	OV_INSTPTR_ksapi_KSCommon	pobj
) {
	OV_INSTPTR_ksapi_Channel channel = (OV_INSTPTR_ksapi_Channel)Ov_SearchChild(ov_containment, pobj, "channel");
	if(pobj->v_status != STATUS_KSCOMMON_OK) return pobj->v_status;

	if(!channel) return STATUS_KSCOMMON_NOCHANNEL;
	if(channel->v_constate > 10 && channel->v_constate <20) return STATUS_KSCOMMON_BUSY;
	if(channel->v_constate >20) return STATUS_KSCOMMON_CHANNELERROR;

	return STATUS_KSCOMMON_OK;
}

/*	ksapi_KSCommon_status_set
 * Sets the status
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_KSCommon_status_set(
		OV_INSTPTR_ksapi_KSCommon	pobj,
		const OV_INT					value
) {
	pobj->v_status = value;
	return OV_ERR_OK;
}


/*	ksapi_KSCommon_startup
 *	On startup set all values to default and close all connections
 */
OV_DLLFNCEXPORT void ksapi_KSCommon_startup(
	OV_INSTPTR_ov_object	pobj
) {
	OV_INSTPTR_ksapi_KSCommon sobj =	Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	OV_TIME curtime;
	
	ov_object_startup(pobj);
	
	//initialize Values
	ksapi_KSCommon_host_set(sobj,"");
	ksapi_KSCommon_server_set(sobj,"");
	ksapi_KSCommon_path_set(sobj,"");
	ov_time_gettime(&curtime);
	
	ksapi_KSCommon_deregisterMethod(sobj);

	return;
} // end startup

/*	ksapi_KSCommon_shutdown
 *	After closing the service and the connection, shutdown the object.
 */
OV_DLLFNCEXPORT void ksapi_KSCommon_shutdown(
	OV_INSTPTR_ov_object	pobj
) {
	OV_INSTPTR_ksapi_KSCommon sobj =	Ov_StaticPtrCast(ksapi_KSCommon, pobj);

	sobj->v_status = 0;
	ov_object_shutdown(pobj);
	return;
} // end shutdown



/*	ksapi_KSCommon_submitget
 *  For all GET: Generates XDR and handsover to channel.
 *	If path,host,server were manually set, you can use this function to read the variable
 */
OV_DLLFNCEXPORT void ksapi_KSCommon_submitget(
	OV_INSTPTR_ksapi_KSCommon	pobj
) {
	OV_INSTPTR_ksapi_Channel channel = (OV_INSTPTR_ksapi_Channel)Ov_SearchChild(ov_containment, pobj, "channel");
	OV_VTBLPTR_ksapi_KSCommon   pvtableop; 
	char *xdr;
	int xdrlength;
	
	Ov_GetVTablePtr(ksapi_KSCommon, pvtableop, pobj);
	
	if(channel == NULL) {
		ov_logfile_error("KScommon GET object without Channel to use: %s", pobj->v_identifier);
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "no channel Object", 2);
		return;
	}

	if((pobj->v_host) && (pobj->v_server) && (pobj->v_path)
			&& (ksapi_KSCommon_status_get(pobj) == STATUS_KSCOMMON_OK)){
		//all values are set
		
		//send
		generategetxdr(&xdr, &xdrlength, pobj->v_path);
		ksapi_Channel_sendxdr(channel, pobj, xdr, xdrlength);
	} else if(ksapi_KSCommon_status_get(pobj) == STATUS_KSCOMMON_BUSY) {
		ksapi_logfile_error("get*: connection in use");
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "connection in use", 5);
	} else {
		//not all values are set
		ksapi_logfile_error("get*: Not all values are set. Reading cancelled");
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "Not all values are set", 2);
	}

	free(xdr);

	return;
}

/*	ksapi_KSCommon_setandsubmitget
 *  For all GET: Sets the necessary values and starts the reading via submitget
 */
OV_DLLFNCEXPORT void ksapi_KSCommon_setandsubmitget(
	OV_INSTPTR_ksapi_KSCommon				pobj,
	OV_STRING 									host,
	OV_STRING 									server,
	OV_STRING 									path
) {
	//set values:
	if (Ov_Fail(ksapi_KSCommon_host_set(pobj, host))) {
		ksapi_logfile_error("get*: error setting host");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_server_set(pobj, server))) {
		ksapi_logfile_error("get*: error setting server");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_path_set(pobj, path))) {
		ksapi_logfile_error("get*: error setting path");
		return;
	}
	ksapi_KSCommon_submitget(pobj);
	return;
}

/*	ksapi_KSCommon_registerMethod
 *
 *	This function sets the return method in the method table. This return method 
 *	contains the things happening at the end of the sending process and it must fit 
 *	to the environment in which the sendapi is used.
 */
OV_DLLFNCEXPORT void ksapi_KSCommon_registerMethod(
	OV_INSTPTR_ksapi_KSCommon				pobj,
	OV_FNCPTR_KSAPI_FNC_RET					pfnc
) {
	if (pobj && pfnc) {
		pobj->v_returnmethodptr = pfnc;
	}
	else {
		ksapi_logfile_error("register method failed; missing object or function pointer");
	}
	return;
} // end ksapi_KSCommon_registerMethod


/*	ksapi_KSCommon_deregisterMethod
 * deletes the return method
 */
OV_DLLFNCEXPORT void ksapi_KSCommon_deregisterMethod(
	OV_INSTPTR_ksapi_KSCommon	pobj
) {
	OV_VTBLPTR_ksapi_KSCommon   pvtableop; 
	if (!pobj) {
		ksapi_logfile_error("missing object in deregister method");
	}
	Ov_GetVTablePtr(ksapi_KSCommon, pvtableop, pobj)
	if (pvtableop){
		pobj->v_returnmethodptr = pvtableop->m_defaultretMethod;
	} else{
		ksapi_logfile_error("Method could not be deregistered; not found");
	}
	return;
} // end ksapi_KSCommon_deregisterMethod

/*	ksapi_KSCommon_returnMethod
 *
 *	This method is empty and must be overwritten. It has to stand here to avoid problems while compiling 
 *	this file.
 *	
 */
OV_DLLFNCEXPORT void ksapi_KSCommon_returnMethod(
	OV_INSTPTR_ov_object	pobj,
	OV_STRING				errorstring,
	OV_INT					errorcode
) {
	OV_INSTPTR_ksapi_KSCommon	pkscommonobj;
	if (!pobj) {
		ksapi_logfile_error("No object pointer in returnMethod given.");
		return;
	}
	pkscommonobj = Ov_DynamicPtrCast(ksapi_KSCommon, pobj);
	if ((pkscommonobj) && (pkscommonobj->v_returnmethodptr)) {
		pkscommonobj->v_returnmethodptr(pobj, errorstring, errorcode);
	}
	else {
		ksapi_logfile_error("Wrong pointer to KSCommon object in returnMethod given. Check your implementation!");
	}
	return;
} // end ksapi_KSCommon_returnMethod


/*	ksapi_KSCommon_defaultretMethod
 *
 *	Dummy-Method. This Message is implemented to have a default return message. 
 *	
 *	
 */
OV_DLLFNCEXPORT void ksapi_KSCommon_defaultretMethod(
		OV_INSTPTR_ov_object 					pobj,
		OV_STRING 									errorstring,
		OV_INT	 									errorcode
) {
	ksapi_logfile_info("No return method defined. The following error occured: ");
	if (errorstring) {
		ksapi_logfile_info(errorstring);
	}
	return;
} // end ksapi_KSCommon_defaultretMethod


/**
 * For all Set: One analyseXDR is enough, which is done here
 * This called by the set* Classe's returnmethodxdr()
 */
OV_DLLFNCEXPORT void ksapi_KSCommon_analysesetxdr(
	OV_INSTPTR_ksapi_KSCommon	 			pobj,
	OV_STRING									xdr,
	OV_INT										xdrlength
) {
	OV_INSTPTR_ov_object	pov = Ov_StaticPtrCast(ov_object, pobj);
	OV_VTBLPTR_ksapi_KSCommon pvtableop;
	Ov_GetVTablePtr(ksapi_KSCommon, pvtableop, pobj);
	
	ksapi_KSCommon_status_set(pobj, STATUS_KSCOMMON_OK);

	if((analysesetxdr(xdr, xdrlength)) == 0)
		pvtableop->m_returnMethod(pov, "Sending completed", 1);
		
	return;
}

OV_DLLFNCEXPORT void ksapi_KSCommon_returnMethodxdr(
	OV_INSTPTR_ksapi_KSCommon 			pobj,
	OV_STRING								xdr,
	OV_INT									xdrlength
) {
	ov_logfile_error("This KSCommon hasnt a returnMethodxdr - but it should (overload this method)!!!");
	return;
}

