/* This work is licensed under a Creative Commons CCZero 1.0 Universal License.
 * See http://creativecommons.org/publicdomain/zero/1.0/ for more information.
 *
 *    Copyright 2019 (c) Julian Grothoff, Torben Deppe, Florian Palm
 */

#ifndef UA_NODESTORE_SWITCH_H_
#define UA_NODESTORE_SWITCH_H_

#include "open62541.h"
/*
 * Forward declaration of nodestore switch
 */
struct UA_Nodestore_Switch;
typedef struct UA_Nodestore_Switch UA_Nodestore_Switch;

/*
 * Creates a Nodestore_Switch with a default nodestore inside
 * Allocates memory for the switch and its default nodestore
 */
UA_StatusCode UA_EXPORT
UA_Nodestore_Switch_new(UA_Nodestore_Switch **nsSwitch);

/*
 * Creates a Nodestore_Switch (like UA_Nodestore_new) but without a default nodestore inside
 * Allocates memory for the switch
 */
UA_StatusCode UA_EXPORT
UA_Nodestore_Switch_newEmpty(UA_Nodestore_Switch **nsSwitch);

/*
 * Links the nsSwitch function pointers to the nodestore
 */
UA_StatusCode UA_EXPORT
UA_Nodestore_Switch_link(UA_Nodestore *ns, UA_Nodestore_Switch* nsSwitch);

/*
 * Creates a new default nodestore and an interface to it
 * Allocates memory for nodestore and the interface
 * The handle to the default nodestore is placed inside the interface context (nsCtx)
 */
UA_StatusCode UA_EXPORT
UA_Nodestore_Default_Interface_new(UA_Nodestore** store);
/*
 * Gets the nodestore interface to an existing switch, which can be created via UA_Nodestore_new or UA_Nodestore_newEmpty
 */
UA_EXPORT UA_Nodestore*
UA_Nodestore_Switch_Interface_get(UA_Nodestore_Switch *storeSwitch);

/*
 * Returns the nodestore interface that is linked to the specified index
 * If useDefault is true, the default nodestore is used if no custom nodestore was linked to the index
 */
UA_EXPORT UA_Nodestore*
UA_Nodestore_Switch_getNodestore(UA_Nodestore_Switch* storeSwitch,
		UA_UInt16 index, UA_Boolean useDefault);
/*
 * Links a nodestore via its nodestore interface to the nodestore switch at the specified namespace index
 * Set with store=NULL is equal to an unlink of the nodestore at the specified index
 * The old store is only unlinked and not deleted
 */
UA_StatusCode UA_EXPORT
UA_Nodestore_Switch_setNodestore(UA_Nodestore_Switch* storeSwitch,
		UA_UInt16 index, UA_Nodestore* store);
/*
 * Links the default nodestore via its nodestore interface to the nodestore switch
 * Set with store=NULL is equal to an unlink of the default nodestore
 * The old store is only unlinked and not deleted
 * The default nodestore is used for every namespace, that has no custom nodestore set
 */
UA_StatusCode UA_EXPORT
UA_Nodestore_Switch_setNodestoreDefault(UA_Nodestore_Switch* storeSwitch,
		UA_Nodestore* store);
/*
 * Replaces a nodestore in all occurances based on the same interface
 * StoreNew=NULL is equal to a complete unlink of the oldNodestore
 */
UA_StatusCode UA_EXPORT
UA_Nodestore_Switch_changeNodestore(
		UA_Nodestore_Switch* storeSwitch, UA_Nodestore *storeOld,
		UA_Nodestore *storeNew);
/*
 * Gets all namespace indices linked to a nodestore interface in the switch
 * Comparision of nodestores is based on the nodestore interface
 * Returns count of occurances
 * If indices parameter is not NULL an array of namespace indices is returned
 */
UA_StatusCode UA_EXPORT
UA_Nodestore_Switch_getIndices(UA_Nodestore_Switch* storeSwitch,
		UA_Nodestore* store, UA_UInt16* count, UA_UInt16** indices);

#endif /* UA_NODESTORE_SWITCH_H_ */
