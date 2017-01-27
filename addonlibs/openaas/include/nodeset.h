/* WARNING: This is a generated file.
 * Any manual changes will be overwritten.

 */
#ifndef NODESET_H_
#define NODESET_H_
#ifdef UA_NO_AMALGAMATION
  #include "ua_util.h"
  #include "ua_server.h"
  #include "ua_types_encoding_binary.h"
  #include "ua_types_generated_encoding_binary.h"
  #include "ua_transport_generated_encoding_binary.h"
#else
  #include "open62541.h"
#endif

/* Definition that (in userspace models) may be 
 * - not included in the amalgamated header or
 * - not part of public headers or
 * - not exported in the shared object in combination with any of the above
 * but are required for value encoding.
 * NOTE: Userspace UA_(decode|encode)Binary /wo amalgamations requires UA_EXPORT to be appended to the appropriate definitions. */
#ifndef UA_ENCODINGOFFSET_BINARY
#  define UA_ENCODINGOFFSET_BINARY 2
#endif
#ifndef NULL
  #define NULL ((void *)0)
#endif
#ifndef UA_malloc
  #define UA_malloc(_p_size) malloc(_p_size)
#endif
#ifndef UA_free
  #define UA_free(_p_ptr) free(_p_ptr)
#endif
#define UA_NS2ID_EXPRESSIONSEMANTICENUM 3004
#define UA_NS2ID_IDENUM 3002
#define UA_NS2ID_RELATIONALEXPRESSIONENUM 3003
#define UA_NS2ID_VIEWENUM 3007
#define UA_NS2ID_AASSTATUSCODE 3009
#define UA_NS2ID_IDENTIFICATION 3005
#define UA_NS2ID_LIFECYCLEENTRY 3008
#define UA_NS2ID_PROPERTYVALUESTATEMENT 3011
#define UA_NS2ID_LIFECYCLEENTRYTYPE 2003
#define UA_NS2ID_PROPERTYVALUESTATEMENTTYPE 2002
#define UA_NS2ID_AASFACTORYTYPE 1005
#define UA_NS2ID_CREATEAAS 7001
#define UA_NS2ID_CREATELCE 7007
#define UA_NS2ID_CREATEPVS 7005
#define UA_NS2ID_CREATEPVSL 7003
#define UA_NS2ID_DELETEAAS 7002
#define UA_NS2ID_DELETELCE 7008
#define UA_NS2ID_DELETEPVS 7006
#define UA_NS2ID_DELETEPVSL 7004
#define UA_NS2ID_GETLCE 7012
#define UA_NS2ID_GETPVS 7010
#define UA_NS2ID_SETLCE 7009
#define UA_NS2ID_SETPVS 7011
#define UA_NS2ID_ASSETADMINISTRATIONSHELLTYPE 1004
#define UA_NS2ID_LIFECYCLEARCHIVETYPE 1003
#define UA_NS2ID_PROPERTYVALUESTATEMENTLISTTYPE 1002

extern UA_StatusCode nodeset(UA_Server *server);
extern UA_StatusCode nodeset_returnIndices(UA_Server *server,
   UA_UInt16 *namespacesSize, UA_UInt16 **oldNameSpaceIndices,
   UA_UInt16 **newNameSpaceIndices, UA_String **nameSpaceUri);

#endif /* NODESET_H_ */
