/* * nodeStoreInterface.c
 *
 *  Created on: 06.12.2016
 *      Author: ubuntu
 */

#ifndef OV_COMPILE_LIBRARY_lifeCycleEntryOPCUAInterface
#define OV_COMPILE_LIBRARY_lifeCycleEntryOPCUAInterface
#endif

#include "lifeCycleEntryOPCUAInterface.h"
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
#include "ua_lifeCycleEntry_generated.h"
#include "ua_lifeCycleEntry_generated_handling.h"
#include "lifeCycleEntry_helpers.h"

static void OV_NodeStore_deleteNodestore(void *handle, UA_UInt16 namespaceIndex){

}

static void OV_NodeStore_deleteNode(UA_Node *node){
	if (node){
		OV_STRING tmpString = NULL;
		if (node->nodeId.identifierType == UA_NODEIDTYPE_STRING)
			copyOPCUAStringToOV(node->nodeId.identifier.string, &tmpString);
		else
			ov_string_print(&tmpString, "%i", node->nodeId.identifier.numeric);

		ov_logfile_error("Delete Node: %s", tmpString);
		ov_string_setvalue(&tmpString, NULL);
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
	OV_STRING tmpString = NULL;
	OV_INSTPTR_ov_object pobj = NULL;
	if (nodeId->identifier.string.data == NULL || nodeId->identifier.string.length == 0 || nodeId->identifierType != UA_NODEIDTYPE_STRING)
		return NULL;
	copyOPCUAStringToOV(nodeId->identifier.string, &tmpString);

	pobj = ov_path_getobjectpointer(tmpString, 2);
	if (pobj == NULL){
		ov_string_setvalue(&tmpString, NULL);
		return NULL;
	}


	if(Ov_CanCastTo(lifeCycleEntry_LifeCycleArchive, pobj)){
		if (lifeCycleEntryOPCUAInterface_interface_ovLifeCycleArchiveNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			tmpNode = opcuaNode;
	}else if(Ov_CanCastTo(lifeCycleEntry_LifeCycleEntry, pobj)){
		if (lifeCycleEntryOPCUAInterface_interface_ovLifeCycleEntryNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			tmpNode = opcuaNode;
	}

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

	if (!pobj){
		if (Ov_CanCastTo(lifeCycleEntry_LifeCycleEntry, pobj)){ // LCE
			OV_ELEMENT tmpPart;
			tmpPart.elemtype = OV_ET_NONE;
			OV_ELEMENT tmpParrent;
			tmpParrent.pobj = pobj;
			tmpParrent.elemtype = OV_ET_OBJECT;
			UA_LifeCycleEntry *tmpLifeCycleEntry = ((UA_LifeCycleEntry*)(((UA_Variant*)&((UA_VariableNode*)node)->value.data.value.value)->data));
			do {
				ov_element_getnextpart(&tmpParrent, &tmpPart, OV_ET_VARIABLE);
				if (tmpPart.elemtype == OV_ET_NONE)
					break;
				if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "CreatingInstanceString") == OV_STRCMP_EQUAL)
					copyOPCUAStringToOV(tmpLifeCycleEntry->creatingInstance.idSpec, (OV_STRING*)tmpPart.pvalue);
				if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "CreatingInstanceType") == OV_STRCMP_EQUAL)
					*(UA_UInt32*)tmpPart.pvalue = tmpLifeCycleEntry->creatingInstance.idType;
				if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "WritingInstanceString") == OV_STRCMP_EQUAL)
					copyOPCUAStringToOV(tmpLifeCycleEntry->writingInstance.idSpec, (OV_STRING*)tmpPart.pvalue);
				if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "WritingInstanceType") == OV_STRCMP_EQUAL)
					*(UA_UInt32*)tmpPart.pvalue = tmpLifeCycleEntry->writingInstance.idType;
				if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "Data") == OV_STRCMP_EQUAL)
					ov_VariantToAny(&tmpLifeCycleEntry->data.value, (OV_ANY*)tmpPart.pvalue);
				if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "TimeStamp") == OV_STRCMP_EQUAL)
					*(UA_DateTime*)tmpPart.pvalue = tmpLifeCycleEntry->data.sourceTimestamp;
				if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "Subject") == OV_STRCMP_EQUAL)
					copyOPCUAStringToOV(tmpLifeCycleEntry->subject, (OV_STRING*)tmpPart.pvalue);
				if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "EventClass") == OV_STRCMP_EQUAL)
					copyOPCUAStringToOV(tmpLifeCycleEntry->eventClass, (OV_STRING*)tmpPart.pvalue);
			} while(TRUE);
		}
	}

	return UA_STATUSCODE_GOOD;
}
static void OV_NodeStore_iterate(void *handle, void* visitorHandle, UA_NodestoreInterface_nodeVisitor visitor){

}
static UA_StatusCode OV_NodeStore_linkNamespace(void *handle, UA_UInt16 namespaceIndex){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}
static UA_StatusCode OV_NodeStore_unlinkNamespace(void *handle, UA_UInt16 namespaceIndex){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

UA_NodestoreInterface* lifeCycleEntryOPCUAInterface_interface_ovNodeStoreInterfaceLifeCycleEntryNew(void) {
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

void lifeCycleEntryOPCUAInterface_interface_ovNodeStoreInterfaceLifeCycleEntryDelete(UA_NodestoreInterface * nsi){
	if (nsi->handle)
		UA_free(nsi->handle);
}
