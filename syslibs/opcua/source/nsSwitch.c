/* This work is licensed under a Creative Commons CCZero 1.0 Universal License.
 * See http://creativecommons.org/publicdomain/zero/1.0/ for more information.
 *
 *    Copyright 2019 (c) Julian Grothoff, Torben Deppe, Florian Palm
 */
#include "opcua_nsSwitch.h"

#ifndef UA_ENABLE_AMALGAMATION
#include <open62541/plugin/nodestore_default.h>
#endif

//TODO Make multithreading save
//TODO rename Switch

/*
 * Definition of a switch based on the namespace id
 */
struct UA_Nodestore_Switch {
	UA_UInt16 size;
	UA_Nodestore *defaultStore; // Nodestore for all namespaces, that have a NULL nodestore interface (and for i>=size)
	UA_Nodestore **stores;      // Array of all nodestore interfaces
	UA_Nodestore *nsi;          // Interface to nodestore switch itself //TODO make const?
};

/*
 * Switch API: linking and unlinking of namespace index to store
 */
UA_Nodestore* UA_Nodestore_Switch_getNodestore(
		UA_Nodestore_Switch* storeSwitch, UA_UInt16 index,
		UA_Boolean useDefault) {
	if (index < storeSwitch->size && storeSwitch->stores[index] != NULL) {
		return storeSwitch->stores[index];
	}
	if (useDefault && storeSwitch->defaultStore != NULL)
		return storeSwitch->defaultStore;
	return NULL;
}

UA_StatusCode UA_Nodestore_Switch_setNodestore(UA_Nodestore_Switch* storeSwitch,
		UA_UInt16 index, UA_Nodestore* store) {
	UA_UInt16 newSize = (UA_UInt16) (index + 1);
	// calculate new size if last nodestore is unlinked
	if (store == NULL && index == (storeSwitch->size - 1)) {
		// check all interface links before last nodestore
		for (newSize = (UA_UInt16) (storeSwitch->size - 1); newSize > 0;
				--newSize) {
			if (storeSwitch->stores[newSize - 1] != NULL)
				break;
		}
	}

	// resize array if neccessary
	if (newSize != storeSwitch->size) {
		UA_Nodestore **newNsArray =
				(UA_Nodestore **) UA_realloc(storeSwitch->stores,
						newSize * sizeof(UA_Nodestore*));
		if (!newNsArray && newSize != 0)
			return UA_STATUSCODE_BADOUTOFMEMORY;
		storeSwitch->stores = newNsArray;
		// fill new array entries with NULL to use the default nodestore
		for (UA_UInt16 i = storeSwitch->size; i < newSize ; ++i) {
			storeSwitch->stores[i] = NULL;
		}
		storeSwitch->size = newSize;
	}

	// set nodestore
	if (index < storeSwitch->size)
		storeSwitch->stores[index] = store;
	return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_Nodestore_Switch_setNodestoreDefault(
		UA_Nodestore_Switch* storeSwitch, UA_Nodestore* store) {
	storeSwitch->defaultStore = store;
	return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_Nodestore_Switch_getIndices(UA_Nodestore_Switch* storeSwitch,
		UA_Nodestore* store, UA_UInt16* count, UA_UInt16** indices) {
	// Count occurances
	UA_UInt16 found = 0;
	for (UA_UInt16 i = 0; i < storeSwitch->size; i++) {
		if (storeSwitch->stores[i] == store)
			++found;
	}
	// Create an array of for indices
	if (indices != NULL) {
		*indices = (UA_UInt16*) UA_malloc(sizeof(UA_Int16) * found);
		if (*indices != NULL) {
			found = 0;
			for (UA_UInt16 i = 0; i < storeSwitch->size; i++) {
				if (storeSwitch->stores[i] == store)
					*indices[found++] = i;
			}
		} else
			return UA_STATUSCODE_BADOUTOFMEMORY;
	}
	if (count)
		*count = found;
	return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_Nodestore_Switch_changeNodestore(
		UA_Nodestore_Switch* storeSwitch, UA_Nodestore *storeOld,
		UA_Nodestore *storeNew) {
	if (storeOld == NULL)
		return UA_STATUSCODE_GOOD;
	for (UA_UInt16 i = 0; i < storeSwitch->size - 1; i++) {
		if (storeSwitch->stores[i] == storeOld)
			storeSwitch->stores[i] = storeNew;
	}
	// resize stores array if last store will be unlinked
	if (storeSwitch->stores[storeSwitch->size - 1] == storeOld) {
		UA_Nodestore_Switch_setNodestore(storeSwitch,
				(UA_UInt16) (storeSwitch->size - 1), storeNew);
	}
	return UA_STATUSCODE_GOOD;
}

/*
 * Functions of nodestore interface that are plugged into the server and do the actual switch
 */

static UA_Node *
UA_Nodestore_Switch_newNode(void *nsCtx, UA_NodeClass nodeClass) {
	//TODO: Not clear in which nodestore the memory has to be allocated. (storeSwitch or defaultNodestore)
	UA_Nodestore_Switch *storeSwitch = (UA_Nodestore_Switch *) nsCtx;
	return storeSwitch->defaultStore->newNode(
			storeSwitch->defaultStore->context, nodeClass);
}

static void
UA_Nodestore_Switch_deleteNode(void *nsCtx, UA_Node *node) {
	UA_Nodestore * nsi = UA_Nodestore_Switch_getNodestore((UA_Nodestore_Switch *) nsCtx,
			node->head.nodeId.namespaceIndex, UA_TRUE);
	if (nsi != NULL) {
		nsi->deleteNode(nsi->context, node);
		return;
	}
}

static const UA_Node *
UA_Nodestore_Switch_getNode(void *nsCtx, const UA_NodeId *nodeId) {
	UA_Nodestore * nsi = UA_Nodestore_Switch_getNodestore((UA_Nodestore_Switch *) nsCtx,
			nodeId->namespaceIndex, UA_TRUE);
	if (nsi != NULL) {
		return (nsi->getNode(nsi->context, nodeId));
	}
	return NULL;
}

// Will be introduced in open62541 versions > 1.3: https://github.com/open62541/open62541/commit/6b8db940e5fb4699c7bcde777fc7b21234cc947b
// const UA_Node *
// UA_Nodestore_Switch_getNodeFromPtr(void *nsCtx, UA_NodePointer ptr,
//                                       UA_UInt32 attributeMask,
//                                       UA_ReferenceTypeSet references,
//                                       UA_BrowseDirection referenceDirections) {
// 	UA_NodeId nodeId = UA_NodePointer_toNodeId(ptr);
// 	UA_Nodestore * nsi = UA_Nodestore_Switch_getNodestore((UA_Nodestore_Switch *) nsCtx,
// 			nodeId.namespaceIndex, UA_TRUE);
// 	if (nsi != NULL) {
// 		if(nsi->getNodeFromPtr != NULL){
// 			return (nsi->getNodeFromPtr(nsi->context, ptr, attributeMask, references, referenceDirections));
// 		} else {
// 			return (nsi->getNode(nsi->context, &nodeId, attributeMask, references, referenceDirections));
// 		}
// 	}
// 	return NULL;
// }

static void 
UA_Nodestore_Switch_releaseNode(void *nsCtx, const UA_Node *node) {
	if (node == NULL) //TODO check wether this should lead to an error?
		return;
	UA_Nodestore * nsi = UA_Nodestore_Switch_getNodestore((UA_Nodestore_Switch *) nsCtx,
			node->head.nodeId.namespaceIndex, UA_TRUE);
	if (nsi != NULL) {
		nsi->releaseNode(nsi->context, node);
		return;
	}
}

static UA_StatusCode 
UA_Nodestore_Switch_getNodeCopy(void *nsCtx,
		const UA_NodeId *nodeId, UA_Node **outNode) {
	UA_Nodestore * nsi = UA_Nodestore_Switch_getNodestore((UA_Nodestore_Switch *) nsCtx,
			nodeId->namespaceIndex, UA_TRUE);
	if (nsi != NULL)
		return nsi->getNodeCopy(nsi->context, nodeId, outNode);
	return UA_STATUSCODE_BADNODEIDUNKNOWN;
}

static UA_StatusCode
UA_Nodestore_Switch_insertNode(void *nsCtx, UA_Node *node,
		UA_NodeId *addedNodeId) {
	UA_Nodestore * nsi = UA_Nodestore_Switch_getNodestore((UA_Nodestore_Switch *) nsCtx,
			node->head.nodeId.namespaceIndex, UA_TRUE);
	if (nsi != NULL)
		return (nsi->insertNode(nsi->context, node, addedNodeId));
	return UA_STATUSCODE_BADNODEIDUNKNOWN;
}

static UA_StatusCode
UA_Nodestore_Switch_replaceNode(void *nsCtx, UA_Node *node) {
	UA_Nodestore * nsi = UA_Nodestore_Switch_getNodestore((UA_Nodestore_Switch *) nsCtx,
			node->head.nodeId.namespaceIndex, UA_TRUE);
	if (nsi != NULL)
		return nsi->replaceNode(nsi->context, node);
	return UA_STATUSCODE_BADNODEIDUNKNOWN; //TODO check if BADNODEIDUNKNOWN
}

static UA_StatusCode
UA_Nodestore_Switch_removeNode(void *nsCtx,
		const UA_NodeId *nodeId) {
	UA_Nodestore * nsi = UA_Nodestore_Switch_getNodestore((UA_Nodestore_Switch *) nsCtx,
			nodeId->namespaceIndex, UA_TRUE);
	if (nsi != NULL)
		return nsi->removeNode(nsi->context, nodeId);
	return UA_STATUSCODE_BADNODEIDUNKNOWN;
}

static void
UA_Nodestore_Switch_iterate(void *nsCtx, UA_NodestoreVisitor visitor,
		void *visitorCtx) {
	UA_Nodestore_Switch *storeSwitch = (UA_Nodestore_Switch *) nsCtx;
	UA_Nodestore *tempArray[storeSwitch->size];
	for (size_t i = 0; i < storeSwitch->size; i++) {
		for (size_t j = 0; j <= i; j++) {
			if (tempArray[j] == storeSwitch->stores[i])
				break;
			else if (j == i) {
				tempArray[i] = storeSwitch->stores[i];
				storeSwitch->stores[i]->iterate(storeSwitch, visitor,
						visitorCtx);
			}
		}
	}
}

static const UA_NodeId *
UA_Nodestore_Switch_getReferenceTypeId(void *nsCtx, UA_Byte refTypeIndex) {
	UA_Nodestore_Switch *storeSwitch = (UA_Nodestore_Switch *) nsCtx;
	return storeSwitch->defaultStore->getReferenceTypeId(storeSwitch->defaultStore->context, refTypeIndex);
}

/*
 * Switch API: Get an interface of the default nodestore or a switch
 */

UA_StatusCode UA_Nodestore_Default_Interface_new(UA_Nodestore** store) {
	UA_Nodestore* defaultStore = (UA_Nodestore*) UA_malloc(
			sizeof(UA_Nodestore));
	if (defaultStore == NULL) {
		return UA_STATUSCODE_BADOUTOFMEMORY;
	}

	UA_StatusCode result = UA_Nodestore_HashMap(defaultStore);
	if (result != UA_STATUSCODE_GOOD) {
		UA_free(defaultStore);
		return result;
	}
    *store = defaultStore;
	return UA_STATUSCODE_GOOD;
}

static void
UA_Nodestore_Switch_delete(void *nsCtx) {
	UA_Nodestore_Switch *storeSwitch = (UA_Nodestore_Switch *) nsCtx;
	// delete custom nodestores
	for (size_t i = 0; i < storeSwitch->size; i++) {
		// if namespace i has custom nodestore
		if (storeSwitch->stores[i]) {
			//check that custom nodestore is not equal to default nodestore
			if (storeSwitch->stores[i] == storeSwitch->defaultStore) {
				storeSwitch->stores[i] = NULL;
				continue;
			}
			// search forward for other occurances of nodestore and set to null
			for (size_t j = i + 1; j < storeSwitch->size; j++) {
				// check that nodestore is equal by comparision of interfaces
				if (storeSwitch->stores[j] == storeSwitch->stores[i])
					storeSwitch->stores[j] = NULL;
			}
			// delete the nodestore
			storeSwitch->stores[i]->clear(
					storeSwitch->stores[i]->context);
			UA_free(storeSwitch->stores[i]);
			storeSwitch->stores[i] = NULL;
		}
	}
	// delete default nodestore
	if (storeSwitch->defaultStore) {
		storeSwitch->defaultStore->clear(
				storeSwitch->defaultStore->context);
		UA_free(storeSwitch->defaultStore);
		storeSwitch->defaultStore = NULL;
	}

	// delete nodestore switch itself
	UA_free(storeSwitch->stores);
	storeSwitch->stores = NULL;
	UA_free(storeSwitch->nsi);
	UA_free(storeSwitch);
}

static UA_Nodestore* UA_Nodestore_Switch_Interface_new(
		UA_Nodestore_Switch *storeSwitch) {
	UA_Nodestore* ns = (UA_Nodestore*) UA_malloc(
			sizeof(UA_Nodestore));
	if (ns == NULL) {
		return NULL;
	}
	ns->context = storeSwitch;
	ns->clear = UA_Nodestore_Switch_delete;
	ns->newNode = UA_Nodestore_Switch_newNode;
	ns->deleteNode = UA_Nodestore_Switch_deleteNode;
	ns->getNode = UA_Nodestore_Switch_getNode;
	//ns->getNodeFromPtr = UA_Nodestore_Switch_getNodeFromPtr;
	ns->releaseNode = UA_Nodestore_Switch_releaseNode;
	ns->getNodeCopy = UA_Nodestore_Switch_getNodeCopy;
	ns->insertNode = UA_Nodestore_Switch_insertNode;
	ns->replaceNode = UA_Nodestore_Switch_replaceNode;
	ns->iterate = UA_Nodestore_Switch_iterate;
	ns->removeNode = UA_Nodestore_Switch_removeNode;
    ns->getReferenceTypeId = UA_Nodestore_Switch_getReferenceTypeId;

	return ns;
}

UA_Nodestore*
UA_Nodestore_Switch_Interface_get(UA_Nodestore_Switch *storeSwitch) {
	return storeSwitch->nsi;
}

/*
 * Nodestore switch life cycle
 */

UA_StatusCode
UA_Nodestore_Switch_newEmpty(UA_Nodestore_Switch **nsSwitch) {
	UA_Nodestore_Switch *storeSwitch = (UA_Nodestore_Switch *) UA_malloc(
			sizeof(UA_Nodestore_Switch));
	if (!storeSwitch)
		return UA_STATUSCODE_BADOUTOFMEMORY;
	storeSwitch->nsi = UA_Nodestore_Switch_Interface_new(storeSwitch);
	if (!storeSwitch->nsi) {
		UA_free(storeSwitch);
		return UA_STATUSCODE_BADOUTOFMEMORY;
	}
	storeSwitch->defaultStore = NULL;
	storeSwitch->stores = NULL;
	storeSwitch->size = 0;
	*nsSwitch = storeSwitch;
	return UA_STATUSCODE_GOOD;
}

UA_StatusCode
UA_Nodestore_Switch_new(UA_Nodestore_Switch **nsSwitch) {
	UA_StatusCode result = UA_Nodestore_Switch_newEmpty(nsSwitch);
	if (result != UA_STATUSCODE_GOOD)
		return result;
	result = UA_Nodestore_Default_Interface_new(
			&(*nsSwitch)->defaultStore);
	if (result != UA_STATUSCODE_GOOD) {
		UA_free(*nsSwitch);
	}
	return result;
}


UA_StatusCode
UA_Nodestore_Switch_link(UA_Nodestore *ns, UA_Nodestore_Switch* nsSwitch) {
	ns->context = nsSwitch;
	ns->clear = UA_Nodestore_Switch_delete;
	ns->newNode = UA_Nodestore_Switch_newNode;
	ns->deleteNode = UA_Nodestore_Switch_deleteNode;
	ns->getNode = UA_Nodestore_Switch_getNode;
	//ns->getNodeFromPtr = UA_Nodestore_Switch_getNodeFromPtr;
	ns->releaseNode = UA_Nodestore_Switch_releaseNode;
	ns->getNodeCopy = UA_Nodestore_Switch_getNodeCopy;
	ns->insertNode = UA_Nodestore_Switch_insertNode;
	ns->replaceNode = UA_Nodestore_Switch_replaceNode;
	ns->iterate = UA_Nodestore_Switch_iterate;
	ns->removeNode = UA_Nodestore_Switch_removeNode;
    ns->getReferenceTypeId = UA_Nodestore_Switch_getReferenceTypeId;
	return UA_STATUSCODE_GOOD;
}