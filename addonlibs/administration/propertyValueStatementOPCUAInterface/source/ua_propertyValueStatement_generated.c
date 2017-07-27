/* Generated from Opc.Ua.Types.bsd, identification.bsd, Custom.Opc.Ua.AssetAdministrationShell.bsd with script /home/ubuntu/opcuaParser/tools/generate_datatypes.py
 * on host ubuntu-VirtualBox by user ubuntu at 2017-07-27 10:58:47 */

#include "stddef.h"
#include "ua_propertyValueStatement_generated.h"

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
UA_DataType UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_COUNT] = {

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
};

