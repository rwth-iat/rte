/* WARNING: This is a generated file.
 * Any manual changes will be overwritten. */

#include "nodeset_openaas.h"


/* AASStatusCode - ns=3;i=3004 */

static UA_StatusCode function_openaas_0_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_DataTypeAttributes attr = UA_DataTypeAttributes_default;
attr.displayName = UA_LOCALIZEDTEXT("", "AASStatusCode");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_DATATYPE,
UA_NODEID_NUMERIC(ns[3], 3004),
UA_NODEID_NUMERIC(ns[0], 19),
UA_NODEID_NUMERIC(ns[0], 45),
UA_QUALIFIEDNAME(ns[3], "AASStatusCode"),
 UA_NODEID_NULL,
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_DATATYPEATTRIBUTES],NULL, NULL);
return retVal;
}

static UA_StatusCode function_openaas_0_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[3], 3004)
);
}

/* TypeDictionary - ns=3;i=6002 */
static const UA_Byte variablenode_ns_3_i_6002_variant_DataContents_byteArray[380] = {60, 111, 112, 99, 58, 84, 121, 112, 101, 68, 105, 99, 116, 105, 111, 110, 97, 114, 121, 32, 120, 109, 108, 110, 115, 58, 120, 115, 105, 61, 34, 104, 116, 116, 112, 58, 47, 47, 119, 119, 119, 46, 119, 51, 46, 111, 114, 103, 47, 50, 48, 48, 49, 47, 88, 77, 76, 83, 99, 104, 101, 109, 97, 45, 105, 110, 115, 116, 97, 110, 99, 101, 34, 32, 120, 109, 108, 110, 115, 58, 116, 110, 115, 61, 34, 104, 116, 116, 112, 58, 47, 47, 121, 111, 117, 114, 111, 114, 103, 97, 110, 105, 115, 97, 116, 105, 111, 110, 46, 111, 114, 103, 47, 111, 112, 101, 110, 97, 97, 115, 47, 34, 32, 68, 101, 102, 97, 117, 108, 116, 66, 121, 116, 101, 79, 114, 100, 101, 114, 61, 34, 76, 105, 116, 116, 108, 101, 69, 110, 100, 105, 97, 110, 34, 32, 120, 109, 108, 110, 115, 58, 111, 112, 99, 61, 34, 104, 116, 116, 112, 58, 47, 47, 111, 112, 99, 102, 111, 117, 110, 100, 97, 116, 105, 111, 110, 46, 111, 114, 103, 47, 66, 105, 110, 97, 114, 121, 83, 99, 104, 101, 109, 97, 47, 34, 32, 120, 109, 108, 110, 115, 58, 117, 97, 61, 34, 104, 116, 116, 112, 58, 47, 47, 111, 112, 99, 102, 111, 117, 110, 100, 97, 116, 105, 111, 110, 46, 111, 114, 103, 47, 85, 65, 47, 34, 32, 84, 97, 114, 103, 101, 116, 78, 97, 109, 101, 115, 112, 97, 99, 101, 61, 34, 104, 116, 116, 112, 58, 47, 47, 121, 111, 117, 114, 111, 114, 103, 97, 110, 105, 115, 97, 116, 105, 111, 110, 46, 111, 114, 103, 47, 111, 112, 101, 110, 97, 97, 115, 47, 34, 62, 10, 32, 60, 111, 112, 99, 58, 73, 109, 112, 111, 114, 116, 32, 78, 97, 109, 101, 115, 112, 97, 99, 101, 61, 34, 104, 116, 116, 112, 58, 47, 47, 111, 112, 99, 102, 111, 117, 110, 100, 97, 116, 105, 111, 110, 46, 111, 114, 103, 47, 85, 65, 47, 34, 47, 62, 10, 60, 47, 111, 112, 99, 58, 84, 121, 112, 101, 68, 105, 99, 116, 105, 111, 110, 97, 114, 121, 62, 10};



static UA_StatusCode function_openaas_1_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
/* Value rank inherited */
attr.valueRank = -1;
attr.dataType = UA_NODEID_NUMERIC(ns[0], 15);
UA_ByteString *variablenode_ns_3_i_6002_variant_DataContents =  UA_ByteString_new();
if (!variablenode_ns_3_i_6002_variant_DataContents) return UA_STATUSCODE_BADOUTOFMEMORY;
UA_ByteString_init(variablenode_ns_3_i_6002_variant_DataContents);
variablenode_ns_3_i_6002_variant_DataContents->length = 380;
variablenode_ns_3_i_6002_variant_DataContents->data = (UA_Byte *)(void*)(uintptr_t)variablenode_ns_3_i_6002_variant_DataContents_byteArray;
UA_Variant_setScalar(&attr.value, variablenode_ns_3_i_6002_variant_DataContents, &UA_TYPES[UA_TYPES_BYTESTRING]);
attr.displayName = UA_LOCALIZEDTEXT("", "TypeDictionary");
attr.description = UA_LOCALIZEDTEXT("", "Collects the data type descriptions of acplt.org/openaas/");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[3], 6002),
UA_NODEID_NUMERIC(ns[0], 93),
UA_NODEID_NUMERIC(ns[0], 47),
UA_QUALIFIEDNAME(ns[3], "TypeDictionary"),
UA_NODEID_NUMERIC(ns[0], 72),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
variablenode_ns_3_i_6002_variant_DataContents->data = NULL;
variablenode_ns_3_i_6002_variant_DataContents->length = 0;
UA_ByteString_delete(variablenode_ns_3_i_6002_variant_DataContents);
return retVal;
}

static UA_StatusCode function_openaas_1_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[3], 6002)
);
}

/* NamespaceUri - ns=3;i=6003 */

static UA_StatusCode function_openaas_2_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
/* Value rank inherited */
attr.valueRank = -1;
attr.dataType = UA_NODEID_NUMERIC(ns[0], 12);
UA_String *variablenode_ns_3_i_6003_variant_DataContents =  UA_String_new();
if (!variablenode_ns_3_i_6003_variant_DataContents) return UA_STATUSCODE_BADOUTOFMEMORY;
UA_String_init(variablenode_ns_3_i_6003_variant_DataContents);
*variablenode_ns_3_i_6003_variant_DataContents = UA_STRING_ALLOC("acplt.org/openaas/");
UA_Variant_setScalar(&attr.value, variablenode_ns_3_i_6003_variant_DataContents, &UA_TYPES[UA_TYPES_STRING]);
attr.displayName = UA_LOCALIZEDTEXT("", "NamespaceUri");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[3], 6003),
UA_NODEID_NUMERIC(ns[3], 6002),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[0], "NamespaceUri"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
UA_String_delete(variablenode_ns_3_i_6003_variant_DataContents);
return retVal;
}

static UA_StatusCode function_openaas_2_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[3], 6003)
);
}

/* TypeDictionary - ns=3;i=6004 */
static const UA_Byte variablenode_ns_3_i_6004_variant_DataContents_byteArray[351] = {60, 120, 115, 58, 115, 99, 104, 101, 109, 97, 32, 101, 108, 101, 109, 101, 110, 116, 70, 111, 114, 109, 68, 101, 102, 97, 117, 108, 116, 61, 34, 113, 117, 97, 108, 105, 102, 105, 101, 100, 34, 32, 116, 97, 114, 103, 101, 116, 78, 97, 109, 101, 115, 112, 97, 99, 101, 61, 34, 104, 116, 116, 112, 58, 47, 47, 121, 111, 117, 114, 111, 114, 103, 97, 110, 105, 115, 97, 116, 105, 111, 110, 46, 111, 114, 103, 47, 111, 112, 101, 110, 97, 97, 115, 47, 84, 121, 112, 101, 115, 46, 120, 115, 100, 34, 32, 120, 109, 108, 110, 115, 58, 116, 110, 115, 61, 34, 104, 116, 116, 112, 58, 47, 47, 121, 111, 117, 114, 111, 114, 103, 97, 110, 105, 115, 97, 116, 105, 111, 110, 46, 111, 114, 103, 47, 111, 112, 101, 110, 97, 97, 115, 47, 84, 121, 112, 101, 115, 46, 120, 115, 100, 34, 32, 120, 109, 108, 110, 115, 58, 117, 97, 61, 34, 104, 116, 116, 112, 58, 47, 47, 111, 112, 99, 102, 111, 117, 110, 100, 97, 116, 105, 111, 110, 46, 111, 114, 103, 47, 85, 65, 47, 50, 48, 48, 56, 47, 48, 50, 47, 84, 121, 112, 101, 115, 46, 120, 115, 100, 34, 32, 120, 109, 108, 110, 115, 58, 120, 115, 61, 34, 104, 116, 116, 112, 58, 47, 47, 119, 119, 119, 46, 119, 51, 46, 111, 114, 103, 47, 50, 48, 48, 49, 47, 88, 77, 76, 83, 99, 104, 101, 109, 97, 34, 62, 10, 32, 60, 120, 115, 58, 105, 109, 112, 111, 114, 116, 32, 110, 97, 109, 101, 115, 112, 97, 99, 101, 61, 34, 104, 116, 116, 112, 58, 47, 47, 111, 112, 99, 102, 111, 117, 110, 100, 97, 116, 105, 111, 110, 46, 111, 114, 103, 47, 85, 65, 47, 50, 48, 48, 56, 47, 48, 50, 47, 84, 121, 112, 101, 115, 46, 120, 115, 100, 34, 47, 62, 10, 60, 47, 120, 115, 58, 115, 99, 104, 101, 109, 97, 62, 10};



static UA_StatusCode function_openaas_3_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
/* Value rank inherited */
attr.valueRank = -1;
attr.dataType = UA_NODEID_NUMERIC(ns[0], 15);
UA_ByteString *variablenode_ns_3_i_6004_variant_DataContents =  UA_ByteString_new();
if (!variablenode_ns_3_i_6004_variant_DataContents) return UA_STATUSCODE_BADOUTOFMEMORY;
UA_ByteString_init(variablenode_ns_3_i_6004_variant_DataContents);
variablenode_ns_3_i_6004_variant_DataContents->length = 351;
variablenode_ns_3_i_6004_variant_DataContents->data = (UA_Byte *)(void*)(uintptr_t)variablenode_ns_3_i_6004_variant_DataContents_byteArray;
UA_Variant_setScalar(&attr.value, variablenode_ns_3_i_6004_variant_DataContents, &UA_TYPES[UA_TYPES_BYTESTRING]);
attr.displayName = UA_LOCALIZEDTEXT("", "TypeDictionary");
attr.description = UA_LOCALIZEDTEXT("", "Collects the data type descriptions of acplt.org/openaas/");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[3], 6004),
UA_NODEID_NUMERIC(ns[0], 92),
UA_NODEID_NUMERIC(ns[0], 47),
UA_QUALIFIEDNAME(ns[3], "TypeDictionary"),
UA_NODEID_NUMERIC(ns[0], 72),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
variablenode_ns_3_i_6004_variant_DataContents->data = NULL;
variablenode_ns_3_i_6004_variant_DataContents->length = 0;
UA_ByteString_delete(variablenode_ns_3_i_6004_variant_DataContents);
return retVal;
}

static UA_StatusCode function_openaas_3_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[3], 6004)
);
}

/* NamespaceUri - ns=3;i=6005 */

static UA_StatusCode function_openaas_4_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
/* Value rank inherited */
attr.valueRank = -1;
attr.dataType = UA_NODEID_NUMERIC(ns[0], 12);
UA_String *variablenode_ns_3_i_6005_variant_DataContents =  UA_String_new();
if (!variablenode_ns_3_i_6005_variant_DataContents) return UA_STATUSCODE_BADOUTOFMEMORY;
UA_String_init(variablenode_ns_3_i_6005_variant_DataContents);
*variablenode_ns_3_i_6005_variant_DataContents = UA_STRING_ALLOC("acplt.org/openaas/Types.xsd");
UA_Variant_setScalar(&attr.value, variablenode_ns_3_i_6005_variant_DataContents, &UA_TYPES[UA_TYPES_STRING]);
attr.displayName = UA_LOCALIZEDTEXT("", "NamespaceUri");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[3], 6005),
UA_NODEID_NUMERIC(ns[3], 6004),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[0], "NamespaceUri"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
UA_String_delete(variablenode_ns_3_i_6005_variant_DataContents);
return retVal;
}

static UA_StatusCode function_openaas_4_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[3], 6005)
);
}

/* SubModelType - ns=3;i=1003 */

static UA_StatusCode function_openaas_5_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_ObjectTypeAttributes attr = UA_ObjectTypeAttributes_default;
attr.displayName = UA_LOCALIZEDTEXT("", "SubModelType");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_OBJECTTYPE,
UA_NODEID_NUMERIC(ns[3], 1003),
UA_NODEID_NUMERIC(ns[0], 58),
UA_NODEID_NUMERIC(ns[0], 45),
UA_QUALIFIEDNAME(ns[3], "SubModelType"),
 UA_NODEID_NULL,
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_OBJECTTYPEATTRIBUTES],NULL, NULL);
return retVal;
}

static UA_StatusCode function_openaas_5_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[3], 1003)
);
}

/* Version - ns=3;i=6011 */

static UA_StatusCode function_openaas_6_begin(UA_Server *server, UA_UInt16* ns) {
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
UA_NODEID_NUMERIC(ns[3], 6011),
UA_NODEID_NUMERIC(ns[3], 1003),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[3], "Version"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[3], 6011), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_openaas_6_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[3], 6011)
);
}

/* Revision - ns=3;i=6010 */

static UA_StatusCode function_openaas_7_begin(UA_Server *server, UA_UInt16* ns) {
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
UA_NODEID_NUMERIC(ns[3], 6010),
UA_NODEID_NUMERIC(ns[3], 1003),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[3], "Revision"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[3], 6010), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_openaas_7_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[3], 6010)
);
}

/* ModelID - ns=3;i=6008 */

static UA_StatusCode function_openaas_8_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 3;
attr.accessLevel = 3;
/* Value rank inherited */
attr.valueRank = -1;
attr.dataType = UA_NODEID_NUMERIC(ns[1], 3002);
attr.displayName = UA_LOCALIZEDTEXT("", "ModelID");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[3], 6008),
UA_NODEID_NUMERIC(ns[3], 1003),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[3], "ModelID"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[3], 6008), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_openaas_8_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[3], 6008)
);
}

/* AssetAdminitrationShellType - ns=3;i=1002 */

static UA_StatusCode function_openaas_9_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_ObjectTypeAttributes attr = UA_ObjectTypeAttributes_default;
attr.displayName = UA_LOCALIZEDTEXT("", "AssetAdminitrationShellType");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_OBJECTTYPE,
UA_NODEID_NUMERIC(ns[3], 1002),
UA_NODEID_NUMERIC(ns[0], 58),
UA_NODEID_NUMERIC(ns[0], 45),
UA_QUALIFIEDNAME(ns[3], "AssetAdminitrationShellType"),
 UA_NODEID_NULL,
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_OBJECTTYPEATTRIBUTES],NULL, NULL);
return retVal;
}

static UA_StatusCode function_openaas_9_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[3], 1002)
);
}

/* Views - ns=3;i=5004 */

static UA_StatusCode function_openaas_10_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_ObjectAttributes attr = UA_ObjectAttributes_default;
attr.displayName = UA_LOCALIZEDTEXT("", "Views");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_OBJECT,
UA_NODEID_NUMERIC(ns[3], 5004),
UA_NODEID_NUMERIC(ns[3], 1002),
UA_NODEID_NUMERIC(ns[0], 47),
UA_QUALIFIEDNAME(ns[3], "Views"),
UA_NODEID_NUMERIC(ns[0], 61),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_OBJECTATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[3], 5004), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 80), true);
return retVal;
}

static UA_StatusCode function_openaas_10_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[3], 5004)
);
}

/* Body - ns=3;i=5001 */

static UA_StatusCode function_openaas_11_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_ObjectAttributes attr = UA_ObjectAttributes_default;
attr.displayName = UA_LOCALIZEDTEXT("", "Body");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_OBJECT,
UA_NODEID_NUMERIC(ns[3], 5001),
UA_NODEID_NUMERIC(ns[3], 1002),
UA_NODEID_NUMERIC(ns[0], 47),
UA_QUALIFIEDNAME(ns[3], "Body"),
UA_NODEID_NUMERIC(ns[0], 61),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_OBJECTATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[3], 5001), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_openaas_11_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[3], 5001)
);
}

/* Header - ns=3;i=5002 */

static UA_StatusCode function_openaas_12_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_ObjectAttributes attr = UA_ObjectAttributes_default;
attr.displayName = UA_LOCALIZEDTEXT("", "Header");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_OBJECT,
UA_NODEID_NUMERIC(ns[3], 5002),
UA_NODEID_NUMERIC(ns[3], 1002),
UA_NODEID_NUMERIC(ns[0], 47),
UA_QUALIFIEDNAME(ns[3], "Header"),
UA_NODEID_NUMERIC(ns[0], 61),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_OBJECTATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[3], 5002), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_openaas_12_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[3], 5002)
);
}

/* Config - ns=3;i=5003 */

static UA_StatusCode function_openaas_13_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_ObjectAttributes attr = UA_ObjectAttributes_default;
attr.displayName = UA_LOCALIZEDTEXT("", "Config");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_OBJECT,
UA_NODEID_NUMERIC(ns[3], 5003),
UA_NODEID_NUMERIC(ns[3], 5002),
UA_NODEID_NUMERIC(ns[0], 47),
UA_QUALIFIEDNAME(ns[3], "Config"),
UA_NODEID_NUMERIC(ns[2], 1002),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_OBJECTATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[3], 5003), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_openaas_13_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[3], 5003)
);
}

/* PropertyValueStatements - ns=3;i=6001 */

static UA_StatusCode function_openaas_14_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 3;
attr.accessLevel = 3;
/* Value rank inherited */
attr.valueRank = -1;
/* DataType inherited */
attr.dataType = UA_NODEID_NUMERIC(ns[0], 24);
attr.displayName = UA_LOCALIZEDTEXT("", "PropertyValueStatements");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[3], 6001),
UA_NODEID_NUMERIC(ns[3], 5003),
UA_NODEID_NUMERIC(ns[0], 47),
UA_QUALIFIEDNAME(ns[2], "PropertyValueStatements"),
UA_NODEID_NUMERIC(ns[2], 2002),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[3], 6001), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 11510), true);
return retVal;
}

static UA_StatusCode function_openaas_14_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[3], 6001)
);
}

/* Value - ns=3;i=6006 */

static UA_StatusCode function_openaas_15_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 3;
attr.accessLevel = 3;
/* Value rank inherited */
attr.valueRank = -1;
/* DataType inherited */
attr.dataType = UA_NODEID_NUMERIC(ns[0], 24);
attr.displayName = UA_LOCALIZEDTEXT("", "Value");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[3], 6006),
UA_NODEID_NUMERIC(ns[3], 6001),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[2], "Value"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[3], 6006), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_openaas_15_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[3], 6006)
);
}

UA_StatusCode nodeset_openaas(UA_Server *server) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
/* Use namespace ids generated by the server */
UA_UInt16 ns[4];
ns[0] = UA_Server_addNamespace(server, "http://opcfoundation.org/UA/");
ns[1] = UA_Server_addNamespace(server, "acplt.org/identification/");
ns[2] = UA_Server_addNamespace(server, "acplt.org/propertyValueStatement/");
ns[3] = UA_Server_addNamespace(server, "acplt.org/openaas/");
bool dummy = (
!(retVal = function_openaas_0_begin(server, ns)) &&
!(retVal = function_openaas_1_begin(server, ns)) &&
!(retVal = function_openaas_2_begin(server, ns)) &&
!(retVal = function_openaas_3_begin(server, ns)) &&
!(retVal = function_openaas_4_begin(server, ns)) &&
!(retVal = function_openaas_5_begin(server, ns)) &&
!(retVal = function_openaas_6_begin(server, ns)) &&
!(retVal = function_openaas_7_begin(server, ns)) &&
!(retVal = function_openaas_8_begin(server, ns)) &&
!(retVal = function_openaas_9_begin(server, ns)) &&
!(retVal = function_openaas_10_begin(server, ns)) &&
!(retVal = function_openaas_11_begin(server, ns)) &&
!(retVal = function_openaas_12_begin(server, ns)) &&
!(retVal = function_openaas_13_begin(server, ns)) &&
!(retVal = function_openaas_14_begin(server, ns)) &&
!(retVal = function_openaas_15_begin(server, ns)) &&
!(retVal = function_openaas_15_finish(server, ns)) &&
!(retVal = function_openaas_14_finish(server, ns)) &&
!(retVal = function_openaas_13_finish(server, ns)) &&
!(retVal = function_openaas_12_finish(server, ns)) &&
!(retVal = function_openaas_11_finish(server, ns)) &&
!(retVal = function_openaas_10_finish(server, ns)) &&
!(retVal = function_openaas_9_finish(server, ns)) &&
!(retVal = function_openaas_8_finish(server, ns)) &&
!(retVal = function_openaas_7_finish(server, ns)) &&
!(retVal = function_openaas_6_finish(server, ns)) &&
!(retVal = function_openaas_5_finish(server, ns)) &&
!(retVal = function_openaas_4_finish(server, ns)) &&
!(retVal = function_openaas_3_finish(server, ns)) &&
!(retVal = function_openaas_2_finish(server, ns)) &&
!(retVal = function_openaas_1_finish(server, ns)) &&
!(retVal = function_openaas_0_finish(server, ns)) 
); (void)(dummy);
return retVal;
}
