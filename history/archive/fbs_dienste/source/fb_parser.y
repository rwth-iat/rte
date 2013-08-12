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
*   Fax      : 02273/9893-33                                                 *
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
*   fb_parser.y                                                              *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   1999-04-07  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*   Parser fuer  Fb-Dienst " FB_ImportProject "                              *
*                                                                            *
*****************************************************************************/


/*
*   bison C declarations
*   --------------------
*/
%{
/*
*    Includes
*/
#include "ks/ks.h"
#include "par_param.h"
#include <stdio.h>
#include <stdlib.h>

void iFBS_SetParserError(int, char*);

/*
*    globale Variablen
*/

//extern int                 current_line;
//extern Dienst_param*     ppar;

%}
/*****************************************************************************/
/*
*   bison declarations
*   ------------------
*/
/*
*    Attribute der Terminale und Nichtterminale
*/
%union {
        KS_VAR_TYPE             datatype;
        char*                   string;
        VariableItem*           var_item;
        Variables*              variable;
        Dienst_param*           par;
        InstanceItems*          instans;
        Child*                  ch_path;
        LinksItems*             link;
        DelInstItems*           delinstans;
        PortType                pt;
}
/*
*   Typen von Terminalen (Token)
*/
%token <string>     TOK_IDENTIFIER TOK_PATH TOK_STRINGVALUE TOK_FLOATVALUE TOK_DOUBLEVALUE
                    TOK_INTEGERVALUE TOK_BOOLVALUE TOK_TIMEVALUE
%token <datatype>   TOK_DATATYPE
%token              TOK_INSTANCE TOK_VARIABLE_VALUES TOK_END_VARIABLE_VALUES
                    TOK_END_INSTANCE TOK_LINK TOK_OF_ASSOCIATION TOK_CLASS TOK_PARENT
                    TOK_CHILDREN TOK_END_LINK 
                    TOK_PARAM_PORT TOK_INPUT_PORT TOK_DUMMY_PORT
                    TOK_LIBRARY TOK_END_LIBRARY TOK_STATE
%type<variable>     scalar_variable_value variable_values_opt variable_values
                    variable_value variable_values_block_opt vector_variable_value
%type<var_item>     scalar_value vector_value vector_value_list
%type<par>          blocks block
%type<instans>      instance_block instance_blocks
%type<link>         link_block link_blocks
%type<ch_path>      child_paths
%type<delinstans>   newlibs_blocks newlibs_block
%type<pt>           port_types
%type<string>       state_opt
/*****************************************************************************/
%%
/*
*   bison grammar rules
*   -------------------
*/

/*
start:*/                                        /* empty */
/*                                                {
*/                                                        /* Params = ppar; */
/*                                                }
| blocks
{
*/                                                        /* Params = $1; */
/*                                                }
;
*/

blocks:                 {
                        /* empty */
                        }
                        | block
                        {
                            $$ = $1;
                        }
                        | blocks block
                        {
                            $$ = $2;
                        }
;
block:                  instance_blocks
                        {
                            InstanceItems* pII;
                            /* Schon Instanzen gefunden ? */
                            if (ppar->Instance) {
                                /* Neue Instanz am Ende hinzufuegen */
                                pII = ppar->Instance;
                                while(pII->next) {
                                    pII=pII->next;
                                }
                                pII->next = $1;
                                    
                            } else {
                                /* Noch keine Instanzen */
                                ppar->Instance = $1;
                            }
                            $$ = ppar;
                        }
                        | link_blocks
                        {
                            LinksItems* pLI;
                            /* Schon Links gefunden ? */
                            if(ppar->Links) {
                                /* Am Ende hinzufuegen */
                                pLI = ppar->Links;
                                while(pLI->next) {
                                    pLI=pLI->next;
                                }
                                pLI->next = $1;
                                
                                /* Doppelte Eintraege? */
                                fb_parser_checkstruct(ppar);
                                
                            } else {
                                /* Noch keine Links */
                                ppar->Links = $1;
                            }
                            $$ = ppar;
                        }
                        | newlibs_blocks
                        {
                            DelInstItems* pDI;
                            if(ppar->NewLibs) {
                                pDI=ppar->NewLibs;
                                while(pDI->next) {
                                    pDI=pDI->next;
                                }
                                pDI->next = $1;
                            } else {
                                ppar->NewLibs = $1;
                            }
                            $$ = ppar;
                        }
;
instance_blocks:        instance_block
                        {
                            $$ = $1;
                        }
                        | instance_blocks instance_block
                        {
                            InstanceItems *pInstList = $1;
                            /* Neue Instanze am List-Ende hinzufuegen */
                            while( pInstList->next) {
                                pInstList = pInstList->next;
                            }
                            pInstList->next = $2;
                            $$ = $1;
                        }
;
instance_block:         TOK_INSTANCE TOK_PATH ':'
                            TOK_CLASS TOK_PATH ';'
                            variable_values_block_opt
                            TOK_END_INSTANCE ';'
                        {
                            Variables      *pVar;
                            InstanceItems  *pinst = (InstanceItems*)malloc(sizeof(InstanceItems));
                                                        
                            if( (!$2) || (!$5) || (!pinst) ) {
                                if(pinst) free(pinst);
                                pVar = $7;
                                while(pVar) {
                                    $7 = pVar->next;
                                    if(pVar->value) {
                                        VariableItem *pVal = pVar->value;
                                        while(pVal) {
                                            pVar->value = pVar->value->next;
                                            free(pVal);
                                            pVal = pVar->value;
                                        }
                                    }
                                    free(pVar);
                                    pVar = $7;
                                }
                                yyerror("out of memory");
                                return EXIT_FAILURE;
                            }
                            pinst->next = 0;
                            pinst->Inst_name = $2;
                            pinst->Class_name = $5;
                            pinst->Inst_var = $7;

                            $$ = pinst;
                        }
;
link_blocks:                link_block
                            {
                                $$ = $1;
                            }
                            | link_blocks link_block
                            {
                                LinksItems *pLinkList = $1;
                                /* Neues Itens am ende der Liste hinzufuegen */
                                while ( pLinkList->next)
                                        pLinkList = pLinkList->next;
                                pLinkList->next = $2;
                                $$ = $1;
                            }
;
link_block:                 TOK_LINK TOK_OF_ASSOCIATION TOK_IDENTIFIER ';'
                            TOK_PARENT TOK_IDENTIFIER ':' TOK_CLASS TOK_IDENTIFIER
                             '=' TOK_PATH ';'
                            TOK_CHILDREN TOK_IDENTIFIER ':' TOK_CLASS TOK_IDENTIFIER
                             '=' '{' child_paths '}' ';'
                            TOK_END_LINK ';'
                            {
                                LinksItems*         plink;
                                Child*              pchild;
                                                        
                                plink = (LinksItems*)malloc(sizeof(LinksItems));
                                if( (!plink) || (!$11) || (!$14) || (!$20) ) {
                                    if(plink) free(plink);
                                    pchild = $20;
                                    while(pchild) {
                                        $20 = pchild->next;
                                        free(pchild);
                                        pchild = $20;
                                    }
                                    yyerror("out of memory");
                                    return EXIT_FAILURE;
                                }
                                plink->next = 0;
                                plink->asso_ident = $3;
                                plink->parent_role = $6;
                                plink->parent_class = $9;
                                plink->parent_path = $11;
                                plink->child_role = $14;
                                plink->child_class = $17;
                                plink->children = $20;

                                $$ = plink;
                            }
;
child_paths:                TOK_PATH
                            {
                                Child* pchild;
                                if(!$1) {
                                    yyerror("out of memory");
                                    return EXIT_FAILURE;
                                }
                                pchild = (Child*)malloc(sizeof(Child));
                                if(!pchild) {
                                    yyerror("out of memory");
                                    return EXIT_FAILURE;
                                }
                                pchild->next = 0;
                                pchild->child_path = $1;
                                $$ = pchild;
                            }
                            | child_paths ',' TOK_PATH
                            {
                                Child* pchild;
                                Child* pchilds;
                                
                                if(!$3) {
                                    yyerror("out of memory");
                                    return EXIT_FAILURE;
                                }
                                pchild = (Child*)malloc(sizeof(Child));
                                if(!pchild) {
                                    yyerror("out of memory");
                                    return EXIT_FAILURE;
                                }
                                pchild->next = 0;
                                pchild->child_path = $3;
                                
                                /* Neues Element am ende der Liste hinzufuegen */
                                pchilds = $1;
                                while(pchilds->next) {
                                    pchilds = pchilds->next;
                                }
                                pchilds->next = pchild;
                                $$ = $1;
                            }
;
newlibs_blocks:             newlibs_block
                            {
                                $$ = $1;
                            }
                            | newlibs_blocks newlibs_block
                            {
                                DelInstItems* pdelinst;
                                pdelinst = $1;
                                /* Neue Bibliothek am Ende der Liste hinzufuegen */
                                while ( pdelinst->next) {
                                        pdelinst = pdelinst->next;
                                }
                                pdelinst->next = $2;
                                $$ = $1;
                            }
;
newlibs_block:              TOK_LIBRARY TOK_IDENTIFIER TOK_END_LIBRARY ';'
                            {
                                    DelInstItems* pd = (DelInstItems*)malloc(sizeof(DelInstItems));
                                    if( (!pd) || (!$2) ) {
                                        if(pd) free(pd);
                                        yyerror("out of memory");
                                        return EXIT_FAILURE;
                                    }
                                    pd->next = NULL;
                                    pd->Inst_name = $2;

                                    $$ = pd;
                            }
                            | TOK_LIBRARY TOK_PATH TOK_END_LIBRARY ';'
                            {
                                    DelInstItems* pd = (DelInstItems*)malloc(sizeof(DelInstItems));
                                    if( (!pd) || (!$2) ) {
                                        if(pd) free(pd);
                                        yyerror("out of memory");
                                        return EXIT_FAILURE;
                                    }
                                    pd->next = NULL;
                                    pd->Inst_name = $2;

                                    $$ = pd;
                            }
;
variable_values_block_opt:  /* empty */
                            {
                                $$ = NULL;
                            }
                            | TOK_VARIABLE_VALUES variable_values_opt TOK_END_VARIABLE_VALUES ';'
                            {
                                $$ = $2;
                            }
;
variable_values_opt:        /* empty */
                            {
                                $$ = NULL;
                            }
                            | variable_values
                            {
                                $$ = $1;
                            }
;
variable_values:            variable_value
                            {
                                    $$ = $1;
                            }
                            | variable_values variable_value
                            {
                                Variables* pVarList = $1;
                                /* Schon Variable vorhanden? */
                                if(pVarList) {
                                    /* Am Ende der Var-Liste hinzufuegen */
                                    while ( pVarList->next)
                                            pVarList = pVarList->next;
                                    pVarList->next = $2;
                                    $$=$1;
                                } else {
                                    /* Es waren nur Dummy-Ports */
                                    $$=$2;
                                }
                            }
;
variable_value:             scalar_variable_value
                            {
                                    $$ = $1;
                            }
                            | vector_variable_value
                            {
                                    $$ = $1;
                            }
;
scalar_variable_value:       TOK_IDENTIFIER ':' port_types TOK_DATATYPE '=' scalar_value state_opt ';'
                            {
                                Variables* pvar;
                                                        
                                if($4 == KS_VT_VOID) {
                                    $3 = DUMMY_PORT;
                                }
                                                        
                                if($3) {
                                    /* Es ist Input oder Parameter Port */
                                    pvar = (Variables*)malloc(sizeof(Variables));
                                    if( (!pvar) || (!$1) ) {
                                        if(pvar) free(pvar);
                                        free($6);
                                        yyerror("out of memory");
                                        return EXIT_FAILURE;
                                    }
                                    pvar->next = 0;
                                    pvar->var_name = $1;
                                    pvar->len = 1;
                                    pvar->value = $6;
                                    pvar->port_typ = $3;
                                    pvar->var_typ = $4;
                                    if($7) {
                                        pvar->state =  (int)atoi($7);
                                    } else {
                                        pvar->state = 0;
                                    }
                                    pvar->vector = 0;
                                                        
                                    switch($4) {        /* Verzweigung : Typ der Variable */
                                        case KS_VT_BOOL:
                                                            if(pvar->value->value_type != DT_BOOLIAN) {
                                                                yyerror("Bad variable type");
                                                                return EXIT_FAILURE;
                                                            }
                                                            break;
                                        case KS_VT_INT:
                                                            if(pvar->value->value_type != DT_GANZZAHL) {
                                                                yyerror("Bad variable type");
                                                                return EXIT_FAILURE;
                                                            }
                                                            break;
                                        case KS_VT_UINT:
                                                            if(pvar->value->value_type != DT_GANZZAHL) {
                                                                yyerror("Bad variable type");
                                                                return EXIT_FAILURE;
                                                            }
                                                            break;
                                        case KS_VT_SINGLE:
                                                            if( (pvar->value->value_type != DT_FLIESSCOMMA) &&
                                                                (pvar->value->value_type != DT_GANZZAHL) ) {
                                                                yyerror("Bad variable type");
                                                                return EXIT_FAILURE;
                                                            }
                                                            break;
                                        case KS_VT_DOUBLE:
                                                            if( (pvar->value->value_type != DT_FLIESSCOMMA) &&
                                                                (pvar->value->value_type != DT_GANZZAHL) ) {
                                                                yyerror("Bad variable type");
                                                                return EXIT_FAILURE;
                                                            }
                                                            break;
                                        case KS_VT_TIME:
                                                            if(pvar->value->value_type != DT_TIMESTRUCT) {
                                                                yyerror("Bad variable type");
                                                                return EXIT_FAILURE;
                                                            }
                                                            break;
                                        case KS_VT_TIME_SPAN:
                                                            if(pvar->value->value_type != DT_FLIESSCOMMA) {
                                                                yyerror("Bad variable type");
                                                                return EXIT_FAILURE;
                                                            }
                                                            break;
                                        case KS_VT_STRING:
                                                            if(pvar->value->value_type != DT_ZEICHEN) {
                                                                yyerror("Bad variable type");
                                                                return EXIT_FAILURE;
                                                            }
                                                            break;
                                            default:
                                                            yyerror("Unknown variable type");
                                                            return EXIT_FAILURE;
                                    }
                            
                                    pvar->value->value_type = (DataType)$4;

                                    $$ = pvar;
                                
                                } else {
                                    /* Es ist Hidden oder Output Port */
                                    /* Speicher freigeben */
                                    if($6) free($6);
                                    $$ = NULL;
                                }
                            }
;
state_opt:                  /* empty */
                            {
                                $$ = NULL;
                            }
                            | TOK_STATE '=' TOK_INTEGERVALUE
                            {
                                if(!$3) {
                                    yyerror("out of memory");
                                    return EXIT_FAILURE;
                                }
                                $$ = $3;
                            }
;
vector_variable_value:      TOK_IDENTIFIER '[' TOK_INTEGERVALUE ']' ':' port_types TOK_DATATYPE '=' vector_value ';'
                            {
                                Variables*                 pvar;
                                VariableItem*         pvar_item;
                                int             count;
                            
                                if($7 == KS_VT_VOID) {
                                    $6 = DUMMY_PORT;
                                }

                                if($6) {
                                    /* Es ist Input oder Parameter Port */
                                    pvar = (Variables*)malloc(sizeof(Variables));
                                    if(( !pvar) || (!$1) || (!$3) ) {
                                        if(pvar) free(pvar);
                                        pvar_item = $9;
                                        while(pvar_item) {
                                            $9 = pvar_item->next;
                                            free(pvar_item);
                                            pvar_item = $9;
                                        }
                                        yyerror("out of memory");
                                        return EXIT_FAILURE;
                                    }

                                    count = (int)atoi($3);
    
                                    pvar->next = 0;
                                    pvar->var_name = $1;
                                    pvar->len = count;
                                    pvar->value = $9;
                                    pvar->port_typ = $6;
                                    pvar->var_typ = $7;
                                    pvar->state = 0;
                                    pvar->vector = 1;
                                
                                    /* Value vorhanden ? */
                                    if(pvar->value) {
                                        count = 0;
                                        pvar_item = pvar->value;
                                        /* Ueber alle Array-Elemente gehen und Typ Pruefen */
                                        while(pvar_item) {
                                            switch(pvar->var_typ) {        /* Verzweigung Typ der Variable */
                                                case KS_VT_BOOL:
                                                                    if(pvar_item->value_type != DT_BOOLIAN) {
                                                                        yyerror("Bad variable type");
                                                                        return EXIT_FAILURE;
                                                                    }
                                                                    break;
                                                case KS_VT_INT:
                                                                    if(pvar_item->value_type != DT_GANZZAHL) {
                                                                        yyerror("Bad variable type");
                                                                        return EXIT_FAILURE;
                                                                    }
                                                                    break;
                                                case KS_VT_UINT:
                                                                    if(pvar_item->value_type != DT_GANZZAHL) {
                                                                        yyerror("Bad variable type");
                                                                        return EXIT_FAILURE;
                                                                    }
                                                                    break;
                                                case KS_VT_SINGLE:
                                                                    if( (pvar_item->value_type != DT_FLIESSCOMMA) &&
                                                                        (pvar->value->value_type != DT_GANZZAHL) ) {
                                                                        yyerror("Bad variable type");
                                                                        return EXIT_FAILURE;
                                                                    }
                                                                    break;
                                                case KS_VT_DOUBLE:
                                                                    if( (pvar_item->value_type != DT_FLIESSCOMMA) &&
                                                                        (pvar->value->value_type != DT_GANZZAHL) ) {
                                                                        yyerror("Bad variable type");
                                                                        return EXIT_FAILURE;
                                                                    }
                                                                    break;
                                                case KS_VT_TIME:
                                                                    if(pvar_item->value_type != DT_TIMESTRUCT) {
                                                                        yyerror("Bad variable type");
                                                                        return EXIT_FAILURE;
                                                                    }
                                                                    break;
                                                case KS_VT_TIME_SPAN:
                                                                    if(pvar_item->value_type != DT_FLIESSCOMMA) {
                                                                        yyerror("Bad variable type");
                                                                        return EXIT_FAILURE;
                                                                    }
                                                                break;
                                                case KS_VT_STRING:
                                                                if(pvar_item->value_type != DT_ZEICHEN) {
                                                                    yyerror("Bad variable type");
                                                                    return EXIT_FAILURE;
                                                                }
                                                                break;
                                                default:
                                                        yyerror("Unknown variable type");
                                                        return EXIT_FAILURE;
                                            }
                                            pvar_item->value_type = (DataType)$7;
                                            pvar_item = pvar_item->next;
                                            count++;
                                        } /* while var_items */
                                    }   /* if value vorhanden */
                                
                                    /* Anzahl elementen pruefen */
                                    if(count > pvar->len) {
                                        yyerror("Too many arguments for initialization.");
                                        return EXIT_FAILURE;
                                    }
                                    if(count < pvar->len) {
                                        yyerror("Too little arguments for initialization.");
                                        return EXIT_FAILURE;
                                    }

                                    $$ = pvar;

                                } else {
                                    /* Es ist Hidden oder Output Port */
                                    /* Speicher freigeben */
                                    pvar_item = $9;
                                    while(pvar_item) {
                                        $9 = pvar_item->next;
                                        free(pvar_item);
                                        pvar_item = $9;
                                    }
                                    $$ = NULL;
                                }
                            }
;
port_types:                 TOK_INPUT_PORT
                            {
                                $$ = INPUT_PORT;
                            }
                            | TOK_DUMMY_PORT
                            {
                                $$ = DUMMY_PORT;
                            }
;
vector_value:              '{' vector_value_list '}'
                            {
                                $$ = $2;
                            }
;
vector_value_list:          /* empty */
                            {
                                $$ = NULL;
                            }
                            | scalar_value
                            {
                                $$ = $1;
                            }
                            | vector_value_list ',' scalar_value
                            { 
                                VariableItem* pvar_item = $1;
                                    
                                /* Neues Array Element am Ende der Liste hinzufuegen */
                                if(pvar_item) {
                                    while(pvar_item->next) {
                                        pvar_item=pvar_item->next;
                                    }
                                    pvar_item->next = $3;
                                }
                                $$ = $1;
                            }
;
scalar_value:               /* empty */
                            {
                                    $$ = NULL;
                            }
                            | TOK_FLOATVALUE 
                            {
                                VariableItem* pvar_item = (VariableItem*)malloc(sizeof(VariableItem));
                                if( (!pvar_item) || (!$1) ) {
                                    if(pvar_item) free(pvar_item);
                                    yyerror("out of memory");
                                    return EXIT_FAILURE;
                                }
                                pvar_item->next = 0;
                                pvar_item->value_type = DT_FLIESSCOMMA;
                                pvar_item->val = $1;
    
                                $$ = pvar_item;
                            }
                            | TOK_INTEGERVALUE 
                            {
                                VariableItem* pvar_item = (VariableItem*)malloc(sizeof(VariableItem));
                                if((!pvar_item) || (!$1) ) {
                                    if(pvar_item) free(pvar_item);
                                    yyerror("out of memory");
                                    return EXIT_FAILURE;
                                }
                                pvar_item->next = 0;
                                pvar_item->value_type = DT_GANZZAHL;
                                pvar_item->val = $1;
    
                                $$ = pvar_item;
                            }
                            | TOK_BOOLVALUE 
                            {
                                VariableItem* pvar_item = (VariableItem*)malloc(sizeof(VariableItem));
                                if((!pvar_item) || (!$1) ) {
                                    if(pvar_item) free(pvar_item);
                                    yyerror("out of memory");
                                    return EXIT_FAILURE;
                                }
                                pvar_item->next = 0;
                                pvar_item->value_type = DT_BOOLIAN;
                                pvar_item->val = $1;
    
                                $$ = pvar_item;
                            }
                            | TOK_STRINGVALUE 
                            {
                                VariableItem* pvar_item = (VariableItem*)malloc(sizeof(VariableItem));
                                if((!pvar_item) || (!$1) ) {
                                    if(pvar_item) free(pvar_item);
                                    yyerror("out of memory");
                                    return EXIT_FAILURE;
                                }
                                pvar_item->next = 0;
                                pvar_item->value_type = DT_ZEICHEN;
    
                                pvar_item->val = $1;
                                $$ = pvar_item;
                            }
                            |  TOK_TIMEVALUE
                            {
                                VariableItem* pvar_item = (VariableItem*)malloc(sizeof(VariableItem));
                                if((!pvar_item) || (!$1) ) {
                                    if(pvar_item) free(pvar_item);
                                        yyerror("out of memory");
                                        return EXIT_FAILURE;
                                }
                                pvar_item->next = 0;
                                pvar_item->value_type = DT_TIMESTRUCT;
                                pvar_item->val = $1;
    
                                $$ = pvar_item;
                            }
;
%%
/*****************************************************************************/
/*
*   supporting C code
*   -----------------
*/
/*
*   Fehlermeldungen
*/
int yyerror(char *s)
{
  static char msg[1024];
  
  strcpy(msg, s);
  
  iFBS_SetParserError(current_line+1, msg);
  
  return EXIT_FAILURE;
}
