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
