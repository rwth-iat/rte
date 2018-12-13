
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



static size_t findNSHandle(UA_NodestoreSwitch *pSwitch, void *nsHandle)
{
	size_t i;
	for(i=0; i<pSwitch->size; i++)
	{
	 if(pSwitch->nodestoreArray[i]->context == nsHandle)
	 {
	   return i;
	 }
	}
	return i;
}

UA_NodestoreSwitch *UA_NodestoreSwitch_new()
{
	//UA_NodestoreSwitch *pSwitch = UA_malloc(sizeof *pSwitch + 1 * sizeof *pSwitch->nodestoreArray);
	UA_NodestoreSwitch *pSwitch = UA_malloc(sizeof *pSwitch);
	if(!pSwitch)
				return NULL;
	pSwitch->nodestoreArray = UA_malloc(sizeof (UA_Nodestore*));
	pSwitch->defaultNodestore = NULL;
	pSwitch->nodestoreArray[0] = NULL;
	pSwitch->size = 0;
	if(!pSwitch->nodestoreArray)
		return NULL;

	return pSwitch;
}


UA_StatusCode UA_NodestoreSwitch_linkDefaultNodestore(UA_NodestoreSwitch *pSwitch, UA_Nodestore *ns)
{
	if(ns == NULL)
		return UA_STATUSCODE_BADNOTFOUND;
	pSwitch->defaultNodestore = ns;
	return UA_STATUSCODE_GOOD;

}

void UA_NodestoreSwitch_linkNodestoreSwitch(UA_NodestoreSwitch *pSwitch ,UA_Nodestore *ns)
{
	ns->context = pSwitch;
	ns->deleteNodestore = UA_NodestoreSwitch_deleteNodestores;
	ns->inPlaceEditAllowed = true;
	ns->newNode = UA_NodestoreSwitch_newNode;
	ns->deleteNode = UA_NodestoreSwitch_deleteNode;
	ns->getNode = UA_NodestoreSwitch_getNode;
	ns->releaseNode = UA_NodestoreSwitch_releaseNode;
	ns->getNodeCopy = UA_NodestoreSwitch_getNodeCopy;
	ns->insertNode = UA_NodestoreSwitch_insertNode;
	ns->replaceNode = UA_NodestoreSwitch_replaceNode;
	ns->iterate = UA_NodestoreSwitch_iterate;
	ns->removeNode = UA_NodestoreSwitch_removeNode;
}


UA_StatusCode UA_NodestoreSwitch_linkNodestore(UA_NodestoreSwitch *pSwitch,
		UA_Nodestore *ns, UA_UInt16 namespaceindex)
{
	if(pSwitch->size <= namespaceindex)
	{
		UA_Nodestore **tmpPointer = NULL;
		tmpPointer = realloc(pSwitch->nodestoreArray , namespaceindex+1 * sizeof (UA_Nodestore*));
		if(tmpPointer)
			return UA_STATUSCODE_BADOUTOFMEMORY;
		pSwitch->nodestoreArray = tmpPointer;
		pSwitch->size = namespaceindex+1;
	}

	pSwitch->nodestoreArray[namespaceindex] = ns;
//	pSwitch->nodestoreArray[pSwitch->size-1].context = nsHandle->context ;
//	pSwitch->nodestoreArray[pSwitch->size-1].deleteNodestore = nsHandle->deleteNodestore;
//	pSwitch->nodestoreArray[pSwitch->size-1].inPlaceEditAllowed = nsHandle->inPlaceEditAllowed;
//	pSwitch->nodestoreArray[pSwitch->size-1].newNode = nsHandle->newNode;
//	pSwitch->nodestoreArray[pSwitch->size-1].deleteNode = ns->deleteNode;
//	pSwitch->nodestoreArray[pSwitch->size-1].getNode = ns->getNode;
//	pSwitch->nodestoreArray[pSwitch->size-1].releaseNode = ns->releaseNode;
//	pSwitch->nodestoreArray[pSwitch->size-1].getNodeCopy = ns->getNodeCopy;
//	pSwitch->nodestoreArray[pSwitch->size-1].insertNode = ns->insertNode;
//	pSwitch->nodestoreArray[pSwitch->size-1].replaceNode = ns->replaceNode;
//	pSwitch->nodestoreArray[pSwitch->size-1].removeNode = ns->removeNode;
//	pSwitch->nodestoreArray[pSwitch->size-1].iterate = ns->iterate;
	return UA_STATUSCODE_GOOD;

}


UA_StatusCode UA_NodestoreSwitch_unlinkNodestore(UA_NodestoreSwitch *pSwitch, UA_Nodestore *ns)
{
	size_t flag = 0;
	if(ns == NULL)
		return UA_STATUSCODE_BADNOTFOUND;
	for(size_t i=0; i< pSwitch->size; i++)
	{
		if(pSwitch->nodestoreArray[i] == ns)
		{
			if(flag == 0)
			{
				pSwitch->nodestoreArray[i]->deleteNodestore(pSwitch->nodestoreArray[i]->context);
				flag = 1;
			}
			UA_free(&pSwitch->nodestoreArray[i]);
		}

	}
	return (flag == 1) ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADNOTFOUND;
}


UA_StatusCode UA_NodestoreSwitch_changeNodestore(UA_NodestoreSwitch *pSwitch, void *nsHandleOut, UA_Nodestore *nsIn) {
	size_t i= findNSHandle(pSwitch, nsHandleOut);
	if(i == pSwitch->size)
		return UA_STATUSCODE_BADNOTFOUND;
	if(nsIn == NULL)
		return UA_STATUSCODE_BADINTERNALERROR;
	pSwitch->nodestoreArray[i] = nsIn;
	return UA_STATUSCODE_GOOD;

}


 UA_Node *UA_NodestoreSwitch_newNode_inNS(UA_NodestoreSwitch *pSwitch, void *nsHandle, UA_NodeClass nodeClass)
  {
	 size_t i = findNSHandle(pSwitch, nsHandle);
	 if(i==pSwitch->size)
		 return NULL;
	 return pSwitch->nodestoreArray[i]->newNode(pSwitch->nodestoreArray[i]->context, nodeClass);
  }



 void UA_NodestoreSwitch_deleteNode_inNS(UA_NodestoreSwitch *pSwitch, void *nsHandle, UA_Node *node)
 {
	 size_t i = findNSHandle(pSwitch, nsHandle);
	 if(i==pSwitch->size)
		 return;
	 pSwitch->nodestoreArray[i]->deleteNode(pSwitch->nodestoreArray[i]->context, node);
 }


 UA_StatusCode UA_NodestoreSwitch_insertNode_inNS(UA_NodestoreSwitch *pSwitch, void *nsHandle, UA_Node *node,
		 UA_NodeId *addedNodeId)
 {
	 size_t i = findNSHandle(pSwitch, nsHandle);
	 if(i==pSwitch->size)
		 return UA_STATUSCODE_BADNOTFOUND;
	 return pSwitch->nodestoreArray[i]->insertNode(pSwitch->nodestoreArray[i]->context, node, addedNodeId);
 }


 const UA_Node *UA_NodestoreSwitch_getNode_inNS(UA_NodestoreSwitch *pSwitch, void *nsHandle, const UA_NodeId *nodeId)
 {
	 size_t i = findNSHandle(pSwitch, nsHandle);
	 if(i==pSwitch->size)
	 	 return NULL;
	 return pSwitch->nodestoreArray[i]->getNode(pSwitch->nodestoreArray[i]->context, nodeId);
 }



 UA_StatusCode UA_NodestoreSwitch_getNodeCopy_inNS(UA_NodestoreSwitch *pSwitch, void *nsHandle, const UA_NodeId *nodeId, UA_Node **outNode)
 {
	 size_t i = findNSHandle(pSwitch, nsHandle);
	 if(i==pSwitch->size)
		 return UA_STATUSCODE_BADNOTFOUND;
	 return pSwitch->nodestoreArray[i]->getNodeCopy(pSwitch->nodestoreArray[i]->context, nodeId, outNode);

 }


 UA_StatusCode UA_NodestoreSwitch_replaceNode_inNS(UA_NodestoreSwitch *pSwitch, void *nsHandle, UA_Node *node)
 {
	 size_t i = findNSHandle(pSwitch, nsHandle);
	 if(i==pSwitch->size)
 		return UA_STATUSCODE_BADNOTFOUND;
    return pSwitch->nodestoreArray[i]->replaceNode(pSwitch->nodestoreArray[i]->context, node);
 }


 UA_StatusCode UA_NodestoreSwitch_removeNode_inNS(UA_NodestoreSwitch *pSwitch, void *nsHandle, const UA_NodeId *nodeId)
  {
	 size_t i = findNSHandle(pSwitch, nsHandle);
	 if(i==pSwitch->size)
  		return UA_STATUSCODE_BADNOTFOUND;
     return pSwitch->nodestoreArray[i]->removeNode(pSwitch->nodestoreArray[i]->context, nodeId);
  }


 void UA_NodestoreSwitch_releaseNode_inNS(UA_NodestoreSwitch *pSwitch, void *nsHandle, UA_Node *node)
  {
	 size_t i = findNSHandle(pSwitch, nsHandle);
	 if(i==pSwitch->size)
		 return;
    pSwitch->nodestoreArray[i]->releaseNode(pSwitch->nodestoreArray[i]->context, node);
  }

void UA_NodestoreSwitch_deleteSwitch(UA_NodestoreSwitch *pSwitch)
{
	UA_NodestoreSwitch_deleteNodestores((void*) pSwitch);
	UA_free(pSwitch);
}


 /* Standard Nodestorefunctions */


void UA_NodestoreSwitch_deleteNode(void *switchHandle, UA_Node *node)
{
	UA_NodestoreSwitch *pSwitch = switchHandle;
	size_t i,j=0;
	for(i=0; i<pSwitch->size; i++)
	{
		j = findNSHandle(pSwitch, pSwitch->nodestoreArray[i]->context);
		if(j != pSwitch->size)
		{
			pSwitch->nodestoreArray[j]->deleteNode(pSwitch->nodestoreArray[j]->context, node);
		}
	}
}

UA_StatusCode UA_NodestoreSwitch_insertNode(void *switchHandle, UA_Node *node, UA_NodeId *addedNodeId)
{
	UA_NodestoreSwitch *pSwitch = switchHandle;
	if (node->nodeId.namespaceIndex < pSwitch->size){
		if (pSwitch->nodestoreArray[node->nodeId.namespaceIndex] != NULL){
			return (pSwitch->nodestoreArray[node->nodeId.namespaceIndex]->insertNode(pSwitch->nodestoreArray[node->nodeId.namespaceIndex]->context, node, addedNodeId));
		}
	}
	return pSwitch->defaultNodestore->insertNode(pSwitch->defaultNodestore->context, node, addedNodeId);
}

const UA_Node *UA_NodestoreSwitch_getNode(void *switchHandle, const UA_NodeId *nodeId)
{
	UA_NodestoreSwitch *pSwitch = switchHandle;
	if (nodeId->namespaceIndex < pSwitch->size){
		if (pSwitch->nodestoreArray[nodeId->namespaceIndex] != NULL){
			return (pSwitch->nodestoreArray[nodeId->namespaceIndex]->getNode(pSwitch->nodestoreArray[nodeId->namespaceIndex]->context,nodeId));
		}
	}
	return (const UA_Node*) pSwitch->defaultNodestore->getNode(pSwitch->defaultNodestore->context, nodeId);
}

UA_StatusCode UA_NodestoreSwitch_getNodeCopy(void *switchHandle, const UA_NodeId *nodeId, UA_Node **outNode)
{
	UA_NodestoreSwitch *pSwitch = switchHandle;
	if (nodeId->namespaceIndex < pSwitch->size){
		if (pSwitch->nodestoreArray[nodeId->namespaceIndex] != NULL){
			return pSwitch->nodestoreArray[nodeId->namespaceIndex]->getNodeCopy(pSwitch->nodestoreArray[nodeId->namespaceIndex]->context, nodeId, outNode);
		}
	}
	return pSwitch->defaultNodestore->getNodeCopy(pSwitch->defaultNodestore->context, nodeId, outNode);
}

UA_StatusCode UA_NodestoreSwitch_replaceNode(void *switchHandle, UA_Node *node)
{
	UA_NodestoreSwitch *pSwitch = switchHandle;
	if (node->nodeId.namespaceIndex < pSwitch->size){
		if (pSwitch->nodestoreArray[node->nodeId.namespaceIndex] != NULL){
			return pSwitch->nodestoreArray[node->nodeId.namespaceIndex]->replaceNode(pSwitch->nodestoreArray[node->nodeId.namespaceIndex]->context, node);
		}
	}
	return pSwitch->defaultNodestore->replaceNode(pSwitch->defaultNodestore->context, node);
}

UA_StatusCode UA_NodestoreSwitch_removeNode(void *switchHandle, const UA_NodeId *nodeId)
{
	UA_NodestoreSwitch *pSwitch = switchHandle;
	if (nodeId->namespaceIndex < pSwitch->size){
		if (pSwitch->nodestoreArray[nodeId->namespaceIndex] != NULL){
			return pSwitch->nodestoreArray[nodeId->namespaceIndex]->removeNode(pSwitch->nodestoreArray[nodeId->namespaceIndex]->context, nodeId);
		}
	}
	return pSwitch->defaultNodestore->removeNode(pSwitch->defaultNodestore->context, nodeId);
}

void UA_NodestoreSwitch_releaseNode(void *switchHandle, const UA_Node *node)
{
	UA_NodestoreSwitch *pSwitch = switchHandle;
	if (node->nodeId.namespaceIndex < pSwitch->size){
		if (pSwitch->nodestoreArray[node->nodeId.namespaceIndex] != NULL){
			return pSwitch->nodestoreArray[node->nodeId.namespaceIndex]->releaseNode(pSwitch->nodestoreArray[node->nodeId.namespaceIndex]->context, node);
		}
	}
	return pSwitch->defaultNodestore->releaseNode(pSwitch->defaultNodestore->context, node);
}

 void UA_NodestoreSwitch_deleteNodestores(void *switchHandle)
 {

	UA_NodestoreSwitch *pSwitch = switchHandle;
 	for(size_t i=0; i< pSwitch->size; i++)
 	{
 		pSwitch->nodestoreArray[i]->deleteNodestore(pSwitch->nodestoreArray[i]->context);
 		UA_free(&pSwitch->nodestoreArray[i]);
 	}
 }


  UA_Node *UA_NodestoreSwitch_newNode(void *switchHandle, UA_NodeClass nodeClass)
  {
	//todo: NOT IMPLEMENTED Cause of, not clear in which nodestore the memory has to be allocated. (switch or defaultNodestore)
 	UA_NodestoreSwitch *pSwitch = switchHandle;
 	return pSwitch->defaultNodestore->newNode(pSwitch->defaultNodestore->context, nodeClass);
	//  return NULL;
  }

  void UA_NodestoreSwitch_iterate(void *switchHandle, void *visitorContext, UA_NodestoreVisitor visitor)
  {
  	UA_NodestoreSwitch *pSwitch = switchHandle;
  	UA_Nodestore *tempArray[pSwitch->size];
  	for(size_t i=0; i<pSwitch->size; i++)
  	{
  		for(size_t j=0; j<=i; j++)
  		{
  			if(tempArray[j] == pSwitch->nodestoreArray[i])
  				break;
  			else if(j==i)
  			{
  				tempArray[i] = pSwitch->nodestoreArray[i];
  				pSwitch->nodestoreArray[i]->iterate(pSwitch, visitorContext, visitor);
  			}
  		}
  	}

  }
