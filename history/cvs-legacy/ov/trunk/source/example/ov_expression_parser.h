typedef union {
	OV_DOUBLE		(* fnc)(const OV_EXPRESSION *pexpr);
	OV_STRING		stringval;
	OV_DOUBLE		floatval;
	OV_EXPRESSION	*pexpr;
} YYSTYPE;
#define	TOK_NOTEQ	258
#define	TOK_LESSEQ	259
#define	TOK_GRTREQ	260
#define	TOK_TIME	261
#define	TOK_FUNCTION	262
#define	TOK_FLOAT	263
#define	TOK_SYMBOL	264
#define	TOK_NEG	265


extern YYSTYPE ov_expression_lval;
