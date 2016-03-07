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
*   update_project.cpp                                                       *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   1999-04-07  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*                                                                            *
*                                                                            *
*****************************************************************************/


/*
*        Includes
*        --------
*/
#include "ifbslibdef.h"


/*
*   Hauptprogramm
*        -------------
*/
/*****************************************************************************/
 KS_RESULT  update_eval(KscServerBase* Server,
                        Dienst_param*  Params,
                        PltString&     out)
/*****************************************************************************/
{
    /*
    *        Variablen
    */
    char      help[256];
    PltString Str;
    KS_RESULT error;

///////////////////////////////////////////////////////////////////////////////
//  Dienst UnLink                                                            //
///////////////////////////////////////////////////////////////////////////////

    error = unlink_old_link(Server, Params, out);
    if(error) {
            return error;
    }

///////////////////////////////////////////////////////////////////////////////
//  Dienst DeleteObject                                                      //
///////////////////////////////////////////////////////////////////////////////

    error = delete_old_instance(Server, Params, out);
    if (error) {
            return error;
    }


///////////////////////////////////////////////////////////////////////////////
//  Alte Bibliotheken loeschen                                               //
///////////////////////////////////////////////////////////////////////////////

    DelInstItems* plib;
    plib = Params->OldLibs;
    while(plib) {
        sprintf(help, "/%s/%s", FB_LIBRARIES_CONTAINER, plib->Inst_name);
        Str = help;
        error = IFBS_DELETE_OBJ(Server, Str);
        if(error) {
            out += log_getErrMsg(error, "Library",plib->Inst_name,"couldn't be deleted.");
        } else {
            out += log_getOkMsg("Library",plib->Inst_name,"deleted.");
        }
        plib = plib->next;
    }

///////////////////////////////////////////////////////////////////////////////
//  Neue Instanzen anlegen                                                   //
///////////////////////////////////////////////////////////////////////////////

    error = import_eval(Server, Params, out, FALSE);
    if(error) {
        return error;
    }

///////////////////////////////////////////////////////////////////////////////
//  Dienst SetVar                                                            //
///////////////////////////////////////////////////////////////////////////////

    error = set_new_value(Server, Params, out);

    return error;
}



/*****************************************************************************/
int set_new_value(KscServerBase* Server, Dienst_param* Params,
                  PltString& out)
/*****************************************************************************/
{
    KsSetVarParams          setpar(1);
    KsSetVarResult          erg(1);
    KscAvSimpleModule       *AV = GetClientAV();

    SetInstVarItems         *pset;
    PltString               log;
    int                     error;
    int                     fehler = 0;
    size_t                  siz;        // Merker : Anzahl zu setzenden Variablen
    size_t                  i;          // Lauf-Variable
    KsString                VarName;    // Merker : NAme der zu setzender Variable
    //char                    help[256];
    int                     isVendor = 0;

    if( !Params->Set_Inst_Var) {
        return 0;
    }
    if( !Params->Set_Inst_Var->Inst_var) {
        return 0;
    }

    pset = Params->Set_Inst_Var;

    // Sonderfall: "Vendor"
    VarName = pset->Inst_name;
    if(VarName == "/vendor") {
        isVendor = 1;
    }
    
    while(pset) {
        error = GetCreateObjectVar( pset->Inst_var, setpar.items);
        if(!error) {
            siz = setpar.items.size();
            for(i = 0; i < siz; i++) {
                VarName = setpar.items[i].path_and_name;
                setpar.items[i].path_and_name = pset->Inst_name;
                if(isVendor) {
                    // Trenner ist '/'. Der Variable wurde '.' hinzugefuegt
                    setpar.items[i].path_and_name += "/";
                    setpar.items[i].path_and_name += VarName.substr(1);
                } else {
                    // Part-Variable. Trenner '.' bereits in Name
                    setpar.items[i].path_and_name += VarName;
                }
            }
            bool ok = Server->requestByOpcode ( KS_SETVAR, AV, setpar, erg);
            if(!ok) {
                error = Server->getLastResult();
                if(error == KS_ERR_OK) error = KS_ERR_GENERIC;
            } else if(erg.result) {
                error = erg.result;
            } else {
                siz = erg.results.size();
                for(i = 0; i < siz; i++) {
                    if( erg.results[i].result) {
                        error = erg.results[i].result;
                        break;
                    }
                }
            }
        }
        if(error) {
            // Letzte Fehler-Kode merken
            fehler = error;
            out += log_getErrMsg(error, "Instance",pset->Inst_name,"couldn't be updated.");
        
            log  = "\"%s\"  \"";
            log += pset->Inst_name;
            log += "\"";
            iFBS_SetLastError(1, error, log);

        } else {
            out += log_getOkMsg("Instance",pset->Inst_name,"updated");
        }

        pset = pset->next;
    } /* while pset */ 

    return fehler;

} /* set_new_value() */

/*****************************************************************************/
KS_RESULT delete_old_instance(KscServerBase* Server, Dienst_param* Params,
                              PltString& out)
/*****************************************************************************/
{
    DelInstItems            *del_inst;      // Hilfszeiger
    KS_RESULT               err;
    PltList<PltString>      InstList;
    PltList<PltString>      VerbList;
    PltString               Str;
    char                    *ph;
    int                     isPart;
    
    if(!Params->DelInst) {
        return 0;
    }
    
    del_inst = Params->DelInst;
    
    // Alle zu loeschende Verbindungen und Instanzen merken
    while(del_inst) {
        if(!strncmp(del_inst->Inst_name, "/Cons" , 4) ) {
            // Es ist Verbindung
            VerbList.addLast(del_inst->Inst_name);
        } else {
            // Ist das ein Part?
            isPart = 0;
            ph = del_inst->Inst_name;
            while(ph && (*ph)) ph++;    // String-Ende suchen
            while( ph != del_inst->Inst_name ) {
                if(*ph == '/') {
                    break;
                }
                if(*ph == '.') {
                    isPart = 1;
                    break;
                }
                ph--;
            }
            if(isPart != 1) {
                // Es ist Task, Instanz oder Container
                // In umgekehrten Reihenfolge hinzufuegen
                // damit die unterste Instanz als erste
                // aufgeraeumt wird:
                InstList.addFirst(del_inst->Inst_name);
            }
        }
        del_inst = del_inst->next;
    }
    // Verbindungen ausschalten und loeschen
    TurnComConOff(Server, VerbList);
    err = DelObjsFromList(Server, VerbList, &out);
    if(err) {
       return err;
    }
    // Instanzen ausschalten und loeschen
    TurnInstOff(Server, InstList);
    err = DelObjsFromList(Server, InstList, &out);
    if(err) {
       return err;
    }
    
    return KS_ERR_OK;
    
} /* delete_old_instance() */


/*****************************************************************************/
KS_RESULT unlink_old_link(KscServerBase* Server, Dienst_param* Params,
                          PltString& out)
/*****************************************************************************/
{
    KS_RESULT               error;
    LinksItems*             punlink;
    Child*                  pch;
    //char                    help[256];
    PltString               Str;
    FbLinkParams            Pars;
    PltString               InpConLinkName("inputcon");
    PltString               OutConLinkName("outputcon");
        
    if(!Params->UnLinks) {
        return 0;
    }
    
    punlink = Params->UnLinks;
    
    while(punlink) {
        // Connection-Links muessen nicht geloest werden
        if( (InpConLinkName != punlink->child_role) &&
            (OutConLinkName != punlink->child_role) ) {
            
            Pars.parent_path = punlink->parent_path;
            Pars.parent_path += ".";
            Pars.parent_path += punlink->child_role;
            
            pch = punlink->children;
            
            while(pch) {
                Pars.elem_path = pch->child_path;
                error = ifb_deleteLink(Server,Pars);
                if(error) {
                    out += log_getErrMsg(
                        error,
                        "Parent",   punlink->parent_path,
                        "und Child",pch->child_path,
                        "couldn't be unlinked.");
               
                    Str = "\"%s %s\"  \"";
                    Str += punlink->parent_path;
                    Str += "\" \"";
                    Str += pch->child_path;
                    Str += "\"";
               
                    iFBS_SetLastError(1, error, Str);
                } else {
                    out += log_getOkMsg(
                        "Parent",punlink->parent_path,
                        "and Child",pch->child_path,"unlinked");
                }
                pch = pch->next;
            }
            punlink = punlink->next;
        } /* Keine Connection-Links */
    } /* while punlink */

    return KS_ERR_OK;

}
