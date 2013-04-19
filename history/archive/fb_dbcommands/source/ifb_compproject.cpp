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
*   ifb_compproject.cpp                                                      *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   1999-04-07  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*   iFSpro-Dienst "IFBS_DBCOMPARE"                                           *
*                                                                            *
*****************************************************************************/


/*
*        Includes
*        --------
*/
#include "ifbslibdef.h"
#include "par_param.h"

/*
*        globale Variablen
*        -----------------
*/

//extern Dienst_param*         ppar;

//extern int                 yydebug;
//extern FILE*         yyin;
//extern int                 current_line;

/*
*        Funktionsprototypen
*        -------------------
*/

int yyparse(void);
extern "C" void InitInputFileStream(FILE*);

/*
*   Hauptprogramm
*   -------------
*/

/*****************************************************************************/
KS_RESULT IFBS_DBCOMPARE(PltString& olddat,
                         PltString& newdat,
                         PltString& proto)
/*****************************************************************************/
{
        /*
        *        Variablen
        */
    int             exit_status;
    KS_RESULT       fehler; /* Funktionsrueckmeldung */
    Dienst_param*   oldpar;
    Dienst_param*   newpar;
    FILE*           yyout;
    PltString       out;
    
        yydebug = 0;
        current_line = 0;

        /*
        *        check options
        */

        if (!olddat.len()) {
          return KS_ERR_BADNAME;
        }

        if (!newdat.len()) {
          return KS_ERR_BADNAME;
        }

        if (!proto.len()) {
          return KS_ERR_BADNAME;
        }

        yyout = fopen((const char*)proto, "w");
        if(!yyout) {
        return OV_ERR_CANTOPENFILE;
        }

fprintf(yyout,"/******************************************************************************\n");
fprintf(yyout,"***                                                                         ***\n");
fprintf(yyout,"***      Vergleichsprotokoll                                                ***\n");
fprintf(yyout,"***      ###################                                                ***\n");
fprintf(yyout,"***                                                                         ***\n");
fprintf(yyout,"*******************************************************************************\n");
fprintf(yyout,"*\n");
fprintf(yyout,"*  Datei : %s\n*\n", (const char*)proto);
fprintf(yyout,"*  Ergebnis des Vergleiches zweier Datenbasen :\n*\n");
fprintf(yyout,"*  comparison : %s\n", (const char*)olddat);
fprintf(yyout,"*  reference  : %s\n", (const char*)newdat);
fprintf(yyout,"*\n*\n");
fprintf(yyout,"*  Historie\n");
fprintf(yyout,"*  --------\n");
fflush(yyout);

PltTime t0;
PltTime tt = t0.now();
struct        tm* t;
time_t        timer;

timer = (time_t)tt.tv_sec;

t=localtime(&timer);
if(t) {
    fprintf(yyout,"*   %4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d",
                t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
}
fprintf(yyout," : Automatische Erstellung\n");

fprintf(yyout,"*\n*\n");
fprintf(yyout,"*   ACHTUNG!\n");
fprintf(yyout,"*   --------\n");
fprintf(yyout,"*   DIESE DATEI DARF NICHT VERAENDERT WERDEN!\n");
fprintf(yyout,"*\n");
fprintf(yyout,"******************************************************************************/\n");
fprintf(yyout,"\n\n");
fflush(yyout);

    yyin = fopen((const char*)olddat, "r");
        if(!yyin) {
                fprintf(yyout,"%s",
                    (const char*)log_getErrMsg(KS_ERR_OK,"can't open file", (const char*)olddat));
                fclose(yyout);
                return OV_ERR_CANTOPENFILE;
        }

        ppar = (Dienst_param*)malloc(sizeof(Dienst_param));
        if( !ppar ) {
                fclose(yyin);
                fclose(yyout);
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
    *   Parser initialisieren
    */
    InitInputFileStream(yyin);

        /*
        *   Zu vergleichende Datei parsen
        */

        out = "";
    iFBS_PrintParserError(1, out); /* LastError loeschen */

        exit_status = yyparse();
        if(exit_status != EXIT_SUCCESS) {
            
            out = IFBS_GetParserError();
            if( out == "" ) {
                fprintf(yyout,"%s",
                    (const char*)log_getErrMsg(KS_ERR_OK,"Parse error. File", (const char*)olddat));
            } else {
                fprintf(yyout,"%s",
                 (const char*)log_getErrMsg(KS_ERR_OK, (const char*)out, "File", (const char*)olddat));
            }
                memfre(ppar);
        free(ppar);
        fb_parser_freestrings();
                fclose(yyin);
                fclose(yyout);
        return KS_ERR_BADPARAM;
    }

        fclose(yyin);

        oldpar = ppar;

    yyin = fopen((const char*)newdat, "r");
        if(!yyin) {            
                fprintf(yyout,"%s",
                    (const char*)log_getErrMsg(KS_ERR_OK,"can't open file", (const char*)newdat));
                memfre(oldpar);
                free(oldpar);
        fb_parser_freestrings();
                fclose(yyout);
        return OV_ERR_CANTOPENFILE;
        }

        ppar = (Dienst_param*)malloc(sizeof(Dienst_param));
        if( !ppar ) {
                memfre(oldpar);
                free(oldpar);
                fclose(yyin);
                fclose(yyout);
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
        *   Aktuelle Datei parsen
        */

        current_line = 0;

        exit_status = yyparse();
        if(exit_status != EXIT_SUCCESS) {
            
            out = IFBS_GetParserError();
            if( out == "" ) {
                fprintf(yyout,"%s",
                    (const char*)log_getErrMsg(KS_ERR_OK,"Parse error. File ", (const char*)newdat));
            } else {
                fprintf(yyout,"%s",
                    (const char*)log_getErrMsg(KS_ERR_OK, (const char*)out,"File", (const char*)newdat));
            }
                memfre(ppar);
                free(ppar);
                memfre(oldpar);
        free(oldpar);
                fclose(yyin);
                fclose(yyout);
        fb_parser_freestrings();
        return KS_ERR_BADPARAM;
    }

        fclose(yyin);

        newpar = ppar;

        /*
        *        Ausgabe erzeugen
        */

        out = "";
    fehler = compare_eval(newpar, oldpar, out);

    fputs((const char*)out, yyout);
    fclose(yyout);

        memfre(oldpar);
        free(oldpar);
        memfre(newpar);
        free(newpar);

    fb_parser_freestrings();

    return fehler;
}
