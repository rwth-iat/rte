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
*   ifb_delobj.cpp                                                           *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   1999-04-07  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*   Loescht ein Funktionbausteinobjekt                                       *
*                                                                            *
*****************************************************************************/

#include "ifbslibdef.h"

/*****************************************************************************/
KS_RESULT IFBS_DELETE_OBJ(KscServerBase* Server,
                          PltString      &path)
/*****************************************************************************/
{
    KsDeleteObjParams  objpar;
    KsArray<KsString>  objpath(1);

    KsDeleteObjResult  res;

    if(!Server)
            return KS_ERR_SERVERUNKNOWN;

    objpath[0] = path;

    objpar.paths = objpath;

    bool ok = Server->requestByOpcode ( KS_DELETEOBJECT, GetClientAV(), objpar, res);
    if(!ok) {
        KS_RESULT err = Server->getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
        return err;
    }
    if(res.result) {
            return res.result;
    }
    if(res.results[0]) {
       return res.results[0];
    }

    return KS_ERR_OK;

} /* DeleteObject() */
