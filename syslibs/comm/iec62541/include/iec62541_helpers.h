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

#define UA_NODEID_OVROOT		(42 * 42)		//	1764
#define UA_NODEID_OVREFTYPES	((42 * 42) + 1)	//	1765
#define UA_NODEID_OVOBJTYPES	((42 * 42) + 2)	//	1766


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

UA_Logger ov_UAlogger_new(void);

UA_StatusCode ov_resultToUaStatusCode(OV_RESULT result);
UA_StatusCode ov_AnyToVariant(OV_ANY* pAny, UA_Variant* pVariant);
UA_NodeId* ov_varTypeToNodeId(OV_VAR_TYPE type);
UA_StatusCode ov_VariantToAny(const UA_Variant* pVariant, OV_ANY* pAny);
UA_Int32 iec62541_nsOv_getNodeClassAndAccess(const OV_ELEMENT* pElem, OV_ACCESS* pAccess);
OV_BOOL iec62541_nsOv_nodeClassMaskMatchAndGetAccess(const OV_ELEMENT* pElem, UA_UInt32 mask, OV_ACCESS* pAccess);

UA_StatusCode iec62541_nsOv_fillReferenceDescription(
		OV_ELEMENT* pElement, UA_UInt16 referenceTypeNamespaceIndex, UA_Int32 referenceType, UA_UInt32 resultMask, UA_ReferenceDescription* dst);
/*
UA_StatusCode iec62541_ns0_fillReferenceDescription(
		OV_INSTPTR_iec62541_uaBaseNodeType pNode, UA_Int32 referenceType, UA_UInt32 resultMask, UA_ReferenceDescription* dst);
*/
/**
 * resolves a UA-nodeId to an OV_PATH object
 * the nodeId has to be of type STRING
 * call ov_memstack_lock() /_unlock() around this one
 */

UA_Int32 iec62541_nodeStoreFunctions_resolveNodeIdToPath(UA_NodeId nodeId, OV_PATH* pPath);

UA_Int32 iec62541_nodeStoreFunctions_getVtblPointerAndCheckAccess(OV_ELEMENT *pelem, OV_TICKET* pTicket, OV_INSTPTR_ov_object *pInstance, OV_VTBLPTR_ov_object *ppVtblObj, OV_ACCESS *access);

UA_ServerNetworkLayer ServerNetworkLayerOV_new(UA_ConnectionConfig conf, UA_UInt32 port);

OV_INSTPTR_iec62541_ovNetworkLayer getOvNetworkLayer();
void iec62541_ovNetworklayer_addConnToDelete(UA_Connection* connection);
void iec62541_ovNetworklayer_addConnToClose(UA_Connection* connection);

#endif /* IEC62541_HELPERS_H_ */
