/* Generated from identification.bsd, lce.bsd with script generate_datatypes.py
 * on host ubuntu-VirtualBox by user ubuntu at 2019-06-03 05:49:28 */

#ifndef LIFECYCLEENTRY_GENERATED_HANDLING_H_
#define LIFECYCLEENTRY_GENERATED_HANDLING_H_

#include "ua_lifeCycleEntry_generated.h"

_UA_BEGIN_DECLS

#if defined(__GNUC__) && __GNUC__ >= 4 && __GNUC_MINOR__ >= 6
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wmissing-field-initializers"
# pragma GCC diagnostic ignored "-Wmissing-braces"
#endif


/* IdEnum */
static UA_INLINE void
UA_IdEnum_init(UA_IdEnum *p) {
    memset(p, 0, sizeof(UA_IdEnum));
}

static UA_INLINE UA_IdEnum *
UA_IdEnum_new(void) {
    return (UA_IdEnum*)UA_new(&UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_IDENUM]);
}

static UA_INLINE UA_StatusCode
UA_IdEnum_copy(const UA_IdEnum *src, UA_IdEnum *dst) {
    *dst = *src;
    return UA_STATUSCODE_GOOD;
}

static UA_INLINE void
UA_IdEnum_deleteMembers(UA_IdEnum *p) {
    memset(p, 0, sizeof(UA_IdEnum));
}

static UA_INLINE void
UA_IdEnum_clear(UA_IdEnum *p) {
    memset(p, 0, sizeof(UA_IdEnum));
}

static UA_INLINE void
UA_IdEnum_delete(UA_IdEnum *p) {
    UA_delete(p, &UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_IDENUM]);
}

/* Identification */
static UA_INLINE void
UA_Identification_init(UA_Identification *p) {
    memset(p, 0, sizeof(UA_Identification));
}

static UA_INLINE UA_Identification *
UA_Identification_new(void) {
    return (UA_Identification*)UA_new(&UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_IDENTIFICATION]);
}

static UA_INLINE UA_StatusCode
UA_Identification_copy(const UA_Identification *src, UA_Identification *dst) {
    return UA_copy(src, dst, &UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_IDENTIFICATION]);
}

static UA_INLINE void
UA_Identification_deleteMembers(UA_Identification *p) {
    UA_clear(p, &UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_IDENTIFICATION]);
}

static UA_INLINE void
UA_Identification_clear(UA_Identification *p) {
    UA_clear(p, &UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_IDENTIFICATION]);
}

static UA_INLINE void
UA_Identification_delete(UA_Identification *p) {
    UA_delete(p, &UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_IDENTIFICATION]);
}

/* LifeCycleEntry */
static UA_INLINE void
UA_LifeCycleEntry_init(UA_LifeCycleEntry *p) {
    memset(p, 0, sizeof(UA_LifeCycleEntry));
}

static UA_INLINE UA_LifeCycleEntry *
UA_LifeCycleEntry_new(void) {
    return (UA_LifeCycleEntry*)UA_new(&UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_LIFECYCLEENTRY]);
}

static UA_INLINE UA_StatusCode
UA_LifeCycleEntry_copy(const UA_LifeCycleEntry *src, UA_LifeCycleEntry *dst) {
    return UA_copy(src, dst, &UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_LIFECYCLEENTRY]);
}

static UA_INLINE void
UA_LifeCycleEntry_deleteMembers(UA_LifeCycleEntry *p) {
    UA_clear(p, &UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_LIFECYCLEENTRY]);
}

static UA_INLINE void
UA_LifeCycleEntry_clear(UA_LifeCycleEntry *p) {
    UA_clear(p, &UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_LIFECYCLEENTRY]);
}

static UA_INLINE void
UA_LifeCycleEntry_delete(UA_LifeCycleEntry *p) {
    UA_delete(p, &UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_LIFECYCLEENTRY]);
}

#if defined(__GNUC__) && __GNUC__ >= 4 && __GNUC_MINOR__ >= 6
# pragma GCC diagnostic pop
#endif

_UA_END_DECLS

#endif /* LIFECYCLEENTRY_GENERATED_HANDLING_H_ */
