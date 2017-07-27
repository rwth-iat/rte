/* Generated from Opc.Ua.Types.bsd, identification.bsd, lce.bsd, pvs.bsd, Custom.Opc.Ua.AssetAdministrationShell.bsd with script /home/ubuntu/opcuaParser/tools/generate_datatypes.py
 * on host ubuntu-VirtualBox by user ubuntu at 2017-07-26 10:43:58 */

#include "stddef.h"
#include "ua_openaas_generated.h"

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
UA_DataType UA_OPENAAS[UA_OPENAAS_COUNT] = {

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
};

