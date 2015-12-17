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
*   set_par.cpp                                                              *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   1999-04-07  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*   Setzt eine Variable einer Funktionblockinstanz                           *
*                                                                            *
*****************************************************************************/

#include "ifbslibdef.h"

/*****************************************************************************/
KS_RESULT IFBS_DBFLUSH(KscServerBase*   Server)
/*****************************************************************************/
{

  KS_RESULT          fehler = KS_ERR_OK;
  PltString          log("");
  PltList<PltString> ValList;
  
  
  if(!Server) {
    fehler = KS_ERR_SERVERUNKNOWN;
    iFBS_SetLastError(1, fehler, log);
    return fehler;
  }
    
  iFBS_SetLastError(1, fehler, log);

  ValList.addFirst("1");
  log = "/dbinfo.flush";
  fehler = ifb_setVar(Server, log, ValList, KS_VT_INT);
   
 if(fehler) {
    log = "";
    iFBS_SetLastError(1, fehler, log);
 }
 
 return fehler;
 
} /* db_flush() */