/* Generated from Opc.Ua.Types.bsd, identification.bsd, Custom.Opc.Ua.AssetAdministrationShell.bsd with script /home/ubuntu/opcuaParser/tools/generate_datatypes.py
 * on host ubuntu-VirtualBox by user ubuntu at 2017-08-01 01:37:54 */

#ifdef UA_NO_AMALGAMATION
#include "ua_types_encoding_binary.h"
#endif
#include "ua_propertyValueStatement_generated.h"

/* ViewEnum */
static UA_INLINE UA_StatusCode
UA_ViewEnum_encodeBinary(const UA_ViewEnum *src, UA_ByteString *dst, size_t *offset) {
    return UA_encodeBinary(src, &UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_VIEWENUM], NULL, NULL, dst, offset);
}
static UA_INLINE UA_StatusCode
UA_ViewEnum_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ViewEnum *dst) {
    return UA_decodeBinary(src, offset, dst, &UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_VIEWENUM], 0, NULL);
}

/* ExpressionLogicEnum */
static UA_INLINE UA_StatusCode
UA_ExpressionLogicEnum_encodeBinary(const UA_ExpressionLogicEnum *src, UA_ByteString *dst, size_t *offset) {
    return UA_encodeBinary(src, &UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_EXPRESSIONLOGICENUM], NULL, NULL, dst, offset);
}
static UA_INLINE UA_StatusCode
UA_ExpressionLogicEnum_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ExpressionLogicEnum *dst) {
    return UA_decodeBinary(src, offset, dst, &UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_EXPRESSIONLOGICENUM], 0, NULL);
}

/* VisibilityEnum */
static UA_INLINE UA_StatusCode
UA_VisibilityEnum_encodeBinary(const UA_VisibilityEnum *src, UA_ByteString *dst, size_t *offset) {
    return UA_encodeBinary(src, &UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_VISIBILITYENUM], NULL, NULL, dst, offset);
}
static UA_INLINE UA_StatusCode
UA_VisibilityEnum_decodeBinary(const UA_ByteString *src, size_t *offset, UA_VisibilityEnum *dst) {
    return UA_decodeBinary(src, offset, dst, &UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_VISIBILITYENUM], 0, NULL);
}

/* ExpressionSemanticEnum */
static UA_INLINE UA_StatusCode
UA_ExpressionSemanticEnum_encodeBinary(const UA_ExpressionSemanticEnum *src, UA_ByteString *dst, size_t *offset) {
    return UA_encodeBinary(src, &UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_EXPRESSIONSEMANTICENUM], NULL, NULL, dst, offset);
}
static UA_INLINE UA_StatusCode
UA_ExpressionSemanticEnum_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ExpressionSemanticEnum *dst) {
    return UA_decodeBinary(src, offset, dst, &UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_EXPRESSIONSEMANTICENUM], 0, NULL);
}
