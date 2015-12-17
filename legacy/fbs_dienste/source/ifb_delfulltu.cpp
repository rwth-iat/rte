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
*   ifb_delfulltu.cpp                                                        *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   2000-01-03  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*   Loescht Pfad eingegebene Container mit Unterverzeichnissen               *
*                                                                            *
*****************************************************************************/

#include "ifbslibdef.h"

/******************************************************************************/
KS_RESULT IFBS_DELETE_FULLCONT(KscServerBase*  Server,
                               PltString       &tu) {
/******************************************************************************/

    char*     help;
    char*     ph;
    KS_RESULT err = 0;
    PltString TU("/");
    PltString log("");

    iFBS_SetLastError(1, err, log);

    if(!Server) {
        err = KS_ERR_SERVERUNKNOWN;
        iFBS_SetLastError(1, err, log);
        return KS_ERR_SERVERUNKNOWN;
    }
  
    if( !tu.len() ) {
        err = KS_ERR_BADPATH;
        iFBS_SetLastError(1, err, log);
        return KS_ERR_BADPATH;
    }

    
    TU += FB_INSTANZ_CONTAINER;
    
    help = (char*)malloc(tu.len() + 1);
    if(!help) {
        return KS_ERR_TARGETGENERIC;
    }
    strcpy(help, (const char*)tu);
    
    ph = strstr(help, FB_INSTANZ_CONTAINER);
    if( ph ) {
        ph += strlen(FB_INSTANZ_CONTAINER);
    } else {
        ph = help;
    }


    if((*ph) == '\0') {
        free(help);
        err = KS_ERR_BADPATH;
        log = "\"%s\"  \"";
        log += tu;
        log = "\"";
        iFBS_SetLastError(1, err, log);
        return KS_ERR_BADPATH;
    }

    if((*ph) == '/') ph++;
    if( ((*ph) == '\0') || ((*ph) == '*') ) {
        free(help);
        err = KS_ERR_BADPATH;
        log = "\"%s\"  \"";
        log += tu;
        log = "\"";
        iFBS_SetLastError(1, err, log);
        return KS_ERR_BADPATH;
    }

    TU += "/";
    TU += ph;

    free(help);
    
    help = (char*)malloc(TU.len() + 1);
    if(!help) {
        return KS_ERR_TARGETGENERIC;
    }
    strcpy(help, (const char*)TU);
    ph = help;
    while( (*ph) ) ph++; ph--;
    if((*ph) == '*') *ph = '\0'; ph--;
    if((*ph) == '/')  *ph = '\0';

    TU = help;
  
    free(help);
    
    err = del_cont_recurs(Server, TU);
    if(err)
        return err;


    return KS_ERR_OK;

}

/******************************************************************************/
KS_RESULT del_InstAndConsFormInstList(KscServerBase*      Server,
                                      PltList<PltString>  &InstListe,
                                      PltString           *Logging) {
/******************************************************************************/
    PltList<PltString>  VerbListe;
    KS_RESULT           err;
    
    err = GetVerbFromList(Server, InstListe, VerbListe);
    if(err) {
        return err;
    }

    TurnComConOff(Server, VerbListe);

    // Zuerst Verbindungen loeschen, dann Instanzen.
    // FIXME : Wenn Verbindungen vor den Instanzen eingefuegt werden,
    //         werden die auch in Richtigen Reihenfolge geloescht ?
    DelObjsFromList(Server, VerbListe, Logging);

    TurnInstOff(Server, InstListe);

    err = DelObjsFromList(Server, InstListe, Logging);

    return err;
}

/******************************************************************************/
KS_RESULT delAllXlinks(KscServerBase* Server, PltString &Str) {
/******************************************************************************/
    KsGetEPParams    params;
    KsGetEPResult    result;
    KS_RESULT        err;
    KS_RESULT        lastErr = KS_ERR_OK;
    KsString         hs;
    
    params.path = (const char*)Str ;
    params.type_mask = KS_OT_LINK;
    params.name_mask = "*";
    params.scope_flags = KS_EPF_DEFAULT;
   
    bool ok = Server->getEP(0, params, result);
    if ( !ok ) {
        err = Server->getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
        return err;
    }
    if ( result.result ) {
        return result.result;
    }

    if(!result.items.size()) {
        // Keine Objekten gefungen
        return KS_ERR_OK;
    }

    KsString root = Server->getHostAndName();    // Merke : //host/server
    FbLinkParams    Params;
    size_t          size;
    size_t          i;
    
    while ( result.items.size() ) {

        KsEngPropsHandle hpp = result.items.removeFirst();
        if(!hpp) {
            lastErr = KS_ERR_GENERIC;
            continue;
        }
        hs = hpp->identifier;
        // FB-Links ausser 'taskchild' nicht loeschen
        //    (hs == "taskchild") ||
        /*
        if( (hs == "inputcon")  ||
            (hs == "outputcon") ||
            (hs == "taskparent") ) { continue;}
        */
        hs = Str;
        hs += ".";
        hs += hpp->identifier;
        
        KscVariable  var(root + hs);
        if(!var.getUpdate() ) {
            // ?
            err = var.getLastResult();
            if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
            lastErr = err;
            continue;
        }
        const KsVarCurrProps* cp = var.getCurrProps();
        if(!cp || !cp->value) {
            // ?
            lastErr = KS_ERR_GENERIC;
            continue;
        }
        
        Params.parent_path = hs;

        switch(cp->value->xdrTypeCode() ) {
                 case KS_VT_STRING:
                                hs = (KsStringValue &) *cp->value;
                                if( hs.len() ) {
                                    Params.elem_path = (const char*)hs;
                                    err = ifb_deleteLink(Server,Params);
                                    if(err) {
                                        lastErr = err;
                                    }
                                }
                                break;
                 case KS_VT_STRING_VEC:
                                size = ((KsStringVecValue &) *cp->value).size();
                                for ( i = 0; i < size; i++ ) {
                                    Params.elem_path = (const char*)(((KsStringVecValue &) *cp->value)[i]);
                                    err = ifb_deleteLink(Server,Params);
                                    if(err) {
                                        lastErr = err;
                                    }
                                }
                                break;
                default:
                        break;
        }
    }
    
    return lastErr;
}

/******************************************************************************/
KS_RESULT del_inst_childs(KscServerBase*  Server,
                          PltString       &inst,
                          PltString*      Logging) {
/******************************************************************************/

    PltList<PltString>  TuListe;
    PltList<PltString>  InstListe;
    PltList<PltString>  VerbListe;
    PltList<PltString>  PartListe;
    PltString           log;
    KsGetEPParams       Pars;
    KsGetEPResult       result;
    KS_RESULT           err = KS_ERR_OK;
    KS_RESULT           LastErr = KS_ERR_OK;
    
    Pars.path = inst;
    Pars.type_mask = (KS_OT_DOMAIN | KS_OT_HISTORY);
    Pars.name_mask = "*";
    Pars.scope_flags = KS_EPF_DEFAULT;

    bool ok = Server->getEP(0, Pars, result);
    if ( !ok ) {
        err = Server->getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
    } else if ( result.result != KS_ERR_OK ) {
            err = result.result;
    }
    if(err) {
        log = "\"%s\"  \"";
        log += inst;
        log += "\"";
        iFBS_SetLastError(1, err, log);
        if(Logging && (*Logging)) {
            *Logging += log_getErrMsg(err, "Can't read",
                        (const char*)inst,"objects.");
        }
        return err;
    }
    
    while(result.items.size() ) {
        KsEngPropsHandle hpp = result.items.removeFirst();
        if( hpp->access_mode & KS_AC_PART) {
            // Alle Unterobjekte der Parts loeschen
            log = inst;
            log += ".";
            log += (const char*)hpp->identifier;
            err = del_inst_childs(Server, log, Logging);
            if(err != KS_ERR_OK) {
                return err;
            }
            PartListe.addLast(log);
        } else {
            log = inst;
            log += "/";
            log += (const char*)hpp->identifier;
            if( ((KsDomainEngProps &)(*hpp)).class_identifier == CONTAINER_CLASS_PATH) {
                TuListe.addLast(log);
            } else {
                InstListe.addLast(log);
            }
        }
    }
    
    // Child-Instanzen ausschalten
    TurnInstOff(Server,InstListe);

    // Verbindungen der Child-Instanzen holen und loeschen
    GetVerbFromList(Server,InstListe,VerbListe);
    TurnComConOff(Server, VerbListe);
    DelObjsFromList(Server, VerbListe, Logging);

    // Haben die Parts auch verbindungen?
    while(VerbListe.size()) {log = VerbListe.removeFirst();}
    GetVerbFromList(Server,PartListe,VerbListe);
    TurnComConOff(Server, VerbListe);
    DelObjsFromList(Server, VerbListe, Logging);
    
    // Alle Container rekursiv loeschen
    while(TuListe.size() ) {
        log = TuListe.removeFirst();
        err = del_cont_recurs(Server,log,Logging);
        if(err) {
            PltString Str = IFBS_GetLastLogError();
            if(Str == "" ) {
                Str = "\"%s\"  \"";
                Str += log;
                Str += "\"";
                iFBS_SetLastError(1, err, Str);
            }
            LastErr = err;
        }
    }
        

    // Alle Child-Instanzen und deren Unterobjekte loeschen
    while(InstListe.size()) {
        PltList<PltString> hLst;
        log = InstListe.removeFirst();
        delAllXlinks(Server, log);
        
        // Instanz-Unterobjekte loeschen
        del_inst_childs(Server, log, Logging);
        
        hLst.addLast(log);
        DelObjsFromList(Server, hLst, Logging);
    }
    
    return LastErr;                  
}

/******************************************************************************/
KS_RESULT del_cont_recurs(KscServerBase*  Server,
                          PltString       &tu,
                          PltString*      Logging) {
/******************************************************************************/

    PltList<PltString>  TuListe;
    PltList<PltString>  InstListe;
    PltList<PltString>  VerbListe;
    PltString           log;
    PltString           curInst;
    KsGetEPParams       Pars;
    KsGetEPResult       result;
    KS_RESULT           err = KS_ERR_OK;
    KS_RESULT           LastErr = KS_ERR_OK;
    char                *help;
    char                *ph;
    
    Pars.path = tu;
    Pars.type_mask = (KS_OT_DOMAIN | KS_OT_HISTORY);
    Pars.name_mask = "*";
    Pars.scope_flags = KS_EPF_DEFAULT;

    bool ok = Server->getEP(0, Pars, result);
    if ( !ok ) {
        err = Server->getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
    } else if ( result.result != KS_ERR_OK ) {
            err = result.result;
    }
    if(err) {
        log = "\"%s\"  \"";
        log += tu;
        log += "\"";
        iFBS_SetLastError(1, err, log);
        if(Logging && (*Logging)) {
            *Logging += log_getErrMsg(err, "Can't read",
                        (const char*)tu," objects.");
        }
        return err;
    }
    
    while(result.items.size() ) {
        KsEngPropsHandle hpp = result.items.removeFirst();
        curInst = tu;
        
        // Parts?
        if( hpp->access_mode & KS_AC_PART) {
            curInst += ".";
        } else {
            curInst += "/";
        }

        curInst += hpp->identifier;
        if( ((KsDomainEngProps &)(*hpp)).class_identifier == CONTAINER_CLASS_PATH) {
            TuListe.addLast(curInst);
        } else {
            InstListe.addLast(curInst);
        }
    }

    // Child-Instanzen ausschalten
    TurnInstOff(Server,InstListe);

    // Verbindungen der Child-Instanzen holen und loeschen
    GetVerbFromList(Server,InstListe,VerbListe);
    DelObjsFromList(Server, VerbListe, Logging);

    // Alle Container rekursiv loeschen
    while(TuListe.size() ) {
        curInst = TuListe.removeFirst();
        err = del_cont_recurs(Server,curInst,Logging);
        if(err) {
            log = IFBS_GetLastLogError();
            if(log == "" ) {
                log = "\"%s\"  \"";
                log += log;
                log += "\"";
                iFBS_SetLastError(1, err, log);
            }
            LastErr = err;
        }
    }

    // Alle Instanzen loeschen
    while(InstListe.size()) {
        curInst = InstListe.removeFirst();
        del_inst_childs(Server, curInst, Logging);
                
        err = IFBS_DELETE_OBJ(Server,curInst);
        if(err != KS_ERR_OK) {
            // Part erwischt?
            int part = 0;
            help = (char*)((const char*)curInst);
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
                log = IFBS_GetLastLogError();
                if(log == "" ) {
                    log = "\"%s\"  \"";
                    log += curInst;
                    log += "\"";
                    iFBS_SetLastError(1, err, log);
                }
                if(Logging && (*Logging)) {
                    *Logging += log_getErrMsg(err, "Instance",
                                (const char*)curInst,"not deleted.");
                }
            }
        } else {
            if(Logging && (*Logging)) {
                *Logging += log_getOkMsg("Instance",(const char*)curInst,"deleted.");
            }
        }
    }
    
    err = IFBS_DELETE_OBJ(Server,tu);
    if(err) {
        // Part?
        int part = 0;
        help = (char*)((const char*)tu);
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
            LastErr = err;
            log = IFBS_GetLastLogError();
            if(log == "" ) {
                log = "\"%s\"  \"";
                log += tu;
                log += "\"";
                iFBS_SetLastError(1, err, log);
            }
            if(Logging && (*Logging)) {
                *Logging += log_getErrMsg(err,
                            "Instance", (const char*)tu, "not deleted.");
            }
        }
    } else {
        if(Logging && (*Logging)) {
            *Logging += log_getOkMsg("Instance",(const char*)tu,"deleted.");
        }
    }
    return LastErr;
}


/******************************************************************************/
KS_RESULT DelObjsFromList(KscServerBase*      Server,
                          PltList<PltString>  &Liste,
                          PltString*          Logging) {
/******************************************************************************/

    if(Liste.isEmpty())
        return KS_ERR_OK;

    PltList<PltString> hListe;
    PltList<PltString> badLst;
    PltString          Str;
    size_t             i;
    
    KS_RESULT err = KS_ERR_OK;
    KS_RESULT erg;
    int       log = (Logging && (*Logging) ) ? 1 : 0;
    
    while(Liste.size() ) {
        Str = Liste.removeFirst();
        hListe.addLast(Str);

        delAllXlinks(Server,Str);

        erg = IFBS_DELETE_OBJ(Server,Str);
        if(erg) {
            // Merke: Instanz nicht geloescht
            badLst.addFirst(Str);
        } else {
            if(log) {
                *Logging += log_getOkMsg("Instance",(const char*)Str,"deleted.");
            }
        }
    }
    
    while(hListe.size() ) {
        Liste.addLast(hListe.removeFirst());
    }

    // Versuchen wir die Instanzen noch 5 mal zu loeschen
    for(i=0; i<5; i++) {
        while(badLst.size() ) {
            Str = badLst.removeFirst();
            erg = IFBS_DELETE_OBJ(Server,Str);
            if(erg) {
                if(i == 4) {
                    // Anzahl der Versuche abgelaufen
                    err = erg;
                    if(log) {
                        *Logging += log_getErrMsg(err,
                                        "Instance", (const char*)Str,
                                        "couldn't be deleted.");
                    }
                } else {
                    // Noch eine Runde
                    hListe.addLast(Str);
                }
            } else {
                if(log) {
                    *Logging += log_getOkMsg("Instance",(const char*)Str,"deleted.");
                }
            }
        }
        
        while(hListe.size() ) {
            badLst.addLast(hListe.removeFirst());
        }
    }

    return err;
}


/******************************************************************************/
KS_RESULT TurnComConOff(KscServerBase*      Server,
                        PltList<PltString>  &VerbListe) {
/******************************************************************************/

    if(VerbListe.isEmpty())
        return KS_ERR_OK;

    PltList<PltString>    hListe;
    PltString             Str;
    KS_RESULT             err = KS_ERR_OK;

    KS_RESULT erg;
    
    PltString Value("FALSE");
    
    while(VerbListe.size()) {
        Str = VerbListe.removeFirst();
        hListe.addLast(Str);
        Str += ".on";
        
        erg = ifb_SeparateValueAndSet(Server, Str, Value, KS_VT_BOOL);
        if(erg) {
            err = erg;
        }
    }
    
    while(hListe.size() ) {
        VerbListe.addLast(hListe.removeFirst());
    }

    return err;
}


/******************************************************************************/
KS_RESULT TurnInstOff(KscServerBase*      Server,
                      PltList<PltString>  &InstListe) {
/******************************************************************************/

    if(InstListe.isEmpty()) {
        return KS_ERR_OK;
    }

    PltString             Str;
    PltList<PltString>    hListe;

    KS_RESULT err = KS_ERR_OK;
    KS_RESULT erg;
    PltString Value("0");
    
    while(InstListe.size()) {
        Str = InstListe.removeFirst();
        hListe.addLast(Str);
        Str += ".actimode";
        
        erg = ifb_SeparateValueAndSet(Server, Str, Value, KS_VT_INT);
        if(erg) {
            err = erg;
        }
    }
    
    while(hListe.size() ) {
        InstListe.addLast(hListe.removeFirst());
    }
    
    return err;
}


/******************************************************************************/
KS_RESULT GetVerbFromList(KscServerBase*        Server,
                          PltList<PltString>    &InstListe,
                          PltList<PltString>    &VerbListe) {
/******************************************************************************/

    if(InstListe.isEmpty() ) {
        return KS_ERR_OK;
    }
    
    KsString           root = Server->getHostAndName();
    KsString           Var;
    PltString          Str;
    PltList<PltString> hListe;
    KS_RESULT          err;
    
    while(InstListe.size() ) {
        KscPackage         *pkg = new KscPackage;
        if(!pkg) {
            while(hListe.size() ) {
                InstListe.addFirst(hListe.removeLast());
            }
            err = OV_ERR_HEAPOUTOFMEMORY;
            Str = "";
            iFBS_SetLastError(1, err, Str);
        }
        
        Str = InstListe.removeFirst();
        hListe.addFirst(Str);
        
        Var = Str;
        Var += ".inputcon";
        KscVariable *var1 = new KscVariable(root+Var);
        if(!var1) {
            delete pkg;
            while(hListe.size() ) {
                InstListe.addFirst(hListe.removeLast());
            }
            err = OV_ERR_HEAPOUTOFMEMORY;
            Str = "";
            iFBS_SetLastError(1, err, Str);
        }
        if(!pkg->add(KscVariableHandle(var1, PltOsNew)) ){
            delete pkg;
            while(hListe.size() ) {
                InstListe.addFirst(hListe.removeLast());
            }
                 return 1;
        }

        Var = Str;
        Var += ".outputcon";
        KscVariable *var2 = new KscVariable(root+Var);
        if(!var2) {
            delete pkg;
            while(hListe.size() ) {
                InstListe.addFirst(hListe.removeLast());
            }
            err = OV_ERR_HEAPOUTOFMEMORY;
            Str = "";
            iFBS_SetLastError(1, err, Str);
        }
        if(!pkg->add(KscVariableHandle(var2, PltOsNew)) ){
            delete pkg;
            while(hListe.size() ) {
                InstListe.addFirst(hListe.removeLast());
            }
                 return 1;
        }

        if(!pkg->getUpdate() ) {
            // Dann haben wir moeglicherweise eine Instanz
            // nicht von FB erwischt :
            delete pkg;                 // Speicher freigeben
            continue;                   // Naechste Instanz
        }
      
        const KsVarCurrProps *cp1 = var1->getCurrProps();
        if(!cp1) {
            delete pkg;
            return KS_ERR_GENERIC;
        }
        InsertAllComCon(*cp1, VerbListe);
        
        const KsVarCurrProps *cp2 = var2->getCurrProps();
        if(!cp2) {
            delete pkg;
            return KS_ERR_GENERIC;
        }
        InsertAllComCon(*cp2, VerbListe);
        
        delete pkg;
    }


    while(hListe.size() ) {
        InstListe.addFirst(hListe.removeLast());
    }

    return KS_ERR_OK;
}
