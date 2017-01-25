/*
 * nodeStoreInterface.c
 *
 *  Created on: 06.12.2016
 *      Author: ubuntu
 */

#ifndef OV_COMPILE_LIBRARY_opcua
#define OV_COMPILE_LIBRARY_opcua
#endif

#include "libov/ov_macros.h"
#include "ksbase.h"
#include "opcua.h"
#include "opcua_helpers.h"
#include "NoneTicketAuthenticator.h"
#include "libov/ov_path.h"
#include "libov/ov_memstack.h"
#include "ks_logfile.h"




static void OV_NodeStore_deleteNodestore(void *handle){

}

static void OV_NodeStore_deleteNode(UA_Node *node){
	ov_database_free(node);
}
static void OV_NodeStore_releaseNode(UA_Node *node){
	OV_NodeStore_deleteNode(node);
}

static UA_StatusCode OV_NodeStore_insert(void *handle, UA_Node *node, UA_NodeId *parrentNode){
	UA_StatusCode result = UA_STATUSCODE_GOOD;
	switch(node->nodeClass){
	case UA_NODECLASS_UNSPECIFIED:
		break;
	case UA_NODECLASS_OBJECT:{
		OV_INSTPTR_opcua_objectNode ovNode = NULL;
		result =opcua_nodeStoreFunctions_opcuaObjectNodeToOv(handle, (UA_ObjectNode*)node, &ovNode, parrentNode);
		break;
	}
	case UA_NODECLASS_VARIABLE:{
		OV_INSTPTR_opcua_variableNode ovNode = NULL;
		return opcua_nodeStoreFunctions_opcuaVariableNodeToOv(handle, (UA_VariableNode*)node, &ovNode, parrentNode);
	}
	case UA_NODECLASS_METHOD:{
		OV_INSTPTR_opcua_methodNode ovNode = NULL;
		result = opcua_nodeStoreFunctions_opcuaMethodNodeToOv(handle, (UA_MethodNode*)node, &ovNode, parrentNode);
		break;
	}
	case UA_NODECLASS_OBJECTTYPE:{
		OV_INSTPTR_opcua_objectTypeNode ovNode = NULL;
		result = opcua_nodeStoreFunctions_opcuaObjectTypeNodeToOv(handle, (UA_ObjectTypeNode*)node, &ovNode, parrentNode);
		break;
	}
	case UA_NODECLASS_VARIABLETYPE:{
		OV_INSTPTR_opcua_variableTypeNode ovNode = NULL;
		result = opcua_nodeStoreFunctions_opcuaVariableTypeNodeToOv(handle, (UA_VariableTypeNode*)node, &ovNode, parrentNode);
		break;
	}
	case UA_NODECLASS_REFERENCETYPE:{
		OV_INSTPTR_opcua_referenceTypeNode ovNode = NULL;
		result = opcua_nodeStoreFunctions_opcuaReferenceTypeNodeToOv(handle, (UA_ReferenceTypeNode*)node, &ovNode, parrentNode);
		break;
	}
	case UA_NODECLASS_DATATYPE:{
		OV_INSTPTR_opcua_dataTypeNode ovNode = NULL;
		result = opcua_nodeStoreFunctions_opcuaDataTypeNodeToOv(handle, (UA_DataTypeNode*)node, &ovNode, parrentNode);
		break;
	}
	case UA_NODECLASS_VIEW:{
		OV_INSTPTR_opcua_viewNode ovNode = NULL;
		result = opcua_nodeStoreFunctions_opcuaViewNodeToOv(handle, (UA_ViewNode*)node, &ovNode, parrentNode);
		break;
	}
	default:
		break;
	}
	OV_NodeStore_releaseNode(node);
	return result;
}

static UA_Node * OV_NodeStore_newNode(UA_NodeClass nodeClass){ //TODO add nodestore handle? --> move nodeStore from static context to main
    //allocate memory for a new node
	UA_Node *newNode = NULL;
	switch(nodeClass){
	case UA_NODECLASS_UNSPECIFIED:
		return NULL;
	case UA_NODECLASS_OBJECT:
		newNode = (UA_Node*) ov_database_malloc(sizeof(UA_ObjectNode));
		break;
	case UA_NODECLASS_VARIABLE:
		newNode = (UA_Node*) ov_database_malloc(sizeof(UA_VariableNode));
		break;
	case UA_NODECLASS_METHOD:
		newNode = (UA_Node*) ov_database_malloc(sizeof(UA_MethodNode));
		break;
	case UA_NODECLASS_OBJECTTYPE:
		newNode = (UA_Node*) ov_database_malloc(sizeof(UA_ObjectTypeNode));
		break;
	case UA_NODECLASS_VARIABLETYPE:
		newNode = (UA_Node*) ov_database_malloc(sizeof(UA_VariableTypeNode));
		break;
	case UA_NODECLASS_REFERENCETYPE:
		newNode = (UA_Node*) ov_database_malloc(sizeof(UA_ReferenceTypeNode));
		break;
	case UA_NODECLASS_DATATYPE:
		newNode = (UA_Node*) ov_database_malloc(sizeof(UA_DataTypeNode));
		break;
	case UA_NODECLASS_VIEW:
		newNode = (UA_Node*) ov_database_malloc(sizeof(UA_ViewNode));
		break;
	default:
		return NULL;
	}
	newNode->nodeClass = nodeClass;
    return newNode;
}

static const UA_Node * OV_NodeStore_getNode(void *handle, const UA_NodeId *nodeId){
	OV_INSTPTR_ov_object pobj = opcua_nodeStoreFunctions_resolveNodeIdToOvObject((UA_NodeId*)nodeId);
	if (pobj == NULL)
		return NULL;
	if(Ov_CanCastTo(opcua_variableNode, pobj)){
		UA_VariableNode* opcuaNode = NULL;
		OV_INSTPTR_opcua_variableNode ovNode = Ov_DynamicPtrCast(opcua_variableNode, pobj);
		if (opcua_nodeStoreFunctions_ovVariableNodeToOPCUA(NULL, ovNode, &opcuaNode) == UA_STATUSCODE_GOOD)
			return (UA_Node*)opcuaNode;
	}else if(Ov_CanCastTo(opcua_variableTypeNode, pobj)){
		UA_VariableTypeNode* opcuaNode = NULL;
		OV_INSTPTR_opcua_variableTypeNode ovNode = Ov_DynamicPtrCast(opcua_variableTypeNode, pobj);
		if (opcua_nodeStoreFunctions_ovVariableTypeNodeToOPCUA(NULL, ovNode, &opcuaNode) == UA_STATUSCODE_GOOD)
			return (UA_Node*) opcuaNode;
	}else if(Ov_CanCastTo(opcua_objectNode, pobj)){
		UA_ObjectNode* opcuaNode = NULL;
		OV_INSTPTR_opcua_objectNode ovNode = Ov_DynamicPtrCast(opcua_objectNode, pobj);
		if (opcua_nodeStoreFunctions_ovObjectNodeToOPCUA(NULL, ovNode, &opcuaNode) == UA_STATUSCODE_GOOD)
			return (UA_Node*) opcuaNode;
	}else if(Ov_CanCastTo(opcua_objectTypeNode, pobj)){
		UA_ObjectTypeNode* opcuaNode = NULL;
		OV_INSTPTR_opcua_objectTypeNode ovNode = Ov_DynamicPtrCast(opcua_objectTypeNode, pobj);
		if (opcua_nodeStoreFunctions_ovObjectTypeNodeToOPCUA(NULL, ovNode, &opcuaNode) == UA_STATUSCODE_GOOD)
			return (UA_Node*) opcuaNode;
	}else if(Ov_CanCastTo(opcua_referenceTypeNode, pobj)){
		UA_ReferenceTypeNode* opcuaNode = NULL;
		OV_INSTPTR_opcua_referenceTypeNode ovNode = Ov_DynamicPtrCast(opcua_referenceTypeNode, pobj);
		if (opcua_nodeStoreFunctions_ovReferenceTypeNodeToOPCUA(NULL, ovNode, &opcuaNode) == UA_STATUSCODE_GOOD)
			return (UA_Node*) opcuaNode;
	}else if(Ov_CanCastTo(opcua_dataTypeNode, pobj)){
		UA_DataTypeNode* opcuaNode = NULL;
		OV_INSTPTR_opcua_dataTypeNode ovNode = Ov_DynamicPtrCast(opcua_dataTypeNode, pobj);
		if (opcua_nodeStoreFunctions_ovDataTypeNodeToOPCUA(NULL, ovNode, &opcuaNode) == UA_STATUSCODE_GOOD)
			return (UA_Node*) opcuaNode;
	}else if(Ov_CanCastTo(opcua_viewNode, pobj)){
		UA_ViewNode* opcuaNode = NULL;
		OV_INSTPTR_opcua_viewNode ovNode = Ov_DynamicPtrCast(opcua_viewNode, pobj);
		if (opcua_nodeStoreFunctions_ovViewNodeToOPCUA(NULL, ovNode, &opcuaNode) == UA_STATUSCODE_GOOD)
			return (UA_Node*) opcuaNode;
	}else if(Ov_CanCastTo(opcua_methodNode, pobj)){
		UA_MethodNode* opcuaNode = NULL;
		OV_INSTPTR_opcua_methodNode ovNode = Ov_DynamicPtrCast(opcua_methodNode, pobj);
		if (opcua_nodeStoreFunctions_ovMethodNodeToOPCUA(NULL, ovNode, &opcuaNode) == UA_STATUSCODE_GOOD)
			return (UA_Node*) opcuaNode;
	}else{
		return NULL;
	}
	return NULL;
}
static UA_Node * OV_NodeStore_getCopyNode(void *handle, const UA_NodeId *nodeId){
	return (UA_Node*)OV_NodeStore_getNode(handle, nodeId);
}
static UA_StatusCode OV_NodeStore_removeNode(void *handle, const UA_NodeId *nodeId){
	OV_INSTPTR_ov_object pobj = opcua_nodeStoreFunctions_resolveNodeIdToOvObject((UA_NodeId*)nodeId);
	if (pobj != NULL){
		Ov_DeleteObject(pobj);
	}
    return UA_STATUSCODE_GOOD;
}
static UA_StatusCode OV_NodeStore_insertNode(void *handle, UA_Node *node, UA_NodeId *parrentNode){
	OV_INSTPTR_ov_object pobj = opcua_nodeStoreFunctions_resolveNodeIdToOvObject(&(node->nodeId));
	if (pobj != NULL)
		return UA_STATUSCODE_BADNODEIDEXISTS;

	return OV_NodeStore_insert(handle, node, parrentNode);
}
static UA_StatusCode OV_NodeStore_replaceNode(void *handle, UA_Node *node){
	UA_StatusCode result = 0;
	// TODO: Lookup for parrent to insert the new node at the right place
	result = OV_NodeStore_removeNode(handle, &node->nodeId);
	if (result != UA_STATUSCODE_GOOD)
		return result;
	return OV_NodeStore_insert(handle, node, NULL);
}

UA_NodestoreInterface*
opcua_nodeStoreFunctions_ovNodeStoreInterfaceNew(void) {
	UA_NodestoreInterface *nsi = ov_database_malloc(sizeof(UA_NodestoreInterface));
    nsi->handle =        	NULL;
    nsi->deleteNodeStore =  (UA_NodestoreInterface_delete) 		OV_NodeStore_deleteNodestore;
    nsi->newNode =       	(UA_NodestoreInterface_newNode)     OV_NodeStore_newNode;
    nsi->deleteNode =    	(UA_NodestoreInterface_deleteNode)  OV_NodeStore_deleteNode;
    nsi->insert =       	(UA_NodestoreInterface_insert)      OV_NodeStore_insertNode;
    nsi->get =          	(UA_NodestoreInterface_get)         OV_NodeStore_getNode;
    nsi->getCopy =      	(UA_NodestoreInterface_getCopy)     OV_NodeStore_getCopyNode;
    nsi->replace =      	(UA_NodestoreInterface_replace)     OV_NodeStore_replaceNode;
    nsi->remove =       	(UA_NodestoreInterface_remove)      OV_NodeStore_removeNode;
    //nsi->iterateNode =       (UA_NodestoreInterface_iterateNode)     OV_NodeStore_iterateNode;
    nsi->release =       	(UA_NodestoreInterface_release)     OV_NodeStore_releaseNode;
    return nsi;
}
void
opcua_nodeStoreFunctions_ovNodeStoreInterfaceDelete(UA_NodestoreInterface * nodestoreInterface){
    UA_free(nodestoreInterface->handle);
    ov_database_free(nodestoreInterface);
}
