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
	 if(pSwitch->nodestoreArray[i].context == nsHandle)
	 {
	   return i;
	 }
	}
	return i;
}

UA_NodestoreSwitch *UA_NodestoreSwitch_new(UA_Nodestore *ns)
{
	//UA_NodestoreSwitch *pSwitch = UA_malloc(sizeof *pSwitch + 1 * sizeof *pSwitch->nodestoreArray);
	UA_NodestoreSwitch *pSwitch = UA_malloc(sizeof *pSwitch);
	pSwitch->nodestoreArray = UA_malloc(sizeof *pSwitch->nodestoreArray);
	pSwitch->size = 0;
	if(!pSwitch)
			return NULL;
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
	ns->removeNode = UA_NodestoreSwitch_removeNode;
	ns->iterate = UA_NodestoreSwitch_iterate;
	pSwitch->size = 0;
	UA_StatusCode retval = UA_Nodestore_default_new(&pSwitch->nodestoreArray[0]);
    if(retval != UA_STATUSCODE_GOOD)
	    	return NULL;
	else
	    UA_NodestoreSwitch_linkNodestore(pSwitch, &pSwitch->nodestoreArray[0]);
	return pSwitch;
}



UA_StatusCode UA_NodestoreSwitch_linkNodestore(UA_NodestoreSwitch *pSwitch,
		UA_Nodestore *ns)
{
	pSwitch->size = pSwitch->size + 1;
	if(pSwitch->size > 1)
		pSwitch->nodestoreArray = realloc(pSwitch->nodestoreArray , pSwitch->size * sizeof *pSwitch->nodestoreArray);
	if(!&pSwitch->nodestoreArray[pSwitch->size-1])
		return UA_STATUSCODE_BADOUTOFMEMORY;

	pSwitch->nodestoreArray[pSwitch->size-1].context = ns->context ;	//todo: muss context von neuem Nodestore zugwiesen werden?
	pSwitch->nodestoreArray[pSwitch->size-1].deleteNodestore = ns->deleteNodestore;
	pSwitch->nodestoreArray[pSwitch->size-1].inPlaceEditAllowed = ns->inPlaceEditAllowed;
	pSwitch->nodestoreArray[pSwitch->size-1].newNode = ns->newNode;
	pSwitch->nodestoreArray[pSwitch->size-1].deleteNode = ns->deleteNode;
	pSwitch->nodestoreArray[pSwitch->size-1].getNode = ns->getNode;
	pSwitch->nodestoreArray[pSwitch->size-1].releaseNode = ns->releaseNode;
	pSwitch->nodestoreArray[pSwitch->size-1].getNodeCopy = ns->getNodeCopy;
	pSwitch->nodestoreArray[pSwitch->size-1].insertNode = ns->insertNode;
	pSwitch->nodestoreArray[pSwitch->size-1].replaceNode = ns->replaceNode;
	pSwitch->nodestoreArray[pSwitch->size-1].removeNode = ns->removeNode;
	pSwitch->nodestoreArray[pSwitch->size-1].iterate = ns->iterate;

	return (pSwitch->nodestoreArray[pSwitch->size-1].context != NULL) ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADINTERNALERROR;

}



UA_StatusCode UA_NodestoreSwitch_unlinkNodestore(UA_NodestoreSwitch *pSwitch,
		void *nsHandle) {
	int i = findNSHandle(pSwitch, nsHandle);
	if(i == 0 || i == pSwitch->size)
		return UA_STATUSCODE_BADNOTFOUND;
	pSwitch->nodestoreArray[i].deleteNodestore(pSwitch->nodestoreArray[i].context);
	UA_free(&pSwitch->nodestoreArray[i]);
	pSwitch->size = pSwitch->size -1;
	//Array um 1 nach vorne damit keine luecken
	for(--i; &pSwitch->nodestoreArray[i] == NULL; i++)
	{

    }
	if(pSwitch->size > 1)
		pSwitch->size = pSwitch->size - 1;
	pSwitch = realloc(pSwitch, sizeof *pSwitch + pSwitch->size * sizeof *pSwitch->nodestoreArray);
	return (&pSwitch->nodestoreArray[--i] != NULL) ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADINTERNALERROR;
}


UA_StatusCode UA_NodestoreSwitch_changeNodestore(UA_NodestoreSwitch *pSwitch,
		void *nsHandleOut, void *nsHandleIn) {
	int i= findNSHandle(pSwitch, nsHandleOut);
	if(i == 0)
		return UA_STATUSCODE_BADNOTFOUND;
	pSwitch->nodestoreArray[i].context = nsHandleIn;
	return UA_STATUSCODE_GOOD;

}


 UA_Node *UA_NodestoreSwitch_newNode_inNS(UA_NodestoreSwitch *pSwitch, void *nsHandle, UA_NodeClass nodeClass)
  {
	int i = findNSHandle(pSwitch, nsHandle);
	if(i == pSwitch->size)
		return NULL;
	return pSwitch->nodestoreArray[i].newNode(pSwitch->nodestoreArray[i].context, nodeClass);
  }



 void UA_NodestoreSwitch_deleteNode_inNS(UA_NodestoreSwitch *pSwitch, void *nsHandle, UA_Node *node)
 {
	 int i = findNSHandle(pSwitch, nsHandle);
	 if(i == pSwitch->size)
		 return;
	 pSwitch->nodestoreArray[i].deleteNode(pSwitch->nodestoreArray[i].context, node);
 }


 UA_StatusCode UA_NodestoreSwitch_insertNode_inNS(UA_NodestoreSwitch *pSwitch, void *nsHandle, UA_Node *node,
		 UA_NodeId *addedNodeId)
 {
	 int i = findNSHandle(pSwitch, nsHandle);
	 if(i == pSwitch->size)
		 return UA_STATUSCODE_BADNOTFOUND;
	 return pSwitch->nodestoreArray[i].insertNode(pSwitch->nodestoreArray[i].context, node, addedNodeId);
 }


 const UA_Node *UA_NodestoreSwitch_getNode_inNS(UA_NodestoreSwitch *pSwitch, void *nsHandle, const UA_NodeId *nodeId)
 {
	 int i = findNSHandle(pSwitch, nsHandle);
	 if(i == pSwitch->size)
		 return NULL;
	 return pSwitch->nodestoreArray[i].getNode(pSwitch->nodestoreArray[i].context, nodeId);
 }



 UA_StatusCode UA_NodestoreSwitch_getNodeCopy_inNS(UA_NodestoreSwitch *pSwitch, void *nsHandle, const UA_NodeId *nodeId, UA_Node **outNode)
 {
	 int i = findNSHandle(pSwitch, nsHandle);
	 return pSwitch->nodestoreArray[i].getNodeCopy(pSwitch->nodestoreArray[i].context, nodeId, outNode);

 }


 UA_StatusCode UA_NodestoreSwitch_replaceNode_inNS(UA_NodestoreSwitch *pSwitch, void *nsHandle, UA_Node *node)
 {
 	int i = findNSHandle(pSwitch, nsHandle);
 	if(i == pSwitch->size)
 		return UA_STATUSCODE_BADNOTFOUND;
    return pSwitch->nodestoreArray[i].replaceNode(pSwitch->nodestoreArray[i].context, node);
 }


 UA_StatusCode UA_NodestoreSwitch_removeNode_inNS(UA_NodestoreSwitch *pSwitch, void *nsHandle, const UA_NodeId *nodeId)
  {
  	int i = findNSHandle(pSwitch, nsHandle);
  	if(i == pSwitch->size)
  		return UA_STATUSCODE_BADNOTFOUND;
     return pSwitch->nodestoreArray[i].removeNode(pSwitch->nodestoreArray[i].context, nodeId);
  }


 void UA_NodestoreSwitch_releaseNode_inNS(UA_NodestoreSwitch *pSwitch, void *nsHandle, UA_Node *node)
  {
  	int i = findNSHandle(pSwitch, nsHandle);
  	if(i == pSwitch->size)
  		return;
    pSwitch->nodestoreArray[i].releaseNode(pSwitch->nodestoreArray[i].context, node);
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
	int i,j=0;
	for(i=0; i<pSwitch->size; i++)
	{
		j = findNSHandle(pSwitch, pSwitch->nodestoreArray[i].context);
		if(j != pSwitch->size)
		{
			pSwitch->nodestoreArray[j].deleteNode(pSwitch->nodestoreArray[j].context, node);
		}
	}
}

UA_StatusCode UA_NodestoreSwitch_insertNode(void *switchHandle, UA_Node *node, UA_NodeId *addedNodeId)
{
	UA_NodestoreSwitch *pSwitch = switchHandle;
	return pSwitch->nodestoreArray[0].insertNode(pSwitch->nodestoreArray[0].context, node, addedNodeId);
}

const UA_Node *UA_NodestoreSwitch_getNode(void *switchHandle, const UA_NodeId *nodeId)
{
	 UA_NodestoreSwitch *pSwitch = (UA_NodestoreSwitch*)switchHandle;			//bekommt context von Nodestore nicht nodestore selbst
	 int i;
	 for(i=0; i< pSwitch->size; i++)
	 {
		 if(pSwitch->nodestoreArray[i].getNode(pSwitch->nodestoreArray[i].context, nodeId))
			 return pSwitch->nodestoreArray[i].getNode(pSwitch->nodestoreArray[i].context, nodeId);

	 }
	 return NULL;
}

UA_StatusCode UA_NodestoreSwitch_getNodeCopy(void *switchHandle, const UA_NodeId *nodeId, UA_Node **outNode)
{
	 UA_NodestoreSwitch *pSwitch = switchHandle;
	 int i;
	 for(i=0; i<pSwitch->size; i++)
	 {
		if(pSwitch->nodestoreArray[i].getNodeCopy(pSwitch->nodestoreArray[i].context, nodeId, outNode))
			return pSwitch->nodestoreArray[i].getNodeCopy(pSwitch->nodestoreArray[i].context, nodeId, outNode);
	 }
	 return UA_STATUSCODE_BADNOTFOUND;
}

UA_StatusCode UA_NodestoreSwitch_replaceNode(void *switchHandle, UA_Node *node)
{
	 UA_NodestoreSwitch *pSwitch = switchHandle;
	 int i;
	 UA_StatusCode retval = UA_STATUSCODE_BADNOTFOUND;
	 for(i=0; i<pSwitch->size; i++)
	 {
		 retval = pSwitch->nodestoreArray[i].replaceNode(pSwitch->nodestoreArray[i].context, node);
		 if(retval == UA_STATUSCODE_GOOD)
			 break;
	 }
	 return retval;
}

UA_StatusCode UA_NodestoreSwitch_removeNode(void *switchHandle, const UA_NodeId *nodeId)
{
	UA_NodestoreSwitch *pSwitch = switchHandle;
	int i;
	UA_StatusCode retval = UA_STATUSCODE_BADNOTFOUND;
	for(i=0; i<pSwitch->size; i++)
	{
		retval = pSwitch->nodestoreArray[i].removeNode(pSwitch->nodestoreArray[i].context, nodeId);
		if(retval == UA_STATUSCODE_GOOD)
			break;
	}
	return retval;
}

void UA_NodestoreSwitch_releaseNode(void *switchHandle, const UA_Node *node)
{
	 UA_NodestoreSwitch *pSwitch = switchHandle;
	 int i;
	 for(i=0; i<pSwitch->size; i++)
	 {
		pSwitch->nodestoreArray[i].releaseNode(pSwitch->nodestoreArray[i].context, node);
	 }
}

 void UA_NodestoreSwitch_deleteNodestores(void *switchHandle)
 {
	UA_NodestoreSwitch *pSwitch = switchHandle;
 	for(int i=0; i< pSwitch->size; i++)
 	{
 		pSwitch->nodestoreArray[i].deleteNodestore(pSwitch->nodestoreArray[i].context);
 		UA_free(&pSwitch->nodestoreArray[i]);
 	}
 }


  UA_Node *UA_NodestoreSwitch_newNode(void *switchHandle, UA_NodeClass nodeClass)
  {
 	UA_NodestoreSwitch *pSwitch = switchHandle;
 	return pSwitch->nodestoreArray[0].newNode(pSwitch->nodestoreArray[0].context, nodeClass);
  }

  void UA_NodestoreSwitch_iterate(void *switchHandle, void *visitorContext, UA_NodestoreVisitor visitor)
  {
  	UA_NodestoreSwitch *pSwitch = switchHandle;
  	int i = findNSHandle(pSwitch, visitorContext);
  	if(i == pSwitch->size)
  		return;
  	pSwitch->nodestoreArray[i].iterate(pSwitch, pSwitch->nodestoreArray[i].context, visitor);
  }
