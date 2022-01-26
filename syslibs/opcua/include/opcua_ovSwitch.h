/*
 * opcua_ovSwitch.h
 *
 *  Created on: 23.02.2019
 *      Author: Julian Grothoff
 *
 */

#ifndef opcua_ovSwitch_H_
#define opcua_ovSwitch_H_

#include "opcua.h"

UA_Nodestore* opcua_ovSwitch_new(OV_INSTPTR_opcua_server context);
void opcua_ovSwitch_delete(UA_Nodestore * nodestoreInterface);

#endif /* opcua_ovSwitch_H_ */
