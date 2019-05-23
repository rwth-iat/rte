/* Generated from identification.bsd with script generate_datatypes.py
 * on host ubuntu-VirtualBox by user ubuntu at 2019-05-20 02:51:10 */

#ifndef IDENTIFICATION_GENERATED_H_
#define IDENTIFICATION_GENERATED_H_

#include "open62541.h"

_UA_BEGIN_DECLS


/**
 * Every type is assigned an index in an array containing the type descriptions.
 * These descriptions are used during type handling (copying, deletion,
 * binary encoding, ...). */
#define UA_IDENTIFICATION_COUNT 2
extern UA_EXPORT const UA_DataType UA_IDENTIFICATION[UA_IDENTIFICATION_COUNT];

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

#define UA_IDENTIFICATION_IDENUM 0

/**
 * Identification
 * ^^^^^^^^^^^^^^
 */
typedef struct {
    UA_String idSpec;
    UA_IdEnum idType;
} UA_Identification;

#define UA_IDENTIFICATION_IDENTIFICATION 1


_UA_END_DECLS

#endif /* IDENTIFICATION_GENERATED_H_ */
