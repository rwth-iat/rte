/*
*   $Id: ov_library.c,v 1.2 1999-08-10 07:11:49 dirk Exp $
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
*/

#define OV_COMPILE_LIBOV

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
#define Ov_Library_OpenDLL(filename)	dlopen(filename, RTLD_NOW)
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
#define Ov_Library_CloseDLL(handle)	dlclose(handle)
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
OV_RESULT OV_DLLFNCEXPORT ov_library_constructor(
	OV_INSTPTR_ov_object	pobj
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_library	plib = Ov_StaticPtrCast(ov_library, pobj);
	OV_LIBRARY_DEF			*plibdef;
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
	return ov_library_load(plib, plibdef);
}

/*	----------------------------------------------------------------------	*/

/*
*	destructor of a library object: close library.
*/
void OV_DLLFNCEXPORT ov_library_destructor(
	OV_INSTPTR_ov_object	pobj
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_library	plib = Ov_StaticPtrCast(ov_library, pobj);
	/*
	*	close the library
	*/
	ov_library_close(plib);
}

/*	----------------------------------------------------------------------	*/

/*
*	Search for a library object with given identifier
*/
OV_INSTPTR_ov_library ov_library_search(
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
OV_LIBRARY_DEF* OV_DLLFNCEXPORT ov_library_open(
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
	OV_STRING			tmpenv;
	OV_STRING			path, nextpath;
	OV_STRING			delim;
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
		*	create a copy of the search path in which we can modify chars
		*/
		if(getenv(OV_LIBRARY_PATH_ENV)) {
			tmpenv =  Ov_HeapStrdup(getenv(OV_LIBRARY_PATH_ENV));
		} else {
			tmpenv =  Ov_HeapStrdup("");
		}
		if(!tmpenv) {
			return NULL;
		}
		/*
		*	iterate over all paths in the search paths
		*/
		for(path=tmpenv; path && !openfnc; path=nextpath) {
			nextpath = strstr(path, OV_DIRPATHSEPARATOR);
			if(nextpath) {
				*nextpath = 0;
				nextpath++;
			}
			/*
			*	if there is a path given, determine the path delimiter;
			*	otherwise there is none
			*/
			if(*path) {
				delim = OV_DIRPATHDELIMITER;
			} else {
				delim = "";
			}
			/*
			*	allocate memory for the DLL name and the function name;
			*	we allocate enough memory for both.
			*/
			tmpstring = (OV_STRING)Ov_HeapMalloc(strlen(path)+strlen(delim)
				+strlen(plib->v_identifier)+strlen(OV_DLLFLNSUFFIX)
				+strlen(OV_CONST_OPENFNC_PREFIX)+1);
			if(!tmpstring) {
				return NULL;
			}
			/*
			*	enter the DLL name into the string and try to open the DLL
			*/
			sprintf(tmpstring, "%s%s%s" OV_DLLFLNSUFFIX, path, delim,
				plib->v_identifier);
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
			/*
			*	free the temporary string
			*/
			Ov_HeapFree(tmpstring);
		}
		/*
		*	free the temporary search path
		*/
		Ov_HeapFree(tmpenv);
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
void OV_DLLFNCEXPORT ov_library_close(
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
OV_RESULT OV_DLLFNCEXPORT ov_library_load(
	OV_INSTPTR_ov_library	plib,
	OV_LIBRARY_DEF*			plibdef
) {
	/*
	*	local variables
	*/
	OV_STRUCTURE_DEF	*pstructdef;
	OV_CLASS_DEF		*pclassdef;
	OV_ASSOCIATION_DEF	*passocdef;
	OV_RESULT			result;
	/*
	*	check parameters
	*/
	if(!plib || !plibdef) {
		return OV_ERR_BADPARAM;
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
	return (plibdef->setglobalvarsfnc)();
}

/*	----------------------------------------------------------------------	*/

/*
*	Compare a library with its definition
*/
OV_RESULT OV_DLLFNCEXPORT ov_library_compare(
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
	OV_INST_ov_association 		assoc_containment;
	OV_INST_ov_association 		assoc_instantiation;
	OV_RESULT					result;
	/*
	*	if we load the OV metamodel, we have to prepare some things
	*/
	if(plib == &pdb->ov) {
		/*
		*	simulate the containment and the instantiation association
		*/
		assoc_containment.v_assoctype = OV_ASSOCIATION_DEF_ov_containment.assoctype;
		assoc_containment.v_assocprops = OV_ASSOCIATION_DEF_ov_containment.assocprops;
		assoc_containment.v_headoffset = OV_ASSOCIATION_DEF_ov_containment.headoffset;
		assoc_containment.v_anchoroffset = OV_ASSOCIATION_DEF_ov_containment.anchoroffset;
		passoc_ov_containment = &assoc_containment;
		assoc_instantiation.v_assoctype = OV_ASSOCIATION_DEF_ov_instantiation.assoctype;
		assoc_instantiation.v_assocprops = OV_ASSOCIATION_DEF_ov_instantiation.assocprops;
		assoc_instantiation.v_headoffset = OV_ASSOCIATION_DEF_ov_instantiation.headoffset;
		assoc_instantiation.v_anchoroffset = OV_ASSOCIATION_DEF_ov_instantiation.anchoroffset;
		passoc_ov_instantiation = &assoc_instantiation;
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
	ov_logfile_error("Error loading library \"%s\".", plib->v_identifier);
	return OV_ERR_LIBDEFMISMATCH;
}

/*	----------------------------------------------------------------------	*/

/*
*	Test if we can unload a library and its definitions from the database
*/
OV_BOOL OV_DLLFNCEXPORT ov_library_canunload(
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
	OV_INST_ov_association 		assoc_containment;
	OV_INST_ov_association 		assoc_instantiation;
	/*
	*	simulate the containment and the instantiation association
	*/
	assoc_containment.v_assoctype = OV_ASSOCIATION_DEF_ov_containment.assoctype;
	assoc_containment.v_assocprops = OV_ASSOCIATION_DEF_ov_containment.assocprops;
	assoc_containment.v_headoffset = OV_ASSOCIATION_DEF_ov_containment.headoffset;
	assoc_containment.v_anchoroffset = OV_ASSOCIATION_DEF_ov_containment.anchoroffset;
	passoc_ov_containment = &assoc_containment;
	assoc_instantiation.v_assoctype = OV_ASSOCIATION_DEF_ov_instantiation.assoctype;
	assoc_instantiation.v_assocprops = OV_ASSOCIATION_DEF_ov_instantiation.assocprops;
	assoc_instantiation.v_headoffset = OV_ASSOCIATION_DEF_ov_instantiation.headoffset;
	assoc_instantiation.v_anchoroffset = OV_ASSOCIATION_DEF_ov_instantiation.anchoroffset;
	passoc_ov_instantiation = &assoc_instantiation;
	/*
	*	link library objekt with root domain
	*/
	Ov_AbortIfNot(Ov_OK(Ov_Link(ov_containment, &pdb->root, &pdb->acplt)));
	Ov_AbortIfNot(Ov_OK(Ov_Link(ov_containment, &pdb->acplt, &pdb->ov)));
	/*
	*	provisorically load class definitions
	*/
	for(pclassdef=plibdef->classes; pclassdef; pclassdef=pclassdef->pnext) {
		pclass = Ov_DbMalloc(OV_INST_ov_class);
		if(!pclass) {
			return OV_ERR_DBOUTOFMEMORY;
		}
		memset(pclass, 0, Ov_GetInstSize(ov_class));
		Ov_AbortIfNot(Ov_OK(ov_string_setvalue(&pclass->v_identifier,
			pclassdef->identifier)));
		pclass->v_creationtime.secs = OV_VL_MAXUINT;
		pclass->v_classprops = pclassdef->classprops;
		pclass->v_size = pclassdef->size;
		pclass->v_pvtable = (OV_VTBLPTR)pclassdef->pvtable;
		Ov_AbortIfNot(Ov_OK(Ov_Link(ov_containment, &pdb->ov, pclass)));
	}
	/*
	*	initialize class pointers
	*/
	pclass_ov_library = Ov_StaticPtrCast(ov_class, Ov_SearchChild(ov_containment,
		&pdb->ov, OV_OBJNAME_LIBRARY));
	Ov_AbortIfNot(pclass_ov_library);
	pclass_ov_class = Ov_StaticPtrCast(ov_class, Ov_SearchChild(ov_containment,
		&pdb->ov, OV_OBJNAME_CLASS));
	Ov_AbortIfNot(pclass_ov_class);
	pclass_ov_association = Ov_StaticPtrCast(ov_class, Ov_SearchChild(ov_containment,
		&pdb->ov, OV_OBJNAME_ASSOCIATION));
	Ov_AbortIfNot(pclass_ov_association);
	/*
	*	link library object with class "library"
	*/
	Ov_AbortIfNot(Ov_OK(Ov_Link(ov_instantiation, pclass_ov_library, &pdb->ov)));
	/*
	*	link all class objects with class "class"
	*/
	for(pclassdef=plibdef->classes; pclassdef; pclassdef=pclassdef->pnext) {
		pobj = Ov_SearchChild(ov_containment, &pdb->ov, pclassdef->identifier);
		Ov_AbortIfNot(pobj);
		Ov_AbortIfNot(Ov_OK(Ov_Link(ov_instantiation, pclass_ov_class, pobj)));
	}
	/*
	*	provisorically load association definitions and link with class "association"
	*/
	for(passocdef=plibdef->associations; passocdef; passocdef=passocdef->pnext) {
		passoc = Ov_DbMalloc(OV_INST_ov_association);
		if(!passoc) {
			return OV_ERR_DBOUTOFMEMORY;
		}
		memset(passoc, 0, Ov_GetInstSize(ov_association));
		Ov_AbortIfNot(Ov_OK(ov_string_setvalue(&passoc->v_identifier,
			passocdef->identifier)));
		passoc->v_creationtime.secs = OV_VL_MAXUINT;
		passoc->v_assoctype = passocdef->assoctype;
		passoc->v_assocprops = passocdef->assocprops;
		passoc->v_headoffset = passocdef->headoffset;
		passoc->v_anchoroffset = passocdef->anchoroffset;
		Ov_AbortIfNot(Ov_OK(Ov_Link(ov_containment, &pdb->ov, passoc)));
		Ov_AbortIfNot(Ov_OK(Ov_Link(ov_instantiation, pclass_ov_association, passoc)));
	}
	/*
	*	set global variables of the OV model
	*/
	Ov_AbortIfNot(Ov_OK(ov_library_setglobalvars_ov()));
	/*
	*	link root and acplt domain with their classes
	*/
	Ov_AbortIfNot(Ov_OK(Ov_Link(ov_instantiation, pclass_ov_domain, &pdb->root)));
	Ov_AbortIfNot(Ov_OK(Ov_Link(ov_instantiation, pclass_ov_domain, &pdb->acplt)));
	/*
	*	realize inheritance relationships: object -- domain -- class
	*/
	Ov_AbortIfNot(Ov_OK(Ov_Link(ov_inheritance, pclass_ov_object, pclass_ov_domain)));
	Ov_AbortIfNot(Ov_OK(Ov_Link(ov_inheritance, pclass_ov_domain, pclass_ov_class)));
	/*
	*	preparation finished
	*/
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Accessor functions
*/
OV_STRING OV_DLLFNCEXPORT ov_library_version_get(
	OV_INSTPTR_ov_library	plib
) {
	return plib->v_version;
}

OV_STRING OV_DLLFNCEXPORT ov_library_author_get(
	OV_INSTPTR_ov_library	plib
) {
	return plib->v_author;
}

OV_STRING OV_DLLFNCEXPORT ov_library_copyright_get(
	OV_INSTPTR_ov_library	plib
) {
	return plib->v_copyright;
}

OV_STRING OV_DLLFNCEXPORT ov_library_comment_get(
	OV_INSTPTR_ov_library	plib
) {
	return plib->v_comment;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get environment variable with library path
*/
OV_STRING OV_DLLFNCEXPORT ov_library_getenv(void) {
	return getenv(OV_LIBRARY_PATH_ENV);
}

/*	----------------------------------------------------------------------	*/

/*
*	Set environment variable with library path
*/
void OV_DLLFNCEXPORT ov_library_setenv(
	OV_STRING path
) {
#if OV_SYSTEM_NT
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
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

