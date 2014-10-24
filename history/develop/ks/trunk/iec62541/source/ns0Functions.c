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
				UA_NodeId_new(&nodeId);
				if(!nodeId){
					readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
					ov_memstack_unlock();
					break;
				}
				nodeId->namespaceIndex = 0;
				nodeId->identifierType = UA_NODEIDTYPE_NUMERIC;
				nodeId->identifier.numeric = pNode->v_NodeId;
				readNodesResults[indices[i]].value.vt = &UA_[UA_NODEID];
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
				UA_NodeClass_new(&nodeClass);
				if(!nodeClass){
					readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
					break;
				}
				if(Ov_GetParent(ov_instantiation, pNode) == pclass_iec62541_uaObjectTypeNode){
					*nodeClass = UA_NODECLASS_OBJECTTYPE;
				} else if(Ov_GetParent(ov_instantiation, pNode) == pclass_iec62541_uaVariableNode){
					*nodeClass = UA_NODECLASS_VARIABLE;
				} else if(Ov_GetParent(ov_instantiation, pNode) == pclass_iec62541_uaObjectNode){
					*nodeClass = UA_NODECLASS_OBJECT;
				} else if(Ov_GetParent(ov_instantiation, pNode) == pclass_iec62541_uaReferenceTypeNode){
					*nodeClass = UA_NODECLASS_REFERENCETYPE;
				} else if(Ov_GetParent(ov_instantiation, pNode) == pclass_iec62541_uaDataTypeNode){
					*nodeClass = UA_NODECLASS_DATATYPE;
				} else if(Ov_GetParent(ov_instantiation, pNode) == pclass_iec62541_uaVariableTypeNode){
					*nodeClass = UA_NODECLASS_VARIABLETYPE;
				} else {
					*nodeClass = UA_NODECLASS_UNSPECIFIED;
				}
				readNodesResults[indices[i]].status = UA_STATUSCODE_GOOD;
				readNodesResults[indices[i]].value.vt = &UA_[UA_NODECLASS];
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
			UA_String_new(&name);
			if(!name){
				readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
				break;
			}
			UA_String_copycstring(pNode->v_identifier, name);
			readNodesResults[indices[i]].status = UA_STATUSCODE_GOOD;
			readNodesResults[indices[i]].value.vt = &UA_[UA_STRING];
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
			UA_UInt32_new(&writeMask);
			if(!writeMask){
				readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
				break;
			}
			tempPointer = iec62541_ns0Node_getValuePointer(pNode, "WriteMask", &(readNodesResults[indices[i]].status));
			if(readNodesResults[indices[i]].status == UA_STATUSCODE_GOOD){
				if(tempPointer){
					*writeMask = *((OV_UINT*)tempPointer);
					readNodesResults[indices[i]].status = UA_STATUSCODE_GOOD;
					readNodesResults[indices[i]].value.vt = &UA_[UA_UINT32];
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
			UA_Boolean_new(&isAbstract);
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
					readNodesResults[indices[i]].value.vt = &UA_[UA_BOOLEAN];
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
			UA_Boolean_new(&isSymmetric);
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
					readNodesResults[indices[i]].value.vt = &UA_[UA_BOOLEAN];
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
			UA_Byte_new(&eventNotifer);
			if(!eventNotifer){
				readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
				break;
			}
			tempPointer = iec62541_ns0Node_getValuePointer(pNode, "EventNotifier", &(readNodesResults[indices[i]].status));
			if(readNodesResults[indices[i]].status == UA_STATUSCODE_GOOD){
				if(tempPointer){
					*eventNotifer = *((OV_UINT*)tempPointer);
					readNodesResults[indices[i]].status = UA_STATUSCODE_GOOD;
					readNodesResults[indices[i]].value.vt = &UA_[UA_BYTE];
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
			UA_NodeId_new(&dataType);
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
					readNodesResults[indices[i]].value.vt = &UA_[UA_NODEID];
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
			UA_Int32_new(&valueRank);
			if(!valueRank){
				readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
				break;
			}
			tempPointer = iec62541_ns0Node_getValuePointer(pNode, "ValueRank", &(readNodesResults[indices[i]].status));
			if(readNodesResults[indices[i]].status == UA_STATUSCODE_GOOD){
				if(tempPointer){
					*valueRank = *((OV_INT*)tempPointer);
					readNodesResults[indices[i]].value.vt = &UA_[UA_INT32];
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
			UA_Int32_new(&arrayDimensions);
			if(!arrayDimensions){
				readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
				break;
			}
			tempPointer = iec62541_ns0Node_getValuePointer(pNode, "ArrayDimensions", &(readNodesResults[indices[i]].status));
			if(readNodesResults[indices[i]].status == UA_STATUSCODE_GOOD){
				if(tempPointer){
					*arrayDimensions = *((OV_INT*)tempPointer);
					readNodesResults[indices[i]].value.vt = &UA_[UA_INT32];
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
			UA_Byte_new(&accessLevel);
			if(!accessLevel){
				readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
				break;
			}
			tempPointer = iec62541_ns0Node_getValuePointer(pNode, "AccessLevel", &(readNodesResults[indices[i]].status));
			if(readNodesResults[indices[i]].status == UA_STATUSCODE_GOOD){
				if(tempPointer){
					*accessLevel = *((OV_UINT*)tempPointer);
					readNodesResults[indices[i]].value.vt = &UA_[UA_BYTE];
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
			UA_Boolean_new(&historizing);
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
					readNodesResults[indices[i]].value.vt = &UA_[UA_BOOLEAN];
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

OV_DLLFNCEXPORT UA_Int32 iec62541_uaNamespace0_browseNodes(
		UA_UInt32 requestedMaxReferencesPerNode,
		UA_BrowseDescription *browseDescriptions,
		UA_Int32 *indices,
		UA_UInt32 indicesSize,
		UA_BrowseResult *browseResults,
		UA_DiagnosticInfo *diagnosticInfos
) {
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

