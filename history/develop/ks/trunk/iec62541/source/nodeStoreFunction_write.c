
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
#include "ks_logfile.h"

OV_DLLFNCEXPORT UA_Int32 iec62541_nodeStoreFunctions_writeNodes(
		void *ensHandle,
		const UA_RequestHeader *requestHeader,
		UA_WriteValue *writeValues,
		UA_UInt32 *indices,
		UA_UInt32 indicesSize,
		UA_StatusCode *writeNodesResults,
		UA_DiagnosticInfo *diagnosticInfo
) {
	UA_UInt32				index		=	0;
	OV_PATH					path;
	OV_INSTPTR_ov_object	pobj		=	NULL;
	OV_VTBLPTR_ov_object	pVtblObj	=	NULL;
	OV_ACCESS				access		=	OV_AC_NONE;
	OV_TICKET				*pTicket	=	NULL;
	OV_ANY					value		=	{.value.vartype = OV_VT_VOID, .value.valueunion = {0}};
	OV_TIME_SPAN			tempTimeSpan	=	{.secs = 0, .usecs = 0};
	OV_TIME_SPAN_VEC		tempTimeSpanVec	=	{.veclen = 0, .value = NULL};
	pTicket = ksbase_NoneAuth->v_ticket.vtbl->createticket(NULL, OV_TT_NONE);

	for(index = 0; index < indicesSize; index++){
		switch(writeValues[indices[index]].attributeId){
		case UA_ATTRIBUTEID_ACCESSLEVEL:
		case UA_ATTRIBUTEID_ARRAYDIMENSIONS:
		case UA_ATTRIBUTEID_CONTAINSNOLOOPS:
		case UA_ATTRIBUTEID_DESCRIPTION:
		case UA_ATTRIBUTEID_EVENTNOTIFIER:
		case UA_ATTRIBUTEID_EXECUTABLE:
		case UA_ATTRIBUTEID_HISTORIZING:
		case UA_ATTRIBUTEID_INVERSENAME:
		case UA_ATTRIBUTEID_ISABSTRACT:
		case UA_ATTRIBUTEID_MINIMUMSAMPLINGINTERVAL:
		case UA_ATTRIBUTEID_NODECLASS:
		case UA_ATTRIBUTEID_SYMMETRIC:
		case UA_ATTRIBUTEID_USERACCESSLEVEL:
		case UA_ATTRIBUTEID_USEREXECUTABLE:
		case UA_ATTRIBUTEID_USERWRITEMASK:
		case UA_ATTRIBUTEID_VALUERANK:
		case UA_ATTRIBUTEID_WRITEMASK:
			writeNodesResults[indices[index]] = UA_STATUSCODE_BADNOTWRITABLE;
			break;

		case UA_ATTRIBUTEID_VALUE:
			ov_memstack_lock();
			writeNodesResults[indices[index]] = iec62541_nodeStoreFunctions_resolveNodeIdToPath(writeValues[indices[index]].nodeId, &path);
			if(writeNodesResults[indices[index]] != UA_STATUSCODE_GOOD){
				ov_memstack_unlock();
				break;
			}
			pobj = path.elements[path.size-1].pobj;
			Ov_GetVTablePtr(ov_object, pVtblObj, pobj);
			if((!pVtblObj) || (ov_activitylock)){
				pVtblObj = pclass_ov_object->v_pvtable;
			}
			access = (pVtblObj)->m_getaccess(path.elements[path.size-1].pobj, &(path.elements[path.size-1]), pTicket);
			if(access & OV_AC_WRITE){
				switch(path.elements[path.size-1].elemtype) {
				case OV_ET_MEMBER:
				case OV_ET_VARIABLE:
					if(writeValues[indices[index]].value.hasValue == UA_TRUE){
						ov_memstack_lock();
						writeNodesResults[indices[index]] = ov_VariantToAny(&(writeValues[indices[index]].value.value), &value);
						if(writeNodesResults[indices[index]] != UA_STATUSCODE_GOOD){
							break;
						}
						if(writeValues[indices[index]].value.hasSourceTimestamp == UA_TRUE){
							value.value.vartype |= OV_VT_HAS_TIMESTAMP;
							value.time = ov_1601nsTimeToOvTime(writeValues[indices[index]].value.sourceTimestamp);
						}
						writeNodesResults[indices[index]] = ov_resultToUaStatusCode((pVtblObj->m_setvar)(pobj, &(path.elements[path.size-1]), &value));
						if(writeNodesResults[indices[index]] == UA_STATUSCODE_BADTYPEMISMATCH
								&& ((value.value.vartype == OV_VT_DOUBLE
											&& (path.elements[path.size-1].pvar->v_vartype & OV_VT_KSMASK) == OV_VT_TIME_SPAN)
									|| (value.value.vartype == OV_VT_DOUBLE_VEC
											&& (path.elements[path.size-1].pvar->v_vartype & OV_VT_KSMASK) == OV_VT_TIME_SPAN_VEC))){
							/*	timeSpans are doubles in UA	*/
							if(!(value.value.vartype & OV_VT_ISVECTOR)){
								Ov_DoubleToTimeSpan(value.value.valueunion.val_double * 1000.0, tempTimeSpan);
								value.value.vartype = OV_VT_TIME_SPAN;
								value.value.valueunion.val_time_span = tempTimeSpan;
							} else {
								OV_UINT iTwo = 0;
								tempTimeSpanVec.value = ov_memstack_alloc(value.value.valueunion.val_double_vec.veclen * sizeof(OV_TIME_SPAN));
								if(!tempTimeSpanVec.value){
									writeNodesResults[indices[index]] = UA_STATUSCODE_BADOUTOFMEMORY;
									break;
								}
								tempTimeSpanVec.veclen = value.value.valueunion.val_double_vec.veclen;
								for(iTwo = 0; iTwo < tempTimeSpanVec.veclen; iTwo++){
									Ov_DoubleToTimeSpan(value.value.valueunion.val_double_vec.value[iTwo] * 1000.0, tempTimeSpanVec.value[iTwo]);
								}
								value.value.vartype = OV_VT_TIME_SPAN_VEC;
								value.value.valueunion.val_time_span_vec = tempTimeSpanVec;
							}
							if(writeValues[indices[index]].value.hasSourceTimestamp == UA_TRUE){
								value.value.vartype |= OV_VT_HAS_TIMESTAMP;
								value.time = ov_1601nsTimeToOvTime(writeValues[indices[index]].value.sourceTimestamp);
							}
							writeNodesResults[indices[index]] = ov_resultToUaStatusCode((pVtblObj->m_setvar)(pobj, &(path.elements[path.size-1]), &value));
						}
						ov_memstack_unlock();
					} else {
						writeNodesResults[indices[index]] = UA_STATUSCODE_BADINVALIDARGUMENT;
						break;
					}
					break;
				default:
					writeNodesResults[indices[index]] = ov_resultToUaStatusCode(OV_ERR_BADPATH);
					break;
				}
				ov_memstack_unlock();
			} else {
				writeNodesResults[indices[index]] = UA_STATUSCODE_BADNOTWRITABLE;
				ov_memstack_unlock();
			}
			break;
		case UA_ATTRIBUTEID_NODEID:
			/*	for objects do a rename	*/
			ov_memstack_lock();
			writeNodesResults[indices[index]] = iec62541_nodeStoreFunctions_resolveNodeIdToPath(writeValues[indices[index]].nodeId, &path);
			if(writeNodesResults[indices[index]] != UA_STATUSCODE_GOOD){
				ov_memstack_unlock();
				break;
			}
			if(path.elements[path.size-1].elemtype == OV_ET_OBJECT){
				pobj = path.elements[path.size-1].pobj;
				Ov_GetVTablePtr(ov_object, pVtblObj, pobj);
				if((!pVtblObj) || (ov_activitylock)){
					pVtblObj = pclass_ov_object->v_pvtable;
				}
				access = (pVtblObj)->m_getaccess(path.elements[path.size-1].pobj, &(path.elements[path.size-1]), pTicket);
				if(access & OV_AC_RENAMEABLE){
					if(writeValues[indices[index]].value.hasValue == UA_TRUE) {
						if(writeValues[indices[index]].value.value.type == &UA_TYPES[UA_TYPES_NODEID]
						        && ((UA_NodeId*)writeValues[indices[index]].value.value.data)->identifierType == UA_NODEIDTYPE_STRING){
							OV_PATH		newPath;
							OV_STRING	tempString	=	NULL;
							OV_STRING	newIdent	=	NULL;
							OV_UINT		iterator;
							tempString = ov_memstack_alloc(((UA_NodeId*)writeValues[indices[index]].value.value.data)->identifier.string.length + 1);
							if(!tempString){
								ov_memstack_unlock();
								writeNodesResults[indices[index]] = UA_STATUSCODE_BADOUTOFMEMORY;
								break;
							}
							//referencesSize is guaranteed to be positive
							for(iterator = 0; iterator < (OV_UINT)((UA_NodeId*)writeValues[indices[index]].value.value.data)->identifier.string.length; iterator++){
								tempString[iterator] = ((UA_NodeId*)writeValues[indices[index]].value.value.data)->identifier.string.data[iterator];
							}
							tempString[iterator] = '\0';
							/*	cut off the last part behind a / as it is the new identifier; the front part is the new path	*/
							for(iterator = ((UA_NodeId*)writeValues[indices[index]].value.value.data)->identifier.string.length; iterator > 0; iterator++){
								if(tempString[iterator] == '/'){
									tempString[iterator] = '\0';
									newIdent = &(tempString[iterator+1]);
								}
							}
							writeNodesResults[indices[index]] = ov_resultToUaStatusCode(ov_path_resolve(&newPath, NULL, tempString, 2));
							if(writeNodesResults[indices[index]] != UA_STATUSCODE_GOOD){
								ov_memstack_unlock();
								break;
							}
							if(Ov_CanCastTo(ov_domain, newPath.elements[newPath.size-1].pobj)){
							writeNodesResults[indices[index]] = ov_resultToUaStatusCode(
									ov_class_renameobject(path.elements[path.size-1].pobj, Ov_StaticPtrCast(ov_domain, newPath.elements[newPath.size-1].pobj), newIdent, OV_PMH_DEFAULT, NULL));
							} else {
								writeNodesResults[indices[index]] = UA_STATUSCODE_BADINVALIDARGUMENT;
							}
						} else {
							writeNodesResults[indices[index]] = UA_STATUSCODE_BADINVALIDARGUMENT;
						}
					} else {
						writeNodesResults[indices[index]] = UA_STATUSCODE_BADINVALIDARGUMENT;
					}
				} else {
					writeNodesResults[indices[index]] = UA_STATUSCODE_BADNOTWRITABLE;
				}
			} else {
				writeNodesResults[indices[index]] = UA_STATUSCODE_BADNOTWRITABLE;
			}
			ov_memstack_unlock();
			break;
		case UA_ATTRIBUTEID_BROWSENAME:
		case UA_ATTRIBUTEID_DISPLAYNAME:
			/*	return an error for now
			 * decide whether this can be changed --> it would result in a rename and hence a change of the (string) nodeID	*/
			writeNodesResults[indices[index]] = UA_STATUSCODE_BADNOTWRITABLE;
			break;
		case UA_ATTRIBUTEID_DATATYPE:
			/*	this falls back to a change of type for any-variables	*/
			writeNodesResults[indices[index]] = UA_STATUSCODE_BADNOTWRITABLE;
			break;

		default:
			KS_logfile_info(("iec62541: got a write-request with bad attributeid (%#08x)", writeValues[indices[index]].attributeId));
			writeNodesResults[indices[index]] = UA_STATUSCODE_BADATTRIBUTEIDINVALID;
			break;
		}
	}

	return UA_STATUSCODE_GOOD;

}
