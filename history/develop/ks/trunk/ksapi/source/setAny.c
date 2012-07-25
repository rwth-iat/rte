#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "ksapi_h.h"
#include "libov/ov_scheduler.h"

#include "xdrhandling.h"
#include "config.h"

OV_DLLFNCEXPORT OV_TIME* ksapi_setAny_varTimeStamp_get(
    OV_INSTPTR_ksapi_setAny          pobj
) {
    return &pobj->v_varTimeStamp;
}

OV_DLLFNCEXPORT OV_RESULT ksapi_setAny_varTimeStamp_set(
    OV_INSTPTR_ksapi_setAny          pobj,
    const OV_TIME*  value
) {
    pobj->v_varTimeStamp = *value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT ksapi_setAny_varQState_get(
    OV_INSTPTR_ksapi_setAny          pobj
) {
    return pobj->v_varQState;
}

OV_DLLFNCEXPORT OV_RESULT ksapi_setAny_varQState_set(
    OV_INSTPTR_ksapi_setAny          pobj,
    const OV_UINT  value
) {
    pobj->v_varQState = value;
    return OV_ERR_OK;
}



OV_DLLFNCEXPORT OV_RESULT ksapi_setAny_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
    OV_INSTPTR_ksapi_setAny pinst = Ov_StaticPtrCast(ksapi_setAny, pobj);
    OV_RESULT    result;

    /* do what the base class does first */
    result = ov_object_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */

    pinst->v_sendany.value.vartype = OV_VT_VOID;
    pinst->v_sendany.value.valueunion.val_double = 0.0;

    return OV_ERR_OK;
}

OV_DLLFNCEXPORT void ksapi_setAny_destructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
    OV_INSTPTR_ksapi_setAny pinst = Ov_StaticPtrCast(ksapi_setAny, pobj);

    /* do what */

    /* destroy object */
    ov_object_destructor(pobj);

    Ov_SetAnyValue(&(pinst->v_sendany), NULL);

    return;
}


/*	ksapi_setAny_sendany_get
 *	returns the current value of the sendany
 */
OV_DLLFNCEXPORT OV_ANY* ksapi_setAny_sendany_get(
	OV_INSTPTR_ksapi_setAny	pobj
) {
	return &(pobj->v_sendany);
}

/*	ksapi_setAny_sendany_set
 *	sets the current sendany to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_setAny_sendany_set(
	OV_INSTPTR_ksapi_setAny	pobj,
	const OV_ANY*           	value
) {
	return Ov_SetAnyValue(&(pobj->v_sendany), value);
}

/*	ksapi_setAny_startup
 *	On startup set all values to default and close all connections
 */
OV_DLLFNCEXPORT void ksapi_setAny_startup(
	OV_INSTPTR_ov_object			pobj
) {
	OV_INSTPTR_ksapi_setAny sobj	=	Ov_StaticPtrCast(ksapi_setAny, pobj);
	//To avoid a double Startup, which occures, because startup is executed while constructing 
	//the object and starting the FB, this check is implemented
	if (pobj->v_objectstate & OV_OS_STARTED) {
		return;
	}
	ksapi_KSCommon_startup(pobj);
	//initialize Values
	ksapi_setAny_sendany_set(sobj,0);
	
	return;
}

/*	ksapi_setAny_shutdown
 *	After closing the service and the connection, shutdown the object.
 */
OV_DLLFNCEXPORT void ksapi_setAny_shutdown(
	OV_INSTPTR_ov_object	pobj
) {
	ksapi_KSCommon_shutdown(pobj);
	return;
}

/*	ksapi_setAny_submit
 *	If all parameters were manually set, you can use this function to send the string
 */
OV_DLLFNCEXPORT void ksapi_setAny_submit(
	OV_INSTPTR_ksapi_setAny	pobj
) {
	OV_VTBLPTR_ksapi_setAny   pvtableop;
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	OV_INSTPTR_ksapi_Channel channel;
	char *xdr=NULL;
	int xdrlength;
	OV_UINT tTsSecs, tTsUsecs, tQstate;

	Ov_GetVTablePtr(ksapi_setAny, pvtableop, pobj);

	channel = (OV_INSTPTR_ksapi_Channel)Ov_SearchChild(ov_containment, pobj, "channel");
	if(channel == NULL) {
	   ov_logfile_error("SETANY object without Channel to use: %s", pobj->v_identifier);
	   pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "no channel Object", 2);
	   return;
	}
	
	if((pobj->v_host) && (pobj->v_server) && (pobj->v_path) && (ksapi_KSCommon_status_get(pksc) != STATUS_KSCOMMON_BUSY)){
		//all values are set
	
	
	generateheader(KS_SETVAR, &xdr, &xdrlength);
	//decide which Timestamp / Qstate to use: values inside the Any override inputs of ksapi-Block
	if(pobj->v_sendany.value.vartype & OV_VT_HAS_STATE)
		tQstate = pobj->v_sendany.state;
	else
		tQstate = pobj->v_varQState;
	if(pobj->v_sendany.value.vartype & OV_VT_HAS_TIMESTAMP)
	{
		tTsSecs = pobj->v_sendany.time.secs;
		tTsUsecs = pobj->v_sendany.time.usecs;
	}
	else
	{
		tTsSecs = pobj->v_varTimeStamp.secs;
		tTsUsecs = pobj->v_varTimeStamp.usecs;
	}

	switch(pobj->v_sendany.value.vartype & OV_VT_KSMASK)
	{
	case OV_VT_BOOL:
		generatesetbody(OV_VT_BOOL, &xdr, &xdrlength, pobj->v_path, &(pobj->v_sendany.value.valueunion.val_bool), tTsSecs, tTsUsecs, tQstate);
		break;
	case OV_VT_UINT:
		generatesetbody(OV_VT_UINT, &xdr, &xdrlength, pobj->v_path, &(pobj->v_sendany.value.valueunion.val_uint), tTsSecs, tTsUsecs, tQstate);
		break;
	case OV_VT_INT:
		generatesetbody(OV_VT_INT, &xdr, &xdrlength, pobj->v_path, &(pobj->v_sendany.value.valueunion.val_int), tTsSecs, tTsUsecs, tQstate);
		break;
	case OV_VT_SINGLE:
		generatesetbody(OV_VT_SINGLE, &xdr, &xdrlength, pobj->v_path, &(pobj->v_sendany.value.valueunion.val_single), tTsSecs, tTsUsecs, tQstate);
		break;
	case OV_VT_DOUBLE:
		generatesetbody(OV_VT_DOUBLE, &xdr, &xdrlength, pobj->v_path, &(pobj->v_sendany.value.valueunion.val_double), tTsSecs, tTsUsecs, tQstate);
		break;
	case OV_VT_TIME:
		generatesetbody(OV_VT_TIME, &xdr, &xdrlength, pobj->v_path, &(pobj->v_sendany.value.valueunion.val_time), tTsSecs, tTsUsecs, tQstate);
		break;
	case OV_VT_TIME_SPAN:
		generatesetbody(OV_VT_TIME_SPAN, &xdr, &xdrlength, pobj->v_path, &(pobj->v_sendany.value.valueunion.val_time_span), tTsSecs, tTsUsecs, tQstate);
		break;
	case OV_VT_STRING:
		generatesetbody(OV_VT_STRING, &xdr, &xdrlength, pobj->v_path, &(pobj->v_sendany.value.valueunion.val_string), tTsSecs, tTsUsecs, tQstate);
		break;
	case OV_VT_BOOL_VEC:
		generatesetvecbody(OV_VT_BOOL_VEC, &xdr, &xdrlength, pobj->v_path,
				(void*) pobj->v_sendany.value.valueunion.val_bool_vec.value, pobj->v_sendany.value.valueunion.val_bool_vec.veclen, tTsSecs, tTsUsecs, tQstate);
		break;
	case OV_VT_UINT_VEC:
		generatesetvecbody(OV_VT_UINT_VEC, &xdr, &xdrlength, pobj->v_path,
				(void*) pobj->v_sendany.value.valueunion.val_uint_vec.value, pobj->v_sendany.value.valueunion.val_uint_vec.veclen, tTsSecs, tTsUsecs, tQstate);
		break;
	case OV_VT_INT_VEC:
		generatesetvecbody(OV_VT_INT_VEC, &xdr, &xdrlength, pobj->v_path,
				(void*) pobj->v_sendany.value.valueunion.val_int_vec.value, pobj->v_sendany.value.valueunion.val_int_vec.veclen, tTsSecs, tTsUsecs, tQstate);
		break;
	case OV_VT_SINGLE_VEC:
		generatesetvecbody(OV_VT_SINGLE_VEC, &xdr, &xdrlength, pobj->v_path,
				(void*) pobj->v_sendany.value.valueunion.val_single_vec.value, pobj->v_sendany.value.valueunion.val_single_vec.veclen, tTsSecs, tTsUsecs, tQstate);
		break;
	case OV_VT_DOUBLE_VEC:
		generatesetvecbody(OV_VT_DOUBLE_VEC, &xdr, &xdrlength, pobj->v_path,
				(void*) pobj->v_sendany.value.valueunion.val_double_vec.value, pobj->v_sendany.value.valueunion.val_double_vec.veclen, tTsSecs, tTsUsecs, tQstate);
		break;
	case OV_VT_STRING_VEC:
		generatesetvecbody(OV_VT_STRING_VEC, &xdr, &xdrlength, pobj->v_path,
				(void*) &(pobj->v_sendany.value.valueunion.val_string_vec.value), pobj->v_sendany.value.valueunion.val_string_vec.veclen, tTsSecs, tTsUsecs, tQstate);
		break;
	case OV_VT_TIME_VEC:
		generatesetvecbody(OV_VT_TIME_VEC, &xdr, &xdrlength, pobj->v_path,
				(void*) pobj->v_sendany.value.valueunion.val_time_vec.value, pobj->v_sendany.value.valueunion.val_time_vec.veclen, tTsSecs, tTsUsecs, tQstate);
		break;
	case OV_VT_TIME_SPAN_VEC:
		generatesetvecbody(OV_VT_TIME_SPAN_VEC, &xdr, &xdrlength, pobj->v_path,
				(void*) pobj->v_sendany.value.valueunion.val_time_span_vec.value, pobj->v_sendany.value.valueunion.val_time_span_vec.veclen, tTsSecs, tTsUsecs, tQstate);
		break;
	default:
		ov_logfile_error("wtf type is this variable??? 0x%x", pobj->v_sendany.value.vartype);
		free(xdr);
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "Bad Variable Type", 4);
		return;
	}

	addrpcheader(&xdr, &xdrlength);

	
	//send
	ksapi_Channel_sendxdr(channel, pksc, xdr, xdrlength);
	
	free(xdr);
		
	}
	else if(ksapi_KSCommon_status_get(pksc) == STATUS_KSCOMMON_BUSY)
	{
		log_error("setAny: connection in use");
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "connection in use", 5);
	}
	else {
		//not all values are set
		log_error("setAny: Not all values are set. Sending cancelled");
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "Not all values are set", 2);
	}
	return;
}


/*	ksapi_setAny_setandsubmit
 *	this function sets the necessary values automatically and starts the sending
 */
OV_DLLFNCEXPORT void ksapi_setAny_setandsubmit(
	OV_INSTPTR_ksapi_setAny	pobj,
	OV_STRING						host,
	OV_STRING						server,
	OV_STRING						path,
	OV_ANY							sendany
) {
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	//set values:
	if (Ov_Fail(ksapi_setAny_sendany_set(pobj, &sendany))) {
		log_error("setAny: error setting sendany");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_host_set(pksc, host))) {
		log_error("setAny: error setting host");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_server_set(pksc, server))) {
		log_error("setAny: error setting server");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_path_set(pksc, path))) {
		log_error("setAny: error setting path");
		return;
	}
	ksapi_setAny_submit(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_setAny_returnMethodxdr(
		OV_INSTPTR_ksapi_KSCommon	 			kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
	ksapi_KSCommon_analysesetxdr(kscommon, xdr, xdrlength);
	
	return;
}
