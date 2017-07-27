/* Generated from Opc.Ua.Types.bsd, identification.bsd, lce.bsd, pvs.bsd, Custom.Opc.Ua.AssetAdministrationShell.bsd with script /home/ubuntu/opcuaParser/tools/generate_datatypes.py
 * on host ubuntu-VirtualBox by user ubuntu at 2017-07-26 10:43:58 */

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

/* VisibilityEnum */
static UA_INLINE void
UA_VisibilityEnum_init(UA_VisibilityEnum *p) {
    memset(p, 0, sizeof(UA_VisibilityEnum));
}

static UA_INLINE UA_VisibilityEnum *
UA_VisibilityEnum_new(void) {
    return (UA_VisibilityEnum*)UA_new(&UA_OPENAAS[UA_OPENAAS_VISIBILITYENUM]);
}

static UA_INLINE UA_StatusCode
UA_VisibilityEnum_copy(const UA_VisibilityEnum *src, UA_VisibilityEnum *dst) {
    *dst = *src;
    return UA_STATUSCODE_GOOD;
}

static UA_INLINE void
UA_VisibilityEnum_deleteMembers(UA_VisibilityEnum *p) { }

static UA_INLINE void
UA_VisibilityEnum_delete(UA_VisibilityEnum *p) {
    UA_delete(p, &UA_OPENAAS[UA_OPENAAS_VISIBILITYENUM]);
}

#if defined(__GNUC__) && __GNUC__ >= 4 && __GNUC_MINOR__ >= 6
# pragma GCC diagnostic pop
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* UA_OPENAAS_GENERATED_HANDLING_H_ */
