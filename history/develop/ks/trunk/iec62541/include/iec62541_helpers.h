/*
 * iec62541_helpers.h
 *
 *  Created on: 21.10.2014
 *      Author: lars
 */

#ifndef IEC62541_HELPERS_H_
#define IEC62541_HELPERS_H_

#include "iec62541.h"
#include "libov/ov_path.h"
UA_StatusCode ov_resultToUaStatusCode(OV_RESULT result);
UA_StatusCode ov_AnyToVariant(OV_ANY* pAny, UA_Variant* pVariant);
UA_NodeId* ov_varTypeToNodeId(OV_VAR_TYPE type);
UA_StatusCode ov_VariantToAny(UA_Variant* pVariant, OV_ANY* pAny);

/**
 * resolves a UA-nodeId to an OV_PATH object
 * the nodeId has to be of type STRING
 * call ov_memstack_lock() /_unlock() around this one
 */

UA_Int32 iec62541_nodeStoreFunctions_resolveNodeIdToPath(UA_NodeId nodeId, OV_PATH* pPath);

UA_Int32 iec62541_nodeStoreFunctions_getVtblPointerAndCheckAccess(OV_ELEMENT *pelem, OV_TICKET* pTicket, OV_INSTPTR_ov_object *pInstance, OV_VTBLPTR_ov_object *ppVtblObj, OV_ACCESS *access);

#endif /* IEC62541_HELPERS_H_ */