/*
*   $Id: ov_library.h,v 1.5 2002-02-01 14:43:32 ansgar Exp $
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

#ifndef OV_LIBRARY_H_INCLUDED1
#define OV_LIBRARY_H_INCLUDED1

#include "libov/ov_ov.h"

#if OV_DYNAMIC_LIBRARIES
#include <stdio.h>
#if OV_SYSTEM_UNIX
#include <dlfcn.h>
#endif
#endif

/*
*	OV_FNC_LIBRARY_OPEN:
*	--------------------
*	Function prototype for opening a library
*/
typedef struct OV_LIBRARY_DEF* OV_FNC_LIBRARY_OPEN(void);

/*
*	OV_FNC_LIBRARY_SETGLOBALVARS:
*	-----------------------------
*	Function prototype for loading the global variables of a library
*/
typedef OV_RESULT OV_FNC_LIBRARY_SETGLOBALVARS(void);

/*
*	OV_LIBRARY_DEF:
*	---------------
*	Library definition, included in the library's code
*/
struct OV_LIBRARY_DEF {
	OV_STRING						identifier;
	OV_STRING						version;
	OV_STRING						ov_version;
	OV_STRING						author;
	OV_STRING						copyright;
	OV_STRING						comment;
	struct OV_STRUCTURE_DEF					*structures;
	struct OV_CLASS_DEF					*classes;
	struct OV_ASSOCIATION_DEF				*associations;
	OV_FNC_LIBRARY_SETGLOBALVARS				*setglobalvarsfnc;
};
typedef struct OV_LIBRARY_DEF OV_LIBRARY_DEF;

#if OV_STATIC_LIBRARIES
/*
*	OV_STATIC_LIBRARY_TABLE:
*	------------------------
*	Global table containing information about static linked libraries.
*	The table is an array; the last element is indicated by a NULL-string
*	as libname.
*/
typedef struct {
	OV_STRING			libname;
	OV_FNC_LIBRARY_OPEN	*openfnc;		
}	OV_STATIC_LIBRARY_TABLE;

extern OV_STATIC_LIBRARY_TABLE static_library_table[];

/*
*	Macros for building a static library table
*/
#define Ov_DeclareStaticLibrary(libname)									\
	OV_DLLFNCEXPORT OV_LIBRARY_DEF *ov_library_open_##libname(void)
#define Ov_BeginStaticLibraryTable											\
	OV_STATIC_LIBRARY_TABLE static_library_table[] = {
#define Ov_DefineStaticLibrary(libname)										\
		{ #libname, ov_library_open_##libname },
#define Ov_EndStaticLibraryTable											\
		{ NULL, NULL }														\
	}

#endif

#endif

#include "ov.h"

#ifdef OV_INSTBODY_ov_object

#ifndef OV_LIBRARY_H_INCLUDED2
#define OV_LIBRARY_H_INCLUDED2

#ifdef __cplusplus
extern "C" {
#endif

/*
*	Environment variable name: search path for DLLs/shared libraries
*/
#define OV_LIBRARY_PATH_ENV			"OV_LIBRARY_PATH"
#define OV_LIBRARY_PATH_ENV_MAXLEN	1024				/* restriction for NT only */

/*
*	Delimiter and separators used in paths
*/
#if OV_SYSTEM_NT
#define OV_DIRPATHDELIMITER "\\"
#define OV_DIRPATHSEPARATOR ";"
#elif OV_SYSTEM_OPENVMS
#define OV_DIRPATHDELIMITER ""
#define OV_DIRPATHSEPARATOR ","
#else
#define OV_DIRPATHDELIMITER "/"
#define OV_DIRPATHSEPARATOR ":"
#endif

/*
*	Search for a library object with given identifier
*/
OV_DLLFNCEXPORT OV_INSTPTR_ov_library ov_library_search(
	OV_STRING	identifier
);

/*
*	Open a library which is either a DLL/shared library or statically linked
*/
OV_DLLFNCEXPORT OV_LIBRARY_DEF *ov_library_open(
	OV_INSTPTR_ov_library	plib
);

/*
*	Close a library file if it is a DLL/shared library
*/
OV_DLLFNCEXPORT void ov_library_close(
	OV_INSTPTR_ov_library	plib
);

/*
*	Load a library and its definitions into the database
*/
OV_DLLFNCEXPORT OV_RESULT ov_library_load(
	OV_INSTPTR_ov_library	plib,
	OV_LIBRARY_DEF*			plibdef
);

/*
*	Compare a library with its definition
*/
OV_DLLFNCEXPORT OV_RESULT ov_library_compare(
	OV_INSTPTR_ov_library	plib,
	OV_LIBRARY_DEF*			plibdef
);

/*
*	Test if we can unload a library and its definitions from the database
*/
OV_DLLFNCEXPORT OV_BOOL ov_library_canunload(
	OV_INSTPTR_ov_library	plib
);

/*
*	Prepare for loading the OV library into the database (subroutine)
*/
OV_RESULT ov_library_prepare(
	OV_LIBRARY_DEF*		plibdef
);

/*
*	Get environment variable with library path
*/
OV_DLLFNCEXPORT OV_STRING ov_library_getenv(void);

/*
*	Set environment variable with library path
*/
OV_DLLFNCEXPORT void ov_library_setenv(
	OV_STRING path
);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif

#endif
/*
*	End of file
*/

