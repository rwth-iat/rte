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
*   load_lib.cpp                                                             *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   1999-04-07  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*   Laden einer Bibliothek                                                   *
*                                                                            *
*****************************************************************************/

#include "ifbslibdef.h"

/*****************************************************************************/
KS_RESULT IFBS_LOADLIB(KscServerBase*  Server,
                                               PltString       &lib) {
/*****************************************************************************/
    char      help[128];
    char*     ph;
    PltString log("");
    KS_RESULT err;
    int       len = strlen(FB_LIBRARIES_CONTAINER);

    if(!Server) {
        err = KS_ERR_SERVERUNKNOWN;
        iFBS_SetLastError(1, err, log);
        return err;
    }
    if(!lib.len()) {
        err = KS_ERR_BADNAME;
        log = "\"%s\"  \"\"";
        iFBS_SetLastError(1, err, log);
        return err;
    }

    strcpy(help, (const char*)lib);
    ph = strstr(help, FB_LIBRARIES_CONTAINER);
    if( ph ) {
        ph += len;
        if( (*ph) == '/') ph++;
        if( !(*ph) ) {
            err = KS_ERR_BADPATH;
            log = "\"%s\"  \"";
            log += lib;
            log += "\"";
            iFBS_SetLastError(1, err, log);
            return err;
        }
    } else {
        ph = help;
        if( (*ph) == '/') ph++;
        if( !(*ph) ) {
            err = KS_ERR_BADPATH;
            log = "\"%s\"  \"";
            log += lib;
            log += "\"";
            iFBS_SetLastError(1, err, log);
            return err;
        }
    }


    err = 0;
    log = "";
    iFBS_SetLastError(1, err, log);

    FbCreateInstParams pars;
    
    pars.factory = LIBRARY_FACTORY_PATH;
    pars.path = "/";
    pars.path += FB_LIBRARIES_CONTAINER;
    pars.path += "/";
    pars.path += ph;
    
    return IFBS_CREATE_INST(Server, pars);

}