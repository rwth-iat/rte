/*
*   $Id: ov_expression_scanner.lex,v 1.1 2000-02-10 13:07:01 dirk Exp $
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
*	27-Oct-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

%{
#include "ov_expression.h"
#include "ov_expression_parser.h"

#include <math.h>

/*
*	get value using a function
*/
static OV_DOUBLE _getvalue_acos(const OV_EXPRESSION *pexpr);
static OV_DOUBLE _getvalue_asin(const OV_EXPRESSION *pexpr);
static OV_DOUBLE _getvalue_atan(const OV_EXPRESSION *pexpr);
static OV_DOUBLE _getvalue_ceil(const OV_EXPRESSION *pexpr);
static OV_DOUBLE _getvalue_cos(const OV_EXPRESSION *pexpr);
static OV_DOUBLE _getvalue_cosh(const OV_EXPRESSION *pexpr);
static OV_DOUBLE _getvalue_exp(const OV_EXPRESSION *pexpr);
static OV_DOUBLE _getvalue_floor(const OV_EXPRESSION *pexpr);
static OV_DOUBLE _getvalue_sin(const OV_EXPRESSION *pexpr);
static OV_DOUBLE _getvalue_sinh(const OV_EXPRESSION *pexpr);
static OV_DOUBLE _getvalue_sqrt(const OV_EXPRESSION *pexpr);
static OV_DOUBLE _getvalue_tan(const OV_EXPRESSION *pexpr);
static OV_DOUBLE _getvalue_tanh(const OV_EXPRESSION *pexpr);

static OV_DOUBLE _getvalue_abs(const OV_EXPRESSION *pexpr);	/* fabs */
static OV_DOUBLE _getvalue_ln(const OV_EXPRESSION *pexpr);	/* log */
static OV_DOUBLE _getvalue_log(const OV_EXPRESSION *pexpr);	/* log10 */
%}

/*	----------------------------------------------------------------------	*/

/*
*	Lex Declarations
*/

DIGIT	[0-9]
ALPHA	[a-zA-Z]
INT		{DIGIT}+
FLOAT	{INT}("."{INT})?([eE]("-")?{INT})?
SYMBOL	{ALPHA}({ALPHA}|{DIGIT})*
SPACE	[ \t\n]+

%%
"acos"	{
	ov_expression_lval.fnc = _getvalue_acos;
	return TOK_FUNCTION;
}

"asin"	{
	ov_expression_lval.fnc = _getvalue_asin;
	return TOK_FUNCTION;
}

"atan"	{
	ov_expression_lval.fnc = _getvalue_atan;
	return TOK_FUNCTION;
}

"ceil"	{
	ov_expression_lval.fnc = _getvalue_ceil;
	return TOK_FUNCTION;
}

"cos"	{
	ov_expression_lval.fnc = _getvalue_cos;
	return TOK_FUNCTION;
}

"cosh"	{
	ov_expression_lval.fnc = _getvalue_cosh;
	return TOK_FUNCTION;
}

"exp"	{
	ov_expression_lval.fnc = _getvalue_exp;
	return TOK_FUNCTION;
}

"floor"	{
	ov_expression_lval.fnc = _getvalue_floor;
	return TOK_FUNCTION;
}

"sin"	{
	ov_expression_lval.fnc = _getvalue_sin;
	return TOK_FUNCTION;
}

"sinh"	{
	ov_expression_lval.fnc = _getvalue_sinh;
	return TOK_FUNCTION;
}

"sqrt"	{
	ov_expression_lval.fnc = _getvalue_sqrt;
	return TOK_FUNCTION;
}

"tan"	{
	ov_expression_lval.fnc = _getvalue_tan;
	return TOK_FUNCTION;
}

"tanh"	{
	ov_expression_lval.fnc = _getvalue_tanh;
	return TOK_FUNCTION;
}

"abs"	{
	ov_expression_lval.fnc = _getvalue_abs;
	return TOK_FUNCTION;
}

"ln"	{
	ov_expression_lval.fnc = _getvalue_ln;
	return TOK_FUNCTION;
}

"log"	{
	ov_expression_lval.fnc = _getvalue_log;
	return TOK_FUNCTION;
}

"pi"	{
	ov_expression_lval.floatval = 3.14159265358979323846;
	return TOK_FLOAT;
}

"e"	{
	ov_expression_lval.floatval = 2.71828182845904523536;
	return TOK_FLOAT;
}

"t"	{
	return TOK_TIME;
}

{FLOAT}		{
	ov_expression_lval.floatval = strtod(yytext, NULL);
	return TOK_FLOAT;
}

{SYMBOL}	{
	ov_expression_lval.stringval = yytext;
	return TOK_SYMBOL;
}

"<>"	{ return TOK_NOTEQ; }
"<="	{ return TOK_LESSEQ; }
">="	{ return TOK_GRTREQ; }

{SPACE}	{}
.		{ return *yytext; }

%%

/*	----------------------------------------------------------------------	*/

/*
*	supporting C code
*/
int yywrap(void) {
	return 1;
}

/*	----------------------------------------------------------------------	*/

/*
*	macros for implementing functions
*/
#define IMPL_FNC(name)	IMPL_FNC2(name, name)

#define IMPL_FNC2(name, cfnc) 						\
	static OV_DOUBLE _getvalue_##name(				\
		const OV_EXPRESSION *pexpr					\
	) {												\
		return cfnc(pexpr->exprunion.parg->getvalue	\
			(pexpr->exprunion.parg));				\
	}

/*	----------------------------------------------------------------------	*/

/*
*	get value using a function
*/
IMPL_FNC(acos)
IMPL_FNC(asin)
IMPL_FNC(atan)
IMPL_FNC(ceil)
IMPL_FNC(cos)
IMPL_FNC(cosh)
IMPL_FNC(exp)
IMPL_FNC(floor)
IMPL_FNC(sin)
IMPL_FNC(sinh)
IMPL_FNC(sqrt)
IMPL_FNC(tan)
IMPL_FNC(tanh)

IMPL_FNC2(abs, fabs)
IMPL_FNC2(ln, log)
IMPL_FNC2(log, log10)

/*
*	End of file
*/

