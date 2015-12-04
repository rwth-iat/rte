/*
*   $Id: ov_element.c,v 1.13 2007-04-25 13:59:03 martin Exp $
*
*   Copyright (C) 1998-1999
*   Lehrstuhl fuer Prozessleittechnik,
*   RWTH Aachen, D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
*
*   This file is part of the ACPLT/OV Package which is licensed as open
*   source under the Artistic License; you can use, redistribute and/or
*   modify it under the terms of that license.
*
*   You should have received a copy of the Artistic License along with
*   this Package; see the file ARTISTIC-LICENSE. If not, write to the
*   Copyright Holder.
*
*   THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
*   WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
*   OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/
/*
*	History:
*	--------
*	21-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

#define OV_COMPILE_LIBOV

#include "libov/ov_element.h"
#include "libov/ov_macros.h"

/*	----------------------------------------------------------------------	*/

/*
*	Search child of an element ("parent/child" in a path)
*/
OV_DLLFNCEXPORT OV_RESULT ov_element_searchchild(
	const OV_ELEMENT	*pparent,
	OV_ELEMENT			*pchild,
	OV_STRING			identifier
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_object	pobj;
	OV_INSTPTR_ov_domain	pdom;
	/*
	*	check parameters
	*/
	if(!pparent || !pchild || !identifier) {
		return OV_ERR_BADPARAM;
	}
	/*
	*	only domain objects may have children
	*/
	if(pparent->elemtype != OV_ET_OBJECT) {
		goto NOCHILD;
	}
	pdom = Ov_DynamicPtrCast(ov_domain, pparent->pobj);
	if(!pdom) {
		goto NOCHILD;
	}
	/*
	*	object is a domain, search for the child
	*/
	pobj = Ov_SearchChild(ov_containment, pdom, identifier);
	if(!pobj) {
		goto NOCHILD;
	}
	pchild->elemtype = OV_ET_OBJECT;
	pchild->pobj = pobj;
	return OV_ERR_OK;
	/*
	*	no child found; exit
	*/
NOCHILD:
	pchild->elemtype = OV_ET_NONE;
	pchild->pobj = NULL;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Search part of an element ("parent.part" in a path)
*/
OV_DLLFNCEXPORT OV_RESULT ov_element_searchpart(
	OV_ELEMENT	*pparent,
	OV_ELEMENT			*ppart,
	OV_ELEM_TYPE		mask,
	OV_STRING			identifier
) {
	/*
	*	check parameters
	*/
	if(!pparent || !ppart || !identifier) {
		return OV_ERR_BADPARAM;
	}
	/*
	*	switch based on the type of the parent element
	*/
	switch(pparent->elemtype) {
	case OV_ET_OBJECT:
		/*
		*	search part of an object
		*/
		ppart->pobj = pparent->pobj;
		return ov_element_searchpart_object(pparent->pobj, ppart, mask, identifier);
	case OV_ET_VARIABLE:
		/*
		*	search part of a variable
		*/
		pparent->pvar = pparent->elemunion.pvar;
		/* fall into... */
	case OV_ET_MEMBER:
		/*
		*	search part of a structure member
		*/
		ppart->pobj = pparent->pobj;
		ppart->pvar = pparent->pvar;
		return ov_element_searchpart_variable(pparent->pobj,
			pparent->elemunion.pvar, pparent->pvalue, ppart, mask, identifier);
	default:
		/*
		*	element has no parts
		*/
		break;
	}
	/*
	*	no part found; exit
	*/
	ppart->elemtype = OV_ET_NONE;
	ppart->pobj = NULL;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get next child of an element ("parent/child" in a path)
*/
OV_DLLFNCEXPORT OV_RESULT ov_element_getnextchild(
	const OV_ELEMENT	*pparent,
	OV_ELEMENT			*pchild
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_domain	pdom;
	/*
	*	check parameters
	*/
	if(!pparent || !pchild) {
		return OV_ERR_BADPARAM;
	}
	if((pchild->elemtype!=OV_ET_NONE) && (pchild->elemtype!=OV_ET_OBJECT)) {
		return OV_ERR_BADPARAM;
	}
	/*
	*	only domain objects may have children
	*/
	if(pparent->elemtype != OV_ET_OBJECT) {
		goto NOCHILD;
	}
	pdom = Ov_DynamicPtrCast(ov_domain, pparent->pobj);
	if(!pdom) {
		goto NOCHILD;
	}
	/*
	*	object is a domain, get the next child
	*/
	if(pchild->pobj && (pchild->elemtype==OV_ET_OBJECT)) {
		pchild->pobj = Ov_GetNextChild(ov_containment, pchild->pobj);
	} else {
		pchild->pobj = Ov_GetFirstChild(ov_containment, pdom);
	}
	if(pchild->pobj) {
		pchild->elemtype = OV_ET_OBJECT;
	} else {
		pchild->elemtype = OV_ET_NONE;
	}
	return OV_ERR_OK;
	/*
	*	no child found; exit
	*/
NOCHILD:
	pchild->elemtype = OV_ET_NONE;
	pchild->pobj = NULL;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get next part of an element ("parent.part" in a path)
*/
OV_DLLFNCEXPORT OV_RESULT ov_element_getnextpart(
	OV_ELEMENT	*pparent,
	OV_ELEMENT			*ppart,
	OV_ELEM_TYPE		mask
) {
	/*
	*	check parameters
	*/
	if(!pparent || !ppart) {
		return OV_ERR_BADPARAM;
	}
	/*
	*	switch based on the type of the parent element
	*/
	switch(pparent->elemtype) {
	case OV_ET_OBJECT:
		/*
		*	get next part of an object
		*/
		ppart->pobj = pparent->pobj;
		return ov_element_getnextpart_object(pparent->pobj, ppart, mask);
	case OV_ET_VARIABLE:
		/*
		*	get next part of a variable
		*/
		/* fall into... */
		pparent->pvar = pparent->elemunion.pvar;
	case OV_ET_MEMBER:
		/*
		*	get next part of a structure member
		*/
		ppart->pobj = pparent->pobj;
		ppart->pvar = pparent->pvar;
		return ov_element_getnextpart_variable(pparent->pobj,
			pparent->elemunion.pvar, pparent->pvalue, ppart, mask);
	default:
		/*
		*	element has no parts
		*/
		break;
	}
	/*
	*	no part found; exit
	*/
	ppart->elemtype = OV_ET_NONE;
	ppart->pobj = NULL;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Search part of an object (subroutine)
*/
OV_RESULT ov_element_searchpart_object(
	OV_INSTPTR_ov_object	pobj,
	OV_ELEMENT				*ppart,
	OV_ELEM_TYPE			mask,
	OV_STRING				identifier
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_class			pclass;
	/*
	*	search in all classes
	*/
	for(pclass=Ov_GetParent(ov_instantiation, pobj); pclass;
		pclass=Ov_GetParent(ov_inheritance, pclass)
	) {
		if(mask & (OV_ET_VARIABLE | OV_ET_OBJECT | OV_ET_OPERATION)) {
			/*
			*	search for variable, part or operation
			*/
			ppart->elemunion.pobj = Ov_SearchChild(ov_containment, pclass, identifier);
			if(ppart->elemunion.pobj) {
				/*
				*	We found a part with the identifier, now check the type
				*/
				if((mask & OV_ET_VARIABLE)
					&& Ov_DynamicPtrCast(ov_variable, ppart->elemunion.pobj)
				) {
					ppart->elemtype = OV_ET_VARIABLE;
					if(ppart->elemunion.pvar->v_varprops & OV_VP_DERIVED) {
						ppart->pvalue = NULL;
					} else if(ppart->elemunion.pvar->v_varprops & OV_VP_STATIC) {
						ppart->pvalue = ((OV_BYTE*)pclass)+Ov_GetInstSize(ov_class)
							+ppart->elemunion.pvar->v_offset;
					} else {
						ppart->pvalue = ((OV_BYTE*)pobj)+ppart->elemunion.pvar->v_offset;
					}
					return OV_ERR_OK;
				}
				if((mask & OV_ET_OBJECT)
					&& Ov_DynamicPtrCast(ov_part, ppart->elemunion.pobj)
				) {
					ppart->elemtype = OV_ET_OBJECT;
					ppart->pobj = (OV_INSTPTR)(((OV_BYTE*)pobj)
						+ppart->elemunion.ppart->v_offset);
					return OV_ERR_OK;
				}
				if((mask & OV_ET_OPERATION)
					&& Ov_DynamicPtrCast(ov_operation, ppart->elemunion.pobj)
				) {
					ppart->elemtype = OV_ET_OPERATION;
					return OV_ERR_OK;
				}
				/*
				*	part is not of specified type
				*/
				goto NOPART;
			}
		}
		if(mask & OV_ET_PARENTLINK) {
			/*
			*	search for a parent link with this name
			*/
			Ov_ForEachChild(ov_parentrelationship, pclass, ppart->elemunion.passoc) {
				if(!strcmp(ppart->elemunion.passoc->v_childrolename, identifier)) {
					/*
					*	parent link found
					*/
					ppart->elemtype = OV_ET_PARENTLINK;
					return OV_ERR_OK;
				}
			}
		}
		if(mask & OV_ET_CHILDLINK) {
			/*
			*	search for a child link with this name
			*/
			Ov_ForEachChild(ov_childrelationship, pclass, ppart->elemunion.passoc) {
				if(!strcmp(ppart->elemunion.passoc->v_parentrolename, identifier)) {
					/*
					*	child link found
					*/
					ppart->elemtype = OV_ET_CHILDLINK;
					return OV_ERR_OK;
				}
			}
		}
	}
	/*
	*	part not found
	*/
NOPART:
	ppart->elemtype = OV_ET_NONE;
	ppart->pobj = NULL;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Search part of a variable or variable member (subroutine)
*/
OV_RESULT ov_element_searchpart_variable(
	OV_INSTPTR_ov_object	pobj,
	OV_INSTPTR_ov_variable	pvar,
	OV_BYTE					*pvalue,
	OV_ELEMENT				*ppart,
	OV_ELEM_TYPE			mask,
	OV_STRING				identifier
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_structure			pstruct;

	pstruct=Ov_GetParent(ov_construction, pvar);
	if (!pstruct) goto NOPART;
	if(mask & OV_ET_MEMBER) {
		/*
		*  search for variable or member
		*/
		ppart->elemunion.pobj = Ov_SearchChild(ov_containment, pstruct, identifier);
		if(ppart->elemunion.pobj) {
			/*
			*	We found a part with the identifier, now check the type
			*/
			if (Ov_DynamicPtrCast(ov_variable, ppart->elemunion.pobj))
			{
				ppart->elemtype = OV_ET_MEMBER;
				if(pvar->v_varprops & OV_VP_DERIVED) {
					ppart->pvalue = NULL;
				} else {
					ppart->pvalue = pvalue + ppart->elemunion.pvar->v_offset;
				}
				return OV_ERR_OK;
			}
		}
	}
	/*
	*	part not found
	*/
NOPART:
	ppart->elemtype = OV_ET_NONE;
	ppart->pobj = NULL;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get next part of an object (subroutine)
*/
OV_RESULT ov_element_getnextpart_object(
	OV_INSTPTR_ov_object	pobj,
	OV_ELEMENT				*ppart,
	OV_ELEM_TYPE			mask
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_class		pclass = NULL;
OV_INSTPTR_ov_object pdum;
	/*
	*	try until we find the right thing
	*/
	while(TRUE) {
		/*
		*	switch based on the type of the part
		*/
		switch(ppart->elemtype) {
		case OV_ET_NONE:
			/*
			*	get the very first part (variable, part, operation)...
			*/
			pclass = Ov_GetParent(ov_instantiation, pobj);
CONTINUE4:	ppart->elemunion.pobj = Ov_GetFirstChild(ov_containment, pclass);
			goto CONTINUE1;
		case OV_ET_VARIABLE:
		case OV_ET_OBJECT:
		case OV_ET_OPERATION:
			/*
			*	get the next part (variable, part, operation)...
			*/
			pdum = (OV_INSTPTR_ov_object)Ov_GetParent(ov_containment, ppart->elemunion.pobj);
			pclass = Ov_DynamicPtrCast(ov_class, pdum);
			if(!pclass) {
				Ov_Warning("internal error");
				return OV_ERR_GENERIC;
			}
			ppart->elemunion.pobj = Ov_GetNextChild(ov_containment,
				ppart->elemunion.pobj);
CONTINUE1:		if(ppart->elemunion.pobj) {
				/*
				*	we found the next part, figure out the type
				*/
				if(Ov_CanCastTo(ov_variable, ppart->elemunion.pobj)) {
					ppart->elemtype = OV_ET_VARIABLE;
					break;
				}
				if(Ov_CanCastTo(ov_part, ppart->elemunion.pobj)) {
					ppart->elemtype = OV_ET_OBJECT;
					break;
				}
				if(Ov_CanCastTo(ov_operation, ppart->elemunion.pobj)) {
					ppart->elemtype = OV_ET_OPERATION;
					break;
				}
				/*
				*	this is strange...
				*/
				Ov_Warning("internal error");
				return OV_ERR_GENERIC;
			}
			/*
			*	no more variables, parts or operations, get first parent link
			*/
			ppart->elemtype = OV_ET_PARENTLINK;
			ppart->elemunion.passoc = Ov_GetFirstChild(ov_parentrelationship,
				pclass);
			goto CONTINUE2;
		case OV_ET_PARENTLINK:
			/*
			*	get next part (parent link)...
			*/
			pclass = Ov_GetParent(ov_parentrelationship, ppart->elemunion.passoc);
			ppart->elemunion.passoc = Ov_GetNextChild(ov_parentrelationship,
				ppart->elemunion.passoc);
CONTINUE2:		if(ppart->elemunion.passoc) {
				/*
				*	we found the next part
				*/
				break;
			}
			/*
			*	no more parent links, get first child link
			*/
			ppart->elemtype = OV_ET_CHILDLINK;
			ppart->elemunion.passoc = Ov_GetFirstChild(ov_childrelationship,
				pclass);
			goto CONTINUE3;
		case OV_ET_CHILDLINK:
			/*
			*	get next part (child link)...
			*/
			pclass = Ov_GetParent(ov_childrelationship, ppart->elemunion.passoc);
			ppart->elemunion.passoc = Ov_GetNextChild(ov_childrelationship,
				ppart->elemunion.passoc);
CONTINUE3:		if(ppart->elemunion.passoc) {
				/*
				*	we found the next part
				*/
				break;
			}
			/*
			*	no more child links, go to base class
			*/
			pclass = Ov_GetParent(ov_inheritance, pclass);
			if(pclass) {
				/*
				*	continue with first variable of this class
				*/
				goto CONTINUE4;
			}
			/*
			*	we are through, no more parts
			*/
			ppart->elemtype = OV_ET_NONE;
			ppart->elemunion.pobj = NULL;
			return OV_ERR_OK;
		}	/* switch */
		/*
		*	if we got an element of the right type, return
		*/
		if(ppart->elemtype & mask) {
			/*
			*	set identifier and the variable value or object pointer if appropriate
			*/
			switch(ppart->elemtype) {
			case OV_ET_VARIABLE:
				if(ppart->elemunion.pvar->v_varprops & OV_VP_DERIVED) {
					ppart->pvalue = NULL;
				} else if(ppart->elemunion.pvar->v_varprops & OV_VP_STATIC) {
					Ov_WarnIfNot(pclass);
					ppart->pvalue = ((OV_BYTE*)pclass)+Ov_GetInstSize(ov_class)
						+ppart->elemunion.pvar->v_offset;
				} else {
					ppart->pvalue = ((OV_BYTE*)pobj)+ppart->elemunion.pvar->v_offset;
				}
				break;
			case OV_ET_OBJECT:
				ppart->pobj = (OV_INSTPTR)(((OV_BYTE*)pobj)
					+ppart->elemunion.ppart->v_offset);
				break;
			default:
				break;
			}
			return OV_ERR_OK;
		}
	}	/* while */
#if OV_SYSTEM_MC164
	return OV_ERR_OK;
#endif
}

/*	----------------------------------------------------------------------	*/

/*
*	Get next part of a variable or variable member (subroutine)
*/
OV_RESULT ov_element_getnextpart_variable(
	OV_INSTPTR_ov_object	pobj,
	OV_INSTPTR_ov_variable	pvar,
	OV_BYTE					*pvalue,
	OV_ELEMENT				*ppart,
	OV_ELEM_TYPE			mask
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_structure		pstruct;
	/*
	*	there can only be members of a structure as parts of a variable
	*/
	if(mask & OV_ET_MEMBER) {
		/*
		*	switch based on the type of the part
		*/
		switch(ppart->elemtype) {
		case OV_ET_NONE:
			/*
			*	get the very first part (first member of a structure)...
			*/
			pstruct=Ov_GetParent(ov_construction, pvar);
			if (!pstruct) goto NOPART;
			ppart->elemunion.pobj = Ov_GetFirstChild(ov_containment, pstruct);
			break;
		case OV_ET_MEMBER:
			ppart->elemunion.pobj = Ov_GetNextChild(ov_containment,	ppart->elemunion.pobj);
			break;
		default:
			/*
			*	this is strange...
			*/
			Ov_Warning("internal error");
			goto NOPART;
		}
		if (!ppart->elemunion.pobj) goto NOPART;
		ppart->elemtype = OV_ET_MEMBER;

		if(pvar->v_varprops & OV_VP_DERIVED) {
			ppart->pvalue = NULL;
		} else {
			ppart->pvalue = pvalue + ppart->elemunion.pvar->v_offset;
		}
		return OV_ERR_OK;
	}

	/*
	*	no more parts of a variable
	*/
NOPART:
	ppart->elemtype = OV_ET_NONE;
	ppart->elemunion.pobj = NULL;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/**
*	Get the identifier of an element
*	@param pelem Element to check for the name
*/
OV_DLLFNCEXPORT OV_STRING ov_element_getidentifier(
	const OV_ELEMENT			*pelem
) {
	if(pelem) {
		/*
		*	return identifier
		*/
		switch(pelem->elemtype) {
		case OV_ET_OBJECT:
			return pelem->pobj->v_identifier;
		case OV_ET_VARIABLE:
		case OV_ET_MEMBER:
			return pelem->elemunion.pvar->v_identifier;
		case OV_ET_PARENTLINK:
			return pelem->elemunion.passoc->v_childrolename;
		case OV_ET_CHILDLINK:
			return pelem->elemunion.passoc->v_parentrolename;
		case OV_ET_OPERATION:
			return pelem->elemunion.pop->v_identifier;
		default:
			break;
		}
	}
	return "";
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

