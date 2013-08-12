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
*    iFBSpro  ACPLT/KS Dienste-Schnittstelle (C++)                           *
*   ===============================================                          *
*                                                                            *
*   Datei                                                                    *
*   -----                                                                    *
*   read_block_param.cpp                                                     *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   1999-12-13  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
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
*        globale Variablen
*        -----------------
*/

/*****************************************************************************/
KS_RESULT getDomainProps(KscServerBase*            Server,
                         PltString             &inst_name,
                         KsEngPropsHandle      &hpo)
/*****************************************************************************/
{
    KsGetEPParams  param;
    KsGetEPResult  res;         // Ergebnis des Lesens der Instanz-Daten
    char*          ph;
    KS_RESULT             err = KS_ERR_OK;
    PltString             log("");
    char                  *help;

    if(!Server) {
        err = KS_ERR_SERVERUNKNOWN;
        iFBS_SetLastError(1, err, log);
        return err;
    }

//   param.path = TecUnitPath; <- es muss den ganzen Pfad eigegeben werden

    param.type_mask = (KS_OT_DOMAIN | KS_OT_HISTORY);
    param.scope_flags = KS_EPF_DEFAULT;

    help = (char*)malloc(inst_name.len() + 1);
    if(!help) {
        return KS_ERR_TARGETGENERIC;
    }
    
    strcpy(help, (const char*)inst_name);
    ph=help;


    while( (*ph) )  ph++;                       /* suche  Ende vom String */

    while((ph != help) && ((*ph)!='/') && ((*ph)!='.')) ph--;   /* Suche das letzte Vorkommen von '/' oder '.' */

    if(ph == help) {
        free(help);
        log = "\"%s\"  \"";
        log += inst_name;
        log += "\"";
        err = KS_ERR_BADPATH;
        
        iFBS_SetLastError(1, err, log);
        return err;
    }
    /* '/' oder '.' gefunden, generiere Parameter */
    *ph = '\0';
    ph++; /* ph zeigt jetzt auf Instanz-Name selbst */

    param.path = help;
    param.name_mask = ph;

    free(help);
    
    bool ok = Server->getEP(0, param, res);
    if ( !ok ) {
        err = Server->getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
    } else {
       if ( res.result != KS_ERR_OK ) {
           err = res.result;
       }
    }
   
    if(err) {
        log = "\"%s\"  \"";
        log += inst_name;
        log += "\"";
        iFBS_SetLastError(1, err, log);
        return err;
    }


    /* Eigenschaften der Domain merken  */

    if ( (res.items.size()) ) {
        hpo = res.items.removeFirst();
    } else {
        err = KS_ERR_BADPATH;
        log = "\"%s\"  \"";
        log += inst_name;
        log += "\"";
        iFBS_SetLastError(1, err, log);
        return err;
    }
    // Handle erzeugt ?
    if( !hpo) {
        err = KS_ERR_GENERIC;
        log = "\"%s\"  \"";
        log += inst_name;
        log += "\"";
        iFBS_SetLastError(1, err, log);
        return err;
    }
    
    return KS_ERR_OK;
}

/*****************************************************************************/
KS_RESULT ReadBlockPorts(KscServerBase*            Server,
                         PltString             &inst_name,
                         PltList<InstVarProps> &hpp)
/*****************************************************************************/
  {
    KsGetEPParams         param;
    KsGetEPResult         result;      // Ergebnis des Lesens der Variablen-Daten
    KS_RESULT             err = KS_ERR_OK;
    PltString             log("");
//    char                  help[128];

    if(!Server) {
        err = KS_ERR_SERVERUNKNOWN;
        iFBS_SetLastError(1, err, log);
        return err;
    }

    // Alle Variable auflisten :
    param.path = inst_name;
    param.type_mask = (KS_OT_VARIABLE | KS_OT_LINK);
    param.name_mask = "*";
    param.scope_flags = KS_EPF_DEFAULT;


    bool ok = Server->getEP(0, param, result);
    if ( !ok ) {
        err = Server->getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
    } else {
       if ( result.result != KS_ERR_OK ) {
           err = result.result;
       }
    }
   
    if(err) {
        log = "\"%s\"  \"";
        log += inst_name;
        log += "\"";
        iFBS_SetLastError(1, err, log);
        return err;
    }
   
    /* Anzahl der gefundenen Variablen merken */
    size_t AnzVars = result.items.size();
   
    /* Variablen gefunden ? */
    if( !AnzVars ) {
        // Keine Variale vorhanden
        return KS_ERR_OK;
    }
   
    /* Parameter generieren : */

    KsString            root;   // Merker : //host/server
    KsString            Var;    // Pfad und Name der Variable
  
    PltArray<KscVariable*> VarArray(AnzVars);
    if(VarArray.size() != AnzVars) {
        err = OV_ERR_HEAPOUTOFMEMORY;
        log = "";
        iFBS_SetLastError(1, err, log);
        return err;
    }
    
    PltList<KsEngPropsHandle>  ListEP;
    KscPackage        *pkg = new KscPackage;
    if( !pkg) {
        err = OV_ERR_HEAPOUTOFMEMORY;
        log = "";
        iFBS_SetLastError(1, err, log);
        return err;
    }

    root = Server->getHostAndName();

    size_t i;           // Laufvariable
    size_t readedVars;  // Anzahl gelesenen Variablen
    readedVars = 0;
    for(i = 0; i < AnzVars; i++) {
        KsEngPropsHandle pv = result.items.removeFirst();
        if(!pv) {
            delete pkg;
            return KS_ERR_GENERIC;
        }
        if((pv->identifier != "Xpos") && (pv->identifier != "Ypos")) {
            ListEP.addLast(pv);
    
            Var = param.path;
            Var += ".";
            Var += pv->identifier;  /* /. */
    
            KscVariable *v = new KscVariable(root+Var);
            VarArray[readedVars++] = v;
             
                if( !pkg->add(KscVariableHandle(v, PltOsNew)) ){
                //delete VarArray[i]; // FIXME : hoffentlich ist die Variable doch an
                                          //         Handle gebunden. Dann darf die auch nicht
                                          //         geloescht werden. Sonst wird die Anwendung
                                          //         abstuerzen. Ist die nicht gebunden - haben
                                          //         wir Speicher-Leck.
                err = KS_ERR_GENERIC;
                log = "";
                iFBS_SetLastError(1, err, log);
                delete pkg;
                return err;
                }
            }
         
    } /* ueber alle Variablen */

    /* Gibt es was zum lesen? */
    if( !readedVars ) {
        // Keine Variable vorhanden
        return KS_ERR_OK;
    }

  if(!pkg->getUpdate() ) {
    int err = pkg->getLastResult();
    if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
    delete pkg;
        return err;
  }

  for(i = 0; i < readedVars; i++ ) {
          KsVarCurrProps *cp = (KsVarCurrProps*)(VarArray[i])->getCurrProps();
          if(!cp) {
          err = KS_ERR_GENERIC;
          log = "";
          iFBS_SetLastError(1, err, log);
          delete pkg;
          return err;
          }
      InstVarProps one_var(*cp, ListEP.removeFirst());
      hpp.addLast(one_var);
  }

  /* Speicher freigeben */

  delete pkg;

  return KS_ERR_OK;

}  /* ReadBlockParameter() */
