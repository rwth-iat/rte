/* Generated from pvs.bsd with script generate_datatypes.py
 * on host ubuntu-VirtualBox by user ubuntu at 2019-06-05 03:06:38 */

#include "ua_propertyValueStatement_generated.h"

/* ViewEnum */
#define ViewEnum_members NULL

/* ExpressionLogicEnum */
#define ExpressionLogicEnum_members NULL

/* VisibilityEnum */
#define VisibilityEnum_members NULL

/* ExpressionSemanticEnum */
#define ExpressionSemanticEnum_members NULL
const UA_DataType UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_COUNT] = {
/* ViewEnum */
{
    UA_TYPENAME("ViewEnum") /* .typeName */
    {2, UA_NODEIDTYPE_NUMERIC, {3004}}, /* .typeId */
    sizeof(UA_ViewEnum), /* .memSize */
    UA_TYPES_INT32, /* .typeIndex */
    UA_DATATYPEKIND_ENUM, /* .typeKind */
    true, /* .pointerFree */
    UA_BINARY_OVERLAYABLE_INTEGER, /* .overlayable */
    0, /* .membersSize */
    0, /* .binaryEncodingId */
    ViewEnum_members /* .members */
},
/* ExpressionLogicEnum */
{
    UA_TYPENAME("ExpressionLogicEnum") /* .typeName */
    {2, UA_NODEIDTYPE_NUMERIC, {3002}}, /* .typeId */
    sizeof(UA_ExpressionLogicEnum), /* .memSize */
    UA_TYPES_INT32, /* .typeIndex */
    UA_DATATYPEKIND_ENUM, /* .typeKind */
    true, /* .pointerFree */
    UA_BINARY_OVERLAYABLE_INTEGER, /* .overlayable */
    0, /* .membersSize */
    0, /* .binaryEncodingId */
    ExpressionLogicEnum_members /* .members */
},
/* VisibilityEnum */
{
    UA_TYPENAME("VisibilityEnum") /* .typeName */
    {2, UA_NODEIDTYPE_NUMERIC, {3005}}, /* .typeId */
    sizeof(UA_VisibilityEnum), /* .memSize */
    UA_TYPES_INT32, /* .typeIndex */
    UA_DATATYPEKIND_ENUM, /* .typeKind */
    true, /* .pointerFree */
    UA_BINARY_OVERLAYABLE_INTEGER, /* .overlayable */
    0, /* .membersSize */
    0, /* .binaryEncodingId */
    VisibilityEnum_members /* .members */
},
/* ExpressionSemanticEnum */
{
    UA_TYPENAME("ExpressionSemanticEnum") /* .typeName */
    {2, UA_NODEIDTYPE_NUMERIC, {3003}}, /* .typeId */
    sizeof(UA_ExpressionSemanticEnum), /* .memSize */
    UA_TYPES_INT32, /* .typeIndex */
    UA_DATATYPEKIND_ENUM, /* .typeKind */
    true, /* .pointerFree */
    UA_BINARY_OVERLAYABLE_INTEGER, /* .overlayable */
    0, /* .membersSize */
    0, /* .binaryEncodingId */
    ExpressionSemanticEnum_members /* .members */
},
};

