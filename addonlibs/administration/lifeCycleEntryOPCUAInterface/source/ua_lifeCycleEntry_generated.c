/* Generated from identification.bsd, lce.bsd with script generate_datatypes.py
 * on host ubuntu-VirtualBox by user ubuntu at 2019-06-03 05:49:28 */

#include "ua_lifeCycleEntry_generated.h"

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
    UA_LIFECYCLEENTRY_IDENUM, /* .memberTypeIndex */
    offsetof(UA_Identification, idType) - offsetof(UA_Identification, idSpec) - sizeof(UA_String), /* .padding */
    false, /* .namespaceZero */
    false /* .isArray */
},};

/* LifeCycleEntry */
static UA_DataTypeMember LifeCycleEntry_members[6] = {
{
    UA_TYPENAME("CreatingInstance") /* .memberName */
    UA_LIFECYCLEENTRY_IDENTIFICATION, /* .memberTypeIndex */
    0, /* .padding */
    false, /* .namespaceZero */
    false /* .isArray */
},
{
    UA_TYPENAME("WritingInstance") /* .memberName */
    UA_LIFECYCLEENTRY_IDENTIFICATION, /* .memberTypeIndex */
    offsetof(UA_LifeCycleEntry, writingInstance) - offsetof(UA_LifeCycleEntry, creatingInstance) - sizeof(UA_Identification), /* .padding */
    false, /* .namespaceZero */
    false /* .isArray */
},
{
    UA_TYPENAME("Data") /* .memberName */
    UA_TYPES_DATAVALUE, /* .memberTypeIndex */
    offsetof(UA_LifeCycleEntry, data) - offsetof(UA_LifeCycleEntry, writingInstance) - sizeof(UA_Identification), /* .padding */
    true, /* .namespaceZero */
    false /* .isArray */
},
{
    UA_TYPENAME("Subject") /* .memberName */
    UA_TYPES_STRING, /* .memberTypeIndex */
    offsetof(UA_LifeCycleEntry, subject) - offsetof(UA_LifeCycleEntry, data) - sizeof(UA_DataValue), /* .padding */
    true, /* .namespaceZero */
    false /* .isArray */
},
{
    UA_TYPENAME("EventClass") /* .memberName */
    UA_TYPES_STRING, /* .memberTypeIndex */
    offsetof(UA_LifeCycleEntry, eventClass) - offsetof(UA_LifeCycleEntry, subject) - sizeof(UA_String), /* .padding */
    true, /* .namespaceZero */
    false /* .isArray */
},
{
    UA_TYPENAME("Id") /* .memberName */
    UA_TYPES_UINT64, /* .memberTypeIndex */
    offsetof(UA_LifeCycleEntry, id) - offsetof(UA_LifeCycleEntry, eventClass) - sizeof(UA_String), /* .padding */
    true, /* .namespaceZero */
    false /* .isArray */
},};
const UA_DataType UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_COUNT] = {
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
    UA_LIFECYCLEENTRY_IDENTIFICATION, /* .typeIndex */
    UA_DATATYPEKIND_STRUCTURE, /* .typeKind */
    false, /* .pointerFree */
    false, /* .overlayable */
    2, /* .membersSize */
    5001, /* .binaryEncodingId */
    Identification_members /* .members */
},
/* LifeCycleEntry */
{
    UA_TYPENAME("LifeCycleEntry") /* .typeName */
    {2, UA_NODEIDTYPE_NUMERIC, {3002}}, /* .typeId */
    sizeof(UA_LifeCycleEntry), /* .memSize */
    UA_LIFECYCLEENTRY_LIFECYCLEENTRY, /* .typeIndex */
    UA_DATATYPEKIND_STRUCTURE, /* .typeKind */
    false, /* .pointerFree */
    false, /* .overlayable */
    6, /* .membersSize */
    5001, /* .binaryEncodingId */
    LifeCycleEntry_members /* .members */
},
};

