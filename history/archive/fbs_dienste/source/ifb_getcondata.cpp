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
*   ifb_getcondata.cpp                                                       *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   1999-04-07  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*   Gibt Daten der Verbindungen zurueck                                      *
*                                                                            *
*****************************************************************************/



#include "ifbslibdef.h"


/******************************************************************************/
KS_RESULT get_comcon_data(KscServerBase*     Server,
                          PltString          &Vname,
                          ConData            &Data)
/******************************************************************************/
{
    if(!Server) {
        return KS_ERR_SERVERUNKNOWN;
    }
    
    PltList<InstVarProps> hpp;
    Data.identifier = Vname;

    KS_RESULT err;
    KscVariable* ar[6];

    KsString            root = Server->getHostAndName();
    KsString            Var;
    size_t          i;
    PltString       log;
    
    KscPackage        *pkg = new KscPackage;
    if( !pkg) {
        err = OV_ERR_HEAPOUTOFMEMORY;
        log = "";
        iFBS_SetLastError(1, err, log);
        return err;
    }
    
    
    for(i = 0; i < 6; i++) {
        Var = Vname;
        
        switch(i) {
            case 0:
                    Var += ".on";
                    break;
            case 1:
                    Var += ".sourcetrig";
                    break;
            case 2:
                    Var += ".sourcefb";
                    break;
            case 3:
                    Var += ".sourceport";
                    break;
            case 4:
                    Var += ".targetfb";
                    break;
            case 5:
                    Var += ".targetport";
                    break;
        }
        ar[i] = new KscVariable(root+Var);
        if(!ar[i]) {
            err = OV_ERR_HEAPOUTOFMEMORY;
            log = "";
            iFBS_SetLastError(1, err, log);
            delete pkg;
            return err;
            }
            if(!pkg->add(KscVariableHandle(ar[i], PltOsNew)) ){
            //delete ar[i];       // FIXME : hoffentlich ist die Variable doch an
                                      //         Handle gebunden. Dann darf die auch nicht
                                      //         geloescht werden. Sonst wird die Anwendung
                                      //         abstuerzen. Ist die nicht gebunden - haben
                                      //         wir Speicher-Leck.
            err = KS_ERR_GENERIC;
            log = "";
            iFBS_SetLastError(1, err, log);
            delete pkg;
            return err;
        }
         
    } /* for alle Variablen */

    if(!pkg->getUpdate() ) {
        err = pkg->getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
        log = "";
        iFBS_SetLastError(1, err, log);
        delete pkg;
        return err;
    }

    for(i = 0; i < 6; i++) {
        
        const KsVarCurrProps *cp = (ar[i])->getCurrProps();
        if( (!cp) || (!cp->value) ) {
            err = KS_ERR_GENERIC;
            log = "";
            iFBS_SetLastError(1, err, log);
            delete pkg;
            return err;
        }
                    
        switch(i) {
            case 0:
                    Data.on = (bool) ((KsBoolValue &) (*cp->value));
                    break;
            case 1:
                    Data.source_trig = (bool) ((KsBoolValue &) (*cp->value));
                    break;
            case 2:
                    Data.source_fb = (const char*)((KsStringValue &) (*cp->value));
                    break;
            case 3:
                    Data.source_port = (const char*)((KsStringValue &) (*cp->value));
                    break;
            case 4:
                    Data.target_fb = (const char*)((KsStringValue &) (*cp->value));
                    break;
            case 5:
                    Data.target_port = (const char*)((KsStringValue &) (*cp->value));
                    break;
        }
    }

    return KS_ERR_OK;
}

/******************************************************************************/
KS_RESULT get_comcon_dataFromList(
    KscServerBase*     Server,
    PltList<PltString> &ConList,
    PltList<ConData>   &Res
) {
/******************************************************************************/
    size_t      AnzVerb = 0;                        // Merker : Anzahl der Verbindungen
    size_t      AnzVerbVars = 0;                    // Merker : Anzahl InfoVars zu Verbindungen
    KS_RESULT   err;
    
    AnzVerb = ConList.size();
    if(AnzVerb == 0) {
        // Keine Verbindungen an Instanz
        return KS_ERR_OK;
    }
    AnzVerbVars = AnzVerb * 6;
    
    // HilfsArray: Variablen aller Verbindungen
    PltArray<KscVariable*> VarArray(AnzVerbVars);
    if(VarArray.size() != AnzVerbVars) {
       return KS_ERR_GENERIC;
    }

    // Host/Server merken
    KsString root = Server->getHostAndName();
    
    PltList<PltString> nameList;
    PltString   hs;               // Pfad der Verbindung
    PltString   vpath;            // Pfad der Verbindung
    KscPackage *pkg = new KscPackage;
    size_t      i,pos = 0;
    while( !ConList.isEmpty() ) {
        
        hs = ConList.removeFirst();
        nameList.addLast(hs);
        
        for(i = 0; i < 6; i++) {
            // Variable anlegen
            vpath = hs;
            switch(i) {
                case 0:
                        vpath += ".sourcefb";
                        break;
                case 1:
                        vpath += ".sourceport";
                        break;
                case 2:
                        vpath += ".targetfb";
                        break;
                case 3:
                        vpath += ".targetport";
                        break;
                case 4:
                        vpath += ".sourcetrig";
                        break;
                default:
                        vpath += ".on";
                        break;
            }
            VarArray[pos] = new KscVariable(root+vpath);

            if(!pkg->add(KscVariableHandle(VarArray[pos], PltOsNew)) ) {
                delete pkg;
                return KS_ERR_GENERIC;
            }
            
            // Naechste Variable
            pos++;
        }
        
        // Trottelsicherung
        if(pos >= AnzVerbVars) {
            break;
        }
    } // Ueber alle Verbindungen

    // Daten holen
    if(!pkg->getUpdate() ) {
        err = pkg->getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
        // Speicher freigeben
        delete pkg;

        return err;
    }
    
    // Ueber alle Verbindungsobjekte
    pos = 0;
    ConData   Data;
    while(pos < AnzVerbVars) {
    
        Data.identifier = nameList.removeFirst();
        
        for(i = 0; i < 6; i++) {
            
            // Eigenschaften holen
            const KsVarCurrProps* cp = (VarArray[pos])->getCurrProps();
            if((!cp) || (!(cp->value)) ) {
                return KS_ERR_GENERIC;
            }

            switch(i) {
                case 0:
                        Data.source_fb = (const char*)((KsStringValue &) (*cp->value));
                        break;
                case 1:
                        Data.source_port = (const char*)((KsStringValue &) (*cp->value));
                        break;
                case 2:
                        Data.target_fb = (const char*)((KsStringValue &) (*cp->value));
                        break;
                case 3:
                        Data.target_port = (const char*)((KsStringValue &) (*cp->value));
                        break;
                case 4:
                        Data.source_trig = (bool) ((KsBoolValue &) (*cp->value));
                        break;
                default:
                        Data.on = (bool) ((KsBoolValue &) (*cp->value));
                        break;
            }

            // Naechste Variable
            pos++;
        }
        
        Res.addLast(Data);
        
    } // Ueber alle Variablen

    return KS_ERR_OK;
}

/******************************************************************************/
KS_RESULT IFBS_GETCONS(KscServerBase*     Server,
                       FbDirParams        &Pars,
                       PltList<ConData>   &Res)
/******************************************************************************/
 {
    if(!Server) {
        return KS_ERR_SERVERUNKNOWN;
    }

    KsString           root = Server->getHostAndName();
    KsString           Var;
    PltList<PltString> helplist;
    PltList<PltString> ConList;
    PltString          mask;
    //char               VerbName[128];
    char*              ph;
    KS_RESULT          err;
    KscPackage               *pkg = new KscPackage;
    PltString          hStr;
    
    if(!pkg) {
        return OV_ERR_HEAPOUTOFMEMORY;
    }
    
     if( (Pars.name_mask).len() )
        mask = Pars.name_mask;
     else
        mask = "*";

     Var = Pars.path;
     Var += ".inputcon";
     KscVariable *var = new KscVariable(root+Var);
     if(!pkg->add(KscVariableHandle(var, PltOsNew)) ){
     // delete var; FIXME : Ist die Variable an handle gebunden ?
     delete pkg;
             return 1;
     }

     Var = Pars.path;
     Var += ".outputcon";
     var = new KscVariable(root+Var);
     if(!pkg->add(KscVariableHandle(var, PltOsNew)) ){
    // delete var; FIXME : Ist die Variable an handle gebunden ?
    delete pkg;
             return 1;
     }

    if(!pkg->getUpdate() ) {
        err = pkg->getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
        delete pkg;
        return err;
    }

    PltIterator<KscVariableHandle> *it = pkg->newVariableIterator();

    for(; *it; ++(*it) ) {
        const KsVarCurrProps *cp = (**it)->getCurrProps();
        if(!cp) {
            delete it;
            delete pkg;
            return KS_ERR_GENERIC;
        }
        InsertAllComCon(*cp, helplist);
    }

    /* Speicher freigeben */
    delete it;
    delete pkg;

    // Verbindungen vorhanden?
    if(helplist.isEmpty() ) {
        return KS_ERR_OK;
    }

    if(mask != "*") {
        int len = strlen(FB_CONN_CONTAINER) +2;
        
        while(!helplist.isEmpty() ) {
            hStr = helplist.removeFirst();
            //strcpy(VerbName, (const char*)hStr);
            ph = (char*)((const char*)hStr);
            ph += len;
            KsMask MStr(mask);
            if(MStr.matches(ph) )
                ConList.addLast(hStr);
        }
    
        while(!ConList.isEmpty() ) {
            ConData   Verb;
            hStr = ConList.removeFirst();
            err = get_comcon_data(Server, hStr, Verb);
            if(err)
                return err;
            else
                Res.addLast(Verb);
        }
        
        return KS_ERR_OK;
    }
    
    // Alle Verbindungen
    return get_comcon_dataFromList(Server, helplist, Res);
}

/*****************************************************************************/
void InsertAllComCon(const KsVarCurrProps &cp, PltList<PltString> &ConList)
/*****************************************************************************/
{
    PltString hStr;
    int       size;
    int       i;
    int       EINFUEGEN;

    if(!cp.value) {
        return;
    }
    
    switch(cp.value->xdrTypeCode() ) {
            case KS_VT_STRING:
                hStr = (const char*)((KsStringValue &) *cp.value);
                        if( !hStr.len() ) {
                            return;
                        }
                        if(!ConList.isEmpty() ) {
                                 PltListIterator<PltString> *it = (PltListIterator<PltString> *)ConList.newIterator();
                                 for(; *it; ++(*it) ) {
                                        if( hStr == (**it) ) {
                                            delete it;
                                                return;
                                        }
                                 }
                                 delete it;
                        }
                        ConList.addLast(hStr);
                break;

            case KS_VT_STRING_VEC:
                size = ((KsStringVecValue &) *cp.value).size();
                        if( !size ) {
                            return;
                        }
                for ( i = 0; i < size; ++i ) {
                                EINFUEGEN = 1;
                        hStr = (const char*)(((KsStringVecValue &) *cp.value)[i]);
                                if(!ConList.isEmpty() ) {
                                    PltListIterator<PltString> *it = (PltListIterator<PltString> *)ConList.newIterator();
                                    for(; *it; ++(*it) ) {
                                            if( hStr == (**it) ) {
                                                    EINFUEGEN = 0;
                                                    break;
                                            }
                                    }
                                    delete it;
                                }
                                if(EINFUEGEN) {
                                        ConList.addLast(hStr);
                                }
                } /* for size */
                            
                break;
            default:
                        break;
        } /* switch */

 return;

} /* InsertAllComCon() */
