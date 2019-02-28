/*
 * trafo.h
 *
 *  Created on: 26.02.2019
 *      Author: julian
 */

#ifndef IPSMS_TRAFO_H_
#define IPSMS_TRAFO_H_

#include "ipsms.h"

UA_Nodestore * ipsms_trafo_new(OV_INSTPTR_ipsms_uaInterface context);
void ipsms_trafo_delete(UA_Nodestore * nodestoreInterface);

#endif /* IPSMS_TRAFO_H_ */
