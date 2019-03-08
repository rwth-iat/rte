/*
 * ovStore.c
 *
 *  Created on: 06.12.2016
 *      Author: Torben Deppe, Julian Grothoff
 *
 *  Collects functions that are necessary to transform ov object to ua nodes via UA_Nodestore as interface.
 */

#ifndef OV_COMPILE_LIBRARY_opcua
#define OV_COMPILE_LIBRARY_opcua
#endif

#include "opcua_ovStore.h"
#include "libov/ov_macros.h"
#include "opcua.h"
#include "opcua_helpers.h"
#include "NoneTicketAuthenticator.h"

OV_DLLFNCEXPORT UA_UInt16 opcua_ovStore_searchNamespaceIndex(void * context, OV_INSTPTR_ov_object pobj, OV_BOOL forTypes){
	//Get ua server connected to this interface
	OV_INSTPTR_opcua_server server = NULL;
	if(Ov_CanCastTo(opcua_interface, (OV_INSTPTR_opcua_interface)context)){
		server = Ov_GetParent(opcua_serverToInterfaces, Ov_StaticPtrCast(opcua_interface, (OV_INSTPTR_opcua_interface)context));
	}
	if(server == NULL)
		return OPCUA_OVSTORE_DEFAULTNSINDEX;

	//Combine search Uri
	OV_INSTPTR_ov_class pclass = Ov_GetClassPtr(pobj);
	OV_INSTPTR_ov_domain plibrary = Ov_GetParent(ov_containment, pclass);
	UA_String searchUri = UA_String_fromChars(OPCUA_OVSTORE_DEFAULTNSURI_PREFIX);
	opcua_helpers_UA_String_append(&searchUri, plibrary->v_identifier);
	if(forTypes) opcua_helpers_UA_String_append(&searchUri, OPCUA_OVSTORE_DEFAULTNSURI_SUFFIX);

	//Search for namespace in UA Server
	UA_UInt16 index = OPCUA_OVSTORE_DEFAULTNSINDEX;
	UA_Server_getNamespaceByName(server->v_server, searchUri, (size_t *)&index);
	//Free Resources from namespace search
	pclass = NULL;
	plibrary = NULL;
	UA_String_deleteMembers(&searchUri);

	return index;
}

/*	only use numeric nodeIDs	*/
//static UA_StatusCode opcua_ovStore_fakeReferenceDescription(
//		UA_NodeId refNodeId, OV_STRING browsename, UA_UInt16 browsenameNsIndex, OV_STRING displayname, UA_NodeClass nodeClass, UA_NodeId type,
//			UA_UInt16 referenceTypeNamespaceIndex, UA_Int32 referenceType, UA_Boolean isForward, UA_ReferenceDescription* dst){
//	UA_StatusCode			result			=	UA_STATUSCODE_GOOD;
//	if(!dst){
//		return UA_STATUSCODE_BADINVALIDARGUMENT;
//	}
//
//	dst->nodeId.nodeId = refNodeId;
//
//	dst->browseName.name = UA_String_fromChars(browsename);
//	dst->browseName.namespaceIndex = browsenameNsIndex;
//
//	dst->displayName.text = UA_String_fromChars(displayname);
//	dst->displayName.locale = UA_String_fromChars("en");
//
//	dst->isForward = isForward;
//
//	dst->nodeClass = nodeClass;
//
//	// in ov-namespace beware that numerical ids may change when the server restarts (but associations can change between /acplt/... and /Libraries/..., too)
//	dst->referenceTypeId.namespaceIndex = referenceTypeNamespaceIndex;
//	dst->referenceTypeId.identifierType = UA_NODEIDTYPE_NUMERIC;
//	dst->referenceTypeId.identifier.numeric = referenceType;
//
//	// TODO fixme	This is the type-node: using 0|58 (baseObjectType) for all variables
//	if(dst->nodeClass == UA_NODECLASS_OBJECT || dst->nodeClass == UA_NODECLASS_VARIABLE){
//		dst->typeDefinition.nodeId = type;
//	} else {
//		dst->typeDefinition.nodeId = UA_NODEID_NULL;
//	}
//
//	return result;
//}

static UA_StatusCode opcua_ovStore_fillReferenceDescription(
		void * context,
		OV_ELEMENT* pElement, UA_UInt16 referenceTypeNamespaceIndex, UA_Int32 referenceType, UA_Boolean isForward, UA_ReferenceDescription* dst){
	OV_INSTPTR_ov_object	pObject			=	NULL;
	UA_StatusCode			result			=	UA_STATUSCODE_GOOD;
	OV_STRING				path			=	NULL;
	OV_STRING				varPath			=	NULL;
	OV_UINT					varPathLen		=	0;
	if(!dst){
		return UA_STATUSCODE_BADINVALIDARGUMENT;
	}

	dst->nodeId.nodeId.identifierType = UA_NODEIDTYPE_STRING;
	dst->nodeId.nodeId.namespaceIndex = opcua_ovStore_searchNamespaceIndex(context, pElement->pobj, FALSE);

	if(pElement->elemtype == OV_ET_OBJECT || pElement->elemtype == OV_ET_VARIABLE || pElement->elemtype == OV_ET_MEMBER){
		pObject = pElement->pobj;
	} else {
		return UA_STATUSCODE_BADNOTSUPPORTED;
	}
	ov_memstack_lock();
	path = ov_path_getcanonicalpath(pObject, 2);
	if(!path){
		ov_memstack_unlock();
		return UA_STATUSCODE_BADOUTOFMEMORY;
	}
	if(pElement->elemtype == OV_ET_VARIABLE){
		varPathLen = ov_string_getlength(path) + ov_string_getlength(pElement->elemunion.pvar->v_identifier) + 2;
		varPath = ov_memstack_alloc(varPathLen);
		if(!varPath){
			ov_memstack_unlock();
			return UA_STATUSCODE_BADOUTOFMEMORY;
		}
		snprintf(varPath, varPathLen, "%s.%s", path, pElement->elemunion.pvar->v_identifier);
		path = varPath;
	}
	dst->nodeId.nodeId.identifier.string = UA_String_fromChars(path);
	if(dst->nodeId.nodeId.identifier.string.length == 0 && dst->nodeId.nodeId.identifier.string.data != UA_EMPTY_ARRAY_SENTINEL){
		result = UA_STATUSCODE_BADOUTOFMEMORY;
	} else {
		result = UA_STATUSCODE_GOOD;
	}

	if(pElement->elemtype == OV_ET_OBJECT){
		dst->browseName.name = UA_String_fromChars(pElement->pobj->v_identifier);
	} else if(pElement->elemtype == OV_ET_VARIABLE){
		dst->browseName.name = UA_String_fromChars(pElement->elemunion.pvar->v_identifier);
	}
	dst->browseName.namespaceIndex = dst->nodeId.nodeId.namespaceIndex;

	if(pElement->elemtype == OV_ET_OBJECT){
		dst->displayName.text = UA_String_fromChars(pElement->pobj->v_identifier);
	} else if(pElement->elemtype == OV_ET_VARIABLE){
		dst->displayName.text = UA_String_fromChars(pElement->elemunion.pvar->v_identifier);
	}

	dst->isForward = isForward;

	dst->nodeClass = opcua_helpers_getNodeClass(pElement);

	// in ov-namespace beware that numerical ids may change when the server restarts (but associations can change between /acplt/... and /Libraries/..., too)
	dst->referenceTypeId.namespaceIndex = referenceTypeNamespaceIndex;
	dst->referenceTypeId.identifierType = UA_NODEIDTYPE_NUMERIC;
	dst->referenceTypeId.identifier.numeric = referenceType;

	// TODO fixme	This is the type-node: using 0|58 (baseObjectType) for all variables
	if(dst->nodeClass == UA_NODECLASS_OBJECT){
		dst->typeDefinition.nodeId.namespaceIndex = OPCUA_OVSTORE_DEFAULTNSINDEX;
		dst->typeDefinition.nodeId.identifierType = UA_NODEIDTYPE_NUMERIC;
		dst->typeDefinition.nodeId.identifier.numeric = pElement->pobj->v_idL; //TODO Use string nodeId from class definition
	} else if(dst->nodeClass == UA_NODECLASS_VARIABLE){
		dst->typeDefinition.nodeId.namespaceIndex = 0;
		dst->typeDefinition.nodeId.identifierType = UA_NODEIDTYPE_NUMERIC;
		dst->typeDefinition.nodeId.identifier.numeric = 58;
	} else {
		dst->typeDefinition.nodeId = UA_NODEID_NULL;
	}
	ov_memstack_unlock();
	return result;
}
/******************************************************************
 * helper for Organizes
 *****************************************************************/

static UA_StatusCode opcua_ovStore_getOrganizes(void * context, OV_ELEMENT* pNode, OV_UINT* refCount,
		OV_BOOL fillDescription, UA_ReferenceDescription* dst,
		UA_StatusCode* statusCode){
	OV_ELEMENT	childElement, parentElement;
	OV_ACCESS access = OV_AC_NONE;
	if(pNode->elemtype != OV_ET_OBJECT)
		return UA_STATUSCODE_GOOD;

	//Forward direction
	childElement.elemtype = OV_ET_OBJECT;
	childElement.pobj = NULL;
	childElement.pvalue = NULL;
	childElement.pvar = NULL;
	Ov_ForEachChild(ov_containment, Ov_StaticPtrCast(ov_domain, pNode->pobj), childElement.pobj){
		childElement.elemunion.pobj = childElement.pobj;
		if(fillDescription){
			*statusCode = opcua_ovStore_fillReferenceDescription(context, &childElement, 0, UA_NS0ID_ORGANIZES, UA_TRUE, &(dst[*refCount]));
		}
		(*refCount)++;
	}
	// Backward / inverse direction
	if(pNode->pobj->v_idL || pNode->pobj->v_idH){	// don't do it for ov root object ("/")
		parentElement.elemtype = OV_ET_OBJECT;
		parentElement.pvalue = NULL;
		parentElement.pvar = NULL;
		parentElement.pobj = Ov_StaticPtrCast(ov_object, Ov_GetParent(ov_containment, pNode->pobj));
		childElement.elemunion.pobj = parentElement.pobj;
		access = opcua_helpers_getAccess(&parentElement);
		if(access & OV_AC_READ){
			if(fillDescription){
				*statusCode = opcua_ovStore_fillReferenceDescription(context, &parentElement, 0, UA_NS0ID_ORGANIZES, UA_FALSE, &(dst[*refCount]));
			}
			(*refCount)++;
		}
	} else {	//	ov root-node --> bridge node in ns0
		if(fillDescription){
			*statusCode = UA_STATUSCODE_BADNOTIMPLEMENTED; //TODO implement backward reference to Objects folder
		}
		(*refCount)++;
	}
	return UA_STATUSCODE_GOOD;
}

/******************************************************************
 * helper for HasTypeDefinition
 *****************************************************************/

static UA_Int32 opcua_ovStore_getHasTypeDefinition(void * context, OV_ELEMENT* pNode, OV_UINT* refCount,
		OV_BOOL fillDescription, UA_ReferenceDescription* dst,
		UA_StatusCode* statusCode){
	OV_ACCESS				access		=	OV_AC_NONE;
	OV_INSTPTR_ov_object	pChildObject	=	NULL;
	OV_ELEMENT				parentTypeDefinition, childTypeDefinition;
	//	maps to ov/instantiation
	// Forward direction
	//	source is object target is type --> this direction is an instantiation getparent for objects,
	//		and analog for links and variables
	if(pNode->elemtype == OV_ET_OBJECT || pNode->elemtype == OV_ET_VARIABLE
			|| pNode->elemtype == OV_ET_MEMBER || pNode->elemtype == OV_ET_CHILDLINK || pNode->elemtype == OV_ET_PARENTLINK){
		access = opcua_helpers_getAccess(pNode);
		if(access & OV_AC_READ){
			if(fillDescription){
				if(pNode->elemtype == OV_ET_OBJECT){
					parentTypeDefinition.elemtype = OV_ET_OBJECT;
					parentTypeDefinition.pobj = Ov_PtrUpCast(ov_object, Ov_GetParent(ov_instantiation, pNode->pobj));
					*statusCode = opcua_ovStore_fillReferenceDescription(context, &parentTypeDefinition, 0, UA_NS0ID_HASTYPEDEFINITION, UA_TRUE, &(dst[*refCount]));
				} else if (pNode->elemtype == OV_ET_VARIABLE || pNode->elemtype == OV_ET_MEMBER){
					parentTypeDefinition.elemtype = OV_ET_OBJECT;
					parentTypeDefinition.pobj = Ov_PtrUpCast(ov_object, pNode->elemunion.pvar);
					*statusCode = opcua_ovStore_fillReferenceDescription(context, &parentTypeDefinition, 0, UA_NS0ID_HASTYPEDEFINITION, UA_TRUE, &(dst[*refCount]));
				}else if (pNode->elemtype == OV_ET_PARENTLINK || pNode->elemtype == OV_ET_CHILDLINK){
					parentTypeDefinition.elemtype = OV_ET_OBJECT;
					parentTypeDefinition.pobj = Ov_PtrUpCast(ov_object, pNode->elemunion.passoc);
					*statusCode = opcua_ovStore_fillReferenceDescription(context, &parentTypeDefinition, 0, UA_NS0ID_HASTYPEDEFINITION, UA_TRUE, &(dst[*refCount]));
				}
			}
			(*refCount)++;
		}
	}
	// Backward / inverse direction
	//	this is an instantiation getchild for objects (classes), for variables and links this direction delivers nothing
	if(pNode->elemtype == OV_ET_OBJECT && Ov_GetClassPtr(pNode->pobj) == pclass_ov_class){
		Ov_ForEachChild(ov_instantiation, Ov_StaticPtrCast(ov_class, pNode->pobj), pChildObject){
			childTypeDefinition.elemtype = OV_ET_OBJECT;
			childTypeDefinition.pobj = pChildObject;
			access = opcua_helpers_getAccess(&childTypeDefinition);
			if(access & OV_AC_READ){
				if(fillDescription){
					*statusCode = opcua_ovStore_fillReferenceDescription(context, &childTypeDefinition, 0, UA_NS0ID_HASTYPEDEFINITION, UA_FALSE, &(dst[*refCount]));
				}
				(*refCount)++;
			}
		}
	}
	return UA_STATUSCODE_GOOD;
}

/******************************************************************
 * helper for HasSubtype
 *****************************************************************/

static UA_Int32 opcua_ovStore_getHasSubtype(void * context, OV_ELEMENT* pNode, OV_UINT* refCount,
		OV_BOOL fillDescription, UA_ReferenceDescription* dst,
		UA_StatusCode* statusCode){
	OV_ACCESS				access		=	OV_AC_NONE;
	OV_INSTPTR_ov_class		pChildClass	=	NULL;
	OV_INSTPTR_ov_class		pParentClass	=	NULL;
	OV_ELEMENT				referencedElement;
	//	maps to ov/inheritance
	// Forward direction
	if(pNode->elemtype == OV_ET_OBJECT && Ov_GetClassPtr(pNode->pobj) == pclass_ov_class){
		Ov_ForEachChild(ov_inheritance, Ov_StaticPtrCast(ov_class, pNode->pobj), pChildClass){
			referencedElement.elemtype = OV_ET_OBJECT;
			referencedElement.pobj = Ov_PtrUpCast(ov_object, pChildClass);
			access = opcua_helpers_getAccess(&referencedElement);
			if(access & OV_AC_READ){
				if(fillDescription){
					*statusCode = opcua_ovStore_fillReferenceDescription(context, &referencedElement, 0, UA_NS0ID_HASSUBTYPE, UA_TRUE, &(dst[*refCount]));
				}
				(*refCount)++;
			}
		}
	}
	// Backward / inverse direction
	if(pNode->elemtype == OV_ET_OBJECT && pNode->pobj == Ov_PtrUpCast(ov_object, pclass_ov_class)){
		pParentClass = Ov_GetParent(ov_inheritance, Ov_StaticPtrCast(ov_class, pNode->pobj));
		if(pParentClass){
			referencedElement.elemtype = OV_ET_OBJECT;
			referencedElement.pobj = Ov_PtrUpCast(ov_object, pParentClass);
			access = opcua_helpers_getAccess(&referencedElement);
			if(access & OV_AC_READ){
				if(fillDescription){
					*statusCode = opcua_ovStore_fillReferenceDescription(context, &referencedElement, 0, UA_NS0ID_HASSUBTYPE, UA_FALSE, &(dst[*refCount]));
				}
				(*refCount)++;
			}
		}
	}
	return UA_STATUSCODE_GOOD;
}

/******************************************************************
 * helper for HasProperty
 *****************************************************************/

static UA_Int32 opcua_ovStore_getHasProperty(void * context, OV_ELEMENT* pNode, OV_UINT* refCount,
		OV_BOOL fillDescription, UA_ReferenceDescription* dst, UA_StatusCode* statusCode){
	OV_ACCESS				access		=	OV_AC_NONE;
	OV_ELEMENT				referencedElement;
	//	maps to variables in objects
	// Forward direction
	if(pNode->elemtype == OV_ET_OBJECT){
		referencedElement.elemtype = OV_ET_NONE;
		ov_element_getnextpart(pNode, &referencedElement, OV_ET_VARIABLE | OV_ET_MEMBER);
		while(referencedElement.elemtype != OV_ET_NONE){
			access = opcua_helpers_getAccess(&referencedElement);
			if(access & OV_AC_READ){
				if(fillDescription){
					*statusCode = opcua_ovStore_fillReferenceDescription(context, &referencedElement, 0, UA_NS0ID_HASPROPERTY, UA_TRUE, &(dst[*refCount]));
				}
				(*refCount)++;
			}
			ov_element_getnextpart(pNode, &referencedElement, OV_ET_VARIABLE | OV_ET_MEMBER);
		}
	}
	// Backward / inverse direction
	if(pNode->elemtype == OV_ET_VARIABLE || pNode->elemtype == OV_ET_MEMBER){
		referencedElement.elemtype = OV_ET_OBJECT;
		referencedElement.pobj = pNode->pobj;
		access = opcua_helpers_getAccess(&referencedElement);
		if(access & OV_AC_READ){
			if(fillDescription){
				*statusCode = opcua_ovStore_fillReferenceDescription(context, &referencedElement, 0, UA_NS0ID_HASPROPERTY, UA_FALSE, &(dst[*refCount]));
			}
			(*refCount)++;
		}
	}
	return UA_STATUSCODE_GOOD;
}

/******************************************************************
 * helper for HasComponent
 *****************************************************************/

static UA_Int32 opcua_ovStore_getHasComponent(void * context, OV_ELEMENT* pNode, OV_UINT* refCount,
		OV_BOOL fillDescription, UA_ReferenceDescription* dst,
		UA_StatusCode* statusCode){
	OV_ACCESS				access		=	OV_AC_NONE;
	OV_ELEMENT				referencedElement;
	//	maps to variables in objects
	// Forward direction
	if(pNode->elemtype == OV_ET_OBJECT){
		referencedElement.elemtype = OV_ET_NONE;
		ov_element_getnextpart(pNode, &referencedElement, OV_ET_OBJECT);
		while(referencedElement.elemtype != OV_ET_NONE){
			access = opcua_helpers_getAccess(&referencedElement);
			if(access & OV_AC_READ){
				if(fillDescription){
					*statusCode = opcua_ovStore_fillReferenceDescription(context, &referencedElement, 0, UA_NS0ID_HASCOMPONENT, UA_TRUE, &(dst[*refCount]));
				}
				(*refCount)++;
			}
			ov_element_getnextpart(pNode, &referencedElement, OV_ET_OBJECT);
		}
	}
	// Backward / inverse direction
	if(pNode->elemtype == OV_ET_OBJECT){
		if(pNode->pobj->v_pouterobject){
			referencedElement.elemtype = OV_ET_OBJECT;
			referencedElement.pobj = pNode->pobj->v_pouterobject;
			access = opcua_helpers_getAccess(&referencedElement);
			if(access & OV_AC_READ){
				if(fillDescription){
					*statusCode = opcua_ovStore_fillReferenceDescription(context, &referencedElement, 0, UA_NS0ID_HASCOMPONENT, UA_FALSE, &(dst[*refCount]));
				}
				(*refCount)++;
			}
		}
	}
	return UA_STATUSCODE_GOOD;
}

/******************************************************************
 * helper for OVReferences
 *****************************************************************/

//static UA_Int32 opcua_ovStore_getOvReferences(void * context, OV_ELEMENT* pNode, signed char flag, OV_INSTPTR_ov_association passoc,
//		OV_UINT* refCount, OV_BOOL fillDescription, UA_ReferenceDescription* dst, UA_StatusCode* statusCode){
//	OV_ACCESS				access		=	OV_AC_NONE;
//	OV_ELEMENT				linkElement;
//	OV_ELEMENT				referencedElement;
//
//	if(flag < 0){	// -1: association definde by passoc; -2 all associations
//		// Forward direction
//		if(pNode->elemtype == OV_ET_OBJECT){
//			linkElement.elemtype = OV_ET_NONE;
//			ov_element_getnextpart(pNode, &linkElement, OV_ET_PARENTLINK);
//			while(linkElement.elemtype != OV_ET_NONE){
//				if(flag == -2 || linkElement.elemunion.passoc == passoc){
//					if(linkElement.elemunion.passoc->v_assoctype == OV_AT_ONE_TO_MANY){
//						referencedElement.elemtype = OV_ET_OBJECT;
//						Ov_Association_ForEachChild(linkElement.elemunion.passoc, pNode->pobj, referencedElement.pobj){
//							access = opcua_helpers_getAccess(&referencedElement);
//							if(access & OV_AC_READ){
//								if(fillDescription){
//									*statusCode = opcua_ovStore_fillReferenceDescription(context, &referencedElement, OPCUA_OVSTORE_DEFAULTNSINDEX,
//											linkElement.elemunion.passoc->v_idL, UA_TRUE, &(dst[*refCount]));
//								}
//								(*refCount)++;
//							}
//						}
//					} else if(linkElement.elemunion.passoc->v_assoctype == OV_AT_ONE_TO_ONE) {
//						referencedElement.elemtype = OV_ET_OBJECT;
//						referencedElement.pobj = Ov_Association_GetChild(linkElement.elemunion.passoc, pNode->pobj);
//						if(referencedElement.pobj){
//							access = opcua_helpers_getAccess(&referencedElement);
//							if(access & OV_AC_READ){
//								if(fillDescription){
//									*statusCode = opcua_ovStore_fillReferenceDescription(context, &referencedElement, OPCUA_OVSTORE_DEFAULTNSINDEX,
//											linkElement.elemunion.passoc->v_idL, UA_TRUE, &(dst[*refCount]));
//								}
//								(*refCount)++;
//							}
//						}
//					}
//					if(flag == -1){
//						break;
//					}
//				}
//				ov_element_getnextpart(pNode, &linkElement, OV_ET_PARENTLINK);
//			}
//		}
//		// Backward / inverse direction
//		if(pNode->elemtype == OV_ET_OBJECT){
//			linkElement.elemtype = OV_ET_NONE;
//			ov_element_getnextpart(pNode, &linkElement, OV_ET_CHILDLINK);
//			while(linkElement.elemtype != OV_ET_NONE){
//				if(flag == -2 || linkElement.elemunion.passoc == passoc){
//					referencedElement.elemtype = OV_ET_OBJECT;
//					referencedElement.pobj = Ov_Association_GetParent(linkElement.elemunion.passoc, pNode->pobj);
//					if(referencedElement.pobj){
//						access = opcua_helpers_getAccess(&referencedElement);
//						if(access & OV_AC_READ){
//							if(fillDescription){
//								*statusCode = opcua_ovStore_fillReferenceDescription(context, &referencedElement, OPCUA_OVSTORE_DEFAULTNSINDEX,
//										linkElement.elemunion.passoc->v_idL, UA_FALSE, &(dst[*refCount]));
//							}
//							(*refCount)++;
//						}
//					}
//					if(flag == -1){
//						break;
//					}
//				}
//				ov_element_getnextpart(pNode, &linkElement, OV_ET_CHILDLINK);
//			}
//		}
//	}
//	return UA_STATUSCODE_GOOD;
//}

//TODO add inverse HasSubtype (HasSupertype) reference for classes.
//TODO variable and port values are NULL, BadNotReadable
//TODO method propertys are not displayed correctly --> servicesOPCUAInterface
//TODO use function UA_Node_addReference and AddReferenceItem instead of ReferenceDescription
OV_DLLFNCEXPORT UA_StatusCode opcua_ovStore_addReference(void * context, UA_Node *node){
	// references
	UA_ReferenceDescription *references = NULL;
	OV_UINT 				refCount = 0;
	OV_ELEMENT 				pNode;
	OV_PATH					nodePath;
	OV_UINT					refCountBefore	=	0;
	UA_StatusCode			result = UA_STATUSCODE_GOOD;

	#define UAREFS_LENGTH		12
	#define OVREFSARRAY_CHUNKSIZE	32
	signed char				uaReferences [UAREFS_LENGTH];	//array for ua-references: 0: leave out, -1: to do, 1: done; specific for ovReferences: -2 do all
	//TODO define as enum
	#define INDEX_References					0
	#define INDEX_NonHierarchicalReferences		1
	#define INDEX_HierarchicalReferences		2
	#define INDEX_HasChild						3
	#define INDEX_Organizes						4
	#define INDEX_HasTypeDefinition				5
	#define INDEX_Aggregates					6
	#define INDEX_HasSubtype					7
	#define INDEX_HasProperty					8
	#define INDEX_HasComponent					9
	#define INDEX_OvReference					10

	ov_memstack_lock();
	if(opcua_helpers_resolveNodeIdToPath(node->nodeId, &nodePath) != UA_STATUSCODE_GOOD){
		return UA_STATUSCODE_BADNODEIDUNKNOWN;
	}
	pNode = nodePath.elements[nodePath.size - 1];
	ov_memstack_unlock();
	for(OV_UINT j=1; j<UAREFS_LENGTH; j++){
		uaReferences[j] = -1;
	}
	uaReferences[INDEX_OvReference] = -2;
/*********************************************************************************************************************************
* count all reference
********************************************************************************************************************************/
	// HasProperty
	opcua_ovStore_getHasProperty(context, &pNode, &refCount, FALSE, NULL, NULL);
	if (refCount == refCountBefore){
		uaReferences[INDEX_HasProperty] = 1;
	}
	refCountBefore = refCount;
	// HasTypeDefinition
	opcua_ovStore_getHasTypeDefinition(context, &pNode, &refCount, FALSE, NULL, NULL);
	if (refCount == refCountBefore){
		uaReferences[INDEX_HasTypeDefinition] = 1;
	}
	refCountBefore = refCount;
	// Organizes
	opcua_ovStore_getOrganizes(context, &pNode, &refCount, FALSE, NULL, NULL);
	if (refCount == refCountBefore){
		uaReferences[INDEX_Organizes] = 1;
	}
	refCountBefore = refCount;
	// HasSubtype
	opcua_ovStore_getHasSubtype(context, &pNode, &refCount, FALSE, NULL, NULL);
	if(refCount == refCountBefore){
		uaReferences[INDEX_HasSubtype] = 1;
	}
	refCountBefore = refCount;
	// HasComponent
	opcua_ovStore_getHasComponent(context, &pNode, &refCount, FALSE, NULL, NULL);
	if(refCount == refCountBefore){
		uaReferences[INDEX_HasComponent] = 1;
	}
	refCountBefore = refCount;
	// OvReferences
	//TODO OvReferences NOTIMPLEMENTED
	//  opcua_ovStore_getOvReferences(pNode, uaReferences[INDEX_OvReference], pAssoc, refCount);
	//  if(*refCount == refCountBefore){
	//		uaReferences[INDEX_OvReference] = 1;
	//	}
	//	refCountBefore = *refCount;
	references = UA_Array_new(refCount, &UA_TYPES[UA_TYPES_REFERENCEDESCRIPTION]);

	refCount = 0;

	/*********************************************************************************************************************************
	 * fill in results for all reference types
	 ********************************************************************************************************************************/
	// HasProperty
	if (uaReferences[INDEX_HasProperty] != 1){
		opcua_ovStore_getHasProperty(context, &pNode, &refCount,
				TRUE, references, &result);
	}
	// HasTypeDefinition
	if (uaReferences[INDEX_HasTypeDefinition] != 1){
		opcua_ovStore_getHasTypeDefinition(context, &pNode, &refCount,
				TRUE, references, &result);
	}
	// Organizes
	if (uaReferences[INDEX_Organizes] != 1){
		opcua_ovStore_getOrganizes(context, &pNode, &refCount,
				TRUE, references, &result);
	}
	// HasSubtype
	if (uaReferences[INDEX_HasSubtype] != 1){
		opcua_ovStore_getHasSubtype(context, &pNode, &refCount,
				TRUE, references, &result);
	}
	// HasComponent
	if (uaReferences[INDEX_HasComponent] != 1){
		opcua_ovStore_getHasComponent(context, &pNode, &refCount,
				TRUE, references, &result);
	}
	// OvReferences
	// NOTIMPLEMENTED
	//  if (uaReferences[INDEX_OvReference] != 1){
	//		opcua_ovStore_getOvReferences(pNode, refCount,
	//  								TRUE, referencesNew, &result);
	//  }

	node->references = UA_calloc(refCount, sizeof(UA_NodeReferenceKind));
	if(!references && !node->references && refCount>0){
		UA_Array_delete(references, refCount, &UA_TYPES[UA_TYPES_REFERENCEDESCRIPTION]);
		UA_free(node);
		return UA_STATUSCODE_BADOUTOFMEMORY;
	}
	node->referencesSize = refCount;

	for (size_t i = 0; i < node->referencesSize; i++){
		UA_NodeId_copy(&(references[i].referenceTypeId), &(node->references[i].referenceTypeId));
		node->references[i].targetIds = UA_malloc(sizeof(UA_ExpandedNodeId));
		UA_ExpandedNodeId_copy(&(references[i].nodeId), node->references[i].targetIds);
		node->references[i].targetIdsSize = 1;
		node->references[i].isInverse = !references[i].isForward;
	}

	UA_Array_delete(references, refCount, &UA_TYPES[UA_TYPES_REFERENCEDESCRIPTION]);
	return result;
}

static void opcua_ovStore_deleteNodestore(void *context){
}

static void opcua_ovStore_deleteNode(void * context, UA_Node *node){
	if (node){
		UA_Node_deleteMembers(node);
	}
	UA_free(node);
}
static void opcua_ovStore_releaseNode(void *context, const UA_Node *node){
	opcua_ovStore_deleteNode(context, (UA_Node*)node);
}
static UA_StatusCode opcua_ovStore_insert(void *context, UA_Node *node, UA_NodeId *parrentNode){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

static void opcua_ovStore_iterate(void *context, void* visitorHandle, UA_NodestoreVisitor visitor){
}
static UA_Node * opcua_ovStore_newNode(void * context, UA_NodeClass nodeClass){
    return NULL;
}

static const UA_Node * opcua_ovStore_getNode(void * context, const UA_NodeId *nodeId){
	UA_Node 				*newNode = NULL;
	UA_StatusCode 			result = UA_STATUSCODE_GOOD;
	OV_PATH 				path;
	OV_INSTPTR_ov_object	pobj = NULL;
	OV_INSTPTR_ov_object	pobjtemp = NULL;
	OV_TICKET 				*pTicket = NULL;
	OV_VTBLPTR_ov_object	pVtblObj = NULL;
	OV_ACCESS				access;
	OV_ELEMENT				element;
	OV_ANY					value = {.value.vartype = OV_VT_VOID, .value.valueunion.val_string = NULL, .state=OV_ST_UNKNOWN, .time.secs = 0, .time.usecs = 0};
	OV_ANY					emptyAny = {.value.vartype = OV_VT_VOID, .value.valueunion.val_string = NULL, .state=OV_ST_UNKNOWN, .time.secs = 0, .time.usecs = 0};

	ov_memstack_lock();
	result = opcua_helpers_resolveNodeIdToPath(*nodeId, &path);
	if(result != UA_STATUSCODE_GOOD){
		ov_memstack_unlock();
		return NULL;
	}
	element = path.elements[path.size-1];
	ov_memstack_unlock();

	result = opcua_helpers_getVtblPointerAndCheckAccess(&element, pTicket, &pobj, &pVtblObj, &access);
	if(result != UA_STATUSCODE_GOOD){
		return NULL;
	}

	switch(element.elemtype){
	case OV_ET_OBJECT:
		if(Ov_GetParent(ov_instantiation, pobj) == pclass_ov_class){
			newNode = (UA_Node*)UA_calloc(1, sizeof(UA_ObjectTypeNode));
			newNode->nodeClass = UA_NODECLASS_OBJECTTYPE;
		} else if(Ov_GetParent(ov_instantiation, pobj) == pclass_ov_variable){
			newNode = (UA_Node*)UA_calloc(1, sizeof(UA_VariableTypeNode));
			newNode->nodeClass = UA_NODECLASS_VARIABLETYPE;
		} else if(Ov_GetParent(ov_instantiation, pobj) == pclass_ov_association){
			newNode = (UA_Node*)UA_calloc(1, sizeof(UA_ReferenceTypeNode));
			newNode->nodeClass = UA_NODECLASS_REFERENCETYPE;
		} else{
			newNode = (UA_Node*)UA_calloc(1, sizeof(UA_ObjectNode));
			newNode->nodeClass = UA_NODECLASS_OBJECT;
		}
		break;
	case OV_ET_VARIABLE:
	case OV_ET_MEMBER:
		newNode = (UA_Node*)UA_calloc(1, sizeof(UA_VariableNode));
		newNode->nodeClass = UA_NODECLASS_VARIABLE;
		break;
	case OV_ET_OPERATION:
		newNode = (UA_Node*)UA_calloc(1, sizeof(UA_MethodNode));
		newNode->nodeClass = UA_NODECLASS_METHOD;
		break;
	case OV_ET_CHILDLINK:
	case OV_ET_PARENTLINK:
	default:
		return NULL;
	}

	// Basic Attribute
	// BrowseName
	UA_QualifiedName_init(&newNode->browseName);
	newNode->browseName.name = UA_String_fromChars(pobj->v_identifier);
	newNode->browseName.namespaceIndex = opcua_ovStore_searchNamespaceIndex(context, pobj, FALSE);

	// Description
	UA_LocalizedText_init(&newNode->description);
	OV_STRING tempString = pVtblObj->m_getcomment(pobj, &element);
	newNode->description.locale = UA_String_fromChars("en");
	if(tempString){
		newNode->description.text = UA_String_fromChars(tempString);
	} else {
		newNode->description.text = UA_String_fromChars("");
	}

	// DisplayName
	UA_LocalizedText_init(&newNode->displayName);
	newNode->displayName.locale = UA_String_fromChars("en");
	newNode->displayName.text = UA_String_fromChars(pobj->v_identifier);

	// NodeId
	UA_NodeId_init(&newNode->nodeId);
	newNode->nodeId.identifierType = nodeId->identifierType;
	newNode->nodeId.namespaceIndex = newNode->browseName.namespaceIndex;

	switch(newNode->nodeId.identifierType){
	case UA_NODEIDTYPE_NUMERIC:
		newNode->nodeId.identifier.numeric = nodeId->identifier.numeric;
		break;
	case UA_NODEIDTYPE_STRING:
		UA_String_copy(&nodeId->identifier.string, &newNode->nodeId.identifier.string);
		break;
	case UA_NODEIDTYPE_GUID:
		newNode->nodeId.identifier.guid = nodeId->identifier.guid;
		break;
	case UA_NODEIDTYPE_BYTESTRING:
		newNode->nodeId.identifier.byteString = nodeId->identifier.byteString;
		break;
	}

	// WriteMask
	UA_UInt32 writeMask = 0;
	if(element.elemtype != OV_ET_VARIABLE){
		if(access & OV_AC_WRITE){
			writeMask |= (1<<2);	/*	BrowseName	*/
			writeMask |= (1<<6);	/*	DisplayName	*/
		}
		if(access & OV_AC_RENAMEABLE){
			writeMask |= (1<<14);	/*	NodeId	*/
		}
	}
	newNode->writeMask 	= writeMask;

	UA_Byte accessLevel = 0;

	switch(newNode->nodeClass){
	case UA_NODECLASS_OBJECT:
		// eventNotifier
		((UA_ObjectNode*)newNode)->eventNotifier = 0;
		break;
	case UA_NODECLASS_OBJECTTYPE:
		// isAbstract
		if(!(Ov_StaticPtrCast(ov_class, pobj)->v_classprops & OV_CP_INSTANTIABLE))
			((UA_ObjectTypeNode*)newNode)->isAbstract = UA_TRUE; /*	is instantiable is the negation of isAbstract	*/
		else
			((UA_ObjectTypeNode*)newNode)->isAbstract = UA_FALSE;
		break;
	case UA_NODECLASS_REFERENCETYPE:
		// isAbstract
		((UA_ReferenceTypeNode*)newNode)->isAbstract = UA_FALSE;
		// symmetric
		((UA_ReferenceTypeNode*)newNode)->symmetric = UA_FALSE;
		// inverseName
		//NOTIMPLEMENTED
		break;
	case UA_NODECLASS_DATATYPE:
		// isAbstract
		((UA_DataTypeNode*)newNode)->isAbstract = UA_FALSE;
		break;
	case UA_NODECLASS_VIEW:
		// eventNotifier
		((UA_ViewNode*)newNode)->eventNotifier = 0;
		// containNoLoops
		//NOTIMPLEMENTED
		break;
	case UA_NODECLASS_METHOD:
		// executable
		//NOTIMPLEMENTED
		break;
	case UA_NODECLASS_VARIABLE:
		// arrayDimensionsSize & arrayDimensions
		pobjtemp = Ov_StaticPtrCast(ov_object, element.elemunion.pvar);
		switch(element.elemtype) {
		case OV_ET_MEMBER:
		case OV_ET_VARIABLE:
			if(Ov_CanCastTo(ov_variable, pobjtemp)){
				switch((((OV_INSTPTR_ov_variable)pobjtemp)->v_vartype) & OV_VT_KSMASK){
				case OV_VT_ANY:
				case OV_VT_VOID:
					((UA_VariableNode*)newNode)->arrayDimensionsSize = 0;
					((UA_VariableNode*)newNode)->arrayDimensions = UA_Array_new(((UA_VariableNode*)newNode)->arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);	/*	scalar or one dimension	*/
					break;
				default:
					if(((OV_INSTPTR_ov_variable)pobjtemp)->v_veclen == 1){
						/*	scalar	*/
						((UA_VariableNode*)newNode)->arrayDimensionsSize = 0;
						((UA_VariableNode*)newNode)->arrayDimensions = UA_Array_new(((UA_VariableNode*)newNode)->arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);	/*	scalar or one dimension	*/
					} else {
						/*	vector	*/
						((UA_VariableNode*)newNode)->arrayDimensionsSize = 1;
						((UA_VariableNode*)newNode)->arrayDimensions = UA_Array_new( ((UA_VariableNode*)newNode)->arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);	/*	scalar or one dimension	*/
						if(!((UA_VariableNode*)newNode)->arrayDimensions){
							result = UA_STATUSCODE_BADOUTOFMEMORY;
							break;
						} else {
							result = UA_STATUSCODE_GOOD;
						}
						result = UA_Array_copy(&(((OV_INSTPTR_ov_variable)pobjtemp)->v_veclen), ((UA_VariableNode*)newNode)->arrayDimensionsSize, (void**)&(((UA_VariableNode*)newNode)->arrayDimensions), &UA_TYPES[UA_TYPES_INT32]);
						break;
					}
				}
			}
			break;
		case OV_ET_OBJECT:
			pobjtemp = Ov_StaticPtrCast(ov_object, element.pobj);
			if(Ov_CanCastTo(ov_variable, pobjtemp)){
				switch((((OV_INSTPTR_ov_variable)pobjtemp)->v_vartype) & OV_VT_KSMASK){
				case OV_VT_ANY:
				case OV_VT_VOID:
					((UA_VariableNode*)newNode)->arrayDimensionsSize = 0;
					((UA_VariableNode*)newNode)->arrayDimensions = UA_Array_new(((UA_VariableNode*)newNode)->arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);	/*	scalar or one dimension	*/
					break;
				default:
					if(((OV_INSTPTR_ov_variable)pobjtemp)->v_veclen == 1){
						/*	scalar	*/
						((UA_VariableNode*)newNode)->arrayDimensionsSize = 0;
						((UA_VariableNode*)newNode)->arrayDimensions = UA_Array_new(((UA_VariableNode*)newNode)->arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);	/*	scalar or one dimension	*/
					} else {
						/*	vector	*/
						((UA_VariableNode*)newNode)->arrayDimensionsSize = 1;
						((UA_VariableNode*)newNode)->arrayDimensions = UA_Array_new(((UA_VariableNode*)newNode)->arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);	/*	scalar or one dimension	*/
						if(!((UA_VariableTypeNode*)newNode)->arrayDimensions){
							result = UA_STATUSCODE_BADOUTOFMEMORY;
							break;
						} else {
							result = UA_STATUSCODE_GOOD;
						}
						result = UA_Array_copy(&(((OV_INSTPTR_ov_variable)pobjtemp)->v_veclen), ((UA_VariableNode*)newNode)->arrayDimensionsSize, (void**)&(((UA_VariableNode*)newNode)->arrayDimensions), &UA_TYPES[UA_TYPES_INT32]);
						break;
					}
				}
			}else {
				result = opcua_helpers_ovResultToUaStatusCode(OV_ERR_BADTYPE);
				break;
			}
			break;
		default:
			result = opcua_helpers_ovResultToUaStatusCode(OV_ERR_BADPATH);
			break;
		}
		// valuerank
		pobjtemp = element.pobj;
		Ov_GetVTablePtr(ov_object, pVtblObj, pobjtemp);
		if((!pVtblObj) || (ov_activitylock)){
			pVtblObj = pclass_ov_object->v_pvtable;
		}
		if (element.elemtype == OV_ET_VARIABLE){
			pobjtemp = Ov_StaticPtrCast(ov_object, element.elemunion.pvar);
		}
		if(Ov_GetParent(ov_instantiation, pobjtemp) == pclass_ov_variable){
			switch((((OV_INSTPTR_ov_variable)pobj)->v_vartype) & OV_VT_KSMASK){
			case OV_VT_ANY:
			case OV_VT_VOID:
				((UA_VariableNode*)newNode)->valueRank = -3;	/*	scalar or one dimension	*/
				result = UA_STATUSCODE_GOOD;
				break;
			default:
				if((((OV_INSTPTR_ov_variable)pobj)->v_vartype) & OV_VT_ISVECTOR){
					((UA_VariableNode*)newNode)->valueRank = 1;	/*	one dimension	*/
					result = UA_STATUSCODE_GOOD;
				} else {
					((UA_VariableNode*)newNode)->valueRank = -1;	/*	scalar	*/
					result = UA_STATUSCODE_GOOD;
				}
				break;
			}
		} else {
			result = opcua_helpers_ovResultToUaStatusCode(OV_ERR_BADTYPE);
			break;
		}
		// value & dataType
		pobjtemp = element.pobj;
		Ov_GetVTablePtr(ov_object, pVtblObj, pobjtemp);
		if((!pVtblObj) || (ov_activitylock)){
			pVtblObj = pclass_ov_object->v_pvtable;
		}
		switch(element.elemtype){
		case OV_ET_MEMBER:
		case OV_ET_VARIABLE:
			ov_memstack_lock();
			result = opcua_helpers_ovResultToUaStatusCode((pVtblObj->m_getvar)(pobjtemp, &element, &value));
			value.value.vartype &= OV_VT_KSMASK;
			if(result == UA_STATUSCODE_GOOD){
				result = opcua_helpers_ovAnyToUAVariant(&value, &(((UA_VariableNode*)newNode)->value.data.value.value));
				((UA_VariableNode*)newNode)->valueSource = UA_VALUESOURCE_DATA;
				((UA_VariableNode*)newNode)->dataType = opcua_helpers_ovVarTypeToNodeId(value.value.vartype);
				value = emptyAny;
			}
			ov_memstack_unlock();
			break;
		default:
			result = opcua_helpers_ovResultToUaStatusCode(OV_ERR_BADPATH);
			break;
		}
		// accessLevel
		accessLevel = 0;
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
		break;
	case UA_NODECLASS_VARIABLETYPE:
		// arrayDimensionsSize & arrayDimensions
		switch(element.elemtype) {
		case OV_ET_MEMBER:
		case OV_ET_VARIABLE:
			pobjtemp = Ov_StaticPtrCast(ov_object, element.elemunion.pvar);
			if(Ov_CanCastTo(ov_variable, pobjtemp)){
				switch((((OV_INSTPTR_ov_variable)pobjtemp)->v_vartype) & OV_VT_KSMASK){
				case OV_VT_ANY:
				case OV_VT_VOID:
					((UA_VariableNode*)newNode)->arrayDimensionsSize = 0;
					((UA_VariableNode*)newNode)->arrayDimensions = UA_Array_new(((UA_VariableNode*)newNode)->arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);	/*	scalar or one dimension	*/
					break;
				default:
					if(((OV_INSTPTR_ov_variable)pobj)->v_veclen == 1){
						/*	scalar	*/
						((UA_VariableNode*)newNode)->arrayDimensionsSize = 0;
						((UA_VariableNode*)newNode)->arrayDimensions = UA_Array_new(((UA_VariableNode*)newNode)->arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);	/*	scalar or one dimension	*/
					} else {
						/*	vector	*/
						((UA_VariableNode*)newNode)->arrayDimensionsSize = 1;
						((UA_VariableNode*)newNode)->arrayDimensions = UA_Array_new( ((UA_VariableNode*)newNode)->arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);	/*	scalar or one dimension	*/
						if(!((UA_VariableNode*)newNode)->arrayDimensions){
							result = UA_STATUSCODE_BADOUTOFMEMORY;
							break;
						} else {
							result = UA_STATUSCODE_GOOD;
						}
						result = UA_Array_copy(&(((OV_INSTPTR_ov_variable)pobjtemp)->v_veclen), ((UA_VariableNode*)newNode)->arrayDimensionsSize, (void**)&(((UA_VariableNode*)newNode)->arrayDimensions), &UA_TYPES[UA_TYPES_INT32]);
						break;
					}
				}
			}
			break;
		case OV_ET_OBJECT:
			pobjtemp = Ov_StaticPtrCast(ov_object, element.pobj);
			if(Ov_CanCastTo(ov_variable, pobjtemp)){
				switch((((OV_INSTPTR_ov_variable)pobj)->v_vartype) & OV_VT_KSMASK){
				case OV_VT_ANY:
				case OV_VT_VOID:
					((UA_VariableNode*)newNode)->arrayDimensionsSize = 0;
					((UA_VariableNode*)newNode)->arrayDimensions = UA_Array_new(((UA_VariableNode*)newNode)->arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);	/*	scalar or one dimension	*/
					break;
				default:
					if(((OV_INSTPTR_ov_variable)pobj)->v_veclen == 1){
						/*	scalar	*/
						((UA_VariableNode*)newNode)->arrayDimensionsSize = 0;
						((UA_VariableNode*)newNode)->arrayDimensions = UA_Array_new(((UA_VariableNode*)newNode)->arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);	/*	scalar or one dimension	*/
					} else {
						/*	vector	*/
						((UA_VariableNode*)newNode)->arrayDimensionsSize = 1;
						((UA_VariableNode*)newNode)->arrayDimensions = UA_Array_new(((UA_VariableNode*)newNode)->arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);	/*	scalar or one dimension	*/
						if(!((UA_VariableTypeNode*)newNode)->arrayDimensions){
							result = UA_STATUSCODE_BADOUTOFMEMORY;
							break;
						} else {
							result = UA_STATUSCODE_GOOD;
						}
						result = UA_Array_copy(&(((OV_INSTPTR_ov_variable)pobjtemp)->v_veclen), ((UA_VariableNode*)newNode)->arrayDimensionsSize, (void**)&(((UA_VariableNode*)newNode)->arrayDimensions), &UA_TYPES[UA_TYPES_INT32]);
						break;
					}
				}
			} else {
				result = opcua_helpers_ovResultToUaStatusCode(OV_ERR_BADTYPE);
				break;
			}
			break;
			default:
			result = opcua_helpers_ovResultToUaStatusCode(OV_ERR_BADPATH);
			break;
		}
		// valueRank
		pobjtemp = element.pobj;
		Ov_GetVTablePtr(ov_object, pVtblObj, pobjtemp);
		if((!pVtblObj) || (ov_activitylock)){
			pVtblObj = pclass_ov_object->v_pvtable;
		}
		switch((((OV_INSTPTR_ov_variable)pobjtemp)->v_vartype) & OV_VT_KSMASK){
		case OV_VT_ANY:
		case OV_VT_VOID:
			((UA_VariableTypeNode*)newNode)->valueRank = -3;	/*	scalar or one dimension	*/
			result = UA_STATUSCODE_GOOD;
			break;
		default:
			if((((OV_INSTPTR_ov_variable)pobjtemp)->v_vartype) & OV_VT_ISVECTOR){
				((UA_VariableTypeNode*)newNode)->valueRank = 1;	/*	one dimension	*/
				result = UA_STATUSCODE_GOOD;
			} else {
				((UA_VariableTypeNode*)newNode)->valueRank = -1;	/*	scalar	*/
				result = UA_STATUSCODE_GOOD;
			}
			break;
		}
		// value
		pobjtemp = element.pobj;
		Ov_GetVTablePtr(ov_object, pVtblObj, pobjtemp);
		if((!pVtblObj) || (ov_activitylock)){
			pVtblObj = pclass_ov_object->v_pvtable;
		}

		result = opcua_helpers_ovAnyToUAVariant(ov_variable_initialvalue_get(Ov_StaticPtrCast(ov_variable,pobjtemp)), &(((UA_VariableTypeNode*)newNode)->value.data.value.value));
		((UA_VariableTypeNode*)newNode)->valueSource = UA_VALUESOURCE_DATA;
		// isAbstract
		((UA_VariableTypeNode*)newNode)->isAbstract = UA_FALSE;
		// dataType
		pobjtemp = element.pobj;
		Ov_GetVTablePtr(ov_object, pVtblObj, pobjtemp);
		if((!pVtblObj) || (ov_activitylock)){
			pVtblObj = pclass_ov_object->v_pvtable;
		}
		((UA_VariableTypeNode*)newNode)->dataType = opcua_helpers_ovVarTypeToNodeId(ov_variable_vartype_get(Ov_StaticPtrCast(ov_variable,pobjtemp)));
		break;
	default:
		break;
	}

	opcua_ovStore_addReference(context, newNode);

	return newNode;
}
static UA_StatusCode opcua_ovStore_getCopyNode(void *context, const UA_NodeId *nodeId, UA_Node ** nodeOut){
	UA_Node* node = (UA_Node*) opcua_ovStore_getNode(context, nodeId);
	if(node == NULL)
		return UA_STATUSCODE_BADNODEIDUNKNOWN;
	*nodeOut = node;
	return UA_STATUSCODE_GOOD;
}
static UA_StatusCode opcua_ovStore_removeNode(void *context, const UA_NodeId *nodeId){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}
static UA_StatusCode opcua_ovStore_insertNode(void *context, UA_Node *node, UA_NodeId *parrentNode){
	return opcua_ovStore_insert(context, node, parrentNode);
}
static UA_StatusCode opcua_ovStore_replaceNode(void *context, UA_Node *node){
	OV_PATH					path;
	OV_INSTPTR_ov_object	pobj		=	NULL;
	OV_VTBLPTR_ov_object	pVtblObj	=	NULL;
	OV_ACCESS				access		=	OV_AC_NONE;
	OV_TICKET				*pTicket	=	NULL;
	OV_ELEMENT				element;
	OV_ANY					value		=	{.value.vartype = OV_VT_VOID, .value.valueunion = {0}, .state=OV_ST_UNKNOWN, .time = {0, 0}};
	OV_TIME_SPAN			tempTimeSpan	=	{.secs = 0, .usecs = 0};
	OV_TIME_SPAN_VEC		tempTimeSpanVec	=	{.veclen = 0, .value = NULL};
	pTicket = ksbase_NoneAuth->v_ticket.vtbl->createticket(NULL, OV_TT_NONE);
	UA_StatusCode			result = UA_STATUSCODE_GOOD;

	ov_memstack_lock();
	result = opcua_helpers_resolveNodeIdToPath(node->nodeId, &path);
	if(result != UA_STATUSCODE_GOOD){
		ov_memstack_unlock();
		return UA_STATUSCODE_BADNODEIDEXISTS;
	}
	element = path.elements[path.size-1];

	// NodeId
	// NOTIMPLEMENTED

	// Value for Variable & VariableType
	switch(node->nodeClass){
		case UA_NODECLASS_UNSPECIFIED:
			break;
		case UA_NODECLASS_OBJECT:{
			break;
		}
		case UA_NODECLASS_VARIABLE:{
			pobj = element.pobj;
			Ov_GetVTablePtr(ov_object, pVtblObj, pobj);
			if((!pVtblObj) || (ov_activitylock)){
				pVtblObj = pclass_ov_object->v_pvtable;
			}
			access = (pVtblObj)->m_getaccess(path.elements[path.size-1].pobj, &element, pTicket);
			if(access & OV_AC_WRITE){
				switch(element.elemtype) {
				case OV_ET_MEMBER:
				case OV_ET_VARIABLE:
					if(((UA_VariableNode*)node)->value.data.value.hasValue == UA_TRUE){
						result = opcua_helpers_UAVariantToOVAny(&(((UA_VariableNode*)node)->value.data.value.value), &value);
						if(result != UA_STATUSCODE_GOOD){
							break;
						}
						if(((UA_VariableNode*)node)->value.data.value.hasSourceTimestamp == UA_TRUE){
							value.value.vartype |= OV_VT_HAS_TIMESTAMP;
							value.time = ov_1601nsTimeToOvTime(((UA_VariableNode*)node)->value.data.value.sourceTimestamp);
						}
						result = opcua_helpers_ovResultToUaStatusCode((pVtblObj->m_setvar)(pobj, &element, &value));
						if(result == UA_STATUSCODE_BADTYPEMISMATCH
								&& ((value.value.vartype == OV_VT_DOUBLE
											&& (element.pvar->v_vartype & OV_VT_KSMASK) == OV_VT_TIME_SPAN)
									|| (value.value.vartype == OV_VT_DOUBLE_VEC
											&& (element.pvar->v_vartype & OV_VT_KSMASK) == OV_VT_TIME_SPAN_VEC))){
							/*	timeSpans are doubles in UA	*/
							if(!(value.value.vartype & OV_VT_ISVECTOR)){
								Ov_DoubleToTimeSpan(value.value.valueunion.val_double * 1000.0, tempTimeSpan);
								value.value.vartype = OV_VT_TIME_SPAN;
								value.value.valueunion.val_time_span = tempTimeSpan;
							} else {
								OV_UINT iTwo = 0;
								tempTimeSpanVec.value = ov_memstack_alloc(value.value.valueunion.val_double_vec.veclen * sizeof(OV_TIME_SPAN));
								if(!tempTimeSpanVec.value){
									result = UA_STATUSCODE_BADOUTOFMEMORY;
									break;
								}
								tempTimeSpanVec.veclen = value.value.valueunion.val_double_vec.veclen;
								for(iTwo = 0; iTwo < tempTimeSpanVec.veclen; iTwo++){
									Ov_DoubleToTimeSpan(value.value.valueunion.val_double_vec.value[iTwo] * 1000.0, tempTimeSpanVec.value[iTwo]);
								}
								value.value.vartype = OV_VT_TIME_SPAN_VEC;
								value.value.valueunion.val_time_span_vec = tempTimeSpanVec;
							}
							if(((UA_VariableNode*)node)->value.data.value.hasSourceTimestamp == UA_TRUE){
								value.value.vartype |= OV_VT_HAS_TIMESTAMP;
								value.time = ov_1601nsTimeToOvTime(((UA_VariableNode*)node)->value.data.value.sourceTimestamp);
							}
							result = opcua_helpers_ovResultToUaStatusCode((pVtblObj->m_setvar)(pobj, &element, &value));
						}
					} else {
						result = UA_STATUSCODE_BADINVALIDARGUMENT;
						break;
					}
					break;
				default:
					result = opcua_helpers_ovResultToUaStatusCode(OV_ERR_BADPATH);
					break;
				}
			} else {
				result = UA_STATUSCODE_BADNOTWRITABLE;
			}
			break;
		}
		case UA_NODECLASS_METHOD:{
			break;
		}
		case UA_NODECLASS_OBJECTTYPE:{
			break;
		}
		case UA_NODECLASS_VARIABLETYPE:{
			pobj = element.pobj;
			Ov_GetVTablePtr(ov_object, pVtblObj, pobj);
			if((!pVtblObj) || (ov_activitylock)){
				pVtblObj = pclass_ov_object->v_pvtable;
			}
			access = (pVtblObj)->m_getaccess(element.pobj, &element, pTicket);
			if(access & OV_AC_WRITE){
				switch(element.elemtype) {
				case OV_ET_MEMBER:
				case OV_ET_VARIABLE:
					if(((UA_VariableNode*)node)->value.data.value.hasValue == UA_TRUE){
						result = opcua_helpers_UAVariantToOVAny(&(((UA_VariableNode*)node)->value.data.value.value), &value);
						if(result != UA_STATUSCODE_GOOD){
							break;
						}
						if(((UA_VariableNode*)node)->value.data.value.hasSourceTimestamp == UA_TRUE){
							value.value.vartype |= OV_VT_HAS_TIMESTAMP;
							value.time = ov_1601nsTimeToOvTime(((UA_VariableNode*)node)->value.data.value.sourceTimestamp);
						}
						result = opcua_helpers_ovResultToUaStatusCode((pVtblObj->m_setvar)(pobj, &element, &value));
						if(result == UA_STATUSCODE_BADTYPEMISMATCH
								&& ((value.value.vartype == OV_VT_DOUBLE
											&& (element.pvar->v_vartype & OV_VT_KSMASK) == OV_VT_TIME_SPAN)
									|| (value.value.vartype == OV_VT_DOUBLE_VEC
											&& (element.pvar->v_vartype & OV_VT_KSMASK) == OV_VT_TIME_SPAN_VEC))){
							/*	timeSpans are doubles in UA	*/
							if(!(value.value.vartype & OV_VT_ISVECTOR)){
								Ov_DoubleToTimeSpan(value.value.valueunion.val_double * 1000.0, tempTimeSpan);
								value.value.vartype = OV_VT_TIME_SPAN;
								value.value.valueunion.val_time_span = tempTimeSpan;
							} else {
								OV_UINT iTwo = 0;
								tempTimeSpanVec.value = ov_memstack_alloc(value.value.valueunion.val_double_vec.veclen * sizeof(OV_TIME_SPAN));
								if(!tempTimeSpanVec.value){
									result = UA_STATUSCODE_BADOUTOFMEMORY;
									break;
								}
								tempTimeSpanVec.veclen = value.value.valueunion.val_double_vec.veclen;
								for(iTwo = 0; iTwo < tempTimeSpanVec.veclen; iTwo++){
									Ov_DoubleToTimeSpan(value.value.valueunion.val_double_vec.value[iTwo] * 1000.0, tempTimeSpanVec.value[iTwo]);
								}
								value.value.vartype = OV_VT_TIME_SPAN_VEC;
								value.value.valueunion.val_time_span_vec = tempTimeSpanVec;
							}
							if(((UA_VariableNode*)node)->value.data.value.hasSourceTimestamp == UA_TRUE){
								value.value.vartype |= OV_VT_HAS_TIMESTAMP;
								value.time = ov_1601nsTimeToOvTime(((UA_VariableNode*)node)->value.data.value.sourceTimestamp);
							}
							result = opcua_helpers_ovResultToUaStatusCode((pVtblObj->m_setvar)(pobj, &element, &value));
						}
					} else {
						result = UA_STATUSCODE_BADINVALIDARGUMENT;
						break;
					}
					break;
				default:
					result = opcua_helpers_ovResultToUaStatusCode(OV_ERR_BADPATH);
					break;
				}
			} else {
				result = UA_STATUSCODE_BADNOTWRITABLE;
			}
			break;
		}
		case UA_NODECLASS_REFERENCETYPE:{
			break;
		}
		case UA_NODECLASS_DATATYPE:{
			break;
		}
		case UA_NODECLASS_VIEW:{
			break;
		}
		default:
			break;
		}

	ov_memstack_unlock();
	opcua_ovStore_releaseNode(context, node);
	return UA_STATUSCODE_GOOD;
}

UA_Nodestore*
opcua_ovStore_new(OV_INSTPTR_opcua_ovInterface context) {
	UA_Nodestore* nsi = UA_malloc(sizeof(UA_Nodestore));
	if(nsi == NULL)
		return NULL;
    nsi->context =        	context;
    nsi->deleteNodestore =  opcua_ovStore_deleteNodestore;
    nsi->newNode =       	opcua_ovStore_newNode;
    nsi->deleteNode =    	opcua_ovStore_deleteNode;
    nsi->insertNode =       opcua_ovStore_insertNode;
    nsi->getNode =          opcua_ovStore_getNode;
    nsi->getNodeCopy =      opcua_ovStore_getCopyNode;
    nsi->replaceNode =      opcua_ovStore_replaceNode;
    nsi->removeNode =       opcua_ovStore_removeNode;
    nsi->iterate =          opcua_ovStore_iterate;
    nsi->releaseNode =      opcua_ovStore_releaseNode;
    return nsi;
}
void
opcua_ovStore_delete(UA_Nodestore * nodestoreInterface){
	nodestoreInterface->context = NULL;
	UA_free(nodestoreInterface);
}
