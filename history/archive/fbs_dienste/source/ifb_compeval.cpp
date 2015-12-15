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
*   Fax      : 02273/9839-33                                                 *
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
*   ifb_compeval.cpp                                                         *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   2000-06-16  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*   iFSpro-Dienst "IFBS_DBCOMPARE"                                           *
*                                                                            *
*****************************************************************************/

/*
*        Includes
*        --------
*/
#include "ifbslibdef.h"

/*****************************************************************************/
KS_RESULT compare_eval(Dienst_param* newpar,
                       Dienst_param* oldpar,
                       PltString& out)
/*****************************************************************************/
{
    /*
    *   Lokale Variablen
    */
    KS_RESULT                fehler; /* Funktionsrueckmeldung */

    InstanceItems*   phelp;
    LinksItems*      philf;
    Child*           pchild;
    DelInstItems*    plibs;

    Dienst_param*    upd = (Dienst_param*)malloc(sizeof(Dienst_param));
    if(!upd) {
                return OV_ERR_HEAPOUTOFMEMORY;
    }
    upd->Instance = 0;
    upd->Set_Inst_Var = 0;
    upd->DelInst = 0;
    upd->OldLibs = 0;
    upd->NewLibs = 0;
    upd->Links = 0;
    upd->UnLinks = 0;

    compare_libraries(newpar,oldpar);

    out += "/*\n* Zu loeschende Bibliotheken :\n";
    out += "* ----------------------------\n*/\n\n";

    plibs = oldpar->NewLibs;
    while(plibs) {
        out += " DELETE_LIBRARY\n    ";
        out += plibs->Inst_name;
        out += "\n END_DELETE_LIBRARY;\n\n";
            plibs = plibs->next;
    }

    out += "/*\n* Zu ladende Bibliotheken :\n";
    out += "* -------------------------\n*/\n\n";

    plibs = newpar->NewLibs;
    while(plibs) {
        out += " LIBRARY\n    ";
        out += plibs->Inst_name;
        out += "\n END_LIBRARY;\n\n";
            plibs = plibs->next;
    }


    fehler = compare_any_inst(newpar,oldpar,upd,out);
    if(fehler) {
        memfre(upd);
        free(upd);
            return KS_ERR_BADPARAM;
    }

    out += "/*\n* Zu loeschende Instanzen :\n";
    out += "* -------------------------\n*/\n\n";

    phelp = oldpar->Instance;
    while(phelp) {
        // Wenn die Instanz eine Verbindung ist, loesche auch ihre links
        if( !strcmp(phelp->Class_name, CONNECTION_CLASS_PATH) ) {
            removeComConnLinks(oldpar, phelp);
        }
            out += " DELETE\n\t";
            out += phelp->Inst_name;
            out += "\n END_DELETE;\n\n" ;
            phelp = phelp->next;
    }


    out += "\n/*\n* Zu erzeugende Instanzen :\n";
    out += "* -------------------------\n*/\n\n";

    phelp = newpar->Instance;
    while(phelp) {
    
            out += " INSTANCE  ";
            out += phelp->Inst_name;
            out += " :\n    CLASS ";
            out += phelp->Class_name;
            out += ";\n";

            if(phelp->Inst_var) {
                    out += "\tVARIABLE_VALUES\n";

                    put_variable(phelp->Inst_var, out);

                    out += "\tEND_VARIABLE_VALUES;\n";
            }

            out += " END_INSTANCE;\n\n";

            phelp = phelp->next;
    }


    out += "\n/*\n* Instanzen mit geaenderten Parameter :\n";
    out += "* -------------------------------------\n*/\n\n";

    phelp = upd->Instance;
    while(phelp) {
            out += " SET ";
            out += phelp->Inst_name;
            out += " :\n\tVARIABLE_VALUES\n";

            put_variable(phelp->Inst_var, out);

            out += "\tEND_VARIABLE_VALUES;\n";
            out += " END_SET;\n\n";

            phelp = phelp->next;
    }

    memfre(upd);
    free(upd);

    get_any_links(newpar, oldpar);
    
    out += "\n/*\n* Zu loesende Links :\n";
    out += "* -------------------\n*/\n\n";

    philf = oldpar->Links;
    PltString               InpConLinkName("inputcon");
    PltString               OutConLinkName("outputcon");

    while(philf) {
        
        if( (InpConLinkName == philf->child_role) ||
            (OutConLinkName == philf->child_role) ) {
            philf = philf->next;
            continue;
        }
        
            out += " UNLINK\n    OF_ASSOCIATION  ";
            
            out += philf->asso_ident;
            out += "\n    PARENT  ";
            out += philf->parent_role;
            out += " : CLASS ";
            out += philf->parent_class;
            out += "\n        = ";
            out += philf->parent_path;
            out += ";\n    CHILDREN ";
            out += philf->child_role;
            out += " : CLASS ";
            out += philf->child_class;
            out += "\n        = {";

            pchild = philf->children;
            while(pchild) {
                    out += pchild->child_path;
                    pchild = pchild->next;
                    if(pchild)
                            out += ",";
            }
            out += "};\n";
            out += " END_UNLINK;\n\n";

            philf = philf->next;

    }

    out += "\n/*\n* Zu erstellende Links :\n";
    out += "* ----------------------\n*/\n\n";

    philf = newpar->Links;
    while(philf) {
    
            out += " LINK\n    OF_ASSOCIATION  ";
            
            out += philf->asso_ident;
            out += "\n    PARENT  ";
            out += philf->parent_role;
            out += " : CLASS ";
            out += philf->parent_class;
            out += "\n        = ";
            out += philf->parent_path;
            out += ";\n    CHILDREN ";
            out += philf->child_role;
            out += " : CLASS ";
            out += philf->child_class;
            out += "\n        = {";
            
            pchild = philf->children;
            while(pchild) {
                    out += pchild->child_path;
                    pchild = pchild->next;
                    if(pchild)
                            out += ",";
            }
            out += "};\n";

            philf = philf->next;
            out += " END_LINK;\n\n";
    }

    return KS_ERR_OK;
}


/*****************************************************************************/
void compare_libraries(Dienst_param* newpar, Dienst_param* oldpar)
/*****************************************************************************/
{
    DelInstItems*        pol = oldpar->NewLibs;  // Zeiger auf alle Bibliotheken in alter Database
    DelInstItems*        pnl = newpar->NewLibs;  // Zeiger auf alle Bibliotheken in neuer Database
    DelInstItems*        ph;                     // Hilfszeiger

    while(pol) {
        while(pol && pnl) {
            
            if( !strcmp(pol->Inst_name, pnl->Inst_name) ) {
                /* Bibliothek in alter DB gibt es auch in neuen */
                        /* Loesche Lib-Strukturen : */
                        /* Old-DB */
                        ph = oldpar->NewLibs;
                        if(ph != pol) {
                            /* pol zeigt nicht auf erste Lib in Structur */
                        
                                while(ph->next != pol) {
                                        ph = ph->next;
                                }
                                ph->next = pol->next;
                                
                                free(pol);
                                pol = ph->next;
                            
                        } else {
                            /* gleich die erste Lib in old DB gibt's auch in new DB */
                        
                                oldpar->NewLibs = pol->next;
                                
                                free(pol);
                                pol = oldpar->NewLibs;
                        }
                        
                        /* new DB : */
                        ph = newpar->NewLibs;
                        if(ph != pnl) {
                            /* pnl zeigt nicht auf erste Lib in Structur */
                        
                                while(ph->next != pnl) {
                                        ph = ph->next;
                                }
                                ph->next = pnl->next;
                                
                                free(pnl);
                                pnl = ph->next;
                            
                        } else {
                            /* gleich die erste Lib in new DB gibt's auch in old DB */
                        
                                newpar->NewLibs = pnl->next;
                                
                                free(pnl);
                                pnl = newpar->NewLibs;
                            }
                } else {
                    /* Durchsuche neue Structur */
                        pnl = pnl->next;
                }
            } /* while pnl */

            if( (!pnl) && pol) {
                // Wir sind am Ende der Striktur der neuen DB.
                // Pruefe naechste Lib in Old-DB
                    pol = pol->next;
            }
        
            pnl = newpar->NewLibs;

    } /* while pol */

    return;

} /* compare_libraries() */


/*****************************************************************************/
KS_RESULT compare_any_inst(Dienst_param* newpar, Dienst_param* oldpar,
                     Dienst_param* upd,    PltString& out)
/*****************************************************************************/
{
    InstanceItems*    pinst = oldpar->Instance;
    InstanceItems*    ph = newpar->Instance;
    InstanceItems*    help;
    KS_RESULT         fehler;
  
    while(pinst && ph) {
    
            while(pinst && ph) {
                // Namen gleich ?
                      if( !(strcmp(pinst->Inst_name, ph->Inst_name)) ) {
                          // Klass gleich?
                            if( !(strcmp(pinst->Class_name, ph->Class_name)) ) {
                            
                                // Name und Klass der Instanzen sind gleich
                                // Puefe, ob das eine Verbindung ist
                                if( !strcmp(pinst->Class_name, CONNECTION_CLASS_PATH) ) {
                                    // Es ist eine Verbindung. Vergleiche Variablen
                                    Variables* pold = pinst->Inst_var;
                                    Variables* pnew = ph->Inst_var;
                                    while(pold && pnew) {
                                        if(!strcmp(pold->var_name, pnew->var_name) ) {
                                        // Parameter mit dem gleichen Name gefunden.
                                            if( value_compare(pold, pnew) ) {
                                                // Werte eines Parameter-Ports unterscheiden sich
                                                break;
                                            }
                                        } else {
                                            pnew = pnew->next;
                                            continue;
                                        }
                                        pold = pold->next;
                                        pnew = ph->Inst_var;
                                    }
                                    if(pold) {
                                        // Vergleich wurde mit dem Fehler abgebrochen
                                        // Alte Verbindung und ihre Links loeschen, neue anlegen
                                        removeComConnLinks(oldpar, pinst);
                                        pinst = pinst->next;
                                        ph = newpar->Instance;
                                        // Naechsten Instanzen vergleichen
                                        continue;
                                    }
                                    // Werte der Variablen sind gleich. Pruefe ob Verbindungen
                                    // auch die gleichen Links haben
                                    if(compareComConnLinks( newpar, oldpar, pinst->Inst_name) ) {
                                        // Die Verbindungen haben unterschiedliche Links.
                                        // Alte Verbindung und ihre Links loeschen, neue anlegen
                                        removeComConnLinks(oldpar, pinst);
                                        pinst = pinst->next;
                                        ph = newpar->Instance;
                                        // Naechsten Instanzen vergleichen
                                        continue;
                                    }
                                }
                                
                                    fehler = param_compare(pinst, ph, out);
                                    if(fehler == KS_ERR_TYPEMISMATCH) {
                                            return fehler;
                                    }
                                
                                    /* Alte Instanz-Information wird in diesem Fall (Instanz aus der alten 
                                    *  Datenbasis gibt es auch in neuer) geloescht, unabhaengig davon, ob die 
                                    *  neue fuer updaten bleibt ( fehler==1 : Parameter sind unterschiedlich) 
                                    *  oder nicht.
                                    */
                    // Falls es eine Verbindung ist, loesche auch ihre Links-Structuren
                                        if( !strcmp(pinst->Class_name, CONNECTION_CLASS_PATH) ) {
                                            removeComConnLinks(oldpar,pinst);
                                        }
                                    if(oldpar->Instance != pinst) {
                                            help = oldpar->Instance;
                                            while(help->next != pinst) {
                                                    help = help->next;
                                            }
                                            help->next = pinst->next;
                                            
                                            free(pinst);
                                            pinst = help->next;
                                    } else {
                                            oldpar->Instance = pinst->next;
                                            free(pinst);
                                            pinst = oldpar->Instance;
                                    }

                                    /* Trenne aktuelle Instanz von gesammten Struktur und
                                     loesche, wenn Parameter gleich sind (fehler==0),
                                         oder schiebe in update-Baum                   */

                                    if(newpar->Instance != ph) {
                                            help = newpar->Instance;
                                            while(help->next != ph) {
                                                    help=help->next;
                                            }
                                            help->next = ph->next;
                                            ph->next = 0;
                                    } else {
                                            newpar->Instance = ph->next;
                                            ph->next = 0;
                                    }

                                    if(!fehler) {
                                        /* alle Parameter sind gleich. Struktur loeschen */
                                        // Falls es eine Verbindung ist, loesche auch ihre Links-Structuren
                                        if( !strcmp(ph->Class_name, CONNECTION_CLASS_PATH) ) {
                                                removeComConnLinks(newpar,ph);
                                            }
                                            free(ph);
                                            ph = newpar->Instance;
                                    } else {
                                        // Instanze zur Update-Structur hinzufuegen
                                            if(upd->Instance) {
                                                    help = upd->Instance;
                                                    while(help->next) {
                                                            help = help->next;
                                                    }
                                                    help->next = ph;
                                            } else {
                                                    upd->Instance = ph;
                                            }
                                            ph = newpar->Instance;
                                    }
                            } else {
                                // Unterschiedlichen Klassen. Naechste Instanzen
                                    pinst = pinst->next;
                                    ph = newpar->Instance;
                            }
                      } else {
                          // Unterschiedlichen Namen. Weiter suchen
                            ph = ph->next;
                    }
            }
        if(!ph && pinst) {
            // Instanz in Old-DB ist in neuer nicht vorhanden.
                pinst = pinst->next;
        }
        ph = newpar->Instance;

    } /* while pinst && ph */

    return KS_ERR_OK;

} /* get_any_inst */

/*****************************************************************************/
int compareComConnLinks( Dienst_param *Newpar
                        ,Dienst_param *Oldpar
                        ,const char   *VerbName) {
/*****************************************************************************/
    PltString   Name;
    PltString   Role;
    LinksItems *oldlink;
    LinksItems *newlink;
    int         i;      // Laufvariable
    
    Name = VerbName;    // Name der Verbindung merken
    Role = "inputcon";  // Association-Ident merken
    
    for(i = 0; i < 2; i++) {
        oldlink = Oldpar->Links;
        newlink = Newpar->Links;
        
        while(oldlink) {
            if( (Name == oldlink->children->child_path) &&
                (Role == oldlink->child_role) ) {
                // Link gefunden
                break;
            }
            oldlink = oldlink->next;
        }
        if(!oldlink) {
            // Link nicht gefunden ?
            return 1;
        }
 
        while(newlink) {
            if( (Name == newlink->children->child_path) &&
                (Role == newlink->child_role) ) {
                // Link gefunden
                break;
            }
            newlink = newlink->next;
        }
        if(!newlink) {
            // Link nicht gefunden ?
            return 1;
        }
        
        // Beide Links gefunden. Vergleiche FB-Instanzname
        if( strcmp(oldlink->parent_path, newlink->parent_path) ) {
            // Verbindungen haben unterschiedliche Links
            return 1;
        }
        Role = "outputcon";
    }

    return 0;
}

/*****************************************************************************/
void removeComConnLinks(Dienst_param *param, InstanceItems* ComConn) {
/*****************************************************************************/
    LinksItems *link = param->Links;
    LinksItems *help;
    PltString   Name;
    
    // Name der Verbindung merken
    Name = ComConn->Inst_name;

    while(link) {
        if(Name == link->children->child_path) {
            // Link gefunden
            help = param->Links;
            if(help == link) {
                // Es ist erste link in der Struktur
                param->Links = link->next;
            } else {
                // Link suchen
                while(help->next != link) {
                    help = help->next;
                }
                help->next = link->next;
            }
            free(link->children);
            free(link);
            link = param->Links;
            continue;
        }
        link = link->next;
    }
    
    return;
}

/*****************************************************************************/
int param_compare(InstanceItems* poi, InstanceItems* pni, PltString& out)
/*****************************************************************************/
{
    Variables* pold;  /* Zeiger auf die Variable der in der alten Datenbasis
                                       vorhandenen Instanz */
    Variables* pnew;  /* Zeiger auf die Variable der in der aktuellen Datenbasis
                                       vorhandenen Instanz */
                                       
    Variables*    hilfsvar;
    VariableItem* pval;               /* Hilfsvariable fuer loeschen Value-Information */
    PltString     err;
  
    int error = 0;
    int UpdFlag = 0;

//    char              ph[64];
  
    pold = poi->Inst_var; 
    pnew = pni->Inst_var;
    
    // Gibt es Variablen zum Vergleichen ?
    if( (!pold) && (!pnew) ) {
        // Keine Variablen in Instanzen
        return 0;
    }
    // Haben beide Instanzen Variablen ?
    if( ((!pold) &&   pnew) ||
        (  pold  && (!pnew)) ) {
        // Eine Instanze hat Variablen, die andere nicht
            out += log_getErrMsg(KS_ERR_OK, "Abweichender Typ :");
            out += log_getErrMsg(KS_ERR_OK, "    Klass    ", pni->Class_name);
            out += log_getErrMsg(KS_ERR_OK, "    Instanz  ", pni->Inst_name);

            err = "\"%s %s\"  \"";
            err += pni->Class_name;
            err += "\"  \"";
            err += pni->Inst_name;
            err += "\"";
            error = KS_ERR_TYPEMISMATCH;
            iFBS_SetLastError(1, error, err);
    
        return KS_ERR_TYPEMISMATCH;
    }
    
    pold = poi->Inst_var; 
    pnew = pni->Inst_var;

    /* Pruefe ob die Variablen in neuer auch in alter Struktur vorhanden sind : */
    while(pold && pnew) {
            /* suche Variable mit dem gleichen Name */
            while( strcmp(pold->var_name, pnew->var_name) ) {
                    pold = pold->next;
                    if(!pold) {
                        out += log_getErrMsg(KS_ERR_OK, "Abweichender Typ :");
                        out += log_getErrMsg(KS_ERR_OK, "    Klass    ", pni->Class_name);
                        out += log_getErrMsg(KS_ERR_OK, "    Instanz  ", pni->Inst_name);
                        out += log_getErrMsg(KS_ERR_OK, "    Variable ", pnew->var_name);
                    
                        err = "\"%s %s %s\"  \"";
                        err += pni->Class_name;
                        err += "\"  \"";
                        err += pni->Inst_name;
                        err += "\"  \"";
                        err += pnew->var_name;
                        err += "\"";
                        error = KS_ERR_TYPEMISMATCH;
                        iFBS_SetLastError(1, error, err);
                    
                        return KS_ERR_TYPEMISMATCH;
                    }
            }

            pnew = pnew->next;
            pold = poi->Inst_var;
    }
    
    pold = poi->Inst_var;
    pnew = pni->Inst_var;
    
    /* Variable der Instanz in neuer DB sind auch in alten vorhanden.
    *  Pruefe ob auch die Variablen der Instanz in alten DB auch in neuen
    *  Struktur vorhanden sind : */
    while(pold && pnew) {
            /* suche Variable mit dem gleichen Name */
            while( strcmp(pold->var_name, pnew->var_name) ) {
                    pnew = pnew->next;
                    if(!pnew) {
                        out += log_getErrMsg(KS_ERR_OK, "Abweichender Typ :");
                        out += log_getErrMsg(KS_ERR_OK, "    Klass    ", poi->Class_name);
                        out += log_getErrMsg(KS_ERR_OK, "    Instanz  ", poi->Inst_name);
                        out += log_getErrMsg(KS_ERR_OK, "    Variable ", pold->var_name);
                    
                        err = "\"%s %s %s\"  \"";
                        err += poi->Class_name;
                        err += "\"  \"";
                        err += poi->Inst_name;
                        err += "\"  \"";
                        err += pold->var_name;
                        err += "\"";
                        error = KS_ERR_TYPEMISMATCH;
                        iFBS_SetLastError(1, error, err);
                    
                        return KS_ERR_TYPEMISMATCH;
                    }
            }

            pold = pold->next;
            pnew = pni->Inst_var;
    }
    
  
    // Value-Vergleich
    
    pold = poi->Inst_var;
    pnew = pni->Inst_var;

    while(pold && pnew) {
    
            while( strcmp(pold->var_name, pnew->var_name) ) {
                
                /* suche Variable mit dem gleichen Name */
                    pnew = pnew->next;
            }

        if(pold->port_typ != pnew->port_typ) {
            
                out += log_getErrMsg(KS_ERR_OK, "Abweichender Port-Typ :");
                out += log_getErrMsg(KS_ERR_OK, "    Instanz  ", poi->Inst_name);
                out += log_getErrMsg(KS_ERR_OK, "    Variable ", pold->var_name);
            
                err = "\"%s %s\"  \"";
                err += poi->Inst_name;
                err += "\"  \"";
                err += pold->var_name;
                err += "\"";
            
                error = OV_ERR_VARDEFMISMATCH;
                iFBS_SetLastError(1, error, err);
            
                    return KS_ERR_TYPEMISMATCH;
        }

            if(pold->port_typ != PARAM_PORT) {
            
                      /* Massgebend fuer die Versionsvergleich sind nur Parameter-Ports.
                         Loesche Information zu Input-Ports. */

                      poi->Inst_var = pold->next;

                    while(pold->value) {
                            pval = pold->value;
                            pold->value = pval->next;
                            free(pval);
                    }
                    free(pold);

                    if(pnew != pni->Inst_var) {
                            hilfsvar = pni->Inst_var;
                            while(hilfsvar->next != pnew) {
                                    hilfsvar = hilfsvar->next;
                            }
                            hilfsvar->next = pnew->next;
                    } else {
                            pni->Inst_var = pnew->next;
                    }
                    
                    pnew->next = 0;

                    while(pnew->value) {
                            pval = pnew->value;
                            pnew->value = pval->next;
                            free(pval);
                    }
                    free(pnew);

                    pold = poi->Inst_var;
                    pnew = pni->Inst_var;

            // Naechste Variable vergleichen
                    continue;
                
            } /* if !PARAM_PORT */

            error = value_compare(pold, pnew);
            if(error == KS_ERR_TYPEMISMATCH) {
                out += log_getErrMsg(KS_ERR_OK, "Abweichender Variablentyp :");
                out += log_getErrMsg(KS_ERR_OK, "    Instanz  ", poi->Inst_name);
                out += log_getErrMsg(KS_ERR_OK, "    Variable ", pold->var_name);
            
                err = "\"%s %s\"  \"";
                err += poi->Inst_name;
                err += "\"  \"";
                err += pold->var_name;
                err += "\"";
            
                error = OV_ERR_VARDEFMISMATCH;
                iFBS_SetLastError(1, error, err);
            
                    return KS_ERR_TYPEMISMATCH;
            }

        /* Die Variable-Information der alten Instanz wird in jedem Fall
            *  geloescht, unabhaengig davon, ob die neue fuer updaten bleibt
            *  (error==1) oder nicht.
        */
            poi->Inst_var = pold->next;

            while(pold->value) {
                    pval = pold->value;
                    pold->value = pval->next;
                    free(pval);
            }

            free(pold);
            pold = poi->Inst_var;

            if(!error) {
                    /* Werte der Variablen sind gleich, loesche neue Variable */

                    if(pnew != pni->Inst_var) {
                            hilfsvar = pni->Inst_var;
                            while(hilfsvar->next != pnew) {
                                    hilfsvar = hilfsvar->next;
                            }
                            hilfsvar->next = pnew->next;
                    } else {
                            pni->Inst_var = pnew->next;
                    }
                    
                    while(pnew->value) {
                            pval = pnew->value;
                            pnew->value = pval->next;
                            free(pval);
                    }
                    free(pnew);
                
            } else {
                // Merke : Instanz-Variable zu updaten
                    UpdFlag = 1;
            }

            pnew = pni->Inst_var;

    } /* while pold && pnew */

    return UpdFlag;
}

/*****************************************************************************/
int value_compare(Variables* pold, Variables* pnew)
/*****************************************************************************/
{

    VariableItem* ov = pold->value;
    VariableItem* nv = pnew->value;

    // Haben beide Variablen Value ?
    if( (!ov) && (!nv) ) {
        // Keine Values zum Vergleichen
        return 0;
    }
    if( ((!ov) &&   nv) ||
        (  ov  && (!nv)) ) {
        // Eine Variable hat Value, andere nicht.
        return 1;
    }
    if(pold->state != pnew->state) {
        // Status der Variablen unterschiedlich
        return 1;
    }
    
    while( ov && nv) {
    
            if(ov->value_type != nv->value_type) {
                    /* Variablen-Typen sind unterschiedlich. Das aendern eines Parameters
                    *  bedeutet abweichung von Typ-Beschreibung, bzw. eine neue Typ-Classe,
                    *  also nicht updatbar.
                    *  ==> Fehlerausgabe, Programm beenden.                              */

                    return KS_ERR_TYPEMISMATCH;
            }
            switch(ov->value_type) {
                
                    case KS_VT_UINT             :  if(((unsigned long)atol(ov->val)) !=
                                               ((unsigned long)atol(nv->val)) ) {
                                                return 1;
                                            }
                                            break;
                    case KS_VT_INT             :        if(((long)atol(ov->val)) !=
                                               ((long)atol(nv->val)) ) {
                                                return 1;
                                            }
                                            break;
                    case KS_VT_SINGLE         :
                    case KS_VT_TIME_SPAN :
                    case KS_VT_DOUBLE         :        if( atof(ov->val) !=
                                                atof(nv->val) ) {
                                                return 1;
                                            }
                                            break;
                    case KS_VT_TIME            :
                    case KS_VT_BOOL            :
                    case KS_VT_STRING        :        if(strcmp(ov->val,nv->val)) {
                                                                            return 1;
                                            }
                                                                    break;

                    default                                :        return 1;
                    
             } /* case */
             
            ov = ov->next;
            nv = nv->next;

    } /* while ov && nv */

    if(ov || nv) {
        /* Die Variablen haben unterschidliche laenge */
            return 1;
    }

    return 0;

} /* value_compare() */

/*****************************************************************************/
void put_variable(Variables* hpv, PltString& Out)
/*****************************************************************************/
{
    Variables* pv = hpv; /* Hilfszeiger ueber die Variablenliste */

    VariableItem* pvi;
    char   help[64];

    while(pv) {
    
            Out += "        ";
            Out += pv->var_name;
    
            if(pv->vector) {
                    sprintf(help," [%d]", pv->len);
                    Out += help;
            }
    
        if(pv->port_typ == PARAM_PORT) {
                      Out += " : PARAMETER ";
              } else {
                      Out += " : INPUT ";
              }
                        
            Out += ifb_getOvValueType(pv->var_typ);
            Out += " = ";
        
            if(pv->vector) {
                Out += "{";
            }
                pvi = pv->value;
                
                while(pvi) {
                    switch(pv->var_typ) {
                        case KS_VT_STRING:
                                            Out += "\"";
                                                Out += pvi->val;
                                                Out += "\"";
                                                break;
                            default:
                                                Out += pvi->val;
                                                break;
                    }
                        pvi = pvi->next;
                        if(pvi) {
                            Out += " , ";
                        }
                }
            if(pv->vector) {
                Out += "}";
            }
        if(pv->state != 0) {
                    sprintf(help, " STATE = %d", pv->state);
                    Out += help;
                }
                Out += ";\n";

            pv = pv->next;
            
    } /* while pv */

    return;

} /* put_variable() */


/*****************************************************************************/
void get_any_links(Dienst_param* newpar, Dienst_param* oldpar)
/*****************************************************************************/
{
    LinksItems* newlinks = newpar->Links;
    LinksItems* oldlinks = oldpar->Links;
    LinksItems* helplinks;

    Child*                oldchild;
    Child*                newchild;

    int                    error;

    while(oldlinks && newlinks) {
    
            while(oldlinks && newlinks) {
                error = 0;
                
                    if( !strcmp(oldlinks->parent_path, newlinks->parent_path) ) {
                    
                            if( !strcmp(oldlinks->child_role, newlinks->child_role) ) {
                            
                                    oldchild = oldlinks->children;
                                    newchild = newlinks->children;

                                    while(oldchild && newchild) {
                                    
                                            if( strcmp(oldchild->child_path,newchild->child_path) ) {
                                                    error = 1;
                                                    break;
                                            }
                                            oldchild = oldchild->next;
                                            newchild = newchild->next;
                                    }

                                    if( (!oldchild) && (!newchild) && (!error) ) {
                                        /* == wenn beide gleichzeitig auf null zeigen
                                            (dann ist Anzahl der Kinder auch gleich)
                                            Loesche gleiche Child-Informationen      */
                                        
                                            if(oldlinks == oldpar->Links) {
                                                    oldpar->Links = oldlinks->next;
                                                    oldlinks->next = 0;
                                                    helplinks = oldpar->Links;
                                            } else {
                                                    helplinks = oldpar->Links;
                                                    while(helplinks->next != oldlinks)
                                                            helplinks = helplinks->next;

                                                    helplinks->next = oldlinks->next;
                                                    oldlinks->next = 0;
                                                    helplinks = helplinks->next;
                                            }

                                            while(oldlinks->children) {
                                                    oldchild = oldlinks->children;
                                                    oldlinks->children = oldchild->next;
                                                    free(oldchild);
                                            }

                                            free(oldlinks);
                                            oldlinks = helplinks;


                                            if(newlinks == newpar->Links) {
                                                    newpar->Links = newlinks->next;
                                                    newlinks->next = 0;
                                                    helplinks = newpar->Links;
                                            } else {
                                                    helplinks = newpar->Links;
                                                    while(helplinks->next != newlinks)
                                                            helplinks = helplinks->next;
                                                    helplinks->next = newlinks->next;
                                                    newlinks->next = 0;
                                                    helplinks = helplinks->next;
                                            }

                                            while(newlinks->children) {
                                                    newchild = newlinks->children;
                                                    newlinks->children = newchild->next;
                                                    free(newchild);
                                            }

                                            free(newlinks);
                                            newlinks = helplinks;

                                    } else { /* Fehler beim Kinder-Vergleich */
                                            oldlinks = oldlinks->next;
                                            newlinks = newpar->Links;
                                    }
                            } else {
                                /* unterschiedliche Assoziation. Weiter suchen */
                                    newlinks = newlinks->next;
                            }
                    } else {
                        /*  parent_path nicht gleich */
                            newlinks = newlinks->next;
                    }
            } /* while oldlinks && newlinks innen */

            if(oldlinks && (!newlinks) ) {
                    oldlinks = oldlinks->next;
                    newlinks = newpar->Links;
            }

    } /* while oldlinks && newlinks aussen */

    return;

} /* get_any_links() */
