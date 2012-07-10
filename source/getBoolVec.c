#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "ksapi_h.h"
#include "libov/ov_scheduler.h"

#include "xdrhandling.h"


/*	ksapi_getBoolVec_receiveboolvec_get
 *	returns the current value and the length of the receiveboolvec
 */
OV_DLLFNCEXPORT OV_BOOL * ksapi_getBoolVec_receiveboolvec_get(
	OV_INSTPTR_ksapi_getBoolVec	pobj,
	OV_UINT								*pveclen
) {
	*pveclen = pobj->v_receiveboolvec.veclen;
	return pobj->v_receiveboolvec.value;
}

/*	ksapi_getBoolVec_receiveboolvec_set
 *	sets the current receiveboolvec to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_getBoolVec_receiveboolvec_set(
	OV_INSTPTR_ksapi_getBoolVec	pobj,
	const OV_BOOL	 					*pvalue,
	const OV_UINT						veclen
) {
	return Ov_SetDynamicVectorValue(&pobj->v_receiveboolvec,pvalue,veclen,BOOL);        
}

/*	ksapi_getBoolVec_startup
 *	On startup set all values to default and close all connections
 */
OV_DLLFNCEXPORT void ksapi_getBoolVec_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	OV_INSTPTR_ksapi_getBoolVec sobj = Ov_StaticPtrCast(ksapi_getBoolVec, pobj);
	//To avoid a bool Startup, which occures, because startup is executed while constructing 
	//the object and starting the FB, this check is implemented
	if (pobj->v_objectstate & OV_OS_STARTED) {
		return;
	}
	ksapi_KSCommon_startup(pobj);
	//initialize Values
	ksapi_getBoolVec_receiveboolvec_set(sobj,NULL,0);
	
	return;
}

/*	ksapi_getBoolVec_shutdown
 *	After closing the service and the connection, shutdown the object.
 */
OV_DLLFNCEXPORT void ksapi_getBoolVec_shutdown(
	OV_INSTPTR_ov_object	pobj
) {
	ksapi_KSCommon_shutdown(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_getBoolVec_returnMethodxdr(
		OV_INSTPTR_ksapi_KSCommon	 			kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
	OV_INSTPTR_ksapi_getBoolVec pgbv = Ov_StaticPtrCast(ksapi_getBoolVec, kscommon);
	OV_VTBLPTR_ksapi_getBoolVec  pvtableop; 
	OV_BOOL *result=NULL;
	int len_res;
	
	Ov_GetVTablePtr(ksapi_getBoolVec, pvtableop, pgbv);

	//Analyse xdr
	if((analysegetvecreply(OV_VT_BOOL_VEC, xdr, xdrlength, (void**) &result, &len_res)) == 0)
	{
		ksapi_getBoolVec_receiveboolvec_set(pgbv, result, len_res);
		free(result);
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "Reading completed", 1);
	}
	else
	{
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "error", -1);
	}
	
	
	return;
}
