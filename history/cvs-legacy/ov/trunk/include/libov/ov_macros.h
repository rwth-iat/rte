/*
*   $Id: ov_macros.h,v 1.3 1999-07-28 16:01:37 dirk Exp $
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
*/

#ifndef OV_MACROS_H_INCLUDED
#define OV_MACROS_H_INCLUDED

#include "libov/ov_association.h"
#include "libov/ov_class.h"
#include "libov/ov_vector.h"
#include "libov/ov_database.h"
#include "libov/ov_memstack.h"
#include "libov/ov_malloc.h"

/*
*	Upcast of an instance pointer of the parent class
*/
#define Ov_ParentPtrUpCast(assoc, pparent)									\
	((OV_PPT_##assoc)((pparent)+(0*((pparent)->__classinfo.					\
	OV_PCI_##assoc))))

/*
*	Upcast of an instance pointer of the child class
*/
#define Ov_ChildPtrUpCast(assoc, pchild)									\
	((OV_CPT_##assoc)((pchild)+(0*((pchild)->__classinfo.					\
	OV_CCI_##assoc))))

/*
*	Get first child in an association
*/
#if OV_SYSTEM_MC164	/* compiler bug */
#define Ov_GetFirstChild(assoc, pparent)									\
	((OV_CPT_##assoc)((pparent)?((pparent)->OV_HN_##assoc.pfirst):(NULL)))
#else
#define Ov_GetFirstChild(assoc, pparent)									\
	((pparent)?((pparent)->OV_HN_##assoc.pfirst):((OV_CPT_##assoc)NULL))
#endif

/*
*	Get last child in an association
*/
#if OV_SYSTEM_MC164	/* compiler bug */
#define Ov_GetLastChild(assoc, pparent)										\
	((OV_CPT_##assoc)((pparent)?((pparent)->OV_HN_##assoc.plast):(NULL)))
#else
#define Ov_GetLastChild(assoc, pparent)										\
	((pparent)?((pparent)->OV_HN_##assoc.plast):((OV_CPT_##assoc)NULL))
#endif

/*
*	Get next child in an association
*/
#if OV_SYSTEM_MC164	/* compiler bug */
#define Ov_GetNextChild(assoc, pchild)										\
	((OV_CPT_##assoc)((pchild)?((pchild)->OV_AN_##assoc.pnext):(NULL)))
#else
#define Ov_GetNextChild(assoc, pchild)										\
	((pchild)?((pchild)->OV_AN_##assoc.pnext):((OV_CPT_##assoc)NULL))
#endif

/*
*	Get previous child in association
*/
#if OV_SYSTEM_MC164	/* compiler bug */
#define Ov_GetPrevChild(assoc, pchild)										\
	((OV_CPT_##assoc)((pchild)?((pchild)->OV_AN_##assoc.pprevious):(NULL)))
#else
#define Ov_GetPrevChild(assoc, pchild)										\
	((pchild)?((pchild)->OV_AN_##assoc.pprevious):((OV_CPT_##assoc)NULL))
#endif

/*
*	Get parent in an association
*/
#if OV_SYSTEM_MC164	/* compiler bug */
#define Ov_GetParent(assoc, pchild)											\
	((OV_PPT_##assoc)((pchild)?((pchild)->OV_AN_##assoc.pparent):(NULL)))
#else
#define Ov_GetParent(assoc, pchild)											\
	((pchild)?((pchild)->OV_AN_##assoc.pparent):((OV_PPT_##assoc)NULL))
#endif

/*
*	Search a child with given identifier in an association
*/
#define Ov_SearchChild(assoc, pparent, ident)								\
	((OV_CPT_##assoc)ov_association_searchchild(passoc_##assoc, 			\
	(OV_INSTPTR_ov_object)Ov_ParentPtrUpCast(assoc, pparent), ident))

/*
*	Search a child with given identifier and cast to child class
*/
#define Ov_SearchChildEx(assoc, pparent, ident, childclass)					\
	Ov_DynamicPtrCast(childclass, ((OV_CPT_##assoc)							\
	ov_association_searchchild(passoc_##assoc, (OV_INSTPTR_ov_object)		\
	Ov_ParentPtrUpCast(assoc, pparent), ident)))

/*
*	Link parent and child object, no placement hint
*/
#define Ov_Link(assoc, pparent, pchild)										\
	assoc##_link(Ov_ParentPtrUpCast(assoc, pparent), Ov_ChildPtrUpCast		\
	(assoc, pchild), OV_PMH_DEFAULT, NULL)

/*
*	Link parent and child object with given placement hint
*/
#define Ov_LinkPlaced(assoc, pparent, pchild, placement)					\
	assoc##_link(Ov_ParentPtrUpCast(assoc, pparent), Ov_ChildPtrUpCast		\
	(assoc, pchild), placement, NULL)

/*
*	Link parent and child object with given placement hint and relative child
*/
#define Ov_LinkRelativePlaced(assoc, pparent, pchild, placement, prelchild)	\
	assoc##_link(Ov_ParentPtrUpCast(assoc, pparent), Ov_ChildPtrUpCast		\
	(assoc, pchild), placement, Ov_ChildPtrUpCast(assoc, prelchild))

/*
*	Unlink parent and child object
*/
#define Ov_Unlink(assoc, pparent, pchild)									\
	assoc##_unlink(Ov_ParentPtrUpCast(assoc, pparent), Ov_ChildPtrUpCast	\
	(assoc, pchild))

/*
*	Iterate over all child objects in an association
*/
#define Ov_ForEachChild(assoc, pparent, pchild)								\
	for(((pchild)=Ov_GetFirstChild(assoc, pparent)); (pchild);	(pchild)	\
	=Ov_GetNextChild(assoc, pchild))

/*
*	Iterate over all child objects in an association and dynamically
*	cast to a given child class
*/
#define Ov_ForEachChildEx(assoc, pparent, pchild, childclass)				\
	for(((pchild)=(OV_INSTPTR_##childclass)Ov_GetFirstChild(assoc, 			\
	pparent)); (pchild); (pchild)=(OV_INSTPTR_##childclass)					\
	Ov_GetNextChild(assoc, pchild)) if(!Ov_DynamicPtrCast(childclass, 		\
	pchild)) {} else

/*
*	Upcast to a pointer of a given base class
*/
#define Ov_PtrUpCast(class, pobj) 											\
	((OV_INSTPTR_##class)((pobj)+(0*((pobj)->__classinfo.					\
	is_of_class_##class))))

/*
*	Static cast to a pointer of a given class
*/
#define Ov_StaticPtrCast(class, pobj) 										\
	((OV_INSTPTR_##class)(pobj))

/*
*	Test if it is allowed to cast to a given class
*/
#define Ov_CanCastTo(class, pobj)											\
	((Ov_GetParent(ov_instantiation, (pobj))==pclass_##class)?(TRUE):		\
	ov_class_cancastto(Ov_GetParent(ov_instantiation, (pobj)), 				\
	pclass_##class))

/*
*	Dynamic cast to a pointer of a given class
*/
#define Ov_DynamicPtrCast(class, pobj) 										\
	((OV_INSTPTR_##class)(Ov_CanCastTo(class, pobj)?(pobj):(NULL)))

/*
*	Create an object of a given class
*/
#define Ov_CreateObject(class, pobj, pparent, ident)						\
	ov_class_createobject(pclass_##class, Ov_PtrUpCast(ov_domain, pparent),	\
	ident, OV_PMH_DEFAULT, NULL, NULL, NULL, ((OV_INSTPTR_ov_object*)		\
	&(pobj))+(0*((pobj)->__classinfo.is_of_class_##class)))

/*
*	Delete an object
*/
#define Ov_DeleteObject(pobj)												\
	ov_class_deleteobject(Ov_PtrUpCast(ov_object, pobj))

/*
*	Get a pointer to a part object
*/
#define Ov_GetPartPtr(part, pobj)											\
	(&((pobj)->p_##part))

/*
*	Get pointer to the class object of an instance
*/
#define Ov_GetClassPtr(pobj)												\
	Ov_GetParent(ov_instantiation, Ov_PtrUpCast(ov_object, pobj))

/*
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

/*
*	Get the vtable pointer of the direct base class of an object
*	of a given class
*/
#define Ov_GetBaseclassVTablePtr(class, pobj)								\
	((OV_BVPT_##class)(Ov_GetParent(ov_inheritance, Ov_GetClassPtr(			\
	Ov_PtrUpCast(ov_class, pobj)))->v_pvtable))

/*
*	Test, if a variable definition object defines a variable with a
*	given name
*/
#define Ov_IsVariable(class, name, pvar)									\
	((pvar)->v_offset == offsetof(OV_INST_##class, v_##name))

/*
*	Set the value of a static vector variable
*/
#define Ov_SetStaticVectorValue(pvector, pvalue, veclen, type)				\
	ov_vector_setstaticvalue((OV_STATIC_VECTOR*)(pvector),					\
		(const OV_POINTER)pvalue, veclen+0*((pvector)-(OV_##type*)0),		\
		veclen*sizeof(OV_##type), OV_VT_##type)

/*
*	Get the vector length of a dynamic vector variable
*/
#define Ov_GetDynamicVectorLength(pvector)									\
	((pvector)?((pvector)->veclen):(0))

/*
*	Get the value of a dynamic vector variable
*/
#define Ov_GetDynamicVectorValue(pvector)									\
	((pvector)?((pvector)->value):(NULL))

/*
*	Set the value of a dynamic vector variable
*/
#define Ov_SetDynamicVectorValue(ppvector, pvalue, veclen, type)			\
	ov_vector_setdynamicvalue((OV_DYNAMIC_VECTOR**)(ppvector),				\
		(const OV_POINTER)pvalue, veclen+0*(*(ppvector)-(OV_##type##_VEC*)	\
		0),	offsetof(OV_##type##_VEC, value), veclen*sizeof(OV_##type), 	\
		OV_VT_##type)

/*
*	Allocate memory in the database
*/
#define Ov_DbMalloc(type)	((type*)ov_database_malloc(sizeof(type)))

/*
*	Free memory allocated in the database
*/
#define Ov_DbFree(ptr)		ov_database_free(ptr)

/*
*	Allocate memory on the stack memory
*/
#define Ov_MemStackAlloc(type)	((type*)ov_memstack_alloc(sizeof(type)))

/*
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

/*
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

/*
*	Rellocate memory on the heap
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

/*
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

/*
*	Duplicate a string on the heap
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

