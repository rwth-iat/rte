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
*   ifb_aufraeumen.cpp                                                       *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   1999-04-07  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*   Raeumt beim Fehlschlagen des Dienstes 'ImportProject' alle neue          *
*   angelegte Instanzen wieder auf                                           *
*****************************************************************************/

/*
*        Includes
*        --------
*/
#include "ifbslibdef.h"

/*****************************************************************************/
KS_RESULT aufraeumen(KscServerBase*  Server,
                     Dienst_param*   Params,
                     PltString&      out)
/*****************************************************************************/
{
    InstanceItems* del_inst;

    KS_RESULT               res;
    PltString               Str;
    PltList<PltString>      InstList;
    PltList<PltString>      VerbList;
    char                    *ph;
    
    if(!Server)
            return KS_ERR_SERVERUNKNOWN;

    del_inst = Params->Instance;
   
    // Alle zu loeschende Verbindungen und Instanzen merken
    while(del_inst) {
        if(!strncmp(del_inst->Inst_name, "/Cons" , 4) ) {
            // Es ist Verbindung
            VerbList.addLast(del_inst->Inst_name);
        } else {
            // Parts werden mit dem oberen Objekt geloescht.
            ph = del_inst->Inst_name;
            while(ph && (*ph)) ph++;
            while( (ph != del_inst->Inst_name) && (*ph != '/') && (*ph != '.') ) ph--;
            if(*ph != '.') {
                // Es ist Task, Instanz oder Container
                InstList.addLast(del_inst->Inst_name);
            }
        }
        del_inst = del_inst->next;
    }
    
    // Verbindungen ausschalten und loeschen
    TurnComConOff(Server, VerbList);
    DelObjsFromList(Server, VerbList, &out);

        
    // Instanzen aussschalten und loeschen
    TurnInstOff(Server, InstList);
    res = DelObjsFromList(Server, InstList, &out);

    return res;
    
} /* aufraeumen */
