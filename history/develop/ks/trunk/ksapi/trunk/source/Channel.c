#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "libov/ov_macros.h"


OV_DLLFNCEXPORT OV_INT ksapi_Channel_constate_get(
    OV_INSTPTR_ksapi_Channel          pobj
) {
    return pobj->v_constate;
}

OV_DLLFNCEXPORT OV_RESULT ksapi_Channel_constate_set(
    OV_INSTPTR_ksapi_Channel          pobj,
    const OV_INT  value
) {
    pobj->v_constate = value;
    return OV_ERR_OK;
}

/** 
 A redirector method for the calling the real sendxdr implemenation of the transportlevel.
*/ 
OV_DLLFNCEXPORT void ksapi_Channel_sendxdr(
		OV_INSTPTR_ksapi_Channel	 			pobj,
		OV_INSTPTR_ksapi_KSCommon	 			kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
	//this is a workaround for calling the overwritten method in subclasse-instances
	OV_VTBLPTR_ksapi_Channel pvtable = NULL;
	Ov_GetVTablePtr(ksapi_Channel, pvtable, pobj);

        pvtable->m_sendxdr(pobj,kscommon,xdr,xdrlength);
}

