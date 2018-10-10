///*
// * ua_nodestore_switch.h
// *
// *  Created on: Sep 30, 2016
// *      Author: julian
// */
//
//#ifndef UA_NS_SWITCH_INTERFACE_H_
//#define UA_NS_SWITCH_INTERFACE_H_
//
//#include "open62541.h"
//#include "nodeStoreSwitch.h"
//#ifdef __cplusplus
//extern "C" {
//#endif
//
//
//
//struct UA_NodestoreSwitch_Interface{
//	    void *UA_NodestoreSwitch;
//	    UA_Boolean inPlaceEditAllowed;
//	    void (*deleteNodestore)(void *nodestoreContext);
//	    UA_Node * (*newNode)(void *nodestoreContext, UA_NodeClass nodeClass);
//	    void (*deleteNode)(void *nodestoreContext, UA_Node *node);
//	    const UA_Node * (*getNode)(void *nodestoreContext, const UA_NodeId *nodeId);
//	    void (*releaseNode)(void *nodestoreContext, const UA_Node *node);
//	    UA_StatusCode (*getNodeCopy)(void *nodestoreContext, const UA_NodeId *nodeId,
//	                                 UA_Node **outNode);
//	    UA_StatusCode (*insertNode)(void *nodestoreContext, UA_Node *node,
//	                                UA_NodeId *addedNodeId);
//	    UA_StatusCode (*replaceNode)(void *nodestoreContext, UA_Node *node);
//	    UA_StatusCode (*removeNode)(void *nodestoreContext, const UA_NodeId *nodeId);
//	    void (*iterate)(void *nodestoreContext, void* visitorContext,
//	                    UA_NodestoreVisitor visitor);
//
//};
//
//typedef struct UA_NodestoreSwitch_Interface UA_NodestoreSwitch_Interface;
//
//
//void UA_NodestoreSwitchInterface_deleteNodestore(void *nodestoreContext);
//
//UA_Node *UA_NodestoreSwitchInterface_newNode(void *nodestoreContext, UA_NodeClass nodeClass);
//void UA_NodestoreSwitchInterface_deleteNode(void *nodestoreContext, UA_Node *node);
//const UA_Node * UA_NodestoreSwitchInterface_getNode(void *nodestoreContext, const UA_NodeId *nodeId);
//void UA_NodestoreSwitchInterface_releaseNode(void *nodestoreContext, const UA_Node *node);
//UA_StatusCode UA_NodestoreSwitchInterface_getNodeCopy(void *nodestoreContext, const UA_NodeId *nodeId,
//	                                 UA_Node **outNode);
//UA_StatusCode UA_NodestoreSwitchInterface_insertNode(void *nodestoreContext, UA_Node *node,
//	                                UA_NodeId *addedNodeId);
//UA_StatusCode UA_NodestoreSwitchInterface_replaceNode(void *nodestoreContext, UA_Node *node);
//UA_StatusCode UA_NodestoreSwitchInterface_removeNode(void *nodestoreContext, const UA_NodeId *nodeId);
//void UA_NodestoreSwitchInterface_iterate(void *nodestoreContext, void* visitorContext,
//	                    UA_NodestoreVisitor visitor);
//
//UA_StatusCode UA_NodestoreSwitch_new(UA_Nodestore *ns);
//
//#ifdef __cplusplus
//} // extern "C"
//#endif
//
//#endif /* UA_NODESTORE_SWITCH_H_ */
