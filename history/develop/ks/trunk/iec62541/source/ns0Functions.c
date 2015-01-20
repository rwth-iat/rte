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
		UA_AddNodesItem *nodesToAdd,
		UA_UInt32 *indices,
		UA_UInt32 indicesSize,
		UA_AddNodesResult* addNodesResults,
		UA_DiagnosticInfo *diagnosticInfos
) {
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

OV_DLLFNCEXPORT UA_Int32 iec62541_uaNamespace0_addReferences(
		UA_AddReferencesItem* referencesToAdd,
		UA_UInt32 *indices,
		UA_UInt32 indicesSize,
		UA_StatusCode *addReferencesResults,
		UA_DiagnosticInfo diagnosticInfos
) {
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

OV_DLLFNCEXPORT UA_Int32 iec62541_uaNamespace0_deleteNodes(
		UA_DeleteNodesItem *nodesToDelete,
		UA_UInt32 *indices,
		UA_UInt32 indicesSize,
		UA_StatusCode *deleteNodesResults,
		UA_DiagnosticInfo *diagnosticInfos
) {
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

OV_DLLFNCEXPORT UA_Int32 iec62541_uaNamespace0_deleteReferences(
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
			readNodesResults[indices[i]].encodingMask = UA_DATAVALUE_ENCODINGMASK_STATUSCODE;
			continue;
		}
		pNode = iec62541_uaNamespace0_getNodePtr(readValueIds[indices[i]].nodeId.identifier.numeric);

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
				readNodesResults[indices[i]].value.vt = &UA_TYPES[UA_NODEID];
				readNodesResults[indices[i]].value.storage.data.arrayLength = 1;
				readNodesResults[indices[i]].value.storage.data.dataPtr = nodeId;
				readNodesResults[indices[i]].value.storage.data.arrayDimensionsLength = 1;
				readNodesResults[indices[i]].value.storage.data.arrayDimensions = NULL;
				readNodesResults[indices[i]].encodingMask = UA_DATAVALUE_ENCODINGMASK_VARIANT;
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
				readNodesResults[indices[i]].value.vt = &UA_TYPES[UA_NODECLASS];
				readNodesResults[indices[i]].value.storage.data.arrayLength = 1;
				readNodesResults[indices[i]].value.storage.data.dataPtr = nodeClass;
				readNodesResults[indices[i]].value.storage.data.arrayDimensionsLength = 1;
				readNodesResults[indices[i]].value.storage.data.arrayDimensions = NULL;
				readNodesResults[indices[i]].encodingMask = UA_DATAVALUE_ENCODINGMASK_VARIANT;
			}
			break;
		case UA_ATTRIBUTEID_BROWSENAME:
		case UA_ATTRIBUTEID_DISPLAYNAME:
		{
			UA_String *name;
			name = UA_String_new();
			if(!name){
				readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
				break;
			}
			UA_String_copycstring(pNode->v_identifier, name);
			readNodesResults[indices[i]].status = UA_STATUSCODE_GOOD;
			readNodesResults[indices[i]].value.vt = &UA_TYPES[UA_STRING];
			readNodesResults[indices[i]].value.storage.data.arrayLength = 1;
			readNodesResults[indices[i]].value.storage.data.dataPtr = name;
			readNodesResults[indices[i]].value.storage.data.arrayDimensionsLength = 1;
			readNodesResults[indices[i]].value.storage.data.arrayDimensions = NULL;
			readNodesResults[indices[i]].encodingMask = UA_DATAVALUE_ENCODINGMASK_VARIANT;
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
					readNodesResults[indices[i]].value.vt = &UA_TYPES[UA_UINT32];
					readNodesResults[indices[i]].value.storage.data.arrayLength = 1;
					readNodesResults[indices[i]].value.storage.data.dataPtr = writeMask;
					readNodesResults[indices[i]].value.storage.data.arrayDimensionsLength = 1;
					readNodesResults[indices[i]].value.storage.data.arrayDimensions = NULL;
					readNodesResults[indices[i]].encodingMask = UA_DATAVALUE_ENCODINGMASK_VARIANT;
				} else {
					readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_GENERIC);
				}
			}
		}
		break;
		case UA_ATTRIBUTEID_ISABSTRACT:
		{
			UA_Boolean *isAbstract;
			isAbstract = UA_Boolean_new();
			if(!isAbstract){
				readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
				break;
			}
			tempPointer = iec62541_ns0Node_getValuePointer(pNode, "IsAbstract", &(readNodesResults[indices[i]].status));
			if(readNodesResults[indices[i]].status == UA_STATUSCODE_GOOD){
				if(tempPointer){
					if(*((OV_BOOL*)tempPointer) == TRUE){
						*isAbstract = UA_TRUE;
					} else {
						*isAbstract = UA_FALSE;
					}
					readNodesResults[indices[i]].status = UA_STATUSCODE_GOOD;
					readNodesResults[indices[i]].value.vt = &UA_TYPES[UA_BOOLEAN];
					readNodesResults[indices[i]].value.storage.data.arrayLength = 1;
					readNodesResults[indices[i]].value.storage.data.dataPtr = isAbstract;
					readNodesResults[indices[i]].value.storage.data.arrayDimensionsLength = 1;
					readNodesResults[indices[i]].value.storage.data.arrayDimensions = NULL;
					readNodesResults[indices[i]].encodingMask = UA_DATAVALUE_ENCODINGMASK_VARIANT;
				} else {
					readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_GENERIC);
				}
			}
		}
		break;
		case UA_ATTRIBUTEID_SYMMETRIC:
		{
			UA_Boolean *isSymmetric;
			isSymmetric = UA_Boolean_new();
			if(!isSymmetric){
				readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
				break;
			}
			tempPointer = iec62541_ns0Node_getValuePointer(pNode, "Symmetric", &(readNodesResults[indices[i]].status));
			if(readNodesResults[indices[i]].status == UA_STATUSCODE_GOOD){
				if(tempPointer){
					if(*((OV_BOOL*)tempPointer) == TRUE){
						*isSymmetric = UA_TRUE;
					} else {
						*isSymmetric = UA_FALSE;
					}
					readNodesResults[indices[i]].status = UA_STATUSCODE_GOOD;
					readNodesResults[indices[i]].value.vt = &UA_TYPES[UA_BOOLEAN];
					readNodesResults[indices[i]].value.storage.data.arrayLength = 1;
					readNodesResults[indices[i]].value.storage.data.dataPtr = isSymmetric;
					readNodesResults[indices[i]].value.storage.data.arrayDimensionsLength = 1;
					readNodesResults[indices[i]].value.storage.data.arrayDimensions = NULL;
					readNodesResults[indices[i]].encodingMask = UA_DATAVALUE_ENCODINGMASK_VARIANT;
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
			eventNotifer = UA_Byte_new();
			if(!eventNotifer){
				readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
				break;
			}
			tempPointer = iec62541_ns0Node_getValuePointer(pNode, "EventNotifier", &(readNodesResults[indices[i]].status));
			if(readNodesResults[indices[i]].status == UA_STATUSCODE_GOOD){
				if(tempPointer){
					*eventNotifer = *((OV_UINT*)tempPointer);
					readNodesResults[indices[i]].status = UA_STATUSCODE_GOOD;
					readNodesResults[indices[i]].value.vt = &UA_TYPES[UA_BYTE];
					readNodesResults[indices[i]].value.storage.data.arrayLength = 1;
					readNodesResults[indices[i]].value.storage.data.dataPtr = eventNotifer;
					readNodesResults[indices[i]].value.storage.data.arrayDimensionsLength = 1;
					readNodesResults[indices[i]].value.storage.data.arrayDimensions = NULL;
					readNodesResults[indices[i]].encodingMask = UA_DATAVALUE_ENCODINGMASK_VARIANT;
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
					readNodesResults[indices[i]].encodingMask = UA_DATAVALUE_ENCODINGMASK_VARIANT;
				} else {
					readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_GENERIC);
				}
			}
		}
		break;
		case UA_ATTRIBUTEID_DATATYPE:
		{
			UA_NodeId *dataType;
			dataType = UA_NodeId_new();
			if(!dataType){
				readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
				ov_memstack_unlock();
				break;
			}
			dataType->namespaceIndex = 0;
			dataType->identifierType = UA_NODEIDTYPE_NUMERIC;
			tempPointer = iec62541_ns0Node_getValuePointer(pNode, "DataType", &(readNodesResults[indices[i]].status));
			if(readNodesResults[indices[i]].status == UA_STATUSCODE_GOOD){
				if(tempPointer){
					dataType->identifier.numeric = *((OV_UINT*)tempPointer);
					readNodesResults[indices[i]].value.vt = &UA_TYPES[UA_NODEID];
					readNodesResults[indices[i]].value.storage.data.arrayLength = 1;
					readNodesResults[indices[i]].value.storage.data.dataPtr = dataType;
					readNodesResults[indices[i]].value.storage.data.arrayDimensionsLength = 1;
					readNodesResults[indices[i]].value.storage.data.arrayDimensions = NULL;
					readNodesResults[indices[i]].encodingMask = UA_DATAVALUE_ENCODINGMASK_VARIANT;

				} else {
					readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_GENERIC);
				}
			}
		}
		break;
		case UA_ATTRIBUTEID_VALUERANK:
		{
			UA_Int32 *valueRank;
			valueRank = UA_Int32_new();
			if(!valueRank){
				readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
				break;
			}
			tempPointer = iec62541_ns0Node_getValuePointer(pNode, "ValueRank", &(readNodesResults[indices[i]].status));
			if(readNodesResults[indices[i]].status == UA_STATUSCODE_GOOD){
				if(tempPointer){
					*valueRank = *((OV_INT*)tempPointer);
					readNodesResults[indices[i]].value.vt = &UA_TYPES[UA_INT32];
					readNodesResults[indices[i]].value.storage.data.arrayLength = 1;
					readNodesResults[indices[i]].value.storage.data.dataPtr = valueRank;
					readNodesResults[indices[i]].value.storage.data.arrayDimensionsLength = 1;
					readNodesResults[indices[i]].value.storage.data.arrayDimensions = NULL;
					readNodesResults[indices[i]].encodingMask = UA_DATAVALUE_ENCODINGMASK_VARIANT;
				}
			}
		}
		break;
		case UA_ATTRIBUTEID_ARRAYDIMENSIONS:
		{
			UA_Int32 *arrayDimensions;
			arrayDimensions = UA_Int32_new();
			if(!arrayDimensions){
				readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
				break;
			}
			tempPointer = iec62541_ns0Node_getValuePointer(pNode, "ArrayDimensions", &(readNodesResults[indices[i]].status));
			if(readNodesResults[indices[i]].status == UA_STATUSCODE_GOOD){
				if(tempPointer){
					*arrayDimensions = *((OV_INT*)tempPointer);
					readNodesResults[indices[i]].value.vt = &UA_TYPES[UA_INT32];
					readNodesResults[indices[i]].value.storage.data.arrayLength = 1;
					readNodesResults[indices[i]].value.storage.data.dataPtr = arrayDimensions;
					readNodesResults[indices[i]].value.storage.data.arrayDimensionsLength = 1;
					readNodesResults[indices[i]].value.storage.data.arrayDimensions = NULL;
					readNodesResults[indices[i]].encodingMask = UA_DATAVALUE_ENCODINGMASK_VARIANT;
				}
			}
		}
		break;
		case UA_ATTRIBUTEID_ACCESSLEVEL:
		case UA_ATTRIBUTEID_USERACCESSLEVEL:
		{
			UA_Byte *accessLevel;
			accessLevel = UA_Byte_new();
			if(!accessLevel){
				readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
				break;
			}
			tempPointer = iec62541_ns0Node_getValuePointer(pNode, "AccessLevel", &(readNodesResults[indices[i]].status));
			if(readNodesResults[indices[i]].status == UA_STATUSCODE_GOOD){
				if(tempPointer){
					*accessLevel = *((OV_UINT*)tempPointer);
					readNodesResults[indices[i]].value.vt = &UA_TYPES[UA_BYTE];
					readNodesResults[indices[i]].value.storage.data.arrayLength = 1;
					readNodesResults[indices[i]].value.storage.data.dataPtr = accessLevel;
					readNodesResults[indices[i]].value.storage.data.arrayDimensionsLength = 1;
					readNodesResults[indices[i]].value.storage.data.arrayDimensions = NULL;
					readNodesResults[indices[i]].encodingMask = UA_DATAVALUE_ENCODINGMASK_VARIANT;
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
			historizing = UA_Boolean_new();
			if(!historizing){
				readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
				break;
			}
			tempPointer = iec62541_ns0Node_getValuePointer(pNode, "Historizing", &(readNodesResults[indices[i]].status));
			if(readNodesResults[indices[i]].status == UA_STATUSCODE_GOOD){
				if(tempPointer){
					if(*((OV_BOOL*)tempPointer) == TRUE){
						*historizing = UA_TRUE;
					} else {
						*historizing = UA_FALSE;
					}
					readNodesResults[indices[i]].status = UA_STATUSCODE_GOOD;
					readNodesResults[indices[i]].value.vt = &UA_TYPES[UA_BOOLEAN];
					readNodesResults[indices[i]].value.storage.data.arrayLength = 1;
					readNodesResults[indices[i]].value.storage.data.dataPtr = historizing;
					readNodesResults[indices[i]].value.storage.data.arrayDimensionsLength = 1;
					readNodesResults[indices[i]].value.storage.data.arrayDimensions = NULL;
					readNodesResults[indices[i]].encodingMask = UA_DATAVALUE_ENCODINGMASK_VARIANT;
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
			readNodesResults[indices[i]].encodingMask = UA_DATAVALUE_ENCODINGMASK_STATUSCODE;
		}
	}
	return UA_STATUSCODE_GOOD;
}

OV_DLLFNCEXPORT UA_Int32 iec62541_uaNamespace0_writeNodes(
		UA_WriteValue *writeValues,
		UA_UInt32 *indices,
		UA_UInt32 indicesSize,
		UA_StatusCode *writeNodesResults,
		UA_DiagnosticInfo *diagnosticInfo
) {
	return UA_STATUSCODE_BADNOTIMPLEMENTED;

}

static OV_BOOL iec62541_nodeClassMaskMatch(OV_INSTPTR_iec62541_uaBaseNodeType pNode, UA_UInt32 mask){
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

static UA_StatusCode iec62541_fillReferenceDescription(
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
		UA_String_copycstring(pNode->v_identifier, &(dst->browseName.name));
		dst->browseName.namespaceIndex = 0;
	}
	if(resultMask & (1<<4)){
		UA_String_copycstring(pNode->v_identifier, &(dst->displayName.text));
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
		dst->referenceTypeId.identifier.numeric = 35; // remove const value
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

		switch(browseDescriptions[indices[i]].referenceTypeId.identifier.numeric){
		case 35:	/*	organizes	*/
		case 33:
		{
			OV_UINT refCount = 0;
			OV_INSTPTR_iec62541_uaBaseNodeType pChild = NULL;
			/*	count references	*/
			if((browseDescriptions[indices[i]].browseDirection == UA_BROWSEDIRECTION_FORWARD)
					|| (browseDescriptions[indices[i]].browseDirection == UA_BROWSEDIRECTION_BOTH)){
				Ov_ForEachChildEx(ov_containment, pNode, pChild, iec62541_uaBaseNodeType){
					if(iec62541_nodeClassMaskMatch(pChild, browseDescriptions[indices[i]].nodeClassMask)){
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
			UA_Array_new((void**) &(browseResults[indices[i]].references), refCount, &UA_TYPES[UA_REFERENCEDESCRIPTION]);
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
						if(iec62541_nodeClassMaskMatch(pChild, browseDescriptions[indices[i]].nodeClassMask)){
							browseResults[indices[i]].statusCode = iec62541_fillReferenceDescription(pChild,
									35 , browseDescriptions[indices[i]].resultMask, &(browseResults[indices[i]].references[refCount]));
							refCount++;
						}
					}
				}
			}
			while(refCount < browseResults[indices[i]].referencesSize){
				if((browseDescriptions[indices[i]].browseDirection == UA_BROWSEDIRECTION_INVERSE)
						|| (browseDescriptions[indices[i]].browseDirection == UA_BROWSEDIRECTION_BOTH)){
					pChild = Ov_DynamicPtrCast(iec62541_uaBaseNodeType, Ov_GetParent(ov_containment, pNode));
					if(iec62541_nodeClassMaskMatch(pChild, browseDescriptions[indices[i]].nodeClassMask)){
						browseResults[indices[i]].statusCode = iec62541_fillReferenceDescription(pChild,
								35 , browseDescriptions[indices[i]].resultMask, &(browseResults[indices[i]].references[refCount]));
						refCount++;
					}
				}
			}

		}
		break;
		default:
			browseResults[indices[i]].statusCode = UA_STATUSCODE_BADNOTSUPPORTED;
			break;
		}

	}
	return UA_STATUSCODE_GOOD;
}

