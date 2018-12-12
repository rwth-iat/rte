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

    //standardnodestore mitübergeben, oder löschen
    UA_NodestoreSwitch *pSwitch = UA_NodestoreSwitch_new();
    UA_Server *server = UA_Server_new(config);



    /* Add the variable node to the information model */
    UA_NodeId myIntegerNodeId = UA_NODEID_STRING(1, "the.answer");

    UA_QualifiedName myIntegerName = UA_QUALIFIEDNAME(1, "the answer");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_Node testNode;
    testNode.context = &testNode;
    testNode.nodeId = myIntegerNodeId;
    testNode.browseName = myIntegerName;
    testNode.writeMask = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;


//    UA_VariableAttributes attr = UA_VariableAttributes_default;
//       UA_Int32 myInteger = 42;
//       UA_Variant_setScalar(&attr.value, &myInteger, &UA_TYPES[UA_TYPES_INT32]);
//       attr.description = UA_LOCALIZEDTEXT("en-US","the answer");
//       attr.displayName = UA_LOCALIZEDTEXT("en-US","the answer");
//       attr.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
//       attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
//
//       /* Add the variable node to the information model */
//       UA_NodeId myIntegerNodeId = UA_NODEID_STRING(1, "the.answer");
//       UA_QualifiedName myIntegerName = UA_QUALIFIEDNAME(1, "the answer");
//       UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
//       UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
//       UA_Server_addVariableNode(server, myIntegerNodeId, parentNodeId,
//                                 parentReferenceNodeId, myIntegerName,
//                                 UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), attr, NULL, NULL);

//    /* Add the variable node to the information model */
//    UA_NodeId myIntegerNodeId = UA_NODEID_STRING(1, "the.answer");
//
//    UA_QualifiedName myIntegerName = UA_QUALIFIEDNAME(1, "the answer");
//    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
//    UA_Node testNode;
//    testNode.context = &testNode;
//    testNode.nodeId = myIntegerNodeId;
//    testNode.browseName = myIntegerName;
//    testNode.writeMask = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
//
//    UA_NodeReferenceKind *ref = (UA_NodeReferenceKind*) UA_malloc(sizeof *ref);
//    ref->isInverse = true;
//    ref->referenceTypeId = parentNodeId;
//    testNode.references = ref;
//
//
//    retval = UA_NodestoreSwitch_insertNode_inNS(pSwitch, ns2, &testNode, &myIntegerNodeId);

    UA_StatusCode retval = UA_Server_run(server, &running);
    UA_Server_delete(server);
    UA_ServerConfig_delete(config);
    return (int)retval;

}
