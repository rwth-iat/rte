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
*   ifb_importeval.cpp                                                       *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   2000-05-10  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*                                                                            *
*                                                                            *
*****************************************************************************/

#include "ifbslibdef.h"

/******************************************************************************/
int IsNotInList(PltString& Value, PltList<PltString> &Liste) {
/******************************************************************************/
    PltString  hStr("");

    PltList<PltString> hList;

    if( Liste.isEmpty() ) {
        return 1;
    }

    while( !Liste.isEmpty() ) {
        hStr = Liste.removeFirst();
        hList.addLast(hStr);
        if(hStr == Value) {
            while( !hList.isEmpty() ) {
                Liste.addFirst(hList.removeLast() );
            }
            return 0;
        }
    }

    while( !hList.isEmpty() ) {
        Liste.addFirst(hList.removeLast() );
    }

    return 1;
}

/*****************************************************************************/
KS_RESULT Check_Vars_in_lib(KscServerBase* Server,
                            InstanceItems* pinst,
                            PltString&     out  )    {
/*****************************************************************************/


  KsGetEPParams         params;
  KsString              Var;
  OV_VAR_TYPE           vt;
  long int              VarType;
  PltString             log;
  KS_RESULT             err;
  PltString             name;
  size_t                anzVars, i;
  
  KsString            root = Server->getHostAndName();
  Variables* pvar;

  // Alle Variablen merken
  pvar = pinst->Inst_var;
  anzVars = 0;
  while(pvar) {
     anzVars++;
     pvar = pvar->next;
  }
  if(anzVars == 0) {
     return KS_ERR_OK;
  }
  PltArray<Variables*>  remVars(anzVars);
  pvar = pinst->Inst_var;

  i = 0;
  while(pvar) {
     remVars[i] = pvar;
     i++;
     pvar = pvar->next;
  }

  // Variablen suchen
  params.path = pinst->Class_name;
  params.type_mask = KS_OT_DOMAIN;
  params.scope_flags = KS_EPF_DEFAULT;

  i = 0;
  while(i < anzVars) {

    if(remVars[i]) {
        // Variable holen
        pvar = remVars[i];
        
        name    = pvar->var_name;
        
        params.name_mask = name;
    
        err = Get_getEP_ErrOnly(Server, params);
        if(err) {
            // Nicht vorhanden
        } else {
    
            Var = params.path;
            Var += "/";
            Var += name;
            Var += ".vartype";
        
            KscVariable var(root+Var);
    
            if (!var.getUpdate() ) {
                  out += "     [ Can't read variable '";
                  out += name;
                  out += "' ]\n";
                  return 1;
            }
    
            /* CurrProps holen, Typ vergleichen. Laenge ? */
    
            const KsVarCurrProps *curr_props = var.getCurrProps();
            if ( (!curr_props) || (!curr_props->value) ) {
                  out += "     [ Can't read variable '";
                  out += name;
                  out += " ]\n";
                  return 1;
            }
            
            /* Typ der Variable holen, PV-Mask ausschneiden */
            VarType = ((KsIntValue &) *(curr_props->value));
            VarType &= OV_VT_KSMASK;
    
            switch( VarType ) {
    
                    case KS_VT_BOOL_VEC:
                    case KS_VT_BOOL:
                                    vt = KS_VT_BOOL;
                                    break;
    
                    case KS_VT_INT_VEC:
                    case KS_VT_INT:
                                    vt = KS_VT_INT;
                                    break;
    
                    case KS_VT_UINT_VEC:
                    case KS_VT_UINT:
                                    vt = KS_VT_UINT;
                                    break;
    
                    case KS_VT_SINGLE_VEC:
                    case KS_VT_SINGLE:
                                    vt = OV_VT_SINGLE;
                                    break;
    
                    case KS_VT_DOUBLE_VEC:
                    case KS_VT_DOUBLE:
                                    vt = KS_VT_SINGLE;
                                    break;
    
                    case KS_VT_STRING_VEC:
                    case KS_VT_STRING:
                                    vt = KS_VT_STRING;
                                    break;
    
                    case KS_VT_TIME_VEC:
                    case KS_VT_TIME:
                                    vt = KS_VT_TIME;
                                    break;
    
                    case KS_VT_TIME_SPAN_VEC:
                    case KS_VT_TIME_SPAN:
                                    vt = KS_VT_TIME_SPAN;
                                    break;
    
                    case KS_VT_BYTE_VEC:
                                    vt = KS_VT_BYTE_VEC;
                                    break;
    
                    default:
                                out += "     [ Type of the variable '" ;
                                out += name;
                                out += "'  unknown ]\n";
                                    return 1;
            } /* switch */
        
            if( vt != pvar->var_typ) {
                out += "     [  Wrong type of variable '" ;
                out += name;
                out += "' ]\n";
                return 1;
            }
                        
            // Dann OK
            remVars[i] = 0;
         }
        
         // Liste durchgelaufen?
         if(i == (anzVars - 1)) {
    
             // Gibt es noch nicht gefundene Variablen?
             int habeNoch = 0;
             i = 0;
             while(i<anzVars) {
                if(remVars[i]) {
                    habeNoch = 1;
                    break;
                }
                i++;
             }
             if( habeNoch == 0) {
                 // Fertig
                 break;
             }
             
             // Dann suche die Variablen in Basis-Klasse
             KsString vp = params.path;
             vp += ".baseclass";
             KscVariable clvar(root+vp);
             if (!clvar.getUpdate() ) {
                 break;
             }
             const KsVarCurrProps *cpbc = clvar.getCurrProps();
             if ( (!cpbc) || (!cpbc->value) ) {
                 break;
             }
             name = (const char*)((KsStringValue &) *cpbc->value);
             if( (name == "") || (name == CONTAINER_CLASS_PATH) ) {
                // Ende der Klass-Architekture
                break;
             }
             
             // Aus neue.
             params.path = name;
             i = (size_t)(-1);
         }
     }
     
     // Naechste Variable
     i++; 
  }

  err = KS_ERR_OK;
  i = 0;
  while(i < anzVars ) {
      if(remVars[i]) {
          out += "     [ Variable '";
          out += remVars[i]->var_name;
          out += "' not found ]\n";
          err = 1;
      }
      i++;
  }

  return err;

}

/*****************************************************************************/
KS_RESULT SearchErrorCreateInst(KscServerBase* Server,
                            KS_RESULT      &result,
                            InstanceItems* pinst,
                            PltString&     out)    {
/*****************************************************************************/

  char*         path;
  char*         ph;
  KsGetEPParams params;
  KS_RESULT     err;
  PltString     log;
  
  switch(result) {
    case KS_ERR_ALREADYEXISTS :
                    out += "     [ Instance already exist ]\n";
                    log  = "\"%s\"  \"";
                    log += pinst->Inst_name;
                    log += "\"";
                    iFBS_SetLastError(1, result, log);
                    return result;
    case KS_ERR_NOACCESS :
                    log  = "\"%s\"  \"";
                    log += pinst->Inst_name;
                    log += "\"";
                    iFBS_SetLastError(1, result, log);
                    return result;
    default :
               params.path = pinst->Class_name;
               params.type_mask = KS_OT_ANY;
               params.name_mask = "*";
               params.scope_flags = KS_EPF_DEFAULT;
               
               err = Get_getEP_ErrOnly(Server, params);
               if(err) {
                  out += "     [ Type ";
                  out += pinst->Class_name;
                  out += " not exist ]\n";
                  
                  log  = "\"%s\"  \"";
                  log += pinst->Class_name;
                  log += "\"";
                  err = KS_ERR_BADPATH;
                  iFBS_SetLastError(1, err, log);

                  return KS_ERR_BADPATH;
               }

               path = (char*)((const char*)pinst->Inst_name);
               ph = path;
               while(*ph) ph++ ;
               while( ((*ph) != '/') && (ph != path) ) ph-- ;
               if(ph == path) {
                  out += "     [ Bad path ";
                  out += pinst->Inst_name;
                  out += " ]\n";
                  
                  log  = "\"%s\"  \"";
                  log += pinst->Inst_name;
                  log += "\"";
                  err = KS_ERR_BADPATH;
                  iFBS_SetLastError(1, err, log);

                  return KS_ERR_BADPATH;
               }
               *ph = '\0';
               params.path = path;
               *ph = '/';
               
               err = Get_getEP_ErrOnly(Server, params);
               if(err) {
                  out += "     [ Container ";
                  out += path;
                  out += " not exist ]\n";
                  
                  log  = "\"%s\"  \"";
                  log += path;
                  log += "\"";
                  err = KS_ERR_BADPATH;
                  iFBS_SetLastError(1, err, log);
                  
                  return KS_ERR_BADPATH;
               }

               err = Check_Vars_in_lib(Server, pinst, out);
               if( err ) {
                  return err;
               }

  }
  
  return KS_ERR_OK;
}

/*****************************************************************************/
int test_ConnectionExists(
    KscServerBase* Server,
    ConData        &con,
    PltString      &out
) {
/*****************************************************************************/
    PltList<InstVarProps>   hpp;    // Eigenschaften der Variablen
    KS_RESULT               fehler;
    PltString               hStr;
    PltString               val;
    char*                   help;
    fehler = ReadBlockPorts(Server, con.identifier, hpp);
    if ( fehler ) {
        return 0;
    }
    
    // Dann ist die Connection vorhanden. Pruefe Links und Ports
    while( hpp.size() ) {
        
        InstVarProps var( hpp.removeFirst() );
        if(!(var.eng_props)) {
            // Handle auf Props. nicht initialisiert
            return 0;
        }
        
        val = "";
        ifb_getValueOnly(                  /*  |> Funktionsrueckmeldung                     */
                   &(var.curr_props)       /* >|  Eigenschaften der Variable                */
                   ,val                    /* >|> Value als String                          */
                   ,1                      /* >|  Max. Anzahl Elementen in Array            */
                   ,TRUE                   /* >|  Merker, ob Time-Ausgabe "... hh:mm:ss"    */
                   ,TRUE                   /* >|  Merker, ob TRUE als "1" und FALSE als "0" */
                   ,FALSE                  /* >|  Merker, ob Array als Liste "{...}"        */
                   ," ; "                  /* >|  Trenner der Array-Elementen               */
                   ,FALSE                  /* >|  Merker, ob string in Hochkommas           */
                );
                
        hStr = (const char*)(var.eng_props->identifier);

        // Links
        if(hStr == "sourcefb") {
            if(con.source_fb != val) {
                help = (char*)malloc(con.source_fb.len() + val.len() + 32);
                if(!help) {
                    return 0;
                }
                sprintf(help, "     [ Bad source fb '%s' != '%s' ]\n",
                            (const char*)con.source_fb, (const char*)val);
                out += help;
                free(help);
                return 0; // Andere Initialisierung
            }
        } else if(hStr == "targetfb") {
            if(con.target_fb != val) {
                help = (char*)malloc(con.target_fb.len() + val.len() + 32);
                if(!help) {
                    return 0;
                }
                sprintf(help, "     [ Bad target fb '%s' != '%s' ]\n",
                            (const char*)con.target_fb, (const char*)val);
                out += help;
                free(help);
                return 0; // Andere Initialisierung
            }
        // Ports
        } else if(hStr == "sourceport") {
            if(con.source_port != val) {
                help = (char*)malloc(con.source_port.len() + val.len() + 32);
                if(!help) {
                    return 0;
                }
                sprintf(help, "     [ Bad source port '%s' != '%s' ]\n",
                            (const char*)con.source_port, (const char*)val);
                out += help;
                free(help);
                return 0; // Andere Initialisierung
            }
        } else if(hStr == "targetport") {
            if(con.target_port != val) {
                help = (char*)malloc(con.target_port.len() + val.len() + 32);
                if(!help) {
                    return 0;
                }
                sprintf(help, "     [ Bad target port '%s' != '%s' ]\n",
                            (const char*)con.target_port, (const char*)val);
                out += help;
                free(help);
                return 0; // Andere Initialisierung
            }
        }
    }
    
    // Dann vorhanden und richtig initialisiert.
    return 1;
}

/*****************************************************************************/
KS_RESULT SearchErrorCreateCon(KscServerBase* Server,
                               KS_RESULT      &result,
                               ConData        &con,
                               Variables      *pVars,
                               PltString&     out)    {
/*****************************************************************************/

  KsGetEPParams params;
  KsGetEPResult res1;
  KsGetEPResult res2;
  KS_RESULT     err;
  bool          ok;
  int           dynam;
  PltString     log;
  
  KS_SEMANTIC_FLAGS  flag_s;
  KS_SEMANTIC_FLAGS  flag_t;
  
  KsString            root = Server->getHostAndName();

  params.type_mask = KS_OT_ANY;
  params.name_mask = "*";
  params.scope_flags = KS_EPF_DEFAULT;

  if(result == KS_ERR_ALREADYEXISTS) {
    out += "     [ Connection object already exist ]\n";

    log  = "\"%s\"  \"";
    log += con.identifier;
    log += "\"";
    iFBS_SetLastError(1, result, log);
    return result;
  }

  if(con.source_fb == "") {
    out += "     [ Source instance name missing ]";
    
    err = KS_ERR_BADNAME;
    log  = "\"%s\"  \"Source instance name missing\"";
    iFBS_SetLastError(1, err, log);

    return KS_ERR_BADNAME;
  }
  if(con.source_port == "") {
    out += "     [ Source port name missing ]";
    
    err = KS_ERR_BADNAME;
    log  = "\"%s\"  \"Source port name missing\"";
    iFBS_SetLastError(1, err, log);

    return KS_ERR_BADNAME;
  }
  if(con.target_fb == "") {
    out += "     [ Target instance name missing ]";
    
    err = KS_ERR_BADNAME;
    log  = "\"%s\"  \"Target instance name missing\"";
    iFBS_SetLastError(1, err, log);

    return KS_ERR_BADNAME;
  }
  if(con.target_port == "") {
    out += "     [ Target port name missing ]";
    
    err = KS_ERR_BADNAME;
    log  = "\"%s\"  \"Target port name missing\"";
    iFBS_SetLastError(1, err, log);

    return KS_ERR_BADNAME;
  }
      
/////  Gibt es Source-FB ?

  params.path = con.source_fb;
  err = Get_getEP_ErrOnly(Server, params);
  if(err) {
    out += "     [ Instance '";
    out += con.source_fb;
    out += "' not exist ]\n";

    err = KS_ERR_BADPATH;
    log  = "\"%s\"  \"";
    log += con.source_fb;
    log += "\"";
    iFBS_SetLastError(1, err, log);

    return KS_ERR_BADPATH;
  }

/////  Gibt es Target-FB ?

  params.path = con.target_fb;
  err = Get_getEP_ErrOnly(Server, params);
  if(err) {
    out += "     [ Instance '";
    out += con.target_fb;
    out += "' not exist ]\n";

    err = KS_ERR_BADPATH;
    log  = "\"%s\"  \"";
    log += con.target_fb;
    log += "\"";
    iFBS_SetLastError(1, err, log);

    return KS_ERR_BADPATH;
  }

/////  Gibt es bei Source-FB Verbindungs-Listen Kopf ?

  params.path = con.source_fb;
  params.name_mask = "outputcon";
  err = Get_getEP_ErrOnly(Server, params);
  if(err) {
    out += "     [ Instance '";
    out += con.source_fb;
    out += "' not linkable ]\n";

    err = KS_ERR_BADPATH;
    log  = "\"Instance %s not linkable\"  \"";
    log += con.source_fb;
    log += "\"";
    iFBS_SetLastError(1, err, log);

    return KS_ERR_BADPATH;
  }

/////  Gibt es bei Source-FB  Source-Port ?

  params.name_mask = con.source_port;

  ok = Server->getEP(0, params, res1);
  if ( !ok ) {
    err = 1;
  } else {
     if ( res1.result != KS_ERR_OK ) {
       err = res1.result;
     }
  }
  if ( !(res1.items.size()) ) {
   err = KS_ERR_BADPATH;
  }

  if(err) {
    out += "     [ Variable '";
    out += con.source_fb;
    out += ".";
    out += con.source_port;
    out += "' not exist ]\n";

    err = KS_ERR_BADPATH;
    log  = "\"%s\"  \"";
    log += con.source_fb;
    log += ".";
    log += con.source_port;
    log += "\"";
    iFBS_SetLastError(1, err, log);

    return err;
  }

  /////  Ist Source-Port lesbar ?

  KsEngPropsHandle pp_sv = res1.items.removeFirst();
  flag_s = ((KsVarEngProps&)(*pp_sv)).semantic_flags;
  if( IsFlagSet( flag_s, 'p') ) {
      out += "     [ Port '";
      out += con.source_fb;
      out += ".";
      out += con.source_port;
      out += "' not linkable ]\n";

    err = KS_ERR_BADINITPARAM;
    log  = "\"Port %s not linkable\"  \"";
    log += con.source_fb;
    log += ".";
    log += con.source_port;
    log += "\"";
    iFBS_SetLastError(1, err, log);

      return KS_ERR_BADINITPARAM;
  }
  if( ! (  IsFlagSet( flag_s, 'o')  ||
           IsFlagSet( flag_s, 'i')   )  ) {
      out += "     [ Port '";
      out += con.source_fb;
      out += ".";
      out += con.source_port;
      out += "' not linkable ]\n";

    err = KS_ERR_BADINITPARAM;
    log  = "\"Port %s not linkable\"  \"";
    log += con.source_fb;
    log += ".";
    log += con.source_port;
    log += "\"";
    iFBS_SetLastError(1, err, log);

      return KS_ERR_BADINITPARAM;
  }

/////  Gibt es bei Target-FB Verbindungs-Listen Kopf ?

  params.path = con.target_fb;
  params.name_mask = "inputcon";
  err = Get_getEP_ErrOnly(Server, params);
  if(err) {
    out += "     [ Instance '";
    out += con.target_fb;
    out += "' not linkable ]\n";

    err = KS_ERR_BADINITPARAM;
    log  = "\"Instance %s not linkable\"  \"";
    log += con.target_fb;
    log += "\"";
    iFBS_SetLastError(1, err, log);

    return KS_ERR_BADINITPARAM;
  }

/////  Gibt es bei Target-FB  Target-Port ?

  params.name_mask = con.target_port;

  ok = Server->getEP(0, params, res2);
  if ( !ok ) {
    err = 1;
  } else {
    if ( res2.result != KS_ERR_OK ) {
       err = res2.result;
    }
  }
  if ( !(res2.items.size()) ) {
    err = KS_ERR_BADPATH;
  }

  if(err) {
    out += "     [ Variable '";
    out += con.target_fb;
    out += ".";
    out += con.target_port;
    out += "' not exist ]\n";

    err = KS_ERR_BADPATH;
    log  = "\"%s\"  \"";
    log += con.target_fb;
    log += ".";
    log += con.target_port;
    log += "\"";
    iFBS_SetLastError(1, err, log);
    
    return err;
  }


/////  Ist Target-port beschreibbar ?

  KsEngPropsHandle pp_tv = res2.items.removeFirst();
  flag_t = ((KsVarEngProps&)(*pp_tv)).semantic_flags;
  if( IsFlagSet( flag_t, 'p') ) {
      out += "     [ Variable '";
      out += con.target_fb;
      out += ".";
      out += con.target_port;
      out += "' not linkable ]\n";

    err = KS_ERR_BADINITPARAM;
    log  = "\"Port %s not linkable\"  \"";
    log += con.target_fb;
    log += ".";
    log += con.target_port;
    log += "\"";
    iFBS_SetLastError(1, err, log);

      return KS_ERR_BADINITPARAM;
  }
  if( ! IsFlagSet( flag_t, 'i') ) {
      out += "     [ Port '";
      out += con.target_fb;
      out += ".";
      out += con.target_port;
      out += "' not writeable ]\n";

    err = KS_ERR_BADINITPARAM;
    log  = "\"Port %s not writeable\"  \"";
    log += con.target_fb;
    log += ".";
    log += con.target_port;
    log += "\"";
    iFBS_SetLastError(1, err, log);

      return KS_ERR_BADINITPARAM;
  }

/////  Sind beide Ports dynamisch ?
  if ( IsFlagSet( flag_t, 'd') ) {
     dynam = 1;
  } else {
     dynam = 0;
  }
  if ( IsFlagSet( flag_t, 'd') != IsFlagSet( flag_s, 'd') ) {
      out += "     [ Variable types mismatch ]\n";
    err = KS_ERR_TYPEMISMATCH;
    log  = "";
    iFBS_SetLastError(1, err, log);
      return KS_ERR_TYPEMISMATCH;
  }

/////  Sind beide Ports PVs ?
  if ( IsFlagSet( flag_t, 's') != IsFlagSet( flag_s, 's') ) {
      out += "     [ Variable types mismatch ]\n";
    err = KS_ERR_TYPEMISMATCH;
    log  = "";
    iFBS_SetLastError(1, err, log);
      return KS_ERR_TYPEMISMATCH;
  }

///// Curr_props :

  KsString tmpvar = con.source_fb;
  tmpvar += ".";
  tmpvar += con.source_port;

  KscVariable svar(root+tmpvar);
  if (!svar.getUpdate() ) {
    out += "     [ Can't read variable '";
    out += tmpvar;
    out += "' ]\n";
    return 1;
  }
  const KsVarCurrProps *cp_sv = svar.getCurrProps();
  if ( !cp_sv ) {
    out += "     [ Can't read variable '";
    out += tmpvar;
    out += "' ]\n";
    return 1;
  }

  tmpvar = con.target_fb;
  tmpvar += ".";
  tmpvar += con.target_port;

  KscVariable tvar(root+tmpvar);
  if (!tvar.getUpdate() ) {
    out += "     [ Can't read variable '";
    out += tmpvar;
    out += "' ]\n";
    return 1;
  }
  
  const KsVarCurrProps *cp_tv = tvar.getCurrProps();
  if ( !cp_tv ) {
    out += "     [ Can't read variable '";
    out += tmpvar;
    out += "' ]\n";
    return 1;
  }

///// Sind die Variablen von gleichen Typen ?

  if( ((cp_tv->value)->xdrTypeCode()) != ((cp_sv->value)->xdrTypeCode() )) {
    out += "     [ Variable types mismatch ]\n";
    err = KS_ERR_TYPEMISMATCH;
    log  = "";
    iFBS_SetLastError(1, err, log);
    return KS_ERR_TYPEMISMATCH;
  }


///// Ist die laenge auch gleich ?
  if( !dynam ) {
    switch((cp_sv->value)->xdrTypeCode() ) {
            case KS_VT_BOOL:
            case KS_VT_INT:
            case KS_VT_UINT:
            case KS_VT_SINGLE:
            case KS_VT_DOUBLE:
            case KS_VT_STRING:
            case KS_VT_TIME:
            case KS_VT_TIME_SPAN:
                                    break;
            case KS_VT_BYTE_VEC:
                 if( ( ((KsByteVecValue &) *(cp_tv->value)).size() ) !=
                     ( ((KsByteVecValue &) *(cp_sv->value)).size() )  ) {
                      out += "     [ Variable lengths mismatch ]\n";
                    err = KS_ERR_TYPEMISMATCH;
                    log  = "";
                    iFBS_SetLastError(1, err, log);
                      return KS_ERR_TYPEMISMATCH;
                 }
                 break;
            case KS_VT_BOOL_VEC:
                 if( ( ((KsBoolVecValue &) *(cp_tv->value)).size() ) !=
                     ( ((KsBoolVecValue &) *(cp_sv->value)).size() )  ) {
                      out += "     [ Variable lengths mismatch ]\n";
                    err = KS_ERR_TYPEMISMATCH;
                    log  = "";
                    iFBS_SetLastError(1, err, log);
                      return KS_ERR_TYPEMISMATCH;
                 }
                 break;
            case KS_VT_INT_VEC:
                 if( ( ((KsIntVecValue &) *(cp_tv->value)).size() ) !=
                     ( ((KsIntVecValue &) *(cp_sv->value)).size() )  ) {
                      out += "     [ Variable lengths mismatch ]\n";
                    err = KS_ERR_TYPEMISMATCH;
                    log  = "";
                    iFBS_SetLastError(1, err, log);
                      return KS_ERR_TYPEMISMATCH;
                 }
                 break;
            case KS_VT_UINT_VEC:
                 if( ( ((KsUIntVecValue &) *(cp_tv->value)).size() ) !=
                     ( ((KsUIntVecValue &) *(cp_sv->value)).size() )  ) {
                      out += "     [ Variable lengths mismatch ]\n";
                    err = KS_ERR_TYPEMISMATCH;
                    log  = "";
                    iFBS_SetLastError(1, err, log);
                      return KS_ERR_TYPEMISMATCH;
                 }
                 break;
            case KS_VT_SINGLE_VEC:
                 if( ( ((KsSingleVecValue &) *(cp_tv->value)).size() ) !=
                     ( ((KsSingleVecValue &) *(cp_sv->value)).size() )  ) {
                      out += "     [ Variable lengths mismatch ]\n";
                    err = KS_ERR_TYPEMISMATCH;
                    log  = "";
                    iFBS_SetLastError(1, err, log);
                      return KS_ERR_TYPEMISMATCH;
                 }
                 break;
            case KS_VT_DOUBLE_VEC:
                 if( ( ((KsDoubleVecValue &) *(cp_tv->value)).size() ) !=
                     ( ((KsDoubleVecValue &) *(cp_sv->value)).size() )  ) {
                      out += "     [ Variable lengths mismatch ]\n";
                    err = KS_ERR_TYPEMISMATCH;
                    log  = "";
                    iFBS_SetLastError(1, err, log);
                      return KS_ERR_TYPEMISMATCH;
                 }
                 break;
            case KS_VT_STRING_VEC:
                 if( ( ((KsStringVecValue &) *(cp_tv->value)).size() ) !=
                     ( ((KsStringVecValue &) *(cp_sv->value)).size() )  ) {
                      out += "     [ Variable lengths mismatch ]\n";
                    err = KS_ERR_TYPEMISMATCH;
                    log  = "";
                    iFBS_SetLastError(1, err, log);
                      return KS_ERR_TYPEMISMATCH;
                 }
                 break;
            case KS_VT_TIME_VEC:
                 if( ( ((KsTimeVecValue &) *(cp_tv->value)).size() ) !=
                     ( ((KsTimeVecValue &) *(cp_sv->value)).size() )  ) {
                      out += "     [ Variable lengths mismatch ]\n";
                    err = KS_ERR_TYPEMISMATCH;
                    log  = "";
                    iFBS_SetLastError(1, err, log);
                      return KS_ERR_TYPEMISMATCH;
                 }
                 break;
            case KS_VT_TIME_SPAN_VEC:
                 if( ( ((KsTimeVecValue &) *(cp_tv->value)).size() ) !=
                     ( ((KsTimeVecValue &) *(cp_sv->value)).size() )  ) {
                      out += "     [ Variable lengths mismatch ]\n";
                    err = KS_ERR_TYPEMISMATCH;
                    log  = "";
                    iFBS_SetLastError(1, err, log);
                      return KS_ERR_TYPEMISMATCH;
                 }
                 break;
            default:
                      out += "     [ Unknown variable type ]\n";
                    err = KS_ERR_BADTYPE;
                    log  = "";
                    iFBS_SetLastError(1, err, log);
                      return KS_ERR_BADTYPE;
    } /* switch */
  } /* if ! dynam */


//// Bibl/Klass vorhanden?
  if(con.conclas == "") {
      con.conclas = CONNECTION_CLASS_PATH;
  }
  params.path = con.conclas;
  params.name_mask = "*";
  err = Get_getEP_ErrOnly(Server, params);
  if(err) {
      out += "     [ Type ";
      out += con.conclas;
      out += " not exist ]\n";
      
      log  = "\"%s\"  \"";
      log += con.conclas;
      log += "\"";
      err = KS_ERR_BADFACTORY;
      iFBS_SetLastError(1, err, log);

      return KS_ERR_BADFACTORY;
  }

  // Abgeleitete Klasse mit nicht mehr vorhandenen Variablen?
  if(pVars) {
      params.path =  con.identifier;
      params.type_mask = KS_OT_VARIABLE;
      params.scope_flags = KS_EPF_PARTS;
      while(pVars) {
          params.name_mask = pVars->var_name;
          
          err = Get_getEP_ErrOnly(Server, params);
          if(err) {
              out += "     [ Variable ";
              out += pVars->var_name;
              out += " not exist ]\n";
              
              log  = "\"%s\"  \"";
              log += pVars->var_name;
              log += "\"";
              err = KS_ERR_BADPATH;
              iFBS_SetLastError(1, err, log);
        
              return KS_ERR_BADPATH;
          }
          
          pVars = pVars->next;
      }
  }
  
  return KS_ERR_OK;
}

// Prueft, ob alle FB-Links/Ports zu Connection-Objekt vorhanden sind
/*****************************************************************************/
int test_connectionDataOk(
    float         serverVersion,
    Dienst_param  *Params,
    InstanceItems *pinst,
    ConData       &CR,
    PltString     &out ) {
/*****************************************************************************/
    PltString       VerbName;
    PltString       log;
    LinksItems*     pOcLink;
    LinksItems*     pIcLink;
    LinksItems*     hLinks;
    
    VerbName = pinst->Inst_name;
    
    // Link "outputcon" suchen
    log = "outputcon";
            
    pOcLink = Params->Links;
    while( (pOcLink) &&
           ((VerbName != pOcLink->children->child_path) ||
           (log != pOcLink->child_role)) ) {
        pOcLink = pOcLink->next;
    }
    if(!pOcLink) {
        if(serverVersion < 2.4) {
            out += log_getErrMsg(KS_ERR_OK, "Link \"outputcon\" to connection",
                        pinst->Inst_name, "not found.");
        }
        return 0;
    }
        
    // Link "inputcon" suchen
    log = "inputcon";
    pIcLink = Params->Links;
    while( (pIcLink) &&
           ((VerbName != pIcLink->children->child_path) ||
           (log != pIcLink->child_role)) ) {
        pIcLink = pIcLink->next;
    }
    if(!pIcLink) {
        if(serverVersion < 2.4) {
            out += log_getErrMsg(KS_ERR_OK, "Link \"inputcon\" to connection",
                        pinst->Inst_name, "not found.");
        }
        return 0;
    }


    // Verbindungs-Daten merken
    CR.conclas     = pinst->Class_name;
    CR.identifier  = pinst->Inst_name;
    CR.source_fb   = pOcLink->parent_path;
    CR.target_fb   = pIcLink->parent_path;

    Variables *pcv = pinst->Inst_var;
    while( pcv ) {
        if( !strcmp(pcv->var_name, "sourceport") ) {
            if(pcv->value) {
                CR.source_port = pcv->value->val;
            }
        } else if( !strcmp(pcv->var_name, "targetport") ) {
            if(pcv->value) {
                CR.target_port = pcv->value->val;
            }
        } else if( !strcmp(pcv->var_name, "on") ) {
            if(pcv->value) {
                log = pcv->value->val;
                if(log == "TRUE")
                    CR.on = TRUE;
                else
                    CR.on = FALSE;
            }
        } else if( !strcmp(pcv->var_name, "sourcetrig") ) {
            if(pcv->value) {
                log = pcv->value->val;
                if(log == "TRUE")
                    CR.source_trig = TRUE;
                else
                    CR.source_trig = FALSE;
            }
        }
        pcv = pcv->next;
    }


    // Die Link-Struktur wird nicht mehr gebraucht. Speicher freigeben
    hLinks = Params->Links;
    if(hLinks != pOcLink) {
        while(hLinks->next != pOcLink) {
            hLinks = hLinks->next;
        }
        hLinks->next = pOcLink->next;
    } else {
        Params->Links = hLinks->next;
    } 
    if(pOcLink->children) {
        free(pOcLink->children);
    }
    free(pOcLink);

    hLinks = Params->Links;
    if(hLinks != pIcLink) {
        while(hLinks->next != pIcLink) {
            hLinks = hLinks->next;
        }
        hLinks->next = pIcLink->next;
    } else {
        Params->Links = hLinks->next;
    }
    
    if(pIcLink->children) {
        free(pIcLink->children);
    }
    free(pIcLink);

    return 1;
}

/*****************************************************************************/
KS_RESULT import_eval(KscServerBase* Server,
                      Dienst_param*  Params,
                      PltString&     out,
                      bool           DEL_INST)    {
/*****************************************************************************/

    char            path[256];
    KS_RESULT       error;
    LinksItems*     pLinks;
    Child*          pChild;
    PltString       log;
    PltString       libpath;

//    KscAvSimpleModule  *AV = GetClientAV();
    FbCreateInstParams  CrPar;

///////////////////////////////////////////////////////////////////////////////
//  Bibliotheke laden                                                        //
///////////////////////////////////////////////////////////////////////////////

    DelInstItems*        plib;
    PltList<PltString> LoadedLibs;
    PltList<PltString> NotLoadedLibs;
    int                i, anz;
    
    CrPar.factory = LIBRARY_FACTORY_PATH;
    libpath = "/";
    libpath += FB_LIBRARIES_CONTAINER;
    libpath += "/";
    
    if(Params->NewLibs) {

        plib = Params->NewLibs;
        while(plib) {
            CrPar.path = libpath;
            CrPar.path += plib->Inst_name;
            
            error = IFBS_CREATE_INST(Server,CrPar);
            if(error) {
                if(error == KS_ERR_ALREADYEXISTS) {
                    LoadedLibs.addFirst(CrPar.path);
               
                    out += log_getErrMsg(error,
                            "Library",plib->Inst_name,"couldn't be loaded.");
                
                    log  = "\"%s\"  \"";
                    log += plib->Inst_name;
                    log += "\"";
                    iFBS_SetLastError(1, error, log);
                } else {
                    NotLoadedLibs.addFirst(CrPar.path);
                }
            } else {
                    out += log_getOkMsg("Library",plib->Inst_name,"loaded.");
                    // In umgekerten Reienfolge merken
                    LoadedLibs.addFirst(CrPar.path);
            }
            plib = plib->next;
        } /* while plib */

        // Gibt es nicht geladene Bibliotheken?
        int anzLoops = 5;
        while(NotLoadedLibs.size() && anzLoops > 0) {

            anz = NotLoadedLibs.size();
            for(i=0; i<anz; i++) {
                log = NotLoadedLibs.removeFirst();

                CrPar.path = log;
                
                error = IFBS_CREATE_INST(Server,CrPar);
                if(error != KS_ERR_OK) {
                    NotLoadedLibs.addLast(log);
                } else {
                    out += log_getOkMsg("Library",(const char*)log,"loaded.");                    
                }
            }
            anzLoops--;
        }

        // Gibt es immer noch nicht geladene Bibliotheken?
        while( NotLoadedLibs.size() ) {
            log = NotLoadedLibs.removeFirst();
            
            CrPar.path = log;
            
            error = IFBS_CREATE_INST(Server,CrPar);
            if(error != KS_ERR_OK) {
                out += log_getErrMsg(error,
                        "Library", (const char*)log,
                        "couldn't be loaded.");
            
                log  = "\"%s\"  \"";
                log += CrPar.path;
                log += "\"";
                iFBS_SetLastError(1, error, log);
            } else {
                out += log_getOkMsg("Library",(const char*)CrPar.path,"loaded.");                    
            }
        }
        
    } /* if NewLibs */


///////////////////////////////////////////////////////////////////////////////
//  Instanzen und Verbindungen anlegen                                       //
///////////////////////////////////////////////////////////////////////////////
    
    size_t               pLen;
    Dienst_param         tempObjs;
    InstanceItems*        pinst;
    InstanceItems*        pverb_objs;
    InstanceItems*        pinst_objs;
    
    KS_RESULT       hr;

    /*
    * Das ist missbrauch von CreateObject-Dienst und Netzwerkuebertragungszeit,
    *  weil wir alle Instanzen nur mit einem Dienst erzeugen koennen. Falls aber
    *  nur eine von n Instanzen nicht angelegt wird, muessen wir alle erfolgreich
    *  angelegten Instanzen aufraeumen. Deswegen muesen wir genau wissen, an
    *  welcher Stelle wir uns befinden.                                        
    */
    tempObjs.Instance = 0;
	tempObjs.Set_Inst_Var = 0;
	tempObjs.DelInst = 0;
	tempObjs.OldLibs = 0;
	tempObjs.NewLibs = 0;
	tempObjs.Links = 0;
	tempObjs.UnLinks = 0;

    /* 
    *  Zunaechst werden alle Funktionsbausteine und Tasks angelegt.
    *  Alle Verbindungsobjekte werden aussortiert
    */
    sprintf(path,"/%s/", FB_CONN_CONTAINER);
    pLen = strlen(path);

    pverb_objs = 0;
    pinst_objs = 0;
    while(Params->Instance) {
    
        pinst=Params->Instance;
        Params->Instance = pinst->next;
        
        if( !strncmp(pinst->Inst_name, path, pLen) ) {
            // Es ist Verbinduns-Objekt
            pinst->next = pverb_objs;
            pverb_objs  = pinst;
        } else {
            // Benutzer-Instanz oder Container
            pinst->next = pinst_objs;
            pinst_objs  = pinst;
        }
    }

    // Nun stehen Instanzen in umgekehrten Reihenfolge
    while(pinst_objs) {
        pinst=pinst_objs;
        pinst_objs = pinst->next;
        
        pinst->next = Params->Instance;
        Params->Instance = pinst;
    }
                        
    // Alle Instanzen anlegen
    while(Params->Instance) {
    
        pinst=Params->Instance;
        Params->Instance = pinst->next;
        
        error = FB_CreateNewInstance(Server,pinst,out);
        if(error) {
            
            log = "";
            hr = KS_ERR_OK;
            iFBS_SetLastError(1, hr, log);
            SearchErrorCreateInst(Server,error,pinst,out);
            log = IFBS_GetLastLogError();
            if(log == "" ) {
                log = "\"%s\"  ";
                log += pinst->Inst_name;
                log += "\"";
                iFBS_SetLastError(1, error, log);
            }
            if(DEL_INST) {
                aufraeumen(Server, &tempObjs, out);  /* Loesche angelegte Instanzen */
                DelObjsFromList(Server, LoadedLibs, &out);/* Loesche geladene Bibliotheken */
            }

            /* Aktuelle Instanz zurueck zu Liste */
            pinst->next = Params->Instance;
            Params->Instance = pinst;
            /* Bereits angelegte Objs anhaengen */
            while(tempObjs.Instance) {
                pinst = tempObjs.Instance;
                tempObjs.Instance = pinst->next;
                
                pinst->next = Params->Instance;
                Params->Instance = pinst;
            }
            /* Einsortierte Verb.-Objs anhaengen */
            while(pverb_objs) {
                pinst = pverb_objs;
                pverb_objs = pinst->next;
                
                pinst->next = Params->Instance;
                Params->Instance = pinst;
            }

            return error;
        }
        
        // Merke: Instanz angelegt
        pinst->next = tempObjs.Instance;
        tempObjs.Instance = pinst;

    } /* Ueber alle Instanzen */


    // Ab Server-Version 2.4 konnen auch "unvollstaendige"
    //    Verbindungsobjekte angelegt werden
    float serverVersion = get_serverVersion(Server);
    
    /*
    *  Alle Instanzen sind angelegt.
    *  Lege Verbindungsobjekte an
    */
    while(pverb_objs) {
        pinst = pverb_objs;
        pverb_objs = pinst->next;

        ConData CR;
        
        // Alle Daten vorhanden?
        if( test_connectionDataOk(serverVersion, Params, pinst, CR, out) ) {
            // Verbindung anlegen
            error = IFBS_CREATE_COMCON(Server, CR, pinst->Inst_var);
            if(error) {
                PltString cMsg("");
                if( test_ConnectionExists(Server, CR, cMsg) ) {
                    // Werte der Variablen aktualisiren
                    Dienst_param        svcPar;
                    SetInstVarItems     setVars;
                    
                    setVars.next = 0;
                    setVars.Inst_name = pinst->Inst_name;                    
                    setVars.Inst_var  = pinst->Inst_var;
                    
                    svcPar.Set_Inst_Var = &setVars;
                    svcPar.DelInst = 0;
                    svcPar.OldLibs = 0;
                    svcPar.NewLibs = 0;
                    svcPar.Links   = 0;
                    svcPar.UnLinks = 0;
                    
                    error = set_new_value(Server, &svcPar, out);
                } else {
                        
                    out += log_getErrMsg(error,
                            "Connection", (const char*)CR.identifier,
                            " couldn't be created.");
                        
                    // Fehler bereits gefunden?
                    if(cMsg != "") {
                        out += cMsg;
                    } else {
                        log = "";
                        hr = KS_ERR_OK;
                        iFBS_SetLastError(1, hr, log);
    
                        SearchErrorCreateCon(Server,error,CR,pinst->Inst_var,out);
    
                        log = IFBS_GetLastLogError();
                        if(log == "" ) {
                            log = "\"%s\"  ";
                            log += CR.identifier;
                            log += "\"";
                            iFBS_SetLastError(1, error, log);
                        }
                    }
                        
                    if(DEL_INST) {
                        aufraeumen(Server, &tempObjs, out);      /* Loesche angelegte Instanzen */
                        DelObjsFromList(Server, LoadedLibs, &out);/* Loesche geladene Bibliotheken */
                    }
                    /* Aktuelle Instanz zurueck zu Liste */
                    pinst->next = Params->Instance;
                    Params->Instance = pinst;
                    /* Bereits angelegte Objs anhaengen */
                    while(tempObjs.Instance) {
                        pinst = tempObjs.Instance;
                        tempObjs.Instance = pinst->next;
                        
                        pinst->next = Params->Instance;
                        Params->Instance = pinst;
                    }
                    /* Einsortierte Verb.-Objs anhaengen */
                    while(pverb_objs) {
                        pinst = pverb_objs;
                        pverb_objs = pinst->next;
                        
                        pinst->next = Params->Instance;
                        Params->Instance = pinst;
                    }
                    return error;
                }
            }
            
        } else {
            // Es sind nicht alle Daten vorhanden.
            if(serverVersion >= 2.4) {
                // Dann...
                error = FB_CreateNewInstance(Server,pinst,out);
                if(error) {
                    
                    if(DEL_INST) {
                        aufraeumen(Server, &tempObjs, out);  /* Loesche angelegte Instanzen */
                        DelObjsFromList(Server, LoadedLibs, &out);/* Loesche geladene Bibliotheken */
                    }
        
                    /* Aktuelle Instanz zurueck zu Liste */
                    pinst->next = Params->Instance;
                    Params->Instance = pinst;
                    /* Bereits angelegte Objs anhaengen */
                    while(tempObjs.Instance) {
                        pinst = tempObjs.Instance;
                        tempObjs.Instance = pinst->next;
                        
                        pinst->next = Params->Instance;
                        Params->Instance = pinst;
                    }
                    /* Einsortierte Verb.-Objs anhaengen */
                    while(pverb_objs) {
                        pinst = pverb_objs;
                        pverb_objs = pinst->next;
                        
                        pinst->next = Params->Instance;
                        Params->Instance = pinst;
                    }
        
                    return error;
                }
                
            } else {
                out += log_getErrMsg(error,
                        "Connection", pinst->Inst_name,
                        " couldn't be created.");

                if(DEL_INST) {
                    aufraeumen(Server, &tempObjs, out);      /* Loesche angelegte Instanzen */
                    DelObjsFromList(Server, LoadedLibs, &out);/* Loesche geladene Bibliotheken */
                }
                /* Aktuelle Instanz zurueck zu Liste */
                pinst->next = Params->Instance;
                Params->Instance = pinst;
                /* Bereits angelegte Objs anhaengen */
                while(tempObjs.Instance) {
                    pinst = tempObjs.Instance;
                    tempObjs.Instance = pinst->next;
                    
                    pinst->next = Params->Instance;
                    Params->Instance = pinst;
                }
                /* Einsortierte Verb.-Objs anhaengen */
                while(pverb_objs) {
                    pinst = pverb_objs;
                    pverb_objs = pinst->next;
                    
                    pinst->next = Params->Instance;
                    Params->Instance = pinst;
                }
                return KS_ERR_BADPARAM;
            }
        }
            
        // Merke: Verbindung angelegt
        pinst->next = tempObjs.Instance;
        tempObjs.Instance = pinst;
        
    } /* while pverb_objs */

    /* Fertig. Objekte zur Gesammt-Liste zurueck */
    Params->Instance = tempObjs.Instance;


///////////////////////////////////////////////////////////////////////////////
//  Dienst Link                                                              //
///////////////////////////////////////////////////////////////////////////////

    if(Params->Links) {
        // Es sind nur Task- und X- Links in der Liste.
        // Die Verbindungs-Links sind schon aussortiert und ausgefuert.
        pLinks = Params->Links;

        FbLinkParams Pars;
        Pars.place = KS_PMH_END;
        
        while(pLinks) {
            Pars.parent_path = pLinks->parent_path;
            Pars.parent_path += ".";
            Pars.parent_path += pLinks->child_role;

            pChild = pLinks->children;

            while(pChild) {
                Pars.elem_path = pChild->child_path;
                
                error = ifb_createLink(Server, Pars);
                if(error) {
                    if(error != KS_ERR_ALREADYEXISTS) {
                        out += log_getErrMsg(error,
                            "Parent ",pLinks->parent_path,
                            " and child ", pChild->child_path,
                            " couldn't be linked.");                    
                    }
                } else {
                        out += log_getOkMsg("Parent",pLinks->parent_path,
                                            "and child",pChild->child_path,
                                            "linked.");
                }
                
                pChild = pChild->next; 
            } /* while pchild */

            pLinks = pLinks->next;
        } /* while pLinks     */

    }   /* if Params->Links */

    return KS_ERR_OK;

}
