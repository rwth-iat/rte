/*
*   $Id: ov_association.h,v 1.5 1999-08-30 15:23:27 dirk Exp $
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
	OV_UINT						parentoffset;
	OV_UINT						childoffset;
	OV_STRING					parentcomment;
	OV_STRING					childcomment;
	OV_UINT						parentflags;
	OV_UINT						childflags;
	OV_FNCPTR_LINK				linkfnc;
	OV_FNCPTR_UNLINK			unlinkfnc;
	OV_FNCPTR_GETACCESS			getaccessfnc;
};
typedef struct OV_ASSOCIATION_DEF OV_ASSOCIATION_DEF;

/*
*	Define the link connectors of a 1:n association
*/
#define OV_TYPEDEF_LINKS(assoc)										\
	typedef struct {												\
		OV_CPT_##assoc	pfirst;										\
		OV_CPT_##assoc	plast;										\
	}   OV_PARENTLINK_##assoc;										\
	typedef struct {												\
		OV_CPT_##assoc	pnext;										\
		OV_CPT_##assoc	pprevious;									\
		OV_PPT_##assoc	pparent;									\
	}   OV_CHILDLINK_##assoc

/*
*	Define the link connectors of an n:m association
*/
#define OV_TYPEDEF_NMLINKS(assoc)									\
	typedef struct {												\
		struct OV_NMLINK_##assoc		*pfirst;					\
		struct OV_NMLINK_##assoc		*plast;						\
	}   OV_PARENTLINK_##assoc;										\
	typedef struct {												\
		struct OV_NMLINK_##assoc		*pfirst;					\
		struct OV_NMLINK_##assoc		*plast;						\
	}   OV_CHILDLINK_##assoc;										\
	struct OV_NMLINK_##assoc {										\
		struct {													\
			struct OV_NMLINK_##assoc	*pnext;						\
			struct OV_NMLINK_##assoc	*pprevious;					\
			OV_PPT_##assoc				pparent;					\
		}	parent;													\
		struct {													\
			struct OV_NMLINK_##assoc	*pnext;						\
			struct OV_NMLINK_##assoc	*pprevious;					\
			OV_CPT_##assoc				pchild;						\
		}	child;													\
	};																\
	typedef struct OV_NMLINK_##assoc OV_NMLINK_##assoc

/*
*	Declare the link function of an association
*/
#define OV_DECL_LINK(assoc) 										\
	OV_RESULT OV_DLLFNCEXPORT assoc##_link(							\
		const OV_PPT_##assoc	pparent,							\
		const OV_CPT_##assoc	pchild,								\
		const OV_PLACEMENT_HINT	parenthint,							\
		const OV_CPT_##assoc	prelparent,							\
		const OV_PLACEMENT_HINT	childhint,							\
		const OV_CPT_##assoc	prelchild							\
	)

/*
*	Declare the unlink function of an association
*/
#define OV_DECL_UNLINK(assoc) 										\
	void OV_DLLFNCEXPORT assoc##_unlink(							\
		const OV_PPT_##assoc	pparent,							\
		const OV_CPT_##assoc	pchild								\
	)

/*
*	Declare the getaccess function of an association
*/
#define OV_DECL_GETACCESS(assoc)									\
	OV_ACCESS OV_DLLFNCEXPORT assoc##_getaccess(					\
		const OV_PPT_##assoc	pparent,							\
		const OV_CPT_##assoc	pchild,								\
		const OV_TICKET			*pticket							\
	)

/*
*	Default implementation for the link function of an association
*/
#define OV_IMPL_LINK(assoc) 										\
	OV_DECL_LINK(assoc) {											\
		return ov_association_link(passoc_##assoc, Ov_PtrUpCast		\
			(ov_object,	pparent), Ov_PtrUpCast(ov_object, pchild),	\
			parenthint, Ov_PtrUpCast(ov_object, prelparent),		\
			childhint, Ov_PtrUpCast(ov_object, prelchild));			\
	}

/*
*	Default implementation for the unlink function of an association
*/
#define OV_IMPL_UNLINK(assoc) 										\
	OV_DECL_UNLINK(assoc) {											\
		ov_association_unlink(passoc_##assoc, Ov_PtrUpCast			\
			(ov_object, pparent), Ov_PtrUpCast(ov_object, pchild));	\
	}

/*
*	Default implementation for the getaccess function of an association
*/
#define OV_IMPL_GETACCESS(assoc)									\
	OV_DECL_GETACCESS(assoc) {										\
		return OV_AC_READ | OV_AC_LINKABLE | OV_AC_UNLINKABLE;		\
	}

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
*	Link attribute in a 1:n association: head in doubly linked list.
*/
typedef struct {
	OV_INSTPTR_ov_object	pfirst;
	OV_INSTPTR_ov_object	plast;
}	OV_HEAD;

/*
*	OV_ANCHOR:
*	----------
*	Link attribute in a 1:n association: anchor in doubly linked list.
*/
typedef struct {
	OV_INSTPTR_ov_object	pnext;
	OV_INSTPTR_ov_object	pprevious;
	OV_INSTPTR_ov_object	pparent;
}	OV_ANCHOR;

/*
*	OV_NMHEAD:
*	----------
*	Link attribute in a n:m association: head in doubly linked list.
*/
typedef struct {
	struct OV_NMLINK			*pfirst;
	struct OV_NMLINK			*plast;
}	OV_NMHEAD;

/*
*	OV_NMLINK:
*	----------
*	Link "object" in a n:m association: it contains	two anchors 
*	of doubly linked list (and the creation time of the link).
*/
struct OV_NMLINK {
	struct {
		struct OV_NMLINK		*pnext;
		struct OV_NMLINK		*pprevious;
		OV_INSTPTR_ov_object	pparent;
	}	parent;
	struct {
		struct OV_NMLINK		*pnext;
		struct OV_NMLINK		*pprevious;
		OV_INSTPTR_ov_object	pchild;
	}	child;
};
typedef struct OV_NMLINK OV_NMLINK;

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
*	Get first child in a 1:n association
*/
#define Ov_Association_GetFirstChild(passoc, pparent)				\
	((OV_INSTPTR_ov_object)((pparent)?(((OV_HEAD*)(((OV_BYTE*)		\
	(pparent))+(passoc)->v_parentoffset))->pfirst):(NULL)))

/*
*	Get last child in a 1:n association
*/
#define Ov_Association_GetLastChild(passoc, pparent)				\
	((OV_INSTPTR_ov_object)((pparent)?(((OV_HEAD*)(((OV_BYTE*)		\
	(pparent))+(passoc)->v_parentoffset))->plast):(NULL)))

/*
*	Get next child in a 1:n association
*/
#define Ov_Association_GetNextChild(passoc, pchild)					\
	((OV_INSTPTR_ov_object)((pchild)?(((OV_ANCHOR*)(((OV_BYTE*)		\
	(pchild))+(passoc)->v_childoffset))->pnext):(NULL)))

/*
*	Get previous child in a 1:n association
*/
#define Ov_Association_GetPreviousChild(passoc, pchild)				\
	((OV_INSTPTR_ov_object)((pchild)?(((OV_ANCHOR*)(((OV_BYTE*)		\
	(pchild))+(passoc)->v_childoffset))->pprevious):(NULL)))

/*
*	Get parent in a 1:n association
*/
#define Ov_Association_GetParent(passoc, pchild)					\
	((OV_INSTPTR_ov_object)((pchild)?(((OV_ANCHOR*)(((OV_BYTE*)		\
	(pchild))+(passoc)->v_childoffset))->pparent):(NULL)))

/*
*	Iterate over all children in an 1:n association
*/
#define Ov_Association_ForEachChild(passoc, pparent, pchild)		\
	for((pchild)=Ov_Association_GetFirstChild((passoc), (pparent));	\
	(pchild); (pchild)=Ov_Association_GetNextChild((passoc),		\
	(pchild)))
	
/*
*	Define an iterator for iterating over n:m associations
*/
#define Ov_Association_DefineIteratorNM(pit)						\
	OV_NMLINK *(pit) = NULL

/*
*	Get first child in an n:m assocation
*/
#define Ov_Association_GetFirstChildNM(passoc, pit, pparent)		\
	((OV_INSTPTR_ov_object)((pparent)?((pit)=((OV_NMHEAD*)			\
	(((OV_BYTE*)(pparent))+(passoc)->v_parentoffset))->pfirst,		\
	(pit)?((pit)->child.pchild):(NULL)):(NULL)))

/*
*	Get last child in an n:m assocation
*/
#define Ov_Association_GetLastChildNM(passoc, pit, pparent)			\
	((OV_INSTPTR_ov_object)((pparent)?((pit)=((OV_NMHEAD*)			\
	(((OV_BYTE*)(pparent))+(passoc)->v_parentoffset))->plast,		\
	(pit)?((pit)->child.pchild):(NULL)):(NULL)))

/*
*	Get next child in an n:m association
*/
#define Ov_Association_GetNextChildNM(passoc, pit)					\
	((OV_INSTPTR_ov_object)((pit)?((pit)=(pit)->parent.pnext,		\
	(pit)?((pit)->child.pchild):(NULL)):(NULL)))

/*
*	Get previous child in an n:m association
*/
#define Ov_Association_GetPreviousChildNM(passoc, pit)				\
	((OV_INSTPTR_ov_object)((pit)?((pit)=(pit)->parent.				\
	pprevious, (pit)?((pit)->child.pchild):(NULL)):(NULL)))

/*
*	Iterate over all children in an n:m association
*/
#define Ov_Association_ForEachChildNM(passoc, pit, pparent, pchild)	\
	for((pchild)=Ov_Association_GetFirstChildNM((passoc), (pit),	\
	(pparent)); (pchild); (pchild)=Ov_Association_GetNextChildNM	\
	((passoc), (pit)))

/*
*	Get first parent in an n:m assocation
*/
#define Ov_Association_GetFirstParentNM(passoc, pit, pchild)		\
	((OV_INSTPTR_ov_object)((pchild)?((pit)=((OV_NMHEAD*)			\
	(((OV_BYTE*)(pchild))+(passoc)->v_childoffset))->pfirst,		\
	(pit)?((pit)->parent.pparent):(NULL)):(NULL)))

/*
*	Get last parent in an n:m assocation
*/
#define Ov_Association_GetLastParentNM(passoc, pit, pchild)			\
	((OV_INSTPTR_ov_object)((pchild)?((pit)=((OV_NMHEAD*)			\
	(((OV_BYTE*)(pchild))+(passoc)->v_childoffset))->plast,			\
	(pit)?((pit)->parent.pparent):(NULL)):(NULL)))

/*
*	Get next parent in an n:m association
*/
#define Ov_Association_GetNextParentNM(passoc, pit)					\
	((OV_INSTPTR_ov_object)((pit)?((pit)=(pit)->child.pnext,		\
	(pit)?((pit)->parent.pparent):(NULL)):(NULL)))

/*
*	Get previous parent in an n:m association
*/
#define Ov_Association_GetPreviousParentNM(passoc, pit)				\
	((OV_INSTPTR_ov_object)((pit)?((pit)=(pit)->child.				\
	pprevious, (pit)?((pit)->parent.pparent):(NULL)):(NULL)))

/*
*	Iterate over all parents in an n:m association
*/
#define Ov_Association_ForEachParentNM(passoc, pit, pchild, pparent)\
	for((pparent)=Ov_Association_GetFirstParentNM((passoc), (pit),	\
	(pchild)); (pparent); (pparent)=Ov_Association_GetNextParentNM	\
	((passoc), (pit)))

/*
*	Search for child with a given identifier in a 1:n association
*/
OV_INSTPTR_ov_object OV_DLLFNCEXPORT ov_association_searchchild(
	const OV_INSTPTR_ov_association	passoc,
	const OV_INSTPTR_ov_object		pparent,
	const OV_STRING					identifier
);

/*
*	Get the number of parents of an association
*/
OV_UINT OV_DLLFNCEXPORT ov_association_getparentcount(
	const OV_INSTPTR_ov_association	passoc,
	const OV_INSTPTR_ov_object		pchild
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
	const OV_PLACEMENT_HINT			parenthint,
	const OV_INSTPTR_ov_object		prelparent,
	const OV_PLACEMENT_HINT			childhint,
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
*	Test if a parent link is used
*/
OV_BOOL OV_DLLFNCEXPORT ov_association_isusedparentlink(
	const OV_INSTPTR_ov_association	passoc,
	const OV_INSTPTR_ov_object		pparent
);

/*
*	Test if an child link is used
*/
OV_BOOL OV_DLLFNCEXPORT ov_association_isusedchildlink(
	const OV_INSTPTR_ov_association	passoc,
	const OV_INSTPTR_ov_object		pchild
);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif

#endif
/*
*	End of file
*/

