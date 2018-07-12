///*
// * nodeStoreSwitch.c
// *
// *  Created on: 21.06.2018
// *      Author: vitus
// */
//
//#ifndef OV_COMPILE_LIBRARY_opcua
//#define OV_COMPILE_LIBRARY_opcua
//#endif
//
//#include "nodeStoreSwitch.h"
//
//UA_StatusCode UA_NodestoreSwitch_linkNodestore(UA_NodestoreSwitch *ptrSwitch,
//		void *nodestoreHandle) {
//	int i;
//	for (i = 0; ptrSwitch->nodestoreArray[i] != NULL; i++) {
//		;
//	}
//	ptrSwitch->nodestoreArray[--i] = nodestoreHandle;
//	return (ptrSwitch->nodestoreArray[i] != NULL) ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADOUTOFMEMORY;
//
//}
//
//UA_StatusCode UA_NodestoreSwitch_unlinkNodestore(UA_NodestoreSwitch *ptrSwitch,
//		void *nodestoreHandle) {
//	int i;
//	for (i = 0; ptrSwitch->nodestoreArray[i] != nodestoreHandle; i++) {
//		;
//	}
//
//	//vorhandene Knoten im zu unlinkenden Nodestore default Nodestore zuordnen
//
//
//
//
//
//	//Array Plätze um 1 nach hinten rücken damit keine Lücken im Array
//
//
//	ptrSwitch->nodestoreArray[i-1]->iterate(ptrSwitch->nodestoreArray[i-1])
//
//	for(--i;ptrSwitch->nodestoreArray[i] != NULL; i++)
//	{
//		if(ptrSwitch->nodestoreArray[i+1])
//			ptrSwitch->nodestoreArray[i] = ptrSwitch->nodestoreArray[i+1];
//    }
//	return (ptrSwitch->nodestoreArray[--i] == NULL) ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADINTERNALERROR;
//}
//
//
//UA_StatusCode UA_NodestoreSwitch_changeNodestore(UA_NodestoreSwitch *ptrSwitch,
//		void *nodestoreHandle) {
//
//
//
//	return UA_STATUSCODE_BADNOTIMPLEMENTED;
//}
//
///*
//
//
//
// static UA_Boolean checkNSIndex(UA_Server* server, UA_UInt16 nsIdx){
// return (UA_Boolean) (nsIdx < (UA_UInt16)server->namespacesSize);
// }
//
// /*
// * NodestoreInterface Function routing
//
//
// UA_Node *
// UA_NodestoreSwitch_newNode(UA_Server* server, UA_NodeClass nodeClass, UA_UInt16 namespaceIndex) {
// if(!checkNSIndex(server, namespaceIndex)){
// return NULL;
// }
//
//
//
// return server->namespaces[namespaceIndex].nodestore->newNode(nodeClass);
//
//
//
//
// }
//
// void
// UA_NodestoreSwitch_deleteNode(UA_Server* server, UA_Node *node){
// if(checkNSIndex(server, node->nodeId.namespaceIndex)){
// server->namespaces[node->nodeId.namespaceIndex].nodestore->deleteNode(node);
// }
// }
//
// UA_StatusCode
// UA_NodestoreSwitch_insertNode(UA_Server* server, UA_Node *node,
// UA_NodeId *addedNodeId) {
// if(!checkNSIndex(server, node->nodeId.namespaceIndex)){
// return UA_STATUSCODE_BADNODEIDUNKNOWN;
// }
// return server->namespaces[node->nodeId.namespaceIndex].nodestore->insertNode(
// server->namespaces[node->nodeId.namespaceIndex].nodestore->handle, node, addedNodeId);
// }
// const UA_Node *
// UA_NodestoreSwitch_getNode(UA_Server* server, const UA_NodeId *nodeId) {
// if(!checkNSIndex(server, nodeId->namespaceIndex)){
// return NULL;
// }
// return server->namespaces[nodeId->namespaceIndex].nodestore->getNode(
// server->namespaces[nodeId->namespaceIndex].nodestore->handle, nodeId);
// }
// UA_Node *
// UA_NodestoreSwitch_getNodeCopy(UA_Server* server, const UA_NodeId *nodeId) {
// if(!checkNSIndex(server, nodeId->namespaceIndex)){
// return NULL;
// }
// return server->namespaces[nodeId->namespaceIndex].nodestore->getNodeCopy(
// server->namespaces[nodeId->namespaceIndex].nodestore->handle, nodeId);
// }
// UA_StatusCode
// UA_NodestoreSwitch_replaceNode(UA_Server* server, UA_Node *node) {
// if(!checkNSIndex(server, node->nodeId.namespaceIndex)){
// return UA_STATUSCODE_BADNODEIDUNKNOWN;
// }
// return server->namespaces[node->nodeId.namespaceIndex].nodestore->replaceNode(
// server->namespaces[node->nodeId.namespaceIndex].nodestore->handle, node);
// }
// UA_StatusCode
// UA_NodestoreSwitch_removeNode(UA_Server* server, const UA_NodeId *nodeId) {
// if(!checkNSIndex(server, nodeId->namespaceIndex)){
// return UA_STATUSCODE_BADNODEIDUNKNOWN;
// }
// return server->namespaces[nodeId->namespaceIndex].nodestore->removeNode(
// server->namespaces[nodeId->namespaceIndex].nodestore->handle, nodeId);
// }
// void UA_NodestoreSwitch_releaseNode(UA_Server* server, const UA_Node *node){
// if(node && checkNSIndex(server, node->nodeId.namespaceIndex)){
// server->namespaces[node->nodeId.namespaceIndex].nodestore->releaseNode(
// server->namespaces[node->nodeId.namespaceIndex].nodestore->handle, node);
// }
// }
// */
