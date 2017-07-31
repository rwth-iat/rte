/* Generated from Opc.Ua.Types.bsd, identification.bsd, Custom.Opc.Ua.AssetAdministrationShell.bsd with script /home/ubuntu/opcuaParser/tools/generate_datatypes.py
 * on host ubuntu-VirtualBox by user ubuntu at 2017-07-31 01:27:48 */

#include "stddef.h"
#include "ua_lifeCycleEntry_generated.h"

/* LifeCycleEntry */
static UA_DataTypeMember LifeCycleEntry_members[6] = {
{
#ifdef UA_ENABLE_TYPENAMES
    "creatingInstance", /* .memberName */
#endif
    UA_IDENTIFICATION_IDENTIFICATION, /* .memberTypeIndex */
    0, /* .padding */
    false, /* .namespaceZero */
    false /* .isArray */
},
{
#ifdef UA_ENABLE_TYPENAMES
    "writingInstance", /* .memberName */
#endif
    UA_IDENTIFICATION_IDENTIFICATION, /* .memberTypeIndex */
    offsetof(UA_LifeCycleEntry, writingInstance) - offsetof(UA_LifeCycleEntry, creatingInstance) - sizeof(UA_Identification), /* .padding */
    false, /* .namespaceZero */
    false /* .isArray */
},
{
#ifdef UA_ENABLE_TYPENAMES
    "data", /* .memberName */
#endif
    UA_TYPES_VARIANT, /* .memberTypeIndex */
    offsetof(UA_LifeCycleEntry, data) - offsetof(UA_LifeCycleEntry, writingInstance) - sizeof(UA_Identification), /* .padding */
    true, /* .namespaceZero */
    false /* .isArray */
},
{
#ifdef UA_ENABLE_TYPENAMES
    "subject", /* .memberName */
#endif
    UA_TYPES_STRING, /* .memberTypeIndex */
    offsetof(UA_LifeCycleEntry, subject) - offsetof(UA_LifeCycleEntry, data) - sizeof(UA_Variant), /* .padding */
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
UA_DataType UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_COUNT] = {

/* LifeCycleEntry */
{
#ifdef UA_ENABLE_TYPENAMES
    "LifeCycleEntry", /* .typeName */
#endif
    {3, UA_NODEIDTYPE_NUMERIC, {3002}}, /* .typeId */
    sizeof(UA_LifeCycleEntry), /* .memSize */
    UA_LIFECYCLEENTRY_LIFECYCLEENTRY, /* .typeIndex */
    6, /* .membersSize */
    false, /* .builtin */
    false, /* .pointerFree */
    false, /* .overlayable */ 
    5001, /* .binaryEncodingId */
    LifeCycleEntry_members /* .members */ },
};

