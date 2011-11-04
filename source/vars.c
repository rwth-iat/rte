/** @file
 * test class for the setter getter FBComCommons.
 */
#ifndef OV_COMPILE_LIBRARY_fbcomlib
#define OV_COMPILE_LIBRARY_fbcomlib
#endif

#include "fbcomlib.h"
#include "libov/ov_macros.h"
#include "libov/ov_logfile.h"
#include "libov/ov_path.h"


OV_DLLFNCEXPORT OV_INT fbcomlib_vars_print_get(
    OV_INSTPTR_fbcomlib_vars          pobj
) {
    return pobj->v_print;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_vars_print_set(
    OV_INSTPTR_fbcomlib_vars          pobj,
    const OV_INT  value
) {
    pobj->v_print = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_BOOL fbcomlib_vars_bool_get(
    OV_INSTPTR_fbcomlib_vars          pobj
) {
    return pobj->v_bool;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_vars_bool_set(
    OV_INSTPTR_fbcomlib_vars          pobj,
    const OV_BOOL  value
) {
    pobj->v_bool = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_INT fbcomlib_vars_int_get(
    OV_INSTPTR_fbcomlib_vars          pobj
) {
    return pobj->v_int;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_vars_int_set(
    OV_INSTPTR_fbcomlib_vars          pobj,
    const OV_INT  value
) {
    pobj->v_int = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT fbcomlib_vars_uint_get(
    OV_INSTPTR_fbcomlib_vars          pobj
) {
    return pobj->v_uint;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_vars_uint_set(
    OV_INSTPTR_fbcomlib_vars          pobj,
    const OV_UINT  value
) {
    pobj->v_uint = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_SINGLE fbcomlib_vars_single_get(
    OV_INSTPTR_fbcomlib_vars          pobj
) {
    return pobj->v_single;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_vars_single_set(
    OV_INSTPTR_fbcomlib_vars          pobj,
    const OV_SINGLE  value
) {
    pobj->v_single = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_DOUBLE fbcomlib_vars_double_get(
    OV_INSTPTR_fbcomlib_vars          pobj
) {
    return pobj->v_double;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_vars_double_set(
    OV_INSTPTR_fbcomlib_vars          pobj,
    const OV_DOUBLE  value
) {
    pobj->v_double = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_STRING fbcomlib_vars_string_get(
    OV_INSTPTR_fbcomlib_vars          pobj
) {
    return pobj->v_string;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_vars_string_set(
    OV_INSTPTR_fbcomlib_vars          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_string,value);
}

OV_DLLFNCEXPORT OV_STRING fbcomlib_vars_tickettest_get(
    OV_INSTPTR_fbcomlib_vars          pobj
) {
    OV_INSTPTR_ksserv_Client client = NULL;
    OV_INSTPTR_ksserv_comcontainer comcontainer = (OV_INSTPTR_ksserv_comcontainer)ov_path_getobjectpointer("/communication", 2);
    //ticket stuff
    if(comcontainer == NULL) ov_logfile_error("fbcomlib_vars_tickettest_get: Couldnt find comcontainer - running without ksserv?");
    else {
    	client = Ov_GetFirstChild(ksserv_AssocCurrentClient, comcontainer);
    	if(client == NULL) {
    		ov_logfile_error("fbcomlib_vars_tickettest_get: Couldnt find currentclient - not implemented by this transport?");
    		return pobj->v_tickettest;
    	}
    	ov_logfile_info("fbcomlib_vars_tickettest_get: Used ticket is:%s", ksserv_Client_ovticket_get(client));
    }

    return pobj->v_tickettest;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_vars_tickettest_set(
    OV_INSTPTR_fbcomlib_vars          pobj,
    const OV_STRING  value
) {
    OV_INSTPTR_ksserv_Client client = NULL;
    OV_INSTPTR_ksserv_comcontainer comcontainer = (OV_INSTPTR_ksserv_comcontainer)ov_path_getobjectpointer("/communication", 2);

    //ticket stuff
    if(comcontainer == NULL) ov_logfile_error("fbcomlib_vars_tickettest_set: Couldnt find comcontainer - running without ksserv?");
    else {
    	client = Ov_GetFirstChild(ksserv_AssocCurrentClient, comcontainer);
    	if(client == NULL) {
    		ov_logfile_info("fbcomlib_vars_tickettest_set: Couldnt find currentclient - not implemented by this transport?");
    	} else {
    	  ov_logfile_error("fbcomlib_vars_tickettest_set: Used ticket is:%s", ksserv_Client_ovticket_get(client));
    	}
    }
    return ov_string_setvalue(&pobj->v_tickettest,value);
}


OV_DLLFNCEXPORT OV_BOOL* fbcomlib_vars_boolvec_get(
    OV_INSTPTR_fbcomlib_vars          pobj,
    OV_UINT *pveclen
) {
    *pveclen = 5;
    return pobj->v_boolvec;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_vars_boolvec_set(
    OV_INSTPTR_fbcomlib_vars          pobj,
    const OV_BOOL*  value,
    const OV_UINT veclen
) {
    return Ov_SetStaticVectorValue(pobj->v_boolvec,value,veclen,BOOL);
}

OV_DLLFNCEXPORT OV_INT* fbcomlib_vars_intvec_get(
    OV_INSTPTR_fbcomlib_vars          pobj,
    OV_UINT *pveclen
) {
    *pveclen = 5;
    return pobj->v_intvec;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_vars_intvec_set(
    OV_INSTPTR_fbcomlib_vars          pobj,
    const OV_INT*  value,
    const OV_UINT veclen
) {
    return Ov_SetStaticVectorValue(pobj->v_intvec,value,veclen,INT);
}

OV_DLLFNCEXPORT OV_UINT* fbcomlib_vars_uintvec_get(
    OV_INSTPTR_fbcomlib_vars          pobj,
    OV_UINT *pveclen
) {
    *pveclen = 5;
    return pobj->v_uintvec;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_vars_uintvec_set(
    OV_INSTPTR_fbcomlib_vars          pobj,
    const OV_UINT*  value,
    const OV_UINT veclen
) {
    return Ov_SetStaticVectorValue(pobj->v_uintvec,value,veclen,UINT);
}

OV_DLLFNCEXPORT OV_SINGLE* fbcomlib_vars_singlevec_get(
    OV_INSTPTR_fbcomlib_vars          pobj,
    OV_UINT *pveclen
) {
    *pveclen = 5;
    return pobj->v_singlevec;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_vars_singlevec_set(
    OV_INSTPTR_fbcomlib_vars          pobj,
    const OV_SINGLE*  value,
    const OV_UINT veclen
) {
    return Ov_SetStaticVectorValue(pobj->v_singlevec,value,veclen,SINGLE);
}

OV_DLLFNCEXPORT OV_DOUBLE* fbcomlib_vars_doublevec_get(
    OV_INSTPTR_fbcomlib_vars          pobj,
    OV_UINT *pveclen
) {
    *pveclen = 5;
    return pobj->v_doublevec;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_vars_doublevec_set(
    OV_INSTPTR_fbcomlib_vars          pobj,
    const OV_DOUBLE*  value,
    const OV_UINT veclen
) {
    return Ov_SetStaticVectorValue(pobj->v_doublevec,value,veclen,DOUBLE);
}

OV_DLLFNCEXPORT OV_STRING* fbcomlib_vars_stringvec_get(
    OV_INSTPTR_fbcomlib_vars          pobj,
    OV_UINT *pveclen
) {
    *pveclen = 5;
    return pobj->v_stringvec;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_vars_stringvec_set(
    OV_INSTPTR_fbcomlib_vars          pobj,
    const OV_STRING*  value,
    const OV_UINT veclen
) {
    return Ov_SetStaticVectorValue(pobj->v_stringvec,value,veclen,STRING);
}

OV_DLLFNCEXPORT void fbcomlib_vars_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    
    /* do what the base class does first */
    ov_object_startup(pobj);

    return;
}

OV_DLLFNCEXPORT void fbcomlib_vars_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {

    /* set the object's state to "shut down" */
    ov_object_shutdown(pobj);

    return;
}
