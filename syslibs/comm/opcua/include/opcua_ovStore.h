/*
 * ovStore.h
 *
 *  Created on: 23.02.2019
 *      Author: Julian Grothoff
 *
 */

#ifndef opcua_ovStore_H_
#define opcua_ovStore_H_

#include "opcua.h"

#define OPCUA_OVSTORE_DEFAULTNSINDEX 1
#define OPCUA_OVSTORE_DEFAULTNSURI_PREFIX OPCUA_DEFAULT_APPLICATIONURI
#define OPCUA_OVSTORE_DEFAULTNSURI_SUFFIX "/types"

UA_UInt16 opcua_ovStore_searchNamespaceIndex(void * context, OV_INSTPTR_ov_object pobj, OV_BOOL forTypes); //TODO move to helpers ?
UA_StatusCode opcua_ovStore_addReference(void * context, UA_Node *node); //TODO move to helpers and rename to genericTrafoAddReferences or similar
UA_Nodestore* opcua_ovStore_new(OV_INSTPTR_opcua_ovInterface context);
void opcua_ovStore_delete(UA_Nodestore * nodestoreInterface);

#endif /* opcua_ovStore_H_ */
