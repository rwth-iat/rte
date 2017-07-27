/* Generated from Opc.Ua.Types.bsd, identification.bsd, lce.bsd, pvs.bsd, Custom.Opc.Ua.AssetAdministrationShell.bsd with script /home/ubuntu/opcuaParser/tools/generate_datatypes.py
 * on host ubuntu-VirtualBox by user ubuntu at 2017-07-26 10:43:58 */

#ifndef UA_OPENAAS_GENERATED_H_
#define UA_OPENAAS_GENERATED_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef UA_NO_AMALGAMATION
#include "ua_types.h"
 #include "ua_types_generated.h"

#else
 #include "open62541.h"
#endif

#include "ua_lifeCycleEntry_generated.h"
#include "ua_propertyValueStatement_generated.h"

/**
 * Every type is assigned an index in an array containing the type descriptions.
 * These descriptions are used during type handling (copying, deletion,
 * binary encoding, ...). */
#define UA_OPENAAS_COUNT 2
extern UA_EXPORT UA_DataType UA_OPENAAS[UA_OPENAAS_COUNT];

/**
 * ViewEnum
 * ^^^^^^^^
 */
typedef enum {
    UA_VIEWENUM_BUSINESS = 0,
    UA_VIEWENUM_CONSTRUCTION = 1,
    UA_VIEWENUM_POWER = 2,
    UA_VIEWENUM_FUNCTIONAL = 3,
    UA_VIEWENUM_LOCATION = 4,
    UA_VIEWENUM_SECURITY = 5,
    UA_VIEWENUM_NETWORK = 6,
    UA_VIEWENUM_LIFECYCLE = 7,
    UA_VIEWENUM_HUMAN = 8
} UA_ViewEnum;

#define UA_OPENAAS_VIEWENUM 0

/**
 * VisibilityEnum
 * ^^^^^^^^^^^^^^
 */
typedef enum {
    UA_VISIBILITYENUM_PRIVAT = 0,
    UA_VISIBILITYENUM_CONTRACT = 1,
    UA_VISIBILITYENUM_PUBLIC = 2
} UA_VisibilityEnum;

#define UA_OPENAAS_VISIBILITYENUM 1

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* UA_OPENAAS_GENERATED_H_ */
