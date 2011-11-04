#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "ksapi_h.h"
#include "libov/ov_scheduler.h"

#include "xdrhandling.h"


/*	ksapi_getUInt_receiveuint_get
 *	returns the current value of the receiveuint
 */
OV_DLLFNCEXPORT OV_UINT ksapi_getUInt_receiveuint_get(
	OV_INSTPTR_ksapi_getUInt	pobj
) {
	return pobj->v_receiveuint;
}

/*	ksapi_getUInt_receiveuint_set
 *	sets the current receiveuint to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_getUInt_receiveuint_set(
	OV_INSTPTR_ksapi_getUInt	pobj,
	const OV_UINT				value
) {
	pobj->v_receiveuint=value;
	return OV_ERR_OK;
}

/*	ksapi_getUInt_startup
 *	On startup set all values to default and close all connections
 */
OV_DLLFNCEXPORT void ksapi_getUInt_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	OV_INSTPTR_ksapi_getUInt sobj = Ov_StaticPtrCast(ksapi_getUInt, pobj);
	//To avoid a double Startup, which occures, because startup is executed while constructing 
	//the object and starting the FB, this check is implemented
	if (pobj->v_objectstate & OV_OS_STARTED) {
		return;
	}
	ksapi_KSCommon_startup(pobj);
	//initialize Values
	ksapi_getUInt_receiveuint_set(sobj,0);
	
	return;
}

/*	ksapi_getUInt_shutdown
 *	After closing the service and the connection, shutdown the object.
 */
OV_DLLFNCEXPORT void ksapi_getUInt_shutdown(
	OV_INSTPTR_ov_object	pobj
) {
	ksapi_KSCommon_shutdown(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_getUInt_returnMethodxdr(
		OV_INSTPTR_ksapi_KSCommon	 			kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
	OV_INSTPTR_ksapi_getUInt pgi = Ov_StaticPtrCast(ksapi_getUInt, kscommon);
	OV_VTBLPTR_ksapi_getUInt  pvtableop; 
	OV_UINT result;
	
	Ov_GetVTablePtr(ksapi_getUInt, pvtableop, pgi);

	//Analyse xdr
	if((analysegetuintreply(xdr, xdrlength, (unsigned int*)&result)) == 0)
	{
		ksapi_getUInt_receiveuint_set(pgi, result);
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "Reading completed", 1);
	}
	else
	{
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "error", -1);
	}
	
	
	return;
}
