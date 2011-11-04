#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "ksapi_h.h"
#include "libov/ov_scheduler.h"

#include "xdrhandling.h"


/*	ksapi_getUIntVec_receiveuintvec_get
 *	returns the current value and the length of the receiveuintvec
 */
OV_DLLFNCEXPORT OV_UINT * ksapi_getUIntVec_receiveuintvec_get(
	OV_INSTPTR_ksapi_getUIntVec	pobj,
	OV_UINT								*pveclen
) {
	*pveclen = pobj->v_receiveuintvec.veclen;
	return pobj->v_receiveuintvec.value;
}

/*	ksapi_getUIntVec_receiveuintvec_set
 *	sets the current receiveuintvec to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_getUIntVec_receiveuintvec_set(
	OV_INSTPTR_ksapi_getUIntVec	pobj,
	const OV_UINT	 					*pvalue,
	const OV_UINT						veclen
) {
	return Ov_SetDynamicVectorValue(&pobj->v_receiveuintvec,pvalue,veclen,UINT);        
}

/*	ksapi_getUIntVec_startup
 *	On startup set all values to default and close all connections
 */
OV_DLLFNCEXPORT void ksapi_getUIntVec_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	OV_INSTPTR_ksapi_getUIntVec sobj = Ov_StaticPtrCast(ksapi_getUIntVec, pobj);
	//To avoid a double Startup, which occures, because startup is executed while constructing 
	//the object and starting the FB, this check is implemented
	if (pobj->v_objectstate & OV_OS_STARTED) {
		return;
	}
	ksapi_KSCommon_startup(pobj);
	//initialize Values
	ksapi_getUIntVec_receiveuintvec_set(sobj,NULL,0);
	
	return;
}

/*	ksapi_getUIntVec_shutdown
 *	After closing the service and the connection, shutdown the object.
 */
OV_DLLFNCEXPORT void ksapi_getUIntVec_shutdown(
	OV_INSTPTR_ov_object	pobj
) {
	ksapi_KSCommon_shutdown(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_getUIntVec_returnMethodxdr(
		OV_INSTPTR_ksapi_KSCommon	 			kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
	OV_INSTPTR_ksapi_getUIntVec pgs = Ov_StaticPtrCast(ksapi_getUIntVec, kscommon);
	OV_VTBLPTR_ksapi_getUIntVec  pvtableop; 
	OV_UINT *result;
	int len_res;
	
	Ov_GetVTablePtr(ksapi_getUIntVec, pvtableop, pgs);

	//Analyse xdr
	if((analysegetuintvecreply(xdr, xdrlength, (unsigned int**)&result, &len_res)) == 0)
	{
		ksapi_getUIntVec_receiveuintvec_set(pgs, result, len_res);
		free(result);
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "Reading completed", 1);
	}
	else
	{
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "error", -1);
	}
	
	
	
	return;
}
