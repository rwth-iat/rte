/* Generated from Opc.Ua.Types.bsd, aas.bsd with script ../tools/generate_datatypes.py
 * on host ubuntu-VirtualBox by user ubuntu at 2017-07-05 12:52:11 */

#ifndef UA_IDENTIFICATION_GENERATED_H_
#define UA_IDENTIFICATION_GENERATED_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef UA_NO_AMALGAMATION
#include "ua_types.h"
 #include "ua_types_generated.h"

#else
 #include "open62541.h"
#endif

/**
 * Every type is assigned an index in an array containing the type descriptions.
 * These descriptions are used during type handling (copying, deletion,
 * binary encoding, ...). */
#define UA_IDENTIFICATION_COUNT 2
extern UA_EXPORT UA_DataType UA_IDENTIFICATION[UA_IDENTIFICATION_COUNT];

/**
 * IdEnum
 * ^^^^^^
 * Determines which kind of Id is specified */
typedef enum {
    UA_IDENUM_URI = 0,
    UA_IDENUM_ISO = 1
} UA_IdEnum;

#define UA_IDENTIFICATION_IDENUM 0

/**
 * Identification
 * ^^^^^^^^^^^^^^
 * Identification */
typedef struct {
    UA_String idSpec;
    UA_IdEnum idType;
} UA_Identification;

#define UA_IDENTIFICATION_IDENTIFICATION 1

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* UA_IDENTIFICATION_GENERATED_H_ */
