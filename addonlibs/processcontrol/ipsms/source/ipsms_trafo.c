/*
 * trafo.c
 *
 *  Created on: 26.02.2019
 *      Author: julian
 */

#include "libov/ov_macros.h"
#include "ipsms_trafo.h"

#include "opcua_helpers.h"
#include "opcua_ovStore.h"

//Has to be the first reference for correct memory allocation/freeing
static UA_AddReferencesItem * ipsms_trafo_addParentReference(UA_Node * node, const UA_NodeId * nodeId, OV_INSTPTR_ov_object pobj){
	node->references = NULL;
	node->referencesSize = 0;

	//parent reference
	UA_AddReferencesItem * ref = UA_AddReferencesItem_new();
	ref->isForward = UA_FALSE;
	ref->referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
	UA_NodeId_copy(nodeId, &ref->sourceNodeId);
	ref->targetNodeClass = UA_NODECLASS_OBJECT;
	OV_INSTPTR_ov_domain pparent = Ov_GetParent(ov_containment, pobj);
	if(pparent){
		ov_memstack_lock();
		ref->targetNodeId = UA_EXPANDEDNODEID_STRING_ALLOC(OPCUA_OVSTORE_DEFAULTNSINDEX,
				ov_path_getcanonicalpath(Ov_StaticPtrCast(ov_object, pparent), 2));
		ov_memstack_unlock();
	}
	UA_String_init(&ref->targetServerUri);
	UA_Node_addReference(node, ref);
	return ref;
}

//Has to be called after ipsms_trafo_addParentReference for correct memory allocation/freeing
static void ipsms_trafo_addTypeDefinitionReference(UA_Server * server, UA_Node* node, const UA_NodeId * nodeId, UA_NodeId targetNodeId, UA_AddReferencesItem * ref){
	//hasTypeDefinition reference forward
	ref->isForward = UA_TRUE;
	UA_NodeId_deleteMembers(&ref->referenceTypeId);
	ref->referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION);
	//UA_NodeId_copy(nodeId, &ref->sourceNodeId);
	ref->targetNodeClass = UA_NODECLASS_OBJECTTYPE;
	UA_ExpandedNodeId_deleteMembers(&ref->targetNodeId);
	UA_ExpandedNodeId_init(&ref->targetNodeId);
	UA_NodeId_copy(&targetNodeId, &ref->targetNodeId.nodeId);
	//UA_String_init(&ref->targetServerUri);
	UA_Node_addReference(node, ref);

	//hasTypeDefinition reference backward
	if(server == NULL)
		return;
	ref->isForward = UA_FALSE;
	UA_NodeId_deleteMembers(&ref->sourceNodeId);
	ref->sourceNodeId = targetNodeId;
	ref->targetNodeClass = UA_NODECLASS_OBJECT;
	UA_ExpandedNodeId_deleteMembers(&ref->targetNodeId);
	UA_ExpandedNodeId_init(&ref->targetNodeId);
	UA_NodeId_copy(nodeId, &ref->targetNodeId.nodeId);
	//Get the node from nodestore edit and replace
	UA_ServerConfig * config  = UA_Server_getConfig(server);
	UA_Node * targetNode = NULL;
	if(config->nodestore.getNodeCopy(config->nodestore.context, &targetNodeId, &targetNode) == UA_STATUSCODE_GOOD){
		if(UA_Node_addReference(targetNode, ref) == UA_STATUSCODE_GOOD){
			config->nodestore.replaceNode(config->nodestore.context, targetNode);
		}else{
			config->nodestore.deleteNode(config->nodestore.context, targetNode);
		}
	}
}

static UA_Node * ipsms_trafo_genericTrafo(UA_Server * server,
		OV_INSTPTR_ov_object pobj, const UA_NodeId * nodeId, UA_NodeId typeNodeId,
		UA_AddReferencesItem ** ref){
	UA_ObjectNode * node = (UA_ObjectNode*) UA_malloc(sizeof(UA_ObjectNode)); //TODO parameter for NodeClass --> Switch case --> helper function
	node->nodeClass = UA_NODECLASS_OBJECT;

	// BrowseName
	UA_QualifiedName_init(&node->browseName);
	node->browseName.name = UA_String_fromChars(pobj->v_identifier);
	node->browseName.namespaceIndex = nodeId->namespaceIndex;

	// DisplayName
	UA_LocalizedText_init(&node->displayName);
	node->displayName.locale = UA_String_fromChars("en");
	node->displayName.text = UA_String_fromChars(pobj->v_identifier);

	// Description
	UA_LocalizedText_init(&node->description);
	OV_INSTPTR_ov_class ovClass = Ov_GetParent(ov_instantiation, pobj);
	if(ovClass != NULL && ovClass->v_comment != NULL){
		node->description.text = UA_String_fromChars(ovClass->v_comment);
	}else{
		UA_String_init(&node->description.text);
	}

	node->eventNotifier = 0;
	node->writeMask = UA_ACCESSLEVELMASK_READ;

	//Add references
	*ref = ipsms_trafo_addParentReference((UA_Node*)node, nodeId, Ov_StaticPtrCast(ov_object, pobj));
	ipsms_trafo_addTypeDefinitionReference(server, (UA_Node*)node, nodeId, typeNodeId, *ref);
	return (UA_Node*)node;
}

static UA_Node * ipsms_trafo_controlchart(OV_INSTPTR_fb_controlchart pobj, const UA_NodeId * nodeId, UA_Server * server){
	UA_AddReferencesItem * ref = NULL;
	UA_Node * node = ipsms_trafo_genericTrafo(server, Ov_StaticPtrCast(ov_object, pobj), nodeId,
			UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE), 	//TODO Add types in uaInterface and link to correct type (BaSys40 Component)
			&ref);

	//Add references

	//free resources
	UA_AddReferencesItem_delete(ref);

	return node;
}
static UA_Node * ipsms_trafo_domain(OV_INSTPTR_ov_domain pobj, const UA_NodeId * nodeId, UA_Server * server){
	UA_AddReferencesItem * ref = NULL;
	UA_Node * node = ipsms_trafo_genericTrafo(server, Ov_StaticPtrCast(ov_object, pobj), nodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE), &ref);
	//Change parent for root domain of interface
	UA_NodeId_deleteMembers(&node->references->targetIds->nodeId);
	node->references->targetIds->nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);

	//Organizes childs
	OV_INSTPTR_ov_object pchild = NULL;
	ref->isForward = UA_TRUE;
	UA_NodeId_deleteMembers(&ref->referenceTypeId);
	ref->referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
	UA_NodeId_deleteMembers(&ref->sourceNodeId);
	UA_NodeId_copy(nodeId, &ref->sourceNodeId);
	ref->targetNodeClass = UA_NODECLASS_OBJECT;
	ov_memstack_lock(); //For ov_path_getcanonicalpath
	Ov_ForEachChild(ov_containment, pobj, pchild){
		if(Ov_CanCastTo(fb_controlchart, pchild)){
			//Add organizes reference for every control chart
			UA_ExpandedNodeId_deleteMembers(&ref->targetNodeId);
			ov_memstack_lock();
			ref->targetNodeId = UA_EXPANDEDNODEID_STRING_ALLOC(nodeId->namespaceIndex,
					ov_path_getcanonicalpath(pchild, 2));
			UA_Node_addReference((UA_Node*)node, ref);
		}
	}
	ov_memstack_unlock();

	//free resources
	UA_AddReferencesItem_delete(ref);

	return (UA_Node*)node;
}

static void ipsms_trafo_deleteNodestore(void *context){
}

static UA_Node * ipsms_trafo_newNode(void * context, UA_NodeClass nodeClass){
    return NULL;
}

static void ipsms_trafo_deleteNode(void * context, UA_Node *node){
	if(node){
		UA_Node_deleteMembers(node);
	}
	UA_free(node);
}

static const UA_Node * ipsms_trafo_getNode(void * context, const UA_NodeId *nodeId){
	UA_Node * 						node = NULL;
	OV_INSTPTR_ov_object			pobj = NULL;
	OV_INSTPTR_ipsms_uaInterface 	pinterface = Ov_StaticPtrCast(ipsms_uaInterface, context);
	OV_INSTPTR_opcua_server 		server = Ov_GetParent(opcua_serverToInterfaces, pinterface);
	//Check for virtual object

	//Resolve nodeId
	pobj = opcua_helpers_resolveNodeIdToOvObject(nodeId);
	if(pobj == NULL){
		return NULL;
	}

	//Visualize every control chart as ESE
	if(Ov_CanCastTo(fb_controlchart, pobj)){
		node = ipsms_trafo_controlchart(Ov_StaticPtrCast(fb_controlchart, pobj), nodeId, (server) ? server->v_server : NULL);
	}
	//Visualize first domain (entry point defined in uaInterface load)
	else if(Ov_CanCastTo(ov_domain, pobj)){

		node = ipsms_trafo_domain(Ov_StaticPtrCast(ov_domain, pobj), nodeId, (server) ? server->v_server : NULL);
	}

	//set nodeId
	if(node)
		UA_NodeId_copy(nodeId, &node->nodeId);

	return node;
}

static UA_StatusCode ipsms_trafo_insert(void *context, UA_Node *node, UA_NodeId *parrentNode){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

static void ipsms_trafo_releaseNode(void *context, const UA_Node *node){
	ipsms_trafo_deleteNode(context, (UA_Node*)node);
}

static UA_StatusCode ipsms_trafo_getCopyNode(void *context, const UA_NodeId *nodeId, UA_Node ** nodeOut){
	UA_Node* node = (UA_Node*) ipsms_trafo_getNode(context, nodeId);
	if(node == NULL)
		return UA_STATUSCODE_BADNODEIDUNKNOWN;
	*nodeOut = node;
	return UA_STATUSCODE_GOOD;
}

static UA_StatusCode ipsms_trafo_insertNode(void *context, UA_Node *node, UA_NodeId *parrentNode){
	return ipsms_trafo_insert(context, node, parrentNode);
}

static UA_StatusCode ipsms_trafo_replaceNode(void *context, UA_Node *node){

	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

static UA_StatusCode ipsms_trafo_removeNode(void *context, const UA_NodeId *nodeId){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

static void ipsms_trafo_iterate(void *context, void* visitorHandle, UA_NodestoreVisitor visitor){
}

UA_Nodestore* ipsms_trafo_new(OV_INSTPTR_ipsms_uaInterface context) {
	UA_Nodestore* trafo = UA_malloc(sizeof(UA_Nodestore));
	if(trafo == NULL)
		return NULL;
    trafo->context =          context;
    trafo->newNode =       	  ipsms_trafo_newNode;
    trafo->deleteNode =    	  ipsms_trafo_deleteNode;
    trafo->deleteNodestore =  ipsms_trafo_deleteNodestore;

    trafo->getNode =          ipsms_trafo_getNode;
    trafo->releaseNode =      ipsms_trafo_releaseNode;

    trafo->getNodeCopy =      ipsms_trafo_getCopyNode;
    trafo->insertNode =       ipsms_trafo_insertNode;
    trafo->replaceNode =      ipsms_trafo_replaceNode;

    trafo->removeNode =       ipsms_trafo_removeNode;

    trafo->iterate =          ipsms_trafo_iterate;
    return trafo;
}

void ipsms_trafo_delete(UA_Nodestore * trafo){
	trafo->context = NULL;
	UA_free(trafo);
}
