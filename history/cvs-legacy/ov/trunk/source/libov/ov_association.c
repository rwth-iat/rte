/*
*   $Id: ov_association.c,v 1.4 1999-08-28 15:55:54 dirk Exp $
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
*	19-Jun-1998 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	08-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*	23-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Optimized (use of macros).
*/

#define OV_COMPILE_LIBOV

#include "libov/ov_association.h"
#include "libov/ov_logfile.h"
#include "libov/ov_macros.h"
#include "libov/ov_time.h"
#include "libov/ov_string.h"

/*	----------------------------------------------------------------------	*/

/*
*	Load an association into the database
*/
OV_RESULT ov_association_load(
	OV_ASSOCIATION_DEF		*passocdef,
	OV_INSTPTR_ov_domain	pparent
) {
	/*
	*	local variables
	*/
	OV_RESULT					result;
	OV_INSTPTR_ov_association	passoc;
	OV_INSTPTR_ov_class			pparentclass, pchildclass;
	/*
	*	look for the parent class definition definition
	*/
	pparentclass = ov_class_search(passocdef->parentclassname);
	if(!pparentclass) {
		return OV_ERR_UNKNOWNCLASSDEF;
	}
	/*
	*	look for the child class definition definition
	*/
	pchildclass = ov_class_search(passocdef->childclassname);
	if(!pchildclass) {
		return OV_ERR_UNKNOWNCLASSDEF;
	}
	/*
	*	test whether the association already exists
	*/
	Ov_ForEachChildEx(ov_containment, pparent, passoc, ov_association) {
		if(!strcmp(passoc->v_identifier, passocdef->identifier)) {
			/*
			*	association already exists; if it was created provisorically,
			*	skip object creation, otherwise it is an error
			*/
			if(passoc->v_creationtime.secs == OV_VL_MAXUINT) {
				goto CONTINUE;
			}
			return OV_ERR_ALREADYEXISTS;
		}
	}
	/*
	*	create association object
	*/
	result = Ov_CreateObject(ov_association, passoc, pparent, passocdef->identifier);
	if(Ov_Fail(result)) {
		return result;
	}
	/*
	*	set parameters
	*/
CONTINUE:
	ov_time_gettime(&passoc->v_creationtime);
	passoc->v_assoctype = passocdef->assoctype;
	passoc->v_assocprops = passocdef->assocprops;
	ov_string_setvalue(&passoc->v_childrolename, passocdef->childrolename);
	ov_string_setvalue(&passoc->v_parentrolename, passocdef->parentrolename);
	passoc->v_parentoffset = passocdef->parentoffset;
	passoc->v_childoffset = passocdef->childoffset;
	ov_string_setvalue(&passoc->v_parentcomment, passocdef->parentcomment);
	ov_string_setvalue(&passoc->v_childcomment, passocdef->childcomment);
	passoc->v_parentflags = passocdef->parentflags;
	passoc->v_childflags = passocdef->childflags;
	passoc->v_linkfnc = passocdef->linkfnc;
	passoc->v_unlinkfnc = passocdef->unlinkfnc;
	passoc->v_getaccessfnc = passocdef->getaccessfnc;
	/*
	*	link association with parent class
	*/
	Ov_WarnIfNot(Ov_OK(Ov_Link(ov_parentrelationship, pparentclass, passoc)));
	/*
	*	link association with child class
	*/
	Ov_WarnIfNot(Ov_OK(Ov_Link(ov_childrelationship, pchildclass, passoc)));
	/*
	*	finished
	*/
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Compare an association with its definition
*/
OV_RESULT ov_association_compare(
	OV_INSTPTR_ov_association	passoc,
	OV_ASSOCIATION_DEF			*passocdef
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_class	pparentclass = Ov_GetParent(ov_parentrelationship, passoc);
	OV_INSTPTR_ov_class	pchildclass = Ov_GetParent(ov_childrelationship, passoc);
	OV_STRING			id, libid;
	/*
	*	compare parent and child class name
	*/
	if(!pparentclass || !pchildclass) {
		goto ERRORMSG;
	}
	libid = Ov_GetParent(ov_containment, pparentclass)->v_identifier;
	id = (OV_STRING)Ov_HeapMalloc(strlen(libid)+1+strlen(pparentclass->v_identifier)+1);
	if(!id) {
		return OV_ERR_HEAPOUTOFMEMORY;
	}
	sprintf(id, "%s/%s", libid, pparentclass->v_identifier);
	if(strcmp(passocdef->parentclassname, id)) {
		Ov_HeapFree(id);
		goto ERRORMSG;
	}
	Ov_HeapFree(id);
	libid = Ov_GetParent(ov_containment, pchildclass)->v_identifier;
	id = (OV_STRING)Ov_HeapMalloc(strlen(libid)+1+strlen(pchildclass->v_identifier)+1);
	if(!id) {
		return OV_ERR_HEAPOUTOFMEMORY;
	}
	sprintf(id, "%s/%s", libid, pchildclass->v_identifier);
	if(strcmp(passocdef->childclassname, id)) {
		Ov_HeapFree(id);
		goto ERRORMSG;
	}
	Ov_HeapFree(id);
	/*
	*	compare association attributes
	*/
	if((passoc->v_assoctype != passocdef->assoctype)
		|| (passoc->v_assocprops != passocdef->assocprops)
		|| (strcmp(passoc->v_childrolename, passocdef->childrolename))
		|| (strcmp(passoc->v_parentrolename, passocdef->parentrolename))
		|| (passoc->v_parentoffset != passocdef->parentoffset)
		|| (passoc->v_childoffset != passocdef->childoffset)
	) {
		goto ERRORMSG;
	}
	/*
	*	all is fine, load function pointers and exit
	*/
	passoc->v_linkfnc = passocdef->linkfnc;
	passoc->v_unlinkfnc = passocdef->unlinkfnc;
	passoc->v_getaccessfnc = passocdef->getaccessfnc;
	return OV_ERR_OK;
	/*
	*	print error message and exit
	*/
ERRORMSG:
	ov_logfile_error("Error loading association \"%s/%s\".",
		Ov_GetParent(ov_containment, passoc)->v_identifier,
		passoc->v_identifier);
	return OV_ERR_ASSOCDEFMISMATCH;
}

/*	----------------------------------------------------------------------	*/

/*
*	Test if we can unload an association from the database
*/
OV_BOOL ov_association_canunload(
	OV_INSTPTR_ov_association	passoc
) {
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	Search for child with a given identifier in an association
*/
OV_INSTPTR_ov_object OV_DLLFNCEXPORT ov_association_searchchild(
	const OV_INSTPTR_ov_association	passoc,
	const OV_INSTPTR_ov_object		pparent,
	const OV_STRING					identifier
) {
	/*
	*	check arguments
	*/
	if((!passoc) || (!identifier)) {
		return NULL;
	}
	/*
	*	search for child object if association is namebinding
	*/
	if(passoc->v_assocprops & OV_AP_LOCAL) {
		OV_INSTPTR_ov_object pchild = Ov_Association_GetFirstChild(passoc, pparent);
		while(pchild) {
			if(!strcmp(identifier, pchild->v_identifier)) {
				return pchild;
			}
			pchild = Ov_Association_GetNextChild(passoc, pchild);
		}
	}
	return NULL;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get the number of children of an association
*/
OV_UINT OV_DLLFNCEXPORT ov_association_getchildcount(
	const OV_INSTPTR_ov_association	passoc,
	const OV_INSTPTR_ov_object		pparent
) {
	/*
	*	local variables
	*/
	OV_UINT					num = 0;
	OV_INSTPTR_ov_object	pchild;
	/*
	*	count children
	*/
	for(pchild = Ov_Association_GetFirstChild(passoc, pparent); pchild;
		pchild = Ov_Association_GetNextChild(passoc, pchild)
	) {
		num++;
	}
	return num;
}

/*	----------------------------------------------------------------------	*/

/*
*	Link a parent and a child object
*/
OV_RESULT OV_DLLFNCEXPORT ov_association_link(
	const OV_INSTPTR_ov_association	passoc,
	const OV_INSTPTR_ov_object		pparent,
	const OV_INSTPTR_ov_object		pchild,
	const OV_PLACEMENT_HINT			parenthint,
	const OV_INSTPTR_ov_object		prelparent,
	const OV_PLACEMENT_HINT			childhint,
	const OV_INSTPTR_ov_object		prelchild
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_object 	ppreviouschild;
	OV_INSTPTR_ov_object	pnextchild;
	OV_PLACEMENT_HINT		childhint2 = childhint;
	OV_UINT					parentoffset;
	OV_UINT					childoffset;
	OV_INSTPTR_ov_object	prel = prelchild;
	/*
	*	check parameters
	*/
	if(!passoc) {
		return OV_ERR_BADPARAM;
	}
	if((!pparent) || (!pchild)) {
		return OV_ERR_BADPARAM;
	}
	/*
	*	initialize variables
	*/
	parentoffset = passoc->v_parentoffset;
	childoffset = passoc->v_childoffset;
	/*
	*	check further conditions
	*/
	switch(passoc->v_assoctype) {
	case OV_AT_1_TO_MANY:
		/*
		*	check, if the child link (anchor) is not already used
		*/
		if(Ov_Association_GetParent(passoc, pchild)) {
			return OV_ERR_ALREADYEXISTS;
		}
		/*
		*	check parent placement
		*/
		if(!((parenthint == OV_PMH_DEFAULT) || (parenthint == OV_PMH_BEGIN)
			|| (parenthint == OV_PMH_END))
		) {
			return OV_ERR_BADPLACEMENT;
		}
		/*
		*	ensure, that there is no name clash
		*/
		if(passoc->v_assocprops & OV_AP_LOCAL) {
			if(ov_association_searchchild(passoc, pparent, pchild->v_identifier)) {
				return OV_ERR_ALREADYEXISTS;
			}
		}
		break;
	default:
		return OV_ERR_BADPARAM;
	}
	/*
	*	handle placements which are not "before" or "after"
	*/
	switch(passoc->v_assoctype) {
	case OV_AT_1_TO_MANY:
		switch(childhint2) {
			case OV_PMH_DEFAULT:
			case OV_PMH_END:
				childhint2 = OV_PMH_AFTER;
				prel = Ov_Association_GetLastChild(passoc, pparent);
				break;
			case OV_PMH_BEGIN:
				childhint2 = OV_PMH_BEFORE;
				prel = Ov_Association_GetFirstChild(passoc, pparent);
				break;
			default:
				break;
		}
		break;
	default:
		return OV_ERR_BADPARAM;
	}
	if(prel) {
		/*
		*   check if relative child is valid...
		*/
		if(Ov_Association_GetParent(passoc, prel) != pparent) {
			return OV_ERR_BADPARAM;
		}
	} else {
		/*
		*	the list must be empty
		*/
		if(ov_association_isusedparentlink(passoc, pparent)) {
			return OV_ERR_BADPARAM;
		}
	}
	/*
	*	determine predecessor and successor
	*/
	switch(childhint2) {
	case OV_PMH_BEFORE:
		ppreviouschild = Ov_Association_GetPrevChild(passoc, prel);
		pnextchild = prel;
		break;
	case OV_PMH_AFTER:
		ppreviouschild = prel;
		pnextchild = Ov_Association_GetNextChild(passoc, prel);
		break;
	default:
		return OV_ERR_BADPLACEMENT;
	}
	/*
	*   set pointers of predecessor or parent object
	*/
	if(ppreviouschild) {
		((OV_ANCHOR*)(((OV_BYTE*)ppreviouschild)+childoffset))->pnext = pchild;
	} else {
		((OV_HEAD*)(((OV_BYTE*)pparent)+parentoffset))->pfirst = pchild;
	}
	/*
	*   set pointers of successor or parent object
	*/
	if(pnextchild) {
		((OV_ANCHOR*)(((OV_BYTE*)pnextchild)+childoffset))->pprevious = pchild;
	} else {
		((OV_HEAD*)(((OV_BYTE*)pparent)+parentoffset))->plast = pchild;
	}
	/*
	*	set pointers of child object
	*/
	((OV_ANCHOR*)(((OV_BYTE*)pchild)+childoffset))->pprevious = ppreviouschild;
	((OV_ANCHOR*)(((OV_BYTE*)pchild)+childoffset))->pnext = pnextchild;
	((OV_ANCHOR*)(((OV_BYTE*)pchild)+childoffset))->pparent = pparent;
	/*
	*	that's it.
	*/
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Unlink a parent and a child object
*/
void OV_DLLFNCEXPORT ov_association_unlink(
	const OV_INSTPTR_ov_association	passoc,
	const OV_INSTPTR_ov_object		pparent,
	const OV_INSTPTR_ov_object		pchild
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_object	ppreviouschild;
	OV_INSTPTR_ov_object	pnextchild;
	OV_UINT					childoffset;
	OV_UINT					parentoffset;
	/*
	*	check parameters
	*/
	if(!passoc || !pparent || !pchild) {
		return;
	}
	/*
	*	check if child is valid
	*/
	if(Ov_Association_GetParent(passoc, pchild) != pparent) {
		return;
	}
	/*
	*	initialize variables
	*/
	parentoffset = passoc->v_parentoffset;
	childoffset = passoc->v_childoffset;
	/*
	*	determine predecessor and successor
	*/
	ppreviouschild = Ov_Association_GetPrevChild(passoc, pchild);
	pnextchild = Ov_Association_GetNextChild(passoc, pchild);
	/*
	*	if there's no predecessor and no successor empty the parent's list
	*/
	if((!ppreviouschild)&&!(pnextchild)) {
		((OV_HEAD*)((OV_BYTE*)pparent+parentoffset))->pfirst
			= (OV_INSTPTR_ov_object)(NULL);
		((OV_HEAD*)((OV_BYTE*)pparent+parentoffset))->plast
			= (OV_INSTPTR_ov_object)(NULL);
	} else {
		/*
		*	set pointers of predecessor and successor, and/or parent object
		*/
		if(ppreviouschild) {
			((OV_ANCHOR*)((OV_BYTE*)ppreviouschild+childoffset))->pnext
				= pnextchild;
		} else {
			((OV_HEAD*)((OV_BYTE*)pparent+parentoffset))->pfirst
				= pnextchild;
		}
		if(pnextchild) {
			((OV_ANCHOR*)((OV_BYTE*)pnextchild+childoffset))->pprevious
				= ppreviouschild;
		} else {
			((OV_HEAD*)((OV_BYTE*)pparent+parentoffset))->plast
				= ppreviouschild;
		}
	}
	/*
	*	reset the child object's pointers
	*/
	((OV_ANCHOR*)((OV_BYTE*)pchild+childoffset))->pprevious
		= (OV_INSTPTR_ov_object)(NULL);
	((OV_ANCHOR*)((OV_BYTE*)pchild+childoffset))->pnext
		= (OV_INSTPTR_ov_object)(NULL);
	((OV_ANCHOR*)((OV_BYTE*)pchild+childoffset))->pparent
		= (OV_INSTPTR_ov_object)(NULL);
}

/*	----------------------------------------------------------------------	*/

/*
*	Test if a parent link is used
*/
OV_BOOL OV_DLLFNCEXPORT ov_association_isusedparentlink(
	const OV_INSTPTR_ov_association	passoc,
	const OV_INSTPTR_ov_object		pparent
) {
	/*
	*	local variables
	*/
	OV_HEAD *phead;
	/*
	*	instructions
	*/
	switch(passoc->v_assoctype) {
	case OV_AT_1_TO_MANY:
		/*
		*	get pointer to parent link (head)
		*/
		phead = (OV_HEAD*)(((OV_BYTE*)pparent)+passoc->v_parentoffset);
		/*
		*	check if used
		*/
		if((phead->pfirst) || (phead->plast)) {
			return TRUE;
		}
		return FALSE;
	default:
		Ov_Warning("no such association type");
		break;
	}
	return FALSE;
}

/*	----------------------------------------------------------------------	*/

/*
*	Test if a child link is used
*/
OV_BOOL OV_DLLFNCEXPORT ov_association_isusedchildlink(
	const OV_INSTPTR_ov_association	passoc,
	const OV_INSTPTR_ov_object		pchild
) {
	/*
	*	local variables
	*/
	OV_ANCHOR *panchor;
	/*
	*	instructions
	*/
	switch(passoc->v_assoctype) {
	case OV_AT_1_TO_MANY:
		/*
		*	get pointer to child link (anchor)
		*/
		panchor = (OV_ANCHOR*)(((OV_BYTE*)pchild)+passoc->v_childoffset);
		/*
		*	check if used
		*/
		if((panchor->pnext) || (panchor->pprevious) || (panchor->pparent)) {
			return TRUE;
		}
		return FALSE;
	default:
		Ov_Warning("no such association type");
		break;
	}
	return FALSE;
}

/*	----------------------------------------------------------------------	*/

/*
*	Accessor functions
*/
OV_INT OV_DLLFNCEXPORT ov_association_assoctype_get(
	OV_INSTPTR_ov_association	passoc
) {
	return passoc->v_assoctype;
}

OV_INT OV_DLLFNCEXPORT ov_association_assocprops_get(
	OV_INSTPTR_ov_association	passoc
) {
	return passoc->v_assocprops;
}

OV_STRING OV_DLLFNCEXPORT ov_association_childrolename_get(
	OV_INSTPTR_ov_association	passoc
) {
	return passoc->v_childrolename;
}

OV_STRING OV_DLLFNCEXPORT ov_association_parentrolename_get(
	OV_INSTPTR_ov_association	passoc
) {
	return passoc->v_parentrolename;
}

OV_STRING OV_DLLFNCEXPORT ov_association_parentcomment_get(
	OV_INSTPTR_ov_association	passoc
) {
	return passoc->v_parentcomment;
}

OV_STRING OV_DLLFNCEXPORT ov_association_childcomment_get(
	OV_INSTPTR_ov_association	passoc
) {
	return passoc->v_childcomment;
}

OV_UINT OV_DLLFNCEXPORT ov_association_parentflags_get(
	OV_INSTPTR_ov_association	passoc
) {
	return passoc->v_parentflags;
}

OV_UINT OV_DLLFNCEXPORT ov_association_childflags_get(
	OV_INSTPTR_ov_association	passoc
) {
	return passoc->v_childflags;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

