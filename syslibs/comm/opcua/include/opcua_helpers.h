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

//TODO move OV_DLLFNCEXPORT to header
UA_Logger opcua_ovUAlogger_new(void);

void opcua_helpers_UA_String_append(UA_String * string, const char * append);

UA_StatusCode opcua_helpers_ovResultToUaStatusCode(OV_RESULT result);
UA_StatusCode opcua_helpers_ovAnyToUAVariant(const OV_ANY* pAny, UA_Variant* pVariant);
UA_StatusCode opcua_helpers_UAVariantToOVAny(const UA_Variant* pVariant, OV_ANY* pAny);
UA_NodeId opcua_helpers_ovVarTypeToNodeId(OV_VAR_TYPE type);
UA_Int32 opcua_helpers_getVtblPointerAndCheckAccess(OV_ELEMENT *pelem, OV_TICKET* pTicket, OV_INSTPTR_ov_object *pInstance, OV_VTBLPTR_ov_object *ppVtblObj, OV_ACCESS *access);
OV_ACCESS opcua_helpers_getAccess(const OV_ELEMENT* pElem);
UA_Int32 opcua_helpers_getNodeClass(const OV_ELEMENT* pElem);

OV_RESULT opcua_helpers_copyUAStringToOV(UA_String src, OV_STRING *dst);
UA_Int32 opcua_helpers_resolveNodeIdToPath(const UA_NodeId nodeId, OV_PATH* pPath);
OV_INSTPTR_ov_object opcua_helpers_resolveNodeIdToOvObject(const UA_NodeId *nodeId);

OV_RESULT opcua_helpers_setRootEntryReference(const OV_STRING newPath, OV_INSTPTR_opcua_interface pobj, OV_STRING * poldPath);
UA_StatusCode opcua_helpers_addReference(
		UA_Node* node, const UA_NodeId * sourceNodeId , const UA_NodeId referenceTypeId,
		const UA_ExpandedNodeId targetNodeId, UA_NodeClass targetNodeClass, UA_Boolean isForward);


#endif /* opcua_HELPERS_H_ */
