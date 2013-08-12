/*****************************************************************************
*                                                                            *
*    i F B S p r o                                                           *
*   ###############                                                          *
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
*   xiFBSpro - Bibliothek                                                    *
*   xIFBSPRO - iFBSpro  ACPLT/KS Dienste-Schnittstelle (C++)                 *
*   ========================================================                 *
*                                                                            *
*   Datei                                                                    *
*   -----                                                                    *
*   ifb_fileup.cpp                                                        *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   2006-01-30  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*   File upload                                                              *
*                                                                            *
*****************************************************************************/

#include "ifbslibdef.h"
#include <sys/stat.h>

/*
*        globale Variablen
*        -----------------
*/
extern KscServerBase* Server;

//  Definitionen
//  ============
extern "C" void lts_encode(FILE* infile,char **outbuf,char *cmd,char *fileName,char* pwd);

//  FN: GET UP
//  ==========


KS_RESULT IFBS_UPLOAD_GETPATH(KscServerBase*  Server, PltString &path) {

    KsGetEPParams       params;           /* Parameter für getPP-Service       */
    KsGetEPResult            result;
    size_t              size;
    size_t              i;
    
    KS_RESULT fehler;
    
    /* INIT */
    path = "";
    
    /*
    *        Alle Variablen und Klassen holen :
    */
    params.path = FB_UPLOAD_PATH;
    params.type_mask = KS_OT_ANY;
    params.name_mask = "*";
    params.scope_flags = KS_EPF_DEFAULT;

    bool ok = Server->getEP(0, params, result);
    if( !ok ) {
        fehler = Server->getLastResult();
        if(fehler == KS_ERR_OK) fehler = KS_ERR_GENERIC;
            return fehler;
    }
    // Instanz gefunden?
    if ( result.result == KS_ERR_OK ) {
        if ( result.items.size() ) {
            path = FB_UPLOAD_PATH;
            return KS_ERR_OK;
        }
    }

    // Instanz suchen
    KsString            Var;
    KsString            root;
    KsString        instPath;
    char            *ph;
    char            *help;
    
    root = Server->getHostAndName();
    Var = OV_VARLIBS_PATH;

    KscVariable v(root+Var); 
    if(!v.getUpdate() ) {
        fehler = v.getLastResult();
        if(fehler == KS_ERR_OK) fehler = KS_ERR_GENERIC;
            return fehler;
    }
    const KsVarCurrProps *cp = v.getCurrProps();
    if((!cp) || (!(cp->value)) ) {
            return KS_ERR_GENERIC;
    }
    size = ((KsStringVecValue &) *cp->value).size();
    for ( i = 0; i < size; i++ ) {
        instPath = ((KsStringVecValue &) *cp->value)[i];
        // String-Ende suchen
        help = (char*)((const char*)instPath);
        ph = help;
        while(ph && (*ph)) ph++;
        // Letzten '/' suchen
        while(ph != help) {
            ph--;
            if( (*ph) == '/') {
                ph++;
                break;
            }
        }
        Var = ph;
        if(Var != "fbsys") {
            // Eine andere Bibliothek
            instPath = "";
        }
        
        // FBSYS gefunden?
        if(instPath != "") {
            break;
        }
    }
    
    // FBSYS-Bibliothek gefunden?
    if(instPath == "") {
        // Bibliothek nicht geladen
        return KS_ERR_OK;
    }
    
    /*
    *        Variable lesen
    */
    instPath += "/fileUpload.instance";
    
    KscVariable V(root+instPath); 
    if(!V.getUpdate() ) {
        fehler = V.getLastResult();
        if(fehler == KS_ERR_OK) fehler = KS_ERR_GENERIC;
            return fehler;
    }
    cp = V.getCurrProps();
    if((!cp) || (!(cp->value)) ) {
            return KS_ERR_GENERIC;
    }
    size = ((KsStringVecValue &) *cp->value).size();
    if(size == 0) {
        // Instanz nicht angelegt
        return KS_ERR_OK;    
    }
    
    // Holen wir die 1. Instanz
    path = (const char*)((KsStringVecValue &) *cp->value)[0];    

    return KS_ERR_OK;
}

//  HFN: Fehlertext beim UPLOAD
//  ===========================
void getUpErrText(PltString &Path, PltString &Out) {

    KscVariable Var(Path);
    if(!Var.getUpdate() ) {
        return;
    }
    const KsVarCurrProps *cp = Var.getCurrProps();
    if((!cp) || (!(cp->value)) ) {
        return;
    }
        
        if(cp->value->xdrTypeCode() != KS_VT_STRING) {
        return;
    }
    
    ifb_getValueOnly(cp, Out, 1,0,0,0,"",0,0);
    return;
}

//  FN: UPLOAD
//  ==========
KS_RESULT IFBS_UPLOAD(
    KscServerBase*  Server,
    PltString       cmd,        // "c" fuer 'Copy', "r" fuer 'Replace'
    PltString       inFile,     // Pfad der zu kopierenden Datei
    PltString       upPath,     // Pfad der Upload-Instanz auf Remote-Server
    PltString       pwd,        // Passwort fuer Replace-Cmd
    PltString       outFil,     // Name der Datei fuer Remote-Server
    PltString      &Out         // Fehlermeldung im Klartext
    ) {

    KS_RESULT fehler;               // Funktionsrueckmeldung
    FILE*     in;                   // File-Descriptor

    struct stat     statbuf;        // Datei Informationen
    char*           databuf;        // Datei Inhalt
    PltString       pathStateVar;   // Pfad der Status-Variable
    PltString       pathBufferVar;  // Pfad der Buffer-Variable
    
    Out = "";
    

    // Datei zum Lesen oeffnen
    in = fopen((const char*)inFile, "rb");
    if(!in) {
        Out = "Can't open file for read";
        return KS_ERR_BADPATH;
    }
    
    /* Datei-Groesse ermitteln */
    if(fstat(fileno(in), &statbuf) ) {
        fclose(in);
        Out = "Can't read file";
        return KS_ERR_GENERIC;
    }
    
    /* 
    *   Daten-Buffer anlegen. Aufbau: "R _pwd_ FileName DATA"
    *       Merke: Kodiert sind die Strings ca. 40% groesser.
    *       Legen wir den Buffer doppelt so gross, dann sind wir in jedem
    *       Fall auf der sicherer Seite.
    */
    databuf = (char*)malloc(2 * (outFil.len() + pwd.len() + statbuf.st_size + 5));
    if(!databuf) {
        fclose(in);
        Out = "Out of memory";
        return KS_ERR_TARGETGENERIC;
    }

    /* Inhalt kodieren */
    lts_encode(in, &databuf,
               (char*)(const char*)cmd,
               (char*)(const char*)outFil,
               (char*)(const char*)pwd);
    
    /* Input-Datei schliessen */
    fclose(in);

    // Rest der Daten aufbereiten
    pathBufferVar = upPath;
    pathBufferVar += ".buffer";
    outFil = databuf;
    
    // Speicher freigeben
    free(databuf);

    // Absenden
    fehler = ifb_SeparateValueAndSet(Server, pathBufferVar, outFil, KS_VT_STRING, KS_ST_GOOD);
    if(fehler != KS_ERR_OK) {
        if(fehler == KS_ERR_BADVALUE) {
            // Fehler im Klartext holen
            pathStateVar = (const char*)Server->getHostAndName();
            pathStateVar += upPath;
            pathStateVar += ".State";
            getUpErrText(pathStateVar, Out);
            if(Out == "") {
                Out = GetErrorCode(fehler);
            }
        } else {
            Out = GetErrorCode(fehler);
        }
        
        return fehler;        
    }
    
    return KS_ERR_OK;        
}

//  FN: UPLOAD FILE
//  ===============
KS_RESULT IFBS_UPLOAD_LIBRARY(
    KscServerBase*  Server,
    PltString       inFile,     // Pfad der zu kopierenden Datei
    PltString       pwd,        // Passwort fuer Replace-Cmd
    PltString      &Out         // Fehlermeldung im Klartext
    ) {
    
    PltString       cmd;        // "c" fuer 'Copy', "r" fuer 'Replace'
    PltString       upPath;     // Pfad der Upload-Instanz auf Remote-Server
    PltString       outFil;     // Name der Datei fuer Remote-Server
    KS_RESULT       res;        // Fehlermeldung
    char            help[1024]; // Hilfsstring
    char           *ph;         // Hilfszeiger
    
    Out = "";
    
    // Pfad OK?
    if(inFile == "") {
        Out = "Missing file name";
        return KS_ERR_BADPATH;
    }
    
    // Pfad der Upload-instanz suchen
    res = IFBS_UPLOAD_GETPATH(Server, upPath);
    if(res != KS_ERR_OK) {
        return res;
    }
    if(upPath == "") {
        Out = "Instance 'fileUpload' not found";
        return KS_ERR_BADPATH;
    }
    
    // Pfad fuer Server
    strncpy(help, (const char*)inFile, 1023);
    ph = help;
    while(ph && (*ph)) ph++;    // String-Ende suchen
    // Datei-Name ausschneiden
    while( (*ph != '/') &&
#if PLT_SYSTEM_NT
           (*ph != '\\') &&
#endif
#if PLT_SYSTEM_OPENVMS
           (*ph != ']') && (*ph != '>') && (*ph != ':') &&
#endif
           (ph != help) ) {
            ph--;
     }
     if(ph != help) {
        ph++;
    }
    outFil = "libs/";
    outFil += ph;
    
    cmd = "r";
    return IFBS_UPLOAD(Server,cmd,inFile,upPath,pwd,outFil,Out);
}
