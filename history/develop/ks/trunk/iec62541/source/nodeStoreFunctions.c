
/******************************************************************************
 *
 *   FILE
 *   ----
 *   nodeStoreFunctions.c
 *
 *   History
 *   -------
 *   2014-10-21   File created
 *
 *******************************************************************************
 *
 *   This file is generated by the 'acplt_builder' command
 *
 ******************************************************************************/


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



OV_DLLFNCEXPORT UA_Int32 iec62541_nodeStoreFunctions_addNodes(
		UA_AddNodesItem *nodesToAdd,
		UA_UInt32 *indices,
		UA_UInt32 indicesSize,
		UA_AddNodesResult* addNodesResults,
		UA_DiagnosticInfo *diagnosticInfos
) {
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

OV_DLLFNCEXPORT UA_Int32 iec62541_nodeStoreFunctions_addReferences(
		UA_AddReferencesItem* referencesToAdd,
		UA_UInt32 *indices,
		UA_UInt32 indicesSize,
		UA_StatusCode *addReferencesResults,
		UA_DiagnosticInfo diagnosticInfos
) {
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

OV_DLLFNCEXPORT UA_Int32 iec62541_nodeStoreFunctions_deleteNodes(
		UA_DeleteNodesItem *nodesToDelete,
		UA_UInt32 *indices,
		UA_UInt32 indicesSize,
		UA_StatusCode *deleteNodesResults,
		UA_DiagnosticInfo *diagnosticInfos
) {
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

OV_DLLFNCEXPORT UA_Int32 iec62541_nodeStoreFunctions_deleteReferences(
		UA_DeleteReferencesItem *referenceToDelete,
		UA_UInt32 *indices,
		UA_UInt32 indicesSize,
		UA_StatusCode deleteReferencesresults,
		UA_DiagnosticInfo diagnosticInfos
) {
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

/**
 * resolves a UA-nodeId to an OV_PATH object
 * the nodeId has to be of type STRING
 * call ov_memstack_lock() /_unlock() around this one
 */

static UA_Int32 iec62541_nodeStoreFunctions_resolveNodeIdToPath(UA_NodeId nodeId, OV_PATH* pPath){
	OV_STRING tmpString = NULL;
	OV_RESULT result;
	if(nodeId.identifierType != UA_NODEIDTYPE_STRING){
		return UA_STATUSCODE_BADNODEIDINVALID;
	}
	tmpString = ov_memstack_alloc(nodeId.identifier.string.length + 1);
	if(!tmpString){
		return UA_STATUSCODE_BADOUTOFMEMORY;
	}
	memcpy(tmpString,nodeId.identifier.string.data,nodeId.identifier.string.length);
	tmpString[nodeId.identifier.string.length] = 0;
	result = ov_path_resolve(pPath,NULL,tmpString, 2);
	if(Ov_Fail(result)){
		return ov_resultToUaStatusCode(result);
	}
	return UA_STATUSCODE_GOOD;
}

static UA_Int32 iec62541_nodeStoreFunctions_getVtblPointerAndCheckAccess(OV_ELEMENT *pelem, OV_TICKET* pTicket, OV_INSTPTR_ov_object *pInstance, OV_VTBLPTR_ov_object *ppVtblObj, OV_ACCESS *access){
	switch(pelem->elemtype){
	case OV_ET_OBJECT:
	case OV_ET_OPERATION:
		*pInstance = pelem->pobj;
		break;
	case OV_ET_MEMBER:
	case OV_ET_VARIABLE:
		*pInstance = pelem->elemunion.pobj;
		break;
	case OV_ET_CHILDLINK:
	case OV_ET_PARENTLINK:
		*pInstance = Ov_StaticPtrCast(ov_object, pelem->elemunion.passoc);
		break;
	default:
		*pInstance = pelem->pobj;
		break;
	}
	Ov_GetVTablePtr(ov_object, *ppVtblObj, *pInstance);
	if((!*ppVtblObj) || (ov_activitylock)){
		*ppVtblObj = pclass_ov_object->v_pvtable;
	}
	*access = (*ppVtblObj)->m_getaccess(pelem->pobj, pelem, pTicket);
	return UA_STATUSCODE_GOOD;
}

OV_DLLFNCEXPORT UA_Int32 iec62541_nodeStoreFunctions_readNodes(
		UA_ReadValueId *readValueIds,
		UA_UInt32 *indices,
		UA_UInt32 indicesSize,
		UA_DataValue *readNodesResults,
		UA_Boolean timeStampToReturn,
		UA_DiagnosticInfo *diagnosticInfos
) {

	OV_TICKET *pTicket = NULL;
	OV_PATH path;
	OV_INSTPTR_ov_object	pobj = NULL;
	OV_VTBLPTR_ov_object	pVtblObj = NULL;
	OV_ACCESS				access;
	OV_ANY					value = {.value.vartype = OV_VT_VOID, .value.valueunion.val_string = NULL};
	OV_ANY					emptyAny = {.value.vartype = OV_VT_VOID, .value.valueunion.val_string = NULL};
	pTicket = ksbase_NoneAuth->v_ticket.vtbl->createticket(NULL, OV_TT_NONE);


	for(OV_UINT i = 0; i<indicesSize;i++){
		switch(readValueIds[indices[i]].attributeId){
		case UA_ATTRIBUTEID_NODEID:
			ov_memstack_lock();
			readNodesResults[indices[i]].status = iec62541_nodeStoreFunctions_resolveNodeIdToPath(readValueIds[indices[i]].nodeId, &path);
			if(readNodesResults[indices[i]].status != UA_STATUSCODE_GOOD){
				ov_memstack_unlock();
				break;
			}
			readNodesResults[indices[i]].status = iec62541_nodeStoreFunctions_getVtblPointerAndCheckAccess(&(path.elements[path.size-1]), pTicket, &pobj, &pVtblObj, &access);
			if(readNodesResults[indices[i]].status != UA_STATUSCODE_GOOD){
				ov_memstack_unlock();
				break;
			}
			if(access & OV_AC_READ){
				UA_NodeId	*nodeId;
				OV_STRING 	tempStr1 = NULL;
				OV_STRING 	tempStr2 = NULL;
				UA_NodeId_new(&nodeId);
				if(!nodeId){
					readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
					ov_memstack_unlock();
					break;
				}
				nodeId->namespaceIndex = readValueIds[indices[i]].nodeId.namespaceIndex;
				nodeId->identifierType = UA_NODEIDTYPE_STRING;
				switch(path.elements[path.size-1].elemtype){
				case OV_ET_OBJECT:
					readNodesResults[indices[i]].status = UA_String_copycstring(ov_path_getcanonicalpath(pobj, 2) , &(nodeId->identifier.string));
					break;
				case OV_ET_VARIABLE:
				case OV_ET_MEMBER:
				case OV_ET_OPERATION:
					tempStr1 = ov_path_getcanonicalpath(path.elements[path.size-1].pobj, 2);
					if(!tempStr1){
						readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
						break;
					}
					if(!path.elements[path.size-1].elemunion.pobj){
						readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
						break;
					}
					tempStr2 = ov_memstack_alloc((strlen(tempStr1) + 1 + strlen(path.elements[path.size-1].elemunion.pobj->v_identifier) + 1) * sizeof(char));
					if(!tempStr1){
						readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
						break;
					}
					sprintf(tempStr2, "%s.%s", tempStr1, path.elements[path.size-1].elemunion.pobj->v_identifier);
					readNodesResults[indices[i]].status = UA_String_copycstring(tempStr2 , &(nodeId->identifier.string));
					break;
				case OV_ET_PARENTLINK:
					tempStr1 = ov_path_getcanonicalpath(path.elements[path.size-1].pobj, 2);
					if(!tempStr1){
						readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
						break;
					}
					if(!path.elements[path.size-1].elemunion.passoc){
						readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
						break;
					}
					tempStr2 = ov_memstack_alloc((strlen(tempStr1) + 1 + strlen(path.elements[path.size-1].elemunion.passoc->v_parentrolename) + 1) * sizeof(char));
					if(!tempStr1){
						readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
						break;
					}
					sprintf(tempStr2, "%s.%s", tempStr1, path.elements[path.size-1].elemunion.passoc->v_parentrolename);
					readNodesResults[indices[i]].status = UA_String_copycstring(tempStr2 , &(nodeId->identifier.string));
					break;
				case OV_ET_CHILDLINK:
					tempStr1 = ov_path_getcanonicalpath(path.elements[path.size-1].pobj, 2);
					if(!tempStr1){
						readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
						break;
					}
					if(!path.elements[path.size-1].elemunion.passoc){
						readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
						break;
					}
					tempStr2 = ov_memstack_alloc((strlen(tempStr1) + 1 + strlen(path.elements[path.size-1].elemunion.passoc->v_childrolename) + 1) * sizeof(char));
					if(!tempStr1){
						readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
						break;
					}
					sprintf(tempStr2, "%s.%s", tempStr1, path.elements[path.size-1].elemunion.passoc->v_childrolename);
					readNodesResults[indices[i]].status = UA_String_copycstring(tempStr2 , &(nodeId->identifier.string));
					break;
				default:
					readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_BADOBJTYPE);
					break;
				}
				readNodesResults[indices[i]].value.vt = &UA_[UA_NODEID];
				readNodesResults[indices[i]].value.storage.data.arrayLength = 1;
				readNodesResults[indices[i]].value.storage.data.dataPtr = nodeId;
				readNodesResults[indices[i]].value.storage.data.arrayDimensionsLength = 1;
				readNodesResults[indices[i]].value.storage.data.arrayDimensions = NULL;
				readNodesResults[indices[i]].encodingMask = UA_DATAVALUE_ENCODINGMASK_VARIANT;
			} else {
				readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_NOACCESS);
			}
			ov_memstack_unlock();
			break;

		case UA_ATTRIBUTEID_NODECLASS:
			ov_memstack_lock();
			readNodesResults[indices[i]].status = iec62541_nodeStoreFunctions_resolveNodeIdToPath(readValueIds[indices[i]].nodeId, &path);
			if(readNodesResults[indices[i]].status != UA_STATUSCODE_GOOD){
				ov_memstack_unlock();
				break;
			}
			readNodesResults[indices[i]].status = iec62541_nodeStoreFunctions_getVtblPointerAndCheckAccess(&(path.elements[path.size-1]), pTicket, &pobj, &pVtblObj, &access);
			if(readNodesResults[indices[i]].status != UA_STATUSCODE_GOOD){
				ov_memstack_unlock();
				break;
			}
			if(access & OV_AC_READ){
				UA_NodeClass 	*nodeClass;
				UA_NodeClass_new(&nodeClass);
				if(!nodeClass){
					readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
				}
				switch(path.elements[path.size-1].elemtype){
				case OV_ET_OBJECT:
					if(Ov_GetParent(ov_instantiation, pobj) == pclass_ov_class){
						*nodeClass = UA_NODECLASS_OBJECTTYPE;
					} else if(Ov_GetParent(ov_instantiation, pobj) == pclass_ov_variable){
						*nodeClass = UA_NODECLASS_VARIABLETYPE;
					} else if(Ov_GetParent(ov_instantiation, pobj) == pclass_ov_association){
						*nodeClass = UA_NODECLASS_REFERENCETYPE;
					} else {
						*nodeClass = UA_NODECLASS_OBJECT;
					}
					break;
				case OV_ET_VARIABLE:
				case OV_ET_MEMBER:
					*nodeClass = UA_NODECLASS_VARIABLE;
					break;
				case OV_ET_OPERATION:
					*nodeClass = UA_NODECLASS_METHOD;
					break;
				case OV_ET_CHILDLINK:
				case OV_ET_PARENTLINK:
				default:
					*nodeClass = UA_NODECLASS_UNSPECIFIED;
					break;
				}
				readNodesResults[indices[i]].status = UA_STATUSCODE_GOOD;
				readNodesResults[indices[i]].value.vt = &UA_[UA_NODECLASS];
				readNodesResults[indices[i]].value.storage.data.arrayLength = 1;
				readNodesResults[indices[i]].value.storage.data.dataPtr = nodeClass;
				readNodesResults[indices[i]].value.storage.data.arrayDimensionsLength = 1;
				readNodesResults[indices[i]].value.storage.data.arrayDimensions = NULL;
				readNodesResults[indices[i]].encodingMask = UA_DATAVALUE_ENCODINGMASK_VARIANT;
			} else {
				readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_NOACCESS);
			}
			break;

		case UA_ATTRIBUTEID_BROWSENAME:
		case UA_ATTRIBUTEID_DISPLAYNAME:
			ov_memstack_lock();
			readNodesResults[indices[i]].status = iec62541_nodeStoreFunctions_resolveNodeIdToPath(readValueIds[indices[i]].nodeId, &path);
			if(readNodesResults[indices[i]].status != UA_STATUSCODE_GOOD){
				ov_memstack_unlock();
				break;
			}
			readNodesResults[indices[i]].status = iec62541_nodeStoreFunctions_getVtblPointerAndCheckAccess(&(path.elements[path.size-1]), pTicket, &pobj, &pVtblObj, &access);
			if(readNodesResults[indices[i]].status != UA_STATUSCODE_GOOD){
				ov_memstack_unlock();
				break;
			}
			if(access & OV_AC_READ){
				if(pobj){
					value.value.vartype = OV_VT_STRING;
					value.value.valueunion.val_string = pobj->v_identifier;
					readNodesResults[indices[i]].status = ov_AnyToVariant(&value, &(readNodesResults[indices[i]].value));
					readNodesResults[indices[i]].encodingMask = UA_DATAVALUE_ENCODINGMASK_VARIANT;
					value = emptyAny;
				} else {
					readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_BADOBJTYPE);
				}
			} else {
				readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_NOACCESS);
			}
			ov_memstack_unlock();
			break;

		case UA_ATTRIBUTEID_DESCRIPTION:
			ov_memstack_lock();
			readNodesResults[indices[i]].status = iec62541_nodeStoreFunctions_resolveNodeIdToPath(readValueIds[indices[i]].nodeId, &path);
			if(readNodesResults[indices[i]].status != UA_STATUSCODE_GOOD){
				ov_memstack_unlock();
				break;
			}
			readNodesResults[indices[i]].status = iec62541_nodeStoreFunctions_getVtblPointerAndCheckAccess(&(path.elements[path.size-1]), pTicket, &pobj, &pVtblObj, &access);
			if(readNodesResults[indices[i]].status != UA_STATUSCODE_GOOD){
				ov_memstack_unlock();
				break;
			}
			if(access & OV_AC_READ){
				if(pobj){
					value.value.vartype = OV_VT_STRING;
					value.value.valueunion.val_string = pVtblObj->m_getcomment(pobj, &(path.elements[path.size-1]));
					readNodesResults[indices[i]].status = ov_AnyToVariant(&value, &(readNodesResults[indices[i]].value));
					readNodesResults[indices[i]].encodingMask = UA_DATAVALUE_ENCODINGMASK_VARIANT;
					value = emptyAny;
				}
			} else {
				readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_NOACCESS);
			}
			ov_memstack_unlock();
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
			ov_memstack_lock();
			readNodesResults[indices[i]].status = iec62541_nodeStoreFunctions_resolveNodeIdToPath(readValueIds[indices[i]].nodeId, &path);
			if(readNodesResults[indices[i]].status != UA_STATUSCODE_GOOD){
				ov_memstack_unlock();
				break;
			}
			if(readValueIds[indices[i]].attributeId == UA_ATTRIBUTEID_WRITEMASK){
				readNodesResults[indices[i]].status = iec62541_nodeStoreFunctions_getVtblPointerAndCheckAccess(&(path.elements[path.size-1]), NULL, &pobj, &pVtblObj, &access);
			} else {
				readNodesResults[indices[i]].status = iec62541_nodeStoreFunctions_getVtblPointerAndCheckAccess(&(path.elements[path.size-1]), pTicket, &pobj, &pVtblObj, &access);
			}
			if(readNodesResults[indices[i]].status != UA_STATUSCODE_GOOD){
				ov_memstack_unlock();
				break;
			}
			writeMask = 0;
			if(path.elements[path.size-1].elemtype != OV_ET_VARIABLE){
				if(access & OV_AC_WRITE){
					*writeMask |= (1<<2);	/*	BrowseName	*/
					*writeMask |= (1<<6);	/*	DisplayName	*/
				}
			}
			readNodesResults[indices[i]].status = UA_STATUSCODE_GOOD;
			readNodesResults[indices[i]].value.vt = &UA_[UA_UINT32];
			readNodesResults[indices[i]].value.storage.data.arrayLength = 1;
			readNodesResults[indices[i]].value.storage.data.dataPtr = writeMask;
			readNodesResults[indices[i]].value.storage.data.arrayDimensionsLength = 1;
			readNodesResults[indices[i]].value.storage.data.arrayDimensions = NULL;
			readNodesResults[indices[i]].encodingMask = UA_DATAVALUE_ENCODINGMASK_VARIANT;
			ov_memstack_unlock();
		}
			break;

		case UA_ATTRIBUTEID_ISABSTRACT:
			readNodesResults[indices[i]].status = UA_STATUSCODE_BADNOTIMPLEMENTED;
			break;

		case UA_ATTRIBUTEID_SYMMETRIC:
			readNodesResults[indices[i]].status = UA_STATUSCODE_BADNOTIMPLEMENTED;
			break;

		case UA_ATTRIBUTEID_INVERSENAME:
			readNodesResults[indices[i]].status = UA_STATUSCODE_BADNOTIMPLEMENTED;
			break;

		case UA_ATTRIBUTEID_CONTAINSNOLOOPS:
			readNodesResults[indices[i]].status = UA_STATUSCODE_BADNOTIMPLEMENTED;
			break;

		case UA_ATTRIBUTEID_EVENTNOTIFIER:
		{
			UA_Byte *eventNotifer;
			UA_Byte_new(&eventNotifer);
			if(!eventNotifer){
				readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
				break;
			}
			ov_memstack_lock();
			readNodesResults[indices[i]].status = iec62541_nodeStoreFunctions_resolveNodeIdToPath(readValueIds[indices[i]].nodeId, &path);
			if(readNodesResults[indices[i]].status != UA_STATUSCODE_GOOD){
				ov_memstack_unlock();
				break;
			}
			eventNotifer = 0;

			readNodesResults[indices[i]].status = UA_STATUSCODE_GOOD;
			readNodesResults[indices[i]].value.vt = &UA_[UA_BYTE];
			readNodesResults[indices[i]].value.storage.data.arrayLength = 1;
			readNodesResults[indices[i]].value.storage.data.dataPtr = eventNotifer;
			readNodesResults[indices[i]].value.storage.data.arrayDimensionsLength = 1;
			readNodesResults[indices[i]].value.storage.data.arrayDimensions = NULL;
			readNodesResults[indices[i]].encodingMask = UA_DATAVALUE_ENCODINGMASK_VARIANT;
			ov_memstack_unlock();
		}
		break;

		case UA_ATTRIBUTEID_VALUE:
			ov_memstack_lock();
			readNodesResults[indices[i]].status = iec62541_nodeStoreFunctions_resolveNodeIdToPath(readValueIds[indices[i]].nodeId, &path);
			if(readNodesResults[indices[i]].status != UA_STATUSCODE_GOOD){
				ov_memstack_unlock();
				break;
			}
			pobj = path.elements[path.size-1].pobj;
			Ov_GetVTablePtr(ov_object, pVtblObj, pobj);
			if((!pVtblObj) || (ov_activitylock)){
				pVtblObj = pclass_ov_object->v_pvtable;
			}
			access = (pVtblObj)->m_getaccess(path.elements[path.size-1].pobj, &(path.elements[path.size-1]), pTicket);
			if(access & OV_AC_READ){
				switch(path.elements[path.size-1].elemtype) {
				case OV_ET_MEMBER:
				case OV_ET_VARIABLE:
					readNodesResults[indices[i]].status = ov_resultToUaStatusCode((pVtblObj->m_getvar)(pobj, &(path.elements[path.size-1]), &value));
					value.value.vartype &= OV_VT_KSMASK;
					if(readNodesResults[indices[i]].status == UA_STATUSCODE_GOOD){
						readNodesResults[indices[i]].status = ov_AnyToVariant(&value, &(readNodesResults[indices[i]].value));
						readNodesResults[indices[i]].encodingMask = UA_DATAVALUE_ENCODINGMASK_VARIANT;
						value = emptyAny;
					}
					ov_memstack_unlock();
					break;
					/*	todo: implement getValue for links as well?	*/
				default:
					readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_BADPATH);
					ov_memstack_unlock();
					break;
				}
			}
			break;

			case UA_ATTRIBUTEID_DATATYPE:
			{
				UA_NodeId *dataType;
				ov_memstack_lock();
				readNodesResults[indices[i]].status = iec62541_nodeStoreFunctions_resolveNodeIdToPath(readValueIds[indices[i]].nodeId, &path);
				if(readNodesResults[indices[i]].status != UA_STATUSCODE_GOOD){
					ov_memstack_unlock();
					break;
				}
				pobj = path.elements[path.size-1].pobj;
				Ov_GetVTablePtr(ov_object, pVtblObj, pobj);
				if((!pVtblObj) || (ov_activitylock)){
					pVtblObj = pclass_ov_object->v_pvtable;
				}
				access = (pVtblObj)->m_getaccess(path.elements[path.size-1].pobj, &(path.elements[path.size-1]), pTicket);
				if(access & OV_AC_READ){
					switch(path.elements[path.size-1].elemtype) {
					case OV_ET_MEMBER:
					case OV_ET_VARIABLE:
						if(Ov_CanCastTo(ov_variable, pobj)){
							dataType = ov_varTypeToNodeId(readNodesResults[indices[i]].status = ((OV_INSTPTR_ov_variable)pobj)->v_vartype);
							if(!dataType){
								readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
								ov_memstack_unlock();
								break;
							}
						}
						ov_memstack_unlock();
						break;
					default:
						readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_BADPATH);
						ov_memstack_unlock();
						break;
					}
				}
				readNodesResults[indices[i]].status = UA_STATUSCODE_GOOD;
				readNodesResults[indices[i]].value.vt = &UA_[UA_NODEID];
				readNodesResults[indices[i]].value.storage.data.arrayLength = 1;
				readNodesResults[indices[i]].value.storage.data.dataPtr = dataType;
				readNodesResults[indices[i]].value.storage.data.arrayDimensionsLength = 1;
				readNodesResults[indices[i]].value.storage.data.arrayDimensions = NULL;
				readNodesResults[indices[i]].encodingMask = UA_DATAVALUE_ENCODINGMASK_VARIANT;
				ov_memstack_unlock();
			}
				break;

			case UA_ATTRIBUTEID_VALUERANK:
				break;

			case UA_ATTRIBUTEID_ARRAYDIMENSIONS:
				break;

			case UA_ATTRIBUTEID_ACCESSLEVEL:
				break;

			case UA_ATTRIBUTEID_USERACCESSLEVEL:
				break;

			case UA_ATTRIBUTEID_MINIMUMSAMPLINGINTERVAL:
				readNodesResults[indices[i]].status = UA_STATUSCODE_BADNOTIMPLEMENTED;
				break;

			case UA_ATTRIBUTEID_HISTORIZING:
			{
				UA_Boolean *historizing;
				UA_Boolean_new(&historizing);
				if(!historizing){
					readNodesResults[indices[i]].status = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
					break;
				}
				ov_memstack_lock();
				readNodesResults[indices[i]].status = iec62541_nodeStoreFunctions_resolveNodeIdToPath(readValueIds[indices[i]].nodeId, &path);
				if(readNodesResults[indices[i]].status != UA_STATUSCODE_GOOD){
					ov_memstack_unlock();
					break;
				}
				historizing = UA_FALSE;

				readNodesResults[indices[i]].status = UA_STATUSCODE_GOOD;
				readNodesResults[indices[i]].value.vt = &UA_[UA_BOOLEAN];
				readNodesResults[indices[i]].value.storage.data.arrayLength = 1;
				readNodesResults[indices[i]].value.storage.data.dataPtr = historizing;
				readNodesResults[indices[i]].value.storage.data.arrayDimensionsLength = 1;
				readNodesResults[indices[i]].value.storage.data.arrayDimensions = NULL;
				readNodesResults[indices[i]].encodingMask = UA_DATAVALUE_ENCODINGMASK_VARIANT;
				ov_memstack_unlock();
			}
			break;

			case UA_ATTRIBUTEID_EXECUTABLE:
				readNodesResults[indices[i]].status = UA_STATUSCODE_BADNOTIMPLEMENTED;
				break;

			case UA_ATTRIBUTEID_USEREXECUTABLE:
				readNodesResults[indices[i]].status = UA_STATUSCODE_BADNOTIMPLEMENTED;
				break;

			default:
				break;
		}

		//if(retval != UA_STATUSCODE_GOOD) {
		//	readNodesResults[indices].encodingMask = UA_DATAVALUE_ENCODINGMASK_STATUSCODE;
		//	readNodesResults[indices].status       = UA_STATUSCODE_BADNOTREADABLE;
		// }
	}
	return UA_STATUSCODE_GOOD;
}

OV_DLLFNCEXPORT UA_Int32 iec62541_nodeStoreFunctions_writeNodes(
		UA_WriteValue *writeValues,
		UA_UInt32 *indices,
		UA_UInt32 indicesSize,
		UA_StatusCode *writeNodesResults,
		UA_DiagnosticInfo *diagnosticInfo
) {
	return UA_STATUSCODE_BADNOTIMPLEMENTED;

}

OV_DLLFNCEXPORT UA_Int32 iec62541_nodeStoreFunctions_browseNodes(
		UA_UInt32 requestedMaxReferencesPerNode,
		UA_BrowseDescription *browseDescriptions,
		UA_Int32 *indices,
		UA_UInt32 indicesSize,
		UA_BrowseResult *browseResults,
		UA_DiagnosticInfo *diagnosticInfos
) {
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

