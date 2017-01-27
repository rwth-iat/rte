/* Generated from Opc.Ua.Types.bsd, Custom.Opc.Ua.AssetAdministrationShell.bsd with script /home/opcua/Documents/openAAS_workshop/external/open62541/tools/generate_datatypes.py
 * on host opcua-VirtualBox by user opcua at 2017-01-27 10:27:49 */

#ifndef UA_OPENAAS_GENERATED_H_
#define UA_OPENAAS_GENERATED_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "openaas.h"

/**
 * Every type is assigned an index in an array containing the type descriptions.
 * These descriptions are used during type handling (copying, deletion,
 * binary encoding, ...). */
#define UA_OPENAAS_COUNT 7
extern UA_EXPORT const UA_DataType UA_OPENAAS[UA_OPENAAS_COUNT];

/**
 * ExpressionSemanticEnum
 * ^^^^^^^^^^^^^^^^^^^^^^
 * Specifies the semantic of the property value statement */
typedef enum {
    UA_EXPRESSIONSEMANTICENUM_ASSURANCE = 0,
    UA_EXPRESSIONSEMANTICENUM_SETTING = 1,
    UA_EXPRESSIONSEMANTICENUM_MEASUREMENT = 2,
    UA_EXPRESSIONSEMANTICENUM_REQUIREMENT = 3
} UA_ExpressionSemanticEnum;

#define UA_OPENAAS_EXPRESSIONSEMANTICENUM 0

/**
 * ViewEnum
 * ^^^^^^^^
 * Kind of View */
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

#define UA_OPENAAS_VIEWENUM 1

/**
 * RelationalExpressionEnum
 * ^^^^^^^^^^^^^^^^^^^^^^^^
 * Specifies the relational semantic of the expression */
typedef enum {
    UA_RELATIONALEXPRESSIONENUM_GREATER_THAN = 0,
    UA_RELATIONALEXPRESSIONENUM_GREATER_EQUAL = 1,
    UA_RELATIONALEXPRESSIONENUM_EQUAL = 2,
    UA_RELATIONALEXPRESSIONENUM_NOT_EQUAL = 3,
    UA_RELATIONALEXPRESSIONENUM_LESS_EQUAL = 4,
    UA_RELATIONALEXPRESSIONENUM_LESS_THAN = 5
} UA_RelationalExpressionEnum;

#define UA_OPENAAS_RELATIONALEXPRESSIONENUM 2

/**
 * IdEnum
 * ^^^^^^
 * Determines which kind of Id is specified */
typedef enum {
    UA_IDENUM_URI = 0,
    UA_IDENUM_ISO = 1
} UA_IdEnum;

#define UA_OPENAAS_IDENUM 3

/**
 * Identification
 * ^^^^^^^^^^^^^^
 * Identification */
typedef struct {
    UA_String idSpec;
    UA_IdEnum idType;
} UA_Identification;

#define UA_OPENAAS_IDENTIFICATION 4

/**
 * LifeCycleEntry
 * ^^^^^^^^^^^^^^
 * LifeCycleEntry  */
typedef struct {
    UA_Identification creatingInstance;
    UA_Identification writingInstance;
    UA_DataValue data;
    UA_String subject;
    UA_String eventClass;
    UA_ViewEnum view;
} UA_LifeCycleEntry;

#define UA_OPENAAS_LIFECYCLEENTRY 5

/**
 * PropertyValueStatement
 * ^^^^^^^^^^^^^^^^^^^^^^
 * Property Value Statement */
typedef struct {
    UA_UInt32 expressionSemantic;
    UA_UInt32 relationalExpression;
    UA_String unit;
    UA_Variant value;
    UA_Identification propertyReference;
    UA_ViewEnum view;
} UA_PropertyValueStatement;

#define UA_OPENAAS_PROPERTYVALUESTATEMENT 6

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* UA_OPENAAS_GENERATED_H_ */
