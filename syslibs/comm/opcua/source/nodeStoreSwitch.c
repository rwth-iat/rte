
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



UA_StatusCode UA_NodestoreSwitch_linkDefaultNodestore(UA_NodestoreSwitch *pSwitch, UA_Nodestore *ns, UA_UInt16 namespaceindex)
{
	if(ns == NULL)
		return UA_STATUSCODE_BADNOTFOUND;
	UA_NodestoreSwitch_linkNodestore(pSwitch, ns, namespaceindex);
	pSwitch->defaultNodestore = ns;
	return (pSwitch->nodestoreArray[namespaceindex]->context != NULL) ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADINTERNALERROR;

}


UA_StatusCode UA_NodestoreSwitch_linkNodestore(UA_NodestoreSwitch *pSwitch,
		UA_Nodestore *ns, UA_UInt16 namespaceindex)
{
	if(pSwitch->nodestoreArray[namespaceindex] != NULL)
		return UA_STATUSCODE_BADNODEIDEXISTS;
	pSwitch->size = pSwitch->size + 1;
	if(pSwitch->size > 1)
	{
		pSwitch->nodestoreArray = realloc(pSwitch->nodestoreArray , pSwitch->size * sizeof *pSwitch->nodestoreArray);
		if(!&pSwitch->nodestoreArray[pSwitch->size-1])
			return UA_STATUSCODE_BADOUTOFMEMORY;
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
	return (pSwitch->nodestoreArray[namespaceindex]->context != NULL) ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADINTERNALERROR;

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
	return pSwitch->defaultNodestore->insertNode(pSwitch->defaultNodestore->context, node, addedNodeId);
}

const UA_Node *UA_NodestoreSwitch_getNode(void *switchHandle, const UA_NodeId *nodeId)
{
	 UA_NodestoreSwitch *pSwitch = (UA_NodestoreSwitch*)switchHandle;			//bekommt context von Nodestore nicht nodestore selbst
	 for(size_t i=0; i< pSwitch->size; i++)
	 {
		 if(pSwitch->nodestoreArray[i]->getNode(pSwitch->nodestoreArray[i]->context, nodeId))
			 return pSwitch->nodestoreArray[i]->getNode(pSwitch->nodestoreArray[i]->context, nodeId);

	 }
	 return NULL;
}

UA_StatusCode UA_NodestoreSwitch_getNodeCopy(void *switchHandle, const UA_NodeId *nodeId, UA_Node **outNode)
{
	 UA_NodestoreSwitch *pSwitch = switchHandle;
	 for(size_t i=0; i<pSwitch->size; i++)
	 {
		if(pSwitch->nodestoreArray[i]->getNodeCopy(pSwitch->nodestoreArray[i]->context, nodeId, outNode))
			return pSwitch->nodestoreArray[i]->getNodeCopy(pSwitch->nodestoreArray[i]->context, nodeId, outNode);
	 }
	 return UA_STATUSCODE_BADNOTFOUND;
}

UA_StatusCode UA_NodestoreSwitch_replaceNode(void *switchHandle, UA_Node *node)
{
	 UA_NodestoreSwitch *pSwitch = switchHandle;
	 UA_StatusCode retval = UA_STATUSCODE_BADNOTFOUND;
	 for(size_t i=0; i<pSwitch->size; i++)
	 {
		 retval = pSwitch->nodestoreArray[i]->replaceNode(pSwitch->nodestoreArray[i]->context, node);
		 if(retval == UA_STATUSCODE_GOOD)
			 break;
	 }
	 return retval;
}

UA_StatusCode UA_NodestoreSwitch_removeNode(void *switchHandle, const UA_NodeId *nodeId)
{
	UA_NodestoreSwitch *pSwitch = switchHandle;
	UA_StatusCode retval = UA_STATUSCODE_BADNOTFOUND;
	for(size_t i=0; i<pSwitch->size; i++)
	{
		retval = pSwitch->nodestoreArray[i]->removeNode(pSwitch->nodestoreArray[i]->context, nodeId);
		if(retval == UA_STATUSCODE_GOOD)
			break;
	}
	return retval;
}

void UA_NodestoreSwitch_releaseNode(void *switchHandle, const UA_Node *node)
{
	 UA_NodestoreSwitch *pSwitch = switchHandle;
	 for(size_t i=0; i<pSwitch->size; i++)
	 {
		pSwitch->nodestoreArray[i]->releaseNode(pSwitch->nodestoreArray[i]->context, node);
	 }
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
 	UA_NodestoreSwitch *pSwitch = switchHandle;
 	return pSwitch->defaultNodestore->newNode(pSwitch->defaultNodestore->context, nodeClass);
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
