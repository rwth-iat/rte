#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "ksapi_h.h"
#include "libov/ov_scheduler.h"

#include "xdrhandling.h"


/*	ksapi_getSingleVec_receivesinglevec_get
 *	returns the current value and the length of the receivesinglevec
 */
OV_DLLFNCEXPORT OV_SINGLE * ksapi_getSingleVec_receivesinglevec_get(
	OV_INSTPTR_ksapi_getSingleVec	pobj,
	OV_UINT								*pveclen
) {
	*pveclen = pobj->v_receivesinglevec.veclen;
	return pobj->v_receivesinglevec.value;
}

/*	ksapi_getSingleVec_receivesinglevec_set
 *	sets the current receivesinglevec to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_getSingleVec_receivesinglevec_set(
	OV_INSTPTR_ksapi_getSingleVec	pobj,
	const OV_SINGLE	 					*pvalue,
	const OV_UINT						veclen
) {
	return Ov_SetDynamicVectorValue(&pobj->v_receivesinglevec,pvalue,veclen,SINGLE);        
}

/*	ksapi_getSingleVec_startup
 *	On startup set all values to default and close all connections
 */
OV_DLLFNCEXPORT void ksapi_getSingleVec_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	OV_INSTPTR_ksapi_getSingleVec sobj = Ov_StaticPtrCast(ksapi_getSingleVec, pobj);
	//To avoid a double Startup, which occures, because startup is executed while constructing 
	//the object and starting the FB, this check is implemented
	if (pobj->v_objectstate & OV_OS_STARTED) {
		return;
	}
	ksapi_KSCommon_startup(pobj);
	//initialize Values
	ksapi_getSingleVec_receivesinglevec_set(sobj,NULL,0);
	
	return;
}

/*	ksapi_getSingleVec_shutdown
 *	After closing the service and the connection, shutdown the object.
 */
OV_DLLFNCEXPORT void ksapi_getSingleVec_shutdown(
	OV_INSTPTR_ov_object	pobj
) {
	ksapi_KSCommon_shutdown(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_getSingleVec_returnMethodxdr(
		OV_INSTPTR_ksapi_KSCommon	 			kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
	OV_INSTPTR_ksapi_getSingleVec pgs = Ov_StaticPtrCast(ksapi_getSingleVec, kscommon);
	OV_VTBLPTR_ksapi_getSingleVec  pvtableop; 
	OV_SINGLE *result;
	int len_res;
	
	Ov_GetVTablePtr(ksapi_getSingleVec, pvtableop, pgs);

	//Analyse xdr
	if((analysegetvecreply(OV_VT_SINGLE_VEC, xdr, xdrlength, (void**) &result, &len_res)) == 0)
	{
		ksapi_getSingleVec_receivesinglevec_set(pgs, result, len_res);
		free(result);
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "Reading completed", 1);
	}
	else
	{
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "error", -1);
	}
	
	
	return;
}
