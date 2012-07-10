#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "ksapi_h.h"
#include "libov/ov_scheduler.h"

#include "xdrhandling.h"


/*	ksapi_getString_receivestring_get
 *	returns the current value of the receivestring
 */
OV_DLLFNCEXPORT OV_STRING ksapi_getString_receivestring_get(
	OV_INSTPTR_ksapi_getString	pobj
) {
	return pobj->v_receivestring;
}

/*	ksapi_getString_receivestring_set
 *	sets the current receivestring to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_getString_receivestring_set(
	OV_INSTPTR_ksapi_getString	pobj,
	const OV_STRING				value
) {
	return ov_string_setvalue(&pobj->v_receivestring,value);
}

/*	ksapi_getString_startup
 *	On startup set all values to default and close all connections
 */
OV_DLLFNCEXPORT void ksapi_getString_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	OV_INSTPTR_ksapi_getString sobj = Ov_StaticPtrCast(ksapi_getString, pobj);
	//To avoid a double Startup, which occures, because startup is executed while constructing 
	//the object and starting the FB, this check is implemented
	if (pobj->v_objectstate & OV_OS_STARTED) {
		return;
	}
	ksapi_KSCommon_startup(pobj);
	//initialize Values
	ksapi_getString_receivestring_set(sobj,"");
	
	return;
}

/*	ksapi_getString_shutdown
 *	After closing the service and the connection, shutdown the object.
 */
OV_DLLFNCEXPORT void ksapi_getString_shutdown(
	OV_INSTPTR_ov_object	pobj
) {
	ksapi_KSCommon_shutdown(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_getString_returnMethodxdr(
		OV_INSTPTR_ksapi_KSCommon			 	kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
	OV_INSTPTR_ksapi_getString pgs = Ov_StaticPtrCast(ksapi_getString, kscommon);
	OV_VTBLPTR_ksapi_getString  pvtableop; 
	char* result=NULL;
	
	Ov_GetVTablePtr(ksapi_getString, pvtableop, pgs);

	//Analyse xdr
	if((analysegetreply(OV_VT_STRING, xdr, xdrlength, &result)) == 0)
	{
		ksapi_getString_receivestring_set(pgs, result);
		free(result);
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "Reading completed", 1);
	}
	else
	{
		ksapi_getString_receivestring_set(pgs, "error");
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "error", -1);
	}
	
	
	return;
}
