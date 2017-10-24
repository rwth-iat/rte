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
#define UA_NS2ID_IDENUM 3003
#define UA_NS2ID_IDENTIFICATION 3002

  extern UA_StatusCode nodeset(UA_Server *server);
  extern UA_StatusCode nodeset_returnNamespaces(UA_Server *server,
          UA_UInt16 *namespacesSize, UA_Namespace **namspaces);
#endif /* NODESET_H_ */
