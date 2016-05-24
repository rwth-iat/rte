/*****************************************************************************
*                                                                            *
*    OV - SERVER                                                             *
*    ###########                                                             *
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
*   INC :                                                                    *
*   -----                                                                    *
*   db_save.h                                                                *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   2001-02-07  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*   Hilfsprogramme fur Ausgabe der Eigenschaften der Variable im Klartext    *
*                                                                            *
*****************************************************************************/


#include "ifbslibdef.h"

/********************************************************************************
* Hilfsfunktion.                                                                *
* Rueckgabe der Value-Laenge als String in Form "[Laenge]", falls               *
* Value ein Vektor ist, bzw. "" bei Skalar-Variablen                            *
********************************************************************************/
char *ifb_getValueLength(const KsVarCurrProps* cp) {
/*******************************************************************************/
    static char help[32];   // Hilfsstring
    size_t      size;       // Laenge des Vectors
    
    if(!cp || !cp->value) {
        return NULL;
    }
    
    switch(cp->value->xdrTypeCode() ) {
        case KS_VT_BOOL:
        case KS_VT_INT:
        case KS_VT_UINT:
        case KS_VT_SINGLE:
        case KS_VT_DOUBLE:
        case KS_VT_STRING:
        case KS_VT_TIME:
        case KS_VT_TIME_SPAN:
                                return NULL;
        case KS_VT_BYTE_VEC:
                                size = ((KsByteVecValue &) *cp->value).size();
                                if (size == 1) {
                                    return NULL;
                                }
                                break;
        case KS_VT_BOOL_VEC:
                                size = ((KsBoolVecValue &) *cp->value).size();
                                break;
        case KS_VT_INT_VEC:
                                size = ((KsIntVecValue &) *cp->value).size();
                                break;
        case KS_VT_UINT_VEC:
                                size = ((KsUIntVecValue &) *cp->value).size();
                                break;
        case KS_VT_SINGLE_VEC:
                                size = ((KsSingleVecValue &) *cp->value).size();
                                break;
        case KS_VT_DOUBLE_VEC:
                                size = ((KsDoubleVecValue &) *cp->value).size();
                                break;
        case KS_VT_STRING_VEC:
                                size = ((KsStringVecValue &) *cp->value).size();
                                break;
        case KS_VT_TIME_VEC:
                                size = ((KsTimeVecValue &) *cp->value).size();
                                break;
        case KS_VT_TIME_SPAN_VEC:
                                size = ((KsTimeSpanVecValue &) *cp->value).size();
                                break;
        default:
                                return NULL;
        } /* switch */
                
        sprintf(help,"[%d]", size);

        return help;
}

/********************************************************************************
* Hilfsfunktion.                                                                *
* Rueckgabe der Value-Laenge als numerischen Wert                               *
********************************************************************************/
size_t ifb_getValueLengthAsNum(const KsVarCurrProps* cp) {
/*******************************************************************************/
    size_t      size = 0;       // Laenge des Vectors
    
    if(!cp || !cp->value) {
        return size;
    }
    
    switch(cp->value->xdrTypeCode() ) {
        case KS_VT_BOOL:
        case KS_VT_INT:
        case KS_VT_UINT:
        case KS_VT_SINGLE:
        case KS_VT_DOUBLE:
        case KS_VT_STRING:
        case KS_VT_TIME:
        case KS_VT_TIME_SPAN:
                                size = 1;
                                break;
        case KS_VT_BYTE_VEC:
                                size = ((KsByteVecValue &) *cp->value).size();
                                break;
        case KS_VT_BOOL_VEC:
                                size = ((KsBoolVecValue &) *cp->value).size();
                                break;
        case KS_VT_INT_VEC:
                                size = ((KsIntVecValue &) *cp->value).size();
                                break;
        case KS_VT_UINT_VEC:
                                size = ((KsUIntVecValue &) *cp->value).size();
                                break;
        case KS_VT_SINGLE_VEC:
                                size = ((KsSingleVecValue &) *cp->value).size();
                                break;
        case KS_VT_DOUBLE_VEC:
                                size = ((KsDoubleVecValue &) *cp->value).size();
                                break;
        case KS_VT_STRING_VEC:
                                size = ((KsStringVecValue &) *cp->value).size();
                                break;
        case KS_VT_TIME_VEC:
                                size = ((KsTimeVecValue &) *cp->value).size();
                                break;
        case KS_VT_TIME_SPAN_VEC:
                                size = ((KsTimeSpanVecValue &) *cp->value).size();
                                break;
        default:
                                break;
    } /* switch */
                
    return size;
}

/********************************************************************************
* Hilfsfunktion.                                                                *
* Rueckgabe Value-Type als String fuer FB-Sicherung der Datenbasis              *
********************************************************************************/
char const *ifb_getOvValueType(KS_VAR_TYPE typ) {
/*******************************************************************************/

#define BAD_TYPE_STRING "UNKNOWN"

    switch(typ) {
                 case KS_VT_BOOL:
                 case KS_VT_BOOL_VEC:
                                return "BOOL";
                 case KS_VT_INT:
                 case KS_VT_INT_VEC:
                                return "INT";
                 case KS_VT_UINT:
                 case KS_VT_UINT_VEC:
                                return "UINT";
                 case KS_VT_SINGLE:
                 case KS_VT_SINGLE_VEC:
                                return "SINGLE";
                 case KS_VT_DOUBLE:
                 case KS_VT_DOUBLE_VEC:
                                return "DOUBLE";
                 case KS_VT_STRING:
                 case KS_VT_STRING_VEC:
                                return "STRING";
                 case KS_VT_TIME:
                 case KS_VT_TIME_VEC:
                                return "TIME";
                 case KS_VT_TIME_SPAN:
                 case KS_VT_TIME_SPAN_VEC:
                                return "TIME_SPAN";
                 case KS_VT_BYTE_VEC:
                                return "BYTE";
                 default:
                                return BAD_TYPE_STRING;
    } /* switch */
}

/********************************************************************************
* Hilfsfunktion.                                                                *
* Rueckgabe Value-Type als String                                               *
********************************************************************************/
char const *ifb_getPortValueType(KS_VAR_TYPE typ) {
/*******************************************************************************/

#define BAD_TYPE_STRING "UNKNOWN"

    switch(typ) {
                 case KS_VT_VOID:
                                return "VOID";
                 case KS_VT_BOOL:
                                return "BOOL";
                 case KS_VT_BOOL_VEC:
                                return "BOOL_VEC";
                 case KS_VT_INT:
                                return "INT";
                 case KS_VT_INT_VEC:
                                return "INT_VEC";
                 case KS_VT_UINT:
                                return "UINT";
                 case KS_VT_UINT_VEC:
                                return "UINT_VEC";
                 case KS_VT_SINGLE:
                                return "SINGLE";
                 case KS_VT_SINGLE_VEC:
                                return "SINGLE_VEC";
                 case KS_VT_DOUBLE:
                                return "DOUBLE";
                 case KS_VT_DOUBLE_VEC:
                                return "DOUBLE_VEC";
                 case KS_VT_STRING:
                                return "STRING";
                 case KS_VT_STRING_VEC:
                                return "STRING_VEC";
                 case KS_VT_TIME:
                                return "TIME";
                 case KS_VT_TIME_VEC:
                                return "TIME_VEC";
                 case KS_VT_TIME_SPAN:
                                return "TIME_SPAN";
                 case KS_VT_TIME_SPAN_VEC:
                                return "TIME_SPAN_VEC";
                 case KS_VT_BYTE_VEC:
                                return "BYTE_VEC";
                 default:
                                return BAD_TYPE_STRING;
    } /* switch */
}

/********************************************************************************
* Hilfsfunktion.                                                                *
* Gibt die Merkmahle des Ports als String :                                     *
* beim gesetztem Flag 'd' "(dynamic)" und                                       *
* beim gesetztem Flag 's' "(PV)"                                                *
********************************************************************************/
char const *ifb_getPortCharacteristic(KS_SEMANTIC_FLAGS flags) {
/*******************************************************************************/

    if( IsFlagSet( flags, 's') ) {
	    return IFB_PV_PORTIDENT;
    }
    if( IsFlagSet( flags, 'd') ) {
		return IFB_DYNAMIC_PORTIDENT;
    }
    return NULL;
}

/******************************************************************************
* Hilfsfunktion :                                                             *
* Aendert ->"<- in ->\"<-                                                     *
******************************************************************************/
 KS_RESULT fixStringValue(KsString &Val) {
/*****************************************************************************/

    char* ph1;      /* Hilfsstring */
    char* pc1;      /* Hilfszeiger auf 1. String */
    char* ph2;      /* Hilfsstring */
    char* pc2;      /* Hilfszeiger auf 2. String */
    int   anz = 0;  /* Merker : Anzahl der " im String */
                      
    if(!Val.len() ) return KS_ERR_OK;
    
    // Hilfsstring anlegen
    ph1 = (char*)malloc(Val.len() + 1);
    if(!ph1) return OV_ERR_HEAPOUTOFMEMORY;
    
    // String fuer weitere Bearbeitung kopieren
    strcpy(ph1, (const char*)Val);
    pc1 = ph1;
    while( pc1 && (*pc1) ) {
        if( (*pc1) == '"') {
            // " direkt am String-Anfang ?
            if(pc1 != ph1) {
                // Es gibt noch Zeichen vor dem Zeiger
                // Steht vor dem " schon Backslash ?
                pc1--;
                if( (*pc1) != '\\') anz++;  // Kein Backslash. Anzahl inkrementieren
                pc1++;                      // Zeiger auf alte Position
            } else {
                // Hochkomma direkt am Anfang. Anzahl inkrementieren
                anz++;
            }
        }
        pc1++;  // Naechstes Zeichen
    }
    pc1 = ph1;  // An Anfang setzen
    
    // Hochkommas gefunden ?
    if(anz) {
        // Dann bearbeite String
        ph2 = (char*)malloc(Val.len() + anz + 1);
        if(!ph2) {
            free(ph1);
            return OV_ERR_HEAPOUTOFMEMORY;
        }
        pc2 = ph2;
        
        while(pc1 && (*pc1) ) {
            if( (*pc1) == '"') {
                //  " direkt am String-Anfang ?
                if(pc1 != ph1) {
                    pc1--;
                    if( (*pc1) != '\\') {
                        *pc2 = '\\';
                        pc2++;
                    }
                    pc1++;
                } else {
                    *pc2 = '\\';
                    pc2++;
                }
            }
            // Zeichen uebertragen
            *pc2 = *pc1;
            pc1++;
            pc2++;
        }
        // String-Ende markieren
        *pc2 = '\0';
        
        // Geaenderten String zurueckgeben
        Val = ph2;
        
        // Speicher freigeben
        free(ph2);
    }

    // Speicher freigeben
    free(ph1);
    
    return KS_ERR_OK;
}
 
/******************************************************************************
* Hilfsfunktion :                                                             *
* Aendert ->,<- in ->.<-  im Float-Wert als String                            *
******************************************************************************/
 KS_RESULT fixFloatValue(KsString &Val) {
/*****************************************************************************/

    char* ph1;      /* Hilfsstring */
    char* pc1;      /* Hilfszeiger auf 1. String */
    char* ph2;      /* Hilfsstring */
    char* pc2;      /* Hilfszeiger auf 2. String */
                      
    if(!Val.len() ) return KS_ERR_OK;
    
    // Hilfsstring anlegen
    ph1 = (char*)malloc(Val.len() + 1);
    if(!ph1) return OV_ERR_HEAPOUTOFMEMORY;

    // String fuer weitere Bearbeitung kopieren
    strcpy(ph1, (const char*)Val);

    // Komma als Dezimal-Trennzeichen ?
    pc1 = strstr(ph1, ",");
    if(pc1) {
        // Dann bearbeite String
        ph2 = (char*)malloc(Val.len() + 1);
        if(!ph2) {
            free(ph1);
            return OV_ERR_HEAPOUTOFMEMORY;
        }
        
        // Bei dem vorhandenen Computer-Einstellung wird 1234.5 als 1.234,5 angezeigt
        // Gehe alle Zeichen durch und entferne die Punkten
        pc1 = ph1;
        pc2 = ph2;
        while(pc1 && (*pc1)) {
            if( (*pc1) == '.') {
                pc1++;
            } else {
                *pc2 = *pc1;
                pc1++;
                pc2++;
            }
        }
        
        // String-Ende markieren
        *pc2 = '\0';
        
        // Nun suchen wir ',' in ph2 und ersetzen die durch '.'
        pc2 = strstr(ph2, ",");
        if(!pc2) {
            // ??
            free(ph2);
            free(ph1);
            return KS_ERR_GENERIC;
        }
        *pc2 = '.';
    
        // Geaenderten String zurueckgeben
        Val = ph2;
        
        // Speicher freigeben
        free(ph2);
    }
    
    // Speicher freigeben
    free(ph1);
    
    return KS_ERR_OK;
}
 
/********************************************************************************
* Hilfsfunktion.
* Zeit als String
********************************************************************************/
char* ifb_time2string(KsTime &zeit, bool ShortTime) {
    static char  help[32];
    char         dStr[32];
    struct tm *t;
    time_t timer;

//    tzset();
    timer = (time_t)zeit.tv_sec;
    
	t = localtime(&timer);
	//t = gmtime(&timer);
	if(!t) {
	    strcpy(help, (ShortTime ? "... 00:00:00" : "1970-01-01 00:00:00.000000"));
	    return help;
	}
	
    if(ShortTime) {
        strftime(help, 31, "... %H:%M:%S", t);
        help[31] = '\0';
        
    } else {
        strftime(dStr, 31, "%Y-%m-%d %H:%M:%S", t);
        dStr[31] = '\0';

        sprintf(help, "%s.%6.6lu", dStr, (unsigned long)zeit.tv_usec );
    }

    return help;
}

/********************************************************************************
* Hilfsfunktion.                                                                *
* Wert der Variable als String                                                  *
********************************************************************************/
KS_RESULT ifb_getValueOnly(  /*  |> Funktionsrueckmeldung         */
    const KsVarCurrProps* cp                    /* >|  Eigenschaften der Variable            */
    ,PltString            &Out                  /* >|> Value als String                      */
    ,unsigned long        MaxElem               /* Max. Anzahl Elementen in Array            */
    ,bool                 ShortTime             /* Merker, ob Time-Ausgabe "... hh:mm:ss"    */
    ,bool                 BoolAsNum             /* Merker, ob TRUE als "1" und FALSE als "0" */
    ,bool                 MakeList              /* Merker, ob Array als Liste "{...}"        */
    ,char*                ELEMENT_TRENNER       /* Trenner der Array-Elementen               */
    ,bool                 Str                   /* Merker, ob string in Hochkommas   */
    ,bool                 FixStr                /* TRUE : '"'->'\"'    */
    ) {
/*******************************************************************************/

#define  BAD_VALUE_STRING "{}"

    char           help[128];
    size_t         size;    /* Vektor-Lenge */
    size_t         i;       /* Laufvariable */
    KsTime         zeit;
    KsTimeSpan     zeitsp;
//    struct         tm* t;
//    time_t         timer;
    KsString       Value;   /* Hilfsstring  */
    KS_RESULT      res;     /* Ergebnis der String-Konvertierung */
    
    bool           Shorting = FALSE;    /* Merker, ob weniger Ausgaben als in Array geschrieben werden */

    if(!cp || !cp->value) {
        return KS_ERR_BADPARAM;
    }

    switch(cp->value->xdrTypeCode() ) {
                 case KS_VT_BOOL:
                                if( (bool) ((KsBoolValue &) *cp->value) )
								    Out += (BoolAsNum ? "1" : "TRUE") ;
							    else
								    Out += (BoolAsNum ? "0" : "FALSE");
                                break;
                 case KS_VT_INT:
                                sprintf(help ,"%ld", (long) ((KsIntValue &) *cp->value) );
                                Out += help;
                                break;
                 case KS_VT_UINT:
                                sprintf(help ,"%lu",
                                        (unsigned long) ((KsUIntValue &) *cp->value));
                                Out += help;
                                break;
                 case KS_VT_SINGLE:
                                sprintf(help ,"%#f",
                                       (float) ((KsSingleValue &) *cp->value) );
                                Value = help;
                                fixFloatValue(Value);
                                Out += Value;
                                break;
                 case KS_VT_DOUBLE:
                                sprintf(help ,"%#f",
                                       (float) ((KsDoubleValue &) *cp->value) );
                                Value = help;
                                fixFloatValue(Value);
                                Out += Value;
                                break;
                 case KS_VT_STRING:
                                if(Str) Out += "\"";
                                Value = (KsStringValue &) *cp->value;
                                if(FixStr) {
                                    res = fixStringValue(Value);
                                    if(res != KS_ERR_OK) return res;
                                }
                                Out += (const char*)Value;
                                if(Str) Out += "\"";
                                break;
                 case KS_VT_TIME:
                                zeit = ((KsTimeValue &) *cp->value);
								Out += ifb_time2string(zeit, ShortTime);
                                break;
                 case KS_VT_TIME_SPAN:
                                //zeit = ((KsTimeValue &) *cp->value);
                                zeitsp = ((KsTimeSpanValue &) *cp->value);
                                zeitsp.normalize();
                                sprintf(help ,"%ld.%6.6ld", zeitsp.tv_sec, zeitsp.tv_usec );
                                Out += help;
                                break;
                 case KS_VT_BYTE_VEC:
                                size = ((KsByteVecValue &) *cp->value).size();
                                if (size ==1) {
                                    
                                    sprintf(help,"%d",
											 ((KsByteVecValue &)* cp->value)[0]);
                                    Out += help;
                                    
                                } else {
                                    if(size > MaxElem) {
                                        size = MaxElem;
                                        Shorting = TRUE;
                                    }
                                    if(MakeList) Out += "{";
                                    for ( i = 0; i < size; i++ ) {
                                        sprintf(help,"%d", ((KsByteVecValue &) *cp->value)[i]);
                                        Out += help;
                                        if ( i < (size-1) ) {
                                            Out += ELEMENT_TRENNER;
                                        }
                                    }
                                    if(Shorting) Out += " ...";
                                    if(MakeList) Out += "}";
                                }
                                break;
                 case KS_VT_BOOL_VEC:
                                size = ((KsBoolVecValue &) *cp->value).size();
                                if(size > MaxElem) {
                                    size = MaxElem;
                                    Shorting = TRUE;
                                }
                                if(MakeList) Out += "{";
                                for ( i = 0; i < size; i++ ) {
									if( (bool) ((KsBoolVecValue &) *cp->value)[i])
                                    	Out += "TRUE";
									else
										Out += "FALSE";
                                    if ( i < (size-1) ) {
                                        Out += ELEMENT_TRENNER;
                                    }
                                }
                                if(Shorting) Out += " ...";
                                if(MakeList) Out += "}";
                                break;
                 case KS_VT_INT_VEC:
                                size = ((KsIntVecValue &) *cp->value).size();
                                if(size > MaxElem) {
                                    size = MaxElem;
                                    Shorting = TRUE;
                                }
                                
                                if(MakeList) Out += "{";
                                for ( i = 0; i < size; i++ ) {
                                    sprintf(help,"%ld",((KsIntVecValue &) *cp->value)[i]);
                                    Out += help;
                                    if ( i < (size-1) ) {
                                        Out += ELEMENT_TRENNER;
                                    }
                                }
                                if(Shorting) Out += " ...";
                                if(MakeList) Out += "}";
                                break;
                 case KS_VT_UINT_VEC:
                                size = ((KsUIntVecValue &) *cp->value).size();
                                if(size > MaxElem) {
                                    size = MaxElem;
                                    Shorting = TRUE;
                                }
                                
                                if(MakeList) Out += "{";
                                for ( i = 0; i < size; i++ ) {
                                    sprintf(help,"%lu",((KsUIntVecValue &) *cp->value)[i]);
                                    Out += help;
                                    if ( i < (size-1) ) {
                                        Out += ELEMENT_TRENNER;
                                    }
                                }
                                if(Shorting) Out += " ...";
                                if(MakeList) Out += "}";
                                break;
                 case KS_VT_SINGLE_VEC:
                                size = ((KsSingleVecValue &) *cp->value).size();
                                if(size > MaxElem) {
                                    size = MaxElem;
                                    Shorting = TRUE;
                                }
                                
                                if(MakeList) Out += "{";
                                for ( i = 0; i < size; i++ ) {
                                    sprintf(help,"%#f",((KsSingleVecValue &) *cp->value)[i]);
                                    Value = help;
                                    fixFloatValue(Value);
                                    Out += Value;
                                    if ( i < (size-1) ) {
                                        Out += ELEMENT_TRENNER;
                                    }
                                }
                                if(Shorting) Out += " ...";
                                if(MakeList) Out += "}";
                                break;
                 case KS_VT_DOUBLE_VEC:
                                size = ((KsDoubleVecValue &) *cp->value).size();
                                if(size > MaxElem) {
                                    size = MaxElem;
                                    Shorting = TRUE;
                                }
                                
                                if(MakeList) Out += "{";
                                for ( i = 0; i < size; i++ ) {
                                    sprintf(help, "%#f",(float)((KsDoubleVecValue &) *cp->value)[i]);
                                    Value = help;
                                    fixFloatValue(Value);
                                    Out += Value;
                                    if ( i < (size-1) ) {
                                        Out += ELEMENT_TRENNER;
                                    }
                                }
                                if(Shorting) Out += " ...";
                                if(MakeList) Out += "}";
                                break;
                 case KS_VT_STRING_VEC:
                                size = ((KsStringVecValue &) *cp->value).size();
                                if(size > MaxElem) {
                                    size = MaxElem;
                                    Shorting = TRUE;
                                }
                                
                                if(MakeList) Out += "{";
                                for ( i = 0; i < size; i++ ) {
                                    if(Str) Out += "\"";
                                    Value = ((KsStringVecValue &) *cp->value)[i];
                                    if(FixStr) {
                                        res = fixStringValue(Value);
                                        if(res != KS_ERR_OK) return res;
                                    }
                                    Out += (const char*)Value;
                                    if(Str) Out += "\"";
                                    if ( i < (size-1) ) {
                                        Out += ELEMENT_TRENNER;
                                    }
                                }
                                if(Shorting) Out += " ...";
                                if(MakeList) Out += "}";
                                break;
                 case KS_VT_TIME_VEC:
                                size = ((KsTimeVecValue &) *cp->value).size();
                                if(size > MaxElem) {
                                    size = MaxElem;
                                    Shorting = TRUE;
                                }
                                
                                if(MakeList) Out += "{";
                                for ( i = 0; i < size; i++ ) {
                                    zeit = ((KsTimeVecValue &) *cp->value)[i];
								    Out += ifb_time2string(zeit, ShortTime);
                                    if ( i < (size-1) ) {
                                        Out += ELEMENT_TRENNER;
                                    }
                                }
                                if(Shorting) Out += " ...";
                                if(MakeList) Out += "}";
                                break;
                 case KS_VT_TIME_SPAN_VEC:
                                size = ((KsTimeSpanVecValue &) *cp->value).size();
                                if(size > MaxElem) {
                                    size = MaxElem;
                                    Shorting = TRUE;
                                }

                                if(MakeList) Out += "{";
                                for ( i = 0; i < size; i++ ) {
                                    zeitsp = ((KsTimeSpanVecValue &) *cp->value)[i];
                                    zeitsp.normalize();
                                    sprintf(help,"%ld.%6.6ld", zeitsp.tv_sec, zeitsp.tv_usec );
                                    Out += help;
                                    if ( i < (size-1) ) {
                                        Out +=  ELEMENT_TRENNER;
                                    }
                                }
                                if(Shorting) Out += " ...";
                                if(MakeList) Out += "}";
                                break;
                default:
                                break;
                } /* switch */
                
    return KS_ERR_OK;
}

/********************************************************************************
* Hilfsfunktion.                                                                *
* Value-Type  und Laenge                                                        *
********************************************************************************/
KS_RESULT ifb_getValueTypeAndLen(  /*  |> Funktionsrueckmeldung                */
    PltString            &Path     /* >|  Pfad der Variable                    */
    ,KS_VAR_TYPE         &Typ      /*  |> Value-Type                           */
    ,size_t              &Len      /*  |> Laenge, falls Variable ein Vektor    */
    ) {
/*******************************************************************************/
    
    KscVariable Var(Path);
    if(!Var.getUpdate() ) {
        KS_RESULT err = Var.getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
        return err;
    }
    const KsVarCurrProps *cp = Var.getCurrProps();
    if((!cp) || (!(cp->value)) ) {
        return KS_ERR_GENERIC;
    }
        
	Len = ifb_getValueLengthAsNum(cp);
	Typ = cp->value->xdrTypeCode();
	
	return KS_ERR_OK;
}
