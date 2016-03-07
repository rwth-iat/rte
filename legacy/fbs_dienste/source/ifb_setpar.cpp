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
KS_RESULT IFBS_SETVAR(KscServerBase*   Server,
                                                 FbSetVarParams   &pars)
/*****************************************************************************/
{

    if(!Server)
            return KS_ERR_SERVERUNKNOWN;

    PltString       VarName("//");
    KS_VAR_TYPE     Typ;            /*  Value-Type      */
    size_t          Len;            /*  Dummy           */
    KS_RESULT       fehler;         /*  Dienst-Ergebnis */
    
    // Typ der Variable holen
    VarName += (const char*)Server->getHost();
    VarName += "/";
    VarName += (const char*)Server->getName();
    VarName += pars.path; /* pars.path sieht so aus : /t_u/.../sum1.inp1 */

    fehler = ifb_getValueTypeAndLen(VarName, Typ, Len);
    if( fehler ) {
        return fehler;
    }

    // Value setzen
    return ifb_SeparateValueAndSet(Server, pars.path, pars.value, Typ, pars.state);
}

/*****************************************************************************/
KS_RESULT ifb_SeparateValueAndSet(KscServerBase*     Server
                                 ,PltString         &VarPath
                                 ,PltString         &Value
                                 ,KS_VAR_TYPE       Typ
                                 ,KS_STATE          Status
                                 ,bool              prepareString)
/*****************************************************************************/
{
    KS_RESULT           err;         /*  Dienst-Ergebnis */
    PltList<PltString>  ValList;
    PltString           val;
    
    switch(Typ) {
        case KS_VT_VOID:
                                break;
        case KS_VT_BOOL:
        case KS_VT_INT:
        case KS_VT_UINT:
        case KS_VT_SINGLE:
        case KS_VT_DOUBLE:
        case KS_VT_STRING:
        case KS_VT_TIME:
        case KS_VT_TIME_SPAN:
                                ValList.addLast(Value);
                                break;
        default:
                                err = SeparateString(Value, Typ, ValList);
                                if(err) {
                                    return err;
                                }
                                // Diskus: Vektor mit 1 Element?
                                //if(ValList.size() == 1) {
                                //    val = ValList.removeFirst();
                                //    if(val != "") {
                                //        // Doch zulassen?
                                //        ValList.addFirst(val);
                                //    }
                                //}
                                break;
    } /* switch */

    return ifb_setVar(Server, VarPath, ValList, Typ, Status, prepareString);
}
/*****************************************************************************/
KS_RESULT ifb_setVar(KscServerBase*         Server
                     ,PltString             &VarPath
                     ,PltList<PltString>    &ValList
                     ,KS_VAR_TYPE           Typ
                     ,KS_STATE              Status
                     ,bool                  prepareString)
/*****************************************************************************/
{
    KsSetVarParams          setpar(1);
    KsSetVarResult          erg(1);
//    KsArray<KsSetVarItem>        param(1);
    KS_RESULT               fehler = KS_ERR_OK;  /*  Dienst-Ergebnis */

    KsVarCurrProps*         var_props;

    if(setpar.items.size() != 1) {
        // Out of memory ?
        return OV_ERR_HEAPOUTOFMEMORY;
    }
    
    var_props = new KsVarCurrProps;
    if(!var_props) {
        return OV_ERR_HEAPOUTOFMEMORY;
    }
    var_props->value.bindTo(ifb_CrNewKsValue(fehler, ValList, Typ, prepareString), PltOsNew);
    if(fehler) {
        delete var_props;
        return fehler;
    }
    var_props->state = Status;
    
    setpar.items[0].path_and_name = VarPath;
    setpar.items[0].curr_props.bindTo( (KsCurrProps*)var_props,PltOsNew);

//    setpar.items = param;

    bool ok = Server->requestByOpcode(KS_SETVAR, GetClientAV(), setpar, erg);
    if(!ok) {
        fehler = Server->getLastResult();
        if(fehler == KS_ERR_OK) fehler = KS_ERR_GENERIC;
        return fehler;
    }
    if(erg.result) {
            return erg.result;
    }
        if(erg.results[0].result) {
                return erg.results[0].result;
        }

    return KS_ERR_OK;
} /* get_par() */

/*****************************************************************************/
void ifb_normalize_usecs(PltString &Val) {
/*****************************************************************************/
 
    char  help[128];
    char  *ph;
    int   i;
    
    if(Val.len() > 127) {
        return;
    }
    
    strcpy(help, (const char*)Val);
    ph = strstr(help, ".");
    if(!ph) {
        return;
    }
    i = 0;
    while(i < 6) {
        ph++;
        if( !(*ph) ) {
            *ph = '0';
            ph++; *ph = '\0'; ph--;
        }
        i++;
    }
    
    ph++;
    *ph = '\0';
    
    Val = help;
    
    return;
}

/*****************************************************************************/
KsValue* ifb_CrNewKsValue(
                         KS_RESULT              &err
                         ,PltList<PltString>    &ValList
                         ,KS_VAR_TYPE           Typ
                         ,bool                  prepareString)
/*****************************************************************************/
{
    
    size_t          anz, c;
    PltString       Val;
    struct tm       time;
    struct timeval        tv;
    long            sec;
    long            usec;
        
    KsBoolValue     *bool_val;

    // Annahme : 
        err = KS_ERR_OK;

        switch(Typ) {
            
                case KS_VT_VOID        :
                                {
                                  return new KsVoidValue();
                                }
                case KS_VT_BOOL        :
                                {
                                    Val = ValList.removeFirst();
                                    Val.toUpper();
                                    if( (Val == "FALSE") || (Val == "0") )
                                             bool_val = new KsBoolValue(0);
                                    else
                                             bool_val = new KsBoolValue(1);

                                    if(!bool_val) {
                                            err = OV_ERR_HEAPOUTOFMEMORY;
                                    }
                                    return bool_val;
                                }
                case KS_VT_INT        :
                                {
                                        Val = ValList.removeFirst();
                                        long int_wert = (long)atoi( (const char*)Val);
                                        KsIntValue *int_val = new KsIntValue(int_wert);
                                        if (!int_val) {
                                            err = OV_ERR_HEAPOUTOFMEMORY;
                                        }
                                        return int_val;
                                }
                case KS_VT_UINT        :
                                {
                                        Val = ValList.removeFirst();
                                        unsigned long uint_wert = (unsigned long)atoi( (const char*)Val);
                                                        KsUIntValue *uint_val = new KsUIntValue(uint_wert);
                                                        if (!uint_val) {
                                                                err = OV_ERR_HEAPOUTOFMEMORY;
                                                        }
                                                        return uint_val;
                                }
                case KS_VT_SINGLE        :
                                {
                                        Val = ValList.removeFirst();
                                        float sing_wert = (float)atof( (const char*)Val);
                                        KsSingleValue *sing_val = new KsSingleValue(sing_wert);
                                        if(!sing_val) {
                                                err = OV_ERR_HEAPOUTOFMEMORY;
                                        }
                                        return sing_val;
                                }
                case KS_VT_DOUBLE        :
                                {
                                        Val = ValList.removeFirst();
                                        double doubl_wert = (double)atof( (const char*)Val);
                                        KsDoubleValue *double_val = new KsDoubleValue(doubl_wert);
                                        if(!double_val) {
                                                err = OV_ERR_HEAPOUTOFMEMORY;
                                        }
                                        return double_val;
                                }
                case KS_VT_STRING        :
                                {
                                        Val = ValList.removeFirst();
                                        if(prepareString) {
                                            err = PrepareStringValue(Val);
                                            if(err) return 0;
                                        }
                                        
                                        KsStringValue *string_val=new KsStringValue(Val);
                                        if(!string_val ) {
                                                err = OV_ERR_HEAPOUTOFMEMORY;
                                        }
                                        return string_val;
                                }
                case KS_VT_TIME            :
                                {
                                        Val = ValList.removeFirst();
                                        time.tm_hour = 0;
                                        time.tm_min = 0;
                                        time.tm_sec = 0;
                                        tv.tv_usec = 0;
                                        
                                        ifb_normalize_usecs(Val);

                                        sscanf((const char*)Val, "%d-%d-%d %d:%d:%d.%ld"
                                                , &time.tm_year, &time.tm_mon, &time.tm_mday
                                                , &time.tm_hour, &time.tm_min, &time.tm_sec, &tv.tv_usec);

                                        time.tm_year -= 1900;
                                        time.tm_mon -=1;
                                        time.tm_isdst = -1; /* keine Sommer-Winterzeit */

                                        tv.tv_sec = mktime(&time);

                                        KsTimeValue *time_val = new KsTimeValue(tv.tv_sec, tv.tv_usec);
                                        if(!time_val) {
                                                err = OV_ERR_HEAPOUTOFMEMORY;
                                        }
                                        return time_val;
                                }
                case KS_VT_TIME_SPAN :
                                {
                                        Val = ValList.removeFirst();
                                        sec = 0;
                                        usec = 0;
                                        ifb_normalize_usecs(Val);
                                        sscanf((const char*)Val, "%ld.%ld", &sec, &usec);
                                        
                                        KsTimeSpanValue *time_sp_val = new KsTimeSpanValue(sec, usec);
                                        if(!time_sp_val) {
                                                err = OV_ERR_HEAPOUTOFMEMORY;
                                        }
                                        return time_sp_val;
                                }
///////////////////////////////////////////////////////////////////////////////////////////////////
                case KS_VT_BOOL_VEC :
                                {
                                        anz  = ValList.size();
                                        KsBoolVecValue *bool_vec = new KsBoolVecValue(anz);
                                        c=0;
                                        while(c < anz) {
                                            Val = ValList.removeFirst();
                                            Val.toUpper();
                                            if( (Val == "FALSE") || (Val == "0" ) )
                                                (*bool_vec)[c] = FALSE;
                                            else
                                                (*bool_vec)[c] = TRUE;
                                            c++;
                                        }
                                        return bool_vec;
                                }
                case KS_VT_INT_VEC :
                                {
                                        anz  = ValList.size();
                                        KsIntVecValue *int_vec = new KsIntVecValue(anz);

                                        c = 0;
                                        while(c < anz) {
                                            Val = ValList.removeFirst();
                                            (*int_vec)[c]= (long)atoi( (const char*)Val);
                                            c++;
                                        }
                                        return int_vec;
                                }
                case KS_VT_UINT_VEC :
                                {
                                        anz  = ValList.size();
                                        KsUIntVecValue *uint_vec = new KsUIntVecValue(anz);

                                        c = 0;
                                        while(c < anz) {
                                            Val = ValList.removeFirst();
                                            (*uint_vec)[c]= (u_long)atoi( (const char*)Val);
                                            c++;
                                        }
                                        return uint_vec;
                                }
                case KS_VT_SINGLE_VEC        :
                                {
                                        anz  = ValList.size();
                                        KsSingleVecValue *sing_vec = new KsSingleVecValue(anz);

                                        c = 0;
                                        while(c < anz) {
                                            Val =ValList.removeFirst();
                                            (*sing_vec)[c] = (float)atof((const char*)Val);
                                            c++;
                                        }
                                        return sing_vec;
                                }
                case KS_VT_DOUBLE_VEC :
                                {
                                        anz  = ValList.size();
                                        KsDoubleVecValue *double_vec = new KsDoubleVecValue(anz);

                                        c = 0;
                                        while(c < anz) {
                                            Val = ValList.removeFirst();
                                            (*double_vec)[c]=(double)atof((const char*)Val);
                                            c++;
                                        }
                                        return double_vec;
                                }
                case KS_VT_TIME_VEC :
                                {
                                        anz  = ValList.size();
                                        KsTimeVecValue *time_vec = new KsTimeVecValue(anz);

                                        c = 0;
                                        while(c < anz)  {
                                            Val = ValList.removeFirst();
                                            time.tm_hour = 0;
                                            time.tm_min = 0;
                                            time.tm_sec = 0;
                                            tv.tv_usec = 0;

                                            ifb_normalize_usecs(Val);
                                            sscanf((const char*)Val, "%d-%d-%d %d:%d:%d.%ld"
                                                    , &time.tm_year, &time.tm_mon, &time.tm_mday
                                                        , &time.tm_hour, &time.tm_min, &time.tm_sec, &tv.tv_usec);

                                            time.tm_year -= 1900;
                                            time.tm_mon -=1;
                                            time.tm_isdst = -1; /* keine Sommer-Winterzeit */

                                            tv.tv_sec = mktime(&time);

                                            (*time_vec)[c].tv_sec = tv.tv_sec;
                                            (*time_vec)[c].tv_usec= tv.tv_usec;

                                            c++;
                                        }
                                        return time_vec;
                                }
                case KS_VT_TIME_SPAN_VEC :
                                {
                                        anz  = ValList.size();
                                        KsTimeSpanVecValue *timespan_vec = new KsTimeSpanVecValue(anz);

                                        c = 0;
                                        while(c < anz) {
                                            Val = ValList.removeFirst();
                                            sec = 0;
                                            usec = 0;

                                            ifb_normalize_usecs(Val);
                                            sscanf((const char*)Val, "%ld.%ld", &sec, &usec);
                                            (*timespan_vec)[c].tv_sec = sec;
                                            (*timespan_vec)[c].tv_usec = usec;
                                            c++;
                                        }
                                        return timespan_vec;
                                }
                case KS_VT_STRING_VEC :
                                {
                                        anz  = ValList.size();
                                        KsStringVecValue *string_vec = new KsStringVecValue(anz);

                                        c = 0;
                                        while(c < anz) {
                                            Val = ValList.removeFirst();
                                            if(prepareString) {
                                                err = PrepareStringValue(Val);
                                                if(err) {
                                                    delete string_vec;
                                                    return 0;
                                                }
                                            }
                                            (*string_vec)[c] = Val;
                                            c++;
                                        }
                                        return string_vec;
                                }
                case KS_VT_BYTE_VEC :
                                {
                                        anz  = ValList.size();
                                        KsByteVecValue *byte_vec = new KsByteVecValue(anz);

                                        c = 0;
                                        while(c < anz) {
                                            Val = ValList.removeFirst();
                                            (*byte_vec)[c]= (char)atoi( (const char*)Val);
                                            c++;
                                        }
                                        return byte_vec;
                                }
                default :        
                                        err = KS_ERR_NOTIMPLEMENTED;
                                        break;
    } /* case */

    return 0;

} /* ifb_CrNewKsValue() */

/*****************************************************************************/
KS_RESULT PrepareStringValue(PltString& wert) {
/*****************************************************************************/
    char *pc;
    char *ph;
    char *hstr;

    if(!wert.len() ) {
        return KS_ERR_OK;
    }
    hstr = (char*)malloc(wert.len() +1);
    if(!hstr) {
        return OV_ERR_HEAPOUTOFMEMORY;
    }

#if 1

    ph = (char*)((const char*)wert);
    pc = hstr;
    
    while(ph && (*ph)) {
        if(*ph == '"') {
            // String-Anfang?
            if(pc == hstr) {
                // Viel Spass...
            } else {
                // '\\' for dem '"'?
                pc--;
                if(*pc != '\\') {
                    // Auf die alte Position
                    pc++;
                }
            }
        }
        *pc = *ph;
        pc++;
        ph++;
    }
    // String-Ende markieren
    *pc = '\0';
    wert = hstr;
            
#else
    char *pAnf;
    // Prufen, ob am Anfang des Strings " steht
    strcpy(hstr, (const char*)wert);
    pc = hstr;
    if((*pc) == '"' ) {
        pc++; pAnf = pc;
            while(pc && (*pc) ) {
                if((*pc) == '"') {
                    // Steht \ vor dem " ?
                    pc--;
                    if((*pc) == '\\') {
                        pc++;   // Auf alte Position setzen
                    } else {
                        pc++; *pc = '\0'; // String-Ende markieren
                        break;
                    }
                }
                pc++;
            }
            wert = pAnf;
    }
#endif


    free(hstr);
    return KS_ERR_OK;
}


#define UNICODE_SELF        0x80
#define UCHAR(c) ((unsigned char) (c))
typedef unsigned short Iut_tclsplitlistUniChar;
 
static const unsigned char totalBytes[256] = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};


int
tcllistsplitUniCharToUtf(
    int    ch
    ,char *str
) {

    if ((ch > 0) && (ch < UNICODE_SELF)) {
            str[0] = (char) ch;
            return 1;
    }
    if (ch <= 0x7FF) {
            str[1] = (char) ((ch | 0x80) & 0xBF);
            str[0] = (char) ((ch >> 6) | 0xC0);
            return 2;
    }
    if (ch <= 0xFFFF) {
three:
            str[2] = (char) ((ch | 0x80) & 0xBF);
            str[1] = (char) (((ch >> 6) | 0x80) & 0xBF);
            str[0] = (char) ((ch >> 12) | 0xE0);
            return 3;
    }

    ch = 0xFFFD;
    goto three;
}

int
tcllistsplitUtfToUniChar(
    register const char   *str
    ,register Iut_tclsplitlistUniChar *chPtr
) {
    register int byte;
    
    /*
     * Unroll 1 to 3 byte UTF-8 sequences, use loop to handle longer ones.
     */

    byte = *((unsigned char *) str);
    if (byte < 0xC0) {
            /*
             * Handles properly formed UTF-8 characters between 0x01 and 0x7F.
             * Also treats \0 and naked trail bytes 0x80 to 0xBF as valid
             * characters representing themselves.
             */

            *chPtr = (Iut_tclsplitlistUniChar) byte;
            return 1;
    } else if (byte < 0xE0) {
            if ((str[1] & 0xC0) == 0x80) {
                /*
                 * Two-byte-character lead-byte followed by a trail-byte.
                 */

                *chPtr = (Iut_tclsplitlistUniChar) (((byte & 0x1F) << 6) | (str[1] & 0x3F));
                return 2;
            }
            /*
             * A two-byte-character lead-byte not followed by trail-byte
             * represents itself.
             */

            *chPtr = (Iut_tclsplitlistUniChar) byte;
            return 1;
    } else if (byte < 0xF0) {
            if (((str[1] & 0xC0) == 0x80) && ((str[2] & 0xC0) == 0x80)) {
                /*
                 * Three-byte-character lead byte followed by two trail bytes.
                 */

                *chPtr = (Iut_tclsplitlistUniChar) (((byte & 0x0F) << 12) 
                        | ((str[1] & 0x3F) << 6) | (str[2] & 0x3F));
                return 3;
            }
            /*
             * A three-byte-character lead-byte not followed by two trail-bytes
             * represents itself.
             */

            *chPtr = (Iut_tclsplitlistUniChar) byte;
            return 1;
    }

    *chPtr = (Iut_tclsplitlistUniChar) byte;
    return 1;
}

int
tcllistsplitUtfCharComplete(
    const char *str     /* String to check if first few bytes contain a complete UTF-8 character. */
    ,int        len     /* Length of above string in bytes. */
) {
    int ch;

    ch = *((unsigned char *) str);
    return (int)(len >= totalBytes[ch]);
}

int tcllistsplitParseHex(
    const char   *src
    ,int          numBytes
    ,Iut_tclsplitlistUniChar *resultPtr
) {
    Iut_tclsplitlistUniChar result = 0;
    register const char *p = src;

    while (numBytes--) {
            unsigned char digit = UCHAR(*p);

            if (!isxdigit(digit))
                break;

            ++p;
            result <<= 4;

            if (digit >= 'a') {
                result |= (10 + digit - 'a');
            } else if (digit >= 'A') {
                result |= (10 + digit - 'A');
            } else {
                result |= (digit - '0');
            }
    }

    *resultPtr = result;
    return (p - src);
}

int
tcllistsplitParseBackslash(
    const char *src         /* Points to the backslash character of a a backslash sequence */
    ,int        numBytes    /* Max number of bytes to scan */
    ,int       *readPtr     /* NULL, or points to storage where the number of bytes scanned should be written. */
    ,char      *dst         /* NULL, or points to buffer */
) {
    register const char *p = src+1;
    unsigned short       result;
    int                  count;
    char                 buf[32];

    if (numBytes == 0) {
            if (readPtr) {
                *readPtr = 0;
            }
            return 0;
    }

    if (dst == NULL) {
        dst = buf;
    }

    if (numBytes == 1) {
            /* Can only scan the backslash.  Return it. */
            result = '\\';
            count = 1;
            goto done;
    }

    count = 2;
    switch (*p) {
        /*
         * Note: in the conversions below, use absolute values (e.g.,
         * 0xa) rather than symbolic values (e.g. \n) that get converted
         * by the compiler.  It's possible that compilers on some
         * platforms will do the symbolic conversions differently, which
         * could result in non-portable Tcl scripts.
         */

        case 'a':
            result = 0x7;
            break;
        case 'b':
            result = 0x8;
            break;
        case 'f':
            result = 0xc;
            break;
        case 'n':
            result = 0xa;
            break;
        case 'r':
            result = 0xd;
            break;
        case 't':
            result = 0x9;
            break;
        case 'v':
            result = 0xb;
            break;
        case 'x':
                count += tcllistsplitParseHex(p+1, numBytes-1, &result);
                if (count == 2) {
                        /* No hexadigits -> This is just "x". */
                        result = 'x';
                } else {
                        /* Keep only the last byte (2 hex digits) */
                        result = (unsigned char) result;
                 }
           break;
        case 'u':
                count += tcllistsplitParseHex(p+1, (numBytes > 5) ? 4 : numBytes-1, &result);
                if (count == 2) {
                        /* No hexadigits -> This is just "u". */
                        result = 'u';
                }
            break;
        case '\n':
            count--;
            do {
                p++; count++;
            } while ((count < numBytes) && ((*p == ' ') || (*p == '\t')));
            result = ' ';
            break;
        case 0:
            result = '\\';
            count = 1;
            break;
        default:
            /*
             * Check for an octal number \oo?o?
             */
            if (isdigit(UCHAR(*p)) && (UCHAR(*p) < '8')) { /* INTL: digit */
                result = (unsigned char)(*p - '0');
                p++;
                if ((numBytes == 2) || !isdigit(UCHAR(*p)) /* INTL: digit */
                                || (UCHAR(*p) >= '8')) { 
                    break;
                }
                count = 3;
                result = (unsigned char)((result << 3) + (*p - '0'));
                p++;
                if ((numBytes == 3) || !isdigit(UCHAR(*p)) /* INTL: digit */
                                || (UCHAR(*p) >= '8')) {
                    break;
                }
                count = 4;
                result = (unsigned char)((result << 3) + (*p - '0'));
                break;
            }
            /*
             * We have to convert here in case the user has put a
             * backslash in front of a multi-byte utf-8 character.
             * While this means nothing special, we shouldn't break up
             * a correct utf-8 character. [Bug #217987] test subst-3.2
                 */
                if (tcllistsplitUtfCharComplete(p, numBytes - 1)) {
                    count = tcllistsplitUtfToUniChar(p, &result) + 1; /* +1 for '\' */
                } else {
                        char utfBytes[32];
                        memcpy(utfBytes, p, (size_t) (numBytes - 1));
                        utfBytes[numBytes - 1] = '\0';
                    count = tcllistsplitUtfToUniChar(utfBytes, &result) + 1;
                }
            break;
    }

    done:
    if (readPtr != NULL) {
        *readPtr = count;
    }
    return tcllistsplitUniCharToUtf((int) result, dst);
}

int tcllistsplitUtfBackslash(
    const char *src         /* Points to the backslash character of a backslash sequence. */
    ,int       *readPtr     /* Fill in with number of characters read from src, unless NULL. */
    ,char      *dst         /* Filled with the bytes represented by the backslash sequence. */
) {

#define LINE_LENGTH 128
    int numRead;
    int result;

    result = tcllistsplitParseBackslash(src, LINE_LENGTH, &numRead, dst);
    if (numRead == LINE_LENGTH) {
            /* We ate a whole line.  Pay the price of a strlen() */
            result = tcllistsplitParseBackslash(src, (int)strlen(src), &numRead, dst);
    }
    if (readPtr != NULL) {
            *readPtr = numRead;
    }
    return result;
}

KS_RESULT
tcllistsplitFindElement(
    const char   *list
    ,int           listLength
    ,const char  **elementPtr
    ,const char  **nextPtr
    ,int          *sizePtr
    ,int          *bracePtr
) {
    const char *p = list;
    const char *elemStart;          /* Points to first byte of first element. */
    const char *limit;              /* Points just after list's last byte. */
    int         openBraces = 0;     /* Brace nesting level during parse. */
    int         inQuotes = 0;
    int         size = 0;
    int         numChars;

    /*
     * Skim off leading white space and check for an opening brace or
     * quote. We treat embedded NULLs in the list as bytes belonging to
     * a list element.
     */

    limit = (list + listLength);
    while ((p < limit) && (isspace(UCHAR(*p)))) { /* INTL: ISO space. */
        p++;
    }
    if (p == limit) {                /* no element found */
        elemStart = limit;
        goto done;
    }

    if (*p == '{') {
        openBraces = 1;
        p++;
    } else if (*p == '"') {
        inQuotes = 1;
        p++;
    }
    elemStart = p;
    if (bracePtr != 0) {
        *bracePtr = openBraces;
    }

    /*
     * Find element's end (a space, close brace, or the end of the string).
     */

    while (p < limit) {
            switch (*p) {

                /*
                 * Open brace: don't treat specially unless the element is in
                 * braces. In this case, keep a nesting count.
                 */

                case '{':
                        if (openBraces != 0) {
                            openBraces++;
                        }
                        break;

                    /*
                     * Close brace: if element is in braces, keep nesting count and
                     * quit when the last close brace is seen.
                     */

                case '}':
                        if (openBraces > 1) {
                            openBraces--;
                        } else if (openBraces == 1) {
                            size = (p - elemStart);
                            p++;
                            if ((p >= limit) || isspace(UCHAR(*p))) { /* INTL: ISO space. */
                                    goto done;
                            }

                            /*
                             * Garbage after the closing brace; return an error.
                             */
                    return KS_ERR_BADVALUE;
                        }
                        break;

                    /*
                     * Backslash:  skip over everything up to the end of the
                     * backslash sequence.
                     */

                case '\\': {
                        tcllistsplitUtfBackslash(p, &numChars, NULL);
                        p += (numChars - 1);
                        break;
                    }

                    /*
                     * Space: ignore if element is in braces or quotes; otherwise
                     * terminate element.
                     */

                case ' ':
                case '\f':
                case '\n':
                case '\r':
                case '\t':
                case '\v':
                        if ((openBraces == 0) && !inQuotes) {
                            size = (p - elemStart);
                            goto done;
                        }
                        break;

                    /*
                     * Double-quote: if element is in quotes then terminate it.
                     */

                case '"':
                        if (inQuotes) {
                            size = (p - elemStart);
                            p++;
                            if ((p >= limit)
                                    || isspace(UCHAR(*p))) { /* INTL: ISO space */
                                    goto done;
                            }

                            /*
                             * Garbage after the closing quote; return an error.
                             */
                    return KS_ERR_BADVALUE;
                        }
                        break;
            }
            p++;
    }


    /*
     * End of list: terminate element.
     */

    if (p == limit) {
            if (openBraces != 0) {
                return KS_ERR_BADVALUE;
            } else if (inQuotes) {
                return KS_ERR_BADVALUE;
            }
            size = (p - elemStart);
    }

done:
    while ((p < limit) && (isspace(UCHAR(*p)))) { /* INTL: ISO space. */
            p++;
    }
    *elementPtr = elemStart;
    *nextPtr = p;
    if (sizePtr != 0) {
            *sizePtr = size;
    }
    return KS_ERR_OK;
}

int
tcllistsplitCopyAndCollapse(
    int          count
    ,const char *src
    ,char       *dst
) {
    register char c;
    int numRead;
    int newCount = 0;
    int backslashCount;

    for (c = *src;  count > 0;  src++, c = *src, count--) {
            if (c == '\\') {
                backslashCount = tcllistsplitUtfBackslash(src, &numRead, dst);
                dst += backslashCount;
                newCount += backslashCount;
                src += numRead-1;
                count -= numRead-1;
            } else {
                *dst = c;
                dst++;
                newCount++;
            }
    }
    *dst = 0;
    return newCount;
}

/*****************************************************************************/
KS_RESULT SeparateString(
/*****************************************************************************/
    PltString          &wert,
    KS_VAR_TYPE         typ,
    PltList<PltString> &Liste)
{
    char **argv;
    const char *l;
    char *p;
    int length, size, i,  elSize, brace;
    const char *element;
    KS_RESULT   result;
    const char *list;
    
    /*
     * Figure out how much space to allocate.  There must be enough
     * space for both the array of pointers and also for a copy of
     * the list.  To estimate the number of pointers needed, count
     * the number of space characters in the list.
     */
    list = (const char*)wert;
    l = (char *)list;
    for (size = 1; *l != 0; l++) {
        if (isspace(UCHAR(*l))) {
            size++;
        }
    }
    size++;                        /* Leave space for final NULL pointer. */
    argv = (char **)malloc((unsigned)((size * sizeof(char *)) + (l - list) + 1));
    length = (int)wert.len();
    for (i = 0, p = ((char *) argv) + size*sizeof(char *); *list != 0;  i++) {
        const char *prevList = list;
        
        result = tcllistsplitFindElement(list, length, &element, &list, &elSize, &brace);
        if (result != KS_ERR_OK) {
            free((char *) argv);
            return result;
        }
        length -= (list - prevList);
        if (*element == 0) {
            break;
        }
        if (i >= size) {
            free((char *) argv);
            return KS_ERR_BADVALUE;
        }
            argv[i] = p;
            if (brace) {
                memcpy((void *) p, (void *) element, (size_t) elSize);
                p += elSize;
                *p = 0;
                p++;
            } else {
                tcllistsplitCopyAndCollapse(elSize, element, p);
                p += elSize+1;
            }
    }

    size = i;
    for(i=0; i<size; i++) {
        Liste.addLast(argv[i]);
    }
    free((char *) argv);
    
    return KS_ERR_OK;

} /* SeparateString() */
