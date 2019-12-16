/*
 * trafo.h
 *
 *  Created on: 26.02.2019
 *      Author: julian
 */

#ifndef IPSMS_TRAFO_H_
#define IPSMS_TRAFO_H_

#include "ipsms.h"

typedef enum IPSMS_PROFILE_SI{
	IPSMS_PROFILE_SI_UNKNOWN,
	IPSMS_PROFILE_SI_NONE,
	IPSMS_PROFILE_SI_ANY,
	IPSMS_PROFILE_SI_CMD,
	IPSMS_PROFILE_SI_OPERATIONS,
	IPSMS_PROFILE_SI_SERVICES
}IPSMS_PROFILE_SI;

UA_NodestoreInterface * ipsms_trafo_new(OV_INSTPTR_ipsms_interface context);
void ipsms_trafo_delete(UA_NodestoreInterface * nodestoreInterface);

#endif /* IPSMS_TRAFO_H_ */
