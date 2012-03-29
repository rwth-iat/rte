#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "ksapi_h.h"
#include "libov/ov_scheduler.h"

#include "xdrhandling.h"


/*	ksapi_getStringVec_receivestringvec_get
 *	returns the current value and the length of the receivestringvec
 */
OV_DLLFNCEXPORT OV_STRING * ksapi_getStringVec_receivestringvec_get(
	OV_INSTPTR_ksapi_getStringVec	pobj,
	OV_UINT								*pveclen
) {
	*pveclen = pobj->v_receivestringvec.veclen;
	return pobj->v_receivestringvec.value;
}

/*	ksapi_getStringVec_receivestringvec_set
 *	sets the current receivestringvec to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_getStringVec_receivestringvec_set(
	OV_INSTPTR_ksapi_getStringVec	pobj,
	const OV_STRING 					*pvalue,
	const OV_UINT						veclen
) {
	return Ov_SetDynamicVectorValue(&pobj->v_receivestringvec,pvalue,veclen,STRING);        
}

/*	ksapi_getStringVec_startup
 *	On startup set all values to default and close all connections
 */
OV_DLLFNCEXPORT void ksapi_getStringVec_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	OV_INSTPTR_ksapi_getStringVec sobj = Ov_StaticPtrCast(ksapi_getStringVec, pobj);
	//To avoid a double Startup, which occures, because startup is executed while constructing 
	//the object and starting the FB, this check is implemented
	if (pobj->v_objectstate & OV_OS_STARTED) {
		return;
	}
	ksapi_KSCommon_startup(pobj);
	//initialize Values
	ksapi_getStringVec_receivestringvec_set(sobj,NULL,0);
	
	return;
}

/*	ksapi_getStringVec_shutdown
 *	After closing the service and the connection, shutdown the object.
 */
OV_DLLFNCEXPORT void ksapi_getStringVec_shutdown(
	OV_INSTPTR_ov_object	pobj
) {
	ksapi_KSCommon_shutdown(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_getStringVec_returnMethodxdr(
		OV_INSTPTR_ksapi_KSCommon	 			kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
	OV_INSTPTR_ksapi_getStringVec pgs = Ov_StaticPtrCast(ksapi_getStringVec, kscommon);
	OV_VTBLPTR_ksapi_getStringVec  pvtableop; 
	char** result;
	int len_res;
	
	Ov_GetVTablePtr(ksapi_getStringVec, pvtableop, pgs);
	
	//Analyse xdr
	if((analysegetvecreply(OV_VT_STRING_VEC, xdr, xdrlength, (void**) &result, &len_res)) == 0)
	{
		ksapi_getStringVec_receivestringvec_set(pgs, result, len_res);
		free(result);
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "Reading completed", 1);
	}
	else
	{
		ksapi_getStringVec_receivestringvec_set(pgs, (char* const*)"error", strlen("error"));
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "error", -1);
	}
	
	
	return;
}
