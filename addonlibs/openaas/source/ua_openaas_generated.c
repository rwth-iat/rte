/* Generated from Opc.Ua.Types.bsd, Custom.Opc.Ua.AssetAdministrationShell.bsd with script /home/opcua/Documents/openAAS_workshop/external/open62541/tools/generate_datatypes.py
 * on host opcua-VirtualBox by user opcua at 2017-02-03 12:43:52 */

#include "stddef.h"
#include "ua_openaas_generated.h"

/* ExpressionSemanticEnum */
static UA_DataTypeMember ExpressionSemanticEnum_members[1] = {
  { .memberTypeIndex = UA_TYPES_INT32,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "",
#endif
    .namespaceZero = true,
    .padding = 0,
    .isArray = false
  },};

/* ViewEnum */
static UA_DataTypeMember ViewEnum_members[1] = {
  { .memberTypeIndex = UA_TYPES_INT32,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "",
#endif
    .namespaceZero = true,
    .padding = 0,
    .isArray = false
  },};

/* RelationalExpressionEnum */
static UA_DataTypeMember RelationalExpressionEnum_members[1] = {
  { .memberTypeIndex = UA_TYPES_INT32,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "",
#endif
    .namespaceZero = true,
    .padding = 0,
    .isArray = false
  },};

/* IdEnum */
static UA_DataTypeMember IdEnum_members[1] = {
  { .memberTypeIndex = UA_TYPES_INT32,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "",
#endif
    .namespaceZero = true,
    .padding = 0,
    .isArray = false
  },};

/* Identification */
static UA_DataTypeMember Identification_members[2] = {
  { .memberTypeIndex = UA_TYPES_STRING,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "idSpec",
#endif
    .namespaceZero = true,
    .padding = 0,
    .isArray = false
  },
  { .memberTypeIndex = UA_OPENAAS_IDENUM,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "idType",
#endif
    .namespaceZero = false,
    .padding = offsetof(UA_Identification, idType) - offsetof(UA_Identification, idSpec) - sizeof(UA_String),
    .isArray = false
  },};

/* LifeCycleEntry */
static UA_DataTypeMember LifeCycleEntry_members[6] = {
  { .memberTypeIndex = UA_OPENAAS_IDENTIFICATION,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "creatingInstance",
#endif
    .namespaceZero = false,
    .padding = 0,
    .isArray = false
  },
  { .memberTypeIndex = UA_OPENAAS_IDENTIFICATION,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "writingInstance",
#endif
    .namespaceZero = false,
    .padding = offsetof(UA_LifeCycleEntry, writingInstance) - offsetof(UA_LifeCycleEntry, creatingInstance) - sizeof(UA_Identification),
    .isArray = false
  },
  { .memberTypeIndex = UA_TYPES_DATAVALUE,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "data",
#endif
    .namespaceZero = true,
    .padding = offsetof(UA_LifeCycleEntry, data) - offsetof(UA_LifeCycleEntry, writingInstance) - sizeof(UA_Identification),
    .isArray = false
  },
  { .memberTypeIndex = UA_TYPES_STRING,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "subject",
#endif
    .namespaceZero = true,
    .padding = offsetof(UA_LifeCycleEntry, subject) - offsetof(UA_LifeCycleEntry, data) - sizeof(UA_DataValue),
    .isArray = false
  },
  { .memberTypeIndex = UA_TYPES_STRING,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "eventClass",
#endif
    .namespaceZero = true,
    .padding = offsetof(UA_LifeCycleEntry, eventClass) - offsetof(UA_LifeCycleEntry, subject) - sizeof(UA_String),
    .isArray = false
  },
  { .memberTypeIndex = UA_OPENAAS_VIEWENUM,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "view",
#endif
    .namespaceZero = false,
    .padding = offsetof(UA_LifeCycleEntry, view) - offsetof(UA_LifeCycleEntry, eventClass) - sizeof(UA_String),
    .isArray = false
  },};

/* PropertyValueStatement */
static UA_DataTypeMember PropertyValueStatement_members[7] = {
  { .memberTypeIndex = UA_TYPES_UINT32,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "expressionSemantic",
#endif
    .namespaceZero = true,
    .padding = 0,
    .isArray = false
  },
  { .memberTypeIndex = UA_TYPES_UINT32,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "relationalExpression",
#endif
    .namespaceZero = true,
    .padding = offsetof(UA_PropertyValueStatement, relationalExpression) - offsetof(UA_PropertyValueStatement, expressionSemantic) - sizeof(UA_UInt32),
    .isArray = false
  },
  { .memberTypeIndex = UA_TYPES_STRING,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "unit",
#endif
    .namespaceZero = true,
    .padding = offsetof(UA_PropertyValueStatement, unit) - offsetof(UA_PropertyValueStatement, relationalExpression) - sizeof(UA_UInt32),
    .isArray = false
  },
  { .memberTypeIndex = UA_TYPES_VARIANT,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "value",
#endif
    .namespaceZero = true,
    .padding = offsetof(UA_PropertyValueStatement, value) - offsetof(UA_PropertyValueStatement, unit) - sizeof(UA_String),
    .isArray = false
  },
  { .memberTypeIndex = UA_OPENAAS_IDENTIFICATION,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "propertyReference",
#endif
    .namespaceZero = false,
    .padding = offsetof(UA_PropertyValueStatement, propertyReference) - offsetof(UA_PropertyValueStatement, value) - sizeof(UA_Variant),
    .isArray = false
  },
  { .memberTypeIndex = UA_OPENAAS_VIEWENUM,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "view",
#endif
    .namespaceZero = false,
    .padding = offsetof(UA_PropertyValueStatement, view) - offsetof(UA_PropertyValueStatement, propertyReference) - sizeof(UA_Identification),
    .isArray = false
  },
  { .memberTypeIndex = UA_TYPES_BOOLEAN,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "isPublic",
#endif
    .namespaceZero = true,
    .padding = offsetof(UA_PropertyValueStatement, isPublic) - offsetof(UA_PropertyValueStatement, view) - sizeof(UA_ViewEnum),
    .isArray = false
  },};
const UA_DataType UA_OPENAAS[UA_OPENAAS_COUNT] = {

/* ExpressionSemanticEnum */
{ .typeId = {.namespaceIndex = 3, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 3004},
  .typeIndex = UA_TYPES_INT32,
#ifdef UA_ENABLE_TYPENAMES
  .typeName = "ExpressionSemanticEnum",
#endif
  .memSize = sizeof(UA_ExpressionSemanticEnum),
  .builtin = true,
  .pointerFree = true,
  .overlayable = UA_BINARY_OVERLAYABLE_INTEGER,
  .binaryEncodingId = 0,
  .membersSize = 1,
  .members = ExpressionSemanticEnum_members },

/* ViewEnum */
{ .typeId = {.namespaceIndex = 3, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 3007},
  .typeIndex = UA_TYPES_INT32,
#ifdef UA_ENABLE_TYPENAMES
  .typeName = "ViewEnum",
#endif
  .memSize = sizeof(UA_ViewEnum),
  .builtin = true,
  .pointerFree = true,
  .overlayable = UA_BINARY_OVERLAYABLE_INTEGER,
  .binaryEncodingId = 0,
  .membersSize = 1,
  .members = ViewEnum_members },

/* RelationalExpressionEnum */
{ .typeId = {.namespaceIndex = 3, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 3003},
  .typeIndex = UA_TYPES_INT32,
#ifdef UA_ENABLE_TYPENAMES
  .typeName = "RelationalExpressionEnum",
#endif
  .memSize = sizeof(UA_RelationalExpressionEnum),
  .builtin = true,
  .pointerFree = true,
  .overlayable = UA_BINARY_OVERLAYABLE_INTEGER,
  .binaryEncodingId = 0,
  .membersSize = 1,
  .members = RelationalExpressionEnum_members },

/* IdEnum */
{ .typeId = {.namespaceIndex = 3, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 3002},
  .typeIndex = UA_TYPES_INT32,
#ifdef UA_ENABLE_TYPENAMES
  .typeName = "IdEnum",
#endif
  .memSize = sizeof(UA_IdEnum),
  .builtin = true,
  .pointerFree = true,
  .overlayable = UA_BINARY_OVERLAYABLE_INTEGER,
  .binaryEncodingId = 0,
  .membersSize = 1,
  .members = IdEnum_members },

/* Identification */
{ .typeId = {.namespaceIndex = 3, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 3005},
  .typeIndex = UA_OPENAAS_IDENTIFICATION,
#ifdef UA_ENABLE_TYPENAMES
  .typeName = "Identification",
#endif
  .memSize = sizeof(UA_Identification),
  .builtin = false,
  .pointerFree = false,
  .overlayable = false,
  .binaryEncodingId = 5001,
  .membersSize = 2,
  .members = Identification_members },

/* LifeCycleEntry */
{ .typeId = {.namespaceIndex = 3, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 3008},
  .typeIndex = UA_OPENAAS_LIFECYCLEENTRY,
#ifdef UA_ENABLE_TYPENAMES
  .typeName = "LifeCycleEntry",
#endif
  .memSize = sizeof(UA_LifeCycleEntry),
  .builtin = false,
  .pointerFree = false,
  .overlayable = false,
  .binaryEncodingId = 5008,
  .membersSize = 6,
  .members = LifeCycleEntry_members },

/* PropertyValueStatement */
{ .typeId = {.namespaceIndex = 3, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 3011},
  .typeIndex = UA_OPENAAS_PROPERTYVALUESTATEMENT,
#ifdef UA_ENABLE_TYPENAMES
  .typeName = "PropertyValueStatement",
#endif
  .memSize = sizeof(UA_PropertyValueStatement),
  .builtin = false,
  .pointerFree = false,
  .overlayable = false,
  .binaryEncodingId = 5006,
  .membersSize = 7,
  .members = PropertyValueStatement_members },
};

