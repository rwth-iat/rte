/*
*   $Id: ov_class.c,v 1.3 1999-07-28 16:01:39 dirk Exp $
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
*	24-Aug-1998 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	12-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/

#define OV_COMPILE_LIBOV

#include "libov/ov_class.h"
#include "libov/ov_logfile.h"
#include "libov/ov_macros.h"
#include "libov/ov_time.h"
#include "libov/ov_variable.h"
#include "libov/ov_part.h"
#include "libov/ov_operation.h"
#include "libov/ov_object.h"
#include "libov/ov_database.h"
#include "libov/ov_scheduler.h"
#include "libov/ov_library.h"
#include "libov/ov_string.h"

#if OV_SYSTEM_MC164
#define memset xmemset
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Search for a class object with given identifier
*/
OV_INSTPTR_ov_class ov_class_search(
	OV_STRING	identifier
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_library	plib;
	OV_INSTPTR_ov_class		pclass;
	OV_STRING				libid;
	OV_STRING				classid;
	/*
	*	check parameters
	*/
	if(!identifier) {
		return NULL;
	}
	/*
	*	allocate memory for the library and the class identifier
	*/
	libid = Ov_HeapStrdup(identifier);
	if(!libid) {
		return NULL;
	}
	/*
	*	search for '/'
	*/
	for(classid = libid; *classid; classid++) {
		if(*classid == '/') {
			*classid = 0;
			classid++;
			break;
		}
	}
	/*
	*	search for the library
	*/
	plib = ov_library_search(libid);
	if(!plib) {
		Ov_HeapFree(libid);
		return NULL;
	}
	/*
	*	search for the class
	*/
	pclass = Ov_SearchChildEx(ov_containment, plib, classid, ov_class);
	Ov_HeapFree(libid);
	return pclass;
}

/*	----------------------------------------------------------------------	*/

/*
*	Load a class into the database
*/
OV_RESULT ov_class_load(
	OV_CLASS_DEF*			pclassdef,
	OV_INSTPTR_ov_domain		pparent
) {
	/*
	*	local variables
	*/
	OV_RESULT			result;
	OV_INSTPTR_ov_class	pclass, pbaseclass;
	OV_VARIABLE_DEF		*pvardef;
	OV_PART_DEF			*ppartdef;
	OV_OPERATION_DEF	*popdef;
	/*
	*	if there is a base class given, look for its definition
	*/
	if(pclassdef->baseclassname) {
		pbaseclass = ov_class_search(pclassdef->baseclassname);
		if(pbaseclass) {
			goto CONTINUE1;
		}
		return OV_ERR_UNKNOWNCLASSDEF;
	} else {
		pbaseclass = NULL;
	}
	/*
	*	test whether the class already exists
	*/
CONTINUE1:
	Ov_ForEachChildEx(ov_containment, pparent, pclass, ov_class) {
		if(!strcmp(pclass->v_identifier, pclassdef->identifier)) {
			/*
			*	class already exists; if it was created provisorically,
			*	skip object creation, otherwise it is an error
			*/
			if(pclass->v_creationtime.secs == OV_VL_MAXUINT) {
				goto CONTINUE2;
			}
			return OV_ERR_ALREADYEXISTS;
		}
	}
	/*
	*	create class object
	*/
	result = Ov_CreateObject(ov_class, pclass, pparent, pclassdef->identifier);
	if(Ov_Fail(result)) {
		return result;
	}
	/*
	*	set parameters
	*/
CONTINUE2:
	ov_time_gettime(&pclass->v_creationtime);
	pclass->v_classprops = pclassdef->classprops;
	ov_string_setvalue(&pclass->v_comment, pclassdef->comment);
	pclass->v_flags = pclassdef->flags;
	pclass->v_size = pclassdef->size;
	pclass->v_pvtable = (OV_VTBLPTR)pclassdef->pvtable;
	/*
	*	load variables of the class
	*/
	for(pvardef=pclassdef->variables; pvardef; pvardef=pvardef->pnext) {
		result = ov_variable_load(pvardef, Ov_PtrUpCast(ov_domain, pclass));
		if(Ov_Fail(result)) {
			return result;
		}
	}
	/*
	*	load parts of the class
	*/
	for(ppartdef=pclassdef->parts; ppartdef; ppartdef=ppartdef->pnext) {
		result = ov_part_load(ppartdef, Ov_PtrUpCast(ov_domain, pclass));
		if(Ov_Fail(result)) {
			return result;
		}
	}
	/*
	*	load operations of the class
	*/
	for(popdef=pclassdef->operations; popdef; popdef=popdef->pnext) {
		result = ov_operation_load(popdef, Ov_PtrUpCast(ov_domain, pclass));
		if(Ov_Fail(result)) {
			return result;
		}
	}
	/*
	*	link with base class if given
	*/
	if(pbaseclass) {
		if(Ov_GetParent(ov_inheritance, pclass)) {
			Ov_WarnIfNot(pbaseclass == Ov_GetParent(ov_inheritance, pclass));
		} else {
			Ov_WarnIfNot(Ov_OK(Ov_Link(ov_inheritance, pbaseclass, pclass)));
		}
	}
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Compare a class with its definition
*/
OV_RESULT ov_class_compare(
	OV_INSTPTR_ov_class		pclass,
	OV_CLASS_DEF*			pclassdef
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_class		pbaseclass;
	OV_INSTPTR_ov_object	pobj;
	OV_INSTPTR_ov_variable	pvar;
	OV_INSTPTR_ov_part		ppart;
	OV_INSTPTR_ov_operation	pop;
	OV_VARIABLE_DEF			*pvardef;
	OV_PART_DEF				*ppartdef;
	OV_OPERATION_DEF		*popdef;
	OV_RESULT				result;
	OV_STRING				id, libid;
	/*
	*	compare the name of the base class
	*/
	pbaseclass = Ov_GetParent(ov_inheritance, pclass);
	if(pclassdef->baseclassname) {
		if(pbaseclass) {
			libid = Ov_GetParent(ov_containment, pbaseclass)->v_identifier;
			id = (OV_STRING)Ov_HeapMalloc(strlen(libid)+1+strlen(pbaseclass->v_identifier)+1);
			if(!id) {
				return OV_ERR_HEAPOUTOFMEMORY;
			}
			sprintf(id, "%s/%s", libid, pbaseclass->v_identifier);
			if(strcmp(pclassdef->baseclassname, id)) {
				Ov_HeapFree(id);
				goto ERRORMSG;
			}
			Ov_HeapFree(id);
		} else {
			goto ERRORMSG;
		}
	} else {
		if(pbaseclass) {
			goto ERRORMSG;
		}
	}
	/*
	*	compare structure attributes
	*/
	if((pclass->v_classprops != pclassdef->classprops)
		|| (pclass->v_size != pclassdef->size)
	) {
		goto ERRORMSG;
	}
	/*
	*	compare the class elements
	*/
	pvardef = pclassdef->variables;
	ppartdef = pclassdef->parts;
	popdef = pclassdef->operations;
	Ov_ForEachChild(ov_containment, pclass, pobj) {
		pvar = Ov_DynamicPtrCast(ov_variable, pobj);
		ppart = Ov_DynamicPtrCast(ov_part, pobj);
		pop = Ov_DynamicPtrCast(ov_operation, pobj);
		if(!pvar && !ppart && !pop) {
			goto ERRORMSG;
		}
		/*
		*	if element is a variable, compare
		*/
		if(pvar) {
			if(!pvardef) {
				goto ERRORMSG;
			}
			result = ov_variable_compare(pvar, pvardef);
			if(Ov_Fail(result)) {
				goto ERRORMSG;
			}
			pvardef = pvardef->pnext;
		}
		/*
		*	if element is a part, compare
		*/
		if(ppart) {
			if(!ppartdef) {
				goto ERRORMSG;
			}
			result = ov_part_compare(ppart, ppartdef);
			if(Ov_Fail(result)) {
				goto ERRORMSG;
			}
			ppartdef = ppartdef->pnext;
		}
		/*
		*	if element is an operation, compare
		*/
		if(pop) {
			if(!popdef) {
				goto ERRORMSG;
			}
			result = ov_operation_compare(pop, popdef);
			if(Ov_Fail(result)) {
				goto ERRORMSG;
			}
			popdef = popdef->pnext;
		}
	}
	if(pvardef || popdef || ppartdef) {
		goto ERRORMSG;
	}
	/*
	*	all is fine, load some stuff
	*/
	ov_string_setvalue(&pclass->v_comment, pclassdef->comment);
	pclass->v_flags = pclassdef->flags;
	pclass->v_pvtable = (OV_VTBLPTR)pclassdef->pvtable;
	return OV_ERR_OK;
	/*
	*	print error message and exit
	*/
ERRORMSG:
	ov_logfile_error("Error loading class \"%s/%s\".",
		Ov_GetParent(ov_containment, pclass)->v_identifier,
		pclass->v_identifier);
	return OV_ERR_CLASSDEFMISMATCH;
}

/*	----------------------------------------------------------------------	*/

/*
*	Test if we can unload a class from the database
*/
OV_BOOL ov_class_canunload(
	OV_INSTPTR_ov_class		pclass
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_object	pobj;
	OV_INSTPTR_ov_variable 	pvar;
	OV_INSTPTR_ov_part		ppart;
	OV_INSTPTR_ov_operation	pop;
	OV_INSTPTR_ov_domain	plib;
	OV_INSTPTR_ov_class		psubclass;
	/*
	*	test if this class has instances or has subclasses or
	*	part classes used of other libraries
	*/
	if(Ov_GetFirstChild(ov_instantiation, pclass)) {
		return FALSE;
	}
	plib = Ov_GetParent(ov_containment, pclass);
	Ov_WarnIfNot(plib);
	Ov_ForEachChild(ov_inheritance, pclass, psubclass) {
		if(plib != Ov_GetParent(ov_containment, psubclass)) {
			return FALSE;
		}
	}
	Ov_ForEachChild(ov_embedment, pclass, ppart) {
		if(plib != Ov_GetParent(ov_containment, Ov_GetParent(ov_containment, ppart))) {
			return FALSE;
		}
	}
	/*
	*	test if we can unload the variables, parts and operations
	*/
	Ov_ForEachChild(ov_containment, pclass, pobj) {
		pvar = Ov_DynamicPtrCast(ov_variable, pobj);
		ppart = Ov_DynamicPtrCast(ov_part, pobj);
		pop = Ov_DynamicPtrCast(ov_operation, pobj);
		/*
		*	if element is a variable, test if we can unload it
		*/
		if(pvar) {
			if(!ov_variable_canunload(pvar)) {
				return FALSE;
			}
		}
		/*
		*	if element is a part, test if we can unload it
		*/
		else if(ppart) {
			if(!ov_part_canunload(ppart)) {
				return FALSE;
			}
		}
		/*
		*	if element is an operation, test if we can unload it
		*/
		else if(pop) {
			if(!ov_operation_canunload(pop)) {
				return FALSE;
			}
		}
		else {
			Ov_Warning("internal error");
			return FALSE;
		}
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	Test if a pointer to an instance of a class can be casted to another one
*/
OV_BOOL OV_DLLFNCEXPORT ov_class_cancastto(
	const OV_INSTPTR_ov_class	pclassfrom,
	const OV_INSTPTR_ov_class	pclassto
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_class pclass;
	/*
	*	check parameters
	*/
	if(!pclassfrom || !pclassto) {
		return FALSE;
	}
	/*
	*	check if the "to" class is the "from" class or any of its superclasses
	*/
	for(pclass = pclassfrom; pclass; pclass = Ov_GetParent(ov_inheritance, pclass)) {
		if(pclassto == pclass) {
			return TRUE;
		}
	}
	/*
	*	nope.
	*/
	return FALSE;
}

/*	----------------------------------------------------------------------	*/

/*
*	Create an instance of the class
*/
OV_RESULT OV_DLLFNCEXPORT ov_class_createobject(
	const OV_INSTPTR_ov_class	pclass,
	const OV_INSTPTR_ov_domain	pparent,
	const OV_STRING				identifier,
	const OV_PLACEMENT_HINT		hint,
	const OV_INSTPTR_ov_object	prelchild,
	OV_FNC_INITOBJ				*initobjfnc,
	OV_POINTER					userdata,
	OV_INSTPTR_ov_object		*ppobj
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_object	pobj;
	OV_VTBLPTR_ov_object	pvtable;
	OV_TIME					time;
	OV_RESULT				result;
	/*
	*	check parameters
	*/
	if(!ppobj || !pclass || !pparent || !identifier) {
		return OV_ERR_BADPARAM;
	}
	if(!Ov_DynamicPtrCast(ov_class, pclass)) {
		return OV_ERR_BADFACTORY;
	}
	if(!(pclass->v_classprops & OV_CP_INSTANTIABLE)) {
		return OV_ERR_BADFACTORY;
	}
	if(!Ov_DynamicPtrCast(ov_domain, pparent)) {
		return OV_ERR_BADPARAM;
	}
	if(!ov_object_identifierok(identifier)) {
		return OV_ERR_BADNAME;
	}
	/*
	*	get the vtable pointer
	*/
	pvtable = pclass->v_pvtable;
	if(!pvtable) {
		Ov_Warning("no vtable");
		return OV_ERR_GENERIC;
	}
	Ov_AbortIfNot(pvtable->m_constructor && pvtable->m_startup
		&& pvtable->m_destructor && pvtable->m_shutdown);
	/*
	*	assume instantiation is not successful
	*/
	*ppobj = NULL;
	/*
	*	allocate database memory for the new object and clear it
	*/
	pobj = (OV_INSTPTR_ov_object)ov_database_malloc(pclass->v_size);
	if(!pobj) {
		return OV_ERR_DBOUTOFMEMORY;
	}
	memset(pobj, 0, pclass->v_size);
	/*
	*	preinitialize the object
	*/
	ov_time_gettime(&time);
	result = ov_class_createobject_preinit(pclass, pobj, identifier, &time, NULL);
	if(Ov_Fail(result)) {
		ov_class_deleteobject_cleanupobj(pobj);
		return result;
	}
	/*
	*	link with parent domain
	*/
	result = Ov_LinkRelativePlaced(ov_containment, pparent, pobj, hint, prelchild);
	if(Ov_Fail(result)) {
		ov_class_deleteobject_cleanupobj(pobj);
		return result;
	}
	/*
	*	call the user-defined initialization function
	*/
	if(initobjfnc) {
		result = (initobjfnc)(pobj, userdata);
		if(Ov_Fail(result)) {
			ov_class_deleteobject_cleanupobj(pobj);
			ov_database_free(pobj);
			return result;
		}
	}
	/*
	*	set the object's state to "initialized"
	*/
	ov_class_createobject_setinit(pobj);
	/*
	*	call the constructor of the object
	*/
	result = (pvtable->m_constructor)(pobj);
	if(Ov_Fail(result)) {
		/*
		*	construction failed, delete object
		*/
		ov_class_deleteobject_cleanupobj(pobj);
		ov_database_free(pobj);
		return result;
	}
	/*
	*	start the object up
	*/
	(pvtable->m_startup)(pobj);
	/*
	*	finished.
	*/
	*ppobj = pobj;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Delete an instance of the class
*/
OV_RESULT OV_DLLFNCEXPORT ov_class_deleteobject(
	const OV_INSTPTR_ov_object	pobj
) {
	/*
	*	local variables
	*/
	OV_VTBLPTR_ov_object	pvtable;
	/*
	*	check parameters
	*/
	if(!pobj) {
		return OV_ERR_BADPARAM;
	}
	if(pobj->v_pouterobject) {
		return OV_ERR_BADPARAM;
	}
	/*
	*	get the vtable pointer of the object
	*/
	Ov_GetVTablePtr(ov_object, pvtable, pobj);
	if(!pvtable) {
		pvtable = pclass_ov_object->v_pvtable;
	}
	/*
	*	shut down the object
	*/
	(pvtable->m_shutdown)(pobj);
	/*
	*	if the object is still active, unregister with the scheduler
	*/
	if(pobj->v_objectstate & OV_OS_ACTIVE) {
		ov_scheduler_unregister(pobj);
	}
	/*
	*	call the destructor of the object
	*/
	(pvtable->m_destructor)(pobj);
	/*
	*	clean up object
	*/
	ov_class_deleteobject_cleanupobj(pobj);
	/*
	*	free the memory used for the object
	*/
	ov_database_free(pobj);
	/*
	*	finished
	*/
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Rename an instance of the class
*/
OV_RESULT OV_DLLFNCEXPORT ov_class_renameobject(
	const OV_INSTPTR_ov_object	pobj,
	const OV_INSTPTR_ov_domain	pparent,
	const OV_STRING				identifier,
	const OV_PLACEMENT_HINT		hint,
	const OV_INSTPTR_ov_object	prelchild
) {
	/*
	*	local variables
	*/
	OV_RESULT				result;
	OV_INSTPTR_ov_domain	pcurrparent;
	OV_INSTPTR_ov_object	pcurrobj;
	OV_BOOL					rename;
	OV_STRING				newid;
	/*
	*	check parameters
	*/
	if(!pobj || !pparent) {
		return OV_ERR_BADPARAM;
	}
	if(!Ov_DynamicPtrCast(ov_domain, pparent)) {
		return OV_ERR_BADPARAM;
	}
	/*
	*	do we want to rename the object?
	*/
	if(identifier) {
		if(!ov_object_identifierok(identifier)) {
			return OV_ERR_BADNAME;
		}
		newid = identifier;
		rename = TRUE;
	} else {
		newid = pobj->v_identifier;
		rename = FALSE;
	}
	/*
	*	we can't move the object to a position relative to itself
	*/
	if(pobj == prelchild) {
		return OV_ERR_BADPLACEMENT;
	}
	/*
	*	if we move the object to another parent domain, this parent must not be
	*	the object or any of it's children and it's new identifier must be unique
	*/
	pcurrparent = Ov_GetParent(ov_containment, pobj);
	Ov_AbortIfNot(pcurrparent);
	if(pparent != pcurrparent) {
		for(pcurrobj=Ov_PtrUpCast(ov_object, pparent); pcurrobj;
			pcurrobj=(pcurrobj->v_pouterobject)?(pcurrobj->v_pouterobject)
			:Ov_PtrUpCast(ov_object, Ov_GetParent(ov_containment, pcurrobj))
		) {
			if(pcurrobj == pobj) {
				return OV_ERR_CANTMOVE;
			}
		}
		if(Ov_SearchChild(ov_containment, pparent, newid)) {
			return OV_ERR_CANTMOVE;
		}
	}
	/*
	*	rename the object if necessary
	*/
	if(rename) {
		result = ov_string_setvalue(&pobj->v_identifier, newid);
		if(Ov_Fail(result)) {
			return result;
		}
	}
	/*
	*	move the object if necessary
	*/
	if(pparent != pcurrparent) {
		Ov_Unlink(ov_containment, pcurrparent, pobj);
		Ov_AbortIfNot(Ov_OK(Ov_LinkRelativePlaced(ov_containment,
			pparent, pobj, hint, prelchild)));
	}
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Preinitialization of an object during instantiation (subroutine)
*/
OV_RESULT ov_class_createobject_preinit(
	const OV_INSTPTR_ov_class	pclass,
	const OV_INSTPTR_ov_object	pobj,
	const OV_STRING				identifier,
	const OV_TIME				*ptime,
	const OV_INSTPTR_ov_object	pouterobj
) {
	/*
	*	local variables
	*/
	OV_ELEMENT				parent, child;
	OV_RESULT				result;
	/*
	*	initialize
	*/
	parent.elemtype = OV_ET_OBJECT;
	parent.pobj = pobj;
	child.elemtype = OV_ET_NONE;
	/*
	*	set parameters
	*/
	result = ov_string_setvalue(&pobj->v_identifier, identifier);
	if(Ov_Fail(result)) {
		return result;
	}
	pobj->v_creationtime = *ptime;
	pobj->v_pouterobject = pouterobj;
	pobj->v_objectstate = OV_OS_NONE;
	/*
	*	link with class object
	*/
	Ov_AbortIfNot(Ov_OK(Ov_Link(ov_instantiation, pclass, pobj)));
	/*
	*	iterate over the object's parts and preinitialize them
	*/
	while(TRUE) {
		Ov_AbortIfNot(Ov_OK(ov_element_getnextpart(&parent, &child, OV_ET_OBJECT)));
		if(child.elemtype == OV_ET_NONE) {
			break;
		}
		result = ov_class_createobject_preinit(
			Ov_GetParent(ov_embedment, child.elemunion.ppart), child.pobj,
			child.elemunion.ppart->v_identifier, ptime, pobj);
		if(Ov_Fail(result)) {
			return result;
		}
	}
	/*
	*	finished.
	*/
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Set object state to initialized during instantiation (subroutine)
*/
void ov_class_createobject_setinit(
	const OV_INSTPTR_ov_object	pobj
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
	*	set object state to initialized
	*/
	pobj->v_objectstate |= OV_OS_INIT;
	/*
	*	iterate over the object's parts and set them to initialized
	*/
	while(TRUE) {
		Ov_AbortIfNot(Ov_OK(ov_element_getnextpart(&parent, &child, OV_ET_OBJECT)));
		if(child.elemtype == OV_ET_NONE) {
			break;
		}
		ov_class_createobject_setinit(child.pobj);
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	Clean up of an object during deletion (subroutine)
*/
void ov_class_deleteobject_cleanupobj(
	OV_INSTPTR_ov_object		pobj
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_class		pclass = Ov_GetParent(ov_instantiation, pobj);
	OV_INSTPTR_ov_domain	pparent = Ov_GetParent(ov_containment, pobj);
	OV_INSTPTR_ov_object	pobj2;
	OV_ELEMENT				parent, child;
	/*
	*	initialize
	*/
	parent.elemtype = OV_ET_OBJECT;
	parent.pobj = pobj;
	child.elemtype = OV_ET_NONE;
	/*
	*	free strings and unlink links of this object and its parts
	*/
	if(pclass) {
		/*
		*	iterate over class elements and free strings, unlink links
		*/
		while(TRUE) {
			Ov_AbortIfNot(Ov_OK(ov_element_getnextpart(&parent, &child,
				OV_ET_VARIABLE | OV_ET_HEAD | OV_ET_ANCHOR)));
			if(child.elemtype == OV_ET_NONE) {
				break;
			}
			switch(child.elemtype) {
				case OV_ET_VARIABLE:
					if(!(child.elemunion.pvar->v_varprops & OV_VP_VIRTUAL)) {
						if(child.elemunion.pvar->v_vartype == OV_VT_STRUCT) {
							ov_class_deleteobject_cleanupstruct(&child);
						} else {
							switch(child.elemunion.pvar->v_veclen) {
								case 1:
									/*
									*	scalar variable
									*/
									if(child.elemunion.pvar->v_vartype == OV_VT_STRING) {
										ov_string_setvalue((OV_STRING*)child.pvalue, NULL);
										Ov_WarnIf(*(OV_STRING*)child.pvalue);									
									}
									break;
								case 0:
									/*
									*	dynamic vector variable
									*/
									if(child.elemunion.pvar->v_vartype == OV_VT_STRING_VEC) {
										Ov_WarnIfNot(Ov_OK(Ov_SetDynamicVectorValue(
											(OV_STRING_VEC**)child.pvalue, NULL, 0, STRING)));
										Ov_WarnIf(*(OV_STRING_VEC**)child.pvalue);
									} else {
										if(*(OV_DYNAMIC_VECTOR**)child.pvalue) {
											ov_database_free(*(OV_DYNAMIC_VECTOR**)child.pvalue);
										}
									}
									break;
								default:
									/*
									*	static vector variable
									*/
									if(child.elemunion.pvar->v_vartype == OV_VT_STRING_VEC) {
										Ov_WarnIfNot(Ov_OK(ov_string_setvecvalue(
											(OV_STRING*)child.pvalue, NULL,
											child.elemunion.pvar->v_veclen)));
									}
									break;
							}
						}
					}
					break;
				case OV_ET_HEAD:
					/*
					*	unlink all children
					*/
					while(TRUE) {
						pobj2 = Ov_Association_GetFirstChild(child.elemunion.passoc, pobj);
						if(!pobj2) {
							break;
						}
						if(child.elemunion.passoc == passoc_ov_containment) {
							/* delete the child */
							ov_class_deleteobject(pobj2);
						} else {
							/* unlink the child */
							(child.elemunion.passoc->v_unlinkfnc)(pobj, pobj2);
							if(Ov_Association_GetParent(child.elemunion.passoc, pobj2)) {
								ov_association_unlink(child.elemunion.passoc, pobj, pobj2);
							}
						}
					}
					break;
				case OV_ET_ANCHOR:
					/*
					*	unlink parent unless it's the class object
					*/
					if(child.elemunion.passoc != passoc_ov_instantiation) {
						pobj2 = Ov_Association_GetParent(child.elemunion.passoc, pobj);
						if(pobj2) {
							(child.elemunion.passoc->v_unlinkfnc)(pobj2, pobj);
							if(Ov_Association_GetParent(child.elemunion.passoc, pobj)) {
								ov_association_unlink(child.elemunion.passoc, pobj2, pobj);
							}
						}
					}
					break;
				default:
					Ov_Warning("internal error");
					break;				
			}
		}
		/*
		*	unlink from class object
		*/
		Ov_Unlink(ov_instantiation, pclass, pobj);
	} else {
		/*
		*	unlink from parent domain
		*/
		if(pparent) {
			Ov_Unlink(ov_containment, pparent, pobj);
		}
		/*
		*	free identifier
		*/
		ov_string_setvalue(&pobj->v_identifier, NULL);
	}
	return;
}

/*	----------------------------------------------------------------------	*/

/*
*	Clean up of a structure during deletion (subroutine)
*/
void ov_class_deleteobject_cleanupstruct(
	OV_ELEMENT	*pelem
) {
	/*
	*	local variables
	*/
	OV_ELEMENT				child;
	/*
	*	initialize
	*/
	child.elemtype = OV_ET_NONE;
	/*
	*	iterate over structure elements and free strings
	*/
	while(TRUE) {
		Ov_AbortIfNot(Ov_OK(ov_element_getnextpart(pelem, &child,
			OV_ET_VARIABLE)));
		if(child.elemtype == OV_ET_NONE) {
			break;
		}
		if(child.elemunion.pvar->v_vartype == OV_VT_STRUCT) {
			ov_class_deleteobject_cleanupstruct(&child);
		} else {
			switch(child.elemunion.pvar->v_veclen) {
				case 1:
					/*
					*	scalar variable
					*/
					if(child.elemunion.pvar->v_vartype == OV_VT_STRING) {
						ov_string_setvalue((OV_STRING*)child.pvalue, NULL);
						Ov_WarnIf(*(OV_STRING*)child.pvalue);									
					}
					break;
				case 0:
					/*
					*	dynamic vector variable
					*/
					if(child.elemunion.pvar->v_vartype == OV_VT_STRING_VEC) {
						Ov_WarnIfNot(Ov_OK(Ov_SetDynamicVectorValue(
							(OV_STRING_VEC**)child.pvalue, NULL, 0, STRING)));
						Ov_WarnIf(*(OV_STRING_VEC**)child.pvalue);
					} else {
						if(*(OV_DYNAMIC_VECTOR**)child.pvalue) {
							ov_database_free(*(OV_DYNAMIC_VECTOR**)child.pvalue);
						}
					}
					break;
				default:
					/*
					*	static vector variable
					*/
					if(child.elemunion.pvar->v_vartype == OV_VT_STRING_VEC) {
						Ov_WarnIfNot(Ov_OK(ov_string_setvecvalue(
							(OV_STRING*)child.pvalue, NULL,
							child.elemunion.pvar->v_veclen)));
					}
					break;
			}
		}
	}
	return;
}

/*	----------------------------------------------------------------------	*/

/*
*	Accessor functions
*/
OV_INT OV_DLLFNCEXPORT ov_class_classprops_get(
	OV_INSTPTR_ov_class	pclass
) {
	return pclass->v_classprops;
}

OV_UINT OV_DLLFNCEXPORT ov_class_size_get(
	OV_INSTPTR_ov_class	pclass
) {
	return pclass->v_size;
}

OV_STRING OV_DLLFNCEXPORT ov_class_comment_get(
	OV_INSTPTR_ov_class	pclass
) {
	return pclass->v_comment;
}

OV_UINT OV_DLLFNCEXPORT ov_class_flags_get(
	OV_INSTPTR_ov_class	pclass
) {
	return pclass->v_flags;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

