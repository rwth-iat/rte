
/******************************************************************************
*
*   FILE
*   ----
*   uaInterface.c
*
*   History
*   -------
*   2019-02-26   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_ipsms
#define OV_COMPILE_LIBRARY_ipsms
#endif


#include "ipsms.h"
#include "libov/ov_macros.h"

#include "ipsms_trafo.h"
#include "opcua_helpers.h"

//OV_DLLFNCEXPORT OV_RESULT ipsms_uaInterface_entryPath_set(
//    OV_INSTPTR_ipsms_uaInterface          pobj,
//    const OV_STRING  value
//) {
//	return opcua_helpers_setRootEntryReference(value, Ov_StaticPtrCast(opcua_interface, pobj), &pobj->v_entryPath);
//}

OV_DLLFNCEXPORT OV_RESULT ipsms_uaInterface_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_ipsms_uaInterface pinst = Ov_StaticPtrCast(ipsms_uaInterface, pobj);
    OV_RESULT    result;

    /* do what the base class does first */
    result = ov_object_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */
    // Create the trafo
    pinst->v_trafo = Ov_HeapAlloc(OPCUA_InformationModel);
    pinst->v_trafo->dataTypes = NULL;
    pinst->v_trafo->index = 0;
    OV_STRING applicationUri = NULL;
    ov_string_setvalue(&applicationUri, OPCUA_DEFAULT_APPLICATIONURI);
    ov_string_append(&applicationUri, "ipsms"); //TODO move to own STRING input of opcua_interface (derived --> set to v_trafo->uri)
    pinst->v_trafo->uri = UA_String_fromChars(applicationUri);
    ov_string_setvalue(&applicationUri, NULL);
    pinst->v_trafo->store = ipsms_trafo_new(pinst);
    pinst->v_trafo->nodeset = NULL;

    // Create the types
    pinst->v_types = NULL;

    return OV_ERR_OK;
}

OV_DLLFNCEXPORT void ipsms_uaInterface_destructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_ipsms_uaInterface pinst = Ov_StaticPtrCast(ipsms_uaInterface, pobj);

    /* do what */
    // Delete trafo
    UA_String_deleteMembers(&pinst->v_trafo->uri);
    ipsms_trafo_delete(pinst->v_trafo->store);
    Ov_HeapFree(pinst->v_trafo);
    pinst->v_trafo = NULL;

    /* destroy object */
    ov_object_destructor(pobj);

    return;
}

//OV_DLLFNCEXPORT OV_RESULT ipsms_uaInterface_load(OV_INSTPTR_opcua_interface pobj, OV_BOOL forceLoad) {
//    /*
//    *   local variables
//    */
//    // Get server and instance pointer
//	OV_INSTPTR_ipsms_uaInterface pinst = Ov_StaticPtrCast(ipsms_uaInterface, pobj);
//	OV_INSTPTR_opcua_server server = Ov_GetParent(opcua_serverToInterfaces, pobj);
//	if(server == NULL){
//		return OV_ERR_GENERIC;
//	}
//
//	//TODO find and link necessary interfaces
//
//    // Use generic load method of uaInterface to load the trafos
//	opcua_interface_load(pobj, forceLoad);
//
////	// Add reference to OV root for ipsms interface
////	UA_StatusCode retval = UA_STATUSCODE_GOOD;
////	retval = UA_Server_addReference(server->v_server, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
////			UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_EXPANDEDNODEID_STRING(pobj->v_trafo->index, pinst->v_entryPath), true);
////	if(retval != UA_STATUSCODE_GOOD){
////		Ov_Warning(UA_StatusCode_name(retval));
////	}
//    return OV_ERR_OK;
//}

OV_DLLFNCEXPORT OV_BOOL ipsms_uaInterface_checkNodeId(OV_INSTPTR_opcua_interface pobj, OV_INSTPTR_ov_object pNode, UA_AddReferencesItem * parentRef) {
    /*
    *   local variables
    */
	if(Ov_CanCastTo(fb_controlchart, pNode)){
		if(parentRef){
			parentRef->isForward = TRUE; //TODO make default
			parentRef->referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
			//parentRef->sourceNodeId
			parentRef->targetNodeClass = UA_NODECLASS_OBJECT;
			ov_memstack_lock();
			parentRef->targetNodeId = UA_EXPANDEDNODEID_STRING_ALLOC(pobj->v_trafo->index, ov_path_getcanonicalpath(pNode,2));
			ov_memstack_unlock();
			//parentRef->targetServerUri
		}
		return TRUE;
	}
    return FALSE;
}
