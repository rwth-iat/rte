/* Generated from identification.bsd, lce.bsd with script generate_datatypes.py
 * on host ubuntu-VirtualBox by user ubuntu at 2019-06-03 05:49:28 */

# include "open62541.h"
#include "ua_lifeCycleEntry_generated.h"

/* LifeCycleEntry */
static UA_INLINE size_t
UA_LifeCycleEntry_calcSizeBinary(const UA_LifeCycleEntry *src) {
    return UA_calcSizeBinary(src, &UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_LIFECYCLEENTRY]);
}
static UA_INLINE UA_StatusCode
UA_LifeCycleEntry_encodeBinary(const UA_LifeCycleEntry *src, UA_Byte **bufPos, const UA_Byte *bufEnd) {
    return UA_encodeBinary(src, &UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_LIFECYCLEENTRY], bufPos, &bufEnd, NULL, NULL);
}
static UA_INLINE UA_StatusCode
UA_LifeCycleEntry_decodeBinary(const UA_ByteString *src, size_t *offset, UA_LifeCycleEntry *dst) {
    return UA_decodeBinary(src, offset, dst, &UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_LIFECYCLEENTRY], NULL);
}
