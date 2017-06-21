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

#ifndef OV_COMPILE_LIBRARY_openaasOPCUAInterface
#define OV_COMPILE_LIBRARY_openaasOPCUAInterface
#endif

#include "openaasOPCUAInterface.h"
#include "libov/ov_macros.h"
#include "ksbase.h"
#include "opcua.h"
#include "opcua_helpers.h"
#include "NoneTicketAuthenticator.h"
#include "libov/ov_path.h"
#include "libov/ov_memstack.h"
#include "ks_logfile.h"
#include "nodeset.h"
#include "ua_openaas_generated.h"
#include "ua_openaas_generated_handling.h"

extern OV_INSTPTR_openaasOPCUAInterface_interface pinterface;




OV_DLLFNCEXPORT UA_StatusCode openaasOPCUAInterface_interface_ovServiceOutputArgumentsNodeToOPCUA(
		void *handle, const UA_NodeId *nodeId, UA_Node** opcuaNode) {
	UA_Node 				*newNode = NULL;
	UA_StatusCode 			result = UA_STATUSCODE_GOOD;
	OV_PATH 				path;
	OV_INSTPTR_ov_object	pobj = NULL;
	OV_TICKET 				*pTicket = NULL;
	OV_VTBLPTR_ov_object	pVtblObj = NULL;
	OV_ACCESS				access;
	UA_NodeClass 			nodeClass;
	OV_ELEMENT				element;
	OV_STRING 				tmpString = NULL;
	OV_UINT 				len = 0;
	OV_STRING 				*plist = NULL;

	copyOPCUAStringToOV(nodeId->identifier.string, &tmpString);
	plist = ov_string_split(tmpString, "||", &len);
	ov_string_setvalue(&tmpString, NULL);

	UA_NodeId tmpNodeId;
	UA_NodeId_init(&tmpNodeId);
	tmpNodeId.namespaceIndex = nodeId->namespaceIndex;
	tmpNodeId.identifierType = nodeId->identifierType;
	tmpNodeId.identifier.string = UA_String_fromChars(plist[0]);
	ov_string_freelist(plist);

	ov_memstack_lock();
	result = opcua_nodeStoreFunctions_resolveNodeIdToPath(tmpNodeId, &path);
	UA_NodeId_deleteMembers(&tmpNodeId);
	if(result != UA_STATUSCODE_GOOD){
		ov_memstack_unlock();
		return result;
	}
	element = path.elements[path.size-1];
	ov_memstack_unlock();
	result = opcua_nodeStoreFunctions_getVtblPointerAndCheckAccess(&(element), pTicket, &pobj, &pVtblObj, &access);
	if(result != UA_STATUSCODE_GOOD){
		return result;
	}

	nodeClass = UA_NODECLASS_VARIABLE;
	newNode = (UA_Node*)UA_calloc(1, sizeof(UA_VariableNode));

	// Basic Attribute
	// BrowseName
	UA_QualifiedName qName;
	qName.name = UA_String_fromChars("OutputArguments");
	qName.namespaceIndex = 0; //pinterface->v_interfacenamespace.index;
	newNode->browseName = qName;

	// Description
	OV_STRING tempString = pVtblObj->m_getcomment(pobj, &(element));
	UA_LocalizedText lText;
	lText.locale = UA_String_fromChars("en");
	if(tempString){
		lText.text = UA_String_fromChars(tempString);
	} else {
		lText.text = UA_String_fromChars("");
	}
	newNode->description = lText;

	// DisplayName
	lText.locale = UA_String_fromChars("en");
	lText.text = UA_String_fromChars("OutputArguments");
	newNode->displayName = lText;

	// NodeId
	UA_NodeId_copy(nodeId, &newNode->nodeId);

	// NodeClass
	newNode->nodeClass 	= nodeClass;

	// WriteMask
	UA_UInt32 writeMask = 0;
	if(element.elemtype != OV_ET_VARIABLE){
		if(access & OV_AC_WRITE){
			writeMask |= (1<<2);	//	BrowseName
			writeMask |= (1<<6);	//	DisplayName
		}
		if(access & OV_AC_RENAMEABLE){
			writeMask |= (1<<14);	//	NodeId
		}
	}
	newNode->writeMask 	= writeMask;

	// Variable specific attributes
	// value
	OV_ELEMENT tmpPart;
	tmpPart.elemtype = OV_ET_NONE;
	tmpPart.pobj = NULL;
	OV_ELEMENT tmpParrent;
	tmpParrent.pobj = pobj;
	tmpParrent.elemtype = OV_ET_OBJECT;
	OV_UINT sizeOutput = 0;
	do {
		ov_element_getnextpart(&tmpParrent, &tmpPart, OV_ET_VARIABLE);
		if (tmpPart.elemtype == OV_ET_NONE)
			break;

		if (tmpPart.elemunion.pvar->v_flags == 16384){ // OutputFlag is set
			sizeOutput++;
			continue;
		}
	} while(TRUE);


	// arrayDemensions
	if (sizeOutput > 1){
		((UA_VariableNode*)newNode)->arrayDimensionsSize = 1;
		UA_UInt32 *pOutputDimension = UA_UInt32_new();
		*pOutputDimension = sizeOutput;
		((UA_VariableNode*)newNode)->arrayDimensions = pOutputDimension; //UA_Array_new(((UA_VariableNode*)newNode)->arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);	/*	scalar or one dimension	*/
		// valuerank
		((UA_VariableNode*)newNode)->valueRank = 1;	/*	one dimension	*/
	}else{
		((UA_VariableNode*)newNode)->arrayDimensionsSize = 0;
		((UA_VariableNode*)newNode)->arrayDimensions = NULL; //UA_Array_new(((UA_VariableNode*)newNode)->arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);	/*	scalar or one dimension	*/
		// valuerank
		((UA_VariableNode*)newNode)->valueRank = -1;	/*	one dimension	*/
	}

	// value
	((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->type = &UA_TYPES[UA_TYPES_ARGUMENT];
	((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->arrayLength = sizeOutput;
	if (sizeOutput > 1){
		((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data = UA_Array_new(sizeOutput, &UA_TYPES[UA_TYPES_ARGUMENT]);
	}else{
		((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data = UA_Argument_new();
	}

	if (!((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data){
		result = UA_STATUSCODE_BADOUTOFMEMORY;
		return result;
	}
	((UA_VariableNode*)newNode)->value.data.value.hasValue = TRUE;
	((UA_VariableNode*)newNode)->valueSource = UA_VALUESOURCE_DATA;

	tmpPart.elemtype = OV_ET_NONE;
	tmpPart.pobj = NULL;
	OV_UINT count = 0;
	do {
		ov_element_getnextpart(&tmpParrent, &tmpPart, OV_ET_VARIABLE);
		if (tmpPart.elemtype == OV_ET_NONE)
			break;

		if (tmpPart.elemunion.pvar->v_flags == 16384){ // OutputFlag is set
			UA_Argument_init(&((UA_Argument*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[count]);
			((UA_Argument*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[count].description = UA_LOCALIZEDTEXT_ALLOC("en_US","");
			((UA_Argument*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[count].name = UA_STRING_ALLOC(tmpPart.elemunion.pvar->v_identifier);
			((UA_Argument*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[count].valueRank = -1;
			switch(tmpPart.elemunion.pvar->v_vartype){
			case OV_VT_BOOL:
				((UA_Argument*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[count].dataType = UA_TYPES[UA_TYPES_BOOLEAN].typeId;
				((UA_Argument*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[count].valueRank = -1;
				break;
			case OV_VT_INT:
				((UA_Argument*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[count].dataType = UA_TYPES[UA_TYPES_INT32].typeId;
				((UA_Argument*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[count].valueRank = -1;
				break;
			case OV_VT_UINT:
				((UA_Argument*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[count].dataType = UA_TYPES[UA_TYPES_UINT32].typeId;
				((UA_Argument*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[count].valueRank = -1;
				break;
			case OV_VT_SINGLE:
				((UA_Argument*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[count].dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
				((UA_Argument*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[count].valueRank = -1;
				break;
			case OV_VT_DOUBLE:
				((UA_Argument*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[count].dataType = UA_TYPES[UA_TYPES_DOUBLE].typeId;
				((UA_Argument*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[count].valueRank = -1;
				break;
			case OV_VT_STRING:
				((UA_Argument*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[count].dataType = UA_TYPES[UA_TYPES_STRING].typeId;
				((UA_Argument*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[count].valueRank = -1;
				break;
			case OV_VT_BOOL_VEC:
				((UA_Argument*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[count].dataType = UA_TYPES[UA_TYPES_BOOLEAN].typeId;
				((UA_Argument*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[count].valueRank = 1;
				break;
			case OV_VT_INT_VEC:
				((UA_Argument*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[count].dataType = UA_TYPES[UA_TYPES_INT32].typeId;
				((UA_Argument*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[count].valueRank = 1;
				break;
			case OV_VT_UINT_VEC:
				((UA_Argument*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[count].dataType = UA_TYPES[UA_TYPES_UINT32].typeId;
				((UA_Argument*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[count].valueRank = 1;
				break;
			case OV_VT_SINGLE_VEC:
				((UA_Argument*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[count].dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
				((UA_Argument*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[count].valueRank = 1;
				break;
			case OV_VT_DOUBLE_VEC:
				((UA_Argument*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[count].dataType = UA_TYPES[UA_TYPES_DOUBLE].typeId;
				((UA_Argument*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[count].valueRank = 1;
				break;
			case OV_VT_STRING_VEC:
				((UA_Argument*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[count].dataType = UA_TYPES[UA_TYPES_STRING].typeId;
				((UA_Argument*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[count].valueRank = 1;
				break;
					break;
			}
			count++;
			if (count == sizeOutput){
				break;
			}else{
				continue;
			}
		}
	} while(TRUE);
	ov_string_setvalue(&tmpString, NULL);

	// accessLevel
	UA_Byte accessLevel = 0;
	if(access & OV_AC_READ){
		accessLevel |= (1<<0);
	}
	if(access & OV_AC_WRITE){
		accessLevel |= (1<<1);
	}
	((UA_VariableNode*)newNode)->accessLevel = accessLevel;
	// minimumSamplingInterval
	((UA_VariableNode*)newNode)->minimumSamplingInterval = -1;
	// historizing
	((UA_VariableNode*)newNode)->historizing = UA_FALSE;
	// dataType
	((UA_VariableNode*)newNode)->dataType = UA_NODEID_NUMERIC(0, 0);

	// References
	OV_INSTPTR_ov_object pchild = NULL;
	size_t size_references = 0;
	Ov_ForEachChild(ov_containment, Ov_DynamicPtrCast(ov_domain,pobj), pchild) {
		size_references++;
	}

	size_references = size_references + 2;// For Parent&TypeNode
	newNode->references = UA_calloc(size_references, sizeof(UA_ReferenceNode));
	if (!newNode->references){
		result = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
		UA_free(newNode);
		return result;
	}
	newNode->referencesSize = size_references;
	// ParentNode
	newNode->references[0].referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY);
	newNode->references[0].isInverse = UA_TRUE;
	len = 0;
	plist = NULL;
	tmpString = NULL;
	copyOPCUAStringToOV(nodeId->identifier.string, &tmpString);
	plist = ov_string_split(tmpString, "||", &len);
	newNode->references[0].targetId = UA_EXPANDEDNODEID_STRING_ALLOC(pinterface->v_interfacenamespace.index, plist[0]);
	ov_string_freelist(plist);
	ov_string_setvalue(&tmpString, NULL);

	// TypeNode
	newNode->references[1].referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION);
	newNode->references[1].isInverse = UA_FALSE;
	newNode->references[1].targetId = UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_PROPERTYTYPE);

	size_t i = 1;
	Ov_ForEachChild(ov_containment, Ov_DynamicPtrCast(ov_domain,pobj), pchild) {
		i++;
	}
	*opcuaNode = newNode;
	return UA_STATUSCODE_GOOD;
}
