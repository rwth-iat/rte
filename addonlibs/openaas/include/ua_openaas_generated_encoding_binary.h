/* Generated from Opc.Ua.Types.bsd, Custom.Opc.Ua.AssetAdministrationShell.bsd with script /home/opcua/Documents/openAAS_workshop/external/open62541/tools/generate_datatypes.py
 * on host opcua-VirtualBox by user opcua at 2017-01-26 07:12:03 */

#include "ua_openaas_generated.h"

/* ExpressionSemanticEnum */
static UA_INLINE UA_StatusCode
UA_ExpressionSemanticEnum_encodeBinary(const UA_ExpressionSemanticEnum *src, UA_ByteString *dst, size_t *offset) {
    return UA_encodeBinary(src, &UA_OPENAAS[UA_OPENAAS_EXPRESSIONSEMANTICENUM], NULL, NULL, dst, offset);
}
static UA_INLINE UA_StatusCode
UA_ExpressionSemanticEnum_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ExpressionSemanticEnum *dst) {
    return UA_decodeBinary(src, offset, dst, &UA_OPENAAS[UA_OPENAAS_EXPRESSIONSEMANTICENUM], 0, NULL);
}

/* ViewEnum */
static UA_INLINE UA_StatusCode
UA_ViewEnum_encodeBinary(const UA_ViewEnum *src, UA_ByteString *dst, size_t *offset) {
    return UA_encodeBinary(src, &UA_OPENAAS[UA_OPENAAS_VIEWENUM], NULL, NULL, dst, offset);
}
static UA_INLINE UA_StatusCode
UA_ViewEnum_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ViewEnum *dst) {
    return UA_decodeBinary(src, offset, dst, &UA_OPENAAS[UA_OPENAAS_VIEWENUM], 0, NULL);
}

/* RelationalExpressionEnum */
static UA_INLINE UA_StatusCode
UA_RelationalExpressionEnum_encodeBinary(const UA_RelationalExpressionEnum *src, UA_ByteString *dst, size_t *offset) {
    return UA_encodeBinary(src, &UA_OPENAAS[UA_OPENAAS_RELATIONALEXPRESSIONENUM], NULL, NULL, dst, offset);
}
static UA_INLINE UA_StatusCode
UA_RelationalExpressionEnum_decodeBinary(const UA_ByteString *src, size_t *offset, UA_RelationalExpressionEnum *dst) {
    return UA_decodeBinary(src, offset, dst, &UA_OPENAAS[UA_OPENAAS_RELATIONALEXPRESSIONENUM], 0, NULL);
}

/* IdEnum */
static UA_INLINE UA_StatusCode
UA_IdEnum_encodeBinary(const UA_IdEnum *src, UA_ByteString *dst, size_t *offset) {
    return UA_encodeBinary(src, &UA_OPENAAS[UA_OPENAAS_IDENUM], NULL, NULL, dst, offset);
}
static UA_INLINE UA_StatusCode
UA_IdEnum_decodeBinary(const UA_ByteString *src, size_t *offset, UA_IdEnum *dst) {
    return UA_decodeBinary(src, offset, dst, &UA_OPENAAS[UA_OPENAAS_IDENUM], 0, NULL);
}

/* Identification */
static UA_INLINE UA_StatusCode
UA_Identification_encodeBinary(const UA_Identification *src, UA_ByteString *dst, size_t *offset) {
    return UA_encodeBinary(src, &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION], NULL, NULL, dst, offset);
}
static UA_INLINE UA_StatusCode
UA_Identification_decodeBinary(const UA_ByteString *src, size_t *offset, UA_Identification *dst) {
    return UA_decodeBinary(src, offset, dst, &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION], 0, NULL);
}

/* LifeCycleEntry */
static UA_INLINE UA_StatusCode
UA_LifeCycleEntry_encodeBinary(const UA_LifeCycleEntry *src, UA_ByteString *dst, size_t *offset) {
    return UA_encodeBinary(src, &UA_OPENAAS[UA_OPENAAS_LIFECYCLEENTRY], NULL, NULL, dst, offset);
}
static UA_INLINE UA_StatusCode
UA_LifeCycleEntry_decodeBinary(const UA_ByteString *src, size_t *offset, UA_LifeCycleEntry *dst) {
    return UA_decodeBinary(src, offset, dst, &UA_OPENAAS[UA_OPENAAS_LIFECYCLEENTRY], 0, NULL);
}

/* PropertyValueStatement */
static UA_INLINE UA_StatusCode
UA_PropertyValueStatement_encodeBinary(const UA_PropertyValueStatement *src, UA_ByteString *dst, size_t *offset) {
    return UA_encodeBinary(src, &UA_OPENAAS[UA_OPENAAS_PROPERTYVALUESTATEMENT], NULL, NULL, dst, offset);
}
static UA_INLINE UA_StatusCode
UA_PropertyValueStatement_decodeBinary(const UA_ByteString *src, size_t *offset, UA_PropertyValueStatement *dst) {
    return UA_decodeBinary(src, offset, dst, &UA_OPENAAS[UA_OPENAAS_PROPERTYVALUESTATEMENT], 0, NULL);
}
