/*
*   $Id: ov_ovmscanner.lex,v 1.11 2002-08-29 11:03:56 ansgar Exp $
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
*	04-Nov-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: variable type ANY added.
*/

/*
*	Lex states
*	----------
*/
%x comment_state

/*
*   lex C definitions
*   -----------------
*/
%{
#include "ov_codegen.h"
#include "ov_ovmparser.h"

/*
*	Global variables
*/
OV_INT					includepath_ptr = 0;
OV_STRING				includepath[MAX_INCLUDEPATHS];
OV_INT					current_line = 1;
OV_STRING				filename;
OV_INT 					include_stack_ptr = 0;
OV_INT 					includename_stack_ptr = 0;
OV_STRING				filename_stack[MAX_INCLUDE_DEPTH];
OV_STRING				includename_stack[MAX_INCLUDES];
OV_INT					current_line_stack[MAX_INCLUDE_DEPTH];
static YY_BUFFER_STATE	include_stack[MAX_INCLUDE_DEPTH];
static OV_STRINGSTACK	*pstringstack = NULL;
static OV_BOOL			cpp_used = FALSE;
static OV_BOOL			include_used = FALSE;
%}

/*
*   lex definitions
*   ---------------
*/
DIGIT			[0-9]
ALPHA			[a-zA-Z]
NALPHA			[^0-9a-zA-Z]
SPACE			[ \t]+
SPACE_OPT		[ \t]*

VOID			"<void>"
STRING			\"[^\"\n]*\"
YEAR			[12]{DIGIT}{3}
MONTH			("0"[1-9])|("1"[012])
DAY			("0"[1-9])|([12]{DIGIT})|("3"[01])
HOUR			([01]{DIGIT})|("2"[0-3])
MINUTE			[0-5]{DIGIT}
SECOND			{MINUTE}
USEC			{DIGIT}{6}

IDENTIFIER		{ALPHA}({ALPHA}|{DIGIT})*
IDENTIFIER_EX		{IDENTIFIER}\/{IDENTIFIER}
C_IDENTIFIER		\<({ALPHA}|_)({ALPHA}|{DIGIT}|_)*\>
FLAGS			FLAGS{SPACE_OPT}={SPACE_OPT}\"{ALPHA}*\"
COMMENT			\/\/.*\n
INCLUDE			\#include{SPACE_OPT}\"[^\"\n]+\"{SPACE_OPT}\n
CPP_DIRECTIVE		\#{SPACE}{DIGIT}+{SPACE}\"[^ \t\n]+\"({SPACE}{DIGIT}+)?{SPACE_OPT}\n

/*
*   lex rules
*   ---------
*/
%%

"LIBRARY"			return TOK_LIBRARY;
"VERSION"			return TOK_VERSION;
"AUTHOR"			return TOK_AUTHOR;
"COPYRIGHT"			return TOK_COPYRIGHT;
"COMMENT"			return TOK_COMMENT;
"END_LIBRARY"		return TOK_END_LIBRARY;
"STRUCTURE"			return TOK_STRUCTURE;
"END_STRUCTURE"		return TOK_END_STRUCTURE;
"CLASS"				return TOK_CLASS;
"VARIABLES"			return TOK_VARIABLES;
"END_VARIABLES"		return TOK_END_VARIABLES;
"OPERATIONS"		return TOK_OPERATIONS;
"END_OPERATIONS"	return TOK_END_OPERATIONS;
"PARTS"				return TOK_PARTS;
"END_PARTS"			return TOK_END_PARTS;
"END_CLASS"			return TOK_END_CLASS;
"ASSOCIATION"		return TOK_ASSOCIATION;
"PARENT"			return TOK_PARENT;
"CHILD"				return TOK_CHILD;
"END_ASSOCIATION"	return TOK_END_ASSOCIATION;
"C_TYPE"			return TOK_C_TYPE;
"UNIT"				return TOK_UNIT;
"C_FUNCTION"		return TOK_C_FUNCTION;
"INITIALVALUE"		return TOK_INITIALVALUE;

"IS_INSTANTIABLE"	{ yylval.classprops = OV_CP_INSTANTIABLE;	return TOK_CLASSPROPS; }
"IS_FINAL"			{ yylval.classprops = OV_CP_FINAL;			return TOK_CLASSPROPS; }

"IS_LOCAL"			{ yylval.assocprops = OV_AP_LOCAL;		return TOK_ASSOCPROPS; }

"HAS_GET_ACCESSOR"	{ yylval.varprops = OV_VP_GETACCESSOR;	return TOK_VARPROPS; }
"HAS_SET_ACCESSOR"	{ yylval.varprops = OV_VP_SETACCESSOR;	return TOK_VARPROPS; }
"HAS_ACCESSORS"		{ yylval.varprops = OV_VP_ACCESSORS;	return TOK_VARPROPS; }
"IS_DERIVED"		{ yylval.varprops = OV_VP_DERIVED;		return TOK_VARPROPS; }
"IS_STATIC"			{ yylval.varprops = OV_VP_STATIC;		return TOK_VARPROPS; }

"IS_ABSTRACT"		{ yylval.opprops = OV_OP_ABSTRACT;		return TOK_OPPROPS; }

"BOOL"				{ yylval.vartype = OV_VT_BOOL;		return TOK_VARTYPE; }
"INT"				{ yylval.vartype = OV_VT_INT;		return TOK_VARTYPE; }
"UINT"				{ yylval.vartype = OV_VT_UINT;		return TOK_VARTYPE; }
"SINGLE"			{ yylval.vartype = OV_VT_SINGLE;	return TOK_VARTYPE; }
"DOUBLE"			{ yylval.vartype = OV_VT_DOUBLE;	return TOK_VARTYPE; }
"TIME"				{ yylval.vartype = OV_VT_TIME;		return TOK_VARTYPE; }
"TIME_SPAN"			{ yylval.vartype = OV_VT_TIME_SPAN;	return TOK_VARTYPE; }
"STRING"			{ yylval.vartype = OV_VT_STRING;	return TOK_VARTYPE; }
"BOOL_PV"			{ yylval.vartype = OV_VT_BOOL_PV;	return TOK_VARTYPE; }
"INT_PV"			{ yylval.vartype = OV_VT_INT_PV;	return TOK_VARTYPE; }
"UINT_PV"			{ yylval.vartype = OV_VT_UINT_PV;	return TOK_VARTYPE; }
"SINGLE_PV"			{ yylval.vartype = OV_VT_SINGLE_PV;	return TOK_VARTYPE; }
"DOUBLE_PV"			{ yylval.vartype = OV_VT_DOUBLE_PV;	return TOK_VARTYPE; }
"STRING_PV"			{ yylval.vartype = OV_VT_STRING_PV;	return TOK_VARTYPE; }
"TIME_PV"			{ yylval.vartype = OV_VT_TIME_PV;	return TOK_VARTYPE; }
"TIME_SPAN_PV"			{ yylval.vartype = OV_VT_TIME_SPAN_PV;	return TOK_VARTYPE; }
"ANY"				{ yylval.vartype = OV_VT_ANY;		return TOK_VARTYPE; }

"ONE_TO_ONE"		{ yylval.assoctype = OV_AT_ONE_TO_ONE;		return TOK_ASSOCTYPE; }
"ONE_TO_MANY"		{ yylval.assoctype = OV_AT_ONE_TO_MANY;		return TOK_ASSOCTYPE; }
"MANY_TO_MANY"		{ yylval.assoctype = OV_AT_MANY_TO_MANY;	return TOK_ASSOCTYPE; }

"TRUE" {
	yylval.bool = TRUE;
	return TOK_BOOL;
}

"FALSE" {
	yylval.bool = FALSE;
	return TOK_BOOL;
}

{FLAGS}	{
	OV_STRING	pc;
	yylval.uint = 0;
	for(pc=yytext+yyleng-2; *pc != '"'; pc--) {
		if((*pc >= 'a') && (*pc <= 'z')) {
			yylval.uint |= 1 << (*pc-'a');
		} else if((*pc >= 'A') && (*pc <= 'Z')) {
			yylval.uint |= 1 << (*pc-'A');
		}
	}
	return TOK_FLAGS;
}

{YEAR}\/{MONTH}\/{DAY}({SPACE}{HOUR}":"{MINUTE}":"{SECOND}("."{USEC})?)? {
	static char		format[] = "0000/00/00 00:00:00.000000";
	char			*pc1, *pc2;
	struct tm		tm;
	time_t			secs;
	OV_UINT			usecs;

	if(!yytext) {
		yyerror("Error: Bad time format.");
		exit(EXIT_FAILURE);
	}
	for(pc1=format, pc2=yytext; *pc1; pc1++, pc2++) {
		if(!*pc2) {
			if((*pc1 == ' ') || (*pc1 == '.')) {
				break;
			}
			yyerror("Error: Bad time format.");
			exit(EXIT_FAILURE);
		}
		switch(*pc1) {
		case '0':
			if(!((*pc2 >= '0') && (*pc2 <= '9'))) {
				yyerror("Error: Bad time format.");
				exit(EXIT_FAILURE);
			}
			break;
		case '/':
		case ':':
		case ' ':
		case '.':
			if(*pc2 != *pc1) {
				yyerror("Error: Bad time format.");
				exit(EXIT_FAILURE);
			}
			break;
		default:
			yyerror("Error: Bad time format.");
			exit(EXIT_FAILURE);
		}
	}

	memset(&tm, 0, sizeof(tm));
	usecs = 0;
	sscanf(yytext, "%d/%d/%d %d:%d:%d.%lu", &tm.tm_year, &tm.tm_mon,
		&tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec, &usecs);
	tm.tm_year -= 1900;
	tm.tm_mon--;
	secs = mktime(&tm);
	if(secs == -1) {
		yyerror("Error: Bad time.");
		exit(EXIT_FAILURE);
	}
	yylval.time.secs = secs;
	yylval.time.usecs = usecs;
	return TOK_TIME;
} 

[+-]?{HOUR}":"{MINUTE}":"{SECOND}("."{USEC})? {
	static char		format[] = "00:00:00.000000";
	unsigned long		hour, min, sec, usecs;
	char			*pc1, *pc2;
	int			sign;

	if(!yytext) {
		yyerror("Error: Bad time span format.");
		exit(EXIT_FAILURE);
	}
	sign = 1;
	pc2=yytext;
	if ((*pc2) == '-') { sign = -1; pc2++; }
	if ((*pc2) == '+') pc2++;
	for(pc1=format; *pc1; pc1++, pc2++) {
		if(!*pc2) {
			if((*pc1 == ' ') || (*pc1 == '.')) {
				break;
			}
			yyerror("Error: Bad time span format.");
			exit(EXIT_FAILURE);
		}
		switch(*pc1) {
		case '0':
			if(!((*pc2 >= '0') && (*pc2 <= '9'))) {
				yyerror("Error: Bad time span format.");
				exit(EXIT_FAILURE);
			}
			break;
		case ':':
		case ' ':
		case '.':
			if(*pc2 != *pc1) {
				yyerror("Error: Bad time span format.");
				exit(EXIT_FAILURE);
			}
			break;
		default:
			yyerror("Error: Bad time span format.");
			exit(EXIT_FAILURE);
		}
	}

	sscanf(yytext, "%lu:%lu:%lu.%lu", &hour, &min, &sec, &usecs);
	yylval.timespan.secs = sign * (abs(hour) * 3600 + min * 60 + sec);
	yylval.timespan.usecs = sign * usecs;
	return TOK_TIMESPAN;
} 

{STRING} {
	yylval.string = ov_codegen_getstring(yytext, yyleng);
	return TOK_STRING;
}


[+-]?{DIGIT}+(("."{DIGIT}+[eE][+-]?{DIGIT}+)|("."{DIGIT}+)|([eE][+-]?{DIGIT}+)) {
	yylval._double = strtod(yytext, NULL);
	return TOK_REAL;
}

{DIGIT}+ {
	yylval.uint = strtoul(yytext, NULL, 10);
	return TOK_UINT;
}

[+-]?{DIGIT}+ {
	yylval._int = strtol(yytext, NULL, 10);
	return TOK_INT;
}

{IDENTIFIER_EX}	{
	yylval.string = ov_codegen_getstring(yytext, yyleng);
	return TOK_IDENTIFIER_EX;
}

{IDENTIFIER} {
	yylval.string = ov_codegen_getstring(yytext, yyleng);
	return TOK_IDENTIFIER;
}

{C_IDENTIFIER} {
	yylval.string = ov_codegen_getstring(yytext+1, yyleng-2);
	return TOK_C_IDENTIFIER;
}

\/\*				BEGIN(comment_state);
<comment_state>.	/* ignore */
<comment_state>\n	current_line++;
<comment_state>\*\/	BEGIN(INITIAL);
<comment_state>\/\*	yyerror("Warning: \"/*\" within comment");
{COMMENT}			current_line++;

{CPP_DIRECTIVE}	{
	if(include_used) {
		yyerror("Warning: ignoring cpp-directive");
	} else {
		OV_STRING	fname = (OV_STRING)ov_codegen_malloc(yyleng);
		OV_UINT 	line = 0;
		OV_UINT 	flag = 0;
		cpp_used = TRUE;
		sscanf(yytext, "#%ld%s%ld", &line, fname, &flag);
		switch(flag) {
			case 1:
				if(include_stack_ptr >= MAX_INCLUDE_DEPTH) {
					yyerror("Includes nested too deeply");
					exit(EXIT_FAILURE);
				}
				filename_stack[include_stack_ptr] = filename;
				current_line_stack[include_stack_ptr]
					= current_line;
				include_stack_ptr++;
				break;
			case 2:
				if(include_stack_ptr) {
					include_stack_ptr--;
				}
				break;
			default:
				break;
		}
		filename = ov_codegen_getstring(fname+1, strlen(fname)-2);
		ov_codegen_free(fname);
		current_line = line+1;
	}
}

{INCLUDE} {
	if(cpp_used) {
		yyerror("Warning: ignoring include-directive");
	} else {
		OV_STRING	fname = (OV_STRING)ov_codegen_malloc(yyleng);
		char* includefilename;
		int i;
		include_used = TRUE;
		sscanf(yytext, "#include %s", fname);
		if(includename_stack_ptr >= MAX_INCLUDES) {
			yyerror("To many Includes");
			exit(EXIT_FAILURE);
		}
		if(include_stack_ptr >= MAX_INCLUDE_DEPTH) {
			yyerror("Includes nested too deeply");
			exit(EXIT_FAILURE);
		}
		includefilename = ov_codegen_getstring(fname+1, strlen(fname)-2);
		ov_codegen_free(fname);
		i = includename_stack_ptr;
		while (i>0) {
			if (strcmp(includefilename, includename_stack[i-1])==0) break;
			i--;
		}
		if (i==0) {
			yyin = fopen(includefilename, "r");
			while((!yyin) && (i<includepath_ptr)) {
				fname = (OV_STRING)ov_codegen_malloc(strlen(includepath[i])+
					strlen(includefilename)+2);
				sprintf(fname, "%s" OV_DIRPATHDELIMITER "%s", includepath[i],
					includefilename);
				i++;
				yyin = fopen(fname, "r");
				ov_codegen_free(fname);
			}
			if(!yyin) {
				yyerror("include file not found");
				exit(EXIT_FAILURE);
			}
			include_stack[include_stack_ptr] = YY_CURRENT_BUFFER;
			includename_stack[includename_stack_ptr] = includefilename;
			filename_stack[include_stack_ptr] = filename;
			current_line_stack[include_stack_ptr] = current_line;
			include_stack_ptr++;
			includename_stack_ptr++;
			filename = includefilename;
			current_line = 1;
			yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
		}
	}
}

<<EOF>>	{
	if(cpp_used) {
		yyterminate();
	} else {
		if(--include_stack_ptr < 0) {
			yyterminate();
		} else {
			fclose(yyin);
			yy_delete_buffer(YY_CURRENT_BUFFER);
			yy_switch_to_buffer(include_stack[include_stack_ptr]);
			filename = filename_stack[include_stack_ptr];
			current_line = current_line_stack[include_stack_ptr]+1;
		}
	}
}

[ \t]*				/* ignore */

\n					current_line++;

.					return *yytext;

%%

/*
*   supporting C functions
*   ----------------------
*/

/*	----------------------------------------------------------------------	*/

/*
*	This functions is needed with some versions of flex
*/
int yywrap(void) {
	return 1;
}

/*	----------------------------------------------------------------------	*/

/*
*	Allocate memory on the heap and abort if we get no memory
*/
OV_POINTER ov_codegen_malloc(
	OV_UINT		size
) {
	/*
	*	local variables
	*/
	OV_POINTER	ptr = malloc(size);
	if(!ptr) {
		fprintf(stderr, "Could not allocate memory on the heap.\n");
		ov_codegen_cleanup();
		exit(EXIT_FAILURE);
	}
	return ptr;
}

/*	----------------------------------------------------------------------	*/

/*
*	Free memory allocated on the heap
*/
void ov_codegen_free(
	OV_POINTER	ptr
) {
	free(ptr);
}

/*	----------------------------------------------------------------------	*/

/*
*	Allocate memory for a string using ov_codegen_malloc()
*/
OV_STRING ov_codegen_getstring(
	const OV_STRING		string,
	const OV_UINT		length
) {
	/*
	*	local variables
	*/
	OV_STRINGSTACK *pentry = Ov_Codegen_Malloc(struct OV_STRINGSTACK);
	/*
	*	enter the new entry
	*/
	pentry->string = (OV_STRING)ov_codegen_malloc(length+1);
	pentry->pnext = pstringstack;
	pstringstack = pentry;
	/*
	*	copy the string and terminate it with zero
	*/
	strncpy(pentry->string, string, length);
	pentry->string[length] = 0;
	/*
	*	finished
	*/
	return pentry->string;
}

/*	----------------------------------------------------------------------	*/

/*
*	Free strings allocated by the scanner
*/
void ov_codegen_freestrings(void) {
	/*
	*	local variables
	*/
	OV_STRINGSTACK *pentry, *pentry2;
	/*
	*	free all entries
	*/
	pentry = pstringstack;
	while(pentry) {
		ov_codegen_free(pentry->string);
		pentry2 = pentry->pnext;
		ov_codegen_free(pentry);
		pentry = pentry2;
	}
	/*
	*	reset string stack pointer
	*/
	pstringstack = NULL;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/
