/*
 * trafo.c
 *
 *  Created on: 26.02.2019
 *      Author: julian
 */

//TODO split file up into 2 files: ipsms_trafo, ipsms_store

#include "libov/ov_macros.h"
#include "ipsms_trafo.h"
#include "fb_macros.h"

#include "opcua_helpers.h"
#include "opcua_ovStore.h"
//#include "ov_call_macros_10.h"

//static UA_NodeId
//ipsms_trafo_getParentNodeId(OV_INSTPTR_ov_object pobj){
//	OV_INSTPTR_ov_domain pparent = Ov_GetParent(ov_containment, pobj);
//	if(pparent){
//		ov_memstack_lock();
//		UA_NodeId nodeId = UA_NODEID_STRING_ALLOC(OPCUA_OVSTORE_DEFAULTNSINDEX,
//				ov_path_getcanonicalpath(Ov_StaticPtrCast(ov_object, pparent), 2));
//		ov_memstack_unlock();
//		return nodeId;
//	}
//	return UA_NODEID_NULL;
//}
/*
static const OV_STRING
ipsms_trafo_getDescription(OV_INSTPTR_ov_object pobj){
	OV_INSTPTR_ov_class ovClass = Ov_GetParent(ov_instantiation, pobj);
	if(ovClass != NULL && ovClass->v_comment != NULL){
		return ovClass->v_comment;
	}else{
		return "";
	}
}
*/

// Has to be the first reference for correct memory allocation/freeing
static UA_AddReferencesItem *
ipsms_trafo_addParentReference(
		const UA_NodeId * nodeId, const UA_NodeId parentNodeId,
		UA_Node * node){
	node->references = NULL;
	node->referencesSize = 0;

	// parent reference
	UA_AddReferencesItem * ref = UA_AddReferencesItem_new();
	ref->isForward = UA_FALSE;
	ref->referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
	UA_NodeId_copy(nodeId, &ref->sourceNodeId);
	ref->targetNodeClass = UA_NODECLASS_OBJECT;
	UA_ExpandedNodeId_init(&ref->targetNodeId);
	UA_NodeId_copy(&parentNodeId, &ref->targetNodeId.nodeId);
	UA_String_init(&ref->targetServerUri);
	UA_Node_addReference(node, ref);
	return ref;
}

// Has to be called after ipsms_trafo_addParentReference for correct memory allocation/freeing
static void
ipsms_trafo_addTypeDefinitionReference(const UA_Server * server,
		const UA_NodeId * nodeId, const UA_NodeId targetNodeId,
		UA_Node* node, UA_AddReferencesItem * ref){
	// hasTypeDefinition reference forward
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

	// hasTypeDefinition reference backward
	if(server == NULL)
		return;
	ref->isForward = UA_FALSE;
	UA_NodeId_deleteMembers(&ref->sourceNodeId);
	ref->sourceNodeId = targetNodeId;
	ref->targetNodeClass = UA_NODECLASS_OBJECT;
	UA_ExpandedNodeId_deleteMembers(&ref->targetNodeId);
	UA_ExpandedNodeId_init(&ref->targetNodeId);
	UA_NodeId_copy(nodeId, &ref->targetNodeId.nodeId);
	// Get the node from nodestore edit and replace
	UA_ServerConfig * config  = UA_Server_getConfig((UA_Server*) server);
	UA_Node * targetNode = NULL;
	if(config->nodestore.getNodeCopy(config->nodestore.context, &targetNodeId, &targetNode) == UA_STATUSCODE_GOOD){
		if(UA_Node_addReference(targetNode, ref) == UA_STATUSCODE_GOOD){
			config->nodestore.replaceNode(config->nodestore.context, targetNode);
		}else{
			config->nodestore.deleteNode(config->nodestore.context, targetNode);
		}
	}
}

static UA_Node *
ipsms_trafo_createNode(UA_NodeClass nodeClass, const OV_STRING identifier, const UA_UInt16 nsIndex){
	UA_Node * node = NULL;
	switch(nodeClass){
	case UA_NODECLASS_VARIABLE:
		node = (UA_Node*) UA_malloc(sizeof(UA_VariableNode));
		break;
	case UA_NODECLASS_OBJECT:
	default:
		node = (UA_Node*) UA_malloc(sizeof(UA_ObjectNode));
		nodeClass = UA_NODECLASS_OBJECT;
		((UA_ObjectNode*)node)->eventNotifier = 0;
		break;
	}
	node->nodeClass = nodeClass;

	// BrowseName
	UA_QualifiedName_init(&node->browseName);
	node->browseName.name = UA_String_fromChars(identifier);
	node->browseName.namespaceIndex = nsIndex;

	// DisplayName
	UA_LocalizedText_init(&node->displayName);
	node->displayName.locale = UA_String_fromChars("en");
	node->displayName.text = UA_String_fromChars(identifier);

	return (UA_Node*)node;
}

static UA_Node *
ipsms_trafo_genericTrafo(
		const UA_Server * server,
		const OV_STRING identifier, const OV_STRING description, UA_NodeClass nodeClass,
		const UA_NodeId * nodeId, const UA_NodeId parentNodeId, const UA_NodeId typeNodeId,
		UA_AddReferencesItem ** ref){
	UA_Node * node = ipsms_trafo_createNode(nodeClass, identifier, nodeId->namespaceIndex);

	// Description
	UA_LocalizedText_init(&node->description);
	node->description.locale = UA_String_fromChars("en");
	node->description.text = UA_String_fromChars(description);

	// Writemask
	node->writeMask = UA_ACCESSLEVELMASK_READ;

	// Add references
	*ref = ipsms_trafo_addParentReference(nodeId, parentNodeId, node);
	ipsms_trafo_addTypeDefinitionReference(server, nodeId, typeNodeId,node, *ref);
	return node;
}

static UA_Node *
ipsms_trafo_controlchart(
		OV_INSTPTR_fb_controlchart pobj, const UA_Server * server,
		const UA_NodeId * nodeId){
	UA_AddReferencesItem * ref = NULL;
	UA_Node * node = ipsms_trafo_genericTrafo(server,
			pobj->v_identifier, "BaSys 4.0 control component.",
			UA_NODECLASS_OBJECT, nodeId, UA_NODEID_NULL,
			UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE), 	//TODO Add types in uaInterface and link to correct type (BaSys40 Component)
			&ref);

	// Add references
	// Organizes SERVICES and STATUS
	ref->isForward = UA_TRUE;
	UA_NodeId_deleteMembers(&ref->referenceTypeId);
	ref->referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
	UA_NodeId_deleteMembers(&ref->sourceNodeId);
	UA_NodeId_copy(nodeId, &ref->sourceNodeId);
	ref->targetNodeClass = UA_NODECLASS_OBJECT;
	// Add STATUS reference as virtual node
	UA_ExpandedNodeId_deleteMembers(&ref->targetNodeId);
	UA_ExpandedNodeId_init(&ref->targetNodeId);
	UA_NodeId_copy(nodeId, &ref->targetNodeId.nodeId);
	opcua_helpers_UA_String_append(&ref->targetNodeId.nodeId.identifier.string, "|STATUS");
	UA_Node_addReference(node, ref);
	// Add SERVICES reference
	UA_ExpandedNodeId_deleteMembers(&ref->targetNodeId);
	UA_ExpandedNodeId_init(&ref->targetNodeId);
	UA_NodeId_copy(nodeId, &ref->targetNodeId.nodeId);
	opcua_helpers_UA_String_append(&ref->targetNodeId.nodeId.identifier.string, "/SERVICES");
	ref->targetNodeId.nodeId.namespaceIndex = OPCUA_OVSTORE_DEFAULTNSINDEX;
	UA_Node_addReference(node, ref);

	// Free resources
	UA_AddReferencesItem_delete(ref);

	return node;
}
//static UA_Node *
//ipsms_trafo_domain(
//		OV_INSTPTR_ov_domain pobj, OV_INSTPTR_opcua_server pServer,
//		const UA_NodeId * nodeId, const UA_NodeId parentNodeId){
//	UA_AddReferencesItem * ref = NULL;
//	UA_Node * node = ipsms_trafo_genericTrafo(pServer->v_server,
//			pobj->v_identifier, "OV domain container.",//ipsms_trafo_getDescription(Ov_StaticPtrCast(ov_object, pobj)),
//			UA_NODECLASS_OBJECT, nodeId, parentNodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE),
//			&ref);
//
//	// Organizes childs
//	OV_INSTPTR_ov_object pchild = NULL;
//	OV_BOOL isServicesDomain = ov_string_compare(pobj->v_identifier, "SERVICES") == OV_STRCMP_EQUAL;
//	ref->isForward = UA_TRUE;
//	UA_NodeId_deleteMembers(&ref->referenceTypeId);
//	UA_NodeId_deleteMembers(&ref->sourceNodeId);
//	UA_NodeId_copy(nodeId, &ref->sourceNodeId);
//	ref->targetNodeClass = UA_NODECLASS_OBJECT;
//	ov_memstack_lock(); // For ov_path_getcanonicalpath
//	Ov_ForEachChild(ov_containment, pobj, pchild){
//		if(isServicesDomain){
//			// Add hasComponent reference for every service
//			opcua_ovStore_addReferenceToSpecificObject(pServer, pchild, node);
//		}else if(Ov_CanCastTo(fb_controlchart, pchild)){
//			// Add organizes reference for every control chart
//			ref->referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
//			UA_ExpandedNodeId_deleteMembers(&ref->targetNodeId);
//			ref->targetNodeId = UA_EXPANDEDNODEID_STRING_ALLOC(nodeId->namespaceIndex, //TODO get services nsIndex
//					ov_path_getcanonicalpath(pchild, 2));
//			UA_Node_addReference(node, ref);
//		}
//	}
//	ov_memstack_unlock();
//
//	// Free resources
//	UA_AddReferencesItem_delete(ref);
//
//	return (UA_Node*)node;
//}

static UA_Node *
ipsms_trafo_statusVariable(const UA_Server * server,
		const UA_NodeId * nodeId, OV_STRING identifier, OV_STRING parentPath){

	ov_memstack_lock();
	OV_STRING virtualParentPath = NULL;
	ov_string_stack_print(&virtualParentPath, "%s|STATUS", parentPath);
	UA_AddReferencesItem * ref = NULL;
	UA_VariableNode * node = (UA_VariableNode*) ipsms_trafo_genericTrafo(server,
			identifier, "Description",
			UA_NODECLASS_VARIABLE, nodeId,
			UA_NODEID_STRING(nodeId->namespaceIndex, virtualParentPath),
			UA_NODEID_NUMERIC(0, UA_NS0ID_BASEVARIABLETYPE),
			&ref);
	ov_memstack_unlock();

	// Resolve parent node id and check that it is a controlchart aka BaSys 4.0 component
	UA_NodeId parentNodeId = UA_NODEID_STRING(nodeId->namespaceIndex, parentPath);
	OV_INSTPTR_ov_object pobj = opcua_helpers_resolveNodeIdToOvObject(&parentNodeId);
	if(pobj == NULL || !Ov_CanCastTo(fb_controlchart, pobj)){
		return (UA_Node*)node;
	}
	OV_INSTPTR_fb_functionchart pcomponent = Ov_StaticPtrCast(fb_functionchart, pobj);
	OV_ANY value = OV_ANY_INIT;
	fb_functionchart_getport(pcomponent, identifier, &value); //TODO get port value directly by path
	node->accessLevel = UA_ACCESSLEVELMASK_READ;

	//TODO move to own function (for later fbInterface library
	UA_StatusCode result = UA_STATUSCODE_GOOD;
	switch(value.value.vartype & OV_VT_KSMASK){
		case OV_VT_ANY:
		case OV_VT_VOID:
			node->valueRank = -3;	//	scalar or one dimension
			node->arrayDimensionsSize = 0;
			node->arrayDimensions = UA_Array_new(node->arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);	//	scalar or one dimension
			break;
		default:
			if(value.value.vartype & OV_VT_ISVECTOR){
				//	vector
				node->valueRank = 1;
				node->arrayDimensionsSize = 1;
				node->arrayDimensions = UA_Array_new(node->arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);	//	scalar or one dimension
				if(!node->arrayDimensions){
					result = UA_STATUSCODE_BADOUTOFMEMORY;
					break;
				} else {
					result = UA_STATUSCODE_GOOD;
				}
				result = UA_Array_copy(&value.value.valueunion.val_generic_vec.veclen,
						node->arrayDimensionsSize, (void**)&(node->arrayDimensions), &UA_TYPES[UA_TYPES_INT32]);
			} else {
				//	scalar
				node->valueRank = -1;
				node->arrayDimensionsSize = 0;
				node->arrayDimensions = UA_Array_new(node->arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);	//	scalar or one dimension
			}
			break;
	}
	value.value.vartype &= OV_VT_KSMASK;
	if(result == UA_STATUSCODE_GOOD){
		result = opcua_helpers_ovAnyToUAVariant(&value, &node->value.data.value.value);
		node->valueSource = UA_VALUESOURCE_DATA;
		node->dataType = opcua_helpers_ovVarTypeToNodeId(value.value.vartype);
	}
	node->minimumSamplingInterval = -1;
	node->historizing = UA_FALSE;
	node->value.data.callback.onRead = NULL;
	node->value.data.callback.onWrite = NULL;

	return (UA_Node*)node;
}

static UA_Node *
ipsms_trafo_status(const UA_Server * server,
		const UA_NodeId * nodeId, OV_STRING parentPath){
	UA_AddReferencesItem * ref = NULL;
	UA_Node * node = ipsms_trafo_genericTrafo(server,
			"STATUS", "IPSMS conform folder for status variables.",
			UA_NODECLASS_OBJECT, nodeId,
			UA_NODEID_STRING(nodeId->namespaceIndex, parentPath),
			UA_NODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE),
			&ref);

	// Add status variables as organizes references
	// Resolve parent node id and check that it is a controlchart aka BaSys 4.0 component
	UA_NodeId parentNodeId = UA_NODEID_STRING(nodeId->namespaceIndex, parentPath);
	OV_INSTPTR_ov_object pobj = opcua_helpers_resolveNodeIdToOvObject(&parentNodeId);
	if(pobj == NULL || !Ov_CanCastTo(fb_controlchart, pobj)){
		return node;
	}

	// Set default values for all ports
	ref->isForward = UA_TRUE;
	UA_NodeId_deleteMembers(&ref->referenceTypeId);
	ref->referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
	UA_NodeId_deleteMembers(&ref->sourceNodeId);
	UA_NodeId_copy(nodeId, &ref->sourceNodeId);
	ref->targetNodeClass = UA_NODECLASS_OBJECT;
	// Iterate over output ports
	OV_INSTPTR_fb_controlchart pcomponent = Ov_StaticPtrCast(fb_controlchart, pobj);
	OV_INSTPTR_fb_port pport = NULL;
	OV_STRING portPath = NULL;
	ov_memstack_lock();
	Ov_ForEachChild(fb_variables, pcomponent, pport){
		//List all outputs
		if(IsFlagSet(pport->v_flags, 'o')){
			//Add organizes reference for every output port
			UA_ExpandedNodeId_deleteMembers(&ref->targetNodeId);
			ov_string_stack_print(&portPath, "%s|STATUS|%s", parentPath, pport->v_identifier);
			ref->targetNodeId = UA_EXPANDEDNODEID_STRING_ALLOC(nodeId->namespaceIndex,
					portPath);
			UA_Node_addReference(node, ref);
		}
	}
	ov_memstack_unlock();
	return node;
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
	OV_INSTPTR_opcua_server 		server = (pinterface) ? Ov_GetParent(opcua_serverToInterfaces, pinterface) : NULL;
	OPCUA_PTR_UA_Server 			uaServer = (server) ? server->v_server : NULL;

	// Check path for virtual node //TODO move to own function
	if(nodeId->identifierType == UA_NODEIDTYPE_STRING){
		OV_STRING path = NULL;
		OV_UINT length = 0;
		opcua_helpers_copyUAStringToOV(nodeId->identifier.string , &path);
		OV_STRING* pathList = ov_string_split(path, "|", &length);
		if(length > 1){
			if(ov_string_compare(pathList[1], "STATUS") == OV_STRCMP_EQUAL){
				if(length == 2){
					node = ipsms_trafo_status(uaServer, nodeId, pathList[0]);
				}else if(length == 3){
					node = ipsms_trafo_statusVariable(uaServer, nodeId, pathList[2], pathList[0]);
				}
			}
		}
		ov_string_freelist(pathList);
	}

	// Check for none virtual node
	if(node == NULL){
		//Resolve nodeId
		pobj = opcua_helpers_resolveNodeIdToOvObject(nodeId);
		if(pobj == NULL){
			return NULL;
		}

		// Visualize every control chart as BaSys 4.0 IPSMS component
		if(Ov_CanCastTo(fb_controlchart, pobj)){
			node = ipsms_trafo_controlchart(Ov_StaticPtrCast(fb_controlchart, pobj), uaServer, nodeId);
		}
//		// Visualize a domain
//		else if(Ov_CanCastTo(ov_domain, pobj)){
//			// Check if root domain of uaInterface (entryPath)
//			UA_String rootDomain = UA_STRING(pinterface->v_entryPath);
//			if(UA_String_equal(&nodeId->identifier.string, &rootDomain) == UA_TRUE){
//				// Change parent for root domain of interface
//				node = ipsms_trafo_domain(Ov_StaticPtrCast(ov_domain, pobj), server, nodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER));
//			}else{
//				//TODO allow mulitple folders for ESE / GSEs
//				node = ipsms_trafo_domain(Ov_StaticPtrCast(ov_domain, pobj), server, nodeId, ipsms_trafo_getParentNodeId(pobj));
//			}
//		}
	}

	// Set nodeId
	if(node != NULL)
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
