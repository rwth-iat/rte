/*
*   $Id: ov_association.h,v 1.1 1999-07-19 15:02:03 dirk Exp $
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
*/

#ifndef OV_ASSOCIATION_H_INCLUDED1
#define OV_ASSOCIATION_H_INCLUDED1

#include "libov/ov_ov.h"
#include "ov.ovt"

/*
*	OV_ASSOCIATION_DEF:
*	-------------------
*	Association definition, included in the library's code
*/
struct OV_ASSOCIATION_DEF {
	struct OV_ASSOCIATION_DEF	*pnext;
	OV_STRING					identifier;
	OV_ASSOC_TYPE				assoctype;
	OV_ASSOC_PROPS				assocprops;
	OV_STRING					childrolename;
	OV_STRING					parentclassname;
	OV_STRING					parentrolename;
	OV_STRING					childclassname;
	OV_UINT						headoffset;
	OV_UINT						anchoroffset;
	OV_STRING					headcomment;
	OV_STRING					anchorcomment;
	OV_UINT						headflags;
	OV_UINT						anchorflags;
	OV_FNCPTR_LINK				linkfnc;
	OV_FNCPTR_UNLINK			unlinkfnc;
	OV_FNCPTR_GETACCESS			getaccessfnc;
};
typedef struct OV_ASSOCIATION_DEF OV_ASSOCIATION_DEF;

/*
*	Define the link connectors of an association
*/
#define OV_TYPEDEF_LINKCONNECTORS(assoc)			\
	typedef struct {								\
		OV_CPT_##assoc	pfirst;						\
		OV_CPT_##assoc	plast;						\
	}   OV_HEAD_##assoc;							\
	typedef struct {								\
		OV_CPT_##assoc	pnext;						\
		OV_CPT_##assoc	pprevious;					\
		OV_PPT_##assoc	pparent;					\
	}   OV_ANCHOR_##assoc

/*
*	Declare the link function of an association
*/
#define OV_DECL_LINK(assoc) 						\
	OV_RESULT OV_DLLFNCEXPORT assoc##_link(			\
		const OV_PPT_##assoc	pparent,			\
		const OV_CPT_##assoc	pchild,				\
		const OV_PLACEMENT_HINT	hint,				\
		const OV_CPT_##assoc	prelchild			\
	)

/*
*	Declare the unlink function of an association
*/
#define OV_DECL_UNLINK(assoc) 						\
	void OV_DLLFNCEXPORT assoc##_unlink(			\
		const OV_PPT_##assoc	pparent,			\
		const OV_CPT_##assoc	pchild				\
	)

/*
*	Declare the getaccess function of an association
*/
#define OV_DECL_GETACCESS(assoc)					\
	OV_ACCESS OV_DLLFNCEXPORT assoc##_getaccess(	\
		const OV_PPT_##assoc	pparent,			\
		const OV_CPT_##assoc	pchild,				\
		const OV_TICKET			*pticket			\
	)
#endif

#include "ov.h"

#ifdef OV_INSTBODY_ov_object

#ifndef OV_ASSOCIATION_H_INCLUDED2
#define OV_ASSOCIATION_H_INCLUDED2

#ifdef __cplusplus
extern "C" {
#endif

/*
*	OV_HEAD:
*	--------
*	Link attribute in an object acting as an parent in an association.
*/
typedef struct {
	OV_INSTPTR_ov_object	pfirst;
	OV_INSTPTR_ov_object	plast;
}	OV_HEAD;

/*
*	OV_ANCHOR:
*	----------
*	Link attribute in an object acting as a child in an association.
*/
typedef struct {
	OV_INSTPTR_ov_object	pnext;
	OV_INSTPTR_ov_object	pprevious;
	OV_INSTPTR_ov_object	pparent;
}	OV_ANCHOR;

/*
*	Load an association into the database
*/
OV_RESULT ov_association_load(
	OV_ASSOCIATION_DEF*				passocdef,
	OV_INSTPTR_ov_domain			pparent
);

/*
*	Compare an association with its definition
*/
OV_RESULT ov_association_compare(
	OV_INSTPTR_ov_association		passoc,
	OV_ASSOCIATION_DEF*				passocdef
);

/*
*	Test if we can unload an association from the database
*/
OV_BOOL ov_association_canunload(
	OV_INSTPTR_ov_association	passoc
);

/*
*	Get first child in an association
*/
#define Ov_Association_GetFirstChild(passoc, pparent)			\
	((OV_INSTPTR_ov_object)(pparent?(((OV_HEAD*)((OV_BYTE*)		\
	pparent+passoc->v_headoffset))->pfirst):NULL))

/*
*	Get last child in an association
*/
#define Ov_Association_GetLastChild(passoc, pparent)			\
	((OV_INSTPTR_ov_object)(pparent?(((OV_HEAD*)((OV_BYTE*)		\
	pparent+passoc->v_headoffset))->plast):NULL))

/*
*	Get next child in an association
*/
#define Ov_Association_GetNextChild(passoc, pchild)				\
	((OV_INSTPTR_ov_object)(pchild?(((OV_ANCHOR*)((OV_BYTE*)	\
	pchild+passoc->v_anchoroffset))->pnext):NULL))

/*
*	Get previous child in an association
*/
#define Ov_Association_GetPrevChild(passoc, pchild)				\
	((OV_INSTPTR_ov_object)(pchild?(((OV_ANCHOR*)((OV_BYTE*)	\
	pchild+passoc->v_anchoroffset))->pprevious):NULL))

/*
*	Get parent in an association
*/
#define Ov_Association_GetParent(passoc, pchild)				\
	((OV_INSTPTR_ov_object)(pchild?(((OV_ANCHOR*)((OV_BYTE*)	\
	pchild+passoc->v_anchoroffset))->pparent):NULL))

/*
*	Search for child with a given identifier in an association
*/
OV_INSTPTR_ov_object OV_DLLFNCEXPORT ov_association_searchchild(
	const OV_INSTPTR_ov_association	passoc,
	const OV_INSTPTR_ov_object		pparent,
	const OV_STRING					identifier
);

/*
*	Get the number of children of an association
*/
OV_UINT OV_DLLFNCEXPORT ov_association_getchildcount(
	const OV_INSTPTR_ov_association	passoc,
	const OV_INSTPTR_ov_object		pparent
);

/*
*	Link a parent and a child object
*/
OV_RESULT OV_DLLFNCEXPORT ov_association_link(
	const OV_INSTPTR_ov_association	passoc,
	const OV_INSTPTR_ov_object		pparent,
	const OV_INSTPTR_ov_object		pchild,
	const OV_PLACEMENT_HINT			hint,
	const OV_INSTPTR_ov_object		prelchild
);

/*
*	Unlink a parent and a child object
*/
void OV_DLLFNCEXPORT ov_association_unlink(
	const OV_INSTPTR_ov_association	passoc,
	const OV_INSTPTR_ov_object		pparent,
	const OV_INSTPTR_ov_object		pchild
);

/*
*	Test if a head is used
*/
OV_BOOL OV_DLLFNCEXPORT ov_association_isusedhead(
	const OV_INSTPTR_ov_association	passoc,
	const OV_INSTPTR_ov_object		pparent
);

/*
*	Test if an anchor is used
*/
OV_BOOL OV_DLLFNCEXPORT ov_association_isusedanchor(
	const OV_INSTPTR_ov_association	passoc,
	const OV_INSTPTR_ov_object		pchild
);

/*
*	Default implementation for the link function of an association
*/
#define OV_IMPL_LINK(assoc) 												\
	OV_DECL_LINK(assoc) {													\
		return ov_association_link(passoc_##assoc, Ov_PtrUpCast(ov_object,	\
			pparent), Ov_PtrUpCast(ov_object, pchild), hint, 				\
			Ov_PtrUpCast(ov_object, prelchild));							\
	}

/*
*	Default implementation for the unlink function of an association
*/
#define OV_IMPL_UNLINK(assoc) 												\
	OV_DECL_UNLINK(assoc) {													\
		ov_association_unlink(passoc_##assoc, Ov_PtrUpCast(ov_object, 		\
			pparent), Ov_PtrUpCast(ov_object, pchild));						\
	}

/*
*	Default implementation for the getaccess function of an association
*/
#define OV_IMPL_GETACCESS(assoc)											\
	OV_DECL_GETACCESS(assoc) {												\
		return OV_AC_READ | OV_AC_LINKABLE | OV_AC_UNLINKABLE;				\
	}

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif

#endif
/*
*	End of file
*/

