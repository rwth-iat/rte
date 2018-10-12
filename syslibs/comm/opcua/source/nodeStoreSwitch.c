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



static int findNSHandle(UA_NodestoreSwitch *pSwitch, void *nsHandle)
{
	int i;
	for(i=0; i<pSwitch->size; i++)
	{
	 if(pSwitch->nodestoreArray[i]->context == nsHandle)
	 {
	   return i;
	 }
	}
	return i;
}



UA_StatusCode UA_NodestoreSwitch_init(UA_NodestoreSwitch *pSwitch)
{
	pSwitch->size = 0;
	pSwitch->nodestoreArray[0] = (UA_Nodestore*) UA_malloc(1 * sizeof(UA_Nodestore));
	UA_StatusCode retval = UA_Nodestore_default_new(pSwitch->nodestoreArray[0]);

    if(retval != UA_STATUSCODE_GOOD)
    {
    	UA_free(pSwitch);
    	return UA_STATUSCODE_BADOUTOFMEMORY;
    }

    else
    	UA_NodestoreSwitch_linkNodestore(pSwitch, pSwitch->nodestoreArray[0]);
    return UA_STATUSCODE_GOOD;

}




UA_StatusCode UA_NodestoreSwitch_linkNodestore(UA_NodestoreSwitch *pSwitch,
		UA_Nodestore *ns)
{
	pSwitch->size = pSwitch->size + 1;
	if(pSwitch->size > 1)
		pSwitch->nodestoreArray[pSwitch->size-1] = (UA_Nodestore*) UA_malloc(sizeof(UA_Nodestore));
	if(!pSwitch->nodestoreArray[pSwitch->size-1])
		return UA_STATUSCODE_BADOUTOFMEMORY;

	ns->context =  pSwitch->nodestoreArray[pSwitch->size-1]->context;
	ns->deleteNodestore = pSwitch->nodestoreArray[pSwitch->size-1]->deleteNodestore;
	ns->inPlaceEditAllowed = pSwitch->nodestoreArray[pSwitch->size-1]->inPlaceEditAllowed;
	ns->newNode = pSwitch->nodestoreArray[pSwitch->size-1]->newNode;
	ns->deleteNode = pSwitch->nodestoreArray[pSwitch->size-1]->deleteNode;
	ns->getNode = pSwitch->nodestoreArray[pSwitch->size-1]->getNode;
	ns->releaseNode = pSwitch->nodestoreArray[pSwitch->size-1]->releaseNode;
	ns->getNodeCopy = pSwitch->nodestoreArray[pSwitch->size-1]->getNodeCopy;
	ns->insertNode = pSwitch->nodestoreArray[pSwitch->size-1]->insertNode;
	ns->replaceNode = pSwitch->nodestoreArray[pSwitch->size-1]->replaceNode;
	ns->removeNode = pSwitch->nodestoreArray[pSwitch->size-1]->removeNode;
	ns->iterate = pSwitch->nodestoreArray[pSwitch->size-1]->iterate;

	return (pSwitch->nodestoreArray[pSwitch->size-1]->context != NULL) ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADINTERNALERROR;

}

void UA_NodestoreSwitch_iterate(UA_NodestoreSwitch *pSwitch, void* visitorContext, UA_NodestoreVisitor visitor)
{
	int i = findNSHandle(pSwitch, visitorContext);
	if(i == pSwitch->size)
		return;
	pSwitch->nodestoreArray[i]->iterate(pSwitch, pSwitch->nodestoreArray[i]->context, visitor);
}

UA_StatusCode UA_NodestoreSwitch_unlinkNodestore(UA_NodestoreSwitch *pSwitch,
		void *nsHandle) {
	int i = findNSHandle(pSwitch, nsHandle);
	if(i == 0 || i == pSwitch->size)
		return UA_STATUSCODE_BADNOTFOUND;
	pSwitch->nodestoreArray[i]->deleteNodestore(pSwitch->nodestoreArray[i]->context);
	UA_free(pSwitch->nodestoreArray[i]);
	//Array um 1 nach vorne damit keine luecken
	for(--i;pSwitch->nodestoreArray[i] == NULL; i++)
	{

    }
	if(pSwitch->size > 1)
		pSwitch->size = pSwitch->size - 1;
	return (pSwitch->nodestoreArray[--i] != NULL) ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADINTERNALERROR;
}


UA_StatusCode UA_NodestoreSwitch_changeNodestore(UA_NodestoreSwitch *pSwitch,
		void *nsHandleOut, void *nsHandleIn) {
	int i= findNSHandle(pSwitch, nsHandleOut);
	if(i == 0)
		return UA_STATUSCODE_BADNOTFOUND;
	//TODO:
	//ptrSwitch->nodestoreArray[i]->iterate  Nodes von altem Nodestore zu Neuem hinzufügen
	UA_NodestoreSwitch_unlinkNodestore(pSwitch, nsHandleOut);
	return UA_STATUSCODE_GOOD;

}


/*
 * NodestoreInterface Function routing
*/
UA_StatusCode UA_NodestoreSwitch_deleteNodestoreSwitch(UA_NodestoreSwitch *pSwitch)
{
	for(int i=0; i< pSwitch->size; i++)
	{
		pSwitch->nodestoreArray[i]->deleteNodestore(pSwitch->nodestoreArray[i]->context);
		UA_free(pSwitch->nodestoreArray[i]);
	}
	UA_free(pSwitch);
	return UA_STATUSCODE_GOOD;
}


 UA_Node *UA_NodestoreSwitch_newNode(UA_NodestoreSwitch *pSwitch, UA_NodeClass nodeClass)
 {
	return pSwitch->nodestoreArray[0]->newNode(pSwitch->nodestoreArray[0]->context, nodeClass);
 }

 UA_Node *UA_NodestoreSwitch_newNode_inNS(UA_NodestoreSwitch *pSwitch, void *nsHandle, UA_NodeClass nodeClass)
  {
	int i = findNSHandle(pSwitch, nsHandle);
	if(i == pSwitch->size)
		return NULL;
	return pSwitch->nodestoreArray[i]->newNode(pSwitch->nodestoreArray[i]->context, nodeClass);
  }

 void UA_NodestoreSwitch_deleteNode(UA_NodestoreSwitch *pSwitch, UA_Node *node)
 {
	int i,j=0;
	for(i=0; i<pSwitch->size; i++)
	{
		j = findNSHandle(pSwitch, pSwitch->nodestoreArray[i]->context);
		if(j != pSwitch->size)
		{
			pSwitch->nodestoreArray[j]->deleteNode(pSwitch->nodestoreArray[j]->context, node);
		}
	}
 }

 void UA_NodestoreSwitch_deleteNode_inNS(UA_NodestoreSwitch *pSwitch, void *nsHandle, UA_Node *node)
 {
	 int i = findNSHandle(pSwitch, nsHandle);
	 if(i == pSwitch->size)
		 return;
	 pSwitch->nodestoreArray[i]->deleteNode(pSwitch->nodestoreArray[i]->context, node);
 }

UA_StatusCode UA_NodestoreSwitch_insertNode(UA_NodestoreSwitch *pSwitch, UA_Node *node, UA_NodeId *addedNodeId)
{

	return pSwitch->nodestoreArray[0]->insertNode(pSwitch->nodestoreArray[0]->context, node, addedNodeId);
}


 UA_StatusCode UA_NodestoreSwitch_insertNode_inNS(UA_NodestoreSwitch *pSwitch, void *nsHandle, UA_Node *node,
		 UA_NodeId *addedNodeId)
 {
	 int i = findNSHandle(pSwitch, nsHandle);
	 if(i == pSwitch->size)
		 return UA_STATUSCODE_BADNOTFOUND;
	 return pSwitch->nodestoreArray[i]->insertNode(pSwitch->nodestoreArray[i]->context, node, addedNodeId);
 }

 const UA_Node *UA_NodestoreSwitch_getNode(UA_NodestoreSwitch *pSwitch, const UA_NodeId *nodeId)
 {
	 int i;
	 for(i=0; i< pSwitch->size; i++)
	 {
		 if(pSwitch->nodestoreArray[i]->getNode(pSwitch->nodestoreArray[i]->context, nodeId))
			 return pSwitch->nodestoreArray[i]->getNode(pSwitch->nodestoreArray[i]->context, nodeId);

	 }
	 return NULL;

 }

 const UA_Node *UA_NodestoreSwitch_getNode_inNS(UA_NodestoreSwitch *pSwitch, void *nsHandle, const UA_NodeId *nodeId)
 {
	 int i = findNSHandle(pSwitch, nsHandle);
	 if(i == pSwitch->size)
		 return NULL;
	 return pSwitch->nodestoreArray[i]->getNode(pSwitch->nodestoreArray[i]->context, nodeId);
 }



 UA_StatusCode UA_NodestoreSwitch_getNodeCopy(UA_NodestoreSwitch *pSwitch, const UA_NodeId *nodeId, UA_Node **outNode)
 {
	 int i;
	 for(i=0; i<pSwitch->size; i++)
	 {
		if(pSwitch->nodestoreArray[i]->getNodeCopy(pSwitch->nodestoreArray[i]->context, nodeId, outNode))
			return pSwitch->nodestoreArray[i]->getNodeCopy(pSwitch->nodestoreArray[i]->context, nodeId, outNode);
	 }
	 return UA_STATUSCODE_BADNOTFOUND;
 }

 UA_StatusCode UA_NodestoreSwitch_getNodeCopy_inNS(UA_NodestoreSwitch *pSwitch, void *nsHandle, const UA_NodeId *nodeId, UA_Node **outNode)
 {
	 int i = findNSHandle(pSwitch, nsHandle);
	 return pSwitch->nodestoreArray[i]->getNodeCopy(pSwitch->nodestoreArray[i]->context, nodeId, outNode);

 }


 UA_StatusCode UA_NodestoreSwitch_replaceNode(UA_NodestoreSwitch *pSwitch, UA_Node *node)
 {
	 int i;
	 UA_StatusCode retval = UA_STATUSCODE_BADNOTFOUND;
	 for(i=0; i<pSwitch->size; i++)
	 {
		 retval = pSwitch->nodestoreArray[i]->replaceNode(pSwitch->nodestoreArray[i]->context, node);
		 if(retval == UA_STATUSCODE_GOOD)
			 break;
	 }
	 return retval;
 }

 UA_StatusCode UA_NodestoreSwitch_replaceNode_inNS(UA_NodestoreSwitch *pSwitch, void *nsHandle, UA_Node *node)
 {
 	int i = findNSHandle(pSwitch, nsHandle);
 	if(i == pSwitch->size)
 		return UA_STATUSCODE_BADNOTFOUND;
    return pSwitch->nodestoreArray[i]->replaceNode(pSwitch->nodestoreArray[i]->context, node);
 }

 UA_StatusCode UA_NodestoreSwitch_removeNode(UA_NodestoreSwitch *pSwitch, const UA_NodeId *nodeId)
 {
	int i;
	UA_StatusCode retval = UA_STATUSCODE_BADNOTFOUND;
	for(i=0; i<pSwitch->size; i++)
	{
		retval = pSwitch->nodestoreArray[i]->removeNode(pSwitch->nodestoreArray[i]->context, nodeId);
		if(retval == UA_STATUSCODE_GOOD)
			break;
	}
	return retval;
 }

 UA_StatusCode UA_NodestoreSwitch_removeNode_inNS(UA_NodestoreSwitch *pSwitch, void *nsHandle, const UA_NodeId *nodeId)
  {
  	int i = findNSHandle(pSwitch, nsHandle);
  	if(i == pSwitch->size)
  		return UA_STATUSCODE_BADNOTFOUND;
     return pSwitch->nodestoreArray[i]->removeNode(pSwitch->nodestoreArray[i]->context, nodeId);
  }

 void UA_NodestoreSwitch_releaseNode(UA_NodestoreSwitch *pSwitch, const UA_Node *node)
 {
	 int i;
	 for(i=0; i<pSwitch->size; i++)
	 {
		pSwitch->nodestoreArray[i]->releaseNode(pSwitch->nodestoreArray[i]->context, node);
	 }
 }

 void UA_NodestoreSwitch_releaseNode_inNS(UA_NodestoreSwitch *pSwitch, void *nsHandle, UA_Node *node)
  {
  	int i = findNSHandle(pSwitch, nsHandle);
  	if(i == pSwitch->size)
  		return;
    pSwitch->nodestoreArray[i]->releaseNode(pSwitch->nodestoreArray[i]->context, node);
  }

