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
*   db_update_from_stream.cpp                                                *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   2000-06-20  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*   2001-05-22  Alexander Neugebauer: Logging-Ausgabe in String              *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*                                                                            *
*                                                                            *
*****************************************************************************/


#include "ifbslibdef.h"
#include "fb_scanner_for_EH.h"

/*
*        globale Variablen
*        -----------------
*/
// extern __declspec( dllexport) Dienst_param*  ppar;

// extern __declspec( dllexport)  int     yydebug;
// extern __declspec( dllexport)  int     current_line;


/*
*   Hauptprogramm
*        -------------
*/
/*****************************************************************************/
KS_RESULT IFBS_DBUPDATE_FROMSTREAM(
                                   KscServerBase* Server,
                                   PltString&     inp_stream,
                                   PltString*     Out,
                                   bool           bAsStream)
/*****************************************************************************/
{
        /*
        *        Variablen
        */
        int         exit_status;
        int         error=0;
    PltString   out;
    char        help[256];
    FILE*       fout;
    
    struct yy_buffer_state* buf;

        yydebug = 0;
        current_line = 0;
    fout = NULL;
    out = "";
    
    iFBS_SetLastError(1, error, out);


        /*
        *        check option settings
        */

    if(!Server){
        error = KS_ERR_SERVERUNKNOWN;
        iFBS_SetLastError(1, error, out);
        return error;
    }

    if(!bAsStream) {
        if(Out && (*Out) && (Out->len() ) ) {
            fout = fopen((const char*)(*Out), "a");
            if(!fout) {
                error = OV_ERR_CANTOPENFILE;
                out = "\"%s\" \"";
                out += *Out;
                out += "\"";
                iFBS_SetLastError(1, error, out);
                return error;
            }
        }
    } else {
        if((!Out) || (!(*Out)) ) {
            error = KS_ERR_BADPARAM;
            iFBS_SetLastError(1, error, out);
            return error;
        }
    }

///////////////////////////////////////////////////////////////////////////////

if(fout)
{
    fprintf(fout, "%s%s%s",
        "\n\n/*********************************************************************\n",
        "======================================================================\n",        
        "  Aktualisierung der Datenbasis.\n  Ereignisprotokoll vom ");

    struct        tm* t;
    time_t        timer;

    PltTime tt = PltTime::now();

    timer = (time_t)tt.tv_sec;
    t=localtime(&timer);
    if(t) {
        sprintf(help," %4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d\n",
                        t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec );
        fprintf(fout, "%s", help);
    }
    fprintf(fout, "\n  HOST       : %s", (const char*)Server->getHost());
    fprintf(fout, "\n  SERVER     : %s", (const char*)Server->getName());

    fprintf(fout, "%s%s",
        "\n======================================================================\n",        
        "*********************************************************************/\n\n");
}
///////////////////////////////////////////////////////////////////////////////


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
        out = "";
    iFBS_PrintParserError(1, out); /* LastError loeschen */

        buf = yy_scan_string( (char*)(const char*)inp_stream );
    exit_status =  yyparse();
    yy_delete_buffer( buf );
        /*
        *        Ausgabe erzeugen
        */
        if(exit_status != EXIT_SUCCESS) {

                out = IFBS_GetParserError();
                if( out == "" ) {
                    if(fout) {
                    fprintf(fout, "%s", (const char*)log_getErrMsg(KS_ERR_OK,"Parse error."));
                    } else {
                        if(bAsStream) {
                            *Out = log_getErrMsg(KS_ERR_OK, "Parse error.");
                        }
                    }
                error = FB_ERR_ATPARSE;
                iFBS_SetLastError(1, error, out);
                } else {
                    if(fout) {
                        fprintf(fout, "%s", (const char*)log_getErrMsg(KS_ERR_OK,(const char*)out));
                    } else {
                        if(bAsStream) {
                            *Out = log_getErrMsg(KS_ERR_OK,"Parse error.");
                        }
                    }
                }
                if(fout) fclose(fout);
                memfre(ppar);
                free(ppar);
        fb_parser_freestrings();
        return KS_ERR_BADPARAM;
    }

    error = update_eval(Server, ppar, out);

    if(fout) {
        fputs((const char*)out, fout);
            fclose(fout);
    } else {
        if(bAsStream) {
            *Out = out;
        }
    }
    memfre(ppar);
    free(ppar);
    fb_parser_freestrings();
    
    return error;
}
