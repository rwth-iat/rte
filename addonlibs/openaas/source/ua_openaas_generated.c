/* Generated from Opc.Ua.Types.bsd, Custom.Opc.Ua.AssetAdministrationShell.bsd with script /home/opcua/Documents/openAAS_workshop/external/open62541/tools/generate_datatypes.py
 * on host opcua-VirtualBox by user opcua at 2017-01-26 07:12:03 */

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

/* RelationalExpressionEnum */
static UA_DataTypeMember RelationalExpressionEnum_members[1] = {
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
    "view", /* .memberName */
#endif
    UA_OPENAAS_VIEWENUM, /* .memberTypeIndex */
    offsetof(UA_LifeCycleEntry, view) - offsetof(UA_LifeCycleEntry, eventClass) - sizeof(UA_String), /* .padding */
    false, /* .namespaceZero */
    false /* .isArray */
},};

/* PropertyValueStatement */
static UA_DataTypeMember PropertyValueStatement_members[6] = {
{
#ifdef UA_ENABLE_TYPENAMES
    "expressionSemantic", /* .memberName */
#endif
    UA_TYPES_UINT32, /* .memberTypeIndex */
    0, /* .padding */
    true, /* .namespaceZero */
    false /* .isArray */
},
{
#ifdef UA_ENABLE_TYPENAMES
    "relationalExpression", /* .memberName */
#endif
    UA_TYPES_UINT32, /* .memberTypeIndex */
    offsetof(UA_PropertyValueStatement, relationalExpression) - offsetof(UA_PropertyValueStatement, expressionSemantic) - sizeof(UA_UInt32), /* .padding */
    true, /* .namespaceZero */
    false /* .isArray */
},
{
#ifdef UA_ENABLE_TYPENAMES
    "unit", /* .memberName */
#endif
    UA_TYPES_STRING, /* .memberTypeIndex */
    offsetof(UA_PropertyValueStatement, unit) - offsetof(UA_PropertyValueStatement, relationalExpression) - sizeof(UA_UInt32), /* .padding */
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
    "propertyReference", /* .memberName */
#endif
    UA_OPENAAS_IDENTIFICATION, /* .memberTypeIndex */
    offsetof(UA_PropertyValueStatement, propertyReference) - offsetof(UA_PropertyValueStatement, value) - sizeof(UA_Variant), /* .padding */
    false, /* .namespaceZero */
    false /* .isArray */
},
{
#ifdef UA_ENABLE_TYPENAMES
    "view", /* .memberName */
#endif
    UA_OPENAAS_VIEWENUM, /* .memberTypeIndex */
    offsetof(UA_PropertyValueStatement, view) - offsetof(UA_PropertyValueStatement, propertyReference) - sizeof(UA_Identification), /* .padding */
    false, /* .namespaceZero */
    false /* .isArray */
},};
const UA_DataType UA_OPENAAS[UA_OPENAAS_COUNT] = {

/* ExpressionSemanticEnum */
{
#ifdef UA_ENABLE_TYPENAMES
    "ExpressionSemanticEnum", /* .typeName */
#endif
    {2, UA_NODEIDTYPE_NUMERIC, {6}}, /* .typeId */
    sizeof(UA_ExpressionSemanticEnum), /* .memSize */
    UA_TYPES_INT32, /* .typeIndex */
    1, /* .membersSize */
    true, /* .builtin */
    true, /* .pointerFree */
    UA_BINARY_OVERLAYABLE_INTEGER, /* .overlayable */ 
    0, /* .binaryEncodingId */
    ExpressionSemanticEnum_members /* .members */ },

/* ViewEnum */
{
#ifdef UA_ENABLE_TYPENAMES
    "ViewEnum", /* .typeName */
#endif
    {2, UA_NODEIDTYPE_NUMERIC, {6}}, /* .typeId */
    sizeof(UA_ViewEnum), /* .memSize */
    UA_TYPES_INT32, /* .typeIndex */
    1, /* .membersSize */
    true, /* .builtin */
    true, /* .pointerFree */
    UA_BINARY_OVERLAYABLE_INTEGER, /* .overlayable */ 
    0, /* .binaryEncodingId */
    ViewEnum_members /* .members */ },

/* RelationalExpressionEnum */
{
#ifdef UA_ENABLE_TYPENAMES
    "RelationalExpressionEnum", /* .typeName */
#endif
    {2, UA_NODEIDTYPE_NUMERIC, {6}}, /* .typeId */
    sizeof(UA_RelationalExpressionEnum), /* .memSize */
    UA_TYPES_INT32, /* .typeIndex */
    1, /* .membersSize */
    true, /* .builtin */
    true, /* .pointerFree */
    UA_BINARY_OVERLAYABLE_INTEGER, /* .overlayable */ 
    0, /* .binaryEncodingId */
    RelationalExpressionEnum_members /* .members */ },

/* IdEnum */
{
#ifdef UA_ENABLE_TYPENAMES
    "IdEnum", /* .typeName */
#endif
    {2, UA_NODEIDTYPE_NUMERIC, {6}}, /* .typeId */
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
    {2, UA_NODEIDTYPE_NUMERIC, {3005}}, /* .typeId */
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
    {2, UA_NODEIDTYPE_NUMERIC, {3008}}, /* .typeId */
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
    {2, UA_NODEIDTYPE_NUMERIC, {3011}}, /* .typeId */
    sizeof(UA_PropertyValueStatement), /* .memSize */
    UA_OPENAAS_PROPERTYVALUESTATEMENT, /* .typeIndex */
    6, /* .membersSize */
    false, /* .builtin */
    false, /* .pointerFree */
    false, /* .overlayable */ 
    5006, /* .binaryEncodingId */
    PropertyValueStatement_members /* .members */ },
};

