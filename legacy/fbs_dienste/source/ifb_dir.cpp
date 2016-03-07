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
*   ==============================================================           *
*                                                                            *
*   Datei                                                                    *
*   -----                                                                    *
*   fb_dir.cpp                                                               *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   1999-04-07  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*   Gibt Inhalt einer Domain zurueck                                         *
*                                                                            *
*****************************************************************************/


#include "ifbslibdef.h"

/******************************************************************************/
KS_RESULT IFBS_DIR(KscServerBase*         Server,
                                         FbDirParams        &Pars,
                                         PltList<ObjProps>  &Res)
/******************************************************************************/
 {

    KsGetEPParams    params;           /* Parameter für getPP-Service       */
    KsGetEPResult    result;
    //char             help[256];
    KS_RESULT        err = 0;
    PltString        log("");

    if(!Server) {
        err = KS_ERR_SERVERUNKNOWN;
        iFBS_SetLastError(1, err, log);
        return err;
    }

    if( !Pars.path.len() ) {
        log = "\"%s\"  \"\"";
        err = KS_ERR_BADNAME;
        iFBS_SetLastError(1, err, log);
        return err;
    }

    iFBS_SetLastError(1, err, log);

    
    //strcpy(help, (const char*)Pars.path);

    params.path = Pars.path;
    if(Pars.name_mask != "")
        params.name_mask = Pars.name_mask;
    else
        params.name_mask = "*";

//cout << "\n Name-Mask ist >" << params.name_mask << "<\n";

    params.type_mask = 0;

    if( Pars.type_mask == FB_OBJ_ANY ) {
        params.type_mask = KS_OT_ANY;
        
    } else {
        if( Pars.type_mask & FB_OBJ_HISTORY )
           params.type_mask |= KS_OT_HISTORY;
        if( Pars.type_mask & FB_OBJ_VARIABLE )
           params.type_mask |= KS_OT_VARIABLE;
        if( Pars.type_mask & FB_OBJ_LINK )
           params.type_mask |= KS_OT_LINK;
        if( (Pars.type_mask & FB_OBJ_INSTANCE)   ||
            (Pars.type_mask & FB_OBJ_TASK)       ||
            (Pars.type_mask & FB_OBJ_LIBRARY)    ||
            (Pars.type_mask & FB_OBJ_CLASS)      ||
            (Pars.type_mask & FB_OBJ_CONNECTION) ||
            (Pars.type_mask & FB_OBJ_CONTAINER) )
           params.type_mask |= KS_OT_DOMAIN;
    }

    if(params.type_mask == 0)
        params.type_mask = KS_OT_ANY;

    params.scope_flags = Pars.scope_flags;

    bool ok = Server->getEP(0, params, result);
    if ( !ok ) {
        log = "\"%s\"  \"";
        log += Pars.path;
        log += "\"";
        err = Server->getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
        iFBS_SetLastError(1, err, log);

        return KS_ERR_GENERIC;
    }
    if ( result.result ) {
        err = result.result;
        log = "\"%s\"  \"";
        log += Pars.path;
        log += "\"";
        iFBS_SetLastError(1, err, log);
        return err;

    }

    /* Sonst ist getPP-Dienst ohne Fehler ausgefuehrt worden und die Rueckgabe- */
    /* Parameter koennen generiert werden :                                     */

    PltList<KsEngPropsHandle>  ListVars;
    size_t                     i = 0;             /* Laufvariable */
    size_t                     Anz ;              /* Merker : Anzahl gefundenen Objekten */

    Anz = result.items.size();
    if(!Anz) {
        // Keine Objekten gefungen
        return KS_ERR_OK;
    }
    PltArray<KscVariable*> VarArray(Anz);
    if(VarArray.size() != Anz) {
        err = OV_ERR_HEAPOUTOFMEMORY;
        log = "";
        iFBS_SetLastError(1, err, log);
        return err;
    }

    KsString            root = Server->getHostAndName();
    KsString            Var;

    KscPackage        *Vpkg = new KscPackage;
    if(!Vpkg) {
        err = OV_ERR_HEAPOUTOFMEMORY;
        iFBS_SetLastError(1, err, log);
        return err;
    }
    while ( result.items.size() ) {

        ObjProps item;
        KsEngPropsHandle hpp = result.items.removeFirst();
        if(!hpp) {
            delete Vpkg;
            err = KS_ERR_GENERIC;
            iFBS_SetLastError(1, err, log);
            return err;
        }
        if(hpp->xdrTypeCode() == KS_OT_HISTORY) {
            item.obj_type = FB_OBJ_HISTORY;
            item.identifier    = (const char*)hpp->identifier;
            item.creation_time = ((KsHistoryEngProps &)(*hpp)).creation_time;
            item.comment       = (const char*)((KsHistoryEngProps &)(*hpp)).comment;
            item.access        = ((KsHistoryEngProps &)(*hpp)).access_mode;
            item.flags         = ((KsHistoryEngProps &)(*hpp)).semantic_flags;

            item.class_ident   = (const char*)((KsHistoryEngProps &)(*hpp)).type_identifier;

            item.value         = "";
            item.tech_unit     = "";
            item.var_type      = 0;
            item.state         = 0;
            item.len           = 1;


            Res.addLast(item);
            
            continue;
        }
        if(hpp->xdrTypeCode() == KS_OT_DOMAIN) {
            if( (Pars.type_mask & FB_OBJ_INSTANCE ) &&
                ( ((KsDomainEngProps &)(*hpp)).class_identifier != CONTAINER_CLASS_PATH )   &&
                ( ((KsDomainEngProps &)(*hpp)).class_identifier != TASK_CLASS_PATH )        &&
                ( ((KsDomainEngProps &)(*hpp)).class_identifier != LIBRARY_FACTORY_PATH )   &&
                ( ((KsDomainEngProps &)(*hpp)).class_identifier != CLASS_CLASS_PATH )       &&
                ( ((KsDomainEngProps &)(*hpp)).class_identifier != CONNECTION_CLASS_PATH ) )    {

                    item.obj_type = FB_OBJ_INSTANCE;
            } else {
                if( ( ((KsDomainEngProps &)(*hpp)).class_identifier == CONTAINER_CLASS_PATH )  && (Pars.type_mask & FB_OBJ_CONTAINER) ) {
                        item.obj_type = FB_OBJ_CONTAINER;
                } else {
                    if( ( ((KsDomainEngProps &)(*hpp)).class_identifier == TASK_CLASS_PATH ) && (Pars.type_mask & FB_OBJ_TASK) ) {
                                 item.obj_type = FB_OBJ_TASK;
                    } else {
                        if( ( ((KsDomainEngProps &)(*hpp)).class_identifier == LIBRARY_FACTORY_PATH ) && (Pars.type_mask & FB_OBJ_LIBRARY) ) {
                                     item.obj_type = FB_OBJ_LIBRARY;
                        } else {
                            if( ( ((KsDomainEngProps &)(*hpp)).class_identifier == CLASS_CLASS_PATH ) && (Pars.type_mask & FB_OBJ_CLASS) ) {
                                         item.obj_type = FB_OBJ_CLASS;
                            } else {
                                if( ( ((KsDomainEngProps &)(*hpp)).class_identifier == CONNECTION_CLASS_PATH ) && (Pars.type_mask & FB_OBJ_CONNECTION) ) {
                                             item.obj_type = FB_OBJ_CONNECTION;
                                } else {
                                    continue;
                                }
                            }
                        }
                    }
                }
            }

            item.identifier    = (const char*)hpp->identifier;
            item.creation_time = ((KsDomainEngProps &)(*hpp)).creation_time;
            item.comment       = (const char*)((KsDomainEngProps &)(*hpp)).comment;
            item.access        = ((KsDomainEngProps &)(*hpp)).access_mode;
            item.flags         = ((KsDomainEngProps &)(*hpp)).semantic_flags;

            item.class_ident   = (const char*)((KsDomainEngProps &)(*hpp)).class_identifier;

            item.value         = "";
            item.tech_unit     = "";
            item.var_type      = 0;
            item.state         = 0;
            item.len           = 1;


            Res.addLast(item);
            
        /* if DOMAIN */
        } else {
            if( (hpp->xdrTypeCode() == KS_OT_VARIABLE) || (hpp->xdrTypeCode() == KS_OT_LINK) ) {
                ListVars.addLast(hpp);
                Var = params.path;
                if( Pars.path.substr(0,7) == "/vendor" )
                    Var += "/";
                else
                    Var += ".";
                    Var += hpp->identifier;  /* /. */
                    VarArray[i] = new KscVariable(root+Var);
                if(!VarArray[i]) {
                    delete Vpkg;
                    err = OV_ERR_HEAPOUTOFMEMORY;
                    iFBS_SetLastError(1, err, log);
                    return err;
                }
                if(!Vpkg->add(KscVariableHandle(VarArray[i], PltOsNew)) ){
                    delete Vpkg;
                             err = 1;
                             log = "";
                             iFBS_SetLastError(1, err, log);
                             return err;
                }
                i++; // Naechste Variable
            }

        } /* else (also keine DOMAIN) */

    } /* while */

    if(!ListVars.isEmpty() ) {
        if(!Vpkg->getUpdate() ) {
            err = Vpkg->getLastResult();
            if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
            delete Vpkg;
            log = "";
                     iFBS_SetLastError(1, err, log);
                     return err;
        }
    }

    i = 0;
    while(!ListVars.isEmpty() ) {

        ObjProps itemv;
            KsEngPropsHandle pv = ListVars.removeFirst();
        if(!pv) {
            err = KS_ERR_GENERIC;
            iFBS_SetLastError(1, err, log);
            return err;
        }
            
        const KsVarCurrProps *cp = (VarArray[i])->getCurrProps();
        if(!cp) {
            err = KS_ERR_GENERIC;
            log = "";
            iFBS_SetLastError(1, err, log);
            delete Vpkg;
            return err;
        }
        GetPortProps( pv, cp, itemv);
                Res.addLast(itemv);
                i++; // Naechste Variable
    }

    /* Speicher freigeben */

    delete Vpkg;

    return KS_ERR_OK;
}


/******************************************************************************/
void ifb_SortList(PltList<PltString> &Liste)
/******************************************************************************/
{
    PltList<PltString> hList;
    PltString          HelpString;
    PltString          hs;
    int                found;
    
    while(Liste.size() ) {
        hs = Liste.removeFirst();
        found = 1;
        
        PltListIterator<PltString> *it = (PltListIterator<PltString> *)Liste.newIterator() ;

//        for(;*it;++(*it) ) {
//                if(hs > (**it) ) {
//                    HelpString = (**it);
//              (PltString)(**it) = hs;
//                hs = HelpString;
//                }
//        }
        
        for(;*it;++(*it) ) {
                if(hs > (**it) ) {
                    found = 0;
                    break;
                }
        }

        delete it;
        
        if(found) {
            hList.addLast(hs);
        } else {
            Liste.addLast(hs);
        }
    }

    while(hList.size()) {
        Liste.addLast(hList.removeFirst());
    }
    
    return;
}
