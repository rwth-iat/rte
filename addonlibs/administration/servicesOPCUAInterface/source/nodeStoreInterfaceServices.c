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
#include "opcua.h"
#include "opcua_helpers.h"

OV_DLLFNCEXPORT UA_StatusCode servicesOPCUAInterface_interface_MethodCallback(UA_Server *server, const UA_NodeId *sessionId,
        void *sessionContext, const UA_NodeId *methodId,
        void *methodContext, const UA_NodeId *objectId,
        void *objectContext, size_t inputSize,
        const UA_Variant *input, size_t outputSize,
        UA_Variant *output){

	UA_String *tmpStringArray = NULL;
	UA_StatusCode result = UA_STATUSCODE_GOOD;

	OV_VTBLPTR_services_Service pvtable;
	OV_INSTPTR_services_Service pService = (OV_INSTPTR_services_Service)methodContext;
	Ov_GetVTablePtr(services_Service, pvtable, pService);

	void **inputs = ov_database_malloc(sizeof(void*)*inputSize);
	for (OV_UINT i = 0; i < inputSize; i++){
		inputs[i] = NULL;
	}

	OV_UINT inputCounts = 0;
	for (OV_UINT i = 0; i < inputSize; i++){
		inputCounts++;
		inputs[i] = NULL;

		//TODO use type transformation from opcua_helpers to ensure integrity
//		OV_ANY inputAny = OV_ANY_INIT;
//		result = opcua_helpers_UAVariantToOVAny(&input[i], &inputAny);
//		if(result != UA_STATUSCODE_GOOD)
//			goto cleanup;
//		inputs[i] = &inputAny.value.valueunion;

		if(input[i].type == NULL){
			continue;
		}

		if(input[i].arrayDimensionsSize == 0 && input[i].arrayLength == 0){
			switch(input[i].type->typeIndex){
				case UA_TYPES_BOOLEAN:
				case UA_TYPES_INT32:
				case UA_TYPES_UINT32:
				case UA_TYPES_FLOAT:
				case UA_TYPES_DOUBLE:
				case UA_TYPES_VARIANT:
					if (input[i].data == NULL || input[i].data == (void*)0x1){
						result = UA_STATUSCODE_BADARGUMENTSMISSING;
						goto cleanup;
					}
					inputs[i] = input[i].data;
					break;
				case UA_TYPES_STRING:
					if (input[i].data == NULL || input[i].data == (void*)0x1){
						result = UA_STATUSCODE_BADARGUMENTSMISSING;
						goto cleanup;
					}
//					if ((*((UA_String*)(input[i].data))).length == 0){
//						result = UA_STATUSCODE_BADARGUMENTSMISSING;
//						goto cleanup;
//					}
					inputs[i] = ov_database_malloc(sizeof(OV_STRING));
					opcua_helpers_copyUAStringToOV(*((UA_String*)(input[i].data)), (OV_STRING*)(inputs[i]));
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
//						if (((UA_String*)(input[i].data))[j].length == 0){
//							result = UA_STATUSCODE_BADARGUMENTSMISSING;
//							goto cleanup;
//						}
						opcua_helpers_copyUAStringToOV(((UA_String*)(input[i].data))[j], &((*(OV_STRING_VEC*)(inputs[i])).value[j]));
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
				if((*(OV_STRING_VEC*)outputs[i]).value[j])
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
    		if(input[i].type == NULL){
    			continue;
    		}
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

static void servicesOPCUAInterface_deleteNodestore(void *context){

}

static void servicesOPCUAInterface_deleteNode(void *context, UA_Node *node){
	if (node){
		UA_Node_deleteMembers(node);
	}
	UA_free(node);
}
static void servicesOPCUAInterface_releaseNode(void *context, const UA_Node *node){
	servicesOPCUAInterface_deleteNode(context, (UA_Node*)node);
}

static UA_Node * servicesOPCUAInterface_newNode(void *context, UA_NodeClass nodeClass){
    //allocate memory for a new node
	//UA_Node *newNode = NULL;
	//newNode = (UA_Node*) ov_database_malloc(sizeof(UA_ObjectNode));
	//newNode->nodeClass = nodeClass;
    return NULL;
}
static const UA_Node * servicesOPCUAInterface_getNode(void *context, const UA_NodeId *nodeId){
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
	opcua_helpers_copyUAStringToOV(nodeId->identifier.string, &tmpString);
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
				if (servicesOPCUAInterface_interface_ovServiceInputArgumentsNodeToOPCUA(context, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD){
					ov_string_freelist(plist);
					ov_string_freelist(plist2);
					ov_string_freelist(plist3);
					ov_string_setvalue(&tmpString, NULL);
					return (UA_Node*) opcuaNode;
				}
			}else if (ov_string_compare(plist[1], "OutputArguments") == OV_STRCMP_EQUAL){
				if (servicesOPCUAInterface_interface_ovServiceOutputArgumentsNodeToOPCUA(context, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD){
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
		if (servicesOPCUAInterface_interface_ovServiceNodeToOPCUA(context, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			tmpNode = opcuaNode;
	}
	ov_string_freelist(plist);
	ov_string_freelist(plist2);
	ov_string_freelist(plist3);
	ov_string_setvalue(&tmpString, NULL);
	return tmpNode;
}
static UA_StatusCode servicesOPCUAInterface_getCopyNode(void *context, const UA_NodeId *nodeId, UA_Node ** nodeOut){
	UA_Node* node = (UA_Node*) servicesOPCUAInterface_getNode(context, nodeId);
	if(node == NULL)
		return UA_STATUSCODE_BADNODEIDUNKNOWN;
	*nodeOut = node;
	return UA_STATUSCODE_GOOD;
}
static UA_StatusCode servicesOPCUAInterface_removeNode(void *context, const UA_NodeId *nodeId){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
	//OV_INSTPTR_ov_object pobj = opcua_nodeStoreFunctions_resolveNodeIdToOvObject((UA_NodeId*)nodeId);
	//if (pobj != NULL){
	//	Ov_DeleteObject(pobj);
	//}
    //return UA_STATUSCODE_GOOD;
}
static UA_StatusCode servicesOPCUAInterface_insertNode(void *context, UA_Node *node, UA_NodeId *parrentNode){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
	//OV_INSTPTR_ov_object pobj = opcua_nodeStoreFunctions_resolveNodeIdToOvObject(&(node->nodeId));
	//if (pobj != NULL)
	//	return UA_STATUSCODE_BADNODEIDEXISTS;

	//return servicesOPCUAInterface_insert(context, node, parrentNode);
}
static UA_StatusCode servicesOPCUAInterface_replaceNode(void *context, UA_Node *node){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}
static void servicesOPCUAInterface_iterate(void *context, void* visitorHandle, UA_NodestoreVisitor visitor){

}

UA_Nodestore* servicesOPCUAInterface_interface_ovNodeStoreInterfaceServicesNew(OV_INSTPTR_servicesOPCUAInterface_interface context) {
	UA_Nodestore* trafo = UA_malloc(sizeof(UA_Nodestore));
	if(trafo == NULL)
		return NULL;
    trafo->context =          context;
    trafo->newNode =       	  servicesOPCUAInterface_newNode;
    trafo->deleteNode =    	  servicesOPCUAInterface_deleteNode;
    trafo->deleteNodestore =  servicesOPCUAInterface_deleteNodestore;

    trafo->getNode =          servicesOPCUAInterface_getNode;
    trafo->releaseNode =      servicesOPCUAInterface_releaseNode;

    trafo->getNodeCopy =      servicesOPCUAInterface_getCopyNode;
    trafo->insertNode =       servicesOPCUAInterface_insertNode;
    trafo->replaceNode =      servicesOPCUAInterface_replaceNode;

    trafo->removeNode =       servicesOPCUAInterface_removeNode;

    trafo->iterate =          servicesOPCUAInterface_iterate;
    return trafo;
}

void servicesOPCUAInterface_interface_ovNodeStoreInterfaceServicesDelete(UA_Nodestore * store){
	store->context = NULL;
	UA_free(store);
}
