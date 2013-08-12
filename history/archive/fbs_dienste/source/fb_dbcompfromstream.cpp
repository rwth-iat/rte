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
*   Fax      : 02273/9839-33                                                 *
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
*   fb_dbcompfromstream.cpp                                                  *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   2000-06-20  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*   iFSpro-Dienst "IFBS_DBCOMPARE"                                           *
*                                                                            *
*****************************************************************************/


#include "ifbslibdef.h"
#include "fb_scanner_for_EH.h"

/*
*        globale Variablen
*        -----------------
*/

// extern __declspec( dllexport) Dienst_param*  ppar;

// extern __declspec( dllexport) int     yydebug;
// extern __declspec( dllexport) int     current_line;

/*
*   Hauptprogramm
*   -------------
*/

/*****************************************************************************/
KS_RESULT IFBS_DBCOMPARE_FROMSTREAM(
                                     PltString& oldstr,
                                     PltString& newstr,
                                     PltString& out)
/*****************************************************************************/
{
    /*
    *        Variablen
    */
    int           exit_status;
    int           error=0;
    PltString     log;
    char          help[256];
    Dienst_param* oldpar;
    Dienst_param* newpar;
        int PROTOFILE = 0;
    
    struct yy_buffer_state* buf;

        yydebug = 0;
        current_line = 0;

    log = "";
    iFBS_SetLastError(1, error, log);


out = "/******************************************************************************\n";
out += "***                                                                         ***\n";
out += "***      Vergleichsprotokoll                                                ***\n";
out += "***      ###################                                                ***\n";
out += "***                                                                         ***\n";
out += "*******************************************************************************\n";
out += "*\n";
out += "*   Historie\n";
out += "*   --------\n";

PltTime tt = PltTime::now();
struct        tm* t;
time_t        timer;

timer = (time_t)tt.tv_sec;

t=localtime(&timer);
if(t) {
sprintf(help,"*   %4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d",
                t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
out += help;
}
out += " : Automatische Erstellung\n";

out += "*\n*\n";
out += "*   ACHTUNG!\n";
out += "*   --------\n";
out += "*   DIESE DATEI DARF NICHT VERAENDERT WERDEN!\n";
out += "*\n";
out += "******************************************************************************/\n";
out += "\n\n";

        ppar = (Dienst_param*)malloc(sizeof(Dienst_param));
        if (!ppar) {
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
        log = "";
    iFBS_PrintParserError(1, log); /* LastError loeschen */

        /*
        *   Zu vergleichender String parsen
        */

        buf = yy_scan_string( (char*)(const char*)oldstr );
        exit_status = yyparse();
    yy_delete_buffer( buf );
        /*
        *        Ausgabe erzeugen
        */
        if(exit_status != EXIT_SUCCESS) {
            
            log = IFBS_GetParserError();
                if( log == "" ) {
                   error = FB_ERR_ATPARSE;
                iFBS_SetLastError(1, error, log);
                }
                memfre(ppar);
                free(ppar);
        fb_parser_freestrings();
        return KS_ERR_BADPARAM;
    }

        oldpar = ppar;

        ppar = (Dienst_param*)malloc(sizeof(Dienst_param));
        if (ppar == NULL) {
                memfre(oldpar);
            free(oldpar);
        fb_parser_freestrings();
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
        *   Aktueller String parsen
        */

        current_line = 0;

        buf = yy_scan_string( (char*)(const char*)newstr );
    exit_status =  yyparse();
    yy_delete_buffer( buf );
        if(exit_status != EXIT_SUCCESS) {
            log = IFBS_GetParserError();
                if( log == "" ) {
                error = FB_ERR_ATPARSE;
                iFBS_SetLastError(1, error, log);
                }
                memfre(oldpar);
                free(oldpar);
                memfre(ppar);
                free(ppar);
        fb_parser_freestrings();
        return KS_ERR_BADPARAM;
    }
    
    newpar = ppar;
    error = compare_eval(newpar, oldpar, out);


    memfre(oldpar);
    free(oldpar);

    memfre(newpar);
    free(newpar);

    fb_parser_freestrings();

    return error;
}
