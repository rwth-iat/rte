/*
*   $Id: ov_object.c,v 1.38 2010-12-20 13:23:06 martin Exp $
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
*	13-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	04-Nov-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: variable type ANY added.
*	06-Dez-2001 Ansgar Münnemann <ansgar@plt.rwth-aachen.de>: macro Ov_VarAddress for object_move changed.
*/
/*
*	Description:
*	------------
*	These are generic methods, which each object supports. Some of the 
*	functions are designed for the use in the ACPLT/KS server of ACPLT/OV
*	(libovks, server part), but may be used with other modules as well.
*	Some of the methods return more than just a variable of a fixed size
*	(e.g. an OV_STRING or OV_VAR_VALUE). In this case, the memory pointed
*	to may be allocated in the database or on the memory stack 
*	(ov_memstack_alloc()), but NEVER on the heap (using ov_malloc()). 
*	This means, that you must use ov_memstack_lock() and ov_memstack_free()
*	in the functions calling these methods. This ensures, that temporary
*	memory is always freed. If you need the result of such a method for
*	a longer time, you will have to copy it to memory allocated on the heap.
*	See also: ov_memstack.h.
*/

#define OV_COMPILE_LIBOV


#include "libov/ov_object.h"
#include "libov/ov_time.h"
#include "libov/ov_string.h"
#include "libov/ov_scheduler.h"
#include "libov/ov_macros.h"
#include "libov/ov_library.h"
#include "libov/ov_variable.h"

#if OV_SYSTEM_MC164
#define memcpy xmemcpy
#endif

#ifdef OV_COMPILE_LIBOV
#define OV_EXTERN extern
#else
#define OV_EXTERN OV_DLLVARIMPORT
#endif
OV_EXTERN OV_VENDORTREE_INFO vendorinfo[OV_NUM_VENDOROBJECTS];
#undef OV_EXTERN


/*	----------------------------------------------------------------------	*/

OV_DLLFNCEXPORT OV_UINT ov_object_idH_get(
	OV_INSTPTR_ov_object 		pobj
) {
	return pobj->v_idH;
}

OV_DLLFNCEXPORT OV_RESULT ov_object_idH_set(
	OV_INSTPTR_ov_object 		pobj,
	OV_UINT						value
) {
	/*	write may never happen --> this accessor fakes successfull accessses for db-dumps etc.	*/
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT ov_object_idL_get(
	OV_INSTPTR_ov_object 		pobj
) {
	return pobj->v_idL;
}

OV_DLLFNCEXPORT OV_RESULT ov_object_idL_set(
	OV_INSTPTR_ov_object 		pobj,
	OV_UINT						value
) {
	/*	write may never happen --> this accessor fakes successfull accessses for db-dumps etc.	*/
	return OV_ERR_OK;
}

/**
*	Default constructor of an object
*/
OV_DLLFNCEXPORT OV_RESULT ov_object_constructor(
	OV_INSTPTR_ov_object 		pobj
) {
	/*
	*	local variables
	*/
	OV_VTBLPTR_ov_object	pvtable;
	OV_ELEMENT				parent, child;
	OV_RESULT				result;
	OV_VAR_VALUE				init;
	OV_STRING				*pstring;
	OV_UINT					i;
	/*
	*	initialize
	*/
	parent.elemtype = OV_ET_OBJECT;
	parent.pobj = pobj;
	child.elemtype = OV_ET_NONE;
	/*
	*	construct part objects
	*/
	while(TRUE) {
		Ov_AbortIfNot(Ov_OK(ov_element_getnextpart(&parent, &child, OV_ET_OBJECT | OV_ET_VARIABLE)));
		if(child.elemtype == OV_ET_NONE) {
			break;
		}
		if (child.elemtype == OV_ET_OBJECT) {
			Ov_GetVTablePtr(ov_object, pvtable, child.pobj);
			if(!pvtable) {
				pvtable = pclass_ov_object->v_pvtable;
			}
			result = pvtable->m_constructor(child.pobj);
			if(Ov_Fail(result)) {
				return result;
			}
		}
		if (child.elemtype == OV_ET_VARIABLE) {
			init = child.elemunion.pvar->v_initialvalue.value;
			if ((init.vartype & OV_VT_KSMASK) && (child.elemunion.pvar->v_varprops & OV_VP_DERIVED)) return OV_ERR_BADINITPARAM;
			if ((init.vartype & OV_VT_KSMASK) && (child.elemunion.pvar->v_varprops & OV_VP_STATIC)) return OV_ERR_BADINITPARAM;
			switch (init.vartype & OV_VT_KSMASK) {
				case OV_VT_VOID:
					break;
				case OV_VT_BYTE_VEC:
					if (child.elemunion.pvar->v_veclen == 0) Ov_SetDynamicVectorValue((OV_BYTE_VEC*)child.pvalue, init.valueunion.val_byte_vec.value, init.valueunion.val_byte_vec.veclen, BYTE);
					else for (i=0;i<init.valueunion.val_byte_vec.veclen;i++) ((OV_BYTE*)child.pvalue)[i] = ((OV_BYTE*)init.valueunion.val_byte_vec.value)[i];
					break;
				case OV_VT_BOOL_VEC:
					if (child.elemunion.pvar->v_veclen == 0) Ov_SetDynamicVectorValue((OV_BOOL_VEC*)child.pvalue, init.valueunion.val_bool_vec.value, init.valueunion.val_bool_vec.veclen, BOOL);
					else for (i=0;i<init.valueunion.val_bool_vec.veclen;i++) ((OV_BOOL*)child.pvalue)[i] = ((OV_BOOL*)init.valueunion.val_bool_vec.value)[i];
					break;
				case OV_VT_INT_VEC:
					if (child.elemunion.pvar->v_veclen == 0) Ov_SetDynamicVectorValue((OV_INT_VEC*)child.pvalue, init.valueunion.val_int_vec.value, init.valueunion.val_int_vec.veclen, INT);
					else for (i=0;i<init.valueunion.val_int_vec.veclen;i++) ((OV_INT*)child.pvalue)[i] = ((OV_INT*)init.valueunion.val_int_vec.value)[i];
					break;
				case OV_VT_UINT_VEC:
					if (child.elemunion.pvar->v_veclen == 0) Ov_SetDynamicVectorValue((OV_UINT_VEC*)child.pvalue, init.valueunion.val_uint_vec.value, init.valueunion.val_uint_vec.veclen, UINT);
					else for (i=0;i<init.valueunion.val_uint_vec.veclen;i++) ((OV_UINT*)child.pvalue)[i] = ((OV_UINT*)init.valueunion.val_uint_vec.value)[i];
					break;
				case OV_VT_SINGLE_VEC:
					if (child.elemunion.pvar->v_veclen == 0) Ov_SetDynamicVectorValue((OV_SINGLE_VEC*)child.pvalue, init.valueunion.val_single_vec.value, init.valueunion.val_single_vec.veclen, SINGLE);
					else for (i=0;i<init.valueunion.val_single_vec.veclen;i++) ((OV_SINGLE*)child.pvalue)[i] = ((OV_SINGLE*)init.valueunion.val_single_vec.value)[i];
					break;
				case OV_VT_DOUBLE_VEC:
					if (child.elemunion.pvar->v_veclen == 0) Ov_SetDynamicVectorValue((OV_DOUBLE_VEC*)child.pvalue, init.valueunion.val_double_vec.value, init.valueunion.val_double_vec.veclen, DOUBLE);
					else for (i=0;i<init.valueunion.val_double_vec.veclen;i++) ((OV_DOUBLE*)child.pvalue)[i] = ((OV_DOUBLE*)init.valueunion.val_double_vec.value)[i];
					break;
				case OV_VT_TIME_VEC:
					if (child.elemunion.pvar->v_veclen == 0) Ov_SetDynamicVectorValue((OV_TIME_VEC*)child.pvalue, init.valueunion.val_time_vec.value, init.valueunion.val_time_vec.veclen, TIME);
					else for (i=0;i<init.valueunion.val_time_vec.veclen;i++) ((OV_TIME*)child.pvalue)[i] = ((OV_TIME*)init.valueunion.val_time_vec.value)[i];
					break;
				case OV_VT_TIME_SPAN_VEC:
					if (child.elemunion.pvar->v_veclen == 0) Ov_SetDynamicVectorValue((OV_TIME_SPAN_VEC*)child.pvalue, init.valueunion.val_time_span_vec.value, init.valueunion.val_time_span_vec.veclen, TIME_SPAN);
					else for (i=0;i<init.valueunion.val_time_span_vec.veclen;i++) ((OV_TIME_SPAN*)child.pvalue)[i] = ((OV_TIME_SPAN*)init.valueunion.val_time_span_vec.value)[i];
					break;
				case OV_VT_STRING_VEC:
					if (child.elemunion.pvar->v_veclen == 0) {
						Ov_SetDynamicVectorLength((OV_STRING_VEC*)child.pvalue, init.valueunion.val_string_vec.veclen, STRING);
						pstring = ((OV_STRING_VEC*)child.pvalue)->value;
					}
					else pstring = (OV_STRING*) child.pvalue;
					for (i=0;i<init.valueunion.val_string_vec.veclen;i++) {
						ov_string_setvalue(&pstring[i], init.valueunion.val_string_vec.value[i]);
					}
					break;
				case OV_VT_STRING:
					pstring = (OV_STRING*) child.pvalue;
					ov_string_setvalue(pstring, init.valueunion.val_string);
					break;
				case OV_VT_BYTE:
					*((OV_BYTE*)child.pvalue) = init.valueunion.val_byte;
					break;
				case OV_VT_BOOL:
					*((OV_BOOL*)child.pvalue) = init.valueunion.val_bool;
					break;
				case OV_VT_INT:
					*((OV_INT*)child.pvalue) = init.valueunion.val_int;
					break;
				case OV_VT_UINT:
					*((OV_UINT*)child.pvalue) = init.valueunion.val_uint;
					break;
				case OV_VT_SINGLE:
					*((OV_SINGLE*)child.pvalue) = init.valueunion.val_single;
					break;
				case OV_VT_DOUBLE:
					*((OV_DOUBLE*)child.pvalue) = init.valueunion.val_double;
					break;
				case OV_VT_TIME:
					*((OV_TIME*)child.pvalue) = init.valueunion.val_time;
					break;
				case OV_VT_TIME_SPAN:
					*((OV_TIME_SPAN*)child.pvalue) = init.valueunion.val_time_span;
					break;
				default:
					break;
			}
		}
	}
	/*
	*	nothing else to do
	*/
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/**
*	Default initialization check of an object
*/
OV_DLLFNCEXPORT OV_RESULT ov_object_checkinit(
	OV_INSTPTR_ov_object 		pobj
) {
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/**
*	Default destructor of an object
*/
void OV_DLLFNCEXPORT ov_object_destructor(
	OV_INSTPTR_ov_object 		pobj
) {
	/*
	*	local variables
	*/
	OV_VTBLPTR_ov_object	pvtable;
	OV_ELEMENT				parent, child;
	/*
	*	initialize
	*/
	parent.elemtype = OV_ET_OBJECT;
	parent.pobj = pobj;
	child.elemtype = OV_ET_NONE;
	/*
	*	destruct part objects
	*/
	while(TRUE) {
		Ov_AbortIfNot(Ov_OK(ov_element_getnextpart(&parent, &child, OV_ET_OBJECT)));
		if(child.elemtype == OV_ET_NONE) {
			break;
		}
		Ov_GetVTablePtr(ov_object, pvtable, child.pobj);
		if(!pvtable) {
			pvtable = pclass_ov_object->v_pvtable;
		}
		pvtable->m_destructor(child.pobj);
	}
	/*
	*	nothing else to do
	*/
	return;
}

/*	----------------------------------------------------------------------	*/

/*
*	Default method for starting up an object
*/
void OV_DLLFNCEXPORT ov_object_startup(
	OV_INSTPTR_ov_object 		pobj
) {
	/*
	*	local variables
	*/
	OV_VTBLPTR_ov_object	pvtable;
	OV_ELEMENT				parent, child;
	/*
	*	initialize
	*/
	parent.elemtype = OV_ET_OBJECT;
	parent.pobj = pobj;
	child.elemtype = OV_ET_NONE;
	/*
	*	start up part objects
	*/
	while(TRUE) {
		Ov_AbortIfNot(Ov_OK(ov_element_getnextpart(&parent, &child, OV_ET_OBJECT)));
		if(child.elemtype == OV_ET_NONE) {
			break;
		}
		Ov_GetVTablePtr(ov_object, pvtable, child.pobj);
		if(!pvtable) {
			pvtable = pclass_ov_object->v_pvtable;
		}
		pvtable->m_startup(child.pobj);
	}
	/*
	*	start up child objects
	*/
	while(TRUE) {
		Ov_AbortIfNot(Ov_OK(ov_element_getnextchild(&parent, &child)));
		if(child.elemtype == OV_ET_NONE) {
			break;
		}
		Ov_GetVTablePtr(ov_object, pvtable, child.pobj);
		if(!pvtable) {
			pvtable = pclass_ov_object->v_pvtable;
		}
		pvtable->m_startup(child.pobj);
	}
	/*
	*	set object state to "started up"
	*/
	pobj->v_objectstate |= OV_OS_STARTED;
	return;
}

/*	----------------------------------------------------------------------	*/

/*
*	Default method for shutting down an object
*/
void OV_DLLFNCEXPORT ov_object_shutdown(
	OV_INSTPTR_ov_object 		pobj
) {
	/*
	*	local variables
	*/
	OV_VTBLPTR_ov_object	pvtable;
	OV_ELEMENT				parent, child;
	/*
	*	initialize
	*/
	parent.elemtype = OV_ET_OBJECT;
	parent.pobj = pobj;
	child.elemtype = OV_ET_NONE;
	/*
	*	shut down part objects
	*/
	while(TRUE) {
		Ov_AbortIfNot(Ov_OK(ov_element_getnextpart(&parent, &child, OV_ET_OBJECT)));
		if(child.elemtype == OV_ET_NONE) {
			break;
		}
		Ov_GetVTablePtr(ov_object, pvtable, child.pobj);
		if(!pvtable) {
			pvtable = pclass_ov_object->v_pvtable;
		}
		pvtable->m_shutdown(child.pobj);
	}
	/*
	*	shut down child objects
	*/
	while(TRUE) {
		Ov_AbortIfNot(Ov_OK(ov_element_getnextchild(&parent, &child)));
		if(child.elemtype == OV_ET_NONE) {
			break;
		}
		Ov_GetVTablePtr(ov_object, pvtable, child.pobj);
		if(!pvtable) {
			pvtable = pclass_ov_object->v_pvtable;
		}
		pvtable->m_shutdown(child.pobj);
	}
	/*
	*	set object state to "shut down"
	*/
	pobj->v_objectstate &= ~OV_OS_STARTED;
	return;
}

/**
*	Default before rename or moving of an object
*/
OV_DLLFNCEXPORT OV_RESULT ov_object_rename(
		OV_INSTPTR_ov_object	pobj,
		OV_STRING				newid,
		OV_INSTPTR_ov_domain	pnewparent
) {
	return OV_ERR_OK;
}


/*	----------------------------------------------------------------------	*/

/**
*	Default method for getting the access rights
*	of an object or one of its elements
*/

OV_ACCESS OV_DLLFNCEXPORT ov_object_getaccess(
	OV_INSTPTR_ov_object		pobj,
	const OV_ELEMENT			*pelem,
	const OV_TICKET				*pticket
) {
	/*
	*	local variables
	*/
	OV_ACCESS			access;
	OV_INSTPTR_ov_class	pclass;
	/*
	*	switch based on the element's type
	*/
	switch(pelem->elemtype) {
	case OV_ET_OBJECT:
		/*
		*	vendor objects are readable only
		*/
		if((pobj < &pdb->vendorobj[OV_NUM_VENDOROBJECTS])
			&& (pobj >= Ov_PtrUpCast(ov_object, &pdb->vendordom))
		) {
			if (pobj >= &pdb->vendorobj[0]) {
				if (vendorinfo[pobj-pdb->vendorobj].setvarfnc)
					return OV_AC_READ | OV_AC_WRITE;
			}
			return OV_AC_READ;
		}
		/*
		*	all other objects are readable and may be deleteable,
		*	renameable or writeable
		*/
		access = OV_AC_READ;
		pclass = Ov_GetParent(ov_instantiation, pobj);
		if(Ov_GetParent(ov_containment, pclass) != Ov_PtrUpCast(ov_domain, &pdb->ov)) {
			if(!pobj->v_pouterobject) {
				access |= OV_AC_RENAMEABLE;
				if(!ov_object_haslinks(pobj)) {
					access |= OV_AC_DELETEABLE;
				}
			}
			if(ov_class_cancastto(pclass, pclass_ov_domain)) {
				access |= OV_AC_WRITE;
			}
		} else {
			if(pclass == pclass_ov_domain) {
				access |= OV_AC_WRITE;
				if(!pobj->v_pouterobject) {
					access |= OV_AC_RENAMEABLE;
					if(!Ov_GetFirstChild(ov_containment,
						Ov_StaticPtrCast(ov_domain, pobj))
					) {
						access |= OV_AC_DELETEABLE;
					}
				}
			}
			else if(pclass == pclass_ov_library) {
				if(ov_library_canunload(Ov_StaticPtrCast(ov_library, pobj))) {
					access |= OV_AC_DELETEABLE;
				}
			}
			else if(pclass == pclass_ov_class) {
				if(Ov_StaticPtrCast(ov_class, pobj)->v_classprops & OV_CP_INSTANTIABLE) {
					if((Ov_GetParent(ov_containment, pobj) != Ov_PtrUpCast(ov_domain, &pdb->ov))
						|| (pobj == Ov_PtrUpCast(ov_object, pclass_ov_domain))
						|| (pobj == Ov_PtrUpCast(ov_object, pclass_ov_library))
					) {
						access |= OV_AC_INSTANTIABLE;
					}
				}
			}
		}
		return access;
	case OV_ET_MEMBER:
		access = OV_AC_NONE;
		if (!pelem->pvar) return access;
		if (((ov_activitylock) || (pelem->pvar->v_varprops & OV_VP_GETACCESSOR)) && (!(pelem->pvar->v_varprops & OV_VP_DERIVED))) {
			access |= OV_AC_READ;
		}
		if((pelem->pvar->v_varprops & OV_VP_SETACCESSOR)
			&& ((pobj->v_objectstate & OV_OS_INIT) 
				|| !(pelem->pvar->v_varprops & OV_VP_STATIC))
		) {
			access |= OV_AC_WRITE;
		}
		return access;
	case OV_ET_VARIABLE:
		access = OV_AC_NONE;
		if (ov_activitylock) {
			if (!(pelem->elemunion.pvar->v_varprops & (OV_VP_DERIVED | OV_VP_STATIC)))
				access |= OV_AC_READ | OV_AC_WRITE;
			return access;
		}
		if(pelem->elemunion.pvar->v_varprops & OV_VP_GETACCESSOR) {
			access |= OV_AC_READ;
		}
		if((pelem->elemunion.pvar->v_varprops & OV_VP_SETACCESSOR)
			&& ((pobj->v_objectstate & OV_OS_INIT) 
				|| !(pelem->elemunion.pvar->v_varprops & OV_VP_STATIC))
		) {
			access |= OV_AC_WRITE;
		}
		return access;
	case OV_ET_CHILDLINK:
		return pelem->elemunion.passoc->v_getaccessfnc(NULL, pobj, pticket);
	case OV_ET_PARENTLINK:
		return pelem->elemunion.passoc->v_getaccessfnc(pobj, NULL, pticket);
	case OV_ET_OPERATION:
		return OV_AC_NONE;
	default:
		break;
	}
	Ov_Warning("internal error");
	return OV_AC_NONE;
}

/*	----------------------------------------------------------------------	*/

/**
*	Default method for reading the semantical flags of an object or one
*	of its elements (part of the engineered properties)
*/
OV_UINT OV_DLLFNCEXPORT ov_object_getflags(
	OV_INSTPTR_ov_object 	pobj,
	const OV_ELEMENT		*pelem
) {
	switch(pelem->elemtype) {
	case OV_ET_OBJECT:
		if ((pelem->pobj)->v_pouterobject) {
			return (pelem->elemunion.ppart)->v_flags;
		}
		return Ov_GetParent(ov_instantiation, pobj)->v_flags;
	case OV_ET_VARIABLE:
	case OV_ET_MEMBER:
		return pelem->elemunion.pvar->v_flags;
	case OV_ET_PARENTLINK:
		return pelem->elemunion.passoc->v_parentflags;
	case OV_ET_CHILDLINK:
		return pelem->elemunion.passoc->v_childflags;
	default:
		break;
	}
	return 0;
}

/*	----------------------------------------------------------------------	*/

/**
*	Default method for getting the comment of an object or
*	one of its elements (variables, parts, operations, links)
*	NOTE: call ov_memstack_lock/unlock() outside of this function!
*/
OV_STRING OV_DLLFNCEXPORT ov_object_getcomment(
	OV_INSTPTR_ov_object 		pobj,
	const OV_ELEMENT			*pelem
) {
	/*
	*	switch based on the element's type
	*/
	switch(pelem->elemtype) {
	case OV_ET_OBJECT:
		return Ov_GetParent(ov_instantiation, pobj)->v_comment;
	case OV_ET_VARIABLE:
	case OV_ET_MEMBER:
		return pelem->elemunion.pvar->v_comment;
	case OV_ET_PARENTLINK:
		return pelem->elemunion.passoc->v_parentcomment;
	case OV_ET_CHILDLINK:
		return pelem->elemunion.passoc->v_childcomment;
	default:
		break;
	}
	return NULL;
}

/*	----------------------------------------------------------------------	*/

/**
*	Default method for getting the technical unit of a variable of the object
*	NOTE: call ov_memstack_lock/unlock() outside of this function!
*/
OV_STRING OV_DLLFNCEXPORT ov_object_gettechunit(
	OV_INSTPTR_ov_object 		pobj,
	const OV_ELEMENT			*pelem
) {
	switch(pelem->elemtype) {
	case OV_ET_VARIABLE:
	case OV_ET_MEMBER:
		if(pelem->elemunion.pvar->v_techunit) {
			return pelem->elemunion.pvar->v_techunit;
		}
		if(((pelem->elemunion.pvar->v_vartype & OV_VT_KSMASK) == OV_VT_TIME)
			|| ((pelem->elemunion.pvar->v_vartype & OV_VT_KSMASK) == OV_VT_TIME_VEC)
		) {
			return "UTC";
		}
	default:
		break;
	}
	return NULL;
}

/*	----------------------------------------------------------------------	*/

/*
*	Helper macros for implementing ov_object_getvar()
*/
#define Ov_Object_GetVarValue(vartype, VARTYPE)									\
	case OV_VT_##VARTYPE:														\
		if((pelem->elemunion.pvar->v_getfnc) && (!ov_activitylock)){			\
			pvarcurrprops->value.valueunion.val_##vartype						\
				= ((OV_FNCPTR_GET(VARTYPE))pelem->elemunion.pvar->v_getfnc)(pobj);\
		} else {																\
			if (pelem->pvalue) pvarcurrprops->value.valueunion.val_##vartype	\
				= *((OV_##VARTYPE*)pelem->pvalue);								\
		}																		\
		break

#define Ov_Object_GetTimeVarValue(vartype, VARTYPE)								\
	case OV_VT_##VARTYPE: {														\
			OV_##VARTYPE *pvalue; 												\
			if((pelem->elemunion.pvar->v_getfnc) && (!ov_activitylock)) {		\
				pvalue = ((OV_FNCPTR_GETTIME(VARTYPE))pelem->elemunion.pvar->v_getfnc)(pobj);\
			} else {															\
				pvalue = (OV_##VARTYPE*)pelem->pvalue;							\
			}																	\
			if (pvalue) {														\
				pvarcurrprops->value.valueunion.val_##vartype = *pvalue;		\
			}																	\
		}																		\
		break

#define Ov_Object_GetVarPVValue(vartype, VARTYPE)								\
	case OV_VT_##VARTYPE##_PV: {												\
			OV_##VARTYPE##_PV *pvalue; 											\
			if((pelem->elemunion.pvar->v_getfnc) && (!ov_activitylock)) {		\
				pvalue = ((OV_FNCPTR_GETPV(VARTYPE))pelem->elemunion.pvar->v_getfnc)(pobj);\
			} else {															\
				pvalue = (OV_##VARTYPE##_PV*)pelem->pvalue;						\
			}																	\
			if (pvalue) {														\
                pvarcurrprops->value.valueunion.val_##vartype = pvalue->value;	\
			    pvarcurrprops->state = pvalue->state;							\
			    pvarcurrprops->time = pvalue->time;								\
			}																	\
		}																		\
		return OV_ERR_OK

#define Ov_Object_GetVarDynVecValue(vartype, VARTYPE)							\
	case OV_VT_##VARTYPE##_VEC:													\
		if((pelem->elemunion.pvar->v_getfnc) && (!ov_activitylock)) {			\
			pvarcurrprops->value.valueunion.val_##vartype##_vec.value			\
				= ((OV_FNCPTR_GETVEC(VARTYPE))pelem->elemunion.pvar->v_getfnc)(pobj,\
				&pvarcurrprops->value.valueunion.val_##vartype##_vec.veclen);	\
		} else {																\
			if (pelem->pvalue) {												\
				pvarcurrprops->value.valueunion.val_##vartype##_vec.veclen		\
					= ((OV_##VARTYPE##_VEC*)pelem->pvalue)->veclen;				\
				pvarcurrprops->value.valueunion.val_##vartype##_vec.value		\
					= ((OV_##VARTYPE##_VEC*)pelem->pvalue)->value;				\
			}																	\
		}																		\
		break

#define Ov_Object_GetVarVecValue(vartype, VARTYPE)							\
	case OV_VT_##VARTYPE##_VEC:												\
		if((pelem->elemunion.pvar->v_getfnc) && (!ov_activitylock)) {		\
			pvarcurrprops->value.valueunion.val_##vartype##_vec.value		\
				= ((OV_FNCPTR_GETVEC(VARTYPE))pelem->elemunion.pvar->v_getfnc)(pobj,\
				&pvarcurrprops->value.valueunion.val_##vartype##_vec.veclen);	\
		} else {															\
			if (pelem->pvalue) pvarcurrprops->value.valueunion.val_##vartype##_vec.veclen		\
				= pelem->elemunion.pvar->v_veclen;							\
			if (pelem->pvalue) pvarcurrprops->value.valueunion.val_##vartype##_vec.value		\
				= ((OV_##VARTYPE*)pelem->pvalue);							\
		}																	\
		break

/* Vectors of PV variables not supported */
#define Ov_Object_GetVarDynVecPVValue(vartype, VARTYPE)							\
	case OV_VT_##VARTYPE##_PV_VEC: {											\
			OV_##VARTYPE##_PV_VEC *pvalue; 										\
			if((pelem->elemunion.pvar->v_getfnc) && (!ov_activitylock)){		\
				pvalue = ((OV_FNCPTR_GETPVVEC(VARTYPE))pelem->elemunion.pvar->v_getfnc)(pobj);	\
			} else {															\
				pvalue = (OV_##VARTYPE##_PV_VEC*)pelem->pvalue;					\
			}																	\
			if (pvalue) {														\
                pvarcurrprops->value.valueunion.val_##vartype##_vec = pvalue->value;\
			    pvarcurrprops->state = pvalue->state;							\
			    pvarcurrprops->time = pvalue->time;								\
			}																	\
		}																		\
		return OV_ERR_OK
		
#define Ov_Object_GetVarVecPVValue(vartype, VARTYPE)							\
	case OV_VT_##VARTYPE##_PV_VEC: {											\
			OV_##VARTYPE##_PV_VEC *pvalue; 										\
			if((pelem->elemunion.pvar->v_getfnc) && (!ov_activitylock)){		\
				pvalue = ((OV_FNCPTR_GETPVVEC(VARTYPE))pelem->elemunion.pvar->v_getfnc)(pobj);	\
			} else {															\
				pvalue = (OV_##VARTYPE##_PV_VEC*)pelem->pvalue;					\
			}																	\
			if (pvalue) {														\
                pvarcurrprops->value.valueunion.val_##vartype##_vec = pvalue->value;\
			    pvarcurrprops->state = pvalue->state;							\
			    pvarcurrprops->time = pvalue->time;								\
			}																	\
		}																		\
		return OV_ERR_OK

/**
*	Default method for reading the current properties of
*	a variable (value, state and timestamp, compare ACPLT/KS)
*/
OV_DLLFNCEXPORT OV_RESULT ov_object_getvar(
	OV_INSTPTR_ov_object 	pobj, 
	const OV_ELEMENT		*pelem,
	OV_ANY					*pvarcurrprops
) {
//	OV_INSTPTR_ov_variable	 	pvar;
	OV_ELEMENT			elem;
	OV_BYTE*			retvalue;
	OV_UINT				dist;

	switch(pelem->elemtype) {
	case OV_ET_MEMBER:
		if (!pelem->pvar) return OV_ERR_GENERIC;
		if((pelem->pvar->v_getfnc) && (!ov_activitylock) && (pelem->pvar->v_veclen==1)) {
			retvalue = (OV_BYTE*)((pelem->pvar->v_getfnc)(pobj));
			elem.pobj = pobj;
			elem.elemtype = OV_ET_VARIABLE;
			elem.elemunion.pvar = pelem->elemunion.pvar;
			dist = pelem->pvalue - (OV_BYTE*) pobj;
			elem.pvalue = retvalue + dist  - pelem->pvar->v_offset;
			return ov_object_getvar(pobj, &elem, pvarcurrprops);
		}
		break;
	case OV_ET_VARIABLE:
		/*
		*	enter default variable type, state and time stamp
		*/
		pvarcurrprops->value.vartype = pelem->elemunion.pvar->v_vartype;
		/*
		*	now get the actual value
		*/
		switch(pelem->elemunion.pvar->v_veclen) {
		case 1:
			/*
			*	scalar variable
			*/
			switch(pelem->elemunion.pvar->v_vartype) {
			Ov_Object_GetVarValue(byte, BYTE);
			Ov_Object_GetVarValue(bool, BOOL);
			Ov_Object_GetVarValue(int, INT);
			Ov_Object_GetVarValue(uint, UINT);
			Ov_Object_GetVarValue(single, SINGLE);
			Ov_Object_GetVarValue(double, DOUBLE);
			Ov_Object_GetVarValue(string, STRING);
			
            Ov_Object_GetTimeVarValue(time, TIME);
            Ov_Object_GetTimeVarValue(time_span, TIME_SPAN);
			
			Ov_Object_GetVarPVValue(bool, BOOL);
			Ov_Object_GetVarPVValue(int, INT);
			Ov_Object_GetVarPVValue(uint, UINT);
			Ov_Object_GetVarPVValue(single, SINGLE);
			Ov_Object_GetVarPVValue(double, DOUBLE);
			Ov_Object_GetVarPVValue(string, STRING);
			Ov_Object_GetVarPVValue(time, TIME);
            Ov_Object_GetVarPVValue(time_span, TIME_SPAN);
			case OV_VT_CTYPE:
				if((pelem->elemunion.pvar->v_getfnc) && (!ov_activitylock)){
					pvarcurrprops->value.valueunion.val_byte_vec.value = ( (OV_FNCPTR_GETVEC(BYTE)) (pelem->elemunion.pvar->v_getfnc) )
						(pobj, &pvarcurrprops->value.valueunion.val_byte_vec.veclen);
				} else {
					if (pelem->pvalue) {
						pvarcurrprops->value.valueunion.val_byte_vec.veclen = ((OV_BYTE_VEC*)pelem->pvalue)->veclen;
						pvarcurrprops->value.valueunion.val_byte_vec.value = ((OV_BYTE*)pelem->pvalue);
					}
				}
				break;
			case OV_VT_STRUCT:
				if((pelem->elemunion.pvar->v_getfnc) && (!ov_activitylock)) {
					pvarcurrprops->value.valueunion.val_struct.value = (OV_BYTE*)((pelem->elemunion.pvar->v_getfnc)(pobj));
				} else {
					pvarcurrprops->value.valueunion.val_struct.value = (OV_BYTE*)(pelem->pvalue);
				}
				break;
			case OV_VT_ANY:
				if((pelem->elemunion.pvar->v_getfnc) && (!ov_activitylock)) {
					*pvarcurrprops = *(((OV_FNCPTR_GETANY)pelem->elemunion.pvar->v_getfnc)(pobj));
				} else {
					*pvarcurrprops = *(OV_ANY*)(pelem->pvalue);
				}
				if(pvarcurrprops->value.vartype & OV_VT_HAS_STATE) {
					if(pvarcurrprops->state == OV_ST_NOTSUPPORTED) {
						pvarcurrprops->state = OV_ST_UNKNOWN;
					}
				} else {
					pvarcurrprops->state = OV_ST_NOTSUPPORTED;
				}
				if(!(pvarcurrprops->value.vartype & OV_VT_HAS_TIMESTAMP)) {
					ov_time_gettime(&pvarcurrprops->time);
				}
				return OV_ERR_OK;
			default:
				Ov_Warning("internal error");
				return OV_ERR_GENERIC;
			}
			break;

		case 0:
			/*
			*	dynamic vector variable
			*/
			switch(pelem->elemunion.pvar->v_vartype) {
			Ov_Object_GetVarDynVecValue(byte, BYTE);
			Ov_Object_GetVarDynVecValue(bool, BOOL);
			Ov_Object_GetVarDynVecValue(int, INT);
			Ov_Object_GetVarDynVecValue(uint, UINT);
			Ov_Object_GetVarDynVecValue(single, SINGLE);
			Ov_Object_GetVarDynVecValue(double, DOUBLE);
			Ov_Object_GetVarDynVecValue(string, STRING);
			Ov_Object_GetVarDynVecValue(time, TIME);
			Ov_Object_GetVarDynVecValue(time_span, TIME_SPAN);
			
			Ov_Object_GetVarDynVecPVValue(bool, BOOL);
			Ov_Object_GetVarDynVecPVValue(int, INT);
			Ov_Object_GetVarDynVecPVValue(uint, UINT);
			Ov_Object_GetVarDynVecPVValue(single, SINGLE);
			Ov_Object_GetVarDynVecPVValue(double, DOUBLE);
			Ov_Object_GetVarDynVecPVValue(string, STRING);
			Ov_Object_GetVarDynVecPVValue(time, TIME);
			Ov_Object_GetVarDynVecPVValue(time_span, TIME_SPAN);
			case OV_VT_STRUCT_VEC:
				/*
				*	vectors of structures are not available
				*/
				/* fall into... */
			default:
				Ov_Warning("internal error");
				return OV_ERR_GENERIC;
			}
			break;

		default:
			/*
			*	static vector variable
			*/
			switch(pelem->elemunion.pvar->v_vartype) {
			Ov_Object_GetVarVecValue(byte, BYTE);
			Ov_Object_GetVarVecValue(bool, BOOL);
			Ov_Object_GetVarVecValue(int, INT);
			Ov_Object_GetVarVecValue(uint, UINT);
			Ov_Object_GetVarVecValue(single, SINGLE);
			Ov_Object_GetVarVecValue(double, DOUBLE);
			Ov_Object_GetVarVecValue(string, STRING);
			Ov_Object_GetVarVecValue(time, TIME);
			Ov_Object_GetVarVecValue(time_span, TIME_SPAN);

			Ov_Object_GetVarVecPVValue(bool, BOOL);
			Ov_Object_GetVarVecPVValue(int, INT);
			Ov_Object_GetVarVecPVValue(uint, UINT);
			Ov_Object_GetVarVecPVValue(single, SINGLE);
			Ov_Object_GetVarVecPVValue(double, DOUBLE);
			Ov_Object_GetVarVecPVValue(string, STRING);
			Ov_Object_GetVarVecPVValue(time, TIME);
			Ov_Object_GetVarVecPVValue(time_span, TIME_SPAN);
			case OV_VT_STRUCT_VEC:
				/*
				*	vectors of structures are not available
				*/
				/* fall into... */
			default:
				Ov_Warning("internal error");
				return OV_ERR_GENERIC;
			}
			break;
		}
		/*
		*	enter state and time stamp
		*/
		pvarcurrprops->state = OV_ST_NOTSUPPORTED;
		ov_time_gettime(&pvarcurrprops->time);
		return OV_ERR_OK;
	default:
		break;
	}
	return OV_ERR_BADPARAM;
}

/*	----------------------------------------------------------------------	*/

/*
*	Helper macros for implementing ov_object_setvar()
*/
#define Ov_Object_SetVarValue(vartype, VARTYPE)								\
	case OV_VT_##VARTYPE:													\
		if((pelem->elemunion.pvar->v_setfnc) && (!ov_activitylock)) {		\
			return ((OV_FNCPTR_SET(VARTYPE))								\
				pelem->elemunion.pvar->v_setfnc)(pobj, 						\
				pvarcurrprops->value.valueunion.val_##vartype);				\
		}																	\
		*((OV_##VARTYPE*)pelem->pvalue)										\
			= pvarcurrprops->value.valueunion.val_##vartype;				\
		return OV_ERR_OK
		
#define Ov_Object_SetTimeVarValue(vartype, VARTYPE)							\
	case OV_VT_##VARTYPE:													\
		if((pelem->elemunion.pvar->v_setfnc) && (!ov_activitylock)) {		\
			return ((OV_FNCPTR_SETTIME(VARTYPE))							\
				pelem->elemunion.pvar->v_setfnc)(pobj,						\
				&pvarcurrprops->value.valueunion.val_##vartype);			\
		}																	\
		*((OV_##VARTYPE*)pelem->pvalue)										\
			= pvarcurrprops->value.valueunion.val_##vartype;				\
		return OV_ERR_OK
		
#define Ov_Object_SetVarPVValue(vartype, VARTYPE)							\
	case OV_VT_##VARTYPE##_PV:												\
		if((pelem->elemunion.pvar->v_setfnc) && (!ov_activitylock)) {		\
			OV_##VARTYPE##_PV var;											\
			var.value = pvarcurrprops->value.valueunion.val_##vartype;		\
			var.state = pvarcurrprops->state;								\
			var.time = pvarcurrprops->time;									\
			return ((OV_FNCPTR_SETPV(VARTYPE))pelem->elemunion.pvar			\
				->v_setfnc)(pobj, &var);									\
		}																	\
		((OV_##VARTYPE##_PV*)pelem->pvalue)->value							\
			= pvarcurrprops->value.valueunion.val_##vartype;				\
		((OV_##VARTYPE##_PV*)pelem->pvalue)->state = pvarcurrprops->state;	\
		((OV_##VARTYPE##_PV*)pelem->pvalue)->time = pvarcurrprops->time;	\
		return OV_ERR_OK
		
#define Ov_Object_SetVarDynVecValue(vartype, VARTYPE)						\
	case OV_VT_##VARTYPE##_VEC:												\
		if((pelem->elemunion.pvar->v_setfnc) && (!ov_activitylock)) {		\
			return ((OV_FNCPTR_SETVEC(VARTYPE))								\
				pelem->elemunion.pvar->v_setfnc)(pobj, 						\
				pvarcurrprops->value.valueunion.val_##vartype##_vec.value,	\
				pvarcurrprops->value.valueunion.val_##vartype##_vec.veclen);\
		}																	\
		Ov_SetDynamicVectorValue((OV_##VARTYPE##_VEC*)pelem->pvalue, 		\
			pvarcurrprops->value.valueunion.val_##vartype##_vec.value,		\
			pvarcurrprops->value.valueunion.val_##vartype##_vec.veclen,		\
			VARTYPE);														\
		return OV_ERR_OK

#define Ov_Object_SetVarVecValue(vartype, VARTYPE)							\
	case OV_VT_##VARTYPE##_VEC:												\
		if((pelem->elemunion.pvar->v_setfnc) && (!ov_activitylock)) {		\
			return ((OV_FNCPTR_SETVEC(VARTYPE))								\
				pelem->elemunion.pvar->v_setfnc)(pobj, 						\
				pvarcurrprops->value.valueunion.val_##vartype##_vec.value,	\
				pvarcurrprops->value.valueunion.val_##vartype##_vec.veclen);\
		}																	\
		Ov_SetStaticVectorValue((OV_##VARTYPE*)pelem->pvalue, 				\
			pvarcurrprops->value.valueunion.val_##vartype##_vec.value,		\
			pvarcurrprops->value.valueunion.val_##vartype##_vec.veclen,		\
			VARTYPE);														\
		return OV_ERR_OK

/* Vectors of PV variables not supported */
#define Ov_Object_SetVarDynVecPVValue(vartype, VARTYPE)							\
	case OV_VT_##VARTYPE##_PV_VEC:												\
		if((pelem->elemunion.pvar->v_setfnc) && (!ov_activitylock)) {			\
			OV_##VARTYPE##_PV_VEC var;											\
			var.value = pvarcurrprops->value.valueunion.val_##vartype##_vec;	\
			var.state = pvarcurrprops->state;									\
			var.time = pvarcurrprops->time;										\
			return ((OV_FNCPTR_SETPVVEC(VARTYPE))pelem->elemunion.pvar			\
				->v_setfnc)(pobj, &var);										\
		}																		\
		((OV_##VARTYPE##_PV_VEC*)pelem->pvalue)->value							\
			= pvarcurrprops->value.valueunion.val_##vartype##_vec;				\
		((OV_##VARTYPE##_PV_VEC*)pelem->pvalue)->state = pvarcurrprops->state;	\
		((OV_##VARTYPE##_PV_VEC*)pelem->pvalue)->time = pvarcurrprops->time;	\
		return OV_ERR_OK
		
#define Ov_Object_SetVarVecPVValue(vartype, VARTYPE)							\
	case OV_VT_##VARTYPE##_PV_VEC:												\
		if((pelem->elemunion.pvar->v_setfnc) && (!ov_activitylock)) {			\
			OV_##VARTYPE##_PV_VEC var;											\
			var.value = pvarcurrprops->value.valueunion.val_##vartype##_vec;	\
			var.state = pvarcurrprops->state;									\
			var.time = pvarcurrprops->time;										\
			return ((OV_FNCPTR_SETPVVEC(VARTYPE))pelem->elemunion.pvar			\
				->v_setfnc)(pobj, &var);										\
		}																		\
		Ov_SetDynamicVectorValue((OV_##VARTYPE##_VEC*)pelem->pvalue, 		\
			pvarcurrprops->value.valueunion.val_##vartype##_vec.value,		\
			pvarcurrprops->value.valueunion.val_##vartype##_vec.veclen,		\
			VARTYPE);														\
		((OV_##VARTYPE##_PV_VEC*)pelem->pvalue)->state = pvarcurrprops->state;	\
		((OV_##VARTYPE##_PV_VEC*)pelem->pvalue)->time = pvarcurrprops->time;	\
		return OV_ERR_OK

/*	----------------------------------------------------------------------	*/

/**
*	Default method for writing the current properties of
*	a variable (value, state and timestamp, compare ACPLT/KS)
*/
OV_DLLFNCEXPORT OV_RESULT ov_object_setvar(
	OV_INSTPTR_ov_object	pobj,
	const OV_ELEMENT		*pelem,
	const OV_ANY			*pvarcurrprops
) {
//	OV_INSTPTR_ov_variable	 	pvar;
	OV_ELEMENT			elem;
	OV_BYTE*			value;
	OV_BYTE*			oldvalue;
	OV_UINT				i;
	OV_UINT				dist;

	switch(pelem->elemtype) {
	case OV_ET_MEMBER:
		if (!pelem->pvar) return  OV_ERR_GENERIC;
		if((pelem->pvar->v_setfnc) && (!ov_activitylock)) {
			value = (OV_BYTE*) ov_memstack_alloc(pelem->pvar->v_size);

			if((pelem->pvar->v_getfnc) && (!ov_activitylock)) 
				oldvalue = (OV_BYTE*)((pelem->pvar->v_getfnc)(pobj));
			else oldvalue = (OV_BYTE*)pobj + pelem->pvar->v_offset;

			for (i=0; i<pelem->pvar->v_size; i++) value[i] = oldvalue[i];

			elem.pobj = pobj;
			dist = value - (OV_BYTE*)pobj;
			elem.pvalue = pelem->pvalue - pelem->pvar->v_offset + dist;
			elem.elemtype = OV_ET_VARIABLE;
			elem.elemunion.pvar = pelem->elemunion.pvar;
			if (!Ov_Fail(ov_object_setvar(pobj, &elem, pvarcurrprops)))
				return (pelem->pvar->v_setfnc) (pobj, value);
			return OV_ERR_BADTYPE;
		}
	case OV_ET_VARIABLE:
		/*
		*	if not an ANY variable, check the variable type
		*/
		if((pelem->elemunion.pvar->v_vartype != OV_VT_ANY)
			&& ((pvarcurrprops->value.vartype & OV_VT_KSMASK)
			!= (pelem->elemunion.pvar->v_vartype & OV_VT_KSMASK))
		) {
			return OV_ERR_BADTYPE;
		}
		/*
		*	set the value -- and, in case of a PV, state and timestamp
		*/
		switch(pelem->elemunion.pvar->v_veclen) {
		case 1:
			/*
			*	scalar variable
			*/
			switch(pelem->elemunion.pvar->v_vartype) {
			Ov_Object_SetVarValue(byte, BYTE);
			Ov_Object_SetVarValue(bool, BOOL);
			Ov_Object_SetVarValue(int, INT);
			Ov_Object_SetVarValue(uint, UINT);
			Ov_Object_SetVarValue(single, SINGLE);
			Ov_Object_SetVarValue(double, DOUBLE);
			case OV_VT_STRING:
				if((pelem->elemunion.pvar->v_setfnc)  && (!ov_activitylock)){
					return ((OV_FNCPTR_SET(STRING))
						pelem->elemunion.pvar->v_setfnc)(pobj,
						pvarcurrprops->value.valueunion.val_string);
				}
				return ov_string_setvalue((OV_STRING*)pelem->pvalue,
					pvarcurrprops->value.valueunion.val_string);
					
			Ov_Object_SetTimeVarValue(time, TIME);
			Ov_Object_SetTimeVarValue(time_span, TIME_SPAN);
			
			Ov_Object_SetVarPVValue(bool, BOOL);
			Ov_Object_SetVarPVValue(int, INT);
			Ov_Object_SetVarPVValue(uint, UINT);
			Ov_Object_SetVarPVValue(single, SINGLE);
			Ov_Object_SetVarPVValue(double, DOUBLE);
			Ov_Object_SetVarPVValue(string, STRING);
			Ov_Object_SetVarPVValue(time, TIME);
			Ov_Object_SetVarPVValue(time_span, TIME_SPAN);
			
			case OV_VT_STRUCT:
				/*
				*	TODO & FIXME!
				*/
				return OV_ERR_NOTIMPLEMENTED;
			case OV_VT_ANY:
				if((pelem->elemunion.pvar->v_setfnc) && (!ov_activitylock)) {
					return ((OV_FNCPTR_SETANY)pelem->elemunion.pvar->v_setfnc)
						(pobj, pvarcurrprops);
				} else {
					return ov_variable_setanyvalue((OV_ANY*)pelem->pvalue,
						pvarcurrprops);
				}
			default:
				break;
			}
			break;

		case 0:
			/*
			*	dynamic vector variable
			*/
			switch(pelem->elemunion.pvar->v_vartype) {
			Ov_Object_SetVarDynVecValue(byte, BYTE);
			Ov_Object_SetVarDynVecValue(bool, BOOL);
			Ov_Object_SetVarDynVecValue(int, INT);
			Ov_Object_SetVarDynVecValue(uint, UINT);
			Ov_Object_SetVarDynVecValue(single, SINGLE);
			Ov_Object_SetVarDynVecValue(double, DOUBLE);
			Ov_Object_SetVarDynVecValue(string, STRING);
			Ov_Object_SetVarDynVecValue(time, TIME);
			Ov_Object_SetVarDynVecValue(time_span, TIME_SPAN);
			
			Ov_Object_SetVarDynVecPVValue(bool, BOOL);
			Ov_Object_SetVarDynVecPVValue(int, INT);
			Ov_Object_SetVarDynVecPVValue(uint, UINT);
			Ov_Object_SetVarDynVecPVValue(single, SINGLE);
			Ov_Object_SetVarDynVecPVValue(double, DOUBLE);
			Ov_Object_SetVarDynVecPVValue(string, STRING);
			Ov_Object_SetVarDynVecPVValue(time, TIME);
			Ov_Object_SetVarDynVecPVValue(time_span, TIME_SPAN);

			case OV_VT_STRUCT:
				/*
				*	vectors of structures are not available
				*/
				/* fall into... */
			default:
				break;
			}
			break;

		default:
			/*
			*	static vector variable
			*/
			if(pelem->elemunion.pvar->v_veclen
				!= pvarcurrprops->value.valueunion.val_generic_vec.veclen
			) {
				return OV_ERR_BADVALUE;
			}
			switch(pelem->elemunion.pvar->v_vartype) {
			Ov_Object_SetVarVecValue(byte, BYTE);
			Ov_Object_SetVarVecValue(bool, BOOL);
			Ov_Object_SetVarVecValue(int, INT);
			Ov_Object_SetVarVecValue(uint, UINT);
			Ov_Object_SetVarVecValue(single, SINGLE);
			Ov_Object_SetVarVecValue(double, DOUBLE);
			Ov_Object_SetVarVecValue(string, STRING);
			Ov_Object_SetVarVecValue(time, TIME);
			Ov_Object_SetVarVecValue(time_span, TIME_SPAN);

			Ov_Object_SetVarVecPVValue(bool, BOOL);
			Ov_Object_SetVarVecPVValue(int, INT);
			Ov_Object_SetVarVecPVValue(uint, UINT);
			Ov_Object_SetVarVecPVValue(single, SINGLE);
			Ov_Object_SetVarVecPVValue(double, DOUBLE);
			Ov_Object_SetVarVecPVValue(string, STRING);
			Ov_Object_SetVarVecPVValue(time, TIME);
			Ov_Object_SetVarVecPVValue(time_span, TIME_SPAN);

			case OV_VT_STRUCT:
				/*
				*	vectors of structures are not available
				*/
				/* fall into... */
			default:
				break;
			}
			break;
		}
		return OV_ERR_BADTYPE;
	default:
		break;
	}
	return OV_ERR_BADPARAM;
}

/*	----------------------------------------------------------------------	*/

/**
*	Test, if an object owns links (except to the parent domain and class)
*/
OV_DLLFNCEXPORT OV_BOOL ov_object_haslinks(
	OV_INSTPTR_ov_object 		pobj
) {
	/*
	*	local variables
	*/
	OV_ELEMENT				parent, child;
	/*
	*	initialize
	*/
	parent.elemtype = OV_ET_OBJECT;
	parent.pobj = pobj;
	child.elemtype = OV_ET_NONE;
	/*
	*	iterate over parent and child links and test if the are empty
	*/
	while(TRUE) {
		Ov_AbortIfNot(Ov_OK(ov_element_getnextpart(&parent, &child,
			OV_ET_OBJECT | OV_ET_PARENTLINK | OV_ET_CHILDLINK)));
		if(child.elemtype == OV_ET_NONE) {
			break;
		}
		switch(child.elemtype) {
		case OV_ET_OBJECT:
			if(ov_object_haslinks(child.pobj)) {
				return TRUE;
			}
			break;
		case OV_ET_PARENTLINK:
			if(ov_association_isusedparentlink(child.elemunion.passoc, pobj)) {
				return TRUE;
			}
			break;
		case OV_ET_CHILDLINK:
			if((child.elemunion.passoc != passoc_ov_containment)
				&& (child.elemunion.passoc != passoc_ov_instantiation)
			) {
				if(ov_association_isusedchildlink(child.elemunion.passoc, pobj)) {
					return TRUE;
				}
			}
			break;
		default:
			Ov_Warning("internal error");
			break;
		}
	}
	return FALSE;
}

/*	----------------------------------------------------------------------	*/

/**
*	Test, if a string is a valid identifier for an object
*/
OV_DLLFNCEXPORT OV_BOOL ov_object_identifierok(
	OV_STRING					identifier
) {
	/*
	*	check parameters
	*/
	if(!identifier) {
		return FALSE;
	}
	if(!*identifier) {
		return FALSE;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/**
*	Constructor of an object in case the database is not started up (subroutine)
*/
OV_RESULT ov_object_constructor_nostartup(
	OV_INSTPTR_ov_object		pobj
) {
	return OV_ERR_NOACCESS;
}

/*	----------------------------------------------------------------------	*/

/**
*	Method for getting the access rights of an object or one of its elements
*	in case the database is not started up (subroutine)
*/
OV_ACCESS ov_object_getaccess_nostartup(
	OV_INSTPTR_ov_object		pobj,
	const OV_ELEMENT			*pelem,
	const OV_TICKET				*pticket
) {
	/*
	*	switch based on the element's type
	*/
	switch(pelem->elemtype) {
	case OV_ET_VARIABLE:
		if(pelem->elemunion.pvar->v_varprops & OV_VP_DERIVED) {
			return OV_AC_NONE;
		}
		if(Ov_GetParent(ov_containment, Ov_GetParent(ov_containment,
			pelem->elemunion.pvar)) != Ov_PtrUpCast(ov_domain, &pdb->ov)
		) {
			return OV_AC_READ;
		}
		break;
	case OV_ET_OBJECT:
		return OV_AC_READ;
	case OV_ET_CHILDLINK:
		if(pelem->elemunion.passoc == passoc_ov_instantiation) {
			return OV_AC_NONE;
		}
		/* fall into... */
	case OV_ET_PARENTLINK:
		if(pelem->elemunion.passoc == passoc_ov_containment) {
			return OV_AC_NONE;
		}
		return OV_AC_READ;
	default:
		break;
	}
	return OV_AC_NONE;
}

/*	----------------------------------------------------------------------	*/

/*
*	Helper macros for moving the database to a new base address
*/
#define Ov_Adjust(type, ptr) 												\
	ptr = (type)(((ptr)?(((OV_BYTE*)(ptr))+distance):(NULL)))

#define Ov_AdjustCopy(type, ptr) 												\
	ptr = (type)(((ptr)?(((OV_BYTE*)(ptr))+offset):(NULL)))

#define Ov_UndoCopyAdjust(type, ptr) 												\
	ptr = (type)(((ptr)?(((OV_BYTE*)(ptr))-offset):(NULL)))

#define Ov_ClassPtr(pclass)													\
	((OV_INSTPTR_ov_class)((pclass)?(((OV_BYTE*)(pclass))+offset):(NULL)))
	
#define Ov_StringPtr(pstring)												\
	((OV_STRING)((pstring)?(((OV_BYTE*)(pstring))+offset):(NULL)))

#define Ov_ObjectPtr(pobj)													\
	((OV_INSTPTR_ov_object)((pobj)?(((OV_BYTE*)(pobj))+offset):(NULL)))

#define Ov_AssociationPtr(passoc)											\
	((OV_INSTPTR_ov_association)((passoc)?(((OV_BYTE*)(passoc))+offset):(NULL)))

#define Ov_IsVar(pelem)														\
	(!ov_string_compare(Ov_StringPtr(Ov_ClassPtr(Ov_GetParent(ov_instantiation, 		\
	pelem))->v_identifier), OV_OBJNAME_VARIABLE))

#define Ov_IsPart(pelem)													\
	(!ov_string_compare(Ov_StringPtr(Ov_ClassPtr(Ov_GetParent(ov_instantiation, 		\
	pelem))->v_identifier), OV_OBJNAME_PART))

#define Ov_VarAddress(pobj, offset)											\
	((OV_POINTER)(((OV_BYTE*)(pobj))+(offset)))

#define Ov_ObjAddress(pobj, offset)											\
	((OV_INSTPTR_ov_object)(((OV_BYTE*)(pobj))+(offset)))

#define Ov_HeadAddress(pobj, offset)										\
	((OV_HEAD*)(((OV_BYTE*)((pobj)->v_linktable))+(offset)))
	
#define Ov_AnchorAddress(pobj, offset)										\
	((OV_ANCHOR*)(((OV_BYTE*)((pobj)->v_linktable))+(offset)))
	
#define Ov_HeadAddressNM(pobj, offset)										\
	((OV_NMHEAD*)(((OV_BYTE*)((pobj)->v_linktable))+(offset)))
	
	
#define DBG_CHECKPOINTERS(ptr)												\
	if(ptr && (((OV_BYTE*) ptr + distance) < pdb->pstart || ((OV_BYTE*) ptr + distance) > pdb->pend))		\
	{		\
		ov_logfile_error("moving pointers: variable %s of object %s points outside the database", Ov_StringPtr(pelem->v_identifier), Ov_StringPtr(pobj->v_identifier));	\
		ptr = NULL;		\
	}
/*	----------------------------------------------------------------------	*/

/**
*	Move an object and its children/parts to a new address (subroutine)
*
*	For this all pointers of the object (pobj), its linktable, variables, parts and childs
*	must be adjusted to pointer+distance. For iterating over links, the copy of the obect (pobjcopy) is used,
*	because it stays unchanged in the algorithm.
*/
OV_RESULT ov_object_move(
	OV_INSTPTR_ov_object		pobj,
	const OV_INSTPTR_ov_object	pobjcopy,
	const OV_INT				distance
) {
	/*
	*	local variables
	*/
	OV_INT						offset;
	OV_INSTPTR_ov_class			pclass, pnextclass;
	OV_INSTPTR_ov_object		pelem, pcurr, pnextelem;
	OV_INSTPTR_ov_variable		pvar;
	OV_INSTPTR_ov_part			ppart;
	OV_INSTPTR_ov_association	passoc, pnextassoc;
	OV_UINT						i;
	OV_BOOL						domain = FALSE;
	OV_INSTPTR_ov_object		pchild, pnextchild;
	OV_STRING_VEC				*pvector;
	OV_ANY						*pany;
	Ov_Association_DefineIteratorNM(pit);

	/*
	*	determine pointer offset of objects in the copy
	*/
	offset = ((OV_BYTE*)pobjcopy-(OV_BYTE*)pobj)+distance;
	/*
	*	adjust pointer to the outer object
	*/
	#ifdef OV_DEBUG
	if(pobj->v_pouterobject && (((OV_BYTE*)pobj->v_pouterobject + distance)< pdb->pstart || ((OV_BYTE*) pobj->v_pouterobject + distance) > pdb->pend))
	{
		pobj->v_pouterobject = NULL;
		ov_logfile_error("moving pointers: pouterobject points outside the database for %s", Ov_StringPtr(pobj->v_identifier));
	}
	#endif
	Ov_Adjust(OV_INSTPTR_ov_object, pobj->v_pouterobject);
	/*
	*	adjust pointer to the association table
	*/
	#ifdef OV_DEBUG
	if(pobj->v_linktable && (((OV_BYTE*) pobj->v_linktable + distance) < pdb->pstart || ((OV_BYTE*) pobj->v_linktable + distance)> pdb->pend))
	{
		pobj->v_linktable = NULL;
		ov_logfile_error("moving pointers: linktable points outside the database for %s", Ov_StringPtr(pobj->v_identifier));
	}
	#endif
	Ov_Adjust(OV_ATBLPTR, pobj->v_linktable);
	/*
	*	to get the objectlinks in the copy the association table
	*	has to be adjusted
	*/
	Ov_AdjustCopy(OV_ATBLPTR, pobjcopy->v_linktable);
	/*
	*	iterate over the object's class and it's superclasses
	*/
	pclass=Ov_ClassPtr(Ov_GetParent(ov_instantiation, pobjcopy));
	/*
	*	in order to prevent multiple adjustments of the association table
	*	in the database copy, we undo it
	*/
	Ov_UndoCopyAdjust(OV_ATBLPTR, pobjcopy->v_linktable);
	while(pclass)
	{
		Ov_AdjustCopy(OV_ATBLPTR, pclass->v_linktable);
		pnextclass=Ov_ClassPtr(Ov_GetParent(ov_inheritance, pclass));
		Ov_UndoCopyAdjust(OV_ATBLPTR, pclass->v_linktable);
		/*
		*	test, if the class is class "domain"
		*/
		if(!domain) {
			domain = !ov_string_compare(Ov_StringPtr(pclass->v_identifier), OV_OBJNAME_DOMAIN);
		}
		/*
		*	iterate over the class elements
		*/
		Ov_AdjustCopy(OV_ATBLPTR, pclass->v_linktable);
		pelem=Ov_ObjectPtr(Ov_GetFirstChild(ov_containment, pclass));
		Ov_UndoCopyAdjust(OV_ATBLPTR, pclass->v_linktable);
		while(pelem)
		{
			Ov_AdjustCopy(OV_ATBLPTR, pelem->v_linktable);
			pnextelem=Ov_ObjectPtr(Ov_GetNextChild(ov_containment, pelem));
			if(Ov_IsVar(pelem)) {
				Ov_UndoCopyAdjust(OV_ATBLPTR, pelem->v_linktable);
				pvar = Ov_StaticPtrCast(ov_variable, pelem);
				if(!(pvar->v_varprops & OV_VP_DERIVED)) {
					if((pvar->v_vartype & OV_VT_KSMASK) == OV_VT_STRUCT) {
						/*
						*	variable is a structure, move it
						*/
						/*
						*	TODO!
						*/
						Ov_Warning("moving of structures not implemented");
					} else {
						switch(pvar->v_veclen) {
						case 1:
							/*
							*	variable is a scalar
							*/
							switch(pvar->v_vartype & OV_VT_KSMASK) {
							case OV_VT_STRING:
								#ifdef OV_DEBUG
								DBG_CHECKPOINTERS(*((OV_STRING*) Ov_VarAddress(pobj, pvar->v_offset)))
								#endif
								Ov_Adjust(OV_STRING, *((OV_STRING*)Ov_VarAddress(pobj, pvar->v_offset)));
								break;
							case OV_VT_ANY:
								pany = (OV_ANY*)Ov_VarAddress(pobj, pvar->v_offset);
								switch(pany->value.vartype & OV_VT_KSMASK) {
								case OV_VT_BYTE_VEC:
								case OV_VT_BOOL_VEC:
								case OV_VT_INT_VEC:
								case OV_VT_UINT_VEC:
								case OV_VT_SINGLE_VEC:
								case OV_VT_DOUBLE_VEC:
								case OV_VT_TIME_VEC:
								case OV_VT_TIME_SPAN_VEC:
									if(pany->value.valueunion.val_generic_vec.value && (((OV_BYTE*) pany->value.valueunion.val_generic_vec.value + distance) < pdb->pstart || ((OV_BYTE*) pany->value.valueunion.val_generic_vec.value + distance) > pdb->pend))
									{
										#ifdef OV_DEBUG
											ov_logfile_error("moving pointers: vector variable %s of object %s points outside the database", Ov_StringPtr(pelem->v_identifier), Ov_StringPtr(pobj->v_identifier));
										#endif
										pany->value.valueunion.val_generic_vec.veclen = 0;
										pany->value.valueunion.val_generic_vec.value = NULL;
									}
									Ov_Adjust(OV_POINTER, pany->value.valueunion.val_generic_vec.value);
									break;
								case OV_VT_STRING_VEC:
									pvector = &pany->value.valueunion.val_string_vec;
									if(pvector) {
										if(pvector->value && (((OV_BYTE*) pvector->value + distance) < pdb->pstart || ((OV_BYTE*) pvector->value + distance) > pdb->pend))
										{
											//fixme ignore initial value?

											#ifdef OV_DEBUG
												ov_logfile_error("moving pointers: string vector variable %s of object %s points outside the database", Ov_StringPtr(pelem->v_identifier), Ov_StringPtr(pobj->v_identifier));
											#endif
											pvector->value = NULL;
											pvector->veclen = 0;
										}
										Ov_Adjust(OV_STRING*, pvector->value);
										for(i=0; i<pvector->veclen; i++) {
											Ov_Adjust(OV_STRING, pvector->value[i]);
										}
									}
									break;
								default:
									break;
								}
								break;
							default:
								break;
							}
							break;

						case 0:
							/*
							*	variable is a dynamic vector
							*/
							#ifdef OV_DEBUG
							DBG_CHECKPOINTERS(((OV_GENERIC_VEC*) Ov_VarAddress
								(pobj, pvar->v_offset))->value)
							#endif
							Ov_Adjust(OV_POINTER, ((OV_GENERIC_VEC*)Ov_VarAddress
								(pobj, pvar->v_offset))->value);
							
							#ifdef OV_DEBUG							
							if(((OV_BYTE*) ((OV_GENERIC_VEC*) Ov_VarAddress
								(pobj, pvar->v_offset))->value + distance) < pdb->pstart || ((OV_BYTE*) ((OV_GENERIC_VEC*) Ov_VarAddress
								(pobj, pvar->v_offset))->value + distance) > pdb->pend)
							{
								((OV_GENERIC_VEC*) Ov_VarAddress (pobj, pvar->v_offset))->veclen = 0;
							}
							#endif
							if((pvar->v_vartype & OV_VT_KSMASK) == OV_VT_STRING_VEC) {
								pvector = (OV_STRING_VEC*)Ov_VarAddress(pobj, pvar->v_offset);
								if(pvector) {
									for(i=0; i<pvector->veclen; i++) {
										Ov_Adjust(OV_STRING, pvector->value[i]);
									}
								}
							}
							break;

						default:
							/*
							*	variable is a static vector
							*/
							if((pvar->v_vartype & OV_VT_KSMASK) == OV_VT_STRING_VEC) {
								for(i=0; i<pvar->v_veclen; i++) {
									#ifdef OV_DEBUG
									DBG_CHECKPOINTERS(*((OV_STRING*) Ov_VarAddress(pobj,
											pvar->v_offset+i*sizeof(OV_STRING))))
									#endif
									Ov_Adjust(OV_STRING, *((OV_STRING*) Ov_VarAddress(pobj,
											pvar->v_offset+i*sizeof(OV_STRING))));
									
								}
							}
							break;
						}	/* switch */
					}	/* if */
				}
			}	/* Ov_IsVar(pelem) */
			else if(Ov_IsPart(pelem)) {
				Ov_UndoCopyAdjust(OV_ATBLPTR, pelem->v_linktable);
				ppart = Ov_StaticPtrCast(ov_part, pelem);
				/*
				*	call ov_object_move on the part object
				*/
				ov_object_move(Ov_ObjAddress(pobj, ppart->v_offset),
					Ov_ObjAddress(pobjcopy, ppart->v_offset), distance);
			}	/* Ov_IsPart(pelem) */
			else {
				Ov_UndoCopyAdjust(OV_ATBLPTR, pelem->v_linktable);
			}
			pelem=pnextelem;
		}	/* for class elements */
		/*
		*	iterate over all associations in which we are parent
		*/
		Ov_AdjustCopy(OV_ATBLPTR, pclass->v_linktable);
		passoc=Ov_AssociationPtr(Ov_GetFirstChild(ov_parentrelationship, pclass));
		Ov_UndoCopyAdjust(OV_ATBLPTR, pclass->v_linktable);
		while(passoc)
		{
			Ov_AdjustCopy(OV_ATBLPTR, passoc->v_linktable);
			pnextassoc=Ov_AssociationPtr(Ov_GetNextChild(ov_parentrelationship, passoc));
			Ov_UndoCopyAdjust(OV_ATBLPTR, passoc->v_linktable);
			/*
			*	adjust parent link pointers
			*/
			switch(passoc->v_assoctype) {
			case OV_AT_ONE_TO_ONE:
				Ov_Adjust(OV_INSTPTR_ov_object, *(OV_INSTPTR_ov_object*) ((OV_BYTE*)pobj->v_linktable + passoc->v_parentoffset) );
				break;
			case OV_AT_ONE_TO_MANY:
				Ov_Adjust(OV_INSTPTR_ov_object, Ov_HeadAddress(pobj, passoc->v_parentoffset)->pfirst);
				Ov_Adjust(OV_INSTPTR_ov_object, Ov_HeadAddress(pobj, passoc->v_parentoffset)->plast);
				break;
			case OV_AT_MANY_TO_MANY:
				Ov_Adjust(OV_NMLINK*, Ov_HeadAddressNM(pobj, passoc->v_parentoffset)->pfirst);
				Ov_Adjust(OV_NMLINK*, Ov_HeadAddressNM(pobj, passoc->v_parentoffset)->plast);
				Ov_AdjustCopy(OV_ATBLPTR, passoc->v_linktable);
				Ov_Association_ForEachChildNM(passoc, pit, pobj, pcurr) {
					Ov_Adjust(OV_NMLINK*, pit->parent.pnext);
					Ov_Adjust(OV_NMLINK*, pit->parent.pprevious);
					Ov_Adjust(OV_INSTPTR_ov_object, pit->parent.pparent);
				}
				Ov_UndoCopyAdjust(OV_ATBLPTR, passoc->v_linktable);
				break;
			default:
				Ov_Warning("no such association type");
				return OV_ERR_GENERIC;
			}
			passoc = pnextassoc;
		}
		/*
		*	iterate over all associations in which we are child
		*/
		Ov_AdjustCopy(OV_ATBLPTR, pclass->v_linktable);
		passoc=Ov_AssociationPtr(Ov_GetFirstChild(ov_childrelationship, pclass));
		Ov_UndoCopyAdjust(OV_ATBLPTR, pclass->v_linktable);
		while(passoc)
		{
			Ov_AdjustCopy(OV_ATBLPTR, passoc->v_linktable);
			pnextassoc=Ov_AssociationPtr(Ov_GetNextChild(ov_childrelationship, passoc));
			Ov_UndoCopyAdjust(OV_ATBLPTR, passoc->v_linktable);
			/*
			*	adjust child link pointers
			*/
			switch(passoc->v_assoctype) {
			case OV_AT_ONE_TO_ONE:
				Ov_Adjust(OV_INSTPTR_ov_object, *(OV_INSTPTR_ov_object*) ((OV_BYTE*)pobj->v_linktable + passoc->v_childoffset) );
				break;
			case OV_AT_ONE_TO_MANY:
				Ov_Adjust(OV_INSTPTR_ov_object, Ov_AnchorAddress(pobj, passoc->v_childoffset)->pnext);
				Ov_Adjust(OV_INSTPTR_ov_object, Ov_AnchorAddress(pobj, passoc->v_childoffset)->pprevious);
				Ov_Adjust(OV_INSTPTR_ov_object, Ov_AnchorAddress(pobj, passoc->v_childoffset)->pparent);
				break;
			case OV_AT_MANY_TO_MANY:
				Ov_Adjust(OV_NMLINK*, Ov_HeadAddressNM(pobj, passoc->v_childoffset)->pfirst);
				Ov_Adjust(OV_NMLINK*, Ov_HeadAddressNM(pobj, passoc->v_childoffset)->plast);
				Ov_AdjustCopy(OV_ATBLPTR, passoc->v_linktable);
				Ov_Association_ForEachParentNM(passoc, pit, pobj, pcurr) {
					Ov_Adjust(OV_NMLINK*, pit->child.pnext);
					Ov_Adjust(OV_NMLINK*, pit->child.pprevious);
					Ov_Adjust(OV_INSTPTR_ov_object, pit->child.pchild);
				}
				Ov_UndoCopyAdjust(OV_ATBLPTR, passoc->v_linktable);
				break;
			default:
				Ov_Warning("no such association type");
				return OV_ERR_GENERIC;
			}
			passoc = pnextassoc;
		}
		pclass = pnextclass;
	}	/* for classes */
	/*
	*	if we are a domain, iterate over child objects and
	*	call ov_object_memmove on child objects
	*/
	if(domain) {
		Ov_AdjustCopy(OV_ATBLPTR, pobjcopy->v_linktable);
		pchild=Ov_ObjectPtr(Ov_GetFirstChild(ov_containment,Ov_StaticPtrCast(ov_domain, pobjcopy)));
		Ov_UndoCopyAdjust(OV_ATBLPTR, pobjcopy->v_linktable);
		while(pchild)
		{
			Ov_AdjustCopy(OV_ATBLPTR, pchild->v_linktable);
			pnextchild=Ov_ObjectPtr(Ov_GetNextChild(ov_containment, pchild));
			Ov_UndoCopyAdjust(OV_ATBLPTR, pchild->v_linktable);
			ov_object_move(Ov_ObjAddress(pchild, ((OV_BYTE*)pobj-(OV_BYTE*)pobjcopy)),
				pchild, distance);
			pchild=pnextchild;
		}
	}
	/*
	*	we are through
	*/
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

