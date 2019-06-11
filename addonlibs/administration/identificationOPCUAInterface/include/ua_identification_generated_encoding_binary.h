/* Generated from identification.bsd with script generate_datatypes.py
 * on host ubuntu-VirtualBox by user ubuntu at 2019-05-20 02:51:10 */


# include "open62541.h"
#include "ua_identification_generated.h"



/* IdEnum */
static UA_INLINE size_t
UA_IdEnum_calcSizeBinary(const UA_IdEnum *src) {
    return UA_calcSizeBinary(src, &UA_IDENTIFICATION[UA_IDENTIFICATION_IDENUM]);
}
static UA_INLINE UA_StatusCode
UA_IdEnum_encodeBinary(const UA_IdEnum *src, UA_Byte **bufPos, const UA_Byte *bufEnd) {
    return UA_encodeBinary(src, &UA_IDENTIFICATION[UA_IDENTIFICATION_IDENUM], bufPos, &bufEnd, NULL, NULL);
}
static UA_INLINE UA_StatusCode
UA_IdEnum_decodeBinary(const UA_ByteString *src, size_t *offset, UA_IdEnum *dst) {
    return UA_decodeBinary(src, offset, dst, &UA_IDENTIFICATION[UA_IDENTIFICATION_IDENUM], NULL);
}

/* Identification */
static UA_INLINE size_t
UA_Identification_calcSizeBinary(const UA_Identification *src) {
    return UA_calcSizeBinary(src, &UA_IDENTIFICATION[UA_IDENTIFICATION_IDENTIFICATION]);
}
static UA_INLINE UA_StatusCode
UA_Identification_encodeBinary(const UA_Identification *src, UA_Byte **bufPos, const UA_Byte *bufEnd) {
    return UA_encodeBinary(src, &UA_IDENTIFICATION[UA_IDENTIFICATION_IDENTIFICATION], bufPos, &bufEnd, NULL, NULL);
}
static UA_INLINE UA_StatusCode
UA_Identification_decodeBinary(const UA_ByteString *src, size_t *offset, UA_Identification *dst) {
    return UA_decodeBinary(src, offset, dst, &UA_IDENTIFICATION[UA_IDENTIFICATION_IDENTIFICATION], NULL);
}
