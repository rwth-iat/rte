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
#define OPCUA_OVTRAFO_OVREFERENCES "||ovReferences"

#define OPCUA_OVTRAFO_ADDHASPROPERTY_FORWARD 			0x00000001
#define OPCUA_OVTRAFO_ADDHASPROPERTY_BACKWARD 			0x00000002
#define OPCUA_OVTRAFO_ADDHASCOMPONENT_FORWARD 			0x00000004
#define OPCUA_OVTRAFO_ADDHASCOMPONENT_BACKWARD 			0x00000008
#define OPCUA_OVTRAFO_ADDORGANIZES_FORWARD 				0x00000010
#define OPCUA_OVTRAFO_ADDORGANIZES_BACKWARD 			0x00000020
#define OPCUA_OVTRAFO_ADDHASTYPEDEFINITION_FORWARD 		0x00000040
#define OPCUA_OVTRAFO_ADDHASTYPEDEFINITION_BACKWARD 	0x00000080
#define OPCUA_OVTRAFO_ADDHASSUBTYPE_FORWARD 			0x00000100
#define OPCUA_OVTRAFO_ADDHASSUBTYPE_BACKWARD 			0x00000200
#define OPCUA_OVTRAFO_ADDOVREFERENCES_FORWARD 			0x00000400
#define OPCUA_OVTRAFO_ADDOVREFERENCES_BACKWARD 			0x00000800
#define OPCUA_OVTRAFO_ADDALL_FORWARD 					0x00000555
#define OPCUA_OVTRAFO_ADDALL_BACKWARD 					0x00000AAA
#define OPCUA_OVTRAFO_ADDALL			 				0x00000FFF

OV_BOOL opcua_ovTrafo_addReferenceToSpecificObject(OV_INSTPTR_opcua_server pServer, OV_INSTPTR_ov_object pobj, UA_Node* node);
UA_StatusCode opcua_ovTrafo_addReferences(OV_INSTPTR_opcua_interface pInterface, UA_Node *node, OV_UINT direction);
UA_NodestoreInterface* opcua_ovTrafo_new(OV_INSTPTR_opcua_ovInterface context);
void opcua_ovTrafo_delete(UA_NodestoreInterface * nodestoreInterface);

#endif /* opcua_ovTrafo_H_ */
