/*
*   $Id: ov_ovmparser.y,v 1.4 1999-07-29 16:32:22 dirk Exp $
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
*	15-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/

/*
*   bison C declarations
*   --------------------
*/
%{
#include "ov_codegen.h"

/*
*	Global variables
*/
OV_OVM_LIBRARY_DEF	*libraries = NULL;
OV_UINT				defnum = 0;
%}

/*
*   bison definitions
*   ------------------
*/

%union {
	OV_STRING				string;
	OV_UINT					uint;
	OV_CLASS_PROPS			classprops;
	OV_ASSOC_PROPS			assocprops;
	OV_VAR_TYPE				vartype;
	OV_VAR_PROPS			varprops;
	OV_OP_PROPS				opprops;
	OV_ASSOC_TYPE			assoctype;
	OV_OVM_LIBRARY_DEF		*plibdef;
	OV_OVM_STRUCTURE_DEF	*pstructdef;
	OV_OVM_CLASS_DEF		*pclassdef;
	OV_OVM_ASSOCIATION_DEF	*passocdef;
	OV_OVM_VARIABLE_DEF		*pvardef;
	OV_OVM_PART_DEF			*ppartdef;
	OV_OVM_OPERATION_DEF	*popdef;
	OV_OVM_VARTYPE_DEF		*pvartypedef;
}

%token TOK_LIBRARY TOK_VERSION TOK_AUTHOR TOK_COPYRIGHT TOK_COMMENT TOK_END_LIBRARY

%token TOK_STRUCTURE TOK_END_STRUCTURE

%token TOK_CLASS TOK_VARIABLES TOK_END_VARIABLES TOK_OPERATIONS TOK_END_OPERATIONS
%token TOK_PARTS TOK_END_PARTS TOK_END_CLASS

%token TOK_ASSOCIATION TOK_PARENT TOK_CHILD TOK_END_ASSOCIATION

%token TOK_C_TYPE TOK_UNIT

%token TOK_C_FUNCTION TOK_IS_ABSTRACT

%token <classprops>	TOK_CLASSPROPS

%token <assocprops>	TOK_ASSOCPROPS

%token <opprops>	TOK_OPPROPS

%token <vartype>	TOK_VARTYPE
%token <varprops>	TOK_VARPROPS
%token <assoctype>	TOK_ASSOCTYPE

%token <uint>		TOK_FLAGS

%token <string>		TOK_STRING
%token <string>		TOK_IDENTIFIER
%token <string>		TOK_IDENTIFIER_EX
%token <string>		TOK_C_IDENTIFIER
%token <uint>		TOK_UINT

%type <plibdef>		libraries library
%type <string>		version_opt author_opt copyright_opt

%type <pstructdef>	structures_opt structures structure

%type <pclassdef>	classes_opt classes class
%type <string>		baseclass_opt
%type <classprops>	classprops_opt classprops
%type <uint>		flags_opt
%type <pvardef>		variables_block_opt variables_opt
%type <ppartdef>	parts_block_opt parts_opt
%type <popdef>		operations_block_opt operations_opt

%type <passocdef>	associations_opt associations association
%type <assoctype>	assoctype_opt
%type <assocprops>	assocprops_opt assocprops

%type <pvardef>		members member

%type <pvardef>		variables variable
%type <varprops>	varprops_opt varprops

%type <ppartdef>	parts part

%type <popdef>		operations operation
%type <opprops>		opprops_opt opprops

%type <string>		comment_opt
%type <uint>		vector_length_opt length_opt
%type <pvartypedef> vartype
%type <string>		unit_opt varcomment_opt assoccomment_opt
%type <uint>		varflags_opt assocflags_opt

%%

/*
*   bison grammar rules
*   -------------------
*/

start:
	libraries
		{
			libraries = $1;
		}
;

/*	----------------------------------------------------------------------	*/

/*
*	Library definition
*/

libraries:
	library
		{
			$$ = $1;
		}
	| libraries library
		{
			Ov_Codegen_InsertDef(LIBRARY, $$, $1, $2);
		}
;

library:
	TOK_LIBRARY TOK_IDENTIFIER
	version_opt
	author_opt
	copyright_opt
	comment_opt
	structures_opt
	classes_opt
	associations_opt
	TOK_END_LIBRARY ';'
		{
			OV_OVM_STRUCTURE_DEF	*pstruct;
			OV_OVM_CLASS_DEF		*pclass;
			OV_OVM_ASSOCIATION_DEF	*passoc;
			for(pstruct=$7; pstruct; pstruct=pstruct->pnext) {
				pstruct->libname = $2;
			}
			for(pclass=$8; pclass; pclass=pclass->pnext) {
				pclass->libname = $2;
			}
			for(passoc=$9; passoc; passoc=passoc->pnext) {
				passoc->libname = $2;
			}
			$$ = Ov_Codegen_Malloc(OV_OVM_LIBRARY_DEF);
			$$->pnext = NULL;
			$$->identifier = $2;
			$$->version = $3;
			$$->author = $4;
			$$->copyright = $5;
			$$->comment = $6;
			$$->structures = $7;
			$$->classes = $8;
			$$->associations = $9;
		}
;

version_opt:
	/* empty */
		{
			$$ = NULL;
		}
	| TOK_VERSION '=' TOK_STRING ';'
		{
			$$ = $3;
		}
;

author_opt:
	/* empty */
		{
			$$ = NULL;
		}
	| TOK_AUTHOR '=' TOK_STRING ';'
		{
			$$ = $3;
		}
;

copyright_opt:
	/* empty */
		{
			$$ = NULL;
		}
	| TOK_COPYRIGHT '=' TOK_STRING ';'
		{
			$$ = $3;
		}
;

/*	----------------------------------------------------------------------	*/

/*
*	Structure definition
*/

structures_opt:
	/* empty */
		{
			$$ = NULL;
		}
	| structures
		{
			$$ = $1;
		}
;

structures:
	structure
		{
			$$ = $1;
		}
	| structures structure
		{
			Ov_Codegen_InsertDef(STRUCTURE, $$, $1, $2);
		}
;

structure:
	TOK_STRUCTURE TOK_IDENTIFIER
	members
	TOK_END_STRUCTURE ';'
		{
			$$ = Ov_Codegen_Malloc(OV_OVM_STRUCTURE_DEF);
			$$->pnext = NULL;
			$$->identifier = $2;
			$$->members = $3;
			$$->defnum = defnum++;
		}
;

/*	----------------------------------------------------------------------	*/

/*
*	Class definition
*/

classes_opt:
	/* emtpy */
		{
			$$ = NULL;
		}
	| classes
		{
			$$ = $1;
		}
;

classes:
	class
		{
			$$ = $1;
		}
	| classes class
		{
			Ov_Codegen_InsertDef(CLASS, $$, $1, $2);
		}
;

class:
	TOK_CLASS TOK_IDENTIFIER baseclass_opt
	classprops_opt
	flags_opt
	comment_opt
	variables_block_opt
	parts_block_opt
	operations_block_opt
	TOK_END_CLASS ';'
		{
			$$ = Ov_Codegen_Malloc(OV_OVM_CLASS_DEF);
			$$->pnext = NULL;
			$$->identifier = $2;
			$$->baseclassname = $3;
			$$->baseclasslibname = NULL;
			$$->classprops = $4;
			$$->flags = $5;
			$$->comment = $6;
			$$->variables = $7;
			$$->parts = $8;
			$$->operations = $9;
			$$->numoperations = 0;
			$$->defnum = defnum++;
		}
;

baseclass_opt:
	/* empty */
		{
		 	$$ = NULL;
		}
	| ':' TOK_CLASS TOK_IDENTIFIER_EX
		{
			$$ = $3;
		}
;

classprops_opt:
	/* empty */
		{
			$$ = 0;
		}
	| classprops
		{
			$$ = $1;
		}
;

classprops:
	TOK_CLASSPROPS ';'
		{
			$$ = $1;
		}
	| classprops TOK_CLASSPROPS ';'
		{
			$$ = $1 | $2;
		}
;

flags_opt:
	/* empty */
		{
			$$ = 0;
		}
	| TOK_FLAGS ';'
		{
			$$ = $1;
		}
;

variables_block_opt:
	/* empty */
		{
			$$ = NULL;
		}
	| TOK_VARIABLES variables_opt TOK_END_VARIABLES ';'
		{
			$$ = $2;
		}
;

variables_opt:
	/* empty */
		{
			$$ = NULL;
		}
	| variables
		{
			$$ = $1;
		}
;

parts_block_opt:
	/* empty */
		{
			$$ = NULL;
		}
	| TOK_PARTS parts_opt TOK_END_PARTS ';'
		{
			$$ = $2;
		}
;

parts_opt:
	/* empty */
		{
			$$ = NULL;
		}
	| parts
		{
			$$ = $1;
		}
;

operations_block_opt:
	/* empty */
		{
			$$ = NULL;
		}
	| TOK_OPERATIONS operations_opt TOK_END_OPERATIONS ';'
		{
			$$ = $2;
		}
;
operations_opt:
	/* empty */
		{
			$$ = NULL;
		}
	| operations
		{
			$$ = $1;
		}
;

/*	----------------------------------------------------------------------	*/

/*
*	Association definition
*/

associations_opt:
	/* emtpy */
		{
			$$ = NULL;
		}
	| associations
		{
			$$ = $1;
		}
;

associations:
	association
		{
			$$ = $1;
		}
	| associations association
		{
			Ov_Codegen_InsertDef(ASSOCIATION, $$, $1, $2);
		}
;

association:
	TOK_ASSOCIATION TOK_IDENTIFIER assoctype_opt
	assocprops_opt
	TOK_PARENT TOK_IDENTIFIER ':' TOK_CLASS TOK_IDENTIFIER_EX assocflags_opt assoccomment_opt ';'
	TOK_CHILD TOK_IDENTIFIER ':' TOK_CLASS TOK_IDENTIFIER_EX assocflags_opt assoccomment_opt ';'
	TOK_END_ASSOCIATION ';'
		{
			$$ = Ov_Codegen_Malloc(OV_OVM_ASSOCIATION_DEF);
			$$->pnext = NULL;
			$$->identifier = $2;
			$$->assoctype = $3;
			$$->assocprops = $4;
			$$->parentrolename = $6;
			$$->parentclassname = $9;
			$$->anchorflags = $10;
			$$->anchorcomment = $11;
			$$->childrolename = $14;
			$$->childclassname = $17;
			$$->headflags = $18;
			$$->headcomment = $19;
			$$->defnum = defnum++;
		}
;

assoctype_opt:
	/* empty */
		{
			$$ = OV_AT_ORDERED_LIST;
		}
	| ':' TOK_ASSOCTYPE
		{
			$$ = $2;
		}
;

assocprops_opt:
	/* empty */
		{
			$$ = 0;
		}
	| assocprops
		{
			$$ = $1;
		}
;

assocprops:
	TOK_ASSOCPROPS ';'
		{
			$$ = $1;
		}
	| assocprops TOK_ASSOCPROPS ';'
		{
			$$ = $1 | $2;
		}
;

/*	----------------------------------------------------------------------	*/

/*
*	Member definition of a structure
*/

members:
	member
		{
			$$ = $1;
		}
	| members member
		{
			Ov_Codegen_InsertDef(VARIABLE, $$, $1, $2);
		}
;

member:
	TOK_IDENTIFIER vector_length_opt ':' vartype varflags_opt unit_opt varcomment_opt ';'
		{
			$$ = Ov_Codegen_Malloc(OV_OVM_VARIABLE_DEF);
			$$->pnext = NULL;
			$$->identifier = $1;
			$$->veclen = $2;
			if($$->veclen == 1) {
				$$->vartype = $4->vartype;
			} else {
				$$->vartype = $4->vartype + OV_VT_BOOL_VEC - OV_VT_BOOL;
			}
			$$->ctypename = $4->ctypename;
			$$->structurename = $4->structurename;
			$$->structurelibname = NULL;
			$$->varprops = OV_AC_NONE;
			$$->flags = $5;
			$$->tech_unit = $6;
			$$->comment = $7;
			ov_codegen_free($4);
		}
;

/*	----------------------------------------------------------------------	*/

/*
*	Variable definition of a class
*/

variables:
	variable
		{
			$$ = $1;
		}
	| variables variable
		{
			Ov_Codegen_InsertDef(VARIABLE, $$, $1, $2);
		}
;

variable:
	TOK_IDENTIFIER vector_length_opt ':' vartype varprops_opt varflags_opt unit_opt varcomment_opt ';'
		{
			$$ = Ov_Codegen_Malloc(OV_OVM_VARIABLE_DEF);
			$$->pnext = NULL;
			$$->identifier = $1;
			$$->veclen = $2;
			if($$->veclen == 1) {
				$$->vartype = $4->vartype;
			} else {
				$$->vartype = $4->vartype + OV_VT_BOOL_VEC - OV_VT_BOOL;
			}
			$$->ctypename = $4->ctypename;
			$$->structurename = $4->structurename;
			$$->structurelibname = NULL;
			$$->varprops = $5;
			$$->flags = $6;
			$$->tech_unit = $7;
			$$->comment = $8;
			ov_codegen_free($4);
		}
;

varprops_opt:
	/* empty */
		{
			$$ = OV_AC_NONE;
		}
	| varprops
		{
			$$ = $1;
		}
;

varprops:
	TOK_VARPROPS
		{
			$$ = $1;
		}
	| varprops TOK_VARPROPS
		{
			$$ = $1 | $2;
		}
;

/*	----------------------------------------------------------------------	*/

/*
*	Part definition of a class
*/

parts:
	part
		{
			$$ = $1;
		}
	| parts part
		{
			Ov_Codegen_InsertDef(PART, $$, $1, $2);
		}
;

part:
	TOK_IDENTIFIER ':' TOK_CLASS TOK_IDENTIFIER_EX ';'
		{
			$$ = Ov_Codegen_Malloc(OV_OVM_PART_DEF);
			$$->pnext = NULL;
			$$->identifier = $1;
			$$->partclassname = $4;
			$$->partclasslibname = NULL;
		}
;

/*	----------------------------------------------------------------------	*/

/*
*	Operation definition of a class
*/

operations:
	operation
		{
			$$ = $1;
		}
	| operations operation
		{
			Ov_Codegen_InsertDef(OPERATION, $$, $1, $2);
		}
;

operation:
	TOK_IDENTIFIER ':' TOK_C_FUNCTION TOK_C_IDENTIFIER opprops_opt ';'
		{
			$$ = Ov_Codegen_Malloc(OV_OVM_OPERATION_DEF);
			$$->pnext = NULL;
			$$->identifier = $1;
			$$->cfnctypename = $4;
			$$->opprops = $5;
			$$->overloaded = FALSE;
			$$->operationnum = 0;
			$$->classname = NULL;
		}
;

opprops_opt:
	/* empty */
		{
			$$ = 0;
		}
	| opprops
		{
			$$ = $1;
		}
;

opprops:
	TOK_OPPROPS
		{
			$$ = $1;
		}
	| opprops TOK_OPPROPS
		{
			$$ = $1 | $2;
		}
;

/*	----------------------------------------------------------------------	*/

/*
*	Common definitions
*/

comment_opt:
	/* empty */
		{
			$$ = NULL;
		}
	| TOK_COMMENT '=' TOK_STRING ';'
		{
			$$ = $3;
		}
;

vector_length_opt:
	/* empty */
		{
			$$ = 1;
		}
	| '[' length_opt ']'
		{
			$$ = $2;
		}
;

length_opt:
	/* empty */
		{
			$$ = 0;
		}
	| TOK_UINT
		{
			$$ = $1;
		}
;

vartype:
	TOK_VARTYPE
		{
			$$ = Ov_Codegen_Malloc(OV_OVM_VARTYPE_DEF);
			$$->vartype = $1;
			$$->structurename = NULL;
			$$->ctypename = NULL;
		}
	| TOK_STRUCTURE TOK_IDENTIFIER_EX
		{
			$$ = Ov_Codegen_Malloc(OV_OVM_VARTYPE_DEF);
			$$->vartype = OV_VT_STRUCT;
			$$->structurename = $2;
			$$->ctypename = NULL;
		}
	| TOK_C_TYPE TOK_C_IDENTIFIER
		{
			$$ = Ov_Codegen_Malloc(OV_OVM_VARTYPE_DEF);
			$$->vartype = OV_VT_BYTE_VEC;
			$$->structurename = NULL;
			$$->ctypename = $2;
		}

varflags_opt:
	/* empty */
		{
			$$ = 0;
		}
	| TOK_FLAGS
		{
			$$ = $1;
		}
;

assocflags_opt:
	varflags_opt
		{
			$$ = $1;
		}
;

unit_opt:
	/* empty */
		{
			$$ = NULL;
		}
	| TOK_UNIT '=' TOK_STRING
		{
			$$ = $3;
		}
;

varcomment_opt:
	/* empty */
		{
			$$ = NULL;
		}
	| TOK_COMMENT '=' TOK_STRING
		{
			$$ = $3;
		}
;

assoccomment_opt:
	varcomment_opt
		{
			$$ = $1;
		}
;

%%

/*
*   supporting C functions
*   ----------------------
*/

/*	----------------------------------------------------------------------	*/

/*
*   Print out an error message
*/
int yyerror(char *msg)
{
	int i;
	if(include_stack_ptr > 0) {
		fprintf(stderr, "In file");
		for(i=0; i<include_stack_ptr; i++) {
			fprintf(stderr, "\tincluded from %s:\n", filename_stack[i]);
		}
	}
	fprintf(stderr, "%s:%ld: %s\n", filename, current_line, msg);
	return EXIT_FAILURE;
}

/*	----------------------------------------------------------------------	*/

/*
*	Check overall semantics
*/
OV_BOOL ov_codegen_checksemantics(void) {
	/*
	*	local variables
	*/
	OV_BOOL				result = TRUE;
	OV_OVM_LIBRARY_DEF	*plib;
	/*
	*	check all libraries
	*/
	for(plib=libraries; plib; plib=plib->pnext) {
		if(!ov_codegen_checksemantics_library(plib)) {
			result = FALSE;
		}
	}
	/*
	*	semantic check is finished
	*/
	return result;
}

/*	----------------------------------------------------------------------	*/

/*
*	Check semantics of a library
*/
OV_BOOL ov_codegen_checksemantics_library(
	OV_OVM_LIBRARY_DEF	*plib
) {
	/*
	*	local variables
	*/
	OV_BOOL					result = TRUE;
	OV_OVM_STRUCTURE_DEF	*pstruct;
	OV_OVM_CLASS_DEF		*pclass;
	OV_OVM_ASSOCIATION_DEF	*passoc;
	/*
	*	check for unique name
	*/
	if(plib != ov_codegen_getlibdef(plib->identifier)) {
		fprintf(stderr, "library \"%s\": identifier already used.\n",
			plib->identifier);
		result = FALSE;
	}
	/*
	*	check structure, class and association definitions
	*/
	for(pstruct=plib->structures; pstruct; pstruct=pstruct->pnext) {
		if(!ov_codegen_checksemantics_structure(plib, pstruct)) {
			result = FALSE;
		}
	}
	for(pclass=plib->classes; pclass; pclass=pclass->pnext) {
		if(!ov_codegen_checksemantics_class(plib, pclass)) {
			result = FALSE;
		}
	}
	for(passoc=plib->associations; passoc; passoc=passoc->pnext) {
		if(!ov_codegen_checksemantics_association(plib, passoc)) {
			result = FALSE;
		}
	}
	/*
	*	semantic check is finished
	*/
	return result;
}

/*	----------------------------------------------------------------------	*/

/*
*	Check semantics of a structure
*/
OV_BOOL ov_codegen_checksemantics_structure(
	OV_OVM_LIBRARY_DEF		*plib,
	OV_OVM_STRUCTURE_DEF	*pstruct
) {
	/*
	*	local variables
	*/
	OV_BOOL				result = TRUE;
	OV_OVM_VARIABLE_DEF	*pvar;
	/*
	*	check for unique name
	*/
	if(pstruct != ov_codegen_getstructdef(plib, pstruct->identifier)) {
		fprintf(stderr, "structure \"%s\": identifier already used.\n",
			pstruct->identifier);
		result = FALSE;
	}
	/*
	*	check member definitions
	*/
	for(pvar=pstruct->members; pvar; pvar=pvar->pnext) {
		if(!ov_codegen_checksemantics_member(plib, pstruct, pvar)) {
			result = FALSE;
		}
	}
	/*
	*	semantic check is finished
	*/
	return result;
}

/*	----------------------------------------------------------------------	*/

/*
*	Check semantics of a class
*/
OV_BOOL ov_codegen_checksemantics_class(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass
) {
	/*
	*	local variables
	*/
	OV_BOOL					result = TRUE;
	OV_OVM_CLASS_DEF		*pbaseclass;
	OV_OVM_VARIABLE_DEF		*pvar;
	OV_OVM_PART_DEF			*ppart;
	OV_OVM_OPERATION_DEF	*pop;
	OV_UINT					i;
	OV_BOOL					instantiable = TRUE;
	/*
	*	check for unique name
	*/
	if(pclass != ov_codegen_getclassdef(plib, pclass->identifier)
		|| ov_codegen_getstructdef(plib, pclass->identifier)
	) {
		fprintf(stderr, "class \"%s\": identifier already used.\n",
			pclass->identifier);
		result = FALSE;
	}
	/*
	*	check for base class and initialize count of operations
	*/
	if(pclass->baseclassname) {
		pbaseclass = ov_codegen_getclassdef(NULL, pclass->baseclassname);
		if(pbaseclass) {
			pclass->baseclasslibname = pbaseclass->libname;
			/*
			*	the base class must not be final
			*/
			if(pbaseclass->classprops & OV_CP_FINAL) {
				fprintf(stderr, "class \"%s\": class may not inherit from a final class.\n",
					pclass->identifier);
				result = FALSE;
			}
			/*
			*	the base class has to be defined prior to the derived classes
			*/
			if(pclass->defnum == pbaseclass->defnum) {
				fprintf(stderr, "class \"%s\": class may not inherit from itself.\n",
					pclass->identifier);
				result = FALSE;
			} else if(pclass->defnum < pbaseclass->defnum) {
				fprintf(stderr, "class \"%s\": definition prior to definition "
					"of base class \"%s\".\n", pclass->identifier,
					pclass->baseclassname);
				result = FALSE;
			} else {
				/*
				*	initialize count of operations
				*/
				pclass->numoperations = pbaseclass->numoperations;
			}
		} else {
			fprintf(stderr, "class \"%s\": base class \"%s\" not defined.\n",
				pclass->identifier, pclass->baseclassname);
			result = FALSE;
		}
	} else {
		/*
		*	class has no base class; this is only possible for
		*	the top level class "object"
		*/
		if(strcmp(pclass->identifier, OV_OBJNAME_OBJECT)) {
			fprintf(stderr, "class \"%s\": not derived from a base class.\n",
				pclass->identifier);
			result = FALSE;
		}
	}
	/*
	*	check variable, part and operation definitions
	*/
	for(pvar=pclass->variables; pvar; pvar=pvar->pnext) {
		if(!ov_codegen_checksemantics_variable(plib, pclass, pvar)) {
			result = FALSE;
		}
	}
	for(ppart=pclass->parts; ppart; ppart=ppart->pnext) {
		if(!ov_codegen_checksemantics_part(plib, pclass, ppart)) {
			result = FALSE;
		}
	}
	for(pop=pclass->operations; pop; pop=pop->pnext) {
		if(!ov_codegen_checksemantics_operation(plib, pclass, pop)) {
			result = FALSE;
		}
	}
	/*
	*	check if class can be instantiated -- that is, if there
	*	are no abstract operations anymore
	*/
	for(i=0; i<pclass->numoperations; i++) {
		pop = ov_codegen_getopdefbynum(pclass, i);
		if(pop->opprops & OV_OP_ABSTRACT) {
			instantiable = FALSE;
		}
	}
	if((pclass->classprops & OV_CP_INSTANTIABLE) && !instantiable) {
		fprintf(stderr, "class \"%s\": class has abstract operations "
			"and cannot be instantiated.\n", pclass->identifier);
		result = FALSE;
	}
	/*
	*	semantic check is finished
	*/
	return result;
}

/*	----------------------------------------------------------------------	*/

/*
*	Check semantics of an association
*/
OV_BOOL ov_codegen_checksemantics_association(
	OV_OVM_LIBRARY_DEF		*plib,
	OV_OVM_ASSOCIATION_DEF	*passoc
) {
	/*
	*	local variables
	*/
	OV_BOOL				result = TRUE;
	OV_OVM_CLASS_DEF	*pparentclass = ov_codegen_getclassdef(NULL, passoc->parentclassname);
	OV_OVM_CLASS_DEF	*pchildclass = ov_codegen_getclassdef(NULL, passoc->childclassname);
	/*
	*	check for unique name
	*/
	if(passoc != ov_codegen_getassocdef(plib, passoc->identifier)
		|| ov_codegen_getstructdef(plib, passoc->identifier)
		|| ov_codegen_getclassdef(plib, passoc->identifier)
	) {
		fprintf(stderr, "association \"%s\": identifier already used.\n",
			passoc->identifier);
		result = FALSE;
	}
	/*
	*	check for child and parent class and their role names
	*/
	if(!pparentclass) {
		fprintf(stderr, "association \"%s\": parent class \"%s\" not defined.\n",
			passoc->identifier, passoc->parentclassname);
		result = FALSE;
	} else {
		if(pparentclass->defnum > passoc->defnum) {
			fprintf(stderr, "association \"%s\": definition prior to definition "
				"of parent class \"%s\".\n", passoc->identifier,
				passoc->parentclassname);
			result = FALSE;
		}
		if(pparentclass->libname != passoc->libname) {
			fprintf(stderr, "association \"%s\": definition not in same library "
				"as parent class \"%s\".\n", passoc->identifier,
				passoc->parentclassname);
			result = FALSE;
		}
		/*
		*	check child role name -- yes, CHILD rolename
		*/
		if(ov_codegen_getvardef(plib, pparentclass, passoc->childrolename)
			|| ov_codegen_getpartdef(plib, pparentclass, passoc->childrolename)
			|| ov_codegen_getopdef(plib, pparentclass, passoc->childrolename)
		) {
			fprintf(stderr, "association \"%s\": child role name \"%s\" already "
				"used as identifier.", passoc->identifier, passoc->childrolename);
			result = FALSE;
		}
	}
	if(!pchildclass) {
		fprintf(stderr, "association \"%s\": child class \"%s\" not defined.\n",
			passoc->identifier, passoc->childclassname);
		result = FALSE;
	} else {
		if(pchildclass->defnum > passoc->defnum) {
			fprintf(stderr, "association \"%s\": definition prior to definition "
				"of child class \"%s\".\n", passoc->identifier,
				passoc->childclassname);
			result = FALSE;
		}
		if(pchildclass->libname != passoc->libname) {
			fprintf(stderr, "association \"%s\": definition not in same library "
				"as child class \"%s\".\n", passoc->identifier,
				passoc->childclassname);
			result = FALSE;
		}
		/*
		*	check parent role name -- yes, PARENT rolename
		*/
		if(ov_codegen_getvardef(plib, pparentclass, passoc->parentrolename)
			|| ov_codegen_getpartdef(plib, pparentclass, passoc->parentrolename)
			|| ov_codegen_getopdef(plib, pparentclass, passoc->parentrolename)
		) {
			fprintf(stderr, "association \"%s\": parent role name \"%s\" already "
				"used as identifier.", passoc->identifier, passoc->parentrolename);
			result = FALSE;
		}
	}
	/*
	*	semantic check is finished
	*/
	return result;
}

/*	----------------------------------------------------------------------	*/

/*
*	Check semantics of a member of a structure
*/
OV_BOOL ov_codegen_checksemantics_member(
	OV_OVM_LIBRARY_DEF		*plib,
	OV_OVM_STRUCTURE_DEF	*pstruct,
	OV_OVM_VARIABLE_DEF		*pvar
) {
	/*
	*	local variables
	*/
	OV_BOOL					result = TRUE;
	OV_OVM_STRUCTURE_DEF	*pvarstruct;
	/*
	*	check for unique name
	*/
	if(pvar != ov_codegen_getmemberdef(plib, pstruct, pvar->identifier)) {
		fprintf(stderr, "structure \"%s\", member \"%s\": duplicate identifier.\n",
			pstruct->identifier, pvar->identifier);
		result = FALSE;
	}
	/*
	*	check vector types
	*/
	if(pvar->veclen != 1) {
		/*
		*	PV vectors are not supported
		*/
		if(pvar->vartype & (OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP)) {
			fprintf(stderr, "structure \"%s\", variable \"%s\": vectors of PV variables "
				"are not supported.\n", pstruct->identifier, pvar->identifier);
			result = FALSE;
		}
		/*
		*	vectors of structures are not (yet) supported
		*/
		if(pvar->structurename) {
			fprintf(stderr, "structure \"%s\", variable \"%s\": vectors of structures "
				"are not supported.\n", pstruct->identifier, pvar->identifier);
			result = FALSE;
		}
		/*
		*	dynamic vectors of C-type variables are not supported
		*/
		if((!pvar->veclen) && (pvar->vartype == OV_VT_BYTE_VEC)) {
			fprintf(stderr, "structure \"%s\", variable \"%s\": dynamic vectors of C-type "
				"variables are not supported.\n", pstruct->identifier, pvar->identifier);
			result = FALSE;
		}
	}
	/*
	*	if member is a structure, check the structure definition
	*/
	if(pvar->structurename) {
		pvarstruct = ov_codegen_getstructdef(plib, pvar->structurename);
		if(pvarstruct) {
			pvar->structurelibname = pvarstruct->libname;
			if(pvarstruct->defnum == pstruct->defnum) {
				fprintf(stderr, "structure \"%s\", member \"%s\": recursive definition.\n",
					pstruct->identifier, pvar->identifier);
				result = FALSE;
			} else if(pvarstruct->defnum > pstruct->defnum) {
				fprintf(stderr, "structure \"%s\", member \"%s\": definition prior to "
					"definition of structure \"%s\".\n", pstruct->identifier,
					pvar->identifier, pvar->structurename);
				result = FALSE;
			}
		} else {
			fprintf(stderr, "structure \"%s\", member \"%s\": structure \"%s\" not "
				"defined.\n", pstruct->identifier, pvar->identifier, pvar->structurename);
			result = FALSE;
		}
	}
	/*
	*	semantic check is finished
	*/
	return result;
}

/*	----------------------------------------------------------------------	*/

/*
*	Check semantics of a variable of a class
*/
OV_BOOL ov_codegen_checksemantics_variable(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	OV_OVM_VARIABLE_DEF	*pvar
) {
	/*
	*	local variables
	*/
	OV_BOOL					result = TRUE;
	OV_OVM_STRUCTURE_DEF	*pvarstruct;
	/*
	*	check for unique name
	*/
	if(pvar != ov_codegen_getvardef(plib, pclass, pvar->identifier)) {
		fprintf(stderr, "class \"%s\", variable \"%s\": duplicate identifier.\n",
			pclass->identifier, pvar->identifier);
		result = FALSE;
	}
	/*
	*	check vector types
	*/
	if(pvar->veclen != 1) {
		/*
		*	PV vectors are not supported
		*/
		if(pvar->vartype & (OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP)) {
			fprintf(stderr, "class \"%s\", variable \"%s\": vectors of PV variables "
				"are not supported.\n", pclass->identifier, pvar->identifier);
			result = FALSE;
		}
		/*
		*	vectors of structures are not (yet) supported
		*/
		if(pvar->structurename) {
			fprintf(stderr, "class \"%s\", variable \"%s\": vectors of structures "
				"are not supported.\n", pclass->identifier, pvar->identifier);
			result = FALSE;
		}
		/*
		*	dynamic vectors of C-type variables are not supported
		*/
		if((!pvar->veclen) && (pvar->vartype == OV_VT_BYTE_VEC)) {
			fprintf(stderr, "class \"%s\", variable \"%s\": dynamic vectors of C-type "
				"variables are not supported.\n", pclass->identifier, pvar->identifier);
			result = FALSE;
		}
	}
	/*
	*	virtual variables need to have get and/or set accessors
	*/
	if((pvar->varprops & OV_VP_DERIVED) && !(pvar->varprops & OV_VP_ACCESSORS)) {
		fprintf(stderr, "class \"%s\", variable \"%s\": virtual variables must "
			"have get and/or set accessors.\n", pclass->identifier, pvar->identifier);
		result = FALSE;
	}
	/*
	*	if variable is a structure, check the structure definition
	*/
	if(pvar->structurename) {
		pvarstruct = ov_codegen_getstructdef(plib, pvar->structurename);
		if(pvarstruct) {
			pvar->structurelibname = pvarstruct->libname;
			if(pvarstruct->defnum > pclass->defnum) {
				fprintf(stderr, "class \"%s\", variable \"%s\": definition prior to "
					"definition of structure \"%s\".\n", pclass->identifier,
					pvar->identifier, pvar->structurename);
				result = FALSE;
			}
		} else {
			fprintf(stderr, "class \"%s\", variable \"%s\": structure \"%s\" not "
				"defined.\n", pclass->identifier, pvar->identifier, pvar->structurename);
			result = FALSE;
		}
	}
	/*
	*	semantic check is finished
	*/
	return result;
}

/*	----------------------------------------------------------------------	*/

/*
*	Check semantics of a part of a class
*/
OV_BOOL ov_codegen_checksemantics_part(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	OV_OVM_PART_DEF		*ppart
) {
	/*
	*	local variables
	*/
	OV_BOOL				result = TRUE;
	OV_OVM_CLASS_DEF	*ppartclass;
	/*
	*	check for unique name
	*/
	if(ppart != ov_codegen_getpartdef(plib, pclass, ppart->identifier)
		|| ov_codegen_getvardef(plib, pclass, ppart->identifier)
	) {
		fprintf(stderr, "class \"%s\", part \"%s\": duplicate identifier.\n",
			pclass->identifier, ppart->identifier);
		result = FALSE;
	}
	/*
	*	check class definition of the part
	*/
	ppartclass = ov_codegen_getclassdef(NULL, ppart->partclassname);
	if(ppartclass) {
		ppart->partclasslibname = ppartclass->libname;
		if(ppartclass->defnum == pclass->defnum) {
			fprintf(stderr, "class \"%s\", part \"%s\": recursive definition.\n",
				pclass->identifier, ppart->identifier);
			result = FALSE;
		} else if(ppartclass->defnum > pclass->defnum) {
			fprintf(stderr, "class \"%s\", part \"%s\": definition prior to "
				"definition of class \"%s\".\n", pclass->identifier,
				ppart->identifier, ppart->partclassname);
			result = FALSE;
		}
		if(!(ppartclass->classprops & OV_CP_INSTANTIABLE)) {
			fprintf(stderr, "class \"%s\", part \"%s\": class \"%s\" is not "
				"instantiable.\n", pclass->identifier, ppart->identifier,
				ppart->partclassname);
			result = FALSE;
		}
	} else {
		fprintf(stderr, "class \"%s\", part \"%s\": class \"%s\" not "
			"defined.\n", pclass->identifier, ppart->identifier, ppart->partclassname);
		result = FALSE;
	}
	/*
	*	semantic check is finished
	*/
	return result;
}

/*	----------------------------------------------------------------------	*/

/*
*	Check semantics of an operation of a class
*/
OV_BOOL ov_codegen_checksemantics_operation(
	OV_OVM_LIBRARY_DEF		*plib,
	OV_OVM_CLASS_DEF		*pclass,
	OV_OVM_OPERATION_DEF	*pop
) {
	/*
	*	local variables
	*/
	OV_BOOL					result = TRUE;
	OV_OVM_CLASS_DEF		*pbaseclass;
	OV_OVM_OPERATION_DEF	*pop2;
	/*
	*	set the classname of the operation
	*/
	pop->classname = ov_codegen_getstring("", strlen(plib->identifier)+1
		+strlen(pclass->identifier));
	sprintf(pop->classname, "%s/%s", plib->identifier, pclass->identifier);
	/*
	*	check if operation overload a base class operation
	*/
	pbaseclass = ov_codegen_getclassdef(NULL, pclass->baseclassname);
	if(pbaseclass) {
		pop2 = ov_codegen_getopdef(plib, pbaseclass, pop->identifier);
		if(pop2) {
			/*
			*	operation overloads an operation of a base class
			*/
			pop->overloaded = TRUE;
			pop->operationnum = pop2->operationnum;
			if(strcmp(pop->cfnctypename, pop2->cfnctypename)) {
				fprintf(stderr, "class \"%s\", operation \"%s\": overloading with "
					"different C-function prototype forbidden.\n", pclass->identifier, pop->identifier);
				result = FALSE;
			}
		}
	}
	if(!pop->overloaded) {
		pop->operationnum = pclass->numoperations;
		pclass->numoperations++;
	}
	/*
	*	check for unique name
	*/
	for(pop2=pclass->operations; pop2; pop2=pop2->pnext) {
		if(pop != pop2) {
			if(!strcmp(pop->identifier, pop2->identifier)) {
				fprintf(stderr, "class \"%s\", operation \"%s\": duplicate identifier.\n",
					pclass->identifier, pop->identifier);
				result = FALSE;
			}
		}
	}
	if(ov_codegen_getvardef(plib, pclass, pop->identifier)
		|| ov_codegen_getpartdef(plib, pclass, pop->identifier)
	) {
		fprintf(stderr, "class \"%s\", operation \"%s\": duplicate identifier.\n",
			pclass->identifier, pop->identifier);
		result = FALSE;
	}
	/*
	*	it is not allowed to overload operations with abstract operations
	*/
	if(pop->overloaded && (pop->opprops & OV_OP_ABSTRACT)) {
		fprintf(stderr, "class \"%s\", operation \"%s\": cannot overload operation "
			"with an abstract operation.\n", pclass->identifier, pop->identifier);
		result = FALSE;
	}
	/*
	*	semantic check is finished
	*/
	return result;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get pointer to library definition
*/
OV_OVM_LIBRARY_DEF *ov_codegen_getlibdef(
	OV_STRING	identifier
) {
	/*
	*	local variables
	*/
	OV_OVM_LIBRARY_DEF *plib;
	/*
	*	check identifier
	*/
	if(!identifier) {
		return NULL;
	}
	/*
	*	search for library
	*/
	for(plib=libraries; plib; plib=plib->pnext) {
		if(!strcmp(plib->identifier, identifier)) {
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
*	Get pointer to structure definition
*/
OV_OVM_STRUCTURE_DEF *ov_codegen_getstructdef(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_STRING			identifier
) {
	/*
	*	local variables
	*/
	OV_OVM_LIBRARY_DEF		*plib2 = plib;
	OV_OVM_STRUCTURE_DEF	*pstruct;
	OV_STRING				libid;
	OV_STRING				structid = identifier;
	/*
	*	check parameters
	*/
	if(!identifier) {
		return NULL;
	}
	if(!plib) {
		/*
		*	allocate memory for the library and the structure identifier
		*/
		libid = ov_codegen_getstring(identifier, strlen(identifier));
		/*
		*	search for '/'
		*/
		for(structid = libid; *structid; structid++) {
			if(*structid == '/') {
				*structid = 0;
				structid++;
				break;
			}
		}
		/*
		*	search for the library
		*/
		plib2 = ov_codegen_getlibdef(libid);
		if(!plib2) {
			return NULL;
		}
	}
	/*
	*	search for the structure
	*/
	for(pstruct=plib2->structures; pstruct; pstruct=pstruct->pnext) {
		if(!strcmp(pstruct->identifier, structid)) {
			return pstruct;
		}
	}
	/*
	*	structure not found
	*/
	return NULL;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get pointer to class definition
*/
OV_OVM_CLASS_DEF *ov_codegen_getclassdef(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_STRING			identifier
) {
	/*
	*	local variables
	*/
	OV_OVM_LIBRARY_DEF	*plib2 = plib;
	OV_OVM_CLASS_DEF	*pclass;
	OV_STRING			libid;
	OV_STRING			classid = identifier;
	/*
	*	check parameters
	*/
	if(!identifier) {
		return NULL;
	}
	if(!plib) {
		/*
		*	allocate memory for the library and the class identifier
		*/
		libid = ov_codegen_getstring(identifier, strlen(identifier));
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
		plib2 = ov_codegen_getlibdef(libid);
		if(!plib2) {
			return NULL;
		}
	}
	/*
	*	search for the class
	*/
	for(pclass=plib2->classes; pclass; pclass=pclass->pnext) {
		if(!strcmp(pclass->identifier, classid)) {
			return pclass;
		}
	}
	/*
	*	class not found
	*/
	return NULL;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get pointer to association definition
*/
OV_OVM_ASSOCIATION_DEF *ov_codegen_getassocdef(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_STRING			identifier
) {
	/*
	*	local variables
	*/
	OV_OVM_LIBRARY_DEF		*plib2 = plib;
	OV_OVM_ASSOCIATION_DEF	*passoc;
	OV_STRING				libid;
	OV_STRING				associd = identifier;
	/*
	*	check parameters
	*/
	if(!identifier) {
		return NULL;
	}
	if(!plib) {
		/*
		*	allocate memory for the library and the association identifier
		*/
		libid = ov_codegen_getstring(identifier, strlen(identifier));
		/*
		*	search for '/'
		*/
		for(associd = libid; *associd; associd++) {
			if(*associd == '/') {
				*associd = 0;
				associd++;
				break;
			}
		}
		/*
		*	search for the library
		*/
		plib2 = ov_codegen_getlibdef(libid);
		if(!plib2) {
			return NULL;
		}
	}
	/*
	*	search for the association
	*/
	for(passoc=plib2->associations; passoc; passoc=passoc->pnext) {
		if(!strcmp(passoc->identifier, associd)) {
			return passoc;
		}
	}
	/*
	*	association not found
	*/
	return NULL;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get pointer to a member definition of a structure
*/
OV_OVM_VARIABLE_DEF *ov_codegen_getmemberdef(
	OV_OVM_LIBRARY_DEF		*plib,
	OV_OVM_STRUCTURE_DEF	*pstruct,
	OV_STRING				identifier
) {
	/*
	*	local variables
	*/
	OV_OVM_VARIABLE_DEF	*pvar;
	/*
	*	check identifier
	*/
	if(!identifier) {
		return NULL;
	}
	/*
	*	search for member
	*/
	for(pvar=pstruct->members; pvar; pvar=pvar->pnext) {
		if(!strcmp(pvar->identifier, identifier)) {
			return pvar;
		}
	}
	/*
	*	member not found
	*/
	return NULL;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get pointer to a variable definition of a class
*/
OV_OVM_VARIABLE_DEF *ov_codegen_getvardef(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	OV_STRING			identifier
) {
	/*
	*	local variables
	*/
	OV_OVM_VARIABLE_DEF	*pvar;
	OV_OVM_CLASS_DEF	*pbaseclass;
	/*
	*	check identifier
	*/
	if(!identifier) {
		return NULL;
	}
	/*
	*	if there's a base class, search there first
	*/
	pbaseclass = ov_codegen_getclassdef(NULL, pclass->baseclassname);
	if(pbaseclass) {
		if(pbaseclass->defnum < pclass->defnum) {
			pvar = ov_codegen_getvardef(plib, pbaseclass, identifier);
			if(pvar) {
				return pvar;
			}
		}
	}
	/*
	*	search for variable in this class
	*/
	for(pvar=pclass->variables; pvar; pvar=pvar->pnext) {
		if(!strcmp(pvar->identifier, identifier)) {
			return pvar;
		}
	}
	/*
	*	variable not found
	*/
	return NULL;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get pointer to a part definition of a class
*/
OV_OVM_PART_DEF *ov_codegen_getpartdef(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	OV_STRING			identifier
) {
	/*
	*	local variables
	*/
	OV_OVM_PART_DEF		*ppart;
	OV_OVM_CLASS_DEF	*pbaseclass;
	/*
	*	check identifier
	*/
	if(!identifier) {
		return NULL;
	}
	/*
	*	if there's a base class, search there first
	*/
	pbaseclass = ov_codegen_getclassdef(NULL, pclass->baseclassname);
	if(pbaseclass) {
		if(pbaseclass->defnum < pclass->defnum) {
			ppart = ov_codegen_getpartdef(plib, pbaseclass, identifier);
			if(ppart) {
				return ppart;
			}
		}
	}
	/*
	*	search for part in this class
	*/
	for(ppart=pclass->parts; ppart; ppart=ppart->pnext) {
		if(!strcmp(ppart->identifier, identifier)) {
			return ppart;
		}
	}
	/*
	*	part not found
	*/
	return NULL;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get pointer to an operation definition of a class
*/
OV_OVM_OPERATION_DEF *ov_codegen_getopdef(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	OV_STRING			identifier
) {
	/*
	*	local variables
	*/
	OV_OVM_OPERATION_DEF	*pop;
	OV_OVM_CLASS_DEF		*pbaseclass;
	/*
	*	check identifier
	*/
	if(!identifier) {
		return NULL;
	}
	/*
	*	if there's a base class, search there first
	*/
	pbaseclass = ov_codegen_getclassdef(NULL, pclass->baseclassname);
	if(pbaseclass) {
		if(pbaseclass->defnum < pclass->defnum) {
			pop = ov_codegen_getopdef(plib, pbaseclass, identifier);
			if(pop) {
				return pop;
			}
		}
	}
	/*
	*	search for operation in this class
	*/
	for(pop=pclass->operations; pop; pop=pop->pnext) {
		if(!strcmp(pop->identifier, identifier)) {
			return pop;
		}
	}
	/*
	*	operation not found
	*/
	return NULL;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get pointer to an operation definition of a class by a given number
*/
OV_OVM_OPERATION_DEF *ov_codegen_getopdefbynum(
	OV_OVM_CLASS_DEF	*pclass,
	OV_UINT				num
) {
	/*
	*	local variables
	*/
	OV_OVM_OPERATION_DEF	*pop = pclass->operations;
	OV_OVM_CLASS_DEF		*pclass2 = pclass;
	/*
	*	search for the operation definition
	*/
	while(TRUE) {
		while(!pop) {
			pclass2 = ov_codegen_getclassdef(NULL, pclass2->baseclassname);
			if(pclass2) {
				pop = pclass2->operations;
			} else {
				fprintf(stderr, "internal error -- sorry.\n");
				exit(EXIT_FAILURE);
			}
		}
		if(pop->operationnum == num) {
			return pop;
		}
		pop = pop->pnext;
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	Clean up memory
*/
void ov_codegen_cleanup(void) {
	/*
	*	local variables
	*/
	/* TODO! delete all definition objects */
	ov_codegen_freestrings();
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

