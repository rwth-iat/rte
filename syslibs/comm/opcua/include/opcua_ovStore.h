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
#define OPCUA_OVSTORE_DEFAULTNSURI_SUFFIX "/Ov"

UA_StatusCode opcua_ovStore_addReference(void * context, UA_Node *node);
UA_Nodestore* opcua_ovStore_new(OV_INSTPTR_opcua_ovInterface context);

#endif /* opcua_ovStore_H_ */
