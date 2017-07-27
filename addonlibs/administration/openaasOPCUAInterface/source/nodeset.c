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
  UA_Namespace* nsArray = UA_malloc(2 * sizeof(UA_Namespace));
  UA_String tempNsUri;

  //Adding namespace for old namespace index = 0 with uri: http://opcfoundation.org/UA/
  tempNsUri = UA_String_fromChars("http://opcfoundation.org/UA/");
  UA_Namespace_init(&nsArray[0], &tempNsUri);
  UA_String_deleteMembers(&tempNsUri);
  retval |= UA_Server_addNamespace_full(server, &nsArray[0]);
  UA_UInt16 nsIdx_0 = nsArray[0].index;
  //Adding namespace for old namespace index = 2 with uri: http://acplt.org/openaas/
  tempNsUri = UA_String_fromChars("http://acplt.org/openaas/");
  UA_Namespace_init(&nsArray[1], &tempNsUri);
  UA_String_deleteMembers(&tempNsUri);
  retval |= UA_Server_addNamespace_full(server, &nsArray[1]);
  UA_UInt16 nsIdx_2 = nsArray[1].index;

  //Adding namespace for old namespace index = 2 with uri: http://acplt.org/openaas/
  UA_Namespace nsArraytmp;
  tempNsUri = UA_String_fromChars("http://acplt.org/propertyValueStatement/");
  UA_Namespace_init(&nsArraytmp, &tempNsUri);
  UA_String_deleteMembers(&tempNsUri);
  retval |= UA_Server_addNamespace_full(server, &nsArraytmp);
  UA_UInt16 nsIdx_4 = nsArraytmp.index;
  UA_Namespace_deleteMembers(&nsArraytmp);
  //Adding namespace for old namespace index = 2 with uri: http://acplt.org/openaas/
  tempNsUri = UA_String_fromChars("http://acplt.org/lifeCycleEntry/");
  UA_Namespace_init(&nsArraytmp, &tempNsUri);
  UA_String_deleteMembers(&tempNsUri);
  retval |= UA_Server_addNamespace_full(server, &nsArraytmp);
  UA_UInt16 nsIdx_5 = nsArraytmp.index;
  UA_Namespace_deleteMembers(&nsArraytmp);


  //Writing back desired namespace values')
  if(namespacesSize) {*namespacesSize = 2;};
  if(namespaces) {namespaces = &nsArray;}
  else {
    for(size_t i = 0 ; i < 2 ; ++i){
      UA_Namespace_deleteMembers(&nsArray[i]);
    }
    UA_free(nsArray);
  }
  if(retval == UA_STATUSCODE_GOOD){retval = UA_STATUSCODE_GOOD;} //ensure that retval is used
  

do {
// Referencing node found and declared as parent: i=29/Enumeration using i=45/HasSubtype
// Node: opcua_node_dataType_t(ns=2;i=3002), 1:ViewEnum
UA_DataTypeAttributes attr;
UA_DataTypeAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "ViewEnum");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 3002);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 29);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 45);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "ViewEnum");
UA_Server_addDataTypeNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, NULL, NULL);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=3002/1:ViewEnum using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6001), EnumStrings
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "EnumStrings");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = 1;
UA_LocalizedText opcua_node_variable_t_ns_2_i_6001_variant_DataContents[9];
opcua_node_variable_t_ns_2_i_6001_variant_DataContents[0] = UA_LOCALIZEDTEXT_ALLOC("en_US", "BUSINESS");
opcua_node_variable_t_ns_2_i_6001_variant_DataContents[1] = UA_LOCALIZEDTEXT_ALLOC("en_US", "CONSTRUCTION");
opcua_node_variable_t_ns_2_i_6001_variant_DataContents[2] = UA_LOCALIZEDTEXT_ALLOC("en_US", "POWER");
opcua_node_variable_t_ns_2_i_6001_variant_DataContents[3] = UA_LOCALIZEDTEXT_ALLOC("en_US", "FUNCTIONAL");
opcua_node_variable_t_ns_2_i_6001_variant_DataContents[4] = UA_LOCALIZEDTEXT_ALLOC("en_US", "LOCATION");
opcua_node_variable_t_ns_2_i_6001_variant_DataContents[5] = UA_LOCALIZEDTEXT_ALLOC("en_US", "SECURITY");
opcua_node_variable_t_ns_2_i_6001_variant_DataContents[6] = UA_LOCALIZEDTEXT_ALLOC("en_US", "NETWORK");
opcua_node_variable_t_ns_2_i_6001_variant_DataContents[7] = UA_LOCALIZEDTEXT_ALLOC("en_US", "LIFECYCLE");
opcua_node_variable_t_ns_2_i_6001_variant_DataContents[8] = UA_LOCALIZEDTEXT_ALLOC("en_US", "HUMAN");
UA_Variant_setArray( &attr.value, &opcua_node_variable_t_ns_2_i_6001_variant_DataContents, (UA_Int32) 9, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6001);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 3002);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(0, "EnumStrings");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6001), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6001), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: i=29/Enumeration using i=45/HasSubtype
// Node: opcua_node_dataType_t(ns=2;i=3003), 1:VisibilityEnum
UA_DataTypeAttributes attr;
UA_DataTypeAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "VisibilityEnum");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 3003);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 29);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 45);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "VisibilityEnum");
UA_Server_addDataTypeNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, NULL, NULL);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=3003/1:VisibilityEnum using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6006), EnumStrings
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "EnumStrings");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = 1;
UA_LocalizedText opcua_node_variable_t_ns_2_i_6006_variant_DataContents[3];
opcua_node_variable_t_ns_2_i_6006_variant_DataContents[0] = UA_LOCALIZEDTEXT_ALLOC("en_US", "PRIVAT");
opcua_node_variable_t_ns_2_i_6006_variant_DataContents[1] = UA_LOCALIZEDTEXT_ALLOC("en_US", "CONTRACT");
opcua_node_variable_t_ns_2_i_6006_variant_DataContents[2] = UA_LOCALIZEDTEXT_ALLOC("en_US", "PUBLIC");
UA_Variant_setArray( &attr.value, &opcua_node_variable_t_ns_2_i_6006_variant_DataContents, (UA_Int32) 3, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6006);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 3003);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(0, "EnumStrings");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6006), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6006), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);
/*
do {
// Referencing node found and declared as parent: i=93/OPC Binary using i=47/HasComponent
// Node: opcua_node_variable_t(ns=2;i=6002), 1:TypeDictionary
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "TypeDictionary");
attr.description = UA_LOCALIZEDTEXT("", "Collects the data type descriptions of http://acplt.org/openaas/");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_ByteString *opcua_node_variable_t_ns_2_i_6002_variant_DataContents =  UA_ByteString_new();
*opcua_node_variable_t_ns_2_i_6002_variant_DataContents = UA_STRING_ALLOC("PG9wYzpUeXBlRGljdGlvbmFyeSB4bWxuczp4c2k9Imh0dHA6Ly93d3cudzMub3JnLzIwMDEvWE1MU2NoZ        W1hLWluc3RhbmNlIiB4bWxuczp0bnM9Imh0dHA6Ly95b3Vyb3JnYW5pc2F0aW9uLm9yZy9vc        GVuYWFzLyIgRGVmYXVsdEJ5dGVPcmRlcj0iTGl0dGxlRW5kaWFuIiB4bWxuczpvcGM9Imh0d        HA6Ly9vcGNmb3VuZGF0aW9uLm9yZy9CaW5hcnlTY2hlbWEvIiB4bWxuczp1YT0iaHR0cDovL        29wY2ZvdW5kYXRpb24ub3JnL1VBLyIgVGFyZ2V0TmFtZXNwYWNlPSJodHRwOi8veW91cm9yZ        2FuaXNhdGlvbi5vcmcvb3BlbmFhcy8iPgogPG9wYzpJbXBvcnQgTmFtZXNwYWNlPSJodHRwO        i8vb3BjZm91bmRhdGlvbi5vcmcvVUEvIi8+CiA8b3BjOkVudW1lcmF0ZWRUeXBlIExlbmd0a        EluQml0cz0iMzIiIE5hbWU9IlZpZXdFbnVtIj4KICA8b3BjOkVudW1lcmF0ZWRWYWx1ZSBOY        W1lPSJCVVNJTkVTUyIgVmFsdWU9IjAiLz4KICA8b3BjOkVudW1lcmF0ZWRWYWx1ZSBOYW1lP        SJDT05TVFJVQ1RJT04iIFZhbHVlPSIxIi8+CiAgPG9wYzpFbnVtZXJhdGVkVmFsdWUgTmFtZ        T0iUE9XRVIiIFZhbHVlPSIyIi8+CiAgPG9wYzpFbnVtZXJhdGVkVmFsdWUgTmFtZT0iRlVOQ        1RJT05BTCIgVmFsdWU9IjMiLz4KICA8b3BjOkVudW1lcmF0ZWRWYWx1ZSBOYW1lPSJMT0NBV        ElPTiIgVmFsdWU9IjQiLz4KICA8b3BjOkVudW1lcmF0ZWRWYWx1ZSBOYW1lPSJTRUNVUklUW        SIgVmFsdWU9IjUiLz4KICA8b3BjOkVudW1lcmF0ZWRWYWx1ZSBOYW1lPSJORVRXT1JLIiBWY        Wx1ZT0iNiIvPgogIDxvcGM6RW51bWVyYXRlZFZhbHVlIE5hbWU9IkxJRkVDWUNMRSIgVmFsd        WU9IjciLz4KICA8b3BjOkVudW1lcmF0ZWRWYWx1ZSBOYW1lPSJIVU1BTiIgVmFsdWU9IjgiL        z4KIDwvb3BjOkVudW1lcmF0ZWRUeXBlPgogPG9wYzpFbnVtZXJhdGVkVHlwZSBMZW5ndGhJb        kJpdHM9IjMyIiBOYW1lPSJWaXNpYmlsaXR5RW51bSI+CiAgPG9wYzpFbnVtZXJhdGVkVmFsd        WUgTmFtZT0iUFJJVkFUIiBWYWx1ZT0iMCIvPgogIDxvcGM6RW51bWVyYXRlZFZhbHVlIE5hb        WU9IkNPTlRSQUNUIiBWYWx1ZT0iMSIvPgogIDxvcGM6RW51bWVyYXRlZFZhbHVlIE5hbWU9I        lBVQkxJQyIgVmFsdWU9IjIiLz4KIDwvb3BjOkVudW1lcmF0ZWRUeXBlPgo8L29wYzpUeXBlR        GljdGlvbmFyeT4K");
UA_Variant_setScalar( &attr.value, opcua_node_variable_t_ns_2_i_6002_variant_DataContents, &UA_TYPES[UA_TYPES_BYTESTRING]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6002);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 72);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 93);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "TypeDictionary");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6002), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 72), true);
} while(0);
*/

do {
// Referencing node found and declared as parent: ns=2;i=6002/1:TypeDictionary using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6003), NamespaceUri
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "NamespaceUri");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_String *opcua_node_variable_t_ns_2_i_6003_variant_DataContents =  UA_String_new();
*opcua_node_variable_t_ns_2_i_6003_variant_DataContents = UA_STRING_ALLOC("http://acplt.org/openaas/");
UA_Variant_setScalar( &attr.value, opcua_node_variable_t_ns_2_i_6003_variant_DataContents, &UA_TYPES[UA_TYPES_STRING]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6003);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 6002);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(0, "NamespaceUri");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6003), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: i=92/XML Schema using i=47/HasComponent
// Node: opcua_node_variable_t(ns=2;i=6004), 1:TypeDictionary
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "TypeDictionary");
attr.description = UA_LOCALIZEDTEXT("", "Collects the data type descriptions of http://acplt.org/openaas/");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_ByteString *opcua_node_variable_t_ns_2_i_6004_variant_DataContents =  UA_ByteString_new();
*opcua_node_variable_t_ns_2_i_6004_variant_DataContents = UA_STRING_ALLOC("PHhzOnNjaGVtYSBlbGVtZW50Rm9ybURlZmF1bHQ9InF1YWxpZmllZCIgdGFyZ2V0TmFtZXNwYWNlPSJod        HRwOi8veW91cm9yZ2FuaXNhdGlvbi5vcmcvb3BlbmFhcy9UeXBlcy54c2QiIHhtbG5zOnRuc        z0iaHR0cDovL3lvdXJvcmdhbmlzYXRpb24ub3JnL29wZW5hYXMvVHlwZXMueHNkIiB4bWxuc        zp1YT0iaHR0cDovL29wY2ZvdW5kYXRpb24ub3JnL1VBLzIwMDgvMDIvVHlwZXMueHNkIiB4b        Wxuczp4cz0iaHR0cDovL3d3dy53My5vcmcvMjAwMS9YTUxTY2hlbWEiPgogPHhzOmltcG9yd        CBuYW1lc3BhY2U9Imh0dHA6Ly9vcGNmb3VuZGF0aW9uLm9yZy9VQS8yMDA4LzAyL1R5cGVzL        nhzZCIvPgogPHhzOnNpbXBsZVR5cGUgbmFtZT0iVmlld0VudW0iPgogIDx4czpyZXN0cmljd        GlvbiBiYXNlPSJ4czpzdHJpbmciPgogICA8eHM6ZW51bWVyYXRpb24gdmFsdWU9IkJVU0lOR        VNTXzAiLz4KICAgPHhzOmVudW1lcmF0aW9uIHZhbHVlPSJDT05TVFJVQ1RJT05fMSIvPgogI        CA8eHM6ZW51bWVyYXRpb24gdmFsdWU9IlBPV0VSXzIiLz4KICAgPHhzOmVudW1lcmF0aW9uI        HZhbHVlPSJGVU5DVElPTkFMXzMiLz4KICAgPHhzOmVudW1lcmF0aW9uIHZhbHVlPSJMT0NBV        ElPTl80Ii8+CiAgIDx4czplbnVtZXJhdGlvbiB2YWx1ZT0iU0VDVVJJVFlfNSIvPgogICA8e        HM6ZW51bWVyYXRpb24gdmFsdWU9Ik5FVFdPUktfNiIvPgogICA8eHM6ZW51bWVyYXRpb24gd        mFsdWU9IkxJRkVDWUNMRV83Ii8+CiAgIDx4czplbnVtZXJhdGlvbiB2YWx1ZT0iSFVNQU5fO        CIvPgogIDwveHM6cmVzdHJpY3Rpb24+CiA8L3hzOnNpbXBsZVR5cGU+CiA8eHM6ZWxlbWVud        CB0eXBlPSJ0bnM6Vmlld0VudW0iIG5hbWU9IlZpZXdFbnVtIi8+CiA8eHM6Y29tcGxleFR5c        GUgbmFtZT0iTGlzdE9mVmlld0VudW0iPgogIDx4czpzZXF1ZW5jZT4KICAgPHhzOmVsZW1lb        nQgbWluT2NjdXJzPSIwIiBtYXhPY2N1cnM9InVuYm91bmRlZCIgdHlwZT0idG5zOlZpZXdFb        nVtIiBuYW1lPSJWaWV3RW51bSIgbmlsbGFibGU9InRydWUiLz4KICA8L3hzOnNlcXVlbmNlP        gogPC94czpjb21wbGV4VHlwZT4KIDx4czplbGVtZW50IHR5cGU9InRuczpMaXN0T2ZWaWV3R        W51bSIgbmFtZT0iTGlzdE9mVmlld0VudW0iIG5pbGxhYmxlPSJ0cnVlIi8+CiA8eHM6c2ltc        GxlVHlwZSBuYW1lPSJWaXNpYmlsaXR5RW51bSI+CiAgPHhzOnJlc3RyaWN0aW9uIGJhc2U9I        nhzOnN0cmluZyI+CiAgIDx4czplbnVtZXJhdGlvbiB2YWx1ZT0iUFJJVkFUXzAiLz4KICAgP        HhzOmVudW1lcmF0aW9uIHZhbHVlPSJDT05UUkFDVF8xIi8+CiAgIDx4czplbnVtZXJhdGlvb        iB2YWx1ZT0iUFVCTElDXzIiLz4KICA8L3hzOnJlc3RyaWN0aW9uPgogPC94czpzaW1wbGVUe        XBlPgogPHhzOmVsZW1lbnQgdHlwZT0idG5zOlZpc2liaWxpdHlFbnVtIiBuYW1lPSJWaXNpY        mlsaXR5RW51bSIvPgogPHhzOmNvbXBsZXhUeXBlIG5hbWU9Ikxpc3RPZlZpc2liaWxpdHlFb        nVtIj4KICA8eHM6c2VxdWVuY2U+CiAgIDx4czplbGVtZW50IG1pbk9jY3Vycz0iMCIgbWF4T        2NjdXJzPSJ1bmJvdW5kZWQiIHR5cGU9InRuczpWaXNpYmlsaXR5RW51bSIgbmFtZT0iVmlza        WJpbGl0eUVudW0iIG5pbGxhYmxlPSJ0cnVlIi8+CiAgPC94czpzZXF1ZW5jZT4KIDwveHM6Y        29tcGxleFR5cGU+CiA8eHM6ZWxlbWVudCB0eXBlPSJ0bnM6TGlzdE9mVmlzaWJpbGl0eUVud        W0iIG5hbWU9Ikxpc3RPZlZpc2liaWxpdHlFbnVtIiBuaWxsYWJsZT0idHJ1ZSIvPgo8L3hzO        nNjaGVtYT4K");
UA_Variant_setScalar( &attr.value, opcua_node_variable_t_ns_2_i_6004_variant_DataContents, &UA_TYPES[UA_TYPES_BYTESTRING]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6004);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 72);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 92);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "TypeDictionary");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6004), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 72), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=6004/1:TypeDictionary using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6005), NamespaceUri
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "NamespaceUri");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_String *opcua_node_variable_t_ns_2_i_6005_variant_DataContents =  UA_String_new();
*opcua_node_variable_t_ns_2_i_6005_variant_DataContents = UA_STRING_ALLOC("http://acplt.org/openaas/Types.xsd");
UA_Variant_setScalar( &attr.value, opcua_node_variable_t_ns_2_i_6005_variant_DataContents, &UA_TYPES[UA_TYPES_STRING]);
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6005);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 6004);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(0, "NamespaceUri");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6005), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: i=58/BaseObjectType using i=45/HasSubtype
// Node: opcua_node_objectType_t(ns=2;i=1002), 1:AssetAdministrationShellType
UA_ObjectTypeAttributes attr;
UA_ObjectTypeAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "AssetAdministrationShellType");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 1002);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 58);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 45);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "AssetAdministrationShellType");
UA_Server_addObjectTypeNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, NULL, NULL);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1002/1:AssetAdministrationShellType using i=47/HasComponent
// Node: opcua_node_object_t(ns=2;i=5001), 1:Body
UA_ObjectAttributes attr;
UA_ObjectAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Body");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 5001);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 61);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1002);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "Body");
UA_Server_addObjectNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 58), true); //remove HasTypeDefinition refs generated by addObjectNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5001), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 61), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5001), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1002/1:AssetAdministrationShellType using i=47/HasComponent
// Node: opcua_node_object_t(ns=2;i=5002), 1:Header
UA_ObjectAttributes attr;
UA_ObjectAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Header");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 5002);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_4, 1002);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1002);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "Header");
UA_Server_addObjectNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 58), true); //remove HasTypeDefinition refs generated by addObjectNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5002), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5002), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 1002), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1002/1:AssetAdministrationShellType using i=47/HasComponent
// Node: opcua_node_object_t(ns=2;i=5003), 1:LifeCycleArchive
UA_ObjectAttributes attr;
UA_ObjectAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "LifeCycleArchive");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 5003);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_5, 1002);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1002);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "LifeCycleArchive");
UA_Server_addObjectNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 58), true); //remove HasTypeDefinition refs generated by addObjectNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5003), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_2, 1002), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5003), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=5003/1:LifeCycleArchive using i=47/HasComponent
// Node: opcua_node_variable_t(ns=2;i=6007), 3:LifeCycleEntries
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "LifeCycleEntries");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6007);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 63);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 5003);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "LifeCycleEntries");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6007), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 63), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6007), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 11510), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1002/1:AssetAdministrationShellType using i=47/HasComponent
// Node: opcua_node_object_t(ns=2;i=5004), 1:Views
UA_ObjectAttributes attr;
UA_ObjectAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Views");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 5004);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 61);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1002);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 47);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "Views");
UA_Server_addObjectNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 58), true); //remove HasTypeDefinition refs generated by addObjectNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5004), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 61), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 5004), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 80), true);
} while(0);

do {
// Referencing node found and declared as parent: i=58/BaseObjectType using i=45/HasSubtype
// Node: opcua_node_objectType_t(ns=2;i=1004), 1:ServiceType
UA_ObjectTypeAttributes attr;
UA_ObjectTypeAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "ServiceType");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 1004);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 58);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 45);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "ServiceType");
UA_Server_addObjectTypeNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, NULL, NULL);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1004/1:ServiceType using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6012), 1:ID
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "ID");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6012);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1004);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "ID");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6012), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6012), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1004/1:ServiceType using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6013), 1:Name
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Name");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6013);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1004);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "Name");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6013), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6013), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1004/1:ServiceType using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6014), 1:Revision
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Revision");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6014);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1004);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "Revision");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6014), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6014), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1004/1:ServiceType using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6015), 1:Version
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Version");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6015);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1004);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "Version");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6015), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6015), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1004/1:ServiceType using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6016), 1:WSDLDescription
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "WSDLDescription");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6016);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1004);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "WSDLDescription");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6016), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6016), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: i=58/BaseObjectType using i=45/HasSubtype
// Node: opcua_node_objectType_t(ns=2;i=1003), 1:SubModelType
UA_ObjectTypeAttributes attr;
UA_ObjectTypeAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "SubModelType");
attr.description = UA_LOCALIZEDTEXT("", "");
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 1003);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_0, 58);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 45);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "SubModelType");
UA_Server_addObjectTypeNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , attr, NULL, NULL);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1003/1:SubModelType using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6008), 1:ID
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "ID");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6008);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1003);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "ID");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6008), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6008), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1003/1:SubModelType using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6009), 1:Name
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Name");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6009);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1003);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "Name");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6009), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6009), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1003/1:SubModelType using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6010), 1:Revision
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Revision");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6010);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1003);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "Revision");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6010), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6010), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);

do {
// Referencing node found and declared as parent: ns=2;i=1003/1:SubModelType using i=46/HasProperty
// Node: opcua_node_variable_t(ns=2;i=6011), 1:Version
UA_VariableAttributes attr;
UA_VariableAttributes_init(&attr);
attr.displayName = UA_LOCALIZEDTEXT("", "Version");
attr.description = UA_LOCALIZEDTEXT("", "");
attr.accessLevel = 3;
attr.valueRank = -1;
UA_NodeId nodeId = UA_NODEID_NUMERIC(nsIdx_2, 6011);
UA_NodeId typeDefinition = UA_NODEID_NUMERIC(nsIdx_0, 68);
UA_NodeId parentNodeId = UA_NODEID_NUMERIC(nsIdx_2, 1003);
UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(nsIdx_0, 46);
UA_QualifiedName nodeName = UA_QUALIFIEDNAME(nsIdx_2, "Version");
UA_Server_addVariableNode(server, nodeId, parentNodeId, parentReferenceNodeId, nodeName
       , typeDefinition
       , attr, NULL, NULL);
UA_Server_deleteReference(server, nodeId, UA_NODEID_NUMERIC(0, 40), true, UA_EXPANDEDNODEID_NUMERIC(0, 62), true); //remove HasTypeDefinition refs generated by addVariableNode
// This node has the following references that can be created:
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6011), UA_NODEID_NUMERIC(nsIdx_0, 37), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 78), true);
UA_Server_addReference(server, UA_NODEID_NUMERIC(nsIdx_2, 6011), UA_NODEID_NUMERIC(nsIdx_0, 40), UA_EXPANDEDNODEID_NUMERIC(nsIdx_0, 68), true);
} while(0);
return UA_STATUSCODE_GOOD;
}
