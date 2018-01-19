/* Generated from Opc.Ua.Types.bsd, identification.bsd, Custom.Opc.Ua.AssetAdministrationShell.bsd with script /home/ubuntu/opcuaParser/tools/generate_datatypes.py
 * on host ubuntu-VirtualBox by user ubuntu at 2017-09-29 03:11:03 */

#ifndef OV_COMPILE_LIBRARY_lifeCycleEntryOPCUAInterface
#define OV_COMPILE_LIBRARY_lifeCycleEntryOPCUAInterface

#ifdef __cplusplus
extern "C" {
#endif

#ifdef UA_NO_AMALGAMATION
#include "ua_types.h"
 #include "ua_types_generated.h"

#else
 #include "open62541.h"
#endif

#include "ua_identification_generated.h"

/**
 * Every type is assigned an index in an array containing the type descriptions.
 * These descriptions are used during type handling (copying, deletion,
 * binary encoding, ...). */
#define UA_LIFECYCLEENTRY_COUNT 1
extern UA_EXPORT UA_DataType UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_COUNT];

/**
 * LifeCycleEntry
 * ^^^^^^^^^^^^^^
 */
typedef struct {
    UA_Identification creatingInstance;
    UA_Identification writingInstance;
    UA_DataValue data;
    UA_String subject;
    UA_String eventClass;
    UA_UInt64 id;
} UA_LifeCycleEntry;

#define UA_LIFECYCLEENTRY_LIFECYCLEENTRY 0

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* UA_LIFECYCLEENTRY_GENERATED_H_ */
