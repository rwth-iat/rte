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
void write_cons_link( PltList<ConData> &List,
                      PltList<PltString> &NameList, PltString& out, FILE *fout) {
/******************************************************************************/

 while( List.size() ) {
    ConData item = List.removeFirst();

    out += " LINK\n    OF_ASSOCIATION  outputconnections;\n    PARENT  sourcefb : CLASS functionblock\n        = ";
    out += item.source_fb;
    out += ";\n    CHILDREN  outputcon : CLASS connection\n        = {";
    out += item.identifier;
    out += "};\n END_LINK;\n";

    if(IsNotInList(item.source_fb, NameList) ) {
        out += " /*\n WARNING ! Instanz ";
        out += item.source_fb;
        out += " nicht gesichert!\n */\n";
    }

    out += "\n LINK\n    OF_ASSOCIATION  inputconnections;\n    PARENT  targetfb : CLASS functionblock\n        = ";
    out += item.target_fb;
    out += ";\n    CHILDREN  inputcon : CLASS connection\n        = {";
    out += item.identifier;
    out +="};\n END_LINK;\n";

    if(IsNotInList(item.target_fb, NameList) ) {
        out += " /*\n WARNING ! Instanz ";
        out += item.target_fb;
        out += " nicht gesichert!\n */\n";
    }
    out += "\n";

    // Schreiben in Datei ?
    if(fout) {
        fputs((const char*)out, fout);
        // String-Buffer leeren
        out = "";
    }

 }
}



/******************************************************************************/
KS_RESULT write_cons_from(KscServerBase* Server, PltList<PltString> &NameList,
                          PltList<PltString> &HasConList, PltString& out, FILE *fout) {
/******************************************************************************/
 FbDirParams        Pars;
 PltList<ConData>   Res;
 PltString          Str;
 PltList<PltString> hList;
 PltList<ConData>   ConList;
 KS_RESULT          err;

 while(HasConList.size() ) {
    Str = HasConList.removeFirst();
    Pars.path = Str;
    Pars.name_mask = "*";
    err = IFBS_GETCONS(Server, Pars, Res);
    if(err) {return err;}
    while(Res.size() ) {
        ConData Con = Res.removeFirst();
        if(IsNotInList(Con.identifier, hList) ) {
            AddValueToList(Con.identifier, hList);
            ConList.addLast(Con);
        }
    }
 }

 while(ConList.size() ) {
    ConData CC = ConList.removeFirst();
    out += " INSTANCE  ";
    out += CC.identifier;
    out += " :\n    CLASS /fb/connection;\n";

    out += "    VARIABLE_VALUES\n";
    out += "        on         : PARAMETER  BOOL   = ";
    if(CC.on)
        out += "TRUE;\n";
    else
        out += "FALSE;\n";

    out += "        sourcetrig : PARAMETER  BOOL   = ";
    if(CC.source_trig)
        out += "TRUE;\n";
    else
        out += "FALSE;\n";

    out += "        sourceport : PARAMETER  STRING = \"";
    out += CC.source_port;
    out += "\";\n        targetport : PARAMETER  STRING = \"";
    out += CC.target_port;
    out += "\";\n    END_VARIABLE_VALUES;\n";
    out += " END_INSTANCE;\n\n";
    Res.addLast(CC);

    // Schreiben in Datei ?
    if(fout) {
        fputs((const char*)out, fout);
        // String-Buffer leeren
        out = "";
    }

 }

 write_cons_link(Res, NameList, out, fout);

 return KS_ERR_OK;
}



/******************************************************************************/
void write_task_link( PltList<ObjProps> &HasTaskList,
                      PltList<PltString> &NameList, PltString& out, FILE *fout) {
/******************************************************************************/

 while( HasTaskList.size() ) {
    ObjProps item = HasTaskList.removeFirst();

    out += " LINK\n    OF_ASSOCIATION  tasklist;\n    PARENT  taskparent : CLASS task\n        = ";
    out += item.identifier;
    out += ";\n    CHILDREN  taskchild : CLASS task\n        = {";
    out += item.value;
    out += "};\n END_LINK;\n";

    if(IsNotInList(item.identifier, NameList) ) {
        out += " /*\n WARNING ! Instanz ";
        out += item.identifier;
        out += " ist nicht gesichert!\n */\n";
    }
    if(IsNotInList(item.value, NameList) ) {
        out += " /*\n WARNING ! Instanz ";
        out += item.value;
        out += " ist nicht gesichert!\n */\n";
    }
    out += "\n";

    // Schreiben in Datei ?
    if(fout) {
        fputs((const char*)out, fout);
        // String-Buffer leeren
        out = "";
    }

 }
}


/******************************************************************************/
KS_RESULT write_inst_from_class(KscServerBase* Server, FbDirParams& param,
                          PltString& out, int& Recurs, PltList<PltString>  &HasConList,
                          PltList<PltString>  &LibList, PltList<PltString> &NameList,
                          PltList<ObjProps>  &HasTaskList, FILE *fout) {
/******************************************************************************/

    KsString    	     root = Server->getHostAndName();
    KsString    	     Var;
    size_t             size,i;
    PltString          Str;
    PltList<PltString> hList;
    char               help[256];
    char*              ph;

    Var = param.path;
    Var += ".instance";

    KscVariable var(root+Var);

    if (!var.getUpdate() ) {
        out += log_getErrMsg(KS_ERR_OK, "Bad path" );
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
                Str = ((KsStringVecValue &) *(cp->value))[i];
                hList.addLast(Str);
            }
            break;
        default :
//cout << "\n !!! Typ der Variable ist nicht KS_VT_STRING_VEC !!!\n\n";
            break;
    }

    while(hList.size() ) {
        strcpy(help, (const char*)hList.removeFirst() );
        ph = help;
        while( (*ph) ) ph++;
        while( (ph != help) && ((*ph) != '/') ) ph--;
        if(ph != help) {
            *ph = '\0';
            ph++;
        } else {
            return KS_ERR_BADPATH;
        }
        param.path = help;
        param.name_mask = ph;
        param.type_mask = FB_OBJ_INSTANCE;

        KS_RESULT err = write_inst_data(Server,param,out,Recurs,HasConList,LibList,NameList,HasTaskList,fout);
        if(err) {
            out += " ERR : ";
            out += GetErrorCode(err);
            out += "\n";
            return err;
        }

    }

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
 out += "    type_mask = FB_OBJ_TASK;        // Oder FB_OBJ_ANY, da im Tasks-Container nur die Tasks stehen\n";
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
KS_RESULT write_inst_data(KscServerBase* Server, FbDirParams& Pars,
                          PltString& out, int& Recurs, PltList<PltString>  &HasConList,
                          PltList<PltString>  &LibList, PltList<PltString> &NameList,
                          PltList<ObjProps>  &HasTaskList, FILE *fout) {
/******************************************************************************/

    PltList<ObjProps>  res;
    PltList<PltString>	Liste;
    FbDirParams        hpar;
    char               help[256];
    char*              ph;
    char*              pc;
    PltString          Str;

if(fout) {
    sprintf(help, "\nwid(%s) : %ld\n", (const char*)Pars.path, (long)Pars.type_mask);
    fputs(help, fout);
}

    KS_RESULT err = IFBS_DIR(Server,Pars,res);
    if(err)
        return err;

    while(res.size() ) {
        ObjProps item = res.removeFirst();
        switch(item.obj_type) {
            case  FB_OBJ_HISTORY:
                                out += " INSTANCE  ";
                                out += Pars.path;
                                if(item.access & KS_AC_PART) {
                                    out += ".";
                                } else {
                                    out += "/";
                                }
                                out += item.identifier;
                                out += " :\n    CLASS ";
                                out += item.class_ident;
                                out += ";\n END_INSTANCE;\n\n";
                                
                                strcpy(help, (const char*)item.class_ident);
                                ph = strstr(help, FB_LIBRARIES_CONTAINER);
                                if(ph) {
                                    if(strlen(help) > (strlen(FB_LIBRARIES_CONTAINER) + 2) ) {
                                        pc = help;
                                        pc += (strlen(FB_LIBRARIES_CONTAINER) + 2);
                                        ph = pc;
                                        while( ph && ((*ph) != '/') && (*ph)) ph++;
                                        if(ph && (*ph)) {
                                            *ph = '\0';
                                            Str = pc;
                                            AddValueToList(Str, LibList);
                                        }
                                    }
                                }
                                
                                
                                break;
	        case  FB_OBJ_CONTAINER :
                                out += " INSTANCE  ";
                                out += Pars.path;
                                if(item.access & KS_AC_PART) {
                                    out += ".";
                                } else {
                                    out += "/";
                                }
                                out += item.identifier;
                                out += " :\n    CLASS ";
                                out += item.class_ident;
                                out += ";\n END_INSTANCE;\n\n";
                                
                                // Schreiben in Datei ?
                                if(fout) {
                                    fputs((const char*)out, fout);
                                    // String-Buffer leeren
                                    out = "";
                                }

                                if(Recurs) {
                                    hpar.path  = Pars.path;
                                    if(item.access & KS_AC_PART) {
                                        hpar.path += ".";
                                    } else {
                                        hpar.path += "/";
                                    }
                                    hpar.path += item.identifier;
                                    hpar.type_mask  = Pars.type_mask;
                                    hpar.name_mask  = "*";

                                    err = write_inst_data(Server,hpar,out,Recurs,HasConList,LibList,NameList,HasTaskList,fout);
                                    if(err)
                                        return err;
                                }
                                break;
	        case  FB_OBJ_INSTANCE  :
                                strcpy(help, (const char*)item.class_ident);
                                ph = strstr(help, FB_LIBRARIES_CONTAINER);
                                if(ph) {
                                    if(strlen(help) > (strlen(FB_LIBRARIES_CONTAINER) + 2) ) {
                                        pc = help;
                                        pc += (strlen(FB_LIBRARIES_CONTAINER) + 2);
                                        ph = pc;
                                        while( ph && ((*ph) != '/') && (*ph)) ph++;
                                        if(ph && (*ph)) {
                                            *ph = '\0';
                                            Str = pc;
                                            AddValueToList(Str, LibList);
                                        }
                                    }
                                }
	        case  FB_OBJ_TASK      :
	        case  FB_OBJ_CONNECTION :
                                Str = Pars.path;
                                if(item.access & KS_AC_PART) {
                                    Str += ".";
                                } else {
                                    Str += "/";
                                }
                                Str += item.identifier;
                                if(Str == "/Tasks/UrTask") break;
                                AddValueToList(Str, NameList);

                                out += " INSTANCE  ";
                                out += Str;
                                out += " :\n    CLASS ";
                                out += item.class_ident;
                                out += ";\n    VARIABLE_VALUES\n";

                                hpar.path  = Pars.path;
                                if(item.access & KS_AC_PART) {
                                    hpar.path += ".";
                                } else {
                                    hpar.path += "/";
                                }
                                hpar.path += item.identifier;
                                hpar.type_mask  = FB_OBJ_VARIABLE;
                                hpar.name_mask  = "*";

                                //err = write_inst_data(Server,hpar,out,Recurs,HasConList,LibList,NameList,HasTaskList, fout);
                                err = write_inst_data(Server,hpar,out,Recurs,HasConList,LibList,NameList,HasTaskList, 0);
                                if(err)
                                    return err;

                                out += "    END_VARIABLE_VALUES;\n END_INSTANCE;\n\n";
                                
                                if(Recurs) {
                                    hpar.path  = Pars.path;
                                    if(item.access & KS_AC_PART) {
                                        hpar.path += ".";
                                    } else {
                                        hpar.path += "/";
                                    }
                                    hpar.path += item.identifier;
                                    hpar.type_mask  = Pars.type_mask;
                                    hpar.name_mask  = "*";

                                    err = write_inst_data(Server,hpar,out,Recurs,HasConList,LibList,NameList,HasTaskList,fout);
                                    if(err)
                                        return err;
                                }
                                break;
	        case  FB_OBJ_VARIABLE  :
                                out += "        ";
                                out += item.identifier;
                            	switch(item.var_type )
                            	   {
                            	    case KS_VT_BYTE_VEC:
                            	    case KS_VT_BOOL_VEC:
                            	    case KS_VT_INT_VEC:
                            	    case KS_VT_UINT_VEC:
                            	    case KS_VT_SINGLE_VEC:
                            	    case KS_VT_DOUBLE_VEC:
                            	    case KS_VT_TIME_VEC:
                            	    case KS_VT_TIME_SPAN_VEC:
                                    case KS_VT_STRING_VEC:
                                         sprintf(help, "[%d]", (unsigned int)item.len);
                                         out += help;
                                         
                                    default : 
                            	         break;
                            	} /* switch */
                            	   
                                if( IsFlagSet( item.flags, 'p') ) {
                                    out += " : PARAMETER ";
                                } else {
                                    if( IsFlagSet( item.flags, 'i') ) {
                                        out += " : INPUT ";
                                    } else {
                                        if( IsFlagSet( item.flags, 'o') ) {
                                            out += " : OUTPUT ";
                                        } else {
                                            out += " : HIDDEN ";
                                        }
                                    }
                                }
                                out += ifb_getOvValueType(item.var_type);
                                out += " = ";
                            	switch(item.var_type )
                            	   {
                            	    case KS_VT_BOOL:
                            	    case KS_VT_INT:
                            	    case KS_VT_UINT:
                            	    case KS_VT_SINGLE:
                            	    case KS_VT_DOUBLE:
                            	    case KS_VT_STRING:
                            	    case KS_VT_TIME:
                            	    case KS_VT_TIME_SPAN:
                                        out += item.value;
                                        out += ";\n";
                            	        break;
                            	    case KS_VT_BYTE_VEC:
                            	    case KS_VT_BOOL_VEC:
                            	    case KS_VT_INT_VEC:
                            	    case KS_VT_UINT_VEC:
                            	    case KS_VT_SINGLE_VEC:
                            	    case KS_VT_DOUBLE_VEC:
                            	    case KS_VT_TIME_VEC:
                            	    case KS_VT_TIME_SPAN_VEC:
                                        out += "{";
                                        err = SeparateString(item.value, item.var_type, Liste);
                                        if(err) {
                                            return err;
                                        }
                                        if( Liste.size() ) {
                                            out += Liste.removeFirst();
                                            while( Liste.size() ) {
                                                out += ",";
                                                out += Liste.removeFirst();
                                            }
                                        }
                                        out += "};\n";
                                        break;
                            	    case KS_VT_STRING_VEC:
                                         out += "{";
                                         err = SeparateString(item.value, KS_VT_STRING_VEC, Liste);
                                         if(err) {
                                            return err;
                                         }
                                         if( Liste.size() ) {
                                            out += "\"";
                                            out += Liste.removeFirst();
                                            out += "\"";
                                            while( Liste.size() ) {
                                                out += ",";
                                                out += "\"";
                                                out += Liste.removeFirst();
                                                out += "\"";
                                            }
                                         }
                                         out += "};\n";
                            	         break;
                            	    default:
                                         out += " UNKNOWN = (";
                                         out += item.value;
                                         out += ");\n";
                                         break;
                            	   } /* switch */

                                break;
	        case  FB_OBJ_LINK      :
                                if( (item.identifier == "taskparent") &&
                                     (item.value).len() && (item.value != "\"\"") ) {
                                    strcpy(help, (const char*)item.value);
                                    pc = help;
                                    pc++; 
                                    ph = pc;
                                    while( (*ph) && ((*ph) != '"') ) ph++;
                                    *ph = '\0';
                                    Str = pc;

                                    item.identifier = Str;
                                    item.value = Pars.path;
                                    HasTaskList.addLast(item);
                                    break;
                                }
                                if( item.identifier == "inputcon" ) {
                                    AddValueToList(Pars.path, HasConList);
                                    break;
                                }
                                if( item.identifier == "outputcon" ) {
                                    AddValueToList(Pars.path, HasConList);
                                }
                                break;
	        case  FB_OBJ_LIBRARY   :
                                hpar.path  = Pars.path;
                                hpar.path += "/";
                                hpar.path += item.identifier;
                                hpar.type_mask  = FB_OBJ_CLASS;
                                hpar.name_mask  = "*";

                                err = write_inst_data(Server,hpar,out,Recurs,HasConList,LibList,NameList,HasTaskList, fout);
                                if(err)
                                    return err;
                                break;
	        case  FB_OBJ_CLASS     :
                                hpar.path  = Pars.path;
                                hpar.path += "/";
                                hpar.path += item.identifier;
                                hpar.name_mask  = "instance";

                                err = write_inst_from_class(Server,hpar,out,Recurs,HasConList,LibList,NameList,HasTaskList, fout);
                                if(err)
                                    return err;

                                break;
            default :
                                break;
        }
        
        // Schreiben in Datei ?
        if(fout) {
            if(out != "") {
                fputs((const char*)out, fout);
                // String-Buffer leeren
                out = "";
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
    PltList<PltString>  LibList;        // Liste aller Bibliotheken
    PltList<PltString>  NameList;       // Liste der gesicherten Instanzen
    PltList<ObjProps>   HasTaskList;    // Liste der Instanzen mit Task-Listen

    // UrTask wird nicht gesichert
    NameList.addLast("/Tasks/UrTask");

    KS_RESULT err = write_inst_data(Server,Pars,out,Recurs,HasConList,LibList,NameList,HasTaskList, fout);
    if(err) {
        out += " ERR : ";
        out += GetErrorCode(err);
        out += "\n";
        return err;
    }

 write_task_link(HasTaskList, NameList, out, fout);

 err = write_cons_from(Server, NameList, HasConList, out, fout);
 if(err) {
    out += " ERR : ";
    out += GetErrorCode(err);
    out += "\n";
    return err;
 }

 while(LibList.size() ) {
    out += " LIBRARY\n    ";
    out += LibList.removeFirst();
    out += "\n END_LIBRARY;\n\n";
 }

 return KS_ERR_OK;
}

/******************************************************************************/
KS_RESULT select_save_eval(KscServerBase   *Server,
                           FbDirParams     &Pars,
                           PltString       &out,
                           FILE*           fout=0)
/******************************************************************************/
{
    char        help[256];
    char*       ph;				 /* Hilfszeiger */
    int         Recurs = 0;
    int         count = 0;
    PltString   log("");
    KS_RESULT   err;
    
    if(!(Pars.path).len() ) {
        write_help_message_from_select_save(out);
        return KS_ERR_BADPATH;
    }

    log = "";
    err = KS_ERR_OK;
    iFBS_SetLastError(1, err, log);

    struct tm* t;
    time_t timer;

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

    FbDirParams  param;
    param.path = "";

    strcpy(help, (const char*)Pars.path);
    ph = help;


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

    param.path += help;
    param.type_mask = Pars.type_mask;
    param.name_mask = Pars.name_mask;


    if( strstr(help, FB_LIBRARIES_CONTAINER ) ){

        param.type_mask = (FB_OBJ_LIBRARY | FB_OBJ_CLASS);

        strcpy(help, (const char*)param.path);
        ph = help;
        while( (*ph)  ) {
            if((*ph) == '/') count++;
            ph++;
        }
        if(count > 2) {
            out += " ERR : Bad path (";
            out += help;
            out += "\n";
            write_help_message_from_select_save(out);
        
            err = KS_ERR_BADPATH;
            log = "\"%s\"  \"";
            log += param.path;
            log += "\"";
            iFBS_SetLastError(1, err, log);
        
            return KS_ERR_BADPATH;
        }

    } else {
        if( !( strstr(help, FB_INSTANZ_CONTAINER) || strstr(help, FB_TASK_CONTAINER) ) ) {
            out += " ERR : Bad path (";
            out += help;
            out += ")\n";
            write_help_message_from_select_save(out);
        
            err = KS_ERR_BADPATH;
            log = "\"%s\"  \"";
            log += help;
            log += "\"";
            iFBS_SetLastError(1, err, log);

            return KS_ERR_BADPATH;
        }
    }

    return write_db_data(Server, param, out, Recurs, fout);
}

/******************************************************************************/
KS_RESULT select_save(KscServerBase* 	Server,
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


