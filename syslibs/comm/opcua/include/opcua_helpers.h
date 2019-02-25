/*
 * opcua_helpers.h
 *
 *  Created on: 21.10.2014
 *      Author: lars
 */

#ifndef opcua_HELPERS_H_
#define opcua_HELPERS_H_

#include "opcua.h"
#include "libov/ov_path.h"

//#define UA_NODEID_OVROOT		(42 * 42)		//	1764
//#define UA_NODEID_OVREFTYPES	((42 * 42) + 1)	//	1765
//#define UA_NODEID_OVOBJTYPES	((42 * 42) + 2)	//	1766

#define UA_NODEID_References					31
#define UA_NODEID_NonHierarchicalReferences		32
#define UA_NODEID_HierarchicalReferences		33
#define UA_NODEID_HasChild						34
#define UA_NODEID_Organizes						35
//#define UA_NODEID_HasEventSource				36
//#define UA_NODEID_HasModellingRule				37
//#define UA_NODEID_HasEncoding					38
//#define UA_NODEID_HasDescription				39
#define UA_NODEID_HasTypeDefinition				40
//#define UA_NODEID_GeneratesEvent				41
#define UA_NODEID_Aggregates					44
#define UA_NODEID_HasSubtype					45
#define UA_NODEID_HasProperty					46
#define UA_NODEID_HasComponent					47
//#define UA_NODEID_HasNotifier					48
//#define UA_NODEID_HasOrderedComponent			49

UA_Logger opcua_ovUAlogger_new(void);

UA_StatusCode opcua_helpers_ovResultToUaStatusCode(OV_RESULT result);
UA_StatusCode opcua_helpers_ovAnyToUAVariant(const OV_ANY* pAny, UA_Variant* pVariant);
UA_StatusCode opcua_helpers_UAVariantToOVAny(const UA_Variant* pVariant, OV_ANY* pAny);
UA_NodeId opcua_helpers_ovVarTypeToNodeId(OV_VAR_TYPE type);
UA_Int32 opcua_helpers_getVtblPointerAndCheckAccess(OV_ELEMENT *pelem, OV_TICKET* pTicket, OV_INSTPTR_ov_object *pInstance, OV_VTBLPTR_ov_object *ppVtblObj, OV_ACCESS *access);
UA_Int32 opcua_helpers_getNodeClassAndAccess(const OV_ELEMENT* pElem, OV_ACCESS* pAccess);
OV_BOOL opcua_helpers_nodeClassMaskMatchAndGetAccess(const OV_ELEMENT* pElem, UA_UInt32 mask, OV_ACCESS* pAccess);

OV_RESULT opcua_helpers_copyUAStringToOV(UA_String src, OV_STRING *dst);
UA_Int32 opcua_helpers_resolveNodeIdToPath(UA_NodeId nodeId, OV_PATH* pPath);

#endif /* opcua_HELPERS_H_ */
