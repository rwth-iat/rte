
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

//TODO checks for defaultNS != NULL --> Allow defaultNS to be NULL and return error (or NULL)


static size_t findNSHandle(UA_NodestoreSwitch *storeSwitch, void *nsHandle)
{
	size_t i;
	for(i=0; i<storeSwitch->nodestoreArraySize; i++)
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
	storeSwitch->nodestoreArraySize = 0;
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
	//TODO move to own function for reuse (~UA_Nodestore_replace(ns_old, ns_new))
	store->context = storeSwitch;
	store->deleteNodestore = UA_NodestoreSwitch_deleteNodestores;
	store->inPlaceEditAllowed = true;
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
	if(storeSwitch->nodestoreArraySize <= namespaceindex)
	{
		UA_Nodestore **tmpPointer = NULL;
		tmpPointer = UA_realloc(storeSwitch->nodestoreArray , namespaceindex+1 * sizeof (UA_Nodestore*));
		if(!tmpPointer)
			return UA_STATUSCODE_BADOUTOFMEMORY;
		for (UA_UInt16 i = storeSwitch->nodestoreArraySize; i < namespaceindex + 1; i++){
			storeSwitch->nodestoreArray[i] = NULL;
		}
		storeSwitch->nodestoreArray = tmpPointer;
		storeSwitch->nodestoreArraySize = namespaceindex+1;
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
	for(size_t i=0; i< storeSwitch->nodestoreArraySize; i++)
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
	if(i == storeSwitch->nodestoreArraySize)
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
	size_t i,j=0;
	for(i=0; i<storeSwitch->nodestoreArraySize; i++)
	{
		j = findNSHandle(storeSwitch, storeSwitch->nodestoreArray[i]->context);
		if(j != storeSwitch->nodestoreArraySize)
		{
			storeSwitch->nodestoreArray[j]->deleteNode(storeSwitch->nodestoreArray[j]->context, node);
		}
	}
}

UA_StatusCode UA_NodestoreSwitch_insertNode(void *storeSwitchHandle, UA_Node *node, UA_NodeId *addedNodeId)
{
	UA_NodestoreSwitch *storeSwitch = storeSwitchHandle;
	if (node->nodeId.namespaceIndex < storeSwitch->nodestoreArraySize){
		if (storeSwitch->nodestoreArray[node->nodeId.namespaceIndex] != NULL){
			return (storeSwitch->nodestoreArray[node->nodeId.namespaceIndex]->insertNode(storeSwitch->nodestoreArray[node->nodeId.namespaceIndex]->context, node, addedNodeId));
		}
	}
	return storeSwitch->defaultNodestore->insertNode(storeSwitch->defaultNodestore->context, node, addedNodeId);
}

const UA_Node *UA_NodestoreSwitch_getNode(void *storeSwitchHandle, const UA_NodeId *nodeId)
{
	UA_NodestoreSwitch *storeSwitch = storeSwitchHandle;
	if (nodeId->namespaceIndex < storeSwitch->nodestoreArraySize){
		if (storeSwitch->nodestoreArray[nodeId->namespaceIndex] != NULL){
			return (storeSwitch->nodestoreArray[nodeId->namespaceIndex]->getNode(storeSwitch->nodestoreArray[nodeId->namespaceIndex]->context,nodeId));
		}
	}
	return (const UA_Node*) storeSwitch->defaultNodestore->getNode(storeSwitch->defaultNodestore->context, nodeId);
}

UA_StatusCode UA_NodestoreSwitch_getNodeCopy(void *storeSwitchHandle, const UA_NodeId *nodeId, UA_Node **outNode)
{
	UA_NodestoreSwitch *storeSwitch = storeSwitchHandle;
	if (nodeId->namespaceIndex < storeSwitch->nodestoreArraySize){
		if (storeSwitch->nodestoreArray[nodeId->namespaceIndex] != NULL){
			return storeSwitch->nodestoreArray[nodeId->namespaceIndex]->getNodeCopy(storeSwitch->nodestoreArray[nodeId->namespaceIndex]->context, nodeId, outNode);
		}
	}
	return storeSwitch->defaultNodestore->getNodeCopy(storeSwitch->defaultNodestore->context, nodeId, outNode);
}

UA_StatusCode UA_NodestoreSwitch_replaceNode(void *storeSwitchHandle, UA_Node *node)
{
	UA_NodestoreSwitch *storeSwitch = storeSwitchHandle;
	if (node->nodeId.namespaceIndex < storeSwitch->nodestoreArraySize){
		if (storeSwitch->nodestoreArray[node->nodeId.namespaceIndex] != NULL){
			return storeSwitch->nodestoreArray[node->nodeId.namespaceIndex]->replaceNode(storeSwitch->nodestoreArray[node->nodeId.namespaceIndex]->context, node);
		}
	}
	return storeSwitch->defaultNodestore->replaceNode(storeSwitch->defaultNodestore->context, node);
}

UA_StatusCode UA_NodestoreSwitch_removeNode(void *storeSwitchHandle, const UA_NodeId *nodeId)
{
	UA_NodestoreSwitch *storeSwitch = storeSwitchHandle;
	if (nodeId->namespaceIndex < storeSwitch->nodestoreArraySize){
		if (storeSwitch->nodestoreArray[nodeId->namespaceIndex] != NULL){
			return storeSwitch->nodestoreArray[nodeId->namespaceIndex]->removeNode(storeSwitch->nodestoreArray[nodeId->namespaceIndex]->context, nodeId);
		}
	}
	return storeSwitch->defaultNodestore->removeNode(storeSwitch->defaultNodestore->context, nodeId);
}

void UA_NodestoreSwitch_releaseNode(void *storeSwitchHandle, const UA_Node *node)
{
	UA_NodestoreSwitch *storeSwitch = storeSwitchHandle;
	if (node->nodeId.namespaceIndex < storeSwitch->nodestoreArraySize){
		if (storeSwitch->nodestoreArray[node->nodeId.namespaceIndex] != NULL){
			return storeSwitch->nodestoreArray[node->nodeId.namespaceIndex]->releaseNode(storeSwitch->nodestoreArray[node->nodeId.namespaceIndex]->context, node);
		}
	}
	return storeSwitch->defaultNodestore->releaseNode(storeSwitch->defaultNodestore->context, node);
}

 void UA_NodestoreSwitch_deleteNodestores(void *storeSwitchHandle)
 {

	UA_NodestoreSwitch *storeSwitch = storeSwitchHandle;
 	for(size_t i=0; i< storeSwitch->nodestoreArraySize; i++)
 	{
 		storeSwitch->nodestoreArray[i]->deleteNodestore(storeSwitch->nodestoreArray[i]->context); //TODO check if NULL before
 		UA_free(&storeSwitch->nodestoreArray[i]); //TODO change to = NULL --> replace in whole document
 	}
	//TODO also delete default nodestore
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
  	UA_Nodestore *tempArray[storeSwitch->nodestoreArraySize];
  	for(size_t i=0; i<storeSwitch->nodestoreArraySize; i++)
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
