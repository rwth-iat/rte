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
UA_StatusCode ov_AnyToVariant(const OV_ANY* pAny, UA_Variant* pVariant);
UA_NodeId ov_varTypeToNodeId(OV_VAR_TYPE type);
UA_StatusCode ov_VariantToAny(const UA_Variant* pVariant, OV_ANY* pAny);
UA_Int32 opcua_nsOv_getNodeClassAndAccess(const OV_ELEMENT* pElem, OV_ACCESS* pAccess);
OV_BOOL opcua_nsOv_nodeClassMaskMatchAndGetAccess(const OV_ELEMENT* pElem, UA_UInt32 mask, OV_ACCESS* pAccess);

//UA_StatusCode opcua_nsOv_fillReferenceDescription(
		//OV_ELEMENT* pElement, UA_UInt16 referenceTypeNamespaceIndex, UA_Int32 referenceType, UA_Boolean isForward, UA_UInt32 resultMask, UA_ReferenceDescription* dst);
/*
UA_StatusCode opcua_ns0_fillReferenceDescription(
		OV_INSTPTR_opcua_uaBaseNodeType pNode, UA_Int32 referenceType, UA_UInt32 resultMask, UA_ReferenceDescription* dst);
*/
/**
 * resolves a UA-nodeId to an OV_PATH object
 * the nodeId has to be of type STRING
 * call ov_memstack_lock() /_unlock() around this one
 */

UA_Int32 opcua_nodeStoreFunctions_resolveNodeIdToPath(UA_NodeId nodeId, OV_PATH* pPath);
OV_INSTPTR_ov_object opcua_nodeStoreFunctions_resolveNodeIdToOvObject(UA_NodeId *nodeId);


UA_Int32 opcua_nodeStoreFunctions_getVtblPointerAndCheckAccess(OV_ELEMENT *pelem, OV_TICKET* pTicket, OV_INSTPTR_ov_object *pInstance, OV_VTBLPTR_ov_object *ppVtblObj, OV_ACCESS *access);


OV_RESULT copyOvStringToOPCUA(OV_STRING src, UA_String* dst);
OV_RESULT copyOPCUAStringToOV(UA_String src, OV_STRING *dst);

OV_RESULT copyOvNodeIdToOPCUA(OV_INSTPTR_opcua_nodeId src, UA_NodeId *dst);
OV_RESULT copyOPCUANodeIdToOV(UA_NodeId *src, OV_INSTPTR_opcua_nodeId dst);

OV_RESULT copyOvExpandedNodeIdToOPCUA(OV_INSTPTR_opcua_expandedNodeId src,
	UA_ExpandedNodeId *dst);
OV_RESULT copyOPCUAExpandedNodeIdToOV(UA_ExpandedNodeId *src,
		OV_INSTPTR_opcua_expandedNodeId dst);

OV_RESULT copyOvQualifiedNameToOPCUA(OV_INSTPTR_opcua_qualifiedName src,
	UA_QualifiedName *dst);
OV_RESULT copyOPCUAQualifiedNameToOV(UA_QualifiedName *src,
		OV_INSTPTR_opcua_qualifiedName dst);

OV_RESULT copyOvLocalizedTextToOPCUA(OV_INSTPTR_opcua_localizedText src,
	UA_LocalizedText *dst);
OV_RESULT copyOPCUALocalizedTextToOV(UA_LocalizedText *src,
		OV_INSTPTR_opcua_localizedText dst);

OV_RESULT copyOvReferenceNodeToOPCUA(OV_INSTPTR_opcua_reference src,
	UA_ReferenceNode *dst);
OV_RESULT copyOPCUAReferenceNodeToOV(UA_ReferenceNode *src,
		OV_INSTPTR_opcua_reference dst);
OV_RESULT createOpcuaReferenceNode(OV_INSTPTR_ov_domain location,
		OV_INSTPTR_opcua_reference *newReferenceNode, OV_STRING name);
#endif /* opcua_HELPERS_H_ */
