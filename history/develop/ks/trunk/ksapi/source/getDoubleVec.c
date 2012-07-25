#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "ksapi_h.h"
#include "libov/ov_scheduler.h"

#include "xdrhandling.h"


OV_DLLFNCEXPORT OV_TIME* ksapi_getDoubleVec_varTimeStamp_get(
    OV_INSTPTR_ksapi_getDoubleVec          pobj
) {
    return &pobj->v_varTimeStamp;
}

OV_DLLFNCEXPORT OV_RESULT ksapi_getDoubleVec_varTimeStamp_set(
    OV_INSTPTR_ksapi_getDoubleVec          pobj,
    const OV_TIME*  value
) {
    pobj->v_varTimeStamp = *value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT ksapi_getDoubleVec_varQState_get(
    OV_INSTPTR_ksapi_getDoubleVec          pobj
) {
    return pobj->v_varQState;
}

OV_DLLFNCEXPORT OV_RESULT ksapi_getDoubleVec_varQState_set(
    OV_INSTPTR_ksapi_getDoubleVec          pobj,
    const OV_UINT  value
) {
    pobj->v_varQState = value;
    return OV_ERR_OK;
}


/*	ksapi_getDoubleVec_receivedoublevec_get
 *	returns the current value and the length of the receivedoublevec
 */
OV_DLLFNCEXPORT OV_DOUBLE * ksapi_getDoubleVec_receivedoublevec_get(
	OV_INSTPTR_ksapi_getDoubleVec	pobj,
	OV_UINT								*pveclen
) {
	*pveclen = pobj->v_receivedoublevec.veclen;
	return pobj->v_receivedoublevec.value;
}

/*	ksapi_getDoubleVec_receivedoublevec_set
 *	sets the current receivedoublevec to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_getDoubleVec_receivedoublevec_set(
	OV_INSTPTR_ksapi_getDoubleVec	pobj,
	const OV_DOUBLE	 					*pvalue,
	const OV_UINT						veclen
) {
	return Ov_SetDynamicVectorValue(&pobj->v_receivedoublevec,pvalue,veclen,DOUBLE);        
}

/*	ksapi_getDoubleVec_startup
 *	On startup set all values to default and close all connections
 */
OV_DLLFNCEXPORT void ksapi_getDoubleVec_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	OV_INSTPTR_ksapi_getDoubleVec sobj = Ov_StaticPtrCast(ksapi_getDoubleVec, pobj);
	//To avoid a double Startup, which occures, because startup is executed while constructing 
	//the object and starting the FB, this check is implemented
	if (pobj->v_objectstate & OV_OS_STARTED) {
		return;
	}
	ksapi_KSCommon_startup(pobj);
	//initialize Values
	ksapi_getDoubleVec_receivedoublevec_set(sobj,NULL,0);
	
	return;
}

/*	ksapi_getDoubleVec_shutdown
 *	After closing the service and the connection, shutdown the object.
 */
OV_DLLFNCEXPORT void ksapi_getDoubleVec_shutdown(
	OV_INSTPTR_ov_object	pobj
) {
	ksapi_KSCommon_shutdown(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_getDoubleVec_returnMethodxdr(
		OV_INSTPTR_ksapi_KSCommon	 			kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
	OV_INSTPTR_ksapi_getDoubleVec pgs = Ov_StaticPtrCast(ksapi_getDoubleVec, kscommon);
	OV_VTBLPTR_ksapi_getDoubleVec  pvtableop; 
	OV_DOUBLE *result=NULL;
	int len_res;
	
	Ov_GetVTablePtr(ksapi_getDoubleVec, pvtableop, pgs);

	//Analyse xdr
	if((analysegetvecreply(OV_VT_DOUBLE_VEC, xdr, xdrlength, (void**) &result, &len_res,
			&(pgs->v_varTimeStamp.secs), &(pgs->v_varTimeStamp.usecs), &(pgs->v_varQState))) == 0)
	{
		ksapi_getDoubleVec_receivedoublevec_set(pgs, result, len_res);
		free(result);
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "Reading completed", 1);
	}
	else
	{
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "error", -1);
	}
	
	
	return;
}
