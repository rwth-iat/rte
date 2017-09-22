/* WARNING: This is a generated file.
 * Any manual changes will be overwritten.

 */
#include "nodeset_openaas.h"
UA_INLINE UA_StatusCode nodeset(UA_Server *server){
  return nodeset_returnNamespaces(server, NULL, NULL);
}

UA_INLINE UA_StatusCode nodeset_returnNamespaces(
        UA_Server *server, UA_UInt16 *namespacesSize, UA_Namespace **namespaces) {
  UA_StatusCode retval = UA_STATUSCODE_GOOD;
  UA_Namespace* nsArray = UA_malloc(5 * sizeof(UA_Namespace));
  UA_String tempNsUri;

  //Adding namespace for old namespace index = 0 with uri: http://opcfoundation.org/UA/
  tempNsUri = UA_String_fromChars("http://opcfoundation.org/UA/");
  UA_Namespace_init(&nsArray[0], &tempNsUri);
  UA_String_deleteMembers(&tempNsUri);
  retval |= UA_Server_addNamespace_full(server, &nsArray[0]);
  UA_UInt16 nsIdx_0 = nsArray[0].index;
  //Adding namespace for old namespace index = 2 with uri: http://acplt.org/identification/
  tempNsUri = UA_String_fromChars("http://acplt.org/identification/");
  UA_Namespace_init(&nsArray[1], &tempNsUri);
  UA_String_deleteMembers(&tempNsUri);
  retval |= UA_Server_addNamespace_full(server, &nsArray[1]);
  UA_UInt16 nsIdx_2 = nsArray[1].index;
  //Adding namespace for old namespace index = 3 with uri: http://acplt.org/lifeCycleEntry/
  tempNsUri = UA_String_fromChars("http://acplt.org/lifeCycleEntry/");
  UA_Namespace_init(&nsArray[2], &tempNsUri);
  UA_String_deleteMembers(&tempNsUri);
  retval |= UA_Server_addNamespace_full(server, &nsArray[2]);
  UA_UInt16 nsIdx_3 = nsArray[2].index;
  //Adding namespace for old namespace index = 4 with uri: http://acplt.org/propertyValueStatement/
  tempNsUri = UA_String_fromChars("http://acplt.org/propertyValueStatement/");
  UA_Namespace_init(&nsArray[3], &tempNsUri);
  UA_String_deleteMembers(&tempNsUri);
  retval |= UA_Server_addNamespace_full(server, &nsArray[3]);
  UA_UInt16 nsIdx_4 = nsArray[3].index;
  //Adding namespace for old namespace index = 5 with uri: http://acplt.org/openaas/
  tempNsUri = UA_String_fromChars("http://acplt.org/openaas/");
  UA_Namespace_init(&nsArray[4], &tempNsUri);
  UA_String_deleteMembers(&tempNsUri);
  retval |= UA_Server_addNamespace_full(server, &nsArray[4]);
  UA_UInt16 nsIdx_5 = nsArray[4].index;

  //Writing back desired namespace values')
  if(namespacesSize) {*namespacesSize = 5;};
  if(namespaces) {namespaces = &nsArray;}
  else {
    for(size_t i = 0 ; i < 5 ; ++i){
      UA_Namespace_deleteMembers(&nsArray[i]);
    }
    UA_free(nsArray);
  }
  if(retval == UA_STATUSCODE_GOOD){retval = UA_STATUSCODE_GOOD;} //ensure that retval is used
  

do {
// Referencing node found and declared as parent: i=19/StatusCode using i=45/HasSubtype
// Node: opcua_node_dataType_t(ns=5;i=3004), 1:AASStatusCode
UA_DataTypeAttributes attr;
UA_DataTypeAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "AASStatusCode");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 3004);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 19);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 45);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "AASStatusCode");
UA_Server_addDataTypeNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, NULL, NULL);
} while(0);
/*
do {
// Referencing node found and declared as parent: i=93/OPC Binary using i=47/HasComponent
// Node: opcua_node_variable_t(ns=5;i=6002), 1:TypeDictionary
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "TypeDictionary");
attr.description = UA_LOCALIZEDTEXT("", "Collects the data type descriptions of http://acplt.org/openaas/");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_ByteString *opcua_node_variable_t_ns_5_i_6002_variant_DataContents =  UA_ByteString_new();
*opcua_node_variable_t_ns_5_i_6002_variant_DataContents = UA_STRING_ALLOC("PG9wYzpUeXBlRGljdGlvbmFyeSB4bWxuczp4c2k9Imh0dHA6Ly93d3cudzMub3JnLzIwMDEvWE1MU2NoZ        W1hLWluc3RhbmNlIiB4bWxuczp0bnM9Imh0dHA6Ly95b3Vyb3JnYW5pc2F0aW9uLm9yZy9vc        GVuYWFzLyIgRGVmYXVsdEJ5dGVPcmRlcj0iTGl0dGxlRW5kaWFuIiB4bWxuczpvcGM9Imh0d        HA6Ly9vcGNmb3VuZGF0aW9uLm9yZy9CaW5hcnlTY2hlbWEvIiB4bWxuczp1YT0iaHR0cDovL        29wY2ZvdW5kYXRpb24ub3JnL1VBLyIgVGFyZ2V0TmFtZXNwYWNlPSJodHRwOi8veW91cm9yZ        2FuaXNhdGlvbi5vcmcvb3BlbmFhcy8iPgogPG9wYzpJbXBvcnQgTmFtZXNwYWNlPSJodHRwO        i8vb3BjZm91bmRhdGlvbi5vcmcvVUEvIi8+Cjwvb3BjOlR5cGVEaWN0aW9uYXJ5Pgo=");
UA_Variant_setScalar( &attr.value, opcua_node_variable_t_ns_5_i_6002_variant_DataContents, &UA_TYPES[UA_TYPES_BYTESTRING]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 6002);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 72);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 93);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "TypeDictionary");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6002), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 72), true);
} while(0);
*/
do {
// Referencing node found and declared as parent: ns=5;i=6002/1:TypeDictionary using i=46/HasProperty
// Node: opcua_node_variable_t(ns=5;i=6003), NamespaceUri
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "NamespaceUri");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_String *opcua_node_variable_t_ns_5_i_6003_variant_DataContents =  UA_String_new();
*opcua_node_variable_t_ns_5_i_6003_variant_DataContents = UA_STRING_ALLOC("http://acplt.org/openaas/");
UA_Variant_setScalar( &attr.value, opcua_node_variable_t_ns_5_i_6003_variant_DataContents, &UA_TYPES[UA_TYPES_STRING]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 6003);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_5, 6002);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(0, "NamespaceUri");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6003), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: i=92/XML Schema using i=47/HasComponent
// Node: opcua_node_variable_t(ns=5;i=6004), 1:TypeDictionary
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "TypeDictionary");
attr.description = UA_LOCALIZEDTEXT("", "Collects the data type descriptions of http://acplt.org/openaas/");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_ByteString *opcua_node_variable_t_ns_5_i_6004_variant_DataContents =  UA_ByteString_new();
*opcua_node_variable_t_ns_5_i_6004_variant_DataContents = UA_STRING_ALLOC("PHhzOnNjaGVtYSBlbGVtZW50Rm9ybURlZmF1bHQ9InF1YWxpZmllZCIgdGFyZ2V0TmFtZXNwYWNlPSJod        HRwOi8veW91cm9yZ2FuaXNhdGlvbi5vcmcvb3BlbmFhcy9UeXBlcy54c2QiIHhtbG5zOnRuc        z0iaHR0cDovL3lvdXJvcmdhbmlzYXRpb24ub3JnL29wZW5hYXMvVHlwZXMueHNkIiB4bWxuc        zp1YT0iaHR0cDovL29wY2ZvdW5kYXRpb24ub3JnL1VBLzIwMDgvMDIvVHlwZXMueHNkIiB4b        Wxuczp4cz0iaHR0cDovL3d3dy53My5vcmcvMjAwMS9YTUxTY2hlbWEiPgogPHhzOmltcG9yd        CBuYW1lc3BhY2U9Imh0dHA6Ly9vcGNmb3VuZGF0aW9uLm9yZy9VQS8yMDA4LzAyL1R5cGVzL        nhzZCIvPgo8L3hzOnNjaGVtYT4K");
UA_Variant_setScalar( &attr.value, opcua_node_variable_t_ns_5_i_6004_variant_DataContents, &UA_TYPES[UA_TYPES_BYTESTRING]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 6004);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 72);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 92);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "TypeDictionary");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6004), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 72), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=5;i=6004/1:TypeDictionary using i=46/HasProperty
// Node: opcua_node_variable_t(ns=5;i=6005), NamespaceUri
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "NamespaceUri");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_String *opcua_node_variable_t_ns_5_i_6005_variant_DataContents =  UA_String_new();
*opcua_node_variable_t_ns_5_i_6005_variant_DataContents = UA_STRING_ALLOC("http://acplt.org/openaas/Types.xsd");
UA_Variant_setScalar( &attr.value, opcua_node_variable_t_ns_5_i_6005_variant_DataContents, &UA_TYPES[UA_TYPES_STRING]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 6005);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_5, 6004);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(0, "NamespaceUri");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6005), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: i=58/BaseObjectType using i=45/HasSubtype
// Node: opcua_node_objectType_t(ns=5;i=1005), 1:AASFactoryType
UA_ObjectTypeAttributes attr;
UA_ObjectTypeAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "AASFactoryType");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 1005);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 58);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 45);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "AASFactoryType");
UA_Server_addObjectTypeNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, NULL, NULL);
} while(0);

do {
// Referencing node found and declared as parent: ns=5;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=5;i=7001), 1:createAAS
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 3);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<3; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("ID");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
inputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[1].name = UA_STRING("Name");
inputArguments[1].valueRank = -1;
inputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
inputArguments[2].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[2].name = UA_STRING("AssetId");
inputArguments[2].valueRank = -1;
inputArguments[2].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<1; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "createAAS");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 7001);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_5, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "createAAS");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 3, inputArguments,  1, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7001), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6017), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7001), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7001), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6018), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6017), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7001), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6018), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7001), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=5;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=5;i=7002), 1:createLCE
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 2);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<2; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("AASID");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
inputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[1].name = UA_STRING("LCE");
inputArguments[1].valueRank = -1;
inputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_3, 3002);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<1; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "createLCE");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 7002);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_5, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "createLCE");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 2, inputArguments,  1, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7002), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6019), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7002), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7002), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6020), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6019), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7002), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6020), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7002), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=5;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=5;i=7013), 1:createPVS
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 11);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<11; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("AAID");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
inputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[1].name = UA_STRING("ListID");
inputArguments[1].valueRank = -1;
inputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
inputArguments[2].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[2].name = UA_STRING("PVSName");
inputArguments[2].valueRank = -1;
inputArguments[2].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
inputArguments[3].description = UA_LOCALIZEDTEXT("en_US","A bit mask that indicates which fields are present in the stream. The mask has the following bits: 0x01: CarrierID; 0x02: ExpressionLogic; 0x04:ExpressionSemantic; 0x08:PropertyID; 0x10:View; 0x20:Visibility");
inputArguments[3].name = UA_STRING("Value");
inputArguments[3].valueRank = -1;
inputArguments[3].dataType = UA_NODEID_NUMERIC(nsIdx_0, 23);
inputArguments[4].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[4].name = UA_STRING("Mask");
inputArguments[4].valueRank = -1;
inputArguments[4].dataType = UA_NODEID_NUMERIC(nsIdx_0, 7);
inputArguments[5].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[5].name = UA_STRING("CarrierID");
inputArguments[5].valueRank = -1;
inputArguments[5].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
inputArguments[6].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[6].name = UA_STRING("ExpressionLogic");
inputArguments[6].valueRank = -1;
inputArguments[6].dataType = UA_NODEID_NUMERIC(nsIdx_4, 3002);
inputArguments[7].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[7].name = UA_STRING("ExpressionSemantic");
inputArguments[7].valueRank = -1;
inputArguments[7].dataType = UA_NODEID_NUMERIC(nsIdx_4, 3003);
inputArguments[8].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[8].name = UA_STRING("PropertyID");
inputArguments[8].valueRank = -1;
inputArguments[8].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
inputArguments[9].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[9].name = UA_STRING("View");
inputArguments[9].valueRank = -1;
inputArguments[9].dataType = UA_NODEID_NUMERIC(nsIdx_4, 3004);
inputArguments[10].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[10].name = UA_STRING("Visibility");
inputArguments[10].valueRank = -1;
inputArguments[10].dataType = UA_NODEID_NUMERIC(nsIdx_4, 3005);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<1; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "createPVS");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 7013);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_5, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "createPVS");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 11, inputArguments,  1, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7013), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6041), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7013), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7013), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6042), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6041), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7013), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6042), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7013), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=5;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=5;i=7011), 1:createPVSL
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 10);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<10; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("AASID");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
inputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[1].name = UA_STRING("ParentID");
inputArguments[1].valueRank = -1;
inputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
inputArguments[2].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[2].name = UA_STRING("PVSLName");
inputArguments[2].valueRank = -1;
inputArguments[2].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
inputArguments[3].description = UA_LOCALIZEDTEXT("en_US","A bit mask that indicates which fields are present in the stream. The mask has following bits: 0x01:CarrierID; 0x02:ExpressionLogic;0x04:ExpressionSemantic;0x08:PropertyID;0x10:View;0x20:Visibility");
inputArguments[3].name = UA_STRING("Mask");
inputArguments[3].valueRank = -1;
inputArguments[3].dataType = UA_NODEID_NUMERIC(nsIdx_0, 7);
inputArguments[4].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[4].name = UA_STRING("CarrierID");
inputArguments[4].valueRank = -1;
inputArguments[4].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
inputArguments[5].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[5].name = UA_STRING("ExpressionLogic");
inputArguments[5].valueRank = -1;
inputArguments[5].dataType = UA_NODEID_NUMERIC(nsIdx_4, 3002);
inputArguments[6].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[6].name = UA_STRING("ExpressionSemantic");
inputArguments[6].valueRank = -1;
inputArguments[6].dataType = UA_NODEID_NUMERIC(nsIdx_4, 3003);
inputArguments[7].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[7].name = UA_STRING("PropertyID");
inputArguments[7].valueRank = -1;
inputArguments[7].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
inputArguments[8].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[8].name = UA_STRING("View");
inputArguments[8].valueRank = -1;
inputArguments[8].dataType = UA_NODEID_NUMERIC(nsIdx_4, 3004);
inputArguments[9].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[9].name = UA_STRING("Visibility");
inputArguments[9].valueRank = -1;
inputArguments[9].dataType = UA_NODEID_NUMERIC(nsIdx_4, 3005);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<1; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "createPVSL");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 7011);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_5, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "createPVSL");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 10, inputArguments,  1, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7011), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6037), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7011), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7011), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6038), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6037), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7011), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6038), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7011), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=5;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=5;i=7009), 1:createSubModel
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 6);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<6; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("AASID");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
inputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[1].name = UA_STRING("ParentID");
inputArguments[1].valueRank = -1;
inputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
inputArguments[2].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[2].name = UA_STRING("ModelID");
inputArguments[2].valueRank = -1;
inputArguments[2].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
inputArguments[3].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[3].name = UA_STRING("Name");
inputArguments[3].valueRank = -1;
inputArguments[3].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
inputArguments[4].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[4].name = UA_STRING("Revision");
inputArguments[4].valueRank = -1;
inputArguments[4].dataType = UA_NODEID_NUMERIC(nsIdx_0, 7);
inputArguments[5].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[5].name = UA_STRING("Version");
inputArguments[5].valueRank = -1;
inputArguments[5].dataType = UA_NODEID_NUMERIC(nsIdx_0, 7);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<1; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("AASStatusCode");
outputArguments[0].valueRank = -1;
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "createSubModel");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 7009);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_5, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "createSubModel");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 6, inputArguments,  1, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7009), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6033), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7009), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7009), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6034), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6033), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7009), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6034), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7009), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=5;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=5;i=7003), 1:deleteAAS
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<1; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("ID");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<1; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "deleteAAS");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 7003);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_5, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "deleteAAS");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 1, inputArguments,  1, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7003), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6021), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7003), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7003), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6022), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6021), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7003), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6022), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7003), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=5;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=5;i=7004), 1:deleteLCE
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 2);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<2; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("AASID");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
inputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[1].name = UA_STRING("LCEID");
inputArguments[1].valueRank = -1;
inputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_0, 9);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<1; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "deleteLCE");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 7004);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_5, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "deleteLCE");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 2, inputArguments,  1, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7004), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6023), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7004), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7004), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6024), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6023), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7004), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6024), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7004), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=5;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=5;i=7014), 1:deletePVS
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 2);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<2; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("AASID");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
inputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[1].name = UA_STRING("ID");
inputArguments[1].valueRank = -1;
inputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<1; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "deletePVS");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 7014);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_5, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "deletePVS");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 2, inputArguments,  1, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7014), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6043), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7014), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7014), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6044), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6043), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7014), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6044), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7014), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=5;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=5;i=7012), 1:deletePVSL
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 2);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<2; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("AASID");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
inputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[1].name = UA_STRING("ID");
inputArguments[1].valueRank = -1;
inputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<1; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "deletePVSL");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 7012);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_5, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "deletePVSL");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 2, inputArguments,  1, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7012), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6039), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7012), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7012), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6040), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6039), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7012), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6040), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7012), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=5;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=5;i=7010), 1:deleteSubModel
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 2);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<2; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("AASID");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
inputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[1].name = UA_STRING("ID");
inputArguments[1].valueRank = -1;
inputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<1; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "deleteSubModel");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 7010);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_5, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "deleteSubModel");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 2, inputArguments,  1, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7010), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6035), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7010), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7010), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6036), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6035), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7010), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6036), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7010), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=5;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=5;i=7005), 1:getAASNodeId
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<1; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("AASID");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<1; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("AASNodeId");
outputArguments[0].valueRank = -1;
outputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_0, 17);
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "getAASNodeId");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 7005);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_5, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "getAASNodeId");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 1, inputArguments,  1, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7005), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6025), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7005), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7005), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6026), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6025), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7005), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6026), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7005), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=5;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=5;i=7006), 1:getLastLCEs
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 2);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<2; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("AASId");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
inputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[1].name = UA_STRING("Count");
inputArguments[1].valueRank = -1;
inputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_0, 7);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 3);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<3; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
outputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[1].name = UA_STRING("LastLCEs");
outputArguments[1].valueRank = 1;
outputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_3, 3002);
outputArguments[2].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[2].name = UA_STRING("LastLCEsCount");
outputArguments[2].valueRank = -1;
outputArguments[2].dataType = UA_NODEID_NUMERIC(nsIdx_0, 7);
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "getLastLCEs");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 7006);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_5, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "getLastLCEs");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 2, inputArguments,  3, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7006), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6027), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7006), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7006), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6028), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6027), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7006), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6028), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7006), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=5;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=5;i=7007), 1:getLCE
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 2);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<2; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("AASID");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
inputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[1].name = UA_STRING("LCEID");
inputArguments[1].valueRank = -1;
inputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_0, 9);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 2);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<2; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
outputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[1].name = UA_STRING("LCEData");
outputArguments[1].valueRank = -1;
outputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_3, 3002);
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "getLCE");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 7007);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_5, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "getLCE");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 2, inputArguments,  2, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7007), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6029), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7007), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7007), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6030), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6029), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7007), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6030), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7007), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=5;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=5;i=7018), 1:getLCESimple
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 2);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<2; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("AASID");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
inputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[1].name = UA_STRING("LCEID");
inputArguments[1].valueRank = -1;
inputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_0, 9);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 6);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<6; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
outputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[1].name = UA_STRING("CreatingInstance");
outputArguments[1].valueRank = -1;
outputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
outputArguments[2].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[2].name = UA_STRING("WritingInstance");
outputArguments[2].valueRank = -1;
outputArguments[2].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
outputArguments[3].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[3].name = UA_STRING("EventClass");
outputArguments[3].valueRank = -1;
outputArguments[3].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
outputArguments[4].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[4].name = UA_STRING("Subject");
outputArguments[4].valueRank = -1;
outputArguments[4].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
outputArguments[5].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[5].name = UA_STRING("Value");
outputArguments[5].valueRank = -1;
outputArguments[5].dataType = UA_NODEID_NUMERIC(nsIdx_0, 23);
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "getLCESimple");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 7018);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_5, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "getLCESimple");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 2, inputArguments,  6, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7018), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6051), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7018), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7018), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6052), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6051), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7018), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6052), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7018), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=5;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=5;i=7016), 1:getPVS
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 2);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<2; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("AASID");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
inputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[1].name = UA_STRING("ID");
inputArguments[1].valueRank = -1;
inputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 9);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<9; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
outputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[1].name = UA_STRING("PVSName");
outputArguments[1].valueRank = -1;
outputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
outputArguments[2].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[2].name = UA_STRING("CarrierID");
outputArguments[2].valueRank = -1;
outputArguments[2].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
outputArguments[3].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[3].name = UA_STRING("ExpressionLogic");
outputArguments[3].valueRank = -1;
outputArguments[3].dataType = UA_NODEID_NUMERIC(nsIdx_4, 3002);
outputArguments[4].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[4].name = UA_STRING("ExpressionSemantic");
outputArguments[4].valueRank = -1;
outputArguments[4].dataType = UA_NODEID_NUMERIC(nsIdx_4, 3003);
outputArguments[5].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[5].name = UA_STRING("PropertyID");
outputArguments[5].valueRank = -1;
outputArguments[5].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
outputArguments[6].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[6].name = UA_STRING("View");
outputArguments[6].valueRank = -1;
outputArguments[6].dataType = UA_NODEID_NUMERIC(nsIdx_4, 3004);
outputArguments[7].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[7].name = UA_STRING("Visibility");
outputArguments[7].valueRank = -1;
outputArguments[7].dataType = UA_NODEID_NUMERIC(nsIdx_4, 3005);
outputArguments[8].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[8].name = UA_STRING("Value");
outputArguments[8].valueRank = -1;
outputArguments[8].dataType = UA_NODEID_NUMERIC(nsIdx_0, 23);
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "getPVS");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 7016);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_5, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "getPVS");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 2, inputArguments,  9, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7016), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6047), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7016), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7016), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6048), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6047), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7016), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6048), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7016), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=5;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=5;i=7008), 1:setLCE
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 3);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<3; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("AASID");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
inputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[1].name = UA_STRING("LCEID");
inputArguments[1].valueRank = -1;
inputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_0, 9);
inputArguments[2].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[2].name = UA_STRING("LCEData");
inputArguments[2].valueRank = -1;
inputArguments[2].dataType = UA_NODEID_NUMERIC(nsIdx_3, 3002);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<1; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "setLCE");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 7008);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_5, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "setLCE");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 3, inputArguments,  1, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7008), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6031), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7008), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7008), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6032), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6031), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7008), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6032), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7008), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=5;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=5;i=7017), 1:setLCESimple
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 7);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<7; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("AASID");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
inputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[1].name = UA_STRING("LCEID");
inputArguments[1].valueRank = -1;
inputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_0, 9);
inputArguments[2].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[2].name = UA_STRING("CreatingInstance");
inputArguments[2].valueRank = -1;
inputArguments[2].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
inputArguments[3].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[3].name = UA_STRING("WritingInstance");
inputArguments[3].valueRank = -1;
inputArguments[3].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
inputArguments[4].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[4].name = UA_STRING("EventClass");
inputArguments[4].valueRank = -1;
inputArguments[4].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
inputArguments[5].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[5].name = UA_STRING("Subject");
inputArguments[5].valueRank = -1;
inputArguments[5].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
inputArguments[6].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[6].name = UA_STRING("Value");
inputArguments[6].valueRank = -1;
inputArguments[6].dataType = UA_NODEID_NUMERIC(nsIdx_0, 23);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<1; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "setLCESimple");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 7017);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_5, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "setLCESimple");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 7, inputArguments,  1, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7017), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6049), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7017), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7017), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6050), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6049), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7017), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6050), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7017), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=5;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=5;i=7015), 1:setPVS
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 11);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<11; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("AASID");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
inputArguments[1].description = UA_LOCALIZEDTEXT("en_US","A bit mask that indicates which fields are present in the stream. The mask has the following bits: 0x01:PVSName; 0x02:CarrierID; 0x04:ExpressionLogic; 0x08:ExpressionSemantic; 0x10PropertyID; 0x20:View; 0x40Visibility; 0x80:Value");
inputArguments[1].name = UA_STRING("ID");
inputArguments[1].valueRank = -1;
inputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
inputArguments[2].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[2].name = UA_STRING("Mask");
inputArguments[2].valueRank = -1;
inputArguments[2].dataType = UA_NODEID_NUMERIC(nsIdx_0, 7);
inputArguments[3].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[3].name = UA_STRING("PVSName");
inputArguments[3].valueRank = -1;
inputArguments[3].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
inputArguments[4].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[4].name = UA_STRING("CarrierID");
inputArguments[4].valueRank = -1;
inputArguments[4].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
inputArguments[5].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[5].name = UA_STRING("ExpressionLogic");
inputArguments[5].valueRank = -1;
inputArguments[5].dataType = UA_NODEID_NUMERIC(nsIdx_4, 3002);
inputArguments[6].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[6].name = UA_STRING("ExpressionSemantic");
inputArguments[6].valueRank = -1;
inputArguments[6].dataType = UA_NODEID_NUMERIC(nsIdx_4, 3003);
inputArguments[7].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[7].name = UA_STRING("PropertyID");
inputArguments[7].valueRank = -1;
inputArguments[7].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3002);
inputArguments[8].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[8].name = UA_STRING("View");
inputArguments[8].valueRank = -1;
inputArguments[8].dataType = UA_NODEID_NUMERIC(nsIdx_4, 3004);
inputArguments[9].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[9].name = UA_STRING("Visibility");
inputArguments[9].valueRank = -1;
inputArguments[9].dataType = UA_NODEID_NUMERIC(nsIdx_4, 3005);
inputArguments[10].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[10].name = UA_STRING("Value");
inputArguments[10].valueRank = -1;
inputArguments[10].dataType = UA_NODEID_NUMERIC(nsIdx_0, 23);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<1; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "setPVS");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 7015);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_5, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "setPVS");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 11, inputArguments,  1, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7015), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6045), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7015), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 7015), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 6046), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6045), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7015), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6046), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_5, 7015), false);
} while(0);

do {
// Referencing node found and declared as parent: i=58/BaseObjectType using i=45/HasSubtype
// Node: opcua_node_objectType_t(ns=5;i=1002), 1:AssetAdminitrationShellType
UA_ObjectTypeAttributes attr;
UA_ObjectTypeAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "AssetAdminitrationShellType");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 1002);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 58);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 45);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "AssetAdminitrationShellType");
UA_Server_addObjectTypeNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, NULL, NULL);
} while(0);

do {
// Referencing node found and declared as parent: ns=5;i=1002/1:AssetAdminitrationShellType using i=47/HasComponent
// Node: opcua_node_object_t(ns=5;i=5001), 1:Body
UA_ObjectAttributes attr;
UA_ObjectAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Body");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 5001);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 61);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_5, 1002);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "Body");
UA_Server_addObjectNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 58), true); //remove HasTypeDefinition refs generated by addObjectNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 5001), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 61), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 5001), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=5;i=1002/1:AssetAdminitrationShellType using i=47/HasComponent
// Node: opcua_node_object_t(ns=5;i=5002), 1:Header
UA_ObjectAttributes attr;
UA_ObjectAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Header");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 5002);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_4, 1002);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_5, 1002);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "Header");
UA_Server_addObjectNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 58), true); //remove HasTypeDefinition refs generated by addObjectNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 5002), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 5002), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_4, 1002), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=5;i=5002/1:Header using i=47/HasComponent
// Node: opcua_node_variable_t(ns=5;i=6001), 4:PropertyValueStatements
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "PropertyValueStatements");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 6001);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_4, 2002);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_5, 5002);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "PropertyValueStatements");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6001), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 11510), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6001), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_4, 2002), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=5;i=6001/4:PropertyValueStatements using i=46/HasProperty
// Node: opcua_node_variable_t(ns=5;i=6006), 4:Value
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Value");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 6006);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_5, 6001);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "Value");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6006), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6006), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=5;i=1002/1:AssetAdminitrationShellType using i=47/HasComponent
// Node: opcua_node_object_t(ns=5;i=5003), 1:LifeCycleArchive
UA_ObjectAttributes attr;
UA_ObjectAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "LifeCycleArchive");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 5003);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_3, 1002);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_5, 1002);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "LifeCycleArchive");
UA_Server_addObjectNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 58), true); //remove HasTypeDefinition refs generated by addObjectNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 5003), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 1002), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 5003), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=5;i=5003/1:LifeCycleArchive using i=47/HasComponent
// Node: opcua_node_variable_t(ns=5;i=6007), 3:LifeCycleEntries
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "LifeCycleEntries");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 6007);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 63);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_5, 5003);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "LifeCycleEntries");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6007), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 63), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6007), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 11510), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=5;i=1002/1:AssetAdminitrationShellType using i=47/HasComponent
// Node: opcua_node_object_t(ns=5;i=5004), 1:Views
UA_ObjectAttributes attr;
UA_ObjectAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Views");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 5004);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 61);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_5, 1002);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "Views");
UA_Server_addObjectNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 58), true); //remove HasTypeDefinition refs generated by addObjectNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 5004), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 61), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 5004), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 80), true);
} while(0);

do {
// Referencing node found and declared as parent: i=58/BaseObjectType using i=45/HasSubtype
// Node: opcua_node_objectType_t(ns=5;i=1003), 1:SubModelType
UA_ObjectTypeAttributes attr;
UA_ObjectTypeAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "SubModelType");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 1003);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 58);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 45);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "SubModelType");
UA_Server_addObjectTypeNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, NULL, NULL);
} while(0);

do {
// Referencing node found and declared as parent: ns=5;i=1003/1:SubModelType using i=46/HasProperty
// Node: opcua_node_variable_t(ns=5;i=6008), 1:ModelID
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "ModelID");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 6008);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_5, 1003);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "ModelID");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6008), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6008), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=5;i=1003/1:SubModelType using i=46/HasProperty
// Node: opcua_node_variable_t(ns=5;i=6010), 1:Revision
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Revision");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 6010);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_5, 1003);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "Revision");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6010), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6010), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=5;i=1003/1:SubModelType using i=46/HasProperty
// Node: opcua_node_variable_t(ns=5;i=6011), 1:Version
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Version");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_5, 6011);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_5, 1003);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_5, "Version");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6011), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_5, 6011), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);
return UA_STATUSCODE_GOOD;
}
