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

/* Scanner definition for ACPLT/OV text file parser */

/* Author : Christian Poensgen <chris@plt.rwth-aachen.de> */
/* db.lex */
/* last change: Mar 21, 2002 */

/* ACPLT/OV Modelling Language */

/*
*   lex C definitions
*   -----------------
*/
%{
#include "dbparse.h"			/* needed for type definitions */

#include "db_y.h"
%}

/*
*   lex definitions
*   ---------------
*/

digit			[0-9]
alpha			[a-zA-Z]
path_identifier	({alpha}|"_")({alpha}|{digit}|"_")*
string			\"[^\"]*\"
year			[12]{digit}{3}
month			("0"[1-9])|("1"[012])
day				("0"[1-9])|([12]{digit})|("3"[01])
hour			([01]{digit})|("2"[0-3])
hours			{digit}{4}
minute			[0-5]{digit}
second			{minute}
usec			{digit}{6}

/*
*   lex rules
*   ---------
*/

/* components of yylval pass value to parser */
%%
"INSTANCE"	{
		return(INSTANCE);
	}
"CLASS"	{
		return(CLASS);
	}
"CREATION_TIME"	{
		return(CREATION_TIME);
	}
"FLAGS"	{
		return(FLAGS);
	}
"COMMENT"	{
		return(COMMENT);
	}
"VARIABLE_VALUES"	{
		return(VARIABLE_VALUES);
	}
"END_VARIABLE_VALUES"	{
		return(END_VARIABLE_VALUES);
	}
"LINK_VALUES"	{
		return(LINK_VALUES);
	}
"END_LINK_VALUES"	{
		return(END_LINK_VALUES);
	}
"PART_INSTANCES"	{
		return(PART_INSTANCES);
	}
"PART_INSTANCE"	{
		return(PART_INSTANCE);
	}
"END_PART_INSTANCES"	{
		return(END_PART_INSTANCES);
	}
"END_PART_INSTANCE"	{
		return(END_PART_INSTANCE);
	}
"END_INSTANCE"	{
		return(END_INSTANCE);
	}
"TIME"	{
		return(TIME);
	}
"STATE"	{
		return(STATE);
	}
"UNIT"	{
		return(UNIT);
	}
"TRUE"	{
		yylval.boolval = true;
		return(BOOL_VALUE);
	}
"FALSE"	{
		yylval.boolval = false;
		return(BOOL_VALUE);
	}
"GOOD"	{
		yylval.stateval = KS_ST_GOOD;
		return(STATE_VALUE);
	}
"QUESTIONABLE"	{
		yylval.stateval = KS_ST_QUESTIONABLE;
		return(STATE_VALUE);
	}
"BAD"	{
		yylval.stateval = KS_ST_BAD;
		return(STATE_VALUE);
	}
"UNKNOWN"	{
		yylval.stateval = KS_ST_UNKNOWN;
		return(STATE_VALUE);
	}
"NOT_SUPPORTED"	{
		yylval.stateval = KS_ST_NOTSUPPORTED;
		return(STATE_VALUE);
	}
"<void>"	{
		yylval.pvoidval = NULL;
		return(VOID_VALUE);
	}
"/"|("/"[.]?{path_identifier}([./]{path_identifier})*)	{
		yylval.ppathval = new(LogPath);
		*yylval.ppathval = LogPath(yytext);
		return(PATH);
	}
{year}"/"{month}"/"{day}(" "{hour}":"{minute}":"{second}("."{usec})?)?	{
		yylval.ptimeval = new(KsTime);
		*yylval.ptimeval = GetTime(yytext);
		return(TIME_VALUE);
	}
[+-]?{hours}":"{minute}":"{second}("."{usec})?	{
		yylval.ptimespanval = new(KsTimeSpan);
		*yylval.ptimespanval = (KsTimeSpan)GetTimeSpan(yytext);
		return(TIME_SPAN_VALUE);
	}
[+-]?{digit}+(("."{digit}+[eE][+-]?{digit}+)|("."{digit}+)|([eE][+-]?{digit}+))		{
		yylval.doubleval = strtod(yytext, NULL);
		return(DOUBLE_VALUE);
	}
[+-]?{digit}+	{
		yylval.intval = strtoul(yytext, NULL, 10);
		return(INT_VALUE);
	}
{alpha}({alpha}|{digit}|"_")*	{
		yylval.pstringval = new(KsString);
		*yylval.pstringval = KsString(yytext);
		return(IDENTIFIER);
	}
{string}	{
		yytext[strlen(yytext)-1] = '\0';
		yylval.pstringval = new(KsString);
		*(yylval.pstringval) = KsString(&(yytext[1]));
		return(STRING_VALUE);
	}
[ \t\n]*	{	/* white space ignored */
	}
.	{	/* default */
		return(*yytext);
	}

%%

/*
*   supporting C functions
*   ----------------------
*/

/*
*	This function is needed with some versions of flex
*/
int yywrap(void) {
	return 1;
}
