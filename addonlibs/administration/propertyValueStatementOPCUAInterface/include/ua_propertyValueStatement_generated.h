/* Generated from Opc.Ua.Types.bsd, identification.bsd, Custom.Opc.Ua.AssetAdministrationShell.bsd with script /home/ubuntu/opcuaParser/tools/generate_datatypes.py
 * on host ubuntu-VirtualBox by user ubuntu at 2017-07-27 10:58:47 */

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
#define UA_PROPERTYVALUESTATEMENT_COUNT 2
extern UA_EXPORT UA_DataType UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_COUNT];

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

#define UA_PROPERTYVALUESTATEMENT_EXPRESSIONLOGICENUM 0

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

#define UA_PROPERTYVALUESTATEMENT_EXPRESSIONSEMANTICENUM 1

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* UA_PROPERTYVALUESTATEMENT_GENERATED_H_ */
