/* Generated from Opc.Ua.Types.bsd, Custom.Opc.Ua.AssetAdministrationShell.bsd with script /home/opcua/Documents/openAAS_workshop/external/open62541/tools/generate_datatypes.py
 * on host opcua-VirtualBox by user opcua at 2017-01-27 10:27:49 */

#ifndef UA_OPENAAS_GENERATED_HANDLING_H_
#define UA_OPENAAS_GENERATED_HANDLING_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ua_openaas_generated.h"

#if defined(__GNUC__) && __GNUC__ >= 4 && __GNUC_MINOR__ >= 6
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wmissing-field-initializers"
# pragma GCC diagnostic ignored "-Wmissing-braces"
#endif


/* ExpressionSemanticEnum */
static UA_INLINE void
UA_ExpressionSemanticEnum_init(UA_ExpressionSemanticEnum *p) {
    memset(p, 0, sizeof(UA_ExpressionSemanticEnum));
}

static UA_INLINE UA_ExpressionSemanticEnum *
UA_ExpressionSemanticEnum_new(void) {
    return (UA_ExpressionSemanticEnum*)UA_new(&UA_OPENAAS[UA_OPENAAS_EXPRESSIONSEMANTICENUM]);
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
    UA_delete(p, &UA_OPENAAS[UA_OPENAAS_EXPRESSIONSEMANTICENUM]);
}

/* ViewEnum */
static UA_INLINE void
UA_ViewEnum_init(UA_ViewEnum *p) {
    memset(p, 0, sizeof(UA_ViewEnum));
}

static UA_INLINE UA_ViewEnum *
UA_ViewEnum_new(void) {
    return (UA_ViewEnum*)UA_new(&UA_OPENAAS[UA_OPENAAS_VIEWENUM]);
}

static UA_INLINE UA_StatusCode
UA_ViewEnum_copy(const UA_ViewEnum *src, UA_ViewEnum *dst) {
    *dst = *src;
    return UA_STATUSCODE_GOOD;
}

static UA_INLINE void
UA_ViewEnum_deleteMembers(UA_ViewEnum *p) { }

static UA_INLINE void
UA_ViewEnum_delete(UA_ViewEnum *p) {
    UA_delete(p, &UA_OPENAAS[UA_OPENAAS_VIEWENUM]);
}

/* RelationalExpressionEnum */
static UA_INLINE void
UA_RelationalExpressionEnum_init(UA_RelationalExpressionEnum *p) {
    memset(p, 0, sizeof(UA_RelationalExpressionEnum));
}

static UA_INLINE UA_RelationalExpressionEnum *
UA_RelationalExpressionEnum_new(void) {
    return (UA_RelationalExpressionEnum*)UA_new(&UA_OPENAAS[UA_OPENAAS_RELATIONALEXPRESSIONENUM]);
}

static UA_INLINE UA_StatusCode
UA_RelationalExpressionEnum_copy(const UA_RelationalExpressionEnum *src, UA_RelationalExpressionEnum *dst) {
    *dst = *src;
    return UA_STATUSCODE_GOOD;
}

static UA_INLINE void
UA_RelationalExpressionEnum_deleteMembers(UA_RelationalExpressionEnum *p) { }

static UA_INLINE void
UA_RelationalExpressionEnum_delete(UA_RelationalExpressionEnum *p) {
    UA_delete(p, &UA_OPENAAS[UA_OPENAAS_RELATIONALEXPRESSIONENUM]);
}

/* IdEnum */
static UA_INLINE void
UA_IdEnum_init(UA_IdEnum *p) {
    memset(p, 0, sizeof(UA_IdEnum));
}

static UA_INLINE UA_IdEnum *
UA_IdEnum_new(void) {
    return (UA_IdEnum*)UA_new(&UA_OPENAAS[UA_OPENAAS_IDENUM]);
}

static UA_INLINE UA_StatusCode
UA_IdEnum_copy(const UA_IdEnum *src, UA_IdEnum *dst) {
    *dst = *src;
    return UA_STATUSCODE_GOOD;
}

static UA_INLINE void
UA_IdEnum_deleteMembers(UA_IdEnum *p) { }

static UA_INLINE void
UA_IdEnum_delete(UA_IdEnum *p) {
    UA_delete(p, &UA_OPENAAS[UA_OPENAAS_IDENUM]);
}

/* Identification */
static UA_INLINE void
UA_Identification_init(UA_Identification *p) {
    memset(p, 0, sizeof(UA_Identification));
}

static UA_INLINE UA_Identification *
UA_Identification_new(void) {
    return (UA_Identification*)UA_new(&UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
}

static UA_INLINE UA_StatusCode
UA_Identification_copy(const UA_Identification *src, UA_Identification *dst) {
    return UA_copy(src, dst, &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
}

static UA_INLINE void
UA_Identification_deleteMembers(UA_Identification *p) {
    UA_deleteMembers(p, &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
}

static UA_INLINE void
UA_Identification_delete(UA_Identification *p) {
    UA_delete(p, &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
}

/* LifeCycleEntry */
static UA_INLINE void
UA_LifeCycleEntry_init(UA_LifeCycleEntry *p) {
    memset(p, 0, sizeof(UA_LifeCycleEntry));
}

static UA_INLINE UA_LifeCycleEntry *
UA_LifeCycleEntry_new(void) {
    return (UA_LifeCycleEntry*)UA_new(&UA_OPENAAS[UA_OPENAAS_LIFECYCLEENTRY]);
}

static UA_INLINE UA_StatusCode
UA_LifeCycleEntry_copy(const UA_LifeCycleEntry *src, UA_LifeCycleEntry *dst) {
    return UA_copy(src, dst, &UA_OPENAAS[UA_OPENAAS_LIFECYCLEENTRY]);
}

static UA_INLINE void
UA_LifeCycleEntry_deleteMembers(UA_LifeCycleEntry *p) {
    UA_deleteMembers(p, &UA_OPENAAS[UA_OPENAAS_LIFECYCLEENTRY]);
}

static UA_INLINE void
UA_LifeCycleEntry_delete(UA_LifeCycleEntry *p) {
    UA_delete(p, &UA_OPENAAS[UA_OPENAAS_LIFECYCLEENTRY]);
}

/* PropertyValueStatement */
static UA_INLINE void
UA_PropertyValueStatement_init(UA_PropertyValueStatement *p) {
    memset(p, 0, sizeof(UA_PropertyValueStatement));
}

static UA_INLINE UA_PropertyValueStatement *
UA_PropertyValueStatement_new(void) {
    return (UA_PropertyValueStatement*)UA_new(&UA_OPENAAS[UA_OPENAAS_PROPERTYVALUESTATEMENT]);
}

static UA_INLINE UA_StatusCode
UA_PropertyValueStatement_copy(const UA_PropertyValueStatement *src, UA_PropertyValueStatement *dst) {
    return UA_copy(src, dst, &UA_OPENAAS[UA_OPENAAS_PROPERTYVALUESTATEMENT]);
}

static UA_INLINE void
UA_PropertyValueStatement_deleteMembers(UA_PropertyValueStatement *p) {
    UA_deleteMembers(p, &UA_OPENAAS[UA_OPENAAS_PROPERTYVALUESTATEMENT]);
}

static UA_INLINE void
UA_PropertyValueStatement_delete(UA_PropertyValueStatement *p) {
    UA_delete(p, &UA_OPENAAS[UA_OPENAAS_PROPERTYVALUESTATEMENT]);
}

#if defined(__GNUC__) && __GNUC__ >= 4 && __GNUC_MINOR__ >= 6
# pragma GCC diagnostic pop
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* UA_OPENAAS_GENERATED_HANDLING_H_ */
