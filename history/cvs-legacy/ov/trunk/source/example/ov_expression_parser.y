/*
*   $Id: ov_expression_parser.y,v 1.1 2000-02-10 13:07:00 dirk Exp $
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
#include "libov/ov_time.h"
#include "libov/ov_macros.h"

#include "ov_expression.h"

#include <math.h>

static OV_INSTPTR_ov_object	_pobj;
static OV_EXPRESSION		*_pexpr;
static OV_EXPRESSION		*_pfirstexpr;
static OV_DOUBLE			_time;

/*
*	get value using an operator
*/
static OV_DOUBLE _getvalue_or(const OV_EXPRESSION *pexpr);		/* expr1 | expr2 */
static OV_DOUBLE _getvalue_and(const OV_EXPRESSION *pexpr);		/* expr1 & expr2 */
static OV_DOUBLE _getvalue_equal(const OV_EXPRESSION *pexpr);	/* expr1 = expr2 */
static OV_DOUBLE _getvalue_noteq(const OV_EXPRESSION *pexpr);	/* expr1 <> expr2 */
static OV_DOUBLE _getvalue_less(const OV_EXPRESSION *pexpr);	/* expr1 < expr2 */
static OV_DOUBLE _getvalue_lesseq(const OV_EXPRESSION *pexpr);	/* expr1 <= expr2 */
static OV_DOUBLE _getvalue_grtreq(const OV_EXPRESSION *pexpr);	/* expr1 >= expr2 */
static OV_DOUBLE _getvalue_greater(const OV_EXPRESSION *pexpr);	/* expr1 > expr2 */
static OV_DOUBLE _getvalue_plus(const OV_EXPRESSION *pexpr);	/* expr1 + expr2 */
static OV_DOUBLE _getvalue_minus(const OV_EXPRESSION *pexpr);	/* expr1 - expr2 */
static OV_DOUBLE _getvalue_mul(const OV_EXPRESSION *pexpr);		/* expr1 * expr2 */
static OV_DOUBLE _getvalue_div(const OV_EXPRESSION *pexpr);		/* expr1 / expr2 */
static OV_DOUBLE _getvalue_mod(const OV_EXPRESSION *pexpr);		/* fmod: expr1 % expr2 */
static OV_DOUBLE _getvalue_pow(const OV_EXPRESSION *pexpr);		/* pow: expr1 ^ expr2 */

/*
*	create a new expression
*/
static OV_EXPRESSION *_new_value(
	OV_DOUBLE value
);

static OV_EXPRESSION *_new_pvalue(
	OV_INSTPTR_ov_object	pobj,
	OV_STRING				portname
);

static OV_EXPRESSION *_new_time(void);

static OV_EXPRESSION *_new_function(
	OV_DOUBLE		(* fnc)(const OV_EXPRESSION *pexpr),
	OV_EXPRESSION	*parg
);

static OV_EXPRESSION *_new_operator(
	OV_DOUBLE		(* fnc)(const OV_EXPRESSION *pexpr),
	OV_EXPRESSION	*pexpr1,
	OV_EXPRESSION	*pexpr2
);

static OV_EXPRESSION *_new_negation(
	OV_EXPRESSION *pexpr
);

static OV_EXPRESSION *_new_not(
	OV_EXPRESSION *pexpr
);
%}

/*	----------------------------------------------------------------------	*/

/*
*	BISON Declarations
*/
%union {
	OV_DOUBLE		(* fnc)(const OV_EXPRESSION *pexpr);
	OV_STRING		stringval;
	OV_DOUBLE		floatval;
	OV_EXPRESSION	*pexpr;
}

%token				TOK_NOTEQ TOK_LESSEQ TOK_GRTREQ

%token				TOK_TIME
%token <fnc>		TOK_FUNCTION
%token <floatval>	TOK_FLOAT
%token <stringval>	TOK_SYMBOL

%left '|'			/* or */
%left '&'			/* and */
%left '=' TOK_NOTEQ
%left '<' TOK_LESSEQ TOK_GRTREQ '>'
%left '+' '-'
%left '*' '/' '%'
%left TOK_NEG		/* negation: unary plus, minus, not */
%right '^'			/* exponentiation */

%type <pexpr> expr

%%
start:	/* empty */					{ _pexpr = NULL; }
		| expr						{ _pexpr = $1; }
;
expr:	TOK_FLOAT					{ $$ = _new_value($1); }
		| TOK_TIME					{ $$ = _new_time(); }
		| TOK_SYMBOL				{ $$ = _new_pvalue(_pobj, $1); }
		| TOK_FUNCTION '(' expr ')'	{ $$ = _new_function($1, $3); }
		| expr '|' expr				{ $$ = _new_operator(_getvalue_or, $1, $3); }
		| expr '&' expr				{ $$ = _new_operator(_getvalue_and, $1, $3); }
		| expr '=' expr				{ $$ = _new_operator(_getvalue_equal, $1, $3); }
		| expr TOK_NOTEQ expr		{ $$ = _new_operator(_getvalue_noteq, $1, $3); }
		| expr '<' expr				{ $$ = _new_operator(_getvalue_less, $1, $3); }
		| expr TOK_LESSEQ expr		{ $$ = _new_operator(_getvalue_lesseq, $1, $3); }
		| expr TOK_GRTREQ expr		{ $$ = _new_operator(_getvalue_grtreq, $1, $3); }
		| expr '>' expr				{ $$ = _new_operator(_getvalue_greater, $1, $3); }
		| expr '+' expr				{ $$ = _new_operator(_getvalue_plus, $1, $3); }
		| expr '-' expr				{ $$ = _new_operator(_getvalue_minus, $1, $3); }
		| expr '*' expr				{ $$ = _new_operator(_getvalue_mul, $1, $3); }
		| expr '/' expr				{ $$ = _new_operator(_getvalue_div, $1, $3); }
		| expr '%' expr				{ $$ = _new_operator(_getvalue_mod, $1, $3); }
		| '+' expr %prec TOK_NEG	{ $$ = $2; }
		| '-' expr %prec TOK_NEG	{ $$ = _new_negation($2); }
		| '!' expr %prec TOK_NEG	{ $$ = _new_not($2); }
		| expr '^' expr				{ $$ = _new_operator(_getvalue_pow, $1, $3); }
		| '(' expr ')'				{ $$ = $2; }
;
%%

/*	----------------------------------------------------------------------	*/

/*
*	supporting C code
*/

#include "libov/ov_macros.h"

#ifdef const
#undef const
#endif

/*	----------------------------------------------------------------------	*/

/*
*	if an error occurs, delete the created expr objects and finish
*/
int yyerror(
	char *msg
) {
	/*
	*	local variables
	*/
	OV_EXPRESSION *pexpr, *pnextexpr;
	/*
	*	instructions
	*/
	for(pexpr=_pfirstexpr; pexpr; pexpr=pnextexpr) {
		pnextexpr = pexpr->pnext;
		Ov_HeapFree(pexpr);
	}
	_pfirstexpr = NULL;
	return 1;
}

/*	----------------------------------------------------------------------	*/

/*
*	create a parse tree of an expression
*/
OV_EXPRESSION *ov_expression_create(
	OV_INSTPTR_ov_object	pobj,
	const OV_STRING			exprstr
) {
	_pobj = pobj;
	_pfirstexpr = NULL;
	ov_expression__scan_string(exprstr);
	if(ov_expression_parse()) {
		return NULL;
	}
	return _pexpr;
}

/*	----------------------------------------------------------------------	*/

/*
*	delete a parse tree of an expression
*/
void ov_expression_delete(
	OV_EXPRESSION *pexpr
) {
	if(pexpr) {
		pexpr->delete(pexpr);
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	evaluate an expression 
*/
OV_DOUBLE ov_expression_evaluate(
	const OV_EXPRESSION	*pexpr,
	const OV_TIME		*ptime
) {
	/*
	*	local variables
	*/
	OV_TIME	time;
	/*
	*	instructions
	*/
	if(ptime) {
		time = *ptime;
	} else {
		ov_time_gettime(&time);
	}
	Ov_TimeToDouble(time, _time);
	return (pexpr)?(pexpr->getvalue(pexpr)):(0.0);
}

/*	----------------------------------------------------------------------	*/

/*
*	add an expression to a linked list
*/
static OV_EXPRESSION *_addexpr(void) {
	/*
	*	local variables
	*/
	OV_EXPRESSION *pexpr = Ov_HeapAlloc(OV_EXPRESSION);
	/*
	*	instructions
	*/
	if(pexpr) {
		pexpr->pnext = _pfirstexpr;
		_pfirstexpr = pexpr;
	}
	return pexpr;
}

/*	----------------------------------------------------------------------	*/

/*
*	remove an expression from a linked list
*/
static void _removeexpr(
	OV_EXPRESSION *pexpr
) {
	/*
	*	local variables
	*/
	OV_EXPRESSION *pcurr, *plast;
	/*
	*	instructions
	*/
	for(plast=NULL, pcurr=_pfirstexpr; pcurr; plast=pcurr, pcurr=pcurr->pnext) {
		if(pcurr == pexpr) {
			if(plast) {
				plast->pnext = pcurr->pnext;
			} else {
				_pfirstexpr = pcurr->pnext;
			}
			Ov_HeapFree(pcurr);
			return;
		}
	}
	Ov_Warning("internal error");
}

/*	----------------------------------------------------------------------	*/

/*
*	get value using a numerical value
*/
static OV_DOUBLE _getvalue_value(
	const OV_EXPRESSION *pexpr
) {
	return pexpr->exprunion.value;
}

/*	----------------------------------------------------------------------	*/

/*
*	macros for implementing geting value using a pointer
*/
#define IMPL_GETVALUE_PVALUE(type, TYPE)							\
	static OV_DOUBLE _getvalue_p##type(								\
		const OV_EXPRESSION *pexpr									\
	) {																\
		return *(OV_##TYPE*)pexpr->exprunion.pvalue;				\
	}

#define IMPL_GETVALUE_PVALUE2(type, TYPE)							\
	static OV_DOUBLE _getvalue_p##type(								\
		const OV_EXPRESSION *pexpr									\
	) {																\
		OV_DOUBLE	dbl;											\
		Ov_TimeToDouble(*(OV_##TYPE*)pexpr->exprunion.pvalue, dbl);	\
		return dbl;													\
	}

IMPL_GETVALUE_PVALUE(bool, BOOL)
IMPL_GETVALUE_PVALUE(int, INT)
IMPL_GETVALUE_PVALUE(uint, UINT)
IMPL_GETVALUE_PVALUE(single, SINGLE)
IMPL_GETVALUE_PVALUE(double, DOUBLE)

IMPL_GETVALUE_PVALUE2(time, TIME)
IMPL_GETVALUE_PVALUE2(timespan, TIME_SPAN)

/*	----------------------------------------------------------------------	*/

/*
*	get time value
*/
static OV_DOUBLE _getvalue_time(
	const OV_EXPRESSION *pexpr
) {
	return _time;
}

/*	----------------------------------------------------------------------	*/

/*
*	get value using negation 
*/
static OV_DOUBLE _getvalue_negation(
	const OV_EXPRESSION *pexpr
) {
	return -pexpr->exprunion.pexpr->getvalue(pexpr->exprunion.pexpr);
}

/*	----------------------------------------------------------------------	*/

/*
*	get value using "not"
*/
static OV_DOUBLE _getvalue_not(
	const OV_EXPRESSION *pexpr
) {
	return !pexpr->exprunion.pexpr->getvalue(pexpr->exprunion.pexpr);
}

/*	----------------------------------------------------------------------	*/

/*
*	delete a given expression using a value
*/
static void _delete_value(
	OV_EXPRESSION *pexpr
) {
	Ov_HeapFree(pexpr);
}

/*	----------------------------------------------------------------------	*/

/*
*	delete a given expression using a pointer to value
*/
static void _delete_pvalue(
	OV_EXPRESSION *pexpr
) {
	Ov_HeapFree(pexpr);
}

/*	----------------------------------------------------------------------	*/

/*
*	delete a given time expression
*/
static void _delete_time(
	OV_EXPRESSION *pexpr
) {
	Ov_HeapFree(pexpr);
}
	
/*	----------------------------------------------------------------------	*/

/*
*	delete a given expression using a function
*/
static void _delete_function(
	OV_EXPRESSION *pexpr
) {
	pexpr->exprunion.parg->delete(pexpr->exprunion.parg);
	Ov_HeapFree(pexpr);
}

/*	----------------------------------------------------------------------	*/

/*
*	delete a given expression using an operator
*/
static void _delete_operator(
	OV_EXPRESSION *pexpr
) {
	pexpr->exprunion.operator.pexpr1->delete(pexpr->exprunion.operator.pexpr1);
	pexpr->exprunion.operator.pexpr2->delete(pexpr->exprunion.operator.pexpr2);
	Ov_HeapFree(pexpr);
}

/*	----------------------------------------------------------------------	*/

/*
*	delete a given expression using negation
*/
static void _delete_negation(
	OV_EXPRESSION *pexpr
) {
	pexpr->exprunion.parg->delete(pexpr->exprunion.pexpr);
	Ov_HeapFree(pexpr);
}

/*	----------------------------------------------------------------------	*/

/*
*	delete a given expression using "not"
*/
static void _delete_not(
	OV_EXPRESSION *pexpr
) {
	pexpr->exprunion.parg->delete(pexpr->exprunion.pexpr);
	Ov_HeapFree(pexpr);
}

/*	----------------------------------------------------------------------	*/

/*
*	create a new expression using a numerical value
*/
static OV_EXPRESSION *_new_value(
	OV_DOUBLE value
) {
	/*
	*	local variables
	*/
	OV_EXPRESSION *pexpr = _addexpr();
	/*
	*	instructions
	*/
	if(pexpr) {
		pexpr->getvalue = _getvalue_value;
		pexpr->delete = _delete_value;
		pexpr->exprunion.value = value;
	}
	return pexpr;
}

/*	----------------------------------------------------------------------	*/

/*
*	create a new expression using a pointer to a value
*/
static OV_EXPRESSION *_new_pvalue(
	OV_INSTPTR_ov_object	pobj,
	OV_STRING				portname
) {
	/*
	*	local variables
	*/
	OV_EXPRESSION	*pexpr;
	OV_ELEMENT		parent, child;
	OV_DOUBLE		(* getvalue)(const OV_EXPRESSION *pexpr);
	/*
	*	instructions
	*/
	parent.elemtype = OV_ET_OBJECT;
	parent.pobj = pobj;
	if(Ov_Fail(ov_element_searchpart(&parent, &child, OV_ET_VARIABLE, portname))) {
		return NULL;
	}
	if(child.elemtype != OV_ET_VARIABLE) {
		return NULL;
	}
	if((child.elemunion.pvar->v_veclen != 1)
		|| (child.elemunion.pvar->v_varprops & (OV_VP_DERIVED | OV_VP_STATIC))
	) {
		return NULL;
	}
	switch(child.elemunion.pvar->v_vartype) {
	case OV_VT_BOOL:
		getvalue = _getvalue_pbool;
		break;
	case OV_VT_INT:
		getvalue = _getvalue_pint;
		break;
	case OV_VT_UINT:
		getvalue = _getvalue_puint;
		break;
	case OV_VT_SINGLE:
		getvalue = _getvalue_psingle;
		break;
	case OV_VT_DOUBLE:
		getvalue = _getvalue_pdouble;
		break;
	case OV_VT_TIME:
		getvalue = _getvalue_ptime;
		break;
	case OV_VT_TIME_SPAN:
		getvalue = _getvalue_ptimespan;
		break;
	default:
		return NULL;
	}
	pexpr = _addexpr();
	if(pexpr) {
		pexpr->getvalue = getvalue;
		pexpr->delete = _delete_pvalue;
		pexpr->exprunion.pvalue = child.pvalue;
	}
	return pexpr;
}

/*	----------------------------------------------------------------------	*/

/*
*	create a new time expression
*/
static OV_EXPRESSION *_new_time(void) {
	/*
	*	local variables
	*/
	OV_EXPRESSION *pexpr;
	/*
	*	instructions
	*/
	pexpr = _addexpr();
	if(pexpr) {
		pexpr->getvalue = _getvalue_time;
		pexpr->delete = _delete_time;
	}
	return pexpr;
}

/*	----------------------------------------------------------------------	*/

/*
*	create a new expression using a function
*/
static OV_EXPRESSION *_new_function(
	OV_DOUBLE		(* fnc)(const OV_EXPRESSION *pexpr),
	OV_EXPRESSION	*parg
) {
	/*
	*	local variables
	*/
	OV_EXPRESSION *pexpr;
	/*
	*	instructions
	*/
	if(!parg) {
		return NULL;
	}
	pexpr = _addexpr();
	if(pexpr) {
		pexpr->exprunion.parg = parg;
		if(parg->getvalue == _getvalue_value) {
			pexpr->exprunion.value = fnc(pexpr);
			pexpr->getvalue = _getvalue_value;
			pexpr->delete = _delete_value;
			_removeexpr(parg);
		} else {
			pexpr->getvalue = fnc;
			pexpr->delete = _delete_function;
		}
	}
	return pexpr;
}

/*	----------------------------------------------------------------------	*/

/*
*	create a new expression using a binary operator
*/
static OV_EXPRESSION *_new_operator(
	OV_DOUBLE		(* fnc)(const OV_EXPRESSION *pexpr),
	OV_EXPRESSION	*pexpr1,
	OV_EXPRESSION	*pexpr2
) {
	/*
	*	local variables
	*/
	OV_EXPRESSION *pexpr = _addexpr();
	/*
	*	instructions
	*/
	if(!pexpr1 || !pexpr2) {
		return NULL;
	}
	if(pexpr) {
		pexpr->exprunion.operator.pexpr1 = pexpr1;
		pexpr->exprunion.operator.pexpr2 = pexpr2;
		if((pexpr1->getvalue == _getvalue_value)
			&& (pexpr2->getvalue == _getvalue_value)
		) {
			pexpr->exprunion.value = fnc(pexpr);
			pexpr->getvalue = _getvalue_value;
			pexpr->delete = _delete_value;
			_removeexpr(pexpr1);
			_removeexpr(pexpr2);
		} else {
			pexpr->getvalue = fnc;
			pexpr->delete = _delete_operator;
		}
	}
	return pexpr;
}

/*	----------------------------------------------------------------------	*/

/*
*	create a new expression using negation
*/
static OV_EXPRESSION *_new_negation(
	OV_EXPRESSION *pexpr
) {
	/*
	*	local variables
	*/
	OV_EXPRESSION *pexpr2;
	/*
	*	instructions
	*/
	if(pexpr->getvalue == _getvalue_value) {
		pexpr2 = pexpr;
		pexpr2->exprunion.value = -pexpr->exprunion.value;
	} else {
		pexpr2 = _addexpr();
		if(pexpr2) {
			pexpr2->getvalue = _getvalue_negation;
			pexpr2->delete = _delete_negation;
			pexpr2->exprunion.pexpr = pexpr;
		}
	}
	return pexpr2;
}

/*	----------------------------------------------------------------------	*/

/*
*	create a new expression using "not"
*/
static OV_EXPRESSION *_new_not(
	OV_EXPRESSION *pexpr
) {
	/*
	*	local variables
	*/
	OV_EXPRESSION *pexpr2;
	/*
	*	instructions
	*/
	if(pexpr->getvalue == _getvalue_value) {
		pexpr2 = pexpr;
		pexpr2->exprunion.value = !pexpr->exprunion.value;
	} else {
		pexpr2 = _addexpr();
		if(pexpr2) {
			pexpr2->getvalue = _getvalue_not;
			pexpr2->delete = _delete_not;
			pexpr2->exprunion.pexpr = pexpr;
		}
	}
	return pexpr2;
}

/*	----------------------------------------------------------------------	*/

/*
*	macros for implementing operators
*/
#define IMPL_OP(name, op) 									\
	static OV_DOUBLE _getvalue_##name(						\
		const OV_EXPRESSION *pexpr							\
	) {														\
		return ((pexpr->exprunion.operator.pexpr1			\
			->getvalue(pexpr->exprunion.operator.pexpr1))	\
			op (pexpr->exprunion.operator.pexpr2			\
			->getvalue(pexpr->exprunion.operator.pexpr2)));	\
	}

#define IMPL_OP2(name, opfnc) 								\
	static OV_DOUBLE _getvalue_##name(						\
		const OV_EXPRESSION *pexpr							\
	) {														\
		return opfnc(pexpr->exprunion.operator.pexpr1		\
			->getvalue(pexpr->exprunion.operator.pexpr1),	\
			pexpr->exprunion.operator.pexpr2				\
			->getvalue(pexpr->exprunion.operator.pexpr2));	\
	}

/*	----------------------------------------------------------------------	*/

/*
*	get value using an operator
*/
IMPL_OP(or, ||)
IMPL_OP(and, &&)
IMPL_OP(equal, ==)
IMPL_OP(noteq, !=)
IMPL_OP(less, <)
IMPL_OP(lesseq, <=)
IMPL_OP(grtreq, >=)
IMPL_OP(greater, >)
IMPL_OP(plus, +)
IMPL_OP(minus, -)
IMPL_OP(mul, *)
IMPL_OP(div, /)

IMPL_OP2(mod, fmod)
IMPL_OP2(pow, pow)

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

