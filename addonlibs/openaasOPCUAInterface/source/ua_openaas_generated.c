/* Generated from Opc.Ua.Types.bsd, Custom.Opc.Ua.AssetAdministrationShell.bsd with script /home/ubuntu/parser/tools/generate_datatypes.py
 * on host ubuntu-VirtualBox by user ubuntu at 2017-02-09 10:14:25 */

#include "stddef.h"
#include "ua_openaas_generated.h"

/* ExpressionSemanticEnum */
static UA_DataTypeMember ExpressionSemanticEnum_members[1] = {
  { .memberTypeIndex = UA_TYPES_INT32,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "",
#endif
    .namespaceZero = TRUE,
    .padding = 0,
    .isArray = FALSE
  },};

/* ExpressionLogicEnum */
static UA_DataTypeMember ExpressionLogicEnum_members[1] = {
  { .memberTypeIndex = UA_TYPES_INT32,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "",
#endif
    .namespaceZero = TRUE,
    .padding = 0,
    .isArray = FALSE
  },};

/* ViewEnum */
static UA_DataTypeMember ViewEnum_members[1] = {
  { .memberTypeIndex = UA_TYPES_INT32,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "",
#endif
    .namespaceZero = TRUE,
    .padding = 0,
    .isArray = FALSE
  },};

/* VisibilityEnum */
static UA_DataTypeMember VisibilityEnum_members[1] = {
  { .memberTypeIndex = UA_TYPES_INT32,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "",
#endif
    .namespaceZero = TRUE,
    .padding = 0,
    .isArray = FALSE
  },};

/* IdEnum */
static UA_DataTypeMember IdEnum_members[1] = {
  { .memberTypeIndex = UA_TYPES_INT32,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "",
#endif
    .namespaceZero = TRUE,
    .padding = 0,
    .isArray = FALSE
  },};

/* Identification */
static UA_DataTypeMember Identification_members[2] = {
  { .memberTypeIndex = UA_TYPES_STRING,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "idSpec",
#endif
    .namespaceZero = TRUE,
    .padding = 0,
    .isArray = FALSE
  },
  { .memberTypeIndex = UA_OPENAAS_IDENUM,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "idType",
#endif
    .namespaceZero = FALSE,
    .padding = offsetof(UA_Identification, idType) - offsetof(UA_Identification, idSpec) - sizeof(UA_String),
    .isArray = FALSE
  },};

/* LifeCycleEntry */
static UA_DataTypeMember LifeCycleEntry_members[6] = {
  { .memberTypeIndex = UA_OPENAAS_IDENTIFICATION,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "creatingInstance",
#endif
    .namespaceZero = FALSE,
    .padding = 0,
    .isArray = FALSE
  },
  { .memberTypeIndex = UA_OPENAAS_IDENTIFICATION,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "writingInstance",
#endif
    .namespaceZero = FALSE,
    .padding = offsetof(UA_LifeCycleEntry, writingInstance) - offsetof(UA_LifeCycleEntry, creatingInstance) - sizeof(UA_Identification),
    .isArray = FALSE
  },
  { .memberTypeIndex = UA_TYPES_DATAVALUE,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "data",
#endif
    .namespaceZero = TRUE,
    .padding = offsetof(UA_LifeCycleEntry, data) - offsetof(UA_LifeCycleEntry, writingInstance) - sizeof(UA_Identification),
    .isArray = FALSE
  },
  { .memberTypeIndex = UA_TYPES_STRING,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "subject",
#endif
    .namespaceZero = TRUE,
    .padding = offsetof(UA_LifeCycleEntry, subject) - offsetof(UA_LifeCycleEntry, data) - sizeof(UA_DataValue),
    .isArray = FALSE
  },
  { .memberTypeIndex = UA_TYPES_STRING,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "eventClass",
#endif
    .namespaceZero = TRUE,
    .padding = offsetof(UA_LifeCycleEntry, eventClass) - offsetof(UA_LifeCycleEntry, subject) - sizeof(UA_String),
    .isArray = FALSE
  },
  { .memberTypeIndex = UA_TYPES_INT64,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "id",
#endif
    .namespaceZero = TRUE,
    .padding = offsetof(UA_LifeCycleEntry, id) - offsetof(UA_LifeCycleEntry, eventClass) - sizeof(UA_String),
    .isArray = FALSE
  },};

/* PropertyValueStatement */
static UA_DataTypeMember PropertyValueStatement_members[7] = {
  { .memberTypeIndex = UA_OPENAAS_EXPRESSIONSEMANTICENUM,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "expressionSemantic",
#endif
    .namespaceZero = FALSE,
    .padding = 0,
    .isArray = FALSE
  },
  { .memberTypeIndex = UA_OPENAAS_EXPRESSIONLOGICENUM,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "expressionLogic",
#endif
    .namespaceZero = FALSE,
    .padding = offsetof(UA_PropertyValueStatement, expressionLogic) - offsetof(UA_PropertyValueStatement, expressionSemantic) - sizeof(UA_ExpressionSemanticEnum),
    .isArray = FALSE
  },
  { .memberTypeIndex = UA_TYPES_STRING,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "unit",
#endif
    .namespaceZero = TRUE,
    .padding = offsetof(UA_PropertyValueStatement, unit) - offsetof(UA_PropertyValueStatement, expressionLogic) - sizeof(UA_ExpressionLogicEnum),
    .isArray = FALSE
  },
  { .memberTypeIndex = UA_TYPES_VARIANT,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "value",
#endif
    .namespaceZero = TRUE,
    .padding = offsetof(UA_PropertyValueStatement, value) - offsetof(UA_PropertyValueStatement, unit) - sizeof(UA_String),
    .isArray = FALSE
  },
  { .memberTypeIndex = UA_OPENAAS_IDENTIFICATION,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "iD",
#endif
    .namespaceZero = FALSE,
    .padding = offsetof(UA_PropertyValueStatement, iD) - offsetof(UA_PropertyValueStatement, value) - sizeof(UA_Variant),
    .isArray = FALSE
  },
  { .memberTypeIndex = UA_OPENAAS_VIEWENUM,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "view",
#endif
    .namespaceZero = FALSE,
    .padding = offsetof(UA_PropertyValueStatement, view) - offsetof(UA_PropertyValueStatement, iD) - sizeof(UA_Identification),
    .isArray = FALSE
  },
  { .memberTypeIndex = UA_OPENAAS_VISIBILITYENUM,
#ifdef UA_ENABLE_TYPENAMES
    .memberName = "visibility",
#endif
    .namespaceZero = FALSE,
    .padding = offsetof(UA_PropertyValueStatement, visibility) - offsetof(UA_PropertyValueStatement, view) - sizeof(UA_ViewEnum),
    .isArray = FALSE
  },};
UA_DataType UA_OPENAAS[UA_OPENAAS_COUNT] = {

/* ExpressionSemanticEnum */
{ .typeId = {.namespaceIndex = 3, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 3004},
  .typeIndex = UA_TYPES_INT32,
#ifdef UA_ENABLE_TYPENAMES
  .typeName = "ExpressionSemanticEnum",
#endif
  .memSize = sizeof(UA_ExpressionSemanticEnum),
  .builtin = TRUE,
  .pointerFree = TRUE,
  .overlayable = UA_BINARY_OVERLAYABLE_INTEGER,
  .binaryEncodingId = 0,
  .membersSize = 1,
  .members = ExpressionSemanticEnum_members },

/* ExpressionLogicEnum */
{ .typeId = {.namespaceIndex = 3, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 3003},
  .typeIndex = UA_TYPES_INT32,
#ifdef UA_ENABLE_TYPENAMES
  .typeName = "ExpressionLogicEnum",
#endif
  .memSize = sizeof(UA_ExpressionLogicEnum),
  .builtin = TRUE,
  .pointerFree = TRUE,
  .overlayable = UA_BINARY_OVERLAYABLE_INTEGER,
  .binaryEncodingId = 0,
  .membersSize = 1,
  .members = ExpressionLogicEnum_members },

/* ViewEnum */
{ .typeId = {.namespaceIndex = 3, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 3007},
  .typeIndex = UA_TYPES_INT32,
#ifdef UA_ENABLE_TYPENAMES
  .typeName = "ViewEnum",
#endif
  .memSize = sizeof(UA_ViewEnum),
  .builtin = TRUE,
  .pointerFree = TRUE,
  .overlayable = UA_BINARY_OVERLAYABLE_INTEGER,
  .binaryEncodingId = 0,
  .membersSize = 1,
  .members = ViewEnum_members },

/* VisibilityEnum */
{ .typeId = {.namespaceIndex = 3, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 3006},
  .typeIndex = UA_TYPES_INT32,
#ifdef UA_ENABLE_TYPENAMES
  .typeName = "VisibilityEnum",
#endif
  .memSize = sizeof(UA_VisibilityEnum),
  .builtin = TRUE,
  .pointerFree = TRUE,
  .overlayable = UA_BINARY_OVERLAYABLE_INTEGER,
  .binaryEncodingId = 0,
  .membersSize = 1,
  .members = VisibilityEnum_members },

/* IdEnum */
{ .typeId = {.namespaceIndex = 3, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 3002},
  .typeIndex = UA_TYPES_INT32,
#ifdef UA_ENABLE_TYPENAMES
  .typeName = "IdEnum",
#endif
  .memSize = sizeof(UA_IdEnum),
  .builtin = TRUE,
  .pointerFree = TRUE,
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
  .builtin = FALSE,
  .pointerFree = FALSE,
  .overlayable = FALSE,
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
  .builtin = FALSE,
  .pointerFree = FALSE,
  .overlayable = FALSE,
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
  .builtin = FALSE,
  .pointerFree = FALSE,
  .overlayable = FALSE,
  .binaryEncodingId = 5006,
  .membersSize = 7,
  .members = PropertyValueStatement_members },
};

