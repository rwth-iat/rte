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
*   ================================================                         *
*                                                                            *
*   Datei                                                                    *
*   -----                                                                    *
*   client_av.cpp                                                            *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   2000-03-07  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*   Setzt Client- oder Server- AV-Ticket                                     *
*                                                                            *
*****************************************************************************/

#include "ifbslibdef.h"
//#include "fb_av.h"

/*****************************************************************************/
KscAvSimpleModule *CreateClientAV( int &set, PltString &AV) {
/*****************************************************************************/

    static PltString          ticket;
    static KscAvSimpleModule* AVM = 0;

    if( set == 1 ) {
        ticket = AV;
    }

    if(ticket == "") {
        return 0;
    }
    
    if(AVM) delete AVM;
    AVM = new KscAvSimpleModule(ticket);

    return AVM;        
}


/*****************************************************************************/
KS_RESULT SetClientAV( PltString &AV ) {
/*****************************************************************************/

 int set = 1;
 KscAvSimpleModule *AVM = CreateClientAV( set, AV);
 if( (!AVM) && (AV != "") ) {
    return KS_ERR_GENERIC;
 }

 return KS_ERR_OK;        
}


/*****************************************************************************/
KscAvSimpleModule * GetClientAV()
/*****************************************************************************/
{
  PltString AV("");
  int set = 0;
  return CreateClientAV(set, AV);
}


/*****************************************************************************/
KS_RESULT SetServerAV(KscServerBase* Server,
                                              PltString &AV )
/*****************************************************************************/
{
  if(Server == NULL) {
        return KS_ERR_SERVERUNKNOWN;
  }

  PltString path("/");
  
  path += FB_DBINFO;
  path += ".ticket";

  return ifb_SeparateValueAndSet(Server,path,AV,KS_VT_STRING);
}
