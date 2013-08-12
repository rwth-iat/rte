/*****************************************************************************
*                                                                            *
*    i F B S p r o                                                           *
*    #############                                                           *
*                                                                            *
*   L T S o f t                                                              *
*   Agentur für Leittechnik Software GmbH                                    *
*   Heinrich-Hertz-Straße 10                                                 *
*   50170 Kerpen                                                             *
*   Tel      : 02273/9893-0                                                  *
*   Fax      : 02273/52526                                                   *
*   e-Mail   : ltsoft@ltsoft.de                                              *
*   Internet : http://www.ltsoft.de                                          *
*                                                                            *
******************************************************************************
*                                                                            *
*   iFBSpro - Client-Bibliothek                                              *
*   IFBSpro/Client - iFBSpro  ACPLT/KS Dienste-Schnittstelle (C++)           *
*   ==============================================================           *
*                                                                            *
*   Datei                                                                    *
*   -----                                                                    *
*   par_param.h                                                              *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   1999-04-07  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*   Definitionen der Parser-Parameter fuer iFBSpro-Dienste                   *
*                                                                            *
*****************************************************************************/


#ifndef _PAR_PARAM_H_
#define _PAR_PARAM_H_


#include "ks/ks.h"
#include <stdio.h>

#ifdef WIN32
#undef sparc
#endif

#ifndef alloca
#define alloca malloc
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef FOR_EH
int yyerror(char *s);
int yyparse(void);
int yylex(void);
#endif

#ifdef __cplusplus
}
#endif

typedef enum {
	DT_BOOLIAN		= 0x01,
	DT_GANZZAHL		= 0x02,
	DT_FLIESSCOMMA	= 0x03,
	DT_TIMESTRUCT	= 0x04,
	DT_ZEICHEN		= 0x05
}	DataType;

typedef enum {
	DUMMY_PORT		= 0x00,
	INPUT_PORT		= 0x01,
	PARAM_PORT		= 0x02
}	PortType;
	
/*
*typedef union {
*	long int				int_val;
*	double				    float_val;
*    char*					string_val;
*	OV_TIME					time_val;
*}	ValueUnion;
*/
/*
*	ValueUnion				var_value;
*/

struct VariableItem {
	DataType				value_type;
    char*                   val;
	struct VariableItem*	next;
};
typedef struct VariableItem VariableItem;

struct Variables {
	char*					var_name;
	VariableItem*			value;
	PortType				port_typ;
	KS_VAR_TYPE				var_typ;
	int 					state;
	int 					len;
	int  					vector;
	struct Variables*		next;
};
typedef struct Variables Variables;

struct InstanceItems {
	char*					Inst_name;
	char*					Class_name;
	Variables*				Inst_var;
	struct InstanceItems*	next;
};
typedef struct InstanceItems InstanceItems;

struct DelInstItems {
	char*					Inst_name;
	struct DelInstItems*	next;
};
typedef struct DelInstItems DelInstItems;

struct SetInstVarItems {
	char*					Inst_name;
	Variables*				Inst_var;
	struct SetInstVarItems*	next;
};
typedef struct SetInstVarItems SetInstVarItems;

struct Child {
	char*				child_path;
	struct Child*		next;
};
typedef struct Child Child;


struct LinksItems {
	char*					asso_ident;
	char*					parent_role;
	char*					parent_class;
	char*					parent_path;
	char*					child_role;
	char*					child_class;
    Child*					children;
	struct LinksItems*		next;
};
typedef struct LinksItems LinksItems;

struct Dienst_param {
	InstanceItems*			Instance;
	SetInstVarItems*		Set_Inst_Var;
	DelInstItems*			DelInst;
	DelInstItems*			OldLibs;
	DelInstItems*			NewLibs;
	LinksItems*				Links;
	LinksItems*				UnLinks;
};
typedef struct Dienst_param Dienst_param;


#ifdef __cplusplus
extern "C" {
#endif

extern Dienst_param 	*ppar;
extern int 		    yydebug;
extern int 		    current_line;
extern FILE 	        *yyin;

#ifdef __cplusplus
}
#endif

/*
*	PARSER_STACK:
*	-------------
*	Manages a list of strings
*/
struct PARSER_STACK {
	struct PARSER_STACK	*pnext;
	char*				string;
};
typedef struct PARSER_STACK PARSER_STACK;



#ifdef __cplusplus
extern "C" {

#endif
/*
*	Allocate memory for a string
*/
char* fb_parser_getstring(
	const char*		    string,
	const unsigned int  length
);
/*
*	Free strings allocated by the scanner
*/
void fb_parser_freestrings(void);
/*
*	Check structures
*/
void fb_parser_checkstruct(Dienst_param* par);

#ifdef __cplusplus
}
#endif

#endif
