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
*   ifb_cleandb.cpp                                                          *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   2000-01-03  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*   Loescht alle in der Datenbasis vorhandene Anwender-Instanzen und         *
*   Anwender-Bibliotheken                                                    *
*                                                                            *
*****************************************************************************/

#include "ifbslibdef.h"

/******************************************************************************/
KS_RESULT IFBS_CLEANDB(KscServerBase*  Server, PltString*  Logging) {
/******************************************************************************/

    KS_RESULT err = 0;
    PltString log("");
    iFBS_SetLastError(1, err, log);
  
    if(!Server) {
        err = KS_ERR_SERVERUNKNOWN;
        iFBS_SetLastError(1, err, log);
        return KS_ERR_SERVERUNKNOWN;
    }

    // Logging Ausgaben ?
    if(Logging && (*Logging) ) {
        // Log-Stream initialisieren
        *Logging = "";
    }
    err = clean_all_comcons(Server, Logging);
    if(err) {
        log = IFBS_GetLastLogError();
        if(log == "") {
            iFBS_SetLastError(1, err, log);
        }
        return err;
    }

    err = clean_all_inst(Server, Logging);
    if(err) {
        log = IFBS_GetLastLogError();
        if(log == "") {
            iFBS_SetLastError(1, err, log);
        }
        return err;
    }

    err = clean_all_tasks(Server, Logging);
    if(err) {
        log = IFBS_GetLastLogError();
        if(log == "") {
            iFBS_SetLastError(1, err, log);
        }
        return err;
    }

    err = clean_all_libs(Server, Logging);
    if(err) {
        log = IFBS_GetLastLogError();
        if(log == "") {
            iFBS_SetLastError(1, err, log);
        }
        return err;
    }

    return KS_ERR_OK;
}


/******************************************************************************/
KS_RESULT get_all_comcons(KscServerBase*     Server,
                          PltList<PltString> &ConList) {
/******************************************************************************/

    KsGetEPParams      params;
    KsGetEPResult      result;
    PltString          hStr;
    KS_RESULT          err;
    
    params.path = "/" ;
    params.path += FB_CONN_CONTAINER;
    params.type_mask = KS_OT_DOMAIN;
    params.name_mask = "*";
    params.scope_flags = KS_EPF_DEFAULT;

    bool ok = Server->getEP(0, params, result);
    if ( !ok ) {
        err = Server->getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
        return err;
    }
    if ( result.result != KS_ERR_OK ) {
        return result.result;
    }

    params.path += "/";

    while ( result.items.size() ) {
        KsEngPropsHandle hpp = result.items.removeFirst();
        hStr = params.path;
        hStr += (const char*)hpp->identifier;
        ConList.addLast( hStr );
    }

    return KS_ERR_OK;
}


/******************************************************************************/
KS_RESULT clean_all_comcons(KscServerBase*  Server, PltString* Logging) {
/******************************************************************************/

    PltList<PltString> ConList;

    KS_RESULT err = get_all_comcons(Server, ConList);
    if(err) {
        if(Logging && (*Logging) ) {
            *Logging += log_getErrMsg(err,"Can't read", FB_CONN_CONTAINER, "objects");
        }
        return err;
    }
    
    TurnComConOff(Server, ConList);

    return DelObjsFromList(Server, ConList, Logging);

}


/******************************************************************************/
KS_RESULT clean_all_inst(KscServerBase*  Server, PltString* Logging) {
/******************************************************************************/

    PltList<ObjProps>   Liste;
    PltList<PltString>  TuListe;
    PltList<PltString>  InstListe;
    PltString           hStr;
    KsGetEPParams       params;
    KsGetEPResult       result;
    KS_RESULT           err = KS_ERR_OK;
    KS_RESULT           lastErr = KS_ERR_OK;
    char                *help;
    char                *ph;
    
    params.path = "/" ;
    params.path += FB_INSTANZ_CONTAINER;
    params.type_mask = (KS_OT_DOMAIN | KS_OT_HISTORY);
    params.name_mask = "*";
    params.scope_flags = KS_EPF_DEFAULT;

    bool ok = Server->getEP(0, params, result);
    if ( !ok ) {
        err = Server->getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
    } else {
        if ( result.result != KS_ERR_OK ) {
            err = result.result;
        }
    }
    if(err) {
        if(Logging && (*Logging) ) {
            *Logging += log_getErrMsg(err,"Can't read",FB_INSTANZ_CONTAINER,"objects.");
        }
        return err;
    }

    while ( result.items.size() ) {
        KsEngPropsHandle hpp = result.items.removeFirst();
        hStr = (const char*)params.path;
        // Part?
        if( hpp->access_mode & KS_AC_PART) {
            hStr += ".";
        } else {
            hStr += "/";
        }
        hStr += (const char*)hpp->identifier;

        if( hpp->xdrTypeCode() == KS_OT_DOMAIN) {
            if( ((KsDomainEngProps &)(*hpp)).class_identifier == CONTAINER_CLASS_PATH) {
                TuListe.addLast(hStr);
            } else {
                InstListe.addLast(hStr);
            }
        } else {
            InstListe.addLast(hStr);
        }
    }

    while(TuListe.size() ) {

        hStr = TuListe.removeFirst();
        err = del_cont_recurs(Server, hStr, Logging);
        if(err) {
            return err;
        }
    }

    // Instanzen ausschalten
    TurnInstOff(Server, InstListe);

    // Anzahl der Versuche
    int count = 10;
    while(count > 0) {
        PltList<PltString> tmpLst;
        while(InstListe.size()) {
            hStr = InstListe.removeFirst();

            err = del_inst_childs(Server, hStr, Logging);
            if(err) {
                // Naechste Runde abwarten?
                lastErr = err;
                tmpLst.addLast(hStr);
            } else {
                delAllXlinks(Server, hStr);
                err = IFBS_DELETE_OBJ(Server,hStr);
                if(err) {
                    // Part?
                    int part = 0;
                    help = (char*)((const char*)hStr);
                    ph = help;
                    while(ph && (*ph)) ph++;
                    while(ph != help) {
                        ph--;
                        if((*ph) == '.') {
                            part = 1;
                            break;
                        }
                        if((*ph) == '/') {
                            // Kein Part
                            break;
                        }
                    }
                    if(!part) {
                        lastErr = err;
                        tmpLst.addLast(hStr);
                    }
                } else {
                    if(Logging && (*Logging) ) {
                        *Logging += log_getOkMsg("Instance",(const char*)hStr,"deleted.");
                    }
                }
            }
        }
        
        if(tmpLst.size() == 0) {
            break;
        }
        while(tmpLst.size()) {
            InstListe.addLast(tmpLst.removeFirst());
        }
        
        count--;
    }
    
    // Alle Instanzen geloescht?
    while(InstListe.size()) {
        hStr = InstListe.removeFirst();
        err = del_inst_childs(Server, hStr, Logging);
        if(err == KS_ERR_OK) {
            delAllXlinks(Server, hStr);
            err = IFBS_DELETE_OBJ(Server,hStr);
        }
        if(err) {
            if(Logging && (*Logging)) {
                *Logging += log_getErrMsg(err,"Instance",(const char*)hStr,"not deleted.");
            }
            
            return err;
        }
    }
    
    return KS_ERR_OK;
}

/******************************************************************************/
KS_RESULT clean_all_libs(KscServerBase*  Server, PltString* Logging) {
/******************************************************************************/

    KsGetEPParams      params;
    KsGetEPResult      result;
    PltString          hStr;
    KS_RESULT          err = KS_ERR_OK;
    KS_RESULT          LastErr = KS_ERR_OK;
    PltList<PltString> NotDeletedLibs;
    int                i, anz;
    int                anzLoops = 5;
    
    params.path = "/" ;
    params.path += FB_LIBRARIES_CONTAINER;
    params.type_mask = KS_OT_DOMAIN;
    params.name_mask = "*";
    params.scope_flags = KS_EPF_DEFAULT;

    bool ok = Server->getEP(0, params, result);
    if ( !ok ) {
        err = Server->getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
    } else {
        if ( result.result != KS_ERR_OK ) {
            err = result.result;
        }
    }
    if(err) {
        if(Logging && (*Logging) ) {
            *Logging += log_getErrMsg(err,"Can't read", FB_LIBRARIES_CONTAINER,"objects.");
        }
        return err;
    }

    params.path += "/";

    while ( result.items.size() ) {
        KsEngPropsHandle hpp = result.items.removeFirst();
        hStr = (const char*)params.path;
        hStr += (const char*)hpp->identifier;
    
        err = IFBS_DELETE_OBJ(Server,hStr);
        if(err) {
            LastErr = err;
        }
        if(Logging && (*Logging) ) {
            if(err) {
                *Logging += log_getErrMsg(err,"Can't delete library",
                            (const char*)hStr);
                
                NotDeletedLibs.addLast(hStr);
                
            } else {
                *Logging += log_getOkMsg("Library",(const char*)hStr," deleted.");
            }
        }
    }
    
    // Gibt es nicht geloeschte Bibliotheken?
    while(NotDeletedLibs.size() && anzLoops > 0) {
        anz = NotDeletedLibs.size();
        for(i=0; i<anz; i++) {
            hStr = NotDeletedLibs.removeFirst();
            
            err = IFBS_DELETE_OBJ(Server,hStr);
            if(err != KS_ERR_OK) {
                NotDeletedLibs.addLast(hStr);
            } else {
                *Logging += log_getOkMsg("Library",(const char*)hStr,"deleted.");                   
            }
        }
        anzLoops--;
    }
    
    // Alles geloescht?
    if(NotDeletedLibs.size() > 0) {
        return LastErr;
    }
    
    return KS_ERR_OK;
}


/******************************************************************************/
KS_RESULT clean_all_tasks(KscServerBase*  Server, PltString* Logging) {
/******************************************************************************/

    KsGetEPParams      params;
    KsGetEPResult      result;
    PltString          hStr;
    PltList<PltString> TaskList;
    KS_RESULT          err = KS_ERR_OK;
    
    params.path = "/" ;
    params.path += FB_TASK_CONTAINER;
    params.type_mask = KS_OT_DOMAIN;
    params.name_mask = "*";
    params.scope_flags = KS_EPF_DEFAULT;

    bool ok = Server->getEP(0, params, result);
    if ( !ok ) {
        err = Server->getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
    } else {
        if ( result.result != KS_ERR_OK ) {
            err = result.result;
        }
    }
    if(err) {
        if(Logging && (*Logging) ) {
            *Logging += log_getErrMsg(err,"Can't read ", FB_TASK_CONTAINER,"objects.");
        }
        return err;
    }

    params.path += "/";

    while ( result.items.size() ) {
        KsEngPropsHandle hpp = result.items.removeFirst();
        if(hpp->identifier != FB_URTASK ) {
            hStr = (const char*)params.path;
            hStr += (const char*)hpp->identifier;
            TaskList.addLast( hStr );
        }
    }


    TurnInstOff(Server, TaskList);
    
    return DelObjsFromList(Server, TaskList, Logging);
}
