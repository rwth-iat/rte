/*
*   $Id: ov_codegen.h,v 1.1 1999-07-19 15:02:08 dirk Exp $
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
*	12-Jun-1998 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	14-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/

#ifndef OV_CODEGEN_H_INCLUDED
#define OV_CODEGEN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include "libov/ov_ov.h"

#if OV_COMPILER_BORLAND
#pragma warn -pro
#pragma warn -nod
#endif

#ifdef WIN32
#undef sparc
#endif

int yyparse(void);
int yylex(void);

#ifdef __cplusplus
extern "C" {
#endif

/*
*	Constants
*	---------
*/
#define MAX_INCLUDE_DEPTH	256
#define MAX_INCLUDEPATHS	256
#if OV_SYSTEM_NT
#define OV_DIRPATHDELIMITER "\\"
#elif OV_SYSTEM_OPENVMS
#define OV_DIRPATHDELIMITER ""
#else
#define OV_DIRPATHDELIMITER "/"
#endif

/*
*	OV_OVM_LIBRARY_DEF:
*	-------------------
*	Definition of a library used in codegen
*/
struct OV_OVM_LIBRARY_DEF {
	struct OV_OVM_LIBRARY_DEF		*pnext;
	OV_STRING						identifier;
	OV_STRING						version;
	OV_STRING						author;
	OV_STRING						copyright;
	OV_STRING						comment;
	struct OV_OVM_STRUCTURE_DEF		*structures;
	struct OV_OVM_CLASS_DEF			*classes;
	struct OV_OVM_ASSOCIATION_DEF	*associations;
};
typedef struct OV_OVM_LIBRARY_DEF OV_OVM_LIBRARY_DEF;

/*
*	OV_OVM_STRUCTURE_DEF:
*	---------------------
*	Definition of a structure used in codegen
*/
struct OV_OVM_STRUCTURE_DEF {
	struct OV_OVM_STRUCTURE_DEF		*pnext;
	OV_STRING						identifier;
	OV_STRING						libname;
	struct OV_OVM_VARIABLE_DEF		*members;
	OV_UINT							defnum;
};
typedef struct OV_OVM_STRUCTURE_DEF OV_OVM_STRUCTURE_DEF;

/*
*	OV_OVM_CLASS_DEF:
*	-----------------
*	Definition of a class used in codegen
*/
struct OV_OVM_CLASS_DEF {
	struct OV_OVM_CLASS_DEF			*pnext;
	OV_STRING						identifier;
	OV_STRING						libname;
	OV_STRING						baseclassname;
	OV_STRING						baseclasslibname;
	OV_CLASS_PROPS					classprops;
	OV_UINT							flags;
	OV_STRING						comment;
	struct OV_OVM_VARIABLE_DEF		*variables;
	struct OV_OVM_PART_DEF			*parts;
	struct OV_OVM_OPERATION_DEF		*operations;
	OV_UINT							numoperations;
	OV_UINT							defnum;
};
typedef struct OV_OVM_CLASS_DEF OV_OVM_CLASS_DEF;

/*
*	OV_OVM_ASSOCIATION_DEF:
*	-----------------------
*	Definition of an association used in codegen
*/
struct OV_OVM_ASSOCIATION_DEF {
	struct OV_OVM_ASSOCIATION_DEF	*pnext;
	OV_STRING						identifier;
	OV_STRING						libname;
	OV_ASSOC_TYPE					assoctype;
	OV_ASSOC_PROPS					assocprops;
	OV_STRING						parentrolename;
	OV_STRING						childrolename;
	OV_STRING						parentclassname;
	OV_STRING						childclassname;
	OV_STRING						headcomment;
	OV_STRING						anchorcomment;
	OV_UINT							headflags;
	OV_UINT							anchorflags;
	OV_UINT							defnum;
};
typedef struct OV_OVM_ASSOCIATION_DEF OV_OVM_ASSOCIATION_DEF;

/*
*	OV_OVM_VARIABLE_DEF:
*	--------------------
*	Definition of a variable used in codegen
*/
struct OV_OVM_VARIABLE_DEF {
	struct OV_OVM_VARIABLE_DEF		*pnext;
	OV_STRING						identifier;
	OV_UINT							veclen;
	OV_VAR_TYPE						vartype;
	OV_STRING						structurename;
	OV_STRING						ctypename;
	OV_STRING						structurelibname;
	OV_VAR_PROPS					varprops;
	OV_UINT							flags;
	OV_STRING						tech_unit;
	OV_STRING						comment;
};
typedef struct OV_OVM_VARIABLE_DEF OV_OVM_VARIABLE_DEF;

/*
*	OV_OVM_PART_DEF:
*	----------------
*	Defintion of a part used in codegen
*/
struct OV_OVM_PART_DEF {
	struct OV_OVM_PART_DEF			*pnext;
	OV_STRING						identifier;
	OV_STRING						partclassname;
	OV_STRING						partclasslibname;
};
typedef struct OV_OVM_PART_DEF OV_OVM_PART_DEF;

/*
*	OV_OVM_OPERATION_DEF:
*	---------------------
*	Definition of an operation used in codegen
*/
struct OV_OVM_OPERATION_DEF {
	struct OV_OVM_OPERATION_DEF		*pnext;
	OV_STRING						identifier;
	OV_STRING						cfnctypename;
	OV_OP_PROPS						opprops;
	OV_BOOL							overloaded;
	OV_UINT							operationnum;
	OV_STRING						classname;
};
typedef struct OV_OVM_OPERATION_DEF OV_OVM_OPERATION_DEF;

/*
*	OV_OVM_VARTYPE_DEF:
*	-------------------
*	Definition of a variable type used in codegen
*/
struct OV_OVM_VARTYPE_DEF {
	OV_VAR_TYPE						vartype;
	OV_STRING						structurename;
	OV_STRING						ctypename;
};
typedef struct OV_OVM_VARTYPE_DEF OV_OVM_VARTYPE_DEF;

/*
*	OV_STRINGSTACK:
*	---------------
*	Manages a list of strings
*/
struct OV_STRINGSTACK {
	struct OV_STRINGSTACK	*pnext;
	OV_STRING				string;
};
typedef struct OV_STRINGSTACK OV_STRINGSTACK;

/*
*	Global variables
*	----------------
*/
extern OV_INT				includepath_ptr;
extern OV_STRING			includepath[MAX_INCLUDEPATHS];
extern OV_INT				current_line;
extern OV_STRING			filename;
extern OV_INT 				include_stack_ptr;
extern OV_STRING			filename_stack[MAX_INCLUDE_DEPTH];
extern OV_INT				current_line_stack[MAX_INCLUDE_DEPTH];

extern OV_OVM_LIBRARY_DEF	*libraries;

/*
*	Functions and macros of the lexical scanner
*	-------------------------------------------
*/
/*
*	Allocate memory on the heap and abort if we get no memory
*/
OV_POINTER ov_codegen_malloc(
	OV_UINT		size
);

/*
*	Free memory allocated on the heap
*/
void ov_codegen_free(
	OV_POINTER	ptr
);

/*
*	Allocate memory for a string using ov_codegen_malloc()
*/
OV_STRING ov_codegen_getstring(
	const OV_STRING		string,
	const OV_UINT		length
);	

/*
*	Free strings allocated by the scanner
*/
void ov_codegen_freestrings(void);

/*
*	Allocate memory for an object of given type using ov_codegen_malloc()
*/
#define Ov_Codegen_Malloc(type)	((type*)ov_codegen_malloc(sizeof(type)))

/*
*	Free memory allocated on the heap
*/
#define Ov_Codegen_Free(ptr)	ov_codegen_free(ptr)

/*
*	Functions and macros of the syntactical parser
*	----------------------------------------------
*/
/*
*   Print out an error message
*/
int yyerror(char *msg);

/*
*	Check overall semantics
*/
OV_BOOL ov_codegen_checksemantics(void);

/*
*	Check semantics of a library
*/
OV_BOOL ov_codegen_checksemantics_library(
	OV_OVM_LIBRARY_DEF	*plib
);

/*
*	Check semantics of a structure
*/
OV_BOOL ov_codegen_checksemantics_structure(
	OV_OVM_LIBRARY_DEF		*plib,
	OV_OVM_STRUCTURE_DEF	*pstruct
);

/*
*	Check semantics of a class
*/
OV_BOOL ov_codegen_checksemantics_class(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass
);

/*
*	Check semantics of an association
*/
OV_BOOL ov_codegen_checksemantics_association(
	OV_OVM_LIBRARY_DEF		*plib,
	OV_OVM_ASSOCIATION_DEF	*passoc
);

/*
*	Check semantics of a member of a structure
*/
OV_BOOL ov_codegen_checksemantics_member(
	OV_OVM_LIBRARY_DEF		*plib,
	OV_OVM_STRUCTURE_DEF	*pstruct,
	OV_OVM_VARIABLE_DEF		*pvar
);

/*
*	Check semantics of a variable of a class
*/
OV_BOOL ov_codegen_checksemantics_variable(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	OV_OVM_VARIABLE_DEF	*pvar
);

/*
*	Check semantics of a part of a class
*/
OV_BOOL ov_codegen_checksemantics_part(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	OV_OVM_PART_DEF		*ppart
);

/*
*	Check semantics of an operation of a class
*/
OV_BOOL ov_codegen_checksemantics_operation(
	OV_OVM_LIBRARY_DEF		*plib,
	OV_OVM_CLASS_DEF		*pclass,
	OV_OVM_OPERATION_DEF	*pop
);

/*
*	Get pointer to library definition
*/
OV_OVM_LIBRARY_DEF *ov_codegen_getlibdef(
	OV_STRING	identifier
);

/*
*	Get pointer to structure definition
*/
OV_OVM_STRUCTURE_DEF *ov_codegen_getstructdef(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_STRING			identifier
);

/*
*	Get pointer to class definition
*/
OV_OVM_CLASS_DEF *ov_codegen_getclassdef(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_STRING			identifier
);

/*
*	Get pointer to association definition
*/
OV_OVM_ASSOCIATION_DEF *ov_codegen_getassocdef(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_STRING			identifier
);

/*
*	Get pointer to a member definition of a structure
*/
OV_OVM_VARIABLE_DEF *ov_codegen_getmemberdef(
	OV_OVM_LIBRARY_DEF		*plib,
	OV_OVM_STRUCTURE_DEF	*pstruct,
	OV_STRING				identifier
);

/*
*	Get pointer to a variable definition of a class
*/
OV_OVM_VARIABLE_DEF *ov_codegen_getvardef(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	OV_STRING			identifier
);

/*
*	Get pointer to a part definition of a class
*/
OV_OVM_PART_DEF *ov_codegen_getpartdef(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	OV_STRING			identifier
);

/*
*	Get pointer to an operation definition of a class
*/
OV_OVM_OPERATION_DEF *ov_codegen_getopdef(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	OV_STRING			identifier
);

/*
*	Get pointer to an operation definition of a class by a given number
*/
OV_OVM_OPERATION_DEF *ov_codegen_getopdefbynum(
	OV_OVM_CLASS_DEF	*pclass,
	OV_UINT				num
);

/*
*	Clean up memory
*/
void ov_codegen_cleanup(void);

#define Ov_Codegen_InsertDef(type, pnewlist, poldlist, pnewdef)	\
	{															\
		OV_OVM_##type##_DEF	*pdef = poldlist;					\
		if(poldlist) {											\
			while(pdef->pnext) {								\
				pdef = pdef->pnext;								\
			}													\
			pdef->pnext = pnewdef;								\
			pnewlist = poldlist;								\
		} else {												\
			pnewlist = pnewdef;									\
		}														\
	}

/*
*	Functions and macros of the backend
*	-----------------------------------
*/
/*
*	Backend of the code generator
*/
int ov_codegen_backend(void);

/*
*	Create header file of a library
*/
int ov_codegen_createheaderfile(
	OV_OVM_LIBRARY_DEF*	plib
);

/*
*	Create source file of a library
*/
int ov_codegen_createsourcefile(
	OV_OVM_LIBRARY_DEF*	plib
);

/*
*	Create a new file with a given extension
*/
FILE *ov_codegen_createfile(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_STRING			extension
);

/*
*	Close an existing file
*/
int ov_codegen_closefile(
	FILE	*fp
);

/*
*	Print typedef associated with a structure
*/
void ov_codegen_printstructtypedef(
	OV_OVM_LIBRARY_DEF		*plib,
	OV_OVM_STRUCTURE_DEF	*pstruct,
	FILE					*fp
);

/*
*	Print defines associated with an associations
*/
void ov_codegen_printassocdefines(
	OV_OVM_LIBRARY_DEF		*plib,
	OV_OVM_ASSOCIATION_DEF	*passoc,
	FILE					*fp
);

/*
*	Print instance defines associated with a class
*/
void ov_codegen_printclassinstdefines(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	FILE				*fp
);

/*
*	Print function declaration associated with a class
*/
void ov_codegen_printclassfncdecls(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	FILE				*fp
);

/*
*	Print vtable defines associated with a class
*/
void ov_codegen_printclassvtbldefines(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	FILE				*fp
);

/*
*	Print accessor function declarations associated with a class
*/
void ov_codegen_printclassaccessorfncdecls(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	FILE				*fp
);

/*
*	Print function declarations associated with an association
*/
void ov_codegen_printassocfncdecls(
	OV_OVM_LIBRARY_DEF		*plib,
	OV_OVM_ASSOCIATION_DEF	*passoc,
	FILE					*fp
);

/*
*	Print vtable definion associated with a class
*/
void ov_codegen_printclassvtbldef(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	FILE				*fp
);

/*
*	Print definition objects associated with a structure
*/
void ov_codegen_printstructdefobjs(
	OV_OVM_LIBRARY_DEF		*plib,
	OV_OVM_STRUCTURE_DEF	*pstruct,
	FILE					*fp
);

/*
*	Print definition objects associated with a class
*/
void ov_codegen_printclassdefobjs(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	FILE				*fp
);

/*
*	Print definition object associated with an association
*/
void ov_codegen_printassocdefobj(
	OV_OVM_LIBRARY_DEF		*plib,
	OV_OVM_ASSOCIATION_DEF	*passoc,
	FILE					*fp
);

/*
*	Print definition object associated with a member of a structure
*/
void ov_codegen_printmemberdefobj(
	OV_OVM_LIBRARY_DEF		*plib,
	OV_OVM_STRUCTURE_DEF	*pstruct,
	OV_OVM_VARIABLE_DEF		*pvar,
	OV_UINT					i,
	FILE					*fp
);

/*
*	Print definition object associated with a variable of a class
*/
void ov_codegen_printvardefobj(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	OV_OVM_VARIABLE_DEF	*pvar,
	OV_UINT				i,
	FILE				*fp
);

/*
*	Print definition object associated with a part of a class
*/
void ov_codegen_printpartdefobj(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	OV_OVM_PART_DEF		*ppart,
	OV_UINT				i,
	FILE				*fp
);

/*
*	Print definition object associated with an operation of a class
*/
void ov_codegen_printopdefobj(
	OV_OVM_LIBRARY_DEF		*plib,
	OV_OVM_CLASS_DEF		*pclass,
	OV_OVM_OPERATION_DEF	*pop,
	OV_UINT					i,
	FILE					*fp
);

/*
*	Return variable type name defined in ov/ov.h for a given vartype
*/
OV_STRING ov_codegen_getvartypetext(
	OV_VAR_TYPE	vartype
);

/*
*	Return text defining a string
*/
OV_STRING ov_codegen_getstringtext(
	OV_STRING	string
);

/*
*	Replace '/' with '_' in an identifier
*/
OV_STRING ov_codegen_replace(
	OV_STRING	identifier
);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif
/*
*	End of file
*/

