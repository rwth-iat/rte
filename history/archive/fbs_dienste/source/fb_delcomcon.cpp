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
*   delete_comcon.cpp                                                        *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   1999-04-07  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*   Loescht eine Verbindung                                                  *
*                                                                            *
*****************************************************************************/

#include "ifbslibdef.h"

/*****************************************************************************/
KS_RESULT IFBS_DELETE_COMCON(KscServerBase* Server,
                                                   ConData &pars)
/*****************************************************************************/
{
    PltString   verb_name;
    PltString   log("");
    KS_RESULT   err = 0;
    
    if(!Server){
        err = KS_ERR_SERVERUNKNOWN;
        iFBS_SetLastError(1, err, log);
        return err;
    }

    iFBS_SetLastError(1,  err, log);

    /* Suche Verbindungsname : */

    err = verb_suchen(Server, pars, verb_name);
    if (err) {
        iFBS_SetLastError(1,  err, log);
        return err;
    }

    log = "/";
    log += FB_CONN_CONTAINER;
    log += "/";
    log += verb_name;
    
    err = IFBS_DELETE_OBJ(Server, log);
    if(err) {
        log = "";
        iFBS_SetLastError(1,  err, log);
    }

    return err;
}  /* delete_comcon() */



/*****************************************************************************/
int verb_suchen(KscServerBase*    Server, ConData &verb, PltString &verb_name)
/*****************************************************************************/
  {
   char           help[250];
   KsGetEPParams  param;
   KsGetEPResult  result;
   int            err;
   PltString      log("");
   KsString       Str("");

   KscVariable *var1;
   KscVariable *var2;
   KscVariable *var3;
   KscVariable *var4;
   
   /* Suche Verbindungsname : */

   param.path  = "/";
   param.path += FB_CONN_CONTAINER;
   param.type_mask = KS_OT_DOMAIN;
   param.name_mask = "*";
   param.scope_flags = KS_EPF_DEFAULT;

   bool ok = Server->getEP(0, param, result);

   if ( !ok )
     {
        err = Server->getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
        return err;
     }
   else 
     {
       if ( result.result != KS_ERR_OK )
         {
           return result.result;
         }
     }

   /* Sonst ist getPP-Dienst ohne Fehler ausgefuehrt */

  KsString            root = Server->getHostAndName();
  KsString            Var;

  sprintf(help, "/%s/", FB_CONN_CONTAINER);

  while ( (result.items.size()) )
    {
      KsEngPropsHandle hpp = result.items.removeFirst();
      if(!hpp) {
        err = OV_ERR_GENERIC;
        iFBS_SetLastError(1, err, log);
        return err;
      }

          KscPackage*        pkg = new KscPackage;
      if(!pkg) {
        err = OV_ERR_HEAPOUTOFMEMORY;
        iFBS_SetLastError(1, err, log);
        return err;
      }
      Var  = help;
      Var += hpp->identifier;
      Var += ".sourcefb";
          var1 = new KscVariable(root+Var);
      if(!var1) {
        delete pkg;
        err = OV_ERR_HEAPOUTOFMEMORY;
        iFBS_SetLastError(1, err, log);
        return err;
      }
           if(!pkg->add(KscVariableHandle(var1, PltOsNew)) ){
                delete pkg;
                 return KS_ERR_GENERIC;
          }

      Var  = help;
      Var += hpp->identifier;
      Var += ".sourceport";
           var2 = new KscVariable(root+Var);
      if(!var2) {
        delete pkg;
        err = OV_ERR_HEAPOUTOFMEMORY;
        iFBS_SetLastError(1, err, log);
        return err;
      }
           if(!pkg->add(KscVariableHandle(var2, PltOsNew)) ){
                delete pkg;
                 return KS_ERR_GENERIC;
          }

      Var  = help;
      Var += hpp->identifier;
      Var += ".targetfb";
           var3 = new KscVariable(root+Var);
      if(!var3) {
        delete pkg;
        err = OV_ERR_HEAPOUTOFMEMORY;
        iFBS_SetLastError(1, err, log);
        return err;
      }
           if(!pkg->add(KscVariableHandle(var3, PltOsNew)) ){
                delete pkg;
                 return KS_ERR_GENERIC;
          }

      Var  = help;
      Var += hpp->identifier;
      Var += ".targetport";
           var4 = new KscVariable(root+Var);
      if(!var4) {
        delete pkg;
        err = OV_ERR_HEAPOUTOFMEMORY;
        iFBS_SetLastError(1, err, log);
        return err;
      }
           if(!pkg->add(KscVariableHandle(var4, PltOsNew)) ) {
                delete pkg;
                 return KS_ERR_GENERIC;
          }

          if(!pkg->getUpdate() ) {
                delete pkg;
                return 1;
          }

      // sourcefb
          const KsVarCurrProps * cp1 = var1->getCurrProps();
          if( (!cp1) || (!(cp1->value)) ) {
            delete pkg;
            return KS_ERR_GENERIC;
          }
          Str = ((KsStringValue &) *(cp1->value));
      if(Str != verb.source_fb ) {
                delete pkg;
                continue;
          }

      // sourceport
          const KsVarCurrProps * cp2 = var2->getCurrProps();
          if( (!cp2) || (!(cp2->value)) ) {
            delete pkg;
            return KS_ERR_GENERIC;
          }
          Str = ((KsStringValue &) *(cp2->value));
          if(Str != verb.source_port) {
                delete pkg;
                continue;
          }

      // targetfb
          const KsVarCurrProps * cp3 = var3->getCurrProps();
          if( (!cp3) || (!(cp3->value)) ) {
            delete pkg;
            return KS_ERR_GENERIC;
          }
          Str = ((KsStringValue &) *(cp3->value));
          if(Str != verb.target_fb) {
                delete pkg;
                continue;
          }
          
          // targetport
          const KsVarCurrProps * cp4 = var4->getCurrProps();
          if( (!cp4) || (!(cp4->value)) ) {
            delete pkg;
            return KS_ERR_GENERIC;
          }
          Str = ((KsStringValue &) *(cp4->value));
          if(Str != verb.target_port ) {
                delete pkg;
                continue;
          }

      verb_name = (const char*)hpp->identifier;
      delete pkg;
      return 0;

    } /* while size() */

    // Verbindung nicht gefunden
    return KS_ERR_BADPARAM;

}  /* verb_suchen() */


/*****************************************************************************/
KS_RESULT IFBS_DELETE_COMCONBYNAME(
                                           KscServerBase*   Server,
                                           PltString        &path)
/*****************************************************************************/
{
 char* v_name;
 char* pv;
 KS_RESULT err;
 PltString log("");
 
 if( path.len() ) {
    v_name = (char*)((const char*)path);
    pv = v_name;
    while(*pv) pv++;
    while( (pv!=v_name) && ((*pv)!='/') ) pv--;
    if( (*pv) == '/') {
        pv++;
    }
 } else {
    log = "\"%s\"  \"\"";
    err = KS_ERR_BADNAME;
    iFBS_SetLastError(1, err, log);
    return err;
 }

 path = "/";
 path += FB_CONN_CONTAINER;
 path += "/";
 path += pv;

 return IFBS_DELETE_OBJ(Server, path);
}
