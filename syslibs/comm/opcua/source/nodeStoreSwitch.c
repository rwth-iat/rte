/*
 * nodeStoreSwitch.c
 *
 *  Created on: 21.06.2018
 *      Author: vitus
 */

#ifndef OV_COMPILE_LIBRARY_opcua
#define OV_COMPILE_LIBRARY_opcua
#endif

#include "nodeStoreSwitch.h"

// beim intiallisieren config->nodestore = nodestoreSwitch;


UA_StatusCode UA_NodestoreSwitch_init(UA_Server *server)
{
	UA_NodestoreSwitch *pSwitch = (UA_NodestoreSwitch*)UA_malloc(sizeof(UA_NodestoreSwitch));
	pSwitch->context = (void*)UA_malloc(sizeof(void));
	pSwitch->server = server;
	UA_StatusCode retval = UA_Nodestore_default_new(pSwitch->nodestoreArray[0]);
    if(retval != UA_STATUSCODE_GOOD) {
	  UA_free(pSwitch);
	  return UA_STATUSCODE_BADINTERNALERROR;
    }
    else
    	return UA_STATUSCODE_GOOD;
}




UA_StatusCode UA_NodestoreSwitch_linkNodestore(UA_NodestoreSwitch *ptrSwitch,
		void *nodestoreHandle) {
	int i;
	for (i = 0; ptrSwitch->nodestoreArray[i] != NULL; i++) {
		;
	}
	ptrSwitch->nodestoreArray[--i] = nodestoreHandle;
	return (ptrSwitch->nodestoreArray[i] != NULL) ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADOUTOFMEMORY;

}

//typedef void (*UA_NodestoreVisitor)(void *visitorContext, const UA_Node *node);


void nodeToDefaultStore(void *visitorContext, const UA_Node *node)
{

}

UA_StatusCode UA_NodestoreSwitch_unlinkNodestore(UA_NodestoreSwitch *ptrSwitch,
		void *nodestoreHandle) {
	int i;
	if(ptrSwitch->nodestoreArray[0] == nodestoreHandle)
		return UA_STATUSCODE_BADINTERNALERROR;
	for (i = 1; ptrSwitch->nodestoreArray[i] != nodestoreHandle; i++) {
		;
	}

	//vorhandene Knoten im zu unlinkenden Nodestore default Nodestore zuordnen
//	ptrSwitch->nodestoreArray[i]->iterate(ptrSwitch->nodestoreArray[i]->context, ptrSwitch->context, );




	//Array Plätze um 1 nach hinten rücken damit keine Lücken im Array
	for(--i;ptrSwitch->nodestoreArray[i] != NULL; i++)
	{
		if(ptrSwitch->nodestoreArray[i+1])
			ptrSwitch->nodestoreArray[i] = ptrSwitch->nodestoreArray[i+1];
    }
	return (ptrSwitch->nodestoreArray[--i] != NULL) ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADINTERNALERROR;
}


UA_StatusCode UA_NodestoreSwitch_changeNodestore(UA_NodestoreSwitch *ptrSwitch,
		void *nodestoreHandle) {



	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

/*



 static UA_Boolean checkNSIndex(UA_Server* server, UA_UInt16 nsIdx){
 return (UA_Boolean) (nsIdx < (UA_UInt16)server->namespacesSize);
 }

 /*
 * NodestoreInterface Function routing


 UA_Node *
 UA_NodestoreSwitch_newNode(UA_Server* server, UA_NodeClass nodeClass, UA_UInt16 namespaceIndex) {
 if(!checkNSIndex(server, namespaceIndex)){
 return NULL;
 }



 return server->namespaces[namespaceIndex].nodestore->newNode(nodeClass);




 }

 void
 UA_NodestoreSwitch_deleteNode(UA_Server* server, UA_Node *node){
 if(checkNSIndex(server, node->nodeId.namespaceIndex)){
 server->namespaces[node->nodeId.namespaceIndex].nodestore->deleteNode(node);
 }
 }

 UA_StatusCode
 UA_NodestoreSwitch_insertNode(UA_Server* server, UA_Node *node,
 UA_NodeId *addedNodeId) {
 if(!checkNSIndex(server, node->nodeId.namespaceIndex)){
 return UA_STATUSCODE_BADNODEIDUNKNOWN;
 }
 return server->namespaces[node->nodeId.namespaceIndex].nodestore->insertNode(
 server->namespaces[node->nodeId.namespaceIndex].nodestore->handle, node, addedNodeId);
 }
 const UA_Node *
 UA_NodestoreSwitch_getNode(UA_Server* server, const UA_NodeId *nodeId) {
 if(!checkNSIndex(server, nodeId->namespaceIndex)){
 return NULL;
 }
 return server->namespaces[nodeId->namespaceIndex].nodestore->getNode(
 server->namespaces[nodeId->namespaceIndex].nodestore->handle, nodeId);
 }
 UA_Node *
 UA_NodestoreSwitch_getNodeCopy(UA_Server* server, const UA_NodeId *nodeId) {
 if(!checkNSIndex(server, nodeId->namespaceIndex)){
 return NULL;
 }
 return server->namespaces[nodeId->namespaceIndex].nodestore->getNodeCopy(
 server->namespaces[nodeId->namespaceIndex].nodestore->handle, nodeId);
 }
 UA_StatusCode
 UA_NodestoreSwitch_replaceNode(UA_Server* server, UA_Node *node) {
 if(!checkNSIndex(server, node->nodeId.namespaceIndex)){
 return UA_STATUSCODE_BADNODEIDUNKNOWN;
 }
 return server->namespaces[node->nodeId.namespaceIndex].nodestore->replaceNode(
 server->namespaces[node->nodeId.namespaceIndex].nodestore->handle, node);
 }
 UA_StatusCode
 UA_NodestoreSwitch_removeNode(UA_Server* server, const UA_NodeId *nodeId) {
 if(!checkNSIndex(server, nodeId->namespaceIndex)){
 return UA_STATUSCODE_BADNODEIDUNKNOWN;
 }
 return server->namespaces[nodeId->namespaceIndex].nodestore->removeNode(
 server->namespaces[nodeId->namespaceIndex].nodestore->handle, nodeId);
 }
 void UA_NodestoreSwitch_releaseNode(UA_Server* server, const UA_Node *node){
 if(node && checkNSIndex(server, node->nodeId.namespaceIndex)){
 server->namespaces[node->nodeId.namespaceIndex].nodestore->releaseNode(
 server->namespaces[node->nodeId.namespaceIndex].nodestore->handle, node);
 }
 }
 */
