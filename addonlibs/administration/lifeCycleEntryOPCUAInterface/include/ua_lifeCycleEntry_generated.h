/* Generated from identification.bsd, lce.bsd with script generate_datatypes.py
 * on host ubuntu-VirtualBox by user ubuntu at 2019-06-03 05:49:28 */

#ifndef LIFECYCLEENTRY_GENERATED_H_
#define LIFECYCLEENTRY_GENERATED_H_

#include "open62541.h"

_UA_BEGIN_DECLS


/**
 * Every type is assigned an index in an array containing the type descriptions.
 * These descriptions are used during type handling (copying, deletion,
 * binary encoding, ...). */
#define UA_LIFECYCLEENTRY_COUNT 3
extern UA_EXPORT const UA_DataType UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_COUNT];

/**
 * IdEnum
 * ^^^^^^
 */
typedef enum {
    UA_IDENUM_URI = 0,
    UA_IDENUM_ISO = 1,
    __UA_IDENUM_FORCE32BIT = 0x7fffffff
} UA_IdEnum;
UA_STATIC_ASSERT(sizeof(UA_IdEnum) == sizeof(UA_Int32), enum_must_be_32bit);

#define UA_LIFECYCLEENTRY_IDENUM 0

/**
 * Identification
 * ^^^^^^^^^^^^^^
 */
typedef struct {
    UA_String idSpec;
    UA_IdEnum idType;
} UA_Identification;

#define UA_LIFECYCLEENTRY_IDENTIFICATION 1

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

#define UA_LIFECYCLEENTRY_LIFECYCLEENTRY 2


_UA_END_DECLS

#endif /* LIFECYCLEENTRY_GENERATED_H_ */
