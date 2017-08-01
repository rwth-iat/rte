/* Generated from Opc.Ua.Types.bsd, identification.bsd, Custom.Opc.Ua.AssetAdministrationShell.bsd with script /home/ubuntu/opcuaParser/tools/generate_datatypes.py
 * on host ubuntu-VirtualBox by user ubuntu at 2017-08-01 01:37:54 */

#ifndef UA_PROPERTYVALUESTATEMENT_GENERATED_H_
#define UA_PROPERTYVALUESTATEMENT_GENERATED_H_

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
#define UA_PROPERTYVALUESTATEMENT_COUNT 4
extern UA_EXPORT UA_DataType UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_COUNT];

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
    UA_EXPRESSIONLOGICENUM_LESS_THAN = 5
} UA_ExpressionLogicEnum;

#define UA_PROPERTYVALUESTATEMENT_EXPRESSIONLOGICENUM 1

/**
 * VisibilityEnum
 * ^^^^^^^^^^^^^^
 */
typedef enum {
    UA_VISIBILITYENUM_PRIVAT = 0,
    UA_VISIBILITYENUM_CONTRACT = 1,
    UA_VISIBILITYENUM_PUBLIC = 2
} UA_VisibilityEnum;

#define UA_PROPERTYVALUESTATEMENT_VISIBILITYENUM 2

/**
 * ExpressionSemanticEnum
 * ^^^^^^^^^^^^^^^^^^^^^^
 */
typedef enum {
    UA_EXPRESSIONSEMANTICENUM_ASSURANCE = 0,
    UA_EXPRESSIONSEMANTICENUM_SETTING = 1,
    UA_EXPRESSIONSEMANTICENUM_MEASUREMENT = 2,
    UA_EXPRESSIONSEMANTICENUM_REQUIREMENT = 3
} UA_ExpressionSemanticEnum;

#define UA_PROPERTYVALUESTATEMENT_EXPRESSIONSEMANTICENUM 3

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* UA_PROPERTYVALUESTATEMENT_GENERATED_H_ */
