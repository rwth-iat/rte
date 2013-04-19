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
*   rename.cpp                                                               *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   2000-01-13  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*   Umbennennen oder Verschieben eines Objektes                              *
*                                                                            *
*****************************************************************************/

#include "ifbslibdef.h"

/******************************************************************************/
KS_RESULT SearchErrorRenameObject(KscServerBase*     Server,
                                  PltString          &old,
                                  PltString          &neu) {
/******************************************************************************/

  KsGetEPParams params;
  KS_RESULT     err;
  PltString     log;
  char*         help;
  char*         ph;
  
  
  if( (!old.len()) || (!neu.len()) ) {
    log = "\"%s\"  \"\"";
    err = KS_ERR_BADPATH;
    iFBS_SetLastError(1, err, log);
    return err;
  }

  params.path = old;
  params.type_mask = KS_OT_ANY;
  params.name_mask = "*";
  params.scope_flags = KS_EPF_DEFAULT;

  err = Get_getEP_ErrOnly(Server, params);
  if(err) {
    log  = "\"%s\"  \"";
    log += old;
    log += "\"";
    err = KS_ERR_BADPATH;
    iFBS_SetLastError(1, err, log);
    return err;
  }

  help = (char*)((const char*)neu);
  ph = help;
  while( (*ph) )  ph++; ph--;
  if( (*ph) == '/') {
    log = "\"%s\"  \"";
    log += neu;
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
    log  = "\"%s\"  \"";
    log += neu;
    log += "\"";
    err = KS_ERR_BADPATH;
    iFBS_SetLastError(1, err, log);
    return err;
  }

  *ph = '\0';

  params.path = help;

  err = Get_getEP_ErrOnly(Server, params);
  if(err) {
    log  = "\"%s\"  \"";
    log += neu;
    log += "\"";
    err = KS_ERR_BADPATH;
    iFBS_SetLastError(1, err, log);
    return err;
  }

  params.path = neu;

  err = Get_getEP_ErrOnly(Server, params);
  if(!err) {
    log  = "\"%s\"  \"";
    log += neu;
    log += "\"";
    err = KS_ERR_ALREADYEXISTS;
    iFBS_SetLastError(1, err, log);
    return err;
  }

  return KS_ERR_OK;
}


/******************************************************************************/
KS_RESULT IFBS_RENAME(KscServerBase* Server,
                                              PltString      &old_path,
                                              PltString      &new_path) {
/******************************************************************************/


    KsRenameObjParams         renamepar;
    KsArray<KsRenameObjItem>  objitem(1); 
    KsRenameObjResult         rres;
    KS_RESULT                 err = KS_ERR_OK;
    PltString                 log("");
  
    iFBS_SetLastError(1, err, log);

    if(Server == NULL) {
        err = KS_ERR_SERVERUNKNOWN;
        iFBS_SetLastError(1, err, log);
        return err;
    }

    objitem[0].old_path = old_path;
    objitem[0].new_path = new_path;

    renamepar.items=objitem;

    bool ok = Server->requestByOpcode ( KS_RENAMEOBJECT, GetClientAV(), renamepar, rres);
    if(!ok) {
        err = Server->getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
        iFBS_SetLastError(1, err, log);
        return err;
    }
    if(rres.result) {
            err = rres.result;
    } else {
        if(rres.results[0]) {
                    err = rres.results[0];
            }
    }

    if(err) {
        SearchErrorRenameObject(Server,old_path,new_path);
            log = IFBS_GetLastLogError();
            if(log == "") {
                log = "\"%s %s\"  \"";
                log += old_path;
                log += "\" \"";
                log += new_path;
                log += "\"";
                iFBS_SetLastError(1, err, log);
            }
    }
    
    return err;
}
