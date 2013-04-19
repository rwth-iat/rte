/*****************************************************************************
*                                                                            *
*    i F B S p r o                                                           *
*    #############                                                           *
*                                                                            *
*   L T S o f t                                                              *
*   Agentur f�r Leittechnik Software GmbH                                    *
*   Heinrich-Hertz-Stra�e 10                                                 *
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
KS_RESULT IFBS_DELETE_LIB(KscServerBase*  Server,
                                                  PltString       &lib) {
/*****************************************************************************/
    char      help[256];
    char      hlib[256];
    char*     ph;
    PltString log;
    int       len = strlen(FB_LIBRARIES_CONTAINER);

    if(!lib.len()) {
        len = KS_ERR_BADNAME;
        log = "\"%s\"  \"\"";
        iFBS_SetLastError(1, len, log);
        return len;
    }

    strcpy(help, (const char*)lib);
    ph = strstr(help, FB_LIBRARIES_CONTAINER);
    if( ph ) {
        ph += len;
        if( (*ph) == '/') ph++;
        if( !(*ph) ) {
            len = KS_ERR_BADPATH;
            log = "\"%s\"  \"";
            log += lib;
            log += "\"";
            iFBS_SetLastError(1, len, log);
            return KS_ERR_BADPATH;
        }
    } else {
        ph = help;
        if( (*ph) == '/') ph++;
        if( !(*ph) ) {
            len = KS_ERR_BADPATH;
            log = "\"%s\"  \"";
            log += lib;
            log += "\"";
            iFBS_SetLastError(1, len, log);
            return KS_ERR_BADPATH;
        }
    }

    sprintf(hlib, "/%s/%s", FB_LIBRARIES_CONTAINER, ph);
    log = hlib;

    return IFBS_DELETE_OBJ(Server, log);
}
