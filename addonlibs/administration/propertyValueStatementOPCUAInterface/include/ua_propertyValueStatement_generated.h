/* Generated from pvs.bsd with script generate_datatypes.py
 * on host ubuntu-VirtualBox by user ubuntu at 2019-06-05 03:06:38 */

#ifndef PROPERTYVALUESTATEMENT_GENERATED_H_
#define PROPERTYVALUESTATEMENT_GENERATED_H_

#include "open62541.h"

_UA_BEGIN_DECLS


/**
 * Every type is assigned an index in an array containing the type descriptions.
 * These descriptions are used during type handling (copying, deletion,
 * binary encoding, ...). */
#define UA_PROPERTYVALUESTATEMENT_COUNT 4
extern UA_EXPORT const UA_DataType UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_COUNT];

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
    UA_VIEWENUM_HUMAN = 8,
    __UA_VIEWENUM_FORCE32BIT = 0x7fffffff
} UA_ViewEnum;
UA_STATIC_ASSERT(sizeof(UA_ViewEnum) == sizeof(UA_Int32), enum_must_be_32bit);

#define UA_PROPERTYVALUESTATEMENT_VIEWENUM 0

/**
 * ExpressionLogicEnum
 * ^^^^^^^^^^^^^^^^^^^
 */
typedef enum {
    UA_EXPRESSIONLOGICENUM_GREATER_THAN = 0,
    UA_EXPRESSIONLOGICENUM_GREATER_EQUAL = 1,
    UA_EXPRESSIONLOGICENUM_EQUAL = 2,
    UA_EXPRESSIONLOGICENUM_NOT_EQUAL = 3,
    UA_EXPRESSIONLOGICENUM_LESS_EQUAL = 4,
    UA_EXPRESSIONLOGICENUM_LESS_THAN = 5,
    __UA_EXPRESSIONLOGICENUM_FORCE32BIT = 0x7fffffff
} UA_ExpressionLogicEnum;
UA_STATIC_ASSERT(sizeof(UA_ExpressionLogicEnum) == sizeof(UA_Int32), enum_must_be_32bit);

#define UA_PROPERTYVALUESTATEMENT_EXPRESSIONLOGICENUM 1

/**
 * VisibilityEnum
 * ^^^^^^^^^^^^^^
 */
typedef enum {
    UA_VISIBILITYENUM_PRIVAT = 0,
    UA_VISIBILITYENUM_CONTRACT = 1,
    UA_VISIBILITYENUM_PUBLIC = 2,
    __UA_VISIBILITYENUM_FORCE32BIT = 0x7fffffff
} UA_VisibilityEnum;
UA_STATIC_ASSERT(sizeof(UA_VisibilityEnum) == sizeof(UA_Int32), enum_must_be_32bit);

#define UA_PROPERTYVALUESTATEMENT_VISIBILITYENUM 2

/**
 * ExpressionSemanticEnum
 * ^^^^^^^^^^^^^^^^^^^^^^
 */
typedef enum {
    UA_EXPRESSIONSEMANTICENUM_ASSURANCE = 0,
    UA_EXPRESSIONSEMANTICENUM_SETTING = 1,
    UA_EXPRESSIONSEMANTICENUM_MEASUREMENT = 2,
    UA_EXPRESSIONSEMANTICENUM_REQUIREMENT = 3,
    __UA_EXPRESSIONSEMANTICENUM_FORCE32BIT = 0x7fffffff
} UA_ExpressionSemanticEnum;
UA_STATIC_ASSERT(sizeof(UA_ExpressionSemanticEnum) == sizeof(UA_Int32), enum_must_be_32bit);

#define UA_PROPERTYVALUESTATEMENT_EXPRESSIONSEMANTICENUM 3


_UA_END_DECLS

#endif /* PROPERTYVALUESTATEMENT_GENERATED_H_ */
