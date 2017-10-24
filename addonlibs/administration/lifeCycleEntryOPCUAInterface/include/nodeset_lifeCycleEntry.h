/* WARNING: This is a generated file.
 * Any manual changes will be overwritten.

 */
#ifndef NODESET_H_
#define NODESET_H_
#ifdef UA_NO_AMALGAMATION
#include "server/ua_server_internal.h"
  #include "ua_util.h"
  #include "ua_types.h"
  #include "ua_nodes.h"
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

#define UA_NS2ID_LCESTATUSCODE 3003
#define UA_NS2ID_LIFECYCLEENTRY 3002
#define UA_NS2ID_LIFECYCLEARCHIVETYPE 1002
#define UA_NS2ID_CREATELCE 7001
#define UA_NS2ID_CREATELCE_INPUTARGUMENTS 1882
#define UA_NS2ID_CREATELCE_OUTPUTARGUMENTS 1883
#define UA_NS2ID_CREATELCESIMPLE 7002
#define UA_NS2ID_CREATELCESIMPLE_INPUTARGUMENTS 1885
#define UA_NS2ID_CREATELCESIMPLE_OUTPUTARGUMENTS 1886
#define UA_NS2ID_DELETELCE 7003
#define UA_NS2ID_DELETELCE_INPUTARGUMENTS 1888
#define UA_NS2ID_DELETELCE_OUTPUTARGUMENTS 1889
#define UA_NS2ID_GETLASTLCES 7009
#define UA_NS2ID_GETLASTLCES_INPUTARGUMENTS 1891
#define UA_NS2ID_GETLASTLCES_OUTPUTARGUMENTS 1892
#define UA_NS2ID_GETLCE 7005
#define UA_NS2ID_GETLCE_INPUTARGUMENTS 1894
#define UA_NS2ID_GETLCE_OUTPUTARGUMENTS 1895
#define UA_NS2ID_GETLCESIMPLE 7006
#define UA_NS2ID_GETLCESIMPLE_INPUTARGUMENTS 1897
#define UA_NS2ID_GETLCESIMPLE_OUTPUTARGUMENTS 1898
#define UA_NS2ID_SETLCE 7007
#define UA_NS2ID_SETLCE_INPUTARGUMENTS 1900
#define UA_NS2ID_SETLCE_OUTPUTARGUMENTS 1901
#define UA_NS2ID_SETLCESIMPLE 7008
#define UA_NS2ID_SETLCESIMPLE_INPUTARGUMENTS 1903
#define UA_NS2ID_SETLCESIMPLE_OUTPUTARGUMENTS 1904

  extern UA_StatusCode nodeset(UA_Server *server);
  extern UA_StatusCode nodeset_returnNamespaces(UA_Server *server,
          UA_UInt16 *namespacesSize, UA_Namespace **namspaces);
#endif /* NODESET_H_ */
