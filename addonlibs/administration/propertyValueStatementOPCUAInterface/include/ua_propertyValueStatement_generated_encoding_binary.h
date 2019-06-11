/* Generated from pvs.bsd with script generate_datatypes.py
 * on host ubuntu-VirtualBox by user ubuntu at 2019-06-05 03:06:38 */

# include "open62541.h"


/* ViewEnum */
static UA_INLINE size_t
UA_ViewEnum_calcSizeBinary(const UA_ViewEnum *src) {
    return UA_calcSizeBinary(src, &UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_VIEWENUM]);
}
static UA_INLINE UA_StatusCode
UA_ViewEnum_encodeBinary(const UA_ViewEnum *src, UA_Byte **bufPos, const UA_Byte *bufEnd) {
    return UA_encodeBinary(src, &UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_VIEWENUM], bufPos, &bufEnd, NULL, NULL);
}
static UA_INLINE UA_StatusCode
UA_ViewEnum_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ViewEnum *dst) {
    return UA_decodeBinary(src, offset, dst, &UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_VIEWENUM], NULL);
}

/* ExpressionLogicEnum */
static UA_INLINE size_t
UA_ExpressionLogicEnum_calcSizeBinary(const UA_ExpressionLogicEnum *src) {
    return UA_calcSizeBinary(src, &UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_EXPRESSIONLOGICENUM]);
}
static UA_INLINE UA_StatusCode
UA_ExpressionLogicEnum_encodeBinary(const UA_ExpressionLogicEnum *src, UA_Byte **bufPos, const UA_Byte *bufEnd) {
    return UA_encodeBinary(src, &UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_EXPRESSIONLOGICENUM], bufPos, &bufEnd, NULL, NULL);
}
static UA_INLINE UA_StatusCode
UA_ExpressionLogicEnum_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ExpressionLogicEnum *dst) {
    return UA_decodeBinary(src, offset, dst, &UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_EXPRESSIONLOGICENUM], NULL);
}

/* VisibilityEnum */
static UA_INLINE size_t
UA_VisibilityEnum_calcSizeBinary(const UA_VisibilityEnum *src) {
    return UA_calcSizeBinary(src, &UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_VISIBILITYENUM]);
}
static UA_INLINE UA_StatusCode
UA_VisibilityEnum_encodeBinary(const UA_VisibilityEnum *src, UA_Byte **bufPos, const UA_Byte *bufEnd) {
    return UA_encodeBinary(src, &UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_VISIBILITYENUM], bufPos, &bufEnd, NULL, NULL);
}
static UA_INLINE UA_StatusCode
UA_VisibilityEnum_decodeBinary(const UA_ByteString *src, size_t *offset, UA_VisibilityEnum *dst) {
    return UA_decodeBinary(src, offset, dst, &UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_VISIBILITYENUM], NULL);
}

/* ExpressionSemanticEnum */
static UA_INLINE size_t
UA_ExpressionSemanticEnum_calcSizeBinary(const UA_ExpressionSemanticEnum *src) {
    return UA_calcSizeBinary(src, &UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_EXPRESSIONSEMANTICENUM]);
}
static UA_INLINE UA_StatusCode
UA_ExpressionSemanticEnum_encodeBinary(const UA_ExpressionSemanticEnum *src, UA_Byte **bufPos, const UA_Byte *bufEnd) {
    return UA_encodeBinary(src, &UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_EXPRESSIONSEMANTICENUM], bufPos, &bufEnd, NULL, NULL);
}
static UA_INLINE UA_StatusCode
UA_ExpressionSemanticEnum_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ExpressionSemanticEnum *dst) {
    return UA_decodeBinary(src, offset, dst, &UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_EXPRESSIONSEMANTICENUM], NULL);
}
