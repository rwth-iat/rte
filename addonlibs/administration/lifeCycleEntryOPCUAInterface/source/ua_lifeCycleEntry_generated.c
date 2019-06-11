/* Generated from identification.bsd, lce.bsd with script generate_datatypes.py
 * on host ubuntu-VirtualBox by user ubuntu at 2019-06-03 05:49:28 */

#include "ua_lifeCycleEntry_generated.h"


/* LifeCycleEntry */
static UA_DataTypeMember LifeCycleEntry_members[6] = {
{
    UA_TYPENAME("CreatingInstance") /* .memberName */
	UA_IDENTIFICATION_IDENTIFICATION, /* .memberTypeIndex */
    0, /* .padding */
    false, /* .namespaceZero */
    false /* .isArray */
},
{
    UA_TYPENAME("WritingInstance") /* .memberName */
	UA_IDENTIFICATION_IDENTIFICATION, /* .memberTypeIndex */
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

