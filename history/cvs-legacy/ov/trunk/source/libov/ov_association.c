/*
*   $Id: ov_association.c,v 1.8 1999-09-15 10:48:21 dirk Exp $
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
*	Search for child with a given identifier in a 1:n association
*/
OV_DLLFNCEXPORT OV_INSTPTR_ov_object ov_association_searchchild(
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
	if((passoc->v_assocprops & OV_AP_LOCAL) && (passoc->v_assoctype == OV_AT_ONE_TO_MANY)) {
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
*	Get the number of parents of an association
*/
OV_DLLFNCEXPORT OV_UINT ov_association_getparentcount(
	const OV_INSTPTR_ov_association	passoc,
	const OV_INSTPTR_ov_object		pchild
) {
	/*
	*	local variables
	*/
	OV_UINT					num = 0;
	OV_INSTPTR_ov_object	pparent;
	Ov_Association_DefineIteratorNM(pit);
	/*
	*	assert, that child object class is OK
	*/
	Ov_WarnIfNot(ov_class_cancastto(Ov_GetParent(ov_instantiation, pchild),
		Ov_GetParent(ov_childrelationship, passoc)));
	/*
	*	switch based on the type of association
	*/
	switch(passoc->v_assoctype) {
	case OV_AT_ONE_TO_MANY:
		return Ov_Association_GetParent(passoc, pchild)?(1):(0);
	case OV_AT_MANY_TO_MANY:
		/*
		*	count parents
		*/
		Ov_Association_ForEachParentNM(passoc, pit, pchild, pparent) {
			num++;
		}
		return num;
	default:
		break;
	}
	return 0;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get the number of children of an association
*/
OV_DLLFNCEXPORT OV_UINT ov_association_getchildcount(
	const OV_INSTPTR_ov_association	passoc,
	const OV_INSTPTR_ov_object		pparent
) {
	/*
	*	local variables
	*/
	OV_UINT					num = 0;
	OV_INSTPTR_ov_object	pchild;
	Ov_Association_DefineIteratorNM(pit);
	/*
	*	assert, that parent object class is OK
	*/
	Ov_WarnIfNot(ov_class_cancastto(Ov_GetParent(ov_instantiation, pparent),
		Ov_GetParent(ov_parentrelationship, passoc)));
	/*
	*	switch based on the type of association
	*/
	switch(passoc->v_assoctype) {
	case OV_AT_ONE_TO_MANY:
		/*
		*	count children
		*/
		Ov_Association_ForEachChild(passoc, pparent, pchild) {
			num++;
		}
		return num;
	case OV_AT_MANY_TO_MANY:
		/*
		*	count parents
		*/
		Ov_Association_ForEachChildNM(passoc, pit, pparent, pchild) {
			num++;
		}
		return num;
	default:
		break;
	}
	return 0;
}

/*	----------------------------------------------------------------------	*/

/*
*	Link a parent and a child object
*/
OV_DLLFNCEXPORT OV_RESULT ov_association_link(
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
	OV_UINT					parentoffset;
	OV_UINT					childoffset;
	OV_INSTPTR_ov_object	pnextchild;
	OV_INSTPTR_ov_object 	ppreviouschild;
	OV_INSTPTR_ov_object	pcurrchild;
	OV_INSTPTR_ov_object	pcurrparent;
	OV_PLACEMENT_HINT		parenthint2 = parenthint;
	OV_PLACEMENT_HINT		childhint2 = childhint;
	OV_INSTPTR_ov_object	prelchild2 = prelchild;
	Ov_Association_DefineIteratorNM(pit);
	Ov_Association_DefineIteratorNM(pparentit);
	Ov_Association_DefineIteratorNM(pchildit);
	Ov_Association_DefineIteratorNM(pnextparentit);
	Ov_Association_DefineIteratorNM(pnextchildit);
	Ov_Association_DefineIteratorNM(ppreviousparentit);
	Ov_Association_DefineIteratorNM(ppreviouschildit);
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
	case OV_AT_ONE_TO_MANY:
		/*
		*	check, if there is not already a link between that parent and child;
		*	in this case simply check, if the child link (anchor) is not already used
		*/
		if(Ov_Association_GetParent(passoc, pchild)) {
			return OV_ERR_ALREADYEXISTS;
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
	case OV_AT_MANY_TO_MANY:
		/*
		*	check, if there is not already a link between that parent and child
		*/
		Ov_Association_ForEachChildNM(passoc, pit, pparent, pcurrchild) {
			if(pcurrchild == pchild) {
				return OV_ERR_ALREADYEXISTS;
			}
		}
		break;
	default:
		Ov_Warning("no such association type");
		return OV_ERR_BADPARAM;
	}
	/*
	*	handle parent placements
	*/
	switch(passoc->v_assoctype) {
	case OV_AT_ONE_TO_MANY:
		if(!((parenthint == OV_PMH_DEFAULT) || (parenthint == OV_PMH_BEGIN)
			|| (parenthint == OV_PMH_END))
		) {
			return OV_ERR_BADPLACEMENT;
		}
		break;
	case OV_AT_MANY_TO_MANY:
		switch(parenthint2) {
		case OV_PMH_DEFAULT:
			/*
			*	default placement is placement at the end
			*/
			/* fall through... */
		case OV_PMH_END:
			/*
			*	placement at the end is placement after the last parent
			*/
			parenthint2 = OV_PMH_AFTER;
			(void)Ov_Association_GetLastParentNM(passoc, pparentit, pchild);
			break;
		case OV_PMH_BEGIN:
			/*
			*	placement at the beginning is placement before the first parent
			*/
			parenthint2 = OV_PMH_BEFORE;
			(void)Ov_Association_GetFirstParentNM(passoc, pparentit, pchild);
			break;
		case OV_PMH_AFTER:
			/* fall through... */
		case OV_PMH_BEFORE:
			/*
			*	check, if the placement is valid
			*/
			Ov_Association_ForEachParentNM(passoc, pparentit, pchild, pcurrparent) {
				if(pcurrparent == prelparent) {
					break;
				}
			}
			if(!pparentit) {
				return OV_ERR_BADPLACEMENT;
			}
			break;
		default:
			return OV_ERR_BADPLACEMENT;
		}
		break;
	default:
		return OV_ERR_BADPARAM;
	}
	/*
	*	handle child placements
	*/
	switch(passoc->v_assoctype) {
	case OV_AT_ONE_TO_MANY:
		switch(childhint2) {
		case OV_PMH_DEFAULT:
			/*
			*	default placement is placement at the end
			*/
			/* fall through... */
		case OV_PMH_END:
			/*
			*	placement at the end is placement after the last child
			*/
			childhint2 = OV_PMH_AFTER;
			prelchild2 = Ov_Association_GetLastChild(passoc, pparent);
			break;
		case OV_PMH_BEGIN:
			/*
			*	placement at the beginning is placement before the first child
			*/
			childhint2 = OV_PMH_BEFORE;
			prelchild2 = Ov_Association_GetFirstChild(passoc, pparent);
			break;
		case OV_PMH_AFTER:
			/* fall through... */
		case OV_PMH_BEFORE:
			/*
			*	check, if the placement is valid;
			*	in this case, check if the relchild has the parent to link
			*/
			if(Ov_Association_GetParent(passoc, prelchild2) != pparent) {
				return OV_ERR_BADPARAM;
			}
			break;
		default:
			return OV_ERR_BADPLACEMENT;
		}
		break;
	case OV_AT_MANY_TO_MANY:
		switch(childhint2) {
		case OV_PMH_DEFAULT:
			/*
			*	default placement is placement at the end
			*/
			/* fall through... */
		case OV_PMH_END:
			/*
			*	placement at the end is placement after the last child
			*/
			childhint2 = OV_PMH_AFTER;
			(void)Ov_Association_GetLastChildNM(passoc, pchildit, pparent);
			break;
		case OV_PMH_BEGIN:
			/*
			*	placement at the beginning is placement before the first child
			*/
			childhint2 = OV_PMH_BEFORE;
			(void)Ov_Association_GetFirstChildNM(passoc, pchildit, pparent);
			break;
		case OV_PMH_AFTER:
			/* fall through... */
		case OV_PMH_BEFORE:
			/*
			*	check, if the placement is valid
			*/
			Ov_Association_ForEachChildNM(passoc, pchildit, pparent, pnextchild) {
				if(pnextchild == prelchild) {
					break;
				}
			}
			if(!pchildit) {
				return OV_ERR_BADPLACEMENT;
			}
			break;
		default:
			return OV_ERR_BADPLACEMENT;
		}
		break;
	default:
		return OV_ERR_BADPARAM;
	}
	/*
	*	determine parent predecessor and successor
	*/
	switch(passoc->v_assoctype) {
	case OV_AT_ONE_TO_MANY:
		/*
		*	there is no parent predecessor/successor
		*/
		break;
	case OV_AT_MANY_TO_MANY:
		switch(parenthint2) {
		case OV_PMH_BEFORE:
			ppreviousparentit = pnextparentit = pparentit;
			(void)Ov_Association_GetPreviousParentNM(passoc, ppreviousparentit);
			break;
		case OV_PMH_AFTER:
			ppreviousparentit = pnextparentit = pparentit;
			(void)Ov_Association_GetNextParentNM(passoc, pnextparentit);
			break;
		default:
			return OV_ERR_BADPLACEMENT;
		}
		break;
	default:
		return OV_ERR_BADPARAM;
	}
	/*
	*	determine child predecessor and successor
	*/
	switch(passoc->v_assoctype) {
	case OV_AT_ONE_TO_MANY:
		switch(childhint2) {
		case OV_PMH_BEFORE:
			ppreviouschild = Ov_Association_GetPreviousChild(passoc, prelchild2);
			pnextchild = prelchild2;
			break;
		case OV_PMH_AFTER:
			ppreviouschild = prelchild2;
			pnextchild = Ov_Association_GetNextChild(passoc, prelchild2);
			break;
		default:
			return OV_ERR_BADPLACEMENT;
		}
		break;
	case OV_AT_MANY_TO_MANY:
		switch(childhint2) {
		case OV_PMH_BEFORE:
			ppreviouschildit = pnextchildit = pchildit;
			pnextchild = prelchild2;
			ppreviouschild = Ov_Association_GetPreviousChildNM(passoc, ppreviouschildit);
			break;
		case OV_PMH_AFTER:
			ppreviouschildit = pnextchildit = pchildit;
			ppreviouschild = prelchild2;
			pnextchild = Ov_Association_GetNextChildNM(passoc, pnextchildit);
			break;
		default:
			return OV_ERR_BADPLACEMENT;
		}
		break;
	default:
		return OV_ERR_BADPARAM;
	}
	/*
	*	finally do the actual linkage
	*/
	switch(passoc->v_assoctype) {
	case OV_AT_ONE_TO_MANY:
		/*
		*   set pointers of predecessor of parent object
		*/
		if(ppreviouschild) {
			((OV_ANCHOR*)(((OV_BYTE*)ppreviouschild)+childoffset))->pnext = pchild;
		} else {
			((OV_HEAD*)(((OV_BYTE*)pparent)+parentoffset))->pfirst = pchild;
		}
		/*
		*   set pointers of successor of parent object
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
		break;
	case OV_AT_MANY_TO_MANY:
		/*
		*	create a new "link object"
		*/
		pit = Ov_DbAlloc(OV_NMLINK);
		if(!pit) {
			return OV_ERR_DBOUTOFMEMORY;
		}
		/*
		*   set pointers of predecessor of parent object
		*/
		if(ppreviouschildit) {
			ppreviouschildit->parent.pnext = pit;
		} else {
			((OV_NMHEAD*)(((OV_BYTE*)pparent)+parentoffset))->pfirst = pit;
		}
		/*
		*   set pointers of successor of parent object
		*/
		if(pnextchildit) {
			pnextchildit->parent.pprevious = pit;
		} else {
			((OV_NMHEAD*)(((OV_BYTE*)pparent)+parentoffset))->plast = pit;
		}
		/*
		*   set pointers of predecessor of child object
		*/
		if(ppreviousparentit) {
			ppreviousparentit->child.pnext = pit;
		} else {
			((OV_NMHEAD*)(((OV_BYTE*)pchild)+childoffset))->pfirst = pit;
		}
		/*
		*   set pointers of successor of child object
		*/
		if(pnextparentit) {
			pnextparentit->child.pprevious = pit;
		} else {
			((OV_NMHEAD*)(((OV_BYTE*)pchild)+childoffset))->plast = pit;
		}
		/*
		*	set pointers of "link object"
		*/
		pit->parent.pprevious = ppreviouschildit;
		pit->parent.pnext = pnextchildit;
		pit->parent.pparent = pparent;
		pit->child.pprevious = ppreviousparentit;
		pit->child.pnext = pnextparentit;
		pit->child.pchild = pchild;
		break;
	default:
		return OV_ERR_BADPARAM;
	}
	/*
	*	that's it.
	*/
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Unlink a parent and a child object
*/
OV_DLLFNCEXPORT void ov_association_unlink(
	const OV_INSTPTR_ov_association	passoc,
	const OV_INSTPTR_ov_object		pparent,
	const OV_INSTPTR_ov_object		pchild
) {
	/*
	*	local variables
	*/
	OV_UINT					childoffset;
	OV_UINT					parentoffset;
	OV_INSTPTR_ov_object	ppreviouschild = NULL;
	OV_INSTPTR_ov_object	pnextchild = NULL;
	OV_INSTPTR_ov_object	pcurrchild;
	Ov_Association_DefineIteratorNM(pit);
	Ov_Association_DefineIteratorNM(pnextparentit);
	Ov_Association_DefineIteratorNM(pnextchildit);
	Ov_Association_DefineIteratorNM(ppreviousparentit);
	Ov_Association_DefineIteratorNM(ppreviouschildit);
	/*
	*	check parameters
	*/
	if(!passoc || !pparent || !pchild) {
		return;
	}
	/*
	*	initialize variables
	*/
	parentoffset = passoc->v_parentoffset;
	childoffset = passoc->v_childoffset;
	/*
	*	check, if there is a link between that parent and child
	*/
	switch(passoc->v_assoctype) {
	case OV_AT_ONE_TO_MANY:
		if(Ov_Association_GetParent(passoc, pchild) != pparent) {
			return;
		}
		break;
	case OV_AT_MANY_TO_MANY:
		Ov_Association_ForEachChildNM(passoc, pit, pparent, pcurrchild) {
			if(pcurrchild == pchild) {
				break;
			}
		}
		if(!pit) {
			return;
		}
		break;
	default:
		Ov_Warning("no such association type");
		return;
	}
	/*
	*	determine parent predecessor and successor
	*/
	switch(passoc->v_assoctype) {
	case OV_AT_ONE_TO_MANY:
		break;
	case OV_AT_MANY_TO_MANY:
		Ov_WarnIfNot(pit);
		ppreviousparentit = pnextparentit = pit;
		(void)Ov_Association_GetPreviousParentNM(passoc, ppreviousparentit);
		(void)Ov_Association_GetNextParentNM(passoc, pnextparentit);
		break;
	default:
		return;
	}
	/*
	*	determine child predecessor and successor
	*/
	switch(passoc->v_assoctype) {
	case OV_AT_ONE_TO_MANY:
		ppreviouschild = Ov_Association_GetPreviousChild(passoc, pchild);
		pnextchild = Ov_Association_GetNextChild(passoc, pchild);
		break;
	case OV_AT_MANY_TO_MANY:
		Ov_WarnIfNot(pit);
		ppreviouschildit = pnextchildit = pit;
		(void)Ov_Association_GetPreviousChildNM(passoc, ppreviouschildit);
		(void)Ov_Association_GetNextChildNM(passoc, pnextchildit);
		break;
	default:
		return;
	}
	/*
	*	finally do the actual unlinkage
	*/
	switch(passoc->v_assoctype) {
	case OV_AT_ONE_TO_MANY:
		/*
		*	if there's no child predecessor and no successor empty the parent's list
		*/
		if((!ppreviouschild)&&!(pnextchild)) {
			((OV_HEAD*)((OV_BYTE*)pparent+parentoffset))->pfirst = NULL;
			((OV_HEAD*)((OV_BYTE*)pparent+parentoffset))->plast = NULL;
		} else {
			/*
			*	set pointers of child predecessor and successor, and/or parent object
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
		((OV_ANCHOR*)((OV_BYTE*)pchild+childoffset))->pprevious = NULL;
		((OV_ANCHOR*)((OV_BYTE*)pchild+childoffset))->pnext = NULL;
		((OV_ANCHOR*)((OV_BYTE*)pchild+childoffset))->pparent = NULL;
		break;
	case OV_AT_MANY_TO_MANY:
		/*
		*	if there's no parent predecessor and no successor empty the parent's list
		*/
		if((!ppreviouschildit)&&!(pnextchildit)) {
			((OV_NMHEAD*)((OV_BYTE*)pparent+parentoffset))->pfirst = NULL;
			((OV_NMHEAD*)((OV_BYTE*)pparent+parentoffset))->plast = NULL;
		} else {
			/*
			*	set pointers of child predecessor and successor, and/or parent object
			*/
			if(ppreviouschildit) {
				ppreviouschildit->parent.pnext = pnextchildit;
			} else {
				((OV_NMHEAD*)((OV_BYTE*)pparent+parentoffset))->pfirst = pnextchildit;
			}
			if(pnextchildit) {
				pnextchildit->parent.pprevious = ppreviouschildit;
			} else {
				((OV_NMHEAD*)((OV_BYTE*)pparent+parentoffset))->plast = ppreviouschildit;
			}
		}
		/*
		*	if there's no child predecessor and no successor empty the child's list
		*/
		if((!ppreviousparentit)&&!(pnextparentit)) {
			((OV_NMHEAD*)((OV_BYTE*)pchild+childoffset))->pfirst = NULL;
			((OV_NMHEAD*)((OV_BYTE*)pchild+childoffset))->plast = NULL;
		} else {
			/*
			*	set pointers of parent predecessor and successor, and/or parent object
			*/
			if(ppreviousparentit) {
				ppreviousparentit->child.pnext = pnextparentit;
			} else {
				((OV_NMHEAD*)((OV_BYTE*)pchild+childoffset))->pfirst = pnextparentit;
			}
			if(pnextparentit) {
				pnextparentit->child.pprevious = ppreviousparentit;
			} else {
				((OV_NMHEAD*)((OV_BYTE*)pchild+childoffset))->plast = ppreviousparentit;
			}
		}
		/*
		*	delete the "link object"
		*/
		Ov_DbFree(pit);
		break;
	default:
		return;
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	Test if a parent link is used
*/
OV_DLLFNCEXPORT OV_BOOL ov_association_isusedparentlink(
	const OV_INSTPTR_ov_association	passoc,
	const OV_INSTPTR_ov_object		pparent
) {
	/*
	*	local variables
	*/
	OV_HEAD	*phead;
	/*
	*	instructions
	*/
	switch(passoc->v_assoctype) {
	case OV_AT_ONE_TO_MANY:
		/* fall into... */
	case OV_AT_MANY_TO_MANY:
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
OV_DLLFNCEXPORT OV_BOOL ov_association_isusedchildlink(
	const OV_INSTPTR_ov_association	passoc,
	const OV_INSTPTR_ov_object		pchild
) {
	/*
	*	local variables
	*/
	OV_ANCHOR	*panchor;
	OV_HEAD		*phead;
	/*
	*	instructions
	*/
	switch(passoc->v_assoctype) {
	case OV_AT_ONE_TO_MANY:
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
	case OV_AT_MANY_TO_MANY:
		/*
		*	get pointer to child link (head)
		*/
		phead = (OV_HEAD*)(((OV_BYTE*)pchild)+passoc->v_childoffset);
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
*	Accessor functions
*/
OV_DLLFNCEXPORT OV_INT ov_association_assoctype_get(
	OV_INSTPTR_ov_association	passoc
) {
	return passoc->v_assoctype;
}

OV_DLLFNCEXPORT OV_INT ov_association_assocprops_get(
	OV_INSTPTR_ov_association	passoc
) {
	return passoc->v_assocprops;
}

OV_DLLFNCEXPORT OV_STRING ov_association_childrolename_get(
	OV_INSTPTR_ov_association	passoc
) {
	return passoc->v_childrolename;
}

OV_DLLFNCEXPORT OV_STRING ov_association_parentrolename_get(
	OV_INSTPTR_ov_association	passoc
) {
	return passoc->v_parentrolename;
}

OV_DLLFNCEXPORT OV_STRING ov_association_parentcomment_get(
	OV_INSTPTR_ov_association	passoc
) {
	return passoc->v_parentcomment;
}

OV_DLLFNCEXPORT OV_STRING ov_association_childcomment_get(
	OV_INSTPTR_ov_association	passoc
) {
	return passoc->v_childcomment;
}

OV_DLLFNCEXPORT OV_UINT ov_association_parentflags_get(
	OV_INSTPTR_ov_association	passoc
) {
	return passoc->v_parentflags;
}

OV_DLLFNCEXPORT OV_UINT ov_association_childflags_get(
	OV_INSTPTR_ov_association	passoc
) {
	return passoc->v_childflags;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

