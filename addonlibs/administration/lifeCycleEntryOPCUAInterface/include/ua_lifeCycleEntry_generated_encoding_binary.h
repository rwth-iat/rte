/* Generated from Opc.Ua.Types.bsd, identification.bsd, Custom.Opc.Ua.AssetAdministrationShell.bsd with script /home/ubuntu/opcuaParser/tools/generate_datatypes.py
 * on host ubuntu-VirtualBox by user ubuntu at 2017-09-29 03:11:03 */
#ifndef UA_LIFECYCLEENTRY_GENERATED_ENCODING_BINARY_H_
#define UA_LIFECYCLEENTRY_GENERATED_ENCODING_BINARY_H_

#ifdef UA_NO_AMALGAMATION
#include "ua_types_encoding_binary.h"
#endif
#include "ua_lifeCycleEntry_generated.h"

/* LifeCycleEntry */
static UA_INLINE UA_StatusCode
UA_LifeCycleEntry_encodeBinary(const UA_LifeCycleEntry *src, UA_ByteString *dst, size_t *offset) {
    return UA_encodeBinary(src, &UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_LIFECYCLEENTRY], NULL, NULL, dst, offset);
}
static UA_INLINE UA_StatusCode
UA_LifeCycleEntry_decodeBinary(const UA_ByteString *src, size_t *offset, UA_LifeCycleEntry *dst) {
    return UA_decodeBinary(src, offset, dst, &UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_LIFECYCLEENTRY], 0, NULL);
}

#endif
