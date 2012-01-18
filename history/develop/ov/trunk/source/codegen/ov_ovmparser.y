/*
*   $Id: ov_ovmparser.y,v 1.16 2005-08-17 09:40:10 markus Exp $
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
*	04-Nov-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: variable type ANY added.
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
	OV_BOOL					bool;
	OV_UINT					uint;
	OV_INT					_int;
	OV_DOUBLE				_double;
	OV_TIME					time;
	OV_TIME_SPAN				timespan;
	OV_CLASS_PROPS				classprops;
	OV_ASSOC_TYPE				assoctype;
	OV_ASSOC_PROPS				assocprops;
	OV_VAR_TYPE				vartype;
	OV_VAR_PROPS				varprops;
	OV_OP_PROPS				opprops;
	OV_OVM_LIBRARY_DEF			*plibdef;
	OV_OVM_STRUCTURE_DEF			*pstructdef;
	OV_OVM_CLASS_DEF			*pclassdef;
	OV_OVM_ASSOCIATION_DEF			*passocdef;
	OV_OVM_VARIABLE_DEF			*pvardef;
	OV_OVM_PART_DEF				*ppartdef;
	OV_OVM_OPERATION_DEF			*popdef;
	OV_OVM_VARTYPE_DEF			*pvartypedef;
	OV_OVM_INITVALUE_DEF			*pinitvalue;
}

%token TOK_LIBRARY TOK_VERSION TOK_AUTHOR TOK_COPYRIGHT TOK_COMMENT TOK_END_LIBRARY

%token TOK_STRUCTURE TOK_END_STRUCTURE

%token TOK_CLASS TOK_VARIABLES TOK_END_VARIABLES TOK_OPERATIONS TOK_END_OPERATIONS
%token TOK_PARTS TOK_END_PARTS TOK_END_CLASS

%token TOK_ASSOCIATION TOK_PARENT TOK_CHILD TOK_END_ASSOCIATION

%token TOK_C_TYPE TOK_UNIT

%token TOK_C_FUNCTION TOK_IS_ABSTRACT

%token <classprops>	TOK_CLASSPROPS

%token <assoctype>	TOK_ASSOCTYPE
%token <assocprops>	TOK_ASSOCPROPS

%token <opprops>	TOK_OPPROPS

%token <vartype>	TOK_VARTYPE
%token <varprops>	TOK_VARPROPS
/*%token <assoctype>	TOK_ASSOCTYPE*/

%token <uint>		TOK_FLAGS

%token <string>		TOK_STRING
%token <string>		TOK_IDENTIFIER
%token <string>		TOK_IDENTIFIER_EX
%token <string>		TOK_C_IDENTIFIER
%token <bool>		TOK_BOOL
%token <uint>		TOK_UINT
%token <_int>		TOK_INT
%token <_double>		TOK_REAL
%token <time>		TOK_TIME
%token <timespan>	TOK_TIMESPAN
%token TOK_INITIALVALUE
%token TOK_VOID

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
%type <assoctype>	assoctype_opt assoctype
%type <assocprops>	assocprops_opt assocprops

%type <pvardef>		members member

%type <pvardef>		variables variable
%type <varprops>	varprops_opt varprops

%type <ppartdef>	parts part

%type <popdef>		operations operation
%type <opprops>		opprops_opt opprops

%type <string>		comment_opt
%type <uint>		vector_length_opt length_opt
%type <pvartypedef> 	vartype
%type <string>		unit_opt varcomment_opt assoccomment_opt
%type <uint>		varflags_opt assocflags_opt
%type <pinitvalue>	initialvalue_opt vector_values structure_values varvalue

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
			$$->libname = NULL;
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
			$$->childflags = $10;
			$$->childcomment = $11;
			$$->childrolename = $14;
			$$->childclassname = $17;
			$$->parentflags = $18;
			$$->parentcomment = $19;
			$$->defnum = defnum++;
		}
;

assoctype_opt:
	/* empty */
		{
			$$ = OV_AT_ONE_TO_MANY;
		}
	| assoctype
		{
			$$ = $1;
		}
;

assoctype:
	':' TOK_ASSOCTYPE
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
			$$->varprops = 0;
			$$->flags = $5;
			$$->tech_unit = $6;
			$$->comment = $7;
			$$->pinitvalue = NULL;
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
	TOK_IDENTIFIER vector_length_opt ':' vartype varprops_opt varflags_opt unit_opt varcomment_opt initialvalue_opt ';'
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
			$$->pinitvalue = $9;
			ov_codegen_free($4);
		}
;

varprops_opt:
	/* empty */
		{
			$$ = 0;
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


initialvalue_opt:
	/* empty */
		{
			$$ = NULL;
		}
	| TOK_INITIALVALUE '=' varvalue
		{
			$$ = $3;
		}
;

varvalue: 
	TOK_BOOL
		{
			$$ = Ov_Codegen_Malloc(OV_OVM_INITVALUE_DEF);
			$$->pvectorelem = NULL;
			$$->pstructelem = NULL;
			$$->pnext = NULL;
			$$->num = 0;
			$$->identifier = NULL;
			$$->value.vartype = OV_VT_BOOL;
			$$->value.valueunion.val_bool = $1;
		}
	| TOK_INT
		{
			$$ = Ov_Codegen_Malloc(OV_OVM_INITVALUE_DEF);
			$$->pvectorelem = NULL;
			$$->pstructelem = NULL;
			$$->pnext = NULL;
			$$->num = 0;
			$$->identifier = NULL;
			$$->value.vartype = OV_VT_INT;
			$$->value.valueunion.val_int = $1;
		}
	| TOK_UINT
		{
			$$ = Ov_Codegen_Malloc(OV_OVM_INITVALUE_DEF);
			$$->pvectorelem = NULL;
			$$->pstructelem = NULL;
			$$->pnext = NULL;
			$$->num = 0;
			$$->identifier = NULL;
			$$->value.vartype = OV_VT_UINT;
			$$->value.valueunion.val_uint = $1;
		}
	| TOK_REAL
		{
			$$ = Ov_Codegen_Malloc(OV_OVM_INITVALUE_DEF);
			$$->pvectorelem = NULL;
			$$->pstructelem = NULL;
			$$->pnext = NULL;
			$$->num = 0;
			$$->identifier = NULL;
			$$->value.vartype = OV_VT_DOUBLE;
			$$->value.valueunion.val_double = $1;
		}
	| TOK_TIME
		{
			$$ = Ov_Codegen_Malloc(OV_OVM_INITVALUE_DEF);
			$$->pvectorelem = NULL;
			$$->pstructelem = NULL;
			$$->pnext = NULL;
			$$->num = 0;
			$$->identifier = NULL;
			$$->value.vartype = OV_VT_TIME;
			$$->value.valueunion.val_time = $1;
		}
	| TOK_TIMESPAN
		{
			$$ = Ov_Codegen_Malloc(OV_OVM_INITVALUE_DEF);
			$$->pvectorelem = NULL;
			$$->pstructelem = NULL;
			$$->pnext = NULL;
			$$->num = 0;
			$$->identifier = NULL;
			$$->value.vartype = OV_VT_TIME_SPAN;
			$$->value.valueunion.val_time_span = $1;
		}
	| TOK_VOID
		{
			$$ = Ov_Codegen_Malloc(OV_OVM_INITVALUE_DEF);
			$$->pvectorelem = NULL;
			$$->pstructelem = NULL;
			$$->pnext = NULL;
			$$->num = 0;
			$$->identifier = NULL;
			$$->value.vartype = OV_VT_STRING;
			$$->value.valueunion.val_string = NULL;
		}
	| TOK_STRING
		{
			$$ = Ov_Codegen_Malloc(OV_OVM_INITVALUE_DEF);
			$$->pvectorelem = NULL;
			$$->pstructelem = NULL;
			$$->pnext = NULL;
			$$->num = 0;
			$$->identifier = NULL;
			$$->value.vartype = OV_VT_STRING;
			$$->value.valueunion.val_string = $1;
		}
	| '{' vector_values '}'
		{
			$$ = Ov_Codegen_Malloc(OV_OVM_INITVALUE_DEF);
			$$->pvectorelem = $2;
			$$->pstructelem = NULL;
			$$->pnext = NULL;
			$$->num = 0;
			$$->identifier = NULL;
			$$->value.vartype = ov_codegen_getvarvectortype($2->value.vartype);
		}
	| '{' structure_values '}'
		{
			$$ = Ov_Codegen_Malloc(OV_OVM_INITVALUE_DEF);
			$$->pvectorelem = NULL;
			$$->pstructelem = $2;
			$$->pnext = NULL;
			$$->num = 0;
			$$->identifier = NULL;
			$$->value.vartype = OV_VT_STRUCT;
		}
;

vector_values:
	/* empty */
		{
			$$ = NULL;
		}
	| varvalue
		{
			$$ = $1;
		}
	| vector_values ',' varvalue
		{
			OV_OVM_INITVALUE_DEF *ptr;

			ptr = $1;
			while (ptr->pnext) ptr = ptr->pnext;
			ptr->pnext = $3;
			$$ = $1;
		}
;
structure_values:
	TOK_IDENTIFIER '=' varvalue
		{
			$3->identifier = $1;
			$$ = $3;
		}
	| structure_values ',' TOK_IDENTIFIER '=' varvalue
		{
			OV_OVM_INITVALUE_DEF *ptr;

			ptr = $1;
			while (ptr->pnext) ptr = ptr->pnext;
			$5->identifier = $3;
			ptr->pnext = $5;
			$$ = $1;
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
	TOK_IDENTIFIER ':' TOK_CLASS TOK_IDENTIFIER_EX varflags_opt';'
		{
			$$ = Ov_Codegen_Malloc(OV_OVM_PART_DEF);
			$$->pnext = NULL;
			$$->identifier = $1;
			$$->partclassname = $4;
			$$->partclasslibname = NULL;
			$$->flags = $5;
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
			$$->vartype = OV_VT_CTYPE;
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
	/* 
	*	There is a segfault with a return, therefore we do an exit. 
	*	TODO: Reasons for the segfault have to be investigated.
	*/
	exit(EXIT_FAILURE);
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
	*	final but not instaniable classes do not seem to make any sense
	*/
	if((pclass->classprops & OV_CP_FINAL) && !(pclass->classprops & OV_CP_INSTANTIABLE)) {
		fprintf(stderr, "class \"%s\": final classes must be instantiable.\n",
			pclass->identifier);
		result = FALSE;
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
	OV_OVM_LIBRARY_DEF		*pparentlib;
	OV_OVM_LIBRARY_DEF		*pchildlib;

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
	*	local n:m associations are not (yet?) supported
	*/
	if((passoc->assoctype == OV_AT_MANY_TO_MANY) 
		&& (passoc->assocprops & OV_AP_LOCAL)
	) {
		fprintf(stderr, "association \"%s\": n:m associations cannot be local.\n",
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
		pparentlib = ov_codegen_getlibdef(pparentclass->libname);
		if(pparentclass->defnum > passoc->defnum) {
			fprintf(stderr, "association \"%s\": definition prior to definition "
				"of parent class \"%s\".\n", passoc->identifier,
				passoc->parentclassname);
			result = FALSE;
		}
/*		if(pparentclass->libname != passoc->libname) {
			fprintf(stderr, "association \"%s\": definition not in same library "
				"as parent class \"%s\".\n", passoc->identifier,
				passoc->parentclassname);
			result = FALSE;
		}
*/
		/*
		*	check child role name -- yes, CHILD rolename
		*	FIXME! do not only check variables, parts and operations
		*	       but also links of the parentclass!!
		*/
		if(ov_codegen_getvardef(pparentlib, pparentclass, passoc->childrolename)
			|| ov_codegen_getpartdef(pparentlib, pparentclass, passoc->childrolename)
			|| ov_codegen_getopdef(pparentlib, pparentclass, passoc->childrolename)
			|| ov_codegen_comparerolename(pparentclass, passoc, passoc->childrolename)
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
		pchildlib = ov_codegen_getlibdef(pchildclass->libname);
		if(pchildclass->defnum > passoc->defnum) {
			fprintf(stderr, "association \"%s\": definition prior to definition "
				"of child class \"%s\".\n", passoc->identifier,
				passoc->childclassname);
			result = FALSE;
		}
/*		if(pchildclass->libname != passoc->libname) {
			fprintf(stderr, "association \"%s\": definition not in same library "
				"as child class \"%s\".\n", passoc->identifier,
				passoc->childclassname);
			result = FALSE;
		}
*/	
	 /*
		*	check parent role name -- yes, PARENT rolename
		*	FIXME! do not only check variables, parts and operations
		*	       but also links of the childclass!!
		*/
		if(ov_codegen_getvardef(pchildlib, pchildclass, passoc->parentrolename)
			|| ov_codegen_getpartdef(pchildlib, pchildclass, passoc->parentrolename)
			|| ov_codegen_getopdef(pchildlib, pchildclass, passoc->parentrolename)
			|| ov_codegen_comparerolename(pchildclass, passoc, passoc->parentrolename)
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
		*	vectors of ANY variables are not supported
		*/
		if(pvar->vartype == OV_VT_ANY) {
			fprintf(stderr, "structure \"%s\", variable \"%s\": vectors of type ANY "
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
		pvarstruct = ov_codegen_getstructdef(NULL, pvar->structurename);
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
OV_BOOL ov_codegen_checkinitialvector(
	OV_OVM_VARIABLE_DEF	*vardef,
	OV_OVM_INITVALUE_DEF	*initdef
) {
	OV_OVM_INITVALUE_DEF	*pvecelem;
	OV_BOOL			result = TRUE;
	OV_VAR_TYPE		type;
	OV_UINT			veclen;

	if ((vardef->vartype == OV_VT_INT_VEC) && (initdef->value.vartype == OV_VT_UINT_VEC))
		initdef->value.vartype = OV_VT_INT_VEC;
	if ((vardef->vartype == OV_VT_SINGLE_VEC) && (initdef->value.vartype == OV_VT_DOUBLE_VEC))
		initdef->value.vartype = OV_VT_SINGLE_VEC;
	if (vardef->vartype != initdef->value.vartype) {
		result = FALSE;
	}

	type = ov_codegen_getvarelementtype(vardef->vartype);
	veclen = 0;
	pvecelem = initdef->pvectorelem;
	while(pvecelem) {
		if ((type == OV_VT_INT) && (pvecelem->value.vartype == OV_VT_UINT))
			pvecelem->value.vartype = OV_VT_INT;
		if ((type == OV_VT_SINGLE) && (pvecelem->value.vartype == OV_VT_DOUBLE))
			pvecelem->value.vartype = OV_VT_SINGLE;
		if (type != pvecelem->value.vartype) {
			result = FALSE;
		}
		pvecelem = pvecelem->pnext;
		veclen++;
	}
	initdef->value.valueunion.val_generic_vec.veclen = veclen;
	if ((vardef->veclen) && (veclen!=vardef->veclen)) result = FALSE;
	return result;
}


OV_BOOL ov_codegen_checksemantics_variable(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	OV_OVM_VARIABLE_DEF	*pvar
) {
	/*
	*	local variables
	*/
	OV_BOOL					result = TRUE;
	OV_OVM_STRUCTURE_DEF			*pvarstruct;
	OV_OVM_INITVALUE_DEF 			*valuedef;
	OV_OVM_VARIABLE_DEF 			*vardef;
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
		*	vectors of ANY variables are not supported
		*/
		if(pvar->vartype == OV_VT_ANY) {
			fprintf(stderr, "class \"%s\", variable \"%s\": vectors of type ANY "
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
	*	variables cannot be both derived and static
	*/
	if((pvar->varprops & OV_VP_DERIVED) && (pvar->varprops & OV_VP_STATIC)) {
		fprintf(stderr, "class \"%s\", variable \"%s\": variable cannot be both "
			"derived and static.\n", pclass->identifier, pvar->identifier);
		result = FALSE;
	}
	/*
	*	if variable is a structure, check the structure definition
	*/
	if(pvar->structurename) {
		pvarstruct = ov_codegen_getstructdef(NULL, pvar->structurename);
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
	*	if variable defines a initialvalue, check if the values match to the variable definition
	*/
	if(pvar->pinitvalue) {
		if((pvar->varprops & OV_VP_DERIVED) || (pvar->varprops & OV_VP_STATIC)) {
			fprintf(stderr, "class \"%s\", variable \"%s\": variable with initialvalues may not be "
				"derived or static.\n", pclass->identifier, pvar->identifier);
			result = FALSE;
		}
		if (pvar->structurename) {	// check structure definition
			if (pvar->pinitvalue->value.vartype != OV_VT_STRUCT) {
				result = FALSE;
				fprintf(stderr, "class \"%s\", variable \"%s\": bad initialvalue: wrong value type\n", pclass->identifier, pvar->identifier);
			}
			pvarstruct = ov_codegen_getstructdef(plib, pvar->structurename);
			valuedef = pvar->pinitvalue->pstructelem;
			vardef = pvarstruct->members;

			while(valuedef && vardef) {
				if (vardef->veclen !=1) {
					if (!ov_codegen_checkinitialvector(vardef, valuedef)) {
						result = FALSE;
						fprintf(stderr, "class \"%s\", variable \"%s\": bad initialvalue: wrong datatype\n", pclass->identifier, pvar->identifier);
					}
				}
				else {
					if ((vardef->vartype == OV_VT_INT) && (valuedef->value.vartype == OV_VT_UINT))
						valuedef->value.vartype = OV_VT_INT;
					if ((vardef->vartype == OV_VT_SINGLE) && (valuedef->value.vartype == OV_VT_DOUBLE))
						valuedef->value.vartype = OV_VT_SINGLE;
					if (vardef->vartype != valuedef->value.vartype) {
						result = FALSE;
						fprintf(stderr, "class \"%s\", variable \"%s\": bad initialvalue: wrong datatype\n", pclass->identifier, pvar->identifier);
					}
					if (strcmp(vardef->identifier, valuedef->identifier)) {
						result = FALSE;
						fprintf(stderr, "class \"%s\", variable \"%s\": bad initialvalue: %s is not a member of structure\n", pclass->identifier, pvar->identifier, valuedef->identifier);
					}
				}
				valuedef = valuedef->pnext;
				vardef = vardef->pnext;
			}
			if (valuedef || vardef) {
				result = FALSE;
				fprintf(stderr, "class \"%s\", variable \"%s\": bad initialvalue: wrong number of structure elements\n", pclass->identifier, pvar->identifier);
			}
		}
		if (pvar->veclen == 0) {	// check dynamic vector
			OV_OVM_INITVALUE_DEF *valuedef = (OV_OVM_INITVALUE_DEF*) pvar->pinitvalue;

			if (!ov_codegen_checkinitialvector(pvar, valuedef)) {
				result = FALSE;
				fprintf(stderr, "class \"%s\", variable \"%s\": bad initialvalue: wrong datatype\n", pclass->identifier, pvar->identifier);
			}
		}
		else if (pvar->veclen != 1) {	// check static vector
			OV_OVM_INITVALUE_DEF *valuedef = (OV_OVM_INITVALUE_DEF*) pvar->pinitvalue;

			if (!ov_codegen_checkinitialvector(pvar, valuedef)) {
				result = FALSE;
				fprintf(stderr, "class \"%s\", variable \"%s\": bad initialvalue: wrong datatype\n", pclass->identifier, pvar->identifier);
			}
		}
		else {				// check scalar value
			if ((pvar->vartype == OV_VT_INT) && (pvar->pinitvalue->value.vartype == OV_VT_UINT))
				pvar->pinitvalue->value.vartype = OV_VT_INT;
			if ((pvar->vartype == OV_VT_SINGLE) && (pvar->pinitvalue->value.vartype == OV_VT_DOUBLE))
				pvar->pinitvalue->value.vartype = OV_VT_SINGLE;
			if (pvar->vartype != pvar->pinitvalue->value.vartype) {
				result = FALSE;
				fprintf(stderr, "class \"%s\", variable \"%s\": bad initialvalue: wrong datatype\n", pclass->identifier, pvar->identifier);
			}
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
*	Compares the Associationrolenames of a class with a given name
*/
OV_BOOL ov_codegen_comparerolename(
	OV_OVM_CLASS_DEF	*pclass,
	OV_OVM_ASSOCIATION_DEF	*pthisassoc,
	OV_STRING		identifier
) {
	OV_OVM_ASSOCIATION_DEF	*passoc;
	OV_OVM_LIBRARY_DEF	*plib;
	char			classname[256];

	sprintf(classname, "%s/%s", pclass->libname, pclass->identifier);
	/*
	*	iterate over libraries
	*/
	for(plib=libraries; plib; plib=plib->pnext) {
		/*
		*	iterate over associations
		*/
		for(passoc=plib->associations; passoc; passoc=passoc->pnext) {
			if (passoc != pthisassoc) {
				if (!strcmp(passoc->childclassname, classname)) {
					if(!strcmp(passoc->parentrolename, identifier)) {
						return TRUE;
					}
				}
				if (!strcmp(passoc->parentclassname, classname)) {
					if(!strcmp(passoc->childrolename, identifier)) {
						return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}

/*	----------------------------------------------------------------------	*/

/*
*	Clean up memory
*/
void ov_codegen_freeassoc(OV_OVM_ASSOCIATION_DEF *passocdef) {

	if (passocdef->pnext) ov_codegen_freeassoc(passocdef->pnext);

	ov_codegen_free(passocdef);
}

void ov_codegen_freepart(OV_OVM_PART_DEF *ppartdef) {

	if (ppartdef->pnext) ov_codegen_freepart(ppartdef->pnext);

	ov_codegen_free(ppartdef);
}

void ov_codegen_freeoperation(OV_OVM_OPERATION_DEF *poperationdef) {

	if (poperationdef->pnext) ov_codegen_freeoperation(poperationdef->pnext);

	ov_codegen_free(poperationdef);
}

void ov_codegen_freeinitvalue(OV_OVM_INITVALUE_DEF *pinitdef) {

	if (pinitdef->pvectorelem) ov_codegen_freeinitvalue(pinitdef->pvectorelem);
	if (pinitdef->pstructelem) ov_codegen_freeinitvalue(pinitdef->pstructelem);
	if (pinitdef->pnext) ov_codegen_freeinitvalue(pinitdef->pnext);

	ov_codegen_free(pinitdef);
}


void ov_codegen_freevariable(OV_OVM_VARIABLE_DEF *pvardef) {

	if (pvardef->pinitvalue) ov_codegen_freeinitvalue(pvardef->pinitvalue);
	if (pvardef->pnext) ov_codegen_freevariable(pvardef->pnext);

	ov_codegen_free(pvardef);
}

void ov_codegen_freeclass(OV_OVM_CLASS_DEF *pclassdef) {

	if (pclassdef->variables) ov_codegen_freevariable(pclassdef->variables);
	if (pclassdef->parts) ov_codegen_freepart(pclassdef->parts);
	if (pclassdef->operations) ov_codegen_freeoperation(pclassdef->operations);
	if (pclassdef->pnext) ov_codegen_freeclass(pclassdef->pnext);

	ov_codegen_free(pclassdef);
}

void ov_codegen_freestructure(OV_OVM_STRUCTURE_DEF *pstructdef) {

	if (pstructdef->members) ov_codegen_freevariable(pstructdef->members);
	if (pstructdef->pnext) ov_codegen_freestructure(pstructdef->pnext);

	ov_codegen_free(pstructdef);
}

void ov_codegen_freelib(OV_OVM_LIBRARY_DEF *plibdef) {
	if (plibdef->structures) ov_codegen_freestructure(plibdef->structures);
	if (plibdef->classes) ov_codegen_freeclass(plibdef->classes);
	if (plibdef->associations) ov_codegen_freeassoc(plibdef->associations);
	if (plibdef->pnext) ov_codegen_freelib(plibdef->pnext);
	ov_codegen_free(plibdef);
}

void ov_codegen_cleanup(void) {
	/*
	*	local variables
	*/

	ov_codegen_freestrings();
	ov_codegen_freelib(libraries);
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

