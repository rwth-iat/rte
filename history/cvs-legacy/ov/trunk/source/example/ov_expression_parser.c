
/*  A Bison parser, made from ov_expression_parser.y
 by  GNU Bison version 1.25
  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse ov_expression_parse
#define yylex ov_expression_lex
#define yyerror ov_expression_error
#define yylval ov_expression_lval
#define yychar ov_expression_char
#define yydebug ov_expression_debug
#define yynerrs ov_expression_nerrs
#define	TOK_NOTEQ	258
#define	TOK_LESSEQ	259
#define	TOK_GRTREQ	260
#define	TOK_TIME	261
#define	TOK_FUNCTION	262
#define	TOK_FLOAT	263
#define	TOK_SYMBOL	264
#define	TOK_NEG	265

#line 29 "ov_expression_parser.y"

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

#line 99 "ov_expression_parser.y"
typedef union {
	OV_DOUBLE		(* fnc)(const OV_EXPRESSION *pexpr);
	OV_STRING		stringval;
	OV_DOUBLE		floatval;
	OV_EXPRESSION	*pexpr;
} YYSTYPE;
#ifndef YYDEBUG
#define YYDEBUG 1
#endif

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		48
#define	YYFLAG		-32768
#define	YYNTBASE	25

#define YYTRANSLATE(x) ((unsigned)(x) <= 265 ? yytranslate[x] : 27)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    24,     2,     2,     2,    19,    11,     2,    22,
    23,    17,    15,     2,    16,     2,    18,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,    13,
    12,    14,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,    21,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,    10,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    20
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     1,     3,     5,     7,     9,    14,    18,    22,    26,
    30,    34,    38,    42,    46,    50,    54,    58,    62,    66,
    69,    72,    75,    79
};

static const short yyrhs[] = {    -1,
    26,     0,     8,     0,     6,     0,     9,     0,     7,    22,
    26,    23,     0,    26,    10,    26,     0,    26,    11,    26,
     0,    26,    12,    26,     0,    26,     3,    26,     0,    26,
    13,    26,     0,    26,     4,    26,     0,    26,     5,    26,
     0,    26,    14,    26,     0,    26,    15,    26,     0,    26,
    16,    26,     0,    26,    17,    26,     0,    26,    18,    26,
     0,    26,    19,    26,     0,    15,    26,     0,    16,    26,
     0,    24,    26,     0,    26,    21,    26,     0,    22,    26,
    23,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   125,   126,   128,   129,   130,   131,   132,   133,   134,   135,
   136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
   146,   147,   148,   149
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","TOK_NOTEQ",
"TOK_LESSEQ","TOK_GRTREQ","TOK_TIME","TOK_FUNCTION","TOK_FLOAT","TOK_SYMBOL",
"'|'","'&'","'='","'<'","'>'","'+'","'-'","'*'","'/'","'%'","TOK_NEG","'^'",
"'('","')'","'!'","start","expr", NULL
};
#endif

static const short yyr1[] = {     0,
    25,    25,    26,    26,    26,    26,    26,    26,    26,    26,
    26,    26,    26,    26,    26,    26,    26,    26,    26,    26,
    26,    26,    26,    26
};

static const short yyr2[] = {     0,
     0,     1,     1,     1,     1,     4,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
     2,     2,     3,     3
};

static const short yydefact[] = {     1,
     4,     0,     3,     5,     0,     0,     0,     0,     2,     0,
    20,    21,     0,    22,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    24,
    10,    12,    13,     7,     8,     9,    11,    14,    15,    16,
    17,    18,    19,    23,     6,     0,     0,     0
};

static const short yydefgoto[] = {    46,
     9
};

static const short yypact[] = {   117,
-32768,   -18,-32768,-32768,   117,   117,   117,   117,    63,   117,
   -15,   -15,    21,   -15,   117,   117,   117,   117,   117,   117,
   117,   117,   117,   117,   117,   117,   117,   117,    42,-32768,
   130,   137,   137,    82,   101,   130,   137,   137,    71,    71,
   -15,   -15,   -15,   -15,-32768,     7,     8,-32768
};

static const short yypgoto[] = {-32768,
    -5
};


#define	YYLAST		158


static const short yytable[] = {    11,
    12,    13,    14,    10,    29,    28,    47,    48,     0,    31,
    32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
    42,    43,    44,    15,    16,    17,     0,     0,     0,     0,
    18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
     0,    28,     0,    30,    15,    16,    17,     0,     0,     0,
     0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
    27,     0,    28,     0,    45,    15,    16,    17,     0,     0,
     0,     0,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,     0,    28,    15,    16,    17,    25,    26,    27,
     0,    28,    19,    20,    21,    22,    23,    24,    25,    26,
    27,     0,    28,    15,    16,    17,     0,     0,     0,     0,
     0,     0,    20,    21,    22,    23,    24,    25,    26,    27,
     0,    28,     1,     2,     3,     4,     0,     0,     0,     0,
     0,     5,     6,    16,    17,     0,     0,     0,     7,     0,
     8,     0,    21,    22,    23,    24,    25,    26,    27,     0,
    28,    23,    24,    25,    26,    27,     0,    28
};

static const short yycheck[] = {     5,
     6,     7,     8,    22,    10,    21,     0,     0,    -1,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,     3,     4,     5,    -1,    -1,    -1,    -1,
    10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
    -1,    21,    -1,    23,     3,     4,     5,    -1,    -1,    -1,
    -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
    19,    -1,    21,    -1,    23,     3,     4,     5,    -1,    -1,
    -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
    18,    19,    -1,    21,     3,     4,     5,    17,    18,    19,
    -1,    21,    11,    12,    13,    14,    15,    16,    17,    18,
    19,    -1,    21,     3,     4,     5,    -1,    -1,    -1,    -1,
    -1,    -1,    12,    13,    14,    15,    16,    17,    18,    19,
    -1,    21,     6,     7,     8,     9,    -1,    -1,    -1,    -1,
    -1,    15,    16,     4,     5,    -1,    -1,    -1,    22,    -1,
    24,    -1,    13,    14,    15,    16,    17,    18,    19,    -1,
    21,    15,    16,    17,    18,    19,    -1,    21
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/share/bison.simple"

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 196 "/usr/share/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 125 "ov_expression_parser.y"
{ _pexpr = NULL; ;
    break;}
case 2:
#line 126 "ov_expression_parser.y"
{ _pexpr = yyvsp[0].pexpr; ;
    break;}
case 3:
#line 128 "ov_expression_parser.y"
{ yyval.pexpr = _new_value(yyvsp[0].floatval); ;
    break;}
case 4:
#line 129 "ov_expression_parser.y"
{ yyval.pexpr = _new_time(); ;
    break;}
case 5:
#line 130 "ov_expression_parser.y"
{ yyval.pexpr = _new_pvalue(_pobj, yyvsp[0].stringval); ;
    break;}
case 6:
#line 131 "ov_expression_parser.y"
{ yyval.pexpr = _new_function(yyvsp[-3].fnc, yyvsp[-1].pexpr); ;
    break;}
case 7:
#line 132 "ov_expression_parser.y"
{ yyval.pexpr = _new_operator(_getvalue_or, yyvsp[-2].pexpr, yyvsp[0].pexpr); ;
    break;}
case 8:
#line 133 "ov_expression_parser.y"
{ yyval.pexpr = _new_operator(_getvalue_and, yyvsp[-2].pexpr, yyvsp[0].pexpr); ;
    break;}
case 9:
#line 134 "ov_expression_parser.y"
{ yyval.pexpr = _new_operator(_getvalue_equal, yyvsp[-2].pexpr, yyvsp[0].pexpr); ;
    break;}
case 10:
#line 135 "ov_expression_parser.y"
{ yyval.pexpr = _new_operator(_getvalue_noteq, yyvsp[-2].pexpr, yyvsp[0].pexpr); ;
    break;}
case 11:
#line 136 "ov_expression_parser.y"
{ yyval.pexpr = _new_operator(_getvalue_less, yyvsp[-2].pexpr, yyvsp[0].pexpr); ;
    break;}
case 12:
#line 137 "ov_expression_parser.y"
{ yyval.pexpr = _new_operator(_getvalue_lesseq, yyvsp[-2].pexpr, yyvsp[0].pexpr); ;
    break;}
case 13:
#line 138 "ov_expression_parser.y"
{ yyval.pexpr = _new_operator(_getvalue_grtreq, yyvsp[-2].pexpr, yyvsp[0].pexpr); ;
    break;}
case 14:
#line 139 "ov_expression_parser.y"
{ yyval.pexpr = _new_operator(_getvalue_greater, yyvsp[-2].pexpr, yyvsp[0].pexpr); ;
    break;}
case 15:
#line 140 "ov_expression_parser.y"
{ yyval.pexpr = _new_operator(_getvalue_plus, yyvsp[-2].pexpr, yyvsp[0].pexpr); ;
    break;}
case 16:
#line 141 "ov_expression_parser.y"
{ yyval.pexpr = _new_operator(_getvalue_minus, yyvsp[-2].pexpr, yyvsp[0].pexpr); ;
    break;}
case 17:
#line 142 "ov_expression_parser.y"
{ yyval.pexpr = _new_operator(_getvalue_mul, yyvsp[-2].pexpr, yyvsp[0].pexpr); ;
    break;}
case 18:
#line 143 "ov_expression_parser.y"
{ yyval.pexpr = _new_operator(_getvalue_div, yyvsp[-2].pexpr, yyvsp[0].pexpr); ;
    break;}
case 19:
#line 144 "ov_expression_parser.y"
{ yyval.pexpr = _new_operator(_getvalue_mod, yyvsp[-2].pexpr, yyvsp[0].pexpr); ;
    break;}
case 20:
#line 145 "ov_expression_parser.y"
{ yyval.pexpr = yyvsp[0].pexpr; ;
    break;}
case 21:
#line 146 "ov_expression_parser.y"
{ yyval.pexpr = _new_negation(yyvsp[0].pexpr); ;
    break;}
case 22:
#line 147 "ov_expression_parser.y"
{ yyval.pexpr = _new_not(yyvsp[0].pexpr); ;
    break;}
case 23:
#line 148 "ov_expression_parser.y"
{ yyval.pexpr = _new_operator(_getvalue_pow, yyvsp[-2].pexpr, yyvsp[0].pexpr); ;
    break;}
case 24:
#line 149 "ov_expression_parser.y"
{ yyval.pexpr = yyvsp[-1].pexpr; ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 498 "/usr/share/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 151 "ov_expression_parser.y"


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

