#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "ksapi_logfile.h"
#include "ksapi_h.h"
#include "libov/ov_scheduler.h"

#include "xdrhandling.h"

OV_DLLFNCEXPORT OV_TIME* ksapi_getInt_varTimeStamp_get(
    OV_INSTPTR_ksapi_getInt          pobj
) {
    return &pobj->v_varTimeStamp;
}

OV_DLLFNCEXPORT OV_RESULT ksapi_getInt_varTimeStamp_set(
    OV_INSTPTR_ksapi_getInt          pobj,
    const OV_TIME*  value
) {
    pobj->v_varTimeStamp = *value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT ksapi_getInt_varQState_get(
    OV_INSTPTR_ksapi_getInt          pobj
) {
    return pobj->v_varQState;
}

OV_DLLFNCEXPORT OV_RESULT ksapi_getInt_varQState_set(
    OV_INSTPTR_ksapi_getInt          pobj,
    const OV_UINT  value
) {
    pobj->v_varQState = value;
    return OV_ERR_OK;
}


/*	ksapi_getInt_receiveint_get
 *	returns the current value of the receiveint
 */
OV_DLLFNCEXPORT OV_INT ksapi_getInt_receiveint_get(
	OV_INSTPTR_ksapi_getInt	pobj
) {
	return pobj->v_receiveint;
}

/*	ksapi_getInt_receiveint_set
 *	sets the current receiveint to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_getInt_receiveint_set(
	OV_INSTPTR_ksapi_getInt	pobj,
	const OV_INT				value
) {
	pobj->v_receiveint=value;
	return OV_ERR_OK;
}

/*	ksapi_getInt_startup
 *	On startup set all values to default and close all connections
 */
OV_DLLFNCEXPORT void ksapi_getInt_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	OV_INSTPTR_ksapi_getInt sobj = Ov_StaticPtrCast(ksapi_getInt, pobj);
	//To avoid a double Startup, which occures, because startup is executed while constructing 
	//the object and starting the FB, this check is implemented
	if (pobj->v_objectstate & OV_OS_STARTED) {
		return;
	}
	ksapi_KSCommon_startup(pobj);
	//initialize Values
	ksapi_getInt_receiveint_set(sobj,0);
	
	return;
}

/*	ksapi_getInt_shutdown
 *	After closing the service and the connection, shutdown the object.
 */
OV_DLLFNCEXPORT void ksapi_getInt_shutdown(
	OV_INSTPTR_ov_object	pobj
) {
	ksapi_KSCommon_shutdown(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_getInt_returnMethodxdr(
		OV_INSTPTR_ksapi_KSCommon	 			kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
	OV_INSTPTR_ksapi_getInt pgi = Ov_StaticPtrCast(ksapi_getInt, kscommon);
	OV_VTBLPTR_ksapi_getInt  pvtableop; 
	OV_INT result;
	
	Ov_GetVTablePtr(ksapi_getInt, pvtableop, pgi);

	ksapi_logfile_debug("ksapi getInt returnmethod, starting analysing ");
	//Analyse xdr
	if((analysegetreply(OV_VT_INT, xdr, xdrlength, (int*)&result, &(pgi->v_varTimeStamp.secs), &(pgi->v_varTimeStamp.usecs), &(pgi->v_varQState))) == 0)
	{
		ksapi_getInt_receiveint_set(pgi, result);
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "Reading completed", 1);
	}
	else
	{
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "error", -1);
	}
	
	
	return;
}
