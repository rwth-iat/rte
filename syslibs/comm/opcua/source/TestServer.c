/*
 * TestServer.c
 *
 *  Created on: 10.10.2018
 *      Author: vitus
 */


#include <signal.h>
#include "nodeStoreSwitch.h"

UA_Boolean running = true;
static void stopHandler(int sig) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
    running = false;
}

int main(void) {
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);

    UA_ServerConfig *config = UA_ServerConfig_new_default();
    UA_Nodestore *ns = (UA_Nodestore*)UA_malloc(sizeof *ns);
    UA_NodestoreSwitch *pSwitch = UA_NodestoreSwitch_new(ns);
    config->nodestore = *ns;
    UA_Server *server = UA_Server_new(config);
    UA_Nodestore *ns2 = (UA_Nodestore*)UA_malloc(sizeof *ns2);
    UA_StatusCode retval = UA_Nodestore_default_new(ns2);

    UA_NodestoreSwitch_linkNodestore(pSwitch, ns2);
    UA_NodestoreSwitch_newNode( pSwitch, UA_NODECLASS_VARIABLE);


    /* Add the variable node to the information model */
    UA_NodeId myIntegerNodeId = UA_NODEID_STRING(1, "the.answer");

    UA_QualifiedName myIntegerName = UA_QUALIFIEDNAME(1, "the answer");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_Node testNode;
    testNode.context = &testNode;
    testNode.nodeId = myIntegerNodeId;
    testNode.browseName = myIntegerName;
    testNode.writeMask = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    UA_NodeReferenceKind *ref = (UA_NodeReferenceKind*) UA_malloc(sizeof *ref);
    ref->isInverse = true;
    ref->referenceTypeId = parentNodeId;
    testNode.references = ref;


    retval = UA_NodestoreSwitch_insertNode_inNS(pSwitch, ns2, &testNode, &myIntegerNodeId);

    retval = UA_Server_run(server, &running);
    UA_Server_delete(server);
    UA_ServerConfig_delete(config);
    return (int)retval;
}
