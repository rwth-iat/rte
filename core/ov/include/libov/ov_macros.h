/*
*   $Id: ov_macros.h,v 1.23 2011-01-18 14:27:34 martin Exp $
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
*	08-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	04-Nov-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: variable type ANY added.
*/

#ifndef OV_MACROS_H_INCLUDED
#define OV_MACROS_H_INCLUDED

// for strdup
#ifdef OV_COMPILE_LIBOV
#define _POSIX_C_SOURCE 200809L
#include <string.h>
#endif

#include "libov/ov_association.h"
#include "libov/ov_class.h"
#include "libov/ov_time.h"
#include "libov/ov_string.h"
#include "libov/ov_vector.h"
#include "libov/ov_database.h"
#include "libov/ov_memstack.h"
#include "libov/ov_malloc.h"

/*
*	Upcast of an instance pointer of the parent class
*/
#define Ov_ParentPtrUpCast(assoc, pparent)									\
	((OV_PPT_##assoc)((pparent)+(0*((pparent)->__classinfo.OV_PCI_##assoc))))

/*
*	Upcast of an instance pointer of the child class
*/
#define Ov_ChildPtrUpCast(assoc, pchild)									\
	((OV_CPT_##assoc)((pchild)+(0*((pchild)->__classinfo.					\
	OV_CCI_##assoc))))

/*
*	Get child in a 1:1 association
*/
#define Ov_GetChild(assoc, pparent)									\
	((OV_CPT_##assoc)((pparent)?(((pparent)->v_linktable)?(*((OV_INSTPTR*)((pparent)->v_linktable+passoc_##assoc->v_parentoffset \
	+(0*((pparent)->__classinfo.OV_PCI_##assoc))))):(NULL)):(NULL)))

/**
*	Get first child in a 1:n association or NULL
*/
#define Ov_GetFirstChild(assoc, pparent)									\
	((OV_CPT_##assoc)((pparent)?(((pparent)->v_linktable)?(((OV_HEAD*)((pparent)->v_linktable+passoc_##assoc->v_parentoffset \
	+(0*((pparent)->__classinfo.OV_PCI_##assoc))))->pfirst):(NULL)):(NULL)))

/**
*	Get last child in a 1:n association or NULL
*/
#define Ov_GetLastChild(assoc, pparent)										\
	((OV_CPT_##assoc)((pparent)?(((pparent)->v_linktable)?(((OV_HEAD*)((pparent)->v_linktable+passoc_##assoc->v_parentoffset \
	+(0*((pparent)->__classinfo.OV_PCI_##assoc))))->plast):(NULL)):(NULL)))

/**
*	Get next child (in relation to pchild) in a 1:n association or NULL
*/
#define Ov_GetNextChild(assoc, pchild)										\
	((OV_CPT_##assoc)((pchild)?(((pchild)->v_linktable)?(((OV_ANCHOR*)((pchild)->v_linktable+passoc_##assoc->v_childoffset \
	+(0*((pchild)->__classinfo.OV_CCI_##assoc))))->pnext):(NULL)):(NULL)))

/**
*	Get previous child (in relation to pchild) a 1:n association or NULL
*/
#define Ov_GetPreviousChild(assoc, pchild)									\
	((OV_CPT_##assoc)((pchild)?(((pchild)->v_linktable)?(((OV_ANCHOR*)((pchild)->v_linktable+passoc_##assoc->v_childoffset \
	+(0*((pchild)->__classinfo.OV_CCI_##assoc))))->pprevious):(NULL)):(NULL)))

/**
*	Get parent in a 1:1 or in a 1:n association or NULL
*/
#define Ov_GetParent(assoc, pchild)											\
	((OV_PPT_##assoc)((pchild)?(((pchild)->v_linktable)?((passoc_##assoc->v_assoctype==OV_AT_ONE_TO_MANY)?(((OV_ANCHOR*)((pchild)->v_linktable+passoc_##assoc->v_childoffset \
	+((pchild)->__classinfo.OV_CCI_##assoc)-((pchild)->__classinfo.OV_CCI_##assoc)))->pparent):(*((OV_INSTPTR*)((pchild)->v_linktable+passoc_##assoc->v_childoffset \
	)))):(NULL)):(NULL)))

/**
*	Iterate over all child objects in a 1:n association
*/
#define Ov_ForEachChild(assoc, pparent, pchild)								\
	for(((pchild)=Ov_GetFirstChild(assoc, (pparent))); (pchild);			\
	(pchild)=Ov_GetNextChild(assoc, (pchild)))

/**
*	Iterate over all child objects in a 1:n association and dynamically
*	cast to a given child class
*/
#define Ov_ForEachChildEx(assoc, pparent, pchild, childclass)				\
	for(((pchild)=(OV_INSTPTR_##childclass)Ov_GetFirstChild(assoc, 			\
	(pparent))); (pchild); (pchild)=(OV_INSTPTR_##childclass)				\
	Ov_GetNextChild(assoc, (pchild))) if(!Ov_DynamicPtrCast(childclass, 	\
	(pchild))) {} else

/**
*	Search a child with given identifier in a 1:n association
*/
#define Ov_SearchChild(assoc, pparent, ident)								\
	((OV_CPT_##assoc)ov_association_searchchild(passoc_##assoc, 			\
	(OV_INSTPTR_ov_object)Ov_ParentPtrUpCast(assoc, (pparent)), (ident)))


/**
*	Search a child with given identifier and cast to child class in a 1:n association
*/
#define Ov_SearchChildEx(assoc, pparent, ident, childclass)					\
	Ov_DynamicPtrCast(childclass, ((OV_CPT_##assoc)							\
	ov_association_searchchild(passoc_##assoc, (OV_INSTPTR_ov_object)		\
	Ov_ParentPtrUpCast(assoc, pparent), ident)))

/**
*	Define an iterator for iterating over n:m associations
*	Caution: NM associations are deprecated by Prof Epple
*/
#define Ov_DefineIteratorNM(assoc, pit)										\
	OV_NMLINK *(pit) = NULL

/**
*	Get first child in an n:m association
*	Caution: NM associations are deprecated by Prof Epple
*/
#define Ov_GetFirstChildNM(assoc, pit, pparent)								\
	((OV_CPT_##assoc)(((pparent)?(((pparent)->v_linktable)?((pit)=((OV_NMHEAD*)((pparent)->v_linktable+passoc_##assoc->v_parentoffset \
	+(0*((pparent)->__classinfo.OV_PCI_##assoc))))->pfirst,	\
	(pit)?((pit)->child.pchild):(NULL)):(NULL)):(NULL))))

/**
*	Get last child in an n:m association
*	Caution: NM associations are deprecated by Prof Epple
*/
#define Ov_GetLastChildNM(assoc, pit, pparent)								\
	((OV_CPT_##assoc)(((pparent)?(((pparent)->v_linktable)?((pit)=((OV_NMHEAD*)((pparent)->v_linktable+passoc_##assoc->v_parentoffset \
	+(0*((pparent)->__classinfo.OV_PCI_##assoc))))->plast,	\
	(pit)?((pit)->child.pchild):(NULL)):(NULL)):(NULL))))

/**
*	Get next child in an n:m association
*	Caution: NM associations are deprecated by Prof Epple
*/
#define Ov_GetNextChildNM(assoc, pit)										\
	((OV_CPT_##assoc)((pit)?((pit)=(pit)->parent.pnext, (pit)?((pit)->child	\
	.pchild):(NULL)):(NULL)))

/**
*	Get previous child in an n:m association
*	Caution: NM associations are deprecated by Prof Epple
*/
#define Ov_GetPreviousChildNM(assoc, pit)									\
	((OV_CPT_##assoc)((pit)?((pit)=(pit)->parent.pprevious, (pit)?((pit)	\
	->child.pchild):(NULL)):(NULL)))

/**
*	Get first parent in an n:m association
*	Caution: NM associations are deprecated by Prof Epple
*/
#define Ov_GetFirstParentNM(assoc, pit, pchild)								\
	((OV_PPT_##assoc)(((pchild)?(((pchild)->v_linktable)?((pit)=((OV_NMHEAD*)((pchild)->v_linktable+passoc_##assoc->v_childoffset \
	+(0*((pchild)->__classinfo.OV_CCI_##assoc))))->pfirst,	\
	(pit)?((pit)->parent.pparent):(NULL)):(NULL)):(NULL))))

/**
*	Get last parent in an n:m association
*	Caution: NM associations are deprecated by Prof Epple
*/
#define Ov_GetLastParentNM(assoc, pit, pchild)								\
	((OV_PPT_##assoc)(((pchild)?(((pchild)->v_linktable)?((pit)=((OV_NMHEAD*)((pchild)->v_linktable+passoc_##assoc->v_childoffset \
	+(0*((pchild)->__classinfo.OV_CCI_##assoc))))->plast,	\
	(pit)?((pit)->parent.pparent):(NULL)):(NULL)):(NULL))))

/**
*	Get next parent in an n:m association
*	Caution: NM associations are deprecated by Prof Epple
*/
#define Ov_GetNextParentNM(assoc, pit)										\
	((OV_PPT_##assoc)((pit)?((pit)=(pit)->child.pnext, (pit)?((pit)->parent	\
	.pparent):(NULL)):(NULL)))

/**
*	Get previous parent in an n:m association
*	Caution: NM associations are deprecated by Prof Epple
*/
#define Ov_GetPreviousParentNM(assoc, pit)									\
	((OV_PPT_##assoc)((pit)?((pit)=(pit)->child.pprevious, (pit)?((pit)		\
	->parent.pparent):(NULL)):(NULL)))

/**
*	Iterate over all child objects in an n:m association
*	Caution: NM associations are deprecated by Prof Epple
*/
#define Ov_ForEachChildNM(assoc, pit, pparent, pchild)						\
	for(((pchild)=Ov_GetFirstChildNM(assoc, (pit), (pparent))); (pchild);	\
	(pchild)=Ov_GetNextChildNM(assoc, (pit)))

/**
*	Iterate over all child objects in an n:m association and dynamically
*	cast to a given child class
*	Caution: NM associations are deprecated by Prof Epple
*/
#define Ov_ForEachChildNMEx(assoc, pit, pparent, pchild, childclass)		\
	for(((pchild)=(OV_INSTPTR_##childclass)Ov_GetFirstChildNM(assoc, (pit),	\
	(pparent))); (pchild); (pchild)=(OV_INSTPTR_##childclass)				\
	Ov_GetNextChildNM(assoc, (pit))) if(!Ov_DynamicPtrCast(childclass, 		\
	(pchild))) {} else

/**
*	Iterate over all parent objects in an n:m association
*	Caution: NM associations are deprecated by Prof Epple
*/
#define Ov_ForEachParentNM(assoc, pit, pchild, pparent)						\
	for(((pparent)=Ov_GetFirstParentNM(assoc, (pit), (pchild))); (pparent);	\
	(pparent)=Ov_GetNextParentNM(assoc, (pit)))

/**
*	Iterate over all parent objects in an n:m association and dynamically
*	cast to a given parent class
*	Caution: NM associations are deprecated by Prof Epple
*/
#define Ov_ForEachParentNMEx(assoc, pit, pchild, pparent, parentclass)		\
	for(((pparent)=(OV_INSTPTR_##parentclass)Ov_GetFirstParentNM(assoc,		\
	(pit), (pchild))); (pparent); (pparent)=(OV_INSTPTR_##parentclass)		\
	Ov_GetNextParentNM(assoc, (pit))) if(!Ov_DynamicPtrCast(parentclass, 	\
	(pparent))) {} else

/**
*	Link parent and child object, no placement hint (1:n or n:m association)
*/
#define Ov_Link(assoc, pparent, pchild)										\
	assoc##_link(Ov_ParentPtrUpCast(assoc, (pparent)), Ov_ChildPtrUpCast	\
	(assoc, (pchild)), OV_PMH_DEFAULT, NULL, OV_PMH_DEFAULT, NULL)

/**
*	Link parent and child object with given child placement hint (1:n association)
*/
#define Ov_LinkPlaced(assoc, pparent, pchild, childhint)					\
	assoc##_link(Ov_ParentPtrUpCast(assoc, (pparent)), Ov_ChildPtrUpCast	\
	(assoc, (pchild)), OV_PMH_DEFAULT, NULL, (childhint), NULL)

/**
*	Link parent and child object with given relative child placement hint (1:n association)
*/
#define Ov_LinkRelativePlaced(assoc, pparent, pchild, childhint, prelchild)	\
	assoc##_link(Ov_ParentPtrUpCast(assoc, (pparent)), Ov_ChildPtrUpCast	\
	(assoc, (pchild)), OV_PMH_DEFAULT, NULL, (childhint), Ov_ChildPtrUpCast	\
	(assoc, (prelchild)))

/**
*	Unlink parent and child object (1:n or n:m association)
*/
#define Ov_Unlink(assoc, pparent, pchild)									\
	assoc##_unlink(Ov_ParentPtrUpCast(assoc, (pparent)), Ov_ChildPtrUpCast	\
	(assoc, (pchild)))

/**
*	Link parent and child object, no placement hint (n:m association)
*	Caution: NM associations are deprecated by Prof Epple
*/
#define Ov_LinkNM(assoc, pparent, pchild)									\
	Ov_Link(assoc, (pparent), (pchild))

/**
*	Link parent and child object with given child placement hint (n:m association)
*	Caution: NM associations are deprecated by Prof Epple
*/
#define Ov_LinkPlacedNM(assoc, pparent, pchild, parenthint, childhint)		\
	assoc##_link(Ov_ParentPtrUpCast(assoc, (pparent)), Ov_ChildPtrUpCast	\
	(assoc, (pchild)), (parenthint), NULL, (childhint), NULL)

/**
*	Link parent and child object with given relative placement hints (n:m association)
*	Caution: NM associations are deprecated by Prof Epple
*/
#define Ov_LinkRelativePlacedNM(assoc, pparent, pchild, parenthint,			\
	prelparent, childhint, prelchild)	\
	assoc##_link(Ov_ParentPtrUpCast(assoc, (pparent)), Ov_ChildPtrUpCast	\
	(assoc, (pchild)), (parenthint), Ov_ParentPtrUpCast(assoc, 				\
	(prelparent)), (childhint), Ov_ChildPtrUpCast(assoc, (prelchild)))

/**
*	Unlink parent and child object (1:n or n:m association)
*	Caution: NM associations are deprecated by Prof Epple
*/
#define Ov_UnlinkNM(assoc, pparent, pchild)									\
	Ov_Unlink(assoc, (pparent), (pchild))

/**
*	Upcast to a pointer of a given base class (checks done at compile time)
*/
#define Ov_PtrUpCast(class, pobj) 											\
	((OV_INSTPTR_##class)((pobj)+(0*((pobj)->__classinfo.					\
	is_of_class_##class))))

/**
*	Static cast to a pointer of a given class (no checks are done!)
*/
#define Ov_StaticPtrCast(class, pobj) 										\
	((OV_INSTPTR_##class)(pobj))

/**
*	Test if it is allowed to cast to a given class (checks done at runtime)
*/
#define Ov_CanCastTo(class, pobj)											\
	((Ov_GetParent(ov_instantiation, (pobj))==pclass_##class)?(TRUE):		\
	ov_class_cancastto(Ov_GetParent(ov_instantiation, (pobj)), 				\
	pclass_##class))

/**
*	Dynamic cast to a pointer of a given class (checks done at runtime)
*/
#define Ov_DynamicPtrCast(class, pobj) 										\
	((OV_INSTPTR_##class)(Ov_CanCastTo(class, pobj)?(pobj):(NULL)))

/**
*	Create an object of a given class
*/
#if OV_SYSTEM_LINUX
#define Ov_CreateObject(class, pobj, pparent, ident)						\
	ov_class_createobject(pclass_##class, Ov_PtrUpCast(ov_domain, pparent),	\
	ident, OV_PMH_DEFAULT, NULL, NULL, NULL, ((OV_INSTPTR_ov_object*)&(pobj)))
#else
#define Ov_CreateObject(class, pobj, pparent, ident)						\
	ov_class_createobject(pclass_##class, Ov_PtrUpCast(ov_domain, pparent),	\
	ident, OV_PMH_DEFAULT, NULL, NULL, NULL, ((OV_INSTPTR_ov_object*)		\
	&(pobj))+(0*((pobj)->__classinfo.is_of_class_##class)))
#endif

/**
*	Create an object of a given class with an identifier representing the instance's id
*/
#if OV_SYSTEM_LINUX
#define Ov_CreateIDedObject(class, pobj, pparent, identPostFix)						\
	ov_class_createIDedObject(pclass_##class, Ov_PtrUpCast(ov_domain, pparent),	\
	identPostFix, OV_PMH_DEFAULT, NULL, NULL, NULL, ((OV_INSTPTR_ov_object*)&(pobj)))
#else
#define Ov_CreateIDedObject(class, pobj, pparent, identPostFix)						\
	ov_class_createIDedObject(pclass_##class, Ov_PtrUpCast(ov_domain, pparent),	\
	identPostFix, OV_PMH_DEFAULT, NULL, NULL, NULL, ((OV_INSTPTR_ov_object*)		\
	&(pobj))+(0*((pobj)->__classinfo.is_of_class_##class)))
#endif

/**
*	Delete an object and all its children
*/
#define Ov_DeleteObject(pobj)												\
	ov_class_deleteobject(Ov_PtrUpCast(ov_object, pobj))

/**
*	Get a pointer to the static part of an object
*/
#define Ov_GetStaticInstPtr(class, pobj)									\
	((OV_STATICINSTPTR_##class)(Ov_PtrUpCast(ov_object, pobj)				\
	?(((OV_BYTE*)pclass_##class)+Ov_GetInstSize(ov_class)):(NULL)))

/**
*	Get a pointer to a part object
*/
#define Ov_GetPartPtr(part, pobj)											\
	(&((pobj)->p_##part))

/**
*	Get pointer to the class object of an instance
*	returns a pointer OV_INSTPTR_ov_class
*/
#define Ov_GetClassPtr(pobj)												\
	Ov_GetParent(ov_instantiation, Ov_PtrUpCast(ov_object, pobj))

/**
*	Get the vtable pointer to an object of a given class
*/
#define Ov_GetVTablePtr(class, pvtable, pobj)								\
	pvtable = NULL;															\
	if(pobj) {																\
		OV_INSTPTR_ov_class pclass = Ov_GetClassPtr(Ov_PtrUpCast(class, 	\
			pobj));															\
		if(pclass) {														\
			pvtable = (OV_VTBLPTR_##class)(pclass->v_pvtable);				\
		}																	\
	}

/**
*	Get the vtable pointer of the direct base class of an object
*	of a given class
*/
#define Ov_GetBaseclassVTablePtr(class, pvtable, pobj)						\
	pvtable = ((OV_BVPT_##class)(Ov_GetParent(ov_inheritance, 				\
		Ov_GetClassPtr(Ov_PtrUpCast(class,pobj)))->v_pvtable))

/**
*	Test, if a variable definition object defines a variable with a
*	given name
*/
#define Ov_IsVariable(class, name, pvar)									\
	((pvar)->v_offset == offsetof(OV_INST_##class, v_##name))

/**
*	Set the value of a static vector variable
*	for example:
*	Ov_SetStaticVectorValue(&pObj->v_myTargetStringvec, pObj->v_mySourceStringvec.value, pObj->v_mySourceStringvec.veclen, STRING);
*/
#if OV_SYSTEM_LINUX
#define Ov_SetStaticVectorValue(pvector, pvalue, veclen, type)				\
	ov_vector_setstaticvalue((pvector), (const OV_POINTER)(pvalue),			\
		(veclen), (veclen)*sizeof(OV_##type), OV_VT_##type)
#else
#define Ov_SetStaticVectorValue(pvector, pvalue, veclen, type)				\
	ov_vector_setstaticvalue((pvector), (const OV_POINTER)(pvalue),			\
		(veclen)+0*((pvector)-(OV_##type*)0), (veclen)*sizeof(OV_##type),	\
		OV_VT_##type)
#endif

/**
*	Set the value of a dynamic vector variable
*	for example:
*	Ov_SetDynamicVectorValue(&pObj->v_myTargetStringvec, pObj->v_mySourceStringvec.value, pObj->v_mySourceStringvec.veclen, STRING);
*/
#define Ov_SetDynamicVectorValue(pvector, pvalue, veclen, type)				\
	ov_vector_setdynamicvalue((OV_GENERIC_VEC*)(pvector),					\
		(const OV_POINTER)(pvalue), (veclen)+0*((pvector)					\
		-(OV_##type##_VEC*)0), (veclen)*sizeof(OV_##type), OV_VT_##type)

/**
*	Set the vector length of a dynamic vector variable
*	If the new veclen is bigger than the old values will survive.
*	If the new veclen is smaller than part of the memory will be freed.
*	If the new veclen is zero than all the memory will be freed.
*
*	for example:
*	Ov_SetDynamicVectorLength(&pObj->v_myDynamicStringvec, 42, STRING);
*	will set the vec length and type but can not free the memory of the whole thing
*	Ov_SetDynamicVectorLength(&pObj->v_MyAnyVariable.value.valueunion.val_string_vec, 1, STRING);
*	will free the memory:
*	Ov_SetDynamicVectorLength(&pObj->v_myDynamicStringvec, 0, STRING);
*/
#define Ov_SetDynamicVectorLength(pvector, veclen, type)					\
	ov_vector_setdynamicveclen((OV_GENERIC_VEC*)(pvector), (veclen)			\
		+0*((pvector)-(OV_##type##_VEC*)0),	(veclen)*sizeof(OV_##type),		\
		OV_VT_##type)

/**
*	Set the value of an ANY variable
*	for example:
*	Ov_SetAnyValue(&pObj->v_myTargetAnyvar, &pObj->v_mySourceAnyvar);
*	Ov_SetAnyValue(&pObj->v_myTargetAnyvar, NULL);
*/
#define Ov_SetAnyValue(pany, pvalue)										\
	ov_variable_setanyvalue((pany), (pvalue))

/**
*	Compare two vector variable values
*	Please check against OV_VECCMP_EQUAL, OV_VECCMP_MORE and OV_VECCMP_LESS
*/
#if OV_SYSTEM_LINUX
#define Ov_CompareVectorValues(pvalue1, pvalue2, veclen, type)				\
	ov_vector_compare((const OV_POINTER)(pvalue1),							\
	(const OV_POINTER)(pvalue2), (veclen), (veclen)*sizeof(OV_##type), OV_VT_##type)
#else
#define Ov_CompareVectorValues(pvalue1, pvalue2, veclen, type)				\
	ov_vector_compare((const OV_POINTER)(pvalue1),							\
	(const OV_POINTER)(pvalue2), (veclen)+0*((pvalue1)-(OV_##type*)0)		\
	+0*((pvalue2)-(OV_##type*)0), (veclen)*sizeof(OV_##type), OV_VT_##type)
#endif

/**
*	Convert a time into a double variable
*/
#define Ov_TimeToDouble(time, dbl)											\
	dbl = (OV_DOUBLE)(time).secs + (OV_DOUBLE)(time).usecs/(OV_DOUBLE)1e6

/**
*	Convert a double into a time variable
*/
#define Ov_DoubleToTime(dbl, time)											\
	(time).secs = (OV_UINT) (dbl);													\
	(time).usecs = (OV_UINT) (((dbl)-(OV_DOUBLE)(time).secs)*(OV_DOUBLE)1e6)

/**
*	Convert a time span into a double variable
*/
#define Ov_TimeSpanToDouble(timespan, dbl)											\
	dbl = (OV_DOUBLE)(timespan).secs + (OV_DOUBLE)(timespan).usecs/(OV_DOUBLE)1e6

/**
*	Convert a double into a time span variable
*/
#define Ov_DoubleToTimeSpan(dbl, timespan)											\
	(timespan).secs = (OV_INT)(dbl);													\
	(timespan).usecs = (OV_INT)(((dbl)-(OV_DOUBLE)(timespan).secs)*(OV_DOUBLE)1e6)

/**
*	Allocate memory in the database
*/
#define Ov_DbAlloc(type)	((type*)ov_database_malloc(sizeof(type)))

/**
*	Allocate memory in the database
*/
#define Ov_DbMalloc(size)	ov_database_malloc(size)

/**
*	Reallocate memory in the database
*/
#define Ov_DbRealloc(ptr, size)	ov_database_realloc(ptr, size)

/**
*	Free memory allocated in the database
*/
#define Ov_DbFree(ptr)		ov_database_free(ptr)

/**
*	Allocate memory on the stack memory
*/
#define Ov_MemStackAlloc(type)	((type*)ov_memstack_alloc(sizeof(type)))

/**
*	Allocate memory on the heap
*/
#ifdef OV_COMPILE_LIBOV
#if OV_SYSTEM_MC164
#define Ov_HeapAlloc(type)	((type*)xmalloc(sizeof(type)))
#else
#define Ov_HeapAlloc(type)	((type*)malloc(sizeof(type)))
#endif
#else
#define Ov_HeapAlloc(type)	((type*)ov_malloc(sizeof(type)))
#endif

/**
*	Allocate memory on the heap
*/
#ifdef OV_COMPILE_LIBOV
#if OV_SYSTEM_MC164
#define Ov_HeapMalloc(size)	xmalloc(size)
#else
#define Ov_HeapMalloc(size)	malloc(size)
#endif
#else
#define Ov_HeapMalloc(size)	ov_malloc(size)
#endif

/**
*	Reallocate memory on the heap
*/
#ifdef OV_COMPILE_LIBOV
#if OV_SYSTEM_MC164
#define Ov_HeapRealloc(ptr, size)	xrealloc(ptr, size)
#else
#define Ov_HeapRealloc(ptr, size)	realloc(ptr, size)
#endif
#else
#define Ov_HeapRealloc(ptr, size)	ov_realloc(ptr, size)
#endif

/**
*	Free memory allocated on the heap
*/
#ifdef OV_COMPILE_LIBOV
#if OV_SYSTEM_MC164
#define Ov_HeapFree(ptr)	xfree(ptr)
#else
#define Ov_HeapFree(ptr)	free(ptr)
#endif
#else
#define Ov_HeapFree(ptr)	ov_free(ptr)
#endif

/**
*	Duplicate a string on the heap
*	Uses strdup on most platforms even if it is not part of ANSI C
*/
#ifdef OV_COMPILE_LIBOV
#if OV_SYSTEM_MC164 || OV_SYSTEM_RMOS
#define Ov_HeapStrdup(ptr)	ov_strdup(ptr)
#else
#define Ov_HeapStrdup(ptr)	strdup(ptr)
#endif
#else
#define Ov_HeapStrdup(ptr)	ov_strdup(ptr)
#endif

#endif
/*
*	End of file
*/

