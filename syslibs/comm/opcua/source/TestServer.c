/*
 * TestServer.c
 *
 *  Created on: 10.10.2018
 *      Author: vitus
 */



#include "nsSwitchInterface.h"
#include <signal.h>

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
    UA_StatusCode retval = UA_NodestoreSwitch_new(ns);
    config->nodestore = *ns;
    UA_Server *server = UA_Server_new(config);

    retval = UA_Server_run(server, &running);
    UA_Server_delete(server);
    UA_ServerConfig_delete(config);
    return (int)retval;
}
