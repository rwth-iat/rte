/*
*   $Id: ov_association.c,v 1.15 2003-08-25 09:16:03 ansgar Exp $
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
*	17-Jul-2001 Ansgar Münnemann <ansgar@plt.rwth-aachen.de>: ONE_TO_ONE Link.
*	10-Dec-2002 Ansgar Münnemann <ansgar@plt.rwth-aachen.de>: Dynamic Assoc Load BugFix (inheritance).
*	22-Aug-2003 Ansgar Münnemann <ansgar@plt.rwth-aachen.de>: Dynamic Assoc Load BugFix (parentoffset).
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
	OV_RESULT			result;
	OV_INSTPTR_ov_association	passoc;
	OV_INSTPTR_ov_class		pparentclass, pchildclass;
	OV_BOOL				exists=FALSE;
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
				exists = TRUE;
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
	if (Ov_Fail(ov_string_setvalue(&passoc->v_childrolename, passocdef->childrolename))) goto CONTINUEERR6;
	if (Ov_Fail(ov_string_setvalue(&passoc->v_parentrolename, passocdef->parentrolename))) goto CONTINUEERR5;
	if (Ov_Fail(ov_string_setvalue(&passoc->v_parentcomment, passocdef->parentcomment))) goto CONTINUEERR4;
	if (Ov_Fail(ov_string_setvalue(&passoc->v_childcomment, passocdef->childcomment))) goto CONTINUEERR3;
	if (!exists) {
		/*	
		*   Calculate offsets and new tablesizes and move instancedata
		*/

		if (passoc->v_assoctype == OV_AT_ONE_TO_MANY) {

			/*
			*	if same class, memory for head and anchor of link must be allocated
			*/
			if (pparentclass==pchildclass) {
				result = ov_association_linktable_allocate(pchildclass, sizeof(OV_HEAD) + sizeof(OV_ANCHOR));
			}
			/*
			*	if childclass is a superclass of parentclass, in parentclass the memory for head and anchor of link must be allocated
			*	and in childclass only the memory for the link anchor
			*/
			else if (ov_class_cancastto(pparentclass, pchildclass)) {
				result = ov_association_linktable_allocate(pchildclass, sizeof(OV_ANCHOR));
				if (Ov_Fail(result)) goto CONTINUEERR2;
				result = ov_association_linktable_allocate(pparentclass, sizeof(OV_HEAD) + sizeof(OV_ANCHOR));
			}
			/*
			*	if parentclass is a superclass of childclass, in childclass memory for head and anchor of link must be allocated
			*	and in parentclass only the memory for the link head
			*/
			else if (ov_class_cancastto(pchildclass, pparentclass)) {
				result = ov_association_linktable_allocate(pparentclass, sizeof(OV_HEAD));
				if (Ov_Fail(result)) goto CONTINUEERR2;
				result = ov_association_linktable_allocate(pchildclass, sizeof(OV_ANCHOR) + sizeof(OV_HEAD));
			}
			/*
			*	if parentclass and childclass have no common baseclass, in childclass memory for the anchor and
			*	and in parentclass memory for the head must be allocated.
			*/
			else {
				result = ov_association_linktable_allocate(pchildclass, sizeof(OV_ANCHOR));
				if (Ov_Fail(result)) goto CONTINUEERR2;
				result = ov_association_linktable_allocate(pparentclass, sizeof(OV_HEAD));
			}
			if (Ov_Fail(result)) goto CONTINUEERR1;
			/*
			*	after allocation, memory space is inserted for the new child/parent association and
			*	and the offset parameter of the particular association objects is adapted.
			*/
			passoc->v_parentoffset = pparentclass->v_linktablesize;
			ov_association_linktable_insert(pparentclass, pparentclass, sizeof(OV_HEAD), pparentclass->v_linktablesize);

			/* 	after adjusting the offsets of the associations of the parentclass and its derived classes the parentclass is
			*	linked to the loaded association by ov_parentrelationship, so in the case that the childclass is a baseclass of the parentclass 
			*	the offset adjusting is also done for the parentoffset of this loaded association
			*	(Bugfix 22.8.2003)
			*/

			/*
			*	link association with parent class
			*/
			Ov_WarnIfNot(Ov_OK(Ov_Link(ov_parentrelationship, pparentclass, passoc)));

			passoc->v_childoffset = pchildclass->v_linktablesize;
			ov_association_linktable_insert(pchildclass, pchildclass, sizeof(OV_ANCHOR), pchildclass->v_linktablesize);
			/*
			*	link association with child class
			*/
			Ov_WarnIfNot(Ov_OK(Ov_Link(ov_childrelationship, pchildclass, passoc)));
		}
		if (passoc->v_assoctype == OV_AT_MANY_TO_MANY) {
			if (pparentclass==pchildclass) {
				result = ov_association_linktable_allocate(pchildclass, 2 * sizeof(OV_NMHEAD));
			}
			else if (ov_class_cancastto(pparentclass, pchildclass)) {
				result = ov_association_linktable_allocate(pchildclass, sizeof(OV_NMHEAD));
				if (Ov_Fail(result)) goto CONTINUEERR2;
				result = ov_association_linktable_allocate(pparentclass, 2 * sizeof(OV_NMHEAD));
			}
			else if (ov_class_cancastto(pchildclass, pparentclass)) {
				result = ov_association_linktable_allocate(pparentclass, sizeof(OV_NMHEAD));
				if (Ov_Fail(result)) goto CONTINUEERR2;
				result = ov_association_linktable_allocate(pchildclass, 2 * sizeof(OV_NMHEAD));
			}
			else {
				result = ov_association_linktable_allocate(pchildclass, sizeof(OV_NMHEAD));
				if (Ov_Fail(result)) goto CONTINUEERR2;
				result = ov_association_linktable_allocate(pparentclass, sizeof(OV_NMHEAD));
			}
			if (Ov_Fail(result)) goto CONTINUEERR1;
			passoc->v_parentoffset = pparentclass->v_linktablesize;
			ov_association_linktable_insert(pparentclass, pparentclass, sizeof(OV_NMHEAD), pparentclass->v_linktablesize);
			/*
			*	link association with parent class
			*/
			Ov_WarnIfNot(Ov_OK(Ov_Link(ov_parentrelationship, pparentclass, passoc)));

			passoc->v_childoffset = pchildclass->v_linktablesize;
			ov_association_linktable_insert(pchildclass, pchildclass, sizeof(OV_NMHEAD), pchildclass->v_linktablesize);
			/*
			*	link association with child class
			*/
			Ov_WarnIfNot(Ov_OK(Ov_Link(ov_childrelationship, pchildclass, passoc)));
		}
		if (passoc->v_assoctype == OV_AT_ONE_TO_ONE) {
			if (pparentclass==pchildclass) {
				result = ov_association_linktable_allocate(pchildclass, 2* sizeof(OV_INSTPTR_ov_object));
			}
			else if (ov_class_cancastto(pparentclass, pchildclass)) {
				result = ov_association_linktable_allocate(pchildclass, sizeof(OV_INSTPTR_ov_object));
				if (Ov_Fail(result)) goto CONTINUEERR2;
				result = ov_association_linktable_allocate(pparentclass,  2* sizeof(OV_INSTPTR_ov_object));
			}
			else if (ov_class_cancastto(pchildclass, pparentclass)) {
				result = ov_association_linktable_allocate(pparentclass, sizeof(OV_INSTPTR_ov_object));
				if (Ov_Fail(result)) goto CONTINUEERR2;
				result = ov_association_linktable_allocate(pchildclass, 2* sizeof(OV_INSTPTR_ov_object));
			}
			else {
				result = ov_association_linktable_allocate(pchildclass, sizeof(OV_INSTPTR_ov_object));
				if (Ov_Fail(result)) goto CONTINUEERR1;
				result = ov_association_linktable_allocate(pparentclass, sizeof(OV_INSTPTR_ov_object));
			}
			if (Ov_Fail(result)) goto CONTINUEERR2;
			passoc->v_parentoffset = pparentclass->v_linktablesize;
			ov_association_linktable_insert(pparentclass, pparentclass, sizeof(OV_INSTPTR_ov_object), pparentclass->v_linktablesize);
			/*
			*	link association with parent class
			*/
			Ov_WarnIfNot(Ov_OK(Ov_Link(ov_parentrelationship, pparentclass, passoc)));
			passoc->v_childoffset = pchildclass->v_linktablesize;
			ov_association_linktable_insert(pchildclass, pchildclass, sizeof(OV_INSTPTR_ov_object), pchildclass->v_linktablesize);
			/*
			*	link association with child class
			*/
			Ov_WarnIfNot(Ov_OK(Ov_Link(ov_childrelationship, pchildclass, passoc)));
		}
	}
	passoc->v_parentflags = passocdef->parentflags;
	passoc->v_childflags = passocdef->childflags;
	passoc->v_linkfnc = passocdef->linkfnc;
	passoc->v_unlinkfnc = passocdef->unlinkfnc;
	passoc->v_getaccessfnc = passocdef->getaccessfnc;
	/*
	*	finished
	*/
	return OV_ERR_OK;

CONTINUEERR1:
CONTINUEERR2:
CONTINUEERR3:
	ov_string_setvalue(&passoc->v_childcomment, NULL);
CONTINUEERR4:
	ov_string_setvalue(&passoc->v_parentcomment, NULL);
CONTINUEERR5:
	ov_string_setvalue(&passoc->v_parentrolename, NULL);
CONTINUEERR6:
	ov_string_setvalue(&passoc->v_childrolename, NULL);
	return OV_ERR_DBOUTOFMEMORY;;
}

/*	----------------------------------------------------------------------	*/

/*
*	unload an association
*/
void ov_association_unload(
	OV_INSTPTR_ov_association	passoc,
	OV_INSTPTR_ov_class		pparentclass, 
	OV_INSTPTR_ov_class		pchildclass
) {

	if ((pparentclass) && (pchildclass) && (passoc)) {
	    if ((passoc->v_parentoffset!=0) && (passoc->v_childoffset!=0)) {
		if (passoc->v_assoctype == OV_AT_ONE_TO_MANY) {
			ov_association_linktable_insert(pchildclass, pchildclass, -sizeof(OV_ANCHOR), passoc->v_childoffset);
			ov_association_linktable_insert(pparentclass, pparentclass, -sizeof(OV_HEAD), passoc->v_parentoffset);
			ov_association_linktable_allocate(pparentclass, 0);
			ov_association_linktable_allocate(pchildclass, 0);
		}
		if (passoc->v_assoctype == OV_AT_MANY_TO_MANY) {
			ov_association_linktable_insert(pchildclass, pchildclass, -sizeof(OV_NMHEAD), passoc->v_childoffset);
			ov_association_linktable_insert(pparentclass, pparentclass, -sizeof(OV_NMHEAD), passoc->v_parentoffset);
			ov_association_linktable_allocate(pparentclass, 0);
			ov_association_linktable_allocate(pchildclass, 0);
		}
		if (passoc->v_assoctype == OV_AT_ONE_TO_ONE) {
			ov_association_linktable_insert(pchildclass, pchildclass, -sizeof(OV_INSTPTR_ov_object), passoc->v_childoffset);
			ov_association_linktable_insert(pparentclass, pparentclass, -sizeof(OV_INSTPTR_ov_object), passoc->v_parentoffset);
			ov_association_linktable_allocate(pparentclass, 0);
			ov_association_linktable_allocate(pchildclass, 0);
		}
		passoc->v_parentoffset=0;
		passoc->v_childoffset=0;
	    }
	}
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
	OV_INSTPTR_ov_class		pclass;
	OV_INSTPTR_ov_domain		plib;
	OV_INSTPTR_ov_object		pinst;
	Ov_Association_DefineIteratorNM(pit);

	/*
	*	test if there exist parent or child classes in an other library which are linked by this association
	*/
	plib   = Ov_GetParent(ov_containment, passoc);
	pclass = Ov_GetParent(ov_parentrelationship, passoc);
	if (Ov_GetParent(ov_containment, pclass) != plib) {
		Ov_ForEachChild(ov_instantiation, pclass, pinst) {
			switch(passoc->v_assoctype) {
			case OV_AT_ONE_TO_ONE:
				if (Ov_Association_GetChild(passoc, pinst)) return FALSE;
			case OV_AT_ONE_TO_MANY:
				if (Ov_Association_GetFirstChild(passoc, pinst)) return FALSE;
			case OV_AT_MANY_TO_MANY:
				if (Ov_Association_GetFirstChildNM(passoc, pit, pinst)) return FALSE;
			}
		}
	}
	pclass = Ov_GetParent(ov_childrelationship, passoc);
	if (Ov_GetParent(ov_containment, pclass) != plib) {
		Ov_ForEachChild(ov_instantiation, pclass, pinst) {
			switch(passoc->v_assoctype) {
			case OV_AT_ONE_TO_ONE:
			case OV_AT_ONE_TO_MANY:
				if (Ov_Association_GetParent(passoc, pinst)) return FALSE;
			case OV_AT_MANY_TO_MANY:
				if (Ov_Association_GetFirstParentNM(passoc, pit, pinst)) return FALSE;
			}
		}
	}
	
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
	if((passoc->v_assocprops & OV_AP_LOCAL) && (passoc->v_assoctype == OV_AT_ONE_TO_ONE)) {
		OV_INSTPTR_ov_object pchild = Ov_Association_GetChild(passoc, pparent);
		if (pchild) {
			if(!strcmp(identifier, pchild->v_identifier)) return pchild;
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
	case OV_AT_ONE_TO_ONE:
		return Ov_Association_GetParent(passoc, pchild)?(1):(0);
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
	case OV_AT_ONE_TO_ONE:
		return Ov_Association_GetChild(passoc, pparent)?(1):(0);
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
	OV_INSTPTR_ov_object	pnextchild = NULL;
	OV_INSTPTR_ov_object 	ppreviouschild = NULL;
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
	case OV_AT_ONE_TO_ONE:
		if(Ov_Association_GetParent(passoc, pchild)) {
			return OV_ERR_ALREADYEXISTS;
		}
		if(Ov_Association_GetChild(passoc, pparent)) {
			return OV_ERR_ALREADYEXISTS;
		}
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
	case OV_AT_ONE_TO_ONE:
		if(!((parenthint == OV_PMH_DEFAULT) || (parenthint == OV_PMH_BEGIN)
			|| (parenthint == OV_PMH_END))
		) {
			return OV_ERR_BADPLACEMENT;
		}
		break;
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
	case OV_AT_ONE_TO_ONE:
		break;
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
	case OV_AT_ONE_TO_ONE:
		break;
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
	case OV_AT_ONE_TO_ONE:
		break;
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
	case OV_AT_ONE_TO_ONE:
		*( (OV_INSTPTR_ov_object*) ((pparent->v_linktable)+parentoffset) ) = pchild;
		*( (OV_INSTPTR_ov_object*) ((pchild->v_linktable)+childoffset) ) = pparent;
		break;
	case OV_AT_ONE_TO_MANY:
		/*
		*   set pointers of predecessor of parent object
		*/
		if(ppreviouschild) {
			((OV_ANCHOR*)((ppreviouschild->v_linktable)+childoffset))->pnext = pchild;
		} else {
			((OV_HEAD*)((pparent->v_linktable)+parentoffset))->pfirst = pchild;
		}
		/*
		*   set pointers of successor of parent object
		*/
		if(pnextchild) {
			((OV_ANCHOR*)((pnextchild->v_linktable)+childoffset))->pprevious = pchild;
		} else {
			((OV_HEAD*)((pparent->v_linktable)+parentoffset))->plast = pchild;
		}
		/*
		*	set pointers of child object
		*/
		((OV_ANCHOR*)((pchild->v_linktable)+childoffset))->pprevious = ppreviouschild;
		((OV_ANCHOR*)((pchild->v_linktable)+childoffset))->pnext = pnextchild;
		((OV_ANCHOR*)((pchild->v_linktable)+childoffset))->pparent = pparent;
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
			((OV_NMHEAD*)((pparent->v_linktable)+parentoffset))->pfirst = pit;
		}
		/*
		*   set pointers of successor of parent object
		*/
		if(pnextchildit) {
			pnextchildit->parent.pprevious = pit;
		} else {
			((OV_NMHEAD*)((pparent->v_linktable)+parentoffset))->plast = pit;
		}
		/*
		*   set pointers of predecessor of child object
		*/
		if(ppreviousparentit) {
			ppreviousparentit->child.pnext = pit;
		} else {
			((OV_NMHEAD*)((pchild->v_linktable)+childoffset))->pfirst = pit;
		}
		/*
		*   set pointers of successor of child object
		*/
		if(pnextparentit) {
			pnextparentit->child.pprevious = pit;
		} else {
			((OV_NMHEAD*)((pchild->v_linktable)+childoffset))->plast = pit;
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
	case OV_AT_ONE_TO_ONE:
		if(Ov_Association_GetParent(passoc, pchild) != pparent) {
			return;
		}
		break;
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
	case OV_AT_ONE_TO_ONE:
		break;
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
	case OV_AT_ONE_TO_ONE:
		break;
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
	case OV_AT_ONE_TO_ONE:
		*( (OV_INSTPTR_ov_object*) ((pparent->v_linktable)+parentoffset) )= NULL;
		*( (OV_INSTPTR_ov_object*) ((pchild->v_linktable)+childoffset) )= NULL;
		break;
	case OV_AT_ONE_TO_MANY:
		/*
		*	if there's no child predecessor and no successor empty the parent's list
		*/
		if((!ppreviouschild)&&!(pnextchild)) {
			((OV_HEAD*)(pparent->v_linktable+parentoffset))->pfirst = NULL;
			((OV_HEAD*)(pparent->v_linktable+parentoffset))->plast = NULL;
		} else {
			/*
			*	set pointers of child predecessor and successor, and/or parent object
			*/
			if(ppreviouschild) {
				((OV_ANCHOR*)(ppreviouschild->v_linktable+childoffset))->pnext
					= pnextchild;
			} else {
				((OV_HEAD*)(pparent->v_linktable+parentoffset))->pfirst
					= pnextchild;
			}
			if(pnextchild) {
				((OV_ANCHOR*)(pnextchild->v_linktable+childoffset))->pprevious
					= ppreviouschild;
			} else {
				((OV_HEAD*)(pparent->v_linktable+parentoffset))->plast
					= ppreviouschild;
			}
		}
		/*
		*	reset the child object's pointers
		*/
		((OV_ANCHOR*)(pchild->v_linktable+childoffset))->pprevious = NULL;
		((OV_ANCHOR*)(pchild->v_linktable+childoffset))->pnext = NULL;
		((OV_ANCHOR*)(pchild->v_linktable+childoffset))->pparent = NULL;
		break;
	case OV_AT_MANY_TO_MANY:
		/*
		*	if there's no parent predecessor and no successor empty the parent's list
		*/
		if((!ppreviouschildit)&&!(pnextchildit)) {
			((OV_NMHEAD*)(pparent->v_linktable+parentoffset))->pfirst = NULL;
			((OV_NMHEAD*)(pparent->v_linktable+parentoffset))->plast = NULL;
		} else {
			/*
			*	set pointers of child predecessor and successor, and/or parent object
			*/
			if(ppreviouschildit) {
				ppreviouschildit->parent.pnext = pnextchildit;
			} else {
				((OV_NMHEAD*)(pparent->v_linktable+parentoffset))->pfirst = pnextchildit;
			}
			if(pnextchildit) {
				pnextchildit->parent.pprevious = ppreviouschildit;
			} else {
				((OV_NMHEAD*)(pparent->v_linktable+parentoffset))->plast = ppreviouschildit;
			}
		}
		/*
		*	if there's no child predecessor and no successor empty the child's list
		*/
		if((!ppreviousparentit)&&!(pnextparentit)) {
			((OV_NMHEAD*)(pchild->v_linktable+childoffset))->pfirst = NULL;
			((OV_NMHEAD*)(pchild->v_linktable+childoffset))->plast = NULL;
		} else {
			/*
			*	set pointers of parent predecessor and successor, and/or parent object
			*/
			if(ppreviousparentit) {
				ppreviousparentit->child.pnext = pnextparentit;
			} else {
				((OV_NMHEAD*)(pchild->v_linktable+childoffset))->pfirst = pnextparentit;
			}
			if(pnextparentit) {
				pnextparentit->child.pprevious = ppreviousparentit;
			} else {
				((OV_NMHEAD*)(pchild->v_linktable+childoffset))->plast = ppreviousparentit;
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
	case OV_AT_ONE_TO_ONE:
		if ( *(OV_INSTPTR_ov_object*) ((pparent->v_linktable)+passoc->v_parentoffset) ) return TRUE;
		return FALSE;
	case OV_AT_ONE_TO_MANY:
		/* fall into... */
	case OV_AT_MANY_TO_MANY:
		/*
		*	get pointer to parent link (head)
		*/
		phead = (OV_HEAD*)((pparent->v_linktable)+passoc->v_parentoffset);
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
	case OV_AT_ONE_TO_ONE:
		if ( *(OV_INSTPTR_ov_object*) ((pchild->v_linktable)+passoc->v_childoffset) ) return TRUE;
		return FALSE;
	case OV_AT_ONE_TO_MANY:
		/*
		*	get pointer to child link (anchor)
		*/
		panchor = (OV_ANCHOR*)((pchild->v_linktable)+passoc->v_childoffset);
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
		phead = (OV_HEAD*)((pchild->v_linktable)+passoc->v_childoffset);
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

OV_DLLFNCEXPORT OV_UINT ov_association_parentoffset_get(
	OV_INSTPTR_ov_association	passoc
) {
	return passoc->v_parentoffset;
}

OV_DLLFNCEXPORT OV_UINT ov_association_childoffset_get(
	OV_INSTPTR_ov_association	passoc
) {
	return passoc->v_childoffset;
}
/*
*	Parent and Child Offset of the ov model associations in the association table
*/

OV_UINT ov_association_getparentoffset(
	OV_ASSOCIATION_DEF*	passocdef
) {
	if (!strcmp(passocdef->identifier,"containment")) return (2*sizeof(OV_ANCHOR));
	if (!strcmp(passocdef->identifier,"instantiation")) return (2*sizeof(OV_ANCHOR)+1*sizeof(OV_HEAD));
	if (!strcmp(passocdef->identifier,"inheritance")) return (2*sizeof(OV_ANCHOR)+2*sizeof(OV_HEAD));
	if (!strcmp(passocdef->identifier,"childrelationship")) return (3*sizeof(OV_ANCHOR)+3*sizeof(OV_HEAD));
	if (!strcmp(passocdef->identifier,"parentrelationship")) return (3*sizeof(OV_ANCHOR)+4*sizeof(OV_HEAD));
	if (!strcmp(passocdef->identifier,"embedment")) return (3*sizeof(OV_ANCHOR)+5*sizeof(OV_HEAD));
	if (!strcmp(passocdef->identifier,"construction")) return (2*sizeof(OV_ANCHOR)+1*sizeof(OV_HEAD));
	return 0;
}

OV_UINT ov_association_getchildoffset(
	OV_ASSOCIATION_DEF*	passocdef
) {
	if (!strcmp(passocdef->identifier,"instantiation")) return 0;
	if (!strcmp(passocdef->identifier,"containment")) return (1*sizeof(OV_ANCHOR));
	if (!strcmp(passocdef->identifier,"inheritance")) return (2*sizeof(OV_ANCHOR)+3*sizeof(OV_HEAD));
	if (!strcmp(passocdef->identifier,"parentrelationship")) return (2*sizeof(OV_ANCHOR));
	if (!strcmp(passocdef->identifier,"childrelationship")) return (3*sizeof(OV_ANCHOR));
	if (!strcmp(passocdef->identifier,"embedment")) return (2*sizeof(OV_ANCHOR));
	if (!strcmp(passocdef->identifier,"construction")) return (2*sizeof(OV_ANCHOR));
	return 0;
}

OV_UINT ov_association_gettablesize(
	OV_CLASS_DEF*	pclassdef
) {
	if (!strcmp(pclassdef->identifier,"object")) return (2*sizeof(OV_ANCHOR));
	if (!strcmp(pclassdef->identifier,"domain")) return (2*sizeof(OV_ANCHOR)+1*sizeof(OV_HEAD));
	if (!strcmp(pclassdef->identifier,"operation")) return (2*sizeof(OV_ANCHOR));
	if (!strcmp(pclassdef->identifier,"variable")) return (3*sizeof(OV_ANCHOR));
	if (!strcmp(pclassdef->identifier,"part")) return (3*sizeof(OV_ANCHOR));
	if (!strcmp(pclassdef->identifier,"association")) return (4*sizeof(OV_ANCHOR));
	if (!strcmp(pclassdef->identifier,"class")) return (3*sizeof(OV_ANCHOR)+6*sizeof(OV_HEAD));
	if (!strcmp(pclassdef->identifier,"library")) return (2*sizeof(OV_ANCHOR)+1*sizeof(OV_HEAD));
	if (!strcmp(pclassdef->identifier,"structure")) return (2*sizeof(OV_ANCHOR)+2*sizeof(OV_HEAD));
	return 0;
}

/*	----------------------------------------------------------------------	*/

/*
*	Allocate memory for the new linktable
*/
OV_RESULT ov_association_linktable_allocate(
	OV_INSTPTR_ov_class		pclass,
	OV_INT				addsize
) {
	OV_INSTPTR_ov_class		pderivedclass;
	OV_INSTPTR_ov_class		pnextclass;
	OV_INSTPTR_ov_object		pinst;
	OV_INSTPTR_ov_object		pnextinst;
	OV_ATBLPTR			pat;
	OV_RESULT			result;

	/*
	*	iterate over all derived classes
	*/
	pderivedclass = Ov_GetFirstChild(ov_inheritance, pclass);
	while (pderivedclass) {
		pnextclass = Ov_GetNextChild(ov_inheritance, pderivedclass);
		result = ov_association_linktable_allocate(pderivedclass, addsize);
		if(Ov_Fail(result)) {
			return result;
		}
		pderivedclass = pnextclass;
	}

	/*
	*	iterate over all instances of this class
	*/
	pinst = Ov_GetFirstChild(ov_instantiation, pclass);
	while (pinst) {
		pnextinst = Ov_GetNextChild(ov_instantiation, pinst);
		pat = (OV_ATBLPTR) ov_database_malloc(pclass->v_linktablesize + addsize);
		if(!pat) {
			return OV_ERR_DBOUTOFMEMORY;
		}
		memset(pat, 0, pclass->v_linktablesize + addsize);
		memcpy(pat, pinst->v_linktable, pclass->v_linktablesize);
		ov_database_free(pinst->v_linktable);
		pinst->v_linktable = pat;
		pinst = pnextinst;
	}
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Calculation of linkoffsets and linktablesizes of derived classes, when   
*	inserting a new link with 'addsize' bytes at the linktable address 'offset'
* 	using the recuriv function 'linktable_insert'
*/
void ov_association_linktable_insert(
	OV_INSTPTR_ov_class		passocclass,
	OV_INSTPTR_ov_class		pclass,
	OV_INT				addsize,
	OV_UINT				offset
) {
	OV_INSTPTR_ov_association	passoc;
	OV_INSTPTR_ov_association	pchildassoc;
	OV_INSTPTR_ov_association	pparentassoc;
	OV_INSTPTR_ov_association	pnextassoc;
	OV_INSTPTR_ov_class		pderivedclass;
	OV_INSTPTR_ov_class		pnextclass;
	OV_INSTPTR_ov_object		pinst;
	OV_INSTPTR_ov_object		pnextinst;
	char 				*pcs, *pct;

	/*
	*	when adjusting the offsets of the child and parent association, we store the
	*	actual offsets of the associations, so we can prevent the changing
	*	of teir offsets before the linktable of the instances has been changed
	*/

	/*
	*	iterate over all derived classes and call recursive this method
	*/
	pderivedclass = Ov_GetFirstChild(ov_inheritance, pclass);
	while (pderivedclass) {
		pnextclass = Ov_GetNextChild(ov_inheritance, pderivedclass);
		ov_association_linktable_insert(passocclass, pderivedclass, addsize, offset);
		pderivedclass = pnextclass;
	}

	/*
	*	First of all get the first child of 'instantiation' , 'childrelationship' and 'parentrelationship'.
	*	If the actual instance is an ov-association, its linktable could be changed and no further access would be correct
	*	So change the association offsets before changing the linktable of the instances
	*	Because the childlink offset of 'instantiation' will never change (in contrast to it's parentlink offset)
	*	we keep completly access to the following childs of this association.
	*/

	pinst = Ov_GetFirstChild(ov_instantiation, pclass);
	pchildassoc = Ov_GetFirstChild(ov_childrelationship, pclass);
	pparentassoc = Ov_GetFirstChild(ov_parentrelationship, pclass);

	/*
	*	the association offsets of derived classes (and only of these) has to be adjusted
	*/
	if (passocclass!=pclass) {
		/*
		*	iterate over all child associations defined to this class and add the inserted datasize to the childoffset
		*/
		passoc = pchildassoc;
		while (passoc) {
			pnextassoc = Ov_GetNextChild(ov_childrelationship, passoc);
			passoc->v_childoffset += addsize;
			passoc = pnextassoc;
		}
	
		/*
		*	iterate over all parent associations defined to this class and add the inserted datasize to the parentoffset
		*/
		passoc = pparentassoc;
		while (passoc) {
			pnextassoc = Ov_GetNextChild(ov_parentrelationship, passoc);
			passoc->v_parentoffset += addsize;
			passoc = pnextassoc;
		}
	}

	/*
	*	now iterate over all instances of the actual class
	*/

	while (pinst) {
		pnextinst = Ov_GetNextChild(ov_instantiation, pinst);
		if (addsize >=0) {
			pcs = pinst->v_linktable+pclass->v_linktablesize-1;
			pct = pinst->v_linktable+pclass->v_linktablesize-1+addsize;
			while ((pcs-pinst->v_linktable) >= offset) {
				*pct = *pcs;
				pct--;
				pcs--;
			}
			pcs = pinst->v_linktable+offset+addsize-1;
			while ((pcs-pinst->v_linktable) >= offset) {
				*pcs = 0;
				pcs--;
			}
		}
		else {
			pcs = pinst->v_linktable+offset-addsize;
			pct = pinst->v_linktable+offset;
			while ((pcs-pinst->v_linktable) < (pclass->v_linktablesize)) {
				*pct = *pcs;
				pct++;
				pcs++;
			}
			while ((pct-pinst->v_linktable) < pclass->v_linktablesize) {
				*pct = 0;
				pct++;
			}
		}
		pinst = pnextinst;
	}

	/*
	*	finaly adjust the linktablesize of the actual class
	*/
	pclass->v_linktablesize += addsize;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

