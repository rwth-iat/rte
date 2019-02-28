
/*
 * nodeStoreSwitch.c
 *
 *  Created on: 21.06.2018
 *      Author: vitus
 */

#ifndef OV_COMPILE_LIBRARY_opcua
#define OV_COMPILE_LIBRARY_opcua
#endif

#include "opcua_storeSwitch.h"
//TODO checks for defaultNS != NULL --> Allow defaultNS to be NULL and return error (or NULL)

//TODO move to open62541
void UA_Nodestore_copy(const UA_Nodestore* src, UA_Nodestore* dst){
	dst->context = src->context;
	dst->deleteNode = src->deleteNode;
	dst->deleteNodestore = src->deleteNodestore;
	dst->getNode = src->getNode;
	dst->getNodeCopy = src->getNodeCopy;
	dst->inPlaceEditAllowed = src->inPlaceEditAllowed; //TODO check
	dst->insertNode = src->insertNode;
	dst->iterate = src->iterate;
	dst->newNode = src->newNode;
	dst->releaseNode = src->releaseNode;
	dst->removeNode = src->removeNode;
    dst->replaceNode = src->replaceNode;
}

static size_t findNSHandle(UA_NodestoreSwitch *storeSwitch, void *nsHandle)
{
	size_t i;
	for(i=0; i<storeSwitch->size; i++)
	{
		if(storeSwitch->nodestoreArray[i]->context == nsHandle)
		{
			return i;
		}
	}
	return i;
}

UA_NodestoreSwitch *UA_NodestoreSwitch_new()
{
	//UA_NodestoreSwitch *storeSwitch = UA_malloc(sizeof *storeSwitch + 1 * sizeof *storeSwitch->nodestoreArray);
	UA_NodestoreSwitch *storeSwitch = UA_malloc(sizeof(UA_NodestoreSwitch));

	if(!storeSwitch)
		return NULL;
	storeSwitch->defaultNodestore = NULL;
	storeSwitch->nodestoreArray = NULL;
	storeSwitch->size = 0;
	return storeSwitch;
}


UA_StatusCode UA_NodestoreSwitch_linkDefaultNodestore(UA_NodestoreSwitch *storeSwitch, UA_Nodestore *store)
{
	if(store == NULL)
		return UA_STATUSCODE_BADNOTFOUND;
	storeSwitch->defaultNodestore = store;
	return UA_STATUSCODE_GOOD;
}

void UA_NodestoreSwitch_linkNodestoreSwitchToServer(UA_NodestoreSwitch *storeSwitch ,UA_Nodestore *store)
{
	store->context = storeSwitch;
	store->deleteNodestore = UA_NodestoreSwitch_deleteNodestores;
	store->inPlaceEditAllowed = false; //TODO check whether in place edit is allowed
	store->newNode = UA_NodestoreSwitch_newNode;
	store->deleteNode = UA_NodestoreSwitch_deleteNode;
	store->getNode = UA_NodestoreSwitch_getNode;
	store->releaseNode = UA_NodestoreSwitch_releaseNode;
	store->getNodeCopy = UA_NodestoreSwitch_getNodeCopy;
	store->insertNode = UA_NodestoreSwitch_insertNode;
	store->replaceNode = UA_NodestoreSwitch_replaceNode;
	store->iterate = UA_NodestoreSwitch_iterate;
	store->removeNode = UA_NodestoreSwitch_removeNode;
}


UA_StatusCode UA_NodestoreSwitch_linkNodestoreToNamespace(UA_NodestoreSwitch *storeSwitch,
		UA_Nodestore *ns, UA_UInt16 namespaceindex)
{
	//TODO make multithreading save --> Lock/Unlock or CRTISECTION
	if(storeSwitch->size <= namespaceindex)
	{
		UA_Nodestore **tmpPointer = NULL;
		tmpPointer = UA_realloc(storeSwitch->nodestoreArray , (namespaceindex + 1) * sizeof (UA_Nodestore*));
		if(!tmpPointer)
			return UA_STATUSCODE_BADOUTOFMEMORY;
		storeSwitch->nodestoreArray = tmpPointer;
		for (UA_UInt16 i = storeSwitch->size; i < namespaceindex + 1; i++){
			storeSwitch->nodestoreArray[i] = NULL;
		}
		storeSwitch->size = namespaceindex+1;
	}

	storeSwitch->nodestoreArray[namespaceindex] = ns;
	return UA_STATUSCODE_GOOD;

}


UA_StatusCode UA_NodestoreSwitch_unlinkNodestoreFromNamespace(UA_NodestoreSwitch *storeSwitch, UA_Nodestore *ns)
{
	//TODO make multithreading save --> Lock/Unlock or CRTISECTION
	size_t flag = 0;
	if(ns == NULL)
		return UA_STATUSCODE_BADNOTFOUND;
	for(size_t i=0; i< storeSwitch->size; i++)
	{
		if(storeSwitch->nodestoreArray[i] == ns)
		{
			if(flag == 0)
			{
				storeSwitch->nodestoreArray[i]->deleteNodestore(storeSwitch->nodestoreArray[i]->context);
				flag = 1;
			}
			storeSwitch->nodestoreArray[i] = NULL;//UA_free(&storeSwitch->nodestoreArray[i]);
			//TODO resize ns array if ns is last in array?
		}

	}
	return (flag == 1) ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADNOTFOUND;
}

//TODO check and change to replace all ns --> comfortfunction --> change to getNodestore(nsIndex)
//TODO  add getNSIndexes(nsHandle) --> change findNSHandle for all matches
UA_StatusCode UA_NodestoreSwitch_changeNodestore(UA_NodestoreSwitch *storeSwitch, void *nsHandleOut, UA_Nodestore *nsIn) {
	size_t i= findNSHandle(storeSwitch, nsHandleOut);
	if(i == storeSwitch->size)
		return UA_STATUSCODE_BADNOTFOUND;
	if(nsIn == NULL)
		return UA_STATUSCODE_BADINTERNALERROR;
	storeSwitch->nodestoreArray[i] = nsIn;
	return UA_STATUSCODE_GOOD;
}

void UA_NodestoreSwitch_deleteSwitch(UA_NodestoreSwitch *storeSwitch)
{
	UA_NodestoreSwitch_deleteNodestores((void*) storeSwitch);
	UA_free(storeSwitch);
}


 /* Standard Nodestorefunctions */
void UA_NodestoreSwitch_deleteNode(void *storeSwitchHandle, UA_Node *node)
{
	UA_NodestoreSwitch *storeSwitch = storeSwitchHandle;
	if (node->nodeId.namespaceIndex < storeSwitch->size){
		if (storeSwitch->nodestoreArray[node->nodeId.namespaceIndex] != NULL){
			return (storeSwitch->nodestoreArray[node->nodeId.namespaceIndex]->deleteNode(storeSwitch->nodestoreArray[node->nodeId.namespaceIndex]->context, node));
		}
	}
	if(storeSwitch->defaultNodestore != NULL){
		return storeSwitch->defaultNodestore->deleteNode(storeSwitch->defaultNodestore->context, node);
	}
}

UA_StatusCode UA_NodestoreSwitch_insertNode(void *storeSwitchHandle, UA_Node *node, UA_NodeId *addedNodeId)
{
	UA_NodestoreSwitch *storeSwitch = storeSwitchHandle;
	if (node->nodeId.namespaceIndex < storeSwitch->size){
		if (storeSwitch->nodestoreArray[node->nodeId.namespaceIndex] != NULL){
			return (storeSwitch->nodestoreArray[node->nodeId.namespaceIndex]->insertNode(storeSwitch->nodestoreArray[node->nodeId.namespaceIndex]->context, node, addedNodeId));
		}
	}
	if(storeSwitch->defaultNodestore != NULL){
		return storeSwitch->defaultNodestore->insertNode(storeSwitch->defaultNodestore->context, node, addedNodeId);
	}
	return UA_STATUSCODE_BADNODEIDINVALID;
}

const UA_Node *UA_NodestoreSwitch_getNode(void *storeSwitchHandle, const UA_NodeId *nodeId)
{
	UA_NodestoreSwitch *storeSwitch = storeSwitchHandle;
	if (nodeId->namespaceIndex < storeSwitch->size){
		if (storeSwitch->nodestoreArray[nodeId->namespaceIndex] != NULL){
			return (storeSwitch->nodestoreArray[nodeId->namespaceIndex]->getNode(storeSwitch->nodestoreArray[nodeId->namespaceIndex]->context,nodeId));
		}
	}

	if(storeSwitch->defaultNodestore != NULL){
		return (const UA_Node*) storeSwitch->defaultNodestore->getNode(storeSwitch->defaultNodestore->context, nodeId);
	}
	return NULL;
}

UA_StatusCode UA_NodestoreSwitch_getNodeCopy(void *storeSwitchHandle, const UA_NodeId *nodeId, UA_Node **outNode)
{
	UA_NodestoreSwitch *storeSwitch = storeSwitchHandle;
	if (nodeId->namespaceIndex < storeSwitch->size){
		if (storeSwitch->nodestoreArray[nodeId->namespaceIndex] != NULL){
			return storeSwitch->nodestoreArray[nodeId->namespaceIndex]->getNodeCopy(storeSwitch->nodestoreArray[nodeId->namespaceIndex]->context, nodeId, outNode);
		}
	}
	if(storeSwitch->defaultNodestore != NULL){
		return storeSwitch->defaultNodestore->getNodeCopy(storeSwitch->defaultNodestore->context, nodeId, outNode);
	}
	return UA_STATUSCODE_BADNODEIDINVALID;
}

UA_StatusCode UA_NodestoreSwitch_replaceNode(void *storeSwitchHandle, UA_Node *node)
{
	UA_NodestoreSwitch *storeSwitch = storeSwitchHandle;
	if (node->nodeId.namespaceIndex < storeSwitch->size){
		if (storeSwitch->nodestoreArray[node->nodeId.namespaceIndex] != NULL){
			return storeSwitch->nodestoreArray[node->nodeId.namespaceIndex]->replaceNode(storeSwitch->nodestoreArray[node->nodeId.namespaceIndex]->context, node);
		}
	}
	if(storeSwitch->defaultNodestore != NULL){
		return storeSwitch->defaultNodestore->replaceNode(storeSwitch->defaultNodestore->context, node);
	}
	return UA_STATUSCODE_BADNODEIDINVALID;
}

UA_StatusCode UA_NodestoreSwitch_removeNode(void *storeSwitchHandle, const UA_NodeId *nodeId)
{
	UA_NodestoreSwitch *storeSwitch = storeSwitchHandle;
	if (nodeId->namespaceIndex < storeSwitch->size){
		if (storeSwitch->nodestoreArray[nodeId->namespaceIndex] != NULL){
			return storeSwitch->nodestoreArray[nodeId->namespaceIndex]->removeNode(storeSwitch->nodestoreArray[nodeId->namespaceIndex]->context, nodeId);
		}
	}
	if(storeSwitch->defaultNodestore != NULL){
		return storeSwitch->defaultNodestore->removeNode(storeSwitch->defaultNodestore->context, nodeId);
	}
	return UA_STATUSCODE_BADNODEIDINVALID;
}

void UA_NodestoreSwitch_releaseNode(void *storeSwitchHandle, const UA_Node *node)
{
	if(node == NULL) return;
	UA_NodestoreSwitch *storeSwitch = storeSwitchHandle;
	if (node->nodeId.namespaceIndex < storeSwitch->size){
		if (storeSwitch->nodestoreArray[node->nodeId.namespaceIndex] != NULL){
			return storeSwitch->nodestoreArray[node->nodeId.namespaceIndex]->releaseNode(storeSwitch->nodestoreArray[node->nodeId.namespaceIndex]->context, node);
		}
	}
	if(storeSwitch->defaultNodestore != NULL){
		return storeSwitch->defaultNodestore->releaseNode(storeSwitch->defaultNodestore->context, node);
	}
}

 void UA_NodestoreSwitch_deleteNodestores(void *storeSwitchHandle)
 {
	UA_NodestoreSwitch *storeSwitch = storeSwitchHandle;

	//Delete default nodestore
	if(storeSwitch->defaultNodestore){
		//Check for explicit link to default nodestore and set to NULL
		for(size_t i=0 ; i < storeSwitch->size ; i++){
			//check that nodestore is equal by comparision of interfaces
			if(storeSwitch->nodestoreArray[i] == storeSwitch->defaultNodestore)
				storeSwitch->nodestoreArray[i]= NULL;
		}
		storeSwitch->defaultNodestore->deleteNodestore(storeSwitch->defaultNodestore->context);
		storeSwitch->defaultNodestore = NULL;
	}

 	for(size_t i=0; i< storeSwitch->size; i++)
 	{
 		//if namespace i has custom nodestore
 		if(storeSwitch->nodestoreArray[i]){
 			// search forward for other occurances of nodestore and set to null
 			for(size_t j=i+1 ; j < storeSwitch->size ; j++){
 				//check that nodestore is equal by comparision of interfaces
 				if(storeSwitch->nodestoreArray[j] == storeSwitch->nodestoreArray[i])
 					storeSwitch->nodestoreArray[j]= NULL;
 			}
 			//delete the nodestore
 			storeSwitch->nodestoreArray[i]->deleteNodestore(storeSwitch->nodestoreArray[i]->context);
 			storeSwitch->nodestoreArray[i] = NULL;
 		}
 	}
 }


  UA_Node *UA_NodestoreSwitch_newNode(void *storeSwitchHandle, UA_NodeClass nodeClass)
  {
	//todo: NOT IMPLEMENTED Cause of, not clear in which nodestore the memory has to be allocated. (storeSwitch or defaultNodestore)
 	UA_NodestoreSwitch *storeSwitch = storeSwitchHandle;
 	return storeSwitch->defaultNodestore->newNode(storeSwitch->defaultNodestore->context, nodeClass);
	//  return NULL;
  }

  void UA_NodestoreSwitch_iterate(void *storeSwitchHandle, void *visitorContext, UA_NodestoreVisitor visitor)
  {
  	UA_NodestoreSwitch *storeSwitch = storeSwitchHandle;
  	UA_Nodestore *tempArray[storeSwitch->size];
  	for(size_t i=0; i<storeSwitch->size; i++)
  	{
  		for(size_t j=0; j<=i; j++)
  		{
  			if(tempArray[j] == storeSwitch->nodestoreArray[i])
  				break;
  			else if(j==i)
  			{
  				tempArray[i] = storeSwitch->nodestoreArray[i];
  				storeSwitch->nodestoreArray[i]->iterate(storeSwitch, visitorContext, visitor);
  			}
  		}
  	}

  }
