/* Generated from Opc.Ua.Types.bsd, identification.bsd, lce.bsd, pvs.bsd, Custom.Opc.Ua.AssetAdministrationShell.bsd with script /home/ubuntu/opcuaParser/tools/generate_datatypes.py
 * on host ubuntu-VirtualBox by user ubuntu at 2017-07-26 10:43:58 */

#ifdef UA_NO_AMALGAMATION
#include "ua_types_encoding_binary.h"
#endif
#include "ua_openaas_generated.h"

/* ViewEnum */
static UA_INLINE UA_StatusCode
UA_ViewEnum_encodeBinary(const UA_ViewEnum *src, UA_ByteString *dst, size_t *offset) {
    return UA_encodeBinary(src, &UA_OPENAAS[UA_OPENAAS_VIEWENUM], NULL, NULL, dst, offset);
}
static UA_INLINE UA_StatusCode
UA_ViewEnum_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ViewEnum *dst) {
    return UA_decodeBinary(src, offset, dst, &UA_OPENAAS[UA_OPENAAS_VIEWENUM], 0, NULL);
}

/* VisibilityEnum */
static UA_INLINE UA_StatusCode
UA_VisibilityEnum_encodeBinary(const UA_VisibilityEnum *src, UA_ByteString *dst, size_t *offset) {
    return UA_encodeBinary(src, &UA_OPENAAS[UA_OPENAAS_VISIBILITYENUM], NULL, NULL, dst, offset);
}
static UA_INLINE UA_StatusCode
UA_VisibilityEnum_decodeBinary(const UA_ByteString *src, size_t *offset, UA_VisibilityEnum *dst) {
    return UA_decodeBinary(src, offset, dst, &UA_OPENAAS[UA_OPENAAS_VISIBILITYENUM], 0, NULL);
}
