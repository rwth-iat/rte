
#include "ifbslibdef.h"

/*
*   Globale Variable
*/
// Anzahl wiederholungen
extern long int retries_for_ia_fox;
extern int debug;

// Gesichertere Objekte
extern unsigned int VarSaved;
extern unsigned int DomSaved;

// Liste der nicht gesicherten Objekten
extern PltList<PltString> VarNotSaved;
extern PltList<PltString> DomNotSaved;

/*****************************************************************************/
// KS-Pfad splitten
/*****************************************************************************/
KS_RESULT ifb___splitPath(const PltString &path, PltList<PltString> &pList) {
/*****************************************************************************/
    // Lokale Variablen
    PltString Item;
    char      *panf;
    char      *pc;
    char      *ph;
    size_t    len;
    
    if( !path.len() ) {
        return KS_ERR_BADPATH;
    }
    
    len = path.len();
    
    ph = (char *)malloc(len + 1);
    if(!ph) {
        // Out of memory
        return KS_ERR_TARGETGENERIC;
    }
    
    strcpy( ph, (const char*)path );
    
    pc = ph;

    while(pc && (*pc) ) {
        
        while(pc && (*pc) && ((*pc) == '/') ) pc++;
   
        panf = pc;
        while(pc && (*pc) && ((*pc) != '/') ) pc++;
        if( pc && (*pc) ) {
            *pc = '\0';
            pc++;
        }
        Item = panf;
        pList.addLast(Item);
    }
    
    // Speicher freigeben
    free(ph);
    return KS_ERR_OK;
}

int main(int argc, char **argv) {
    /*
    *        local variables
    */
    PltString     filename("");
    PltString     path("localhost/xml_server");
    char*         ph;
    char          help[256];
    int           i;
    long int      retr = 5;  // Anzahl wiederholungen
    unsigned long tdur = 10; // Wartezeit zw. 2 Requests [msec]
    PltString     hs;
    KS_RESULT     err;
    PltTime       wTim;

    // Copyrigth Ausgabe
    fprintf(stderr, "\n © 2002-2010 LTSoft GmbH. Alle Rechte vorbehalten\n");
#if XMLDEMO
    fprintf(stderr, " Demoversion\n");
#endif

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
        *        set path option
        */
        else if(!strcmp(argv[i], "-p")) {
                i++;
                if(i<argc) {
                    path = argv[i];
                } else {
                    goto HELP;
                }
        }
        else if(!strcmp(argv[i], "-d")) {
            debug = 1;
        }
        else if(!strcmp(argv[i], "-retries")) {
                i++;
                if(i<argc) {
                    retr = atoi(argv[i]);
                } else {
                    goto HELP;
                }
        }
        else if(!strcmp(argv[i], "-t")) {
                i++;
                if(i<argc) {
                    tdur = (unsigned long)atol(argv[i]);
                } else {
                    goto HELP;
                }
        }
        /*
        *        display help option
        */
        else if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
HELP:                fprintf(stderr, "\nUsage: %s [arguments]\n", argv[0]);
            fprintf(stderr, 
                        "\n"
                        "The following optional arguments are available:\n"
                        "-p             PATH          Set full path '//Host/Server/Path/To/Dom' (default '//localhost/xml_server')\n"
                        "-f             NAME.XML      Set filename NAME (default Host_Server.xml)\n"
                    );
            fprintf(stderr, "-retries       RETRIES       Set number of retries (default 5)\n");
            fprintf(stderr, "-d                           Set debug mode (to stderr)\n");
            fprintf(stderr, "-t             time_duration Set time duration between two data requests [msec] (default 10)\n");
            fprintf(stderr, "-h OR --help                 Display this help message and exit\n");
            return 1;
        } else {
            goto HELP;
        }
    }

    // Anzahl wiederholungen merken;
    retries_for_ia_fox = retr;

    // Host/Server merken
    err = ifb___splitPath(path, DomNotSaved);
    if(err != KS_ERR_OK) {
        fprintf(stderr,"\n Can't get host/Server from path '%s'.\n     Error 0x%x (%s)\n\n\n",
                                 (const char*)path, err, GetErrorCode(err));
        goto HELP;
    }
    // Pfad OK?
    while( DomNotSaved.size() ) {
        hs = DomNotSaved.removeFirst();
        if(hs != "") {
            VarNotSaved.addLast( hs );
        }
    }
    if( VarNotSaved.size() < 2 ) {
        fprintf(stderr,"\n Bad path %s\n\n", (const char*)path);
        goto HELP;
    }
    hs = VarNotSaved.removeFirst();
    hs += "/";
    hs += VarNotSaved.removeFirst();

    // Pfad
    path = "";
    while( VarNotSaved.size() ) {
        path += "/";
        path += VarNotSaved.removeFirst();
    }
    if(path == "") {
        path = "/";
    }
    
    // Dateiname
    if(filename == "") {
        strcpy(help, (const char*)hs);
        ph = help;
        while( ph && (*ph) ) {
            if( ((*ph) == '.') || ((*ph) == '/') ) {
                *ph = '_';
            }
            ph++;
        }
        filename = help;
        filename += ".xml";
    }

    
    // Verbinden
    err = KS_ERR_OK;
    KscServerBase* Server = GetServerByName(hs, err);
    if(err) {
        fprintf(stderr,"\n Can't connect to //%s.\n     Error 0x%x (%s)\n\n\n",
                                 (const char*)hs, err, GetErrorCode(err));
        return 1;
    }

    // Warte-Zeit zw. 2 Requests?
    wTim.tv_sec = 0;
    wTim.tv_usec= 0;
    if(tdur > 0) {
        tdur *= 1000; // Eingabe in 'msec'
        wTim.tv_sec  = (unsigned long)(tdur/1000000);
        wTim.tv_usec = tdur - wTim.tv_sec*1000000; 
    }
    
    // Server sichern
    err = IFBS_DBSAVEXML(Server, path, filename, wTim);

    fprintf(stderr, "\n Gelesen %lu Domains  %lu Variables\n"
                ,DomSaved + DomNotSaved.size()
                ,VarSaved + VarNotSaved.size()
                );
        

    if(err) {
        fprintf(stderr,"\n Fehler beim Sicherung der Datenbasis.\n    Nr. 0x%x (%s)\n    Datei '%s'\n\n",
                         err, GetErrorCode(err), (const char*)filename);
    } else{
        fprintf(stderr,"\n Datenbasis %sgesichert. Dateiname : '%s'\n\n"
                , ((DomNotSaved.size() || VarNotSaved.size()) ? "nicht vollstaendig " : "")
                , (const char*)filename
                );
    }

    if(debug) {
        if(DomNotSaved.size()) {
            fprintf(stderr, " Folgende Domains konnten nicht gelesen werden :\n");
            while(DomNotSaved.size()) {
                fprintf(stderr, "     %s\n", (const char*)DomNotSaved.removeFirst());
            }
        }
        if(VarNotSaved.size()) {
            fprintf(stderr, " Folgende Variablen konnten nicht gelesen werden :\n");
            while(VarNotSaved.size()) {
                fprintf(stderr, "     %s\n", (const char*)VarNotSaved.removeFirst());
            }
        }
        fprintf(stderr,"\n\n");
    }

    return 0;
    
} /* main() */
