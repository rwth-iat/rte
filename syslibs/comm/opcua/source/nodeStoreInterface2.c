/*
 * nodeStoreInterface.c
 *
 *  Created on: 06.12.2016
 *      Author: ubuntu
 */

#ifndef OV_COMPILE_LIBRARY_opcua
#define OV_COMPILE_LIBRARY_opcua
#endif

#include "libov/ov_macros.h"
#include "ksbase.h"
#include "opcua.h"
#include "opcua_helpers.h"
#include "NoneTicketAuthenticator.h"
#include "libov/ov_path.h"
#include "libov/ov_memstack.h"
#include "ks_logfile.h"

extern OV_INSTPTR_opcua_uaServer opcua_pUaServer;


/*	only use numeric nodeIDs	*/
UA_StatusCode opcua_ns0_fakeReferenceDescription2(
		UA_NodeId refNodeId, OV_STRING browsename, UA_UInt16 browsenameNsIndex, OV_STRING displayname, UA_NodeClass nodeClass, UA_NodeId type,
			UA_UInt16 referenceTypeNamespaceIndex, UA_Int32 referenceType, UA_Boolean isForward, UA_UInt32 resultMask, UA_ReferenceDescription* dst){
	UA_StatusCode			result			=	UA_STATUSCODE_GOOD;
	if(!dst){
		return UA_STATUSCODE_BADINVALIDARGUMENT;
	}

	dst->nodeId.nodeId = refNodeId;

	if(resultMask & (1<<3)){
		dst->browseName.name = UA_String_fromChars(browsename);
		dst->browseName.namespaceIndex = browsenameNsIndex;
	}
	if(resultMask & (1<<4)){
		dst->displayName.text = UA_String_fromChars(displayname);
		dst->displayName.locale = UA_String_fromChars("en");
	}
	if(resultMask & (1<<1)){
		dst->isForward = isForward;
	}
	if(resultMask & (1<<2)){
		dst->nodeClass = nodeClass;
	}
	if(resultMask & (1<<0)){	// in ov-namespace beware that numerical ids may change when the server restarts (but associations can change between /acplt/... and /Libraries/..., too)
		dst->referenceTypeId.namespaceIndex = referenceTypeNamespaceIndex;
		dst->referenceTypeId.identifierType = UA_NODEIDTYPE_NUMERIC;
		dst->referenceTypeId.identifier.numeric = referenceType;
	}
	if(resultMask & (1<<5)){	// TODO fixme	This is the type-node: using 0|58 (baseObjectType) for all variables
		if(dst->nodeClass == UA_NODECLASS_OBJECT || dst->nodeClass == UA_NODECLASS_VARIABLE){
			dst->typeDefinition.nodeId = type;
		} else {
			dst->typeDefinition.nodeId = UA_NODEID_NULL;
		}
	}
	return result;
}

UA_StatusCode opcua_nsOv_fillReferenceDescription2(
		OV_ELEMENT* pElement, UA_UInt16 referenceTypeNamespaceIndex, UA_Int32 referenceType, UA_Boolean isForward, UA_UInt32 resultMask, UA_ReferenceDescription* dst){
	OV_INSTPTR_ov_object	pObject			=	NULL;
	UA_StatusCode			result			=	UA_STATUSCODE_GOOD;
	OV_STRING				path			=	NULL;
	OV_STRING				varPath			=	NULL;
	OV_UINT					varPathLen		=	0;
	if(!dst){
		return UA_STATUSCODE_BADINVALIDARGUMENT;
	}
	dst->nodeId.nodeId.identifierType = UA_NODEIDTYPE_STRING;
	dst->nodeId.nodeId.namespaceIndex = opcua_pUaServer->v_NameSpaceIndex;
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

	if(resultMask & (1<<3)){
		if(pElement->elemtype == OV_ET_OBJECT){
			dst->browseName.name = UA_String_fromChars(pElement->pobj->v_identifier);
		} else if(pElement->elemtype == OV_ET_VARIABLE){
			dst->browseName.name = UA_String_fromChars(pElement->elemunion.pvar->v_identifier);
		}
		dst->browseName.namespaceIndex = opcua_pUaServer->v_NameSpaceIndex;
	}
	if(resultMask & (1<<4)){
		if(pElement->elemtype == OV_ET_OBJECT){
			dst->displayName.text = UA_String_fromChars(pElement->pobj->v_identifier);
		} else if(pElement->elemtype == OV_ET_VARIABLE){
			dst->displayName.text = UA_String_fromChars(pElement->elemunion.pvar->v_identifier);
		}
	}
	if(resultMask & (1<<1)){
		dst->isForward = isForward;
	}
	if(resultMask & (1<<2)){
		dst->nodeClass = opcua_nsOv_getNodeClassAndAccess(pElement, NULL);
	}
	if(resultMask & (1<<0)){	// in ov-namespace beware that numerical ids may change when the server restarts (but associations can change between /acplt/... and /Libraries/..., too)
		dst->referenceTypeId.namespaceIndex = referenceTypeNamespaceIndex;
		dst->referenceTypeId.identifierType = UA_NODEIDTYPE_NUMERIC;
		dst->referenceTypeId.identifier.numeric = referenceType;
	}
	if(resultMask & (1<<5)){	// TODO fixme	This is the type-node: using 0|58 (baseObjectType) for all variables
		if(dst->nodeClass == UA_NODECLASS_OBJECT){
			dst->typeDefinition.nodeId.namespaceIndex = opcua_pUaServer->v_NameSpaceIndex;
			dst->typeDefinition.nodeId.identifierType = UA_NODEIDTYPE_NUMERIC;
			dst->typeDefinition.nodeId.identifier.numeric = pElement->pobj->v_idL;
		} else if(dst->nodeClass == UA_NODECLASS_VARIABLE){
			dst->typeDefinition.nodeId.namespaceIndex = 0;
			dst->typeDefinition.nodeId.identifierType = UA_NODEIDTYPE_NUMERIC;
			dst->typeDefinition.nodeId.identifier.numeric = 58;
		} else {
			dst->typeDefinition.nodeId = UA_NODEID_NULL;
		}
	}
	ov_memstack_unlock();
	return result;
}
/******************************************************************
 * helper for Organizes
 *****************************************************************/

UA_Int32 getReferenceDescriptions_Organizes2(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, OV_UINT* refCount,
		OV_BOOL fillDescription, UA_UInt32 resultMask, UA_ReferenceDescription* dst,
		UA_StatusCode* statusCode){
	OV_ELEMENT	childPart, parentElement;
	OV_BOOL		maskMatch;
	OV_ACCESS	access;
	//	this is mapped to ov/containment
	if((browseDescription->browseDirection == UA_BROWSEDIRECTION_FORWARD)
			|| (browseDescription->browseDirection == UA_BROWSEDIRECTION_BOTH)){
		OV_INSTPTR_opcua_methodNode pMethod = Ov_DynamicPtrCast(opcua_methodNode, pNode->pobj);
		// children
		childPart.elemtype = OV_ET_NONE;
		childPart.pobj = NULL;
		ov_element_getnextchild(pNode, &childPart);
		while(childPart.elemtype!=OV_ET_NONE){
			maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&childPart, browseDescription->nodeClassMask, &access);
			if(maskMatch && (access & OV_AC_READ)){
				if(fillDescription){
					*statusCode = opcua_nsOv_fillReferenceDescription2(&childPart, 0, UA_NODEID_Organizes, UA_TRUE, resultMask, &(dst[*refCount]));
				}
				(*refCount)++;
			}
			ov_element_getnextchild(pNode, &childPart);
		}
		// parts
		childPart.elemtype = OV_ET_NONE;
		childPart.pobj = NULL;
		ov_element_getnextpart(pNode, &childPart, OV_ET_OBJECT | OV_ET_VARIABLE);
		while(childPart.elemtype!=OV_ET_NONE){
			maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&childPart, browseDescription->nodeClassMask, &access);
			if(maskMatch && (access & OV_AC_READ)){
				if(childPart.elemtype == OV_ET_OBJECT || childPart.elemtype == OV_ET_VARIABLE || childPart.elemtype == OV_ET_MEMBER){
					/*	exclude the Arguments-parts of method nodes	*/
					if(!pMethod || childPart.elemtype != OV_ET_OBJECT
							|| (ov_string_compare(childPart.pobj->v_identifier, "InputArguments") != OV_STRCMP_EQUAL
									&& ov_string_compare(childPart.pobj->v_identifier, "OutputArguments") != OV_STRCMP_EQUAL)){
						if(fillDescription){
							*statusCode = opcua_nsOv_fillReferenceDescription2(&childPart, 0, UA_NODEID_Organizes, UA_TRUE, resultMask, &(dst[*refCount]));
						}
						(*refCount)++;
					}
				}
			}
			ov_element_getnextpart(pNode, &childPart, OV_ET_OBJECT | OV_ET_VARIABLE);
		}
	}
	if((browseDescription->browseDirection == UA_BROWSEDIRECTION_INVERSE)
			|| (browseDescription->browseDirection == UA_BROWSEDIRECTION_BOTH)){
		if(pNode->elemtype == OV_ET_OBJECT){
			if(pNode->pobj->v_idL || pNode->pobj->v_idH){	// don't do it for ov root object ("/")
				parentElement.elemtype = OV_ET_OBJECT;
				parentElement.pobj = (pNode->pobj->v_pouterobject ? pNode->pobj->v_pouterobject : Ov_StaticPtrCast(ov_object, Ov_GetParent(ov_containment, pNode->pobj)));
				maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&parentElement, browseDescription->nodeClassMask, &access);
				if(maskMatch && (access & OV_AC_READ)){
					if(fillDescription){
						*statusCode = opcua_nsOv_fillReferenceDescription2(&parentElement, 0, UA_NODEID_Organizes, UA_FALSE, resultMask, &(dst[*refCount]));
					}
					(*refCount)++;
				}
			} else {	//	ov root-node --> bridge node in ns0
				if(fillDescription){
					*statusCode = UA_STATUSCODE_BADNOTIMPLEMENTED;
				}
				(*refCount)++;
			}
		} else {
			parentElement.elemtype = OV_ET_OBJECT;
			parentElement.pobj = pNode->pobj;
			maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&parentElement, browseDescription->nodeClassMask, &access);
			if(maskMatch && (access & OV_AC_READ)){
				if(fillDescription){
					*statusCode = opcua_nsOv_fillReferenceDescription2(&parentElement, 0, UA_NODEID_Organizes, UA_FALSE, resultMask, &(dst)[*refCount]);
				}
				(*refCount)++;
			}
		}

	}
	return UA_STATUSCODE_GOOD;
}

UA_Int32 countReferenceDescriptions_Organizes2(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, OV_UINT* refCount){
	return getReferenceDescriptions_Organizes2(browseDescription, pNode,refCount, FALSE, 0, NULL, NULL);
}

UA_Int32 fillReferenceDescriptions_Organizes2(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, OV_UINT* refCount,
		UA_UInt32 resultMask, UA_ReferenceDescription* dst, UA_StatusCode* statusCode){
	return getReferenceDescriptions_Organizes2(browseDescription, pNode, refCount, TRUE, resultMask, dst, statusCode);
}

/******************************************************************
 * helper for HasTypeDefinition
 *****************************************************************/

UA_Int32 getReferenceDescriptions_HasTypeDefinition2(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, OV_UINT* refCount,
		OV_BOOL fillDescription, UA_UInt32 resultMask, UA_ReferenceDescription* dst,
		UA_StatusCode* statusCode){
	OV_BOOL					maskMatch;
	OV_ACCESS				access		=	OV_AC_NONE;
	OV_INSTPTR_ov_object	pChildObject	=	NULL;
	OV_ELEMENT				parentTypeDefinition, childTypeDefinition;
	//	maps to ov/instantiation
	if((browseDescription->browseDirection == UA_BROWSEDIRECTION_FORWARD)
			|| (browseDescription->browseDirection == UA_BROWSEDIRECTION_BOTH)){
		//	source is object target is type --> this direction is an instantiation getparent for objects,
		//		and analog for links and variables
		if(pNode->elemtype == OV_ET_OBJECT || pNode->elemtype == OV_ET_VARIABLE
				|| pNode->elemtype == OV_ET_MEMBER || pNode->elemtype == OV_ET_CHILDLINK || pNode->elemtype == OV_ET_PARENTLINK){
			maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(pNode, browseDescription->nodeClassMask, &access);
			if(maskMatch && (access & OV_AC_READ)){
				if(fillDescription){
					if(pNode->elemtype == OV_ET_OBJECT){
						if(Ov_GetClassPtr(pNode->pobj) != pclass_opcua_arguments){
							parentTypeDefinition.elemtype = OV_ET_OBJECT;
							parentTypeDefinition.pobj = Ov_PtrUpCast(ov_object, Ov_GetParent(ov_instantiation, pNode->pobj));
							*statusCode = opcua_nsOv_fillReferenceDescription2(&parentTypeDefinition, 0, UA_NODEID_HasTypeDefinition, UA_TRUE, resultMask, &(dst[*refCount]));
						} else {
							*statusCode = opcua_ns0_fakeReferenceDescription2(UA_NODEID_NUMERIC(0, 62), "BaseVariableType", 0, "BaseVariableType", UA_NODECLASS_VARIABLETYPE, UA_NODEID_NULL,
									0, UA_NODEID_HasTypeDefinition, UA_TRUE, resultMask, &(dst[*refCount]));
						}
					} else if (pNode->elemtype == OV_ET_VARIABLE || pNode->elemtype == OV_ET_MEMBER){
						parentTypeDefinition.elemtype = OV_ET_OBJECT;
						parentTypeDefinition.pobj = Ov_PtrUpCast(ov_object, pNode->elemunion.pvar);
						*statusCode = opcua_nsOv_fillReferenceDescription2(&parentTypeDefinition, 0, UA_NODEID_HasTypeDefinition, UA_TRUE, resultMask, &(dst[*refCount]));
					}else if (pNode->elemtype == OV_ET_PARENTLINK || pNode->elemtype == OV_ET_CHILDLINK){
						parentTypeDefinition.elemtype = OV_ET_OBJECT;
						parentTypeDefinition.pobj = Ov_PtrUpCast(ov_object, pNode->elemunion.passoc);
						*statusCode = opcua_nsOv_fillReferenceDescription2(&parentTypeDefinition, 0, UA_NODEID_HasTypeDefinition, UA_TRUE, resultMask, &(dst[*refCount]));
					}
				}
				(*refCount)++;
			}
		}
	}
	if((browseDescription->browseDirection == UA_BROWSEDIRECTION_INVERSE)
			|| (browseDescription->browseDirection == UA_BROWSEDIRECTION_BOTH)){
		//	this is an instantiation getchild for objects (classes), for variables and links this direction delivers nothing
		if(pNode->elemtype == OV_ET_OBJECT && Ov_GetClassPtr(pNode->pobj) == pclass_ov_class){
			Ov_ForEachChild(ov_instantiation, Ov_StaticPtrCast(ov_class, pNode->pobj), pChildObject){
				childTypeDefinition.elemtype = OV_ET_OBJECT;
				childTypeDefinition.pobj = pChildObject;
				maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&childTypeDefinition, browseDescription->nodeClassMask, &access);
				if(maskMatch && (access & OV_AC_READ)){
					if(fillDescription){
						*statusCode = opcua_nsOv_fillReferenceDescription2(&childTypeDefinition, 0, UA_NODEID_HasTypeDefinition, UA_FALSE, resultMask, &(dst[*refCount]));
					}
					(*refCount)++;
				}
			}
		}
	}
	return UA_STATUSCODE_GOOD;
}

UA_Int32 countReferenceDescriptions_HasTypeDefinition2(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, OV_UINT* refCount){
	return getReferenceDescriptions_HasTypeDefinition2(browseDescription, pNode,refCount, FALSE, 0, NULL, NULL);
}

UA_Int32 fillReferenceDescriptions_HasTypeDefinition2(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, OV_UINT* refCount,
		UA_UInt32 resultMask, UA_ReferenceDescription* dst, UA_StatusCode* statusCode){
	return getReferenceDescriptions_HasTypeDefinition2(browseDescription, pNode, refCount, TRUE, resultMask, dst, statusCode);
}

/******************************************************************
 * helper for HasSubtype
 *****************************************************************/

UA_Int32 getReferenceDescriptions_HasSubtype2(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, OV_UINT* refCount,
		OV_BOOL fillDescription, UA_UInt32 resultMask, UA_ReferenceDescription* dst,
		UA_StatusCode* statusCode){
	OV_BOOL					maskMatch;
	OV_ACCESS				access		=	OV_AC_NONE;
	OV_INSTPTR_ov_class		pChildClass	=	NULL;
	OV_INSTPTR_ov_class		pParentClass	=	NULL;
	OV_ELEMENT				referencedElement;
	//	maps to ov/inheritance
	if((browseDescription->browseDirection == UA_BROWSEDIRECTION_FORWARD)
			|| (browseDescription->browseDirection == UA_BROWSEDIRECTION_BOTH)){
		if(pNode->elemtype == OV_ET_OBJECT && Ov_GetClassPtr(pNode->pobj) == pclass_ov_class){
			Ov_ForEachChild(ov_inheritance, Ov_StaticPtrCast(ov_class, pNode->pobj), pChildClass){
				referencedElement.elemtype = OV_ET_OBJECT;
				referencedElement.pobj = Ov_PtrUpCast(ov_object, pChildClass);
				maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&referencedElement, browseDescription->nodeClassMask, &access);
				if(maskMatch && (access & OV_AC_READ)){
					if(fillDescription){
						*statusCode = opcua_nsOv_fillReferenceDescription2(&referencedElement, 0, UA_NODEID_HasSubtype, UA_TRUE, resultMask, &(dst[*refCount]));
					}
					(*refCount)++;
				}
			}
		}
	}
	if((browseDescription->browseDirection == UA_BROWSEDIRECTION_INVERSE)
			|| (browseDescription->browseDirection == UA_BROWSEDIRECTION_BOTH)){
		if(pNode->elemtype == OV_ET_OBJECT && pNode->pobj == Ov_PtrUpCast(ov_object, pclass_ov_class)){
			pParentClass = Ov_GetParent(ov_inheritance, Ov_StaticPtrCast(ov_class, pNode->pobj));
			if(pParentClass){
				referencedElement.elemtype = OV_ET_OBJECT;
				referencedElement.pobj = Ov_PtrUpCast(ov_object, pParentClass);
				maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&referencedElement, browseDescription->nodeClassMask, &access);
				if(maskMatch && (access & OV_AC_READ)){
					if(fillDescription){
						*statusCode = opcua_nsOv_fillReferenceDescription2(&referencedElement, 0, UA_NODEID_HasSubtype, UA_FALSE, resultMask, &(dst[*refCount]));
					}
					(*refCount)++;
				}
			}
		}
	}
	return UA_STATUSCODE_GOOD;
}

UA_Int32 countReferenceDescriptions_HasSubtype2(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, OV_UINT* refCount){
	return getReferenceDescriptions_HasSubtype2(browseDescription, pNode,refCount, FALSE, 0, NULL, NULL);
}

UA_Int32 fillReferenceDescriptions_HasSubtype2(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, OV_UINT* refCount,
		UA_UInt32 resultMask, UA_ReferenceDescription* dst, UA_StatusCode* statusCode){
	return getReferenceDescriptions_HasSubtype2(browseDescription, pNode, refCount, TRUE, resultMask, dst, statusCode);
}

/******************************************************************
 * helper for HasProperty
 *****************************************************************/

UA_Int32 getReferenceDescriptions_HasProperty2(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, OV_UINT* refCount,
		OV_BOOL fillDescription, UA_UInt32 resultMask, UA_ReferenceDescription* dst, UA_StatusCode* statusCode){
	OV_BOOL					maskMatch;
	OV_ACCESS				access		=	OV_AC_NONE;
	OV_ELEMENT				referencedElement;
	//	maps to variables in objects
	if((browseDescription->browseDirection == UA_BROWSEDIRECTION_FORWARD)
			|| (browseDescription->browseDirection == UA_BROWSEDIRECTION_BOTH)){
		if(pNode->elemtype == OV_ET_OBJECT){
			/*	check if it is a methodNode	*/
			OV_INSTPTR_opcua_methodNode pMethod = Ov_DynamicPtrCast(opcua_methodNode, pNode->pobj);
			if(pMethod){
				/*	add the hasproperty references for arguments	*/
				referencedElement.elemtype = OV_ET_NONE;
				ov_element_getnextpart(pNode, &referencedElement, OV_ET_OBJECT);
				while(referencedElement.elemtype != OV_ET_NONE){
					maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&referencedElement, browseDescription->nodeClassMask, &access);
					if(maskMatch && (access & OV_AC_READ)){
						/*	check if we are handling the InputArguments or OutputArguments part, if so add reference hasProperty	*/
						if(ov_string_compare(referencedElement.pobj->v_identifier, "InputArguments") == OV_STRCMP_EQUAL
								|| ov_string_compare(referencedElement.pobj->v_identifier, "OutputArguments") == OV_STRCMP_EQUAL){
							if(fillDescription){
								/*	standard description fill	*/
								*statusCode = opcua_nsOv_fillReferenceDescription2(&referencedElement, 0, UA_NODEID_HasProperty, UA_TRUE, resultMask, &(dst[*refCount]));
								/*	specific modifications	*/
								if(resultMask & (1<<2)){
									(dst[*refCount]).nodeClass = UA_NODECLASS_VARIABLE;
								}
								if(resultMask & (1<<5)){
									(dst[*refCount]).typeDefinition.nodeId.namespaceIndex = 0;
									(dst[*refCount]).typeDefinition.nodeId.identifierType = UA_NODEIDTYPE_NUMERIC;
									(dst[*refCount]).typeDefinition.nodeId.identifier.numeric = 296;
								}
								if(resultMask & (1<<3)){
									(dst[*refCount]).browseName.namespaceIndex = 0;
								}
							}
							(*refCount)++;
						}
					}
					ov_element_getnextpart(pNode, &referencedElement, OV_ET_OBJECT);
				}
			}
			if(Ov_GetClassPtr(pNode->pobj) != pclass_opcua_arguments) {
				referencedElement.elemtype = OV_ET_NONE;
				ov_element_getnextpart(pNode, &referencedElement, OV_ET_VARIABLE | OV_ET_MEMBER);
				while(referencedElement.elemtype != OV_ET_NONE){
					maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&referencedElement, browseDescription->nodeClassMask, &access);
					if(maskMatch && (access & OV_AC_READ)){
						if(fillDescription){
							*statusCode = opcua_nsOv_fillReferenceDescription2(&referencedElement, 0, UA_NODEID_HasProperty, UA_TRUE, resultMask, &(dst[*refCount]));
						}
						(*refCount)++;
					}
					ov_element_getnextpart(pNode, &referencedElement, OV_ET_VARIABLE | OV_ET_MEMBER);
				}
			}
		}
	}
	if((browseDescription->browseDirection == UA_BROWSEDIRECTION_INVERSE)
			|| (browseDescription->browseDirection == UA_BROWSEDIRECTION_BOTH)){
		if(pNode->elemtype == OV_ET_VARIABLE || pNode->elemtype == OV_ET_MEMBER){
			referencedElement.elemtype = OV_ET_OBJECT;
			referencedElement.pobj = pNode->pobj;
			maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&referencedElement, browseDescription->nodeClassMask, &access);
			if(maskMatch && (access & OV_AC_READ)){
				if(fillDescription){
					*statusCode = opcua_nsOv_fillReferenceDescription2(&referencedElement, 0, UA_NODEID_HasProperty, UA_FALSE, resultMask, &(dst[*refCount]));
				}
				(*refCount)++;
			}
		} else if(pNode->elemtype == OV_ET_OBJECT && Ov_GetClassPtr(pNode->pobj) == pclass_opcua_arguments){
			referencedElement.elemtype = OV_ET_OBJECT;
			referencedElement.pobj = pNode->pobj->v_pouterobject;
			maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&referencedElement, browseDescription->nodeClassMask, &access);
			if(maskMatch && (access & OV_AC_READ)){
				if(fillDescription){
					*statusCode = opcua_nsOv_fillReferenceDescription2(&referencedElement, 0, UA_NODEID_HasProperty, UA_FALSE, resultMask, &(dst[*refCount]));
				}
				(*refCount)++;
			}
		}
	}
	return UA_STATUSCODE_GOOD;
}

UA_Int32 countReferenceDescriptions_HasProperty2(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, OV_UINT* refCount){
	return getReferenceDescriptions_HasProperty2(browseDescription, pNode,refCount, FALSE, 0, NULL, NULL);
}

UA_Int32 fillReferenceDescriptions_HasProperty2(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, OV_UINT* refCount,
		UA_UInt32 resultMask, UA_ReferenceDescription* dst, UA_StatusCode* statusCode){
	return getReferenceDescriptions_HasProperty2(browseDescription, pNode, refCount, TRUE, resultMask, dst, statusCode);
}

/******************************************************************
 * helper for HasComponent
 *****************************************************************/

UA_Int32 getReferenceDescriptions_HasComponent2(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, OV_UINT* refCount,
		OV_BOOL fillDescription, UA_UInt32 resultMask, UA_ReferenceDescription* dst,
		UA_StatusCode* statusCode){
	OV_BOOL					maskMatch;
	OV_ACCESS				access		=	OV_AC_NONE;
	OV_ELEMENT				referencedElement;
	//	maps to variables in objects
	if((browseDescription->browseDirection == UA_BROWSEDIRECTION_FORWARD)
			|| (browseDescription->browseDirection == UA_BROWSEDIRECTION_BOTH)){
		OV_INSTPTR_opcua_methodNode pMethod = Ov_DynamicPtrCast(opcua_methodNode, pNode->pobj);
		if(pNode->elemtype == OV_ET_OBJECT){
			/*	children - only for methods	*/
			referencedElement.elemtype = OV_ET_NONE;
			referencedElement.pobj = NULL;
			ov_element_getnextchild(pNode, &referencedElement);
			while(referencedElement.elemtype!=OV_ET_NONE){
				if(Ov_CanCastTo(opcua_methodNode, referencedElement.pobj)){
					maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&referencedElement, browseDescription->nodeClassMask, &access);
					if(maskMatch && (access & OV_AC_READ)){
						if(fillDescription){
							*statusCode = opcua_nsOv_fillReferenceDescription2(&referencedElement, 0, UA_NODEID_HasComponent, UA_TRUE, resultMask, &(dst[*refCount]));
						}
						(*refCount)++;
					}
				}
				ov_element_getnextchild(pNode, &referencedElement);
			}

			referencedElement.elemtype = OV_ET_NONE;
			ov_element_getnextpart(pNode, &referencedElement, OV_ET_OBJECT);
			while(referencedElement.elemtype != OV_ET_NONE){
				maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&referencedElement, browseDescription->nodeClassMask, &access);
				if(maskMatch && (access & OV_AC_READ)){
					/*	exclude the Arguments-parts of method nodes	*/
					if(!pMethod || referencedElement.elemtype != OV_ET_OBJECT
							|| (ov_string_compare(referencedElement.pobj->v_identifier, "InputArguments") != OV_STRCMP_EQUAL
									&& ov_string_compare(referencedElement.pobj->v_identifier, "OutputArguments") != OV_STRCMP_EQUAL)){
						if(fillDescription){
							*statusCode = opcua_nsOv_fillReferenceDescription2(&referencedElement, 0, UA_NODEID_HasComponent, UA_TRUE, resultMask, &(dst[*refCount]));
						}
						(*refCount)++;
					}
				}
				ov_element_getnextpart(pNode, &referencedElement, OV_ET_OBJECT);
			}
		}
	}
	if((browseDescription->browseDirection == UA_BROWSEDIRECTION_INVERSE)
			|| (browseDescription->browseDirection == UA_BROWSEDIRECTION_BOTH)){
		if(pNode->elemtype == OV_ET_OBJECT){
			if(pNode->pobj->v_pouterobject){
				referencedElement.elemtype = OV_ET_OBJECT;
				referencedElement.pobj = pNode->pobj->v_pouterobject;
				maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&referencedElement, browseDescription->nodeClassMask, &access);
				if(maskMatch && (access & OV_AC_READ)){
					if(fillDescription){
						*statusCode = opcua_nsOv_fillReferenceDescription2(&referencedElement, 0, UA_NODEID_HasComponent, UA_FALSE, resultMask, &(dst[*refCount]));
					}
					(*refCount)++;
				}
			} else if(Ov_CanCastTo(opcua_methodNode, referencedElement.pobj)){
				referencedElement.elemtype = OV_ET_OBJECT;
				referencedElement.pobj = (OV_INSTPTR_ov_object)Ov_GetParent(ov_containment, pNode->pobj);
				maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&referencedElement, browseDescription->nodeClassMask, &access);
				if(maskMatch && (access & OV_AC_READ)){
					if(fillDescription){
						*statusCode = opcua_nsOv_fillReferenceDescription2(&referencedElement, 0, UA_NODEID_HasComponent, UA_FALSE, resultMask, &(dst[*refCount]));
					}
					(*refCount)++;
				}
			}
		}
	}
	return UA_STATUSCODE_GOOD;
}

UA_Int32 countReferenceDescriptions_HasComponent2(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, OV_UINT* refCount){
	return getReferenceDescriptions_HasComponent2(browseDescription, pNode,refCount, FALSE, 0, NULL, NULL);
}

UA_Int32 fillReferenceDescriptions_HasComponent2(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, OV_UINT* refCount,
		UA_UInt32 resultMask, UA_ReferenceDescription* dst, UA_StatusCode* statusCode){
	return getReferenceDescriptions_HasComponent2(browseDescription, pNode, refCount, TRUE, resultMask, dst, statusCode);
}

/******************************************************************
 * helper for OVReferences
 *****************************************************************/

UA_Int32 getReferenceDescriptions_OvReferences2(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, signed char flag, OV_INSTPTR_ov_association passoc,
		OV_UINT* refCount, OV_BOOL fillDescription, UA_UInt32 resultMask, UA_ReferenceDescription* dst, UA_StatusCode* statusCode){
	OV_BOOL					maskMatch;
	OV_ACCESS				access		=	OV_AC_NONE;
	OV_ELEMENT				linkElement;
	OV_ELEMENT				referencedElement;

	if(flag < 0){	// -1: association definde by passoc; -2 all associations
		if((browseDescription->browseDirection == UA_BROWSEDIRECTION_FORWARD)
				|| (browseDescription->browseDirection == UA_BROWSEDIRECTION_BOTH)){
			if(pNode->elemtype == OV_ET_OBJECT){
				linkElement.elemtype = OV_ET_NONE;
				ov_element_getnextpart(pNode, &linkElement, OV_ET_PARENTLINK);
				while(linkElement.elemtype != OV_ET_NONE){
					if(flag == -2 || linkElement.elemunion.passoc == passoc){
						if(linkElement.elemunion.passoc->v_assoctype == OV_AT_ONE_TO_MANY){
							referencedElement.elemtype = OV_ET_OBJECT;
							Ov_Association_ForEachChild(linkElement.elemunion.passoc, pNode->pobj, referencedElement.pobj){
								maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&referencedElement, browseDescription->nodeClassMask, &access);
								if(maskMatch && (access & OV_AC_READ)){
									if(fillDescription){
										*statusCode = opcua_nsOv_fillReferenceDescription2(&referencedElement, opcua_pUaServer->v_NameSpaceIndex,
												linkElement.elemunion.passoc->v_idL, UA_TRUE, resultMask, &(dst[*refCount]));
									}
									(*refCount)++;
								}
							}
						} else if(linkElement.elemunion.passoc->v_assoctype == OV_AT_ONE_TO_ONE) {
							referencedElement.elemtype = OV_ET_OBJECT;
							referencedElement.pobj = Ov_Association_GetChild(linkElement.elemunion.passoc, pNode->pobj);
							if(referencedElement.pobj){
								maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&referencedElement, browseDescription->nodeClassMask, &access);
								if(maskMatch && (access & OV_AC_READ)){
									if(fillDescription){
										*statusCode = opcua_nsOv_fillReferenceDescription2(&referencedElement, opcua_pUaServer->v_NameSpaceIndex,
												linkElement.elemunion.passoc->v_idL, UA_TRUE, resultMask, &(dst[*refCount]));
									}
									(*refCount)++;
								}
							}
						}
						if(flag == -1){
							break;
						}
					}
					ov_element_getnextpart(pNode, &linkElement, OV_ET_PARENTLINK);
				}
			}
		}
		if((browseDescription->browseDirection == UA_BROWSEDIRECTION_INVERSE)
				|| (browseDescription->browseDirection == UA_BROWSEDIRECTION_BOTH)){
			if(pNode->elemtype == OV_ET_OBJECT){
				linkElement.elemtype = OV_ET_NONE;
				ov_element_getnextpart(pNode, &linkElement, OV_ET_CHILDLINK);
				while(linkElement.elemtype != OV_ET_NONE){
					if(flag == -2 || linkElement.elemunion.passoc == passoc){
						referencedElement.elemtype = OV_ET_OBJECT;
						referencedElement.pobj = Ov_Association_GetParent(linkElement.elemunion.passoc, pNode->pobj);
						if(referencedElement.pobj){
							maskMatch = opcua_nsOv_nodeClassMaskMatchAndGetAccess(&referencedElement, browseDescription->nodeClassMask, &access);
							if(maskMatch && (access & OV_AC_READ)){
								if(fillDescription){
									*statusCode = opcua_nsOv_fillReferenceDescription2(&referencedElement, opcua_pUaServer->v_NameSpaceIndex,
											linkElement.elemunion.passoc->v_idL, UA_FALSE, resultMask, &(dst[*refCount]));
								}
								(*refCount)++;
							}
						}
						if(flag == -1){
							break;
						}
					}
					ov_element_getnextpart(pNode, &linkElement, OV_ET_CHILDLINK);
				}
			}
		}
	}
	return UA_STATUSCODE_GOOD;
}

UA_Int32 countReferenceDescriptions_OvReferences2(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, signed char flag, OV_INSTPTR_ov_association passoc,
		OV_UINT* refCount){
	return getReferenceDescriptions_OvReferences2(browseDescription, pNode, flag, passoc, refCount, FALSE, 0, NULL, NULL);
}

UA_Int32 fillReferenceDescriptions_OvReferences2(const UA_BrowseDescription* browseDescription, OV_ELEMENT* pNode, signed char flag, OV_INSTPTR_ov_association passoc,
		OV_UINT* refCount, UA_UInt32 resultMask, UA_ReferenceDescription* dst, UA_StatusCode* statusCode){
	return getReferenceDescriptions_OvReferences2(browseDescription, pNode, flag, passoc, refCount, TRUE, resultMask, dst, statusCode);
}




static void OV_NodeStore2_deleteNodestore(void *handle){

}

static void OV_NodeStore2_deleteNode(UA_Node *node){
	if (node)
		UA_Node_deleteMembersAnyNodeClass(node);
	UA_free(node);
}
static void OV_NodeStore2_releaseNode(void *handle, const UA_Node *node){
	OV_NodeStore2_deleteNode((UA_Node*)node);
}
static UA_StatusCode OV_NodeStore2_insert(void *handle, UA_Node *node, UA_NodeId *parrentNode){
	//OV_NodeStore2_releaseNode(handle, node);
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

static UA_Node * OV_NodeStore2_newNode(UA_NodeClass nodeClass){ //TODO add nodestore handle? --> move nodeStore from static context to main
    return NULL;
}

static const UA_Node * OV_NodeStore2_getNode(void *handle, const UA_NodeId *nodeId){
	UA_Node 				*newNode = NULL;
	UA_StatusCode 			result = UA_STATUSCODE_GOOD;
	OV_PATH 				path;
	OV_INSTPTR_ov_object	pobj = NULL;
	OV_INSTPTR_ov_object	pobjtemp = NULL;
	OV_TICKET 				*pTicket = NULL;
	OV_VTBLPTR_ov_object	pVtblObj = NULL;
	OV_ACCESS				access;
	OV_ELEMENT				element;
	UA_NodeClass 			*nodeClass = NULL;
	OV_ANY					value = {.value.vartype = OV_VT_VOID, .value.valueunion.val_string = NULL, .state=OV_ST_UNKNOWN, .time.secs = 0, .time.usecs = 0};
	OV_ANY					emptyAny = {.value.vartype = OV_VT_VOID, .value.valueunion.val_string = NULL, .state=OV_ST_UNKNOWN, .time.secs = 0, .time.usecs = 0};

	ov_memstack_lock();
	result = opcua_nodeStoreFunctions_resolveNodeIdToPath(*nodeId, &path);
	if(result != UA_STATUSCODE_GOOD){
		ov_memstack_unlock();
		return NULL;
	}
	element = path.elements[path.size-1];
	ov_memstack_unlock();

	result = opcua_nodeStoreFunctions_getVtblPointerAndCheckAccess(&element, pTicket, &pobj, &pVtblObj, &access);
	if(result != UA_STATUSCODE_GOOD){
		return NULL;
	}

	nodeClass = UA_NodeClass_new();
	if(!nodeClass){
		result = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
		return NULL;
	}
	switch(element.elemtype){
	case OV_ET_OBJECT:
		if(Ov_GetParent(ov_instantiation, pobj) == pclass_ov_class){
			newNode = (UA_Node*)UA_malloc(sizeof(UA_ObjectTypeNode));
			*nodeClass = UA_NODECLASS_OBJECTTYPE;
		} else if(Ov_GetParent(ov_instantiation, pobj) == pclass_ov_variable){
			*nodeClass = UA_NODECLASS_VARIABLETYPE;
			newNode = (UA_Node*)UA_malloc(sizeof(UA_VariableTypeNode));
		} else if(Ov_GetParent(ov_instantiation, pobj) == pclass_ov_association){
			*nodeClass = UA_NODECLASS_REFERENCETYPE;
			newNode = (UA_Node*)UA_malloc(sizeof(UA_ReferenceTypeNode));
		} else if(Ov_GetParent(ov_instantiation, pobj) == pclass_opcua_arguments){
			*nodeClass = UA_NODECLASS_VARIABLE;
			newNode = (UA_Node*)UA_malloc(sizeof(UA_VariableNode));
		} else if(Ov_CanCastTo(opcua_methodNode, pobj)){
			*nodeClass = UA_NODECLASS_METHOD;
			newNode = (UA_Node*)UA_malloc(sizeof(UA_MethodNode));
		} else{
			*nodeClass = UA_NODECLASS_OBJECT;
			newNode = (UA_Node*)UA_malloc(sizeof(UA_ObjectNode));
		}
		break;
	case OV_ET_VARIABLE:
	case OV_ET_MEMBER:
		*nodeClass = UA_NODECLASS_VARIABLE;
		newNode = (UA_Node*)UA_malloc(sizeof(UA_VariableNode));
		break;
	case OV_ET_OPERATION:
		*nodeClass = UA_NODECLASS_METHOD;
		newNode = (UA_Node*)UA_malloc(sizeof(UA_MethodNode));
		break;
	case OV_ET_CHILDLINK:
	case OV_ET_PARENTLINK:
	default:
		*nodeClass = UA_NODECLASS_UNSPECIFIED;
		return NULL;
	}

	// Basic Attribute
	// BrowseName
	UA_QualifiedName qName;
	qName.name = UA_String_fromChars(pobj->v_identifier);
	if(Ov_GetClassPtr(pobj) != pclass_opcua_arguments){
		qName.namespaceIndex = opcua_pUaServer->v_NameSpaceIndex;
	} else {
		qName.namespaceIndex = 0;
	}
	newNode->browseName = qName;

	// Description
	OV_STRING tempString = pVtblObj->m_getcomment(pobj, &element);
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
	lText.text = UA_String_fromChars(pobj->v_identifier);
	newNode->displayName = lText;

	// NodeId
	newNode->nodeId.identifierType = nodeId->identifierType;
	newNode->nodeId.namespaceIndex = opcua_pUaServer->v_NameSpaceIndex;
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

	// NodeClass
	newNode->nodeClass 	= *nodeClass;

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

	switch(*nodeClass){
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
			}else if(Ov_GetParent(ov_instantiation, pobjtemp) == pclass_opcua_arguments){
				((UA_VariableNode*)newNode)->arrayDimensionsSize = 0;
			} else {
				result = ov_resultToUaStatusCode(OV_ERR_BADTYPE);
				break;
			}
			break;
		default:
			result = ov_resultToUaStatusCode(OV_ERR_BADPATH);
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
		}else  if(Ov_GetParent(ov_instantiation, pobjtemp) == pclass_opcua_arguments){
			((UA_VariableNode*)newNode)->valueRank = 1;	/*	one dimension	*/
			result = UA_STATUSCODE_GOOD;
			break;
		} else {
			result = ov_resultToUaStatusCode(OV_ERR_BADTYPE);
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
			result = ov_resultToUaStatusCode((pVtblObj->m_getvar)(pobjtemp, &element, &value));
			value.value.vartype &= OV_VT_KSMASK;
			if(result == UA_STATUSCODE_GOOD){
				result = ov_AnyToVariant(&value, &(((UA_VariableNode*)newNode)->value.data.value.value));
				((UA_VariableNode*)newNode)->dataType = ov_varTypeToNodeId(value.value.vartype);
				value = emptyAny;
			}
			break;
		case OV_ET_OBJECT:
			if(Ov_GetParent(ov_instantiation, pobjtemp) == pclass_opcua_arguments){
				size_t numberofArgs = 0;
				UA_Argument *argArray = NULL;
				result = opcua_nodeStoreFunctions_getCallArgs((OV_INSTPTR_opcua_arguments)pobjtemp, &numberofArgs, &argArray);
				((UA_VariableNode*)newNode)->value.data.value.value.data = argArray;
				((UA_VariableNode*)newNode)->dataType = ov_varTypeToNodeId(value.value.vartype);
			} else {
				result = ov_resultToUaStatusCode(OV_ERR_BADPATH);
			}
			break;
		default:
			result = ov_resultToUaStatusCode(OV_ERR_BADPATH);
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
			}else if(Ov_GetParent(ov_instantiation, pobjtemp) == pclass_opcua_arguments){
				((UA_VariableNode*)newNode)->arrayDimensionsSize = 0;
			} else {
				result = ov_resultToUaStatusCode(OV_ERR_BADTYPE);
				break;
			}
			break;
		default:
			result = ov_resultToUaStatusCode(OV_ERR_BADPATH);
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
		result = ov_AnyToVariant(ov_variable_initialvalue_get(Ov_StaticPtrCast(ov_variable,pobjtemp)), &(((UA_VariableTypeNode*)newNode)->value.data.value.value));
		// isAbstract
		((UA_VariableTypeNode*)newNode)->isAbstract = UA_FALSE;
		// dataType
		pobjtemp = element.pobj;
		Ov_GetVTablePtr(ov_object, pVtblObj, pobjtemp);
		if((!pVtblObj) || (ov_activitylock)){
			pVtblObj = pclass_ov_object->v_pvtable;
		}
		((UA_VariableTypeNode*)newNode)->dataType = ov_varTypeToNodeId(ov_variable_vartype_get(Ov_StaticPtrCast(ov_variable,pobjtemp)));
		break;
	default:
		break;
	}

	// references
	UA_BrowseDescription 	*browseDescriptions = NULL;
	UA_ReferenceDescription *references = NULL;
	OV_ELEMENT				pNode;
	OV_PATH					nodePath;
	OV_UINT					j;
	OV_UINT					refCount	=	0;
	OV_UINT					refCountBefore	=	0;


	#define UAREFS_LENGTH		12
	#define OVREFSARRAY_CHUNKSIZE	32
	signed char				uaReferences [UAREFS_LENGTH];	//array for ua-references: 0: leave out, -1: to do, 1: done; specific for ovReferences: -2 do all
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
	if(opcua_nodeStoreFunctions_resolveNodeIdToPath(*nodeId, &nodePath) != UA_STATUSCODE_GOOD){
		result = UA_STATUSCODE_BADNODEIDUNKNOWN;
		ov_memstack_unlock();
		return NULL;
	}
	pNode = nodePath.elements[nodePath.size - 1];
	ov_memstack_unlock();
	for(j=1; j<UAREFS_LENGTH; j++){
		uaReferences[j] = -1;
	}
	uaReferences[INDEX_OvReference] = -2;

	browseDescriptions = UA_BrowseDescription_new();
	browseDescriptions->includeSubtypes = UA_TRUE;
	browseDescriptions->browseDirection = UA_BROWSEDIRECTION_BOTH;
	browseDescriptions->resultMask = (1<<0) | (1<<1) |(1<<2) | (1<<3) | (1<<4) | (1<<5);
	browseDescriptions->nodeClassMask = (1<<0) | (1<<1) |(1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6) | (1<<7);
/*********************************************************************************************************************************
* count all reference
********************************************************************************************************************************/
	// HasProperty
	countReferenceDescriptions_HasProperty2(browseDescriptions, &pNode, &refCount);
	if (refCount == refCountBefore){
		uaReferences[INDEX_HasProperty] = 1;
	}
	refCountBefore = refCount;
	// HasTypeDefinition
	countReferenceDescriptions_HasTypeDefinition2(browseDescriptions, &pNode, &refCount);
	if (refCount == refCountBefore){
		uaReferences[INDEX_HasTypeDefinition] = 1;
	}
	refCountBefore = refCount;
	// References
	// Do nothing as this reference type is abstract
	// NonHierarchical
	// Do nothing as this reference type is abstract
	// Hierarchical
	// Do nothing as this reference type is abstract
	// HasChild
	// Do nothing as this reference type is abstract
	// Aggregates
	// Do nothing as this reference type is abstract
	// Organizes
	countReferenceDescriptions_Organizes2(browseDescriptions, &pNode, &refCount);
	if (refCount == refCountBefore){
		uaReferences[INDEX_Organizes] = 1;
	}
	refCountBefore = refCount;
	// HasSubtype
	countReferenceDescriptions_HasSubtype2(browseDescriptions, &pNode, &refCount);
	if(refCount == refCountBefore){
		uaReferences[INDEX_HasSubtype] = 1;
	}
	refCountBefore = refCount;
	// HasComponent
	countReferenceDescriptions_HasComponent2(browseDescriptions, &pNode, &refCount);
	if(refCount == refCountBefore){
		uaReferences[INDEX_HasComponent] = 1;
	}
	refCountBefore = refCount;
	// OvReferences
	// NOTIMPLEMENTED
	//  countReferenceDescriptions_OvReferences2(browseDescriptions, pNode, uaReferences[INDEX_OvReference], pAssoc, &refCount);
	//  if(refCount == refCountBefore){
	//		uaReferences[INDEX_OvReference] = 1;
	//	}
	//	refCountBefore = refCount;

	/*************************************************************************************************************
	 * create Array
	 ************************************************************************************************************/
	references = UA_Array_new(refCount, &UA_TYPES[UA_TYPES_REFERENCEDESCRIPTION]);
	newNode->references = UA_calloc(refCount, sizeof(UA_ReferenceNode));
	if(!references && !newNode->references && refCount>0){
		result = UA_STATUSCODE_BADOUTOFMEMORY;
		return NULL;
	}
	newNode->referencesSize = refCount;
	refCount = 0;
	/*********************************************************************************************************************************
	 * fill in results for all reference types
	 ********************************************************************************************************************************/
	// HasProperty
	if (uaReferences[INDEX_HasProperty] != 1){
		fillReferenceDescriptions_HasProperty2(browseDescriptions, &pNode, &refCount, browseDescriptions->resultMask,
												references, &result);
	}
	// HasTypeDefinition
	if (uaReferences[INDEX_HasTypeDefinition] != 1){
		fillReferenceDescriptions_HasTypeDefinition2(browseDescriptions, &pNode, &refCount, browseDescriptions->resultMask,
												references, &result);
	}
	// References
	// Do nothing as this reference type is abstract
	// NonHierarchical
	// Do nothing as this reference type is abstract
	// Hierarchical
	// Do nothing as this reference type is abstract
	// HasChild
	// Do nothing as this reference type is abstract
	// Aggregates
	// Do nothing as this reference type is abstract
	// Organizes
	if (uaReferences[INDEX_Organizes] != 1){
		fillReferenceDescriptions_Organizes2(browseDescriptions, &pNode, &refCount, browseDescriptions->resultMask,
												references, &result);
	}
	// HasSubtype
	if (uaReferences[INDEX_HasSubtype] != 1){
		fillReferenceDescriptions_HasSubtype2(browseDescriptions, &pNode, &refCount, browseDescriptions->resultMask,
												references, &result);
	}
	// HasComponent
	if (uaReferences[INDEX_HasComponent] != 1){
		fillReferenceDescriptions_HasComponent2(browseDescriptions, &pNode, &refCount, browseDescriptions->resultMask,
												references, &result);
	}
	// OvReferences
	// NOTIMPLEMENTED
	//  if (uaReferences[INDEX_OvReference] != 1){
	//		fillReferenceDescriptions_OvReferences2(browseDescriptions, pNode, &refCount, browseDescriptions->resultMask,
	//  								references, &result);
	//  }
	for (size_t i = 0; i < newNode->referencesSize; i++){
		newNode->references[i].referenceTypeId = references[i].referenceTypeId;
		newNode->references[i].targetId = references[i].nodeId;
		newNode->references[i].isInverse = !references[i].isForward;
	}
	return newNode;
}
static UA_Node * OV_NodeStore2_getCopyNode(void *handle, const UA_NodeId *nodeId){
	return (UA_Node*)OV_NodeStore2_getNode(handle, nodeId);
}
static UA_StatusCode OV_NodeStore2_removeNode(void *handle, const UA_NodeId *nodeId){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}
static UA_StatusCode OV_NodeStore2_insertNode(void *handle, UA_Node *node, UA_NodeId *parrentNode){
	return OV_NodeStore2_insert(handle, node, parrentNode);
}
static UA_StatusCode OV_NodeStore2_replaceNode(void *handle, UA_Node *node){
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
	result = opcua_nodeStoreFunctions_resolveNodeIdToPath(node->nodeId, &path);
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
						result = ov_VariantToAny(&(((UA_VariableNode*)node)->value.data.value.value), &value);
						if(result != UA_STATUSCODE_GOOD){
							break;
						}
						if(((UA_VariableNode*)node)->value.data.value.hasSourceTimestamp == UA_TRUE){
							value.value.vartype |= OV_VT_HAS_TIMESTAMP;
							value.time = ov_1601nsTimeToOvTime(((UA_VariableNode*)node)->value.data.value.sourceTimestamp);
						}
						result = ov_resultToUaStatusCode((pVtblObj->m_setvar)(pobj, &element, &value));
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
							result = ov_resultToUaStatusCode((pVtblObj->m_setvar)(pobj, &element, &value));
						}
					} else {
						result = UA_STATUSCODE_BADINVALIDARGUMENT;
						break;
					}
					break;
				default:
					result = ov_resultToUaStatusCode(OV_ERR_BADPATH);
					break;
				}
			} else {
				result = UA_STATUSCODE_BADNOTWRITABLE;
			}
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
						result = ov_VariantToAny(&(((UA_VariableNode*)node)->value.data.value.value), &value);
						if(result != UA_STATUSCODE_GOOD){
							break;
						}
						if(((UA_VariableNode*)node)->value.data.value.hasSourceTimestamp == UA_TRUE){
							value.value.vartype |= OV_VT_HAS_TIMESTAMP;
							value.time = ov_1601nsTimeToOvTime(((UA_VariableNode*)node)->value.data.value.sourceTimestamp);
						}
						result = ov_resultToUaStatusCode((pVtblObj->m_setvar)(pobj, &element, &value));
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
							result = ov_resultToUaStatusCode((pVtblObj->m_setvar)(pobj, &element, &value));
						}
					} else {
						result = UA_STATUSCODE_BADINVALIDARGUMENT;
						break;
					}
					break;
				default:
					result = ov_resultToUaStatusCode(OV_ERR_BADPATH);
					break;
				}
			} else {
				result = UA_STATUSCODE_BADNOTWRITABLE;
			}
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
	OV_NodeStore2_releaseNode(handle, node);
	return UA_STATUSCODE_GOOD;
}

void
opcua_nodeStoreFunctions_ovNodeStoreInterface2New(UA_NodestoreInterface* nsi) {
    nsi->handle =        	NULL;
    nsi->deleteNodeStore =  (UA_NodestoreInterface_delete) 		OV_NodeStore2_deleteNodestore;
    nsi->newNode =       	(UA_NodestoreInterface_newNode)     OV_NodeStore2_newNode;
    nsi->deleteNode =    	(UA_NodestoreInterface_deleteNode)  OV_NodeStore2_deleteNode;
    nsi->insert =       	(UA_NodestoreInterface_insert)      OV_NodeStore2_insertNode;
    nsi->get =          	(UA_NodestoreInterface_get)         OV_NodeStore2_getNode;
    nsi->getCopy =      	(UA_NodestoreInterface_getCopy)     OV_NodeStore2_getCopyNode;
    nsi->replace =      	(UA_NodestoreInterface_replace)     OV_NodeStore2_replaceNode;
    nsi->remove =       	(UA_NodestoreInterface_remove)      OV_NodeStore2_removeNode;
    //nsi->iterateNode =       (UA_NodestoreInterface_iterateNode)     OV_NodeStore2iterateNode;
    nsi->release =      	(UA_NodestoreInterface_release) 	OV_NodeStore2_releaseNode;
}
void
opcua_nodeStoreFunctions_ovNodeStoreInterface2Delete(UA_NodestoreInterface * nodestoreInterface){
	if (nodestoreInterface->handle)
		UA_free(nodestoreInterface->handle);
}

