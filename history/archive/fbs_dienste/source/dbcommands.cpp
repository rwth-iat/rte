
#include "ifbslibdef.h"

void getFileNameFromHS(PltString hs, PltString &filename, PltString &logfile) {
    char            help[256];
    char            *ph;
    
    if(filename == "") {
        strcpy(help, (const char*)hs);
        ph = help;
        while( (*ph) ) {
            if( ((*ph) == '.') || ((*ph) == '/') ) {
                *ph = '_';
            }
            ph++;
        }
    
    } else {
        strcpy(help, (const char*)filename);
        ph = help;

        while ( ph && (*ph) ) ph ++;
        while(  (ph  != help) &&
           ((*ph) != '.')  &&
           ((*ph) != ']')  &&
           ((*ph) != ':')   ) ph --;

        if( (*ph) == '.' ) {
            *ph = '\0';
        }
    }
    filename = help;
    filename += ".fbd";

    logfile = help;
    logfile += ".log";
  
    
    return;
}

int doOneServer(PltString hs, PltString filename, PltString logfile
                ,int saveId, int cleanId, int loadId
                ,unsigned int anzLibs
                ,PltArray<PltString> *pLibArr
                ,PltString pwd) {
  
    KscServerBase*  Server;
    int             err;
    unsigned int    i;
    PltString       libName;
    PltString       Out;
    
    Server = GetServerByName(hs, err);
    if(err) {
        fprintf(stderr," Can't connect to %s.\n     Error 0x%x (%s)\n\n\n",
                                     (const char*)hs, err, GetErrorCode(err));
        return 1;
    }
    
    
    /* Datenbasis sichern */
    if(saveId) {
        err = IFBS_DBSAVE(Server, filename);
        if(err) {
            fprintf(stderr," Fehler beim Sichern der Datenbasis.\n    Nr. 0x%x (%s)\n    Datei '%s'\n\n\n",
                             err, GetErrorCode(err), (const char*)filename);
            return 1;
        } else{
            fprintf(stderr," Datenbasis '%s' gesichert. Dateiname : '%s'\n",
                            (const char*)hs, (const char*)filename);
        }
    }
 
    /* Datenbasis loeschen */
    if(cleanId) {
        err = IFBS_CLEANDB(Server, 0);
        if(err) {
            fprintf(stderr," Fehler beim Loeschen der Datenbasis '%s'.\n    Nr. 0x%x (%s)\n\n",
                            (const char*)hs, err, GetErrorCode(err));
            return 1;
        } else{
            fprintf(stderr," Datenbasis '%s' geloescht\n", (const char*)hs);
        }
    }
 
    /* Bibliothek upload */
    for(i=0; i<anzLibs; i++) {
        libName = (*pLibArr)[i];
        err = IFBS_UPLOAD_LIBRARY(Server, libName, pwd, Out);
        if(err) {
            fprintf(stderr," %s : Fehler beim laden der Bibliothek '%s'.\n",
                     (const char*)hs, (const char*)libName);
            if(Out == "") {
                fprintf(stderr,"    Nr. 0x%x (%s)\n\n", err, GetErrorCode(err));
            } else {
                fprintf(stderr,"    %s\n\n", (const char*)Out);
            }
            return 1;
        } else{
            fprintf(stderr," %s: Bibliothek '%s' kopiert\n",
                         (const char*)hs, (const char*)libName);
        }
    }
 
    /* Datenbasis laden */
    if(loadId) {
        err = IFBS_DBLOAD(Server, filename, logfile);
        if(err) {
            fprintf(stderr," Fehler beim Laden der Datenbasis '%s'.\n    Nr. 0x%x (%s)\n\n",
                             (const char*)hs, err, GetErrorCode(err));
            return 1;
        } else{
            fprintf(stderr," Datenbasis '%s' geladen\n\n\n", (const char*)hs);
        }
    }
  
    return 0;
}

int doAllServers(PltString  hst
                 ,PltString filename
                 ,int       saveId
                 ,int       cleanId
                 ,int       loadId
                 ,int       protoId
                 ,unsigned int anzLibs
                 ,PltArray<PltString> *pLibArr
                 ,PltString pwd) {

        char*                    ph;
    char            help[256];
    PltString       HS;
    KS_RESULT       err;
    PltString       filName;
    PltString       logfile;
    int             ret = 0;
    KscServerBase*  Server;
    KsGetEPParams  param;
    KsGetEPResult  result;
    
    size_t         i;
    size_t         anzHS;
    
    // Host-Name vom Host/Server-String holen
    strcpy(help, (const char*)hst);
    ph = help;
    while( ph && (*ph) ) {
        if( (*ph) == '/') {
            *ph = '\0';
            break;
        }
        ph++;
    }
    hst = help;
    
    // Alle FB-Servers holen
    HS = hst;
    HS += "/";
    HS += "MANAGER";
    
    Server = GetServerByName(HS, err);
    if(err) {
        fprintf(stderr,"\n Can't connect to host '%s'.\n     Error 0x%x (%s)\n\n\n",
                (const char*)hst, err, GetErrorCode(err));
        return 1;
    }
    
    param.path = "/servers";
    param.name_mask = "fb_*";
    param.type_mask = KS_OT_DOMAIN;
    param.scope_flags = KS_EPF_DEFAULT;

    bool ok = Server->getEP(0, param, result);
    if ( !ok ) {
        err = Server->getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
    } else {
       if ( result.result != KS_ERR_OK ) {
           err = result.result;
       }
    }
   
    if(err) {
        fprintf(stderr,"\n Can't read from host '%s'.\n     Error 0x%x (%s)\n\n\n",
                (const char*)hst, err, GetErrorCode(err));
        return err;
    }
    
    if( !result.items.size() ) {
        fprintf(stderr,"\n Keine FB-Server vorhanden ( Host '%s').\n\n", (const char*)hst );
        return 1;
    }

    anzHS = result.items.size();
    PltArray<PltString> hsLst(anzHS);
    anzHS = 0;
    
    while ( result.items.size() ) {

        KsEngPropsHandle hpp = result.items.removeFirst();
        if(!hpp) {
            // ??
            continue;
        }
        
        HS = hst;
        HS += "/";
        HS += (const char*)hpp->identifier;
        
        hsLst[anzHS] = HS;
        anzHS++;
    }
    
    // Alle Server sichern und loeschen
    for(i=0; i<anzHS; i++) {
        filName = "";
        if(anzHS == 1) {
            if(filename != "") {
                filName = filename;
            }
        }
        
        HS = hsLst[i];
        getFileNameFromHS(HS, filName, logfile);
        if(protoId == 0) {
            logfile = "";
        }
        
        err = doOneServer(HS, filName, logfile, saveId, cleanId, 0, 0, 0, pwd);
        if(err != 0) {
            ret = 1;
        }
    }

    // Bibliotheken und DB laden
    for(i=0; i<anzHS; i++) {
        filName = "";
        if(anzHS == 1) {
            if(filename != "") {
                filName = filename;
            }
        }
        
        HS = hsLst[i];
        getFileNameFromHS(HS, filName, logfile);
        if(protoId == 0) {
            logfile = "";
        }
        
        err = doOneServer(HS, filName, logfile, 0, 0, loadId, anzLibs, pLibArr, pwd);
        if(err != 0) {
            ret = 1;
        }
    }
    
    return ret;
}





int main(int argc, char **argv) {
    /*
    *        local variables
    */
    PltString       filename("");
    PltString       logfile("");
    PltString       AV("");
    PltString       PWD("");
    char*           servername = "localhost/fb_database";
    int             i;
    int             saveId   = 0;
    int             loadId   = 0;
    int             cleanId  = 0;
    int             allId    = 0;
    int             protoId  = 1;
    
    unsigned int    l;
    unsigned int    libNr    = 0;
    PltArray<PltString> *libArr = 0;
    
        /*
        *        parse command line arguments
        */
        for(i=1; i<argc; i++) {
                /*
                *        set database filename option
                */
                if(!strcmp(argv[i], "-f") ) {
                        i++;
                        if(i<argc) {
                                filename = argv[i];
                        } else {
                                goto HELP;
                        }
                }
                /*
                *        set server option
                */
                else if(!strcmp(argv[i], "-s")) {
                        i++;
                        if(i<argc) {
                servername = argv[i];
                        } else {
                                goto HELP;
                        }
                }
                /*
                *        AV-Ticket
                */
                else if(!strcmp(argv[i], "-t")) {
                        i++;
                        if(i<argc) {
                AV = argv[i];
                        } else {
                                goto HELP;
                        }
                }
                /*
                *        Upload Password
                */
                else if(!strcmp(argv[i], "-uploadPwd")) {
                        i++;
                        if(i<argc) {
                PWD = argv[i];
                        } else {
                                goto HELP;
                        }
                }
                /*
                *        set option
                */
                else if(!strcmp(argv[i], "-save")) {
                        saveId = 1;
                }
                else if(!strcmp(argv[i], "-load")) {
                        loadId = 1;
                }
                else if(!strcmp(argv[i], "-clean")) {
                        cleanId = 1;
                }
                else if(!strcmp(argv[i], "-nolog")) {
                        protoId = 0;
                }
                else if(!strcmp(argv[i], "-upload")) {
                        i++;
                        if(i<argc) {
                            PltArray<PltString> *pLib = new PltArray<PltString>(libNr + 1);
                            for(l=0; l<libNr; l++) {
                    (*pLib)[l] = (*libArr)[l]; 
                }
                (*pLib)[libNr] = argv[i];
                
                // Speicher freigeben
                if(libArr) {
                    delete libArr;
                }
                
                // Bibliothek merken
                libArr = pLib;
                            libNr++;     // Eine Bibliothen mehr
                        } else {
                                goto HELP;
                        }
                }
                else if(!strcmp(argv[i], "-reload")) {
                        saveId = 1;
                        loadId = 1;
                        cleanId = 1;
                }
                else if(!strcmp(argv[i], "-all")) {
                        allId = 1;
                }
                /*
                *        display help option
                */
                else if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
HELP:                fprintf(stderr, "\nUsage: fb_dbcommands [arguments]\n"
                                "\n"
                                "The following optional arguments are available:\n"
                                "-s            HOST[/SERVER]  Set host and server name (default 'localhost/fb_database')\n"
                                "-f            NAME           Set filename NAME.fbd (default HOST_SERVER.fbd)\n"
                                "-t            TICKET         Set AV-ticket\n"
                                "-save                        Save database\n"
                                "-clean                       Clean database\n"
                                "-load                        Load database\n"
                                "-reload                      Save, clean and load database\n"
                                "-upload       LIB_PATH       upload library 'LIB_PATH'\n"
                                "-uploadPwd    PASSWORD       password for replace library\n"
                                "-all                         Save, clean or load all fb-server on host HOST (option \"-s HOST\")\n"
                                "-nolog                       Do not protocol file\n"
                                "-h OR --help                 Display this help message and exit\n"
                                "\n"
                                "Sample:\n"
                                "> fb_dbcommands -s localhost/fb_database -reload -upload [path]mylib.dll -uploadPwd geheim\n"
                                "\n"
                                "  fb_dbcommands steps:\n"
                                "  1. Save database (file localhost_database.fdb)\n"
                                "  2. Clean database localhost/fb_database\n"
                                "  3. Upload library mylib.dll\n"
                                "  4. Load database localhost/fb_database from file localhost_database.fdb\n"
                                "\n");
                        return 1;
                } else {
                    fprintf(stderr, "\n\nUnknown option '%s'\n", argv[i]);
                        goto HELP;
                }
        }

    if((saveId + loadId + cleanId + libNr) == 0 ) {
        fprintf(stderr, "\n\n Option ?\n");
        goto HELP;
    }


 PltString  hs(servername);
 KS_RESULT err;

 /* Ticket setzen */
 SetClientAV(AV);
 
 // Alle FB-Servers ?
 if(allId) {
    err = doAllServers(hs, filename, saveId, cleanId, loadId, protoId, libNr, libArr, PWD);
    return err;
 } else {
    getFileNameFromHS(hs, filename, logfile);
    if(protoId == 0) {
        logfile = "";
    }
    err = doOneServer(hs, filename, logfile, saveId, cleanId, loadId, libNr, libArr, PWD);
 }
 
 return err ? 1 : 0;
 
} /* main() */

