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
*   ifb_importproject.cpp                                                    *
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
*        C-Includes
*        ----------
*/
#include "ifbslibdef.h"

/*
*        globale Variablen
*        -----------------
*/

#ifdef __cplusplus
extern "C" {
#endif

Dienst_param*   ppar;

//extern int      yydebug;
//extern FILE*    yyin;
//extern int      current_line;

#ifdef __cplusplus
}
#endif

/*
*        Funktionsprototypen
*        -------------------
*/

int yyparse(void);

#ifdef __cplusplus
extern "C" {
#endif
    
struct yy_buffer_state* yy_scan_string( char* str );
void   yy_delete_buffer(struct yy_buffer_state* buf);

void InitInputFileStream(FILE*);

#ifdef __cplusplus
}
#endif

/*****************************************************************************/
KS_RESULT IFBS_DBLOAD(KscServerBase* Server,
                      PltString&     inpfile,
                      PltString&     err_outfile)
/*****************************************************************************/
{
        /*
        *        Variablen
        */
        int       exit_status;
        int       error;
    PltString out;
        int PROTOFILE = 0;
    FILE*    yyout = 0;
    
        yydebug = 0;
        current_line = 0;

        /*
        *        check option settings
        */

    if(!Server) {
            return KS_ERR_SERVERUNKNOWN;
    }
    
        if(!inpfile.len() ) {
                return KS_ERR_BADNAME;
    }

    if(err_outfile.len() ) {
        yyout = fopen((const char*)err_outfile, "a");
        if(!yyout) {
                    return OV_ERR_CANTOPENFILE;
            }

            PROTOFILE = 1;
    }

    yyin = fopen((const char*)inpfile, "r");
    if(!yyin) {
        if(PROTOFILE) {
            fprintf(yyout,"%s", (const char*)log_getErrMsg(KS_ERR_OK,"Can't open file",(const char*)inpfile));
            fclose(yyout);
        }
                return OV_ERR_CANTOPENFILE;
    }
///////////////////////////////////////////////////////////////////////////////

struct tm* t;
time_t timer;
PltTime tt = PltTime::now();

timer = (time_t)tt.tv_sec;

t=localtime(&timer);

if(PROTOFILE) {
    
fprintf(yyout,"\n\n/*********************************************************************\n");
fprintf(yyout,"======================================================================\n");        
fprintf(yyout,"  Datei : %s\n\n", (const char*)err_outfile);
fprintf(yyout,"  Laden der Datenbasis\n");
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
            fprintf(yyout, "%s", (const char*)log_getErrMsg(KS_ERR_OK, "Not enough memory to allocate buffer."));
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

    InitInputFileStream(yyin);
        exit_status = yyparse();

        fclose(yyin);

        /*
        *        Ausgabe erzeugen
        */
        if(exit_status != EXIT_SUCCESS) {  
            
            if(PROTOFILE) {
                out = IFBS_GetParserError();
                if( out == "" ) {
                    fprintf(yyout,"%s", (const char*)log_getErrMsg(KS_ERR_OK,"Parse error.","File", (const char*)inpfile));
                } else {
                    fprintf(yyout,"%s",(const char*)log_getErrMsg(KS_ERR_OK,(const char*)out,"File", (const char*)inpfile));
                }
                fflush(yyout);
                fclose(yyout);
            }
            memfre(ppar);
            free(ppar);
            fb_parser_freestrings();
                
            return KS_ERR_BADPARAM;
        }

        if(PROTOFILE) {
        fflush(yyout);
        }
        
        out = "";
    error = import_eval(Server, ppar, out);

    if(PROTOFILE) {
        fputs((const char*)out, yyout);
            fclose(yyout);
    }
    
    /* Speicher freigeben */
    memfre(ppar);
    free(ppar);
    fb_parser_freestrings();
    
    return error;
}



/*****************************************************************************/
KS_RESULT LoadDupl(KscServerBase* Server, PltString &ImpString)
/*****************************************************************************/
{
        /*
        *        Variablen
        */
        int                exit_status;
        size_t         i,j;            // Laufvariablen
        char        path[32];       // Hilfsstring

    InstanceItems*        pinst;
    LinksItems*                pLinks;

    PltString       VerbName;   // Name der Verbindung
    PltString       NewName;    // Neuen Name der Verbindung
    PltString       Assoc;      // Assoziations-Identifier
    
        yydebug = 0;
        current_line = 0;

    size_t    Count;            // Merker : Anzahl der Verbindungen
    KS_RESULT err;              // Ergebnis des Dienstes
    
    struct yy_buffer_state* buf;
    
        /*
        *        check option settings
        */

    if(!Server)
            return KS_ERR_SERVERUNKNOWN;

        if(!ImpString.len() ) {
                return KS_ERR_OK;
        }

        ppar = (Dienst_param*)malloc(sizeof(Dienst_param));
        if(!ppar) {
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
        buf = yy_scan_string( (char*)(const char*)ImpString );
    exit_status =  yyparse();
    yy_delete_buffer( buf );

    /* String geparst ? */
        if(exit_status != EXIT_SUCCESS) {
            memfre(ppar);
            free(ppar);
        fb_parser_freestrings();
        
        return KS_ERR_BADPARAM;
    }


///////////////////////////////////////////////////////////////////////////////
//  Es muessen Namen der Verbindungen geaendert werden                       //
///////////////////////////////////////////////////////////////////////////////

    sprintf(path, "/%s/",FB_CONN_CONTAINER);

    /* Anzahl der Verbindungen ermitteln */
    pinst = ppar->Instance;
    Count = 0;
    
    while(pinst) {

        if( !strncmp(pinst->Inst_name, path, 4) ) {
            // Verbindung gefunden
            Count++;
        }
        pinst = pinst->next;
    }
    
    PltArray<char*> Verbindung(Count);
    if(Verbindung.size() != Count) {
        // Out of memory ?
        err = OV_ERR_HEAPOUTOFMEMORY;
        goto EXIT_FNC;
    }
    // Array initialisieren
    for(j = 0; j < Count; j++) {
        Verbindung[j] = 0;
    }
    
    // Gibt es Verbindungen ?
    if(Count) {
        
        // Dann erzeugen wir neuen Name fuer Verbindungs-Objekt.
        GenerateComConName(Assoc);      // Neuen Name der Verbindung erzeugen
        NewName = path;
        NewName += Assoc;

        /* Verbindung-Namen aendern */
        pinst = ppar->Instance;
        j = 0;
        
        while(pinst) {
            char* ph;
            
            if( !strncmp(pinst->Inst_name, path, 4) ) {
                // Verbindung gefunden
                VerbName = pinst->Inst_name;    // Name der Verbindung merken

                ph = (char*)malloc(NewName.len() + 33);
                if(!ph) {
                    // Out of memory
                    for(i = 0; i < j; i++) {
                        if(Verbindung[i]) free(Verbindung[i]);
                    }
                    err = OV_ERR_HEAPOUTOFMEMORY;
                    goto EXIT_FNC;
                }
            
                // Neuen Name kopieren
                sprintf(ph, "%s%d", (const char*)NewName, j);

                pinst->Inst_name = ph;
                
                // String merken
                Verbindung[j] = ph;
                j++;
                
                for(i = 0; i < 2; i++) {
                    switch(i) {
                        case 0:
                            // Link "outputcon" suchen
                            Assoc = "outputcon";
                            break;
                        default:
                            // Link "inputcon" suchen
                            Assoc = "inputcon";
                            break;
                    }
                
                        pLinks = ppar->Links;
                        while(  (pLinks) ) {
                            if( (VerbName == pLinks->children->child_path) &&
                                (Assoc == pLinks->child_role) ) {
                                    break;
                             }
                            pLinks = pLinks->next;
                        }
                        if(!pLinks) {
                        // Link nicht gefunden
                        for(i = 0; i < j; i++) {
                            if(Verbindung[i]) free(Verbindung[i]);
                        }
                        err = KS_ERR_BADPARAM;
                        goto EXIT_FNC;
                        }

                        // Neuen Name in Link-String merken
                        pLinks->children->child_path = ph;
                    
                } // Links suchen
                } // if Verb. gefunden
            
                pinst = pinst->next;    // Naechste Instance
            
        } // while Instanzen
    } // if Verbindungen vorhanden
    
    // Alle Verbindungen und Links umbennant.
    // Instanzen anlegen :
    
    VerbName = "";
    err = import_eval(Server, ppar, VerbName);

    // Neue erzeugte Strings freigeben
    for(i = 0; i < Count; i++) {
        if(Verbindung[i]) free(Verbindung[i]);
    }
    
EXIT_FNC:
        memfre(ppar);
        free(ppar);
    fb_parser_freestrings();

    return err;
}
