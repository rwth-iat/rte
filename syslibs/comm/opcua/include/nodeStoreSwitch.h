
#ifndef UA_NODESTORE_SWITCH_H_
#define UA_NODESTORE_SWITCH_H_

#include "open62541.h"

#ifdef __cplusplus

extern "C" {

#endif

/**
 * Nodestore lifecycle
 *
 *
 */

struct UA_NodestoreSwitch {

	UA_UInt16 size;
	UA_Nodestore *defaultNodestore;
	UA_Nodestore **nodestoreArray;


};

typedef struct UA_NodestoreSwitch UA_NodestoreSwitch;


/* Standard functions for nodestore
 * Searches for the Node or inserts/creates it to defaultNodestore
 * */

void UA_NodestoreSwitch_iterate(void *switchHandle, void* visitorContext, UA_NodestoreVisitor visitor);
void UA_NodestoreSwitch_deleteNodestores(void *switchHandle);
void UA_NodestoreSwitch_deleteNode(void *switchHandle, UA_Node *node);
UA_StatusCode UA_NodestoreSwitch_insertNode(void *switchHandle,UA_Node *node, UA_NodeId *addedNodeId);
const UA_Node *UA_NodestoreSwitch_getNode(void *switchHandle ,const UA_NodeId *nodeId);
UA_StatusCode UA_NodestoreSwitch_getNodeCopy(void *switchHandle ,const UA_NodeId *nodeId, UA_Node **outNode);
UA_StatusCode UA_NodestoreSwitch_replaceNode(void *switchHandle ,UA_Node *node);
UA_StatusCode UA_NodestoreSwitch_removeNode(void *switchHandle ,const UA_NodeId *nodeId);
void releaseNode(void *switchHandle ,const UA_Node *node);
UA_Node * UA_NodestoreSwitch_newNode(void *switchHandle, UA_NodeClass nodeClass);
void UA_NodestoreSwitch_releaseNode(void *switchHandle, const UA_Node *node);

//* Added functions*//
UA_NodestoreSwitch *UA_NodestoreSwitch_new();
void UA_NodestoreSwitch_deleteSwitch(UA_NodestoreSwitch *pSwitch);
UA_StatusCode UA_NodestoreSwitch_linkDefaultNodestore(UA_NodestoreSwitch *pSwitch, UA_Nodestore *ns);
UA_StatusCode UA_NodestoreSwitch_changeNodestore(UA_NodestoreSwitch *pSwitch, void *nodestoreHandleOut, UA_Nodestore *nsIn);
UA_StatusCode UA_NodestoreSwitch_linkNodestoreToNamespace(UA_NodestoreSwitch *storeSwitch, UA_Nodestore *ns, UA_UInt16 namespaceindex);
UA_StatusCode UA_NodestoreSwitch_unlinkNodestoreFromNamespace(UA_NodestoreSwitch *storeSwitch, UA_Nodestore *ns);
void UA_NodestoreSwitch_linkNodestoreSwitchToServer(UA_NodestoreSwitch *storeSwitch ,UA_Nodestore *store);


#ifdef __cplusplus

} // extern "C"

#endif



#endif /* UA_NODESTORE_SWITCH_H_ */

