/* Generated from Opc.Ua.Types.bsd, aas.bsd with script ../tools/generate_datatypes.py
 * on host ubuntu-VirtualBox by user ubuntu at 2017-07-05 12:52:11 */

#ifndef OV_COMPILE_LIBRARY_identificationOPCUAInterface
#define OV_COMPILE_LIBRARY_identificationOPCUAInterface

#ifdef __cplusplus
extern "C" {
#endif

#include "ua_identification_generated.h"

#if defined(__GNUC__) && __GNUC__ >= 4 && __GNUC_MINOR__ >= 6
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wmissing-field-initializers"
# pragma GCC diagnostic ignored "-Wmissing-braces"
#endif

//
///* IdEnum */
//static UA_INLINE void
//UA_IdEnum_init(UA_IdEnum *p) {
//    memset(p, 0, sizeof(UA_IdEnum));
//}
//
//static UA_INLINE UA_IdEnum *
//UA_IdEnum_new(void) {
//    return (UA_IdEnum*)UA_new(&UA_IDENTIFICATION[UA_IDENTIFICATION_IDENUM]);
//}
//
//static UA_INLINE UA_StatusCode
//UA_IdEnum_copy(const UA_IdEnum *src, UA_IdEnum *dst) {
//    *dst = *src;
//    return UA_STATUSCODE_GOOD;
//}
//
//static UA_INLINE void
//UA_IdEnum_deleteMembers(UA_IdEnum *p) { }
//
//static UA_INLINE void
//UA_IdEnum_delete(UA_IdEnum *p) {
//    UA_delete(p, &UA_IDENTIFICATION[UA_IDENTIFICATION_IDENUM]);
//}
//
///* Identification */
//static UA_INLINE void
//UA_Identification_init(UA_Identification *p) {
//    memset(p, 0, sizeof(UA_Identification));
//}
//
//static UA_INLINE UA_Identification *
//UA_Identification_new(void) {
//    return (UA_Identification*)UA_new(&UA_IDENTIFICATION[UA_IDENTIFICATION_IDENTIFICATION]);
//}
//
//static UA_INLINE UA_StatusCode
//UA_Identification_copy(const UA_Identification *src, UA_Identification *dst) {
//    return UA_copy(src, dst, &UA_IDENTIFICATION[UA_IDENTIFICATION_IDENTIFICATION]);
//}
//
//static UA_INLINE void
//UA_Identification_deleteMembers(UA_Identification *p) {
//    UA_deleteMembers(p, &UA_IDENTIFICATION[UA_IDENTIFICATION_IDENTIFICATION]);
//}
//
//static UA_INLINE void
//UA_Identification_delete(UA_Identification *p) {
//    UA_delete(p, &UA_IDENTIFICATION[UA_IDENTIFICATION_IDENTIFICATION]);
//}

#if defined(__GNUC__) && __GNUC__ >= 4 && __GNUC_MINOR__ >= 6
# pragma GCC diagnostic pop
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* UA_IDENTIFICATION_GENERATED_HANDLING_H_ */
