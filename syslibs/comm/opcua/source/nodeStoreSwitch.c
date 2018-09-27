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

static UA_UInt16 findNSHandle(UA_NodestoreSwitch *pSwitch, void *nodestoreHandle)
{
	UA_UInt16 i;
	for(i=0; i<pSwitch->size; i++)
	{
	 if(pSwitch->nodestoreArray[i]->context == *nodestoreHandle)
	 {
	   return i;
	 }
	}
	return 0;
}

 static UA_Boolean checkNSHandle(UA_NodestoreSwitch *pSwitch, void *nodestoreHandle){
	 int i;
	 for(i=0; i<pSwitch->size; i++)
	 {
		 if(pSwitch->nodestoreArray[i]->context == *nodestoreHandle)
		 {
			 return true;
		 }
	 }
	 return false;
 }

static void nsArrayResize(UA_NodestoreSwitch *pSwitch, UA_UInt16 newSize)
{
	pSwitch->size = newSize;
	pSwitch->nodestoreArray = UA_realloc(pSwitch->nodestoreArray,pSwitch->size);
}


UA_StatusCode UA_NodestoreSwitch_init(UA_Server *server)
{
	UA_NodestoreSwitch *pSwitch = (UA_NodestoreSwitch*)UA_malloc(sizeof(UA_NodestoreSwitch));
	pSwitch->context = (void*)UA_malloc(sizeof(void));
	pSwitch->server = server;
	pSwitch->size = 1;
	pSwitch->nodestoreArray = UA_malloc(pSwitch->size * sizeof(UA_Nodestore));
	UA_StatusCode retval = UA_Nodestore_default_new(pSwitch->nodestoreArray[0]);

    if(retval != UA_STATUSCODE_GOOD) {
	  UA_free(pSwitch);
	  return UA_STATUSCODE_BADINTERNALERROR;
    }
    else
    	return UA_STATUSCODE_GOOD;
}




UA_StatusCode UA_NodestoreSwitch_linkNodestore(UA_NodestoreSwitch *pSwitch,
		UA_Nodestore *ns)
{
	nsArrayResize(pSwitch,(UA_UInt16*) pSwitch->size +1);
	pSwitch->nodestoreArray[pSwitch->size-1]->context = ns->context;
	pSwitch->nodestoreArray[pSwitch->size-1]->deleteNodestore =ns->deleteNodestore;
	pSwitch->nodestoreArray[pSwitch->size-1]->inPlaceEditAllowed = ns->inPlaceEditAllowed;
	pSwitch->nodestoreArray[pSwitch->size-1]->newNode = ns->newNode;
	pSwitch->nodestoreArray[pSwitch->size-1]->deleteNode = ns->deleteNode;
	pSwitch->nodestoreArray[pSwitch->size-1]->getNode = ns->getNode;
	pSwitch->nodestoreArray[pSwitch->size-1]->releaseNode = ns->releaseNode;
	pSwitch->nodestoreArray[pSwitch->size-1]->getNodeCopy = ns->getNodeCopy;
	pSwitch->nodestoreArray[pSwitch->size-1]->insertNode = ns->insertNode;
	pSwitch->nodestoreArray[pSwitch->size-1]->replaceNode = ns->replaceNode;
	pSwitch->nodestoreArray[pSwitch->size-1]->removeNode = ns->removeNode;
	pSwitch->nodestoreArray[pSwitch->size-1]->iterate = ns->iterate;

	return (pSwitch->nodestoreArray[pSwitch->size-1]->context != NULL) ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADINTERNALERROR;

}

//typedef void (*UA_NodestoreVisitor)(void *visitorContext, const UA_Node *node);


void nodeToDefaultStore(void *visitorContext, const UA_Node *node)
{

}

UA_StatusCode UA_NodestoreSwitch_unlinkNodestore(UA_NodestoreSwitch *pSwitch,
		void *nodestoreHandle) {
	int i = findNSHandle(pSwitch, nodestoreHandle);
	if(i = 0)
		return UA_STATUSCODE_BADNOTFOUND;
	//TODO:
	//vorhandene Knoten im zu unlinkenden Nodestore default Nodestore zuordnen
//	ptrSwitch->nodestoreArray[i]->iterate(ptrSwitch->nodestoreArray[i]->context, ptrSwitch->context, );


	//Array um 1 nach vorne damit keine luecken
	for(--i;pSwitch->nodestoreArray[i] == NULL; i++)
	{

    }
	return (pSwitch->nodestoreArray[--i] != NULL) ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADINTERNALERROR;
}


UA_StatusCode UA_NodestoreSwitch_changeNodestore(UA_NodestoreSwitch *pSwitch,
		void *nodestoreHandleOut, void *nodestoreHandleIn) {
	int i= findNSHandle(pSwitch, nodestoreHandleOut);
	if(i == 0)
		return UA_STATUSCODE_BADNOTFOUND;
	//ptrSwitch->nodestoreArray[i]->iterate  Nodes von altem Nodestore zu neuem hinzufügen
	UA_NodestoreSwitch_deleteNodestore(pSwitch, nodestoreHandleOut);
	return UA_STATUSCODE_GOOD;

}


UA_StatusCode UA_NodestoreSwitch_deleteNodestore(UA_NodestoreSwitch *pSwitch, void *nodestoreHandle)
{
	if(pSwitch->nodestoreArray[0] == nodestoreHandle)
			return UA_STATUSCODE_BADINTERNALERROR;
	return UA_STATUSCODE_BADNOTFOUND;
}


/*
 * NodestoreInterface Function routing
*/

 UA_Node *UA_NodestoreSwitch_newNode(UA_NodestoreSwitch *pSwitch, UA_NodeClass nodeClass, void *nodestoreHandle)
 {
	 if(!checkNSHandle(pSwitch, nodestoreHandle))
	 {
		 return NULL;
	 }
	 return server->namespaces[namespaceIndex].nodestore->newNode(nodeClass);
 }

 void UA_NodestoreSwitch_deleteNode(UA_Server* server, UA_Node *node)
 {
	 if(checkNSHandle(server, node->nodeId.namespaceIndex))
	 {
		 server->namespaces[node->nodeId.namespaceIndex].nodestore->deleteNode(node);
	 }
 }

 UA_StatusCode UA_NodestoreSwitch_insertNode(UA_Server* server, UA_Node *node,
 UA_NodeId *addedNodeId)
 {
	 if(!checkNSHandle(server, node->nodeId.namespaceIndex)){
		 return UA_STATUSCODE_BADNODEIDUNKNOWN;
	 }
	 return server->namespaces[node->nodeId.namespaceIndex].nodestore->insertNode(
			 server->namespaces[node->nodeId.namespaceIndex].nodestore->handle, node, addedNodeId);
 }

 const UA_Node *UA_NodestoreSwitch_getNode(UA_Server* server, const UA_NodeId *nodeId)
 {
	 if(!checkNSHandle(server, nodeId->namespaceIndex)){
		 return NULL;
	 }
	 return server->namespaces[nodeId->namespaceIndex].nodestore->getNode(
			 server->namespaces[nodeId->namespaceIndex].nodestore->handle, nodeId);
 }

 UA_Node *UA_NodestoreSwitch_getNodeCopy(UA_Server* server, const UA_NodeId *nodeId) {
	 if(!checkNSHandle(server, nodeId->namespaceIndex)){
		 return NULL;
	 }
	 return server->namespaces[nodeId->namespaceIndex].nodestore->getNodeCopy(
			 server->namespaces[nodeId->namespaceIndex].nodestore->handle, nodeId);
 }

 UA_StatusCode UA_NodestoreSwitch_replaceNode(UA_Server* server, UA_Node *node)
 {
	 if(!checkNSHandle(server, node->nodeId.namespaceIndex)){
		 return UA_STATUSCODE_BADNODEIDUNKNOWN;
	 }
	 return server->namespaces[node->nodeId.namespaceIndex].nodestore->replaceNode(
			 server->namespaces[node->nodeId.namespaceIndex].nodestore->handle, node);
 }

 UA_StatusCode
 UA_NodestoreSwitch_removeNode(UA_Server* server, const UA_NodeId *nodeId)
 {
	 if(!checkNSHandle(server, nodeId->namespaceIndex))
	 {
		 return UA_STATUSCODE_BADNODEIDUNKNOWN;
	 }
	 return server->namespaces[nodeId->namespaceIndex].nodestore->removeNode(
			 server->namespaces[nodeId->namespaceIndex].nodestore->handle, nodeId);
 }

 void UA_NodestoreSwitch_releaseNode(UA_Server* server, const UA_Node *node)
 {
	 if(node && checkNSHandle(server, node->nodeId.namespaceIndex))
	 {
		 server->namespaces[node->nodeId.namespaceIndex].nodestore->releaseNode(
				 server->namespaces[node->nodeId.namespaceIndex].nodestore->handle, node);
	 }
 }

