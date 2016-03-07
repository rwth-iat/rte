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
*   IFBSpro/Client - iFBSpro  ACPLT/KS Dienste-Schnittstelle (C++)           *
*   ==============================================================           *
*                                                                            *
*   Datei                                                                    *
*   -----                                                                    *
*   ifb_dupl.cpp                                                             *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   2000-01-03  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*                                                                            *
*                                                                            *
*****************************************************************************/

#include "ifbslibdef.h"

/******************************************************************************/
KS_RESULT IFBS_DUPLICATE(KscServerBase*  Server,
                                         PltString       &tu,
                                         PltString       &new_name) {
/******************************************************************************/

    char*     help;       // Hilfsstring
    char*     ph;         // Hilfszeiger
    PltString TU("/");    // Hilfsstring
    PltString NewName;    // Name des anzulegenden Containers
    PltString NewPath;    // Pfad und Name des anzulegenden Containers
    PltString OriPath;    // Pfad und Name des zu kopierenden Containers
    PltString TmpPath;    // Temp-Name des anzulegenden Containers
    PltString RootName;   // Pfad den Containers
    KS_RESULT err;        // Funktions-Rueckmeldung
    
    if( !tu.len() || !new_name.len() ) {
        return KS_ERR_BADPATH;
    }

    int len = strlen(FB_INSTANZ_CONTAINER);

    help = (char*)malloc(tu.len() + 1);
    if(!help) {
        return KS_ERR_TARGETGENERIC;
    }
    
    strcpy(help, (const char*)tu);

    TU += FB_INSTANZ_CONTAINER;
    TU += "/";

    ph = strstr(help, FB_INSTANZ_CONTAINER );

    if( ph ) {
        ph += len;
    } else {
        ph = help;
    }

    if( (*ph) == '/') ph++;
    if( !(*ph) ) {
        free(help);
        return KS_ERR_BADPATH;
    }

    TU += ph;
    free(help);
    
    
    help = (char*)malloc(TU.len() + 1);
    if(!help) {
        return KS_ERR_TARGETGENERIC;
    }
    
    strcpy(help, (const char*)TU);
    ph = help;
    while((*ph) ) ph++;
    while((*ph) != '/' ) ph--;
    *ph = '\0'; ph++;
    RootName = help;
    TU = ph;
    
    free(help);
    
    
    help = (char*)malloc(new_name.len() + 1);
    if(!help) {
        return KS_ERR_TARGETGENERIC;
    }
    
    strcpy(help, (const char*)new_name);
    ph = help;
    while((*ph) ) ph++;
    ph--;
    if((*ph) == '/') *ph = '\0';
    while( (ph != help) && ((*ph) != '/')) ph--;
    if(*ph == '/') ph++;
    NewName = ph;

    free(help);
    
    // Trottel-Sicherung : 
    // Ist ein Container mit dem gleichen Name schon vorhanden ?
    KsGetEPParams params;
    PltList<PltString> erg;
    
    params.path = RootName;
    params.type_mask = KS_OT_ANY;
    params.name_mask = NewName;
    params.scope_flags = KS_EPF_DEFAULT;
    
    err = getChildObjectsList(Server, params, erg);
    if(err) {
        return err;
    }
    while(erg.size() ) {
        if(NewName == erg.removeFirst() ) {
            return KS_ERR_ALREADYEXISTS;
        }
    }
    
    FbDirParams Pars;
    PltString   out;
    
    Pars.path = RootName;
    Pars.path += "/*";    // Container soll rekursiv gesichert werden
    Pars.type_mask = FB_OBJ_ANY;
    Pars.name_mask = TU;

    err = IFBS_SELECT_SAVE(Server, Pars, out);
    if(err) {
        return err;
    }

    // Pfade aufbereiten
    
    OriPath = RootName;
    OriPath += "/";
    OriPath += TU;
    
    TmpPath = RootName;
    TmpPath += "/TEMP_";
    TmpPath += TU;

    NewPath = RootName;
    NewPath += "/";
    NewPath += NewName;
    
    
    // Vorhandenen Container als TEMP_Name umbenennen
    err = IFBS_RENAME(Server, OriPath, TmpPath);
    if(err) {
        return err;
    }

    // Neuen Container anlegen
    err = LoadDupl(Server, out);
    if(err) {
        // Neue angelegten Container loeschen (if not already deleted)
        IFBS_DELETE_FULLCONT(Server, OriPath);

        // Temp-Container umbenennen
        IFBS_RENAME(Server, TmpPath, OriPath);
        return err;
    }
    
    // Neue angelegten Container umbenennen
    err = IFBS_RENAME(Server, OriPath, NewPath);
    if(err) {
        return err;
    }

    // Alten Container umbenennen
    err = IFBS_RENAME(Server, TmpPath, OriPath);
    
    return err;
}


/*****************************************************************************/
KS_RESULT getChildObjectsList( KscServerBase            *Server,
                                                        KsGetEPParams      &params,
                                                        PltList<PltString> &erg)
/*****************************************************************************/
{
    KsGetEPResult  result;

    bool ok = Server->getEP(0, params, result);
    if( !ok ) {
        KS_RESULT err = Server->getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
        return err;
    }
    if( result.result != KS_ERR_OK ) {
        return result.result;
    }

    PltString hStr;

    while (result.items.size() ) {
            KsEngPropsHandle hpp = result.items.removeFirst();
            hStr = (const char*)hpp->identifier;
            erg.addLast(hStr);
    }

    return KS_ERR_OK;

 } /* getChildObjectsList */
