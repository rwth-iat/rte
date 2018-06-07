
/******************************************************************************
*
*   FILE
*   ----
*   uaServer.c
*
*   History
*   -------
*   2018-06-04   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_opcua
#define OV_COMPILE_LIBRARY_opcua
#endif


#include "opcua.h"
#include "libov/ov_macros.h"


OV_DLLFNCEXPORT OV_BOOL opcua_uaServer_LoginEnableAnonymous_get(
    OV_INSTPTR_opcua_uaServer          pobj
) {
    return pobj->v_LoginEnableAnonymous;
}

OV_DLLFNCEXPORT OV_RESULT opcua_uaServer_LoginEnableAnonymous_set(
    OV_INSTPTR_opcua_uaServer          pobj,
    const OV_BOOL  value
) {
    pobj->v_LoginEnableAnonymous = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_BOOL opcua_uaServer_LoginEnableUsernamePassword_get(
    OV_INSTPTR_opcua_uaServer          pobj
) {
    return pobj->v_LoginEnableUsernamePassword;
}

OV_DLLFNCEXPORT OV_RESULT opcua_uaServer_LoginEnableUsernamePassword_set(
    OV_INSTPTR_opcua_uaServer          pobj,
    const OV_BOOL  value
) {
    pobj->v_LoginEnableUsernamePassword = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_STRING* opcua_uaServer_LoginUsernames_get(
    OV_INSTPTR_opcua_uaServer          pobj,
    OV_UINT *pveclen
) {
    *pveclen = pobj->v_LoginUsernames.veclen;
    return pobj->v_LoginUsernames.value;
}

OV_DLLFNCEXPORT OV_RESULT opcua_uaServer_LoginUsernames_set(
    OV_INSTPTR_opcua_uaServer          pobj,
    const OV_STRING*  value,
    const OV_UINT veclen
) {
    return Ov_SetDynamicVectorValue(&pobj->v_LoginUsernames,value,veclen,STRING);
}

OV_DLLFNCEXPORT OV_STRING* opcua_uaServer_LoginPasswords_get(
    OV_INSTPTR_opcua_uaServer          pobj,
    OV_UINT *pveclen
) {
    *pveclen = pobj->v_LoginPasswords.veclen;
    return pobj->v_LoginPasswords.value;
}

OV_DLLFNCEXPORT OV_RESULT opcua_uaServer_LoginPasswords_set(
    OV_INSTPTR_opcua_uaServer          pobj,
    const OV_STRING*  value,
    const OV_UINT veclen
) {
    return Ov_SetDynamicVectorValue(&pobj->v_LoginPasswords,value,veclen,STRING);
}

OV_DLLFNCEXPORT OV_UINT opcua_uaServer_LoginLoginsCount_get(
    OV_INSTPTR_opcua_uaServer          pobj
) {
    return (OV_UINT)0;
}

OV_DLLFNCEXPORT OV_STRING opcua_uaServer_ApplicationURI_get(
    OV_INSTPTR_opcua_uaServer          pobj
) {
    return pobj->v_ApplicationURI;
}

OV_DLLFNCEXPORT OV_RESULT opcua_uaServer_ApplicationURI_set(
    OV_INSTPTR_opcua_uaServer          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_ApplicationURI,value);
}

OV_DLLFNCEXPORT OV_STRING opcua_uaServer_ApplicationName_get(
    OV_INSTPTR_opcua_uaServer          pobj
) {
    return pobj->v_ApplicationName;
}

OV_DLLFNCEXPORT OV_RESULT opcua_uaServer_ApplicationName_set(
    OV_INSTPTR_opcua_uaServer          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_ApplicationName,value);
}

OV_DLLFNCEXPORT OV_RESULT opcua_uaServer_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_opcua_uaServer pinst = Ov_StaticPtrCast(opcua_uaServer, pobj);
    OV_RESULT    result;

    /* do what the base class does first */
    result = ksbase_ComTask_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */


    return OV_ERR_OK;
}

OV_DLLFNCEXPORT void opcua_uaServer_destructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_opcua_uaServer pinst = Ov_StaticPtrCast(opcua_uaServer, pobj);

    /* do what */

    /* destroy object */
    ksbase_ComTask_destructor(pobj);

    return;
}

OV_DLLFNCEXPORT void opcua_uaServer_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_opcua_uaServer pinst = Ov_StaticPtrCast(opcua_uaServer, pobj);

    /* do what the base class does first */
    ov_object_startup(pobj);

    /* do what */


    return;
}

OV_DLLFNCEXPORT void opcua_uaServer_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_opcua_uaServer pinst = Ov_StaticPtrCast(opcua_uaServer, pobj);

    /* do what */

    /* set the object's state to "shut down" */
    ov_object_shutdown(pobj);

    return;
}

OV_DLLFNCEXPORT OV_ACCESS opcua_uaServer_getaccess(
	OV_INSTPTR_ov_object	pobj,
	const OV_ELEMENT		*pelem,
	const OV_TICKET			*pticket
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_opcua_uaServer pinst = Ov_StaticPtrCast(opcua_uaServer, pobj);

    return (OV_ACCESS)0;
}

OV_DLLFNCEXPORT void opcua_uaServer_typemethod (
	OV_INSTPTR_ksbase_ComTask	this
) {
    /*    
    *   local variables
    */

    return;
}

OV_DLLFNCEXPORT UA_StatusCode opcua_uaServer_addInformationModel(UA_Namespace **namespace, OV_UINT namespaceSize, OV_STRING *StartFolder, opcua_loadInformationModel *loadInfoModel) {

    return (UA_StatusCode)0;
}

