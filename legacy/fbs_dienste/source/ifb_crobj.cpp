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
*   ifb_crobj.cpp                                                            *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   1999-04-07  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*   Erzeugt ein Funktionbausteinobjekt                                       *
*                                                                            *
*****************************************************************************/

#include "ifbslibdef.h"

/******************************************************************************/
KS_RESULT SearchErrorCreateObject(KscServerBase*     Server,
                                  KS_RESULT          &res,
                                  FbCreateInstParams &pars) {
/******************************************************************************/

  KsGetEPParams params;
  KS_RESULT     err;
  PltString     log;
  char*         help;
  char*         ph;
  
  switch(res) {
    case KS_ERR_ALREADYEXISTS :
                    log  = "\"%s\"  \"";
                    log += pars.path;
                    log += "\"";
                    iFBS_SetLastError(1, res, log);
                    return res;
    case KS_ERR_NOACCESS :
                    log  = "\"%s\"  \"";
                    log += pars.path;
                    log += "\"";
                    iFBS_SetLastError(1, res, log);
                    return res;
    default : break;
  }
  
  if( !pars.factory.len() ) {
    log = "\"%s\"  \"\"";
    err = KS_ERR_BADFACTORY;
    iFBS_SetLastError(1, err, log);
    return err;
  }
  params.path = pars.factory;
  params.type_mask = KS_OT_ANY;
  params.name_mask = "*";
  params.scope_flags = KS_EPF_DEFAULT;

  err = Get_getEP_ErrOnly(Server, params);
  if(err) {
    log  = "\"%s\"  \"";
    log += pars.factory;
    log += "\"";
    err = KS_ERR_BADFACTORY;
    iFBS_SetLastError(1, err, log);
    return err;
  }

  if( !pars.path.len() ) {
    log = "\"%s\"  \"\"";
    err = KS_ERR_BADNAME;
    iFBS_SetLastError(1, err, log);
    return err;
  }

  help = (char*)malloc(pars.path.len() + 1);
  strcpy(help, (const char*)pars.path);
  ph = help;
  
  while( (*ph) )  ph++; ph--;
  if( (*ph) == '/') {
    free(help);
    log = "\"%s\"  \"";
    log += pars.path;
    log += "\"";
    err = KS_ERR_BADPATH;
    iFBS_SetLastError(1, err, log);
    return err;
  }
  
  while( (ph != help  )  &&
         ((*ph) != '/' )   ) {
       ph--;
  }
  
  if( ph == help ) {
    free(help);
    log  = "\"%s\"  \"";
    log += pars.path;
    log += "\"";
    err = KS_ERR_BADPATH;
    iFBS_SetLastError(1, err, log);
    return err;
  }

  *ph = '\0';

  params.path = help;

  free(help);
  
  err = Get_getEP_ErrOnly(Server, params);
  if(err) {
    log  = "\"%s\"  \"";
    log += pars.path;
    log += "\"";
    err = KS_ERR_BADPATH;
    iFBS_SetLastError(1, err, log);
    return err;
  }

  return KS_ERR_OK;
}

/******************************************************************************/
KS_RESULT IFBS_CREATE_INST(KscServerBase*     Server,
                           FbCreateInstParams &pars)

/******************************************************************************/
 {
    KS_RESULT err = 0;
    PltString log("");

        KsArray<KsCreateObjItem>  objitem(1);
        KsCreateObjParams         objpar;
    KsCreateObjResult         res;

    iFBS_SetLastError(1, err, log);
    
    if(!Server) {
        err = KS_ERR_SERVERUNKNOWN;
        iFBS_SetLastError(1, err, log);
              return err;
    }

        objitem[0].factory_path = pars.factory;
        objitem[0].new_path     = pars.path;
        objitem[0].place.hint   = KS_PMH_END;


        objpar.items = objitem;

        bool ok = Server->requestByOpcode ( KS_CREATEOBJECT, GetClientAV(), objpar, res);
    if(!ok) {
        err = Server->getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
    } else if(res.result) {
                err = res.result;
        } else if( res.obj_results[0].result ) {
            err = res.obj_results[0].result;
        }

    if(err) {
             SearchErrorCreateObject(Server,err,pars);
            log = IFBS_GetLastLogError();
            if(log == "") {
                log = "\"%s %s\"  \"";
                log += pars.factory;
                log += "\" \"";
                log += pars.path;
                log += "\"";
                iFBS_SetLastError(1, err, log);
            }
        }

    return err;
}

/*****************************************************************************/
int test_InstanceExists(KscServerBase* Server,
                        const char*    instPath,
                        const char*    instClass)  
/*****************************************************************************/
{
    char *help;
    char *ph;
    PltString instName;
    KsGetEPParams    params;           /* Parameter für getEP-Service       */
    KsGetEPResult    result;
    int              isPart;
    
    if( (!instPath) || (!(*instPath)) ) {
        // Pfad?
        return 0;
    }
    
    isPart = 0; // Annahme: Child
    help = (char*)malloc(strlen(instPath) + 1);
    if(!help) {
        return 0;
    }
    strcpy(help, instPath);
    ph = help;
    
    // Pfad und Name trennen
    while(ph && (*ph)) ph++;    // String-Ende suchen
    while(ph != help) {
        ph--;
        if( ((*ph) == '.') || ((*ph) == '/') ) {
            if((*ph) == '.') {
                isPart = 1;
            }
            
            ph++;
            instName = ph;
            ph--;
            *ph = '\0';
            break;
        }
    }
    if(instName.len() == 0) {
        // Bad path
        return 0;
    }
    
    // Fix: Instanz im Root?
    if( (!help) || (!(*help)) ) {
        params.path = "/";
    } else {
        params.path = help;
    }
    
    free(help);
    
    params.name_mask = instName;
    params.type_mask = KS_OT_DOMAIN | KS_OT_HISTORY;
    params.scope_flags = KS_EPF_DEFAULT;
    
    // Dienst ausfuehren
    bool ok = Server->getEP(0, params, result);
    if ( !ok ) {
        return 0;
    }
    
    if ( result.result ) {
        return 0;
    }

    if(result.items.size()) {
        // Instanz mit dem Namen vorhanden. Mehrere Instanzen (Part+Child)?
        while(result.items.size()) {
            KsEngPropsHandle hpp = result.items.removeFirst();
            if(!hpp) {
                return 0;
            }
            PltString typ;
            KS_ACCESS acc;
            if(hpp->xdrTypeCode() == KS_OT_HISTORY) {
                typ = (const char*)((KsHistoryEngProps &)(*hpp)).type_identifier;
                acc = ((KsHistoryEngProps &)(*hpp)).access_mode;
            } else {
                typ = (const char*)((KsDomainEngProps &)(*hpp)).class_identifier;
                acc = ((KsDomainEngProps &)(*hpp)).access_mode;
            }
            
            if(isPart) {
                if(acc & KS_AC_PART) {
                    if(typ == instClass) {
                        // Instanz als Part und vom gleichen Klass.
                        return 1;
                    }
                }
            } else {

                if(!(acc & KS_AC_PART) ) {
                    if(typ == instClass) {
                        // Instanz als Child und vom gleichen Klass.
                        return 1;
                    }
                }
            }
        }
    }
    
    return 0;
}
/*****************************************************************************/
KS_RESULT FB_CreateNewInstance(KscServerBase* Server,
                               InstanceItems* pinst,
                               PltString&     out)  
/*****************************************************************************/
{   
    KsCreateObjParams        objpar;
    KsCreateObjResult        res;
    KsArray<KsCreateObjItem> objitem(1);
    KS_RESULT                err;
    KS_RESULT                remErr;
    
    int                      part = 0;
    char                     *ph;
    char                     *pc;

    if(!Server) {
        out += log_getErrMsg(KS_ERR_SERVERUNKNOWN, "Instance",
                pinst->Inst_name,"couldn't be created.");
        
        return KS_ERR_SERVERUNKNOWN;
    }
    // ist das ein Part?
    ph = pinst->Inst_name;
    pc = ph;
    while(pc && (*pc)) pc++;    // String-Ende suchen
    while(pc != ph) {
        pc--;
        if((*pc) == '.') {
            part = 1;
            break;
        }
        if((*pc) == '/') {
            // Kein Part
            break;
        }
    }

    // Instanz bereits vorhanden?
    if(part == 0 ) {
        if( test_InstanceExists(Server, pinst->Inst_name, pinst->Class_name) ) {
            part = 1;
        }
    }
    
    if(part == 1) {
        // Part-Objekt bereits mit Parent-Objekt angelegt. Nur Werte setzen.
        err = KS_ERR_OK;
        if(pinst->Inst_var) {
            Dienst_param        svcPar;
            SetInstVarItems         setVars;
            
            setVars.next = 0;
            setVars.Inst_name = pinst->Inst_name;
            setVars.Inst_var  = pinst->Inst_var;
            
            svcPar.Set_Inst_Var = &setVars;
            svcPar.DelInst = 0;
            svcPar.OldLibs = 0;
            svcPar.NewLibs = 0;
            svcPar.Links   = 0;
            svcPar.UnLinks = 0;
            
            err = (KS_RESULT)set_new_value(Server, &svcPar, out);
            if(err == KS_ERR_NOACCESS) {
                err = KS_ERR_OK;
            }
        } else {
            // Keine Variablen in Domain
        }
        return err;
    }
    
    objitem[0].factory_path = pinst->Class_name;

    objitem[0].new_path = pinst->Inst_name;

    objitem[0].place.hint = KS_PMH_END;

    if(pinst->Inst_var) {
        err = GetCreateObjectVar(pinst->Inst_var,objitem[0].parameters);
        if(err) {
            out += log_getErrMsg(err, "Instance",
                        pinst->Inst_name,"couldn't be created.");

            return err;
        }
    }
    
    objpar.items = objitem;

    bool ok = Server->requestByOpcode ( KS_CREATEOBJECT, GetClientAV(), objpar, res);
    if(!ok) {
        err = Server->getLastResult();
        if(err == KS_ERR_OK) {
            err = KS_ERR_GENERIC;
            out += log_getErrMsg(err, "Instance",pinst->Inst_name,
            "couldn't be created.");

            return err;
        }
    }
    
    remErr = KS_ERR_OK;
    if( res.result ) {
        remErr = res.result;
    } else if( ! res.obj_results.size() ) {
        remErr = KS_ERR_GENERIC;
    } else if( res.obj_results[0].result ) {
        remErr = res.obj_results[0].result;
    }
    
    
    if(remErr != KS_ERR_OK) {
        out += log_getErrMsg(remErr, "Instance",pinst->Inst_name,"couldn't be created.");
    } else {
        out += log_getOkMsg("Instance", pinst->Inst_name,"created.");    
    }
    
    return remErr;
}

/******************************************************************************/
KsSetVarItem* obj_CreateObjectVar( Variables* pvar)
/******************************************************************************/
{
    KsVarCurrProps*         var_props;
    KsSetVarItem*           setpar;
    VariableItem*           var_val;
    KS_VAR_TYPE             vt;
    PltList<PltString>      ValList;
    KS_RESULT               err;

    var_props = new KsVarCurrProps;
    if(!var_props) {
        return 0;
    }

    var_props->state = pvar->state;

    var_val = pvar->value;
    // Alle Elemente in Liste einfuegen
    while(var_val) {
        ValList.addLast(var_val->val);
        var_val = var_val->next;
    }
    vt = pvar->var_typ;
    // Ist die Variable ein Vektor ?
    if(pvar->vector) {
        // Es ist ein Vektor. Variable-Typ setzen
        switch(pvar->var_typ) {
        case KS_VT_BOOL: vt = KS_VT_BOOL_VEC;
                        break;
        case KS_VT_INT: vt = KS_VT_INT_VEC;
                        break;
        case KS_VT_UINT: vt = KS_VT_UINT_VEC;
                        break;
        case KS_VT_SINGLE: vt = KS_VT_SINGLE_VEC;
                        break;
        case KS_VT_DOUBLE: vt = KS_VT_DOUBLE_VEC;
                        break;
        case KS_VT_STRING: vt = KS_VT_STRING_VEC;
                        break;
        case KS_VT_TIME: vt = KS_VT_TIME_VEC;
                        break;
        case KS_VT_TIME_SPAN: vt = KS_VT_TIME_SPAN_VEC;
                        break;
        default:
                 break;
        }
    }

    var_props->value.bindTo(ifb_CrNewKsValue(err, ValList, vt), PltOsNew);
    if(err) {
        delete var_props;
        return 0;
    }
    setpar = new KsSetVarItem;
    if(!setpar) {
        delete var_props;
        return 0;
    }

    setpar->path_and_name = ".";
    setpar->path_and_name += pvar->var_name;
    setpar->curr_props.bindTo( (KsCurrProps*)var_props, PltOsNew);

    return setpar;
}
/******************************************************************************/
KS_RESULT GetCreateObjectVar( Variables* pvar, KsArray<KsSetVarItem> &pars)
/******************************************************************************/
{
    
    if(!pvar) {
        return KS_ERR_OK;
    }

    size_t                  i;          // Laufvariable
    PltList<KsSetVarItem*>  hilf;
    //KsVarCurrProps*                    var_props;
    KsSetVarItem*                        setpar;
    
    while(pvar) {
        if(pvar->len != 0) {
            setpar = obj_CreateObjectVar(pvar);
            if(!setpar) {
                while(hilf.size() ) {
                    setpar = hilf.removeFirst();
                    if(setpar) delete setpar;
                }
                return OV_ERR_HEAPOUTOFMEMORY;
            }
            hilf.addLast(setpar);

        } /* if len != 0 */
   
        pvar=pvar->next;
    } /* while pvar */
 
    size_t siz = hilf.size();  // Anzahl der zu setzenden Variablen merken

    KsArray<KsSetVarItem> param(siz);
    if(param.size() != siz) {
        while(hilf.size() ) {
            setpar = hilf.removeFirst();
            if(setpar) delete setpar;
        }
        return OV_ERR_HEAPOUTOFMEMORY;
    }
    
    for (i = 0; i < siz; i++) {
        setpar = hilf.removeFirst();
        param[i] = *setpar;
        delete setpar;
    }

    pars = param;
    
    return KS_ERR_OK;
}
