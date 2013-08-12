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
*   ifb_dbsaveinxml.cpp                                                      *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   2002-04-22  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*                                                                            *
*                                                                            *
*****************************************************************************/

#include "ifbslibdef.h"
#include "ks/conversions.h"

/*
*   Definitionen
*/
#define DEPTH_STRING "    "
#define VALUE_TAG_ANF "<value>"
#define VALUE_TAG_END "</value>"

KS_RESULT get_xmlobjs(KscServerBase* Server, PltString &path
                      ,PltString&    Out,    PltTime &wTim
                      ,FILE *fout,           size_t nDepth = 1);
/*
*   Globale Variable
*/
// Anzahl wiederholungen
long int retries_for_ia_fox;

// Debug-Ausgaben
int debug = 0;

// Gesichertere Objekte
unsigned int VarSaved;
unsigned int DomSaved;

// Liste der nicht gesicherten Objekten
PltList<PltString> VarNotSaved;
PltList<PltString> DomNotSaved;

/*****************************************************************************/
/* Hilfs-Fnc
*  ---------
*  Formatierte Ausgabe der Zeit
******************************************************************************/
char* ifb___xmlFormatTime(                                                 //  |> Funktionsrueckmeldung
                                               KsTime    &tim        // >|  Ks-Zeit
                                             )
{
    struct tm* t;
    time_t timer;
    static char help[32];
    
    timer = (time_t)tim.tv_sec;
    t = localtime(&timer);

    if(t) {
        sprintf(help, "%4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d",
                        t->tm_year+1900, t->tm_mon+1, t->tm_mday,
                        t->tm_hour, t->tm_min, t->tm_sec );
    } else {
        strcpy(help, "1970-01-01 00:00:00");
    }
    
    return help;
}

/*****************************************************************************/
/* Hilfs-Fnc
*  ---------
*  Objekt-Typ im Klartext schreiben
******************************************************************************/
char* ifb___xmlObjType(                                                     //  |> Funktionsrueckmeldung
                                           KS_OBJ_TYPE     type        // >|  Objekt-Typ
                                          )
{
    switch(type) {
        case KS_OT_DOMAIN    : return "DOM";     // object is a domain
        case KS_OT_VARIABLE  : return "VAR";     // object is a variable
        case KS_OT_HISTORY   : return "HIST";    // object is a history
        case KS_OT_LINK      : return "LINK";    // object is a link
        case KS_OT_STRUCTURE : return "STRUCT";  // object is a structure
        default :              return "UNKNOWN"; // unknown object type
    }
}

/*****************************************************************************/
/* Hilfs-Fnc
*  ---------
*  Zugriffs-Rechte im Klartext schreiben
******************************************************************************/
char* ifb___xmlAccessMode(                                                           //  |> Funktionsrueckmeldung
                                                  KS_ACCESS    mode                // >|  Access mode
                                                 )
{
        /*
        *        Lokale Variablen
        */
        static char   ModeString[32];

        if(mode == KS_AC_NONE) {
                return "";                         // no access at all
        }
        
    sprintf(ModeString, "%s%s%s%s%s%s%s"
                ,(mode & KS_AC_READ)         ? "R" : ""          // read access
                ,(mode & KS_AC_WRITE)        ? "W" : ""          // write access
                ,(mode & KS_AC_DELETEABLE)   ? "D" : ""          // delete access
                ,(mode & KS_AC_RENAMEABLE)   ? "N" : ""          // rename access
                ,(mode & KS_AC_LINKABLE)     ? "L" : ""          // link access
                ,(mode & KS_AC_UNLINKABLE)   ? "U" : ""          // unlink access
                ,(mode & KS_AC_INSTANTIABLE) ? "I" : ""          // object can act as a factory
        );
        
        return ModeString;
}

/*****************************************************************************/
/* Hilfs-Fnc
*  ---------
*  Semantics-Flags im Klartext schreiben
******************************************************************************/
PltString ifb___xmlSemanticFlags(                                                     //  |> Funktionsrueckmeldung
                                                        long    Flags                // >|  Flags
                                                        ) {
        
    PltString help = "";
    char cr[32];
    char i;
    
    if(Flags != 0) {
        for(i = 'a'; i <= 'z'; i++) {
            if(IsFlagSet(Flags, i) ) {
                sprintf(cr, "%c", i);
                help += cr;
            }
        }
    }

    return help;
}

/*****************************************************************************/
/* Hilfs-Fnc
*  ---------
*  Link-Typ im Klartext schreiben
******************************************************************************/
char* ifb___xmlLinkType(                                                     //  |> Funktionsrueckmeldung
                                        KS_LINK_TYPE    type             // >|  Link-Typ
                                        )
{
        switch(type) {
        case KS_LT_LOCAL_1_1        : return "KS_LT_LOCAL_1_1";        // local   link 1:1
        case KS_LT_LOCAL_1_MANY     : return "KS_LT_LOCAL_1_MANY";     // local   link 1:n
        case KS_LT_LOCAL_MANY_MANY  : return "KS_LT_LOCAL_MANY_MANY";  // local   link n:m
        case KS_LT_LOCAL_MANY_1     : return "KS_LT_LOCAL_MANY_1";     // local   link n:1
        case KS_LT_GLOBAL_1_1       : return "KS_LT_GLOBAL_1_1";       // global  link 1:1
        case KS_LT_GLOBAL_1_MANY    : return "KS_LT_GLOBAL_1_MANY";    // global  link 1:n
        case KS_LT_GLOBAL_MANY_MANY : return "KS_LT_GLOBAL_MANY_MANY"; // global  link n:m
        case KS_LT_GLOBAL_MANY_1    : return "KS_LT_GLOBAL_MANY_1";    // global  link n:1
        default                     : return "UNKNOWN";                // unknown link type
        }
}

/*****************************************************************************/
/* Hilfs-Fnc
*  ---------
*  Typ des History-Objektes im Klartext schreiben
******************************************************************************/
char* ifb___xmlHistType(                                                     //  |> Funktionsrueckmeldung
                                        KS_HIST_TYPE    type             // >|  Typ des History-Objektes
                                        )
{
        switch(type) {
        case KS_HT_LOG           : return "KS_HT_LOG";           // logging history
        case KS_HT_TABLE         : return "KS_HT_TABLE";                 // table history
        case KS_HT_BOOL          : return "KS_HT_BOOL";                         // bool history
        case KS_HT_INT           : return "KS_HT_INT";           // integer history
        case KS_HT_UINT          : return "KS_HT_UINT";          // unsigned int history
        case KS_HT_SINGLE        : return "KS_HT_SINGLE";        // single history
        case KS_HT_DOUBLE        : return "KS_HT_DOUBLE";        // double history
        case KS_HT_STRING        : return "KS_HT_STRING";        // string history
        case KS_HT_TIME          : return "KS_HT_TIME";          // time history
        case KS_HT_TIME_SPAN     : return "KS_HT_TIME_SPAN";     // time span history
        case KS_HT_TYPE_MASK     : return "KS_HT_TYPE_MASK";     // type mask history
        case KS_HT_TIME_DRIVEN   : return "KS_HT_TIME_DRIVEN";   // time driven history
        case KS_HT_CHANGE_DRIVEN : return "KS_HT_CHANGE_DRIVEN"; // change driven history
        default                  : return "UNKNOWN";             // unknown history type
        }
}

/*****************************************************************************/
/* Hilfs-Fnc
*  ---------
*  Interpolation-Modis des History-Objektes im Klartext schreiben
******************************************************************************/
PltString ifb___xmlHistInterpMode(                                                        //  |> Funktionsrueckmeldung
                                                                KS_INTERPOLATION_MODE  mode   // >|  Interp. Mode des History-Objektes
                                                                )
{
#define MODE_TRENNER " | "
        /*
        *        Lokale Variablen
        */
        PltString   ModeString("");
        
        if(mode == KS_IPM_NONE)  {
                ModeString = "KS_IPM_NONE";                        // keine interpolation
        } else {
                
                if(mode & KS_IPM_LINEAR) {
                        ModeString = "KS_IPM_LINEAR";        // liniare Interpolation
                }
                if(mode & KS_IPM_MIN) {
                    if(ModeString != "") {
                        ModeString += MODE_TRENNER;
                    }
                        ModeString += "KS_IPM_MIN";                // Interpolation Min.-Werte
                }
                if(mode & KS_IPM_MAX) {
                    if(ModeString != "") {
                        ModeString += MODE_TRENNER;
                    }
                        ModeString += "KS_IPM_MAX";                // Interpolation Max.-Werte 
                }
                if(mode & KS_IPM_HOLD) {
                    if(ModeString != "") {
                        ModeString += MODE_TRENNER;
                    }
                        ModeString += "KS_IPM_HOLD";                // letzter Wert
                }
                if(mode & KS_IPM_DEFAULT) {
                    if(ModeString != "") {
                        ModeString += MODE_TRENNER;
                    }
                        ModeString += "KS_IPM_DEFAULT";        // default-Interpolation
                }
        }

        return ModeString;
}

/*****************************************************************************/
/* Hilfs-Fnc
*  ---------
*  Status als String
******************************************************************************/
char* ifb_getState(KS_STATE status)
{
    static char stat[64];
    
    sprintf(stat, "%d", status);
    
    return stat;
}

/*****************************************************************************/
/* Hilfs-Fnc
*  ---------
*  Sonder-Zeichen in String convertieren
******************************************************************************/
KS_RESULT xml_fixValue(PltString &Val) {
/*****************************************************************************/


    char*  pc1;
    char*  pc2;
    //char*  ph;
    char*  po;
    size_t anz=0;
    
    if(!Val.len() ) return KS_ERR_OK;

    // Hilfsstring anlegen
//    ph = (char*)malloc(Val.len() + 1);
//    if(!ph) return OV_ERR_HEAPOUTOFMEMORY;
    // String fuer weitere Bearbeitung kopieren
//    strcpy(ph, (const char*)Val);
//    pc1 = ph;

    pc1 = (char*)((const char*)Val);
    
    while( pc1 && (*pc1) ) {
        
        switch(*pc1) {
            
#if NUR_TAG_ZIECHEN_CODIEREN
            case '<':
            case '>':
            case '&':
            case '"':
#else
                case 'À':
                case 'à':
                case 'Á':
                case 'á':
                case 'Â':
                case 'â':
                case 'Ã':
                case 'ã':
                case 'Ä':
                case 'ä':
                case 'Å':
                case 'å':
                case 'Æ':
                case 'ª':
                case 'Ç':
                case 'ç':
                case 'È':
                case 'è':
                case 'É':
                case 'é':
                case 'Ê':
                case 'ê':
                case 'Ë':
                case 'ë':
                case 'Ì':
                case 'ì':
                case 'Í':
                case 'í':
                case 'Î':
                case 'î':
                case 'Ï':
                case 'ï':
                case 'Ð':
                case 'Ñ':
                case 'Ò':
                case 'ò':
                case 'Ó':
                case 'ó':
                case 'Ô':
                case 'ô':
                case 'Õ':
                case 'õ':
                case 'Ö':
                case 'ö':
                case 'Ø':
                case 'Ù':
                case 'ù':
                case 'Û':
                case 'û':
                case 'Ü':
                case 'ü':
                case 'Ú':
                case 'ú':
                case '×':
                case '¡':
                case '¢':
                case '£':
                case '¤':
                case '¥':
                case 'Ý':
                case '¦':
                case 'Þ':
                case '§':
                case 'ß':
                case '¨':
                case '©':
                case '«':
                case '¬':
                case '­':
                case '®':
                case 'æ':
                case '¯':
                case '°':
                case '±':
                case '²':
                case '³':
                case '´':
                case 'µ':
                case '¶':
                case '·':
                case '¸':
                case 'ð':
                case '¹':
                case 'ñ':
                case 'º':
                case '»':
                case '¼':
                case '½':
                case '¾':
                case '¿':
                case '÷':
                case 'ø':
                case '<':
                case '>':
                case 'ý':
                case 'þ':
                case 'ÿ':
#endif
                anz++;
            default:
                break;
        }
        pc1++;
    }
    
//    pc1 = ph;  // An Anfang setzen
    pc1 = (char*)((const char*)Val);
    
    if(anz) {
        // Dann bearbeite String
        po = (char*)malloc(Val.len() + anz*6 + 1);
        if(!po) {
//            free(ph);
            return OV_ERR_HEAPOUTOFMEMORY;
        }
        pc2 = po;
    
        while( pc1 && (*pc1) ) {
            switch(*pc1) {

#if NUR_TAG_ZIECHEN_CODIEREN
                
                case '<':
                    *pc2 = '&'; pc2++;
                    *pc2 = 'l'; pc2++;
                    *pc2 = 't'; pc2++;
                    *pc2 = ';';
                    break;
                case '>':
                    *pc2 = '&'; pc2++;
                    *pc2 = 'g'; pc2++;
                    *pc2 = 't'; pc2++;
                    *pc2 = ';';
                    break;
                case '&':
                    *pc2 = '&'; pc2++;
                    *pc2 = 'a'; pc2++;
                    *pc2 = 'm'; pc2++;
                    *pc2 = 'p'; pc2++;
                    *pc2 = ';';
                    break;
                case '"':
                    *pc2 = '&'; pc2++;
                    *pc2 = 'q'; pc2++;
                    *pc2 = 'u'; pc2++;
                    *pc2 = 'o'; pc2++;
                    *pc2 = 't'; pc2++;
                    *pc2 = ';';
                    break;
#else
                case 'À':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '9'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = ';';
                            break;
                case 'à':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '4'; pc2++;
                            *pc2 = ';';
                            break;
                case 'Á':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '9'; pc2++;
                            *pc2 = '3'; pc2++;
                            *pc2 = ';';
                            break;
                case 'á':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '5'; pc2++;
                            *pc2 = ';';
                            break;
                case 'Â':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '9'; pc2++;
                            *pc2 = '4'; pc2++;
                            *pc2 = ';';
                            break;
                case 'â':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '6'; pc2++;
                            *pc2 = ';';
                            break;
                case 'Ã':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '9'; pc2++;
                            *pc2 = '5'; pc2++;
                            *pc2 = ';';
                            break;
                case 'ã':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '7'; pc2++;
                            *pc2 = ';';
                            break;
                case 'Ä':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '9'; pc2++;
                            *pc2 = '6'; pc2++;
                            *pc2 = ';';
                            break;
                case 'ä':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '8'; pc2++;
                            *pc2 = ';';
                            break;
                case 'Å':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '9'; pc2++;
                            *pc2 = '7'; pc2++;
                            *pc2 = ';';
                            break;
                case 'å':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '9'; pc2++;
                            *pc2 = ';';
                            break;
                case 'Æ':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '9'; pc2++;
                            *pc2 = '8'; pc2++;
                            *pc2 = ';';
                            break;
                case 'ª':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '7'; pc2++;
                            *pc2 = '0'; pc2++;
                            *pc2 = ';';
                            break;
                case 'Ç':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '9'; pc2++;
                            *pc2 = '9'; pc2++;
                            *pc2 = ';';
                            break;
                case 'ç':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '3'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = ';';
                            break;
                case 'È':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '0'; pc2++;
                            *pc2 = '0'; pc2++;
                            *pc2 = ';';
                            break;
                case 'è':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '3'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = ';';
                            break;
                case 'É':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '0'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = ';';
                            break;
                case 'é':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '3'; pc2++;
                            *pc2 = '3'; pc2++;
                            *pc2 = ';';
                            break;
                case 'Ê':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '0'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = ';';
                            break;
                case 'Ë':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '0'; pc2++;
                            *pc2 = '3'; pc2++;
                            *pc2 = ';';
                            break;
                case 'ë':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '3'; pc2++;
                            *pc2 = '5'; pc2++;
                            *pc2 = ';';
                            break;
                case 'ê':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '3'; pc2++;
                            *pc2 = '4'; pc2++;
                            *pc2 = ';';
                            break;
                case 'Ì':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '0'; pc2++;
                            *pc2 = '4'; pc2++;
                            *pc2 = ';';
                            break;
                case 'ì':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '3'; pc2++;
                            *pc2 = '6'; pc2++;
                            *pc2 = ';';
                            break;
                case 'Í':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '0'; pc2++;
                            *pc2 = '5'; pc2++;
                            *pc2 = ';';
                            break;
                case 'í':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '3'; pc2++;
                            *pc2 = '7'; pc2++;
                            *pc2 = ';';
                            break;
                case 'Î':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '0'; pc2++;
                            *pc2 = '6'; pc2++;
                            *pc2 = ';';
                            break;
                case 'î':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '3'; pc2++;
                            *pc2 = '8'; pc2++;
                            *pc2 = ';';
                            break;
                case 'Ï':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '0'; pc2++;
                            *pc2 = '7'; pc2++;
                            *pc2 = ';';
                            break;
                case 'ï':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '3'; pc2++;
                            *pc2 = '9'; pc2++;
                            *pc2 = ';';
                            break;
                case 'Ð':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '0'; pc2++;
                            *pc2 = '8'; pc2++;
                            *pc2 = ';';
                            break;
                case 'Ñ':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '0'; pc2++;
                            *pc2 = '9'; pc2++;
                            *pc2 = ';';
                            break;
                case 'Ò':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '0'; pc2++;
                            *pc2 = ';';
                            break;
                case 'ò':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '4'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = ';';
                            break;
                case 'Ó':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = ';';
                            break;
                case 'ó':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '4'; pc2++;
                            *pc2 = '3'; pc2++;
                            *pc2 = ';';
                            break;
                case 'Ô':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = ';';
                            break;
                case 'ô':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '4'; pc2++;
                            *pc2 = '4'; pc2++;
                            *pc2 = ';';
                            break;
                case 'Õ':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '3'; pc2++;
                            *pc2 = ';';
                            break;
                case 'õ':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '4'; pc2++;
                            *pc2 = '5'; pc2++;
                            *pc2 = ';';
                            break;
                case 'Ö':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '4'; pc2++;
                            *pc2 = ';';
                            break;
                case 'ö':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '4'; pc2++;
                            *pc2 = '6'; pc2++;
                            *pc2 = ';';
                            break;
                case 'Ø':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '6'; pc2++;
                            *pc2 = ';';
                            break;
                case 'Ù':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '7'; pc2++;
                            *pc2 = ';';
                            break;
                case 'ù':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '4'; pc2++;
                            *pc2 = '9'; pc2++;
                            *pc2 = ';';
                            break;
                case 'Û':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '9'; pc2++;
                            *pc2 = ';';
                            break;
                case 'û':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '5'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = ';';
                            break;
                case 'Ü':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '0'; pc2++;
                            *pc2 = ';';
                            break;
                case 'ü':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '5'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = ';';
                            break;
                case 'Ú':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '8'; pc2++;
                            *pc2 = ';';
                            break;
                case 'ú':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '5'; pc2++;
                            *pc2 = '0'; pc2++;
                            *pc2 = ';';
                            break;
                case '×':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '5'; pc2++;
                            *pc2 = ';';
                            break;
                case '¡':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '6'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = ';';
                            break;
                case '¢':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '6'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = ';';
                            break;
                case '£':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '6'; pc2++;
                            *pc2 = '3'; pc2++;
                            *pc2 = ';';
                            break;
                case '¤':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '6'; pc2++;
                            *pc2 = '4'; pc2++;
                            *pc2 = ';';
                            break;
                case '¥':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '6'; pc2++;
                            *pc2 = '5'; pc2++;
                            *pc2 = ';';
                            break;
                case 'Ý':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = ';';
                            break;
                case '¦':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '6'; pc2++;
                            *pc2 = '6'; pc2++;
                            *pc2 = ';';
                            break;
                case 'Þ':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = ';';
                            break;
                case '§':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '6'; pc2++;
                            *pc2 = '7'; pc2++;
                            *pc2 = ';';
                            break;
                case 'ß':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '3'; pc2++;
                            *pc2 = ';';
                            break;
                case '¨':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '6'; pc2++;
                            *pc2 = '8'; pc2++;
                            *pc2 = ';';
                            break;
                case '©':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '6'; pc2++;
                            *pc2 = '9'; pc2++;
                            *pc2 = ';';
                            break;
                case '«':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '7'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = ';';
                            break;
                case '¬':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '7'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = ';';
                            break;
                case '­':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '7'; pc2++;
                            *pc2 = '3'; pc2++;
                            *pc2 = ';';
                            break;
                case '®':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '7'; pc2++;
                            *pc2 = '4'; pc2++;
                            *pc2 = ';';
                            break;
                case 'æ':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '3'; pc2++;
                            *pc2 = '0'; pc2++;
                            *pc2 = ';';
                            break;
                case '¯':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '7'; pc2++;
                            *pc2 = '5'; pc2++;
                            *pc2 = ';';
                            break;
                case '°':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '7'; pc2++;
                            *pc2 = '6'; pc2++;
                            *pc2 = ';';
                            break;
                case '±':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '7'; pc2++;
                            *pc2 = '7'; pc2++;
                            *pc2 = ';';
                            break;
                case '²':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '7'; pc2++;
                            *pc2 = '8'; pc2++;
                            *pc2 = ';';
                            break;
                case '³':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '7'; pc2++;
                            *pc2 = '9'; pc2++;
                            *pc2 = ';';
                            break;
                case '´':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '8'; pc2++;
                            *pc2 = '0'; pc2++;
                            *pc2 = ';';
                            break;
                case 'µ':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '8'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = ';';
                            break;
                case '¶':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '8'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = ';';
                            break;
                case '·':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '8'; pc2++;
                            *pc2 = '3'; pc2++;
                            *pc2 = ';';
                            break;
                case '¸':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '8'; pc2++;
                            *pc2 = '4'; pc2++;
                            *pc2 = ';';
                            break;
                case 'ð':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '4'; pc2++;
                            *pc2 = '0'; pc2++;
                            *pc2 = ';';
                            break;
                case '¹':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '8'; pc2++;
                            *pc2 = '5'; pc2++;
                            *pc2 = ';';
                            break;
                case 'ñ':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '4'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = ';';
                            break;
                case 'º':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '8'; pc2++;
                            *pc2 = '6'; pc2++;
                            *pc2 = ';';
                            break;
                case '»':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '8'; pc2++;
                            *pc2 = '7'; pc2++;
                            *pc2 = ';';
                            break;
                case '¼':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '8'; pc2++;
                            *pc2 = '8'; pc2++;
                            *pc2 = ';';
                            break;
                case '½':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '8'; pc2++;
                            *pc2 = '9'; pc2++;
                            *pc2 = ';';
                            break;
                case '¾':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '9'; pc2++;
                            *pc2 = '0'; pc2++;
                            *pc2 = ';';
                            break;
                case '¿':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = '9'; pc2++;
                            *pc2 = '1'; pc2++;
                            *pc2 = ';';
                            break;
                case '÷':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '4'; pc2++;
                            *pc2 = '7'; pc2++;
                            *pc2 = ';';
                            break;
                case 'ø':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '4'; pc2++;
                            *pc2 = '8'; pc2++;
                            *pc2 = ';';
                            break;
                case '<':
                            *pc2 = '&'; pc2++;
                            *pc2 = 'l'; pc2++;
                            *pc2 = 't'; pc2++;
                            *pc2 = ';';
                            break;
                case '>':
                            *pc2 = '&'; pc2++;
                            *pc2 = 'g'; pc2++;
                            *pc2 = 't'; pc2++;
                            *pc2 = ';';
                            break;
                case 'ý':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '5'; pc2++;
                            *pc2 = '3'; pc2++;
                            *pc2 = ';';
                            break;
                case 'þ':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '5'; pc2++;
                            *pc2 = '4'; pc2++;
                            *pc2 = ';';
                            break;
                case 'ÿ':
                            *pc2 = '&'; pc2++;
                            *pc2 = '#'; pc2++;
                            *pc2 = '2'; pc2++;
                            *pc2 = '5'; pc2++;
                            *pc2 = '5'; pc2++;
                            *pc2 = ';';
                            break;             
#endif
                default:
                    // Zeichen uebertragen
                    *pc2 = *pc1;
                    break;
            }
            pc1++;
            pc2++;
        }
        // String-Ende markieren
        *pc2 = '\0';
            
        // Geaenderten String zurueckgeben
        Val = po;
        
        // Speicher freigeben
        free(po);
    }
    
    // Speicher freigeben
//    free(ph);
    
    return KS_ERR_OK;

}

/*****************************************************************************/
KS_RESULT ifb_getxmlValueOnly(
    const KsVarCurrProps* cp        /* >|  Eigenschaften der Variable  */
    ,PltString            &Out      /* >|> Value als String            */
    ,PltString            &Depth    /* >|  Einrueckung                 */
)
/*****************************************************************************/
{
    PltString Val("");          // Value der Variable als String
    KsString  KsValue;          // Hilfsstring
    PltString ArrTren("");      // Trenner der Array-Elementen
    KS_RESULT err;              // Funktions-Rueckmeldung
    char      help[128];        // Hilfsstring
    size_t    size;             // Vektor-Laenge
    size_t    i;                // Laufvariable

    // Variablen fuer Zeit-Konvertierung
    KsTime         zeit;
    KsTimeSpan     zeitsp;
    struct         tm* t;
    time_t         timer;

    // Array-Trenner
    ArrTren = "\n";
    ArrTren += Depth;
    ArrTren += DEPTH_STRING;
    ArrTren += VALUE_TAG_ANF;
   
    if( (!cp) || (!cp->value) ) {
        ArrTren += VALUE_TAG_END;
        Out += ArrTren;
        return KS_ERR_OK;
    }
    
    switch(cp->value->xdrTypeCode() ) {
         case KS_VT_VOID:
         case KS_VT_BOOL:
         case KS_VT_INT:
         case KS_VT_UINT:
         case KS_VT_SINGLE:
         case KS_VT_DOUBLE:
         case KS_VT_TIME:
         case KS_VT_TIME_SPAN:
                                Out += ArrTren;
                                err = ifb_getValueOnly( /*  |> Funktionsrueckmeldung         */
                                        cp              /* >|  Eigenschaften der Variable            */
                                        ,Out            /* >|> Value als String                      */
                                        );
                                if(err) return err;
                                Out += VALUE_TAG_END;
                                break;
         case KS_VT_STRING:
                                Out += ArrTren;

                                KsValue = (KsStringValue &) *cp->value;
                                Val = (const char*)KsValue;
                                err = xml_fixValue(Val);
                                if(err) return err;
                                
                                Out += Val;
                                Out += VALUE_TAG_END;
                                break;
                                
         case KS_VT_BYTE_VEC:
                        size = ((KsByteVecValue &) *cp->value).size();
                        if (size ==1) {
                    
                            sprintf(help,"%d",
                                             ((KsByteVecValue &)* cp->value)[0]);

                            Out += ArrTren;
                            Out += help;
                            Out += VALUE_TAG_END;
                        } else {
                            Out += ArrTren;
                            for ( i = 0; i < size; i++ ) {
                                sprintf(help,"%d", ((KsByteVecValue &) *cp->value)[i]);
                                Out += help;
                                if(i < (size-1) ) {
                                    Out += VALUE_TAG_END;
                                    Out += ArrTren;
                                }
                            }
                            Out += VALUE_TAG_END;
                        }
                        break;
         case KS_VT_BOOL_VEC:
                        size = ((KsBoolVecValue &) *cp->value).size();
                        Out += ArrTren;
                        for ( i = 0; i < size; i++ ) {
                                                if( (bool) ((KsBoolVecValue &) *cp->value)[i])
                                    Out += "TRUE";
                                                else
                                                        Out += "FALSE";
                                        
                            if(i < (size-1) ) {
                                Out += VALUE_TAG_END;
                                Out += ArrTren;
                            }
                        }
                        Out += VALUE_TAG_END;
                        break;
         case KS_VT_INT_VEC:
                        size = ((KsIntVecValue &) *cp->value).size();
                        Out += ArrTren;
                        for ( i = 0; i < size; i++ ) {
                            sprintf(help,"%ld",((KsIntVecValue &) *cp->value)[i]);
                            Out += help;
                            if(i < (size-1))  {
                                Out += VALUE_TAG_END;
                                Out += ArrTren;
                            }
                        }
                        Out += VALUE_TAG_END;

                        break;
         case KS_VT_UINT_VEC:
                        size = ((KsUIntVecValue &) *cp->value).size();
                        Out += ArrTren;
                        for ( i = 0; i < size; i++ ) {
                            sprintf(help,"%lu",((KsUIntVecValue &) *cp->value)[i]);
                            Out += help;
                            if(i < (size-1) ) {
                                Out += VALUE_TAG_END;
                                Out += ArrTren;
                            }
                        }
                        Out += VALUE_TAG_END;
                        break;
         case KS_VT_SINGLE_VEC:
                        size = ((KsSingleVecValue &) *cp->value).size();
                        Out += ArrTren;
                        for ( i = 0; i < size; i++ ) {
                            sprintf(help,"%#f",((KsSingleVecValue &) *cp->value)[i]);
                            KsValue = help;
                            fixFloatValue(KsValue);
                            Out += (const char*)KsValue;
                            if(i < (size-1) ) {
                                Out += VALUE_TAG_END;
                                Out += ArrTren;
                            }
                        }
                        Out += VALUE_TAG_END;
                        break;
         case KS_VT_DOUBLE_VEC:
                        size = ((KsDoubleVecValue &) *cp->value).size();
                        Out += ArrTren;
                        for ( i = 0; i < size; i++ ) {
                            sprintf(help, "%#f",(float)((KsDoubleVecValue &) *cp->value)[i]);
                            KsValue = help;
                            fixFloatValue(KsValue);
                            Out += (const char*)KsValue;
                            if(i < (size-1)) {
                                Out += VALUE_TAG_END;
                                Out += ArrTren;
                            }
                        }
                        Out += VALUE_TAG_END;
                        break;
         case KS_VT_STRING_VEC:
                        size = ((KsStringVecValue &) *cp->value).size();
                        Out += ArrTren;
                        for ( i = 0; i < size; i++ ) {
                            KsValue = ((KsStringVecValue &) *cp->value)[i];
                            Val = (const char*)KsValue;
                            err = xml_fixValue(Val);
                            if(err) return err;
                            Out += Val;
                            if(i < (size-1) ) {
                                Out += VALUE_TAG_END;
                                Out += ArrTren;
                            }
                        }
                        Out += VALUE_TAG_END;
                        break;
         case KS_VT_TIME_VEC:
                        size = ((KsTimeVecValue &) *cp->value).size();
                        Out += ArrTren;
                        for ( i = 0; i < size; i++ ) {
                            zeit = ((KsTimeVecValue &) *cp->value)[i];
                                            timer = (time_t)zeit.tv_sec;
                                            t=localtime(&timer);
                                    
                            if(!t) {
                                Val = "1970-01-01 00:00:00.000000";
                            } else {
                                sprintf(help,"%4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d.%6.6d",
                                                                t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min,
                                                                t->tm_sec, (int)zeit.tv_usec );
                                Val = help;
                            }
                            Out += Val;
                            if(i < (size-1) ) {
                                Out += VALUE_TAG_END;
                                Out += ArrTren;
                            }
                        }
                        Out += VALUE_TAG_END;
                        break;
         case KS_VT_TIME_SPAN_VEC:
                        size = ((KsTimeSpanVecValue &) *cp->value).size();
                        Out += ArrTren;
                        for ( i = 0; i < size; i++ ) {
                            zeitsp = ((KsTimeSpanVecValue &) *cp->value)[i];
                            zeitsp.normalize();
                            sprintf(help,"%ld.%6.6ld", zeitsp.tv_sec, zeitsp.tv_usec );
                            Out += help;
                            if(i < (size-1) ) {
                                Out += VALUE_TAG_END;
                                Out += ArrTren;
                            }
                        }
                        Out += VALUE_TAG_END;
                        break;
        default:
                        return KS_ERR_BADTYPE;
    } /* switch */
                
    return KS_ERR_OK;
}

/*****************************************************************************/
 KS_RESULT  get_xmlValue(KscServerBase* Server
                        , KsString      &path
                        , PltList<KsEngPropsHandle> &ListEP
                        , PltList<PltString>        &VarValues
                        , PltList<KS_STATE>         &VarStates
                        , PltList<KS_VAR_TYPE>      &VarTypes
                        , PltString                 &Depth)
/*****************************************************************************/
{    
    PltString Out("");                          // Hilfsstring
    size_t    anzVars;                          // Merker : Anzahl der Variablen
    size_t    i;                                // Laufvariable
    long int  upd;                              // Laufvariable
    KsString  root;                             // Hilfsstring : Host/Server
    KsString  Var;                              // Hilfsstring : Pfad der Variable
    KS_RESULT err;                              // Merker : Fehler
    
    anzVars = ListEP.size();
    if(!anzVars) {
        // Keine Variablen vorhanden
        return KS_ERR_OK;
    }
    
    /* Alle KS-Variable vermerken */
    PltArray<KscVariable*> VarArray(anzVars);
    if(VarArray.size() != anzVars) {
            return OV_ERR_HEAPOUTOFMEMORY;
    }

    // Host und Server-Namen merken
    root = Server->getHostAndName();

    // Package anlegen
    KscPackage        *pkg = new KscPackage;
    if(!pkg) {
        return OV_ERR_HEAPOUTOFMEMORY;
    }

    for(i = 0; i < anzVars; i++) {
            KsEngPropsHandle pv(ListEP.removeFirst());
        if(!pv) {
            
            // Debug-Ausgabe
            if(debug) {
                fprintf(stderr, "%s%sFEHLER : Can't remove props from EP-list\n", (const char*)Depth, DEPTH_STRING);
            }
            
            delete pkg;
            return KS_ERR_GENERIC;
        }
        
            Var = path;
            if(pv->access_mode & KS_AC_PART) {
                Var += ".";
            } else {
                Var += "/";
            }
            Var += pv->identifier;
            
        if(debug > 2) {
            fprintf(stderr, "%s%sAdd variable '%s'\n"
            , (const char*)Depth, DEPTH_STRING, (const char*)Var);
        }   
        
            ListEP.addLast(pv);

            VarArray[i] = new KscVariable(root+Var);
            if(!VarArray[i]) {
                delete pkg;
                return OV_ERR_HEAPOUTOFMEMORY;
            }
        
            if(!pkg->add(KscVariableHandle(VarArray[i], PltOsNew)) ) {
                err = pkg->getLastResult();
            if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
            // Debug-Ausgabe
            if(debug) {
                fprintf(stderr, "%s%sFEHLER : Can't add variable '%s' to package\n"
                , (const char*)Depth, DEPTH_STRING, (const char*)Var);
            }
                delete pkg;
                     return err;
            }
         
    } /* for alle Variablen */
    
    if(retries_for_ia_fox <= 0) {
        retries_for_ia_fox = 1;
    }

    for(upd = 0; upd < retries_for_ia_fox; upd++) {

        err = KS_ERR_OK;
        if(!pkg->getUpdate() ) {
            err = pkg->getLastResult();
            if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
            // Debug-Ausgabe
            switch(debug) {
                case 1:
                    fprintf(stderr, ".");
                default:
                    break;
            }
        } 
        switch (debug) {
            case 2:
            case 3:
                for(i = 0; i < anzVars; i++) {
                    const KsVarCurrProps *cp = VarArray[i]->getCurrProps();
                        if( (!cp) || (!cp->value) ) {
                                err = KS_ERR_GENERIC;
                            
                        // Debug-Ausgabe
                       fprintf(stderr, "%s%s Can't read variable '%s'.\n"
                                        , (const char*)Depth, DEPTH_STRING
                                        ,(const char*)VarArray[i]->getFullPath());
                       
                        } else {
                        // Debug-Ausgabe
                            if(debug > 2) {
                            fprintf(stderr, "%s%s Variable '%s' OK.\n"
                                        , (const char*)Depth, DEPTH_STRING
                                        ,(const char*)VarArray[i]->getFullPath());
                            }
                        }
                }
            default:
                        break;
        }
        if(err == KS_ERR_OK) {
            break;
        } else {
            // Warte kurz ...
            PltTime After(0, 500000);
            After.sleep();
        }
    }
    
#if 0
    // Zwischenloesung : Kein Abbruch, sondern Keine Value in XML-Datei
    
    if(err != KS_ERR_OK) {
        delete pkg;
            return err;
    }
#endif
    
    err = KS_ERR_OK;
    
    for(i = 0; i < anzVars; i++) {
        const KsVarCurrProps *cp = VarArray[i]->getCurrProps();
            if( (!cp) || (!cp->value) ) {
                VarNotSaved.addLast((const char*)VarArray[i]->getFullPath());
            VarStates.addLast(0);
            VarTypes.addLast(KS_VT_VOID);
            } else {
                VarSaved++;
                VarStates.addLast(cp->state);
            VarTypes.addLast(cp->value->xdrTypeCode());
            }

        err = ifb_getxmlValueOnly(              /*  |> Funktionsrueckmeldung      */
                                cp              /* >|  Eigenschaften der Variable */
                                ,Out            /* >|> Value als String           */
                                ,Depth          /* >|  Einrueckung                */
                                );
        if(err != KS_ERR_OK) break;
    
        VarValues.addLast(Out);
    
        Out = "";
    }

    /* Speicher freigeben */
    delete pkg;

    return err;
}

// 2010-05-24 AN:
// ServerInfo, Vendor und XML_Structure nicht sichern!
bool xml_ignoreObject(PltString &path) {
    if( (path == "/vendor") || (path == "/serverinfo") || (path == "/XML_Structure") ) {
        return true;
    }
    if( path.len() >= strlen("/vendor") ) {
        if(path.substr(0, strlen("/vendor")) == "/vendor") {
            return true;
        }
    }
    if( path.len() >= strlen("/serverinfo") ) {
        if(path.substr(0, strlen("/serverinfo")) == "/serverinfo") {
            return true;
        }
    }
    if( path.len() >= strlen("/XML_Structure") ) {
        if(path.substr(0, strlen("/XML_Structure")) == "/XML_Structure") {
            return true;
        }
    }
    
    return false;
}

/*****************************************************************************/
 KS_RESULT get_xmlObj(KscServerBase* Server
                      , PltString    &path
                      ,PltString     &Out
                      ,PltTime       &wTim
                      ,FILE          *fout
                      ,KsEngPropsHandle &hpp
                      ,PltString     &Value
                      ,KS_STATE      &State
                      ,KS_VAR_TYPE   &ValType
                      ,PltString     &Depth
                      ,size_t        nDepth)
/*****************************************************************************/
{
    KS_RESULT                   fehler;     // Funktions-Rueckmeldung
    KS_OBJ_TYPE                 ot;         // Merker : Objekt-Type
    KsString                    newPath;    // Pfad des UnterObjektes
    KsString                    help;       // Hilfsstring
    PltString                   comm;       // Comment
    
#if XMLDEMO
    static int anzObjs = 0;
    
    anzObjs++;
    if(anzObjs == 101) {
        fprintf(stderr, "\n In der Demo-Version werden nur 100 Objekte gesichert.\n");
    }
    if(anzObjs > 100) {
        return KS_ERR_OK;
    }
#endif
    
    if (!hpp) {
        return KS_ERR_GENERIC;
    }
    
    // 2010-05-24 AN:
    // ServerInfo, Vendor und XML_Structure nicht sichern!
    comm = path;
    if(comm != "/") {
        comm += "/";
    }
    comm += (const char *)hpp->identifier;
    
    if( xml_ignoreObject(comm) ) {
        return KS_ERR_OK;
    }
    comm = "";
    
    
    Out += Depth;           
    Out += "<";           
    ot = hpp->xdrTypeCode();
    Out += ifb___xmlObjType( ot );  
        
    // Identifier
    ksStringFromPercent((const char *)hpp->identifier,help);
    Out += " ident=\"";
    comm = (const char*)help;
    fehler = xml_fixValue(comm);
    if(fehler) return fehler;
    Out += (const char*)help;
        
    // Kommentar
    Out += "\" comment=\"";
    comm = (const char*)hpp->comment;
    fehler = xml_fixValue(comm);
    if(fehler) return fehler;
    Out += comm;
    
    // Access mode
    Out += "\" access=\"";
        Out += ifb___xmlAccessMode(hpp->access_mode);

    // Semantic flags
    Out += "\" flags=\"";
    Out += ifb___xmlSemanticFlags(hpp->semantic_flags);
            
    // Creations time
    Out += "\" ctime=\"";
    Out += ifb___xmlFormatTime(hpp->creation_time);

//        newPath += hpp->identifier;
            
        // Weitere Eigenschaften sind Objekt-spezifisch
        switch(ot) {
                case KS_OT_DOMAIN :
                        {                                            // object is a domain
                                // class identifier
                            Out += "\" class=\"";
                                comm = (const char*)(((KsDomainEngProps &)(*hpp)).class_identifier);
                fehler = xml_fixValue(comm);
                if(fehler) return fehler;
                                Out += comm;
                                Out += "\">\n";
                        }
                        break;    
                case KS_OT_VARIABLE :
                        {                                            // object is a variable
                                // Typ der Variable
                            Out += "\" class=\"KS_VT_";
                            if(ValType == KS_VT_VOID) {
                                ValType = ((KsVarEngProps &)(*hpp)).type;
                            }
                                Out += ifb_getPortValueType( ValType );
                                
                                // TechUnits (Einheit)
                                Out += "\" techunit=\"";
                                comm = (const char*)((KsVarEngProps &)(*hpp)).tech_unit;
                fehler = xml_fixValue(comm);
                if(fehler) return fehler;

                                Out += comm;
                                
                // Status
                                Out += "\" state=\"";
                Out += ifb_getState(State);
                                
                                // Value
                                Out += "\">";
                                Out += Value;
                                Out += "\n";
                            Out += Depth;           
                        }
                        break;
                case KS_OT_HISTORY :
                        {                                            // object is a history
                            DomSaved++;
                                // Typ des History-Objektes
                                Out += "\" class=\"";
                                Out += ifb___xmlHistType(((KsHistoryEngProps &)(*hpp)).type);
                                
                                // Default-Interpolation
                                Out += "\" defint=\"";
                                Out += ifb___xmlHistInterpMode( ((KsHistoryEngProps &)(*hpp)).default_interpolation);

                                // Supported interpolation
                                Out += "\" supint=\"";
                                Out += ifb___xmlHistInterpMode( ((KsHistoryEngProps &)(*hpp)).supported_interpolations);
                                
                                // Type identifier
                                Out += "\" typeid=\"";
                                comm = (const char*)((KsHistoryEngProps &)(*hpp)).type_identifier;
                fehler = xml_fixValue(comm);
                if(fehler) return fehler;
                                Out += comm;
                                Out += "\">";
                        }
                        break;
                case KS_OT_LINK :
                        {                                            // object is a link
                                // link type
                                Out += "\" class=\"";
                                Out += ifb___xmlLinkType( ((KsLinkEngProps &)(*hpp)).type);
                                
                                // role identifier
                                Out += "\" role=\"";
                                Out += (const char*)((KsLinkEngProps &)(*hpp)).opposite_role_identifier;
                                
                                // association identifier
                                Out += "\" assoc=\"";
                                comm = (const char*)((KsLinkEngProps &)(*hpp)).association_identifier;
                fehler = xml_fixValue(comm);
                if(fehler) return fehler;
                                Out += comm;

                                // Value
                                Out += "\">";
                                Out += Value;
                                Out += "\n";
                            Out += Depth;           
                           }
                        break;
                default:                                                                       // unknown object type
                        Out += "\">";
                    break;
        }
    
    // Schreiben in Datei ?
    if(fout) {
        fputs((const char*)Out, fout);
        // String-Buffer leeren
        Out = "";
    }
    if(ot == KS_OT_DOMAIN) {
            newPath = path;
        if(newPath != "/") {
                if(hpp->access_mode & KS_AC_PART) {
                    newPath += ".";
                } else {
                    newPath += "/";
                }
        }
            newPath += hpp->identifier;
#if XML_SIMPLE
        if(hpp->access_mode & KS_AC_PART) {
            // Nut Child-Objekte sichern
        } else {
#endif
            fehler = get_xmlobjs(Server, newPath, Out, wTim, fout, nDepth+1);
            if(fehler) {
                return fehler;
            }
            Out += Depth;
#if XML_SIMPLE
        }
#endif
    }
    
    Out += "</";
    Out += ifb___xmlObjType( ot );
    Out += ">\n";

    // Schreiben in Datei ?
    if(fout) {
        fputs((const char*)Out, fout);
        // String-Buffer leeren
        Out = "";
    }

    return KS_ERR_OK;
}

/*****************************************************************************/
KS_RESULT get_xmlobjs(
    KscServerBase *Server,
    PltString      &path,
    PltString      &Out,
    PltTime        &wTim,
    FILE          *fout,
    size_t         nDepth
)  {
/*****************************************************************************/

    KS_RESULT                   fehler;     // Funktions-Rueckmeldung
    KsGetEPResult               result;     // Dienst-Ergebnis
    KsGetEPParams               params;     // Dienst-Parameter
    size_t                      ot;         // Laufvariable
    PltList<KsEngPropsHandle>   VarList;    // Hilfsliste : Variablen und Links
    PltList<KsEngPropsHandle>   DomList;    // Hilfsliste : Domains
    PltList<PltString>          VarValues;  // Value der Variablen als String
    PltList<KS_STATE>           VarStates;  // Status der Variablen
    PltList<KS_VAR_TYPE>        VarTypes;   // Type der Variablen
    PltString                   Value;      // Wert der Variable
    KS_STATE                    State=0;    // Status der Variable
    KS_VAR_TYPE                 Type=0;     // Typ der Variable
    PltString                   Depth("");  // Text-Einrueckung
    KsString                    newPath;    // Pfad des Objektes
    long int                    upd;        // Laufvariable
    
    // 2010-05-24 AN:
    // ServerInfo, Vendor und XML_Structure nicht sichern!
    if( xml_ignoreObject(path) ) {
        return KS_ERR_OK;
    }
    
    // Wartezeit?
    if( (wTim.tv_sec != 0) || (wTim.tv_usec != 0) ) {
        wTim.sleep();
    }
    
    // Einrueckung
    for(ot = 0; ot < nDepth; ot++) {
        Depth += DEPTH_STRING;
    }
    
    // Parameter vorbereiten
    fehler = KS_ERR_OK;
    params.path = path;
    params.name_mask = "*";
    params.scope_flags = KS_EPF_DEFAULT;
#if XML_SIMPLE
    params.type_mask = KS_OT_DOMAIN | KS_OT_VARIABLE;
#else
    params.type_mask = KS_OT_ANY;
#endif

    // Debug-Ausgabe
    if(debug) {
        fprintf(stderr, "%sRead domain '%s'  ", (const char*)Depth, (const char*)path);
    }


    if(retries_for_ia_fox <= 0) {
        retries_for_ia_fox = 1;
    }

    // Dienst ausfuehren
    for(upd = 0; upd < retries_for_ia_fox; upd++) {

        bool ok = Server->getEP(0, params, result);
        if( !ok ) {
            // Debug-Ausgabe
            fehler = Server->getLastResult();
            if(fehler == KS_ERR_TARGETGENERIC) {
                if(debug) {
                    fprintf(stderr, ".");
                }
                PltTime After(0, 500000);
                After.sleep();
                continue;
            }
            if(fehler == KS_ERR_OK) fehler = KS_ERR_GENERIC;
            break;
        }
        fehler = result.result;
        if( fehler == KS_ERR_TARGETGENERIC) {
            if(debug) {
                fprintf(stderr, ".");
            }
            PltTime After(0, 500000);
            After.sleep();
            continue;
        } else {
            // Dienst ausgefuehrt
            break;
        }
    }

    if(fehler != KS_ERR_OK) {
        DomNotSaved.addLast(path);
        if(debug) {
            fprintf(stderr, " Error: %s.\n", GetErrorCode(fehler));
        }
        return KS_ERR_OK;
    } else {
    
        DomSaved++;
    
        if(debug) {
            fprintf(stderr, " OK.\n");
        }
    }
    
    /* Objekte sortieren */
    while ( result.items.size() ) {
        KsEngPropsHandle hpp = result.items.removeFirst();
        if(!hpp) {
            return KS_ERR_GENERIC;
        }

            switch(hpp->xdrTypeCode()) {
                    case KS_OT_VARIABLE :
                    case KS_OT_LINK :
                        VarList.addLast(hpp);
                            break;
                    default:                                                                       // Domains und Histories
                        DomList.addLast(hpp);
                            break;
            }
    }

    // Wartezeit?
    if( (wTim.tv_sec != 0) || (wTim.tv_usec != 0) ) {
        wTim.sleep();
    }
    
    // Variablen-Value holen
    newPath = path;
    fehler = get_xmlValue(Server, newPath, VarList, VarValues, VarStates, VarTypes, Depth);
    if( fehler != KS_ERR_OK ) { 
        return fehler;
    }

    // Alle Objekte ausgeben
    while(VarList.size()) {

        KsEngPropsHandle hpp = VarList.removeFirst();
        Value = VarValues.removeFirst();
        State = VarStates.removeFirst();
        Type  = VarTypes.removeFirst();
        fehler = get_xmlObj(Server, path, Out, wTim, fout, hpp, Value, State, Type, Depth, nDepth);
        if(fehler) {
            return fehler;
        }
    }
    while(DomList.size() ) {
        KsEngPropsHandle hpp = DomList.removeFirst();
        fehler = get_xmlObj(Server, path, Out, wTim, fout, hpp, Value, State, Type, Depth, nDepth);
        if(fehler) {
            return fehler;
        }
    }

    return KS_ERR_OK;

 } /* get_xmlobjs */


/******************************************************************************/
KS_RESULT IFBS_XMLDBCONTENTS(KscServerBase *Server,
                             PltString     &path,
                             PltString     &Out,
                             PltTime       &wTim,
                             FILE          *fout = 0)
/******************************************************************************/
 {
    KsGetEPParams       params;
    PltString           log("");
    KS_RESULT           err;

    if(!Server) {
        err = KS_ERR_SERVERUNKNOWN;
        iFBS_SetLastError(1, err, log);
            return err;
    }
    
    // Gesichertere Objekte
    VarSaved = 0;
    DomSaved = 0;

    // Last-Error loeschen
    err = KS_ERR_OK;
    iFBS_SetLastError(1, err, log);

    // 2010-05-24 AN:
    // ServerInfo, Vendor und XML_Structure nicht sichern!
    if( xml_ignoreObject(path) ) {
        return KS_ERR_OK;
    }

    KsTime tt = KsTime::now();
    
    // Ueberschrift
    
    Out =  "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n\n<backup server=\"";
    Out += (const char*)(Server->getHostAndName());        

    Out += "\" path=\"";
    Out += path;

    Out += "\" ctime=\"";
    Out += ifb___xmlFormatTime(tt);
 
    Out += "\">\n";
    
    // Schreiben in Datei ?
    if(fout) {
        fputs((const char*)Out, fout);
        // String-Buffer leeren
        Out = "";
    }
    

  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  // Rueckdokumentation der Instanzen
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    err = get_xmlobjs(Server, path, Out, wTim, fout);
    if(err) {
        return err;
    }
    
    Out +=  "</backup>\n";

    // Schreiben in Datei ?
    if(fout) {
        fputs((const char*)Out, fout);
        // String-Buffer leeren
        Out = "";
    }
    
    return KS_ERR_OK;
}

/******************************************************************************/
KS_RESULT IFBS_DBSAVEXML(KscServerBase*   Server,
                         PltString        &path,
                         PltString        &datei,
                         PltTime          &wTim) {
/******************************************************************************/

    /* 
    *  Lokale Variablen
    */
    PltString Str("");

    if(!datei.len() ) {
            return KS_ERR_BADNAME;
    }
    FILE *fout = fopen((const char*)datei, "w");
    if(!fout) {
                return KS_ERR_BADPATH;
    }

    KS_RESULT err = IFBS_XMLDBCONTENTS(Server, path, Str, wTim, fout);

    fclose(fout);

    return err;
}
