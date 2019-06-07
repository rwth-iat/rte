/* WARNING: This is a generated file.
 * Any manual changes will be overwritten. */

#include "nodeset_propertyValueStatement.h"


/* ExpressionLogicEnum - ns=2;i=3002 */

static UA_StatusCode function_propertyValueStatement_0_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_DataTypeAttributes attr = UA_DataTypeAttributes_default;
attr.displayName = UA_LOCALIZEDTEXT("", "ExpressionLogicEnum");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_DATATYPE,
UA_NODEID_NUMERIC(ns[2], 3002),
UA_NODEID_NUMERIC(ns[0], 29),
UA_NODEID_NUMERIC(ns[0], 45),
UA_QUALIFIEDNAME(ns[2], "ExpressionLogicEnum"),
 UA_NODEID_NULL,
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_DATATYPEATTRIBUTES],NULL, NULL);
return retVal;
}

static UA_StatusCode function_propertyValueStatement_0_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 3002)
);
}

/* EnumStrings - ns=2;i=6004 */

static UA_StatusCode function_propertyValueStatement_1_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
attr.valueRank = 1;
attr.arrayDimensionsSize = 1;
UA_UInt32 arrayDimensions[1];
arrayDimensions[0] = 6;
attr.arrayDimensions = &arrayDimensions[0];
attr.dataType = UA_NODEID_NUMERIC(ns[0], 21);
UA_LocalizedText variablenode_ns_2_i_6004_variant_DataContents[6];
variablenode_ns_2_i_6004_variant_DataContents[0] = UA_LOCALIZEDTEXT("", "GREATER_THAN");
variablenode_ns_2_i_6004_variant_DataContents[1] = UA_LOCALIZEDTEXT("", "GREATER_EQUAL");
variablenode_ns_2_i_6004_variant_DataContents[2] = UA_LOCALIZEDTEXT("", "EQUAL");
variablenode_ns_2_i_6004_variant_DataContents[3] = UA_LOCALIZEDTEXT("", "NOT_EQUAL");
variablenode_ns_2_i_6004_variant_DataContents[4] = UA_LOCALIZEDTEXT("", "LESS_EQUAL");
variablenode_ns_2_i_6004_variant_DataContents[5] = UA_LOCALIZEDTEXT("", "LESS_THAN");
UA_Variant_setArray(&attr.value, &variablenode_ns_2_i_6004_variant_DataContents, (UA_Int32) 6, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
attr.value.arrayDimensionsSize = attr.arrayDimensionsSize;
attr.value.arrayDimensions = attr.arrayDimensions;
attr.displayName = UA_LOCALIZEDTEXT("", "EnumStrings");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6004),
UA_NODEID_NUMERIC(ns[2], 3002),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[0], "EnumStrings"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6004), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_propertyValueStatement_1_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6004)
);
}

/* ExpressionSemanticEnum - ns=2;i=3003 */

static UA_StatusCode function_propertyValueStatement_2_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_DataTypeAttributes attr = UA_DataTypeAttributes_default;
attr.displayName = UA_LOCALIZEDTEXT("", "ExpressionSemanticEnum");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_DATATYPE,
UA_NODEID_NUMERIC(ns[2], 3003),
UA_NODEID_NUMERIC(ns[0], 29),
UA_NODEID_NUMERIC(ns[0], 45),
UA_QUALIFIEDNAME(ns[2], "ExpressionSemanticEnum"),
 UA_NODEID_NULL,
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_DATATYPEATTRIBUTES],NULL, NULL);
return retVal;
}

static UA_StatusCode function_propertyValueStatement_2_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 3003)
);
}

/* EnumStrings - ns=2;i=6009 */

static UA_StatusCode function_propertyValueStatement_3_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
attr.valueRank = 1;
attr.arrayDimensionsSize = 1;
UA_UInt32 arrayDimensions[1];
arrayDimensions[0] = 4;
attr.arrayDimensions = &arrayDimensions[0];
attr.dataType = UA_NODEID_NUMERIC(ns[0], 21);
UA_LocalizedText variablenode_ns_2_i_6009_variant_DataContents[4];
variablenode_ns_2_i_6009_variant_DataContents[0] = UA_LOCALIZEDTEXT("", "ASSURANCE");
variablenode_ns_2_i_6009_variant_DataContents[1] = UA_LOCALIZEDTEXT("", "SETTING");
variablenode_ns_2_i_6009_variant_DataContents[2] = UA_LOCALIZEDTEXT("", "MEASUREMENT");
variablenode_ns_2_i_6009_variant_DataContents[3] = UA_LOCALIZEDTEXT("", "REQUIREMENT");
UA_Variant_setArray(&attr.value, &variablenode_ns_2_i_6009_variant_DataContents, (UA_Int32) 4, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
attr.value.arrayDimensionsSize = attr.arrayDimensionsSize;
attr.value.arrayDimensions = attr.arrayDimensions;
attr.displayName = UA_LOCALIZEDTEXT("", "EnumStrings");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6009),
UA_NODEID_NUMERIC(ns[2], 3003),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[0], "EnumStrings"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6009), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_propertyValueStatement_3_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6009)
);
}

/* ViewEnum - ns=2;i=3004 */

static UA_StatusCode function_propertyValueStatement_4_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_DataTypeAttributes attr = UA_DataTypeAttributes_default;
attr.displayName = UA_LOCALIZEDTEXT("", "ViewEnum");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_DATATYPE,
UA_NODEID_NUMERIC(ns[2], 3004),
UA_NODEID_NUMERIC(ns[0], 29),
UA_NODEID_NUMERIC(ns[0], 45),
UA_QUALIFIEDNAME(ns[2], "ViewEnum"),
 UA_NODEID_NULL,
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_DATATYPEATTRIBUTES],NULL, NULL);
return retVal;
}

static UA_StatusCode function_propertyValueStatement_4_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 3004)
);
}

/* EnumStrings - ns=2;i=6015 */

static UA_StatusCode function_propertyValueStatement_5_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
attr.valueRank = 1;
attr.arrayDimensionsSize = 1;
UA_UInt32 arrayDimensions[1];
arrayDimensions[0] = 9;
attr.arrayDimensions = &arrayDimensions[0];
attr.dataType = UA_NODEID_NUMERIC(ns[0], 21);
UA_LocalizedText variablenode_ns_2_i_6015_variant_DataContents[9];
variablenode_ns_2_i_6015_variant_DataContents[0] = UA_LOCALIZEDTEXT("", "BUSINESS");
variablenode_ns_2_i_6015_variant_DataContents[1] = UA_LOCALIZEDTEXT("", "CONSTRUCTION");
variablenode_ns_2_i_6015_variant_DataContents[2] = UA_LOCALIZEDTEXT("", "POWER");
variablenode_ns_2_i_6015_variant_DataContents[3] = UA_LOCALIZEDTEXT("", "FUNCTIONAL");
variablenode_ns_2_i_6015_variant_DataContents[4] = UA_LOCALIZEDTEXT("", "LOCATION");
variablenode_ns_2_i_6015_variant_DataContents[5] = UA_LOCALIZEDTEXT("", "SECURITY");
variablenode_ns_2_i_6015_variant_DataContents[6] = UA_LOCALIZEDTEXT("", "NETWORK");
variablenode_ns_2_i_6015_variant_DataContents[7] = UA_LOCALIZEDTEXT("", "LIFECYCLE");
variablenode_ns_2_i_6015_variant_DataContents[8] = UA_LOCALIZEDTEXT("", "HUMAN");
UA_Variant_setArray(&attr.value, &variablenode_ns_2_i_6015_variant_DataContents, (UA_Int32) 9, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
attr.value.arrayDimensionsSize = attr.arrayDimensionsSize;
attr.value.arrayDimensions = attr.arrayDimensions;
attr.displayName = UA_LOCALIZEDTEXT("", "EnumStrings");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6015),
UA_NODEID_NUMERIC(ns[2], 3004),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[0], "EnumStrings"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6015), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_propertyValueStatement_5_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6015)
);
}

/* VisibilityEnum - ns=2;i=3005 */

static UA_StatusCode function_propertyValueStatement_6_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_DataTypeAttributes attr = UA_DataTypeAttributes_default;
attr.displayName = UA_LOCALIZEDTEXT("", "VisibilityEnum");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_DATATYPE,
UA_NODEID_NUMERIC(ns[2], 3005),
UA_NODEID_NUMERIC(ns[0], 29),
UA_NODEID_NUMERIC(ns[0], 45),
UA_QUALIFIEDNAME(ns[2], "VisibilityEnum"),
 UA_NODEID_NULL,
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_DATATYPEATTRIBUTES],NULL, NULL);
return retVal;
}

static UA_StatusCode function_propertyValueStatement_6_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 3005)
);
}

/* EnumStrings - ns=2;i=6016 */

static UA_StatusCode function_propertyValueStatement_7_begin(UA_Server *server, UA_UInt16* ns) {
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
attr.dataType = UA_NODEID_NUMERIC(ns[0], 21);
UA_LocalizedText variablenode_ns_2_i_6016_variant_DataContents[3];
variablenode_ns_2_i_6016_variant_DataContents[0] = UA_LOCALIZEDTEXT("", "PRIVAT");
variablenode_ns_2_i_6016_variant_DataContents[1] = UA_LOCALIZEDTEXT("", "CONTRACT");
variablenode_ns_2_i_6016_variant_DataContents[2] = UA_LOCALIZEDTEXT("", "PUBLIC");
UA_Variant_setArray(&attr.value, &variablenode_ns_2_i_6016_variant_DataContents, (UA_Int32) 3, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
attr.value.arrayDimensionsSize = attr.arrayDimensionsSize;
attr.value.arrayDimensions = attr.arrayDimensions;
attr.displayName = UA_LOCALIZEDTEXT("", "EnumStrings");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6016),
UA_NODEID_NUMERIC(ns[2], 3005),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[0], "EnumStrings"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6016), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_propertyValueStatement_7_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6016)
);
}

/* TypeDictionary - ns=2;i=6007 */
static const UA_Byte variablenode_ns_2_i_6007_variant_DataContents_byteArray[3219] = {60, 120, 115, 58, 115, 99, 104, 101, 109, 97, 32, 101, 108, 101, 109, 101, 110, 116, 70, 111, 114, 109, 68, 101, 102, 97, 117, 108, 116, 61, 34, 113, 117, 97, 108, 105, 102, 105, 101, 100, 34, 32, 116, 97, 114, 103, 101, 116, 78, 97, 109, 101, 115, 112, 97, 99, 101, 61, 34, 104, 116, 116, 112, 58, 47, 47, 121, 111, 117, 114, 111, 114, 103, 97, 110, 105, 115, 97, 116, 105, 111, 110, 46, 111, 114, 103, 47, 112, 118, 115, 47, 84, 121, 112, 101, 115, 46, 120, 115, 100, 34, 32, 120, 109, 108, 110, 115, 58, 116, 110, 115, 61, 34, 104, 116, 116, 112, 58, 47, 47, 121, 111, 117, 114, 111, 114, 103, 97, 110, 105, 115, 97, 116, 105, 111, 110, 46, 111, 114, 103, 47, 112, 118, 115, 47, 84, 121, 112, 101, 115, 46, 120, 115, 100, 34, 32, 120, 109, 108, 110, 115, 58, 117, 97, 61, 34, 104, 116, 116, 112, 58, 47, 47, 111, 112, 99, 102, 111, 117, 110, 100, 97, 116, 105, 111, 110, 46, 111, 114, 103, 47, 85, 65, 47, 50, 48, 48, 56, 47, 48, 50, 47, 84, 121, 112, 101, 115, 46, 120, 115, 100, 34, 32, 120, 109, 108, 110, 115, 58, 120, 115, 61, 34, 104, 116, 116, 112, 58, 47, 47, 119, 119, 119, 46, 119, 51, 46, 111, 114, 103, 47, 50, 48, 48, 49, 47, 88, 77, 76, 83, 99, 104, 101, 109, 97, 34, 62, 10, 32, 60, 120, 115, 58, 105, 109, 112, 111, 114, 116, 32, 110, 97, 109, 101, 115, 112, 97, 99, 101, 61, 34, 104, 116, 116, 112, 58, 47, 47, 111, 112, 99, 102, 111, 117, 110, 100, 97, 116, 105, 111, 110, 46, 111, 114, 103, 47, 85, 65, 47, 50, 48, 48, 56, 47, 48, 50, 47, 84, 121, 112, 101, 115, 46, 120, 115, 100, 34, 47, 62, 10, 32, 60, 120, 115, 58, 115, 105, 109, 112, 108, 101, 84, 121, 112, 101, 32, 110, 97, 109, 101, 61, 34, 69, 120, 112, 114, 101, 115, 115, 105, 111, 110, 76, 111, 103, 105, 99, 69, 110, 117, 109, 34, 62, 10, 32, 32, 60, 120, 115, 58, 114, 101, 115, 116, 114, 105, 99, 116, 105, 111, 110, 32, 98, 97, 115, 101, 61, 34, 120, 115, 58, 115, 116, 114, 105, 110, 103, 34, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 110, 117, 109, 101, 114, 97, 116, 105, 111, 110, 32, 118, 97, 108, 117, 101, 61, 34, 71, 82, 69, 65, 84, 69, 82, 95, 84, 72, 65, 78, 95, 48, 34, 47, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 110, 117, 109, 101, 114, 97, 116, 105, 111, 110, 32, 118, 97, 108, 117, 101, 61, 34, 71, 82, 69, 65, 84, 69, 82, 95, 69, 81, 85, 65, 76, 95, 49, 34, 47, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 110, 117, 109, 101, 114, 97, 116, 105, 111, 110, 32, 118, 97, 108, 117, 101, 61, 34, 69, 81, 85, 65, 76, 95, 50, 34, 47, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 110, 117, 109, 101, 114, 97, 116, 105, 111, 110, 32, 118, 97, 108, 117, 101, 61, 34, 78, 79, 84, 95, 69, 81, 85, 65, 76, 95, 51, 34, 47, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 110, 117, 109, 101, 114, 97, 116, 105, 111, 110, 32, 118, 97, 108, 117, 101, 61, 34, 76, 69, 83, 83, 95, 69, 81, 85, 65, 76, 95, 52, 34, 47, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 110, 117, 109, 101, 114, 97, 116, 105, 111, 110, 32, 118, 97, 108, 117, 101, 61, 34, 76, 69, 83, 83, 95, 84, 72, 65, 78, 95, 53, 34, 47, 62, 10, 32, 32, 60, 47, 120, 115, 58, 114, 101, 115, 116, 114, 105, 99, 116, 105, 111, 110, 62, 10, 32, 60, 47, 120, 115, 58, 115, 105, 109, 112, 108, 101, 84, 121, 112, 101, 62, 10, 32, 60, 120, 115, 58, 101, 108, 101, 109, 101, 110, 116, 32, 116, 121, 112, 101, 61, 34, 116, 110, 115, 58, 69, 120, 112, 114, 101, 115, 115, 105, 111, 110, 76, 111, 103, 105, 99, 69, 110, 117, 109, 34, 32, 110, 97, 109, 101, 61, 34, 69, 120, 112, 114, 101, 115, 115, 105, 111, 110, 76, 111, 103, 105, 99, 69, 110, 117, 109, 34, 47, 62, 10, 32, 60, 120, 115, 58, 99, 111, 109, 112, 108, 101, 120, 84, 121, 112, 101, 32, 110, 97, 109, 101, 61, 34, 76, 105, 115, 116, 79, 102, 69, 120, 112, 114, 101, 115, 115, 105, 111, 110, 76, 111, 103, 105, 99, 69, 110, 117, 109, 34, 62, 10, 32, 32, 60, 120, 115, 58, 115, 101, 113, 117, 101, 110, 99, 101, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 108, 101, 109, 101, 110, 116, 32, 109, 105, 110, 79, 99, 99, 117, 114, 115, 61, 34, 48, 34, 32, 109, 97, 120, 79, 99, 99, 117, 114, 115, 61, 34, 117, 110, 98, 111, 117, 110, 100, 101, 100, 34, 32, 116, 121, 112, 101, 61, 34, 116, 110, 115, 58, 69, 120, 112, 114, 101, 115, 115, 105, 111, 110, 76, 111, 103, 105, 99, 69, 110, 117, 109, 34, 32, 110, 97, 109, 101, 61, 34, 69, 120, 112, 114, 101, 115, 115, 105, 111, 110, 76, 111, 103, 105, 99, 69, 110, 117, 109, 34, 32, 110, 105, 108, 108, 97, 98, 108, 101, 61, 34, 116, 114, 117, 101, 34, 47, 62, 10, 32, 32, 60, 47, 120, 115, 58, 115, 101, 113, 117, 101, 110, 99, 101, 62, 10, 32, 60, 47, 120, 115, 58, 99, 111, 109, 112, 108, 101, 120, 84, 121, 112, 101, 62, 10, 32, 60, 120, 115, 58, 101, 108, 101, 109, 101, 110, 116, 32, 116, 121, 112, 101, 61, 34, 116, 110, 115, 58, 76, 105, 115, 116, 79, 102, 69, 120, 112, 114, 101, 115, 115, 105, 111, 110, 76, 111, 103, 105, 99, 69, 110, 117, 109, 34, 32, 110, 97, 109, 101, 61, 34, 76, 105, 115, 116, 79, 102, 69, 120, 112, 114, 101, 115, 115, 105, 111, 110, 76, 111, 103, 105, 99, 69, 110, 117, 109, 34, 32, 110, 105, 108, 108, 97, 98, 108, 101, 61, 34, 116, 114, 117, 101, 34, 47, 62, 10, 32, 60, 120, 115, 58, 115, 105, 109, 112, 108, 101, 84, 121, 112, 101, 32, 110, 97, 109, 101, 61, 34, 69, 120, 112, 114, 101, 115, 115, 105, 111, 110, 83, 101, 109, 97, 110, 116, 105, 99, 69, 110, 117, 109, 34, 62, 10, 32, 32, 60, 120, 115, 58, 114, 101, 115, 116, 114, 105, 99, 116, 105, 111, 110, 32, 98, 97, 115, 101, 61, 34, 120, 115, 58, 115, 116, 114, 105, 110, 103, 34, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 110, 117, 109, 101, 114, 97, 116, 105, 111, 110, 32, 118, 97, 108, 117, 101, 61, 34, 65, 83, 83, 85, 82, 65, 78, 67, 69, 95, 48, 34, 47, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 110, 117, 109, 101, 114, 97, 116, 105, 111, 110, 32, 118, 97, 108, 117, 101, 61, 34, 83, 69, 84, 84, 73, 78, 71, 95, 49, 34, 47, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 110, 117, 109, 101, 114, 97, 116, 105, 111, 110, 32, 118, 97, 108, 117, 101, 61, 34, 77, 69, 65, 83, 85, 82, 69, 77, 69, 78, 84, 95, 50, 34, 47, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 110, 117, 109, 101, 114, 97, 116, 105, 111, 110, 32, 118, 97, 108, 117, 101, 61, 34, 82, 69, 81, 85, 73, 82, 69, 77, 69, 78, 84, 95, 51, 34, 47, 62, 10, 32, 32, 60, 47, 120, 115, 58, 114, 101, 115, 116, 114, 105, 99, 116, 105, 111, 110, 62, 10, 32, 60, 47, 120, 115, 58, 115, 105, 109, 112, 108, 101, 84, 121, 112, 101, 62, 10, 32, 60, 120, 115, 58, 101, 108, 101, 109, 101, 110, 116, 32, 116, 121, 112, 101, 61, 34, 116, 110, 115, 58, 69, 120, 112, 114, 101, 115, 115, 105, 111, 110, 83, 101, 109, 97, 110, 116, 105, 99, 69, 110, 117, 109, 34, 32, 110, 97, 109, 101, 61, 34, 69, 120, 112, 114, 101, 115, 115, 105, 111, 110, 83, 101, 109, 97, 110, 116, 105, 99, 69, 110, 117, 109, 34, 47, 62, 10, 32, 60, 120, 115, 58, 99, 111, 109, 112, 108, 101, 120, 84, 121, 112, 101, 32, 110, 97, 109, 101, 61, 34, 76, 105, 115, 116, 79, 102, 69, 120, 112, 114, 101, 115, 115, 105, 111, 110, 83, 101, 109, 97, 110, 116, 105, 99, 69, 110, 117, 109, 34, 62, 10, 32, 32, 60, 120, 115, 58, 115, 101, 113, 117, 101, 110, 99, 101, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 108, 101, 109, 101, 110, 116, 32, 109, 105, 110, 79, 99, 99, 117, 114, 115, 61, 34, 48, 34, 32, 109, 97, 120, 79, 99, 99, 117, 114, 115, 61, 34, 117, 110, 98, 111, 117, 110, 100, 101, 100, 34, 32, 116, 121, 112, 101, 61, 34, 116, 110, 115, 58, 69, 120, 112, 114, 101, 115, 115, 105, 111, 110, 83, 101, 109, 97, 110, 116, 105, 99, 69, 110, 117, 109, 34, 32, 110, 97, 109, 101, 61, 34, 69, 120, 112, 114, 101, 115, 115, 105, 111, 110, 83, 101, 109, 97, 110, 116, 105, 99, 69, 110, 117, 109, 34, 32, 110, 105, 108, 108, 97, 98, 108, 101, 61, 34, 116, 114, 117, 101, 34, 47, 62, 10, 32, 32, 60, 47, 120, 115, 58, 115, 101, 113, 117, 101, 110, 99, 101, 62, 10, 32, 60, 47, 120, 115, 58, 99, 111, 109, 112, 108, 101, 120, 84, 121, 112, 101, 62, 10, 32, 60, 120, 115, 58, 101, 108, 101, 109, 101, 110, 116, 32, 116, 121, 112, 101, 61, 34, 116, 110, 115, 58, 76, 105, 115, 116, 79, 102, 69, 120, 112, 114, 101, 115, 115, 105, 111, 110, 83, 101, 109, 97, 110, 116, 105, 99, 69, 110, 117, 109, 34, 32, 110, 97, 109, 101, 61, 34, 76, 105, 115, 116, 79, 102, 69, 120, 112, 114, 101, 115, 115, 105, 111, 110, 83, 101, 109, 97, 110, 116, 105, 99, 69, 110, 117, 109, 34, 32, 110, 105, 108, 108, 97, 98, 108, 101, 61, 34, 116, 114, 117, 101, 34, 47, 62, 10, 32, 60, 120, 115, 58, 115, 105, 109, 112, 108, 101, 84, 121, 112, 101, 32, 110, 97, 109, 101, 61, 34, 86, 105, 101, 119, 69, 110, 117, 109, 34, 62, 10, 32, 32, 60, 120, 115, 58, 114, 101, 115, 116, 114, 105, 99, 116, 105, 111, 110, 32, 98, 97, 115, 101, 61, 34, 120, 115, 58, 115, 116, 114, 105, 110, 103, 34, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 110, 117, 109, 101, 114, 97, 116, 105, 111, 110, 32, 118, 97, 108, 117, 101, 61, 34, 66, 85, 83, 73, 78, 69, 83, 83, 95, 48, 34, 47, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 110, 117, 109, 101, 114, 97, 116, 105, 111, 110, 32, 118, 97, 108, 117, 101, 61, 34, 67, 79, 78, 83, 84, 82, 85, 67, 84, 73, 79, 78, 95, 49, 34, 47, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 110, 117, 109, 101, 114, 97, 116, 105, 111, 110, 32, 118, 97, 108, 117, 101, 61, 34, 80, 79, 87, 69, 82, 95, 50, 34, 47, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 110, 117, 109, 101, 114, 97, 116, 105, 111, 110, 32, 118, 97, 108, 117, 101, 61, 34, 70, 85, 78, 67, 84, 73, 79, 78, 65, 76, 95, 51, 34, 47, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 110, 117, 109, 101, 114, 97, 116, 105, 111, 110, 32, 118, 97, 108, 117, 101, 61, 34, 76, 79, 67, 65, 84, 73, 79, 78, 95, 52, 34, 47, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 110, 117, 109, 101, 114, 97, 116, 105, 111, 110, 32, 118, 97, 108, 117, 101, 61, 34, 83, 69, 67, 85, 82, 73, 84, 89, 95, 53, 34, 47, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 110, 117, 109, 101, 114, 97, 116, 105, 111, 110, 32, 118, 97, 108, 117, 101, 61, 34, 78, 69, 84, 87, 79, 82, 75, 95, 54, 34, 47, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 110, 117, 109, 101, 114, 97, 116, 105, 111, 110, 32, 118, 97, 108, 117, 101, 61, 34, 76, 73, 70, 69, 67, 89, 67, 76, 69, 95, 55, 34, 47, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 110, 117, 109, 101, 114, 97, 116, 105, 111, 110, 32, 118, 97, 108, 117, 101, 61, 34, 72, 85, 77, 65, 78, 95, 56, 34, 47, 62, 10, 32, 32, 60, 47, 120, 115, 58, 114, 101, 115, 116, 114, 105, 99, 116, 105, 111, 110, 62, 10, 32, 60, 47, 120, 115, 58, 115, 105, 109, 112, 108, 101, 84, 121, 112, 101, 62, 10, 32, 60, 120, 115, 58, 101, 108, 101, 109, 101, 110, 116, 32, 116, 121, 112, 101, 61, 34, 116, 110, 115, 58, 86, 105, 101, 119, 69, 110, 117, 109, 34, 32, 110, 97, 109, 101, 61, 34, 86, 105, 101, 119, 69, 110, 117, 109, 34, 47, 62, 10, 32, 60, 120, 115, 58, 99, 111, 109, 112, 108, 101, 120, 84, 121, 112, 101, 32, 110, 97, 109, 101, 61, 34, 76, 105, 115, 116, 79, 102, 86, 105, 101, 119, 69, 110, 117, 109, 34, 62, 10, 32, 32, 60, 120, 115, 58, 115, 101, 113, 117, 101, 110, 99, 101, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 108, 101, 109, 101, 110, 116, 32, 109, 105, 110, 79, 99, 99, 117, 114, 115, 61, 34, 48, 34, 32, 109, 97, 120, 79, 99, 99, 117, 114, 115, 61, 34, 117, 110, 98, 111, 117, 110, 100, 101, 100, 34, 32, 116, 121, 112, 101, 61, 34, 116, 110, 115, 58, 86, 105, 101, 119, 69, 110, 117, 109, 34, 32, 110, 97, 109, 101, 61, 34, 86, 105, 101, 119, 69, 110, 117, 109, 34, 32, 110, 105, 108, 108, 97, 98, 108, 101, 61, 34, 116, 114, 117, 101, 34, 47, 62, 10, 32, 32, 60, 47, 120, 115, 58, 115, 101, 113, 117, 101, 110, 99, 101, 62, 10, 32, 60, 47, 120, 115, 58, 99, 111, 109, 112, 108, 101, 120, 84, 121, 112, 101, 62, 10, 32, 60, 120, 115, 58, 101, 108, 101, 109, 101, 110, 116, 32, 116, 121, 112, 101, 61, 34, 116, 110, 115, 58, 76, 105, 115, 116, 79, 102, 86, 105, 101, 119, 69, 110, 117, 109, 34, 32, 110, 97, 109, 101, 61, 34, 76, 105, 115, 116, 79, 102, 86, 105, 101, 119, 69, 110, 117, 109, 34, 32, 110, 105, 108, 108, 97, 98, 108, 101, 61, 34, 116, 114, 117, 101, 34, 47, 62, 10, 32, 60, 120, 115, 58, 115, 105, 109, 112, 108, 101, 84, 121, 112, 101, 32, 110, 97, 109, 101, 61, 34, 86, 105, 115, 105, 98, 105, 108, 105, 116, 121, 69, 110, 117, 109, 34, 62, 10, 32, 32, 60, 120, 115, 58, 114, 101, 115, 116, 114, 105, 99, 116, 105, 111, 110, 32, 98, 97, 115, 101, 61, 34, 120, 115, 58, 115, 116, 114, 105, 110, 103, 34, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 110, 117, 109, 101, 114, 97, 116, 105, 111, 110, 32, 118, 97, 108, 117, 101, 61, 34, 80, 82, 73, 86, 65, 84, 95, 48, 34, 47, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 110, 117, 109, 101, 114, 97, 116, 105, 111, 110, 32, 118, 97, 108, 117, 101, 61, 34, 67, 79, 78, 84, 82, 65, 67, 84, 95, 49, 34, 47, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 110, 117, 109, 101, 114, 97, 116, 105, 111, 110, 32, 118, 97, 108, 117, 101, 61, 34, 80, 85, 66, 76, 73, 67, 95, 50, 34, 47, 62, 10, 32, 32, 60, 47, 120, 115, 58, 114, 101, 115, 116, 114, 105, 99, 116, 105, 111, 110, 62, 10, 32, 60, 47, 120, 115, 58, 115, 105, 109, 112, 108, 101, 84, 121, 112, 101, 62, 10, 32, 60, 120, 115, 58, 101, 108, 101, 109, 101, 110, 116, 32, 116, 121, 112, 101, 61, 34, 116, 110, 115, 58, 86, 105, 115, 105, 98, 105, 108, 105, 116, 121, 69, 110, 117, 109, 34, 32, 110, 97, 109, 101, 61, 34, 86, 105, 115, 105, 98, 105, 108, 105, 116, 121, 69, 110, 117, 109, 34, 47, 62, 10, 32, 60, 120, 115, 58, 99, 111, 109, 112, 108, 101, 120, 84, 121, 112, 101, 32, 110, 97, 109, 101, 61, 34, 76, 105, 115, 116, 79, 102, 86, 105, 115, 105, 98, 105, 108, 105, 116, 121, 69, 110, 117, 109, 34, 62, 10, 32, 32, 60, 120, 115, 58, 115, 101, 113, 117, 101, 110, 99, 101, 62, 10, 32, 32, 32, 60, 120, 115, 58, 101, 108, 101, 109, 101, 110, 116, 32, 109, 105, 110, 79, 99, 99, 117, 114, 115, 61, 34, 48, 34, 32, 109, 97, 120, 79, 99, 99, 117, 114, 115, 61, 34, 117, 110, 98, 111, 117, 110, 100, 101, 100, 34, 32, 116, 121, 112, 101, 61, 34, 116, 110, 115, 58, 86, 105, 115, 105, 98, 105, 108, 105, 116, 121, 69, 110, 117, 109, 34, 32, 110, 97, 109, 101, 61, 34, 86, 105, 115, 105, 98, 105, 108, 105, 116, 121, 69, 110, 117, 109, 34, 32, 110, 105, 108, 108, 97, 98, 108, 101, 61, 34, 116, 114, 117, 101, 34, 47, 62, 10, 32, 32, 60, 47, 120, 115, 58, 115, 101, 113, 117, 101, 110, 99, 101, 62, 10, 32, 60, 47, 120, 115, 58, 99, 111, 109, 112, 108, 101, 120, 84, 121, 112, 101, 62, 10, 32, 60, 120, 115, 58, 101, 108, 101, 109, 101, 110, 116, 32, 116, 121, 112, 101, 61, 34, 116, 110, 115, 58, 76, 105, 115, 116, 79, 102, 86, 105, 115, 105, 98, 105, 108, 105, 116, 121, 69, 110, 117, 109, 34, 32, 110, 97, 109, 101, 61, 34, 76, 105, 115, 116, 79, 102, 86, 105, 115, 105, 98, 105, 108, 105, 116, 121, 69, 110, 117, 109, 34, 32, 110, 105, 108, 108, 97, 98, 108, 101, 61, 34, 116, 114, 117, 101, 34, 47, 62, 10, 60, 47, 120, 115, 58, 115, 99, 104, 101, 109, 97, 62, 10};



static UA_StatusCode function_propertyValueStatement_8_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
/* Value rank inherited */
attr.valueRank = -1;
attr.dataType = UA_NODEID_NUMERIC(ns[0], 15);
UA_ByteString *variablenode_ns_2_i_6007_variant_DataContents =  UA_ByteString_new();
if (!variablenode_ns_2_i_6007_variant_DataContents) return UA_STATUSCODE_BADOUTOFMEMORY;
UA_ByteString_init(variablenode_ns_2_i_6007_variant_DataContents);
variablenode_ns_2_i_6007_variant_DataContents->length = 3219;
variablenode_ns_2_i_6007_variant_DataContents->data = (UA_Byte *)(void*)(uintptr_t)variablenode_ns_2_i_6007_variant_DataContents_byteArray;
UA_Variant_setScalar(&attr.value, variablenode_ns_2_i_6007_variant_DataContents, &UA_TYPES[UA_TYPES_BYTESTRING]);
attr.displayName = UA_LOCALIZEDTEXT("", "TypeDictionary");
attr.description = UA_LOCALIZEDTEXT("", "Collects the data type descriptions of acplt.org/propertyValueStatement/");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6007),
UA_NODEID_NUMERIC(ns[0], 92),
UA_NODEID_NUMERIC(ns[0], 47),
UA_QUALIFIEDNAME(ns[2], "TypeDictionary"),
UA_NODEID_NUMERIC(ns[0], 72),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
variablenode_ns_2_i_6007_variant_DataContents->data = NULL;
variablenode_ns_2_i_6007_variant_DataContents->length = 0;
UA_ByteString_delete(variablenode_ns_2_i_6007_variant_DataContents);
return retVal;
}

static UA_StatusCode function_propertyValueStatement_8_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6007)
);
}

/* NamespaceUri - ns=2;i=6008 */

static UA_StatusCode function_propertyValueStatement_9_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
/* Value rank inherited */
attr.valueRank = -1;
attr.dataType = UA_NODEID_NUMERIC(ns[0], 12);
UA_String *variablenode_ns_2_i_6008_variant_DataContents =  UA_String_new();
if (!variablenode_ns_2_i_6008_variant_DataContents) return UA_STATUSCODE_BADOUTOFMEMORY;
UA_String_init(variablenode_ns_2_i_6008_variant_DataContents);
*variablenode_ns_2_i_6008_variant_DataContents = UA_STRING_ALLOC("acplt.org/propertyValueStatement/Types.xsd");
UA_Variant_setScalar(&attr.value, variablenode_ns_2_i_6008_variant_DataContents, &UA_TYPES[UA_TYPES_STRING]);
attr.displayName = UA_LOCALIZEDTEXT("", "NamespaceUri");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6008),
UA_NODEID_NUMERIC(ns[2], 6007),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[0], "NamespaceUri"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
UA_String_delete(variablenode_ns_2_i_6008_variant_DataContents);
return retVal;
}

static UA_StatusCode function_propertyValueStatement_9_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6008)
);
}

/* TypeDictionary - ns=2;i=6005 */
static const UA_Byte variablenode_ns_2_i_6005_variant_DataContents_byteArray[1853] = {60, 111, 112, 99, 58, 84, 121, 112, 101, 68, 105, 99, 116, 105, 111, 110, 97, 114, 121, 32, 120, 109, 108, 110, 115, 58, 120, 115, 105, 61, 34, 104, 116, 116, 112, 58, 47, 47, 119, 119, 119, 46, 119, 51, 46, 111, 114, 103, 47, 50, 48, 48, 49, 47, 88, 77, 76, 83, 99, 104, 101, 109, 97, 45, 105, 110, 115, 116, 97, 110, 99, 101, 34, 32, 120, 109, 108, 110, 115, 58, 116, 110, 115, 61, 34, 104, 116, 116, 112, 58, 47, 47, 121, 111, 117, 114, 111, 114, 103, 97, 110, 105, 115, 97, 116, 105, 111, 110, 46, 111, 114, 103, 47, 112, 118, 115, 47, 34, 32, 68, 101, 102, 97, 117, 108, 116, 66, 121, 116, 101, 79, 114, 100, 101, 114, 61, 34, 76, 105, 116, 116, 108, 101, 69, 110, 100, 105, 97, 110, 34, 32, 120, 109, 108, 110, 115, 58, 111, 112, 99, 61, 34, 104, 116, 116, 112, 58, 47, 47, 111, 112, 99, 102, 111, 117, 110, 100, 97, 116, 105, 111, 110, 46, 111, 114, 103, 47, 66, 105, 110, 97, 114, 121, 83, 99, 104, 101, 109, 97, 47, 34, 32, 120, 109, 108, 110, 115, 58, 117, 97, 61, 34, 104, 116, 116, 112, 58, 47, 47, 111, 112, 99, 102, 111, 117, 110, 100, 97, 116, 105, 111, 110, 46, 111, 114, 103, 47, 85, 65, 47, 34, 32, 84, 97, 114, 103, 101, 116, 78, 97, 109, 101, 115, 112, 97, 99, 101, 61, 34, 104, 116, 116, 112, 58, 47, 47, 121, 111, 117, 114, 111, 114, 103, 97, 110, 105, 115, 97, 116, 105, 111, 110, 46, 111, 114, 103, 47, 112, 118, 115, 47, 34, 62, 10, 32, 60, 111, 112, 99, 58, 73, 109, 112, 111, 114, 116, 32, 78, 97, 109, 101, 115, 112, 97, 99, 101, 61, 34, 104, 116, 116, 112, 58, 47, 47, 111, 112, 99, 102, 111, 117, 110, 100, 97, 116, 105, 111, 110, 46, 111, 114, 103, 47, 85, 65, 47, 34, 47, 62, 10, 32, 60, 111, 112, 99, 58, 69, 110, 117, 109, 101, 114, 97, 116, 101, 100, 84, 121, 112, 101, 32, 76, 101, 110, 103, 116, 104, 73, 110, 66, 105, 116, 115, 61, 34, 51, 50, 34, 32, 78, 97, 109, 101, 61, 34, 69, 120, 112, 114, 101, 115, 115, 105, 111, 110, 76, 111, 103, 105, 99, 69, 110, 117, 109, 34, 62, 10, 32, 32, 60, 111, 112, 99, 58, 69, 110, 117, 109, 101, 114, 97, 116, 101, 100, 86, 97, 108, 117, 101, 32, 78, 97, 109, 101, 61, 34, 71, 82, 69, 65, 84, 69, 82, 95, 84, 72, 65, 78, 34, 32, 86, 97, 108, 117, 101, 61, 34, 48, 34, 47, 62, 10, 32, 32, 60, 111, 112, 99, 58, 69, 110, 117, 109, 101, 114, 97, 116, 101, 100, 86, 97, 108, 117, 101, 32, 78, 97, 109, 101, 61, 34, 71, 82, 69, 65, 84, 69, 82, 95, 69, 81, 85, 65, 76, 34, 32, 86, 97, 108, 117, 101, 61, 34, 49, 34, 47, 62, 10, 32, 32, 60, 111, 112, 99, 58, 69, 110, 117, 109, 101, 114, 97, 116, 101, 100, 86, 97, 108, 117, 101, 32, 78, 97, 109, 101, 61, 34, 69, 81, 85, 65, 76, 34, 32, 86, 97, 108, 117, 101, 61, 34, 50, 34, 47, 62, 10, 32, 32, 60, 111, 112, 99, 58, 69, 110, 117, 109, 101, 114, 97, 116, 101, 100, 86, 97, 108, 117, 101, 32, 78, 97, 109, 101, 61, 34, 78, 79, 84, 95, 69, 81, 85, 65, 76, 34, 32, 86, 97, 108, 117, 101, 61, 34, 51, 34, 47, 62, 10, 32, 32, 60, 111, 112, 99, 58, 69, 110, 117, 109, 101, 114, 97, 116, 101, 100, 86, 97, 108, 117, 101, 32, 78, 97, 109, 101, 61, 34, 76, 69, 83, 83, 95, 69, 81, 85, 65, 76, 34, 32, 86, 97, 108, 117, 101, 61, 34, 52, 34, 47, 62, 10, 32, 32, 60, 111, 112, 99, 58, 69, 110, 117, 109, 101, 114, 97, 116, 101, 100, 86, 97, 108, 117, 101, 32, 78, 97, 109, 101, 61, 34, 76, 69, 83, 83, 95, 84, 72, 65, 78, 34, 32, 86, 97, 108, 117, 101, 61, 34, 53, 34, 47, 62, 10, 32, 60, 47, 111, 112, 99, 58, 69, 110, 117, 109, 101, 114, 97, 116, 101, 100, 84, 121, 112, 101, 62, 10, 32, 60, 111, 112, 99, 58, 69, 110, 117, 109, 101, 114, 97, 116, 101, 100, 84, 121, 112, 101, 32, 76, 101, 110, 103, 116, 104, 73, 110, 66, 105, 116, 115, 61, 34, 51, 50, 34, 32, 78, 97, 109, 101, 61, 34, 69, 120, 112, 114, 101, 115, 115, 105, 111, 110, 83, 101, 109, 97, 110, 116, 105, 99, 69, 110, 117, 109, 34, 62, 10, 32, 32, 60, 111, 112, 99, 58, 69, 110, 117, 109, 101, 114, 97, 116, 101, 100, 86, 97, 108, 117, 101, 32, 78, 97, 109, 101, 61, 34, 65, 83, 83, 85, 82, 65, 78, 67, 69, 34, 32, 86, 97, 108, 117, 101, 61, 34, 48, 34, 47, 62, 10, 32, 32, 60, 111, 112, 99, 58, 69, 110, 117, 109, 101, 114, 97, 116, 101, 100, 86, 97, 108, 117, 101, 32, 78, 97, 109, 101, 61, 34, 83, 69, 84, 84, 73, 78, 71, 34, 32, 86, 97, 108, 117, 101, 61, 34, 49, 34, 47, 62, 10, 32, 32, 60, 111, 112, 99, 58, 69, 110, 117, 109, 101, 114, 97, 116, 101, 100, 86, 97, 108, 117, 101, 32, 78, 97, 109, 101, 61, 34, 77, 69, 65, 83, 85, 82, 69, 77, 69, 78, 84, 34, 32, 86, 97, 108, 117, 101, 61, 34, 50, 34, 47, 62, 10, 32, 32, 60, 111, 112, 99, 58, 69, 110, 117, 109, 101, 114, 97, 116, 101, 100, 86, 97, 108, 117, 101, 32, 78, 97, 109, 101, 61, 34, 82, 69, 81, 85, 73, 82, 69, 77, 69, 78, 84, 34, 32, 86, 97, 108, 117, 101, 61, 34, 51, 34, 47, 62, 10, 32, 60, 47, 111, 112, 99, 58, 69, 110, 117, 109, 101, 114, 97, 116, 101, 100, 84, 121, 112, 101, 62, 10, 32, 60, 111, 112, 99, 58, 69, 110, 117, 109, 101, 114, 97, 116, 101, 100, 84, 121, 112, 101, 32, 76, 101, 110, 103, 116, 104, 73, 110, 66, 105, 116, 115, 61, 34, 51, 50, 34, 32, 78, 97, 109, 101, 61, 34, 86, 105, 101, 119, 69, 110, 117, 109, 34, 62, 10, 32, 32, 60, 111, 112, 99, 58, 69, 110, 117, 109, 101, 114, 97, 116, 101, 100, 86, 97, 108, 117, 101, 32, 78, 97, 109, 101, 61, 34, 66, 85, 83, 73, 78, 69, 83, 83, 34, 32, 86, 97, 108, 117, 101, 61, 34, 48, 34, 47, 62, 10, 32, 32, 60, 111, 112, 99, 58, 69, 110, 117, 109, 101, 114, 97, 116, 101, 100, 86, 97, 108, 117, 101, 32, 78, 97, 109, 101, 61, 34, 67, 79, 78, 83, 84, 82, 85, 67, 84, 73, 79, 78, 34, 32, 86, 97, 108, 117, 101, 61, 34, 49, 34, 47, 62, 10, 32, 32, 60, 111, 112, 99, 58, 69, 110, 117, 109, 101, 114, 97, 116, 101, 100, 86, 97, 108, 117, 101, 32, 78, 97, 109, 101, 61, 34, 80, 79, 87, 69, 82, 34, 32, 86, 97, 108, 117, 101, 61, 34, 50, 34, 47, 62, 10, 32, 32, 60, 111, 112, 99, 58, 69, 110, 117, 109, 101, 114, 97, 116, 101, 100, 86, 97, 108, 117, 101, 32, 78, 97, 109, 101, 61, 34, 70, 85, 78, 67, 84, 73, 79, 78, 65, 76, 34, 32, 86, 97, 108, 117, 101, 61, 34, 51, 34, 47, 62, 10, 32, 32, 60, 111, 112, 99, 58, 69, 110, 117, 109, 101, 114, 97, 116, 101, 100, 86, 97, 108, 117, 101, 32, 78, 97, 109, 101, 61, 34, 76, 79, 67, 65, 84, 73, 79, 78, 34, 32, 86, 97, 108, 117, 101, 61, 34, 52, 34, 47, 62, 10, 32, 32, 60, 111, 112, 99, 58, 69, 110, 117, 109, 101, 114, 97, 116, 101, 100, 86, 97, 108, 117, 101, 32, 78, 97, 109, 101, 61, 34, 83, 69, 67, 85, 82, 73, 84, 89, 34, 32, 86, 97, 108, 117, 101, 61, 34, 53, 34, 47, 62, 10, 32, 32, 60, 111, 112, 99, 58, 69, 110, 117, 109, 101, 114, 97, 116, 101, 100, 86, 97, 108, 117, 101, 32, 78, 97, 109, 101, 61, 34, 78, 69, 84, 87, 79, 82, 75, 34, 32, 86, 97, 108, 117, 101, 61, 34, 54, 34, 47, 62, 10, 32, 32, 60, 111, 112, 99, 58, 69, 110, 117, 109, 101, 114, 97, 116, 101, 100, 86, 97, 108, 117, 101, 32, 78, 97, 109, 101, 61, 34, 76, 73, 70, 69, 67, 89, 67, 76, 69, 34, 32, 86, 97, 108, 117, 101, 61, 34, 55, 34, 47, 62, 10, 32, 32, 60, 111, 112, 99, 58, 69, 110, 117, 109, 101, 114, 97, 116, 101, 100, 86, 97, 108, 117, 101, 32, 78, 97, 109, 101, 61, 34, 72, 85, 77, 65, 78, 34, 32, 86, 97, 108, 117, 101, 61, 34, 56, 34, 47, 62, 10, 32, 60, 47, 111, 112, 99, 58, 69, 110, 117, 109, 101, 114, 97, 116, 101, 100, 84, 121, 112, 101, 62, 10, 32, 60, 111, 112, 99, 58, 69, 110, 117, 109, 101, 114, 97, 116, 101, 100, 84, 121, 112, 101, 32, 76, 101, 110, 103, 116, 104, 73, 110, 66, 105, 116, 115, 61, 34, 51, 50, 34, 32, 78, 97, 109, 101, 61, 34, 86, 105, 115, 105, 98, 105, 108, 105, 116, 121, 69, 110, 117, 109, 34, 62, 10, 32, 32, 60, 111, 112, 99, 58, 69, 110, 117, 109, 101, 114, 97, 116, 101, 100, 86, 97, 108, 117, 101, 32, 78, 97, 109, 101, 61, 34, 80, 82, 73, 86, 65, 84, 34, 32, 86, 97, 108, 117, 101, 61, 34, 48, 34, 47, 62, 10, 32, 32, 60, 111, 112, 99, 58, 69, 110, 117, 109, 101, 114, 97, 116, 101, 100, 86, 97, 108, 117, 101, 32, 78, 97, 109, 101, 61, 34, 67, 79, 78, 84, 82, 65, 67, 84, 34, 32, 86, 97, 108, 117, 101, 61, 34, 49, 34, 47, 62, 10, 32, 32, 60, 111, 112, 99, 58, 69, 110, 117, 109, 101, 114, 97, 116, 101, 100, 86, 97, 108, 117, 101, 32, 78, 97, 109, 101, 61, 34, 80, 85, 66, 76, 73, 67, 34, 32, 86, 97, 108, 117, 101, 61, 34, 50, 34, 47, 62, 10, 32, 60, 47, 111, 112, 99, 58, 69, 110, 117, 109, 101, 114, 97, 116, 101, 100, 84, 121, 112, 101, 62, 10, 60, 47, 111, 112, 99, 58, 84, 121, 112, 101, 68, 105, 99, 116, 105, 111, 110, 97, 114, 121, 62, 10};



static UA_StatusCode function_propertyValueStatement_10_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
/* Value rank inherited */
attr.valueRank = -1;
attr.dataType = UA_NODEID_NUMERIC(ns[0], 15);
UA_ByteString *variablenode_ns_2_i_6005_variant_DataContents =  UA_ByteString_new();
if (!variablenode_ns_2_i_6005_variant_DataContents) return UA_STATUSCODE_BADOUTOFMEMORY;
UA_ByteString_init(variablenode_ns_2_i_6005_variant_DataContents);
variablenode_ns_2_i_6005_variant_DataContents->length = 1853;
variablenode_ns_2_i_6005_variant_DataContents->data = (UA_Byte *)(void*)(uintptr_t)variablenode_ns_2_i_6005_variant_DataContents_byteArray;
UA_Variant_setScalar(&attr.value, variablenode_ns_2_i_6005_variant_DataContents, &UA_TYPES[UA_TYPES_BYTESTRING]);
attr.displayName = UA_LOCALIZEDTEXT("", "TypeDictionary");
attr.description = UA_LOCALIZEDTEXT("", "Collects the data type descriptions of acplt.org/propertyValueStatement/");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6005),
UA_NODEID_NUMERIC(ns[0], 93),
UA_NODEID_NUMERIC(ns[0], 47),
UA_QUALIFIEDNAME(ns[2], "TypeDictionary"),
UA_NODEID_NUMERIC(ns[0], 72),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
variablenode_ns_2_i_6005_variant_DataContents->data = NULL;
variablenode_ns_2_i_6005_variant_DataContents->length = 0;
UA_ByteString_delete(variablenode_ns_2_i_6005_variant_DataContents);
return retVal;
}

static UA_StatusCode function_propertyValueStatement_10_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6005)
);
}

/* NamespaceUri - ns=2;i=6006 */

static UA_StatusCode function_propertyValueStatement_11_begin(UA_Server *server, UA_UInt16* ns) {
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
*variablenode_ns_2_i_6006_variant_DataContents = UA_STRING_ALLOC("acplt.org/propertyValueStatement/");
UA_Variant_setScalar(&attr.value, variablenode_ns_2_i_6006_variant_DataContents, &UA_TYPES[UA_TYPES_STRING]);
attr.displayName = UA_LOCALIZEDTEXT("", "NamespaceUri");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6006),
UA_NODEID_NUMERIC(ns[2], 6005),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[0], "NamespaceUri"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
UA_String_delete(variablenode_ns_2_i_6006_variant_DataContents);
return retVal;
}

static UA_StatusCode function_propertyValueStatement_11_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6006)
);
}

/* PropertyValueStatementListType - ns=2;i=1002 */

static UA_StatusCode function_propertyValueStatement_12_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_ObjectTypeAttributes attr = UA_ObjectTypeAttributes_default;
attr.displayName = UA_LOCALIZEDTEXT("", "PropertyValueStatementListType");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_OBJECTTYPE,
UA_NODEID_NUMERIC(ns[2], 1002),
UA_NODEID_NUMERIC(ns[0], 58),
UA_NODEID_NUMERIC(ns[0], 45),
UA_QUALIFIEDNAME(ns[2], "PropertyValueStatementListType"),
 UA_NODEID_NULL,
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_OBJECTTYPEATTRIBUTES],NULL, NULL);
return retVal;
}

static UA_StatusCode function_propertyValueStatement_12_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 1002)
);
}

/* ExpressionSemantic - ns=2;i=6003 */

static UA_StatusCode function_propertyValueStatement_13_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 3;
attr.accessLevel = 3;
/* Value rank inherited */
attr.valueRank = -1;
attr.dataType = UA_NODEID_NUMERIC(ns[2], 3003);
attr.displayName = UA_LOCALIZEDTEXT("", "ExpressionSemantic");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6003),
UA_NODEID_NUMERIC(ns[2], 1002),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[2], "ExpressionSemantic"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6003), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 80), true);
return retVal;
}

static UA_StatusCode function_propertyValueStatement_13_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6003)
);
}

/* View - ns=2;i=6014 */

static UA_StatusCode function_propertyValueStatement_14_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 3;
attr.accessLevel = 3;
/* Value rank inherited */
attr.valueRank = -1;
attr.dataType = UA_NODEID_NUMERIC(ns[2], 3004);
attr.displayName = UA_LOCALIZEDTEXT("", "View");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6014),
UA_NODEID_NUMERIC(ns[2], 1002),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[2], "View"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6014), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 80), true);
return retVal;
}

static UA_StatusCode function_propertyValueStatement_14_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6014)
);
}

/* CarrierID - ns=2;i=6001 */

static UA_StatusCode function_propertyValueStatement_15_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 3;
attr.accessLevel = 3;
/* Value rank inherited */
attr.valueRank = -1;
attr.dataType = UA_NODEID_NUMERIC(ns[1], 3002);
attr.displayName = UA_LOCALIZEDTEXT("", "CarrierID");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6001),
UA_NODEID_NUMERIC(ns[2], 1002),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[2], "CarrierID"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6001), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 80), true);
return retVal;
}

static UA_StatusCode function_propertyValueStatement_15_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6001)
);
}

/* PropertyID - ns=2;i=6010 */

static UA_StatusCode function_propertyValueStatement_16_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 3;
attr.accessLevel = 3;
/* Value rank inherited */
attr.valueRank = -1;
attr.dataType = UA_NODEID_NUMERIC(ns[1], 3002);
attr.displayName = UA_LOCALIZEDTEXT("", "PropertyID");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6010),
UA_NODEID_NUMERIC(ns[2], 1002),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[2], "PropertyID"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6010), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 80), true);
return retVal;
}

static UA_StatusCode function_propertyValueStatement_16_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6010)
);
}

/* ExpressionLogic - ns=2;i=6002 */

static UA_StatusCode function_propertyValueStatement_17_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 3;
attr.accessLevel = 3;
/* Value rank inherited */
attr.valueRank = -1;
attr.dataType = UA_NODEID_NUMERIC(ns[2], 3002);
attr.displayName = UA_LOCALIZEDTEXT("", "ExpressionLogic");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6002),
UA_NODEID_NUMERIC(ns[2], 1002),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[2], "ExpressionLogic"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6002), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 80), true);
return retVal;
}

static UA_StatusCode function_propertyValueStatement_17_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6002)
);
}

/* Visibility - ns=2;i=6019 */

static UA_StatusCode function_propertyValueStatement_18_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 3;
attr.accessLevel = 3;
/* Value rank inherited */
attr.valueRank = -1;
attr.dataType = UA_NODEID_NUMERIC(ns[2], 3005);
attr.displayName = UA_LOCALIZEDTEXT("", "Visibility");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6019),
UA_NODEID_NUMERIC(ns[2], 1002),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[2], "Visibility"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6019), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 80), true);
return retVal;
}

static UA_StatusCode function_propertyValueStatement_18_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6019)
);
}

/* PropertyValueStatementType - ns=2;i=2002 */

static UA_StatusCode function_propertyValueStatement_19_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableTypeAttributes attr = UA_VariableTypeAttributes_default;
attr.valueRank = -2;
/* DataType inherited */
attr.dataType = UA_NODEID_NUMERIC(ns[0], 24);
attr.displayName = UA_LOCALIZEDTEXT("", "PropertyValueStatementType");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLETYPE,
UA_NODEID_NUMERIC(ns[2], 2002),
UA_NODEID_NUMERIC(ns[0], 63),
UA_NODEID_NUMERIC(ns[0], 45),
UA_QUALIFIEDNAME(ns[2], "PropertyValueStatementType"),
UA_NODEID_NUMERIC(ns[0], 0),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLETYPEATTRIBUTES],NULL, NULL);
return retVal;
}

static UA_StatusCode function_propertyValueStatement_19_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 2002)
);
}

/* <PropertyValueStatements> - ns=2;i=6011 */

static UA_StatusCode function_propertyValueStatement_20_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 3;
attr.accessLevel = 3;
/* Value rank inherited */
attr.valueRank = -1;
/* DataType inherited */
attr.dataType = UA_NODEID_NUMERIC(ns[0], 24);
attr.displayName = UA_LOCALIZEDTEXT("", "<PropertyValueStatements>");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6011),
UA_NODEID_NUMERIC(ns[2], 1002),
UA_NODEID_NUMERIC(ns[0], 47),
UA_QUALIFIEDNAME(ns[2], "<PropertyValueStatements>"),
UA_NODEID_NUMERIC(ns[2], 2002),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6011), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 11510), true);
return retVal;
}

static UA_StatusCode function_propertyValueStatement_20_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6011)
);
}

/* Value - ns=2;i=6013 */

static UA_StatusCode function_propertyValueStatement_21_begin(UA_Server *server, UA_UInt16* ns) {
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
UA_NODEID_NUMERIC(ns[2], 6013),
UA_NODEID_NUMERIC(ns[2], 6011),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[2], "Value"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6013), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_propertyValueStatement_21_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6013)
);
}

/* CarrierId - ns=2;i=6025 */

static UA_StatusCode function_propertyValueStatement_22_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 3;
attr.accessLevel = 3;
/* Value rank inherited */
attr.valueRank = -1;
attr.dataType = UA_NODEID_NUMERIC(ns[1], 3002);
attr.displayName = UA_LOCALIZEDTEXT("", "CarrierId");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6025),
UA_NODEID_NUMERIC(ns[2], 2002),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[2], "CarrierId"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6025), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 80), true);
return retVal;
}

static UA_StatusCode function_propertyValueStatement_22_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6025)
);
}

/* PropertyId - ns=2;i=6026 */

static UA_StatusCode function_propertyValueStatement_23_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 3;
attr.accessLevel = 3;
/* Value rank inherited */
attr.valueRank = -1;
attr.dataType = UA_NODEID_NUMERIC(ns[1], 3002);
attr.displayName = UA_LOCALIZEDTEXT("", "PropertyId");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6026),
UA_NODEID_NUMERIC(ns[2], 2002),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[2], "PropertyId"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6026), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 80), true);
return retVal;
}

static UA_StatusCode function_propertyValueStatement_23_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6026)
);
}

/* ExpressionLogic - ns=2;i=6027 */

static UA_StatusCode function_propertyValueStatement_24_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 3;
attr.accessLevel = 3;
/* Value rank inherited */
attr.valueRank = -1;
attr.dataType = UA_NODEID_NUMERIC(ns[2], 3002);
attr.displayName = UA_LOCALIZEDTEXT("", "ExpressionLogic");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6027),
UA_NODEID_NUMERIC(ns[2], 2002),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[2], "ExpressionLogic"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6027), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 80), true);
return retVal;
}

static UA_StatusCode function_propertyValueStatement_24_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6027)
);
}

/* Visibility - ns=2;i=6018 */

static UA_StatusCode function_propertyValueStatement_25_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 3;
attr.accessLevel = 3;
/* Value rank inherited */
attr.valueRank = -1;
attr.dataType = UA_NODEID_NUMERIC(ns[2], 3005);
attr.displayName = UA_LOCALIZEDTEXT("", "Visibility");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6018),
UA_NODEID_NUMERIC(ns[2], 2002),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[2], "Visibility"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6018), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 80), true);
return retVal;
}

static UA_StatusCode function_propertyValueStatement_25_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6018)
);
}

/* View - ns=2;i=6017 */

static UA_StatusCode function_propertyValueStatement_26_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 3;
attr.accessLevel = 3;
/* Value rank inherited */
attr.valueRank = -1;
attr.dataType = UA_NODEID_NUMERIC(ns[2], 3004);
attr.displayName = UA_LOCALIZEDTEXT("", "View");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6017),
UA_NODEID_NUMERIC(ns[2], 2002),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[2], "View"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6017), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 80), true);
return retVal;
}

static UA_StatusCode function_propertyValueStatement_26_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6017)
);
}

/* ExpressionSemantic - ns=2;i=6028 */

static UA_StatusCode function_propertyValueStatement_27_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 3;
attr.accessLevel = 3;
/* Value rank inherited */
attr.valueRank = -1;
attr.dataType = UA_NODEID_NUMERIC(ns[2], 3003);
attr.displayName = UA_LOCALIZEDTEXT("", "ExpressionSemantic");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[2], 6028),
UA_NODEID_NUMERIC(ns[2], 2002),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[2], "ExpressionSemantic"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6028), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 80), true);
return retVal;
}

static UA_StatusCode function_propertyValueStatement_27_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6028)
);
}

/* Value - ns=2;i=6012 */

static UA_StatusCode function_propertyValueStatement_28_begin(UA_Server *server, UA_UInt16* ns) {
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
UA_NODEID_NUMERIC(ns[2], 6012),
UA_NODEID_NUMERIC(ns[2], 2002),
UA_NODEID_NUMERIC(ns[0], 46),
UA_QUALIFIEDNAME(ns[2], "Value"),
UA_NODEID_NUMERIC(ns[0], 68),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[2], 6012), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
return retVal;
}

static UA_StatusCode function_propertyValueStatement_28_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[2], 6012)
);
}

UA_StatusCode nodeset_propertyValueStatement(UA_Server *server) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
/* Use namespace ids generated by the server */
UA_UInt16 ns[3];
ns[0] = UA_Server_addNamespace(server, "http://opcfoundation.org/UA/");
ns[1] = UA_Server_addNamespace(server, "acplt.org/identification/");
ns[2] = UA_Server_addNamespace(server, "acplt.org/propertyValueStatement/");
bool dummy = (
!(retVal = function_propertyValueStatement_0_begin(server, ns)) &&
!(retVal = function_propertyValueStatement_1_begin(server, ns)) &&
!(retVal = function_propertyValueStatement_2_begin(server, ns)) &&
!(retVal = function_propertyValueStatement_3_begin(server, ns)) &&
!(retVal = function_propertyValueStatement_4_begin(server, ns)) &&
!(retVal = function_propertyValueStatement_5_begin(server, ns)) &&
!(retVal = function_propertyValueStatement_6_begin(server, ns)) &&
!(retVal = function_propertyValueStatement_7_begin(server, ns)) &&
!(retVal = function_propertyValueStatement_8_begin(server, ns)) &&
!(retVal = function_propertyValueStatement_9_begin(server, ns)) &&
!(retVal = function_propertyValueStatement_10_begin(server, ns)) &&
!(retVal = function_propertyValueStatement_11_begin(server, ns)) &&
!(retVal = function_propertyValueStatement_12_begin(server, ns)) &&
!(retVal = function_propertyValueStatement_13_begin(server, ns)) &&
!(retVal = function_propertyValueStatement_14_begin(server, ns)) &&
!(retVal = function_propertyValueStatement_15_begin(server, ns)) &&
!(retVal = function_propertyValueStatement_16_begin(server, ns)) &&
!(retVal = function_propertyValueStatement_17_begin(server, ns)) &&
!(retVal = function_propertyValueStatement_18_begin(server, ns)) &&
!(retVal = function_propertyValueStatement_19_begin(server, ns)) &&
!(retVal = function_propertyValueStatement_20_begin(server, ns)) &&
!(retVal = function_propertyValueStatement_21_begin(server, ns)) &&
!(retVal = function_propertyValueStatement_22_begin(server, ns)) &&
!(retVal = function_propertyValueStatement_23_begin(server, ns)) &&
!(retVal = function_propertyValueStatement_24_begin(server, ns)) &&
!(retVal = function_propertyValueStatement_25_begin(server, ns)) &&
!(retVal = function_propertyValueStatement_26_begin(server, ns)) &&
!(retVal = function_propertyValueStatement_27_begin(server, ns)) &&
!(retVal = function_propertyValueStatement_28_begin(server, ns)) &&
!(retVal = function_propertyValueStatement_28_finish(server, ns)) &&
!(retVal = function_propertyValueStatement_27_finish(server, ns)) &&
!(retVal = function_propertyValueStatement_26_finish(server, ns)) &&
!(retVal = function_propertyValueStatement_25_finish(server, ns)) &&
!(retVal = function_propertyValueStatement_24_finish(server, ns)) &&
!(retVal = function_propertyValueStatement_23_finish(server, ns)) &&
!(retVal = function_propertyValueStatement_22_finish(server, ns)) &&
!(retVal = function_propertyValueStatement_21_finish(server, ns)) &&
!(retVal = function_propertyValueStatement_20_finish(server, ns)) &&
!(retVal = function_propertyValueStatement_19_finish(server, ns)) &&
!(retVal = function_propertyValueStatement_18_finish(server, ns)) &&
!(retVal = function_propertyValueStatement_17_finish(server, ns)) &&
!(retVal = function_propertyValueStatement_16_finish(server, ns)) &&
!(retVal = function_propertyValueStatement_15_finish(server, ns)) &&
!(retVal = function_propertyValueStatement_14_finish(server, ns)) &&
!(retVal = function_propertyValueStatement_13_finish(server, ns)) &&
!(retVal = function_propertyValueStatement_12_finish(server, ns)) &&
!(retVal = function_propertyValueStatement_11_finish(server, ns)) &&
!(retVal = function_propertyValueStatement_10_finish(server, ns)) &&
!(retVal = function_propertyValueStatement_9_finish(server, ns)) &&
!(retVal = function_propertyValueStatement_8_finish(server, ns)) &&
!(retVal = function_propertyValueStatement_7_finish(server, ns)) &&
!(retVal = function_propertyValueStatement_6_finish(server, ns)) &&
!(retVal = function_propertyValueStatement_5_finish(server, ns)) &&
!(retVal = function_propertyValueStatement_4_finish(server, ns)) &&
!(retVal = function_propertyValueStatement_3_finish(server, ns)) &&
!(retVal = function_propertyValueStatement_2_finish(server, ns)) &&
!(retVal = function_propertyValueStatement_1_finish(server, ns)) &&
!(retVal = function_propertyValueStatement_0_finish(server, ns)) 
); (void)(dummy);
return retVal;
}
