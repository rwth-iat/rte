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
*   fb_scanner.lex                                                           *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   1999-04-07  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*   Scanner fuer  Fb-Dienst " FB_ImportProject "                             *
*                                                                            *
*****************************************************************************/

/*
*        States
*        ------
*/
%x comment
/*
*   lex C definitions
*   -----------------
*/
%{
/*
*  #include "gov_symboltable.h"
*  #include <time.h>
*/
#include "par_param.h"
#include "fb_parser.h"
#include "ks/ks.h"
/*
*        globale Variablen
*        -----------------
*/
#ifdef __cplusplus
extern "C" {
#endif

int current_line;

#ifdef __cplusplus
}
#endif


static PARSER_STACK *pStrStack = NULL;

%}
/*****************************************************************************/
/*
*   lex definitions
*   ---------------
    STRING                  [\"][^\"\n]*[\"]
    
*/

DIGIT                   [0-9]
ALPHA                   [a-zA-Z]

FLOAT                   ("+"|"-")?{DIGIT}*[\.]{DIGIT}+((e|E)("+"|"-")?{DIGIT}+)?
INT                     ("+"|"-")?({DIGIT})+
STRING                  [\"](([\\][\"])|[^\"])*[\"]
SPECIAL                 ("%"({ALPHA}|{DIGIT})({ALPHA}|{DIGIT}))
IDENTIFIER              ({ALPHA}|_|{SPECIAL})({ALPHA}|{DIGIT}|_|{SPECIAL})*
PATH                    (([\/]|[.])({ALPHA}|{DIGIT}|_|{SPECIAL})+)+
DATE                    ({DIGIT}{4})("-")({DIGIT}{2})("-")({DIGIT}{2})
DAYTIME                 ({DIGIT}{2})(":")({DIGIT}{2})(":")({DIGIT}{2})((".")({DIGIT}{6}))?
TIME                    ({DATE})([ \t]+({DAYTIME}))?

/*****************************************************************************/
/*
*   lex rules
*   ---------
*/
%%

\/\*                                        BEGIN(comment);
<comment>.                                /* ignore */
<comment>\n                                current_line++;
<comment>\*\/                        BEGIN(INITIAL);

BOOL                                        { yylval.datatype = KS_VT_BOOL;      return TOK_DATATYPE; }
INT                                                { yylval.datatype = KS_VT_INT;       return TOK_DATATYPE; }
UINT                                        { yylval.datatype = KS_VT_UINT;      return TOK_DATATYPE; }
SINGLE                                        { yylval.datatype = KS_VT_SINGLE;    return TOK_DATATYPE; }
DOUBLE                                        { yylval.datatype = KS_VT_DOUBLE;    return TOK_DATATYPE; }
TIME                                        { yylval.datatype = KS_VT_TIME;      return TOK_DATATYPE; }
TIME_SPAN                                { yylval.datatype = KS_VT_TIME_SPAN; return TOK_DATATYPE; }
STRING                                        { yylval.datatype = KS_VT_STRING;    return TOK_DATATYPE; }
UNKNOWN                                        { yylval.datatype = KS_VT_VOID;      return TOK_DATATYPE; }

LIBRARY                                        return TOK_LIBRARY;
END_LIBRARY                                return TOK_END_LIBRARY;

DELETE_LIBRARY                        return TOK_DEL_LIBRARY;
END_DELETE_LIBRARY                return TOK_END_DEL_LIBRARY;

INSTANCE                                return TOK_INSTANCE;
CLASS                                        return TOK_CLASS;
VARIABLE_VALUES                        return TOK_VARIABLE_VALUES;
END_VARIABLE_VALUES                return TOK_END_VARIABLE_VALUES;
END_INSTANCE                        return TOK_END_INSTANCE;

LINK                                        return TOK_LINK;
OF_ASSOCIATION                        return TOK_OF_ASSOCIATION;
PARENT                                        return TOK_PARENT;
CHILDREN                                return TOK_CHILDREN;
END_LINK                                return TOK_END_LINK;

SET                                                return TOK_SET;
END_SET                                        return TOK_END_SET;

DELETE                                        return TOK_DELETE;
END_DELETE                                return TOK_END_DELETE;

UNLINK                                        return TOK_UNLINK;
END_UNLINK                                return TOK_END_UNLINK;

PARAMETER                                return TOK_PARAM_PORT;
INPUT                                        return TOK_INPUT_PORT;
OUTPUT                                        return TOK_DUMMY_PORT;
HIDDEN                                        return TOK_DUMMY_PORT;

STATE                                        return TOK_STATE;

TRUE                                        {
                                                        yylval.string = fb_parser_getstring("TRUE", 4);
                                                        return TOK_BOOLVALUE;                                                
                                                }
FALSE                                        {
                                                        yylval.string = fb_parser_getstring("FALSE", 5);
                                                        return TOK_BOOLVALUE;                                                
                                                }

{IDENTIFIER}                        {
                                                        yylval.string = fb_parser_getstring(yytext, yyleng);
                                                        return TOK_IDENTIFIER;
                                                }
{PATH}                                        {
                                                        yylval.string = fb_parser_getstring(yytext, yyleng);
                                                        return TOK_PATH;
                                                }
{TIME}                                        {
                                                        yylval.string = fb_parser_getstring(yytext, yyleng);
                                                        return TOK_TIMEVALUE;
                                                }
{FLOAT}                                        { 
                                                        yylval.string = fb_parser_getstring(yytext, yyleng);
                                                        return TOK_FLOATVALUE; 
                                                }
{INT}                                        { 
                                                        yylval.string = fb_parser_getstring(yytext, yyleng);
                                                        return TOK_INTEGERVALUE; 
                                                }
{STRING}                                {
                                                        yylval.string = fb_parser_getstring(yytext+1, yyleng-2);
                                                        return TOK_STRINGVALUE;
                                                }
[ \t]*                                        /* ignore */
\n\r                                        current_line++;
\r\n                                        current_line++;
\n                                                current_line++;
.                                                return *yytext;
%%
/*****************************************************************************/
/*
*   supporting C functions
*   ----------------------
*/
#ifdef __cplusplus
extern "C"
#endif
void InitInputFileStream(FILE* finp) {
    yyrestart(finp);
    return;
}


/*
*        Ersatz fuer Flex-Bibliothek libfl.a
*/
int yywrap()
{
        return 1;
}

/*
*        Allocate memory for a string
*/
#ifdef __cplusplus
extern "C"
#endif
char* fb_parser_getstring(
        const char*         string,
        const unsigned int  length
) {
        /*
        *        local variables
        */
        PARSER_STACK *pentry;
        char         *pStr;
        unsigned int  len = length;
        
        /*
        *        copy the string and terminate it with zero
        */
        if(!string) {
            string = "";
            len = 0;
        }
        pStr = (char*)malloc(len+1);
        if(!pStr) {
            /* Out of memory */
            return NULL;
        }
        if(len > 0) {
            strncpy(pStr, string, len);
        }
        pStr[len] = 0;
        
        /* Eintrag bereits vorhanden? */
        pentry = pStrStack;
        while(pentry) {
            if(pentry->string) {
                if(!strcmp(pentry->string, pStr)) {
                    free(pStr);
                    return pentry->string;
                }
            }
            pentry = pentry->pnext;
        }
        
        /*
        *   Create new entry
        */        
        pentry = (PARSER_STACK*)malloc(sizeof(PARSER_STACK));
        if(!pentry) {
            /* Out of memory */
            free(pStr);
            return NULL;
        }
        
        /*
        *   enter the new entry
        */
        pentry->string = pStr;
        pentry->pnext = pStrStack;
        pStrStack = pentry;
        /*
        *        finished
        */
        return pentry->string;
}

/*
*        Free strings allocated by the scanner
*/
#ifdef __cplusplus
extern "C"
#endif
void fb_parser_freestrings(void) {
        /*
        *        local variables
        */
        PARSER_STACK *pentry;
        /*
        *        free all entries
        */
        while(pStrStack) {
            pentry = pStrStack;
            pStrStack = pStrStack->pnext;
            if(pentry->string) free(pentry->string);
            free(pentry);
        }
}

#ifdef __cplusplus
extern "C"
#endif
void fb_parser_check_removeLinkChild(LinksItems* pItem, Child* remChild) {
    Child*      pchild;

    /* Eintrag am Anfang der Liste ? */
    if(pItem->children == remChild) {
        pItem->children = remChild->next;
    } else {
        /* Suche Eintrag */
        pchild = pItem->children;
        while(pchild) {
            if(pchild->next == remChild) {
                pchild->next = remChild->next;
                break;
            }
            pchild = pchild->next;
        }
    }
    
    free(remChild);
}

#ifdef __cplusplus
extern "C"
#endif
void fb_parser_check_removeLink(Dienst_param* par, LinksItems* remLink) {
    LinksItems*      pLinks;

    /* Eintrag am Anfang der Liste ? */
    if(par->Links == remLink) {
        par->Links = remLink->next;
    } else {
        /* Suche Eintrag */
        pLinks = par->Links;
        while(pLinks) {
            if(pLinks->next == remLink) {
                pLinks->next = remLink->next;
                break;
            }
            pLinks = pLinks->next;
        }
    }
    
    free(remLink);
}

#ifdef __cplusplus
extern "C"
#endif
void fb_parser_checkstruct(Dienst_param* par) {

    LinksItems* pLmaster;
    LinksItems* pLcomp;
    Child*      pCmaster;
    Child*      pCcomp;

    if( !par) {
        return;
    }
    /* Prufen, ob Links doppelt vorhanden sind */
    if( !par->Links) {
        return;
    }
    if( !par->Links->next) {
        return;
    }
    
    pLmaster = par->Links;
    while(pLmaster) {
        /* Solange noch weitere Links-Strukturen vorhanden */
        pLcomp = pLmaster->next;
        while(pLcomp) {
            /* Parent-Pfad und Role gleich? */
            if( (pLmaster->parent_path == pLcomp->parent_path) &&
                (pLmaster->child_role  == pLcomp->child_role) ) {
                                
                /* Auf 1. Child setzen */
                pCmaster    = pLmaster->children;
                
                /* Ueber alle Childs in Master */
                while(pCmaster) {
                    /* Ueber alle Childs in Comp */
                    pCcomp = pLcomp->children;
                    while(pCcomp) {
                        if(pCmaster->child_path == pCcomp->child_path) {
                            fb_parser_check_removeLinkChild(pLcomp, pCcomp);
                            /* Noch Eintraege in Comp vorhanden? */
                            if(!pLcomp->children) {
                                fb_parser_check_removeLink(par, pLcomp);
                            }
                            /* Suche neue starten */
                            fb_parser_checkstruct(par);
                            return;
                        }
                        /* Naechsten Child in Comp */
                        pCcomp = pCcomp->next;
                    }
                    pCmaster = pCmaster->next;
                }
            }
            /* Naechste Link-Strukture pruefen */
            pLcomp = pLcomp->next;
        }
        
        /* Naechste Link-Strukture als Master setzen */
        pLmaster = pLmaster->next;
    }

}
