/*
*   $Id: ov_library.c,v 1.25 2007-09-25 13:19:41 martin Exp $
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
*	02-Jul-1998 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	09-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*	07-Jun-2001 J.Nagelmann <nagelmann@ltsoft.de>: Changes for Sun Solaris.
*   02-Jun-2013 Sten Gruener <s.gruener@plt.rwth-aachen.de>: Notes on valgrind.
*/

#define OV_COMPILE_LIBOV

#define _BSD_SOURCE
#include <stdlib.h>

#include "libov/ov_library.h"
#include "libov/ov_structure.h"
#include "libov/ov_class.h"
#include "libov/ov_association.h"
#include "libov/ov_logfile.h"
#include "libov/ov_database.h"
#include "libov/ov_string.h"
#include "libov/ov_time.h"
#include "libov/ov_macros.h"

#if OV_SYSTEM_MC164
#define memset xmemset
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Makro wrapping function to open a DLL/shared library
*/
#if OV_DYNAMIC_LIBRARIES
#if OV_SYSTEM_UNIX
#define Ov_Library_OpenDLL(filename)	dlopen(filename, RTLD_NOW | RTLD_GLOBAL)
#elif OV_SYSTEM_NT
#define Ov_Library_OpenDLL(filename)	LoadLibrary(filename)
#endif
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Makro wrapping function to close a DLL/shared library
*/
#if OV_DYNAMIC_LIBRARIES
#if OV_SYSTEM_UNIX
	#ifdef OV_VALGRIND
		//do not unload shared objects in valgrind mode to keep symbols for trace
		#define Ov_Library_CloseDLL(handle)	
	#else
		#define Ov_Library_CloseDLL(handle)	dlclose(handle)
	#endif
#elif OV_SYSTEM_NT
	#define Ov_Library_CloseDLL(handle)	FreeLibrary(handle)
#endif
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Makro wrapping the address resolution of a symbol in a DLL/shared library
*/
#if OV_DYNAMIC_LIBRARIES
#if OV_SYSTEM_UNIX
#define Ov_Library_GetAddr(handle, symbolname)	dlsym(handle, symbolname)
#elif OV_SYSTEM_NT
#define Ov_Library_GetAddr(handle, symbolname)	GetProcAddress(handle, symbolname)
#endif
#endif

/*	----------------------------------------------------------------------	*/

/*
*	constructor of a library object: open and load library.
*/
OV_DLLFNCEXPORT OV_RESULT ov_library_constructor(
	OV_INSTPTR_ov_object	pobj
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_library	plib = Ov_StaticPtrCast(ov_library, pobj);
	OV_LIBRARY_DEF			*plibdef;
	OV_RESULT			result;
	/*
	*	ensure, that there is no other library with this identifier
	*/
	if(plib != ov_library_search(plib->v_identifier)) {
		return OV_ERR_ALREADYEXISTS;
	}
	/*
	*	open the appropriate dynamic/static library
	*/
	plib->v_handle = 0;
	plibdef = ov_library_open(plib);
	if(!plibdef) {
		return OV_ERR_GENERIC;	/* TODO! error code? */
	}
	/*
	*	load the library into the database
	*/
	result = ov_library_load(plib, plibdef);
	if (Ov_Fail(result)) {
		ov_library_close(plib);
	}
	return result;
}

/*	----------------------------------------------------------------------	*/

/*
*	destructor of a library object: close library.
*/
OV_DLLFNCEXPORT void ov_library_destructor(
	OV_INSTPTR_ov_object	pobj
) {
	OV_INSTPTR_ov_class		pclass;
	OV_INSTPTR_ov_variable		pvar;
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_library	plib = Ov_StaticPtrCast(ov_library, pobj);
	/* 
	* 	delete all initialvalues references by deleting the vartype
	*/
	Ov_ForEachChildEx(ov_containment, plib, pclass, ov_class) {
		Ov_ForEachChildEx(ov_containment, pclass, pvar, ov_variable) {
			pvar->v_initialvalue.value.vartype = 0;
			
		}
	}
	/*
	*	close the library
	*/
	ov_library_close(plib);

}

/*	----------------------------------------------------------------------	*/

/*
*	Search for a library object with given identifier
*/
OV_DLLFNCEXPORT OV_INSTPTR_ov_library ov_library_search(
	OV_STRING	identifier
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_library	plib;
	/*
	*	ensure, that there is no other library with this identifier
	*/
	Ov_ForEachChildEx(ov_instantiation, pclass_ov_library, plib, ov_library) {
		if(!strcmp(plib->v_identifier, identifier)) {
			return plib;
		}
	}
	/*
	*	library not found
	*/
	return NULL;
}

/*	----------------------------------------------------------------------	*/

/*
*	Open a library which is either a DLL/shared library or statically linked
*/
OV_DLLFNCEXPORT OV_LIBRARY_DEF *ov_library_open(
	OV_INSTPTR_ov_library	plib
) {
	/*
	*	local variables
	*/
	OV_FNC_LIBRARY_OPEN	*openfnc = NULL;
#if OV_STATIC_LIBRARIES
	OV_UINT 			i;
#endif
#if OV_DYNAMIC_LIBRARIES
	OV_STRING			tmpstring;
	OV_DLLHANDLE		handle;
#endif
	/*
	*	is the library the OV metamodel?
	*/
	if(!strcmp(plib->v_identifier, OV_OBJNAME_OV)) {
		return ov_library_open_ov();
	}
#if OV_STATIC_LIBRARIES
	/*
	*	look for a statically linked library
	*/
	i = 0;
	while(static_library_table[i].libname) {
		if(!strcmp(static_library_table[i].libname, plib->v_identifier)) {
			openfnc = static_library_table[i].openfnc;
			break;
		}
		i++;
	}
#endif
#if OV_DYNAMIC_LIBRARIES
	/*
	*	look for a DLL/shared library
	*/
	if(!openfnc) {
		/*
		 *	allocate memory for the DLL name and the function name;
		 *	we allocate enough memory for both.
		 */
		tmpstring = (OV_STRING)Ov_HeapMalloc(strlen(plib->v_identifier)+strlen(OV_DLLFLNSUFFIX)
				+strlen(OV_CONST_OPENFNC_PREFIX)+1);
		if(!tmpstring) {
			return NULL;
		}
		/*
		 *	enter the DLL name into the string and try to open the DLL
		 */
		sprintf(tmpstring, "%s" OV_DLLFLNSUFFIX, plib->v_identifier);
		handle = Ov_Library_OpenDLL(tmpstring);
		if(handle) {
			plib->v_handle = handle;
			/*
			 *	we opened the DLL, get the address of the open function;
			 *	for that purpose enter its name into the string
			 */
			sprintf(tmpstring, OV_CONST_OPENFNC_PREFIX "%s",
					plib->v_identifier);
			/*
			 *	get the address of the open function
			 */
			openfnc = (OV_FNC_LIBRARY_OPEN*)Ov_Library_GetAddr(
					handle, tmpstring);
			if(!openfnc) {
				/*
				 *	we got no open function, close the DLL
				 */
				ov_library_close(plib);
			}
		}
		else {
			//specific error print
#if OV_SYSTEM_LINUX | OV_SYSTEM_SOLARIS
			ov_logfile_error("Can't load library '%s'. Reason: %s", plib->v_identifier, dlerror());
#endif
#if OV_SYSTEM_NT
			ov_logfile_error("Can't load library '%s'. Errorcode: %d", plib->v_identifier, GetLastError());
#endif
		}
		/*
		 *	free the temporary string
		 */
		Ov_HeapFree(tmpstring);
	}

#endif
	if(openfnc) {
		return openfnc();
	}
	return NULL;
}

/*	----------------------------------------------------------------------	*/

/*
*	Close a library file if it is a DLL/shared library
*/
OV_DLLFNCEXPORT void ov_library_close(
	OV_INSTPTR_ov_library	plib
) {
	if(plib->v_handle) {
#if OV_DYNAMIC_LIBRARIES
		Ov_Library_CloseDLL((OV_DLLHANDLE)plib->v_handle);
#endif
		plib->v_handle = 0;
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	Load a library and its definitions into the database
*/
OV_DLLFNCEXPORT OV_RESULT ov_library_load(
	OV_INSTPTR_ov_library	plib,
	OV_LIBRARY_DEF*			plibdef
) {
	/*
	*	local variables
	*/
	OV_STRUCTURE_DEF	*pstructdef;
	OV_CLASS_DEF		*pclassdef;
	OV_ASSOCIATION_DEF	*passocdef;
	OV_RESULT		result;
	char			ovversion1[16];
	char			ovversion2[16];
	char			ovversion[]=OV_VER_LIBOV;
	char*			pc1;
	char*			pc2;
	/*
	*	check parameters
	*/
	if(!plib || !plibdef) {
		return OV_ERR_BADPARAM;
	}
	/*
	*	check ov version of library
	*/
	if (!plibdef->ov_version) return OV_ERR_LIBDEFMISMATCH;
	if (strcmp(plibdef->ov_version, ovversion)) {
		strncpy(ovversion1, plibdef->ov_version, 15);
		strncpy(ovversion2, ovversion, 15);
		ovversion1[15]=0;
		ovversion2[15]=0;
		pc1 = (char*) ovversion1 + strlen(ovversion1)-1;
		pc2 = (char*) ovversion2 + strlen(ovversion2)-1;
		while (pc1>ovversion1) {
			if (*pc1 == '.') {
				*pc1=0;
				break;
			}
			pc1--;
		}
		while (pc2>ovversion2) {
			if (*pc2 == '.') {
				*pc2=0;
				break;
			}
			pc2--;
		}
		if ((!pc1) || (!pc2) || (strcmp(ovversion1, ovversion2))) {
			ov_logfile_error("Can't load library '%s'. OV-version of library (%s) too old for OV-server (%s)\n", plibdef->identifier, ovversion1,ovversion2);
			return OV_ERR_LIBDEFMISMATCH;
		}
	}
	/*
	*	if we load the OV metamodel, we have to prepare some things
	*/
	if(plib == &pdb->ov) {
		result = ov_library_prepare(plibdef);
		if(Ov_Fail(result)) {
			return result;
		}
	}
	/*
	*	set parameters
	*/
	ov_string_setvalue(&plib->v_version, plibdef->version);
	ov_string_setvalue(&plib->v_author, plibdef->author);
	ov_string_setvalue(&plib->v_copyright, plibdef->copyright);
	ov_string_setvalue(&plib->v_comment, plibdef->comment);
	/*
	*	load structures of the library
	*/
	for(pstructdef=plibdef->structures; pstructdef; pstructdef=pstructdef->pnext) {
		result = ov_structure_load(pstructdef, Ov_PtrUpCast(ov_domain, plib));
		if(Ov_Fail(result)) {
			return result;
		}
	}
	/*
	*	load classes of the library
	*/
	for(pclassdef=plibdef->classes; pclassdef; pclassdef=pclassdef->pnext) {
		result = ov_class_load(pclassdef, Ov_PtrUpCast(ov_domain, plib));
		if(Ov_Fail(result)) {
			return result;
		}
	}
	/*
	*	load associations of the library
	*/
	for(passocdef=plibdef->associations; passocdef; passocdef=passocdef->pnext) {
		result = ov_association_load(passocdef, Ov_PtrUpCast(ov_domain, plib));
		if(Ov_Fail(result)) {
			return result;
		}
	}
	/*
	*	load global variables of the library
	*/
	if (plibdef->setglobalvarsfnc) {
    
        result = plibdef->setglobalvarsfnc();
        return result;
    }

	return OV_ERR_LIBDEFMISMATCH;
}

/*	----------------------------------------------------------------------	*/

/*
*	Compare a library with its definition
*/
OV_DLLFNCEXPORT OV_RESULT ov_library_compare(
	OV_INSTPTR_ov_library	plib,
	OV_LIBRARY_DEF*			plibdef
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_object		pobj;
	OV_INSTPTR_ov_structure		pstruct;
	OV_INSTPTR_ov_class			pclass;
	OV_INSTPTR_ov_association	passoc;
	OV_STRUCTURE_DEF			*pstructdef;
	OV_CLASS_DEF				*pclassdef;
	OV_ASSOCIATION_DEF			*passocdef;
	OV_RESULT					result;
	/*
	*	if we load the OV metamodel, we have to prepare some things
	*/
	if(plib == &pdb->ov) {
		/*
		*	get the containment and the instantiation association
		*/
		passoc_ov_containment = &pdb->containment;
		passoc_ov_instantiation = &pdb->instantiation;
		/*
		*	the inheritance, childrelationship and parentrelationship association must be searched manually
		*   because the Ov_SearchChild function uses these relations indirectly
		*/
		pobj = ((OV_HEAD*)(((&pdb->ov)->v_linktable)+passoc_ov_containment->v_parentoffset))->pfirst;
		while (pobj) {
			if (!strcmp(pobj->v_identifier, "inheritance")) passoc_ov_inheritance = Ov_StaticPtrCast(ov_association, pobj);
			if (!strcmp(pobj->v_identifier, "childrelationship")) passoc_ov_childrelationship = Ov_StaticPtrCast(ov_association, pobj);
			if (!strcmp(pobj->v_identifier, "parentrelationship")) passoc_ov_parentrelationship = Ov_StaticPtrCast(ov_association, pobj);
			pobj = ((OV_ANCHOR*)((pobj->v_linktable)+passoc_ov_containment->v_childoffset))->pnext;
		}
		Ov_AbortIfNot(passoc_ov_inheritance);			
		Ov_AbortIfNot(passoc_ov_childrelationship);			
		Ov_AbortIfNot(passoc_ov_parentrelationship);			
			
		/*
		*	get pointer to class "library"
		*/
		pclass_ov_library = (OV_INSTPTR_ov_class)
			Ov_SearchChild(ov_containment, &pdb->ov, OV_OBJNAME_LIBRARY);
		Ov_AbortIfNot(pclass_ov_library);			
		/*
		*	get pointer to class "class"
		*/
		pclass_ov_class = (OV_INSTPTR_ov_class)
			Ov_SearchChild(ov_containment, &pdb->ov, OV_OBJNAME_CLASS);
		Ov_AbortIfNot(pclass_ov_library);			
		/*
		*	set OV global variables
		*/
		Ov_AbortIfNot(Ov_OK(ov_library_setglobalvars_ov()));
	}
	/*
	*	compare structures, classes and associations of the library
	*/
	pstructdef = plibdef->structures;
	pclassdef = plibdef->classes;
	passocdef = plibdef->associations;
	Ov_ForEachChild(ov_containment, plib, pobj) {
		pstruct = Ov_DynamicPtrCast(ov_structure, pobj);
		pclass = Ov_DynamicPtrCast(ov_class, pobj);
		passoc = Ov_DynamicPtrCast(ov_association, pobj);
		if(!pstruct && !pclass && !passoc) {
			goto ERRORMSG;
		}
		/*
		*	if element is a structure, compare
		*/
		if(pstruct) {
			if(!pstructdef) {
				goto ERRORMSG;
			}
			result = ov_structure_compare(pstruct, pstructdef);
			if(Ov_Fail(result)) {
				goto ERRORMSG;
			}
			pstructdef = pstructdef->pnext;
		}
		/*
		*	if element is a class, compare
		*/
		if(pclass) {
			if(!pclassdef) {
				goto ERRORMSG;
			}
			result = ov_class_compare(pclass, pclassdef);
			if(Ov_Fail(result)) {
				goto ERRORMSG;
			}
			pclassdef = pclassdef->pnext;
		}
		/*
		*	if element is an association, compare
		*/
		if(passoc) {
			if(!passocdef) {
				goto ERRORMSG;
			}
			result = ov_association_compare(passoc, passocdef);
			if(Ov_Fail(result)) {
				goto ERRORMSG;
			}
			passocdef = passocdef->pnext;
		}
	}
	if(pstructdef || pclassdef || passocdef) {
		goto ERRORMSG;
	}
	/*
	*	set parameters
	*/
	ov_string_setvalue(&plib->v_version, plibdef->version);
	ov_string_setvalue(&plib->v_author, plibdef->author);
	ov_string_setvalue(&plib->v_copyright, plibdef->copyright);
	ov_string_setvalue(&plib->v_comment, plibdef->comment);
	/*
	*	load global variables of the library and exit
	*/
	return (plibdef->setglobalvarsfnc)();
	/*
	*	print error message and exit
	*/
ERRORMSG:
	ov_logfile_error("Error loading library '%s'.", plib->v_identifier);
	return OV_ERR_LIBDEFMISMATCH;
}

/*	----------------------------------------------------------------------	*/

/*
*	Test if we can unload a library and its definitions from the database
*/
OV_DLLFNCEXPORT OV_BOOL ov_library_canunload(
	OV_INSTPTR_ov_library	plib
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_object		pobj;
	OV_INSTPTR_ov_structure		pstruct;
	OV_INSTPTR_ov_class			pclass;
	OV_INSTPTR_ov_association	passoc;
	/*
	*	test if we can unload the structures, classes and associations
	*/
	Ov_ForEachChild(ov_containment, plib, pobj) {
		pstruct = Ov_DynamicPtrCast(ov_structure, pobj);
		pclass = Ov_DynamicPtrCast(ov_class, pobj);
		passoc = Ov_DynamicPtrCast(ov_association, pobj);
		/*
		*	if element is a structure, test if we can unload it
		*/
		if(pstruct) {
			if(!ov_structure_canunload(pstruct)) {
				return FALSE;
			}
		}
		/*
		*	if element is a class, test if we can unload it
		*/
		else if(pclass) {
			if(!ov_class_canunload(pclass)) {
				return FALSE;
			}
		}
		/*
		*	if element is an association, test if we can unload it
		*/
		else if(passoc) {
			if(!ov_association_canunload(passoc)) {
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
*	Prepare for loading the OV library into the database (subroutine)
*/
OV_RESULT ov_library_prepare(
	OV_LIBRARY_DEF*		plibdef
) {

	/*
	*	local variables
	*/
	OV_INSTPTR_ov_object		pobj;
	OV_INSTPTR_ov_class			pclass;
	OV_INSTPTR_ov_association	passoc;
	OV_CLASS_DEF				*pclassdef;
	OV_ASSOCIATION_DEF			*passocdef;
	OV_INSTPTR_ov_association	childrelationship=NULL;
	OV_INSTPTR_ov_association	parentrelationship=NULL;
	/*
	*	simulate the containment and the instantiation association
	*/
	pdb->containment.v_assoctype = OV_ASSOCIATION_DEF_ov_containment.assoctype;
	pdb->containment.v_assocprops = OV_ASSOCIATION_DEF_ov_containment.assocprops;
	pdb->containment.v_parentoffset = ov_association_getparentoffset (&OV_ASSOCIATION_DEF_ov_containment);
	pdb->containment.v_childoffset = ov_association_getchildoffset (&OV_ASSOCIATION_DEF_ov_containment);
	passoc_ov_containment = &pdb->containment;
	pdb->instantiation.v_assoctype = OV_ASSOCIATION_DEF_ov_instantiation.assoctype;
	pdb->instantiation.v_assocprops = OV_ASSOCIATION_DEF_ov_instantiation.assocprops;
	pdb->instantiation.v_parentoffset = ov_association_getparentoffset (&OV_ASSOCIATION_DEF_ov_instantiation);
	pdb->instantiation.v_childoffset = ov_association_getchildoffset (&OV_ASSOCIATION_DEF_ov_instantiation);
	passoc_ov_instantiation = &pdb->instantiation;
	/*
	*	simulate the childrelationship and the parentrelationship association
	*/
	childrelationship = Ov_DbAlloc(OV_INST_ov_association);
	if(!childrelationship) {
		return OV_ERR_DBOUTOFMEMORY;
	}
	memset(childrelationship, 0, Ov_GetInstSize(ov_association));
	childrelationship->v_assoctype = OV_ASSOCIATION_DEF_ov_childrelationship.assoctype;
	childrelationship->v_assocprops = OV_ASSOCIATION_DEF_ov_childrelationship.assocprops;
	childrelationship->v_parentoffset = ov_association_getparentoffset (&OV_ASSOCIATION_DEF_ov_childrelationship);
	childrelationship->v_childoffset = ov_association_getchildoffset (&OV_ASSOCIATION_DEF_ov_childrelationship);
	passoc_ov_childrelationship = childrelationship;
	
	parentrelationship = Ov_DbAlloc(OV_INST_ov_association);
	if(!parentrelationship) {
		return OV_ERR_DBOUTOFMEMORY;
	}
	memset(parentrelationship, 0, Ov_GetInstSize(ov_association));
	parentrelationship->v_assoctype = OV_ASSOCIATION_DEF_ov_parentrelationship.assoctype;
	parentrelationship->v_assocprops = OV_ASSOCIATION_DEF_ov_parentrelationship.assocprops;
	parentrelationship->v_parentoffset = ov_association_getparentoffset (&OV_ASSOCIATION_DEF_ov_parentrelationship);
	parentrelationship->v_childoffset = ov_association_getchildoffset (&OV_ASSOCIATION_DEF_ov_parentrelationship);
	passoc_ov_parentrelationship = parentrelationship;
	/*
	*	link library objekt with root domain
	*/
	pdb->root.v_linktable = ov_database_malloc(ov_association_gettablesize(&OV_CLASS_DEF_ov_domain));
	if(!pdb->root.v_linktable) {
		return OV_ERR_DBOUTOFMEMORY;
	}
	memset(pdb->root.v_linktable, 0, ov_association_gettablesize(&OV_CLASS_DEF_ov_domain));
	pdb->acplt.v_linktable = ov_database_malloc(ov_association_gettablesize(&OV_CLASS_DEF_ov_domain));
	if(!pdb->acplt.v_linktable) {
		return OV_ERR_DBOUTOFMEMORY;
	}
	memset(pdb->acplt.v_linktable, 0, ov_association_gettablesize(&OV_CLASS_DEF_ov_domain));
	pdb->ov.v_linktable = ov_database_malloc(ov_association_gettablesize(&OV_CLASS_DEF_ov_library));
	if(!pdb->ov.v_linktable) {
		return OV_ERR_DBOUTOFMEMORY;
	}
	memset(pdb->ov.v_linktable, 0, ov_association_gettablesize(&OV_CLASS_DEF_ov_library));
	DoLink(ov_containment, &pdb->root, &pdb->acplt);
	DoLink(ov_containment, &pdb->acplt, &pdb->ov);
	/*
	*	provisorically load class definitions
	*/
	for(pclassdef=plibdef->classes; pclassdef; pclassdef=pclassdef->pnext) {
		pclass = (OV_INSTPTR_ov_class)ov_database_malloc(
			Ov_GetInstSize(ov_class)+pclassdef->staticsize);
		if(!pclass) {
			return OV_ERR_DBOUTOFMEMORY;
		}
		memset(pclass, 0, Ov_GetInstSize(ov_class)+pclassdef->staticsize);
		Ov_AbortIfNot(Ov_OK(ov_string_setvalue(&pclass->v_identifier,
			pclassdef->identifier)));
		pclass->v_creationtime.secs = OV_VL_MAXUINT;
		pclass->v_classprops = pclassdef->classprops;
		pclass->v_size = pclassdef->size;
		pclass->v_staticsize = pclassdef->staticsize;
		pclass->v_pvtable = (OV_VTBLPTR)pclassdef->pvtable;
		pclass->v_linktablesize = ov_association_gettablesize(pclassdef);
		pclass->v_linktable = ov_database_malloc(ov_association_gettablesize(&OV_CLASS_DEF_ov_class));
		if(!pclass->v_linktable) {
			return OV_ERR_DBOUTOFMEMORY;
		}
		memset(pclass->v_linktable, 0, ov_association_gettablesize(&OV_CLASS_DEF_ov_class));
		DoLink(ov_containment, &pdb->ov, pclass);
		ov_database_getId(&(pclass->v_idH), &(pclass->v_idL));
		ov_database_idListInsert(pclass->v_idH, pclass->v_idL, Ov_PtrUpCast(ov_object, pclass));
		/*
		*	initialize class pointers
		*/
		if (!strcmp(pclassdef->identifier, OV_OBJNAME_LIBRARY)) pclass_ov_library = pclass;
		if (!strcmp(pclassdef->identifier, OV_OBJNAME_CLASS)) pclass_ov_class = pclass;
		if (!strcmp(pclassdef->identifier, OV_OBJNAME_DOMAIN)) pclass_ov_domain = pclass;
		if (!strcmp(pclassdef->identifier, OV_OBJNAME_ASSOCIATION)) pclass_ov_association = pclass;
		if (!strcmp(pclassdef->identifier, OV_OBJNAME_OBJECT)) pclass_ov_object = pclass;
	}
	Ov_AbortIfNot(pclass_ov_library);
	Ov_AbortIfNot(pclass_ov_class);
	Ov_AbortIfNot(pclass_ov_association);
	/*
	*	link library object with class "library"
	*/
	DoLink(ov_instantiation, pclass_ov_library, &pdb->ov);
	/*
	*	provisorically load association definitions and link with class "association"
	*/
	for(passocdef=plibdef->associations; passocdef; passocdef=passocdef->pnext) {
		if (!strcmp(passocdef->identifier, "containment")) passoc = &pdb->containment;
		else if (!strcmp(passocdef->identifier, "instantiation")) passoc = &pdb->instantiation;
		else if (!strcmp(passocdef->identifier, "childrelationship")) passoc = childrelationship;
		else if (!strcmp(passocdef->identifier, "parentrelationship")) passoc = parentrelationship;
		else passoc = Ov_DbAlloc(OV_INST_ov_association);
		if(!passoc) {
			return OV_ERR_DBOUTOFMEMORY;
		}
		memset(passoc, 0, Ov_GetInstSize(ov_association));
		Ov_AbortIfNot(Ov_OK(ov_string_setvalue(&passoc->v_identifier,
			passocdef->identifier)));
		passoc->v_creationtime.secs = OV_VL_MAXUINT;
		passoc->v_assoctype = passocdef->assoctype;
		passoc->v_assocprops = passocdef->assocprops;
		passoc->v_parentoffset = ov_association_getparentoffset (passocdef);
		passoc->v_childoffset = ov_association_getchildoffset (passocdef);
		passoc->v_linktable = ov_database_malloc(pclass_ov_association->v_linktablesize);
		if(!passoc->v_linktable) {
			return OV_ERR_DBOUTOFMEMORY;
		}
		memset(passoc->v_linktable, 0, pclass_ov_association->v_linktablesize);
		DoLink(ov_containment, &pdb->ov, passoc);
		DoLink(ov_instantiation, pclass_ov_association, passoc);
		if (!strcmp(passocdef->identifier, "inheritance")){
			passoc_ov_inheritance = passoc;
		}
		ov_database_getId(&(passoc->v_idH), &(passoc->v_idL));
		ov_database_idListInsert(passoc->v_idH, passoc->v_idL, Ov_PtrUpCast(ov_object, passoc));
	}
	/*
	*	link all class objects with class "class"
	*/
	for(pclassdef=plibdef->classes; pclassdef; pclassdef=pclassdef->pnext) {
		Ov_ForEachChild(ov_containment, &pdb->ov, pobj) {
			if (!strcmp(pobj->v_identifier, pclassdef->identifier)) {
				DoLink(ov_instantiation, pclass_ov_class, pobj);
				break;
			}
		}
	}
	/*
	*	realize the parentrelationship link of the "containment" association and the class "domain"
	*/
	DoLink(ov_parentrelationship, pclass_ov_domain, &pdb->containment);
	/*
	*	realize the childrelationship link of the "containment" association and the class "object"
	*/
	DoLink(ov_childrelationship, pclass_ov_object, &pdb->containment);
	/*
	*	realize the inheritance link between "object" and "domain"
	*/
	DoLink(ov_inheritance, pclass_ov_object, pclass_ov_domain);
	/*
	*	realize the inheritance link between "object" and "association"
	*/
	DoLink(ov_inheritance, pclass_ov_object, pclass_ov_association);
	/*
	*	realize the inheritance link between "domain" and "library"
	*/
	DoLink(ov_inheritance, pclass_ov_domain, pclass_ov_library);
	/*
	*	realize the inheritance link between "domain" and "class"
	*/
	DoLink(ov_inheritance, pclass_ov_domain, pclass_ov_class);
	/*
	*	set global variables of the OV model
	*/
	Ov_AbortIfNot(Ov_OK(ov_library_setglobalvars_ov()));
	/*
	*	link root and acplt domain with their classes
	*/
	DoLink(ov_instantiation, pclass_ov_domain, &pdb->root);
	DoLink(ov_instantiation, pclass_ov_domain, &pdb->acplt);
	/*
	*	realize the inheritance link between "object" and "variable", "operation", "part"
	*/
	DoLink(ov_inheritance, pclass_ov_object, pclass_ov_variable);
	DoLink(ov_inheritance, pclass_ov_object, pclass_ov_operation);
	DoLink(ov_inheritance, pclass_ov_object, pclass_ov_part);
	/*
	*	realize the inheritance link between "domain" and "structure"
	*/
	DoLink(ov_inheritance, pclass_ov_domain, pclass_ov_structure);
	/*
	*	preparation finished
	*/
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get environment variable with library path
*/
OV_DLLFNCEXPORT OV_STRING ov_library_getenv(void) {
#if OV_SYSTEM_MC164
    return NULL;
#else	
    return getenv(OV_LIBRARY_PATH_ENV);
#endif
}

/*	----------------------------------------------------------------------	*/

/*
*	Set environment variable with library path
*/
OV_DLLFNCEXPORT void ov_library_setenv(
	OV_STRING path
) {
#if OV_SYSTEM_MC164  || OV_SYSTEM_SOLARIS
    return;
#else	
#if OV_SYSTEM_NT || OV_SYSTEM_RMOS
	/*
	*	local variables
	*/
	static char setpath[OV_LIBRARY_PATH_ENV_MAXLEN];
	/*
	*	instructions
	*/
	strcpy(setpath, OV_LIBRARY_PATH_ENV "=");
	strncat(setpath, path, sizeof(setpath)-strlen(setpath)-1);
	putenv(setpath);
#else
	setenv(OV_LIBRARY_PATH_ENV, path, TRUE);
#endif
#endif
}

/*	----------------------------------------------------------------------	*/

/*
*	Accessor functions
*/
OV_DLLFNCEXPORT OV_STRING ov_library_version_get(
	OV_INSTPTR_ov_library	plib
) {
	return plib->v_version;
}

OV_DLLFNCEXPORT OV_STRING ov_library_author_get(
	OV_INSTPTR_ov_library	plib
) {
	return plib->v_author;
}

OV_DLLFNCEXPORT OV_STRING ov_library_copyright_get(
	OV_INSTPTR_ov_library	plib
) {
	return plib->v_copyright;
}

OV_DLLFNCEXPORT OV_STRING ov_library_comment_get(
	OV_INSTPTR_ov_library	plib
) {
	return plib->v_comment;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/


