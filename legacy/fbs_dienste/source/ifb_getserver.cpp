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
*   Fax      : 02273/52526                                                   *
*   e-Mail   : ltsoft@ltsoft.de                                              *
*   Internet : http://www.ltsoft.de                                          *
*                                                                            *
******************************************************************************
*                                                                            *
*    iFBSpro  ACPLT/KS Dienste-Schnittstelle (C++)                           *
*   ================================================                         *
*                                                                            *
*   Datei                                                                    *
*   -----                                                                    *
*   get_server.cpp                                                           *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   1999-12-01  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*   Prueft ob Server existiert, bzw. aktuell ist oder erzeugt einen neuen    *
*                                                                            *
*****************************************************************************/

//  Includes
//  --------
#include "ifbslibdef.h"

/*****************************************************************************/
KscServerBase *GetServerByName( PltString &HaS, KS_RESULT &err)
/*****************************************************************************/
{
    static KscDomain*       premember = 0;
    static KscServerBase*   serv = 0;
    int                                SUCHE_SERVER = 0;
//    int                                SUCHE_SERVER = 1;
    KsString                h_a_s("//");
    PltString               log("");
  
    h_a_s += HaS;
    err = KS_ERR_OK;
    iFBS_SetLastError(1, err, log);            // Alte Error-Meldung loeschen

    if(!serv) {
        SUCHE_SERVER = 1;
    } else {
        if(h_a_s != serv->getHostAndName() ) {
            SUCHE_SERVER = 1;
        } else {
            // Ist der Server noch "ansprechbar" ?
            KsGetEPParams params;
            params.path = "/vendor/server_time";
            params.type_mask = KS_OT_ANY;
            params.name_mask = "*";
            params.scope_flags = KS_EPF_DEFAULT;
            if( Get_getEP_ErrOnly(serv, params) != KS_ERR_OK ) {
                // Server ist nicht mehr vorhanden
                SUCHE_SERVER = 1;
            }
        }
    }
    
    
    if(SUCHE_SERVER) {

        h_a_s += "/";
        serv   = NULL;
        if(premember) delete premember;

        premember = new KscDomain(h_a_s);
        if(!premember) {
            err =  OV_ERR_HEAPOUTOFMEMORY;
            iFBS_SetLastError(1, err, log);
            return NULL;
        }

        /* Gibt es ueberhaupt diese Domain ? */

        if( !premember->getEngPropsUpdate() ) {
            err =  premember->getLastResult();
            if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
            log = "\"%s\"  \"";
            log += HaS;
            log += "\"";
            iFBS_SetLastError(1, err, log);
            return NULL;
        }
        /* Server holen : */

        serv = premember->getServer();

        if ( !serv ) {
            err = premember->getLastResult() ;
            if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
            log = "\"%s\"  \"";
            log += HaS;
            log += "\"";
            iFBS_SetLastError(1, err, log);
            return NULL;
        }
    } /* if SUCHE_SERVER */

    return serv;        
}
