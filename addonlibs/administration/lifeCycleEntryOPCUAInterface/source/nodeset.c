/* WARNING: This is a generated file.
 * Any manual changes will be overwritten. */

#include "nodeset_lifeCycleEntry.h"


/* LifeCycleEntry - ns=2;i=3002 */

static UA_StatusCode function_lifeCycleEntry_0_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_DataTypeAttributes attr = UA_DataTypeAttributes_default;
attr.displayName = UA_LOCALIZEDTEXT("", "LifeCycleEntry");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_DATATYPE,
UA_NODEID_NUMERIC(ns[2], 3002),
UA_NODEID_NUMERIC(ns[0], 22),
UA_NODEID_NUMERIC(ns[0], 45),
UA_QUALIFIEDNAME(ns[2], "LifeCycleEntry"),
 UA_NODEID_NULL,
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_DATATYPEATTRIBUTES],NULL, NULL);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_0_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 3002)
);
}

/* Default Binary - ns=2;i=5001 */

static UA_StatusCode function_lifeCycleEntry_1_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_ObjectAttributes attr = UA_ObjectAttributes_default;
attr.displayName = UA_LOCALIZEDTEXT("", "Default Binary");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_OBJECT,
UA_NODEID_NUMERIC(ns[2], 5001),
UA_NODEID_NUMERIC(ns[0], 0),
UA_NODEID_NUMERIC(ns[0], 0),
UA_QUALIFIEDNAME(ns[0], "Default Binary"),
UA_NODEID_NUMERIC(ns[0], 76),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_OBJECTATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 5001), UA_NODEID_NUMERIC(ns[0], 38), UA_EXPANDEDNODEID_NUMERIC(ns[2], 3002), false);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_1_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 5001)
);
}

/* Default XML - ns=2;i=5002 */

static UA_StatusCode function_lifeCycleEntry_2_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_ObjectAttributes attr = UA_ObjectAttributes_default;
attr.displayName = UA_LOCALIZEDTEXT("", "Default XML");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_OBJECT,
UA_NODEID_NUMERIC(ns[2], 5002),
UA_NODEID_NUMERIC(ns[0], 0),
UA_NODEID_NUMERIC(ns[0], 0),
UA_QUALIFIEDNAME(ns[0], "Default XML"),
UA_NODEID_NUMERIC(ns[0], 76),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_OBJECTATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 5002), UA_NODEID_NUMERIC(ns[0], 38), UA_EXPANDEDNODEID_NUMERIC(ns[2], 3002), false);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_2_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 5002)
);
}

/* LCEStatusCode - ns=2;i=3003 */

static UA_StatusCode function_lifeCycleEntry_3_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_DataTypeAttributes attr = UA_DataTypeAttributes_default;
attr.displayName = UA_LOCALIZEDTEXT("", "LCEStatusCode");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_DATATYPE,
UA_NODEID_NUMERIC(ns[2], 3003),
UA_NODEID_NUMERIC(ns[0], 19),
UA_NODEID_NUMERIC(ns[0], 45),
UA_QUALIFIEDNAME(ns[2], "LCEStatusCode"),
 UA_NODEID_NULL,
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_DATATYPEATTRIBUTES],NULL, NULL);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_3_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 3003)
);
}

/* TypeDictionary - ns=2;i=6001 */
static const UA_Byte variablenode_ns_2_i_6001_variant_DataContents_byteArray[944] = {60, 111, 112, 99, 58, 84, 121, 112, 101, 68, 105, 99, 116, 105, 111, 110, 97, 114, 121, 32, 120, 109, 108, 110, 115, 58, 120, 115, 105, 61, 34, 104, 116, 116, 112, 58, 47, 47, 119, 119, 119, 46, 119, 51, 46, 111, 114, 103, 47, 50, 48, 48, 49, 47, 88, 77, 76, 83, 99, 104, 101, 109, 97, 45, 105, 110, 115, 116, 97, 110, 99, 101, 34, 32, 120, 109, 108, 110, 115, 58, 116, 110, 115, 61, 34, 104, 116, 116, 112, 58, 47, 47, 121, 111, 117, 114, 111, 114, 103, 97, 110, 105, 115, 97, 116, 105, 111, 110, 46, 111, 114, 103, 47, 108, 99, 101, 47, 34, 32, 68, 101, 102, 97, 117, 108, 116, 66, 121, 116, 101, 79, 114, 100, 101, 114, 61, 34, 76, 105, 116, 116, 108, 101, 69, 110, 100, 105, 97, 110, 34, 32, 120, 109, 108, 110, 115, 58, 111, 112, 99, 61, 34, 104, 116, 116, 112, 58, 47, 47, 111, 112, 99, 102, 111, 117, 110, 100, 97, 116, 105, 111, 110, 46, 111, 114, 103, 47, 66, 105, 110, 97, 114, 121, 83, 99, 104, 101, 109, 97, 47, 34, 32, 120, 109, 108, 110, 115, 58, 110, 115, 49, 61, 34, 104, 116, 116, 112, 58, 47, 47, 121, 111, 117, 114, 111, 114, 103, 97, 110, 105, 115, 97, 116, 105, 111, 110, 46, 111, 114, 103, 47, 105, 100, 101, 110, 116, 105, 102, 105, 99, 97, 116, 105, 111, 110, 47, 34, 32, 120, 109, 108, 110, 115, 58, 117, 97, 61, 34, 104, 116, 116, 112, 58, 47, 47, 111, 112, 99, 102, 111, 117, 110, 100, 97, 116, 105, 111, 110, 46, 111, 114, 103, 47, 85, 65, 47, 34, 32, 84, 97, 114, 103, 101, 116, 78, 97, 109, 101, 115, 112, 97, 99, 101, 61, 34, 104, 116, 116, 112, 58, 47, 47, 121, 111, 117, 114, 111, 114, 103, 97, 110, 105, 115, 97, 116, 105, 111, 110, 46, 111, 114, 103, 47, 108, 99, 101, 47, 34, 62, 10, 32, 60, 111, 112, 99, 58, 73, 109, 112, 111, 114, 116, 32, 78, 97, 109, 101, 115, 112, 97, 99, 101, 61, 34, 104, 116, 116, 112, 58, 47, 47, 111, 112, 99, 102, 111, 117, 110, 100, 97, 116, 105, 111, 110, 46, 111, 114, 103, 47, 85, 65, 47, 34, 47, 62, 10, 32, 60, 111, 112, 99, 58, 83, 116, 114, 117, 99, 116, 117, 114, 101, 100, 84, 121, 112, 101, 32, 66, 97, 115, 101, 84, 121, 112, 101, 61, 34, 117, 97, 58, 69, 120, 116, 101, 110, 115, 105, 111, 110, 79, 98, 106, 101, 99, 116, 34, 32, 78, 97, 109, 101, 61, 34, 76, 105, 102, 101, 67, 121, 99, 108, 101, 69, 110, 116, 114, 121, 34, 62, 10, 32, 32, 60, 111, 112, 99, 58, 70, 105, 101, 108, 100, 32, 84, 121, 112, 101, 78, 97, 109, 101, 61, 34, 110, 115, 49, 58, 73, 100, 101, 110, 116, 105, 102, 105, 99, 97, 116, 105, 111, 110, 34, 32, 78, 97, 109, 101, 61, 34, 67, 114, 101, 97, 116, 105, 110, 103, 73, 110, 115, 116, 97, 110, 99, 101, 34, 47, 62, 10, 32, 32, 60, 111, 112, 99, 58, 70, 105, 101, 108, 100, 32, 84, 121, 112, 101, 78, 97, 109, 101, 61, 34, 110, 115, 49, 58, 73, 100, 101, 110, 116, 105, 102, 105, 99, 97, 116, 105, 111, 110, 34, 32, 78, 97, 109, 101, 61, 34, 87, 114, 105, 116, 105, 110, 103, 73, 110, 115, 116, 97, 110, 99, 101, 34, 47, 62, 10, 32, 32, 60, 111, 112, 99, 58, 70, 105, 101, 108, 100, 32, 84, 121, 112, 101, 78, 97, 109, 101, 61, 34, 117, 97, 58, 68, 97, 116, 97, 86, 97, 108, 117, 101, 34, 32, 78, 97, 109, 101, 61, 34, 68, 97, 116, 97, 34, 47, 62, 10, 32, 32, 60, 111, 112, 99, 58, 70, 105, 101, 108, 100, 32, 84, 121, 112, 101, 78, 97, 109, 101, 61, 34, 111, 112, 99, 58, 67, 104, 97, 114, 65, 114, 114, 97, 121, 34, 32, 78, 97, 109, 101, 61, 34, 83, 117, 98, 106, 101, 99, 116, 34, 47, 62, 10, 32, 32, 60, 111, 112, 99, 58, 70, 105, 101, 108, 100, 32, 84, 121, 112, 101, 78, 97, 109, 101, 61, 34, 111, 112, 99, 58, 67, 104, 97, 114, 65, 114, 114, 97, 121, 34, 32, 78, 97, 109, 101, 61, 34, 69, 118, 101, 110, 116, 67, 108, 97, 115, 115, 34, 47, 62, 10, 32, 32, 60, 111, 112, 99, 58, 70, 105, 101, 108, 100, 32, 84, 121, 112, 101, 78, 97, 109, 101, 61, 34, 111, 112, 99, 58, 85, 73, 110, 116, 54, 52, 34, 32, 78, 97, 109, 101, 61, 34, 73, 100, 34, 47, 62, 10, 32, 60, 47, 111, 112, 99, 58, 83, 116, 114, 117, 99, 116, 117, 114, 101, 100, 84, 121, 112, 101, 62, 10, 32, 60, 111, 112, 99, 58, 105, 109, 112, 111, 114, 116, 32, 78, 97, 109, 101, 115, 112, 97, 99, 101, 61, 34, 104, 116, 116, 112, 58, 47, 47, 121, 111, 117, 114, 111, 114, 103, 97, 110, 105, 115, 97, 116, 105, 111, 110, 46, 111, 114, 103, 47, 105, 100, 101, 110, 116, 105, 102, 105, 99, 97, 116, 105, 111, 110, 47, 34, 47, 62, 10, 60, 47, 111, 112, 99, 58, 84, 121, 112, 101, 68, 105, 99, 116, 105, 111, 110, 97, 114, 121, 62, 10};



static UA_StatusCode function_lifeCycleEntry_4_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
/* Value rank inherited */
attr.valueRank = -1;
attr.dataType = UA_NODEID_NUMERIC(ns[0], 15);
UA_ByteString *variablenode_ns_2_i_6001_variant_DataContents =  UA_ByteString_new();
if (!variablenode_ns_2_i_6001_variant_DataContents) return UA_STATUSCODE_BADOUTOFMEMORY;
UA_ByteString_init(variablenode_ns_2_i_6001_variant_DataContents);
variablenode_ns_2_i_6001_variant_DataContents->length = 944;
variablenode_ns_2_i_6001_variant_DataContents->data = (UA_Byte *)(void*)(uintptr_t)variablenode_ns_2_i_6001_variant_DataContents_byteArray;
UA_Variant_setScalar(&attr.value, variablenode_ns_2_i_6001_variant_DataContents, &UA_TYPES[UA_TYPES_BYTESTRING]);
attr.displayName = UA_LOCALIZEDTEXT("", "TypeDictionary");
attr.description = UA_LOCALIZEDTEXT("", "Collects the data type descriptions of acplt.org/lifeCycleEntry/");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6001),
UA_NODEID_NUMERIC(ns[0], 93),
UA_NODEID_NUMERIC(ns[0], 47),
UA_QUALIFIEDNAME(ns[2], "TypeDictionary"),
UA_NODEID_NUMERIC(ns[0], 72),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
variablenode_ns_2_i_6001_variant_DataContents->data = NULL;
variablenode_ns_2_i_6001_variant_DataContents->length = 0;
UA_ByteString_delete(variablenode_ns_2_i_6001_variant_DataContents);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_4_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6001)
);
}

/* LifeCycleEntry - ns=2;i=6005 */

static UA_StatusCode function_lifeCycleEntry_5_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
/* Value rank inherited */
attr.valueRank = -1;
attr.dataType = UA_NODEID_NUMERIC(ns[0], 12);
UA_String *variablenode_ns_2_i_6005_variant_DataContents =  UA_String_new();
if (!variablenode_ns_2_i_6005_variant_DataContents) return UA_STATUSCODE_BADOUTOFMEMORY;
UA_String_init(variablenode_ns_2_i_6005_variant_DataContents);
*variablenode_ns_2_i_6005_variant_DataContents = UA_STRING_ALLOC("LifeCycleEntry");
UA_Variant_setScalar(&attr.value, variablenode_ns_2_i_6005_variant_DataContents, &UA_TYPES[UA_TYPES_STRING]);
attr.displayName = UA_LOCALIZEDTEXT("", "LifeCycleEntry");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6005),
UA_NODEID_NUMERIC(ns[2], 6001),
UA_NODEID_NUMERIC(ns[0], 47),
UA_QUALIFIEDNAME(ns[2], "LifeCycleEntry"),
UA_NODEID_NUMERIC(ns[0], 69),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
UA_String_delete(variablenode_ns_2_i_6005_variant_DataContents);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6005), UA_NODEID_NUMERIC(ns[0], 39), UA_EXPANDEDNODEID_NUMERIC(ns[2], 5001), false);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_5_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6005)
);
}

/* NamespaceUri - ns=2;i=6002 */

static UA_StatusCode function_lifeCycleEntry_6_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
/* Value rank inherited */
attr.valueRank = -1;
attr.dataType = UA_NODEID_NUMERIC(ns[0], 12);
UA_String *variablenode_ns_2_i_6002_variant_DataContents =  UA_String_new();
if (!variablenode_ns_2_i_6002_variant_DataContents) return UA_STATUSCODE_BADOUTOFMEMORY;
UA_String_init(variablenode_ns_2_i_6002_variant_DataContents);
*variablenode_ns_2_i_6002_variant_DataContents = UA_STRING_ALLOC("acplt.org/lifeCycleEntry/");
UA_Variant_setScalar(&attr.value, variablenode_ns_2_i_6002_variant_DataContents, &UA_TYPES[UA_TYPES_STRING]);
attr.displayName = UA_LOCALIZEDTEXT("", "NamespaceUri");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6002),
UA_NODEID_NUMERIC(ns[2], 6001),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[0], "NamespaceUri"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
UA_String_delete(variablenode_ns_2_i_6002_variant_DataContents);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_6_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6002)
);
}

/* LifeCycleArchiveType - ns=2;i=1002 */

static UA_StatusCode function_lifeCycleEntry_7_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_ObjectTypeAttributes attr = UA_ObjectTypeAttributes_default;
attr.displayName = UA_LOCALIZEDTEXT("", "LifeCycleArchiveType");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_OBJECTTYPE,
UA_NODEID_NUMERIC(ns[2], 1002),
UA_NODEID_NUMERIC(ns[0], 58),
UA_NODEID_NUMERIC(ns[0], 45),
UA_QUALIFIEDNAME(ns[2], "LifeCycleArchiveType"),
 UA_NODEID_NULL,
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_OBJECTTYPEATTRIBUTES],NULL, NULL);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_7_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 1002)
);
}

/* getLastLCEs - ns=2;i=7009 */

static UA_StatusCode function_lifeCycleEntry_8_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_MethodAttributes attr = UA_MethodAttributes_default;
attr.executable = true;
attr.userExecutable = true;
attr.displayName = UA_LOCALIZEDTEXT("", "getLastLCEs");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_METHOD,
UA_NODEID_NUMERIC(ns[2], 7009),
UA_NODEID_NUMERIC(ns[2], 1002),
UA_NODEID_NUMERIC(ns[0], 47),
UA_QUALIFIEDNAME(ns[2], "getLastLCEs"),
 UA_NODEID_NULL,
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_METHODATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 7009), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 11510), true);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_8_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addMethodNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 7009)
, NULL, 0, NULL, 0, NULL);
}

/* OutputArguments - ns=2;i=6015 */

static UA_StatusCode function_lifeCycleEntry_9_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
attr.valueRank = 1;
attr.arrayDimensionsSize = 1;
UA_UInt32 arrayDimensions[1];
arrayDimensions[0] = 3;
attr.arrayDimensions = &arrayDimensions[0];
attr.dataType = UA_NODEID_NUMERIC(ns[0], 296);
UA_Argument variablenode_ns_2_i_6015_variant_DataContents[3];

UA_init(&variablenode_ns_2_i_6015_variant_DataContents[0], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6015_variant_DataContents[0].name = UA_STRING("Status");
variablenode_ns_2_i_6015_variant_DataContents[0].dataType = UA_NODEID_NUMERIC(ns[2], 3003);
variablenode_ns_2_i_6015_variant_DataContents[0].valueRank = (UA_Int32) -1;

UA_init(&variablenode_ns_2_i_6015_variant_DataContents[1], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6015_variant_DataContents[1].name = UA_STRING("LastLCEs");
variablenode_ns_2_i_6015_variant_DataContents[1].dataType = UA_NODEID_NUMERIC(ns[2], 3002);
variablenode_ns_2_i_6015_variant_DataContents[1].valueRank = (UA_Int32) -1;

UA_init(&variablenode_ns_2_i_6015_variant_DataContents[2], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6015_variant_DataContents[2].name = UA_STRING("LastLCEsCount");
variablenode_ns_2_i_6015_variant_DataContents[2].dataType = UA_NODEID_NUMERIC(ns[0], 7);
variablenode_ns_2_i_6015_variant_DataContents[2].valueRank = (UA_Int32) -1;
UA_Variant_setArray(&attr.value, &variablenode_ns_2_i_6015_variant_DataContents, (UA_Int32) 3, &UA_TYPES[UA_TYPES_ARGUMENT]);
attr.value.arrayDimensionsSize = attr.arrayDimensionsSize;
attr.value.arrayDimensions = attr.arrayDimensions;
attr.displayName = UA_LOCALIZEDTEXT("", "OutputArguments");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6015),
UA_NODEID_NUMERIC(ns[2], 7009),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[0], "OutputArguments"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);



retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6015), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_9_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6015)
);
}

/* InputArguments - ns=2;i=6014 */

static UA_StatusCode function_lifeCycleEntry_10_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
attr.valueRank = 1;
attr.arrayDimensionsSize = 1;
UA_UInt32 arrayDimensions[1];
arrayDimensions[0] = 1;
attr.arrayDimensions = &arrayDimensions[0];
attr.dataType = UA_NODEID_NUMERIC(ns[0], 296);
UA_Argument variablenode_ns_2_i_6014_variant_DataContents[1];

UA_init(&variablenode_ns_2_i_6014_variant_DataContents[0], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6014_variant_DataContents[0].name = UA_STRING("Count");
variablenode_ns_2_i_6014_variant_DataContents[0].dataType = UA_NODEID_NUMERIC(ns[0], 7);
variablenode_ns_2_i_6014_variant_DataContents[0].valueRank = (UA_Int32) -1;
UA_Variant_setArray(&attr.value, &variablenode_ns_2_i_6014_variant_DataContents, (UA_Int32) 1, &UA_TYPES[UA_TYPES_ARGUMENT]);
attr.value.arrayDimensionsSize = attr.arrayDimensionsSize;
attr.value.arrayDimensions = attr.arrayDimensions;
attr.displayName = UA_LOCALIZEDTEXT("", "InputArguments");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6014),
UA_NODEID_NUMERIC(ns[2], 7009),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[0], "InputArguments"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);

retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6014), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_10_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6014)
);
}

/* setLCESimple - ns=2;i=7008 */

static UA_StatusCode function_lifeCycleEntry_11_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_MethodAttributes attr = UA_MethodAttributes_default;
attr.executable = true;
attr.userExecutable = true;
attr.displayName = UA_LOCALIZEDTEXT("", "setLCESimple");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_METHOD,
UA_NODEID_NUMERIC(ns[2], 7008),
UA_NODEID_NUMERIC(ns[2], 1002),
UA_NODEID_NUMERIC(ns[0], 47),
UA_QUALIFIEDNAME(ns[2], "setLCESimple"),
 UA_NODEID_NULL,
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_METHODATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 7008), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 11510), true);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_11_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addMethodNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 7008)
, NULL, 0, NULL, 0, NULL);
}

/* InputArguments - ns=2;i=6022 */

static UA_StatusCode function_lifeCycleEntry_12_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
attr.valueRank = 1;
attr.arrayDimensionsSize = 1;
UA_UInt32 arrayDimensions[1];
arrayDimensions[0] = 8;
attr.arrayDimensions = &arrayDimensions[0];
attr.dataType = UA_NODEID_NUMERIC(ns[0], 296);
UA_Argument variablenode_ns_2_i_6022_variant_DataContents[8];

UA_init(&variablenode_ns_2_i_6022_variant_DataContents[0], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6022_variant_DataContents[0].name = UA_STRING("LCEId");
variablenode_ns_2_i_6022_variant_DataContents[0].dataType = UA_NODEID_NUMERIC(ns[0], 9);
variablenode_ns_2_i_6022_variant_DataContents[0].valueRank = (UA_Int32) -1;

UA_init(&variablenode_ns_2_i_6022_variant_DataContents[1], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6022_variant_DataContents[1].name = UA_STRING("CreatingInstanceIdType");
variablenode_ns_2_i_6022_variant_DataContents[1].dataType = UA_NODEID_NUMERIC(ns[0], 7);
variablenode_ns_2_i_6022_variant_DataContents[1].valueRank = (UA_Int32) -1;

UA_init(&variablenode_ns_2_i_6022_variant_DataContents[2], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6022_variant_DataContents[2].name = UA_STRING("CreatingInstanceIdSpec");
variablenode_ns_2_i_6022_variant_DataContents[2].dataType = UA_NODEID_NUMERIC(ns[0], 12);
variablenode_ns_2_i_6022_variant_DataContents[2].valueRank = (UA_Int32) -1;

UA_init(&variablenode_ns_2_i_6022_variant_DataContents[3], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6022_variant_DataContents[3].name = UA_STRING("WritingInstanceIdType");
variablenode_ns_2_i_6022_variant_DataContents[3].dataType = UA_NODEID_NUMERIC(ns[0], 7);
variablenode_ns_2_i_6022_variant_DataContents[3].valueRank = (UA_Int32) -1;

UA_init(&variablenode_ns_2_i_6022_variant_DataContents[4], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6022_variant_DataContents[4].name = UA_STRING("WritingInstanceIdSpec");
variablenode_ns_2_i_6022_variant_DataContents[4].dataType = UA_NODEID_NUMERIC(ns[0], 12);
variablenode_ns_2_i_6022_variant_DataContents[4].valueRank = (UA_Int32) -1;

UA_init(&variablenode_ns_2_i_6022_variant_DataContents[5], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6022_variant_DataContents[5].name = UA_STRING("Data");
variablenode_ns_2_i_6022_variant_DataContents[5].dataType = UA_NODEID_NUMERIC(ns[0], 23);
variablenode_ns_2_i_6022_variant_DataContents[5].valueRank = (UA_Int32) -1;

UA_init(&variablenode_ns_2_i_6022_variant_DataContents[6], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6022_variant_DataContents[6].name = UA_STRING("Subject");
variablenode_ns_2_i_6022_variant_DataContents[6].dataType = UA_NODEID_NUMERIC(ns[0], 12);
variablenode_ns_2_i_6022_variant_DataContents[6].valueRank = (UA_Int32) -1;

UA_init(&variablenode_ns_2_i_6022_variant_DataContents[7], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6022_variant_DataContents[7].name = UA_STRING("EventClass");
variablenode_ns_2_i_6022_variant_DataContents[7].dataType = UA_NODEID_NUMERIC(ns[0], 12);
variablenode_ns_2_i_6022_variant_DataContents[7].valueRank = (UA_Int32) -1;
UA_Variant_setArray(&attr.value, &variablenode_ns_2_i_6022_variant_DataContents, (UA_Int32) 8, &UA_TYPES[UA_TYPES_ARGUMENT]);
attr.value.arrayDimensionsSize = attr.arrayDimensionsSize;
attr.value.arrayDimensions = attr.arrayDimensions;
attr.displayName = UA_LOCALIZEDTEXT("", "InputArguments");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6022),
UA_NODEID_NUMERIC(ns[2], 7008),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[0], "InputArguments"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);








retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6022), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_12_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6022)
);
}

/* OutputArguments - ns=2;i=6007 */

static UA_StatusCode function_lifeCycleEntry_13_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
attr.valueRank = 1;
attr.arrayDimensionsSize = 1;
UA_UInt32 arrayDimensions[1];
arrayDimensions[0] = 1;
attr.arrayDimensions = &arrayDimensions[0];
attr.dataType = UA_NODEID_NUMERIC(ns[0], 296);
UA_Argument variablenode_ns_2_i_6007_variant_DataContents[1];

UA_init(&variablenode_ns_2_i_6007_variant_DataContents[0], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6007_variant_DataContents[0].name = UA_STRING("Status");
variablenode_ns_2_i_6007_variant_DataContents[0].dataType = UA_NODEID_NUMERIC(ns[2], 3003);
variablenode_ns_2_i_6007_variant_DataContents[0].valueRank = (UA_Int32) -1;
UA_Variant_setArray(&attr.value, &variablenode_ns_2_i_6007_variant_DataContents, (UA_Int32) 1, &UA_TYPES[UA_TYPES_ARGUMENT]);
attr.value.arrayDimensionsSize = attr.arrayDimensionsSize;
attr.value.arrayDimensions = attr.arrayDimensions;
attr.displayName = UA_LOCALIZEDTEXT("", "OutputArguments");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6007),
UA_NODEID_NUMERIC(ns[2], 7008),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[0], "OutputArguments"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);

retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6007), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_13_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6007)
);
}

/* createLCE - ns=2;i=7001 */

static UA_StatusCode function_lifeCycleEntry_14_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_MethodAttributes attr = UA_MethodAttributes_default;
attr.executable = true;
attr.userExecutable = true;
attr.displayName = UA_LOCALIZEDTEXT("", "createLCE");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_METHOD,
UA_NODEID_NUMERIC(ns[2], 7001),
UA_NODEID_NUMERIC(ns[2], 1002),
UA_NODEID_NUMERIC(ns[0], 47),
UA_QUALIFIEDNAME(ns[2], "createLCE"),
 UA_NODEID_NULL,
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_METHODATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 7001), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 11510), true);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_14_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addMethodNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 7001)
, NULL, 0, NULL, 0, NULL);
}

/* InputArguments - ns=2;i=6008 */

static UA_StatusCode function_lifeCycleEntry_15_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
attr.valueRank = 1;
attr.arrayDimensionsSize = 1;
UA_UInt32 arrayDimensions[1];
arrayDimensions[0] = 1;
attr.arrayDimensions = &arrayDimensions[0];
attr.dataType = UA_NODEID_NUMERIC(ns[0], 296);
UA_Argument variablenode_ns_2_i_6008_variant_DataContents[1];

UA_init(&variablenode_ns_2_i_6008_variant_DataContents[0], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6008_variant_DataContents[0].name = UA_STRING("LCE");
variablenode_ns_2_i_6008_variant_DataContents[0].dataType = UA_NODEID_NUMERIC(ns[2], 3002);
variablenode_ns_2_i_6008_variant_DataContents[0].valueRank = (UA_Int32) -1;
UA_Variant_setArray(&attr.value, &variablenode_ns_2_i_6008_variant_DataContents, (UA_Int32) 1, &UA_TYPES[UA_TYPES_ARGUMENT]);
attr.value.arrayDimensionsSize = attr.arrayDimensionsSize;
attr.value.arrayDimensions = attr.arrayDimensions;
attr.displayName = UA_LOCALIZEDTEXT("", "InputArguments");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6008),
UA_NODEID_NUMERIC(ns[2], 7001),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[0], "InputArguments"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);

retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6008), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_15_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6008)
);
}

/* OutputArguments - ns=2;i=6009 */

static UA_StatusCode function_lifeCycleEntry_16_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
attr.valueRank = 1;
attr.arrayDimensionsSize = 1;
UA_UInt32 arrayDimensions[1];
arrayDimensions[0] = 1;
attr.arrayDimensions = &arrayDimensions[0];
attr.dataType = UA_NODEID_NUMERIC(ns[0], 296);
UA_Argument variablenode_ns_2_i_6009_variant_DataContents[1];

UA_init(&variablenode_ns_2_i_6009_variant_DataContents[0], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6009_variant_DataContents[0].name = UA_STRING("Status");
variablenode_ns_2_i_6009_variant_DataContents[0].dataType = UA_NODEID_NUMERIC(ns[2], 3003);
variablenode_ns_2_i_6009_variant_DataContents[0].valueRank = (UA_Int32) -1;
UA_Variant_setArray(&attr.value, &variablenode_ns_2_i_6009_variant_DataContents, (UA_Int32) 1, &UA_TYPES[UA_TYPES_ARGUMENT]);
attr.value.arrayDimensionsSize = attr.arrayDimensionsSize;
attr.value.arrayDimensions = attr.arrayDimensions;
attr.displayName = UA_LOCALIZEDTEXT("", "OutputArguments");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6009),
UA_NODEID_NUMERIC(ns[2], 7001),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[0], "OutputArguments"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);

retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6009), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_16_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6009)
);
}

/* deleteLCE - ns=2;i=7003 */

static UA_StatusCode function_lifeCycleEntry_17_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_MethodAttributes attr = UA_MethodAttributes_default;
attr.executable = true;
attr.userExecutable = true;
attr.displayName = UA_LOCALIZEDTEXT("", "deleteLCE");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_METHOD,
UA_NODEID_NUMERIC(ns[2], 7003),
UA_NODEID_NUMERIC(ns[2], 1002),
UA_NODEID_NUMERIC(ns[0], 47),
UA_QUALIFIEDNAME(ns[2], "deleteLCE"),
 UA_NODEID_NULL,
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_METHODATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 7003), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 11510), true);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_17_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addMethodNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 7003)
, NULL, 0, NULL, 0, NULL);
}

/* OutputArguments - ns=2;i=6013 */

static UA_StatusCode function_lifeCycleEntry_18_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
attr.valueRank = 1;
attr.arrayDimensionsSize = 1;
UA_UInt32 arrayDimensions[1];
arrayDimensions[0] = 1;
attr.arrayDimensions = &arrayDimensions[0];
attr.dataType = UA_NODEID_NUMERIC(ns[0], 296);
UA_Argument variablenode_ns_2_i_6013_variant_DataContents[1];

UA_init(&variablenode_ns_2_i_6013_variant_DataContents[0], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6013_variant_DataContents[0].name = UA_STRING("Status");
variablenode_ns_2_i_6013_variant_DataContents[0].dataType = UA_NODEID_NUMERIC(ns[2], 3003);
variablenode_ns_2_i_6013_variant_DataContents[0].valueRank = (UA_Int32) -1;
UA_Variant_setArray(&attr.value, &variablenode_ns_2_i_6013_variant_DataContents, (UA_Int32) 1, &UA_TYPES[UA_TYPES_ARGUMENT]);
attr.value.arrayDimensionsSize = attr.arrayDimensionsSize;
attr.value.arrayDimensions = attr.arrayDimensions;
attr.displayName = UA_LOCALIZEDTEXT("", "OutputArguments");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6013),
UA_NODEID_NUMERIC(ns[2], 7003),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[0], "OutputArguments"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);

retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6013), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_18_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6013)
);
}

/* InputArguments - ns=2;i=6012 */

static UA_StatusCode function_lifeCycleEntry_19_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
attr.valueRank = 1;
attr.arrayDimensionsSize = 1;
UA_UInt32 arrayDimensions[1];
arrayDimensions[0] = 1;
attr.arrayDimensions = &arrayDimensions[0];
attr.dataType = UA_NODEID_NUMERIC(ns[0], 296);
UA_Argument variablenode_ns_2_i_6012_variant_DataContents[1];

UA_init(&variablenode_ns_2_i_6012_variant_DataContents[0], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6012_variant_DataContents[0].name = UA_STRING("LCEId");
variablenode_ns_2_i_6012_variant_DataContents[0].dataType = UA_NODEID_NUMERIC(ns[0], 9);
variablenode_ns_2_i_6012_variant_DataContents[0].valueRank = (UA_Int32) -1;
UA_Variant_setArray(&attr.value, &variablenode_ns_2_i_6012_variant_DataContents, (UA_Int32) 1, &UA_TYPES[UA_TYPES_ARGUMENT]);
attr.value.arrayDimensionsSize = attr.arrayDimensionsSize;
attr.value.arrayDimensions = attr.arrayDimensions;
attr.displayName = UA_LOCALIZEDTEXT("", "InputArguments");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6012),
UA_NODEID_NUMERIC(ns[2], 7003),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[0], "InputArguments"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);

retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6012), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_19_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6012)
);
}

/* createLCESimple - ns=2;i=7002 */

static UA_StatusCode function_lifeCycleEntry_20_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_MethodAttributes attr = UA_MethodAttributes_default;
attr.executable = true;
attr.userExecutable = true;
attr.displayName = UA_LOCALIZEDTEXT("", "createLCESimple");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_METHOD,
UA_NODEID_NUMERIC(ns[2], 7002),
UA_NODEID_NUMERIC(ns[2], 1002),
UA_NODEID_NUMERIC(ns[0], 47),
UA_QUALIFIEDNAME(ns[2], "createLCESimple"),
 UA_NODEID_NULL,
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_METHODATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 7002), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 11510), true);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_20_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addMethodNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 7002)
, NULL, 0, NULL, 0, NULL);
}

/* InputArguments - ns=2;i=6010 */

static UA_StatusCode function_lifeCycleEntry_21_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
attr.valueRank = 1;
attr.arrayDimensionsSize = 1;
UA_UInt32 arrayDimensions[1];
arrayDimensions[0] = 7;
attr.arrayDimensions = &arrayDimensions[0];
attr.dataType = UA_NODEID_NUMERIC(ns[0], 296);
UA_Argument variablenode_ns_2_i_6010_variant_DataContents[7];

UA_init(&variablenode_ns_2_i_6010_variant_DataContents[0], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6010_variant_DataContents[0].name = UA_STRING("CreatingInstanceIdType");
variablenode_ns_2_i_6010_variant_DataContents[0].dataType = UA_NODEID_NUMERIC(ns[0], 7);
variablenode_ns_2_i_6010_variant_DataContents[0].valueRank = (UA_Int32) -1;

UA_init(&variablenode_ns_2_i_6010_variant_DataContents[1], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6010_variant_DataContents[1].name = UA_STRING("CreatingInstanceIdSpec");
variablenode_ns_2_i_6010_variant_DataContents[1].dataType = UA_NODEID_NUMERIC(ns[0], 12);
variablenode_ns_2_i_6010_variant_DataContents[1].valueRank = (UA_Int32) -1;

UA_init(&variablenode_ns_2_i_6010_variant_DataContents[2], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6010_variant_DataContents[2].name = UA_STRING("WritingInstanceIdType");
variablenode_ns_2_i_6010_variant_DataContents[2].dataType = UA_NODEID_NUMERIC(ns[0], 7);
variablenode_ns_2_i_6010_variant_DataContents[2].valueRank = (UA_Int32) -1;

UA_init(&variablenode_ns_2_i_6010_variant_DataContents[3], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6010_variant_DataContents[3].name = UA_STRING("WritingInstanceIdSpec");
variablenode_ns_2_i_6010_variant_DataContents[3].dataType = UA_NODEID_NUMERIC(ns[0], 12);
variablenode_ns_2_i_6010_variant_DataContents[3].valueRank = (UA_Int32) -1;

UA_init(&variablenode_ns_2_i_6010_variant_DataContents[4], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6010_variant_DataContents[4].name = UA_STRING("Data");
variablenode_ns_2_i_6010_variant_DataContents[4].dataType = UA_NODEID_NUMERIC(ns[0], 23);
variablenode_ns_2_i_6010_variant_DataContents[4].valueRank = (UA_Int32) -1;

UA_init(&variablenode_ns_2_i_6010_variant_DataContents[5], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6010_variant_DataContents[5].name = UA_STRING("Subject");
variablenode_ns_2_i_6010_variant_DataContents[5].dataType = UA_NODEID_NUMERIC(ns[0], 12);
variablenode_ns_2_i_6010_variant_DataContents[5].valueRank = (UA_Int32) -1;

UA_init(&variablenode_ns_2_i_6010_variant_DataContents[6], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6010_variant_DataContents[6].name = UA_STRING("EventClass");
variablenode_ns_2_i_6010_variant_DataContents[6].dataType = UA_NODEID_NUMERIC(ns[0], 12);
variablenode_ns_2_i_6010_variant_DataContents[6].valueRank = (UA_Int32) -1;
UA_Variant_setArray(&attr.value, &variablenode_ns_2_i_6010_variant_DataContents, (UA_Int32) 7, &UA_TYPES[UA_TYPES_ARGUMENT]);
attr.value.arrayDimensionsSize = attr.arrayDimensionsSize;
attr.value.arrayDimensions = attr.arrayDimensions;
attr.displayName = UA_LOCALIZEDTEXT("", "InputArguments");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6010),
UA_NODEID_NUMERIC(ns[2], 7002),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[0], "InputArguments"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);







retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6010), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_21_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6010)
);
}

/* OutputArguments - ns=2;i=6011 */

static UA_StatusCode function_lifeCycleEntry_22_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
attr.valueRank = 1;
attr.arrayDimensionsSize = 1;
UA_UInt32 arrayDimensions[1];
arrayDimensions[0] = 1;
attr.arrayDimensions = &arrayDimensions[0];
attr.dataType = UA_NODEID_NUMERIC(ns[0], 296);
UA_Argument variablenode_ns_2_i_6011_variant_DataContents[1];

UA_init(&variablenode_ns_2_i_6011_variant_DataContents[0], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6011_variant_DataContents[0].name = UA_STRING("Status");
variablenode_ns_2_i_6011_variant_DataContents[0].dataType = UA_NODEID_NUMERIC(ns[2], 3003);
variablenode_ns_2_i_6011_variant_DataContents[0].valueRank = (UA_Int32) -1;
UA_Variant_setArray(&attr.value, &variablenode_ns_2_i_6011_variant_DataContents, (UA_Int32) 1, &UA_TYPES[UA_TYPES_ARGUMENT]);
attr.value.arrayDimensionsSize = attr.arrayDimensionsSize;
attr.value.arrayDimensions = attr.arrayDimensions;
attr.displayName = UA_LOCALIZEDTEXT("", "OutputArguments");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6011),
UA_NODEID_NUMERIC(ns[2], 7002),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[0], "OutputArguments"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);

retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6011), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_22_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6011)
);
}

/* getLCE - ns=2;i=7005 */

static UA_StatusCode function_lifeCycleEntry_23_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_MethodAttributes attr = UA_MethodAttributes_default;
attr.executable = true;
attr.userExecutable = true;
attr.displayName = UA_LOCALIZEDTEXT("", "getLCE");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_METHOD,
UA_NODEID_NUMERIC(ns[2], 7005),
UA_NODEID_NUMERIC(ns[2], 1002),
UA_NODEID_NUMERIC(ns[0], 47),
UA_QUALIFIEDNAME(ns[2], "getLCE"),
 UA_NODEID_NULL,
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_METHODATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 7005), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 11510), true);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_23_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addMethodNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 7005)
, NULL, 0, NULL, 0, NULL);
}

/* OutputArguments - ns=2;i=6017 */

static UA_StatusCode function_lifeCycleEntry_24_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
attr.valueRank = 1;
attr.arrayDimensionsSize = 1;
UA_UInt32 arrayDimensions[1];
arrayDimensions[0] = 2;
attr.arrayDimensions = &arrayDimensions[0];
attr.dataType = UA_NODEID_NUMERIC(ns[0], 296);
UA_Argument variablenode_ns_2_i_6017_variant_DataContents[2];

UA_init(&variablenode_ns_2_i_6017_variant_DataContents[0], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6017_variant_DataContents[0].name = UA_STRING("Status");
variablenode_ns_2_i_6017_variant_DataContents[0].dataType = UA_NODEID_NUMERIC(ns[2], 3003);
variablenode_ns_2_i_6017_variant_DataContents[0].valueRank = (UA_Int32) -1;

UA_init(&variablenode_ns_2_i_6017_variant_DataContents[1], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6017_variant_DataContents[1].name = UA_STRING("LCEData");
variablenode_ns_2_i_6017_variant_DataContents[1].dataType = UA_NODEID_NUMERIC(ns[2], 3002);
variablenode_ns_2_i_6017_variant_DataContents[1].valueRank = (UA_Int32) -1;
UA_Variant_setArray(&attr.value, &variablenode_ns_2_i_6017_variant_DataContents, (UA_Int32) 2, &UA_TYPES[UA_TYPES_ARGUMENT]);
attr.value.arrayDimensionsSize = attr.arrayDimensionsSize;
attr.value.arrayDimensions = attr.arrayDimensions;
attr.displayName = UA_LOCALIZEDTEXT("", "OutputArguments");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6017),
UA_NODEID_NUMERIC(ns[2], 7005),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[0], "OutputArguments"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);


retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6017), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_24_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6017)
);
}

/* InputArguments - ns=2;i=6016 */

static UA_StatusCode function_lifeCycleEntry_25_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
attr.valueRank = 1;
attr.arrayDimensionsSize = 1;
UA_UInt32 arrayDimensions[1];
arrayDimensions[0] = 1;
attr.arrayDimensions = &arrayDimensions[0];
attr.dataType = UA_NODEID_NUMERIC(ns[0], 296);
UA_Argument variablenode_ns_2_i_6016_variant_DataContents[1];

UA_init(&variablenode_ns_2_i_6016_variant_DataContents[0], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6016_variant_DataContents[0].name = UA_STRING("LCEId");
variablenode_ns_2_i_6016_variant_DataContents[0].dataType = UA_NODEID_NUMERIC(ns[0], 9);
variablenode_ns_2_i_6016_variant_DataContents[0].valueRank = (UA_Int32) -1;
UA_Variant_setArray(&attr.value, &variablenode_ns_2_i_6016_variant_DataContents, (UA_Int32) 1, &UA_TYPES[UA_TYPES_ARGUMENT]);
attr.value.arrayDimensionsSize = attr.arrayDimensionsSize;
attr.value.arrayDimensions = attr.arrayDimensions;
attr.displayName = UA_LOCALIZEDTEXT("", "InputArguments");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6016),
UA_NODEID_NUMERIC(ns[2], 7005),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[0], "InputArguments"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);

retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6016), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_25_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6016)
);
}

/* setLCE - ns=2;i=7007 */

static UA_StatusCode function_lifeCycleEntry_26_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_MethodAttributes attr = UA_MethodAttributes_default;
attr.executable = true;
attr.userExecutable = true;
attr.displayName = UA_LOCALIZEDTEXT("", "setLCE");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_METHOD,
UA_NODEID_NUMERIC(ns[2], 7007),
UA_NODEID_NUMERIC(ns[2], 1002),
UA_NODEID_NUMERIC(ns[0], 47),
UA_QUALIFIEDNAME(ns[2], "setLCE"),
 UA_NODEID_NULL,
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_METHODATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 7007), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 11510), true);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_26_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addMethodNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 7007)
, NULL, 0, NULL, 0, NULL);
}

/* InputArguments - ns=2;i=6020 */

static UA_StatusCode function_lifeCycleEntry_27_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
attr.valueRank = 1;
attr.arrayDimensionsSize = 1;
UA_UInt32 arrayDimensions[1];
arrayDimensions[0] = 1;
attr.arrayDimensions = &arrayDimensions[0];
attr.dataType = UA_NODEID_NUMERIC(ns[0], 296);
UA_Argument variablenode_ns_2_i_6020_variant_DataContents[1];

UA_init(&variablenode_ns_2_i_6020_variant_DataContents[0], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6020_variant_DataContents[0].name = UA_STRING("LCEData");
variablenode_ns_2_i_6020_variant_DataContents[0].dataType = UA_NODEID_NUMERIC(ns[2], 3002);
variablenode_ns_2_i_6020_variant_DataContents[0].valueRank = (UA_Int32) -1;
UA_Variant_setArray(&attr.value, &variablenode_ns_2_i_6020_variant_DataContents, (UA_Int32) 1, &UA_TYPES[UA_TYPES_ARGUMENT]);
attr.value.arrayDimensionsSize = attr.arrayDimensionsSize;
attr.value.arrayDimensions = attr.arrayDimensions;
attr.displayName = UA_LOCALIZEDTEXT("", "InputArguments");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6020),
UA_NODEID_NUMERIC(ns[2], 7007),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[0], "InputArguments"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);

retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6020), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_27_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6020)
);
}

/* OutputArguments - ns=2;i=6021 */

static UA_StatusCode function_lifeCycleEntry_28_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
attr.valueRank = 1;
attr.arrayDimensionsSize = 1;
UA_UInt32 arrayDimensions[1];
arrayDimensions[0] = 1;
attr.arrayDimensions = &arrayDimensions[0];
attr.dataType = UA_NODEID_NUMERIC(ns[0], 296);
UA_Argument variablenode_ns_2_i_6021_variant_DataContents[1];

UA_init(&variablenode_ns_2_i_6021_variant_DataContents[0], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6021_variant_DataContents[0].name = UA_STRING("Status");
variablenode_ns_2_i_6021_variant_DataContents[0].dataType = UA_NODEID_NUMERIC(ns[2], 3003);
variablenode_ns_2_i_6021_variant_DataContents[0].valueRank = (UA_Int32) -1;
UA_Variant_setArray(&attr.value, &variablenode_ns_2_i_6021_variant_DataContents, (UA_Int32) 1, &UA_TYPES[UA_TYPES_ARGUMENT]);
attr.value.arrayDimensionsSize = attr.arrayDimensionsSize;
attr.value.arrayDimensions = attr.arrayDimensions;
attr.displayName = UA_LOCALIZEDTEXT("", "OutputArguments");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6021),
UA_NODEID_NUMERIC(ns[2], 7007),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[0], "OutputArguments"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);

retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6021), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_28_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6021)
);
}

/* getLCESimple - ns=2;i=7006 */

static UA_StatusCode function_lifeCycleEntry_29_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_MethodAttributes attr = UA_MethodAttributes_default;
attr.executable = true;
attr.userExecutable = true;
attr.displayName = UA_LOCALIZEDTEXT("", "getLCESimple");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_METHOD,
UA_NODEID_NUMERIC(ns[2], 7006),
UA_NODEID_NUMERIC(ns[2], 1002),
UA_NODEID_NUMERIC(ns[0], 47),
UA_QUALIFIEDNAME(ns[2], "getLCESimple"),
 UA_NODEID_NULL,
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_METHODATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 7006), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 11510), true);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_29_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addMethodNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 7006)
, NULL, 0, NULL, 0, NULL);
}

/* InputArguments - ns=2;i=6018 */

static UA_StatusCode function_lifeCycleEntry_30_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
attr.valueRank = 1;
attr.arrayDimensionsSize = 1;
UA_UInt32 arrayDimensions[1];
arrayDimensions[0] = 1;
attr.arrayDimensions = &arrayDimensions[0];
attr.dataType = UA_NODEID_NUMERIC(ns[0], 296);
UA_Argument variablenode_ns_2_i_6018_variant_DataContents[1];

UA_init(&variablenode_ns_2_i_6018_variant_DataContents[0], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6018_variant_DataContents[0].name = UA_STRING("LCEId");
variablenode_ns_2_i_6018_variant_DataContents[0].dataType = UA_NODEID_NUMERIC(ns[0], 9);
variablenode_ns_2_i_6018_variant_DataContents[0].valueRank = (UA_Int32) -1;
UA_Variant_setArray(&attr.value, &variablenode_ns_2_i_6018_variant_DataContents, (UA_Int32) 1, &UA_TYPES[UA_TYPES_ARGUMENT]);
attr.value.arrayDimensionsSize = attr.arrayDimensionsSize;
attr.value.arrayDimensions = attr.arrayDimensions;
attr.displayName = UA_LOCALIZEDTEXT("", "InputArguments");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6018),
UA_NODEID_NUMERIC(ns[2], 7006),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[0], "InputArguments"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);

retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6018), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_30_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6018)
);
}

/* OutputArguments - ns=2;i=6019 */

static UA_StatusCode function_lifeCycleEntry_31_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
attr.valueRank = 1;
attr.arrayDimensionsSize = 1;
UA_UInt32 arrayDimensions[1];
arrayDimensions[0] = 8;
attr.arrayDimensions = &arrayDimensions[0];
attr.dataType = UA_NODEID_NUMERIC(ns[0], 296);
UA_Argument variablenode_ns_2_i_6019_variant_DataContents[8];

UA_init(&variablenode_ns_2_i_6019_variant_DataContents[0], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6019_variant_DataContents[0].name = UA_STRING("Status");
variablenode_ns_2_i_6019_variant_DataContents[0].dataType = UA_NODEID_NUMERIC(ns[2], 3003);
variablenode_ns_2_i_6019_variant_DataContents[0].valueRank = (UA_Int32) -1;

UA_init(&variablenode_ns_2_i_6019_variant_DataContents[1], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6019_variant_DataContents[1].name = UA_STRING("CreatingInstanceIdType");
variablenode_ns_2_i_6019_variant_DataContents[1].dataType = UA_NODEID_NUMERIC(ns[0], 7);
variablenode_ns_2_i_6019_variant_DataContents[1].valueRank = (UA_Int32) -1;

UA_init(&variablenode_ns_2_i_6019_variant_DataContents[2], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6019_variant_DataContents[2].name = UA_STRING("CreatingInstanceIdSpec");
variablenode_ns_2_i_6019_variant_DataContents[2].dataType = UA_NODEID_NUMERIC(ns[0], 12);
variablenode_ns_2_i_6019_variant_DataContents[2].valueRank = (UA_Int32) -1;

UA_init(&variablenode_ns_2_i_6019_variant_DataContents[3], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6019_variant_DataContents[3].name = UA_STRING("WritingInstanceIdType");
variablenode_ns_2_i_6019_variant_DataContents[3].dataType = UA_NODEID_NUMERIC(ns[0], 7);
variablenode_ns_2_i_6019_variant_DataContents[3].valueRank = (UA_Int32) -1;

UA_init(&variablenode_ns_2_i_6019_variant_DataContents[4], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6019_variant_DataContents[4].name = UA_STRING("WritingInstanceIdSpec");
variablenode_ns_2_i_6019_variant_DataContents[4].dataType = UA_NODEID_NUMERIC(ns[0], 12);
variablenode_ns_2_i_6019_variant_DataContents[4].valueRank = (UA_Int32) -1;

UA_init(&variablenode_ns_2_i_6019_variant_DataContents[5], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6019_variant_DataContents[5].name = UA_STRING("Data");
variablenode_ns_2_i_6019_variant_DataContents[5].dataType = UA_NODEID_NUMERIC(ns[0], 23);
variablenode_ns_2_i_6019_variant_DataContents[5].valueRank = (UA_Int32) -1;

UA_init(&variablenode_ns_2_i_6019_variant_DataContents[6], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6019_variant_DataContents[6].name = UA_STRING("Subject");
variablenode_ns_2_i_6019_variant_DataContents[6].dataType = UA_NODEID_NUMERIC(ns[0], 12);
variablenode_ns_2_i_6019_variant_DataContents[6].valueRank = (UA_Int32) -1;

UA_init(&variablenode_ns_2_i_6019_variant_DataContents[7], &UA_TYPES[UA_TYPES_ARGUMENT]);
variablenode_ns_2_i_6019_variant_DataContents[7].name = UA_STRING("EventClass");
variablenode_ns_2_i_6019_variant_DataContents[7].dataType = UA_NODEID_NUMERIC(ns[0], 12);
variablenode_ns_2_i_6019_variant_DataContents[7].valueRank = (UA_Int32) -1;
UA_Variant_setArray(&attr.value, &variablenode_ns_2_i_6019_variant_DataContents, (UA_Int32) 8, &UA_TYPES[UA_TYPES_ARGUMENT]);
attr.value.arrayDimensionsSize = attr.arrayDimensionsSize;
attr.value.arrayDimensions = attr.arrayDimensions;
attr.displayName = UA_LOCALIZEDTEXT("", "OutputArguments");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6019),
UA_NODEID_NUMERIC(ns[2], 7006),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[0], "OutputArguments"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);








retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6019), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_31_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6019)
);
}

/* Archiv - ns=2;i=5003 */

static UA_StatusCode function_lifeCycleEntry_32_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_ObjectAttributes attr = UA_ObjectAttributes_default;
attr.displayName = UA_LOCALIZEDTEXT("", "Archiv");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_OBJECT,
UA_NODEID_NUMERIC(ns[2], 5003),
UA_NODEID_NUMERIC(ns[2], 1002),
UA_NODEID_NUMERIC(ns[0], 47),
UA_QUALIFIEDNAME(ns[2], "Archiv"),
UA_NODEID_NUMERIC(ns[0], 61),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_OBJECTATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 5003), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 11510), true);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_32_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 5003)
);
}

/* <LifeCycleEntries> - ns=2;i=6023 */

static UA_StatusCode function_lifeCycleEntry_33_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 3;
attr.accessLevel = 3;
/* Value rank inherited */
attr.valueRank = -1;
attr.dataType = UA_NODEID_NUMERIC(ns[2], 3002);
attr.displayName = UA_LOCALIZEDTEXT("", "<LifeCycleEntries>");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6023),
UA_NODEID_NUMERIC(ns[2], 5003),
UA_NODEID_NUMERIC(ns[0], 47),
UA_QUALIFIEDNAME(ns[2], "<LifeCycleEntries>"),
UA_NODEID_NUMERIC(ns[0], 63),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6023), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 11510), true);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_33_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6023)
);
}

/* TypeDictionary - ns=2;i=6003 */
static const UA_Byte variablenode_ns_2_i_6003_variant_DataContents_byteArray[1449] = {60, 120, 115, 58, 115, 99, 104, 101, 109, 97, 32, 101, 108, 101, 109, 101, 110, 116, 70, 111, 114, 109, 68, 101, 102, 97, 117, 108, 116, 61, 34, 113, 117, 97, 108, 105, 102, 105, 101, 100, 34, 32, 116, 97, 114, 103, 101, 116, 78, 97, 109, 101, 115, 112, 97, 99, 101, 61, 34, 104, 116, 116, 112, 58, 47, 47, 121, 111, 117, 114, 111, 114, 103, 97, 110, 105, 115, 97, 116, 105, 111, 110, 46, 111, 114, 103, 47, 108, 99, 101, 47, 84, 121, 112, 101, 115, 46, 120, 115, 100, 34, 32, 120, 109, 108, 110, 115, 58, 116, 110, 115, 61, 34, 104, 116, 116, 112, 58, 47, 47, 121, 111, 117, 114, 111, 114, 103, 97, 110, 105, 115, 97, 116, 105, 111, 110, 46, 111, 114, 103, 47, 108, 99, 101, 47, 84, 121, 112, 101, 115, 46, 120, 115, 100, 34, 32, 120, 109, 108, 110, 115, 58, 110, 115, 49, 61, 34, 104, 116, 116, 112, 58, 47, 47, 121, 111, 117, 114, 111, 114, 103, 97, 110, 105, 115, 97, 116, 105, 111, 110, 46, 111, 114, 103, 47, 105, 100, 101, 110, 116, 105, 102, 105, 99, 97, 116, 105, 111, 110, 47, 84, 121, 112, 101, 115, 46, 120, 115, 100, 34, 32, 120, 109, 108, 110, 115, 58, 117, 97, 61, 34, 104, 116, 116, 112, 58, 47, 47, 111, 112, 99, 102, 111, 117, 110, 100, 97, 116, 105, 111, 110, 46, 111, 114, 103, 47, 85, 65, 47, 50, 48, 48, 56, 47, 48, 50, 47, 84, 121, 112, 101, 115, 46, 120, 115, 100, 34, 32, 120, 109, 108, 110, 115, 58, 120, 115, 61, 34, 104, 116, 116, 112, 58, 47, 47, 119, 119, 119, 46, 119, 51, 46, 111, 114, 103, 47, 50, 48, 48, 49, 47, 88, 77, 76, 83, 99, 104, 101, 109, 97, 34, 62, 10, 32, 60, 120, 115, 58, 105, 109, 112, 111, 114, 116, 32, 110, 97, 109, 101, 115, 112, 97, 99, 101, 61, 34, 104, 116, 116, 112, 58, 47, 47, 111, 112, 99, 102, 111, 117, 110, 100, 97, 116, 105, 111, 110, 46, 111, 114, 103, 47, 85, 65, 47, 50, 48, 48, 56, 47, 48, 50, 47, 84, 121, 112, 101, 115, 46, 120, 115, 100, 34, 47, 62, 10, 32, 60, 120, 115, 58, 105, 109, 112, 111, 114, 116, 32, 110, 97, 109, 101, 115, 112, 97, 99, 101, 61, 34, 104, 116, 116, 112, 58, 47, 47, 121, 111, 117, 114, 111, 114, 103, 97, 110, 105, 115, 97, 116, 105, 111, 110, 46, 111, 114, 103, 47, 105, 100, 101, 110, 116, 105, 102, 105, 99, 97, 116, 105, 111, 110, 47, 84, 121, 112, 101, 115, 46, 120, 115, 100, 34, 47, 62, 10, 32, 60, 120, 115, 58, 99, 111, 109, 112, 108, 101, 120, 84, 121, 112, 101, 32, 110, 97, 109, 101, 61, 34, 76, 105, 102, 101, 67, 121, 99, 108, 101, 69, 110, 116, 114, 121, 34, 62, 10, 32, 32, 60, 120, 115, 58, 115, 101, 113, 117, 101, 110, 99, 101, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 108, 101, 109, 101, 110, 116, 32, 109, 105, 110, 79, 99, 99, 117, 114, 115, 61, 34, 48, 34, 32, 109, 97, 120, 79, 99, 99, 117, 114, 115, 61, 34, 49, 34, 32, 116, 121, 112, 101, 61, 34, 110, 115, 49, 58, 73, 100, 101, 110, 116, 105, 102, 105, 99, 97, 116, 105, 111, 110, 34, 32, 110, 97, 109, 101, 61, 34, 67, 114, 101, 97, 116, 105, 110, 103, 73, 110, 115, 116, 97, 110, 99, 101, 34, 47, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 108, 101, 109, 101, 110, 116, 32, 109, 105, 110, 79, 99, 99, 117, 114, 115, 61, 34, 48, 34, 32, 109, 97, 120, 79, 99, 99, 117, 114, 115, 61, 34, 49, 34, 32, 116, 121, 112, 101, 61, 34, 110, 115, 49, 58, 73, 100, 101, 110, 116, 105, 102, 105, 99, 97, 116, 105, 111, 110, 34, 32, 110, 97, 109, 101, 61, 34, 87, 114, 105, 116, 105, 110, 103, 73, 110, 115, 116, 97, 110, 99, 101, 34, 47, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 108, 101, 109, 101, 110, 116, 32, 109, 105, 110, 79, 99, 99, 117, 114, 115, 61, 34, 48, 34, 32, 109, 97, 120, 79, 99, 99, 117, 114, 115, 61, 34, 49, 34, 32, 116, 121, 112, 101, 61, 34, 117, 97, 58, 68, 97, 116, 97, 86, 97, 108, 117, 101, 34, 32, 110, 97, 109, 101, 61, 34, 68, 97, 116, 97, 34, 47, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 108, 101, 109, 101, 110, 116, 32, 109, 105, 110, 79, 99, 99, 117, 114, 115, 61, 34, 48, 34, 32, 109, 97, 120, 79, 99, 99, 117, 114, 115, 61, 34, 49, 34, 32, 116, 121, 112, 101, 61, 34, 120, 115, 58, 115, 116, 114, 105, 110, 103, 34, 32, 110, 97, 109, 101, 61, 34, 83, 117, 98, 106, 101, 99, 116, 34, 47, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 108, 101, 109, 101, 110, 116, 32, 109, 105, 110, 79, 99, 99, 117, 114, 115, 61, 34, 48, 34, 32, 109, 97, 120, 79, 99, 99, 117, 114, 115, 61, 34, 49, 34, 32, 116, 121, 112, 101, 61, 34, 120, 115, 58, 115, 116, 114, 105, 110, 103, 34, 32, 110, 97, 109, 101, 61, 34, 69, 118, 101, 110, 116, 67, 108, 97, 115, 115, 34, 47, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 108, 101, 109, 101, 110, 116, 32, 109, 105, 110, 79, 99, 99, 117, 114, 115, 61, 34, 48, 34, 32, 109, 97, 120, 79, 99, 99, 117, 114, 115, 61, 34, 49, 34, 32, 116, 121, 112, 101, 61, 34, 120, 115, 58, 117, 110, 115, 105, 103, 110, 101, 100, 76, 111, 110, 103, 34, 32, 110, 97, 109, 101, 61, 34, 73, 100, 34, 47, 62, 10, 32, 32, 60, 47, 120, 115, 58, 115, 101, 113, 117, 101, 110, 99, 101, 62, 10, 32, 60, 47, 120, 115, 58, 99, 111, 109, 112, 108, 101, 120, 84, 121, 112, 101, 62, 10, 32, 60, 120, 115, 58, 101, 108, 101, 109, 101, 110, 116, 32, 116, 121, 112, 101, 61, 34, 116, 110, 115, 58, 76, 105, 102, 101, 67, 121, 99, 108, 101, 69, 110, 116, 114, 121, 34, 32, 110, 97, 109, 101, 61, 34, 76, 105, 102, 101, 67, 121, 99, 108, 101, 69, 110, 116, 114, 121, 34, 47, 62, 10, 32, 60, 120, 115, 58, 99, 111, 109, 112, 108, 101, 120, 84, 121, 112, 101, 32, 110, 97, 109, 101, 61, 34, 76, 105, 115, 116, 79, 102, 76, 105, 102, 101, 67, 121, 99, 108, 101, 69, 110, 116, 114, 121, 34, 62, 10, 32, 32, 60, 120, 115, 58, 115, 101, 113, 117, 101, 110, 99, 101, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 108, 101, 109, 101, 110, 116, 32, 109, 105, 110, 79, 99, 99, 117, 114, 115, 61, 34, 48, 34, 32, 109, 97, 120, 79, 99, 99, 117, 114, 115, 61, 34, 117, 110, 98, 111, 117, 110, 100, 101, 100, 34, 32, 116, 121, 112, 101, 61, 34, 116, 110, 115, 58, 76, 105, 102, 101, 67, 121, 99, 108, 101, 69, 110, 116, 114, 121, 34, 32, 110, 97, 109, 101, 61, 34, 76, 105, 102, 101, 67, 121, 99, 108, 101, 69, 110, 116, 114, 121, 34, 32, 110, 105, 108, 108, 97, 98, 108, 101, 61, 34, 116, 114, 117, 101, 34, 47, 62, 10, 32, 32, 60, 47, 120, 115, 58, 115, 101, 113, 117, 101, 110, 99, 101, 62, 10, 32, 60, 47, 120, 115, 58, 99, 111, 109, 112, 108, 101, 120, 84, 121, 112, 101, 62, 10, 32, 60, 120, 115, 58, 101, 108, 101, 109, 101, 110, 116, 32, 116, 121, 112, 101, 61, 34, 116, 110, 115, 58, 76, 105, 115, 116, 79, 102, 76, 105, 102, 101, 67, 121, 99, 108, 101, 69, 110, 116, 114, 121, 34, 32, 110, 97, 109, 101, 61, 34, 76, 105, 115, 116, 79, 102, 76, 105, 102, 101, 67, 121, 99, 108, 101, 69, 110, 116, 114, 121, 34, 32, 110, 105, 108, 108, 97, 98, 108, 101, 61, 34, 116, 114, 117, 101, 34, 47, 62, 10, 60, 47, 120, 115, 58, 115, 99, 104, 101, 109, 97, 62, 10};



static UA_StatusCode function_lifeCycleEntry_34_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
/* Value rank inherited */
attr.valueRank = -1;
attr.dataType = UA_NODEID_NUMERIC(ns[0], 15);
UA_ByteString *variablenode_ns_2_i_6003_variant_DataContents =  UA_ByteString_new();
if (!variablenode_ns_2_i_6003_variant_DataContents) return UA_STATUSCODE_BADOUTOFMEMORY;
UA_ByteString_init(variablenode_ns_2_i_6003_variant_DataContents);
variablenode_ns_2_i_6003_variant_DataContents->length = 1449;
variablenode_ns_2_i_6003_variant_DataContents->data = (UA_Byte *)(void*)(uintptr_t)variablenode_ns_2_i_6003_variant_DataContents_byteArray;
UA_Variant_setScalar(&attr.value, variablenode_ns_2_i_6003_variant_DataContents, &UA_TYPES[UA_TYPES_BYTESTRING]);
attr.displayName = UA_LOCALIZEDTEXT("", "TypeDictionary");
attr.description = UA_LOCALIZEDTEXT("", "Collects the data type descriptions of acplt.org/lifeCycleEntry/");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6003),
UA_NODEID_NUMERIC(ns[0], 92),
UA_NODEID_NUMERIC(ns[0], 47),
UA_QUALIFIEDNAME(ns[2], "TypeDictionary"),
UA_NODEID_NUMERIC(ns[0], 72),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
variablenode_ns_2_i_6003_variant_DataContents->data = NULL;
variablenode_ns_2_i_6003_variant_DataContents->length = 0;
UA_ByteString_delete(variablenode_ns_2_i_6003_variant_DataContents);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_34_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6003)
);
}

/* NamespaceUri - ns=2;i=6004 */

static UA_StatusCode function_lifeCycleEntry_35_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
/* Value rank inherited */
attr.valueRank = -1;
attr.dataType = UA_NODEID_NUMERIC(ns[0], 12);
UA_String *variablenode_ns_2_i_6004_variant_DataContents =  UA_String_new();
if (!variablenode_ns_2_i_6004_variant_DataContents) return UA_STATUSCODE_BADOUTOFMEMORY;
UA_String_init(variablenode_ns_2_i_6004_variant_DataContents);
*variablenode_ns_2_i_6004_variant_DataContents = UA_STRING_ALLOC("acplt.org/lifeCycleEntry/Types.xsd");
UA_Variant_setScalar(&attr.value, variablenode_ns_2_i_6004_variant_DataContents, &UA_TYPES[UA_TYPES_STRING]);
attr.displayName = UA_LOCALIZEDTEXT("", "NamespaceUri");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6004),
UA_NODEID_NUMERIC(ns[2], 6003),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[0], "NamespaceUri"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
UA_String_delete(variablenode_ns_2_i_6004_variant_DataContents);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_35_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6004)
);
}

/* LifeCycleEntry - ns=2;i=6006 */

static UA_StatusCode function_lifeCycleEntry_36_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
/* Value rank inherited */
attr.valueRank = -1;
attr.dataType = UA_NODEID_NUMERIC(ns[0], 12);
UA_String *variablenode_ns_2_i_6006_variant_DataContents =  UA_String_new();
if (!variablenode_ns_2_i_6006_variant_DataContents) return UA_STATUSCODE_BADOUTOFMEMORY;
UA_String_init(variablenode_ns_2_i_6006_variant_DataContents);
*variablenode_ns_2_i_6006_variant_DataContents = UA_STRING_ALLOC("//xs:element[@name='LifeCycleEntry']");
UA_Variant_setScalar(&attr.value, variablenode_ns_2_i_6006_variant_DataContents, &UA_TYPES[UA_TYPES_STRING]);
attr.displayName = UA_LOCALIZEDTEXT("", "LifeCycleEntry");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6006),
UA_NODEID_NUMERIC(ns[2], 6003),
UA_NODEID_NUMERIC(ns[0], 47),
UA_QUALIFIEDNAME(ns[2], "LifeCycleEntry"),
UA_NODEID_NUMERIC(ns[0], 69),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
UA_String_delete(variablenode_ns_2_i_6006_variant_DataContents);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6006), UA_NODEID_NUMERIC(ns[0], 39), UA_EXPANDEDNODEID_NUMERIC(ns[2], 5002), false);
return retVal;
}

static UA_StatusCode function_lifeCycleEntry_36_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6006)
);
}

UA_StatusCode nodeset_lifeCycleEntry(UA_Server *server) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
/* Use namespace ids generated by the server */
UA_UInt16 ns[3];
ns[0] = UA_Server_addNamespace(server, "http://opcfoundation.org/UA/");
ns[1] = UA_Server_addNamespace(server, "acplt.org/identification/");
ns[2] = UA_Server_addNamespace(server, "acplt.org/lifeCycleEntry/");
bool dummy = (
!(retVal = function_lifeCycleEntry_0_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_1_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_2_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_3_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_4_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_5_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_6_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_7_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_8_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_9_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_10_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_11_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_12_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_13_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_14_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_15_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_16_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_17_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_18_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_19_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_20_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_21_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_22_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_23_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_24_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_25_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_26_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_27_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_28_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_29_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_30_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_31_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_32_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_33_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_34_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_35_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_36_begin(server, ns)) &&
!(retVal = function_lifeCycleEntry_36_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_35_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_34_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_33_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_32_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_31_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_30_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_29_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_28_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_27_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_26_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_25_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_24_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_23_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_22_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_21_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_20_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_19_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_18_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_17_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_16_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_15_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_14_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_13_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_12_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_11_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_10_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_9_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_8_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_7_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_6_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_5_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_4_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_3_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_2_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_1_finish(server, ns)) &&
!(retVal = function_lifeCycleEntry_0_finish(server, ns)) 
); (void)(dummy);
return retVal;
}
