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

UA_Nodestore* opcua_ovTrafo_new(OV_INSTPTR_opcua_server context);
void opcua_ovTrafo_delete(UA_Nodestore * nodestoreInterface);

#endif /* opcua_ovTrafo_H_ */
