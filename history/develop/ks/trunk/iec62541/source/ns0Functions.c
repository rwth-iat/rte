/*
 * ns0Functions.c
 *
 *  Created on: 24.10.2014
 *      Author: lars
 */
#ifndef OV_COMPILE_LIBRARY_iec62541
#define OV_COMPILE_LIBRARY_iec62541
#endif

#include "libov/ov_macros.h"
#include "ksbase.h"
#include "ks_logfile.h"
#include "iec62541.h"
#include "iec62541_helpers.h"
#include "NoneTicketAuthenticator.h"
#include "libov/ov_path.h"
#include "libov/ov_memstack.h"

OV_DLLFNCEXPORT	OV_INSTPTR_iec62541_uaBaseNodeType iec62541_uaNamespace0_getNodePtr(
		OV_UINT nodeId
) {
	OV_INSTPTR_iec62541_uaBaseNodeType	pNode		=	NULL;
	OV_INSTPTR_ov_class					pNodeClass	=	NULL;

	Ov_ForEachChildEx(ov_inheritance, pclass_iec62541_uaBaseNodeType, pNodeClass, ov_class){
		Ov_ForEachChildEx(ov_instantiation, pNodeClass, pNode, iec62541_uaBaseNodeType){
			if(pNode->v_NodeId == nodeId){
				return pNode;
			}
		}
	}


	return NULL;
}

OV_DLLFNCEXPORT UA_Int32 iec62541_uaNamespace0_addNodes(
		void *ensHandle,
		UA_RequestHeader *requestHeader,
		UA_AddNodesItem *nodesToAdd,
		UA_UInt32 *indices,
		UA_UInt32 indicesSize,
		UA_AddNodesResult* addNodesResults,
		UA_DiagnosticInfo *diagnosticInfos
) {
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

OV_DLLFNCEXPORT UA_Int32 iec62541_uaNamespace0_addReferences(
		void *ensHandle,
		UA_RequestHeader *requestHeader,
		UA_AddReferencesItem* referencesToAdd,
		UA_UInt32 *indices,
		UA_UInt32 indicesSize,
		UA_StatusCode *addReferencesResults,
		UA_DiagnosticInfo diagnosticInfos
) {
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

OV_DLLFNCEXPORT UA_Int32 iec62541_uaNamespace0_deleteNodes(
		void *ensHandle,
		UA_RequestHeader *requestHeader,
		UA_DeleteNodesItem *nodesToDelete,
		UA_UInt32 *indices,
		UA_UInt32 indicesSize,
		UA_StatusCode *deleteNodesResults,
		UA_DiagnosticInfo *diagnosticInfos
) {
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

OV_DLLFNCEXPORT UA_Int32 iec62541_uaNamespace0_deleteReferences(
		void *ensHandle,
		UA_RequestHeader *requestHeader,
		UA_DeleteReferencesItem *referenceToDelete,
		UA_UInt32 *indices,
		UA_UInt32 indicesSize,
		UA_StatusCode deleteReferencesresults,
		UA_DiagnosticInfo diagnosticInfos
) {
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}


static void* iec62541_ns0Node_getValuePointer(OV_INSTPTR_iec62541_uaBaseNodeType pNode, OV_STRING varName, UA_StatusCode* result){
	OV_ELEMENT elem;
	OV_ELEMENT parent;
	parent.elemtype = OV_ET_OBJECT;
	parent.pobj = Ov_StaticPtrCast(ov_object, pNode);
	elem.elemtype = OV_ET_NONE;
	*result = ov_resultToUaStatusCode(ov_element_searchpart(&parent, &elem, OV_ET_VARIABLE, varName));
	if(*result == UA_STATUSCODE_GOOD){
		if((elem.elemtype == OV_ET_VARIABLE) && (elem.pvalue)){
			return elem.pvalue;
		}
	}
	return NULL;
}

UA_Int32 iec62541_uaNamespace0_getNodeClass(OV_INSTPTR_iec62541_uaBaseNodeType pNode){
	if(Ov_GetParent(ov_instantiation, pNode) == pclass_iec62541_uaObjectTypeNode){
		return UA_NODECLASS_OBJECTTYPE;
	} else if(Ov_GetParent(ov_instantiation, pNode) == pclass_iec62541_uaVariableNode){
		return UA_NODECLASS_VARIABLE;
	} else if(Ov_GetParent(ov_instantiation, pNode) == pclass_iec62541_uaObjectNode){
		return UA_NODECLASS_OBJECT;
	} else if(Ov_GetParent(ov_instantiation, pNode) == pclass_iec62541_uaReferenceTypeNode){
		return UA_NODECLASS_REFERENCETYPE;
	} else if(Ov_GetParent(ov_instantiation, pNode) == pclass_iec62541_uaDataTypeNode){
		return UA_NODECLASS_DATATYPE;
	} else if(Ov_GetParent(ov_instantiation, pNode) == pclass_iec62541_uaVariableTypeNode){
		return UA_NODECLASS_VARIABLETYPE;
	} else {
		return UA_NODECLASS_UNSPECIFIED;
	}
}

OV_DLLFNCEXPORT UA_Int32 iec62541_uaNamespace0_readNodes(
		void *ensHandle,
		UA_RequestHeader *requestHeader,
		UA_ReadValueId *readValueIds,
		UA_UInt32 *indices,
		UA_UInt32 indicesSize,
		UA_DataValue *readNodesResults,
		UA_Boolean timeStampToReturn,
		UA_DiagnosticInfo *diagnosticInfos
) {
	OV_INSTPTR_iec62541_uaBaseNodeType pNode = NULL;
	void* tempPointer = NULL;
	for(OV_UINT i = 0; i<indicesSize;i++){
		if(readValueIds[indices[i]].nodeId.identifierType != UA_NODEIDTYPE_NUMERIC){
			readNodesResults[indices[i]].status = UA_STATUSCODE_BADNODEIDREJECTED;
			readNodesResults[indices[i]].hasStatus = UA_TRUE;
			readNodesResults[indices[i]].hasValue = UA_FALSE;
			continue;
		}
		pNode = iec62541_uaNamespace0_getNodePtr(readValueIds[indices[i]].nodeId.identifier.numeric);
		if(!pNode){
			KS_logfile_debug(("uaNamespace0_readNodes: could not resolve node for numeric id %u (index %u in list). Skipping.", readValueIds[indices[i]].nodeId.identifier.numeric, i));
			readNodesResults[indices[i]].status = UA_STATUSCODE_BADNODEIDUNKNOWN;
			readNodesResults[indices[i]].hasStatus = UA_TRUE;
			readNodesResults[indices[i]].hasValue = UA_FALSE;
			continue;
		}
		switch(readValueIds[indices[i]].attributeId){
		case UA_ATTRIBUTEID_NODEID:
			{
				UA_NodeId	*nodeId;
				nodeId = UA_NodeId_new();
				if(!nodeId){
					readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
					ov_memstack_unlock();
					break;
				}
				nodeId->namespaceIndex = 0;
				nodeId->identifierType = UA_NODEIDTYPE_NUMERIC;
				nodeId->identifier.numeric = pNode->v_NodeId;
				readNodesResults[indices[i]].status = UA_STATUSCODE_GOOD;
				readNodesResults[indices[i]].value.type = &UA_TYPES[UA_TYPES_NODEID];
				readNodesResults[indices[i]].value.arrayLength = -1;
				readNodesResults[indices[i]].value.data = nodeId;
				readNodesResults[indices[i]].value.arrayDimensionsSize = -1;
				readNodesResults[indices[i]].value.arrayDimensions = NULL;
				readNodesResults[indices[i]].hasStatus = UA_TRUE;
				readNodesResults[indices[i]].hasValue = UA_TRUE;
			}
			break;
		case UA_ATTRIBUTEID_NODECLASS:
			{
				UA_NodeClass 	*nodeClass;
				nodeClass = UA_NodeClass_new();
				if(!nodeClass){
					readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
					break;
				}
				*nodeClass = iec62541_uaNamespace0_getNodeClass(pNode);
				readNodesResults[indices[i]].status = UA_STATUSCODE_GOOD;
				readNodesResults[indices[i]].value.type = &UA_TYPES[UA_TYPES_INT32];
				readNodesResults[indices[i]].value.arrayLength = -1;
				readNodesResults[indices[i]].value.data = nodeClass;
				readNodesResults[indices[i]].value.arrayDimensionsSize = -1;
				readNodesResults[indices[i]].value.arrayDimensions = NULL;
				readNodesResults[indices[i]].hasStatus = UA_TRUE;
				readNodesResults[indices[i]].hasValue = UA_TRUE;
			}
			break;
		case UA_ATTRIBUTEID_BROWSENAME:
		case UA_ATTRIBUTEID_DISPLAYNAME:
		{
			UA_String *name = UA_String_new();
			if(!name){
				readNodesResults[indices[i]].status = UA_STATUSCODE_BADOUTOFMEMORY;
				break;
			}
			*name = UA_String_fromChars(pNode->v_identifier);
			if(name->length == -1){
				readNodesResults[indices[i]].status = UA_STATUSCODE_BADOUTOFMEMORY;
			} else {
				readNodesResults[indices[i]].status = UA_STATUSCODE_GOOD;
			}
			readNodesResults[indices[i]].value.type = &UA_TYPES[UA_TYPES_STRING];
			readNodesResults[indices[i]].value.arrayLength = -1;
			readNodesResults[indices[i]].value.data = name;
			readNodesResults[indices[i]].value.arrayDimensionsSize = -1;
			readNodesResults[indices[i]].hasStatus = UA_TRUE;
			readNodesResults[indices[i]].hasValue = UA_TRUE;
		}
		break;
		case UA_ATTRIBUTEID_DESCRIPTION:
			readNodesResults[indices[i]].status = UA_STATUSCODE_BADNOTSUPPORTED;
			break;
		case UA_ATTRIBUTEID_WRITEMASK:
		case UA_ATTRIBUTEID_USERWRITEMASK:
		{
			UA_UInt32 *writeMask;
			writeMask = UA_UInt32_new();
			if(!writeMask){
				readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
				break;
			}
			tempPointer = iec62541_ns0Node_getValuePointer(pNode, "WriteMask", &(readNodesResults[indices[i]].status));
			if(readNodesResults[indices[i]].status == UA_STATUSCODE_GOOD){
				if(tempPointer){
					*writeMask = *((OV_UINT*)tempPointer);
					readNodesResults[indices[i]].status = UA_STATUSCODE_GOOD;
					readNodesResults[indices[i]].value.type = &UA_TYPES[UA_TYPES_UINT32];
					readNodesResults[indices[i]].value.arrayLength = -1;
					readNodesResults[indices[i]].value.data = writeMask;
					readNodesResults[indices[i]].value.arrayDimensionsSize = -1;
					readNodesResults[indices[i]].value.arrayDimensions = NULL;
					readNodesResults[indices[i]].hasStatus = UA_TRUE;
					readNodesResults[indices[i]].hasValue = UA_TRUE;
				} else {
					readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_GENERIC);
				}
			}
		}
		break;
		case UA_ATTRIBUTEID_ISABSTRACT:
		{
			UA_Boolean *isAbstract;
			tempPointer = iec62541_ns0Node_getValuePointer(pNode, "IsAbstract", &(readNodesResults[indices[i]].status));
			if(readNodesResults[indices[i]].status == UA_STATUSCODE_GOOD){
				if(tempPointer){
					isAbstract = UA_Boolean_new();
					if(!isAbstract){
						readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
						break;
					}
					if(*((OV_BOOL*)tempPointer) == TRUE){
						*isAbstract = UA_TRUE;
					} else {
						*isAbstract = UA_FALSE;
					}
					readNodesResults[indices[i]].status = UA_STATUSCODE_GOOD;
					readNodesResults[indices[i]].value.type = &UA_TYPES[UA_TYPES_BOOLEAN];
					readNodesResults[indices[i]].value.arrayLength = -1;
					readNodesResults[indices[i]].value.data = isAbstract;
					readNodesResults[indices[i]].value.arrayDimensionsSize = -1;
					readNodesResults[indices[i]].value.arrayDimensions = NULL;
					readNodesResults[indices[i]].hasStatus = UA_TRUE;
					readNodesResults[indices[i]].hasValue = UA_TRUE;
				} else {
					readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_GENERIC);
				}
			}
		}
		break;
		case UA_ATTRIBUTEID_SYMMETRIC:
		{
			UA_Boolean *isSymmetric;
			tempPointer = iec62541_ns0Node_getValuePointer(pNode, "Symmetric", &(readNodesResults[indices[i]].status));
			if(readNodesResults[indices[i]].status == UA_STATUSCODE_GOOD){
				if(tempPointer){
					isSymmetric = UA_Boolean_new();
					if(!isSymmetric){
						readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
						break;
					}
					if(*((OV_BOOL*)tempPointer) == TRUE){
						*isSymmetric = UA_TRUE;
					} else {
						*isSymmetric = UA_FALSE;
					}
					readNodesResults[indices[i]].status = UA_STATUSCODE_GOOD;
					readNodesResults[indices[i]].value.type = &UA_TYPES[UA_TYPES_BOOLEAN];
					readNodesResults[indices[i]].value.arrayLength = -1;
					readNodesResults[indices[i]].value.data = isSymmetric;
					readNodesResults[indices[i]].value.arrayDimensionsSize = -1;
					readNodesResults[indices[i]].value.arrayDimensions = NULL;
					readNodesResults[indices[i]].hasStatus = UA_TRUE;
					readNodesResults[indices[i]].hasValue = UA_TRUE;
				} else {
					readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_GENERIC);
				}
			}
		}
		break;
		case UA_ATTRIBUTEID_INVERSENAME:	/*	optional and not really a meaningfull mirror in ov (as we define a reference as an assiciation whereas UA defines a reference as one end of an association)	*/
			readNodesResults[indices[i]].status = UA_STATUSCODE_BADNOTSUPPORTED;
			break;
		case UA_ATTRIBUTEID_CONTAINSNOLOOPS:	/*	ov has no view nodes	*/
			readNodesResults[indices[i]].status = UA_STATUSCODE_BADNOTSUPPORTED;
			break;
		case UA_ATTRIBUTEID_EVENTNOTIFIER:
		{
			UA_Byte *eventNotifer;
			tempPointer = iec62541_ns0Node_getValuePointer(pNode, "EventNotifier", &(readNodesResults[indices[i]].status));
			if(readNodesResults[indices[i]].status == UA_STATUSCODE_GOOD){
				if(tempPointer){
					eventNotifer = UA_Byte_new();
					if(!eventNotifer){
						readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
						break;
					}
					*eventNotifer = *((OV_UINT*)tempPointer);
					readNodesResults[indices[i]].status = UA_STATUSCODE_GOOD;
					readNodesResults[indices[i]].value.type = &UA_TYPES[UA_TYPES_BYTE];
					readNodesResults[indices[i]].value.arrayLength = -1;
					readNodesResults[indices[i]].value.data = eventNotifer;
					readNodesResults[indices[i]].value.arrayDimensionsSize = -1;
					readNodesResults[indices[i]].value.arrayDimensions = NULL;
					readNodesResults[indices[i]].hasStatus = UA_TRUE;
					readNodesResults[indices[i]].hasValue = UA_TRUE;
				} else {
					readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_GENERIC);
				}
			}
		}
		break;
		case UA_ATTRIBUTEID_VALUE:
		{
			tempPointer = iec62541_ns0Node_getValuePointer(pNode, "Value", &(readNodesResults[indices[i]].status));
			if(readNodesResults[indices[i]].status == UA_STATUSCODE_GOOD){
				if(tempPointer){
					readNodesResults[indices[i]].status = ov_AnyToVariant((OV_ANY*)tempPointer, &(readNodesResults[indices[i]].value));
					readNodesResults[indices[i]].hasStatus = UA_TRUE;
					readNodesResults[indices[i]].hasValue = UA_TRUE;
				} else {
					readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_GENERIC);
				}
			}
		}
		break;
		case UA_ATTRIBUTEID_DATATYPE:
		{
			UA_NodeId *dataType;
			tempPointer = iec62541_ns0Node_getValuePointer(pNode, "DataType", &(readNodesResults[indices[i]].status));
			if(readNodesResults[indices[i]].status == UA_STATUSCODE_GOOD){
				if(tempPointer){
					dataType = UA_NodeId_new();
					if(!dataType){
						readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
						ov_memstack_unlock();
						break;
					}
					dataType->namespaceIndex = 0;
					dataType->identifierType = UA_NODEIDTYPE_NUMERIC;
					dataType->identifier.numeric = *((OV_UINT*)tempPointer);
					readNodesResults[indices[i]].value.type = &UA_TYPES[UA_TYPES_NODEID];
					readNodesResults[indices[i]].value.arrayLength = -1;
					readNodesResults[indices[i]].value.data = dataType;
					readNodesResults[indices[i]].value.arrayDimensionsSize = -1;
					readNodesResults[indices[i]].value.arrayDimensions = NULL;
					readNodesResults[indices[i]].hasStatus = UA_TRUE;
					readNodesResults[indices[i]].hasValue = UA_TRUE;

				} else {
					readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_GENERIC);
					readNodesResults[indices[i]].hasStatus = UA_TRUE;
					readNodesResults[indices[i]].hasValue = UA_FALSE;
				}
			}
		}
		break;
		case UA_ATTRIBUTEID_VALUERANK:
		{
			UA_Int32 *valueRank;
			tempPointer = iec62541_ns0Node_getValuePointer(pNode, "ValueRank", &(readNodesResults[indices[i]].status));
			if(readNodesResults[indices[i]].status == UA_STATUSCODE_GOOD){
				if(tempPointer){
					valueRank = UA_Int32_new();
					if(!valueRank){
						readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
						break;
					}
					*valueRank = *((OV_INT*)tempPointer);
					readNodesResults[indices[i]].value.type = &UA_TYPES[UA_TYPES_INT32];
					readNodesResults[indices[i]].value.arrayLength = -1;
					readNodesResults[indices[i]].value.data = valueRank;
					readNodesResults[indices[i]].value.arrayDimensionsSize = -1;
					readNodesResults[indices[i]].value.arrayDimensions = NULL;
					readNodesResults[indices[i]].hasStatus = UA_TRUE;
					readNodesResults[indices[i]].hasValue = UA_TRUE;
				}
			}
		}
		break;
		case UA_ATTRIBUTEID_ARRAYDIMENSIONS:
		{
			UA_Int32 *arrayDimensions;
			tempPointer = iec62541_ns0Node_getValuePointer(pNode, "ArrayDimensions", &(readNodesResults[indices[i]].status));
			if(readNodesResults[indices[i]].status == UA_STATUSCODE_GOOD){
				if(tempPointer){
					arrayDimensions = UA_Int32_new();
					if(!arrayDimensions){
						readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
						break;
					}
					*arrayDimensions = *((OV_INT*)tempPointer);
					readNodesResults[indices[i]].value.type = &UA_TYPES[UA_TYPES_INT32];
					readNodesResults[indices[i]].value.arrayLength = -1;
					readNodesResults[indices[i]].value.data = arrayDimensions;
					readNodesResults[indices[i]].value.arrayDimensionsSize = -1;
					readNodesResults[indices[i]].value.arrayDimensions = NULL;
					readNodesResults[indices[i]].hasStatus = UA_TRUE;
					readNodesResults[indices[i]].hasValue = UA_TRUE;
				}
			}
		}
		break;
		case UA_ATTRIBUTEID_ACCESSLEVEL:
		case UA_ATTRIBUTEID_USERACCESSLEVEL:
		{
			UA_Byte *accessLevel;
			tempPointer = iec62541_ns0Node_getValuePointer(pNode, "AccessLevel", &(readNodesResults[indices[i]].status));
			if(readNodesResults[indices[i]].status == UA_STATUSCODE_GOOD){
				if(tempPointer){
					accessLevel = UA_Byte_new();
					if(!accessLevel){
						readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
						break;
					}
					*accessLevel = *((OV_UINT*)tempPointer);
					readNodesResults[indices[i]].value.type = &UA_TYPES[UA_TYPES_BYTE];
					readNodesResults[indices[i]].value.arrayLength = -1;
					readNodesResults[indices[i]].value.data = accessLevel;
					readNodesResults[indices[i]].value.arrayDimensionsSize = -1;
					readNodesResults[indices[i]].value.arrayDimensions = NULL;
					readNodesResults[indices[i]].hasStatus = UA_TRUE;
					readNodesResults[indices[i]].hasValue = UA_TRUE;
				}
			}
		}
		break;
		case UA_ATTRIBUTEID_MINIMUMSAMPLINGINTERVAL:
			readNodesResults[indices[i]].status = UA_STATUSCODE_BADNOTSUPPORTED;
		break;
		case UA_ATTRIBUTEID_HISTORIZING:
		{
			UA_Boolean *historizing;
			tempPointer = iec62541_ns0Node_getValuePointer(pNode, "Historizing", &(readNodesResults[indices[i]].status));
			if(readNodesResults[indices[i]].status == UA_STATUSCODE_GOOD){
				if(tempPointer){
					historizing = UA_Boolean_new();
					if(!historizing){
						readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
						break;
					}
					if(*((OV_BOOL*)tempPointer) == TRUE){
						*historizing = UA_TRUE;
					} else {
						*historizing = UA_FALSE;
					}
					readNodesResults[indices[i]].status = UA_STATUSCODE_GOOD;
					readNodesResults[indices[i]].value.type = &UA_TYPES[UA_TYPES_BOOLEAN];
					readNodesResults[indices[i]].value.arrayLength = -1;
					readNodesResults[indices[i]].value.data = historizing;
					readNodesResults[indices[i]].value.arrayDimensionsSize = -1;
					readNodesResults[indices[i]].value.arrayDimensions = NULL;
					readNodesResults[indices[i]].hasStatus = UA_TRUE;
					readNodesResults[indices[i]].hasValue = UA_TRUE;
				} else {
					readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_GENERIC);
				}
			}
		}
		break;
		case UA_ATTRIBUTEID_EXECUTABLE:
		case UA_ATTRIBUTEID_USEREXECUTABLE:
			readNodesResults[indices[i]].status = UA_STATUSCODE_BADNOTSUPPORTED;
		break;
		/*****************************************************************************************************************************************************
		 * wtf???
		 ****************************************************************************************************************************************************/
		default:
			readNodesResults[indices[i]].status = UA_STATUSCODE_BADATTRIBUTEIDINVALID;
			break;
		}

		if(readNodesResults[indices[i]].status != UA_STATUSCODE_GOOD) {
			readNodesResults[indices[i]].hasStatus = UA_TRUE;
			readNodesResults[indices[i]].hasValue = UA_FALSE;
		}
	}
	return UA_STATUSCODE_GOOD;
}

OV_DLLFNCEXPORT UA_Int32 iec62541_uaNamespace0_writeNodes(
		void *ensHandle,
		UA_RequestHeader *requestHeader,
		UA_WriteValue *writeValues,
		UA_UInt32 *indices,
		UA_UInt32 indicesSize,
		UA_StatusCode *writeNodesResults,
		UA_DiagnosticInfo *diagnosticInfo
) {
	return UA_STATUSCODE_BADNOTIMPLEMENTED;

}

static OV_BOOL iec62541_ns0_nodeClassMaskMatch(OV_INSTPTR_iec62541_uaBaseNodeType pNode, UA_UInt32 mask){
	UA_Int32 nodeClass = iec62541_uaNamespace0_getNodeClass(pNode);
	if(mask == 0){
		return TRUE; //if no bit is set, all attributes should be returned
	}
	switch(nodeClass){
	case UA_NODECLASS_OBJECT:
		return ((mask & (1<<0)) ? TRUE : FALSE);
	case UA_NODECLASS_VARIABLE:
		return ((mask & (1<<1)) ? TRUE : FALSE);
	case UA_NODECLASS_METHOD:
		return ((mask & (1<<2)) ? TRUE : FALSE);
	case UA_NODECLASS_OBJECTTYPE:
		return ((mask & (1<<3)) ? TRUE : FALSE);
	case UA_NODECLASS_VARIABLETYPE:
		return ((mask & (1<<4)) ? TRUE : FALSE);
	case UA_NODECLASS_REFERENCETYPE:
		return ((mask & (1<<5)) ? TRUE : FALSE);
	case UA_NODECLASS_DATATYPE:
		return ((mask & (1<<6)) ? TRUE : FALSE);
	case UA_NODECLASS_VIEW:
		return ((mask & (1<<7)) ? TRUE : FALSE);
	default:
		return FALSE;
	}
}

UA_StatusCode iec62541_ns0_fillReferenceDescription(
		OV_INSTPTR_iec62541_uaBaseNodeType pNode, UA_Int32 referenceType, UA_UInt32 resultMask, UA_ReferenceDescription* dst){
	void *tempPointer = NULL;
	UA_StatusCode result = UA_STATUSCODE_GOOD;
	if(!dst){
		return UA_STATUSCODE_BADINVALIDARGUMENT;
	}
	dst->nodeId.nodeId.identifierType = UA_NODEIDTYPE_NUMERIC;
	dst->nodeId.nodeId.namespaceIndex = 0;
	dst->nodeId.nodeId.identifier.numeric = pNode->v_NodeId;

	if(resultMask & (1<<3)){
		dst->browseName.name = UA_String_fromChars(pNode->v_identifier);
		dst->browseName.namespaceIndex = 0;
	}
	if(resultMask & (1<<4)){
		dst->displayName.text = UA_String_fromChars(pNode->v_identifier);
	}
	if(resultMask & (1<<1)){
		dst->isForward = UA_TRUE;
	}
	if(resultMask & (1<<2)){
		dst->nodeClass = iec62541_uaNamespace0_getNodeClass(pNode);
	}
	if(resultMask & (1<<0)){
		dst->referenceTypeId.namespaceIndex = 0;
		dst->referenceTypeId.identifierType = UA_NODEIDTYPE_NUMERIC;
		dst->referenceTypeId.identifier.numeric = referenceType;
	}
	if(resultMask & (1<<5)){
		dst->typeDefinition.nodeId.namespaceIndex = 0;
		dst->typeDefinition.nodeId.identifierType = UA_NODEIDTYPE_NUMERIC;
		tempPointer = iec62541_ns0Node_getValuePointer(pNode, "Type", &result);
		if((result != UA_STATUSCODE_GOOD) || (!tempPointer)){
			dst->typeDefinition.nodeId.identifier.numeric = 0;
		} else {
			dst->typeDefinition.nodeId.identifier.numeric = *((UA_Int32*)tempPointer);
		}
	}
	return result;
}

OV_DLLFNCEXPORT UA_Int32 iec62541_uaNamespace0_browseNodes(
		void *ensHandle,
		UA_RequestHeader *requestHeader,
		UA_BrowseDescription *browseDescriptions,
		UA_UInt32 *indices,
		UA_UInt32 indicesSize,
		UA_UInt32 requestedMaxReferencesPerNode,
		UA_BrowseResult *browseResults,
		UA_DiagnosticInfo *diagnosticInfos
) {
	OV_INSTPTR_iec62541_uaBaseNodeType pNode = NULL;

	for(OV_UINT i = 0; i<indicesSize;i++){
		if((browseDescriptions[indices[i]].nodeId.identifierType != UA_NODEIDTYPE_NUMERIC)
				|| (browseDescriptions[indices[i]].referenceTypeId.identifierType != UA_NODEIDTYPE_NUMERIC)){
			browseResults[indices[i]].statusCode = UA_STATUSCODE_BADNODEIDREJECTED;
			continue;
		}
		pNode = iec62541_uaNamespace0_getNodePtr(browseDescriptions[indices[i]].nodeId.identifier.numeric);
		if(!pNode){
			KS_logfile_debug(("uaNamespace0_browseNodes: could not resolve node for numeric id %u (index %u in list). Skipping.", browseDescriptions[indices[i]].nodeId.identifier.numeric, i));
			browseResults[indices[i]].statusCode = UA_STATUSCODE_BADNODEIDUNKNOWN;
			continue;
		}

		switch(browseDescriptions[indices[i]].referenceTypeId.identifier.numeric){
		case UA_NODEID_Organizes:
		case UA_NODEID_HierarchicalReferences:
		//case UA_NODEID_References: //FIXME remove me. references /
			if((browseDescriptions[indices[i]].referenceTypeId.identifier.numeric == UA_NODEID_HierarchicalReferences
						/*|| browseDescriptions[indices[i]].referenceTypeId.identifier.numeric == UA_NODEID_References*/)
					&& !(browseDescriptions[indices[i]].includeSubtypes == UA_TRUE)){
				//parent reference types are empty (only Organizes supported)
				browseResults[indices[i]].references = UA_Array_new(&UA_TYPES[UA_TYPES_REFERENCEDESCRIPTION], 0);
				browseResults[indices[i]].referencesSize = 0;
				browseResults[indices[i]].statusCode = UA_STATUSCODE_GOOD;
			} else {
				OV_UINT refCount = 0;
				OV_INSTPTR_iec62541_uaBaseNodeType pChild = NULL;
				if(pNode->v_NodeId != UA_NODEID_OVROOT){
					//	normal namespace 0
					/*	count references	*/
					if((browseDescriptions[indices[i]].browseDirection == UA_BROWSEDIRECTION_FORWARD)
							|| (browseDescriptions[indices[i]].browseDirection == UA_BROWSEDIRECTION_BOTH)){
						Ov_ForEachChildEx(ov_containment, pNode, pChild, iec62541_uaBaseNodeType){
							if(iec62541_ns0_nodeClassMaskMatch(pChild, browseDescriptions[indices[i]].nodeClassMask)){
								refCount++;
							}
						}
					}
					if((browseDescriptions[indices[i]].browseDirection == UA_BROWSEDIRECTION_INVERSE)
							|| (browseDescriptions[indices[i]].browseDirection == UA_BROWSEDIRECTION_BOTH)){
						if(pNode->v_NodeId != 84){	/*	84 is root node	*/
							refCount++;
						}
					}
					browseResults[indices[i]].references = UA_Array_new(&UA_TYPES[UA_TYPES_REFERENCEDESCRIPTION], refCount);
					if(!browseResults[indices[i]].references && refCount>0){
						browseResults[indices[i]].statusCode = UA_STATUSCODE_BADOUTOFMEMORY;
						break;
					}
					browseResults[indices[i]].referencesSize = refCount;
					refCount = 0;
					while(refCount < browseResults[indices[i]].referencesSize){
						if((browseDescriptions[indices[i]].browseDirection == UA_BROWSEDIRECTION_FORWARD)
								|| (browseDescriptions[indices[i]].browseDirection == UA_BROWSEDIRECTION_BOTH)){
							Ov_ForEachChildEx(ov_containment, pNode, pChild, iec62541_uaBaseNodeType){
								if(iec62541_ns0_nodeClassMaskMatch(pChild, browseDescriptions[indices[i]].nodeClassMask)){
									browseResults[indices[i]].statusCode = iec62541_ns0_fillReferenceDescription(pChild,
											UA_NODEID_Organizes , browseDescriptions[indices[i]].resultMask, &(browseResults[indices[i]].references[refCount]));
									refCount++;
								}
							}
						}
					}
					while(refCount < browseResults[indices[i]].referencesSize){
						if((browseDescriptions[indices[i]].browseDirection == UA_BROWSEDIRECTION_INVERSE)
								|| (browseDescriptions[indices[i]].browseDirection == UA_BROWSEDIRECTION_BOTH)){
							pChild = Ov_DynamicPtrCast(iec62541_uaBaseNodeType, Ov_GetParent(ov_containment, pNode));
							if(iec62541_ns0_nodeClassMaskMatch(pChild, browseDescriptions[indices[i]].nodeClassMask)){
								browseResults[indices[i]].statusCode = iec62541_ns0_fillReferenceDescription(pChild,
										UA_NODEID_Organizes , browseDescriptions[indices[i]].resultMask, &(browseResults[indices[i]].references[refCount]));
								refCount++;
							}
						}
					}
				} else {
					// bridging to ov namespace
					/*	count references	*/
					OV_ELEMENT					parent;
					OV_ELEMENT					part;
					OV_ACCESS					access = OV_AC_NONE;
					OV_BOOL						maskMatch;

					parent.elemtype = OV_ET_OBJECT;
					parent.pobj = Ov_PtrUpCast(ov_object, &pdb->root);

					if((browseDescriptions[indices[i]].browseDirection == UA_BROWSEDIRECTION_FORWARD)
							|| (browseDescriptions[indices[i]].browseDirection == UA_BROWSEDIRECTION_BOTH)){
						// children
						part.elemtype = OV_ET_NONE;
						ov_element_getnextchild(&parent, &part);
						while(part.elemtype!=OV_ET_NONE){
							maskMatch = iec62541_nsOv_nodeClassMaskMatchAndGetAccess(&part, browseDescriptions[indices[i]].nodeClassMask, &access);
							if(maskMatch && (access & OV_AC_READ)){
								refCount++;
							}
							ov_element_getnextchild(&parent, &part);
						}
						// parts
						part.elemtype = OV_ET_NONE;
						ov_element_getnextpart(&parent, &part, OV_ET_OBJECT | OV_ET_VARIABLE);
						while(part.elemtype!=OV_ET_NONE){
							maskMatch = iec62541_nsOv_nodeClassMaskMatchAndGetAccess(&part, browseDescriptions[indices[i]].nodeClassMask, &access);
							if(maskMatch && (access & OV_AC_READ)){
								refCount++;
							}
							ov_element_getnextpart(&parent, &part, OV_ET_OBJECT | OV_ET_VARIABLE);
						}
					}
					if((browseDescriptions[indices[i]].browseDirection == UA_BROWSEDIRECTION_INVERSE)
							|| (browseDescriptions[indices[i]].browseDirection == UA_BROWSEDIRECTION_BOTH)){
						refCount++;
					}
					browseResults[indices[i]].references = UA_Array_new(&UA_TYPES[UA_TYPES_REFERENCEDESCRIPTION], refCount);
					if(!browseResults[indices[i]].references && refCount>0){
						browseResults[indices[i]].statusCode = UA_STATUSCODE_BADOUTOFMEMORY;
						break;
					}
					browseResults[indices[i]].referencesSize = refCount;
					refCount = 0;
					if((browseDescriptions[indices[i]].browseDirection == UA_BROWSEDIRECTION_FORWARD)
							|| (browseDescriptions[indices[i]].browseDirection == UA_BROWSEDIRECTION_BOTH)){
						// children
						part.elemtype = OV_ET_NONE;
						ov_element_getnextchild(&parent, &part);
						while(refCount < browseResults[indices[i]].referencesSize && part.elemtype!=OV_ET_NONE){
							maskMatch = iec62541_nsOv_nodeClassMaskMatchAndGetAccess(&part, browseDescriptions[indices[i]].nodeClassMask, &access);
							if(maskMatch && (access & OV_AC_READ)){
								browseResults[indices[i]].statusCode =
										iec62541_nsOv_fillReferenceDescription(&part, 0, UA_NODEID_Organizes , browseDescriptions[indices[i]].resultMask,
												&(browseResults[indices[i]].references[refCount]));
								refCount++;
							}
							ov_element_getnextchild(&parent, &part);
						}
						// parts
						part.elemtype = OV_ET_NONE;
						ov_element_getnextpart(&parent, &part, OV_ET_OBJECT | OV_ET_VARIABLE);
						while(refCount < browseResults[indices[i]].referencesSize && part.elemtype!=OV_ET_NONE){
							maskMatch = iec62541_nsOv_nodeClassMaskMatchAndGetAccess(&part, browseDescriptions[indices[i]].nodeClassMask, &access);
							if(maskMatch && (access & OV_AC_READ)){
								browseResults[indices[i]].statusCode =
										iec62541_nsOv_fillReferenceDescription(&part, 0, UA_NODEID_Organizes , browseDescriptions[indices[i]].resultMask,
												&(browseResults[indices[i]].references[refCount]));
								refCount++;
							}
							ov_element_getnextpart(&parent, &part, OV_ET_OBJECT | OV_ET_VARIABLE);
						}

					}
					while(refCount < browseResults[indices[i]].referencesSize){
						if((browseDescriptions[indices[i]].browseDirection == UA_BROWSEDIRECTION_INVERSE)
								|| (browseDescriptions[indices[i]].browseDirection == UA_BROWSEDIRECTION_BOTH)){
							pChild = Ov_DynamicPtrCast(iec62541_uaBaseNodeType, Ov_GetParent(ov_containment, pNode));
							if(iec62541_ns0_nodeClassMaskMatch(pChild, browseDescriptions[indices[i]].nodeClassMask)){
								browseResults[indices[i]].statusCode = iec62541_ns0_fillReferenceDescription(pChild,
										UA_NODEID_Organizes , browseDescriptions[indices[i]].resultMask, &(browseResults[indices[i]].references[refCount]));
								refCount++;
							}
						}
					}
				}
			}
		break;
		case UA_NODEID_NonHierarchicalReferences:
		case UA_NODEID_HasTypeDefinition:
			if(browseDescriptions[indices[i]].referenceTypeId.identifier.numeric == UA_NODEID_NonHierarchicalReferences
					&& !(browseDescriptions[indices[i]].includeSubtypes == UA_TRUE)){
				//parent reference types are empty (only HasTypeDefinition supported)
				browseResults[indices[i]].references = UA_Array_new(&UA_TYPES[UA_TYPES_REFERENCEDESCRIPTION], 0);
				browseResults[indices[i]].referencesSize = 0;
				browseResults[indices[i]].statusCode = UA_STATUSCODE_GOOD;
			} else {
				OV_INSTPTR_iec62541_uaBaseNodeType	pType = NULL;
				UA_StatusCode						resFindTypeVar;
				OV_UINT								*Type;
				Type =  ((OV_UINT*) iec62541_ns0Node_getValuePointer(pNode, "Type", &resFindTypeVar));
				if(resFindTypeVar != UA_STATUSCODE_GOOD){
					browseResults[indices[i]].statusCode = resFindTypeVar;
					break;
				} else if(!Type){
					browseResults[indices[i]].statusCode = UA_STATUSCODE_BADNOTFOUND;
					break;
				}
				pType = iec62541_uaNamespace0_getNodePtr(*Type);
				if(!pType){
					browseResults[indices[i]].statusCode = UA_STATUSCODE_BADTYPEDEFINITIONINVALID;
					break;
				}
				browseResults[indices[i]].references = UA_Array_new(&UA_TYPES[UA_TYPES_REFERENCEDESCRIPTION], 1);
				if(!browseResults[indices[i]].references){
					browseResults[indices[i]].statusCode = UA_STATUSCODE_BADOUTOFMEMORY;
					break;
				}
				browseResults[indices[i]].referencesSize = 1;
				if(iec62541_ns0_nodeClassMaskMatch(pType, browseDescriptions[indices[i]].nodeClassMask)){
					browseResults[indices[i]].statusCode = iec62541_ns0_fillReferenceDescription(pType,
							UA_NODEID_HasTypeDefinition , browseDescriptions[indices[i]].resultMask, &(browseResults[indices[i]].references[0]));
				}
			}
		break;
		default:
			browseResults[indices[i]].statusCode = UA_STATUSCODE_BADNOTSUPPORTED;
			break;
		}
		if(browseResults[indices[i]].statusCode != UA_STATUSCODE_GOOD){
			browseResults[indices[i]].referencesSize = 0;
			browseResults[indices[i]].references = NULL;
		}
	}
	return UA_STATUSCODE_GOOD;
}

OV_DLLFNCEXPORT UA_Int32 iec62541_uaNamespace0_translateBrowsePathsToNodeIDs(
	void *ensHandle,
	const UA_RequestHeader *requestHeader,
	UA_BrowsePath *browsePath,
	UA_UInt32 *indices,
	UA_UInt32 indicesSize,
	UA_BrowsePathResult *browsePathResults,
	UA_DiagnosticInfo *diagnosticInfos
) {

    return UA_STATUSCODE_BADNOTIMPLEMENTED;
}
