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
*   select_save.cpp                                                          *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   2000-01-03  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*                                                                            *
*                                                                            *
*****************************************************************************/


#include "ifbslibdef.h"

/******************************************************************************/
KS_RESULT get_connectionList(
    KscServerBase*      Server,
    PltString           Path,
    PltList<PltString> &conList
) {
/******************************************************************************/
    KsGetEPParams      params;
    KsGetEPResult      result;
    KS_RESULT          err;
    int                found = 0;
    KsString           hs;              /* Hilfsstring              */
    KsString           root = Server->getHostAndName();
    size_t              i;
    
    params.path = Path;
    params.name_mask = "*";
    params.type_mask = KS_OT_LINK;
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

    while ( result.items.size() ) {
        KsEngPropsHandle hpp = result.items.removeFirst();
        if(!hpp) {
            return KS_ERR_GENERIC;
        }
        hs = hpp->identifier;
        if( (hs == "inputcon")  ||
            (hs == "outputcon") ) {
            found++;
        }
    }
    
    // Beide Links gefunden?
    if(found == 2) {
        
        // Hole Input-Connections
        hs = Path;
        hs += ".inputcon";
        KscVariable  varI(root + hs);
        if(!varI.getUpdate() ) {
            // ?
            return KS_ERR_GENERIC;
        }
        const KsVarCurrProps* cpi = varI.getCurrProps();
        if(!cpi || !cpi->value) {
            // ?
            return KS_ERR_GENERIC;
        }
 
        switch(cpi->value->xdrTypeCode() ) {
            case KS_VT_STRING_VEC: {
                    size_t size  = ((KsStringVecValue &) *(cpi->value)).size();
                    for ( i = 0; i < size; ++i ) {
                        PltString pth = (const char*)(((KsStringVecValue &) *(cpi->value))[i]);
                        
                        if(IsNotInList(pth, conList) ) {
                            conList.addLast(pth);
                        }
                    }
                }
                break;
            default :
                // Typ der Variable ist kein KS_VT_STRING_VEC !!!?
                break;
        }
        
        // Hole Output-Connections
        hs = Path;
        hs += ".outputcon";
        KscVariable  varO(root + hs);
        if(!varO.getUpdate() ) {
            // ?
            return KS_ERR_GENERIC;
        }
        const KsVarCurrProps* cp = varO.getCurrProps();
        if(!cp || !cp->value) {
            // ?
            return KS_ERR_GENERIC;
        }
 
        switch(cp->value->xdrTypeCode() ) {
            case KS_VT_STRING_VEC: {
                    size_t size  = ((KsStringVecValue &) *(cp->value)).size();
                    for ( i = 0; i < size; ++i ) {
                        PltString pth = (const char*)(((KsStringVecValue &) *(cp->value))[i]);
                        
                        if(IsNotInList(pth, conList) ) {
                            conList.addLast(pth);
                        }
                    }
                }
                break;
            default :
                // Typ der Variable ist kein KS_VT_STRING_VEC !!!?
                break;
        }
    }
    
    return KS_ERR_OK;
}

/******************************************************************************/
KS_RESULT saveConsFromList(KscServerBase* Server, PltList<PltString> &conList,
                          PltString& out, FILE *fout) {
/******************************************************************************/
    char     *help;
    char     *ph;
    PltString Str;
    KS_RESULT err;
    
    while( conList.size() ) {
        Str = conList.removeFirst();
        
        help = (char*)malloc(Str.len() + 1);
        if(!help) {
            return KS_ERR_TARGETGENERIC;
        }
        strcpy(help, (const char*)Str );
        ph = help;
        while( (*ph) ) ph++;
        while( (ph != help) && ((*ph) != '/') && ((*ph) != '.') ) ph--;
        if(ph != help) {
            *ph = '\0';
            ph++;
        } else {
            return KS_ERR_BADPATH;
        }
    
        KsGetEPParams      par;
        par.path = help;
        par.name_mask = ph;
        par.type_mask = KS_OT_DOMAIN;
        par.scope_flags = KS_EPF_DEFAULT;
            
        free(help);
            
        err = ifb_writeInstData(
                                    Server,
                                    par,
                                    out,
                                    FALSE,      // TRUE: Rekursive Sicherung
                                    TRUE,       // TRUE: FB-Links sichern
                                    FALSE,      // TRUE: Nur ParentLinks sichern
                                    fout
                                    );
        if(err) {
            out += log_getErrMsg(err);
            return err;
        }
    }
    
    return KS_ERR_OK;
}


/******************************************************************************/
KS_RESULT write_inst_from_class(KscServerBase* Server, FbDirParams& param,
                          PltString& out, FILE *fout) {
/******************************************************************************/

    KsString                 root = Server->getHostAndName();
    KsString                 Var;
    size_t             size,i;
    PltString          Str;
    PltList<PltString> conList;
    PltList<PltString> hList;
    char*              help;
    char*              ph;
    KsGetEPParams      params;
    KsGetEPResult      result;
    KS_RESULT          err;

    // Alle Typen holen
    params.path = param.path;
    params.name_mask = param.name_mask;
    params.type_mask = KS_OT_DOMAIN;
    params.scope_flags = KS_EPF_DEFAULT;

    bool ok = Server->getEP(0, params, result);
    if( !ok ) {
        err = Server->getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
        return err;
    }
    if( result.result != KS_ERR_OK ) {
        return result.result;
    }

    /* Ueber alle gefundene Typen */
    while ( result.items.size() ) {
        KsEngPropsHandle hpp = result.items.removeFirst();
        if(!hpp) {
            return KS_ERR_GENERIC;
        }

        // Ist das ein Bibliothek-Class? (Kann auch Association sein)
        Str = (const char*)(((KsDomainEngProps &)(*hpp)).class_identifier);
        if(Str != CLASS_CLASS_PATH) {
            continue;
        }
        
        
        // Instanzen des Types suchen
        Var = params.path;
        Var += "/";
        Var += hpp->identifier;
        Var += ".instance";


        KscVariable var(root+Var);

        if (!var.getUpdate() ) {
            out += log_getErrMsg(KS_ERR_OK,"Bad path", (const char*)param.path);
            return KS_ERR_BADPATH;
        }

        const KsVarCurrProps *cp = var.getCurrProps();
        if( (!cp) || (!cp->value) ) {
            return KS_ERR_GENERIC;
        }
    
        switch(cp->value->xdrTypeCode() ) {
            case KS_VT_STRING_VEC:
                size  = ((KsStringVecValue &) *(cp->value)).size();
                for ( i = 0; i < size; ++i ) {
                    Str = (const char*)(((KsStringVecValue &) *(cp->value))[i]);
                    hList.addLast(Str);
                }
                break;
            default :
                // Typ der Variable ist kein KS_VT_STRING_VEC !!!?
                break;
        }

        while(hList.size() ) {
        
            // Pfad und Instanzname trennen
            Str = hList.removeFirst();
            help = (char*)malloc(Str.len() + 1);
            if(!help) {
                return KS_ERR_TARGETGENERIC;
            }
            strcpy(help, (const char*)Str );
            ph = help;
            while( (*ph) ) ph++;
            while( (ph != help) && ((*ph) != '/') && ((*ph) != '.') ) ph--;
            if(ph != help) {
                
                // Parts allein werden nicht gesichert
                if( (*ph) == '.' ) {
                    free(help);
                    continue;
                }
                
                *ph = '\0';
                ph++;
            } else {
                return KS_ERR_BADPATH;
            }
    
            // Instanz sichern
            KsGetEPParams      par;
            
            par.path = help;
            par.name_mask = ph;
            par.type_mask = KS_OT_DOMAIN | KS_OT_HISTORY;
            par.scope_flags = KS_EPF_DEFAULT;
            
            free(help);
            
            err = ifb_writeInstData(
                                    Server,
                                    par,
                                    out,
                                    FALSE,      // Keine Rekursion!
                                    FALSE,      // FB-Links noch nicht sichern
                                    FALSE,      // Alle sonstige Links sichern
                                    fout
                                    );
            if(err) {
                out += log_getErrMsg(err);
                return err;
            }
    
            // Hat Instanz Connection-Links?
            err = get_connectionList(Server,Str,conList);
            if(err) {
                out += log_getErrMsg(err);
                return err;
            }
        }
    }

    // Verbindungen sichern
    err = saveConsFromList(Server, conList, out, fout);
    if(err) {
        out += log_getErrMsg(err);
        return err;
    }
    
    // Bibliothek
    Var = (const char*)param.path;
    ifb_writeLibItem(Var, out);
    
    return KS_ERR_OK;
}

/******************************************************************************/
void AddValueToList(PltString& Value,  PltList<PltString>& Liste) {
/******************************************************************************/

    PltString  hStr("");

    PltList<PltString> hList;

    if( !Value.len() )
        return;

    if( Liste.isEmpty() ) {
        Liste.addLast(Value);
        return;
    }   

    while( !Liste.isEmpty() ) {
        hStr = Liste.removeFirst();
        hList.addLast(hStr);
        if(hStr == Value) {
            while( !hList.isEmpty() ) {
                Liste.addFirst(hList.removeLast() );
            }
            return;
        }
    }

    while( !hList.isEmpty() ) {
        Liste.addFirst(hList.removeLast() );
    }
    Liste.addLast(Value);

    return;
}



/******************************************************************************/
void write_help_message_from_select_save(PltString &out) {
/******************************************************************************/

 out += "\n\n Selektierte Sicherung (Info-Text)\n";
 out += " =====================================\n\n";

 out += " SICHERUNG DER INSTANZEN IM CONTAINER :\n";
 out += " --------------------------------------\n\n";

 out += "    path = \"/TechUnits\";            //\n";
 out += "    path = \"/TechUnits/Modul1\";     // Falls Inhalt eines Subcontainers gesichert werden soll\n";
 out += "    path = \"/TechUnits/Modul1/*\";   // Falls Sicherung rekursiv in Subcontainers\n";
 out += "                                    // durchgefuert werden soll\n\n";

 out += "    type_mask = FB_OBJ_ANY;         // Falls alles gesichert werden soll\n";
 out += "    type_mask = FB_OBJ_INSTANCE;    // Falls nur Instanzen gesichert werden sollen\n";
 out += "    type_mask = FB_OBJ_CONTAINER;   // Falls nur Container gesichert werden sollen\n\n";

 out += "    name_mask = \"*\";                // Fuer alle Instanzen und Containers\n";
 out += "    name_mask = \"sum1\";             // Falls nur z.B. Instanz \"sum1\" gesichert werden soll\n\n";

 out += " SICHERUNG DER VORHANDENEN TASKS :\n";
 out += " ---------------------------------\n\n";

 out += "    path      = \"/Tasks\";           // UrTask wird nicht mitgesichert\n";
 out += "    type_mask = FB_OBJ_TASK;        // Oder FB_OBJ_ANY, wenn im Tasks-Container nicht nur die Tasks stehen\n";
 out += "    name_mask = \"*\";                // Fuer alle Tasks\n";
 out += "    name_mask = \"Task1\";            // Falls nur z.B. Task \"Task1\" gesichert werden soll\n\n";

 out += " SICHERUNG DER INSTANZEN VON ALLEN TYPEN IN EINER BIBLIOTHEK :\n";
 out += " -------------------------------------------------------------\n\n";

 out += "    path = \"/Libraries\";\n";
 out += "    name_mask = \"LibName\";\n";
 out += "    //type_mask  wird nicht ausgewertet\n";
 out += " oder\n";
 out += "    path = \"/Libraries/LibName\";\n";
 out += "    name_mask = \"*\";\n\n";

 out += " SICHERUNG DER INSTANZEN VON EINEM BESTIMMTEN TYP :\n";
 out += " --------------------------------------------------\n\n";

 out += "    path = \"/Libraries/LibName\";\n";
 out += "    name_mask = \"TypName\";\n";
 out += "    //type_mask wird nicht ausgewertet\n";
 out += "    //path = \"/Libraries/LibName/TypName\";  nicht zulaessig\n";

}




/******************************************************************************/
KS_RESULT saveInstances(KscServerBase* Server, FbDirParams& Pars,
                        PltList<PltString> &libList, PltList<PltString> &conList,
                        PltString& out, int& Recurs, FILE *fout) {
/******************************************************************************/

    KsGetEPParams       params;         // Hilfsparameter
    KsGetEPResult       result;         // Hilfsparameter
    PltString           libName;
    KS_RESULT            err;
    PltString            Path;
    FbDirParams          helpParams;

    params.path = Pars.path;
    params.name_mask = Pars.name_mask;
    params.type_mask = KS_OT_DOMAIN | KS_OT_HISTORY;
    params.scope_flags = KS_EPF_DEFAULT;

    bool ok = Server->getEP(0, params, result);
    if( !ok ) {
        err = Server->getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
        return err;
    }
    if( result.result != KS_ERR_OK ) {
        return result.result;
    }

    /* Alle Instanzen sichern */
    while ( result.items.size() ) {
        KsEngPropsHandle hpp = result.items.removeFirst();
        if(!hpp) {
            return KS_ERR_GENERIC;
        }

        // Bibliothek-Name holen
        if( hpp->xdrTypeCode() == KS_OT_HISTORY) {
            libName = (const char*)(((KsHistoryEngProps &)(*hpp)).type_identifier);
        } else {
            // Instanz oder Container
            libName = (const char*)(((KsDomainEngProps &)(*hpp)).class_identifier);
        }
        
        if( (libName != CONTAINER_CLASS_PATH) && (libName != TASK_CLASS_PATH) ) {
             AddValueToList(libName, libList);
        }
        
        // Merke Name der Instanz
        params.name_mask = (const char*)hpp->identifier;
        
        // Objekt sichern?
        if(hpp->access_mode & KS_AC_PART) {
            // Bereits mit Parent-Objekt gesichert
        } else {
            err = ifb_writeInstData(
                                    Server,
                                    params,
                                    out,
                                    FALSE,      // Keine Rekursion!
                                    FALSE,      // FB-Links nicht sichern
                                    FALSE,      // Alle sonstige Links sichern
                                    fout
                                    );
            if(err) {
                out += log_getErrMsg(err);
                return err;
            }
        }
        
        // Hat Instanz Connection-Links?
        Path = (const char*)params.path;
        if( hpp->access_mode & KS_AC_PART) {
            Path += ".";
        } else {
            Path += "/";
        }
        Path += (const char*)hpp->identifier;

        err = get_connectionList(Server,Path,conList);
        if(err) {
            out += log_getErrMsg(err);
            return err;
        }

        // Schreiben in Datei ?
        if(fout) {
            if(out != "") {
                fputs((const char*)out, fout);
                // String-Buffer leeren
                out = "";
            }
        }

        // Rekursive Sicherung oder Part?        
        if( ( hpp->access_mode & KS_AC_PART) || Recurs) {
            
            helpParams.path = Path;
            helpParams.name_mask = "*";
            
            err = saveInstances(Server, helpParams, libList, conList,
                                out, Recurs, fout);
            if(err) {
                out += log_getErrMsg(err);
                return err;
            }
        }
    }
    
    return KS_ERR_OK;
}

/******************************************************************************/
KS_RESULT write_db_data(KscServerBase* Server, FbDirParams& Pars,
                        PltString& out, int& Recurs, FILE *fout) {
/******************************************************************************/

    PltList<PltString>  HasConList;     // Liste der Instanzen mit Verbindungen
    PltList<PltString>  libList;        // Liste aller Bibliotheken
    PltList<PltString>  conList;        // Liste aller Verbindungen
    KsString            libName;

    KS_RESULT err = saveInstances(Server, Pars, libList, conList, out, Recurs, fout);
    if(err) {
        out += log_getErrMsg(err);
        return err;
    }

    // Verbindungen sichern
    err = saveConsFromList(Server, conList, out, fout);
    if(err) {
        out += log_getErrMsg(err);
        return err;
    }

    // Schreiben in Datei ?
    if(fout) {
        if(out != "") {
            fputs((const char*)out, fout);
            // String-Buffer leeren
            out = "";
        }
    }

    while(libList.size() ) {
        libName = libList.removeFirst();
        ifb_writeLibItem(libName, out);
    }

    // Schreiben in Datei ?
    if(fout) {
        if(out != "") {
            fputs((const char*)out, fout);
            // String-Buffer leeren
            out = "";
        }
    }
    
    return KS_ERR_OK;
}

/******************************************************************************/
void writeHeadSelectSave(KscServerBase* Server, PltString &out, FILE* fout)
/******************************************************************************/
{
    struct tm* t;
    time_t timer;
    char   help[64];

    PltTime t0;
    PltTime tt = t0.now();

    timer = (time_t)tt.tv_sec;
    t = localtime(&timer);

    out += "/*********************************************************************\n";
    out += "======================================================================\n";        
    out += "  Selektierte Sicherung der Datenbasis.\n";

    if(t) {
        sprintf(help,"\n  Erstellt : %4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d\n\n",
                        t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec );
                out += help;
    }

    out += "  HOST                : ";
    out +=  (const char*)Server->getHost();
    out += "\n  SERVER              : ";
    out +=  (const char*)Server->getName();

    out += "\n======================================================================\n";        
    out += "*********************************************************************/\n\n";

    // Schreiben in Datei ?
    if(fout) {
        fputs((const char*)out, fout);
        // String-Buffer leeren
        out = "";
    }
}

/******************************************************************************/
KS_RESULT select_save_eval(KscServerBase   *Server,
                           FbDirParams     &Pars,
                           PltString       &out,
                           FILE*           fout=0)
/******************************************************************************/
{
    char*       ph;                                 /* Hilfszeiger */
    char*       phelp;                                 /* Hilfszeiger */
    int         Recurs = 0;
    int         count = 0;
    PltString   log("");
    KS_RESULT   err;
    
    if( (Pars.path).len() < 2) {
        write_help_message_from_select_save(out);
        return KS_ERR_BADPATH;
    }

    log = "";
    err = KS_ERR_OK;
    iFBS_SetLastError(1, err, log);

    // Head der Datei
    writeHeadSelectSave(Server, out, fout);
    
    // Was soll gesichert werden?
    FbDirParams  param;
    param.path = "";

    phelp = (char*)malloc(Pars.path.len() + 1);
    if(!phelp) {
        return KS_ERR_TARGETGENERIC;
    }
    strcpy(phelp, (const char*)Pars.path);
    ph = phelp;
    
    if( (*ph) != '/') {
        param.path = "/";
    }

    while( ph && (*ph) ) ph++; // Suche Ende
    ph--;

    if( (*ph) == '*') {
        Recurs = 1;
        *ph = '\0';
        ph--;
    }
    if( (*ph) == '/') {
        *ph = '\0';
    }

    param.path += phelp;
    param.type_mask = Pars.type_mask;
    param.name_mask = Pars.name_mask;

    ph = phelp;
    if( (*ph) == '/') {
        ph++;
    }
    
    if( !strncmp(ph, FB_LIBRARIES_CONTAINER, strlen(FB_LIBRARIES_CONTAINER)) ) {

        param.type_mask = (FB_OBJ_LIBRARY | FB_OBJ_CLASS);

        while( (*ph)  ) {
            if((*ph) == '/') count++;
            ph++;
        }
        if(count > 1) {
            free(phelp);
            out += log_getErrMsg(KS_ERR_OK,"Bad path",(const char*)param.path);
            write_help_message_from_select_save(out);
        
            err = KS_ERR_BADPATH;
            log = "\"%s\"  \"";
            log += param.path;
            log += "\"";
            iFBS_SetLastError(1, err, log);
        
            return KS_ERR_BADPATH;
        }
        
        err = write_inst_from_class(Server, param, out, fout);

    } else {
        if( (!strncmp(ph, FB_INSTANZ_CONTAINER, strlen(FB_INSTANZ_CONTAINER))) &&
            (!strncmp(ph, FB_TASK_CONTAINER, strlen(FB_TASK_CONTAINER))) ) {
            free(phelp);
            
            out += log_getErrMsg(KS_ERR_OK,"Bad path",(const char*)Pars.path);
            write_help_message_from_select_save(out);
        
            err = KS_ERR_BADPATH;
            log = "\"%s\"  \"";
            log += Pars.path;
            log += "\"";
            iFBS_SetLastError(1, err, log);

            return KS_ERR_BADPATH;
        }
        
        err = write_db_data(Server, param, out, Recurs, fout);
    }

    free(phelp);

    // Schreiben in Datei ?
    if(fout) {
        if(out != "") {
            fputs((const char*)out, fout);
            // String-Buffer leeren
            out = "";
        }
    }
    
    return err;
}

/******************************************************************************/
KS_RESULT select_save(KscServerBase*  Server,
                      FbDirParams     &Pars,
                      PltString       &datei)
/******************************************************************************/
 {
    FILE*              fout;
    KS_RESULT          err;
    PltString          Out("");

    if(Server == NULL) {
        err = KS_ERR_SERVERUNKNOWN;
        iFBS_SetLastError(1, err, Out);
              return err;
    }
    if( !datei.len() ) {
        err = OV_ERR_CANTOPENFILE;
        Out = "\"%s\"  \"\"";
        iFBS_SetLastError(1, err, Out);
        return err;
    }
    fout = fopen((const char*)datei, "w");
    if(!fout) {
        err = OV_ERR_CANTOPENFILE;
        Out = "\"%s\"  \"";
        Out += datei;
        Out += "\"";
        iFBS_SetLastError(1, err, Out);
        return err;
    }

    err = select_save_eval(Server, Pars, Out, fout);
    fputs((const char*)Out, fout);
    if(err) {
        Out = IFBS_GetLastLogError();
        if(Out == "") {
            iFBS_SetLastError(1, err, Out);
        }
    }

    fclose(fout);
    return err;
}


/******************************************************************************/
KS_RESULT IFBS_SELECT_SAVE(KscServerBase   *Server,
                           FbDirParams     &Pars,
                           PltString       &out)
/******************************************************************************/
{
    return select_save_eval(Server, Pars, out);
}


