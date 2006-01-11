/*
 * Copyright (c) 1996-2002
 * Lehrstuhl fuer Prozessleittechnik, RWTH Aachen
 * D-52064 Aachen, Germany.
 * All rights reserved.
 *
 * This file is part of the ACPLT/KS Package which is licensed as open
 * source under the Artistic License; you can use, redistribute and/or
 * modify it under the terms of that license.
 *
 * You should have received a copy of the Artistic License along with
 * this Package; see the file ARTISTIC-LICENSE. If not, write to the
 * Copyright Holder.
 *
 * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
 * OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/* Parser definition for ACPLT/OV text file parser */

/* Author : Christian Poensgen <chris@plt.rwth-aachen.de> */
/* db.y */
/* last change: Jul 18, 2002 */

/* ACPLT/OV Modelling Language */

/*
*   bison C declarations
*   --------------------
*/
%{
#include "dbparse.h"

#define YYDEBUG 0
//#define YYPRINT(file, type, value) yyprint(file, type, value)

//void yyprint(FILE*, int, YYSTYPE);
extern parsetree *parse_tree;		/* the parse tree to be created */
%}

/*
*   bison definitions
*   ------------------
*/

/* variable type names to be used in %token and %type definitions */
%union {
	bool										boolval;
	KsString									*pstringval;
	LogPath										*ppathval;
	int											intval;
	double										doubleval;
	KsTime										*ptimeval;
	KsTimeSpan									*ptimespanval;
	void										*pvoidval;
	KS_STATE									stateval;
	instance									*pinstance;
	variable_value								*pvarval;
	link_value									*plinkval;
	value										*pvalue;
	vector										*pvector;
	structure									*pstructure;
	PltList<instance *>							*pinstlist;
	PltList<variable_value *>					*pvarlist;
	PltList<link_value *>						*plinklist;
	PltList<LogPath>							*ppathlist;
};

%token INSTANCE
%token CLASS
%token CREATION_TIME
%token FLAGS
%token COMMENT
%token VARIABLE_VALUES
%token END_VARIABLE_VALUES
%token LINK_VALUES
%token END_LINK_VALUES
%token PART_INSTANCES
%token END_PART_INSTANCES
%token END_INSTANCE

%token TIME
%token STATE
%token UNIT

%token PART_INSTANCE
%token END_PART_INSTANCE

%token <pstringval> IDENTIFIER
%token <ppathval> PATH
%token <boolval> BOOL_VALUE
%token <intval> INT_VALUE
%token <doubleval> DOUBLE_VALUE
%token <pstringval> STRING_VALUE
%token <ptimeval> TIME_VALUE
%token <ptimespanval> TIME_SPAN_VALUE
%token <pvoidval> VOID_VALUE
%token <stateval> STATE_VALUE

%type <pinstance> instances inst part_inst
%type <ptimeval> creation_time_opt
%type <intval> flags_opt
%type <pstringval> comment_opt
%type <pvarval> variable_value
%type <plinkval> link_value
%type <pvalue> value
%type <pvector> vector_values
%type <pstructure> structure_values
%type <ptimeval> var_time_opt
%type <stateval> var_state_opt
%type <intval> var_flags_opt
%type <pstringval> var_unit_opt
%type <pstringval> var_comment_opt
%type <ppathlist> paths
%type <pinstlist> part_instances_block_opt parts_opt
%type <pvarlist> variable_values_block_opt variables_opt
%type <plinklist> link_values_block_opt links_opt
%type <ppathval> abs_or_rel_path

%%

/*
*   bison grammar rules
*   -------------------
*/

/* root of the parse tree */
instances:	  inst
				{
					parse_tree->add($1);
				}
			| instances inst
				{
					parse_tree->add($2);
				}
			;

inst:	  INSTANCE abs_or_rel_path ':' CLASS abs_or_rel_path
		  creation_time_opt
		  flags_opt
		  comment_opt
		  variable_values_block_opt
		  link_values_block_opt
		  part_instances_block_opt
		  END_INSTANCE ';'
			{
				$$ = new(instance);
				$$->ident = $2;
				$$->is_part = false;
				$$->class_ident = $5;
				$$->creation_time = $6;
				$$->sem_flags = $7;
				$$->comment = $8;
				$$->var_block_list = $9;
				$$->link_block_list = $10;
				$$->part_block_list = $11;
			}
		;

creation_time_opt:
						{
							$$ = NULL;
						}
					| CREATION_TIME '=' TIME_VALUE ';'
						{
							$$ = $3;
						}
					;

flags_opt:
				{
					$$ = 0;
				}
			| FLAGS '=' STRING_VALUE ';'
				{
					$$ = GetSemFlags($3);
				}
			;

comment_opt:
					{
						$$ = NULL;
					}
				| COMMENT '=' STRING_VALUE ';'
					{
						$$ = new(KsString);
						$$ = $3;
					}
				;

variable_values_block_opt:
								{
									$$ = NULL;
								}
							| VARIABLE_VALUES
							  variables_opt
							  END_VARIABLE_VALUES ';'
								{
									$$ = $2;
								}
							;

variables_opt:
				{
					$$ = NULL;
				}
			| variables_opt variable_value
				{
					if ($1 == NULL) {
						$$ = new(PltList<variable_value *>);
					} else {
						$$ = $1;
					}
					$$->addLast($2);
				}
			;

variable_value:	  IDENTIFIER '=' value
				  var_time_opt
				  var_state_opt
				  var_flags_opt
				  var_unit_opt
				  var_comment_opt ';'
					{
						$$ = new(variable_value);
						$$->ident = $1;
						$$->val = $3;
						$$->var_time = $4;
						$$->var_state = $5;
						$$->var_flags = $6;
						$$->var_unit = $7;
						$$->var_comment = $8;
					}
				;

value:	  BOOL_VALUE
			{
				$$ = new(value);
				$$->type = DB_VT_BOOL;
				$$->v.bool_val = $1;
			}
		| INT_VALUE
			{
				$$ = new(value);
				$$->type = DB_VT_INT;
				$$->v.int_val = $1;
			}
		| DOUBLE_VALUE
			{
				$$ = new(value);
				$$->type = DB_VT_DOUBLE;
				$$->v.double_val = $1;
			}
		| STRING_VALUE
			{
				$$ = new(value);
				$$->type = DB_VT_STRING;
				$$->v.pstring_val = $1;
			}
		| TIME_VALUE
			{
				$$ = new(value);
				$$->type = DB_VT_TIME;
				$$->v.ptime_val = $1;
			}
		| TIME_SPAN_VALUE
			{
				$$ = new(value);
				$$->type = DB_VT_TIME_SPAN;
				$$->v.ptime_span_val = $1;
			}
		| VOID_VALUE
			{
				$$ = new(value);
				$$->type = DB_VT_VOID;
				$$->v.pvoid_val = $1;
			}
		| '{' vector_values '}'
			{
				$$ = new(value);
				$$->type = DB_VT_VECTOR;
				$$->v.pvector_val = $2;
			}
		| '{' structure_values '}'
			{
				$$ = new(value);
				$$->type = DB_VT_STRUCTURE;
				$$->v.pstructure_val = $2;
			}
		;

vector_values:
					{
						$$ = new(vector);
						$$->content = NULL;
						$$->type = DB_VT_VOID;
						$$->next = NULL;
					}
				| value
					{
						$$ = new(vector);
						$$->content = $1;
						$$->type = DB_VT_VOID;
						$$->next = NULL;
					}
				| vector_values ',' value
					{
						vector	*ptr, *new_ptr;
						new_ptr = new(vector);
						new_ptr->content = $3;
						new_ptr->type = DB_VT_VOID;
						new_ptr->next = NULL;
						$$ = $1;
						ptr = $1;
						while (ptr->next != NULL) {
							ptr = ptr->next;
						};
						ptr->next = new_ptr;
					}
				;

structure_values:	  IDENTIFIER '=' value
						{
							$$ = new(structure);
							$$->content = $3;
							$$->next = NULL;
						}
					| structure_values ',' IDENTIFIER '=' value
						{
							structure		*ptr, *new_ptr;
							new_ptr = new(structure);
							new_ptr->content = $5;
							new_ptr->next = NULL;
							$$ = $1;
							ptr = $1;
							while (ptr->next != NULL) {
								ptr = ptr->next;
							};
							ptr->next = new_ptr;
						}
					;

var_time_opt:
					{
						$$ = NULL;
					}
				| TIME '=' TIME_VALUE
					{
						$$ = $3;
					}
				;

var_state_opt:
					{
						$$ = KS_ST_UNKNOWN;
					}
				| STATE '=' STATE_VALUE
					{
						$$ = $3;
					}
				;

var_flags_opt:
					{
						$$ = 0;
					}
				| FLAGS '=' STRING_VALUE
					{
						$$ = GetSemFlags($3);
					}
				;

var_unit_opt:
					{
						$$ = NULL;
					}
				| UNIT '=' STRING_VALUE
					{
						$$ = new(KsString);
						$$ = $3;
					}
				;

var_comment_opt:
						{
							$$ = NULL;
						}
					| COMMENT '=' STRING_VALUE
						{
							$$ = new(KsString);
							$$ = $3;
						}
					;

link_values_block_opt:
							{
								$$ = NULL;
							}
						| LINK_VALUES
						  links_opt
						  END_LINK_VALUES ';'
							{
								$$ = $2;
							}
						;

links_opt:
				{
					$$ = NULL;
				}
			| links_opt link_value
				{
					if ($1 == NULL) {
						$$ = new(PltList<link_value *>);
					} else {
						$$ = $1;
					}
					$$->addLast($2);
				}
			;


link_value:	  IDENTIFIER '=' '{''}'
				{
					$$ = new(link_value);
					$$->ident = $1;
					$$->link_paths = NULL;
				}
		| IDENTIFIER '=' '{' paths '}'
				{
					$$ = new(link_value);
					$$->ident = $1;
					$$->link_paths = $4;
				}
			;

paths:		  abs_or_rel_path
			{
				$$ = new(PltList<LogPath>);
				$$->addLast(*$1);
			}
		| paths ',' abs_or_rel_path
			{
				$$->addLast(*$3);
			}
		;

part_instances_block_opt:
								{
									$$ = NULL;
								}
							| PART_INSTANCES
							  parts_opt
							  END_PART_INSTANCES ';'
								{
									$$ = $2;
								}
							;

parts_opt:
				{
					$$ = NULL;
				}
			| parts_opt part_inst
				{
					if ($1 == NULL) {
						$$ = new(PltList<instance *>);
					} else {
						$$ = $1;
					}
					$$->addLast($2);
				}
			;

part_inst:	  PART_INSTANCE abs_or_rel_path ':' CLASS abs_or_rel_path
			  creation_time_opt
			  flags_opt
			  comment_opt
			  variable_values_block_opt
			  link_values_block_opt
			  part_instances_block_opt
			  END_PART_INSTANCE ';'
				{
					$$ = new(instance);
					$$->ident = $2;
					$$->is_part = true;
					$$->class_ident = $5;
					$$->creation_time = $6;
					$$->sem_flags = $7;
					$$->comment = $8;
					$$->var_block_list = $9;
					$$->link_block_list = $10;
					$$->part_block_list = $11;
					$$->children = NULL;
					$$->cr_opts = CO_NONE;
				}
			;

abs_or_rel_path: PATH
						{
							if (relative) {
								if ($1->isRelative()) {
									$$ = new(LogPath);
									if (*path!=LogPath("/"))
										*$$ = LogPath( (PltString) *path + "/" + PltString(*$1));
									else
										*$$ = LogPath( (PltString) *path +  PltString(*$1));
									free($1);
								}
								else $$ = $1;
							}
							else $$ = $1;
						}
					| IDENTIFIER
						{
							if (relative) {
								$$ = new(LogPath);
								if (*path!=LogPath("/"))
									*$$ = LogPath( (PltString) *path + "/" + PltString(*$1));
								else
									*$$ = LogPath( (PltString) *path +  PltString(*$1));
							} else {
								$$  = new (LogPath);
								*$$ = LogPath(*$1);
							}
							free($1);
						}
%%

int yyerror(char *s)		/* required standard function */
{
	fprintf(stderr, "%s\n", s);
	return 0;
}

// debugging
/*void yyprint(FILE* file, int type, YYSTYPE value)
{
	 switch (type) {
	        case STRING_VALUE:
	        	cout << " " << value.pstringval << " ";
	        	break;
	        case PATH:
	        	cout << " " << *(value.ppathval) << " ";
	        	break;
	 }
}*/

