/* Generated from Opc.Ua.Types.bsd, Custom.Opc.Ua.AssetAdministrationShell.bsd with script /home/ubuntu/open62541/tools/generate_datatypes.py
 * on host ubuntu-VirtualBox by user ubuntu at 2017-05-23 02:23:01 */

#include "stddef.h"
#include "ua_openaas_generated.h"

/* ExpressionSemanticEnum */
static UA_DataTypeMember ExpressionSemanticEnum_members[1] = {
{
#ifdef UA_ENABLE_TYPENAMES
    "", /* .memberName */
#endif
    UA_TYPES_INT32, /* .memberTypeIndex */
    0, /* .padding */
    true, /* .namespaceZero */
    false /* .isArray */
},};

/* ExpressionLogicEnum */
static UA_DataTypeMember ExpressionLogicEnum_members[1] = {
{
#ifdef UA_ENABLE_TYPENAMES
    "", /* .memberName */
#endif
    UA_TYPES_INT32, /* .memberTypeIndex */
    0, /* .padding */
    true, /* .namespaceZero */
    false /* .isArray */
},};

/* ViewEnum */
static UA_DataTypeMember ViewEnum_members[1] = {
{
#ifdef UA_ENABLE_TYPENAMES
    "", /* .memberName */
#endif
    UA_TYPES_INT32, /* .memberTypeIndex */
    0, /* .padding */
    true, /* .namespaceZero */
    false /* .isArray */
},};

/* VisibilityEnum */
static UA_DataTypeMember VisibilityEnum_members[1] = {
{
#ifdef UA_ENABLE_TYPENAMES
    "", /* .memberName */
#endif
    UA_TYPES_INT32, /* .memberTypeIndex */
    0, /* .padding */
    true, /* .namespaceZero */
    false /* .isArray */
},};

/* IdEnum */
static UA_DataTypeMember IdEnum_members[1] = {
{
#ifdef UA_ENABLE_TYPENAMES
    "", /* .memberName */
#endif
    UA_TYPES_INT32, /* .memberTypeIndex */
    0, /* .padding */
    true, /* .namespaceZero */
    false /* .isArray */
},};

/* Identification */
static UA_DataTypeMember Identification_members[2] = {
{
#ifdef UA_ENABLE_TYPENAMES
    "idSpec", /* .memberName */
#endif
    UA_TYPES_STRING, /* .memberTypeIndex */
    0, /* .padding */
    true, /* .namespaceZero */
    false /* .isArray */
},
{
#ifdef UA_ENABLE_TYPENAMES
    "idType", /* .memberName */
#endif
    UA_OPENAAS_IDENUM, /* .memberTypeIndex */
    offsetof(UA_Identification, idType) - offsetof(UA_Identification, idSpec) - sizeof(UA_String), /* .padding */
    false, /* .namespaceZero */
    false /* .isArray */
},};

/* LifeCycleEntry */
static UA_DataTypeMember LifeCycleEntry_members[6] = {
{
#ifdef UA_ENABLE_TYPENAMES
    "creatingInstance", /* .memberName */
#endif
    UA_OPENAAS_IDENTIFICATION, /* .memberTypeIndex */
    0, /* .padding */
    false, /* .namespaceZero */
    false /* .isArray */
},
{
#ifdef UA_ENABLE_TYPENAMES
    "writingInstance", /* .memberName */
#endif
    UA_OPENAAS_IDENTIFICATION, /* .memberTypeIndex */
    offsetof(UA_LifeCycleEntry, writingInstance) - offsetof(UA_LifeCycleEntry, creatingInstance) - sizeof(UA_Identification), /* .padding */
    false, /* .namespaceZero */
    false /* .isArray */
},
{
#ifdef UA_ENABLE_TYPENAMES
    "data", /* .memberName */
#endif
    UA_TYPES_DATAVALUE, /* .memberTypeIndex */
    offsetof(UA_LifeCycleEntry, data) - offsetof(UA_LifeCycleEntry, writingInstance) - sizeof(UA_Identification), /* .padding */
    true, /* .namespaceZero */
    false /* .isArray */
},
{
#ifdef UA_ENABLE_TYPENAMES
    "subject", /* .memberName */
#endif
    UA_TYPES_STRING, /* .memberTypeIndex */
    offsetof(UA_LifeCycleEntry, subject) - offsetof(UA_LifeCycleEntry, data) - sizeof(UA_DataValue), /* .padding */
    true, /* .namespaceZero */
    false /* .isArray */
},
{
#ifdef UA_ENABLE_TYPENAMES
    "eventClass", /* .memberName */
#endif
    UA_TYPES_STRING, /* .memberTypeIndex */
    offsetof(UA_LifeCycleEntry, eventClass) - offsetof(UA_LifeCycleEntry, subject) - sizeof(UA_String), /* .padding */
    true, /* .namespaceZero */
    false /* .isArray */
},
{
#ifdef UA_ENABLE_TYPENAMES
    "id", /* .memberName */
#endif
    UA_TYPES_INT64, /* .memberTypeIndex */
    offsetof(UA_LifeCycleEntry, id) - offsetof(UA_LifeCycleEntry, eventClass) - sizeof(UA_String), /* .padding */
    true, /* .namespaceZero */
    false /* .isArray */
},};

/* PropertyValueStatement */
static UA_DataTypeMember PropertyValueStatement_members[7] = {
{
#ifdef UA_ENABLE_TYPENAMES
    "expressionSemantic", /* .memberName */
#endif
    UA_OPENAAS_EXPRESSIONSEMANTICENUM, /* .memberTypeIndex */
    0, /* .padding */
    false, /* .namespaceZero */
    false /* .isArray */
},
{
#ifdef UA_ENABLE_TYPENAMES
    "expressionLogic", /* .memberName */
#endif
    UA_OPENAAS_EXPRESSIONLOGICENUM, /* .memberTypeIndex */
    offsetof(UA_PropertyValueStatement, expressionLogic) - offsetof(UA_PropertyValueStatement, expressionSemantic) - sizeof(UA_ExpressionSemanticEnum), /* .padding */
    false, /* .namespaceZero */
    false /* .isArray */
},
{
#ifdef UA_ENABLE_TYPENAMES
    "unit", /* .memberName */
#endif
    UA_TYPES_STRING, /* .memberTypeIndex */
    offsetof(UA_PropertyValueStatement, unit) - offsetof(UA_PropertyValueStatement, expressionLogic) - sizeof(UA_ExpressionLogicEnum), /* .padding */
    true, /* .namespaceZero */
    false /* .isArray */
},
{
#ifdef UA_ENABLE_TYPENAMES
    "value", /* .memberName */
#endif
    UA_TYPES_VARIANT, /* .memberTypeIndex */
    offsetof(UA_PropertyValueStatement, value) - offsetof(UA_PropertyValueStatement, unit) - sizeof(UA_String), /* .padding */
    true, /* .namespaceZero */
    false /* .isArray */
},
{
#ifdef UA_ENABLE_TYPENAMES
    "iD", /* .memberName */
#endif
    UA_OPENAAS_IDENTIFICATION, /* .memberTypeIndex */
    offsetof(UA_PropertyValueStatement, iD) - offsetof(UA_PropertyValueStatement, value) - sizeof(UA_Variant), /* .padding */
    false, /* .namespaceZero */
    false /* .isArray */
},
{
#ifdef UA_ENABLE_TYPENAMES
    "view", /* .memberName */
#endif
    UA_OPENAAS_VIEWENUM, /* .memberTypeIndex */
    offsetof(UA_PropertyValueStatement, view) - offsetof(UA_PropertyValueStatement, iD) - sizeof(UA_Identification), /* .padding */
    false, /* .namespaceZero */
    false /* .isArray */
},
{
#ifdef UA_ENABLE_TYPENAMES
    "visibility", /* .memberName */
#endif
    UA_OPENAAS_VISIBILITYENUM, /* .memberTypeIndex */
    offsetof(UA_PropertyValueStatement, visibility) - offsetof(UA_PropertyValueStatement, view) - sizeof(UA_ViewEnum), /* .padding */
    false, /* .namespaceZero */
    false /* .isArray */
},};
UA_DataType UA_OPENAAS[UA_OPENAAS_COUNT] = {

/* ExpressionSemanticEnum */
{
#ifdef UA_ENABLE_TYPENAMES
    "ExpressionSemanticEnum", /* .typeName */
#endif
    {3, UA_NODEIDTYPE_NUMERIC, {6}}, /* .typeId */
    sizeof(UA_ExpressionSemanticEnum), /* .memSize */
    UA_TYPES_INT32, /* .typeIndex */
    1, /* .membersSize */
    true, /* .builtin */
    true, /* .pointerFree */
    UA_BINARY_OVERLAYABLE_INTEGER, /* .overlayable */ 
    0, /* .binaryEncodingId */
    ExpressionSemanticEnum_members /* .members */ },

/* ExpressionLogicEnum */
{
#ifdef UA_ENABLE_TYPENAMES
    "ExpressionLogicEnum", /* .typeName */
#endif
    {3, UA_NODEIDTYPE_NUMERIC, {6}}, /* .typeId */
    sizeof(UA_ExpressionLogicEnum), /* .memSize */
    UA_TYPES_INT32, /* .typeIndex */
    1, /* .membersSize */
    true, /* .builtin */
    true, /* .pointerFree */
    UA_BINARY_OVERLAYABLE_INTEGER, /* .overlayable */ 
    0, /* .binaryEncodingId */
    ExpressionLogicEnum_members /* .members */ },

/* ViewEnum */
{
#ifdef UA_ENABLE_TYPENAMES
    "ViewEnum", /* .typeName */
#endif
    {3, UA_NODEIDTYPE_NUMERIC, {6}}, /* .typeId */
    sizeof(UA_ViewEnum), /* .memSize */
    UA_TYPES_INT32, /* .typeIndex */
    1, /* .membersSize */
    true, /* .builtin */
    true, /* .pointerFree */
    UA_BINARY_OVERLAYABLE_INTEGER, /* .overlayable */ 
    0, /* .binaryEncodingId */
    ViewEnum_members /* .members */ },

/* VisibilityEnum */
{
#ifdef UA_ENABLE_TYPENAMES
    "VisibilityEnum", /* .typeName */
#endif
    {3, UA_NODEIDTYPE_NUMERIC, {6}}, /* .typeId */
    sizeof(UA_VisibilityEnum), /* .memSize */
    UA_TYPES_INT32, /* .typeIndex */
    1, /* .membersSize */
    true, /* .builtin */
    true, /* .pointerFree */
    UA_BINARY_OVERLAYABLE_INTEGER, /* .overlayable */ 
    0, /* .binaryEncodingId */
    VisibilityEnum_members /* .members */ },

/* IdEnum */
{
#ifdef UA_ENABLE_TYPENAMES
    "IdEnum", /* .typeName */
#endif
    {3, UA_NODEIDTYPE_NUMERIC, {6}}, /* .typeId */
    sizeof(UA_IdEnum), /* .memSize */
    UA_TYPES_INT32, /* .typeIndex */
    1, /* .membersSize */
    true, /* .builtin */
    true, /* .pointerFree */
    UA_BINARY_OVERLAYABLE_INTEGER, /* .overlayable */ 
    0, /* .binaryEncodingId */
    IdEnum_members /* .members */ },

/* Identification */
{
#ifdef UA_ENABLE_TYPENAMES
    "Identification", /* .typeName */
#endif
    {3, UA_NODEIDTYPE_NUMERIC, {3005}}, /* .typeId */
    sizeof(UA_Identification), /* .memSize */
    UA_OPENAAS_IDENTIFICATION, /* .typeIndex */
    2, /* .membersSize */
    false, /* .builtin */
    false, /* .pointerFree */
    false, /* .overlayable */ 
    5001, /* .binaryEncodingId */
    Identification_members /* .members */ },

/* LifeCycleEntry */
{
#ifdef UA_ENABLE_TYPENAMES
    "LifeCycleEntry", /* .typeName */
#endif
    {3, UA_NODEIDTYPE_NUMERIC, {3008}}, /* .typeId */
    sizeof(UA_LifeCycleEntry), /* .memSize */
    UA_OPENAAS_LIFECYCLEENTRY, /* .typeIndex */
    6, /* .membersSize */
    false, /* .builtin */
    false, /* .pointerFree */
    false, /* .overlayable */ 
    5008, /* .binaryEncodingId */
    LifeCycleEntry_members /* .members */ },

/* PropertyValueStatement */
{
#ifdef UA_ENABLE_TYPENAMES
    "PropertyValueStatement", /* .typeName */
#endif
    {3, UA_NODEIDTYPE_NUMERIC, {3011}}, /* .typeId */
    sizeof(UA_PropertyValueStatement), /* .memSize */
    UA_OPENAAS_PROPERTYVALUESTATEMENT, /* .typeIndex */
    7, /* .membersSize */
    false, /* .builtin */
    false, /* .pointerFree */
    false, /* .overlayable */ 
    5006, /* .binaryEncodingId */
    PropertyValueStatement_members /* .members */ },
};

