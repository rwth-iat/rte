/* Generated from identification.bsd with script generate_datatypes.py
 * on host ubuntu-VirtualBox by user ubuntu at 2019-05-20 02:51:10 */

#include "ua_identification_generated.h"

/* IdEnum */
#define IdEnum_members NULL

/* Identification */
static UA_DataTypeMember Identification_members[2] = {
{
    UA_TYPENAME("IdSpec") /* .memberName */
    UA_TYPES_STRING, /* .memberTypeIndex */
    0, /* .padding */
    true, /* .namespaceZero */
    false /* .isArray */
},
{
    UA_TYPENAME("IdType") /* .memberName */
    UA_IDENTIFICATION_IDENUM, /* .memberTypeIndex */
    offsetof(UA_Identification, idType) - offsetof(UA_Identification, idSpec) - sizeof(UA_String), /* .padding */
    false, /* .namespaceZero */
    false /* .isArray */
},};
const UA_DataType UA_IDENTIFICATION[UA_IDENTIFICATION_COUNT] = {
/* IdEnum */
{
    UA_TYPENAME("IdEnum") /* .typeName */
    {2, UA_NODEIDTYPE_NUMERIC, {3003}}, /* .typeId */
    sizeof(UA_IdEnum), /* .memSize */
    UA_TYPES_INT32, /* .typeIndex */
    UA_DATATYPEKIND_ENUM, /* .typeKind */
    true, /* .pointerFree */
    UA_BINARY_OVERLAYABLE_INTEGER, /* .overlayable */
    0, /* .membersSize */
    0, /* .binaryEncodingId */
    IdEnum_members /* .members */
},
/* Identification */
{
    UA_TYPENAME("Identification") /* .typeName */
    {2, UA_NODEIDTYPE_NUMERIC, {3002}}, /* .typeId */
    sizeof(UA_Identification), /* .memSize */
    UA_IDENTIFICATION_IDENTIFICATION, /* .typeIndex */
    UA_DATATYPEKIND_STRUCTURE, /* .typeKind */
    false, /* .pointerFree */
    false, /* .overlayable */
    2, /* .membersSize */
    5001, /* .binaryEncodingId */
    Identification_members /* .members */
},
};

