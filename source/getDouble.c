#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "ksapi_h.h"
#include "libov/ov_scheduler.h"

#include "xdrhandling.h"


/*	ksapi_getDouble_receivedouble_get
 *	returns the current value of the receivedouble
 */
OV_DLLFNCEXPORT OV_DOUBLE ksapi_getDouble_receivedouble_get(
	OV_INSTPTR_ksapi_getDouble	pobj
) {
	return pobj->v_receivedouble;
}

/*	ksapi_getDouble_receivedouble_set
 *	sets the current receivedouble to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_getDouble_receivedouble_set(
	OV_INSTPTR_ksapi_getDouble	pobj,
	const OV_DOUBLE				value
) {
	pobj->v_receivedouble=value;
	return OV_ERR_OK;
}

/*	ksapi_getDouble_startup
 *	On startup set all values to default and close all connections
 */
OV_DLLFNCEXPORT void ksapi_getDouble_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	OV_INSTPTR_ksapi_getDouble sobj = Ov_StaticPtrCast(ksapi_getDouble, pobj);
	//To avoid a double Startup, which occures, because startup is executed while constructing 
	//the object and starting the FB, this check is implemented
	if (pobj->v_objectstate & OV_OS_STARTED) {
		return;
	}
	ksapi_KSCommon_startup(pobj);
	//initialize Values
	ksapi_getDouble_receivedouble_set(sobj,TRUE);
	
	return;
}

/*	ksapi_getDouble_shutdown
 *	After closing the service and the connection, shutdown the object.
 */
OV_DLLFNCEXPORT void ksapi_getDouble_shutdown(
	OV_INSTPTR_ov_object	pobj
) {
	ksapi_KSCommon_shutdown(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_getDouble_returnMethodxdr(
		OV_INSTPTR_ksapi_KSCommon	 			kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
	OV_INSTPTR_ksapi_getDouble pgd = Ov_StaticPtrCast(ksapi_getDouble, kscommon);
	OV_VTBLPTR_ksapi_getDouble  pvtableop; 
	OV_DOUBLE result;
	
	Ov_GetVTablePtr(ksapi_getDouble, pvtableop, pgd);

	//Analyse xdr
	if((analysegetreply(OV_VT_DOUBLE, xdr, xdrlength, &result)) == 0)
	{
		ksapi_getDouble_receivedouble_set(pgd, result);
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "Reading completed", 1);
	}
	else
	{
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "error", -1);
	}
	
	
	return;
}
