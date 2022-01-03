/*
 * trafo.c
 *
 *  Created on: 26.02.2019
 *      Author: julian
 */

#include "libov/ov_macros.h"
#include "myLibrary_trafo.h"

static void myLibrary_trafo_deleteNodestore(void *context){
}

static UA_Node * myLibrary_trafo_newNode(void * context, UA_NodeClass nodeClass){
    return NULL;
}

static void myLibrary_trafo_deleteNode(void * context, UA_Node *node){
	if(node){
		UA_Node_deleteMembers(node);
	}
	UA_free(node);
}

static const UA_Node * myLibrary_trafo_getNode(void * context, const UA_NodeId *nodeId){
	return NULL;
}

static UA_StatusCode myLibrary_trafo_insert(void *context, UA_Node *node, UA_NodeId *parrentNode){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

static void myLibrary_trafo_releaseNode(void *context, const UA_Node *node){
	myLibrary_trafo_deleteNode(context, (UA_Node*)node);
}

static UA_StatusCode myLibrary_trafo_getCopyNode(void *context, const UA_NodeId *nodeId, UA_Node ** nodeOut){
	UA_Node* node = (UA_Node*) myLibrary_trafo_getNode(context, nodeId);
	if(node == NULL)
		return UA_STATUSCODE_BADNODEIDUNKNOWN;
	*nodeOut = node;
	return UA_STATUSCODE_GOOD;
}

static UA_StatusCode myLibrary_trafo_insertNode(void *context, UA_Node *node, UA_NodeId *parrentNode){
	return myLibrary_trafo_insert(context, node, parrentNode);
}

static UA_StatusCode myLibrary_trafo_replaceNode(void *context, UA_Node *node){

	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

static UA_StatusCode myLibrary_trafo_removeNode(void *context, const UA_NodeId *nodeId){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

static void myLibrary_trafo_iterate(void *context, void* visitorHandle, UA_NodestoreVisitor visitor){
}

UA_Nodestore* myLibrary_trafo_new(OV_INSTPTR_myLibrary_uaInterface context) {
	UA_Nodestore* trafo = UA_malloc(sizeof(UA_Nodestore));
	if(trafo == NULL)
		return NULL;
    trafo->context =        	context;
    trafo->newNode =       	  myLibrary_trafo_newNode;
    trafo->deleteNode =    	  myLibrary_trafo_deleteNode;
    trafo->deleteNodestore =  myLibrary_trafo_deleteNodestore;

    trafo->getNode =          myLibrary_trafo_getNode;
    trafo->releaseNode =      myLibrary_trafo_releaseNode;

    trafo->getNodeCopy =      myLibrary_trafo_getCopyNode;
    trafo->insertNode =       myLibrary_trafo_insertNode;
    trafo->replaceNode =      myLibrary_trafo_replaceNode;

    trafo->removeNode =       myLibrary_trafo_removeNode;

    trafo->iterate =          myLibrary_trafo_iterate;
    return trafo;
}

void myLibrary_trafo_delete(UA_Nodestore * trafo){
	trafo->context = NULL;
	UA_free(trafo);
}
