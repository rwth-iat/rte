/*
 * ua_nodestore_switch.h
 *
 *  Created on: Sep 30, 2016
 *      Author: julian
 */

#ifndef UA_NODESTORE_SWITCH_H_
#define UA_NODESTORE_SWITCH_H_

#include "open62541.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Nodestore lifecycle
 * ^^^^^^^^^^^^^^^^
 *
 */

/* in ServerConfig wird UA_NodestoreSwitch als UA_Nodestore angegeben  */

struct UA_NodestoreSwitch {
	void *context;
	int size=1;
	UA_Nodestore *nodestoreArray[];

};


typedef struct UA_NodestoreSwitch UA_NodestoreSwitch;

UA_StatusCode UA_NodestoreSwitch_init(UA_NodestoreSwitch *pSwitch);
UA_StatusCode UA_NodestoreSwitch_iterate(UA_NodestoreSwitch, void* visitorContext, UA_NodestoreVisitor visitor);
UA_StatusCode UA_NodestoreSwitch_init(UA_NodestoreSwitch *pSwitch);
UA_StatusCode UA_NodestoreSwitch_deleteNodestore(UA_NodestoreSwitch *pSwitch, void *nodestoreHandle);
UA_StatusCode UA_NodestoreSwitch_changeNodestore(UA_NodestoreSwitch *pSwitch,
		void *nodestoreHandleOut, void *nodestoreHandleIn);
UA_StatusCode UA_NodestoreSwitch_unlinkNodestore(UA_NodestoreSwitch *pSwitch,
		void *nodestoreHandle);
UA_StatusCode UA_NodestoreSwitch_linkNodestore(UA_NodestoreSwitch *pSwitch,
		UA_Nodestore *ns);

void UA_NodestoreSwitch_deleteNode(UA_NodestoreSwitch pSwitch, UA_NodeId *NodeId);


UA_StatusCode UA_NodestoreSwitch_insertNode(UA_Node *node, UA_NodeId *addedNodeId);


const UA_Node *UA_NodestoreSwitch_getNode(const UA_NodeId *nodeId);


UA_Node * UA_NodestoreSwitch_getNodeCopy(const UA_NodeId *nodeId);


UA_StatusCode UA_NodestoreSwitch_replaceNode(UA_Node *node);


UA_StatusCode UA_NodestoreSwitch_removeNode(const UA_NodeId *nodeId);

void releaseNode(const UA_Node *node);


UA_Node * UA_NodestoreSwitch_newNode(UA_Server* server, UA_NodeClass nodeClass,
		UA_UInt16 namespaceIndex);



/*
 * The following definitions are used to create empty nodes of the different
 * node types in NameSpace 0.
 */
#define UA_Nodestore_newObjectNode() \
    (UA_ObjectNode*)UA_NodestoreSwitch_newNode(server, UA_NODECLASS_OBJECT, 0)
#define UA_Nodestore_newVariableNode() \
    (UA_VariableNode*)UA_NodestoreSwitch_newNode(server, UA_NODECLASS_VARIABLE, 0)
#define UA_Nodestore_newMethodNode() \
    (UA_MethodNode*)UA_NodestoreSwitch_newNode(server, UA_NODECLASS_METHOD, 0)
#define UA_Nodestore_newObjectTypeNode() \
    (UA_ObjectTypeNode*)UA_NodestoreSwitch_newNode(server, UA_NODECLASS_OBJECTTYPE, 0)
#define UA_Nodestore_newVariableTypeNode() \
    (UA_VariableTypeNode*)UA_NodestoreSwitch_newNode(server, UA_NODECLASS_VARIABLETYPE, 0)
#define UA_Nodestore_newReferenceTypeNode() \
    (UA_ReferenceTypeNode*)UA_NodestoreSwitch_newNode(server, UA_NODECLASS_REFERENCETYPE, 0)
#define UA_Nodestore_newDataTypeNode() \
    (UA_DataTypeNode*)UA_NodestoreSwitch_newNode(server, UA_NODECLASS_DATATYPE, 0)
#define UA_Nodestore_newViewNode() \
    (UA_ViewNode*)UA_NodestoreSwitch_newNode(server, UA_NODECLASS_VIEW, 0)

/* Delete an editable node. */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* UA_NODESTORE_SWITCH_H_ */
