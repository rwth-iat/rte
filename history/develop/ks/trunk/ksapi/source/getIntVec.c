#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "ksapi_h.h"
#include "libov/ov_scheduler.h"

#include "xdrhandling.h"


/*	ksapi_getIntVec_receiveintvec_get
 *	returns the current value and the length of the receiveintvec
 */
OV_DLLFNCEXPORT OV_INT * ksapi_getIntVec_receiveintvec_get(
	OV_INSTPTR_ksapi_getIntVec	pobj,
	OV_UINT								*pveclen
) {
	*pveclen = pobj->v_receiveintvec.veclen;
	return pobj->v_receiveintvec.value;
}

/*	ksapi_getIntVec_receiveintvec_set
 *	sets the current receiveintvec to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_getIntVec_receiveintvec_set(
	OV_INSTPTR_ksapi_getIntVec	pobj,
	const OV_INT	 					*pvalue,
	const OV_UINT						veclen
) {
	return Ov_SetDynamicVectorValue(&pobj->v_receiveintvec,pvalue,veclen,INT);        
}

/*	ksapi_getIntVec_startup
 *	On startup set all values to default and close all connections
 */
OV_DLLFNCEXPORT void ksapi_getIntVec_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	OV_INSTPTR_ksapi_getIntVec sobj = Ov_StaticPtrCast(ksapi_getIntVec, pobj);
	//To avoid a double Startup, which occures, because startup is executed while constructing 
	//the object and starting the FB, this check is implemented
	if (pobj->v_objectstate & OV_OS_STARTED) {
		return;
	}
	ksapi_KSCommon_startup(pobj);
	//initialize Values
	ksapi_getIntVec_receiveintvec_set(sobj,NULL,0);
	
	return;
}

/*	ksapi_getIntVec_shutdown
 *	After closing the service and the connection, shutdown the object.
 */
OV_DLLFNCEXPORT void ksapi_getIntVec_shutdown(
	OV_INSTPTR_ov_object	pobj
) {
	ksapi_KSCommon_shutdown(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_getIntVec_returnMethodxdr(
		OV_INSTPTR_ksapi_KSCommon			 	kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
	OV_INSTPTR_ksapi_getIntVec pgs = Ov_StaticPtrCast(ksapi_getIntVec, kscommon);
	OV_VTBLPTR_ksapi_getIntVec  pvtableop; 
	OV_INT *result;
	int len_res;
	
	Ov_GetVTablePtr(ksapi_getIntVec, pvtableop, pgs);
	
	//Analyse xdr
	if((analysegetvecreply(OV_VT_INT_VEC, xdr, xdrlength, (void**) &result, &len_res)) == 0)
	{
		ksapi_getIntVec_receiveintvec_set(pgs, result, len_res);
		free(result);
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "Reading completed", 1);
	}
	else
	{
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "error", -1);
	}
	
	
	return;
}
