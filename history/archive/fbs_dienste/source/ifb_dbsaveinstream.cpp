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
*   ifb_dbsaveinstream.cpp                                                   *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   07-04-1999  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*   30-03-2000  IFBS_DB_SAVE_TOSTREAM                                        *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*                                                                            *
*                                                                            *
*****************************************************************************/

#include "ifbslibdef.h"

/******************************************************************************/
void ifb_writeLinkItem(
    KscServerBase    *Server,
    KsString         &path,
    int               istParent,
    FbAssoParam      &assPar,
    PltString        &Out) {
/******************************************************************************/
    PltString help;                               // Hilfsstring
    char      hStr[256];                          // Hilfsstring
    char     *ph;                                 // Hilfszeiger
    KsString  root = Server->getHostAndName();    // Merke : //host/server
    
    // Daten holen
    KscVariable  var(root + path);
    if(!var.getUpdate() ) {
        // ?
        return;
    }
    const KsVarCurrProps* cp = var.getCurrProps();
    if(!cp || !cp->value) {
        // ?
        return;
    }

    PltString hs("");
    ifb_getValueOnly(                          /*  |> Funktionsrueckmeldung                     */
                       cp                      /* >|  Eigenschaften der Variable                */
                       ,hs                     /* >|> Value als String                          */
                       ,ULONG_MAX              /* >|  Max. Anzahl Elementen in Array            */
                       ,TRUE                   /* >|  Merker, ob Time-Ausgabe "... hh:mm:ss"    */
                       ,FALSE                  /* >|  Merker, ob TRUE als "1" und FALSE als "0" */
                       ,FALSE                  /* >|  Merker, ob Array als Liste "{...}"        */
                       ," , "                  /* >|  Trenner der Array-Elementen               */
                       ,FALSE                  /* >|  Merker, ob string in Hochkommas           */
                    );
    if( hs == "" ) {
        // Keine Objekte in der Liste
        return;
    }
        

    if(istParent == 1) {
        assPar.child_path = (const char*)hs;
    } else {
        assPar.parent_path = (const char*)hs;
    }
        
        
    help = " LINK\n    OF_ASSOCIATION  ";
    strcpy(hStr, (const char*)assPar.identifier);
    ph = hStr;
    while(ph && (*ph)) ph++;
    //the following is save even without a *ph check, as we go backwards
    // cppcheck-suppress nullPointer
    while(((*ph) != '/') && (ph != hStr)) ph--;
    if((*ph) == '/') ph++;
    help += ph;

    help += ";\n    PARENT  ";
    strcpy(hStr, (const char*)assPar.parent_ident);
    ph = hStr;
    while(ph && (*ph)) ph++;
    // cppcheck-suppress nullPointer
    while(((*ph) != '/') && (ph != hStr)) ph--;
    if((*ph) == '/') ph++;
    help += ph;

    help += " : CLASS ";
    strcpy(hStr, (const char*)assPar.parent_class);
    ph = hStr;
    while(ph && (*ph)) ph++;
    // cppcheck-suppress nullPointer
    while(((*ph) != '/') && (ph != hStr)) ph--;
    if((*ph) == '/') ph++;
    help += ph;
    help += "\n        = ";
    help += assPar.parent_path;

    help += ";\n    CHILDREN  ";
    help += assPar.child_ident;
    
    help += " : CLASS ";
    strcpy(hStr, (const char*)assPar.child_class);
    ph = hStr;
    while(ph && (*ph)) ph++;
    // cppcheck-suppress nullPointer
    while(((*ph) != '/') && (ph != hStr)) ph--;
    if((*ph) == '/') ph++;
    help += ph;
    help += "\n        = {";
    help += assPar.child_path;
    help += "};\n";

    help += " END_LINK;\n\n";

    // Fertig
    Out += help;
    return;
}

/******************************************************************************/
KS_RESULT ifb_writeLinks(
    KscServerBase    *Server,
    KsGetEPParams    &params,
    KsString         &instClass,
    PltString        &Out,
    bool              saveConLinks,
    bool              parentOnly,
    FILE             *fout) {
/******************************************************************************/
    KsGetEPResult    result;
    KS_RESULT        err;

    // Wir suchen Links
    params.type_mask = KS_OT_LINK;
    params.scope_flags = KS_EPF_PARTS;

    bool ok = Server->getEP(0, params, result);
    if ( !ok ) {
        err = Server->getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
        return err;
    }
    if ( result.result ) {
        return result.result;
    }

    size_t                     Anz ;            /* Merker : Anzahl gefundenen Objekten */
    
    Anz = result.items.size();
    if(!Anz) {
        // Keine Objekten gefungen
        return KS_ERR_OK;
    }   

    PltList<KsEngPropsHandle>    parentList;      /* Merker : gefundene Links */
    PltList<KsEngPropsHandle>    childList;       /* Merker : gefundene Links */
    KS_LINK_TYPE                 LinkTyp;         /* Merker : Link-Typ        */
    KsString                     hs;              /* Hilfsstring              */
    while ( result.items.size() ) {

        KsEngPropsHandle hpp = result.items.removeFirst();
        if(!hpp) {
            return KS_ERR_GENERIC;
        }


        // FB-Links?
        hs = hpp->identifier;
        if(saveConLinks == FALSE) {
            if( (hs == "inputcon")  ||
                (hs == "outputcon") ||
                (hs == "sourcefb") ||
                (hs == "targetfb") ) {

                 continue;
            }
        }
        
        // Es genuegt, wenn wir fuer 1:M-Links nur die Parent-Seite sichern.
        // Bei den M:N-Links sichern wir beide Seiten.
        LinkTyp = ((KsLinkEngProps&)(*hpp)).type;
        switch(LinkTyp) {
            case KS_LT_LOCAL_1_1:
            case KS_LT_LOCAL_1_MANY:
            case KS_LT_GLOBAL_1_1:
            case KS_LT_GLOBAL_1_MANY:
            case KS_LT_LOCAL_MANY_MANY:
            case KS_LT_GLOBAL_MANY_MANY:
                                parentList.addLast(hpp);
                                break;
            default:
                                if(parentOnly == FALSE) {
                                    childList.addLast(hpp);
                                }
                                break;
        }
    }

    if( parentList.isEmpty() && childList.isEmpty()) {
        // Keine Links gefunden
        return KS_ERR_OK;
    }
    
    KsString    help;
    int         istParent;
    FbAssoParam assPar;

    while( parentList.size() || childList.size() ) {
        
        hs = "";
        istParent = 1;
        if( parentList.size() ) {
            KsEngPropsHandle hpv = parentList.removeFirst();
            if(!hpv) {
                // ?
                continue;
            }
            hs = (const char*)hpv->identifier;


            assPar.identifier  = (const char*)((KsLinkEngProps &)(*hpv)).association_identifier;
            assPar.parent_ident= (const char*)((KsLinkEngProps &)(*hpv)).opposite_role_identifier;
            assPar.parent_class= (const char*)instClass;
            assPar.parent_path = (const char*)params.path;
            assPar.child_ident = (const char*)hs;
            assPar.child_class = "unknown";
            assPar.child_path = "";
        } else {
            KsEngPropsHandle hpv = childList.removeFirst();
            if(!hpv) {
                // ?
                continue;
            }
            hs = (const char*)hpv->identifier;
            
            istParent = 0;
            assPar.identifier  = (const char*)((KsLinkEngProps &)(*hpv)).association_identifier;
            assPar.parent_ident= (const char*)hs;
            assPar.parent_class= "unknown";
            assPar.parent_path = "";
            assPar.child_ident = (const char*)((KsLinkEngProps &)(*hpv)).opposite_role_identifier;
            assPar.child_class = (const char*)instClass;
            assPar.child_path  = (const char*)params.path;
        }
        
        help = params.path;
        help += ".";
        help += hs;

        // Link dokumentieren
        ifb_writeLinkItem(Server,help,istParent,assPar,Out);
        

        // Schreiben in Datei ?
        
        if(fout) {
            if( Out.len() ) {
                fputs((const char*)Out, fout);
                Out = "";
            }
        }
        
    }
    
    return KS_ERR_OK;
}

// Holt Server-Version
/*****************************************************************************/
float get_serverVersion(KscServerBase* Server)
/*****************************************************************************/
{
    PltString   hStr("");
    KsString    Var;
    char        versString[32];
    char       *ph;
    KsString    root = Server->getHostAndName();
    
    Var = "/vendor/server_version";
    KscVariable  vvar(root + Var);
    if(!vvar.getUpdate() ) {
        // ?
        return 0;
    }
    const KsVarCurrProps* cp = vvar.getCurrProps();
    if(!cp || !cp->value) {
        // ?
        return 0;
    }
 
    ifb_getValueOnly(                                              /*  |> Funktionsrueckmeldung      */
                       cp                      /* >|  Eigenschaften der Variable */
                       ,hStr                    /* >|> Value als String           */
                       ,1       /* Max. Anzahl Elementen in Array */
                       ,FALSE       /* Merker, ob Time-Ausgabe "... hh:mm:ss" */
                       ,FALSE       /* Merker, ob TRUE als "1" und FALSE als "0" */
                       ,FALSE         /* Merker, ob Array als Liste "{...}"        */
                       ," "         /* Trenner der Array-Elementen               */
                       ,FALSE              /* Merker, ob string in Hochkommas   */
                       ,FALSE
                       );
    if( hStr == "" ) {
        // ?
        return 0;
    }
        
    strncpy(versString, (const char*)hStr, 16);
    ph = strstr(versString, ".");
    if(!ph) {
        // Kein Dezimal-Punkt?
        ph = versString;
    }
    ph++;
    while(ph && (*ph)) {
        if(*ph == '.') {
            // 2. Punkt gefunden
            *ph = '\0';
            break;                
        }
        ph++;
    }
        
    return (float)atof(versString);
}

// Objekt-Variablen protokollieren
/*****************************************************************************/
 KS_RESULT get_variable(KscServerBase* Server,KsGetEPParams& params,PltString& Out)
/*****************************************************************************/
{
    KS_RESULT      err;
    KsGetEPResult  result;
    KsString       Var;
    KsString       root;
    size_t         i;               /* Laufvariable */
    size_t         AnzVars;         /* Merker : Anzahl gefundenen Variablen */
    size_t         AnzFoundObjs;    /* Merker : Anzahl gefundenen Unterobjekten */
    char           PortType[32];
    PltString      hStr;
    float          srvVersion;
    PltString      trenner;
    
    // Wir suchen Variablen
    trenner = ".";
    params.type_mask = KS_OT_VARIABLE;
    params.scope_flags = KS_EPF_PARTS;
    
    // Sonderfall "Vendor"
    if(params.path == "/vendor") {
        trenner = "/";
        params.scope_flags = KS_EPF_DEFAULT;
    }
    
    // Zugriffsrechte ab iFBSpro v2.4.0 geaendert
    srvVersion = get_serverVersion(Server);

    // Alle Variablennamen holen
    bool ok = Server->getEP(0, params, result);
    if( !ok ) {
        err = Server->getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
        return err;
    }
    if( result.result != KS_ERR_OK ) {
        return result.result;
    }

    AnzFoundObjs = result.items.size();
    
    if ( !AnzFoundObjs ) {
        // Keine Variablen in Domain
        return KS_ERR_OK;
    }

    /* Variablen sichern */

    Out += "    VARIABLE_VALUES\n";

    /* Alle Variable merken */
    PltArray<KscVariable*> VarArray(AnzFoundObjs);
    if(VarArray.size() != AnzFoundObjs) {
            return OV_ERR_HEAPOUTOFMEMORY;
    }
    

    // Package anlegen
    KscPackage        *pkg = new KscPackage;
    if(!pkg) {
        return OV_ERR_HEAPOUTOFMEMORY;
    }

    PltList<KsEngPropsHandle>  ListEP;
    AnzVars = 0;
    root = Server->getHostAndName();
    
    for(i = 0; i < AnzFoundObjs; i++) {
        
        KsEngPropsHandle pv(result.items.removeFirst());
        if(!pv) {
            delete pkg;
            return KS_ERR_GENERIC;
        }
        
        // Ist das eine Variable?
        if(pv->xdrTypeCode() == KS_OT_VARIABLE) {
            
            // FIX: "ServerPassword" in "/vendor" ignorieren
            if( (params.path == "/vendor") && (pv->identifier == "server_password") ) {
                // Ignorieren
            } else {
                ListEP.addLast(pv);
             
                Var = params.path;
                Var += trenner;
                Var += pv->identifier;  /* /. */    

                VarArray[AnzVars] = new KscVariable(root+Var);
                if(!VarArray[AnzVars]) {
                    delete pkg;
                    return OV_ERR_HEAPOUTOFMEMORY;
                }
            
                if(!pkg->add(KscVariableHandle(VarArray[AnzVars], PltOsNew)) ) {
                    delete pkg;
                         return KS_ERR_GENERIC;
                }
            
                // Eine Variable mehr
                AnzVars++;
            }
        }
    } /* for (alle Unterobjkte) */
  
    if(!pkg->getUpdate() ) {
        err = pkg->getLastResult();
        
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
        delete pkg;
        return err;
    }

    for(i = 0; i < AnzVars; i++) {
        
        KsEngPropsHandle hpp = ListEP.removeFirst();
        if(!hpp) {
            delete pkg;
            return KS_ERR_GENERIC;
        }

        const KsVarCurrProps *cp = VarArray[i]->getCurrProps();
        if( (!cp) || (!cp->value) ) {
            delete pkg;
            return KS_ERR_GENERIC;
        }

        // Zugriffsrechte ab iFBSpro v2.4.0 geaendert
        if(srvVersion < 2.4) {
        
           if( IsFlagSet( ((KsVarEngProps&)(*hpp)).semantic_flags, 'i') ) {
                    strcpy(PortType,"INPUT");
            } else if( IsFlagSet( ((KsVarEngProps&)(*hpp)).semantic_flags, 'p') ) {
                    strcpy(PortType,"PARAMETER");
            } else if( IsFlagSet( ((KsVarEngProps&)(*hpp)).semantic_flags, 'o') ) {
                    strcpy(PortType,"OUTPUT");
            } else if( IsFlagSet( ((KsVarEngProps&)(*hpp)).semantic_flags, 'n') ) {
                  strcpy(PortType,"HIDDEN");
            } else {
                // Instanz hat keine FB-Flags. Dann ist die von OV.
                if( ((KsVarEngProps&)(*hpp)).access_mode & KS_AC_WRITE ) {
                    strcpy(PortType,"INPUT");
                } else {
                    // Es bleiben nur Outputs
                    strcpy(PortType,"OUTPUT");
                }
            }
        
        // Version >= 2.4
        } else {
            /* 
            *  Da die Flags zB. in CAEX-Server fuer Objekte "missbraucht" werden,
            *  setze INPUT/OUTPUT-Merker anhand Zugriffs-Rechten
            */
            if( ((KsVarEngProps&)(*hpp)).access_mode & KS_AC_WRITE ) {
                if( IsFlagSet( ((KsVarEngProps&)(*hpp)).semantic_flags, 'p') ) {
                    // Parameter
                    strcpy(PortType,"PARAMETER");
                } else {
                    // Input
                    strcpy(PortType,"INPUT");
                }
            } else {
                
                if( IsFlagSet( ((KsVarEngProps&)(*hpp)).semantic_flags, 'n') ) {
                      strcpy(PortType,"HIDDEN");
                } else {
                    // Es bleiben nur Outputs
                    strcpy(PortType,"OUTPUT");
                }
            }
        }
        Out += "        ";
        Out += (const char*)hpp->identifier;
        Out += ifb_getValueLength(cp);
        Out += " : ";
        Out += PortType;
        Out += "  ";
        Out += ifb_getOvValueType(cp->value->xdrTypeCode() );
        Out += " = ";
        ifb_getValueOnly(
                                cp              /* >|  Eigenschaften der Variable            */
                                ,Out            /* >|> Value als String                      */
                                ,ULONG_MAX      /* Max. Anzahl Elementen in Array            */
                                ,FALSE          /* Merker, ob Time-Ausgabe "... hh:mm:ss"    */
                                ,FALSE          /* Merker, ob TRUE als "1" und FALSE als "0" */
                                ,TRUE           /* Merker, ob Array als Liste "{...}"        */
                                ," , "          /* Trenner der Array-Elementen               */
                                );
        if( IsFlagSet( ((KsVarEngProps&)(*hpp)).semantic_flags, 's') ) {
            sprintf(PortType," STATE = %d", cp->state);
            Out += PortType;
        }
        
        Out += ";\n";
        
    } // for alle Variablen

    Out += "    END_VARIABLE_VALUES;\n";

    /* Speicher freigeben */
    delete pkg;

    return KS_ERR_OK;

} /* get_variable */

/*****************************************************************************/
 void ifb_writeLibItem(KsString libname, PltString& Out)
/*****************************************************************************/
{
    Out += " LIBRARY\n    ";
    Out += (const char*)libname;
    Out += "\n";
    Out += " END_LIBRARY;\n\n";
}

/*****************************************************************************/
 KS_RESULT get_libs(KscServerBase* Server, PltString& Out)
/*****************************************************************************/
{
    size_t   i, siz;
    KsString Path("");
    Path = Server->getHostAndName();
    Path += "/vendor/libraries";
    
    KscVariable Var(Path);
    if(!Var.getUpdate() ) {
        KS_RESULT err = Var.getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
        return err;
    }
    const KsVarCurrProps *cp = Var.getCurrProps();
    if((!cp) || (!(cp->value)) ) {
        return KS_ERR_GENERIC;
    }
    
    siz = ((KsStringVecValue &) *cp->value).size();
    for ( i = 0; i < siz; i++ ) {
        Path = ((KsStringVecValue &) *cp->value)[i];
        
        if(Path != "/acplt/ov") {
            // Bibliothek sichern
            ifb_writeLibItem(Path, Out);
        }
    }

    return KS_ERR_OK;

} /* get_libs */

int isFbToolInstance(KsString& fullclas) {
    char help[512];
    char *ph;
    PltString lib;
    PltString clas;
    
    // String fuer weitere Bearbeitung kopieren
    strcpy(help, (const char*)fullclas);
    
    // String-Ende suchen
    ph = help;
    while(ph && (*ph) ) ph++;
    
    // Class suchen
    while(ph != help) {
        if(*ph == '/') {
            *ph = '\0';
            ph++;
            clas = ph;
            break;
        }
        ph--;
    }
    // Class suchen
    while(ph != help) {
        if(*ph == '/') {
            *ph = '\0';
            ph++;
            lib = ph;
            break;
        }
        ph--;
    }
    
    if(lib == "") {
        // ?
        return 0;
    }
    
    if( (lib == "fb") && (clas == "dbinfoclass") ) {
        return 1;
    }
    if( lib != "fbtool" ) {
        return 0;
    }
    return 1;
}

/*****************************************************************************/
void ifb_writeInstBlockAnfang(KsString &inst, KsString &clas, PltString &Out) {
/*****************************************************************************/
    Out += " INSTANCE  ";
    Out += (const char*)inst;
    Out += " :\n";
    Out += "    CLASS ";
    Out += clas;
    Out += ";\n";
}
/*****************************************************************************/
void ifb_writeInstBlockEnd(PltString &Out) {
/*****************************************************************************/
    Out += " END_INSTANCE;\n\n";
}

/*****************************************************************************/
KS_RESULT ifb_writeInstData(
    KscServerBase *Server,
    KsGetEPParams &params,
    PltString     &Out,
    bool           recurs,
    bool           saveConLinks,
    bool           linkParentOnly,
    FILE          *fout
) {
/*****************************************************************************/
 

    KS_RESULT      fehler;
    KsGetEPResult  result;
    KsString       instClass;
    KsString       instPath;
    
    bool ok = Server->getEP(0, params, result);
    if( !ok ) {
        fehler = Server->getLastResult();
        if(fehler == KS_ERR_OK) fehler = KS_ERR_GENERIC;
        return fehler;
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
        

        if( (params.path == FB_TASK_CONTAINER_PATH) && (hpp->identifier == FB_URTASK) ) {
            // UrTask nicht sichern
            continue;
        }

        // Bibliothek?
        if(hpp->xdrTypeCode() == KS_OT_HISTORY) {
             instClass = ((KsHistoryEngProps &)(*hpp)).type_identifier;
        } else {
             instClass = ((KsDomainEngProps &)(*hpp)).class_identifier;
        }
        if(instClass == "/acplt/ov/library") {
            // Bibliotheken bereits gesichert
            continue;
        }
        
        // Automatisch erzeugte FB/Fbtool-Instanz?
        if( isFbToolInstance(instClass) == 1 ) {
            continue;
        }
        
        instPath = (const char*)params.path;
        if( hpp->access_mode & KS_AC_PART) {
            instPath += ".";
        } else {
            instPath += "/";
        }
        instPath += (const char*)hpp->identifier;

        ifb_writeInstBlockAnfang(instPath, instClass, Out);
        
        if( hpp->xdrTypeCode() != KS_OT_HISTORY) {
            // Instanz oder Container
            KsGetEPParams helpParams;
            
            helpParams.path = instPath;
            helpParams.name_mask = "*";
        
            if( ((KsDomainEngProps &)(*hpp)).class_identifier != CONTAINER_CLASS_PATH ) {
                // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                // Es ist eine Instanz vom benutzerdefiniertem Typ
                // Variablen von Instanz rueckdokumentieren :
                // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                fehler = get_variable(Server, helpParams, Out);
                if (fehler) {
                    return fehler;
                }
            }
        }
        ifb_writeInstBlockEnd(Out);
                    
        // Schreiben in Datei ?
        if(fout) {
            if(Out.len() ) {
                fputs((const char*)Out, fout);
                // String-Buffer leeren
                Out = "";
            }
        }
        
        // Unterliegende Instanzen sichern
        KsGetEPParams helpPars;
        
        helpPars.path = params.path;
        if( hpp->access_mode & KS_AC_PART) {
            helpPars.path += ".";
        } else {
            helpPars.path += "/";
        }
        
        helpPars.path += hpp->identifier;
        helpPars.name_mask = "*";
        helpPars.type_mask = KS_OT_DOMAIN | KS_OT_HISTORY;
        
        
        if(recurs == TRUE) {
            // Alle Unterobjekte
            helpPars.scope_flags = KS_EPF_DEFAULT;
        } else {
            // Nur Parts
            helpPars.scope_flags = KS_EPF_PARTS;
        }
        fehler = ifb_writeInstData(Server,helpPars,Out,recurs,saveConLinks,linkParentOnly,fout);
        if(fehler) {
            return fehler;
        }
        
        // Links sichern
        fehler = ifb_writeLinks(Server,helpPars,instClass,Out,saveConLinks,linkParentOnly,fout);
        if(fehler) {
            return fehler;
        }

    } /* while size() */

    return KS_ERR_OK;

} /* ifb_writeInstData */

/******************************************************************************/
// Dokumentation der vom Benutzer hinzugefuegten dynamischen Links zu Basis-Objs
/******************************************************************************/
KS_RESULT ifb_writeXlinksOfObj(KscServerBase *Server,
                               KsString      &path,
                               KsString      &instClass,
                               PltString     &Out,
                               FILE          *fout)
/******************************************************************************/
{
    KsGetEPParams       params;
    KsGetEPResult       result;
    KS_RESULT           err;
    KS_LINK_TYPE        LinkTyp;         /* Merker : Link-Typ        */
    KsString            hs;              /* Hilfsstring              */

    // Links vorhanden?
    params.path = path;
    params.name_mask = "*";
    params.type_mask = KS_OT_LINK;
    params.scope_flags = KS_EPF_PARTS;
    
    bool ok = Server->getEP(0, params, result);
    if( !ok ) {
        err = Server->getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
        return err;
    }
    if( result.result != KS_ERR_OK ) {
        return result.result;
    }


    /* Ueber alle Objekte */
    FbAssoParam assPar;
    while( result.items.size() ) {
        KsEngPropsHandle hpp = result.items.removeFirst();
        if(!hpp) {
            return KS_ERR_GENERIC;
        }

        // FB- oder OV-Links?
        hs = ((KsLinkEngProps &)(*hpp)).association_identifier;
        if( (!strncmp( (const char*)hs, FB_LIB_PATH, strlen(FB_LIB_PATH))) ||
            (!strncmp( (const char*)hs, OV_LIB_PATH, strlen(OV_LIB_PATH))) ) {
            
            continue;
        }
        
        // Wir sichern nur die Parent-Seite fuer 1:M-Links.
        // Bei den M:N-Links sichern wir beide Seiten.
        LinkTyp = ((KsLinkEngProps&)(*hpp)).type;
        switch(LinkTyp) {
            case KS_LT_LOCAL_1_1:
            case KS_LT_LOCAL_1_MANY:
            case KS_LT_GLOBAL_1_1:
            case KS_LT_GLOBAL_1_MANY:
            case KS_LT_LOCAL_MANY_MANY:
            case KS_LT_GLOBAL_MANY_MANY:
                assPar.identifier  = (const char*)((KsLinkEngProps &)(*hpp)).association_identifier;
                assPar.parent_ident= (const char*)((KsLinkEngProps &)(*hpp)).opposite_role_identifier;
                assPar.parent_class= (const char*)instClass;
                assPar.parent_path = (const char*)path;
                assPar.child_ident = (const char*)hpp->identifier;
                assPar.child_class = "unknown";
                assPar.child_path = "";
                
                // Link dokumentieren
                hs = path;
                hs += ".";
                hs += hpp->identifier;
                ifb_writeLinkItem(Server, hs, 1, assPar, Out);
        
                break;
            default:
                break;
        }
        
        // Schreiben in Datei ?
        if(fout) {
            if( Out.len() ) {
                fputs((const char*)Out, fout);
                Out = "";
            }
        }
    } // Ueber alle vorhandene Links
    
    return KS_ERR_OK;        
}
/******************************************************************************/
KS_RESULT ifb_writeXlinksOfBases(KscServerBase *Server,
                                 PltString     &Out,
                                 FILE          *fout)
/******************************************************************************/
{
    KsGetEPParams       params;
    KsGetEPResult       result;
    KS_RESULT           err;
    KsString            path;
    KsString            instClass;
    size_t              i;
    
    // --------------------
    // Alle Domains in Root
    // --------------------
    
    params.path = "/";
    params.name_mask = "*";
    params.type_mask = KS_OT_DOMAIN;
    params.scope_flags = KS_EPF_CHILDREN;

    bool ok = Server->getEP(0, params, result);
    if( !ok ) {
        err = Server->getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
        return err;
    }
    if( result.result != KS_ERR_OK ) {
        return result.result;
    }

    /* Ueber alle Objekte */
    while( result.items.size() ) {
        KsEngPropsHandle hpp = result.items.removeFirst();
        if(!hpp) {
            return KS_ERR_GENERIC;
        }
        
        // Merke Pfad
        path = "/";
        path += hpp->identifier;
        instClass = ((KsDomainEngProps &)(*hpp)).class_identifier;
        err = ifb_writeXlinksOfObj(Server, path, instClass, Out, fout);
        
        // Schreiben in Datei?
        if(fout) {
            if( Out.len() ) {
                fputs((const char*)Out, fout);
                // String-Buffer leeren
                Out = "";
            }
        }
        // Fehler bei Ausfuehrung?
        if(err) {
            return err;
        }
    } /* Ueber alle Root-Objekte */


    // -----------------------
    // Ueber alle Bibliotheken,Klassen,Associationen
    // -----------------------
    PltList<PltString> varList;
    PltList<PltString> typList;
    varList.addLast("/vendor/libraries");    typList.addLast("library");
    varList.addLast("/vendor/classes");      typList.addLast("class");
    varList.addLast("/vendor/associations");  typList.addLast("association");
    
    KsString  root = Server->getHostAndName();    // Merke : //host/server
    while( varList.size() ) {
        path = varList.removeFirst();
        instClass = typList.removeFirst();
        
        KscVariable  varL(root + path);
        if(!varL.getUpdate() ) {
            // ?
            return KS_ERR_GENERIC;
        }
        const KsVarCurrProps* cpi = varL.getCurrProps();
        if(!cpi || !cpi->value) {
            // ?
            return KS_ERR_GENERIC;
        }

        switch(cpi->value->xdrTypeCode() ) {
            case KS_VT_STRING_VEC: {
                    size_t size  = ((KsStringVecValue &) *(cpi->value)).size();
                    for ( i = 0; i < size; ++i ) {
                        path = (const char*)(((KsStringVecValue &) *(cpi->value))[i]);
                        err = ifb_writeXlinksOfObj(Server, path, instClass, Out, fout);
                        
                        // Schreiben in Datei?
                        if(fout) {
                            if( Out.len() ) {
                                fputs((const char*)Out, fout);
                                // String-Buffer leeren
                                Out = "";
                            }
                        }
                        // Fehler bei Ausfuehrung?
                        if(err) {
                            return err;
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

/*****************************************************************************/
KS_RESULT ifb_writeRootObjs(
    KscServerBase *Server,
    KsGetEPParams &params,
    PltString     &Out,
    FILE          *fout
) {
/*****************************************************************************/
    KS_RESULT      fehler;
    KsGetEPResult  result;
    KsString       instPath;
    KsString       instClass;
    KsString       vendorClass;
    
    bool ok = Server->getEP(0, params, result);
    if( !ok ) {
        fehler = Server->getLastResult();
        if(fehler == KS_ERR_OK) fehler = KS_ERR_GENERIC;
        return fehler;
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

        // OV-Basis?
        if( hpp->identifier == "acplt" ) {
            // Nicht sichern
            continue;
        }
        if( hpp->identifier == "vendor" ) {
            // Wird spaeter gesichert
            if( hpp->xdrTypeCode() != KS_OT_HISTORY) {
                vendorClass = (const char*)(((KsDomainEngProps &)(*hpp)).class_identifier);
            }
            continue;
        }

        // Bibliothek?
        if( hpp->xdrTypeCode() == KS_OT_HISTORY) {
            instClass = (const char*)(((KsHistoryEngProps &)(*hpp)).type_identifier);            
        } else {
            // Instanz oder Container
            instClass = (const char*)(((KsDomainEngProps &)(*hpp)).class_identifier);
        }
        if(instClass == "/acplt/ov/library") {
            // Bibliotheken bereits gesichert
            continue;
        }
        // Automatisch erzeugte FB/Fbtool-Instanz?
        if( isFbToolInstance(instClass) == 1 ) {
            continue;
        }
        

        // Instanz oder Container
        instPath = "/";
        instPath += (const char*)hpp->identifier;
        ifb_writeInstBlockAnfang(instPath, instClass, Out);
                
        if( hpp->xdrTypeCode() != KS_OT_HISTORY) {
            KsGetEPParams helpParams;
            
            helpParams.path = instPath;
            helpParams.name_mask = "*";
            
            if( ((KsDomainEngProps &)(*hpp)).class_identifier != CONTAINER_CLASS_PATH ) {
                // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                // Es ist eine Instanz vom benutzerdefiniertem Typ
                // Variablen von Instanz rueckdokumentieren :
                // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


                fehler = get_variable(Server, helpParams, Out);
                if (fehler) {
                    return fehler;
                }
            }
        }
        ifb_writeInstBlockEnd(Out);
                    
        // Schreiben in Datei ?
        if(fout) {
            if( Out.len() ) {
                fputs((const char*)Out, fout);
                // String-Buffer leeren
                Out = "";
            }
        }

        // Unterliegende Instanzen sichern
        KsGetEPParams helpPars;
        
        helpPars.path = instPath;
        helpPars.name_mask = "*";
        helpPars.type_mask = KS_OT_DOMAIN | KS_OT_HISTORY;
        helpPars.scope_flags = KS_EPF_DEFAULT;
    
        //                                      Recurs conLnk parentOnly
        fehler = ifb_writeInstData(Server,helpPars,Out,TRUE,TRUE,  FALSE,  fout);
        if(fehler) {
            return fehler;
        }
            
        // Links sichern
        fehler = ifb_writeLinks(Server,helpPars,instClass,Out,TRUE,FALSE,fout);
        if(fehler) {
            return fehler;
    }

    } /* while size() */

    // Sonderfall: "Vendor"-Tree
    if(vendorClass != "") {
        KsGetEPParams helpPar;
        instPath = "/vendor";
        
        ifb_writeInstBlockAnfang(instPath, vendorClass, Out);
    
        helpPar.path = instPath;
        helpPar.name_mask = "*";

        fehler = get_variable(Server, helpPar, Out);
        if (fehler) {
            return fehler;
        }
        ifb_writeInstBlockEnd(Out);


        // Schreiben in Datei ?
        if(fout) {
            if( Out.len() ) {
                fputs((const char*)Out, fout);
                // String-Buffer leeren
                Out = "";
            }
        }
    }
    
    return KS_ERR_OK;
}

/******************************************************************************/
KS_RESULT IFBS_GETDBCONTENTS(KscServerBase *Server,
                             PltString     &Out,
                             FILE *fout = 0)
/******************************************************************************/
 {
    KsGetEPParams       params;
    char                help[256];
    PltString           log("");
    KS_RESULT           err = KS_ERR_OK;
   
    if(!Server) {
        err = KS_ERR_SERVERUNKNOWN;
        iFBS_SetLastError(1, err, log);
            return err;
    }

    iFBS_SetLastError(1, err, log);

    struct tm* t;
    time_t timer;

    PltTime tt = PltTime::now();

    timer = (time_t)tt.tv_sec;
    t = localtime(&timer);

Out =  "/*********************************************************************\n";
Out += "======================================================================\n";        
Out += "  Sicherung der Datenbasis.\n\n";

    if(t) {
        sprintf(help, "  Erstellt : %4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d\n\n",
                t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec );
                
        Out += help;
    }

    sprintf(help, "  HOST                : %s\n  SERVER              : %s\n",
            (const char*)Server->getHost(), (const char*)Server->getName() );
    Out += help;
  
    // Schreiben in Datei ?
    if(fout) {
        fputs((const char*)Out, fout);
        // String-Buffer leeren
        Out = "";
    }
    

Out += "======================================================================\n";        
Out += "*********************************************************************/\n\n";

    // Schreiben in Datei ?
    if(fout) {
        fputs((const char*)Out, fout);
        // String-Buffer leeren
        Out = "";
    }

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // Rueckdokumentation der Bibliotheken
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    err = get_libs(Server, Out);
    if(err) {
        if(fout) {
            if(Out.len() ) {
                fputs((const char*)Out, fout);
                // String-Buffer leeren
                Out = "";
            }
        }
        iFBS_SetLastError(1, err, log);
        return err;
    }

    // Schreiben in Datei ?
    if(fout) {
        if( Out.len() ) {
            fputs((const char*)Out, fout);
            // String-Buffer leeren
            Out = "";
        }
    }

    
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // Rueckdokumentation Root-Domains
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    params.path = "/";
    params.type_mask = KS_OT_DOMAIN;
    params.name_mask = "*";
    params.scope_flags = KS_EPF_DEFAULT;
    err = ifb_writeRootObjs(Server, params, Out, fout);
    if(err) {
        if(fout) {
            if( Out.len() ) {
                fputs((const char*)Out, fout);
                // String-Buffer leeren
                Out = "";
            }
        }
        iFBS_SetLastError(1, err, log);
        return err;
    }

    return KS_ERR_OK;
    
#if 0
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // Rueckdokumentation der X-Links Basis-Objektes
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    err = ifb_writeXlinksOfBases(Server, Out, fout);
    if(err) {
        if(fout) {
            if(Out != "") {
                fputs((const char*)Out, fout);
                // String-Buffer leeren
                Out = "";
            }
        }
        iFBS_SetLastError(1, err, log);
        return err;
    }

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // Rueckdokumentation der Task-Instanzen
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    params.path = "/";
    params.path += FB_TASK_CONTAINER;
    params.type_mask = KS_OT_DOMAIN;
    params.name_mask = "*";
    params.scope_flags = KS_EPF_DEFAULT;

    //                                      Recurs conLnk parentOnly
    err = ifb_writeInstData(Server,params,Out,TRUE,FALSE, TRUE,      fout);
    if(err) {
        if(fout) {
            if( Out.len() ) {
                fputs((const char*)Out, fout);
                // String-Buffer leeren
                Out = "";
            }
        }
        iFBS_SetLastError(1, err, log);
        return err;
    }

    
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // Rueckdokumentation der Funktionsblock-Instanzen
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    params.path = "/";
    params.path += FB_INSTANZ_CONTAINER;
    params.type_mask = (KS_OT_DOMAIN | KS_OT_HISTORY);
    params.name_mask = "*";
    params.scope_flags = KS_EPF_DEFAULT;

    //                                      Recurs conLnk parentOnly
    err = ifb_writeInstData(Server,params,Out,TRUE,FALSE,TRUE,fout);
    if(err) {
        if(fout) {
            if( Out.len() ) {
                fputs((const char*)Out, fout);
                // String-Buffer leeren
                Out = "";
            }
        }
        iFBS_SetLastError(1, err, log);
        return err;
    }

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // Rueckdokumentation der Verbindungen
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    params.path = "/";
    params.path += FB_CONN_CONTAINER;
    params.type_mask = KS_OT_DOMAIN;
    params.name_mask = "*";
    params.scope_flags = KS_EPF_DEFAULT;

    //                                      Recurs conLnk parentOnly
    err = ifb_writeInstData(Server,params,Out,TRUE,TRUE,  FALSE,  fout);
    if(err) {
        if(fout) {
            if( Out.len() ) {
                fputs((const char*)Out, fout);
                // String-Buffer leeren
                Out = "";
            }
        }
        iFBS_SetLastError(1, err, log);
        return err;
    }

   return KS_ERR_OK;

#endif

} /* exportproject */



/******************************************************************************/
KS_RESULT IFBS_DBSAVE(KscServerBase*         Server,
                                      PltString        &datei) {
/******************************************************************************/

    if(!Server) {
            return KS_ERR_SERVERUNKNOWN;
    }

    if(!datei.len() ) {
            return KS_ERR_BADNAME;
    }

    FILE *fout = fopen((const char*)datei, "w");
    if(!fout) {
                return KS_ERR_BADPATH;
    }

    fputs("/*********************************************************************\n", fout);
    fputs("* Datei : ", fout);
    fputs((const char*)datei, fout);
    fputs("\n*********************************************************************/\n", fout);

    PltString Str("");

    KS_RESULT err = IFBS_GETDBCONTENTS(Server, Str, fout);

//    fputs((const char*)Str, fout);

    fclose(fout);

    return err;
}


/******************************************************************************/
KS_RESULT IFBS_DBSAVE_TOSTREAM(KscServerBase*         Server,
                                                     PltString         &Out) {
/******************************************************************************/

 return IFBS_GETDBCONTENTS(Server, Out);

}
