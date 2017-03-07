/*
*   $Id: ov_expression.h,v 1.2 2004-05-24 15:23:25 ansgar Exp $
*
*   Copyright (C) 1998
*   Lehrstuhl fuer Prozessleittechnik,
*   D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
*
*   This file is part of the ACPLT/OV Package 
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*/
/*
*	History:
*	--------
*	27-Oct-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

#ifndef OV_EXPRESSION_H_INCLUDED
#define OV_EXPRESSION_H_INCLUDED

#include "ov.h"

#if OV_COMPILER_BORLAND
#pragma warn -pro
#pragma warn -nod
#endif

#ifdef WIN32
#undef sparc
#include <malloc.h>
#endif

/*
*	flex/bison prototypes
*/
int yyerror(char *msg);
int yyparse(void);
int yylex(void);
struct yy_buffer_state *yy_scan_string(const char *str);

/*
*	a structure representing an expression.
*	objects with these data are items in the parse tree.
*/
struct OV_EXPRESSION {
	OV_DOUBLE	(* getvalue)(const struct OV_EXPRESSION *pexpr);
	void		(* delete)(struct OV_EXPRESSION *pexpr);
	union {
		OV_DOUBLE					value;		/* a numerical value */
		OV_BYTE						*pvalue;	/* generic pointer to a value */
		struct OV_EXPRESSION		*parg;		/* a function: fnc(arg) */
		struct {
			struct OV_EXPRESSION	*pexpr1;
			struct OV_EXPRESSION	*pexpr2;
		}	operator;							/* an operator: expr OP expr */
		struct OV_EXPRESSION		*pexpr;		/* negation: -expr */
	}	exprunion;
	struct OV_EXPRESSION			*pnext;		/* for the parser's memory management */
};
typedef struct OV_EXPRESSION OV_EXPRESSION;

/*
*	create an expression 
*/
OV_EXPRESSION *ov_expression_create(
	OV_INSTPTR_ov_object	pobj,
	const OV_STRING			exprstr
);

/*
*	delete an expression 
*/
void ov_expression_delete(
	OV_EXPRESSION			*pexpr
);

/*
*	evaluate an expression 
*/
OV_DOUBLE ov_expression_evaluate(
	const OV_EXPRESSION		*pexpr,
	const OV_TIME			*ptime
);

#endif

/*
*	End of file
*/

