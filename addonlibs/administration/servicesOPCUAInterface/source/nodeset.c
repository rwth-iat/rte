/* WARNING: This is a generated file.
 * Any manual changes will be overwritten. */

#include <nodeset_services.h>


/* ServiceType - ns=2;i=1002 */

static UA_StatusCode function_services_0_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_ObjectTypeAttributes attr = UA_ObjectTypeAttributes_default;
attr.displayName = UA_LOCALIZEDTEXT("", "ServiceType");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_OBJECTTYPE,
UA_NODEID_NUMERIC(ns[2], UA_NSSERVICESID_SERVICESTYPE),
UA_NODEID_NUMERIC(ns[0], 58),
UA_NODEID_NUMERIC(ns[0], 45),
UA_QUALIFIEDNAME(ns[2], "ServiceType"),
 UA_NODEID_NULL,
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_OBJECTTYPEATTRIBUTES],NULL, NULL);
return retVal;
}

static UA_StatusCode function_services_0_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 1002)
);
}

/* WSDLDescription - ns=2;i=6004 */

static UA_StatusCode function_services_1_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 3;
attr.accessLevel = 3;
/* Value rank inherited */
attr.valueRank = -1;
attr.dataType = UA_NODEID_NUMERIC(ns[0], 12);
attr.displayName = UA_LOCALIZEDTEXT("", "WSDLDescription");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6004),
UA_NODEID_NUMERIC(ns[2], UA_NSSERVICESID_SERVICESTYPE),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[2], "WSDLDescription"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6004), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_services_1_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6004)
);
}

/* ID - ns=2;i=6001 */

static UA_StatusCode function_services_2_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 3;
attr.accessLevel = 3;
/* Value rank inherited */
attr.valueRank = -1;
attr.dataType = UA_NODEID_NUMERIC(ns[1], 3002);
attr.displayName = UA_LOCALIZEDTEXT("", "ID");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6001),
UA_NODEID_NUMERIC(ns[2], UA_NSSERVICESID_SERVICESTYPE),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[2], "ID"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6001), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_services_2_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6001)
);
}

/* Version - ns=2;i=6003 */

static UA_StatusCode function_services_3_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 3;
attr.accessLevel = 3;
/* Value rank inherited */
attr.valueRank = -1;
attr.dataType = UA_NODEID_NUMERIC(ns[0], 7);
attr.displayName = UA_LOCALIZEDTEXT("", "Version");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6003),
UA_NODEID_NUMERIC(ns[2], UA_NSSERVICESID_SERVICESTYPE),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[2], "Version"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6003), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_services_3_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6003)
);
}

/* Revision - ns=2;i=6002 */

static UA_StatusCode function_services_4_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 3;
attr.accessLevel = 3;
/* Value rank inherited */
attr.valueRank = -1;
attr.dataType = UA_NODEID_NUMERIC(ns[0], 7);
attr.displayName = UA_LOCALIZEDTEXT("", "Revision");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6002),
UA_NODEID_NUMERIC(ns[2], UA_NSSERVICESID_SERVICESTYPE),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[2], "Revision"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6002), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_services_4_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6002)
);
}

UA_StatusCode nodeset_services(UA_Server *server) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
/* Use namespace ids generated by the server */
UA_UInt16 ns[3];
ns[0] = UA_Server_addNamespace(server, "http://opcfoundation.org/UA/");
ns[1] = UA_Server_addNamespace(server, "acplt.org/identification/");
ns[2] = UA_Server_addNamespace(server, "acplt.org/services/");
bool dummy = (
!(retVal = function_services_0_begin(server, ns)) &&
!(retVal = function_services_1_begin(server, ns)) &&
!(retVal = function_services_2_begin(server, ns)) &&
!(retVal = function_services_3_begin(server, ns)) &&
!(retVal = function_services_4_begin(server, ns)) &&
!(retVal = function_services_4_finish(server, ns)) &&
!(retVal = function_services_3_finish(server, ns)) &&
!(retVal = function_services_2_finish(server, ns)) &&
!(retVal = function_services_1_finish(server, ns)) &&
!(retVal = function_services_0_finish(server, ns)) 
); (void)(dummy);
return retVal;
}
