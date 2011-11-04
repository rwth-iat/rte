#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "ksapi_h.h"
#include "libov/ov_scheduler.h"

#include "xdrhandling.h"


/*	ksapi_getBool_receivebool_get
 *	returns the current value of the receivebool
 */
OV_DLLFNCEXPORT OV_BOOL ksapi_getBool_receivebool_get(
	OV_INSTPTR_ksapi_getBool	pobj
) {
	return pobj->v_receivebool;
}

/*	ksapi_getBool_receivebool_set
 *	sets the current receivebool to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_getBool_receivebool_set(
	OV_INSTPTR_ksapi_getBool	pobj,
	const OV_BOOL					value
) {
	pobj->v_receivebool=value;
	return OV_ERR_OK;
}

/*	ksapi_getBool_startup
 *	On startup set all values to default and close all connections
 */
OV_DLLFNCEXPORT void ksapi_getBool_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	OV_INSTPTR_ksapi_getBool sobj = Ov_StaticPtrCast(ksapi_getBool, pobj);
	//To avoid a double Startup, which occures, because startup is executed while constructing 
	//the object and starting the FB, this check is implemented
	if (pobj->v_objectstate & OV_OS_STARTED) {
		return;
	}
	ksapi_KSCommon_startup(pobj);
	//initialize Values
	ksapi_getBool_receivebool_set(sobj,TRUE);
	
	return;
}

/*	ksapi_getBool_shutdown
 *	After closing the service and the connection, shutdown the object.
 */
OV_DLLFNCEXPORT void ksapi_getBool_shutdown(
	OV_INSTPTR_ov_object	pobj
) {
	ksapi_KSCommon_shutdown(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_getBool_returnMethodxdr(
		OV_INSTPTR_ksapi_KSCommon	 			kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
	OV_INSTPTR_ksapi_getBool pgb = Ov_StaticPtrCast(ksapi_getBool, kscommon);
	OV_VTBLPTR_ksapi_getBool   pvtableop; 
	OV_BOOL result;
	
	Ov_GetVTablePtr(ksapi_getBool, pvtableop, pgb);

	//Analyse xdr
	if((analysegetboolreply(xdr, xdrlength, &result)) == 0)
	{
		ksapi_getBool_receivebool_set(pgb, result);
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "Reading completed", 1);
	}
	else
	{
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "error", -1);
	}
	
	
	return;
}
