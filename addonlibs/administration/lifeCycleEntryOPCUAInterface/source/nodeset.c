/* WARNING: This is a generated file.
 * Any manual changes will be overwritten.

 */
#include "nodeset_lifeCycleEntry.h"
UA_INLINE UA_StatusCode nodeset(UA_Server *server){
  return nodeset_returnNamespaces(server, NULL, NULL);
}

UA_INLINE UA_StatusCode nodeset_returnNamespaces(
        UA_Server *server, UA_UInt16 *namespacesSize, UA_Namespace **namespaces) {
  UA_StatusCode retval = UA_STATUSCODE_GOOD;
  UA_Namespace* nsArray = UA_malloc(3 * sizeof(UA_Namespace));
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
  //UA_UInt16 nsIdx_2 = nsArray[1].index;
  //Adding namespace for old namespace index = 3 with uri: http://acplt.org/lifeCycleEntry/
  tempNsUri = UA_String_fromChars("http://acplt.org/lifeCycleEntry/");
  UA_Namespace_init(&nsArray[2], &tempNsUri);
  UA_String_deleteMembers(&tempNsUri);
  retval |= UA_Server_addNamespace_full(server, &nsArray[2]);
  UA_UInt16 nsIdx_3 = nsArray[2].index;

  //Writing back desired namespace values')
  if(namespacesSize) {*namespacesSize = 3;};
  if(namespaces) {namespaces = &nsArray;}
  else {
    for(size_t i = 0 ; i < 3 ; ++i){
      UA_Namespace_deleteMembers(&nsArray[i]);
    }
    UA_free(nsArray);
  }
  if(retval == UA_STATUSCODE_GOOD){retval = UA_STATUSCODE_GOOD;} //ensure that retval is used
  


do {
// Referencing node found and declared as parent: i=19/StatusCode using i=45/HasSubtype
// Node: opcua_node_dataType_t(ns=3;i=3003), 1:LCEStatusCode
UA_DataTypeAttributes attr;
UA_DataTypeAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "LCEStatusCode");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_3, 3003);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 19);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 45);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_3, "LCEStatusCode");
UA_Server_addDataTypeNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, NULL, NULL);
} while(0);

do {
// Referencing node found and declared as parent: i=22/Structure using i=45/HasSubtype
// Node: opcua_node_dataType_t(ns=3;i=3002), 1:LifeCycleEntry
UA_DataTypeAttributes attr;
UA_DataTypeAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "LifeCycleEntry");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_3, 3002);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 22);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 45);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_3, "LifeCycleEntry");
UA_Server_addDataTypeNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, NULL, NULL);
} while(0);

/*
do {
// Referencing node found and declared as parent: i=93/OPC Binary using i=47/HasComponent
// Node: opcua_node_variable_t(ns=3;i=6001), 1:TypeDictionary
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "TypeDictionary");
attr.description = UA_LOCALIZEDTEXT("", "Collects the data type descriptions of http://acplt.org/lifeCycleEntry/");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_ByteString *opcua_node_variable_t_ns_3_i_6001_variant_DataContents =  UA_ByteString_new();
*opcua_node_variable_t_ns_3_i_6001_variant_DataContents = UA_STRING_ALLOC("PG9wYzpUeXBlRGljdGlvbmFyeSB4bWxuczp4c2k9Imh0dHA6Ly93d3cudzMub3JnLzIwMDEvWE1MU2NoZ        W1hLWluc3RhbmNlIiB4bWxuczp0bnM9Imh0dHA6Ly95b3Vyb3JnYW5pc2F0aW9uLm9yZy9sY        2UvIiBEZWZhdWx0Qnl0ZU9yZGVyPSJMaXR0bGVFbmRpYW4iIHhtbG5zOm9wYz0iaHR0cDovL        29wY2ZvdW5kYXRpb24ub3JnL0JpbmFyeVNjaGVtYS8iIHhtbG5zOm5zMT0iaHR0cDovL3lvd        XJvcmdhbmlzYXRpb24ub3JnL2lkZW50aWZpY2F0aW9uLyIgeG1sbnM6dWE9Imh0dHA6Ly9vc        GNmb3VuZGF0aW9uLm9yZy9VQS8iIFRhcmdldE5hbWVzcGFjZT0iaHR0cDovL3lvdXJvcmdhb        mlzYXRpb24ub3JnL2xjZS8iPgogPG9wYzpJbXBvcnQgTmFtZXNwYWNlPSJodHRwOi8vb3BjZ        m91bmRhdGlvbi5vcmcvVUEvIi8+CiA8b3BjOlN0cnVjdHVyZWRUeXBlIEJhc2VUeXBlPSJ1Y        TpFeHRlbnNpb25PYmplY3QiIE5hbWU9IkxpZmVDeWNsZUVudHJ5Ij4KICA8b3BjOkZpZWxkI        FR5cGVOYW1lPSJuczE6SWRlbnRpZmljYXRpb24iIE5hbWU9IkNyZWF0aW5nSW5zdGFuY2UiL        z4KICA8b3BjOkZpZWxkIFR5cGVOYW1lPSJuczE6SWRlbnRpZmljYXRpb24iIE5hbWU9Ildya        XRpbmdJbnN0YW5jZSIvPgogIDxvcGM6RmllbGQgVHlwZU5hbWU9InVhOkRhdGFWYWx1ZSIgT        mFtZT0iRGF0YSIvPgogIDxvcGM6RmllbGQgVHlwZU5hbWU9Im9wYzpDaGFyQXJyYXkiIE5hb        WU9IlN1YmplY3QiLz4KICA8b3BjOkZpZWxkIFR5cGVOYW1lPSJvcGM6Q2hhckFycmF5IiBOY        W1lPSJFdmVudENsYXNzIi8+CiAgPG9wYzpGaWVsZCBUeXBlTmFtZT0ib3BjOlVJbnQ2NCIgT        mFtZT0iSWQiLz4KIDwvb3BjOlN0cnVjdHVyZWRUeXBlPgogPG9wYzppbXBvcnQgTmFtZXNwY        WNlPSJodHRwOi8veW91cm9yZ2FuaXNhdGlvbi5vcmcvaWRlbnRpZmljYXRpb24vIi8+Cjwvb        3BjOlR5cGVEaWN0aW9uYXJ5Pgo=");
UA_Variant_setScalar( &attr.value, opcua_node_variable_t_ns_3_i_6001_variant_DataContents, &UA_TYPES[UA_TYPES_BYTESTRING]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_3, 6001);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 72);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 93);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_3, "TypeDictionary");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 6001), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 72), true);
} while(0);
*/

do {
// Referencing node found and declared as parent: ns=3;i=6001/1:TypeDictionary using i=47/HasComponent
// Node: opcua_node_variable_t(ns=3;i=6005), 1:LifeCycleEntry
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "LifeCycleEntry");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_String *opcua_node_variable_t_ns_3_i_6005_variant_DataContents =  UA_String_new();
*opcua_node_variable_t_ns_3_i_6005_variant_DataContents = UA_STRING_ALLOC("LifeCycleEntry");
UA_Variant_setScalar( &attr.value, opcua_node_variable_t_ns_3_i_6005_variant_DataContents, &UA_TYPES[UA_TYPES_STRING]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_3, 6005);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 69);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_3, 6001);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_3, "LifeCycleEntry");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 6005), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 69), true);
} while(0);

do {
// Referencing node found and declared as parent: i=92/XML Schema using i=47/HasComponent
// Node: opcua_node_variable_t(ns=3;i=6003), 1:TypeDictionary
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "TypeDictionary");
attr.description = UA_LOCALIZEDTEXT("", "Collects the data type descriptions of http://acplt.org/lifeCycleEntry/");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_ByteString *opcua_node_variable_t_ns_3_i_6003_variant_DataContents =  UA_ByteString_new();
*opcua_node_variable_t_ns_3_i_6003_variant_DataContents = UA_STRING_ALLOC("PHhzOnNjaGVtYSBlbGVtZW50Rm9ybURlZmF1bHQ9InF1YWxpZmllZCIgdGFyZ2V0TmFtZXNwYWNlPSJod        HRwOi8veW91cm9yZ2FuaXNhdGlvbi5vcmcvbGNlL1R5cGVzLnhzZCIgeG1sbnM6dG5zPSJod        HRwOi8veW91cm9yZ2FuaXNhdGlvbi5vcmcvbGNlL1R5cGVzLnhzZCIgeG1sbnM6bnMxPSJod        HRwOi8veW91cm9yZ2FuaXNhdGlvbi5vcmcvaWRlbnRpZmljYXRpb24vVHlwZXMueHNkIiB4b        Wxuczp1YT0iaHR0cDovL29wY2ZvdW5kYXRpb24ub3JnL1VBLzIwMDgvMDIvVHlwZXMueHNkI        iB4bWxuczp4cz0iaHR0cDovL3d3dy53My5vcmcvMjAwMS9YTUxTY2hlbWEiPgogPHhzOmltc        G9ydCBuYW1lc3BhY2U9Imh0dHA6Ly9vcGNmb3VuZGF0aW9uLm9yZy9VQS8yMDA4LzAyL1R5c        GVzLnhzZCIvPgogPHhzOmltcG9ydCBuYW1lc3BhY2U9Imh0dHA6Ly95b3Vyb3JnYW5pc2F0a        W9uLm9yZy9pZGVudGlmaWNhdGlvbi9UeXBlcy54c2QiLz4KIDx4czpjb21wbGV4VHlwZSBuY        W1lPSJMaWZlQ3ljbGVFbnRyeSI+CiAgPHhzOnNlcXVlbmNlPgogICA8eHM6ZWxlbWVudCBta        W5PY2N1cnM9IjAiIG1heE9jY3Vycz0iMSIgdHlwZT0ibnMxOklkZW50aWZpY2F0aW9uIiBuY        W1lPSJDcmVhdGluZ0luc3RhbmNlIi8+CiAgIDx4czplbGVtZW50IG1pbk9jY3Vycz0iMCIgb        WF4T2NjdXJzPSIxIiB0eXBlPSJuczE6SWRlbnRpZmljYXRpb24iIG5hbWU9IldyaXRpbmdJb        nN0YW5jZSIvPgogICA8eHM6ZWxlbWVudCBtaW5PY2N1cnM9IjAiIG1heE9jY3Vycz0iMSIgd        HlwZT0idWE6RGF0YVZhbHVlIiBuYW1lPSJEYXRhIi8+CiAgIDx4czplbGVtZW50IG1pbk9jY        3Vycz0iMCIgbWF4T2NjdXJzPSIxIiB0eXBlPSJ4czpzdHJpbmciIG5hbWU9IlN1YmplY3QiL        z4KICAgPHhzOmVsZW1lbnQgbWluT2NjdXJzPSIwIiBtYXhPY2N1cnM9IjEiIHR5cGU9InhzO        nN0cmluZyIgbmFtZT0iRXZlbnRDbGFzcyIvPgogICA8eHM6ZWxlbWVudCBtaW5PY2N1cnM9I        jAiIG1heE9jY3Vycz0iMSIgdHlwZT0ieHM6dW5zaWduZWRMb25nIiBuYW1lPSJJZCIvPgogI        DwveHM6c2VxdWVuY2U+CiA8L3hzOmNvbXBsZXhUeXBlPgogPHhzOmVsZW1lbnQgdHlwZT0id        G5zOkxpZmVDeWNsZUVudHJ5IiBuYW1lPSJMaWZlQ3ljbGVFbnRyeSIvPgogPHhzOmNvbXBsZ        XhUeXBlIG5hbWU9Ikxpc3RPZkxpZmVDeWNsZUVudHJ5Ij4KICA8eHM6c2VxdWVuY2U+CiAgI        Dx4czplbGVtZW50IG1pbk9jY3Vycz0iMCIgbWF4T2NjdXJzPSJ1bmJvdW5kZWQiIHR5cGU9I        nRuczpMaWZlQ3ljbGVFbnRyeSIgbmFtZT0iTGlmZUN5Y2xlRW50cnkiIG5pbGxhYmxlPSJ0c        nVlIi8+CiAgPC94czpzZXF1ZW5jZT4KIDwveHM6Y29tcGxleFR5cGU+CiA8eHM6ZWxlbWVud        CB0eXBlPSJ0bnM6TGlzdE9mTGlmZUN5Y2xlRW50cnkiIG5hbWU9Ikxpc3RPZkxpZmVDeWNsZ        UVudHJ5IiBuaWxsYWJsZT0idHJ1ZSIvPgo8L3hzOnNjaGVtYT4K");
UA_Variant_setScalar( &attr.value, opcua_node_variable_t_ns_3_i_6003_variant_DataContents, &UA_TYPES[UA_TYPES_BYTESTRING]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_3, 6003);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 72);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 92);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_3, "TypeDictionary");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 6003), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 72), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=3;i=6003/1:TypeDictionary using i=47/HasComponent
// Node: opcua_node_variable_t(ns=3;i=6006), 1:LifeCycleEntry
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "LifeCycleEntry");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_String *opcua_node_variable_t_ns_3_i_6006_variant_DataContents =  UA_String_new();
*opcua_node_variable_t_ns_3_i_6006_variant_DataContents = UA_STRING_ALLOC("//xs:element[@name='LifeCycleEntry']");
UA_Variant_setScalar( &attr.value, opcua_node_variable_t_ns_3_i_6006_variant_DataContents, &UA_TYPES[UA_TYPES_STRING]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_3, 6006);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 69);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_3, 6003);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_3, "LifeCycleEntry");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 6006), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 69), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=3;i=6001/1:TypeDictionary using i=46/HasProperty
// Node: opcua_node_variable_t(ns=3;i=6002), NamespaceUri
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "NamespaceUri");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_String *opcua_node_variable_t_ns_3_i_6002_variant_DataContents =  UA_String_new();
*opcua_node_variable_t_ns_3_i_6002_variant_DataContents = UA_STRING_ALLOC("http://acplt.org/lifeCycleEntry/");
UA_Variant_setScalar( &attr.value, opcua_node_variable_t_ns_3_i_6002_variant_DataContents, &UA_TYPES[UA_TYPES_STRING]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_3, 6002);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_3, 6001);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(0, "NamespaceUri");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 6002), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=3;i=6003/1:TypeDictionary using i=46/HasProperty
// Node: opcua_node_variable_t(ns=3;i=6004), NamespaceUri
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "NamespaceUri");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_String *opcua_node_variable_t_ns_3_i_6004_variant_DataContents =  UA_String_new();
*opcua_node_variable_t_ns_3_i_6004_variant_DataContents = UA_STRING_ALLOC("http://acplt.org/lifeCycleEntry/Types.xsd");
UA_Variant_setScalar( &attr.value, opcua_node_variable_t_ns_3_i_6004_variant_DataContents, &UA_TYPES[UA_TYPES_STRING]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_3, 6004);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_3, 6003);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(0, "NamespaceUri");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 6004), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: i=58/BaseObjectType using i=45/HasSubtype
// Node: opcua_node_objectType_t(ns=3;i=1002), 1:LifeCycleArchiveType
UA_ObjectTypeAttributes attr;
UA_ObjectTypeAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "LifeCycleArchiveType");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_3, 1002);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 58);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 45);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_3, "LifeCycleArchiveType");
UA_Server_addObjectTypeNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, NULL, NULL);
} while(0);

do {
// Referencing node found and declared as parent: ns=3;i=1002/1:LifeCycleArchiveType using i=47/HasComponent
// Node: opcua_node_object_t(ns=3;i=5003), 1:Archiv
UA_ObjectAttributes attr;
UA_ObjectAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Archiv");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_3, 5003);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 61);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_3, 1002);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_3, "Archiv");
UA_Server_addObjectNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 58), true); //remove HasTypeDefinition refs generated by addObjectNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 5003), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 61), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 5003), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 11510), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=3;i=5003/1:Archiv using i=47/HasComponent
// Node: opcua_node_variable_t(ns=3;i=6023), 1:<LifeCycleEntries>
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "<LifeCycleEntries>");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_3, 6023);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 63);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_3, 5003);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_3, "<LifeCycleEntries>");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 6023), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 63), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 6023), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 11510), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=3;i=1002/1:LifeCycleArchiveType using i=47/HasComponent
// Node: opcua_node_method_t(ns=3;i=7001), 1:createLCE
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<1; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("LCE");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_3, 3002);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<1; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
outputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_3, 3003);
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "createLCE");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_3, 7001);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_3, 1002);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_3, "createLCE");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 1, inputArguments,  1, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 7001), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 6008), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 7001), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 11510), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 7001), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 6009), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 6008), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 7001), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 6009), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 7001), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=3;i=1002/1:LifeCycleArchiveType using i=47/HasComponent
// Node: opcua_node_method_t(ns=3;i=7002), 1:createLCESimple
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 7);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<7; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("CreatingInstanceIdType");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_0, 7);
inputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[1].name = UA_STRING("CreatingInstanceIdSpec");
inputArguments[1].valueRank = -1;
inputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
inputArguments[2].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[2].name = UA_STRING("WritingInstanceIdType");
inputArguments[2].valueRank = -1;
inputArguments[2].dataType = UA_NODEID_NUMERIC(nsIdx_0, 7);
inputArguments[3].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[3].name = UA_STRING("WritingInstanceIdSpec");
inputArguments[3].valueRank = -1;
inputArguments[3].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
inputArguments[4].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[4].name = UA_STRING("Data");
inputArguments[4].valueRank = -1;
inputArguments[4].dataType = UA_NODEID_NUMERIC(nsIdx_0, 23);
inputArguments[5].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[5].name = UA_STRING("Subject");
inputArguments[5].valueRank = -1;
inputArguments[5].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
inputArguments[6].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[6].name = UA_STRING("EventClass");
inputArguments[6].valueRank = -1;
inputArguments[6].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<1; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
outputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_3, 3003);
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "createLCESimple");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_3, 7002);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_3, 1002);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_3, "createLCESimple");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 7, inputArguments,  1, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 7002), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 6010), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 7002), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 11510), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 7002), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 6011), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 6010), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 7002), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 6011), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 7002), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=3;i=1002/1:LifeCycleArchiveType using i=47/HasComponent
// Node: opcua_node_method_t(ns=3;i=7003), 1:deleteLCE
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<1; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("LCEId");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_0, 9);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<1; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
outputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_3, 3003);
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "deleteLCE");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_3, 7003);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_3, 1002);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_3, "deleteLCE");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 1, inputArguments,  1, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 7003), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 6012), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 7003), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 11510), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 7003), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 6013), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 6012), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 7003), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 6013), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 7003), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=3;i=1002/1:LifeCycleArchiveType using i=47/HasComponent
// Node: opcua_node_method_t(ns=3;i=7009), 1:getLastLCEs
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<1; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("Count");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_0, 7);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 3);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<3; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
outputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_3, 3003);
outputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[1].name = UA_STRING("LastLCEs");
outputArguments[1].valueRank = -1;
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
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_3, 7009);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_3, 1002);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_3, "getLastLCEs");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 1, inputArguments,  3, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 7009), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 6014), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 7009), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 11510), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 7009), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 6015), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 6014), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 7009), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 6015), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 7009), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=3;i=1002/1:LifeCycleArchiveType using i=47/HasComponent
// Node: opcua_node_method_t(ns=3;i=7005), 1:getLCE
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<1; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("LCEId");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_0, 9);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 2);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<2; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
outputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_3, 3003);
outputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[1].name = UA_STRING("LCEData");
outputArguments[1].valueRank = -1;
outputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_3, 3002);
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "getLCE");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_3, 7005);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_3, 1002);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_3, "getLCE");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 1, inputArguments,  2, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 7005), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 6016), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 7005), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 11510), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 7005), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 6017), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 6016), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 7005), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 6017), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 7005), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=3;i=1002/1:LifeCycleArchiveType using i=47/HasComponent
// Node: opcua_node_method_t(ns=3;i=7006), 1:getLCESimple
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<1; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("LCEId");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_0, 9);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 8);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<8; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
outputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_3, 3003);
outputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[1].name = UA_STRING("CreatingInstanceIdType");
outputArguments[1].valueRank = -1;
outputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_0, 7);
outputArguments[2].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[2].name = UA_STRING("CreatingInstanceIdSpec");
outputArguments[2].valueRank = -1;
outputArguments[2].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
outputArguments[3].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[3].name = UA_STRING("WritingInstanceIdType");
outputArguments[3].valueRank = -1;
outputArguments[3].dataType = UA_NODEID_NUMERIC(nsIdx_0, 7);
outputArguments[4].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[4].name = UA_STRING("WritingInstanceIdSpec");
outputArguments[4].valueRank = -1;
outputArguments[4].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
outputArguments[5].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[5].name = UA_STRING("Data");
outputArguments[5].valueRank = -1;
outputArguments[5].dataType = UA_NODEID_NUMERIC(nsIdx_0, 23);
outputArguments[6].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[6].name = UA_STRING("Subject");
outputArguments[6].valueRank = -1;
outputArguments[6].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
outputArguments[7].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[7].name = UA_STRING("EventClass");
outputArguments[7].valueRank = -1;
outputArguments[7].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "getLCESimple");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_3, 7006);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_3, 1002);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_3, "getLCESimple");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 1, inputArguments,  8, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 7006), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 6018), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 7006), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 11510), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 7006), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 6019), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 6018), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 7006), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 6019), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 7006), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=3;i=1002/1:LifeCycleArchiveType using i=47/HasComponent
// Node: opcua_node_method_t(ns=3;i=7007), 1:setLCE
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<1; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("LCEData");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_3, 3002);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<1; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
outputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_3, 3003);
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "setLCE");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_3, 7007);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_3, 1002);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_3, "setLCE");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 1, inputArguments,  1, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 7007), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 6020), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 7007), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 11510), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 7007), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 6021), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 6020), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 7007), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 6021), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 7007), false);
} while(0);

do {
// Referencing node found and declared as parent: ns=3;i=1002/1:LifeCycleArchiveType using i=47/HasComponent
// Node: opcua_node_method_t(ns=3;i=7008), 1:setLCESimple
UA_Argument *inputArguments = NULL;
UA_Argument *outputArguments = NULL;

inputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 8);
int inputArgumentCnt;
for (inputArgumentCnt=0; inputArgumentCnt<8; ++inputArgumentCnt) UA_Argument_init(&inputArguments[inputArgumentCnt]); 
inputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[0].name = UA_STRING("LCEId");
inputArguments[0].valueRank = -1;
inputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_0, 9);
inputArguments[1].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[1].name = UA_STRING("CreatingInstanceIdType");
inputArguments[1].valueRank = -1;
inputArguments[1].dataType = UA_NODEID_NUMERIC(nsIdx_0, 7);
inputArguments[2].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[2].name = UA_STRING("CreatingInstanceIdSpec");
inputArguments[2].valueRank = -1;
inputArguments[2].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
inputArguments[3].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[3].name = UA_STRING("WritingInstanceIdType");
inputArguments[3].valueRank = -1;
inputArguments[3].dataType = UA_NODEID_NUMERIC(nsIdx_0, 7);
inputArguments[4].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[4].name = UA_STRING("WritingInstanceIdSpec");
inputArguments[4].valueRank = -1;
inputArguments[4].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
inputArguments[5].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[5].name = UA_STRING("Data");
inputArguments[5].valueRank = -1;
inputArguments[5].dataType = UA_NODEID_NUMERIC(nsIdx_0, 23);
inputArguments[6].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[6].name = UA_STRING("Subject");
inputArguments[6].valueRank = -1;
inputArguments[6].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);
inputArguments[7].description = UA_LOCALIZEDTEXT("en_US","");
inputArguments[7].name = UA_STRING("EventClass");
inputArguments[7].valueRank = -1;
inputArguments[7].dataType = UA_NODEID_NUMERIC(nsIdx_0, 12);

outputArguments = (UA_Argument *) UA_malloc(sizeof(UA_Argument) * 1);
int outputArgumentCnt;
for (outputArgumentCnt=0; outputArgumentCnt<1; ++outputArgumentCnt) UA_Argument_init(&outputArguments[outputArgumentCnt]); 
outputArguments[0].description = UA_LOCALIZEDTEXT("en_US","");
outputArguments[0].name = UA_STRING("Status");
outputArguments[0].valueRank = -1;
outputArguments[0].dataType = UA_NODEID_NUMERIC(nsIdx_3, 3003);
UA_MethodAttributes attr;
UA_MethodAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "setLCESimple");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.executable = true;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_3, 7008);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_3, 1002);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_3, "setLCESimple");
UA_Server_addMethodNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, (UA_MethodCallback) NULL, NULL, 8, inputArguments,  1, outputArguments, NULL);
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 7008), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 6022), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 7008), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 11510), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 7008), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 6007), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 6022), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 7008), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 6007), UA_NODEID_NUMERIC(nsIdx_0, 46), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 7008), false);
} while(0);

do {
// Referencing node not found. Adding Node without parent and resolving parent reference (hopefully) later.
// Node: opcua_node_object_t(ns=3;i=5001), Default Binary
UA_ObjectAttributes attr;
UA_ObjectAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Default Binary");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_3, 5001);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 76);
UA_NodeId parentNodeId = UA_NODEID_NULL;
UA_NodeId parentReferenceNodeId = UA_NODEID_NULL;
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(0, "Default Binary");
UA_Server_addObjectNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 58), true); //remove HasTypeDefinition refs generated by addObjectNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 5001), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 76), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 5001), UA_NODEID_NUMERIC(nsIdx_0, 38), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 3002), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 5001), UA_NODEID_NUMERIC(nsIdx_0, 39), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 6005), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 3002), UA_NODEID_NUMERIC(nsIdx_0, 38), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 5001), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 6005), UA_NODEID_NUMERIC(nsIdx_0, 39), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 5001), false);
} while(0);

do {
// Referencing node not found. Adding Node without parent and resolving parent reference (hopefully) later.
// Node: opcua_node_object_t(ns=3;i=5002), Default XML
UA_ObjectAttributes attr;
UA_ObjectAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Default XML");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_3, 5002);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 76);
UA_NodeId parentNodeId = UA_NODEID_NULL;
UA_NodeId parentReferenceNodeId = UA_NODEID_NULL;
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(0, "Default XML");
UA_Server_addObjectNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 58), true); //remove HasTypeDefinition refs generated by addObjectNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 5002), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 76), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 5002), UA_NODEID_NUMERIC(nsIdx_0, 38), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 3002), false);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 5002), UA_NODEID_NUMERIC(nsIdx_0, 39), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 6006), true);
//  Creating this node has resolved the following open references:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 3002), UA_NODEID_NUMERIC(nsIdx_0, 38), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 5002), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_3, 6006), UA_NODEID_NUMERIC(nsIdx_0, 39), UA_EXPANDEDNODEID_NUMERIC(nsIdx_3, 5002), false);
} while(0);
return UA_STATUSCODE_GOOD;
}
