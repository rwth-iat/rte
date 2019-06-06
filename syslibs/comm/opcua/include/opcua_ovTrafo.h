/*
 * ovTrafo.h
 *
 *  Created on: 23.02.2019
 *      Author: Julian Grothoff
 *
 */

#ifndef opcua_ovTrafo_H_
#define opcua_ovTrafo_H_

#include "opcua.h"
#define OPCUA_OVTRAFO_DEFAULTNSINDEX 1
#define OPCUA_OVTRAFO_DEFAULTNSURI_PREFIX OPCUA_DEFAULT_APPLICATIONURI


UA_UInt16 opcua_ovTrafo_searchNamespaceIndex(OV_INSTPTR_opcua_interface pInterface, OV_INSTPTR_ov_object pobj, OV_BOOL forTypes); //TODO move to helpers ?
OV_BOOL opcua_ovTrafo_addReferenceToSpecificObject(OV_INSTPTR_opcua_server pServer, OV_UINT application, OV_INSTPTR_ov_object pobj, UA_Node* node);
UA_StatusCode opcua_ovTrafo_addReferences(OV_INSTPTR_opcua_interface pInterface, UA_Node *node);
UA_NodestoreInterface* opcua_ovTrafo_new(OV_INSTPTR_opcua_ovInterface context);
void opcua_ovTrafo_delete(UA_NodestoreInterface * nodestoreInterface);

#endif /* opcua_ovTrafo_H_ */
