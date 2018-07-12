
/******************************************************************************
*
*   FILE
*   ----
*   nodeStoreFunctions.c
*
*   History
*   -------
*   2018-06-28   File created
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


OV_DLLFNCEXPORT UA_Nodestore* opcua_nodeStoreFunctions_ovNodeStoreInterface2New();
typedef void UA_FNC_OVNODESTOREINTERFACE2DELETE(UA_Nodestore * nodestoreInterface) {

    return (UA_Nodestore*)0;
}

OV_DLLFNCEXPORT void opcua_nodeStoreFunctions_ovNodeStoreInterface2Delete(UA_Nodestore * nodestoreInterface) {

    return;
}

OV_DLLFNCEXPORT UA_Int32 opcua_nodeStoreFunctions_resolveNodeIdToPath(UA_NodeId nodeId, OV_PATH* pPath) {

    return (UA_Int32)0;
}

OV_DLLFNCEXPORT OV_INSTPTR_ov_object opcua_nodeStoreFunctions_resolveNodeIdToOvObject(UA_NodeId *nodeId) {

    return (OV_INSTPTR_ov_object)0;
}

OV_DLLFNCEXPORT UA_Int32 opcua_nodeStoreFunctions_getVtblPointerAndCheckAccess(OV_ELEMENT *pelem, OV_TICKET* pTicket, OV_INSTPTR_ov_object *pInstance, OV_VTBLPTR_ov_object *ppVtblObj, OV_ACCESS *access) {

    return (UA_Int32)0;
}

