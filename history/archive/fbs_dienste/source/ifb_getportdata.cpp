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
*   get_par.cpp                                                              *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   1999-12-20  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*   Gibt die Daten einer Funktionsbausteinvariable zurueck                   *
*                                                                            *
*****************************************************************************/


#include "ifbslibdef.h"

/*****************************************************************************/
KS_RESULT IFBS_GETPORTDATA(
                                                 KscServerBase*     Server
                                                 ,PltString          &port
                                                 ,ObjProps           &res
                                                 ,unsigned long      MaxElem
                                                 )
/*****************************************************************************/
{
    KsGetEPParams  param;
    KsGetEPResult  result;
    char*          ph;
    char*          help;
    KS_RESULT      err = 0;
    PltString      log("");

    if(Server == NULL) {
        err = KS_ERR_SERVERUNKNOWN;
        iFBS_SetLastError(1, err, log);
        return err;
    }
    if( !port.len() ) {
        log = "\"%s\"  \"\"";
        err = KS_ERR_BADNAME;
        iFBS_SetLastError(1, err, log);
        return err;
    }

    iFBS_SetLastError(1, err, log);

    help = (char*)((const char*)port);
    ph = help;

    /* var_name sieht so aus : z.B. /t_u/.../sum1.inp1 */

    while(*ph)
        ph++;  /* Suche Ende des Strings */

    while( (ph!=help) && ((*ph)!='.') && ((*ph)!='/') )
        ph--;  /* Suche  '.'  */

    if( ((*ph)=='/') || (ph == help) ) {
        log = "\"%s\"  \"";
        log += port;
        log += "\"";
        err = KS_ERR_BADPATH;
        iFBS_SetLastError(1, err, log);
        return err;
    }

    /* '.' gefunden, generiere Parameter */
    *ph = '\0';
    param.path = help;
    *ph = '.';
    ph++; /* ph zeigt jetzt auf Variablen-Name selbst */
    param.name_mask = ph;
    param.type_mask = KS_OT_ANY;
    param.scope_flags = KS_EPF_DEFAULT;

    bool ok = Server->getEP(0, param, result);

    if ( !ok ) {
        err = KS_ERR_GENERIC;
    } else {
        if ( result.result != KS_ERR_OK ) {
            err = result.result;
        }
    }

    if(err) {
        log = "\"%s\"  \"";
        log += port;
        log += "\"";
        iFBS_SetLastError(1, err, log);
        return err;
    }

    
    KsString            root = Server->getHostAndName();
    KsString            Var;

    if( !result.items.size() ) {
        log = "\"%s\"  \"";
        log += port;
        log += "\"";
        err = KS_ERR_BADPATH;
        iFBS_SetLastError(1, err, log);
        return err;
    }
    
    KsEngPropsHandle pv = result.items.removeFirst();
    if(!pv) {
        err = KS_ERR_GENERIC;
        iFBS_SetLastError(1, err, log);
        return err;
    }

    Var = param.path;
    Var += ".";
    Var += pv->identifier;  /* /. */

    KscVariable var(root+Var);

    if (!var.getUpdate() ) {
        err = var.getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
        log = "\"%s\"  \"";
        log += port;
        log += "\"";
        iFBS_SetLastError(1, err, log);
        return err;
    }

    const KsVarCurrProps *cp = var.getCurrProps();
    if(!cp) {
        err = KS_ERR_GENERIC;
        iFBS_SetLastError(1, err, log);
        return err;
    }
    
    GetPortProps(pv, cp, res, MaxElem);
    return KS_ERR_OK;
}

/*****************************************************************************/
void GetPortProps(
                   KsEngPropsHandle      &hep
                   ,const KsVarCurrProps *cp
                   ,ObjProps             &item
                   ,unsigned long        MaxElem          /* Max. Anzahl Elementen in Array */
                  )
/*****************************************************************************/
{

    if(!hep) {
        // FEHLER : Handle nicht vorhanden ?
        return;
    }
    if(!cp || !cp->value) {
        // FEHLER : NULL-Zeiger.
        return;
    }

    item.identifier    = hep->identifier;
    item.comment       = hep->comment;
    item.access        = hep->access_mode;
    item.flags         = hep->semantic_flags;
    item.state         = cp->state;
    item.var_type      = cp->value->xdrTypeCode();


    item.class_ident = "KS_VT_";
    item.class_ident += ifb_getPortValueType(cp->value->xdrTypeCode());
    item.len = ifb_getValueLengthAsNum(cp);
    
    item.value = "";
    ifb_getValueOnly(cp                    /* >|  Eigenschaften der Variable            */
                    ,item.value            /* >|> Value als String                      */
                    ,MaxElem               /* Max. Anzahl Elementen in Array            */
                    ,FALSE                 /* Merker, ob Time-Ausgabe "... hh:mm:ss"    */
                    ,FALSE                 /* Merker, ob TRUE als "1" und FALSE als "0" */
                    ,FALSE                 /* Merker, ob Array als Liste "{...}"        */
                    );
    
    switch(hep->xdrTypeCode() ) {
        
        case KS_OT_VARIABLE:
                            item.obj_type      = FB_OBJ_VARIABLE;
                                item.tech_unit     = ((KsVarEngProps&)(*hep)).tech_unit;
                            item.creation_time = cp->time;
                            // Dynamic oder PV?
                            item.class_ident += ifb_getPortCharacteristic(item.flags);
                            
                            break;
        case KS_OT_LINK:
                            item.obj_type      = FB_OBJ_LINK;
                            item.tech_unit     = "";
                            item.creation_time = ((KsLinkEngProps&)(*hep)).creation_time;
                            
                            break;
        default :
                 break;
    }

    return;
}
