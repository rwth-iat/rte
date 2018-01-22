/* * nodeStoreInterface.c
 *
 *  Created on: 06.12.2016
 *      Author: ubuntu
 */

#ifndef OV_COMPILE_LIBRARY_servicesOPCUAInterface
#define OV_COMPILE_LIBRARY_servicesOPCUAInterface
#endif

#include "servicesOPCUAInterface.h"
#include "libov/ov_macros.h"
#include "ksbase.h"
#include "opcua.h"
#include "opcua_helpers.h"
#include "NoneTicketAuthenticator.h"
#include "libov/ov_path.h"
#include "libov/ov_memstack.h"
#include "ks_logfile.h"

OV_DLLFNCEXPORT UA_StatusCode servicesOPCUAInterface_interface_MethodCallback(void *methodHandle, const UA_NodeId *objectId,
                     const UA_NodeId *sessionId, void *sessionHandle,
                     size_t inputSize, const UA_Variant *input,
                     size_t outputSize, UA_Variant *output) {

	UA_String *tmpStringArray = NULL;
	UA_StatusCode result = UA_STATUSCODE_GOOD;

	OV_VTBLPTR_services_Service pvtable;
	OV_INSTPTR_services_Service pService = (OV_INSTPTR_services_Service)methodHandle;
	Ov_GetVTablePtr(services_Service, pvtable, pService);

	void **inputs = ov_database_malloc(sizeof(void*)*inputSize);
	for (OV_UINT i = 0; i < inputSize; i++){
		inputs[i] = NULL;
	}
	OV_UINT inputCounts = 0;
	for (OV_UINT i = 0; i < inputSize; i++){
		inputCounts++;
		inputs[i] = NULL;
		if (input[i].arrayLength == 0){
			switch (input[i].type->typeIndex){
				case UA_TYPES_BOOLEAN:
				case UA_TYPES_INT32:
				case UA_TYPES_UINT32:
				case UA_TYPES_FLOAT:
				case UA_TYPES_DOUBLE:
				case UA_TYPES_VARIANT:
					if (input[i].data == NULL || input[i].data == 0x1){
						result = UA_STATUSCODE_BADARGUMENTSMISSING;
						goto cleanup;
					}
					inputs[i] = input[i].data;
					break;
				case UA_TYPES_STRING:
					if (input[i].data == NULL || input[i].data == 0x1){
						result = UA_STATUSCODE_BADARGUMENTSMISSING;
						goto cleanup;
					}
					if ((*((UA_String*)(input[i].data))).length == 0){
						result = UA_STATUSCODE_BADARGUMENTSMISSING;
						goto cleanup;
					}
					inputs[i] = ov_database_malloc(sizeof(OV_STRING));
					copyOPCUAStringToOV(*((UA_String*)(input[i].data)), (OV_STRING*)(inputs[i]));
					break;
				default:
					break;
			}
		}else{
			switch (input[i].type->typeIndex){
				case UA_TYPES_BOOLEAN:
					if (input[i].data == NULL){
						result = UA_STATUSCODE_BADARGUMENTSMISSING;
						goto cleanup;
					}
					inputs[i] = ov_database_malloc(sizeof(OV_BOOL_VEC));
					(*(OV_BOOL_VEC*)(inputs[i])).veclen = input[i].arrayLength;
					(*(OV_BOOL_VEC*)(inputs[i])).value = input[i].data;
					break;
				case UA_TYPES_INT32:
					if (input[i].data == NULL){
						result = UA_STATUSCODE_BADARGUMENTSMISSING;
						goto cleanup;
					}
					inputs[i] = ov_database_malloc(sizeof(OV_INT_VEC));
					(*(OV_INT_VEC*)(inputs[i])).veclen = input[i].arrayLength;
					(*(OV_INT_VEC*)(inputs[i])).value = input[i].data;
					break;
				case UA_TYPES_UINT32:
					if (input[i].data == NULL){
						result = UA_STATUSCODE_BADARGUMENTSMISSING;
						goto cleanup;
					}
					inputs[i] = ov_database_malloc(sizeof(OV_UINT_VEC));
					(*(OV_UINT_VEC*)(inputs[i])).veclen = input[i].arrayLength;
					(*(OV_UINT_VEC*)(inputs[i])).value = input[i].data;
					break;
				case UA_TYPES_FLOAT:
					if (input[i].data == NULL){
						result = UA_STATUSCODE_BADARGUMENTSMISSING;
						goto cleanup;
					}
					inputs[i] = ov_database_malloc(sizeof(OV_SINGLE_VEC));
					(*(OV_SINGLE_VEC*)(inputs[i])).veclen = input[i].arrayLength;
					(*(OV_SINGLE_VEC*)(inputs[i])).value = input[i].data;
					break;
				case UA_TYPES_DOUBLE:
					if (input[i].data == NULL){
						result = UA_STATUSCODE_BADARGUMENTSMISSING;
						goto cleanup;
					}
					inputs[i] = ov_database_malloc(sizeof(OV_DOUBLE_VEC));
					(*(OV_DOUBLE_VEC*)(inputs[i])).veclen = input[i].arrayLength;
					(*(OV_DOUBLE_VEC*)(inputs[i])).value = input[i].data;
					break;
				case UA_TYPES_STRING:
					if (input[i].data == NULL){
						result = UA_STATUSCODE_BADARGUMENTSMISSING;
						goto cleanup;
					}
					inputs[i] = ov_database_malloc(sizeof(OV_STRING_VEC));
					(*(OV_STRING_VEC*)(inputs[i])).veclen = input[i].arrayLength;
					(*(OV_STRING_VEC*)(inputs[i])).value = malloc(input[i].arrayLength*sizeof(OV_STRING));
					for (OV_UINT j = 0; j < input[i].arrayLength; j++){
						(*(OV_STRING_VEC*)(inputs[i])).value[j] = NULL;
						if (((UA_String*)(input[i].data))[j].length == 0){
							result = UA_STATUSCODE_BADARGUMENTSMISSING;
							goto cleanup;
						}
						copyOPCUAStringToOV(((UA_String*)(input[i].data))[j], &((*(OV_STRING_VEC*)(inputs[i])).value[j]));
					}
					break;
				default:
					break;
			}
		}
	}

	void **outputs = ov_database_malloc(sizeof(void*)*outputSize);
	for (int i = 0; i < outputSize; i++){
		outputs[i] = NULL;
	}
	OV_UINT *typeArray= ov_database_malloc(sizeof(OV_UINT)*outputSize);
	pvtable->m_CallMethod(pService, inputSize, (const void**)inputs, outputSize, outputs, typeArray);

	for (int i = 0; i < outputSize; i++){
		switch (typeArray[i]){
		case OV_VT_BOOL:
			UA_Variant_setScalarCopy(&output[i], outputs[i], &UA_TYPES[UA_TYPES_BOOLEAN]);
			if (outputs[i])
				ov_database_free(outputs[i]);
			break;
		case OV_VT_INT:
			UA_Variant_setScalarCopy(&output[i], outputs[i], &UA_TYPES[UA_TYPES_INT32]);
			if (outputs[i])
				ov_database_free(outputs[i]);
			break;
		case OV_VT_UINT:
			UA_Variant_setScalarCopy(&output[i], outputs[i], &UA_TYPES[UA_TYPES_UINT32]);
			if (outputs[i])
				ov_database_free(outputs[i]);
			break;
		case OV_VT_SINGLE:
			UA_Variant_setScalarCopy(&output[i], outputs[i], &UA_TYPES[UA_TYPES_FLOAT]);
			if (outputs[i])
				ov_database_free(outputs[i]);
			break;
		case OV_VT_DOUBLE:
			UA_Variant_setScalarCopy(&output[i], outputs[i], &UA_TYPES[UA_TYPES_DOUBLE]);
			if (outputs[i])
				ov_database_free(outputs[i]);
			break;
		case OV_VT_ANY:
			UA_Variant_setScalarCopy(&output[i], outputs[i], &UA_TYPES[UA_TYPES_VARIANT]);
			if (outputs[i])
				ov_database_free(outputs[i]);
			break;
		case OV_VT_STRING:
			{
			UA_String tmpString;
			UA_String_init(&tmpString);
			if (*(OV_STRING*)outputs[i])
				tmpString = UA_String_fromChars(*(OV_STRING*)outputs[i]);
			UA_Variant_setScalarCopy(&output[i], &tmpString, &UA_TYPES[UA_TYPES_STRING]);
			UA_String_deleteMembers(&tmpString);
			if (*(OV_STRING*)outputs[i])
				ov_string_setvalue((OV_STRING*)outputs[i], NULL);
			if (outputs[i])
				ov_database_free(outputs[i]);
			}
			break;
		case OV_VT_BOOL_VEC:
			UA_Variant_setArrayCopy(&output[i], (*(OV_GENERIC_VEC*)(outputs[i])).value, (*(OV_GENERIC_VEC*)(outputs[i])).veclen, &UA_TYPES[UA_TYPES_BOOLEAN]);
			if ((*(OV_GENERIC_VEC*)(outputs[i])).value)
				ov_database_free((*(OV_GENERIC_VEC*)(outputs[i])).value);
			if (outputs[i])
				ov_database_free(outputs[i]);
			break;
		case OV_VT_INT_VEC:
			UA_Variant_setArrayCopy(&output[i], (*(OV_GENERIC_VEC*)(outputs[i])).value, (*(OV_GENERIC_VEC*)(outputs[i])).veclen, &UA_TYPES[UA_TYPES_INT32]);
			if ((*(OV_GENERIC_VEC*)(outputs[i])).value)
				ov_database_free((*(OV_GENERIC_VEC*)(outputs[i])).value);
			if (outputs[i])
				ov_database_free(outputs[i]);
			break;
		case OV_VT_UINT_VEC:
			UA_Variant_setArrayCopy(&output[i], (*(OV_GENERIC_VEC*)(outputs[i])).value, (*(OV_GENERIC_VEC*)(outputs[i])).veclen, &UA_TYPES[UA_TYPES_UINT32]);
			if ((*(OV_GENERIC_VEC*)(outputs[i])).value)
				ov_database_free((*(OV_GENERIC_VEC*)(outputs[i])).value);
			if (outputs[i])
				ov_database_free(outputs[i]);
			break;
		case OV_VT_SINGLE_VEC:
			UA_Variant_setArrayCopy(&output[i], (*(OV_GENERIC_VEC*)(outputs[i])).value, (*(OV_GENERIC_VEC*)(outputs[i])).veclen, &UA_TYPES[UA_TYPES_FLOAT]);
			if ((*(OV_GENERIC_VEC*)(outputs[i])).value)
				ov_database_free((*(OV_GENERIC_VEC*)(outputs[i])).value);
			if (outputs[i])
				ov_database_free(outputs[i]);
			break;
		case OV_VT_DOUBLE_VEC:
			UA_Variant_setArrayCopy(&output[i], (*(OV_GENERIC_VEC*)(outputs[i])).value, (*(OV_GENERIC_VEC*)(outputs[i])).veclen, &UA_TYPES[UA_TYPES_DOUBLE]);
			if ((*(OV_GENERIC_VEC*)(outputs[i])).value)
				ov_database_free((*(OV_GENERIC_VEC*)(outputs[i])).value);
			if (outputs[i])
				ov_database_free(outputs[i]);
			break;
		case OV_VT_STRING_VEC:
			tmpStringArray = UA_Array_new((*(OV_GENERIC_VEC*)(outputs[i])).veclen, &UA_TYPES[UA_TYPES_STRING]);
			for (OV_UINT j = 0; j < (*(OV_GENERIC_VEC*)(outputs[i])).veclen; j++){
				tmpStringArray[j] = UA_String_fromChars((*(OV_STRING_VEC*)outputs[i]).value[j]);
			}
			UA_Variant_setArrayCopy(&output[i], tmpStringArray, (*(OV_GENERIC_VEC*)(outputs[i])).veclen, &UA_TYPES[UA_TYPES_STRING]);
			UA_Array_delete(tmpStringArray, (*(OV_GENERIC_VEC*)(outputs[i])).veclen,&UA_TYPES[UA_TYPES_STRING]);

			if ((*(OV_STRING_VEC*)(outputs[i])).value)
				for (int k = 0; k < (*(OV_STRING_VEC*)(outputs[i])).veclen; k++){
					ov_database_free(((*(OV_STRING_VEC*)(outputs[i])).value)[k]);
				}
			if ((*(OV_GENERIC_VEC*)(outputs[i])).value)
				ov_database_free((*(OV_GENERIC_VEC*)(outputs[i])).value);
			if (outputs[i])
				ov_database_free(outputs[i]);
			break;
		default:
			break;
		}
	}

	ov_database_free(outputs);
	ov_database_free(typeArray);

	cleanup:
    	for (int i = 0; i < inputCounts; i++){
			if (input[i].arrayLength == 0){
				switch (input[i].type->typeIndex){
					case UA_TYPES_BOOLEAN:
					case UA_TYPES_INT32:
					case UA_TYPES_UINT32:
					case UA_TYPES_FLOAT:
					case UA_TYPES_DOUBLE:
						inputs[i] = NULL;
						break;
					case UA_TYPES_VARIANT:
						Ov_SetAnyValue((OV_ANY*)inputs[i], NULL);
						inputs[i] = NULL;
						break;
					case UA_TYPES_STRING:
						ov_string_setvalue((OV_STRING*)(inputs[i]), NULL);
						ov_database_free(inputs[i]);
						break;
					default:
						break;
				}
			}else{
				switch (input[i].type->typeIndex){
					case UA_TYPES_BOOLEAN:
						if (inputs[i] != NULL){
							(*(OV_BOOL_VEC*)(inputs[i])).veclen = 0;
							ov_database_free(inputs[i]);
							(*(OV_BOOL_VEC*)(inputs[i])).value = NULL;
						}
						break;
					case UA_TYPES_INT32:
						if (inputs[i] != NULL){
							(*(OV_INT_VEC*)(inputs[i])).veclen = 0;
							ov_database_free(inputs[i]);
							(*(OV_INT_VEC*)(inputs[i])).value = NULL;
						}
						break;
					case UA_TYPES_UINT32:
						if (inputs[i] != NULL){
							(*(OV_UINT_VEC*)(inputs[i])).veclen = 0;
							ov_database_free(inputs[i]);
							(*(OV_UINT_VEC*)(inputs[i])).value = NULL;
						}
						break;
					case UA_TYPES_FLOAT:
						if (inputs[i] != NULL){
							(*(OV_SINGLE_VEC*)(inputs[i])).veclen = 0;
							ov_database_free(inputs[i]);
							(*(OV_SINGLE_VEC*)(inputs[i])).value = NULL;
						}
						break;
					case UA_TYPES_DOUBLE:
						if (inputs[i] != NULL){
							(*(OV_DOUBLE_VEC*)(inputs[i])).veclen = 0;
							ov_database_free(inputs[i]);
							(*(OV_DOUBLE_VEC*)(inputs[i])).value = NULL;
						}
						break;
					case UA_TYPES_STRING:
						if (inputs[i] != NULL){
							for (OV_UINT j = 0; j < (*(OV_STRING_VEC*)(inputs[i])).veclen; j++){
								if ((*(OV_STRING_VEC*)(inputs[i])).value[j] != NULL)
									ov_string_setvalue(&(*(OV_STRING_VEC*)(inputs[i])).value[j], NULL);
							}
							(*(OV_STRING_VEC*)(inputs[i])).value = NULL;
							ov_database_free(inputs[i]);
							(*(OV_STRING_VEC*)(inputs[i])).veclen = 0;
						}
						break;
					default:
						break;
				}
			}
    	}
    	ov_database_free(inputs);

	return result;
}

static void OV_NodeStore_deleteNodestore(void *handle, UA_UInt16 namespaceIndex){

}

static void OV_NodeStore_deleteNode(UA_Node *node){
	if (node){
		UA_Node_deleteMembersAnyNodeClass(node);
	}
	UA_free(node);
}
static void OV_NodeStore_releaseNode(void *handle, const UA_Node *node){
	OV_NodeStore_deleteNode((UA_Node*)node);
}

static UA_Node * OV_NodeStore_newNode(UA_NodeClass nodeClass){ //TODO add nodestore handle? --> move nodeStore from static context to main
    //allocate memory for a new node
	//UA_Node *newNode = NULL;
	//newNode = (UA_Node*) ov_database_malloc(sizeof(UA_ObjectNode));
	//newNode->nodeClass = nodeClass;
    return NULL;
}
static const UA_Node * OV_NodeStore_getNode(void *handle, const UA_NodeId *nodeId){
	UA_Node * tmpNode = NULL;
	UA_Node* opcuaNode = NULL;
	OV_STRING *plist = NULL;
	OV_STRING *plist2 = NULL;
	OV_STRING *plist3 = NULL;
	OV_STRING tmpString = NULL;
	OV_INSTPTR_ov_object pobj = NULL;
	OV_UINT len = 0;
	OV_UINT len2 = 0;
	OV_UINT len3 = 0;
	if (nodeId->identifier.string.data == NULL || nodeId->identifier.string.length == 0 || nodeId->identifierType != UA_NODEIDTYPE_STRING)
		return NULL;
	copyOPCUAStringToOV(nodeId->identifier.string, &tmpString);
	plist = ov_string_split(tmpString, "||", &len);
	plist2 = ov_string_split(tmpString, "/", &len2);
	plist3 = ov_string_split(plist2[len2-1], ".", &len3);

	if (len3 > 1){
		if (ov_string_compare(plist3[len3-1], "ServiceRevision") == OV_STRCMP_EQUAL ||
			ov_string_compare(plist3[len3-1], "ServiceVersion") == OV_STRCMP_EQUAL ||
			ov_string_compare(plist3[len3-1], "WSDL") == OV_STRCMP_EQUAL){
			if (servicesOPCUAInterface_interface_ovServiceVariablesNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD){
				ov_string_freelist(plist);
				ov_string_freelist(plist2);
				ov_string_freelist(plist3);
				ov_string_setvalue(&tmpString, NULL);
				return (UA_Node*) opcuaNode;
			}
		}
	}

	if (len > 1){
		pobj = ov_path_getobjectpointer(plist[0], 2);
		if (pobj == NULL){
			ov_string_freelist(plist);
			ov_string_freelist(plist2);
			ov_string_freelist(plist3);
			ov_string_setvalue(&tmpString, NULL);
			return NULL;
		}
		if (Ov_CanCastTo(services_Service, pobj)){
			if (ov_string_compare(plist[1], "InputArguments") == OV_STRCMP_EQUAL){
				if (servicesOPCUAInterface_interface_ovServiceInputArgumentsNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD){
					ov_string_freelist(plist);
					ov_string_freelist(plist2);
					ov_string_freelist(plist3);
					ov_string_setvalue(&tmpString, NULL);
					return (UA_Node*) opcuaNode;
				}
			}else if (ov_string_compare(plist[1], "OutputArguments") == OV_STRCMP_EQUAL){
				if (servicesOPCUAInterface_interface_ovServiceOutputArgumentsNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD){
					ov_string_freelist(plist);
					ov_string_freelist(plist2);
					ov_string_freelist(plist3);
					ov_string_setvalue(&tmpString, NULL);
					return (UA_Node*) opcuaNode;
				}
			}
		}
	}else{
		pobj = ov_path_getobjectpointer(tmpString, 2);
		if (pobj == NULL){
			ov_string_freelist(plist);
			ov_string_freelist(plist2);
			ov_string_freelist(plist3);
			ov_string_setvalue(&tmpString, NULL);
			return NULL;
		}
	}

	if(Ov_CanCastTo(services_Service, pobj)){
		if (servicesOPCUAInterface_interface_ovServiceNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			tmpNode = opcuaNode;
	}
	ov_string_freelist(plist);
	ov_string_freelist(plist2);
	ov_string_freelist(plist3);
	ov_string_setvalue(&tmpString, NULL);
	return tmpNode;
}
static UA_Node * OV_NodeStore_getCopyNode(void *handle, const UA_NodeId *nodeId){
	return (UA_Node*)OV_NodeStore_getNode(handle, nodeId);
}
static UA_StatusCode OV_NodeStore_removeNode(void *handle, const UA_NodeId *nodeId){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
	//OV_INSTPTR_ov_object pobj = opcua_nodeStoreFunctions_resolveNodeIdToOvObject((UA_NodeId*)nodeId);
	//if (pobj != NULL){
	//	Ov_DeleteObject(pobj);
	//}
    //return UA_STATUSCODE_GOOD;
}
static UA_StatusCode OV_NodeStore_insertNode(void *handle, UA_Node *node, UA_NodeId *parrentNode){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
	//OV_INSTPTR_ov_object pobj = opcua_nodeStoreFunctions_resolveNodeIdToOvObject(&(node->nodeId));
	//if (pobj != NULL)
	//	return UA_STATUSCODE_BADNODEIDEXISTS;

	//return OV_NodeStore_insert(handle, node, parrentNode);
}
static UA_StatusCode OV_NodeStore_replaceNode(void *handle, UA_Node *node){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}
static void OV_NodeStore_iterate(void *handle, void* visitorHandle, UA_NodestoreInterface_nodeVisitor visitor){

}
static UA_StatusCode OV_NodeStore_linkNamespace(void *handle, UA_UInt16 namespaceIndex){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}
static UA_StatusCode OV_NodeStore_unlinkNamespace(void *handle, UA_UInt16 namespaceIndex){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

UA_NodestoreInterface* servicesOPCUAInterface_interface_ovNodeStoreInterfaceServicesNew(void) {
	UA_NodestoreInterface *nsi = ov_database_malloc(sizeof(UA_NodestoreInterface));
    nsi->handle =        	NULL;
    nsi->deleteNodestore =  (UA_NodestoreInterface_deleteNodeStore) 		OV_NodeStore_deleteNodestore;
    nsi->newNode =       	(UA_NodestoreInterface_newNode)     OV_NodeStore_newNode;
    nsi->deleteNode =    	(UA_NodestoreInterface_deleteNode)  OV_NodeStore_deleteNode;
    nsi->insertNode =       	(UA_NodestoreInterface_insertNode)      OV_NodeStore_insertNode;
    nsi->getNode =          	(UA_NodestoreInterface_getNode)         OV_NodeStore_getNode;
    nsi->getNodeCopy =      	(UA_NodestoreInterface_getNodeCopy)     OV_NodeStore_getCopyNode;
    nsi->replaceNode =      	(UA_NodestoreInterface_replaceNode)     OV_NodeStore_replaceNode;
    nsi->removeNode =       	(UA_NodestoreInterface_removeNode)      OV_NodeStore_removeNode;
    nsi->iterate =       (UA_NodestoreInterface_iterate)     OV_NodeStore_iterate;
    nsi->releaseNode =       	(UA_NodestoreInterface_releaseNode)     OV_NodeStore_releaseNode;
    nsi->linkNamespace =       (UA_NodestoreInterface_linkNamespace)     OV_NodeStore_linkNamespace;
	nsi->unlinkNamespace =       	(UA_NodestoreInterface_unlinkNamespace)     OV_NodeStore_unlinkNamespace;
	return nsi;
}

void servicesOPCUAInterface_interface_ovNodeStoreInterfaceServicesDelete(UA_NodestoreInterface * nsi){
	if (nsi->handle)
		UA_free(nsi->handle);
}
