/* * nodeStoreInterface.c
 *
 *  Created on: 06.12.2016
 *      Author: ubuntu
 */

#ifndef OV_COMPILE_LIBRARY_propertyValueStatementOPCUAInterface
#define OV_COMPILE_LIBRARY_propertyValueStatementOPCUAInterface
#endif

#include "propertyValueStatementOPCUAInterface.h"
#include "libov/ov_macros.h"
#include "ksbase.h"
#include "opcua.h"
#include "opcua_helpers.h"
#include "NoneTicketAuthenticator.h"
#include "libov/ov_path.h"
#include "libov/ov_memstack.h"
#include "ks_logfile.h"
#include "ua_identification_generated.h"
#include "ua_identification_generated_handling.h"
#include "ua_propertyValueStatement_generated.h"
#include "ua_propertyValueStatement_generated_handling.h"


static void OV_NodeStore_deleteNodestore(void *handle, UA_UInt16 namespaceIndex){

}

static void OV_NodeStore_deleteNode(UA_Node *node){
	if (node){
		if (node->nodeClass == UA_NODECLASS_METHOD){
			ov_string_setvalue((OV_STRING*) (&(((UA_MethodNode*) node)->methodHandle)),	NULL);
		}
		UA_Node_deleteMembersAnyNodeClass(node);
	}
	UA_free(node);
}
static void OV_NodeStore_releaseNode(void *handle, const UA_Node *node){
	OV_NodeStore_deleteNode((UA_Node*)node);
}

static UA_Node * OV_NodeStore_newNode(UA_NodeClass nodeClass){ //TODO add nodestore handle? --> move nodeStore from static context to main
    //allocate memory for a new node
	//UA_Node *newNode = NULL;
	//newNode = (UA_Node*) ov_database_malloc(sizeof(UA_ObjectNode));
	//newNode->nodeClass = nodeClass;
    return NULL;
}
static const UA_Node * OV_NodeStore_getNode(void *handle, const UA_NodeId *nodeId){
	UA_Node * tmpNode = NULL;
	UA_Node* opcuaNode = NULL;
	OV_STRING *plist = NULL;
	OV_STRING *plist2 = NULL;
	OV_STRING tmpString = NULL;
	OV_INSTPTR_ov_object pobj = NULL;
	OV_UINT len = 0;
	OV_UINT len2 = 0;

	if (nodeId->identifier.string.data == NULL || nodeId->identifier.string.length == 0 || nodeId->identifierType != UA_NODEIDTYPE_STRING)
		return NULL;
	copyOPCUAStringToOV(nodeId->identifier.string, &tmpString);
	plist = ov_string_split(tmpString, "|", &len);
	plist2 = ov_string_split(tmpString, ".", &len2);


	if (len > 1){
		pobj = ov_path_getobjectpointer(plist[0], 2);
		if (pobj == NULL){
			ov_string_freelist(plist);
			ov_string_freelist(plist2);
			ov_string_setvalue(&tmpString, NULL);
			return NULL;
		}
	}else{
		pobj = ov_path_getobjectpointer(tmpString, 2);
		if (pobj == NULL){
			ov_string_freelist(plist);
			ov_string_freelist(plist2);
			ov_string_setvalue(&tmpString, NULL);
			return NULL;
		}
	}

	if(Ov_CanCastTo(propertyValueStatement_PropertyValueStatementList, pobj)){
		if (propertyValueStatementOPCUAInterface_interface_ovPropertyValueStatementListNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			tmpNode = opcuaNode;
	}else if(Ov_CanCastTo(propertyValueStatement_PropertyValueStatement, pobj)){
		if (propertyValueStatementOPCUAInterface_interface_ovPropertyValueStatementNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			tmpNode = opcuaNode;
	}else if (Ov_CanCastTo(propertyValueStatement_PropertyId, pobj) || Ov_CanCastTo(propertyValueStatement_CarrierId, pobj)){
		if (propertyValueStatementOPCUAInterface_interface_ovPropertyIdCarrierIdNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			tmpNode = opcuaNode;
	}else if (Ov_CanCastTo(propertyValueStatement_ExpressionLogic, pobj)){
		if (propertyValueStatementOPCUAInterface_interface_ovExpressionLogicNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			tmpNode = opcuaNode;
	}else if (Ov_CanCastTo(propertyValueStatement_ExpressionSemantic, pobj)){
		if (propertyValueStatementOPCUAInterface_interface_ovExpressionSemanticNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			tmpNode = opcuaNode;
	}else if (Ov_CanCastTo(propertyValueStatement_View, pobj)){
		if (propertyValueStatementOPCUAInterface_interface_ovViewNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			tmpNode = opcuaNode;
	}else if (Ov_CanCastTo(propertyValueStatement_Visibility, pobj)){
		if (propertyValueStatementOPCUAInterface_interface_ovVisibilityNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			tmpNode = opcuaNode;
	}

	ov_string_freelist(plist);
	ov_string_freelist(plist2);
	ov_string_setvalue(&tmpString, NULL);
	return tmpNode;
}
static UA_Node * OV_NodeStore_getCopyNode(void *handle, const UA_NodeId *nodeId){
	return (UA_Node*)OV_NodeStore_getNode(handle, nodeId);
}
static UA_StatusCode OV_NodeStore_removeNode(void *handle, const UA_NodeId *nodeId){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
	//OV_INSTPTR_ov_object pobj = opcua_nodeStoreFunctions_resolveNodeIdToOvObject((UA_NodeId*)nodeId);
	//if (pobj != NULL){
	//	Ov_DeleteObject(pobj);
	//}
    //return UA_STATUSCODE_GOOD;
}
static UA_StatusCode OV_NodeStore_insertNode(void *handle, UA_Node *node, UA_NodeId *parrentNode){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
	//OV_INSTPTR_ov_object pobj = opcua_nodeStoreFunctions_resolveNodeIdToOvObject(&(node->nodeId));
	//if (pobj != NULL)
	//	return UA_STATUSCODE_BADNODEIDEXISTS;

	//return OV_NodeStore_insert(handle, node, parrentNode);
}
static UA_StatusCode OV_NodeStore_replaceNode(void *handle, UA_Node *node){
	UA_StatusCode 			result = UA_STATUSCODE_GOOD;
	OV_PATH 				path;
	OV_INSTPTR_ov_object	pobj = NULL;
	OV_TICKET 				*pTicket = NULL;
	OV_VTBLPTR_ov_object	pVtblObj = NULL;
	OV_ACCESS				access;

	ov_memstack_lock();
	result = opcua_nodeStoreFunctions_resolveNodeIdToPath(node->nodeId, &path);
	if(result != UA_STATUSCODE_GOOD){
		ov_memstack_unlock();
		return result;
	}
	result = opcua_nodeStoreFunctions_getVtblPointerAndCheckAccess(&(path.elements[path.size-1]), pTicket, &pobj, &pVtblObj, &access);
	ov_memstack_unlock();
	if(result != UA_STATUSCODE_GOOD){
		return result;
	}

	if (pobj){
		if (Ov_CanCastTo(propertyValueStatement_PropertyValueStatement, pobj)){ // PVS
			OV_ELEMENT tmpPart;
			tmpPart.elemtype = OV_ET_NONE;
			OV_ELEMENT tmpParrent;
			tmpParrent.pobj = pobj;
			tmpParrent.elemtype = OV_ET_OBJECT;
			do {
				ov_element_getnextpart(&tmpParrent, &tmpPart, OV_ET_VARIABLE);
				if (tmpPart.elemtype == OV_ET_NONE)
					break;
				if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "Value") == OV_STRCMP_EQUAL)
					ov_VariantToAny(((UA_Variant*)&((UA_VariableNode*)node)->value.data.value.value), (OV_ANY*)tmpPart.pvalue);
			} while(TRUE);
		}
	}
	return result;
}
static void OV_NodeStore_iterate(void *handle, void* visitorHandle, UA_NodestoreInterface_nodeVisitor visitor){

}
static UA_StatusCode OV_NodeStore_linkNamespace(void *handle, UA_UInt16 namespaceIndex){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}
static UA_StatusCode OV_NodeStore_unlinkNamespace(void *handle, UA_UInt16 namespaceIndex){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

UA_NodestoreInterface* propertyValueStatementOPCUAInterface_interface_ovNodeStoreInterfacePropertyValueStatementNew(void) {
	UA_NodestoreInterface *nsi = ov_database_malloc(sizeof(UA_NodestoreInterface));
    nsi->handle =        	NULL;
    nsi->deleteNodestore =  (UA_NodestoreInterface_deleteNodeStore) 		OV_NodeStore_deleteNodestore;
    nsi->newNode =       	(UA_NodestoreInterface_newNode)     OV_NodeStore_newNode;
    nsi->deleteNode =    	(UA_NodestoreInterface_deleteNode)  OV_NodeStore_deleteNode;
    nsi->insertNode =       	(UA_NodestoreInterface_insertNode)      OV_NodeStore_insertNode;
    nsi->getNode =          	(UA_NodestoreInterface_getNode)         OV_NodeStore_getNode;
    nsi->getNodeCopy =      	(UA_NodestoreInterface_getNodeCopy)     OV_NodeStore_getCopyNode;
    nsi->replaceNode =      	(UA_NodestoreInterface_replaceNode)     OV_NodeStore_replaceNode;
    nsi->removeNode =       	(UA_NodestoreInterface_removeNode)      OV_NodeStore_removeNode;
    nsi->iterate =       (UA_NodestoreInterface_iterate)     OV_NodeStore_iterate;
    nsi->releaseNode =       	(UA_NodestoreInterface_releaseNode)     OV_NodeStore_releaseNode;
    nsi->linkNamespace =       (UA_NodestoreInterface_linkNamespace)     OV_NodeStore_linkNamespace;
	nsi->unlinkNamespace =       	(UA_NodestoreInterface_unlinkNamespace)     OV_NodeStore_unlinkNamespace;
	return nsi;
}

void propertyValueStatementOPCUAInterface_interface_ovNodeStoreInterfacePropertyValueStatementDelete(UA_NodestoreInterface * nsi){
	if (nsi->handle)
		UA_free(nsi->handle);
}
