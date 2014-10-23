/*
 * iec62541_helpers.h
 *
 *  Created on: 21.10.2014
 *      Author: lars
 */

#ifndef IEC62541_HELPERS_H_
#define IEC62541_HELPERS_H_

#include "iec62541.h"
UA_StatusCode ov_resultToUaStatusCode(OV_RESULT result);
UA_StatusCode ov_AnyToVariant(OV_ANY* pAny, UA_Variant* pVariant);
UA_NodeId* ov_varTypeToNodeId(OV_VAR_TYPE type);


#endif /* IEC62541_HELPERS_H_ */
