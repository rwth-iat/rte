/*
 * ovTrafo.c
 *
 *  Created on: 06.12.2016
 *      Author: Torben Deppe, Julian Grothoff
 *
 *  Collects functions that are necessary to transform ov object to ua nodes via UA_Nodestore as interface.
 */

#ifndef OV_COMPILE_LIBRARY_opcua
#define OV_COMPILE_LIBRARY_opcua
#endif

#include "ovTrafo.h"
#include "libov/ov_macros.h"
#include "opcua.h"
#include "helpers.h"
#include "NoneTicketAuthenticator.h"

#define OPCUA_OVTRAFO_ADDREFERENCEDIRECTION_FORWARD 0x01
#define OPCUA_OVTRAFO_ADDREFERENCEDIRECTION_BACKWARD 0x02
#define OPCUA_OVTRAFO_ADDREFERENCEDIRECTION_BOTH 0x04

OV_DLLFNCEXPORT OV_BOOL opcua_ovTrafo_addReferenceToSpecificObject(
		OV_INSTPTR_opcua_server pServer, OV_INSTPTR_ov_object pobj, UA_Node* node){
	if(!pServer)
		return FALSE;
	UA_AddReferencesItem childReferenceItem;
	OV_INSTPTR_opcua_interface pInterfaceCheck = NULL;
	OV_VTBLPTR_opcua_interface pVtblInterface = NULL; //TODO use Call makro instead?
	Ov_ForEachChild(opcua_serverToInterfaces, pServer, pInterfaceCheck){
		Ov_GetVTablePtr(opcua_interface, pVtblInterface, pInterfaceCheck);
		if(pVtblInterface){
			UA_AddReferencesItem_init(&childReferenceItem);
			if(pVtblInterface->m_checkReference(pInterfaceCheck, OPCUA_OVTRAFO_DEFAULTNSINDEX, pobj, &childReferenceItem)){
				UA_Node_addReference(node, &childReferenceItem);
				UA_AddReferencesItem_deleteMembers(&childReferenceItem);
				return TRUE;
			}
		}
	}
	return FALSE;
}

static UA_StatusCode opcua_ovTrafo_addReference(OV_ELEMENT* pElement, UA_NodeId referenceTypeId, UA_Boolean isForward,
		UA_Node * node){
	OV_INSTPTR_ov_object	pObject			=	NULL;
	UA_StatusCode			result			=	UA_STATUSCODE_GOOD;
	OV_STRING				path			=	NULL;

	// Check element type
	if(pElement->elemtype == OV_ET_OBJECT || pElement->elemtype == OV_ET_VARIABLE || pElement->elemtype == OV_ET_MEMBER){
		pObject = pElement->pobj;
	} else {
		return UA_STATUSCODE_BADNOTSUPPORTED;
	}

	// Concat path for node id of target
	ov_memstack_lock();
	path = ov_path_getcanonicalpath(pObject, 2);
	if(!path){
		ov_memstack_unlock();
		return UA_STATUSCODE_BADOUTOFMEMORY;
	}
	if(pElement->elemtype == OV_ET_VARIABLE){
		ov_string_stack_print(&path, "%s.%s", path, pElement->elemunion.pvar->v_identifier);
	}

	UA_ExpandedNodeId tmpNodeId = UA_EXPANDEDNODEID_STRING_ALLOC(OPCUA_OVTRAFO_DEFAULTNSINDEX, path);
	result = opcua_helpers_addReference(node, NULL, referenceTypeId,
			tmpNodeId,
			opcua_helpers_getNodeClass(pElement),isForward);
	UA_ExpandedNodeId_deleteMembers(&tmpNodeId);
	ov_memstack_unlock();
	return result;
}

/******************************************************************
 * helper for Organizes
 *****************************************************************/

//Maps to ov_containment
static UA_StatusCode opcua_ovTrafo_addOrganizes(OV_INSTPTR_opcua_server pServer,
		OV_ELEMENT* pNode,
		UA_Node * node, OV_UINT direction){
	UA_StatusCode statusCode = UA_STATUSCODE_GOOD;
	OV_ELEMENT	childElement, parentElement;
	OV_ACCESS access = OV_AC_NONE;
	if(pNode->elemtype != OV_ET_OBJECT)
		return UA_STATUSCODE_GOOD;

	//Forward direction
	if (direction & OPCUA_OVTRAFO_ADDORGANIZES_FORWARD){
		childElement.elemtype = OV_ET_OBJECT;
		childElement.pobj = NULL;
		childElement.pvalue = NULL;
		childElement.pvar = NULL;
		if (Ov_CanCastTo(ov_domain, pNode->pobj)){
			Ov_ForEachChild(ov_containment, Ov_StaticPtrCast(ov_domain, pNode->pobj), childElement.pobj){
				childElement.elemunion.pobj = childElement.pobj;
				// Check whether child has a specific transformation
				if(!opcua_ovTrafo_addReferenceToSpecificObject(pServer, childElement.pobj, node))
					// Child has no specific transformation --> use generic interface
					statusCode |= opcua_ovTrafo_addReference(&childElement,
							UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_TRUE, node);
			}
		}
	}
	// Backward / inverse direction
	if (direction & OPCUA_OVTRAFO_ADDORGANIZES_BACKWARD){
		if(pNode->pobj->v_idL || pNode->pobj->v_idH){	// don't do it for ov root object ("/")
			parentElement.elemtype = OV_ET_OBJECT;
			parentElement.pvalue = NULL;
			parentElement.pvar = NULL;
			parentElement.pobj = Ov_StaticPtrCast(ov_object, Ov_GetParent(ov_containment, pNode->pobj));
			childElement.elemunion.pobj = parentElement.pobj;
			access = opcua_helpers_getAccess(&parentElement);
			if(access & OV_AC_READ){
				statusCode |= opcua_ovTrafo_addReference(&parentElement,
						UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_FALSE, node);
			}
		} else {	//	ov root-node --> bridge node in ns0
			statusCode |= UA_STATUSCODE_BADNOTIMPLEMENTED; //TODO implement backward reference to Objects folder
		}
	}
	return statusCode;
}

/******************************************************************
 * helper for HasTypeDefinition
 *****************************************************************/

//	maps to ov_instantiation
static UA_StatusCode opcua_ovTrafo_addHasTypeDefinition(OV_ELEMENT* pNode, UA_Node* node, OV_UINT direction){
	UA_StatusCode statusCode = UA_STATUSCODE_GOOD;
	OV_ACCESS				access		=	OV_AC_NONE;
	OV_INSTPTR_ov_object	pChildObject	=	NULL;
	OV_ELEMENT				parentTypeDefinition, childTypeDefinition;

	// Forward direction
	if (direction & OPCUA_OVTRAFO_ADDHASTYPEDEFINITION_FORWARD){
		//	source is object target is type --> this direction is an instantiation getparent for objects,
		//		and analog for links and variables

		if(pNode->elemtype == OV_ET_OBJECT || pNode->elemtype == OV_ET_VARIABLE
				|| pNode->elemtype == OV_ET_MEMBER || pNode->elemtype == OV_ET_CHILDLINK || pNode->elemtype == OV_ET_PARENTLINK){
			access = opcua_helpers_getAccess(pNode);
			if(access & OV_AC_READ){
				if(pNode->elemtype == OV_ET_OBJECT){
					parentTypeDefinition.elemtype = OV_ET_OBJECT;
					parentTypeDefinition.pobj = Ov_PtrUpCast(ov_object, Ov_GetParent(ov_instantiation, pNode->pobj));
					if (Ov_GetClassPtr(pNode->pobj) != pclass_ov_class){
						statusCode |= opcua_ovTrafo_addReference(&parentTypeDefinition,
									UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION), UA_TRUE, node);
					}
				} else if (pNode->elemtype == OV_ET_VARIABLE || pNode->elemtype == OV_ET_MEMBER){
					parentTypeDefinition.elemtype = OV_ET_OBJECT;
					parentTypeDefinition.pobj = Ov_PtrUpCast(ov_object, pNode->elemunion.pvar);
					// if variable node add hasTypedefinition to PropertyType
					opcua_helpers_addReference(node, NULL, UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
							UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_PROPERTYTYPE), UA_NODECLASS_OBJECTTYPE,
							UA_TRUE);
				}else if (pNode->elemtype == OV_ET_PARENTLINK || pNode->elemtype == OV_ET_CHILDLINK){
					parentTypeDefinition.elemtype = OV_ET_OBJECT;
					parentTypeDefinition.pobj = Ov_PtrUpCast(ov_object, pNode->elemunion.passoc);
					statusCode |= opcua_ovTrafo_addReference(&parentTypeDefinition,
									UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION), UA_TRUE, node);
				}
			}
		}
	}
	// Backward / inverse direction
	if (direction & OPCUA_OVTRAFO_ADDHASTYPEDEFINITION_BACKWARD){
		//	this is an instantiation getchild for objects (classes), for variables and links this direction delivers nothing
		if(pNode->elemtype == OV_ET_OBJECT && Ov_GetClassPtr(pNode->pobj) == pclass_ov_class){
			Ov_ForEachChild(ov_instantiation, Ov_StaticPtrCast(ov_class, pNode->pobj), pChildObject){
				childTypeDefinition.elemtype = OV_ET_OBJECT;
				childTypeDefinition.pobj = pChildObject;
				access = opcua_helpers_getAccess(&childTypeDefinition);
				if(access & OV_AC_READ){
					statusCode |= opcua_ovTrafo_addReference(&childTypeDefinition,
							UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION), UA_FALSE, node);
				}
			}
		}
	}
	return statusCode;
}

/******************************************************************
 * helper for HasSubtype
 *****************************************************************/

// Maps to ov_inheritance
static UA_StatusCode opcua_ovTrafo_addHasSubtype(OV_ELEMENT* pNode, UA_Node * node, OV_UINT direction){
	UA_StatusCode statusCode = UA_STATUSCODE_GOOD;
	OV_ACCESS				access		=	OV_AC_NONE;
	OV_INSTPTR_ov_class		pChildClass	=	NULL;
	OV_INSTPTR_ov_class		pParentClass	=	NULL;
	OV_ELEMENT				referencedElement;
	// Forward direction
	if (direction & OPCUA_OVTRAFO_ADDHASSUBTYPE_FORWARD){
		if(pNode->elemtype == OV_ET_OBJECT && Ov_GetClassPtr(pNode->pobj) == pclass_ov_class){
			Ov_ForEachChild(ov_inheritance, Ov_StaticPtrCast(ov_class, pNode->pobj), pChildClass){
				referencedElement.elemtype = OV_ET_OBJECT;
				referencedElement.pobj = Ov_PtrUpCast(ov_object, pChildClass);
				access = opcua_helpers_getAccess(&referencedElement);
				if(access & OV_AC_READ){
						statusCode |= opcua_ovTrafo_addReference(&referencedElement,
							UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE), UA_TRUE, node);
				}
			}
		}
	}
	// Backward / inverse direction
	if (direction & OPCUA_OVTRAFO_ADDHASSUBTYPE_BACKWARD){
		if(pNode->elemtype == OV_ET_OBJECT &&
				Ov_GetClassPtr(pNode->pobj) == pclass_ov_class){

			// Set inverse reference to namespace 0 UA types
			if(ov_string_compare(pNode->pobj->v_identifier, "object") == OV_STRCMP_EQUAL){
				statusCode |= opcua_helpers_addReference(node, NULL, UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
						UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE), UA_NODECLASS_OBJECTTYPE, UA_FALSE);
			}
			// Set inverse reference to namespace 0 UA types
			if(ov_string_compare(pNode->pobj->v_identifier, "domain") == OV_STRCMP_EQUAL){
				statusCode |= opcua_helpers_addReference(node, NULL, UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
						UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE), UA_NODECLASS_OBJECTTYPE, UA_FALSE);
			}

			pParentClass = Ov_GetParent(ov_inheritance, Ov_StaticPtrCast(ov_class, pNode->pobj));
			if(pParentClass){
				referencedElement.elemtype = OV_ET_OBJECT;
				referencedElement.pobj = Ov_PtrUpCast(ov_object, pParentClass);
				access = opcua_helpers_getAccess(&referencedElement);
				if(access & OV_AC_READ){
					statusCode |= opcua_ovTrafo_addReference(&referencedElement,
							UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE), UA_FALSE, node);
				}
			}
		}
		// if object is an association add hasSubType Reference to UA_NS0ID_NONHIERARCHICALREFERENCES
		if (Ov_GetClassPtr(pNode->pobj) == pclass_ov_association){
			referencedElement.elemtype = OV_ET_OBJECT;
			referencedElement.pobj = pNode->pobj;
			access = opcua_helpers_getAccess(&referencedElement);
			if(access & OV_AC_READ){
				statusCode |= opcua_helpers_addReference(node, NULL, UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE), UA_EXPANDEDNODEID_STRING(OPCUA_OVTRAFO_DEFAULTNSINDEX, OPCUA_OVTRAFO_OVREFERENCES), UA_NODECLASS_REFERENCETYPE, UA_FALSE);
			}
		}
	}

	return statusCode;
}

/******************************************************************
 * helper for HasProperty
 *****************************************************************/

//	Maps to variables in objects
static UA_StatusCode opcua_ovTrafo_addHasProperty(OV_INSTPTR_opcua_server pServer, OV_ELEMENT* pNode, UA_Node* node, OV_UINT direction){
	UA_StatusCode statusCode = UA_STATUSCODE_GOOD;
	OV_ACCESS				access		=	OV_AC_NONE;
	OV_ELEMENT				referencedElement;

	// Forward direction
	if (direction & OPCUA_OVTRAFO_ADDHASPROPERTY_FORWARD){
		if(pNode->elemtype == OV_ET_OBJECT){
			referencedElement.elemtype = OV_ET_NONE;
			ov_element_getnextpart(pNode, &referencedElement, OV_ET_VARIABLE | OV_ET_MEMBER);
			while(referencedElement.elemtype != OV_ET_NONE){
				// Ignore idH and idL variables
				if(referencedElement.elemtype == OV_ET_VARIABLE &&
						(ov_string_compare(referencedElement.elemunion.pvar->v_identifier, "idH") == OV_STRCMP_EQUAL ||
						 ov_string_compare(referencedElement.elemunion.pvar->v_identifier, "idL") == OV_STRCMP_EQUAL)){
					ov_element_getnextpart(pNode, &referencedElement, OV_ET_VARIABLE | OV_ET_MEMBER);
					continue;
				}
				access = opcua_helpers_getAccess(&referencedElement);
				if(access & OV_AC_READ){
					// Check whether child has a specific transformation
					if(!opcua_ovTrafo_addReferenceToSpecificObject(pServer, referencedElement.pobj, node))
						// Child has no specific transformation --> use generic interface
						statusCode |= opcua_ovTrafo_addReference(&referencedElement,
							UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), UA_TRUE, node);
				}
				ov_element_getnextpart(pNode, &referencedElement, OV_ET_VARIABLE | OV_ET_MEMBER);
			}
		}
	}
	// Backward / inverse direction
		if (direction & OPCUA_OVTRAFO_ADDHASPROPERTY_BACKWARD){
		if(pNode->elemtype == OV_ET_VARIABLE || pNode->elemtype == OV_ET_MEMBER){
			referencedElement.elemtype = OV_ET_OBJECT;
			referencedElement.pobj = pNode->pobj;
			access = opcua_helpers_getAccess(&referencedElement);
			if(access & OV_AC_READ){
				statusCode |= opcua_ovTrafo_addReference(&referencedElement,
						UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), UA_FALSE, node);
			}
		}
	}
	return statusCode;
}

/******************************************************************
 * helper for HasComponent
 *****************************************************************/

static UA_StatusCode opcua_ovTrafo_addHasComponent(OV_INSTPTR_opcua_server pServer, OV_ELEMENT* pNode, UA_Node* node, OV_UINT direction){
	UA_StatusCode statusCode = UA_STATUSCODE_GOOD;
	OV_ACCESS				access		=	OV_AC_NONE;
	OV_ELEMENT				referencedElement;

	//	maps to variables in objects
	// Forward direction
	if (direction & OPCUA_OVTRAFO_ADDHASCOMPONENT_FORWARD){
		if(pNode->elemtype == OV_ET_OBJECT){
			referencedElement.elemtype = OV_ET_NONE;
			ov_element_getnextpart(pNode, &referencedElement, OV_ET_OBJECT);
			while(referencedElement.elemtype != OV_ET_NONE){
				access = opcua_helpers_getAccess(&referencedElement);
				if(access & OV_AC_READ){
					// Check whether child has a specific transformation
					if(!opcua_ovTrafo_addReferenceToSpecificObject(pServer, referencedElement.pobj, node))
						// Child has no specific transformation --> use generic interface
						statusCode |= opcua_ovTrafo_addReference(&referencedElement,
								UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT), UA_TRUE, node);
				}
				ov_element_getnextpart(pNode, &referencedElement, OV_ET_OBJECT);
			}
		}
	}
	// Backward / inverse direction
	if (direction & OPCUA_OVTRAFO_ADDHASCOMPONENT_BACKWARD){
		if(pNode->elemtype == OV_ET_OBJECT){
			if(pNode->pobj->v_pouterobject){
				referencedElement.elemtype = OV_ET_OBJECT;
				referencedElement.pobj = pNode->pobj->v_pouterobject;
				access = opcua_helpers_getAccess(&referencedElement);
				if(access & OV_AC_READ){
					statusCode |= opcua_ovTrafo_addReference(&referencedElement,
							UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT), UA_FALSE, node);
				}
			}
		}
	}
	return statusCode;
}

/******************************************************************
 * helper for OVReferences
 *****************************************************************/

static UA_StatusCode opcua_ovTrafo_getOvReferences(OV_ELEMENT* pNode, UA_Node* node, OV_UINT direction){
	OV_ACCESS				access		=	OV_AC_NONE;
	OV_ELEMENT				linkElement;
	OV_ELEMENT				referencedElement;
	UA_StatusCode statusCode = UA_STATUSCODE_GOOD;
	OV_STRING path = NULL;

	// Forward direction
	if (direction & OPCUA_OVTRAFO_ADDOVREFERENCES_FORWARD){
		if(pNode->elemtype == OV_ET_OBJECT){
			linkElement.elemtype = OV_ET_NONE;
			ov_element_getnextpart(pNode, &linkElement, OV_ET_PARENTLINK);
			while(linkElement.elemtype != OV_ET_NONE){
				if(linkElement.elemunion.passoc->v_assoctype == OV_AT_ONE_TO_MANY){
					referencedElement.elemtype = OV_ET_OBJECT;
					Ov_Association_ForEachChild(linkElement.elemunion.passoc, pNode->pobj, referencedElement.pobj){
						access = opcua_helpers_getAccess(&referencedElement);
						if(access & OV_AC_READ){
							ov_memstack_lock();
							path = ov_path_getcanonicalpath(Ov_StaticPtrCast(ov_object, linkElement.elemunion.passoc), 2);
							statusCode |= opcua_ovTrafo_addReference(&referencedElement,
										UA_NODEID_STRING(OPCUA_OVTRAFO_DEFAULTNSINDEX, path), UA_TRUE, node);
							ov_memstack_unlock();
						}
					}
				} else if(linkElement.elemunion.passoc->v_assoctype == OV_AT_ONE_TO_ONE) {
					referencedElement.elemtype = OV_ET_OBJECT;
					referencedElement.pobj = Ov_Association_GetChild(linkElement.elemunion.passoc, pNode->pobj);
					if(referencedElement.pobj){
						access = opcua_helpers_getAccess(&referencedElement);
						if(access & OV_AC_READ){
							ov_memstack_lock();
							path = ov_path_getcanonicalpath(Ov_StaticPtrCast(ov_object, linkElement.elemunion.passoc), 2);
							statusCode |= opcua_ovTrafo_addReference(&referencedElement,
										UA_NODEID_STRING(OPCUA_OVTRAFO_DEFAULTNSINDEX, path), UA_TRUE, node);
							ov_memstack_unlock();
						}
					}
				}
				ov_element_getnextpart(pNode, &linkElement, OV_ET_PARENTLINK);
			}
		}
	}
	// Backward / inverse direction
	if (direction & OPCUA_OVTRAFO_ADDOVREFERENCES_BACKWARD){
		if(pNode->elemtype == OV_ET_OBJECT){
			linkElement.elemtype = OV_ET_NONE;
			ov_element_getnextpart(pNode, &linkElement, OV_ET_CHILDLINK);
			while(linkElement.elemtype != OV_ET_NONE){
				referencedElement.elemtype = OV_ET_OBJECT;
				referencedElement.pobj = Ov_Association_GetParent(linkElement.elemunion.passoc, pNode->pobj);
				if(referencedElement.pobj){
					access = opcua_helpers_getAccess(&referencedElement);
					if(access & OV_AC_READ){
						ov_memstack_lock();
						path = ov_path_getcanonicalpath(Ov_StaticPtrCast(ov_object, linkElement.elemunion.passoc), 2);
						statusCode |= opcua_ovTrafo_addReference(&referencedElement,
									UA_NODEID_STRING(OPCUA_OVTRAFO_DEFAULTNSINDEX, path), UA_FALSE, node);
						ov_memstack_unlock();
					}
				}
			ov_element_getnextpart(pNode, &linkElement, OV_ET_CHILDLINK);
			}
		}
	}

	return UA_STATUSCODE_GOOD;
}

//TODO add inverse HasSubtype (HasSupertype) reference for classes.
//TODO variable and port values are NULL, BadNotReadable
//TODO method propertys are not displayed correctly --> servicesOPCUAInterface
OV_DLLFNCEXPORT UA_StatusCode opcua_ovTrafo_addReferences(OV_INSTPTR_opcua_server pServer, UA_Node *node, OV_UINT direction){
	// references
	OV_ELEMENT 				pNode;
	OV_PATH					nodePath;
	UA_StatusCode			result = UA_STATUSCODE_GOOD;

	ov_memstack_lock();
	if(opcua_helpers_resolveNodeIdToPath(node->nodeId, &nodePath) != UA_STATUSCODE_GOOD){
		return UA_STATUSCODE_BADNODEIDUNKNOWN;
	}
	pNode = nodePath.elements[nodePath.size - 1];
	ov_memstack_unlock();

	/*********************************************************************************************************************************
	 * fill in results for all reference types
	 ********************************************************************************************************************************/
	// HasProperty
	result |= opcua_ovTrafo_addHasProperty(pServer, &pNode, node, direction);
	// HasComponent
	result |= opcua_ovTrafo_addHasComponent(pServer, &pNode, node, direction);
	// Organizes
	result |= opcua_ovTrafo_addOrganizes(pServer, &pNode, node, direction);
	// HasTypeDefinition
	result |= opcua_ovTrafo_addHasTypeDefinition(&pNode, node, direction);
	// HasSubtype
	result |= opcua_ovTrafo_addHasSubtype(&pNode, node, direction);
	// OvReferences
	result |= opcua_ovTrafo_getOvReferences(&pNode, node, direction);

	return result;
}

static void opcua_ovTrafo_deleteNodestore(void *context){
}

static void opcua_ovTrafo_deleteNode(void * context, UA_Node *node){
	if (node){
		UA_Node_deleteMembers(node);
	}
	UA_free(node);
}
static void opcua_ovTrafo_releaseNode(void *context, const UA_Node *node){
	opcua_ovTrafo_deleteNode(context, (UA_Node*)node);
}

static void opcua_ovTrafo_iterate(void *context, UA_NodestoreVisitor visitor, void* visitorHandle){
}
static UA_Node * opcua_ovTrafo_newNode(void * context, UA_NodeClass nodeClass){
    return NULL;
}


static const UA_Node * opcua_ovTrafo_getOvReferenceNode(void * context, const UA_NodeId *nodeId){
	UA_Node 				*newNode = NULL;
	UA_StatusCode 			result = UA_STATUSCODE_GOOD;


	newNode = (UA_Node*)UA_calloc(1, sizeof(UA_ReferenceTypeNode));
	newNode->nodeClass = UA_NODECLASS_REFERENCETYPE;

	// Basic Attribute
	// BrowseName
	UA_QualifiedName_init(&newNode->browseName);
	newNode->browseName.name = UA_String_fromChars("||ovReferences");
	newNode->browseName.namespaceIndex = nodeId->namespaceIndex;

	// Description
	UA_LocalizedText_init(&newNode->description);
	newNode->description.locale = UA_String_fromChars("en");
	newNode->description.text = UA_String_fromChars("||ovReferences");

	// DisplayName
	UA_LocalizedText_init(&newNode->displayName);
	newNode->displayName.locale = UA_String_fromChars("en");
	newNode->displayName.text = UA_String_fromChars("||ovReferences");

	// NodeId
	UA_NodeId_init(&newNode->nodeId);
	newNode->nodeId.identifierType = nodeId->identifierType;
	newNode->nodeId.namespaceIndex = newNode->browseName.namespaceIndex;

	UA_String_copy(&nodeId->identifier.string, &newNode->nodeId.identifier.string);

	// WriteMask
	UA_UInt32 writeMask = 0;
	newNode->writeMask 	= writeMask;


	// isAbstract
	((UA_ReferenceTypeNode*)newNode)->isAbstract = UA_FALSE;
	// symmetric
	((UA_ReferenceTypeNode*)newNode)->symmetric = UA_FALSE;
	// inverseName
	//NOTIMPLEMENTED


	// addReferences
	OV_INSTPTR_ov_object pParent = ov_path_getobjectpointer("/acplt/ov/association",2);
	OV_INSTPTR_ov_class pParentClass = Ov_StaticPtrCast(ov_class, pParent);
	OV_INSTPTR_ov_object pChild = NULL;
	OV_ACCESS				access		=	OV_AC_NONE;
	OV_ELEMENT				referencedElement;
	Ov_ForEachChild(ov_instantiation, pParentClass, pChild){
		referencedElement.elemtype = OV_ET_OBJECT;
		referencedElement.pobj = pChild;
		access = opcua_helpers_getAccess(&referencedElement);
		if(access & OV_AC_READ){
			result |= opcua_ovTrafo_addReference(context, &referencedElement,
					UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE), UA_TRUE, newNode);
		}
	}
	// addParentReference
	referencedElement.elemtype = OV_ET_OBJECT;
	referencedElement.pobj = pParent;
	access = opcua_helpers_getAccess(&referencedElement);
	if(access & OV_AC_READ){
		result |= opcua_helpers_addReference(newNode, NULL, UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE), UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_NONHIERARCHICALREFERENCES), UA_NODECLASS_REFERENCETYPE, UA_FALSE);
	}

	return newNode;
}


static const UA_Node * opcua_ovTrafo_getNode(void * context, const UA_NodeId *nodeId){
	UA_Node 				*newNode = NULL;
	UA_StatusCode 			result = UA_STATUSCODE_GOOD;
	OV_PATH 				path;
	OV_INSTPTR_ov_object	pobj = NULL;
	OV_INSTPTR_ov_object	pobjtemp = NULL;
	OV_VTBLPTR_ov_object	pVtblObj = NULL;
	OV_ACCESS				access;
	OV_ELEMENT				element;
	OV_ANY					value = OV_ANY_INIT;
	OV_ANY					emptyAny = OV_ANY_INIT;


	// check for ovReference Node
	if (nodeId->identifierType == UA_NODEIDTYPE_STRING){
		OV_STRING tmpString = NULL;
		opcua_helpers_copyUAStringToOV(nodeId->identifier.string, &tmpString);
		if (ov_string_compare(tmpString, OPCUA_OVTRAFO_OVREFERENCES) == OV_STRCMP_EQUAL){
			// Transformation ovReferenceNode
			return opcua_ovTrafo_getOvReferenceNode(context, nodeId);
		}
	}


	ov_memstack_lock();
	result = opcua_helpers_resolveNodeIdToPath(*nodeId, &path);
	if(result != UA_STATUSCODE_GOOD){
		ov_memstack_unlock();
		return NULL;
	}
	element = path.elements[path.size-1];
	ov_memstack_unlock();

	result = opcua_helpers_getVtblPointerAndCheckAccess(&element, &pobj, &pVtblObj, &access);
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
	newNode->browseName.namespaceIndex = OPCUA_OVTRAFO_DEFAULTNSINDEX;

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
			writeMask |= UA_WRITEMASK_BROWSENAME;
			writeMask |= UA_WRITEMASK_DISPLAYNAME;

		}
		if(access & OV_AC_RENAMEABLE){
			writeMask |= (1<<14);	/*	NodeId	*/
		}
	}
	newNode->writeMask 	= writeMask;

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
					((UA_VariableNode*)newNode)->arrayDimensions = NULL; //UA_Array_new(((UA_VariableNode*)newNode)->arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);	/*	scalar or one dimension	*/
					break;
				default:
					if(((OV_INSTPTR_ov_variable)pobjtemp)->v_veclen == 1){ //TODO use var type and vector mask
						/*	scalar	*/
						((UA_VariableNode*)newNode)->arrayDimensionsSize = 0;
						((UA_VariableNode*)newNode)->arrayDimensions = NULL; //UA_Array_new(((UA_VariableNode*)newNode)->arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);	/*	scalar or one dimension	*/
					} else {
						/*	vector	*/
						((UA_VariableNode*)newNode)->arrayDimensionsSize = 1;
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
				((UA_VariableNode*)newNode)->valueRank = UA_VALUERANK_SCALAR_OR_ONE_DIMENSION;
				result = UA_STATUSCODE_GOOD;
				break;
			default:
				if((((OV_INSTPTR_ov_variable)pobj)->v_vartype) & OV_VT_ISVECTOR){
					((UA_VariableNode*)newNode)->valueRank = UA_VALUERANK_ONE_DIMENSION;
					result = UA_STATUSCODE_GOOD;
				} else {
					((UA_VariableNode*)newNode)->valueRank = UA_VALUERANK_SCALAR;
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
		UA_Byte accessLevel = 0;
		if(access & OV_AC_READ){
			accessLevel |= UA_ACCESSLEVELMASK_READ;
		}
		if(access & OV_AC_WRITE){
			accessLevel |= UA_ACCESSLEVELMASK_WRITE;
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

	OV_UINT direction = OPCUA_OVTRAFO_ADDHASPROPERTY_FORWARD
					|	OPCUA_OVTRAFO_ADDHASPROPERTY_BACKWARD
					|	OPCUA_OVTRAFO_ADDHASCOMPONENT_FORWARD
					|	OPCUA_OVTRAFO_ADDHASCOMPONENT_BACKWARD
					|	OPCUA_OVTRAFO_ADDORGANIZES_FORWARD
					|	OPCUA_OVTRAFO_ADDORGANIZES_BACKWARD
					|	OPCUA_OVTRAFO_ADDHASTYPEDEFINITION_FORWARD
					|	OPCUA_OVTRAFO_ADDHASTYPEDEFINITION_BACKWARD
					|	OPCUA_OVTRAFO_ADDHASSUBTYPE_FORWARD
					|	OPCUA_OVTRAFO_ADDHASSUBTYPE_BACKWARD
					| 	OPCUA_OVTRAFO_ADDOVREFERENCES_FORWARD
					|	OPCUA_OVTRAFO_ADDOVREFERENCES_BACKWARD;

	opcua_ovTrafo_addReferences(context, newNode, direction);

	return newNode;
}
static UA_StatusCode opcua_ovTrafo_getNodeCopy(void *context, const UA_NodeId *nodeId, UA_Node ** nodeOut){
	UA_Node* node = (UA_Node*) opcua_ovTrafo_getNode(context, nodeId);
	if(node == NULL)
		return UA_STATUSCODE_BADNODEIDUNKNOWN;
	*nodeOut = node;
	return UA_STATUSCODE_GOOD;
}
static UA_StatusCode opcua_ovTrafo_removeNode(void *context, const UA_NodeId *nodeId){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}
static UA_StatusCode opcua_ovTrafo_insertNode(void *context, UA_Node *node, UA_NodeId *parrentNode){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}
static UA_StatusCode opcua_ovTrafo_replaceNode(void *context, UA_Node *node){
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
	opcua_ovTrafo_releaseNode(context, node);
	return UA_STATUSCODE_GOOD;
}

UA_NodestoreInterface*
opcua_ovTrafo_new(OV_INSTPTR_opcua_server context) {
	UA_NodestoreInterface* nsi = (UA_NodestoreInterface*)UA_malloc(sizeof(UA_NodestoreInterface));
	if(nsi == NULL)
		return NULL;
    nsi->context =        	context;
    nsi->deleteNodestore =  opcua_ovTrafo_deleteNodestore;
    nsi->newNode =       	opcua_ovTrafo_newNode;
    nsi->deleteNode =    	opcua_ovTrafo_deleteNode;
    nsi->insertNode =       opcua_ovTrafo_insertNode;
    nsi->getNode =          opcua_ovTrafo_getNode;
    nsi->getNodeCopy =      opcua_ovTrafo_getNodeCopy;
    nsi->replaceNode =      opcua_ovTrafo_replaceNode;
    nsi->removeNode =       opcua_ovTrafo_removeNode;
    nsi->iterate =          opcua_ovTrafo_iterate;
    nsi->releaseNode =      opcua_ovTrafo_releaseNode;
    return nsi;
}
void
opcua_ovTrafo_delete(UA_NodestoreInterface * nodestoreInterface){
	nodestoreInterface->context = NULL;
	UA_free(nodestoreInterface);
}
