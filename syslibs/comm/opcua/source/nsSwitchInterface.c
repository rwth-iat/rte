#include "nsSwitchInterface.h"

 //typedef void (*UA_NodestoreVisitor)(void *visitorContext, const UA_Node *node);


UA_StatusCode UA_NoedstoreSwitch_new(UA_Nodestore *ns)
{
	UA_NodestoreSwitch *pSwitch = UA_malloc(sizeof *pSwitch);
	if(!pSwitch)
		return UA_STATUSCODE_BADOUTOFMEMORY;
	UA_StatusCode retval = UA_NodestoreSwitch_init(pSwitch);
	ns->context = pSwitch;
    ns->deleteNodestore = UA_NodestoreSwitchInterface_deleteNodestore;
	ns->inPlaceEditAllowed = true;
	ns->newNode = UA_NodestoreSwitchInterface_newNode;
	ns->deleteNode = UA_NodestoreSwitchInterface_deleteNode;
	ns->getNode = UA_NodestoreSwitchInterface_getNode;
	ns->releaseNode = UA_NodestoreSwitchInterface_releaseNode;
	ns->getNodeCopy = UA_NodestoreSwitchInterface_getNodeCopy;
	ns->insertNode = UA_NodestoreSwitchInterface_insertNode;
	ns->replaceNode = UA_NodestoreSwitchInterface_replaceNode;
	ns->removeNode = UA_NodestoreSwitchInterface_removeNode;
	ns->iterate = UA_NodestoreSwitchInterface_iterate;
	return retval;

}

// *nodestoreContext ist Pointer auf NodestoreSwitch

void UA_NodestoreSwitchInterface_deleteNodestore(void *nodestoreContext)
{
	UA_NodestoreSwitch_deleteNodestoreSwitch(nodestoreContext);
}

UA_Node *UA_NodestoreSwitchInterface_newNode(void *nodestoreContext, UA_NodeClass nodeClass)
{
	return UA_NodestoreSwitch_newNode(nodestoreContext, nodeClass);
}

void UA_NodestoreSwitchInterface_deleteNode(void *nodestoreContext, UA_Node *node)
{
	UA_NodestoreSwitch_deleteNode(nodestoreContext, node);
}

const UA_Node * UA_NodestoreSwitchInterface_getNode(void *nodestoreContext, const UA_NodeId *nodeId)
{
	return UA_NodestoreSwitch_getNode(nodestoreContext, nodeId);
}

void UA_NodestoreSwitchInterface_releaseNode(void *nodestoreContext, const UA_Node *node)
{
	UA_NodestoreSwitch_releaseNode(nodestoreContext, node);
}

UA_StatusCode UA_NodestoreSwitchInterface_getNodeCopy(void *nodestoreContext, const UA_NodeId *nodeId,
	                                 UA_Node **outNode)
{
	return UA_NodestoreSwitch_getNodeCopy(nodestoreContext, nodeId, outNode);
}

UA_StatusCode UA_NodestoreSwitchInterface_insertNode(void *nodestoreContext, UA_Node *node,
	                                UA_NodeId *addedNodeId)
{
	return UA_NodestoreSwitch_insertNode(nodestoreContext, node, addedNodeId);
}

UA_StatusCode UA_NodestoreSwitchInterface_replaceNode(void *nodestoreContext, UA_Node *node)
{
	return UA_NodestoreSwitch_replaceNode(nodestoreContext, node);
}

UA_StatusCode UA_NodestoreSwitchInterface_removeNode(void *nodestoreContext, const UA_NodeId *nodeId)
{
	return UA_NodestoreSwitch_removeNode(nodestoreContext, nodeId);
}

void UA_NodestoreSwitchInterface_iterate(void *nodestoreContext, void* visitorContext,
	                    UA_NodestoreVisitor visitor)
{
	UA_NodestoreSwitch_iterate(nodestoreContext, visitorContext, visitor);
}

