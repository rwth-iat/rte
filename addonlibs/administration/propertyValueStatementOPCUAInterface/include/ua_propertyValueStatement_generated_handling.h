/* Generated from Opc.Ua.Types.bsd, identification.bsd, Custom.Opc.Ua.AssetAdministrationShell.bsd with script /home/ubuntu/opcuaParser/tools/generate_datatypes.py
 * on host ubuntu-VirtualBox by user ubuntu at 2017-07-31 01:37:22 */

#ifndef UA_PROPERTYVALUESTATEMENT_GENERATED_HANDLING_H_
#define UA_PROPERTYVALUESTATEMENT_GENERATED_HANDLING_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ua_propertyValueStatement_generated.h"

#if defined(__GNUC__) && __GNUC__ >= 4 && __GNUC_MINOR__ >= 6
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wmissing-field-initializers"
# pragma GCC diagnostic ignored "-Wmissing-braces"
#endif


/* ExpressionLogicEnum */
static UA_INLINE void
UA_ExpressionLogicEnum_init(UA_ExpressionLogicEnum *p) {
    memset(p, 0, sizeof(UA_ExpressionLogicEnum));
}

static UA_INLINE UA_ExpressionLogicEnum *
UA_ExpressionLogicEnum_new(void) {
    return (UA_ExpressionLogicEnum*)UA_new(&UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_EXPRESSIONLOGICENUM]);
}

static UA_INLINE UA_StatusCode
UA_ExpressionLogicEnum_copy(const UA_ExpressionLogicEnum *src, UA_ExpressionLogicEnum *dst) {
    *dst = *src;
    return UA_STATUSCODE_GOOD;
}

static UA_INLINE void
UA_ExpressionLogicEnum_deleteMembers(UA_ExpressionLogicEnum *p) { }

static UA_INLINE void
UA_ExpressionLogicEnum_delete(UA_ExpressionLogicEnum *p) {
    UA_delete(p, &UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_EXPRESSIONLOGICENUM]);
}

/* ExpressionSemanticEnum */
static UA_INLINE void
UA_ExpressionSemanticEnum_init(UA_ExpressionSemanticEnum *p) {
    memset(p, 0, sizeof(UA_ExpressionSemanticEnum));
}

static UA_INLINE UA_ExpressionSemanticEnum *
UA_ExpressionSemanticEnum_new(void) {
    return (UA_ExpressionSemanticEnum*)UA_new(&UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_EXPRESSIONSEMANTICENUM]);
}

static UA_INLINE UA_StatusCode
UA_ExpressionSemanticEnum_copy(const UA_ExpressionSemanticEnum *src, UA_ExpressionSemanticEnum *dst) {
    *dst = *src;
    return UA_STATUSCODE_GOOD;
}

static UA_INLINE void
UA_ExpressionSemanticEnum_deleteMembers(UA_ExpressionSemanticEnum *p) { }

static UA_INLINE void
UA_ExpressionSemanticEnum_delete(UA_ExpressionSemanticEnum *p) {
    UA_delete(p, &UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_EXPRESSIONSEMANTICENUM]);
}

#if defined(__GNUC__) && __GNUC__ >= 4 && __GNUC_MINOR__ >= 6
# pragma GCC diagnostic pop
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* UA_PROPERTYVALUESTATEMENT_GENERATED_HANDLING_H_ */
