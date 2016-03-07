/*****************************************************************************
*                                                                            *
*    i F B S p r o                                                           *
*   ###############                                                          *
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
*    iFBSpro  ACPLT/KS Dienste-Schnittstelle (C++)                           *
*   ===============================================                          *
*                                                                            *
*   Datei                                                                    *
*   -----                                                                    *
*   task_link.cpp                                                            *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   1999-12-13  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*   Link eine Instanz zum Task                                               *
*                                                                            *
*****************************************************************************/

#include "ifbslibdef.h"

/*****************************************************************************/
KS_RESULT SearchErrorLinkToTask(KscServerBase* Server,
                      KS_RESULT& res, FbLinkParams& pars) {
/*****************************************************************************/

  KsGetEPParams params;
  KS_RESULT     err = KS_ERR_OK;
  PltString     log;

  if( !pars.parent_path.len() ) {
    log = "\"%s\"  \"\"";
    err = KS_ERR_BADNAME;
    iFBS_SetLastError(1, err, log);
    return err;
  }

  params.path = pars.parent_path;
  params.type_mask = KS_OT_ANY;
  params.name_mask = "*";
  params.scope_flags = KS_EPF_DEFAULT;

  err = Get_getEP_ErrOnly(Server, params);
  if(err) {
    log  = "\"%s\"  \"";
    log += pars.parent_path;
    log += "\"";
    err = KS_ERR_BADPATH;
    iFBS_SetLastError(1, err, log);
    return err;
  }

  params.name_mask = "taskchild";

  err = Get_getEP_ErrOnly(Server, params);
  if(err) {
    log  = "\"Instance %s not linkable\"  \"";
    log += pars.parent_path;
    log += "\"";
    err = KS_ERR_BADPATH;
    iFBS_SetLastError(1, err, log);
    return err;
  }

  if( !pars.elem_path.len() ) {
    log = "\"%s\"  \"\"";
    err = KS_ERR_BADNAME;
    iFBS_SetLastError(1, err, log);
    return err;
  }

  params.path = pars.elem_path;
  params.name_mask = "*";

  err = Get_getEP_ErrOnly(Server, params);
  if(err) {
    log  = "\"%s\"  \"";
    log += pars.elem_path;
    log += "\"";
    err = KS_ERR_BADPATH;
    iFBS_SetLastError(1, err, log);
    return err;
  }

  params.name_mask = "taskparent";

  err = Get_getEP_ErrOnly(Server, params);
  if(err) {
    log  = "\"Instance %s not linkable\"  \"";
    log += pars.elem_path;
    log += "\"";
    err = KS_ERR_BADPATH;
    iFBS_SetLastError(1, err, log);
  }

  KsString            root = Server->getHostAndName();
  KsString            Var  = pars.elem_path;
  
  Var += ".";
  Var += "taskparent";

  KscVariable var(root+Var);

  if (!var.getUpdate() ) {
     return 0;
  }

  const KsVarCurrProps *curr_props = var.getCurrProps();
  if ( !curr_props ) {
     return 0;
  }

  switch(curr_props->value->xdrTypeCode() )
    {
      case KS_VT_STRING:
                        log = (KsStringValue &) *curr_props->value;
                        if(log != "") {
                            log  = "\"Link %s already exist\"  \"";
                            log += pars.elem_path;
                            log += "\"";
                            err = KS_ERR_ALREADYEXISTS;
                            iFBS_SetLastError(1, err, log);
                        }
                        break;
          default:
                                                   break;

    } /*case */
    
  return err;

}

/*****************************************************************************/
KS_RESULT ifb_createLink(KscServerBase*     Server,
                         FbLinkParams       &Pars)
/*****************************************************************************/
{

    KsLinkParams         linkpar;
    KsArray<KsLinkItem>  objlinks(1); 
    KsLinkResult         lres;

    objlinks[0].link_path = Pars.parent_path;
    objlinks[0].element_path = Pars.elem_path;
    objlinks[0].place.hint = Pars.place;
    objlinks[0].place.place_path = Pars.place_path;

    linkpar.items = objlinks;

    bool ok = Server->requestByOpcode ( KS_LINK, GetClientAV(), linkpar, lres);
    if(!ok) {
        KS_RESULT err = Server->getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
        return err;
    }
    if(lres.result) {
        return lres.result;
    }
    if(lres.results[0]) {
        return lres.results[0];
    }

    return KS_ERR_OK;
}

/*****************************************************************************/
KS_RESULT IFBS_LINK_TO_TASK(KscServerBase*     Server,
                            FbLinkParams       &Pars)
/*****************************************************************************/
{
    KS_RESULT err = KS_ERR_OK;
    FbLinkParams    Params(Pars);
    PltString       log("");

    if(!Server) {
        err = KS_ERR_SERVERUNKNOWN;
        iFBS_SetLastError(1, err, log);
        return err;
    }
    
    iFBS_SetLastError(1, err, log);
    
//    Params.parent_path = Pars.parent_path;
    Params.parent_path += ".taskchild";
//    Params.elem_path = Pars.elem_path;
//    Params.place = Pars.place;
//    Params.place_path = Pars.place_path;
    
    err = ifb_createLink(Server, Params);
    if(err) {
        SearchErrorLinkToTask(Server,err,Pars);
        log = IFBS_GetLastLogError();
        if(log == "") {
            log = "\"%s %s\"  \"";
            log += Pars.parent_path;
            log += "\" \"";
            log += Pars.elem_path;
            log += "\"";
            iFBS_SetLastError(1, err, log);
        }
    }
  
    return err;

} /* LinkInstToTask() */


/*****************************************************************************/
KS_RESULT ifb_deleteLink(KscServerBase*     Server,
                         FbLinkParams       &Pars)
/*****************************************************************************/
{
    KsUnlinkParams          unlinkpar;
    KsArray<KsUnlinkItem>   unlinkit(1);
    KsUnlinkResult          ulres;

    unlinkit[0].link_path = Pars.parent_path;
    unlinkit[0].element_path = Pars.elem_path;

    unlinkpar.items = unlinkit;

    bool ok = Server->requestByOpcode ( KS_UNLINK, GetClientAV(), unlinkpar, ulres);
    if(!ok) {
        KS_RESULT err = Server->getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
        return err;
    }
    if(ulres.result) {
            return ulres.result;
    }
    if(ulres.results[0]) {
                return ulres.results[0];
        }

    return KS_ERR_OK;
}

/*****************************************************************************/
KS_RESULT IFBS_UNLINK_FROM_TASK(KscServerBase* Server,
                                FbLinkParams   &Pars)
/*****************************************************************************/
{
    KS_RESULT     err;
    FbLinkParams  Params(Pars);
    
    if(Server == NULL) {
            return KS_ERR_SERVERUNKNOWN;
    }
    
//    Params.parent_path = Pars.parent_path;
    Params.parent_path += ".taskchild";
//    Params.elem_path = Pars.elem_path;

    err = ifb_deleteLink(Server, Params);
    if(err) {
        PltString log;
        if(log == "") {
            log = "\"%s %s\"  \"";
            log += Pars.parent_path;
            log += "\" \"";
            log += Pars.elem_path;
            log += "\"";
            iFBS_SetLastError(1, err, log);
        }
    }
    
    return err;  
}
