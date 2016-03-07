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
*   ifb_createcomcon.cpp                                                     *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   1999-12-16  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*   Erzeugt eine Verbindung                                                  *
*                                                                            *
*****************************************************************************/

#include "ifbslibdef.h"

/******************************************************************************/
void GenerateComConName(PltString &VerbName)
/******************************************************************************/
{
    PltTime tt = PltTime::now();
    char    ph[2];
    VerbName = "v_";

    while(tt.tv_sec/26) {
        sprintf(ph,"%c", (int)(tt.tv_sec%26 + 97) );
        VerbName += ph;
        tt.tv_sec /=26;
    }
    sprintf(ph,"%c", (int)tt.tv_sec + 97);
    VerbName += ph;

    while(tt.tv_usec/26) {
        sprintf(ph,"%c", (int)(tt.tv_usec%26 + 97) );
        VerbName += ph;
        tt.tv_usec/=26;
    }
    sprintf(ph,"%c", (int)(tt.tv_usec + 97) );
    VerbName += ph;
    
    // Die kleinste Zeit-Aufloesung hat auf unseren Plattformen NT (10 msec)
    // Sollte jemand innerhalb dieser Zeit noch Mal Funktion aufrufen,
    // Wird den gleichen Verb.-Name erzeugt. Um das zu vermeiden, "legen"
    // wir die ganze Ausfuerung fuer den Rest der Zeit "schlafen" :
    
    tt.tv_usec += 10000;            // Zeit-Aufloesung 10 msec
    tt.normalize();                 // no comment
    
    PltTime now(PltTime::now());    // Aktuelle Zeit holen
    if(tt > now) {
         PltTime dif((tt.tv_sec - now.tv_sec) , (tt.tv_usec - now.tv_usec));
         dif.sleep();
    }
        
}

/******************************************************************************/
KS_RESULT IFBS_CREATE_COMCON(KscServerBase* Server,
                             ConData        &pars,
                             Variables      *pVars)
/******************************************************************************/
 {
 
    if(!Server) {
            return KS_ERR_SERVERUNKNOWN;
    }
    KS_RESULT                 err;
    KsArray<KsCreateObjItem>  objitem(1);
    KsArray<KsLinkItem>                  objlinks(2);

    KsCreateObjParams         objpar;
    KsCreateObjResult         res;

    KsLinkParams                          linkpar;

    char*       pv;
    char        v_name[128];
    PltString   VerbName;
    
    if( (pars.identifier).len() ) {
        // Pruefe, ob nur den Name oder den ganzen Pfad uebergeben wurde
        strcpy(v_name, (const char*)pars.identifier);
        pv = v_name;
        while( pv && (*pv) ) pv++;
        while( (pv!=v_name) && ((*pv)!='/') ) pv--;

        if( (*pv) == '/') pv++;

        // Name der Verbindung merken
        VerbName = pv;
        
    } else {
        /* Erzeuge Verbindungsname */
        GenerateComConName(VerbName);
        pars.identifier = VerbName;
    }

    // Class uebergeben?
    if(pars.conclas == "") {
        pars.conclas = CONNECTION_CLASS_PATH;
    }
    
    objitem[0].factory_path = pars.conclas;

    objitem[0].new_path = "/";
    objitem[0].new_path += FB_CONN_CONTAINER;
    objitem[0].new_path += "/";
    objitem[0].new_path += VerbName;

    objitem[0].place.hint = KS_PMH_END;

    /*
    * Setze Source- und TargetPort-Name :
    */

    KsSetVarItem                        *setpar;
    KsVarCurrProps                        *var_props;
    KsStringValue                     *string_var;
    KsBoolValue                     *bool_var;
    PltList<KsSetVarItem*>        hilf;
    size_t                  i;
    
    for( i = 0; i < 2; i++) {
        var_props= new KsVarCurrProps;
        if (!var_props) {
            // Speicher freigeben
            while(hilf.size() ) {
                setpar = hilf.removeFirst();
                delete setpar;
            }
            return OV_ERR_HEAPOUTOFMEMORY;
        }

        switch(i) {
            case 0:
                        string_var = new KsStringValue(pars.source_port);
                        break;
            default:
                        string_var = new KsStringValue(pars.target_port);
                        break;
        }
        if (!string_var) {
            // Speicher freigeben
            while(hilf.size() ) {
                setpar = hilf.removeFirst();
                delete setpar;
            }
            delete var_props;
            return OV_ERR_HEAPOUTOFMEMORY;
        }
    
        var_props->value.bindTo(string_var, PltOsNew);

        setpar = new KsSetVarItem;
        if (!setpar) {
            // Speicher freigeben
            while(hilf.size() ) {
                setpar = hilf.removeFirst();
                delete setpar;
            }
            delete var_props;
            return OV_ERR_HEAPOUTOFMEMORY;
        }
        switch(i) {
            case 0:
                        setpar->path_and_name = ".sourceport";
                        break;
            default:
                        setpar->path_and_name = ".targetport";
                        break;
        }
        setpar->curr_props.bindTo( (KsCurrProps*)var_props, PltOsNew);

        hilf.addLast(setpar);
    }
    
    /*
    *  Setze Betrieb und Richtung
    */

    for( i = 0; i < 2; i++) {
        var_props = new KsVarCurrProps;
        if(!var_props) {
            // Speicher freigeben
            while(hilf.size() ) {
                setpar = hilf.removeFirst();
                delete setpar;
            }
            return OV_ERR_HEAPOUTOFMEMORY;
        }
    
        switch(i) {
            case 0:
                    bool_var = new KsBoolValue(pars.on);
                    break;
            default:
                    bool_var = new KsBoolValue(pars.source_trig);
                    break;
        }
        if(!bool_var) {
            // Speicher freigeben
            while(hilf.size() ) {
                setpar = hilf.removeFirst();
                delete setpar;
            }
            delete var_props;
            return OV_ERR_HEAPOUTOFMEMORY;
        }
        var_props->value.bindTo(bool_var, PltOsNew);

        setpar = new KsSetVarItem;
        if(!setpar) {
                // Speicher freigeben
            while(hilf.size() ) {
                setpar = hilf.removeFirst();
                delete setpar;
            }
            delete var_props;
            return OV_ERR_HEAPOUTOFMEMORY;
        }

        switch(i) {
            case 0:
                    setpar->path_and_name = ".on";
                    break;
            default:
                    setpar->path_and_name = ".sourcetrig";
                    break;
        }
        
        setpar->curr_props.bindTo( (KsCurrProps*)var_props, PltOsNew);

        hilf.addLast(setpar);
    }

    // Basis-Variablen hinzugefuegt. Gibt es extra-Variablen?
    if(pVars) {
        while(pVars) {
            if(pVars->len != 0) {
                if( strcmp(pVars->var_name, "sourceport") &&
                    strcmp(pVars->var_name, "targetport") &&
                    strcmp(pVars->var_name, "on")         &&
                    strcmp(pVars->var_name, "sourcetrig") ) {
        
                    // Neue Variable hinzufuegen
                    setpar = obj_CreateObjectVar(pVars);
                    if(setpar) {
                        hilf.addLast(setpar);
                    }
                }
            }
            pVars = pVars->next;
        }
    }
    
    size_t siz = hilf.size();
    
    KsArray<KsSetVarItem> param(siz);
    if(param.size() != siz) {  
        // Speicher freigeben
        while(hilf.size() ) {
            setpar = hilf.removeFirst();
            delete setpar;
        }
        return OV_ERR_HEAPOUTOFMEMORY;
    }
    // Zu setzende Variablen einfuegen
    for(i = 0; i < siz; i++) {
        setpar = hilf.removeFirst();
        param[i] = *setpar;
        delete setpar;
    }

    objitem[0].parameters = param;

    objlinks[0].link_path = ".sourcefb"; /* /. */
    objlinks[0].element_path = pars.source_fb;
    objlinks[0].place.hint = KS_PMH_END;

    objlinks[1].link_path = ".targetfb"; /* /. */
    objlinks[1].element_path = pars.target_fb;
    objlinks[1].place.hint = KS_PMH_END;

    objitem[0].links = objlinks;

    objpar.items = objitem;

    // Dienst ausfueren
    bool ok = Server->requestByOpcode ( KS_CREATEOBJECT, GetClientAV(), objpar, res);
    if(!ok) {
        err = Server->getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
        return err;
    }
    // Dienst ausgefuert ?
    if(res.result ) {
        // Dienst ist fehlgeschlagen :
        return res.result;
    }
    if(!res.obj_results.size() ) {
        return KS_ERR_GENERIC;
    }
    if( res.obj_results[0].result ) {
        return res.obj_results[0].result;
    }
    size_t AnzResults = res.obj_results[0].param_results.size();
    for(i = 0; i < AnzResults; i++) {
        if(res.obj_results[0].param_results[i]) {
            return res.obj_results[0].param_results[i];
        }
    }
    AnzResults = res.obj_results[0].link_results.size();
    for(i = 0; i < AnzResults; i++) {
        if(res.obj_results[0].link_results[i]) {
            return res.obj_results[0].link_results[i];
        }
    }
    
    return KS_ERR_OK;

}
