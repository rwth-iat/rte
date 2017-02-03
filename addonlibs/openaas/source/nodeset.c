/* WARNING: This is a generated file.
 * Any manual changes will be overwritten.

 */
#include "nodeset.h"
UA_INLINE UA_StatusCode nodeset(UA_Server *server){
 return nodeset_returnIndices(server, NULL, NULL, NULL, NULL);
}

UA_INLINE UA_StatusCode nodeset_returnIndices(UA_Server *server, UA_UInt16 *namespacesSize, UA_UInt16 **oldNameSpaceIndices,
    UA_UInt16 **newNameSpaceIndices, UA_String **nameSpaceUri) {
  UA_UInt16* oldNsIdx = UA_malloc(2*sizeof(UA_UInt16));
  UA_UInt16* newNsIdx = UA_malloc(2*sizeof(UA_UInt16));
  UA_String* nsUri = UA_malloc(2*sizeof(UA_String*));

  //Adding namespace for old namespace index = 0 with uri: http://opcfoundation.org/UA/
  UA_UInt16 nsIdx_0 = UA_Server_addNamespace(server, "http://opcfoundation.org/UA/");
  oldNsIdx[0] = 0;
  newNsIdx[0] = nsIdx_0;
  if(nameSpaceUri) nsUri[0] = UA_STRING_ALLOC("http://opcfoundation.org/UA/");

  //Adding namespace for old namespace index = 2 with uri: http://openAAS.org/AAS/
  UA_UInt16 nsIdx_2 = UA_Server_addNamespace(server, "http://openAAS.org/AAS/");
  oldNsIdx[1] = 2;
  newNsIdx[1] = nsIdx_2;
  if(nameSpaceUri) nsUri[1] = UA_STRING_ALLOC("http://openAAS.org/AAS/");

  //Writing back desired new and old namespace values
  if(namespacesSize) *namespacesSize = 2;
  if(oldNameSpaceIndices) *oldNameSpaceIndices = oldNsIdx;
  if(newNameSpaceIndices) *newNameSpaceIndices = newNsIdx;
  if(nameSpaceUri) *nameSpaceUri = nsUri;
  UA_StatusCode retval = UA_STATUSCODE_GOOD; 
  if(retval == UA_STATUSCODE_GOOD){retval = UA_STATUSCODE_GOOD;} //ensure that retval is used

do {
// Referencing node found and declared as parent: i=29/Enumeration using i=45/HasSubtype
// Node: opcua_node_dataType_t(ns=2;i=3004), 1:ExpressionSemanticEnum
UA_DataTypeAttributes attr;
UA_DataTypeAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "ExpressionSemanticEnum");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 3004);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 29);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 45);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "ExpressionSemanticEnum");
UA_Server_addDataTypeNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, NULL, NULL);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=3004/1:ExpressionSemanticEnum using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6007), EnumValues
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "EnumValues");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = 1;

struct {
  UA_Int64 Value;
  UA_LocalizedText DisplayName;
  UA_LocalizedText Description;
} opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0_struct;
opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0_struct.Value = (UA_Int64) 0;
opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0_struct.DisplayName = UA_LOCALIZEDTEXT("en_US", "ASSURANCE");
opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0_struct.Description = UA_LOCALIZEDTEXT("en_US", "Aussage ist eine Zusicherung");
UA_ExtensionObject *opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0 =  UA_ExtensionObject_new();
opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0->encoding = UA_EXTENSIONOBJECT_ENCODED_BYTESTRING;
opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0->content.encoded.typeId = UA_NODEID_NUMERIC(0, 7594+ UA_ENCODINGOFFSET_BINARY);
if(UA_ByteString_allocBuffer(&opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0->content.encoded.body, 65000) != UA_STATUSCODE_GOOD) {}
size_t opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0_encOffset = 0;
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0_struct.Value, &UA_TYPES[UA_TYPES_INT64], NULL, NULL, &opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0_encOffset);
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0_struct.DisplayName, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT], NULL, NULL, &opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0_encOffset);
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0_struct.Description, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT], NULL, NULL, &opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0_encOffset);
opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0->content.encoded.body.length = opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0_encOffset;
UA_Byte *opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0_newBody = (UA_Byte *) UA_malloc(opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0_encOffset );
memcpy(opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0_newBody, opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0->content.encoded.body.data, opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0_encOffset);
UA_Byte *opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0_oldBody = opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0->content.encoded.body.data;
opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0->content.encoded.body.data = opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0_newBody;
UA_free(opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0_oldBody);


struct {
  UA_Int64 Value;
  UA_LocalizedText DisplayName;
  UA_LocalizedText Description;
} opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0_struct;
opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0_struct.Value = (UA_Int64) 1;
opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0_struct.DisplayName = UA_LOCALIZEDTEXT("en_US", "SETTING");
opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0_struct.Description = UA_LOCALIZEDTEXT("en_US", "Aussage ist ein Setting (z.B. Parameter)");
UA_ExtensionObject *opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0 =  UA_ExtensionObject_new();
opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0->encoding = UA_EXTENSIONOBJECT_ENCODED_BYTESTRING;
opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0->content.encoded.typeId = UA_NODEID_NUMERIC(0, 7594+ UA_ENCODINGOFFSET_BINARY);
if(UA_ByteString_allocBuffer(&opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0->content.encoded.body, 65000) != UA_STATUSCODE_GOOD) {}
size_t opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0_encOffset = 0;
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0_struct.Value, &UA_TYPES[UA_TYPES_INT64], NULL, NULL, &opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0_encOffset);
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0_struct.DisplayName, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT], NULL, NULL, &opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0_encOffset);
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0_struct.Description, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT], NULL, NULL, &opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0_encOffset);
opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0->content.encoded.body.length = opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0_encOffset;
UA_Byte *opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0_newBody = (UA_Byte *) UA_malloc(opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0_encOffset );
memcpy(opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0_newBody, opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0->content.encoded.body.data, opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0_encOffset);
UA_Byte *opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0_oldBody = opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0->content.encoded.body.data;
opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0->content.encoded.body.data = opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0_newBody;
UA_free(opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0_oldBody);


struct {
  UA_Int64 Value;
  UA_LocalizedText DisplayName;
  UA_LocalizedText Description;
} opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0_struct;
opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0_struct.Value = (UA_Int64) 2;
opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0_struct.DisplayName = UA_LOCALIZEDTEXT("en_US", "MEASUREMENT");
opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0_struct.Description = UA_LOCALIZEDTEXT("en_US", "Aussage ist ein Messwert");
UA_ExtensionObject *opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0 =  UA_ExtensionObject_new();
opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0->encoding = UA_EXTENSIONOBJECT_ENCODED_BYTESTRING;
opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0->content.encoded.typeId = UA_NODEID_NUMERIC(0, 7594+ UA_ENCODINGOFFSET_BINARY);
if(UA_ByteString_allocBuffer(&opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0->content.encoded.body, 65000) != UA_STATUSCODE_GOOD) {}
size_t opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0_encOffset = 0;
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0_struct.Value, &UA_TYPES[UA_TYPES_INT64], NULL, NULL, &opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0_encOffset);
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0_struct.DisplayName, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT], NULL, NULL, &opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0_encOffset);
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0_struct.Description, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT], NULL, NULL, &opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0_encOffset);
opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0->content.encoded.body.length = opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0_encOffset;
UA_Byte *opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0_newBody = (UA_Byte *) UA_malloc(opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0_encOffset );
memcpy(opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0_newBody, opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0->content.encoded.body.data, opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0_encOffset);
UA_Byte *opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0_oldBody = opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0->content.encoded.body.data;
opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0->content.encoded.body.data = opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0_newBody;
UA_free(opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0_oldBody);


struct {
  UA_Int64 Value;
  UA_LocalizedText DisplayName;
  UA_LocalizedText Description;
} opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0_struct;
opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0_struct.Value = (UA_Int64) 3;
opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0_struct.DisplayName = UA_LOCALIZEDTEXT("en_US", "REQUIREMENT");
opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0_struct.Description = UA_LOCALIZEDTEXT("en_US", "Aussage ist eine Anforderung");
UA_ExtensionObject *opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0 =  UA_ExtensionObject_new();
opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0->encoding = UA_EXTENSIONOBJECT_ENCODED_BYTESTRING;
opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0->content.encoded.typeId = UA_NODEID_NUMERIC(0, 7594+ UA_ENCODINGOFFSET_BINARY);
if(UA_ByteString_allocBuffer(&opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0->content.encoded.body, 65000) != UA_STATUSCODE_GOOD) {}
size_t opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0_encOffset = 0;
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0_struct.Value, &UA_TYPES[UA_TYPES_INT64], NULL, NULL, &opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0_encOffset);
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0_struct.DisplayName, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT], NULL, NULL, &opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0_encOffset);
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0_struct.Description, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT], NULL, NULL, &opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0_encOffset);
opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0->content.encoded.body.length = opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0_encOffset;
UA_Byte *opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0_newBody = (UA_Byte *) UA_malloc(opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0_encOffset );
memcpy(opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0_newBody, opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0->content.encoded.body.data, opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0_encOffset);
UA_Byte *opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0_oldBody = opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0->content.encoded.body.data;
opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0->content.encoded.body.data = opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0_newBody;
UA_free(opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0_oldBody);

UA_ExtensionObject opcua_node_variable_t_ns_2_i_6007_variant_DataContents[4];
opcua_node_variable_t_ns_2_i_6007_variant_DataContents[0] = *opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0;
UA_free(opcua_node_variable_t_ns_2_i_6007_EnumValueType_0_0);
opcua_node_variable_t_ns_2_i_6007_variant_DataContents[1] = *opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0;
UA_free(opcua_node_variable_t_ns_2_i_6007_EnumValueType_1_0);
opcua_node_variable_t_ns_2_i_6007_variant_DataContents[2] = *opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0;
UA_free(opcua_node_variable_t_ns_2_i_6007_EnumValueType_2_0);
opcua_node_variable_t_ns_2_i_6007_variant_DataContents[3] = *opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0;
UA_free(opcua_node_variable_t_ns_2_i_6007_EnumValueType_3_0);
UA_Variant_setArray( &attr.value, &opcua_node_variable_t_ns_2_i_6007_variant_DataContents, (UA_Int32) 4, &UA_TYPES[UA_TYPES_EXTENSIONOBJECT]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6007);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 3004);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(0, "EnumValues");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6007), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6007), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: i=29/Enumeration using i=45/HasSubtype
// Node: opcua_node_dataType_t(ns=2;i=3002), 1:IdEnum
UA_DataTypeAttributes attr;
UA_DataTypeAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "IdEnum");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 3002);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 29);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 45);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "IdEnum");
UA_Server_addDataTypeNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, NULL, NULL);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=3002/1:IdEnum using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6005), EnumValues
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "EnumValues");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = 1;

struct {
  UA_Int64 Value;
  UA_LocalizedText DisplayName;
  UA_LocalizedText Description;
} opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0_struct;
opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0_struct.Value = (UA_Int64) 0;
opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0_struct.DisplayName = UA_LOCALIZEDTEXT("en_US", "URI");
opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0_struct.Description = UA_LOCALIZEDTEXT("en_US", "Unique Resource Identifier");
UA_ExtensionObject *opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0 =  UA_ExtensionObject_new();
opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0->encoding = UA_EXTENSIONOBJECT_ENCODED_BYTESTRING;
opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0->content.encoded.typeId = UA_NODEID_NUMERIC(0, 7594+ UA_ENCODINGOFFSET_BINARY);
if(UA_ByteString_allocBuffer(&opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0->content.encoded.body, 65000) != UA_STATUSCODE_GOOD) {}
size_t opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0_encOffset = 0;
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0_struct.Value, &UA_TYPES[UA_TYPES_INT64], NULL, NULL, &opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0_encOffset);
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0_struct.DisplayName, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT], NULL, NULL, &opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0_encOffset);
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0_struct.Description, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT], NULL, NULL, &opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0_encOffset);
opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0->content.encoded.body.length = opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0_encOffset;
UA_Byte *opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0_newBody = (UA_Byte *) UA_malloc(opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0_encOffset );
memcpy(opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0_newBody, opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0->content.encoded.body.data, opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0_encOffset);
UA_Byte *opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0_oldBody = opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0->content.encoded.body.data;
opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0->content.encoded.body.data = opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0_newBody;
UA_free(opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0_oldBody);


struct {
  UA_Int64 Value;
  UA_LocalizedText DisplayName;
  UA_LocalizedText Description;
} opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0_struct;
opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0_struct.Value = (UA_Int64) 1;
opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0_struct.DisplayName = UA_LOCALIZEDTEXT("en_US", "ISO");
opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0_struct.Description = UA_LOCALIZEDTEXT("en_US", "ISO 29002-5 Identifier");
UA_ExtensionObject *opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0 =  UA_ExtensionObject_new();
opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0->encoding = UA_EXTENSIONOBJECT_ENCODED_BYTESTRING;
opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0->content.encoded.typeId = UA_NODEID_NUMERIC(0, 7594+ UA_ENCODINGOFFSET_BINARY);
if(UA_ByteString_allocBuffer(&opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0->content.encoded.body, 65000) != UA_STATUSCODE_GOOD) {}
size_t opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0_encOffset = 0;
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0_struct.Value, &UA_TYPES[UA_TYPES_INT64], NULL, NULL, &opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0_encOffset);
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0_struct.DisplayName, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT], NULL, NULL, &opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0_encOffset);
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0_struct.Description, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT], NULL, NULL, &opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0_encOffset);
opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0->content.encoded.body.length = opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0_encOffset;
UA_Byte *opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0_newBody = (UA_Byte *) UA_malloc(opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0_encOffset );
memcpy(opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0_newBody, opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0->content.encoded.body.data, opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0_encOffset);
UA_Byte *opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0_oldBody = opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0->content.encoded.body.data;
opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0->content.encoded.body.data = opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0_newBody;
UA_free(opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0_oldBody);

UA_ExtensionObject opcua_node_variable_t_ns_2_i_6005_variant_DataContents[2];
opcua_node_variable_t_ns_2_i_6005_variant_DataContents[0] = *opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0;
UA_free(opcua_node_variable_t_ns_2_i_6005_EnumValueType_0_0);
opcua_node_variable_t_ns_2_i_6005_variant_DataContents[1] = *opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0;
UA_free(opcua_node_variable_t_ns_2_i_6005_EnumValueType_1_0);
UA_Variant_setArray( &attr.value, &opcua_node_variable_t_ns_2_i_6005_variant_DataContents, (UA_Int32) 2, &UA_TYPES[UA_TYPES_EXTENSIONOBJECT]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6005);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 3002);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(0, "EnumValues");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6005), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6005), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: i=29/Enumeration using i=45/HasSubtype
// Node: opcua_node_dataType_t(ns=2;i=3003), 1:RelationalExpressionEnum
UA_DataTypeAttributes attr;
UA_DataTypeAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "RelationalExpressionEnum");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 3003);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 29);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 45);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "RelationalExpressionEnum");
UA_Server_addDataTypeNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, NULL, NULL);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=3003/1:RelationalExpressionEnum using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6006), EnumValues
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "EnumValues");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = 1;

struct {
  UA_Int64 Value;
  UA_LocalizedText DisplayName;
  UA_LocalizedText Description;
} opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0_struct;
opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0_struct.Value = (UA_Int64) 0;
opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0_struct.DisplayName = UA_LOCALIZEDTEXT("en_US", "GREATER_THAN");
opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0_struct.Description = UA_LOCALIZEDTEXT("en_US", "Wert ist größer als Ausprägungsaussage");
UA_ExtensionObject *opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0 =  UA_ExtensionObject_new();
opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0->encoding = UA_EXTENSIONOBJECT_ENCODED_BYTESTRING;
opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0->content.encoded.typeId = UA_NODEID_NUMERIC(0, 7594+ UA_ENCODINGOFFSET_BINARY);
if(UA_ByteString_allocBuffer(&opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0->content.encoded.body, 65000) != UA_STATUSCODE_GOOD) {}
size_t opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0_encOffset = 0;
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0_struct.Value, &UA_TYPES[UA_TYPES_INT64], NULL, NULL, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0_encOffset);
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0_struct.DisplayName, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT], NULL, NULL, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0_encOffset);
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0_struct.Description, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT], NULL, NULL, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0_encOffset);
opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0->content.encoded.body.length = opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0_encOffset;
UA_Byte *opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0_newBody = (UA_Byte *) UA_malloc(opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0_encOffset );
memcpy(opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0_newBody, opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0->content.encoded.body.data, opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0_encOffset);
UA_Byte *opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0_oldBody = opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0->content.encoded.body.data;
opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0->content.encoded.body.data = opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0_newBody;
UA_free(opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0_oldBody);


struct {
  UA_Int64 Value;
  UA_LocalizedText DisplayName;
  UA_LocalizedText Description;
} opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0_struct;
opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0_struct.Value = (UA_Int64) 1;
opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0_struct.DisplayName = UA_LOCALIZEDTEXT("en_US", "GREATER_EQUAL");
opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0_struct.Description = UA_LOCALIZEDTEXT("en_US", "Wert ist größer/gleich als Ausprägungsaussage");
UA_ExtensionObject *opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0 =  UA_ExtensionObject_new();
opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0->encoding = UA_EXTENSIONOBJECT_ENCODED_BYTESTRING;
opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0->content.encoded.typeId = UA_NODEID_NUMERIC(0, 7594+ UA_ENCODINGOFFSET_BINARY);
if(UA_ByteString_allocBuffer(&opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0->content.encoded.body, 65000) != UA_STATUSCODE_GOOD) {}
size_t opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0_encOffset = 0;
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0_struct.Value, &UA_TYPES[UA_TYPES_INT64], NULL, NULL, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0_encOffset);
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0_struct.DisplayName, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT], NULL, NULL, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0_encOffset);
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0_struct.Description, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT], NULL, NULL, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0_encOffset);
opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0->content.encoded.body.length = opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0_encOffset;
UA_Byte *opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0_newBody = (UA_Byte *) UA_malloc(opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0_encOffset );
memcpy(opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0_newBody, opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0->content.encoded.body.data, opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0_encOffset);
UA_Byte *opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0_oldBody = opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0->content.encoded.body.data;
opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0->content.encoded.body.data = opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0_newBody;
UA_free(opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0_oldBody);


struct {
  UA_Int64 Value;
  UA_LocalizedText DisplayName;
  UA_LocalizedText Description;
} opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0_struct;
opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0_struct.Value = (UA_Int64) 2;
opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0_struct.DisplayName = UA_LOCALIZEDTEXT("en_US", "EQUAL");
opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0_struct.Description = UA_LOCALIZEDTEXT("en_US", "Wert ist gleich als Ausprägungsaussage");
UA_ExtensionObject *opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0 =  UA_ExtensionObject_new();
opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0->encoding = UA_EXTENSIONOBJECT_ENCODED_BYTESTRING;
opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0->content.encoded.typeId = UA_NODEID_NUMERIC(0, 7594+ UA_ENCODINGOFFSET_BINARY);
if(UA_ByteString_allocBuffer(&opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0->content.encoded.body, 65000) != UA_STATUSCODE_GOOD) {}
size_t opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0_encOffset = 0;
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0_struct.Value, &UA_TYPES[UA_TYPES_INT64], NULL, NULL, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0_encOffset);
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0_struct.DisplayName, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT], NULL, NULL, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0_encOffset);
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0_struct.Description, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT], NULL, NULL, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0_encOffset);
opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0->content.encoded.body.length = opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0_encOffset;
UA_Byte *opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0_newBody = (UA_Byte *) UA_malloc(opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0_encOffset );
memcpy(opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0_newBody, opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0->content.encoded.body.data, opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0_encOffset);
UA_Byte *opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0_oldBody = opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0->content.encoded.body.data;
opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0->content.encoded.body.data = opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0_newBody;
UA_free(opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0_oldBody);


struct {
  UA_Int64 Value;
  UA_LocalizedText DisplayName;
  UA_LocalizedText Description;
} opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0_struct;
opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0_struct.Value = (UA_Int64) 3;
opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0_struct.DisplayName = UA_LOCALIZEDTEXT("en_US", "NOT_EQUAL");
opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0_struct.Description = UA_LOCALIZEDTEXT("en_US", "Wert ist nicht gleich der Ausprägungsaussage");
UA_ExtensionObject *opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0 =  UA_ExtensionObject_new();
opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0->encoding = UA_EXTENSIONOBJECT_ENCODED_BYTESTRING;
opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0->content.encoded.typeId = UA_NODEID_NUMERIC(0, 7594+ UA_ENCODINGOFFSET_BINARY);
if(UA_ByteString_allocBuffer(&opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0->content.encoded.body, 65000) != UA_STATUSCODE_GOOD) {}
size_t opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0_encOffset = 0;
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0_struct.Value, &UA_TYPES[UA_TYPES_INT64], NULL, NULL, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0_encOffset);
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0_struct.DisplayName, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT], NULL, NULL, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0_encOffset);
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0_struct.Description, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT], NULL, NULL, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0_encOffset);
opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0->content.encoded.body.length = opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0_encOffset;
UA_Byte *opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0_newBody = (UA_Byte *) UA_malloc(opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0_encOffset );
memcpy(opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0_newBody, opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0->content.encoded.body.data, opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0_encOffset);
UA_Byte *opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0_oldBody = opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0->content.encoded.body.data;
opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0->content.encoded.body.data = opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0_newBody;
UA_free(opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0_oldBody);


struct {
  UA_Int64 Value;
  UA_LocalizedText DisplayName;
  UA_LocalizedText Description;
} opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0_struct;
opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0_struct.Value = (UA_Int64) 4;
opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0_struct.DisplayName = UA_LOCALIZEDTEXT("en_US", "LESS_EQUAL");
opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0_struct.Description = UA_LOCALIZEDTEXT("en_US", "Wert ist kleiner/gleich als Ausprägungsaussage");
UA_ExtensionObject *opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0 =  UA_ExtensionObject_new();
opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0->encoding = UA_EXTENSIONOBJECT_ENCODED_BYTESTRING;
opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0->content.encoded.typeId = UA_NODEID_NUMERIC(0, 7594+ UA_ENCODINGOFFSET_BINARY);
if(UA_ByteString_allocBuffer(&opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0->content.encoded.body, 65000) != UA_STATUSCODE_GOOD) {}
size_t opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0_encOffset = 0;
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0_struct.Value, &UA_TYPES[UA_TYPES_INT64], NULL, NULL, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0_encOffset);
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0_struct.DisplayName, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT], NULL, NULL, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0_encOffset);
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0_struct.Description, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT], NULL, NULL, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0_encOffset);
opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0->content.encoded.body.length = opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0_encOffset;
UA_Byte *opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0_newBody = (UA_Byte *) UA_malloc(opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0_encOffset );
memcpy(opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0_newBody, opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0->content.encoded.body.data, opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0_encOffset);
UA_Byte *opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0_oldBody = opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0->content.encoded.body.data;
opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0->content.encoded.body.data = opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0_newBody;
UA_free(opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0_oldBody);


struct {
  UA_Int64 Value;
  UA_LocalizedText DisplayName;
  UA_LocalizedText Description;
} opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0_struct;
opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0_struct.Value = (UA_Int64) 5;
opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0_struct.DisplayName = UA_LOCALIZEDTEXT("en_US", "LESS_THAN");
opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0_struct.Description = UA_LOCALIZEDTEXT("en_US", "Wert ist kleiner als Ausprägungsaussage");
UA_ExtensionObject *opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0 =  UA_ExtensionObject_new();
opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0->encoding = UA_EXTENSIONOBJECT_ENCODED_BYTESTRING;
opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0->content.encoded.typeId = UA_NODEID_NUMERIC(0, 7594+ UA_ENCODINGOFFSET_BINARY);
if(UA_ByteString_allocBuffer(&opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0->content.encoded.body, 65000) != UA_STATUSCODE_GOOD) {}
size_t opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0_encOffset = 0;
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0_struct.Value, &UA_TYPES[UA_TYPES_INT64], NULL, NULL, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0_encOffset);
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0_struct.DisplayName, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT], NULL, NULL, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0_encOffset);
retval |= UA_encodeBinary(&opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0_struct.Description, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT], NULL, NULL, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0->content.encoded.body, &opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0_encOffset);
opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0->content.encoded.body.length = opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0_encOffset;
UA_Byte *opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0_newBody = (UA_Byte *) UA_malloc(opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0_encOffset );
memcpy(opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0_newBody, opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0->content.encoded.body.data, opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0_encOffset);
UA_Byte *opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0_oldBody = opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0->content.encoded.body.data;
opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0->content.encoded.body.data = opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0_newBody;
UA_free(opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0_oldBody);

UA_ExtensionObject opcua_node_variable_t_ns_2_i_6006_variant_DataContents[6];
opcua_node_variable_t_ns_2_i_6006_variant_DataContents[0] = *opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0;
UA_free(opcua_node_variable_t_ns_2_i_6006_EnumValueType_0_0);
opcua_node_variable_t_ns_2_i_6006_variant_DataContents[1] = *opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0;
UA_free(opcua_node_variable_t_ns_2_i_6006_EnumValueType_1_0);
opcua_node_variable_t_ns_2_i_6006_variant_DataContents[2] = *opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0;
UA_free(opcua_node_variable_t_ns_2_i_6006_EnumValueType_2_0);
opcua_node_variable_t_ns_2_i_6006_variant_DataContents[3] = *opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0;
UA_free(opcua_node_variable_t_ns_2_i_6006_EnumValueType_3_0);
opcua_node_variable_t_ns_2_i_6006_variant_DataContents[4] = *opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0;
UA_free(opcua_node_variable_t_ns_2_i_6006_EnumValueType_4_0);
opcua_node_variable_t_ns_2_i_6006_variant_DataContents[5] = *opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0;
UA_free(opcua_node_variable_t_ns_2_i_6006_EnumValueType_5_0);
UA_Variant_setArray( &attr.value, &opcua_node_variable_t_ns_2_i_6006_variant_DataContents, (UA_Int32) 6, &UA_TYPES[UA_TYPES_EXTENSIONOBJECT]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6006);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 3003);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(0, "EnumValues");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6006), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6006), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: i=29/Enumeration using i=45/HasSubtype
// Node: opcua_node_dataType_t(ns=2;i=3007), 1:ViewEnum
UA_DataTypeAttributes attr;
UA_DataTypeAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "ViewEnum");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 3007);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 29);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 45);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "ViewEnum");
UA_Server_addDataTypeNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, NULL, NULL);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=3007/1:ViewEnum using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6028), EnumStrings
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "EnumStrings");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = 1;
UA_LocalizedText opcua_node_variable_t_ns_2_i_6028_variant_DataContents[9];
opcua_node_variable_t_ns_2_i_6028_variant_DataContents[0] = UA_LOCALIZEDTEXT_ALLOC("en_US", "BUSINESS");
opcua_node_variable_t_ns_2_i_6028_variant_DataContents[1] = UA_LOCALIZEDTEXT_ALLOC("en_US", "CONSTRUCTION");
opcua_node_variable_t_ns_2_i_6028_variant_DataContents[2] = UA_LOCALIZEDTEXT_ALLOC("en_US", "POWER");
opcua_node_variable_t_ns_2_i_6028_variant_DataContents[3] = UA_LOCALIZEDTEXT_ALLOC("en_US", "FUNCTIONAL");
opcua_node_variable_t_ns_2_i_6028_variant_DataContents[4] = UA_LOCALIZEDTEXT_ALLOC("en_US", "LOCATION");
opcua_node_variable_t_ns_2_i_6028_variant_DataContents[5] = UA_LOCALIZEDTEXT_ALLOC("en_US", "SECURITY");
opcua_node_variable_t_ns_2_i_6028_variant_DataContents[6] = UA_LOCALIZEDTEXT_ALLOC("en_US", "NETWORK");
opcua_node_variable_t_ns_2_i_6028_variant_DataContents[7] = UA_LOCALIZEDTEXT_ALLOC("en_US", "LIFECYCLE");
opcua_node_variable_t_ns_2_i_6028_variant_DataContents[8] = UA_LOCALIZEDTEXT_ALLOC("en_US", "HUMAN");
UA_Variant_setArray( &attr.value, &opcua_node_variable_t_ns_2_i_6028_variant_DataContents, (UA_Int32) 9, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6028);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 3007);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(0, "EnumStrings");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6028), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6028), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: i=19/StatusCode using i=45/HasSubtype
// Node: opcua_node_dataType_t(ns=2;i=3009), 1:AASStatusCode
UA_DataTypeAttributes attr;
UA_DataTypeAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "AASStatusCode");
attr.description = UA_LOCALIZEDTEXT("", "Status code for Asset Administration Shell operations.");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 3009);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 19);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 45);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "AASStatusCode");
UA_Server_addDataTypeNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, NULL, NULL);
} while(0);

do {
// Referencing node found and declared as parent: i=22/Structure using i=45/HasSubtype
// Node: opcua_node_dataType_t(ns=2;i=3005), 1:Identification
UA_DataTypeAttributes attr;
UA_DataTypeAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Identification");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 3005);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 22);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 45);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "Identification");
UA_Server_addDataTypeNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, NULL, NULL);
} while(0);

do {
// Referencing node found and declared as parent: i=93/OPC Binary using i=47/HasComponent
// Node: opcua_node_variable_t(ns=2;i=6001), 1:TypeDictionary
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "TypeDictionary");
attr.description = UA_LOCALIZEDTEXT("", "Collects the data type descriptions of http://openAAS.org/AAS/");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_ByteString *opcua_node_variable_t_ns_2_i_6001_variant_DataContents =  UA_ByteString_new();
*opcua_node_variable_t_ns_2_i_6001_variant_DataContents = UA_STRING_ALLOC("PG9wYzpUeXBlRGljdGlvbmFyeSB4bWxuczp4c2k9Imh0dHA6Ly93d3cudzMub3JnLzIwMDEvWE1MU2NoZ        W1hLWluc3RhbmNlIiB4bWxuczp0bnM9Imh0dHA6Ly9vcGVuQUFTLm9yZy9BQVMvIiBEZWZhd        Wx0Qnl0ZU9yZGVyPSJMaXR0bGVFbmRpYW4iIHhtbG5zOm9wYz0iaHR0cDovL29wY2ZvdW5kY        XRpb24ub3JnL0JpbmFyeVNjaGVtYS8iIHhtbG5zOnVhPSJodHRwOi8vb3BjZm91bmRhdGlvb        i5vcmcvVUEvIiBUYXJnZXROYW1lc3BhY2U9Imh0dHA6Ly9vcGVuQUFTLm9yZy9BQVMvIj4KI        DxvcGM6SW1wb3J0IE5hbWVzcGFjZT0iaHR0cDovL29wY2ZvdW5kYXRpb24ub3JnL1VBLyIvP        gogPG9wYzpTdHJ1Y3R1cmVkVHlwZSBCYXNlVHlwZT0idWE6RXh0ZW5zaW9uT2JqZWN0IiBOY        W1lPSJJZGVudGlmaWNhdGlvbiI+CiAgPG9wYzpGaWVsZCBUeXBlTmFtZT0ib3BjOkNoYXJBc        nJheSIgTmFtZT0iSWRTcGVjIi8+CiAgPG9wYzpGaWVsZCBUeXBlTmFtZT0idG5zOklkRW51b        SIgTmFtZT0iSWRUeXBlIi8+CiA8L29wYzpTdHJ1Y3R1cmVkVHlwZT4KIDxvcGM6U3RydWN0d        XJlZFR5cGUgQmFzZVR5cGU9InVhOkV4dGVuc2lvbk9iamVjdCIgTmFtZT0iTGlmZUN5Y2xlR        W50cnkiPgogIDxvcGM6RmllbGQgVHlwZU5hbWU9InRuczpJZGVudGlmaWNhdGlvbiIgTmFtZ        T0iQ3JlYXRpbmdJbnN0YW5jZSIvPgogIDxvcGM6RmllbGQgVHlwZU5hbWU9InRuczpJZGVud        GlmaWNhdGlvbiIgTmFtZT0iV3JpdGluZ0luc3RhbmNlIi8+CiAgPG9wYzpGaWVsZCBUeXBlT        mFtZT0idWE6RGF0YVZhbHVlIiBOYW1lPSJEYXRhIi8+CiAgPG9wYzpGaWVsZCBUeXBlTmFtZ        T0ib3BjOkNoYXJBcnJheSIgTmFtZT0iU3ViamVjdCIvPgogIDxvcGM6RmllbGQgVHlwZU5hb        WU9Im9wYzpDaGFyQXJyYXkiIE5hbWU9IkV2ZW50Q2xhc3MiLz4KIDwvb3BjOlN0cnVjdHVyZ        WRUeXBlPgogPG9wYzpTdHJ1Y3R1cmVkVHlwZSBCYXNlVHlwZT0idWE6RXh0ZW5zaW9uT2JqZ        WN0IiBOYW1lPSJQcm9wZXJ0eVZhbHVlU3RhdGVtZW50Ij4KICA8b3BjOkZpZWxkIFR5cGVOY        W1lPSJ0bnM6RXhwcmVzc2lvblNlbWFudGljRW51bSIgTmFtZT0iRXhwcmVzc2lvblNlbWFud        GljIi8+CiAgPG9wYzpGaWVsZCBUeXBlTmFtZT0idG5zOlJlbGF0aW9uYWxFeHByZXNzaW9uR        W51bSIgTmFtZT0iUmVsYXRpb25hbEV4cHJlc3Npb24iLz4KICA8b3BjOkZpZWxkIFR5cGVOY        W1lPSJvcGM6Q2hhckFycmF5IiBOYW1lPSJVbml0Ii8+CiAgPG9wYzpGaWVsZCBUeXBlTmFtZ        T0idWE6VmFyaWFudCIgTmFtZT0iVmFsdWUiLz4KICA8b3BjOkZpZWxkIFR5cGVOYW1lPSJ0b        nM6SWRlbnRpZmljYXRpb24iIE5hbWU9IlByb3BlcnR5UmVmZXJlbmNlIi8+CiAgPG9wYzpGa        WVsZCBUeXBlTmFtZT0idG5zOlZpZXdFbnVtIiBOYW1lPSJWaWV3Ii8+CiAgPG9wYzpGaWVsZ        CBUeXBlTmFtZT0ib3BjOkJvb2xlYW4iIE5hbWU9IklzUHVibGljIi8+CiA8L29wYzpTdHJ1Y        3R1cmVkVHlwZT4KIDxvcGM6RW51bWVyYXRlZFR5cGUgTGVuZ3RoSW5CaXRzPSIzMiIgTmFtZ        T0iRXhwcmVzc2lvblNlbWFudGljRW51bSI+CiAgPG9wYzpFbnVtZXJhdGVkVmFsdWUgTmFtZ        T0iQVNTVVJBTkNFIiBWYWx1ZT0iMCIvPgogIDxvcGM6RW51bWVyYXRlZFZhbHVlIE5hbWU9I        lNFVFRJTkciIFZhbHVlPSIxIi8+CiAgPG9wYzpFbnVtZXJhdGVkVmFsdWUgTmFtZT0iTUVBU        1VSRU1FTlQiIFZhbHVlPSIyIi8+CiAgPG9wYzpFbnVtZXJhdGVkVmFsdWUgTmFtZT0iUkVRV        UlSRU1FTlQiIFZhbHVlPSIzIi8+CiA8L29wYzpFbnVtZXJhdGVkVHlwZT4KIDxvcGM6RW51b        WVyYXRlZFR5cGUgTGVuZ3RoSW5CaXRzPSIzMiIgTmFtZT0iSWRFbnVtIj4KICA8b3BjOkVud        W1lcmF0ZWRWYWx1ZSBOYW1lPSJVUkkiIFZhbHVlPSIwIi8+CiAgPG9wYzpFbnVtZXJhdGVkV        mFsdWUgTmFtZT0iSVNPIiBWYWx1ZT0iMSIvPgogPC9vcGM6RW51bWVyYXRlZFR5cGU+CiA8b        3BjOkVudW1lcmF0ZWRUeXBlIExlbmd0aEluQml0cz0iMzIiIE5hbWU9IlJlbGF0aW9uYWxFe        HByZXNzaW9uRW51bSI+CiAgPG9wYzpFbnVtZXJhdGVkVmFsdWUgTmFtZT0iR1JFQVRFUl9US        EFOIiBWYWx1ZT0iMCIvPgogIDxvcGM6RW51bWVyYXRlZFZhbHVlIE5hbWU9IkdSRUFURVJfR        VFVQUwiIFZhbHVlPSIxIi8+CiAgPG9wYzpFbnVtZXJhdGVkVmFsdWUgTmFtZT0iRVFVQUwiI        FZhbHVlPSIyIi8+CiAgPG9wYzpFbnVtZXJhdGVkVmFsdWUgTmFtZT0iTk9UX0VRVUFMIiBWY        Wx1ZT0iMyIvPgogIDxvcGM6RW51bWVyYXRlZFZhbHVlIE5hbWU9IkxFU1NfRVFVQUwiIFZhb        HVlPSI0Ii8+CiAgPG9wYzpFbnVtZXJhdGVkVmFsdWUgTmFtZT0iTEVTU19USEFOIiBWYWx1Z        T0iNSIvPgogPC9vcGM6RW51bWVyYXRlZFR5cGU+CiA8b3BjOkVudW1lcmF0ZWRUeXBlIExlb        md0aEluQml0cz0iMzIiIE5hbWU9IlZpZXdFbnVtIj4KICA8b3BjOkVudW1lcmF0ZWRWYWx1Z        SBOYW1lPSJCVVNJTkVTUyIgVmFsdWU9IjAiLz4KICA8b3BjOkVudW1lcmF0ZWRWYWx1ZSBOY        W1lPSJDT05TVFJVQ1RJT04iIFZhbHVlPSIxIi8+CiAgPG9wYzpFbnVtZXJhdGVkVmFsdWUgT        mFtZT0iUE9XRVIiIFZhbHVlPSIyIi8+CiAgPG9wYzpFbnVtZXJhdGVkVmFsdWUgTmFtZT0iR        lVOQ1RJT05BTCIgVmFsdWU9IjMiLz4KICA8b3BjOkVudW1lcmF0ZWRWYWx1ZSBOYW1lPSJMT        0NBVElPTiIgVmFsdWU9IjQiLz4KICA8b3BjOkVudW1lcmF0ZWRWYWx1ZSBOYW1lPSJTRUNVU        klUWSIgVmFsdWU9IjUiLz4KICA8b3BjOkVudW1lcmF0ZWRWYWx1ZSBOYW1lPSJORVRXT1JLI        iBWYWx1ZT0iNiIvPgogIDxvcGM6RW51bWVyYXRlZFZhbHVlIE5hbWU9IkxJRkVDWUNMRSIgV        mFsdWU9IjciLz4KICA8b3BjOkVudW1lcmF0ZWRWYWx1ZSBOYW1lPSJIVU1BTiIgVmFsdWU9I        jgiLz4KIDwvb3BjOkVudW1lcmF0ZWRUeXBlPgo8L29wYzpUeXBlRGljdGlvbmFyeT4K");
UA_Variant_setScalar( &attr.value, opcua_node_variable_t_ns_2_i_6001_variant_DataContents, &UA_TYPES[UA_TYPES_BYTESTRING]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6001);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 72);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 93);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "TypeDictionary");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6001), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 72), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=6001/1:TypeDictionary using i=47/HasComponent
// Node: opcua_node_variable_t(ns=2;i=6013), 1:Identification
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Identification");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_String *opcua_node_variable_t_ns_2_i_6013_variant_DataContents =  UA_String_new();
*opcua_node_variable_t_ns_2_i_6013_variant_DataContents = UA_STRING_ALLOC("Identification");
UA_Variant_setScalar( &attr.value, opcua_node_variable_t_ns_2_i_6013_variant_DataContents, &UA_TYPES[UA_TYPES_STRING]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6013);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 69);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 6001);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "Identification");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6013), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 69), true);
} while(0);

do {
// Referencing node found and declared as parent: i=92/XML Schema using i=47/HasComponent
// Node: opcua_node_variable_t(ns=2;i=6003), 1:TypeDictionary
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "TypeDictionary");
attr.description = UA_LOCALIZEDTEXT("", "Collects the data type descriptions of http://openAAS.org/AAS/");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_ByteString *opcua_node_variable_t_ns_2_i_6003_variant_DataContents =  UA_ByteString_new();
*opcua_node_variable_t_ns_2_i_6003_variant_DataContents = UA_STRING_ALLOC("PHhzOnNjaGVtYSBlbGVtZW50Rm9ybURlZmF1bHQ9InF1YWxpZmllZCIgdGFyZ2V0TmFtZXNwYWNlPSJod        HRwOi8vb3BlbkFBUy5vcmcvQUFTL1R5cGVzLnhzZCIgeG1sbnM6dG5zPSJodHRwOi8vb3Blb        kFBUy5vcmcvQUFTL1R5cGVzLnhzZCIgeG1sbnM6dWE9Imh0dHA6Ly9vcGNmb3VuZGF0aW9uL        m9yZy9VQS8yMDA4LzAyL1R5cGVzLnhzZCIgeG1sbnM6eHM9Imh0dHA6Ly93d3cudzMub3JnL        zIwMDEvWE1MU2NoZW1hIj4KIDx4czppbXBvcnQgbmFtZXNwYWNlPSJodHRwOi8vb3BjZm91b        mRhdGlvbi5vcmcvVUEvMjAwOC8wMi9UeXBlcy54c2QiLz4KIDx4czpzaW1wbGVUeXBlIG5hb        WU9IkV4cHJlc3Npb25TZW1hbnRpY0VudW0iPgogIDx4czpyZXN0cmljdGlvbiBiYXNlPSJ4c        zpzdHJpbmciPgogICA8eHM6ZW51bWVyYXRpb24gdmFsdWU9IkFTU1VSQU5DRV8wIi8+CiAgI        Dx4czplbnVtZXJhdGlvbiB2YWx1ZT0iU0VUVElOR18xIi8+CiAgIDx4czplbnVtZXJhdGlvb        iB2YWx1ZT0iTUVBU1VSRU1FTlRfMiIvPgogICA8eHM6ZW51bWVyYXRpb24gdmFsdWU9IlJFU        VVJUkVNRU5UXzMiLz4KICA8L3hzOnJlc3RyaWN0aW9uPgogPC94czpzaW1wbGVUeXBlPgogP        HhzOmVsZW1lbnQgdHlwZT0idG5zOkV4cHJlc3Npb25TZW1hbnRpY0VudW0iIG5hbWU9IkV4c        HJlc3Npb25TZW1hbnRpY0VudW0iLz4KIDx4czpjb21wbGV4VHlwZSBuYW1lPSJMaXN0T2ZFe        HByZXNzaW9uU2VtYW50aWNFbnVtIj4KICA8eHM6c2VxdWVuY2U+CiAgIDx4czplbGVtZW50I        G1pbk9jY3Vycz0iMCIgbWF4T2NjdXJzPSJ1bmJvdW5kZWQiIHR5cGU9InRuczpFeHByZXNza        W9uU2VtYW50aWNFbnVtIiBuYW1lPSJFeHByZXNzaW9uU2VtYW50aWNFbnVtIiBuaWxsYWJsZ        T0idHJ1ZSIvPgogIDwveHM6c2VxdWVuY2U+CiA8L3hzOmNvbXBsZXhUeXBlPgogPHhzOmVsZ        W1lbnQgdHlwZT0idG5zOkxpc3RPZkV4cHJlc3Npb25TZW1hbnRpY0VudW0iIG5hbWU9Ikxpc        3RPZkV4cHJlc3Npb25TZW1hbnRpY0VudW0iIG5pbGxhYmxlPSJ0cnVlIi8+CiA8eHM6c2ltc        GxlVHlwZSBuYW1lPSJJZEVudW0iPgogIDx4czpyZXN0cmljdGlvbiBiYXNlPSJ4czpzdHJpb        mciPgogICA8eHM6ZW51bWVyYXRpb24gdmFsdWU9IlVSSV8wIi8+CiAgIDx4czplbnVtZXJhd        GlvbiB2YWx1ZT0iSVNPXzEiLz4KICA8L3hzOnJlc3RyaWN0aW9uPgogPC94czpzaW1wbGVUe        XBlPgogPHhzOmVsZW1lbnQgdHlwZT0idG5zOklkRW51bSIgbmFtZT0iSWRFbnVtIi8+CiA8e        HM6Y29tcGxleFR5cGUgbmFtZT0iTGlzdE9mSWRFbnVtIj4KICA8eHM6c2VxdWVuY2U+CiAgI        Dx4czplbGVtZW50IG1pbk9jY3Vycz0iMCIgbWF4T2NjdXJzPSJ1bmJvdW5kZWQiIHR5cGU9I        nRuczpJZEVudW0iIG5hbWU9IklkRW51bSIgbmlsbGFibGU9InRydWUiLz4KICA8L3hzOnNlc        XVlbmNlPgogPC94czpjb21wbGV4VHlwZT4KIDx4czplbGVtZW50IHR5cGU9InRuczpMaXN0T        2ZJZEVudW0iIG5hbWU9Ikxpc3RPZklkRW51bSIgbmlsbGFibGU9InRydWUiLz4KIDx4czpza        W1wbGVUeXBlIG5hbWU9IlJlbGF0aW9uYWxFeHByZXNzaW9uRW51bSI+CiAgPHhzOnJlc3Rya        WN0aW9uIGJhc2U9InhzOnN0cmluZyI+CiAgIDx4czplbnVtZXJhdGlvbiB2YWx1ZT0iR1JFQ        VRFUl9USEFOXzAiLz4KICAgPHhzOmVudW1lcmF0aW9uIHZhbHVlPSJHUkVBVEVSX0VRVUFMX        zEiLz4KICAgPHhzOmVudW1lcmF0aW9uIHZhbHVlPSJFUVVBTF8yIi8+CiAgIDx4czplbnVtZ        XJhdGlvbiB2YWx1ZT0iTk9UX0VRVUFMXzMiLz4KICAgPHhzOmVudW1lcmF0aW9uIHZhbHVlP        SJMRVNTX0VRVUFMXzQiLz4KICAgPHhzOmVudW1lcmF0aW9uIHZhbHVlPSJMRVNTX1RIQU5fN        SIvPgogIDwveHM6cmVzdHJpY3Rpb24+CiA8L3hzOnNpbXBsZVR5cGU+CiA8eHM6ZWxlbWVud        CB0eXBlPSJ0bnM6UmVsYXRpb25hbEV4cHJlc3Npb25FbnVtIiBuYW1lPSJSZWxhdGlvbmFsR        XhwcmVzc2lvbkVudW0iLz4KIDx4czpjb21wbGV4VHlwZSBuYW1lPSJMaXN0T2ZSZWxhdGlvb        mFsRXhwcmVzc2lvbkVudW0iPgogIDx4czpzZXF1ZW5jZT4KICAgPHhzOmVsZW1lbnQgbWluT        2NjdXJzPSIwIiBtYXhPY2N1cnM9InVuYm91bmRlZCIgdHlwZT0idG5zOlJlbGF0aW9uYWxFe        HByZXNzaW9uRW51bSIgbmFtZT0iUmVsYXRpb25hbEV4cHJlc3Npb25FbnVtIiBuaWxsYWJsZ        T0idHJ1ZSIvPgogIDwveHM6c2VxdWVuY2U+CiA8L3hzOmNvbXBsZXhUeXBlPgogPHhzOmVsZ        W1lbnQgdHlwZT0idG5zOkxpc3RPZlJlbGF0aW9uYWxFeHByZXNzaW9uRW51bSIgbmFtZT0iT        GlzdE9mUmVsYXRpb25hbEV4cHJlc3Npb25FbnVtIiBuaWxsYWJsZT0idHJ1ZSIvPgogPHhzO        nNpbXBsZVR5cGUgbmFtZT0iVmlld0VudW0iPgogIDx4czpyZXN0cmljdGlvbiBiYXNlPSJ4c        zpzdHJpbmciPgogICA8eHM6ZW51bWVyYXRpb24gdmFsdWU9IkJVU0lORVNTXzAiLz4KICAgP        HhzOmVudW1lcmF0aW9uIHZhbHVlPSJDT05TVFJVQ1RJT05fMSIvPgogICA8eHM6ZW51bWVyY        XRpb24gdmFsdWU9IlBPV0VSXzIiLz4KICAgPHhzOmVudW1lcmF0aW9uIHZhbHVlPSJGVU5DV        ElPTkFMXzMiLz4KICAgPHhzOmVudW1lcmF0aW9uIHZhbHVlPSJMT0NBVElPTl80Ii8+CiAgI        Dx4czplbnVtZXJhdGlvbiB2YWx1ZT0iU0VDVVJJVFlfNSIvPgogICA8eHM6ZW51bWVyYXRpb        24gdmFsdWU9Ik5FVFdPUktfNiIvPgogICA8eHM6ZW51bWVyYXRpb24gdmFsdWU9IkxJRkVDW        UNMRV83Ii8+CiAgIDx4czplbnVtZXJhdGlvbiB2YWx1ZT0iSFVNQU5fOCIvPgogIDwveHM6c        mVzdHJpY3Rpb24+CiA8L3hzOnNpbXBsZVR5cGU+CiA8eHM6ZWxlbWVudCB0eXBlPSJ0bnM6V        mlld0VudW0iIG5hbWU9IlZpZXdFbnVtIi8+CiA8eHM6Y29tcGxleFR5cGUgbmFtZT0iTGlzd        E9mVmlld0VudW0iPgogIDx4czpzZXF1ZW5jZT4KICAgPHhzOmVsZW1lbnQgbWluT2NjdXJzP        SIwIiBtYXhPY2N1cnM9InVuYm91bmRlZCIgdHlwZT0idG5zOlZpZXdFbnVtIiBuYW1lPSJWa        WV3RW51bSIgbmlsbGFibGU9InRydWUiLz4KICA8L3hzOnNlcXVlbmNlPgogPC94czpjb21wb        GV4VHlwZT4KIDx4czplbGVtZW50IHR5cGU9InRuczpMaXN0T2ZWaWV3RW51bSIgbmFtZT0iT        GlzdE9mVmlld0VudW0iIG5pbGxhYmxlPSJ0cnVlIi8+CiA8eHM6Y29tcGxleFR5cGUgbmFtZ        T0iSWRlbnRpZmljYXRpb24iPgogIDx4czpzZXF1ZW5jZT4KICAgPHhzOmVsZW1lbnQgbWluT        2NjdXJzPSIwIiBtYXhPY2N1cnM9IjEiIHR5cGU9InhzOnN0cmluZyIgbmFtZT0iSWRTcGVjI        i8+CiAgIDx4czplbGVtZW50IG1pbk9jY3Vycz0iMCIgbWF4T2NjdXJzPSIxIiB0eXBlPSJ0b        nM6SWRFbnVtIiBuYW1lPSJJZFR5cGUiLz4KICA8L3hzOnNlcXVlbmNlPgogPC94czpjb21wb        GV4VHlwZT4KIDx4czplbGVtZW50IHR5cGU9InRuczpJZGVudGlmaWNhdGlvbiIgbmFtZT0iS        WRlbnRpZmljYXRpb24iLz4KIDx4czpjb21wbGV4VHlwZSBuYW1lPSJMaXN0T2ZJZGVudGlma        WNhdGlvbiI+CiAgPHhzOnNlcXVlbmNlPgogICA8eHM6ZWxlbWVudCBtaW5PY2N1cnM9IjAiI        G1heE9jY3Vycz0idW5ib3VuZGVkIiB0eXBlPSJ0bnM6SWRlbnRpZmljYXRpb24iIG5hbWU9I        klkZW50aWZpY2F0aW9uIiBuaWxsYWJsZT0idHJ1ZSIvPgogIDwveHM6c2VxdWVuY2U+CiA8L        3hzOmNvbXBsZXhUeXBlPgogPHhzOmVsZW1lbnQgdHlwZT0idG5zOkxpc3RPZklkZW50aWZpY        2F0aW9uIiBuYW1lPSJMaXN0T2ZJZGVudGlmaWNhdGlvbiIgbmlsbGFibGU9InRydWUiLz4KI        Dx4czpjb21wbGV4VHlwZSBuYW1lPSJMaWZlQ3ljbGVFbnRyeSI+CiAgPHhzOnNlcXVlbmNlP        gogICA8eHM6ZWxlbWVudCBtaW5PY2N1cnM9IjAiIG1heE9jY3Vycz0iMSIgdHlwZT0idG5zO        klkZW50aWZpY2F0aW9uIiBuYW1lPSJDcmVhdGluZ0luc3RhbmNlIi8+CiAgIDx4czplbGVtZ        W50IG1pbk9jY3Vycz0iMCIgbWF4T2NjdXJzPSIxIiB0eXBlPSJ0bnM6SWRlbnRpZmljYXRpb        24iIG5hbWU9IldyaXRpbmdJbnN0YW5jZSIvPgogICA8eHM6ZWxlbWVudCBtaW5PY2N1cnM9I        jAiIG1heE9jY3Vycz0iMSIgdHlwZT0idWE6RGF0YVZhbHVlIiBuYW1lPSJEYXRhIi8+CiAgI        Dx4czplbGVtZW50IG1pbk9jY3Vycz0iMCIgbWF4T2NjdXJzPSIxIiB0eXBlPSJ4czpzdHJpb        mciIG5hbWU9IlN1YmplY3QiLz4KICAgPHhzOmVsZW1lbnQgbWluT2NjdXJzPSIwIiBtYXhPY        2N1cnM9IjEiIHR5cGU9InhzOnN0cmluZyIgbmFtZT0iRXZlbnRDbGFzcyIvPgogIDwveHM6c        2VxdWVuY2U+CiA8L3hzOmNvbXBsZXhUeXBlPgogPHhzOmVsZW1lbnQgdHlwZT0idG5zOkxpZ        mVDeWNsZUVudHJ5IiBuYW1lPSJMaWZlQ3ljbGVFbnRyeSIvPgogPHhzOmNvbXBsZXhUeXBlI        G5hbWU9Ikxpc3RPZkxpZmVDeWNsZUVudHJ5Ij4KICA8eHM6c2VxdWVuY2U+CiAgIDx4czplb        GVtZW50IG1pbk9jY3Vycz0iMCIgbWF4T2NjdXJzPSJ1bmJvdW5kZWQiIHR5cGU9InRuczpMa        WZlQ3ljbGVFbnRyeSIgbmFtZT0iTGlmZUN5Y2xlRW50cnkiIG5pbGxhYmxlPSJ0cnVlIi8+C        iAgPC94czpzZXF1ZW5jZT4KIDwveHM6Y29tcGxleFR5cGU+CiA8eHM6ZWxlbWVudCB0eXBlP        SJ0bnM6TGlzdE9mTGlmZUN5Y2xlRW50cnkiIG5hbWU9Ikxpc3RPZkxpZmVDeWNsZUVudHJ5I        iBuaWxsYWJsZT0idHJ1ZSIvPgogPHhzOmNvbXBsZXhUeXBlIG5hbWU9IlByb3BlcnR5VmFsd        WVTdGF0ZW1lbnQiPgogIDx4czpzZXF1ZW5jZT4KICAgPHhzOmVsZW1lbnQgbWluT2NjdXJzP        SIwIiBtYXhPY2N1cnM9IjEiIHR5cGU9InRuczpFeHByZXNzaW9uU2VtYW50aWNFbnVtIiBuY        W1lPSJFeHByZXNzaW9uU2VtYW50aWMiLz4KICAgPHhzOmVsZW1lbnQgbWluT2NjdXJzPSIwI        iBtYXhPY2N1cnM9IjEiIHR5cGU9InRuczpSZWxhdGlvbmFsRXhwcmVzc2lvbkVudW0iIG5hb        WU9IlJlbGF0aW9uYWxFeHByZXNzaW9uIi8+CiAgIDx4czplbGVtZW50IG1pbk9jY3Vycz0iM        CIgbWF4T2NjdXJzPSIxIiB0eXBlPSJ4czpzdHJpbmciIG5hbWU9IlVuaXQiLz4KICAgPHhzO        mVsZW1lbnQgbWluT2NjdXJzPSIwIiBtYXhPY2N1cnM9IjEiIHR5cGU9InVhOlZhcmlhbnQiI        G5hbWU9IlZhbHVlIi8+CiAgIDx4czplbGVtZW50IG1pbk9jY3Vycz0iMCIgbWF4T2NjdXJzP        SIxIiB0eXBlPSJ0bnM6SWRlbnRpZmljYXRpb24iIG5hbWU9IlByb3BlcnR5UmVmZXJlbmNlI        i8+CiAgIDx4czplbGVtZW50IG1pbk9jY3Vycz0iMCIgbWF4T2NjdXJzPSIxIiB0eXBlPSJ0b        nM6Vmlld0VudW0iIG5hbWU9IlZpZXciLz4KICAgPHhzOmVsZW1lbnQgbWluT2NjdXJzPSIwI        iBtYXhPY2N1cnM9IjEiIHR5cGU9InhzOmJvb2xlYW4iIG5hbWU9IklzUHVibGljIi8+CiAgP        C94czpzZXF1ZW5jZT4KIDwveHM6Y29tcGxleFR5cGU+CiA8eHM6ZWxlbWVudCB0eXBlPSJ0b        nM6UHJvcGVydHlWYWx1ZVN0YXRlbWVudCIgbmFtZT0iUHJvcGVydHlWYWx1ZVN0YXRlbWVud        CIvPgogPHhzOmNvbXBsZXhUeXBlIG5hbWU9Ikxpc3RPZlByb3BlcnR5VmFsdWVTdGF0ZW1lb        nQiPgogIDx4czpzZXF1ZW5jZT4KICAgPHhzOmVsZW1lbnQgbWluT2NjdXJzPSIwIiBtYXhPY        2N1cnM9InVuYm91bmRlZCIgdHlwZT0idG5zOlByb3BlcnR5VmFsdWVTdGF0ZW1lbnQiIG5hb        WU9IlByb3BlcnR5VmFsdWVTdGF0ZW1lbnQiIG5pbGxhYmxlPSJ0cnVlIi8+CiAgPC94czpzZ        XF1ZW5jZT4KIDwveHM6Y29tcGxleFR5cGU+CiA8eHM6ZWxlbWVudCB0eXBlPSJ0bnM6TGlzd        E9mUHJvcGVydHlWYWx1ZVN0YXRlbWVudCIgbmFtZT0iTGlzdE9mUHJvcGVydHlWYWx1ZVN0Y        XRlbWVudCIgbmlsbGFibGU9InRydWUiLz4KPC94czpzY2hlbWE+Cg==");
UA_Variant_setScalar( &attr.value, opcua_node_variable_t_ns_2_i_6003_variant_DataContents, &UA_TYPES[UA_TYPES_BYTESTRING]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6003);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 72);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 92);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "TypeDictionary");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6003), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 72), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=6003/1:TypeDictionary using i=47/HasComponent
// Node: opcua_node_variable_t(ns=2;i=6014), 1:Identification
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Identification");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_String *opcua_node_variable_t_ns_2_i_6014_variant_DataContents =  UA_String_new();
*opcua_node_variable_t_ns_2_i_6014_variant_DataContents = UA_STRING_ALLOC("//xs:element[@name='Identification']");
UA_Variant_setScalar( &attr.value, opcua_node_variable_t_ns_2_i_6014_variant_DataContents, &UA_TYPES[UA_TYPES_STRING]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6014);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 69);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 6003);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "Identification");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6014), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 69), true);
} while(0);

do {
// Referencing node found and declared as parent: i=22/Structure using i=45/HasSubtype
// Node: opcua_node_dataType_t(ns=2;i=3008), 1:LifeCycleEntry
UA_DataTypeAttributes attr;
UA_DataTypeAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "LifeCycleEntry");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 3008);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 22);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 45);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "LifeCycleEntry");
UA_Server_addDataTypeNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, NULL, NULL);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=6001/1:TypeDictionary using i=47/HasComponent
// Node: opcua_node_variable_t(ns=2;i=6031), 1:LifeCycleEntry
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "LifeCycleEntry");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_String *opcua_node_variable_t_ns_2_i_6031_variant_DataContents =  UA_String_new();
*opcua_node_variable_t_ns_2_i_6031_variant_DataContents = UA_STRING_ALLOC("LifeCycleEntry");
UA_Variant_setScalar( &attr.value, opcua_node_variable_t_ns_2_i_6031_variant_DataContents, &UA_TYPES[UA_TYPES_STRING]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6031);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 69);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 6001);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "LifeCycleEntry");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6031), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 69), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=6003/1:TypeDictionary using i=47/HasComponent
// Node: opcua_node_variable_t(ns=2;i=6032), 1:LifeCycleEntry
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "LifeCycleEntry");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_String *opcua_node_variable_t_ns_2_i_6032_variant_DataContents =  UA_String_new();
*opcua_node_variable_t_ns_2_i_6032_variant_DataContents = UA_STRING_ALLOC("//xs:element[@name='LifeCycleEntry']");
UA_Variant_setScalar( &attr.value, opcua_node_variable_t_ns_2_i_6032_variant_DataContents, &UA_TYPES[UA_TYPES_STRING]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6032);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 69);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 6003);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "LifeCycleEntry");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6032), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 69), true);
} while(0);

do {
// Referencing node found and declared as parent: i=22/Structure using i=45/HasSubtype
// Node: opcua_node_dataType_t(ns=2;i=3011), 1:PropertyValueStatement
UA_DataTypeAttributes attr;
UA_DataTypeAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "PropertyValueStatement");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 3011);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 22);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 45);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "PropertyValueStatement");
UA_Server_addDataTypeNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, NULL, NULL);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=6001/1:TypeDictionary using i=47/HasComponent
// Node: opcua_node_variable_t(ns=2;i=6025), 1:PropertyValueStatement
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "PropertyValueStatement");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_String *opcua_node_variable_t_ns_2_i_6025_variant_DataContents =  UA_String_new();
*opcua_node_variable_t_ns_2_i_6025_variant_DataContents = UA_STRING_ALLOC("PropertyValueStatement");
UA_Variant_setScalar( &attr.value, opcua_node_variable_t_ns_2_i_6025_variant_DataContents, &UA_TYPES[UA_TYPES_STRING]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6025);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 69);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 6001);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "PropertyValueStatement");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6025), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 69), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=6003/1:TypeDictionary using i=47/HasComponent
// Node: opcua_node_variable_t(ns=2;i=6027), 1:PropertyValueStatement
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "PropertyValueStatement");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_String *opcua_node_variable_t_ns_2_i_6027_variant_DataContents =  UA_String_new();
*opcua_node_variable_t_ns_2_i_6027_variant_DataContents = UA_STRING_ALLOC("//xs:element[@name='PropertyValueStatement']");
UA_Variant_setScalar( &attr.value, opcua_node_variable_t_ns_2_i_6027_variant_DataContents, &UA_TYPES[UA_TYPES_STRING]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6027);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 69);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 6003);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "PropertyValueStatement");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6027), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 69), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=6001/1:TypeDictionary using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6002), NamespaceUri
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "NamespaceUri");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_String *opcua_node_variable_t_ns_2_i_6002_variant_DataContents =  UA_String_new();
*opcua_node_variable_t_ns_2_i_6002_variant_DataContents = UA_STRING_ALLOC("http://openAAS.org/AAS/");
UA_Variant_setScalar( &attr.value, opcua_node_variable_t_ns_2_i_6002_variant_DataContents, &UA_TYPES[UA_TYPES_STRING]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6002);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 6001);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(0, "NamespaceUri");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6002), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=6003/1:TypeDictionary using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6004), NamespaceUri
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "NamespaceUri");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_String *opcua_node_variable_t_ns_2_i_6004_variant_DataContents =  UA_String_new();
*opcua_node_variable_t_ns_2_i_6004_variant_DataContents = UA_STRING_ALLOC("http://openAAS.org/AAS/Types.xsd");
UA_Variant_setScalar( &attr.value, opcua_node_variable_t_ns_2_i_6004_variant_DataContents, &UA_TYPES[UA_TYPES_STRING]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6004);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 6003);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(0, "NamespaceUri");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6004), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: i=63/BaseDataVariableType using i=45/HasSubtype
// Node: opcua_node_variableType_t(ns=2;i=2003), 1:LifeCycleEntryType
UA_VariableTypeAttributes attr;
UA_VariableTypeAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "LifeCycleEntryType");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.valueRank = -2;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 2003);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 63);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 45);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "LifeCycleEntryType");
UA_Server_addVariableTypeNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=2003/1:LifeCycleEntryType using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6015), 1:CreatingInstance
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "CreatingInstance");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6015);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 2003);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "CreatingInstance");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6015), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6015), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=2003/1:LifeCycleEntryType using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6017), 1:Data
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Data");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6017);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 2003);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "Data");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6017), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6017), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=2003/1:LifeCycleEntryType using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6019), 1:EventClass
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "EventClass");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_String *opcua_node_variable_t_ns_2_i_6019_variant_DataContents =  UA_String_new();
*opcua_node_variable_t_ns_2_i_6019_variant_DataContents = UA_STRING_ALLOC("");
UA_Variant_setScalar( &attr.value, opcua_node_variable_t_ns_2_i_6019_variant_DataContents, &UA_TYPES[UA_TYPES_STRING]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6019);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 2003);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "EventClass");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6019), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6019), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=2003/1:LifeCycleEntryType using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6018), 1:Subject
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Subject");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6018);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 2003);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "Subject");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6018), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6018), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=2003/1:LifeCycleEntryType using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6016), 1:WritingInstance
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "WritingInstance");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6016);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 2003);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "WritingInstance");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6016), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6016), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: i=63/BaseDataVariableType using i=45/HasSubtype
// Node: opcua_node_variableType_t(ns=2;i=2002), 1:PropertyValueStatementType
UA_VariableTypeAttributes attr;
UA_VariableTypeAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "PropertyValueStatementType");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.valueRank = -1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 2002);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 63);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 45);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "PropertyValueStatementType");
UA_Server_addVariableTypeNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=2002/1:PropertyValueStatementType using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6008), 1:ExpressionSemantic
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "ExpressionSemantic");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_Int32 *opcua_node_variable_t_ns_2_i_6008_variant_DataContents =  UA_Int32_new();
*opcua_node_variable_t_ns_2_i_6008_variant_DataContents = (UA_Int32) 0;
UA_Variant_setScalar( &attr.value, opcua_node_variable_t_ns_2_i_6008_variant_DataContents, &UA_TYPES[UA_TYPES_INT32]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6008);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 2002);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "ExpressionSemantic");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6008), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6008), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=2002/1:PropertyValueStatementType using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6012), 1:PropertyReference
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "PropertyReference");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6012);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 2002);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "PropertyReference");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6012), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6012), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=2002/1:PropertyValueStatementType using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6009), 1:RelationalExpression
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "RelationalExpression");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_Int32 *opcua_node_variable_t_ns_2_i_6009_variant_DataContents =  UA_Int32_new();
*opcua_node_variable_t_ns_2_i_6009_variant_DataContents = (UA_Int32) 0;
UA_Variant_setScalar( &attr.value, opcua_node_variable_t_ns_2_i_6009_variant_DataContents, &UA_TYPES[UA_TYPES_INT32]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6009);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 2002);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "RelationalExpression");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6009), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6009), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=2002/1:PropertyValueStatementType using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6010), 1:Unit
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Unit");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6010);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 2002);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "Unit");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6010), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6010), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=2002/1:PropertyValueStatementType using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6011), 1:Value
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Value");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6011);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 2002);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "Value");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6011), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6011), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=2002/1:PropertyValueStatementType using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6021), 1:View
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "View");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6021);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 2002);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "View");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6021), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6021), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: i=58/BaseObjectType using i=45/HasSubtype
// Node: opcua_node_objectType_t(ns=2;i=1005), 1:AASFactoryType
UA_ObjectTypeAttributes attr;
UA_ObjectTypeAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "AASFactoryType");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 1005);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 58);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 45);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "AASFactoryType");
UA_Server_addObjectTypeNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, NULL, NULL);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=2;i=7001), 1:createAAS
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 3);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<3; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("AASId");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3005);
inputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[1].name = UA_STRING("Name");
inputArguments[1].valueRank = -1;
inputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
inputArguments[2].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[2].name = UA_STRING("AssetId");
inputArguments[2].valueRank = -1;
inputArguments[2].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3005);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<1; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
outputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3009);
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "createAAS");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 7001);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "createAAS");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 3, inputArguments,  1, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7001), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6033), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7001), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7001), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6034), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6033), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 7001), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6034), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 7001), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=2;i=7007), 1:createLCE
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 6);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<6; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("AASId");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3005);
inputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[1].name = UA_STRING("CreatingInstanceId");
inputArguments[1].valueRank = -1;
inputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3005);
inputArguments[2].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[2].name = UA_STRING("WritingInstanceId");
inputArguments[2].valueRank = -1;
inputArguments[2].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3005);
inputArguments[3].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[3].name = UA_STRING("EventClass");
inputArguments[3].valueRank = -1;
inputArguments[3].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
inputArguments[4].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[4].name = UA_STRING("Subject");
inputArguments[4].valueRank = -1;
inputArguments[4].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
inputArguments[5].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[5].name = UA_STRING("Data");
inputArguments[5].valueRank = -1;
inputArguments[5].dataType = UA_NODEID_NUMERIC(nsIdx_0, 23);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<1; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
outputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3009);
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "createLCE");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 7007);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "createLCE");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 6, inputArguments,  1, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7007), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6045), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7007), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7007), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6046), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6045), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 7007), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6046), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 7007), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=2;i=7005), 1:createPVS
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 10);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<10; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("AASId");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3005);
inputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[1].name = UA_STRING("PVSLName");
inputArguments[1].valueRank = -1;
inputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
inputArguments[2].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[2].name = UA_STRING("Name");
inputArguments[2].valueRank = -1;
inputArguments[2].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
inputArguments[3].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[3].name = UA_STRING("RelationalExpression");
inputArguments[3].valueRank = -1;
inputArguments[3].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3003);
inputArguments[4].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[4].name = UA_STRING("ExpressionSemantic");
inputArguments[4].valueRank = -1;
inputArguments[4].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3004);
inputArguments[5].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[5].name = UA_STRING("Value");
inputArguments[5].valueRank = -1;
inputArguments[5].dataType = UA_NODEID_NUMERIC(nsIdx_0, 23);
inputArguments[6].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[6].name = UA_STRING("Unit");
inputArguments[6].valueRank = -1;
inputArguments[6].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
inputArguments[7].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[7].name = UA_STRING("PropertyReference");
inputArguments[7].valueRank = -1;
inputArguments[7].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3005);
inputArguments[8].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[8].name = UA_STRING("View");
inputArguments[8].valueRank = -1;
inputArguments[8].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3007);
inputArguments[9].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[9].name = UA_STRING("IsPublic");
inputArguments[9].valueRank = -1;
inputArguments[9].dataType = UA_NODEID_NUMERIC(nsIdx_0, 1);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<1; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
outputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3009);
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "createPVS");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 7005);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "createPVS");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 10, inputArguments,  1, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7005), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6041), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7005), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7005), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6042), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6041), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 7005), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6042), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 7005), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=2;i=7003), 1:createPVSL
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 3);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<3; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("AASId");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3005);
inputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[1].name = UA_STRING("Name");
inputArguments[1].valueRank = -1;
inputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
inputArguments[2].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[2].name = UA_STRING("Carrier");
inputArguments[2].valueRank = -1;
inputArguments[2].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3005);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<1; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
outputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3009);
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "createPVSL");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 7003);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "createPVSL");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 3, inputArguments,  1, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7003), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6035), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7003), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7003), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6036), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6035), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 7003), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6036), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 7003), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=2;i=7002), 1:deleteAAS
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<1; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("AASId");
inputArguments[0].valueRank = -1;

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<1; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
outputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3009);
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "deleteAAS");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 7002);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "deleteAAS");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 1, inputArguments,  1, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7002), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6037), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7002), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7002), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6038), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6037), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 7002), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6038), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 7002), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=2;i=7008), 1:deleteLCE
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 2);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<2; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("AASId");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3005);
inputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[1].name = UA_STRING("LCEId");
inputArguments[1].valueRank = -1;
inputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_0, 9);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<1; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
outputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3009);
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "deleteLCE");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 7008);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "deleteLCE");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 2, inputArguments,  1, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7008), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6047), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7008), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7008), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6048), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6047), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 7008), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6048), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 7008), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=2;i=7006), 1:deletePVS
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 3);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<3; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("AASId");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3005);
inputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[1].name = UA_STRING("PVSLName");
inputArguments[1].valueRank = -1;
inputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
inputArguments[2].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[2].name = UA_STRING("Name");
inputArguments[2].valueRank = -1;
inputArguments[2].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<1; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
outputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3009);
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "deletePVS");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 7006);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "deletePVS");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 3, inputArguments,  1, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7006), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6043), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7006), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7006), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6044), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6043), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 7006), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6044), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 7006), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=2;i=7004), 1:deletePVSL
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 2);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<2; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("AASId");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3005);
inputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[1].name = UA_STRING("Name");
inputArguments[1].valueRank = -1;
inputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<1; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
outputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3009);
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "deletePVSL");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 7004);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "deletePVSL");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 2, inputArguments,  1, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7004), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6039), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7004), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7004), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6040), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6039), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 7004), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6040), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 7004), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=2;i=7013), 1:getAASNodeId
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<1; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("AASId");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3005);

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
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 7013);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "getAASNodeId");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 1, inputArguments,  1, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7013), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6026), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7013), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7013), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6057), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6026), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 7013), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6057), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 7013), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=2;i=7012), 1:getLCE
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 2);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<2; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("AASId");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3005);
inputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[1].name = UA_STRING("LCEId");
inputArguments[1].valueRank = -1;
inputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_0, 9);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 6);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<6; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("CreatingInstanceId");
outputArguments[0].valueRank = -1;
outputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3005);
outputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[1].name = UA_STRING("WritingInstanceId");
outputArguments[1].valueRank = -1;
outputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3005);
outputArguments[2].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[2].name = UA_STRING("EventClass");
outputArguments[2].valueRank = -1;
outputArguments[2].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
outputArguments[3].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[3].name = UA_STRING("Subject");
outputArguments[3].valueRank = -1;
outputArguments[3].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
outputArguments[4].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[4].name = UA_STRING("Data");
outputArguments[4].valueRank = -1;
outputArguments[4].dataType = UA_NODEID_NUMERIC(nsIdx_0, 23);
outputArguments[5].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[5].name = UA_STRING("Status");
outputArguments[5].valueRank = -1;
outputArguments[5].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3009);
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "getLCE");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 7012);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "getLCE");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 2, inputArguments,  6, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7012), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6053), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7012), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7012), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6054), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6053), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 7012), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6054), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 7012), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=2;i=7014), 1:getLastLCEs
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 2);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<2; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("AASId");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3005);
inputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[1].name = UA_STRING("Count");
inputArguments[1].valueRank = -1;
inputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_0, 7);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 6);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<6; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("CreatingInstanceId");
outputArguments[0].valueRank = 1;
outputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3005);
outputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[1].name = UA_STRING("WritingInstanceId");
outputArguments[1].valueRank = 1;
outputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3005);
outputArguments[2].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[2].name = UA_STRING("EventClass");
outputArguments[2].valueRank = 1;
outputArguments[2].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
outputArguments[3].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[3].name = UA_STRING("Subject");
outputArguments[3].valueRank = 1;
outputArguments[3].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
outputArguments[4].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[4].name = UA_STRING("Data");
outputArguments[4].valueRank = 1;
outputArguments[4].dataType = UA_NODEID_NUMERIC(nsIdx_0, 23);
outputArguments[5].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[5].name = UA_STRING("Status");
outputArguments[5].valueRank = -1;
outputArguments[5].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3009);
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "getLastLCEs");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 7014);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "getLastLCEs");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 2, inputArguments,  6, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7014), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6058), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7014), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7014), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6059), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6058), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 7014), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6059), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 7014), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=2;i=7010), 1:getPVS
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 3);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<3; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("AASId");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3005);
inputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[1].name = UA_STRING("PVSLName");
inputArguments[1].valueRank = -1;
inputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
inputArguments[2].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[2].name = UA_STRING("Name");
inputArguments[2].valueRank = -1;
inputArguments[2].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 8);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<8; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("RelationalExpression");
outputArguments[0].valueRank = -1;
outputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3003);
outputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[1].name = UA_STRING("ExpressionSemantic");
outputArguments[1].valueRank = -1;
outputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3004);
outputArguments[2].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[2].name = UA_STRING("Value");
outputArguments[2].valueRank = -1;
outputArguments[2].dataType = UA_NODEID_NUMERIC(nsIdx_0, 23);
outputArguments[3].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[3].name = UA_STRING("Unit");
outputArguments[3].valueRank = -1;
outputArguments[3].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
outputArguments[4].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[4].name = UA_STRING("PropertyReference");
outputArguments[4].valueRank = -1;
outputArguments[4].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3005);
outputArguments[5].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[5].name = UA_STRING("View");
outputArguments[5].valueRank = -1;
outputArguments[5].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3007);
outputArguments[6].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[6].name = UA_STRING("IsPublic");
outputArguments[6].valueRank = -1;
outputArguments[6].dataType = UA_NODEID_NUMERIC(nsIdx_0, 1);
outputArguments[7].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[7].name = UA_STRING("Status");
outputArguments[7].valueRank = -1;
outputArguments[7].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3009);
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "getPVS");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 7010);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "getPVS");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 3, inputArguments,  8, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7010), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6055), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7010), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7010), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6056), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6055), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 7010), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6056), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 7010), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=2;i=7009), 1:setLCE
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 7);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<7; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("AASId");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3005);
inputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[1].name = UA_STRING("LCEId");
inputArguments[1].valueRank = -1;
inputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_0, 9);
inputArguments[2].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[2].name = UA_STRING("CreatingInstanceId");
inputArguments[2].valueRank = -1;
inputArguments[2].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3005);
inputArguments[3].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[3].name = UA_STRING("WritingInstanceId");
inputArguments[3].valueRank = -1;
inputArguments[3].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3005);
inputArguments[4].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[4].name = UA_STRING("EventClass");
inputArguments[4].valueRank = -1;
inputArguments[4].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
inputArguments[5].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[5].name = UA_STRING("Subject");
inputArguments[5].valueRank = -1;
inputArguments[5].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
inputArguments[6].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[6].name = UA_STRING("Data");
inputArguments[6].valueRank = -1;
inputArguments[6].dataType = UA_NODEID_NUMERIC(nsIdx_0, 23);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<1; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
outputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3009);
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "setLCE");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 7009);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "setLCE");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 7, inputArguments,  1, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7009), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6051), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7009), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7009), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6052), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6051), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 7009), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6052), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 7009), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1005/1:AASFactoryType using i=47/HasComponent
// Node: opcua_node_method_t(ns=2;i=7011), 1:setPVS
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 10);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<10; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("AASId");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3005);
inputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[1].name = UA_STRING("PVSLName");
inputArguments[1].valueRank = -1;
inputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
inputArguments[2].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[2].name = UA_STRING("Name");
inputArguments[2].valueRank = -1;
inputArguments[2].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
inputArguments[3].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[3].name = UA_STRING("RelationalExpression");
inputArguments[3].valueRank = -1;
inputArguments[3].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3003);
inputArguments[4].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[4].name = UA_STRING("ExpressionSemantic");
inputArguments[4].valueRank = -1;
inputArguments[4].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3004);
inputArguments[5].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[5].name = UA_STRING("Value");
inputArguments[5].valueRank = -1;
inputArguments[5].dataType = UA_NODEID_NUMERIC(nsIdx_0, 23);
inputArguments[6].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[6].name = UA_STRING("Unit");
inputArguments[6].valueRank = -1;
inputArguments[6].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
inputArguments[7].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[7].name = UA_STRING("PropertyReference");
inputArguments[7].valueRank = -1;
inputArguments[7].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3005);
inputArguments[8].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[8].name = UA_STRING("View");
inputArguments[8].valueRank = -1;
inputArguments[8].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3007);
inputArguments[9].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[9].name = UA_STRING("IsPublic");
inputArguments[9].valueRank = -1;
inputArguments[9].dataType = UA_NODEID_NUMERIC(nsIdx_0, 1);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<1; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
outputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_2, 3009);
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "setPVS");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 7011);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1005);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "setPVS");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 10, inputArguments,  1, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7011), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6049), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7011), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 7011), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6050), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6049), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 7011), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6050), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 7011), false);
} while(0);

do {
// Referencing node found and declared as parent: i=58/BaseObjectType using i=45/HasSubtype
// Node: opcua_node_objectType_t(ns=2;i=1004), 1:AssetAdministrationShellType
UA_ObjectTypeAttributes attr;
UA_ObjectTypeAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "AssetAdministrationShellType");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 1004);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 58);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 45);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "AssetAdministrationShellType");
UA_Server_addObjectTypeNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, NULL, NULL);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1004/1:AssetAdministrationShellType using i=47/HasComponent
// Node: opcua_node_object_t(ns=2;i=5010), 1:Body
UA_ObjectAttributes attr;
UA_ObjectAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Body");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 5010);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 61);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1004);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "Body");
UA_Server_addObjectNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 58), true); //remove HasTypeDefinition refs generated by addObjectNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5010), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 61), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5010), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
} while(0);

do {
// Referencing node found and declared as parent: i=58/BaseObjectType using i=45/HasSubtype
// Node: opcua_node_objectType_t(ns=2;i=1002), 1:PropertyValueStatementListType
UA_ObjectTypeAttributes attr;
UA_ObjectTypeAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "PropertyValueStatementListType");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 1002);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 58);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 45);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "PropertyValueStatementListType");
UA_Server_addObjectTypeNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, NULL, NULL);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1004/1:AssetAdministrationShellType using i=47/HasComponent
// Node: opcua_node_object_t(ns=2;i=5004), 1:Header
UA_ObjectAttributes attr;
UA_ObjectAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Header");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 5004);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_2, 1002);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1004);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "Header");
UA_Server_addObjectNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 58), true); //remove HasTypeDefinition refs generated by addObjectNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5004), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5004), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 1002), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=5004/1:Header using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6024), 1:CarrierId
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "CarrierId");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6024);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 5004);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "CarrierId");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6024), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6024), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=5004/1:Header using i=47/HasComponent
// Node: opcua_node_variable_t(ns=2;i=6029), 1:PropertyValueStatements
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "PropertyValueStatements");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = 1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6029);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 63);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 5004);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "PropertyValueStatements");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6029), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 63), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6029), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
} while(0);

do {
// Referencing node found and declared as parent: i=58/BaseObjectType using i=45/HasSubtype
// Node: opcua_node_objectType_t(ns=2;i=1003), 1:LifeCycleArchiveType
UA_ObjectTypeAttributes attr;
UA_ObjectTypeAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "LifeCycleArchiveType");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 1003);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 58);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 45);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "LifeCycleArchiveType");
UA_Server_addObjectTypeNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, NULL, NULL);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1004/1:AssetAdministrationShellType using i=47/HasComponent
// Node: opcua_node_object_t(ns=2;i=5003), 1:LifeCycleArchive
UA_ObjectAttributes attr;
UA_ObjectAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "LifeCycleArchive");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 5003);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_2, 1003);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1004);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "LifeCycleArchive");
UA_Server_addObjectNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 58), true); //remove HasTypeDefinition refs generated by addObjectNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5003), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 1003), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5003), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=5003/1:LifeCycleArchive using i=47/HasComponent
// Node: opcua_node_variable_t(ns=2;i=6030), 1:LifeCycleEntries
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "LifeCycleEntries");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = 1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6030);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 63);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 5003);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "LifeCycleEntries");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6030), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 63), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6030), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1004/1:AssetAdministrationShellType using i=47/HasComponent
// Node: opcua_node_object_t(ns=2;i=5005), 1:Views
UA_ObjectAttributes attr;
UA_ObjectAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Views");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 5005);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 61);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1004);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "Views");
UA_Server_addObjectNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 58), true); //remove HasTypeDefinition refs generated by addObjectNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5005), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 61), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5005), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 80), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1003/1:LifeCycleArchiveType using i=47/HasComponent
// Node: opcua_node_variable_t(ns=2;i=6023), 1:LifeCycleEntries
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "LifeCycleEntries");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = 1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6023);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 63);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1003);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "LifeCycleEntries");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6023), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 63), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6023), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1002/1:PropertyValueStatementListType using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6020), 1:CarrierId
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "CarrierId");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6020);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1002);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "CarrierId");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6020), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6020), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1002/1:PropertyValueStatementListType using i=47/HasComponent
// Node: opcua_node_variable_t(ns=2;i=6022), 1:PropertyValueStatements
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "PropertyValueStatements");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = 1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6022);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 63);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1002);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "PropertyValueStatements");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6022), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 63), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6022), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
} while(0);

do {
// Referencing node not found. Adding Node without parent and resolving parent reference (hopefully) later.
// Node: opcua_node_object_t(ns=2;i=5001), Default Binary
UA_ObjectAttributes attr;
UA_ObjectAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Default Binary");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 5001);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 76);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(0, "Default Binary");
UA_Server_addObjectNode(server, nodeId, UA_NODEID_NULL, UA_NODEID_NULL, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 58), true); //remove HasTypeDefinition refs generated by addObjectNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5001), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 76), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5001), UA_NODEID_NUMERIC(nsIdx_0, 38), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 3005), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5001), UA_NODEID_NUMERIC(nsIdx_0, 39), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6013), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 3005), UA_NODEID_NUMERIC(nsIdx_0, 38), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 5001), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6013), UA_NODEID_NUMERIC(nsIdx_0, 39), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 5001), false);
} while(0);

do {
// Referencing node not found. Adding Node without parent and resolving parent reference (hopefully) later.
// Node: opcua_node_object_t(ns=2;i=5002), Default XML
UA_ObjectAttributes attr;
UA_ObjectAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Default XML");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 5002);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 76);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(0, "Default XML");
UA_Server_addObjectNode(server, nodeId, UA_NODEID_NULL, UA_NODEID_NULL, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 58), true); //remove HasTypeDefinition refs generated by addObjectNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5002), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 76), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5002), UA_NODEID_NUMERIC(nsIdx_0, 38), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 3005), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5002), UA_NODEID_NUMERIC(nsIdx_0, 39), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6014), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 3005), UA_NODEID_NUMERIC(nsIdx_0, 38), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 5002), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6014), UA_NODEID_NUMERIC(nsIdx_0, 39), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 5002), false);
} while(0);

do {
// Referencing node not found. Adding Node without parent and resolving parent reference (hopefully) later.
// Node: opcua_node_object_t(ns=2;i=5006), Default Binary
UA_ObjectAttributes attr;
UA_ObjectAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Default Binary");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 5006);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 76);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(0, "Default Binary");
UA_Server_addObjectNode(server, nodeId, UA_NODEID_NULL, UA_NODEID_NULL, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 58), true); //remove HasTypeDefinition refs generated by addObjectNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5006), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 76), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5006), UA_NODEID_NUMERIC(nsIdx_0, 38), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 3011), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5006), UA_NODEID_NUMERIC(nsIdx_0, 39), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6025), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 3011), UA_NODEID_NUMERIC(nsIdx_0, 38), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 5006), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6025), UA_NODEID_NUMERIC(nsIdx_0, 39), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 5006), false);
} while(0);

do {
// Referencing node not found. Adding Node without parent and resolving parent reference (hopefully) later.
// Node: opcua_node_object_t(ns=2;i=5007), Default XML
UA_ObjectAttributes attr;
UA_ObjectAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Default XML");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 5007);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 76);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(0, "Default XML");
UA_Server_addObjectNode(server, nodeId, UA_NODEID_NULL, UA_NODEID_NULL, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 58), true); //remove HasTypeDefinition refs generated by addObjectNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5007), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 76), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5007), UA_NODEID_NUMERIC(nsIdx_0, 38), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 3011), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5007), UA_NODEID_NUMERIC(nsIdx_0, 39), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6027), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 3011), UA_NODEID_NUMERIC(nsIdx_0, 38), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 5007), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6027), UA_NODEID_NUMERIC(nsIdx_0, 39), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 5007), false);
} while(0);

do {
// Referencing node not found. Adding Node without parent and resolving parent reference (hopefully) later.
// Node: opcua_node_object_t(ns=2;i=5008), Default Binary
UA_ObjectAttributes attr;
UA_ObjectAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Default Binary");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 5008);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 76);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(0, "Default Binary");
UA_Server_addObjectNode(server, nodeId, UA_NODEID_NULL, UA_NODEID_NULL, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 58), true); //remove HasTypeDefinition refs generated by addObjectNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5008), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 76), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5008), UA_NODEID_NUMERIC(nsIdx_0, 38), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 3008), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5008), UA_NODEID_NUMERIC(nsIdx_0, 39), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6031), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 3008), UA_NODEID_NUMERIC(nsIdx_0, 38), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 5008), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6031), UA_NODEID_NUMERIC(nsIdx_0, 39), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 5008), false);
} while(0);

do {
// Referencing node not found. Adding Node without parent and resolving parent reference (hopefully) later.
// Node: opcua_node_object_t(ns=2;i=5009), Default XML
UA_ObjectAttributes attr;
UA_ObjectAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Default XML");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 5009);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 76);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(0, "Default XML");
UA_Server_addObjectNode(server, nodeId, UA_NODEID_NULL, UA_NODEID_NULL, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 58), true); //remove HasTypeDefinition refs generated by addObjectNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5009), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 76), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5009), UA_NODEID_NUMERIC(nsIdx_0, 38), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 3008), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5009), UA_NODEID_NUMERIC(nsIdx_0, 39), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 6032), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 3008), UA_NODEID_NUMERIC(nsIdx_0, 38), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 5009), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6032), UA_NODEID_NUMERIC(nsIdx_0, 39), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 5009), false);
} while(0);
return UA_STATUSCODE_GOOD;
}
