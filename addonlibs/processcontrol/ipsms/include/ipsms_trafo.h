/*
 * trafo.h
 *
 *  Created on: 26.02.2019
 *      Author: julian
 */

#ifndef IPSMS_TRAFO_H_
#define IPSMS_TRAFO_H_

#include "ipsms.h"

UA_NodestoreInterface * ipsms_trafo_new(OV_INSTPTR_ipsms_uaInterface context);
void ipsms_trafo_delete(UA_NodestoreInterface * nodestoreInterface);

#endif /* IPSMS_TRAFO_H_ */
