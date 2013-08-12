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
*   update_project.cpp                                                       *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   1999-04-07  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*                                                                            *
*                                                                            *
*****************************************************************************/


/*
*        Includes
*        --------
*/
#include "ifbslibdef.h"

/*
*        globale Variablen
*        -----------------
*/
//extern Dienst_param*         ppar;

//extern int                 yydebug;
//extern int                 current_line;
//extern FILE*         yyin;

/*
*        Funktionsprototypen
*        -------------------
*/
int yyparse(void);
extern "C" void InitInputFileStream(FILE*);

/*
*   Hauptprogramm
*        -------------
*/
/*****************************************************************************/
KS_RESULT  IFBS_DBUPDATE(KscServerBase *Server,
                         PltString     &datei,
                         PltString     &err_outfile)
/*****************************************************************************/
{
        /*
        *        Variablen
        */
        int        exit_status;
        int error;
        int PROTOFILE = 0;

        yydebug = 0;
        current_line = 0;

    PltString Str;
    FILE           *yyout = 0;
    
    if(!Server) {
        return KS_ERR_SERVERUNKNOWN;
    }
        /*
        *        check option settings
        */

    if( err_outfile.len() ) {
        yyout = fopen((const char*)err_outfile, "a");
        if(!yyout) {
                    return OV_ERR_CANTOPENFILE;
            }
            PROTOFILE = 1;
    }

    yyin = fopen((const char*)datei, "r");
    if(!yyin) {
            if(PROTOFILE) {
                fclose(yyout);
            }
        return OV_ERR_CANTOPENFILE;
    }

///////////////////////////////////////////////////////////////////////////////

if(PROTOFILE)
{
fprintf(yyout,"\n\n/*********************************************************************\n");
fprintf(yyout,"======================================================================\n");        
fprintf(yyout,"  Datei : %s\n\n", (const char*)err_outfile);
fprintf(yyout,"  Aktualisierung der Datenbasis.\n");

struct        tm* t;
time_t        timer;

PltTime tt = PltTime::now();

timer = (time_t)tt.tv_sec;
t=localtime(&timer);
if(t) {
    fprintf(yyout,"  Ereignisprotokoll vom  %4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d\n\n",
                t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec );
}
fprintf(yyout,"  HOST       : %s\n", (const char*)Server->getHost());

fprintf(yyout,"  SERVER     : %s\n", (const char*)Server->getName());

fprintf(yyout,"======================================================================\n");        
fprintf(yyout,"*********************************************************************/\n\n");
fflush(yyout);
}
///////////////////////////////////////////////////////////////////////////////

        ppar = (Dienst_param*)malloc(sizeof(Dienst_param));
        if(!ppar) {
                if(PROTOFILE) {
                        fclose(yyout);
                }
                return OV_ERR_HEAPOUTOFMEMORY;
        }

        ppar->Instance = 0;
        ppar->Set_Inst_Var = 0;
        ppar->DelInst = 0;
        ppar->OldLibs = 0;
        ppar->NewLibs = 0;
        ppar->Links = 0;
        ppar->UnLinks = 0;

        /*
        *   Eingabe parsen
        */
        Str = "";
    iFBS_PrintParserError(1, Str); /* LastError loeschen */

    InitInputFileStream(yyin);

        exit_status = yyparse();
        fclose(yyin);

        /*
        *        Ausgabe erzeugen
        */
        if(exit_status != EXIT_SUCCESS) {
                if(PROTOFILE) {
                    Str = IFBS_GetParserError();
                    if( Str == "" ) {
                        fprintf(yyout, "%s",
                            (const char*)log_getErrMsg(
                                    KS_ERR_OK,
                                    "Parse error.",
                                    "File",
                                    (const char*)datei));
                    } else {
                        fprintf(yyout, "%s",
                            (const char*)log_getErrMsg( KS_ERR_OK,
                                    (const char*)Str,
                                    "File",
                                    (const char*)datei));
                    }
                    fclose(yyout);
                }
        memfre(ppar);
        free(ppar);
        fb_parser_freestrings();
        return KS_ERR_BADPARAM;
    }

    Str = "";
    error = update_eval(Server, ppar, Str);

    if(PROTOFILE) {
       fputs((const char*)Str, yyout);
           fclose(yyout);
    }
    
    memfre(ppar);
    free(ppar);
    fb_parser_freestrings();

    return error;
}
