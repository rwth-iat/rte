/*
*        KS-Includes
*        -----------
*/
#include "ifbslibdef.h"



/*****************************************************************************/
int main(int argc, char **argv) {
/*****************************************************************************/

    char      licstring[128];
    
    
    PltString HaS;                          // Hilfsstring
    PltString FbPath("/fb");                // Pfad der FB-Library
    bool      FbFound = FALSE;              // Merker : FB-Lib noch nicht gefunden
    KS_RESULT err;                          // Ergebnis der Diensten
    char      *pHS = "localhost/ov_server"; // Host/Server
    char      *pS = pHS;                    // Host/Server
    char      *pT = 0;                      // Ticket
    char      *ph;                          // Hilfszeiger
    char      help[512];                    // Hilfsstring
    FbCreateInstParams pars;                // Create-Dienst Parameter
    int       i;                            // Laufvariable
    
    time_t timer;
    struct tm *tblock;
    /* Erhält die Tageszeit */
    timer = time(NULL);
    /* Wandelt Datum und Zeit in eine Struktur um */
    tblock = localtime(&timer);

    sprintf(licstring, "Copyright © LTSoft GmbH 2002-%d", (tblock->tm_year + 1900) );

        for(i=1; i<argc; i++) {
                /*
                *        set host und server name option
                */
                if(!strcmp(argv[i], "-s") ) {
                        i++;
                        if(i<argc) {
                                pS = argv[i];
                        } else {
                        fprintf(stderr, "\n Missing server name\n");
                                goto HELP;
                        }
                }
                else if(!strcmp(argv[i], "-t") ) {
                        i++;
                        if(i<argc) {
                                pT = argv[i];
                        } else {
                        fprintf(stderr, "\n Missing ticket\n");
                                goto HELP;
                        }
                }
                /*
                *        display help option
                */
                else if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
                    fprintf(stderr, "\n %s\n", licstring);
HELP:
                    fprintf(stderr, "\nUsage: %s [arguments]\n\n", argv[0]);
                        fprintf(stderr, "The following optional arguments are available:\n");
                        fprintf(stderr, "-s             HOST/SERVER   Set host and server name (default '%s')\n", pHS);
                        fprintf(stderr, "-t             TICKET        Set ticket identification for server access\n");
                        fprintf(stderr, "-h OR --help                 Display this help message and exit\n");
                        return 1;
                } else {
                      fprintf(stderr, "\n %s\n\n Bad option '%s'\n", licstring, argv[i]);
                        goto HELP;
                }
        }
        
        // Host und Server Name
        ph = pS;
        while(ph && ((*ph) == '/')) ph++;   // Vorangestelle '/' ausschneiden
        if( (!ph) || (!(*ph)) ) {
            fprintf(stderr, "\n Bad server name '%s'\n", pS);
            goto HELP;
        }
        HaS = ph;
    KscServerBase* Server = GetServerByName(HaS, err);
    if(err != KS_ERR_OK) {
        fprintf(stderr, "\n Can't contact to server.\n");
        return 1;
    }
    if(!Server) {
        fprintf(stderr, "\n Can't contact to server.\n");
        return 1;
    }

    if(pT) {
        // Client - Ticket setzen
        HaS = pT;
        err = SetClientAV( HaS );
        if(err != KS_ERR_OK) {
            fprintf(stderr, "\n Can't set ticket.\n");
            return 1;
        }
    }
        
    // Ist die FB-Dll schon geladen ?
    HaS = (const char*)(Server->getHostAndName());
    HaS += LIBRARY_FACTORY_PATH;
    HaS += ".instance";

    KscVariable Var(HaS);
    if(!Var.getUpdate() ) {
        KS_RESULT err = Var.getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
        fprintf(stderr, "\n Can't read libraries.\n");
        return 1;
    }
    const KsVarCurrProps *cp = Var.getCurrProps();
    if((!cp) || (!(cp->value)) ) {
        fprintf(stderr, "\n Can't read libraries.\n");
        return 1;
    }
    switch(cp->value->xdrTypeCode() ) {
        case KS_VT_STRING_VEC: {
                size_t size = ((KsStringVecValue &) *cp->value).size();
                size_t ii;
                for ( ii = 0; ii < size; ii++ ) {
                    strcpy(help, (const char*)( ((KsStringVecValue &) *cp->value)[ii]) );
                    ph = help;
                    // Ende des String suchen
                    while(ph && (*ph)) ph++;
                    // Letzten '/' suchen
                    while((ph != help) && (*ph != '/')) ph--;
                    if(FbPath == ph) {
                        FbFound = TRUE;
                        FbPath = help;
                        break;
                    }
                }
            }
            break;
        default:
            fprintf(stderr, "\n Can't read libraries.\n");
            return 1;
    }
    if(!FbFound) {
        // FB-Dll noch nicht geladen
        pars.factory = LIBRARY_FACTORY_PATH;
        pars.path = FbPath;
        err = IFBS_CREATE_INST(Server, pars);
        if(err) {
            fprintf(stderr, "\n Can't load 'FB'-Library. Error 0x%x (%s)\n",
                    err, GetErrorCode(err));
            return 1;
        }
    }

    // 'Libraries' - Container angelegt ?
    pars.factory = CONTAINER_CLASS_PATH;
    pars.path = "/";
    pars.path += FB_LIBRARIES_CONTAINER;
    err = IFBS_CREATE_INST(Server, pars);
    if(err) {
        if(err != KS_ERR_ALREADYEXISTS) {
            fprintf(stderr, "\n Can't create '%s'-Container. Error 0x%x (%s)\n",
                    FB_LIBRARIES_CONTAINER, err, GetErrorCode(err));
            return 1;
        }
    }
    // 'TechUnits' - Container angelegt ?
    pars.path = "/";
    pars.path += FB_INSTANZ_CONTAINER;
    err = IFBS_CREATE_INST(Server, pars);
    if(err) {
        if(err != KS_ERR_ALREADYEXISTS) {
            fprintf(stderr, "\n Can't create '%s'-Container. Error 0x%x (%s)\n",
                    FB_INSTANZ_CONTAINER, err, GetErrorCode(err));
            return 1;
        }
    }
    // 'Cons' - Container angelegt ?
    pars.path = "/";
    pars.path += FB_CONN_CONTAINER;
    err = IFBS_CREATE_INST(Server, pars);
    if(err) {
        if(err != KS_ERR_ALREADYEXISTS) {
            fprintf(stderr, "\n Can't create '%s'-Container. Error 0x%x (%s)\n",
                    FB_CONN_CONTAINER, err, GetErrorCode(err));
            return 1;
        }
    }
    // 'Tasks' - Container angelegt ?
    pars.path = "/";
    pars.path += FB_TASK_CONTAINER;
    err = IFBS_CREATE_INST(Server, pars);
    if(err) {
        if(err != KS_ERR_ALREADYEXISTS) {
            fprintf(stderr, "\n Can't create '%s'-Container. Error 0x%x (%s)\n",
                    FB_TASK_CONTAINER, err, GetErrorCode(err));
            return 1;
        }
    }
    
    // 'serverinfo' - Container angelegt ?
    pars.path = FB_LOGGER_CONTAINER_PATH;
    err = IFBS_CREATE_INST(Server, pars);
    if(err) {
        if(err != KS_ERR_ALREADYEXISTS) {
            fprintf(stderr, "\n Can't create '%s'-Container. Error 0x%x (%s)\n",
                    FB_LOGGER_CONTAINER, err, GetErrorCode(err));
            return 1;
        }
    }
    
    // 'dbinfo' angelegt ?
    pars.factory = FbPath;
    pars.factory += "/";
    pars.factory += FB_DBINFO;
    pars.factory += "class";
    pars.path = FB_LOGGER_CONTAINER_PATH;
    pars.path += "/";
    pars.path += FB_DBINFO;
    err = IFBS_CREATE_INST(Server, pars);
    if(err) {
        if(err != KS_ERR_ALREADYEXISTS) {
            fprintf(stderr, "\n Can't create '%s'-Instance. Error 0x%x (%s)\n",
                FB_DBINFO, err, GetErrorCode(err));
            return 1;
        }
    }
    
    // 'UrTask' angelegt ?
    pars.factory = FbPath;
    pars.factory += "/task";
        pars.path = "/";
        pars.path += FB_TASK_CONTAINER;
        pars.path += "/";
        pars.path += FB_URTASK;
    err = IFBS_CREATE_INST(Server, pars);
    if(err) {
        if(err != KS_ERR_ALREADYEXISTS) {
            fprintf(stderr, "\n Can't create %s. Error 0x%x (%s)\n",
                    FB_URTASK, err, GetErrorCode(err));
            return 1;
        }
    }

    // 'UrTask' einschalten
    PltList<PltString> ValList;
    ValList.addLast("1.0");
    HaS = pars.path;
    HaS += ".cyctime";
    ifb_setVar(Server, HaS, ValList, KS_VT_TIME_SPAN, 0);

    while(ValList.size()) ValList.removeFirst();  // Liste leeren
    ValList.addLast("1");
    HaS = pars.path;
    HaS += ".actimode";
    ifb_setVar(Server, HaS, ValList, KS_VT_INT, 0);

#if 0
    // Logger angelegt?
    pars.factory = FbPath;
    pars.factory += "/Logger";
        pars.path = FB_LOGGER_PATH;
    err = IFBS_CREATE_INST(Server, pars);
    if(err) {
        if(err != KS_ERR_ALREADYEXISTS) {
            fprintf(stderr, "\n Can't create logger. Error 0x%x (%s)\n",
                    err, GetErrorCode(err));
        }
    }
    // FileUpload angelegt?
    pars.factory = FbPath;
    pars.factory += "/fileUpload";
        pars.path = FB_UPLOAD_PATH;
    err = IFBS_CREATE_INST(Server, pars);
    if(err) {
        if(err != KS_ERR_ALREADYEXISTS) {
            fprintf(stderr, "\n Can't create instance. Error 0x%x (%s)\n",
                    err, GetErrorCode(err));
        }
    }
#endif

    return 0;
} /* main() */
