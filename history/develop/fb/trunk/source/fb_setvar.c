/******************************************************************************
***                                                                         ***
***   iFBSpro   -   Funktionsbaustein-Model                                 ***
***   #####################################                                 ***
***                                                                         ***
***   L T S o f t                                                           ***
***   Agentur für Leittechnik Software GmbH                                 ***
***   Brabanterstr. 13                                                      ***
***   D-50171 Kerpen                                                        ***
***   Tel : 02237/92869-2                                                   ***
***   Fax : 02237/92869-9                                                   ***
***   e-Mail   : ltsoft@ltsoft.de                                           ***
***   Internet : http://www.ltsoft.de                                       ***
***                                                                         ***
***   -------------------------------------------------------------------   ***
***                                                                         ***
***   Implementierung des Funktionsbausteinsystems IFBSpro                  ***
***   RWTH,   Aachen                                                        ***
***   LTSoft, Kerpen                                                        ***
***                                                                         ***
*******************************************************************************
*                                                                             *
*   Datei                                                                     *
*   -----                                                                     *
*   fb_setvar.c                                                               *
*                                                                             *
*                                                                             *
*   Historie                                                                  *
*   --------                                                                  *
*   2012-05-04 Alexander Neugebauer: Erstellung, LTSoft GmbH, Kerpen          *
*                                                                             *
*                                                                             *
*   Beschreibung                                                              *
*   ------------                                                              *
*   Typ-unabaendiges Setzen der Variablen-Werten                              *
*                                                                             *
******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_fb
#define OV_COMPILE_LIBRARY_fb
#endif

#include "libov/ov_macros.h"
#include "libov/ov_string.h"
#include "libov/ov_time.h"
#include "libov/ov_path.h"
#include "libov/ov_variable.h"
#include "fb.h"

#include <limits.h>
#include <float.h>
#include <time.h>
#include <sys/timeb.h>


#define SETVAR_DEBUG 0
/*	----------------------------------------------------------------------	*/
/*  Hilfsfunktionen                                                     	*/
/*	----------------------------------------------------------------------	*/
long int fb___str2time_getusec(char* pusec) {
    char         *ph;
    unsigned int  i;
    long int      val = 0;
    
    ph = pusec;
    // Bis auf 6 Zeichen mit '0' fuellen
    for(i=0; i<6; i++) {
        if(!ph || (!(*ph)) ) {
            *ph = '0';
            ph++;
            *ph = '\0';
            ph--;                  
        }
        ph++;
    }
    ph = pusec;
    ph[6] = '\0';

    // Forgestellte '0' weg   
    while(ph && (*ph) && (*ph == '0') ) {
        ph++;
    }
    if(ph && (*ph) ) {
        val = atol(ph);
    }            
    return val;
}
OV_RESULT fb_ascii2time(const OV_STRING timeStr, OV_TIME* ptime) {
    char        hStr[64];
    char        *ph;
	struct tm	tm;
	time_t		secs;
	OV_UINT		usecs;

	ptime->secs = 0;
	ptime->usecs = 0;

    if(!ov_string_compare(timeStr, "")) {
        return OV_ERR_BADVALUE;
    }
    strncpy(hStr, timeStr, 31);
    hStr[31] = '\0';
    ph = hStr;
    while(ph && (*ph) ) {
        if( *ph == '/' ) {
            *ph = '-';
        }
        ph++;
    }
    /* Mikrosekunden */
    usecs = 0;
    ph = strstr(hStr, ".");
    if( ph ) {
        *ph = '\0';
        ph++;
        usecs = fb___str2time_getusec(ph); /* Anfang des uSec-Bereichs merken */
    }
	memset(&tm, 0, sizeof(tm));
	sscanf(timeStr, "%d-%d-%d %d:%d:%d", &tm.tm_year, &tm.tm_mon,
		&tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
	tm.tm_year -= 1900;
	tm.tm_mon--;
	tm.tm_isdst  = 0;
	secs = mktime(&tm);
	if(secs == -1) {
		return OV_ERR_BADPARAM;
	}
	/*
	*	time is OK, return value
	*/
	ptime->secs = secs;
	ptime->usecs = usecs;
	return OV_ERR_OK;
}
void fb_time2ascii(const OV_TIME *ptim, char *out) {
	/*
	*	Lokale Variablen
	*/
    time_t secs = ptim->secs;
    struct tm* t;

    t=localtime(&secs);
    if(!t) {
        strcpy(out, "1970-01-01 00:00:00.000");
    } else {
        sprintf(out, "%4d-%02d-%02d %02d:%02d:%02d.%06ld",
                t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min,
                t->tm_sec, ptim->usecs);
    }
    return;
}
void fb_fs2double(const OV_STRING floatStr, OV_DOUBLE *doublWert) {
    char *end;
    double d;

    *doublWert = 0;
    d = strtod(floatStr, &end);
    if (end == floatStr) {
	   return;
    }
    *doublWert = d;
    return;
}
void fb_f2ts(const OV_STRING floatStr, OV_INT *ganzzahl, OV_INT *nachkomma) {
    char    hStr[64];
    char    *ph;
    
    *ganzzahl  = 0;
    *nachkomma = 0;
    
    if(!ov_string_compare(floatStr, "")) {
        return;
    }
    /* Trim left */
    ph = floatStr;
    while(ph && (*ph)) {
        if( (*ph !=' ') && (*ph != '\t') ) {
            break;
        }
        ph++;
    }
    if(!ph) {
        return;
    }
    strncpy(hStr, ph, 63);
    hStr[63] = '\0';
    /* Format "dddd.ddd" ? */
    ph = strstr(hStr, ".");
    if(ph) {
        *ph = '\0';
        ph++;        
        *nachkomma = fb___str2time_getusec(ph);
    }
    *ganzzahl = atol(hStr);
    if(*ganzzahl < 0) {
        *nachkomma *= -1;
    }
}
OV_RESULT fb_ascii2timespan(const OV_STRING timeStr, OV_TIME_SPAN* ptime) {
    char        hStr[32];
    char        *ph;
	struct tm	tm;
	OV_INT		secs;
	OV_INT		usecs;

    int         anz;
    int         mul;
    
    if(!timeStr) {
        return OV_ERR_BADVALUE;
    }
    strncpy(hStr, timeStr, 31);
    hStr[31] = '\0';

    /* Format "HH:MM:SS.uuuuuu"? */
	memset(&tm, 0, sizeof(tm));
	usecs = 0;
	ptime->usecs = 0;
	mul = 1;
	ph = hStr;
	if(*ph == '-') {
        ph++;
        mul = -1;
    }
	anz = sscanf(ph, "%d:%d:%d.%lu", &tm.tm_hour, &tm.tm_min, &tm.tm_sec, &usecs);
	if(anz >= 3) {
        ptime->secs = mul * (tm.tm_hour*60*60 + tm.tm_min*60 + tm.tm_sec);
        ptime->usecs = mul * usecs;
	    return OV_ERR_OK;
    }
    
	/* Format "SS.uuuuuu" ? */
    fb_f2ts(timeStr, &secs, &usecs);
	ptime->secs = secs;
	ptime->usecs = usecs;
	return OV_ERR_OK;
}
void fb_timespan2ascii(const OV_TIME_SPAN* pts, OV_STRING out) {
    OV_TIME_SPAN  ts;
    char         *vorz = "";
    
    ts.secs = pts->secs;
    ts.usecs= pts->usecs;
    if( (ts.secs<0) || ((ts.secs == 0) && (ts.usecs < 0)) ) {
        vorz = "-";
        
        if( ts.secs < 0 ) {
            ts.secs *= -1;
        }
        if( ts.usecs < 0 ) {
            ts.usecs *= -1;
        }
    }
    sprintf(out, "%s%ld.%06ld", vorz, ts.secs, ts.usecs);
}


/*	----------------------------------------------------------------------
*   Scalar variable
*	------------------------------------------------------------------- */
OV_RESULT fb_set_scalar_byte_varvalue_eval(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed,
    OV_BYTE                     *pvalue
) {
    /* Wert geaendert? */
    if( *((OV_BYTE*)pelem->pvalue) != *pvalue ) {
        *changed = TRUE;
    }
	/* Set-FNC vorhanden? */
    if(pelem->elemunion.pvar->v_setfnc ) {
        return ((OV_FNCPTR_SET(BYTE))pelem->elemunion.pvar->v_setfnc)(pobj, *pvalue);
    }
    if(*changed == TRUE) {
        *((OV_BYTE*)pelem->pvalue) = *pvalue;
    }
    return OV_ERR_OK;
}
/*	------------------------------------------------------------------- */
OV_RESULT fb_set_scalar_byte_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {
	OV_BYTE   val;
	OV_INT    ival;
	OV_UINT   uival;
	OV_SINGLE sval;
	OV_DOUBLE dval;
	OV_UINT     ksTyp;
	
	/* Typ gleich? */
	ksTyp = pvarcurrprops->value.vartype & OV_VT_KSMASK;
	if(ksTyp == OV_VT_BYTE) {
	    val = pvarcurrprops->value.valueunion.val_byte;
	    return fb_set_scalar_byte_varvalue_eval(pobj,pelem,pvarcurrprops,changed,&val);
	}
	
	/* Wert holen und vergleichen */
    switch(ksTyp) {
        case OV_VT_VOID:
            val = 0;
            break;
        case OV_VT_BYTE:
            val = pvarcurrprops->value.valueunion.val_byte;
            break;   
        case OV_VT_BOOL:
            val = pvarcurrprops->value.valueunion.val_bool ? 1 : 0;
            break;
        case OV_VT_INT: {
                ival = pvarcurrprops->value.valueunion.val_int;
                val = (OV_BYTE)ival;
            }
            break;
        case OV_VT_UINT: {
                uival = pvarcurrprops->value.valueunion.val_uint;
                val = (OV_BYTE)uival;
            }
            break;
        case OV_VT_SINGLE: {
                sval = pvarcurrprops->value.valueunion.val_single;
                val = (OV_BYTE)sval;
            }
            break;
        case OV_VT_DOUBLE: {
                dval = pvarcurrprops->value.valueunion.val_double;
                val = (OV_BYTE)dval;
            }
            break;
        case OV_VT_TIME: {
                uival = pvarcurrprops->value.valueunion.val_time.secs;
                val = (OV_BYTE)uival;
            }
            break;
        case OV_VT_TIME_SPAN: {
                ival = pvarcurrprops->value.valueunion.val_time_span.secs;
                val = (OV_BYTE)ival;
            }
            break;
        case OV_VT_STRING: {
                if(!ov_string_compare(pvarcurrprops->value.valueunion.val_string, "")) {
                    val = 0;
                } else {
                    ival = atol(pvarcurrprops->value.valueunion.val_string);
                    val = (OV_BYTE)ival;
                }
            }
            break;
        /* Vectoren */
        case OV_VT_BYTE_VEC: {
                if(pvarcurrprops->value.valueunion.val_byte_vec.veclen == 0) {
                    val = 0;
                } else {
                    val = ((OV_BYTE*)(pvarcurrprops->value.valueunion.val_byte_vec.value))[0];
                }
            }
            break;
        case OV_VT_BOOL_VEC: {
                if(pvarcurrprops->value.valueunion.val_bool_vec.veclen == 0) {
                    val = 0;
                } else {
                    val = ((OV_BOOL*)(pvarcurrprops->value.valueunion.val_bool_vec.value))[0] ? 1 : 0;
                }
            }
            break;
        case OV_VT_INT_VEC: {
                if(pvarcurrprops->value.valueunion.val_int_vec.veclen == 0) {
                    val = 0;
                } else {
                    ival = ((OV_INT*)(pvarcurrprops->value.valueunion.val_int_vec.value))[0];
                    val = (OV_BYTE)ival;
                }
            }
            break;
        case OV_VT_UINT_VEC: {
                if(pvarcurrprops->value.valueunion.val_uint_vec.veclen == 0) {
                    val = 0;
                } else {
                    uival = ((OV_UINT*)(pvarcurrprops->value.valueunion.val_uint_vec.value))[0];
                    val = (OV_BYTE)uival;
                }
            }
            break;
        case OV_VT_SINGLE_VEC: {
                if(pvarcurrprops->value.valueunion.val_single_vec.veclen == 0) {
                    val = 0;
                } else {
                    sval = ((OV_SINGLE*)(pvarcurrprops->value.valueunion.val_single_vec.value))[0];
                    val = (OV_BYTE)sval;
                }
            }
            break;
        case OV_VT_DOUBLE_VEC: {
                if(pvarcurrprops->value.valueunion.val_double_vec.veclen == 0) {
                    val = 0;
                } else {
                    dval = ((OV_DOUBLE*)(pvarcurrprops->value.valueunion.val_double_vec.value))[0];
                    val = (OV_BYTE)dval;
                }
            }
            break;
        case OV_VT_TIME_VEC:  {
                if(pvarcurrprops->value.valueunion.val_time_vec.veclen == 0) {
                    val = 0;
                } else {
                    uival = ((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[0].secs;
                    val = (OV_BYTE)uival;
                }
            }
            break;
        case OV_VT_TIME_SPAN_VEC: {
                if(pvarcurrprops->value.valueunion.val_time_span_vec.veclen == 0) {
                    val = 0;
                } else {
                    ival = ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[0].secs;
                    val = (OV_BYTE)ival;
                }
            }
            break;
        case OV_VT_STRING_VEC: {
                OV_STRING str;
                if(pvarcurrprops->value.valueunion.val_string_vec.veclen == 0) {
                    val = 0;
                } else {
                    str = ((OV_STRING*)(pvarcurrprops->value.valueunion.val_string_vec.value))[0];
                    if(!ov_string_compare(str, "")) {
                        val = 0;
                    } else {
                        ival = (OV_INT)atol(str);
                        val = (OV_BYTE)ival;
                    }
                }
            }
            break;
        default:
	           return OV_ERR_BADTYPE;
    }

    return fb_set_scalar_byte_varvalue_eval(pobj,pelem,pvarcurrprops,changed,&val);
}
/*	======================================================================	*/
OV_RESULT fb_set_scalar_bool_varvalue_eval(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed,
	OV_BOOL                     *pvalue
) {
    /* PV? */
    if(pelem->elemunion.pvar->v_vartype == OV_VT_BOOL) {
        if(	*((OV_BOOL*)pelem->pvalue) != *pvalue ) {
            *changed = TRUE;
        }
    } else {
        /* PV */
		if( (((OV_BOOL_PV*)pelem->pvalue)->value != *pvalue) ||
    		(((OV_BOOL_PV*)pelem->pvalue)->state != pvarcurrprops->state) ||
    		(((OV_BOOL_PV*)pelem->pvalue)->time.secs != pvarcurrprops->time.secs) ||
    		(((OV_BOOL_PV*)pelem->pvalue)->time.usecs == pvarcurrprops->time.usecs) ) {
            *changed = TRUE;
        }
    }
        
	/* Set-Funktion vorhanden? */
    if( pelem->elemunion.pvar->v_setfnc ) {
        /* PV ? */
        if(pelem->elemunion.pvar->v_vartype == OV_VT_BOOL_PV) {
        	OV_BOOL_PV var;
			var.value = *pvalue;
			var.state = pvarcurrprops->state;
			var.time = pvarcurrprops->time;
			return ((OV_FNCPTR_SETPV(BOOL))pelem->elemunion.pvar->v_setfnc)(pobj, &var);
        }
        /* Sonnst: */
        return ((OV_FNCPTR_SET(BOOL))pelem->elemunion.pvar->v_setfnc)(pobj, *pvalue);
    }

    if(*changed == TRUE) {
        /* PV ? */
        if(pelem->elemunion.pvar->v_vartype == OV_VT_BOOL_PV) {
            ((OV_BOOL_PV*)pelem->pvalue)->value = *pvalue;
            ((OV_BOOL_PV*)pelem->pvalue)->state = pvarcurrprops->state;
        	((OV_BOOL_PV*)pelem->pvalue)->time = pvarcurrprops->time;
        } else {
    	   *((OV_BOOL*)pelem->pvalue) = *pvalue;
        }
    }

    return OV_ERR_OK;
}
/*	----------------------------------------------------------------------	*/
OV_RESULT fb_set_scalar_bool_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {
	OV_BOOL   val;
	OV_INT    ival;
	OV_UINT     ksTyp;
	
	/* Typ gleich? */
	ksTyp = pvarcurrprops->value.vartype & OV_VT_KSMASK;
	if(ksTyp == OV_VT_BYTE) {
        val = pvarcurrprops->value.valueunion.val_bool;
        return fb_set_scalar_bool_varvalue_eval(pobj,pelem,pvarcurrprops,changed,&val);
	}
	
	/* Wert holen und vergleichen */
    switch(ksTyp) {
        case OV_VT_VOID:
            val = FALSE;
            break;
        case OV_VT_BYTE:
            val = pvarcurrprops->value.valueunion.val_byte ? TRUE : FALSE;
            break;   
        case OV_VT_BOOL:
            val = pvarcurrprops->value.valueunion.val_bool;
            break;
        case OV_VT_INT:
            val = pvarcurrprops->value.valueunion.val_int ? TRUE : FALSE;
            break;
        case OV_VT_UINT:
            val = pvarcurrprops->value.valueunion.val_uint ? TRUE : FALSE;
            break;
        case OV_VT_SINGLE:
            val = pvarcurrprops->value.valueunion.val_single ? TRUE : FALSE;
            break;
        case OV_VT_DOUBLE:
            val = pvarcurrprops->value.valueunion.val_double ? TRUE : FALSE;
            break;
        case OV_VT_TIME: {
                val = pvarcurrprops->value.valueunion.val_time.secs ? TRUE : FALSE;
                if(val == FALSE) {
                    val = pvarcurrprops->value.valueunion.val_time.usecs ? TRUE : FALSE;
                }
            }
            break;
        case OV_VT_TIME_SPAN: {
                val = pvarcurrprops->value.valueunion.val_time_span.secs ? TRUE : FALSE;
                if(val == FALSE) {
                    val = pvarcurrprops->value.valueunion.val_time_span.usecs ? TRUE : FALSE;
                }
            }
            break;
        case OV_VT_STRING: {
                if(!ov_string_compare(pvarcurrprops->value.valueunion.val_string, "")) {
                    val = FALSE;
                } else {
                    if( !ov_string_compare(pvarcurrprops->value.valueunion.val_string, "TRUE") ||
                        !ov_string_compare(pvarcurrprops->value.valueunion.val_string, "true") ) {
                        val = TRUE;
                    } else {
                        ival = atol(pvarcurrprops->value.valueunion.val_string);
                        if(ival == 0) {
                            val = FALSE;
                        } else {
                            val = TRUE;
                        }
                    }
                }
            }
            break;
        /* Vectoren */
        case OV_VT_BYTE_VEC: {
                if(pvarcurrprops->value.valueunion.val_byte_vec.veclen == 0) {
                    val = FALSE;
                } else {
                    val = ((OV_BYTE*)(pvarcurrprops->value.valueunion.val_byte_vec.value))[0] ? TRUE : FALSE;
                }
            }
            break;
        case OV_VT_BOOL_VEC: {
                if(pvarcurrprops->value.valueunion.val_bool_vec.veclen == 0) {
                    val = FALSE;
                } else {
                    val = ((OV_BOOL*)(pvarcurrprops->value.valueunion.val_bool_vec.value))[0];
                }
            }
            break;
        case OV_VT_INT_VEC: {
                if(pvarcurrprops->value.valueunion.val_int_vec.veclen == 0) {
                    val = FALSE;
                } else {
                    val = ((OV_INT*)(pvarcurrprops->value.valueunion.val_int_vec.value))[0] ? TRUE : FALSE;
                }
            }
            break;
        case OV_VT_UINT_VEC: {
                if(pvarcurrprops->value.valueunion.val_uint_vec.veclen == 0) {
                    val = FALSE;
                } else {
                    val = ((OV_UINT*)(pvarcurrprops->value.valueunion.val_uint_vec.value))[0] ? TRUE : FALSE;
                }
            }
            break;
        case OV_VT_SINGLE_VEC: {
                if(pvarcurrprops->value.valueunion.val_single_vec.veclen == 0) {
                    val = FALSE;
                } else {
                    val = ((OV_SINGLE*)(pvarcurrprops->value.valueunion.val_single_vec.value))[0] ? TRUE : FALSE;
                }
            }
            break;
        case OV_VT_DOUBLE_VEC: {
                if(pvarcurrprops->value.valueunion.val_double_vec.veclen == 0) {
                    val = FALSE;
                } else {
                    val = ((OV_DOUBLE*)(pvarcurrprops->value.valueunion.val_double_vec.value))[0] ? TRUE : FALSE;
                }
            }
            break;
        case OV_VT_TIME_VEC:  {
                if(pvarcurrprops->value.valueunion.val_time_vec.veclen == 0) {
                    val = FALSE;
                } else {
                    val = ((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[0].secs ? TRUE : FALSE;
                    if(val == FALSE) {
                        val = ((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[0].usecs ? TRUE : FALSE;
                    }
                }
            }
            break;
        case OV_VT_TIME_SPAN_VEC: {
                if(pvarcurrprops->value.valueunion.val_time_span_vec.veclen == 0) {
                    val = FALSE;
                } else {
                    val = ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[0].secs ? TRUE : FALSE;
                    if(val == FALSE) {
                        val = ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[0].usecs ? TRUE : FALSE;
                    }
                }
            }
            break;
        case OV_VT_STRING_VEC: {
                OV_STRING str;
                if(pvarcurrprops->value.valueunion.val_string_vec.veclen == 0) {
                    val = 0;
                } else {
                    str = ((OV_STRING*)(pvarcurrprops->value.valueunion.val_string_vec.value))[0];
                    if(!str) {
                        val = FALSE;
                    } else {
                        if( !ov_string_compare(str, "TRUE") ||
                            !ov_string_compare(str, "true") ) {
                            val = TRUE;
                        } else {
                            ival = (OV_INT)atol(str);
                            val = ival ? TRUE : FALSE;
                        }
                    }
                }
            }
            break;
        default:
	           return OV_ERR_BADTYPE;
    }

    return fb_set_scalar_bool_varvalue_eval(pobj,pelem,pvarcurrprops,changed,&val);
}

/*	======================================================================	*/
OV_RESULT fb_set_scalar_int_varvalue_eval(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed,
	OV_INT                      *pvalue
) {
    /* PV? */
    if(pelem->elemunion.pvar->v_vartype == OV_VT_INT) {
        if(	*((OV_INT*)pelem->pvalue) != *pvalue ) {
            *changed = TRUE;
        }
    } else {
        /* PV */
		if( (((OV_INT_PV*)pelem->pvalue)->value != *pvalue) ||
    		(((OV_INT_PV*)pelem->pvalue)->state != pvarcurrprops->state) ||
    		(((OV_INT_PV*)pelem->pvalue)->time.secs != pvarcurrprops->time.secs) ||
    		(((OV_INT_PV*)pelem->pvalue)->time.usecs == pvarcurrprops->time.usecs) ) {
            *changed = TRUE;
        }
    }
        
	/* Set-Funktion vorhanden? */
    if( pelem->elemunion.pvar->v_setfnc ) {
        /* PV ? */
        if(pelem->elemunion.pvar->v_vartype == OV_VT_INT_PV) {
        	OV_INT_PV var;
			var.value = *pvalue;
			var.state = pvarcurrprops->state;
			var.time = pvarcurrprops->time;
			return ((OV_FNCPTR_SETPV(INT))pelem->elemunion.pvar->v_setfnc)(pobj, &var);
        }
        /* Sonnst: */
        return ((OV_FNCPTR_SET(INT))pelem->elemunion.pvar->v_setfnc)(pobj, *pvalue);
    }

    if(*changed == TRUE) {
        /* PV ? */
        if(pelem->elemunion.pvar->v_vartype == OV_VT_INT_PV) {
            ((OV_INT_PV*)pelem->pvalue)->value = *pvalue;
            ((OV_INT_PV*)pelem->pvalue)->state = pvarcurrprops->state;
        	((OV_INT_PV*)pelem->pvalue)->time = pvarcurrprops->time;
        } else {
    	   *((OV_INT*)pelem->pvalue) = *pvalue;
        }
    }

    return OV_ERR_OK;
}
/*	----------------------------------------------------------------------	*/
OV_RESULT fb_set_scalar_int_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {
	OV_INT    val;
	OV_UINT   uival;
	OV_SINGLE sval;
	OV_DOUBLE dval;
	OV_UINT     ksTyp;
	
	/* Typ gleich? */
	ksTyp = pvarcurrprops->value.vartype & OV_VT_KSMASK;
	if(ksTyp == OV_VT_INT) {
        val = pvarcurrprops->value.valueunion.val_int;
        return fb_set_scalar_int_varvalue_eval(pobj,pelem,pvarcurrprops,changed,&val);
	}
	
    switch(ksTyp) {
        case OV_VT_VOID:
            val = 0;
            break;
        case OV_VT_BYTE:
            val = (OV_INT)pvarcurrprops->value.valueunion.val_byte;
            break;   
        case OV_VT_BOOL:
            val = pvarcurrprops->value.valueunion.val_bool ? 1 : 0;
            break;
        case OV_VT_INT:
                val = pvarcurrprops->value.valueunion.val_int;
            break;
        case OV_VT_UINT: {
                uival = pvarcurrprops->value.valueunion.val_uint;
                val = (OV_INT)uival;
            }
            break;
        case OV_VT_SINGLE: {
                sval = pvarcurrprops->value.valueunion.val_single;
                val = (OV_INT)sval;
            }
            break;
        case OV_VT_DOUBLE: {
                dval = pvarcurrprops->value.valueunion.val_double;
                val = (OV_INT)dval;
            }
            break;
        case OV_VT_TIME: {
                uival = pvarcurrprops->value.valueunion.val_time.secs;
                val = (OV_INT)uival;
            }
            break;
        case OV_VT_TIME_SPAN: {
                val = pvarcurrprops->value.valueunion.val_time_span.secs;
            }
            break;
        case OV_VT_STRING: {
                if(!ov_string_compare(pvarcurrprops->value.valueunion.val_string, "")) {
                    val = 0;
                } else {
                    val = atol(pvarcurrprops->value.valueunion.val_string);
                }
            }
            break;
        /* Vectoren */
        case OV_VT_BYTE_VEC: {
                if(pvarcurrprops->value.valueunion.val_byte_vec.veclen == 0) {
                    val = 0;
                } else {
                    val = (OV_INT)((OV_BYTE*)(pvarcurrprops->value.valueunion.val_byte_vec.value))[0];
                }
            }
            break;
        case OV_VT_BOOL_VEC: {
                if(pvarcurrprops->value.valueunion.val_bool_vec.veclen == 0) {
                    val = 0;
                } else {
                    val = ((OV_BOOL*)(pvarcurrprops->value.valueunion.val_bool_vec.value))[0] ? 1 : 0;
                }
            }
            break;
        case OV_VT_INT_VEC: {
                if(pvarcurrprops->value.valueunion.val_int_vec.veclen == 0) {
                    val = 0;
                } else {
                    val = ((OV_INT*)(pvarcurrprops->value.valueunion.val_int_vec.value))[0];
                }
            }
            break;
        case OV_VT_UINT_VEC: {
                if(pvarcurrprops->value.valueunion.val_uint_vec.veclen == 0) {
                    val = 0;
                } else {
                    uival = ((OV_UINT*)(pvarcurrprops->value.valueunion.val_uint_vec.value))[0];
                    val = (OV_INT)uival;
                }
            }
            break;
        case OV_VT_SINGLE_VEC: {
                if(pvarcurrprops->value.valueunion.val_single_vec.veclen == 0) {
                    val = 0;
                } else {
                    sval = ((OV_SINGLE*)(pvarcurrprops->value.valueunion.val_single_vec.value))[0];
                    val = (OV_INT)sval;
                }
            }
            break;
        case OV_VT_DOUBLE_VEC: {
                if(pvarcurrprops->value.valueunion.val_double_vec.veclen == 0) {
                    val = 0;
                } else {
                    dval = ((OV_DOUBLE*)(pvarcurrprops->value.valueunion.val_double_vec.value))[0];
                    val = (OV_INT)dval;
                }
            }
            break;
        case OV_VT_TIME_VEC:  {
                if(pvarcurrprops->value.valueunion.val_time_vec.veclen == 0) {
                    val = 0;
                } else {
                    uival = ((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[0].secs;
                    val = (OV_INT)uival;
                }
            }
            break;
        case OV_VT_TIME_SPAN_VEC: {
                if(pvarcurrprops->value.valueunion.val_time_span_vec.veclen == 0) {
                    val = 0;
                } else {
                    val = ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[0].secs;
                }
            }
            break;
        case OV_VT_STRING_VEC: {
                OV_STRING str;
                if(pvarcurrprops->value.valueunion.val_string_vec.veclen == 0) {
                    val = 0;
                } else {
                    str = ((OV_STRING*)(pvarcurrprops->value.valueunion.val_string_vec.value))[0];
                    if(!ov_string_compare(str, "")) {
                        val = 0;
                    } else {
                        val = atol(str);
                    }
                }
            }
            break;
        default:
	           return OV_ERR_BADTYPE;
    }

    return fb_set_scalar_int_varvalue_eval(pobj,pelem,pvarcurrprops,changed,&val);
}
/*	======================================================================	*/
OV_RESULT fb_set_scalar_uint_varvalue_eval(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed,
	OV_UINT                      *pvalue
) {
    /* PV? */
    if(pelem->elemunion.pvar->v_vartype == OV_VT_UINT) {
        if(	*((OV_UINT*)pelem->pvalue) != *pvalue ) {
            *changed = TRUE;
        }
    } else {
        /* PV */
		if( (((OV_UINT_PV*)pelem->pvalue)->value != *pvalue) ||
    		(((OV_UINT_PV*)pelem->pvalue)->state != pvarcurrprops->state) ||
    		(((OV_UINT_PV*)pelem->pvalue)->time.secs != pvarcurrprops->time.secs) ||
    		(((OV_UINT_PV*)pelem->pvalue)->time.usecs == pvarcurrprops->time.usecs) ) {
            *changed = TRUE;
        }
    }
        
	/* Set-Funktion vorhanden? */
    if( pelem->elemunion.pvar->v_setfnc ) {
        /* PV ? */
        if(pelem->elemunion.pvar->v_vartype == OV_VT_UINT_PV) {
        	OV_UINT_PV var;
			var.value = *pvalue;
			var.state = pvarcurrprops->state;
			var.time = pvarcurrprops->time;
			return ((OV_FNCPTR_SETPV(UINT))pelem->elemunion.pvar->v_setfnc)(pobj, &var);
        }
        /* Sonnst: */
        return ((OV_FNCPTR_SET(UINT))pelem->elemunion.pvar->v_setfnc)(pobj, *pvalue);
    }

    if(*changed == TRUE) {
        /* PV ? */
        if(pelem->elemunion.pvar->v_vartype == OV_VT_UINT_PV) {
            ((OV_UINT_PV*)pelem->pvalue)->value = *pvalue;
            ((OV_UINT_PV*)pelem->pvalue)->state = pvarcurrprops->state;
        	((OV_UINT_PV*)pelem->pvalue)->time = pvarcurrprops->time;
        } else {
    	   *((OV_UINT*)pelem->pvalue) = *pvalue;
        }
    }

    return OV_ERR_OK;
}
/*	----------------------------------------------------------------------	*/
OV_RESULT fb_set_scalar_uint_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {
	OV_UINT   val;
	OV_INT    ival;
	OV_SINGLE sval;
	OV_DOUBLE dval;
	OV_UINT     ksTyp;
	
	/* Typ gleich? */
	ksTyp = pvarcurrprops->value.vartype & OV_VT_KSMASK;
	if(ksTyp == OV_VT_UINT) {
        val = pvarcurrprops->value.valueunion.val_uint;
        return fb_set_scalar_uint_varvalue_eval(pobj,pelem,pvarcurrprops,changed,&val);
	}
	
    switch(ksTyp) {
        case OV_VT_VOID:
            val = 0;
            break;
        case OV_VT_BYTE:
            val = (OV_UINT)pvarcurrprops->value.valueunion.val_byte;
            break;   
        case OV_VT_BOOL:
            val = pvarcurrprops->value.valueunion.val_bool ? 1 : 0;
            break;
        case OV_VT_INT: {
                ival = pvarcurrprops->value.valueunion.val_int;
                val = (OV_UINT)ival;
            }
            break;
        case OV_VT_UINT: {
                val = pvarcurrprops->value.valueunion.val_uint;
            }
            break;
        case OV_VT_SINGLE: {
                sval = pvarcurrprops->value.valueunion.val_single;
                val = (OV_UINT)sval;
            }
            break;
        case OV_VT_DOUBLE: {
                dval = pvarcurrprops->value.valueunion.val_double;
                val = (OV_UINT)dval;
            }
            break;
        case OV_VT_TIME: {
                val = pvarcurrprops->value.valueunion.val_time.secs;
            }
            break;
        case OV_VT_TIME_SPAN: {
                val = (OV_UINT)pvarcurrprops->value.valueunion.val_time_span.secs;
            }
            break;
        case OV_VT_STRING: {
                if(!ov_string_compare(pvarcurrprops->value.valueunion.val_string, "")) {
                    val = 0;
                } else {
                    val = (OV_UINT)atol(pvarcurrprops->value.valueunion.val_string);
                }
            }
            break;
        /* Vectoren */
        case OV_VT_BYTE_VEC: {
                if(pvarcurrprops->value.valueunion.val_byte_vec.veclen == 0) {
                    val = 0;
                } else {
                    val = (OV_UINT)((OV_BYTE*)(pvarcurrprops->value.valueunion.val_byte_vec.value))[0];
                }
            }
            break;
        case OV_VT_BOOL_VEC: {
                if(pvarcurrprops->value.valueunion.val_bool_vec.veclen == 0) {
                    val = 0;
                } else {
                    val = ((OV_BOOL*)(pvarcurrprops->value.valueunion.val_bool_vec.value))[0] ? 1 : 0;
                }
            }
            break;
        case OV_VT_INT_VEC: {
                if(pvarcurrprops->value.valueunion.val_int_vec.veclen == 0) {
                    val = 0;
                } else {
                    ival = ((OV_INT*)(pvarcurrprops->value.valueunion.val_int_vec.value))[0];
                    val = (OV_UINT)ival;
                }
            }
            break;
        case OV_VT_UINT_VEC: {
                if(pvarcurrprops->value.valueunion.val_uint_vec.veclen == 0) {
                    val = 0;
                } else {
                    val = ((OV_UINT*)(pvarcurrprops->value.valueunion.val_uint_vec.value))[0];
                }
            }
            break;
        case OV_VT_SINGLE_VEC: {
                if(pvarcurrprops->value.valueunion.val_single_vec.veclen == 0) {
                    val = 0;
                } else {
                    sval = ((OV_SINGLE*)(pvarcurrprops->value.valueunion.val_single_vec.value))[0];
                    val = (OV_UINT)sval;
                }
            }
            break;
        case OV_VT_DOUBLE_VEC: {
                if(pvarcurrprops->value.valueunion.val_double_vec.veclen == 0) {
                    val = 0;
                } else {
                    dval = ((OV_DOUBLE*)(pvarcurrprops->value.valueunion.val_double_vec.value))[0];
                    val = (OV_UINT)dval;
                }
            }
            break;
        case OV_VT_TIME_VEC:  {
                if(pvarcurrprops->value.valueunion.val_time_vec.veclen == 0) {
                    val = 0;
                } else {
                    val = ((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[0].secs;
                }
            }
            break;
        case OV_VT_TIME_SPAN_VEC: {
                if(pvarcurrprops->value.valueunion.val_time_span_vec.veclen == 0) {
                    val = 0;
                } else {
                    ival = ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[0].secs;
                    val = (OV_UINT)ival;
                }
            }
            break;
        case OV_VT_STRING_VEC: {
                OV_STRING str;
                if(pvarcurrprops->value.valueunion.val_string_vec.veclen == 0) {
                    val = 0;
                } else {
                    str = ((OV_STRING*)(pvarcurrprops->value.valueunion.val_string_vec.value))[0];
                    if(!ov_string_compare(str, "")) {
                        val = 0;
                    } else {
                        ival = atol(str);
                        val = (OV_UINT)ival;
                    }
                }
            }
            break;
        default:
	           return OV_ERR_BADTYPE;
    }

    return fb_set_scalar_uint_varvalue_eval(pobj,pelem,pvarcurrprops,changed,&val);
}
/*	======================================================================	*/
OV_RESULT fb_set_scalar_single_varvalue_eval(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed,
	OV_SINGLE                   *pvalue
) {
    /* PV? */
    if(pelem->elemunion.pvar->v_vartype == OV_VT_SINGLE) {
#if SETVAR_DEBUG
fprintf(stderr, "  Variable: SINGLE\n");
#endif
        if(	*((OV_SINGLE*)pelem->pvalue) != *pvalue ) {
            *changed = TRUE;
#if SETVAR_DEBUG
fprintf(stderr, "  %f != %f\n", *((OV_SINGLE*)pelem->pvalue), *pvalue);
#endif
        }
#if SETVAR_DEBUG
        else {
fprintf(stderr, "  Wert gleich\n");
        }
#endif
    } else {
    
#if SETVAR_DEBUG
fprintf(stderr, "  Variable: SINGLE_PV? %d\n", pelem->elemunion.pvar->v_vartype);
#endif
        /* PV */
		if( (((OV_SINGLE_PV*)pelem->pvalue)->value != *pvalue) ||
    		(((OV_SINGLE_PV*)pelem->pvalue)->state != pvarcurrprops->state) ||
    		(((OV_SINGLE_PV*)pelem->pvalue)->time.secs != pvarcurrprops->time.secs) ||
    		(((OV_SINGLE_PV*)pelem->pvalue)->time.usecs == pvarcurrprops->time.usecs) ) {
            *changed = TRUE;
        }
    }
        
	/* Set-Funktion vorhanden? */
    if( pelem->elemunion.pvar->v_setfnc ) {
        /* PV ? */
        if(pelem->elemunion.pvar->v_vartype == OV_VT_SINGLE_PV) {
        	OV_SINGLE_PV var;
			var.value = *pvalue;
			var.state = pvarcurrprops->state;
			var.time = pvarcurrprops->time;
			return ((OV_FNCPTR_SETPV(SINGLE))pelem->elemunion.pvar->v_setfnc)(pobj, &var);
        }
        /* Sonnst: */
        return ((OV_FNCPTR_SET(SINGLE))pelem->elemunion.pvar->v_setfnc)(pobj, *pvalue);
    }

    if(*changed == TRUE) {
        /* PV ? */
        if(pelem->elemunion.pvar->v_vartype == OV_VT_SINGLE_PV) {
            ((OV_SINGLE_PV*)pelem->pvalue)->value = *pvalue;
            ((OV_SINGLE_PV*)pelem->pvalue)->state = pvarcurrprops->state;
        	((OV_SINGLE_PV*)pelem->pvalue)->time = pvarcurrprops->time;
        } else {
    	   *((OV_SINGLE*)pelem->pvalue) = *pvalue;
        }
    }

    return OV_ERR_OK;
}
/*	----------------------------------------------------------------------	*/
OV_RESULT fb_set_scalar_single_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {
	OV_SINGLE   val;
	OV_DOUBLE   dval;
	OV_UINT     ksTyp;
	
#if SETVAR_DEBUG
fprintf(stderr, "OV_SINGLE: Setze %s\n", pelem->elemunion.pvar->v_identifier);
#endif

	/* Typ gleich? */
	ksTyp = pvarcurrprops->value.vartype & OV_VT_KSMASK;
	if(ksTyp == OV_VT_SINGLE) {
#if SETVAR_DEBUG
fprintf(stderr, "Props auch OV_SINGLE\n");
#endif
        val = pvarcurrprops->value.valueunion.val_single;
        return fb_set_scalar_single_varvalue_eval(pobj,pelem,pvarcurrprops,changed,&val);
	}

#if SETVAR_DEBUG
fprintf(stderr, "Props :  %d\n", ksTyp);
#endif
    switch(ksTyp) {
        case OV_VT_VOID:
            val = 0;
            break;
        case OV_VT_BYTE:
            val = (OV_SINGLE)pvarcurrprops->value.valueunion.val_byte;
            break;   
        case OV_VT_BOOL: {
                if(pvarcurrprops->value.valueunion.val_bool) {
                    val = 1.0;
                } else {
                    val = 0.0;
                }
            }
            break;
        case OV_VT_INT:
                val = (OV_SINGLE)pvarcurrprops->value.valueunion.val_int;
            break;
        case OV_VT_UINT:
                val = (OV_SINGLE)pvarcurrprops->value.valueunion.val_uint;
            break;
        case OV_VT_SINGLE:
                val = pvarcurrprops->value.valueunion.val_single;
            break;
        case OV_VT_DOUBLE: {
                dval = pvarcurrprops->value.valueunion.val_double;
                val = (OV_SINGLE)dval;
            }
            break;
        case OV_VT_TIME: {
                val = (OV_SINGLE)(1.0 * pvarcurrprops->value.valueunion.val_time.secs +
                                  1.0 * pvarcurrprops->value.valueunion.val_time.usecs / 1000000);
            }
            break;
        case OV_VT_TIME_SPAN: {
                val = (OV_SINGLE)(1.0*pvarcurrprops->value.valueunion.val_time_span.secs +
                                  1.0 * pvarcurrprops->value.valueunion.val_time_span.usecs / 1000000);
            }
            break;
        case OV_VT_STRING: {
                if(!ov_string_compare(pvarcurrprops->value.valueunion.val_string, "")) {
                    val = 0;
                } else {
                    fb_fs2double(pvarcurrprops->value.valueunion.val_string, &dval);
                    val = (OV_SINGLE)dval;
                }
            }
            break;
        /* Vectoren */
        case OV_VT_BYTE_VEC: {
                if(pvarcurrprops->value.valueunion.val_byte_vec.veclen == 0) {
                    val = 0;
                } else {
                    val = (OV_SINGLE)((OV_BYTE*)(pvarcurrprops->value.valueunion.val_byte_vec.value))[0];
                }
            }
            break;
        case OV_VT_BOOL_VEC: {
                if(pvarcurrprops->value.valueunion.val_bool_vec.veclen == 0) {
                    val = 0;
                } else {
                    val = (OV_SINGLE)(((OV_BOOL*)(pvarcurrprops->value.valueunion.val_bool_vec.value))[0] ? 1 : 0);
                }
            }
            break;
        case OV_VT_INT_VEC: {
                if(pvarcurrprops->value.valueunion.val_int_vec.veclen == 0) {
                    val = 0;
                } else {
                    val = (OV_SINGLE)(((OV_INT*)(pvarcurrprops->value.valueunion.val_int_vec.value))[0]);
                }
            }
            break;
        case OV_VT_UINT_VEC: {
                if(pvarcurrprops->value.valueunion.val_uint_vec.veclen == 0) {
                    val = 0;
                } else {
                    val = (OV_SINGLE)(((OV_UINT*)(pvarcurrprops->value.valueunion.val_uint_vec.value))[0]);
                }
            }
            break;
        case OV_VT_SINGLE_VEC: {
                if(pvarcurrprops->value.valueunion.val_single_vec.veclen == 0) {
                    val = 0;
                } else {
                    val = ((OV_SINGLE*)(pvarcurrprops->value.valueunion.val_single_vec.value))[0];
                }
            }
            break;
        case OV_VT_DOUBLE_VEC: {
                if(pvarcurrprops->value.valueunion.val_double_vec.veclen == 0) {
                    val = 0;
                } else {
                    dval = ((OV_DOUBLE*)(pvarcurrprops->value.valueunion.val_double_vec.value))[0];
                    val = (OV_SINGLE)dval;
                }
            }
            break;
        case OV_VT_TIME_VEC:  {
                if(pvarcurrprops->value.valueunion.val_time_vec.veclen == 0) {
                    val = 0;
                } else {
                    OV_UINT uival;
                    uival =  ((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[0].secs;
                    val = (OV_SINGLE)uival;
                    uival =  ((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[0].usecs;
                    val += (OV_SINGLE)(1.0 * uival / 1000000);
                }
            }
            break;
        case OV_VT_TIME_SPAN_VEC: {
                if(pvarcurrprops->value.valueunion.val_time_span_vec.veclen == 0) {
                    val = 0;
                } else {
                    OV_INT ival;
                    ival =  ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[0].secs;
                    val = (OV_SINGLE)ival;
                    ival =  ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[0].usecs;
                    val += (OV_SINGLE)(1.0 * ival / 1000000);
                }
            }
            break;
        case OV_VT_STRING_VEC: {
                if(pvarcurrprops->value.valueunion.val_string_vec.veclen == 0) {
                    val = 0;
                } else {
                    OV_STRING str;
                    str = ((OV_STRING*)(pvarcurrprops->value.valueunion.val_string_vec.value))[0];
                    if(!ov_string_compare(str,"")) {
                        val = 0;
                    } else {
                        fb_fs2double(str, &dval);
                        val = (OV_SINGLE)dval;
                    }
                }
            }
            break;
        default:
	           return OV_ERR_BADTYPE;
    }

    return fb_set_scalar_single_varvalue_eval(pobj,pelem,pvarcurrprops,changed,&val);
}

/*	======================================================================	*/
OV_RESULT fb_set_scalar_double_varvalue_eval(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed,
	OV_DOUBLE                   *pvalue
) {
    /* PV? */
    if(pelem->elemunion.pvar->v_vartype == OV_VT_DOUBLE) {
        if(	*((OV_DOUBLE*)pelem->pvalue) != *pvalue ) {
            *changed = TRUE;
        }
    } else {
        /* PV */
		if( (((OV_DOUBLE_PV*)pelem->pvalue)->value != *pvalue) ||
    		(((OV_DOUBLE_PV*)pelem->pvalue)->state != pvarcurrprops->state) ||
    		(((OV_DOUBLE_PV*)pelem->pvalue)->time.secs != pvarcurrprops->time.secs) ||
    		(((OV_DOUBLE_PV*)pelem->pvalue)->time.usecs == pvarcurrprops->time.usecs) ) {
            *changed = TRUE;
        }
    }
        
	/* Set-Funktion vorhanden? */
    if( pelem->elemunion.pvar->v_setfnc ) {
        /* PV ? */
        if(pelem->elemunion.pvar->v_vartype == OV_VT_DOUBLE_PV) {
        	OV_DOUBLE_PV var;
			var.value = *pvalue;
			var.state = pvarcurrprops->state;
			var.time = pvarcurrprops->time;
			return ((OV_FNCPTR_SETPV(DOUBLE))pelem->elemunion.pvar->v_setfnc)(pobj, &var);
        }
        /* Sonnst: */
        return ((OV_FNCPTR_SET(DOUBLE))pelem->elemunion.pvar->v_setfnc)(pobj, *pvalue);
    }

    if(*changed == TRUE) {
        /* PV ? */
        if(pelem->elemunion.pvar->v_vartype == OV_VT_DOUBLE_PV) {
            ((OV_DOUBLE_PV*)pelem->pvalue)->value = *pvalue;
            ((OV_DOUBLE_PV*)pelem->pvalue)->state = pvarcurrprops->state;
        	((OV_DOUBLE_PV*)pelem->pvalue)->time = pvarcurrprops->time;
        } else {
    	   *((OV_DOUBLE*)pelem->pvalue) = *pvalue;
        }
    }

    return OV_ERR_OK;
}
/*	----------------------------------------------------------------------	*/
OV_RESULT fb_set_scalar_double_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {
	OV_DOUBLE val;
	OV_UINT     ksTyp;
	
	/* Typ gleich? */
	ksTyp = pvarcurrprops->value.vartype & OV_VT_KSMASK;
	if(ksTyp == OV_VT_DOUBLE) {
        val = pvarcurrprops->value.valueunion.val_double;
        return fb_set_scalar_double_varvalue_eval(pobj,pelem,pvarcurrprops,changed,&val);
	}
	
    switch(ksTyp) {
        case OV_VT_VOID:
            val = 0;
            break;
        case OV_VT_BYTE:
            val = (OV_DOUBLE)pvarcurrprops->value.valueunion.val_byte;
            break;   
        case OV_VT_BOOL:
            val = (OV_DOUBLE)(pvarcurrprops->value.valueunion.val_bool ? 1 : 0);
            break;
        case OV_VT_INT:
            val = (OV_DOUBLE)(pvarcurrprops->value.valueunion.val_int);
            break;
        case OV_VT_UINT:
            val = (OV_DOUBLE)pvarcurrprops->value.valueunion.val_uint;
            break;
        case OV_VT_SINGLE:
            val = (OV_DOUBLE)pvarcurrprops->value.valueunion.val_single;
            break;
        case OV_VT_DOUBLE:
            val = pvarcurrprops->value.valueunion.val_double;
            break;
        case OV_VT_TIME: {
                OV_UINT uival;
                uival = pvarcurrprops->value.valueunion.val_time.secs;
                val = (OV_DOUBLE)uival;
                uival = pvarcurrprops->value.valueunion.val_time.usecs;
                val += (OV_DOUBLE)(1.0 * uival / 1000000);
            }
            break;
        case OV_VT_TIME_SPAN: {
                OV_INT ival;
                ival = pvarcurrprops->value.valueunion.val_time_span.secs;
                val = (OV_DOUBLE)ival;
                ival = pvarcurrprops->value.valueunion.val_time_span.usecs;
                val += (OV_DOUBLE)(1.0 * ival / 1000000);
            }
            break;
        case OV_VT_STRING: {
                if(!ov_string_compare(pvarcurrprops->value.valueunion.val_string,"")) {
                    val = 0;
                } else {
                    fb_fs2double(pvarcurrprops->value.valueunion.val_string, &val);
                }
            }
            break;
        /* Vectoren */
        case OV_VT_BYTE_VEC: {
                if(pvarcurrprops->value.valueunion.val_byte_vec.veclen == 0) {
                    val = 0;
                } else {
                    val = (OV_DOUBLE)((OV_BYTE*)(pvarcurrprops->value.valueunion.val_byte_vec.value))[0];
                }
            }
            break;
        case OV_VT_BOOL_VEC: {
                if(pvarcurrprops->value.valueunion.val_bool_vec.veclen == 0) {
                    val = 0;
                } else {
                    val = ((OV_BOOL*)(pvarcurrprops->value.valueunion.val_bool_vec.value))[0] ? 1 : 0;
                }
            }
            break;
        case OV_VT_INT_VEC: {
                if(pvarcurrprops->value.valueunion.val_int_vec.veclen == 0) {
                    val = 0;
                } else {
                    OV_INT ival;
                    ival = ((OV_INT*)(pvarcurrprops->value.valueunion.val_int_vec.value))[0];
                    val = (OV_DOUBLE)ival;
                }
            }
            break;
        case OV_VT_UINT_VEC: {
                if(pvarcurrprops->value.valueunion.val_uint_vec.veclen == 0) {
                    val = 0;
                } else {
                    OV_UINT uival;
                    uival = ((OV_UINT*)(pvarcurrprops->value.valueunion.val_uint_vec.value))[0];
                    val = (OV_DOUBLE)uival;
                }
            }
            break;
        case OV_VT_SINGLE_VEC: {
                if(pvarcurrprops->value.valueunion.val_single_vec.veclen == 0) {
                    val = 0;
                } else {
                    val = (OV_DOUBLE)((OV_SINGLE*)(pvarcurrprops->value.valueunion.val_single_vec.value))[0];
                }
            }
            break;
        case OV_VT_DOUBLE_VEC: {
                if(pvarcurrprops->value.valueunion.val_double_vec.veclen == 0) {
                    val = 0;
                } else {
                    val = ((OV_DOUBLE*)(pvarcurrprops->value.valueunion.val_double_vec.value))[0];
                }
            }
            break;
        case OV_VT_TIME_VEC:  {
                if(pvarcurrprops->value.valueunion.val_time_vec.veclen == 0) {
                    val = 0;
                } else {
                    OV_UINT uival;
                    uival = ((OV_TIME*)pvarcurrprops->value.valueunion.val_time_vec.value)[0].secs;
                    val = (OV_DOUBLE)uival;
                    uival = ((OV_TIME*)pvarcurrprops->value.valueunion.val_time_vec.value)[0].usecs;
                    val += (OV_DOUBLE)(1.0 * uival / 1000000);
                }
            }
            break;
        case OV_VT_TIME_SPAN_VEC: {
                if(pvarcurrprops->value.valueunion.val_time_span_vec.veclen == 0) {
                    val = 0;
                } else {
                    OV_INT ival;
                    ival = ((OV_TIME_SPAN*)pvarcurrprops->value.valueunion.val_time_span_vec.value)[0].secs;
                    val = (OV_DOUBLE)ival;
                    ival = ((OV_TIME_SPAN*)pvarcurrprops->value.valueunion.val_time_span_vec.value)[0].usecs;
                    val += (OV_DOUBLE)(1.0 * ival / 1000000);
                }
            }
            break;
        case OV_VT_STRING_VEC: {
                OV_STRING str;
                if(pvarcurrprops->value.valueunion.val_string_vec.veclen == 0) {
                    val = 0;
                } else {
                    str = ((OV_STRING*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[0];
                    if(!ov_string_compare(str, "")) {
                        val = 0;
                    } else {
                        fb_fs2double(str, &val);
                    }
                }
            }
            break;
        default:
	           return OV_ERR_BADTYPE;
    }

    return fb_set_scalar_double_varvalue_eval(pobj,pelem,pvarcurrprops,changed,&val);
}
/*	======================================================================	*/
OV_RESULT fb_set_scalar_time_varvalue_eval(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed,
	OV_TIME                     *pvalue
) {
    /* PV? */
    if(pelem->elemunion.pvar->v_vartype == OV_VT_TIME) {
        if(	((((OV_TIME*)pelem->pvalue)->secs) != pvalue->secs) ||
            ((((OV_TIME*)pelem->pvalue)->usecs) != pvalue->usecs) ) {
            *changed = TRUE;
        }
    } else {
        /* PV */
		if( (((OV_TIME_PV*)pelem->pvalue)->value.secs != pvalue->secs) ||
    		(((OV_TIME_PV*)pelem->pvalue)->value.usecs != pvalue->usecs) ||
    		(((OV_TIME_PV*)pelem->pvalue)->state != pvarcurrprops->state) ||
    		(((OV_TIME_PV*)pelem->pvalue)->time.secs != pvarcurrprops->time.secs) ||
    		(((OV_TIME_PV*)pelem->pvalue)->time.usecs != pvarcurrprops->time.usecs) ) {
            *changed = TRUE;
        }
    }

        
	/* Set-Funktion vorhanden? */
    if( pelem->elemunion.pvar->v_setfnc ) {
        /* PV ? */
        if(pelem->elemunion.pvar->v_vartype == OV_VT_TIME_PV) {
        	OV_TIME_PV var;
			var.value = *pvalue;
			var.state = pvarcurrprops->state;
			var.time = pvarcurrprops->time;
			return ((OV_FNCPTR_SETPV(TIME))pelem->elemunion.pvar->v_setfnc)(pobj, &var);
        }
        /* Sonnst */
        return ((OV_FNCPTR_SETTIME(TIME))pelem->elemunion.pvar->v_setfnc)(pobj, pvalue);
    }
    
    if( *changed == TRUE ) {
        /* PV ? */
        if(pelem->elemunion.pvar->v_vartype == OV_VT_TIME_PV) {
            ((OV_TIME_PV*)pelem->pvalue)->value = *pvalue;
            ((OV_TIME_PV*)pelem->pvalue)->state = pvarcurrprops->state;
        	((OV_TIME_PV*)pelem->pvalue)->time = pvarcurrprops->time;
        } else {
    	   *((OV_TIME*)pelem->pvalue) = *pvalue;
        }
    }

    return OV_ERR_OK;
}
/*	----------------------------------------------------------------------	*/
OV_RESULT fb_set_scalar_time_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {
    OV_TIME   val;
	OV_INT    ival;
	OV_SINGLE sval;
	OV_DOUBLE dval;
	OV_UINT     ksTyp;
	
	/* Typ gleich? */
	ksTyp = pvarcurrprops->value.vartype & OV_VT_KSMASK;
	if(ksTyp == OV_VT_TIME) {
        val.secs  = pvarcurrprops->value.valueunion.val_time.secs;
        val.usecs = pvarcurrprops->value.valueunion.val_time.usecs;
        return fb_set_scalar_time_varvalue_eval(pobj,pelem,pvarcurrprops,changed,&val);
	}
	
	val.usecs = 0;
    switch(ksTyp) {
        case OV_VT_VOID:
            val.secs = 0;
            break;
        case OV_VT_BYTE:
            val.secs = (OV_UINT)pvarcurrprops->value.valueunion.val_byte;
            break;   
        case OV_VT_BOOL:
            val.secs = pvarcurrprops->value.valueunion.val_bool ? 1 : 0;
            break;
        case OV_VT_INT: {
                ival = pvarcurrprops->value.valueunion.val_int;
                val.secs = (OV_UINT)ival;
            }
            break;
        case OV_VT_UINT: {
                val.secs = pvarcurrprops->value.valueunion.val_uint;
            }
            break;
        case OV_VT_SINGLE: {
                sval = pvarcurrprops->value.valueunion.val_single;
                val.secs = (OV_UINT)sval;
                val.usecs = (OV_UINT)( (sval - (OV_SINGLE)val.secs) * 1000000);
            }
            break;
        case OV_VT_DOUBLE: {
                dval = pvarcurrprops->value.valueunion.val_double;
                val.secs = (OV_UINT)dval;
                val.usecs = (OV_UINT)( (dval - (OV_DOUBLE)val.secs) * 1000000);
            }
            break;
        case OV_VT_TIME: {
                val.secs  = pvarcurrprops->value.valueunion.val_time.secs;
                val.usecs = pvarcurrprops->value.valueunion.val_time.usecs;
            }
            break;
        case OV_VT_TIME_SPAN: {
                ival = pvarcurrprops->value.valueunion.val_time_span.secs;
                val.secs  = (OV_UINT)ival;
                ival = pvarcurrprops->value.valueunion.val_time_span.usecs;
                val.usecs = (OV_UINT)ival;
            }
            break;
        case OV_VT_STRING: {
                if(fb_ascii2time(pvarcurrprops->value.valueunion.val_string, &val) != OV_ERR_OK) {
                    val.secs = 0;
                }
            }
            break;
        /* Vectoren */
        case OV_VT_BYTE_VEC: {
                if(pvarcurrprops->value.valueunion.val_byte_vec.veclen == 0) {
                    val.secs = 0;
                } else {
                    val.secs = (OV_UINT)((OV_BYTE*)(pvarcurrprops->value.valueunion.val_byte_vec.value))[0];
                }
            }
            break;
        case OV_VT_BOOL_VEC: {
                if(pvarcurrprops->value.valueunion.val_bool_vec.veclen == 0) {
                    val.secs = 0;
                } else {
                    val.secs = ((OV_BOOL*)(pvarcurrprops->value.valueunion.val_bool_vec.value))[0] ? 1 : 0;
                }
            }
            break;
        case OV_VT_INT_VEC: {
                if(pvarcurrprops->value.valueunion.val_int_vec.veclen == 0) {
                    val.secs = 0;
                } else {
                    ival = ((OV_INT*)(pvarcurrprops->value.valueunion.val_int_vec.value))[0];
                    val.secs = (OV_UINT)ival;
                }
            }
            break;
        case OV_VT_UINT_VEC: {
                if(pvarcurrprops->value.valueunion.val_uint_vec.veclen == 0) {
                    val.secs = 0;
                } else {
                    val.secs = ((OV_UINT*)(pvarcurrprops->value.valueunion.val_uint_vec.value))[0];
                }
            }
            break;
        case OV_VT_SINGLE_VEC: {
                if(pvarcurrprops->value.valueunion.val_single_vec.veclen == 0) {
                    val.secs = 0;
                } else {
                    sval = ((OV_SINGLE*)(pvarcurrprops->value.valueunion.val_single_vec.value))[0];
                    val.secs = (OV_UINT)sval;
                    val.usecs = (OV_UINT)( (sval - (OV_SINGLE)val.secs) * 1000000);
                }
            }
            break;
        case OV_VT_DOUBLE_VEC: {
                if(pvarcurrprops->value.valueunion.val_double_vec.veclen == 0) {
                    val.secs = 0;
                } else {
                    dval = ((OV_DOUBLE*)(pvarcurrprops->value.valueunion.val_double_vec.value))[0];
                    val.secs = (OV_UINT)dval;
                    val.usecs = (OV_UINT)( (dval - (OV_DOUBLE)val.secs) * 1000000);
                }
            }
            break;
        case OV_VT_TIME_VEC:  {
                if(pvarcurrprops->value.valueunion.val_time_vec.veclen == 0) {
                    val.secs = 0;
                } else {
                    val = ((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[0];
                }
            }
            break;
        case OV_VT_TIME_SPAN_VEC: {
                if(pvarcurrprops->value.valueunion.val_time_span_vec.veclen == 0) {
                    val.secs = 0;
                } else {
                    ival = ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[0].secs;
                    val.secs = (OV_UINT)ival;
                    ival = ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[0].usecs;
                    val.usecs = (OV_UINT)ival;
                }
            }
            break;
        case OV_VT_STRING_VEC: {
                OV_STRING str;
                if(pvarcurrprops->value.valueunion.val_string_vec.veclen == 0) {
                    val.secs = 0;
                } else {
                    str = ((OV_STRING*)(pvarcurrprops->value.valueunion.val_string_vec.value))[0];
                    if(!ov_string_compare(str,"")) {
                        val.secs = 0;
                    } else {
                        if(fb_ascii2time(str, &val) != OV_ERR_OK) {
                            val.secs = 0;
                        }
                    }
                }
            }
            break;
        default:
	           return OV_ERR_BADTYPE;
    }

    return fb_set_scalar_time_varvalue_eval(pobj,pelem,pvarcurrprops,changed,&val);
}
/*	======================================================================	*/
OV_RESULT fb_set_scalar_time_span_varvalue_eval(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed,
	OV_TIME_SPAN                *pvalue
) {
    /* PV? */
    if(pelem->elemunion.pvar->v_vartype == OV_VT_TIME_SPAN) {
        if(	((((OV_TIME_SPAN*)pelem->pvalue)->secs) != pvalue->secs) ||
            ((((OV_TIME_SPAN*)pelem->pvalue)->usecs) != pvalue->usecs) ) {
            *changed = TRUE;
        }
    } else {
        /* PV */
		if( (((OV_TIME_SPAN_PV*)pelem->pvalue)->value.secs != pvalue->secs) ||
    		(((OV_TIME_SPAN_PV*)pelem->pvalue)->value.usecs != pvalue->usecs) ||
    		(((OV_TIME_SPAN_PV*)pelem->pvalue)->state != pvarcurrprops->state) ||
    		(((OV_TIME_SPAN_PV*)pelem->pvalue)->time.secs != pvarcurrprops->time.secs) ||
    		(((OV_TIME_SPAN_PV*)pelem->pvalue)->time.usecs != pvarcurrprops->time.usecs) ) {
            *changed = TRUE;
        }
    }

        
	/* Set-Funktion vorhanden? */
    if( pelem->elemunion.pvar->v_setfnc ) {
        /* PV ? */
        if(pelem->elemunion.pvar->v_vartype == OV_VT_TIME_SPAN_PV) {
        	OV_TIME_SPAN_PV var;
			var.value = *pvalue;
			var.state = pvarcurrprops->state;
			var.time = pvarcurrprops->time;
			return ((OV_FNCPTR_SETPV(TIME_SPAN))pelem->elemunion.pvar->v_setfnc)(pobj, &var);
        }
        /* Sonnst */
        return ((OV_FNCPTR_SETTIME(TIME_SPAN))pelem->elemunion.pvar->v_setfnc)(pobj, pvalue);
    }
    
    if( *changed == TRUE ) {
        /* PV ? */
        if(pelem->elemunion.pvar->v_vartype == OV_VT_TIME_SPAN_PV) {
            ((OV_TIME_SPAN_PV*)pelem->pvalue)->value = *pvalue;
            ((OV_TIME_SPAN_PV*)pelem->pvalue)->state = pvarcurrprops->state;
        	((OV_TIME_SPAN_PV*)pelem->pvalue)->time = pvarcurrprops->time;
        } else {
    	   *((OV_TIME_SPAN*)pelem->pvalue) = *pvalue;
        }
    }

    return OV_ERR_OK;
}
/*	----------------------------------------------------------------------	*/
OV_RESULT fb_set_scalar_time_span_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {
    OV_TIME_SPAN   val;
	OV_UINT        uival;
	OV_SINGLE      sval;
	OV_DOUBLE      dval;
	OV_UINT     ksTyp;
	
	/* Typ gleich? */
	ksTyp = pvarcurrprops->value.vartype & OV_VT_KSMASK;
	if(ksTyp == OV_VT_TIME_SPAN) {
        val.secs  = pvarcurrprops->value.valueunion.val_time.secs;
        val.usecs = pvarcurrprops->value.valueunion.val_time.usecs;
        return fb_set_scalar_time_span_varvalue_eval(pobj,pelem,pvarcurrprops,changed,&val);
	}

	val.usecs = 0;
    switch(ksTyp) {
        case OV_VT_VOID:
            val.secs = 0;
            break;
        case OV_VT_BYTE:
            val.secs = (OV_INT)pvarcurrprops->value.valueunion.val_byte;
            break;   
        case OV_VT_BOOL:
            val.secs = pvarcurrprops->value.valueunion.val_bool ? 1 : 0;
            break;
        case OV_VT_INT:
                val.secs = pvarcurrprops->value.valueunion.val_int;
            break;
        case OV_VT_UINT:
                val.secs = (OV_INT)pvarcurrprops->value.valueunion.val_uint;
            break;
        case OV_VT_SINGLE: {
                sval = pvarcurrprops->value.valueunion.val_single;
                val.secs = (OV_INT)sval;
                val.usecs = (OV_INT)( (sval - (OV_SINGLE)val.secs) * 1000000);
            }
            break;
        case OV_VT_DOUBLE: {
                dval = pvarcurrprops->value.valueunion.val_double;
                val.secs = (OV_INT)dval;
                val.usecs = (OV_INT)( (dval - (OV_DOUBLE)val.secs) * 1000000);
            }
            break;
        case OV_VT_TIME: {
                val.secs = (OV_INT)pvarcurrprops->value.valueunion.val_time.secs;
                val.usecs = (OV_INT)pvarcurrprops->value.valueunion.val_time.usecs;
            }
            break;
        case OV_VT_TIME_SPAN: {
                val.secs = pvarcurrprops->value.valueunion.val_time_span.secs;
                val.secs = pvarcurrprops->value.valueunion.val_time_span.usecs;
            }
            break;
        case OV_VT_STRING: {
                if(fb_ascii2timespan(pvarcurrprops->value.valueunion.val_string, &val) != OV_ERR_OK) {
                    val.secs = 0;
                }
            }
            break;
        /* Vectoren */
        case OV_VT_BYTE_VEC: {
                if(pvarcurrprops->value.valueunion.val_byte_vec.veclen == 0) {
                    val.secs = 0;
                } else {
                    val.secs = (OV_UINT)((OV_BYTE*)(pvarcurrprops->value.valueunion.val_byte_vec.value))[0];
                }
            }
            break;
        case OV_VT_BOOL_VEC: {
                if(pvarcurrprops->value.valueunion.val_bool_vec.veclen == 0) {
                    val.secs = 0;
                } else {
                    val.secs = ((OV_BOOL*)(pvarcurrprops->value.valueunion.val_bool_vec.value))[0] ? 1 : 0;
                }
            }
            break;
        case OV_VT_INT_VEC: {
                if(pvarcurrprops->value.valueunion.val_int_vec.veclen == 0) {
                    val.secs = 0;
                } else {
                    val.secs = ((OV_INT*)(pvarcurrprops->value.valueunion.val_int_vec.value))[0];
                }
            }
            break;
        case OV_VT_UINT_VEC: {
                if(pvarcurrprops->value.valueunion.val_uint_vec.veclen == 0) {
                    val.secs = 0;
                } else {
                    uival = ((OV_UINT*)(pvarcurrprops->value.valueunion.val_uint_vec.value))[0];
                    val.secs = (OV_INT)uival;
                }
            }
            break;
        case OV_VT_SINGLE_VEC: {
                if(pvarcurrprops->value.valueunion.val_single_vec.veclen == 0) {
                    val.secs = 0;
                } else {
                    sval = ((OV_SINGLE*)(pvarcurrprops->value.valueunion.val_single_vec.value))[0];
                    val.secs = (OV_INT)sval;
                    val.usecs = (OV_INT)( (sval - (OV_SINGLE)val.secs) * 1000000);
                }
            }
            break;
        case OV_VT_DOUBLE_VEC: {
                if(pvarcurrprops->value.valueunion.val_double_vec.veclen == 0) {
                    val.secs = 0;
                } else {
                    dval = ((OV_DOUBLE*)(pvarcurrprops->value.valueunion.val_double_vec.value))[0];
                    val.secs = (OV_INT)dval;
                    val.usecs = (OV_INT)( (dval - (OV_DOUBLE)val.secs) * 1000000);
                }
            }
            break;
        case OV_VT_TIME_VEC:  {
                if(pvarcurrprops->value.valueunion.val_time_vec.veclen == 0) {
                    val.secs = 0;
                } else {
                    uival = ((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[0].secs;
                    val.secs = (OV_INT)uival;
                    uival = ((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[0].usecs;
                    val.usecs = (OV_INT)uival;
                }
            }
            break;
        case OV_VT_TIME_SPAN_VEC: {
                if(pvarcurrprops->value.valueunion.val_time_span_vec.veclen == 0) {
                    val.secs = 0;
                } else {
                    val.secs = ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[0].secs;
                    val.usecs = ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[0].usecs;
                }
            }
            break;
        case OV_VT_STRING_VEC: {
                OV_STRING str;
                if(pvarcurrprops->value.valueunion.val_string_vec.veclen == 0) {
                    val.secs = 0;
                } else {
                    str = ((OV_STRING*)(pvarcurrprops->value.valueunion.val_string_vec.value))[0];
                    if(!ov_string_compare(str,"")) {
                        val.secs = 0;
                    } else {
                        if(fb_ascii2timespan(str, &val) != OV_ERR_OK) {
                            val.secs = 0;
                        }
                    }
                }
            }
            break;
        default:
	           return OV_ERR_BADTYPE;
    }

    return fb_set_scalar_time_span_varvalue_eval(pobj,pelem,pvarcurrprops,changed,&val);
}
/*	======================================================================	*/
OV_RESULT fb_set_scalar_string_varvalue_eval(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed,
	OV_STRING                   pvalue
) {
    /* Wert geaendert? */
    if(pelem->elemunion.pvar->v_vartype == OV_VT_STRING) {
		if( ov_string_compare( *(OV_STRING*)pelem->pvalue, pvalue) ) {
            *changed = TRUE;
        }
    } else {
        /* PV */
		if( ov_string_compare( ((OV_STRING_PV*)pelem->pvalue)->value, pvalue)  ||
    		(((OV_STRING_PV*)pelem->pvalue)->state != pvarcurrprops->state)    ||
    		(((OV_STRING_PV*)pelem->pvalue)->time.secs != pvarcurrprops->time.secs) ||
    		(((OV_STRING_PV*)pelem->pvalue)->time.usecs != pvarcurrprops->time.usecs) ) {
            *changed = TRUE;
        }
    }
        
	/* Set-Funktion vorhanden? */
    if( pelem->elemunion.pvar->v_setfnc ) {
        /* PV ? */
        if(pelem->elemunion.pvar->v_vartype == OV_VT_STRING_PV) {
        	OV_STRING_PV var;
			var.value = pvalue;
			var.state = pvarcurrprops->state;
			var.time = pvarcurrprops->time;
			return ((OV_FNCPTR_SETPV(STRING))pelem->elemunion.pvar->v_setfnc)(pobj, &var);
        }
        /* Sonnst ... */
        return ((OV_FNCPTR_SET(STRING))pelem->elemunion.pvar->v_setfnc)(pobj, pvalue);
    }
    
    if( *changed == TRUE ) {

        /* PV ? */
        if(pelem->elemunion.pvar->v_vartype == OV_VT_STRING_PV) {
            ((OV_STRING_PV*)pelem->pvalue)->state = pvarcurrprops->state;
        	((OV_STRING_PV*)pelem->pvalue)->time = pvarcurrprops->time;
            return ov_string_setvalue( &(((OV_STRING_PV*)pelem->pvalue)->value), pvalue);
        }

    	/* Kein PV */
        return ov_string_setvalue( (OV_STRING*)pelem->pvalue, pvalue);
    }

    return OV_ERR_OK;
}
/*	----------------------------------------------------------------------	*/
OV_RESULT fb_set_scalar_string_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {
	char        hStr[256];
	OV_STRING   val;
	OV_UINT     ksTyp;
	
	val  = hStr;
	*val = '\0';

	
	/* Typ gleich? */
	ksTyp = pvarcurrprops->value.vartype & OV_VT_KSMASK;
	if(ksTyp == OV_VT_STRING) {
        val  = pvarcurrprops->value.valueunion.val_string;
        return fb_set_scalar_string_varvalue_eval(pobj,pelem,pvarcurrprops,changed,val);
	}
		
    switch(ksTyp) {
        case OV_VT_VOID:
            break;
        case OV_VT_BYTE:
            sprintf(val, "%d", pvarcurrprops->value.valueunion.val_byte);
            break;   
        case OV_VT_BOOL: {
                if(pvarcurrprops->value.valueunion.val_bool) {
                    strcpy(val, "TRUE");
                } else {
                    strcpy(val, "FALSE");
                }
            }
            break;
        case OV_VT_INT:
            sprintf(val, "%ld", pvarcurrprops->value.valueunion.val_int);
            break;
        case OV_VT_UINT:
            sprintf(val, "%lu", pvarcurrprops->value.valueunion.val_uint);
            break;
        case OV_VT_SINGLE:
            sprintf(val, "%f", pvarcurrprops->value.valueunion.val_single);
            break;
        case OV_VT_DOUBLE:
            sprintf(val, "%f", pvarcurrprops->value.valueunion.val_double);
            break;
        case OV_VT_TIME: {
                fb_time2ascii(&(pvarcurrprops->value.valueunion.val_time), val);
            }
            break;
        case OV_VT_TIME_SPAN: {
                fb_timespan2ascii(&(pvarcurrprops->value.valueunion.val_time_span), val);
            }
            break;
        case OV_VT_STRING: {
                val = pvarcurrprops->value.valueunion.val_string;
            }
            break;
        /* Vectoren */
        case OV_VT_BYTE_VEC: {
                if(pvarcurrprops->value.valueunion.val_byte_vec.veclen > 0) {
                    sprintf(val, "%d", ((OV_BYTE*)(pvarcurrprops->value.valueunion.val_byte_vec.value))[0]);
                }
            }
            break;
        case OV_VT_BOOL_VEC: {
                if(pvarcurrprops->value.valueunion.val_bool_vec.veclen > 0) {
                    if(((OV_BOOL*)(pvarcurrprops->value.valueunion.val_bool_vec.value))[0]) {
                        strcpy(val,"TRUE");
                    } else {
                        strcpy(val,"FALSE");
                    }
                }
            }
            break;
        case OV_VT_INT_VEC: {
                if(pvarcurrprops->value.valueunion.val_int_vec.veclen > 0) {
                    sprintf(val, "%ld", ((OV_INT*)(pvarcurrprops->value.valueunion.val_int_vec.value))[0] );
                }
            }
            break;
        case OV_VT_UINT_VEC: {
                if(pvarcurrprops->value.valueunion.val_uint_vec.veclen > 0) {
                    sprintf(val, "%lu", ((OV_UINT*)(pvarcurrprops->value.valueunion.val_uint_vec.value))[0] );
                }
            }
            break;
        case OV_VT_SINGLE_VEC: {
                if(pvarcurrprops->value.valueunion.val_single_vec.veclen > 0) {
                    sprintf(val, "%f", ((OV_SINGLE*)(pvarcurrprops->value.valueunion.val_single_vec.value))[0] );
                }
            }
            break;
        case OV_VT_DOUBLE_VEC: {
                if(pvarcurrprops->value.valueunion.val_double_vec.veclen > 0) {
                    sprintf(val, "%f", ((OV_DOUBLE*)(pvarcurrprops->value.valueunion.val_double_vec.value))[0] );
                }
            }
            break;
        case OV_VT_TIME_VEC:  {
                if(pvarcurrprops->value.valueunion.val_time_vec.veclen > 0) {
                    fb_time2ascii( &(((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[0]), val);
                }
            }
            break;
        case OV_VT_TIME_SPAN_VEC: {
                if(pvarcurrprops->value.valueunion.val_time_span_vec.veclen > 0) {
                    fb_timespan2ascii( &(((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[0]), val);
                }
            }
            break;
        case OV_VT_STRING_VEC: {
                if(pvarcurrprops->value.valueunion.val_string_vec.veclen > 0) {
                    val = ((OV_STRING*)(pvarcurrprops->value.valueunion.val_string_vec.value))[0];
                }
            }
            break;
        default:
	           return OV_ERR_BADTYPE;
    }

    return fb_set_scalar_string_varvalue_eval(pobj,pelem,pvarcurrprops,changed,val);
}


/*	----------------------------------------------------------------------	*/
OV_RESULT fb_set_any_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {
    OV_ANY* pany;
	OV_UINT ksTyp;
	
    if(!pelem->pvalue) {
#if SETVAR_DEBUG
fprintf(stderr, "ANY: Element ohne Value\n");
#endif
        *changed = TRUE;
    	return ov_object_setvar(pobj, pelem, pvarcurrprops);
    }

    pany = (OV_ANY*)pelem->pvalue;

	/* Typ gleich? */
	ksTyp = pvarcurrprops->value.vartype & OV_VT_KSMASK;

    if( ksTyp != (OV_UINT)(pany->value.vartype & OV_VT_KSMASK) ) {
#if SETVAR_DEBUG
fprintf(stderr, "ANY: TYP %d != %d\n", ksTyp, (pany->value.vartype & OV_VT_KSMASK));
#endif
        *changed = TRUE;
    	return ov_object_setvar(pobj, pelem, pvarcurrprops);
    }
    
    switch(ksTyp) {
        case OV_VT_VOID:
                *changed = TRUE;
    	        return ov_object_setvar(pobj, pelem, pvarcurrprops);            
        case OV_VT_BYTE:
            if(	pany->value.valueunion.val_byte != pvarcurrprops->value.valueunion.val_byte) {
                *changed = TRUE;
    	        return ov_object_setvar(pobj, pelem, pvarcurrprops);
            }
            break;
        case OV_VT_BOOL:
            if(	pany->value.valueunion.val_bool != pvarcurrprops->value.valueunion.val_bool) {
#if SETVAR_DEBUG
fprintf(stderr, "ANY: BOOL_VAL %d != %d\n",pany->value.valueunion.val_bool , pvarcurrprops->value.valueunion.val_bool);
#endif
                *changed = TRUE;
    	        return ov_object_setvar(pobj, pelem, pvarcurrprops);
            }
            break;
        case OV_VT_INT:
            if(	pany->value.valueunion.val_int != pvarcurrprops->value.valueunion.val_int) {
#if SETVAR_DEBUG
fprintf(stderr, "ANY: INT_VAL %ld != %ld\n",pany->value.valueunion.val_int , pvarcurrprops->value.valueunion.val_int);
#endif
                *changed = TRUE;
    	        return ov_object_setvar(pobj, pelem, pvarcurrprops);
            }
            break;
        case OV_VT_UINT:
            if(	pany->value.valueunion.val_uint != pvarcurrprops->value.valueunion.val_uint) {
                *changed = TRUE;
    	        return ov_object_setvar(pobj, pelem, pvarcurrprops);
            }
            break;
        case OV_VT_SINGLE:
            if(	pany->value.valueunion.val_single != pvarcurrprops->value.valueunion.val_single) {
                *changed = TRUE;
    	        return ov_object_setvar(pobj, pelem, pvarcurrprops);
            }
            break;
        case OV_VT_DOUBLE:
            if(	pany->value.valueunion.val_double != pvarcurrprops->value.valueunion.val_double) {
                *changed = TRUE;
    	        return ov_object_setvar(pobj, pelem, pvarcurrprops);
            }
            break;
        case OV_VT_TIME:
            if(	pany->value.valueunion.val_time.secs != pvarcurrprops->value.valueunion.val_time.secs) {
                *changed = TRUE;
    	        return ov_object_setvar(pobj, pelem, pvarcurrprops);
            }
            if(	pany->value.valueunion.val_time.usecs != pvarcurrprops->value.valueunion.val_time.usecs) {
                *changed = TRUE;
    	        return ov_object_setvar(pobj, pelem, pvarcurrprops);
            }
            break;
        case OV_VT_TIME_SPAN:
            if(	pany->value.valueunion.val_time_span.secs != pvarcurrprops->value.valueunion.val_time_span.secs) {
                *changed = TRUE;
    	        return ov_object_setvar(pobj, pelem, pvarcurrprops);
            }
            if(	pany->value.valueunion.val_time_span.usecs != pvarcurrprops->value.valueunion.val_time_span.usecs) {
                *changed = TRUE;
    	        return ov_object_setvar(pobj, pelem, pvarcurrprops);
            }
            break;
        case OV_VT_STRING:
            if(	ov_string_compare(pany->value.valueunion.val_string, pvarcurrprops->value.valueunion.val_string) ) {
                *changed = TRUE;
    	        return ov_object_setvar(pobj, pelem, pvarcurrprops);
            }
            break;
        /* Vectoren */
        case OV_VT_BYTE_VEC: {
        		if( pany->value.valueunion.val_byte_vec.veclen !=
                    pvarcurrprops->value.valueunion.val_byte_vec.veclen) {
        			*changed = TRUE;
            	    return ov_object_setvar(pobj, pelem, pvarcurrprops);
        		}
        		if( Ov_CompareVectorValues(pany->value.valueunion.val_byte_vec.value,
                        pvarcurrprops->value.valueunion.val_byte_vec.value,
                        pvarcurrprops->value.valueunion.val_byte_vec.veclen,
                        BYTE)) {
                    *changed = TRUE;
            	    return ov_object_setvar(pobj, pelem, pvarcurrprops);
                }
            }
            break;
        case OV_VT_BOOL_VEC: {
        		if( pany->value.valueunion.val_bool_vec.veclen !=
                    pvarcurrprops->value.valueunion.val_bool_vec.veclen) {
        			*changed = TRUE;
            	    return ov_object_setvar(pobj, pelem, pvarcurrprops);
        		}
        		if( Ov_CompareVectorValues(pany->value.valueunion.val_bool_vec.value,
                        pvarcurrprops->value.valueunion.val_bool_vec.value,
                        pvarcurrprops->value.valueunion.val_bool_vec.veclen,
                        BOOL)) {
                    *changed = TRUE;
            	    return ov_object_setvar(pobj, pelem, pvarcurrprops);
                }
            }
            break;
        case OV_VT_INT_VEC: {
        		if( pany->value.valueunion.val_int_vec.veclen !=
                    pvarcurrprops->value.valueunion.val_int_vec.veclen) {
        			*changed = TRUE;
            	    return ov_object_setvar(pobj, pelem, pvarcurrprops);
        		}
        		if( Ov_CompareVectorValues(pany->value.valueunion.val_int_vec.value,
                        pvarcurrprops->value.valueunion.val_int_vec.value,
                        pvarcurrprops->value.valueunion.val_int_vec.veclen,
                        INT)) {
                    *changed = TRUE;
            	    return ov_object_setvar(pobj, pelem, pvarcurrprops);
                }
            }
            break;
        case OV_VT_UINT_VEC: {
        		if( pany->value.valueunion.val_uint_vec.veclen !=
                    pvarcurrprops->value.valueunion.val_uint_vec.veclen) {
        			*changed = TRUE;
            	    return ov_object_setvar(pobj, pelem, pvarcurrprops);
        		}
        		if( Ov_CompareVectorValues(pany->value.valueunion.val_uint_vec.value,
                        pvarcurrprops->value.valueunion.val_uint_vec.value,
                        pvarcurrprops->value.valueunion.val_uint_vec.veclen,
                        UINT)) {
                    *changed = TRUE;
            	    return ov_object_setvar(pobj, pelem, pvarcurrprops);
                }
            }
            break;
        case OV_VT_SINGLE_VEC: {
        		if( pany->value.valueunion.val_single_vec.veclen !=
                    pvarcurrprops->value.valueunion.val_single_vec.veclen) {
        			*changed = TRUE;
            	    return ov_object_setvar(pobj, pelem, pvarcurrprops);
        		}
        		if( Ov_CompareVectorValues(pany->value.valueunion.val_single_vec.value,
                        pvarcurrprops->value.valueunion.val_single_vec.value,
                        pvarcurrprops->value.valueunion.val_single_vec.veclen,
                        SINGLE)) {
                    *changed = TRUE;
            	    return ov_object_setvar(pobj, pelem, pvarcurrprops);
                }
            }
            break;
        case OV_VT_DOUBLE_VEC: {
        		if( pany->value.valueunion.val_double_vec.veclen !=
                    pvarcurrprops->value.valueunion.val_double_vec.veclen) {
        			*changed = TRUE;
            	    return ov_object_setvar(pobj, pelem, pvarcurrprops);
        		}
        		if( Ov_CompareVectorValues(pany->value.valueunion.val_double_vec.value,
                        pvarcurrprops->value.valueunion.val_double_vec.value,
                        pvarcurrprops->value.valueunion.val_double_vec.veclen,
                        DOUBLE)) {
                    *changed = TRUE;
            	    return ov_object_setvar(pobj, pelem, pvarcurrprops);
                }
            }
            break;
        case OV_VT_TIME_VEC:  {
        		if( pany->value.valueunion.val_time_vec.veclen !=
                    pvarcurrprops->value.valueunion.val_time_vec.veclen) {
        			*changed = TRUE;
            	    return ov_object_setvar(pobj, pelem, pvarcurrprops);
        		}
        		if( Ov_CompareVectorValues(pany->value.valueunion.val_time_vec.value,
                        pvarcurrprops->value.valueunion.val_time_vec.value,
                        pvarcurrprops->value.valueunion.val_time_vec.veclen,
                        TIME)) {
                    *changed = TRUE;
            	    return ov_object_setvar(pobj, pelem, pvarcurrprops);
                }
            }
            break;
        case OV_VT_TIME_SPAN_VEC: {
        		if( pany->value.valueunion.val_time_span_vec.veclen !=
                    pvarcurrprops->value.valueunion.val_time_span_vec.veclen) {
        			*changed = TRUE;
            	    return ov_object_setvar(pobj, pelem, pvarcurrprops);
        		}
        		if( Ov_CompareVectorValues(pany->value.valueunion.val_time_span_vec.value,
                        pvarcurrprops->value.valueunion.val_time_span_vec.value,
                        pvarcurrprops->value.valueunion.val_time_span_vec.veclen,
                        TIME_SPAN)) {
                    *changed = TRUE;
            	    return ov_object_setvar(pobj, pelem, pvarcurrprops);
                }
            }
            break;
        case OV_VT_STRING_VEC: {
        		if( pany->value.valueunion.val_string_vec.veclen !=
                    pvarcurrprops->value.valueunion.val_string_vec.veclen) {
        			*changed = TRUE;
            	    return ov_object_setvar(pobj, pelem, pvarcurrprops);
        		}
        		if( Ov_CompareVectorValues(pany->value.valueunion.val_string_vec.value,
                        pvarcurrprops->value.valueunion.val_string_vec.value,
                        pvarcurrprops->value.valueunion.val_string_vec.veclen,
                        STRING)) {
                    *changed = TRUE;
            	    return ov_object_setvar(pobj, pelem, pvarcurrprops);
                }
            }
            break;
        default:
	           return OV_ERR_BADTYPE;
    }

    return OV_ERR_OK;
}


/*	======================================================================	*/
OV_RESULT fb_set_scalar_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {

    switch(pelem->elemunion.pvar->v_vartype) {
        case OV_VT_BYTE:
    		return fb_set_scalar_byte_varvalue(pobj, pelem, pvarcurrprops, changed);
        case OV_VT_BOOL:
        case OV_VT_BOOL_PV:
    		return fb_set_scalar_bool_varvalue(pobj, pelem, pvarcurrprops, changed);
        case OV_VT_INT:
        case OV_VT_INT_PV:
    		return fb_set_scalar_int_varvalue(pobj, pelem, pvarcurrprops, changed);
        case OV_VT_UINT:
        case OV_VT_UINT_PV:
    		return fb_set_scalar_uint_varvalue(pobj, pelem, pvarcurrprops, changed);
        case OV_VT_SINGLE:
        case OV_VT_SINGLE_PV:
    		return fb_set_scalar_single_varvalue(pobj, pelem, pvarcurrprops, changed);
        case OV_VT_DOUBLE:
        case OV_VT_DOUBLE_PV:
    		return fb_set_scalar_double_varvalue(pobj, pelem, pvarcurrprops, changed);
        case OV_VT_TIME:
        case OV_VT_TIME_PV:
    		return fb_set_scalar_time_varvalue(pobj, pelem, pvarcurrprops, changed);
        case OV_VT_TIME_SPAN:
        case OV_VT_TIME_SPAN_PV:
    		return fb_set_scalar_time_span_varvalue(pobj, pelem, pvarcurrprops, changed);
    	case OV_VT_STRING :
        case OV_VT_STRING_PV:
    		return fb_set_scalar_string_varvalue(pobj, pelem, pvarcurrprops, changed);
		            
		case OV_VT_ANY:
		    return fb_set_any_varvalue(pobj, pelem, pvarcurrprops, changed);
                    
		default:
			break;
    }

	return OV_ERR_BADTYPE;
}




/*	----------------------------------------------------------------------
*   Dynamic vector variable
*	------------------------------------------------------------------- */
OV_RESULT fb_set_dynvec_byte_varvalue_eval(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed,
	OV_BYTE                     *pvalue,
    OV_UINT                      veclen
) {
    OV_UINT   size;

    if(	((OV_BYTE_VEC*)pelem->pvalue)->veclen != veclen ) {
        *changed = TRUE;
    } else {
        /* Werte gleich? */
        if(veclen > 0) {
            size = veclen*(sizeof(OV_BYTE));
            if( memcmp(pvalue, ((OV_BYTE_VEC*)pelem->pvalue)->value, size) ) {
                *changed = TRUE;
            }
        }
    }  

	/* Set-FNC vorhanden? */
    if( pelem->elemunion.pvar->v_setfnc ) {
        return ((OV_FNCPTR_SETVEC(BYTE))pelem->elemunion.pvar->v_setfnc)(pobj, pvalue, veclen);
    }
    
    if(*changed == TRUE) {
/*
        return Ov_SetDynamicVectorValue( (OV_BYTE_VEC*)pelem->pvalue, pvalue, veclen, BYTE);
*/
        size = veclen * sizeof(OV_BYTE);
        return  ov_vector_setdynamicvalue((OV_GENERIC_VEC*)((OV_BYTE_VEC*)pelem->pvalue), pvalue,veclen,size,OV_VT_BYTE);
    }    

    return OV_ERR_OK;
}
/*	----------------------------------------------------------------------	*/
OV_RESULT fb_set_dynvec_byte_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {
	OV_BYTE            *pval;
    OV_UINT             i,veclen;
    OV_RESULT           res;

	OV_INT    ival;
	OV_UINT   uival;
	OV_SINGLE sval;
	OV_DOUBLE dval;
	OV_UINT     ksTyp;

	pval = NULL;
	
	/* Typ gleich? */
	ksTyp = pvarcurrprops->value.vartype & OV_VT_KSMASK;
	if(ksTyp == OV_VT_BYTE_VEC) {
	    pval = (OV_BYTE*)(pvarcurrprops->value.valueunion.val_byte_vec.value);
        veclen  = pvarcurrprops->value.valueunion.val_byte_vec.veclen;
        
        return fb_set_dynvec_byte_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);
	}
	
    switch(ksTyp) {
        case OV_VT_VOID:
            veclen = 0;
            break;
        case OV_VT_BYTE:
            veclen = 1;
            pval = (OV_BYTE*)malloc(sizeof(OV_BYTE));
            if(!pval) {
                return OV_ERR_DBOUTOFMEMORY;
            }
            pval[0] = pvarcurrprops->value.valueunion.val_byte;
            break;   
        case OV_VT_BOOL:
            veclen = 1;
            pval = (OV_BYTE*)malloc(sizeof(OV_BYTE));
            if(!pval) {
                return OV_ERR_DBOUTOFMEMORY;
            }
            pval[0] = pvarcurrprops->value.valueunion.val_bool ? 1 : 0;
            break;
        case OV_VT_INT: {
                veclen = 1;
                pval = (OV_BYTE*)malloc(sizeof(OV_BYTE));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                ival = pvarcurrprops->value.valueunion.val_int;
                pval[0] = (OV_BYTE)ival;
            }
            break;
        case OV_VT_UINT: {
                veclen = 1;
                pval = (OV_BYTE*)malloc(sizeof(OV_BYTE));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                uival = pvarcurrprops->value.valueunion.val_uint;
                pval[0] = (OV_BYTE)uival;
            }
            break;
        case OV_VT_SINGLE: {
                veclen = 1;
                pval = (OV_BYTE*)malloc(sizeof(OV_BYTE));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                sval = pvarcurrprops->value.valueunion.val_single;
                pval[0] = (OV_BYTE)sval;
            }
            break;
        case OV_VT_DOUBLE: {
                veclen = 1;
                pval = (OV_BYTE*)malloc(sizeof(OV_BYTE));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                dval = pvarcurrprops->value.valueunion.val_double;
                pval[0] = (OV_BYTE)dval;
            }
            break;
        case OV_VT_TIME: {
                veclen = 1;
                pval = (OV_BYTE*)malloc(sizeof(OV_BYTE));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                uival = pvarcurrprops->value.valueunion.val_time.secs;
                pval[0] = (OV_BYTE)uival;
            }
            break;
        case OV_VT_TIME_SPAN: {
                veclen = 1;
                pval = (OV_BYTE*)malloc(sizeof(OV_BYTE));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                ival = pvarcurrprops->value.valueunion.val_time_span.secs;
                pval[0] = (OV_BYTE)ival;
            }
            break;
        case OV_VT_STRING: {
                veclen = 1;
                pval = (OV_BYTE*)malloc(sizeof(OV_BYTE));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                if(!ov_string_compare(pvarcurrprops->value.valueunion.val_string, "")) {
                    ival = 0;
                } else {
                    ival = atol(pvarcurrprops->value.valueunion.val_string);
                }
                pval[0] = (OV_BYTE)ival;
            }
            break;
        /* Vectoren */
        case OV_VT_BYTE_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_byte_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_BYTE*)malloc(veclen*sizeof(OV_BYTE));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = ((OV_BYTE*)(pvarcurrprops->value.valueunion.val_byte_vec.value))[i];
                    }
                }
            }
            break;
        case OV_VT_BOOL_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_bool_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_BYTE*)malloc(veclen*sizeof(OV_BYTE));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = ((OV_BOOL*)(pvarcurrprops->value.valueunion.val_bool_vec.value))[i] ? 1 : 0;
                    }
                }
            }
            break;
        case OV_VT_INT_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_int_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_BYTE*)malloc(veclen*sizeof(OV_BYTE));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        ival = ((OV_INT*)(pvarcurrprops->value.valueunion.val_int_vec.value))[i];
                        pval[i] = (OV_BYTE)ival;
                    }
                }
            }
            break;
        case OV_VT_UINT_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_uint_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_BYTE*)malloc(veclen*sizeof(OV_BYTE));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        uival = ((OV_UINT*)(pvarcurrprops->value.valueunion.val_uint_vec.value))[i];
                        pval[i] = (OV_BYTE)uival;
                    }
                }
            }
            break;
        case OV_VT_SINGLE_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_single_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_BYTE*)malloc(veclen*sizeof(OV_BYTE));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        sval = ((OV_SINGLE*)(pvarcurrprops->value.valueunion.val_single_vec.value))[i];
                        pval[i] = (OV_BYTE)sval;
                    }
                }
            }
            break;
        case OV_VT_DOUBLE_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_double_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_BYTE*)malloc(veclen*sizeof(OV_BYTE));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        dval = ((OV_DOUBLE*)(pvarcurrprops->value.valueunion.val_double_vec.value))[i];
                        pval[i] = (OV_BYTE)dval;
                    }
                }
            }
            break;
        case OV_VT_TIME_VEC:  {
                veclen  = pvarcurrprops->value.valueunion.val_time_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_BYTE*)malloc(veclen*sizeof(OV_BYTE));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        uival = ((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[i].secs;
                        pval[i] = (OV_BYTE)uival;
                    }
                }
            }
            break;
        case OV_VT_TIME_SPAN_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_time_span_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_BYTE*)malloc(veclen*sizeof(OV_BYTE));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        ival = ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[i].secs;
                        pval[i] = (OV_BYTE)ival;
                    }
                }
            }
            break;
        case OV_VT_STRING_VEC: {
                OV_STRING str;
                veclen  = pvarcurrprops->value.valueunion.val_time_span_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_BYTE*)malloc(veclen*sizeof(OV_BYTE));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        str = ((OV_STRING*)(pvarcurrprops->value.valueunion.val_string_vec.value))[i];
                        if(!ov_string_compare(str, "")) {
                            ival = 0;
                        } else {
                            ival = (OV_INT)atol(str);
                        }
                        pval[i] = (OV_BYTE)ival;
                    }
                }
            }
            break;
        default:
	           return OV_ERR_BADTYPE;
    }

    res = fb_set_dynvec_byte_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);
    
    /* Und fertig */
    if(pval) { free(pval); }

    return res;
}
/*	======================================================================	*/
OV_RESULT fb_set_dynvec_bool_varvalue_eval(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed,
	OV_BOOL                     *pvalue,
    OV_UINT                      veclen
) {
    OV_UINT   size;

    /* PV? */
    if(pelem->elemunion.pvar->v_vartype == OV_VT_BOOL_VEC) {
        /* Laenge geaendert? */
        if(	((OV_BOOL_VEC*)pelem->pvalue)->veclen != veclen ) {
            *changed = TRUE;
        } else {        
            /* Wert geaendert? */
            if(veclen > 0) {
                size = veclen*(sizeof(OV_BOOL));
                if( memcmp(pvalue, ((OV_BOOL_VEC*)pelem->pvalue)->value, size) ) {
                    *changed = TRUE;
                }
            }
            /*
            for(i=0; i<veclen; i++) {
                if(pval[i] != ((OV_BOOL_VEC*)pelem->pvalue)->value[i]) {
                    *changed = TRUE;
                    break;                
                }
            }
            */
        }
    } else {
        /* Laenge geaendert? */
        if(	((OV_BOOL_PV_VEC*)pelem->pvalue)->value.veclen != veclen ) {
            *changed = TRUE;
        } else {        
            /* PV-Status geaendert? */
    		if(((OV_BOOL_PV_VEC*)pelem->pvalue)->state != pvarcurrprops->state ||
    		    (((OV_BOOL_PV_VEC*)pelem->pvalue)->time.secs != pvarcurrprops->time.secs) ||
    		    (((OV_BOOL_PV_VEC*)pelem->pvalue)->time.usecs != pvarcurrprops->time.usecs) ) {
        		*changed = TRUE;
        	} else {      		
                /* Wert geaendert? */
                if(veclen > 0) {
                    size = veclen*(sizeof(OV_BOOL));
                    if( memcmp(pvalue, ((OV_BOOL_PV_VEC*)pelem->pvalue)->value.value, size) ) {
                        *changed = TRUE;
                    }
                }
                /*
                for(i=0; i<veclen; i++) {
                    if(pval[i] != ((OV_BOOL_PV_VEC*)pelem->pvalue)->value.value[i]) {
                        *changed = TRUE;
                        break;                
                    }
                }
                */
            }
        }
    }
    
	/* SET-FNC vorhanden? */
    if( pelem->elemunion.pvar->v_setfnc ) {
        if(pelem->elemunion.pvar->v_vartype == OV_VT_BOOL_PV_VEC) {
			OV_BOOL_PV_VEC var;
			var.value.value = pvalue;
			var.value.veclen= veclen;
			var.state = pvarcurrprops->state;
			var.time = pvarcurrprops->time;
			return ((OV_FNCPTR_SETPVVEC(BOOL))pelem->elemunion.pvar->v_setfnc)(pobj, &var);
        }
        return ((OV_FNCPTR_SETVEC(BOOL))pelem->elemunion.pvar->v_setfnc)(pobj, pvalue, veclen);
    }
    
    if(*changed == TRUE) {
        size = veclen * sizeof(OV_BOOL);
        if(pelem->elemunion.pvar->v_vartype == OV_VT_BOOL_VEC) {
            return  ov_vector_setdynamicvalue((OV_GENERIC_VEC*)((OV_BOOL_VEC*)pelem->pvalue), pvalue,veclen,size,OV_VT_BOOL);
        }
        
		((OV_BOOL_PV_VEC*)pelem->pvalue)->state = pvarcurrprops->state;
		((OV_BOOL_PV_VEC*)pelem->pvalue)->time = pvarcurrprops->time;
        return  ov_vector_setdynamicvalue( (OV_GENERIC_VEC*)(&(((OV_BOOL_PV_VEC*)pelem->pvalue)->value)), pvalue,veclen,size,OV_VT_BOOL);
    }

    return OV_ERR_OK;
}
/*	----------------------------------------------------------------------	*/
OV_RESULT fb_set_dynvec_bool_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {
	OV_BOOL            *pval;
    OV_UINT             i,veclen;
    OV_RESULT           res;

	OV_INT    ival;
	OV_UINT   uival;
	OV_SINGLE sval;
	OV_DOUBLE dval;
	OV_UINT     ksTyp;

	pval = NULL;
	
	/* Typ gleich? */
	ksTyp = pvarcurrprops->value.vartype & OV_VT_KSMASK;
	if(ksTyp == OV_VT_BYTE_VEC) {
	    pval = (OV_BOOL*)(pvarcurrprops->value.valueunion.val_bool_vec.value);
        veclen  = pvarcurrprops->value.valueunion.val_bool_vec.veclen;
        
        return fb_set_dynvec_bool_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);
	}
	
    switch(ksTyp) {
        case OV_VT_VOID:
            veclen = 0;
            break;
        case OV_VT_BYTE:
            veclen = 1;
            pval = (OV_BOOL*)malloc(sizeof(OV_BOOL));
            if(!pval) {
                return OV_ERR_DBOUTOFMEMORY;
            }
            pval[0] = pvarcurrprops->value.valueunion.val_byte ? TRUE : FALSE;
            break;   
        case OV_VT_BOOL:
            veclen = 1;
            pval = (OV_BOOL*)malloc(sizeof(OV_BOOL));
            if(!pval) {
                return OV_ERR_DBOUTOFMEMORY;
            }
            pval[0] = pvarcurrprops->value.valueunion.val_bool;
            break;
        case OV_VT_INT: {
                veclen = 1;
                pval = (OV_BOOL*)malloc(sizeof(OV_BOOL));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                ival = pvarcurrprops->value.valueunion.val_int;
                pval[0] = ival ? TRUE: FALSE;
            }
            break;
        case OV_VT_UINT: {
                veclen = 1;
                pval = (OV_BOOL*)malloc(sizeof(OV_BOOL));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                uival = pvarcurrprops->value.valueunion.val_uint;
                pval[0] = uival ? TRUE : FALSE;
            }
            break;
        case OV_VT_SINGLE: {
                veclen = 1;
                pval = (OV_BOOL*)malloc(sizeof(OV_BOOL));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                sval = pvarcurrprops->value.valueunion.val_single;
                pval[0] = sval ? TRUE : FALSE;
            }
            break;
        case OV_VT_DOUBLE: {
                veclen = 1;
                pval = (OV_BOOL*)malloc(sizeof(OV_BOOL));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                dval = pvarcurrprops->value.valueunion.val_double;
                pval[0] = dval ? TRUE : FALSE;
            }
            break;
        case OV_VT_TIME: {
                veclen = 1;
                pval = (OV_BOOL*)malloc(sizeof(OV_BOOL));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                uival = pvarcurrprops->value.valueunion.val_time.secs;
                pval[0] = uival ? TRUE : FALSE;
                if(!uival) {
                    uival = pvarcurrprops->value.valueunion.val_time.usecs;
                    pval[0] = uival ? TRUE : FALSE;
                }
            }
            break;
        case OV_VT_TIME_SPAN: {
                veclen = 1;
                pval = (OV_BOOL*)malloc(sizeof(OV_BOOL));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                ival = pvarcurrprops->value.valueunion.val_time_span.secs;
                pval[0] = ival ? TRUE : FALSE;
                if(!ival) {
                    ival = pvarcurrprops->value.valueunion.val_time_span.usecs;
                    pval[0] = ival ? TRUE : FALSE;
                }
            }
            break;
        case OV_VT_STRING: {
                OV_STRING str;
                veclen = 1;
                pval = (OV_BOOL*)malloc(sizeof(OV_BOOL));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                str = pvarcurrprops->value.valueunion.val_string;
                if(!ov_string_compare(str, "")) {
                    ival = 0;
                } else {
                    if( !ov_string_compare(str, "TRUE") || !ov_string_compare(str, "true") ) {
                        ival = 1;
                    } else {
                        ival = atol(str);
                    }
                }
                pval[0] = ival ? TRUE : FALSE;
            }
            break;
        /* Vectoren */
        case OV_VT_BYTE_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_byte_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_BOOL*)malloc(veclen*sizeof(OV_BOOL));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = ((OV_BYTE*)(pvarcurrprops->value.valueunion.val_byte_vec.value))[i] ? TRUE : FALSE;
                    }
                }
            }
            break;
        case OV_VT_BOOL_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_bool_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_BOOL*)malloc(veclen*sizeof(OV_BOOL));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = ((OV_BOOL*)(pvarcurrprops->value.valueunion.val_bool_vec.value))[i];
                    }
                }
            }
            break;
        case OV_VT_INT_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_int_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_BOOL*)malloc(veclen*sizeof(OV_BOOL));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        ival = ((OV_INT*)(pvarcurrprops->value.valueunion.val_int_vec.value))[i];
                        pval[i] = ival ? TRUE : FALSE;
                    }
                }
            }
            break;
        case OV_VT_UINT_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_uint_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_BOOL*)malloc(veclen*sizeof(OV_BOOL));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        uival = ((OV_UINT*)(pvarcurrprops->value.valueunion.val_uint_vec.value))[i];
                        pval[i] = uival ? TRUE : FALSE;
                    }
                }
            }
            break;
        case OV_VT_SINGLE_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_single_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_BOOL*)malloc(veclen*sizeof(OV_BOOL));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        sval = ((OV_SINGLE*)(pvarcurrprops->value.valueunion.val_single_vec.value))[i];
                        pval[i] = sval ? TRUE : FALSE;
                    }
                }
            }
            break;
        case OV_VT_DOUBLE_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_double_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_BOOL*)malloc(veclen*sizeof(OV_BOOL));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        dval = ((OV_DOUBLE*)(pvarcurrprops->value.valueunion.val_double_vec.value))[i];
                        pval[i] = dval ? TRUE : FALSE;
                    }
                }
            }
            break;
        case OV_VT_TIME_VEC:  {
                veclen  = pvarcurrprops->value.valueunion.val_time_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_BOOL*)malloc(veclen*sizeof(OV_BOOL));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        uival = ((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[i].secs;
                        pval[i] = uival ? TRUE : FALSE;
                        if(!uival) {
                            uival = ((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[i].usecs;
                            pval[i] = uival ? TRUE : FALSE;
                        }
                    }
                }
            }
            break;
        case OV_VT_TIME_SPAN_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_time_span_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_BOOL*)malloc(veclen*sizeof(OV_BOOL));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        ival = ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[i].secs;
                        if(!ival) {
                            ival = ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[i].usecs;
                        }
                        pval[i] = ival ? TRUE : FALSE;
                    }
                }
            }
            break;
        case OV_VT_STRING_VEC: {
                OV_STRING str;
                veclen  = pvarcurrprops->value.valueunion.val_time_span_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_BOOL*)malloc(veclen*sizeof(OV_BOOL));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        str = ((OV_STRING*)(pvarcurrprops->value.valueunion.val_string_vec.value))[i];
                        if(!ov_string_compare(str, "")) {
                            ival = 0;
                        } else {
                            if( !ov_string_compare(str, "TRUE") || !ov_string_compare(str, "true") ) {
                                ival = 1;
                            } else {
                                ival = atol(str);
                            }
                        }
                        pval[i] = ival ? TRUE : FALSE;
                    }
                }
            }
            break;
        default:
	           return OV_ERR_BADTYPE;
    }

    res = fb_set_dynvec_bool_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);

    /* Speicher freigeben */
    if(pval) { free(pval); }

    return res;
}

/*	======================================================================	*/
OV_RESULT fb_set_dynvec_int_varvalue_eval(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed,
	OV_INT                      *pvalue,
    OV_UINT                      veclen
) {
    OV_UINT   size;

    /* PV? */
    if(pelem->elemunion.pvar->v_vartype == OV_VT_INT_VEC) {
        /* Laenge geaendert? */
        if(	((OV_INT_VEC*)pelem->pvalue)->veclen != veclen ) {
            *changed = TRUE;
        } else {        
            /* Wert geaendert? */
            if(veclen > 0) {
                size = veclen*(sizeof(OV_INT));
                if( memcmp(pvalue, ((OV_INT_VEC*)pelem->pvalue)->value, size) ) {
                    *changed = TRUE;
                }
            }
        }
    } else {
        /* Laenge geaendert? */
        if(	((OV_INT_PV_VEC*)pelem->pvalue)->value.veclen != veclen ) {
            *changed = TRUE;
        } else {        
            /* PV-Status geaendert? */
    		if(((OV_INT_PV_VEC*)pelem->pvalue)->state != pvarcurrprops->state ||
    		    (((OV_INT_PV_VEC*)pelem->pvalue)->time.secs != pvarcurrprops->time.secs) ||
    		    (((OV_INT_PV_VEC*)pelem->pvalue)->time.usecs != pvarcurrprops->time.usecs) ) {
        		*changed = TRUE;
        	} else {      		
                /* Wert geaendert? */
                if(veclen > 0) {
                    size = veclen*(sizeof(OV_INT));
                    if( memcmp(pvalue, ((OV_INT_PV_VEC*)pelem->pvalue)->value.value, size) ) {
                        *changed = TRUE;
                    }
                }
            }
        }
    }
    
	/* SET-FNC vorhanden? */
    if( pelem->elemunion.pvar->v_setfnc ) {
        if(pelem->elemunion.pvar->v_vartype == OV_VT_INT_PV_VEC) {
			OV_INT_PV_VEC var;
			var.value.value = pvalue;
			var.value.veclen= veclen;
			var.state = pvarcurrprops->state;
			var.time = pvarcurrprops->time;
			return ((OV_FNCPTR_SETPVVEC(INT))pelem->elemunion.pvar->v_setfnc)(pobj, &var);
        }
        return ((OV_FNCPTR_SETVEC(INT))pelem->elemunion.pvar->v_setfnc)(pobj, pvalue, veclen);
    }
    
    if(*changed == TRUE) {
        size = veclen*(sizeof(OV_INT));
        if(pelem->elemunion.pvar->v_vartype == OV_VT_INT_VEC) {
            return  ov_vector_setdynamicvalue( (OV_GENERIC_VEC*)((OV_INT_VEC*)pelem->pvalue), pvalue,veclen,size,OV_VT_INT);
        }
        
		((OV_INT_PV_VEC*)pelem->pvalue)->state = pvarcurrprops->state;
		((OV_INT_PV_VEC*)pelem->pvalue)->time = pvarcurrprops->time;
        return  ov_vector_setdynamicvalue( (OV_GENERIC_VEC*)(&(((OV_INT_PV_VEC*)pelem->pvalue)->value)), pvalue,veclen,size,OV_VT_INT);
    }

    return OV_ERR_OK;
}
/*	----------------------------------------------------------------------	*/
OV_RESULT fb_set_dynvec_int_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {
	OV_INT             *pval;
    OV_UINT             i,veclen;
    OV_RESULT           res;

	OV_UINT   uival;
	OV_SINGLE sval;
	OV_DOUBLE dval;
	OV_UINT     ksTyp;

	pval = NULL;
	
	/* Typ gleich? */
	ksTyp = pvarcurrprops->value.vartype & OV_VT_KSMASK;
	if(ksTyp == OV_VT_INT_VEC) {
	    pval = (OV_INT*)(pvarcurrprops->value.valueunion.val_int_vec.value);
        veclen  = pvarcurrprops->value.valueunion.val_int_vec.veclen;
        
        return fb_set_dynvec_int_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);
	}
	
    switch(pvarcurrprops->value.vartype) {
        case OV_VT_VOID:
            veclen = 0;
            break;
        case OV_VT_BYTE:
            veclen = 1;
            pval = (OV_INT*)malloc(sizeof(OV_INT));
            if(!pval) {
                return OV_ERR_DBOUTOFMEMORY;
            }
            pval[0] = (OV_INT)pvarcurrprops->value.valueunion.val_byte;
            break;   
        case OV_VT_BOOL:
            veclen = 1;
            pval = (OV_INT*)malloc(sizeof(OV_INT));
            if(!pval) {
                return OV_ERR_DBOUTOFMEMORY;
            }
            pval[0] = (OV_INT)pvarcurrprops->value.valueunion.val_bool;
            break;
        case OV_VT_INT: {
                veclen = 1;
                pval = (OV_INT*)malloc(sizeof(OV_INT));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                pval[0] = pvarcurrprops->value.valueunion.val_int;
            }
            break;
        case OV_VT_UINT: {
                veclen = 1;
                pval = (OV_INT*)malloc(sizeof(OV_INT));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                uival = pvarcurrprops->value.valueunion.val_uint;
                pval[0] = (OV_INT)uival;
            }
            break;
        case OV_VT_SINGLE: {
                veclen = 1;
                pval = (OV_INT*)malloc(sizeof(OV_INT));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                sval = pvarcurrprops->value.valueunion.val_single;
                pval[0] = (OV_INT)sval;
            }
            break;
        case OV_VT_DOUBLE: {
                veclen = 1;
                pval = (OV_INT*)malloc(sizeof(OV_INT));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                dval = pvarcurrprops->value.valueunion.val_double;
                pval[0] = (OV_INT)dval;
            }
            break;
        case OV_VT_TIME: {
                veclen = 1;
                pval = (OV_INT*)malloc(sizeof(OV_INT));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                uival = pvarcurrprops->value.valueunion.val_time.secs;
                pval[0] = (OV_INT)uival;
            }
            break;
        case OV_VT_TIME_SPAN: {
                veclen = 1;
                pval = (OV_INT*)malloc(sizeof(OV_INT));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                pval[0] = pvarcurrprops->value.valueunion.val_time_span.secs;
            }
            break;
        case OV_VT_STRING: {
                OV_STRING str;
                veclen = 1;
                pval = (OV_INT*)malloc(sizeof(OV_INT));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                str = pvarcurrprops->value.valueunion.val_string;
                if(!ov_string_compare(str,"")) {
                    pval[0] = 0;
                } else {
                    pval[0] = atol(str);
                }
            }
            break;
        /* Vectoren */
        case OV_VT_BYTE_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_byte_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_INT*)malloc(veclen*sizeof(OV_INT));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = (OV_INT)(((OV_BYTE*)(pvarcurrprops->value.valueunion.val_byte_vec.value))[i]);
                    }
                }
            }
            break;
        case OV_VT_BOOL_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_bool_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_INT*)malloc(veclen*sizeof(OV_INT));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = (OV_INT)(((OV_BOOL*)(pvarcurrprops->value.valueunion.val_bool_vec.value))[i]);
                    }
                }
            }
            break;
        case OV_VT_INT_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_int_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_INT*)malloc(veclen*sizeof(OV_INT));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = ((OV_INT*)(pvarcurrprops->value.valueunion.val_int_vec.value))[i];
                    }
                }
            }
            break;
        case OV_VT_UINT_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_uint_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_INT*)malloc(veclen*sizeof(OV_INT));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        uival = ((OV_UINT*)(pvarcurrprops->value.valueunion.val_uint_vec.value))[i];
                        pval[i] = (OV_INT)uival;
                    }
                }
            }
            break;
        case OV_VT_SINGLE_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_single_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_INT*)malloc(veclen*sizeof(OV_INT));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        sval = ((OV_SINGLE*)(pvarcurrprops->value.valueunion.val_single_vec.value))[i];
                        pval[i] = (OV_INT)sval;
                    }
                }
            }
            break;
        case OV_VT_DOUBLE_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_double_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_INT*)malloc(veclen*sizeof(OV_INT));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        dval = ((OV_DOUBLE*)(pvarcurrprops->value.valueunion.val_double_vec.value))[i];
                        pval[i] = (OV_INT)dval;
                    }
                }
            }
            break;
        case OV_VT_TIME_VEC:  {
                veclen  = pvarcurrprops->value.valueunion.val_time_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_INT*)malloc(veclen*sizeof(OV_INT));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        uival = ((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[i].secs;
                        pval[i] = (OV_INT)uival;
                    }
                }
            }
            break;
        case OV_VT_TIME_SPAN_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_time_span_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_INT*)malloc(veclen*sizeof(OV_INT));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[i].secs;
                    }
                }
            }
            break;
        case OV_VT_STRING_VEC: {
                OV_STRING str;
                veclen  = pvarcurrprops->value.valueunion.val_time_span_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_INT*)malloc(veclen*sizeof(OV_INT));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        str = ((OV_STRING*)(pvarcurrprops->value.valueunion.val_string_vec.value))[i];
                        if(!ov_string_compare(str, "")) {
                            pval[i] = 0;
                        } else {
                            pval[i] = atol(str);
                        }
                    }
                }
            }
            break;
        default:
	           return OV_ERR_BADTYPE;
    }

    res = fb_set_dynvec_int_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);
    
    /* Speicher freigeben */
    if(pval) { free(pval); }

    return res;
}
/*	======================================================================	*/
OV_RESULT fb_set_dynvec_uint_varvalue_eval(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed,
	OV_UINT                     *pvalue,
    OV_UINT                      veclen
) {
    OV_UINT   size;

    /* PV? */
    if(pelem->elemunion.pvar->v_vartype == OV_VT_UINT_VEC) {
        /* Laenge geaendert? */
        if(	((OV_UINT_VEC*)pelem->pvalue)->veclen != veclen ) {
            *changed = TRUE;
        } else {        
            /* Wert geaendert? */
            if(veclen > 0) {
                size = veclen*(sizeof(OV_UINT));
                if( memcmp(pvalue, ((OV_UINT_VEC*)pelem->pvalue)->value, size) ) {
                    *changed = TRUE;
                }
            }
        }
    } else {
        /* Laenge geaendert? */
        if(	((OV_UINT_PV_VEC*)pelem->pvalue)->value.veclen != veclen ) {
            *changed = TRUE;
        } else {        
            /* PV-Status geaendert? */
    		if(  ((OV_UINT_PV_VEC*)pelem->pvalue)->state != pvarcurrprops->state ||
    		    (((OV_UINT_PV_VEC*)pelem->pvalue)->time.secs != pvarcurrprops->time.secs) ||
    		    (((OV_UINT_PV_VEC*)pelem->pvalue)->time.usecs != pvarcurrprops->time.usecs) ) {
        		*changed = TRUE;
        	} else {      		
                /* Wert geaendert? */
                if(veclen > 0) {
                    size = veclen*(sizeof(OV_UINT));
                    if( memcmp(pvalue, ((OV_UINT_PV_VEC*)pelem->pvalue)->value.value, size) ) {
                        *changed = TRUE;
                    }
                }
            }
        }
    }
    
	/* SET-FNC vorhanden? */
    if( pelem->elemunion.pvar->v_setfnc ) {
        if(pelem->elemunion.pvar->v_vartype == OV_VT_UINT_PV_VEC) {
			OV_UINT_PV_VEC var;
			var.value.value = pvalue;
			var.value.veclen= veclen;
			var.state = pvarcurrprops->state;
			var.time = pvarcurrprops->time;
			return ((OV_FNCPTR_SETPVVEC(UINT))pelem->elemunion.pvar->v_setfnc)(pobj, &var);
        }
        return ((OV_FNCPTR_SETVEC(UINT))pelem->elemunion.pvar->v_setfnc)(pobj, pvalue, veclen);
    }
    
    if(*changed == TRUE) {
        size = veclen*(sizeof(OV_UINT));
        if(pelem->elemunion.pvar->v_vartype == OV_VT_UINT_VEC) {
            return  ov_vector_setdynamicvalue( (OV_GENERIC_VEC*)((OV_UINT_VEC*)pelem->pvalue), pvalue,veclen,size,OV_VT_UINT);
        }
        
		((OV_UINT_PV_VEC*)pelem->pvalue)->state = pvarcurrprops->state;
		((OV_UINT_PV_VEC*)pelem->pvalue)->time = pvarcurrprops->time;
        return  ov_vector_setdynamicvalue( (OV_GENERIC_VEC*)(&(((OV_UINT_PV_VEC*)pelem->pvalue)->value)), pvalue,veclen,size,OV_VT_UINT);
    }

    return OV_ERR_OK;
}
/*	----------------------------------------------------------------------	*/
OV_RESULT fb_set_dynvec_uint_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {
	OV_UINT             *pval;
    OV_UINT             i,veclen;
    OV_RESULT           res;

    
	OV_SINGLE sval;
	OV_DOUBLE dval;
	OV_UINT     ksTyp;

	pval = NULL;
	
	/* Typ gleich? */
	ksTyp = pvarcurrprops->value.vartype & OV_VT_KSMASK;
	if(ksTyp == OV_VT_UINT_VEC) {
	    pval = (OV_UINT*)(pvarcurrprops->value.valueunion.val_uint_vec.value);
        veclen  = pvarcurrprops->value.valueunion.val_uint_vec.veclen;
        
        return fb_set_dynvec_uint_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);
	}

    switch(ksTyp) {
        case OV_VT_VOID:
            veclen = 0;
            break;
        case OV_VT_BYTE:
            veclen = 1;
            pval = (OV_UINT*)malloc(sizeof(OV_UINT));
            if(!pval) {
                return OV_ERR_DBOUTOFMEMORY;
            }
            pval[0] = (OV_UINT)pvarcurrprops->value.valueunion.val_byte;
            break;   
        case OV_VT_BOOL:
            veclen = 1;
            pval = (OV_UINT*)malloc(sizeof(OV_UINT));
            if(!pval) {
                return OV_ERR_DBOUTOFMEMORY;
            }
            pval[0] = (OV_UINT)pvarcurrprops->value.valueunion.val_bool;
            break;
        case OV_VT_INT: {
                veclen = 1;
                pval = (OV_UINT*)malloc(sizeof(OV_UINT));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                pval[0] = (OV_UINT)pvarcurrprops->value.valueunion.val_int;
            }
            break;
        case OV_VT_UINT: {
                veclen = 1;
                pval = (OV_UINT*)malloc(sizeof(OV_UINT));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                pval[0] = pvarcurrprops->value.valueunion.val_uint;
            }
            break;
        case OV_VT_SINGLE: {
                veclen = 1;
                pval = (OV_UINT*)malloc(sizeof(OV_UINT));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                sval = pvarcurrprops->value.valueunion.val_single;
                pval[0] = (OV_UINT)sval;
            }
            break;
        case OV_VT_DOUBLE: {
                veclen = 1;
                pval = (OV_UINT*)malloc(sizeof(OV_UINT));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                dval = pvarcurrprops->value.valueunion.val_double;
                pval[0] = (OV_UINT)dval;
            }
            break;
        case OV_VT_TIME: {
                veclen = 1;
                pval = (OV_UINT*)malloc(sizeof(OV_UINT));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                pval[0] = pvarcurrprops->value.valueunion.val_time.secs;
            }
            break;
        case OV_VT_TIME_SPAN: {
                veclen = 1;
                pval = (OV_UINT*)malloc(sizeof(OV_UINT));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                pval[0] = (OV_UINT)pvarcurrprops->value.valueunion.val_time_span.secs;
            }
            break;
        case OV_VT_STRING: {
                OV_STRING str;
                veclen = 1;
                pval = (OV_UINT*)malloc(sizeof(OV_UINT));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                str = pvarcurrprops->value.valueunion.val_string;
                if(!ov_string_compare(str,"")) {
                    pval[0] = 0;
                } else {
                    pval[0] = (OV_UINT)atol(str);
                }
            }
            break;
        /* Vectoren */
        case OV_VT_BYTE_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_byte_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_UINT*)malloc(veclen*sizeof(OV_UINT));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = (OV_UINT)(((OV_BYTE*)(pvarcurrprops->value.valueunion.val_byte_vec.value))[i]);
                    }
                }
            }
            break;
        case OV_VT_BOOL_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_bool_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_UINT*)malloc(veclen*sizeof(OV_UINT));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = (OV_UINT)(((OV_BOOL*)(pvarcurrprops->value.valueunion.val_bool_vec.value))[i]);
                    }
                }
            }
            break;
        case OV_VT_INT_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_int_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_UINT*)malloc(veclen*sizeof(OV_UINT));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = (OV_UINT)(((OV_INT*)(pvarcurrprops->value.valueunion.val_int_vec.value))[i]);
                    }
                }
            }
            break;
        case OV_VT_UINT_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_uint_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_UINT*)malloc(veclen*sizeof(OV_UINT));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = ((OV_UINT*)(pvarcurrprops->value.valueunion.val_uint_vec.value))[i];
                    }
                }
            }
            break;
        case OV_VT_SINGLE_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_single_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_UINT*)malloc(veclen*sizeof(OV_UINT));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        sval = ((OV_SINGLE*)(pvarcurrprops->value.valueunion.val_single_vec.value))[i];
                        pval[i] = (OV_UINT)sval;
                    }
                }
            }
            break;
        case OV_VT_DOUBLE_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_double_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_UINT*)malloc(veclen*sizeof(OV_UINT));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        dval = ((OV_DOUBLE*)(pvarcurrprops->value.valueunion.val_double_vec.value))[i];
                        pval[i] = (OV_UINT)dval;
                    }
                }
            }
            break;
        case OV_VT_TIME_VEC:  {
                veclen  = pvarcurrprops->value.valueunion.val_time_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_UINT*)malloc(veclen*sizeof(OV_UINT));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = ((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[i].secs;
                    }
                }
            }
            break;
        case OV_VT_TIME_SPAN_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_time_span_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_UINT*)malloc(veclen*sizeof(OV_UINT));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = (OV_UINT)(((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[i].secs);
                    }
                }
            }
            break;
        case OV_VT_STRING_VEC: {
                OV_STRING str;
                veclen  = pvarcurrprops->value.valueunion.val_time_span_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_UINT*)malloc(veclen*sizeof(OV_UINT));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        str = ((OV_STRING*)(pvarcurrprops->value.valueunion.val_string_vec.value))[i];
                        if(!ov_string_compare(str, "")) {
                            pval[i] = 0;
                        } else {
                            pval[i] = (OV_UINT)atol(str);
                        }
                    }
                }
            }
            break;
        default:
	           return OV_ERR_BADTYPE;
    }

    res = fb_set_dynvec_uint_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);

    /* Speicher freigeben */
    if(pval) { free(pval); }

    return res;
}

/*	======================================================================	*/
OV_RESULT fb_set_dynvec_single_varvalue_eval(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed,
	OV_SINGLE                   *pvalue,
    OV_UINT                      veclen
) {
    OV_UINT   size;

    /* PV? */
    if(pelem->elemunion.pvar->v_vartype == OV_VT_SINGLE_VEC) {
        /* Laenge geaendert? */
        if(	((OV_SINGLE_VEC*)pelem->pvalue)->veclen != veclen ) {
            *changed = TRUE;
        } else {        
            /* Wert geaendert? */
            if(veclen > 0) {
                size = veclen*(sizeof(OV_SINGLE));
                if( memcmp(pvalue, ((OV_SINGLE_VEC*)pelem->pvalue)->value, size) ) {
                    *changed = TRUE;
                }
            }
        }
    } else {
        /* Laenge geaendert? */
        if(	((OV_SINGLE_PV_VEC*)pelem->pvalue)->value.veclen != veclen ) {
            *changed = TRUE;
        } else {        
            /* PV-Status geaendert? */
    		if(  ((OV_SINGLE_PV_VEC*)pelem->pvalue)->state != pvarcurrprops->state ||
    		    (((OV_SINGLE_PV_VEC*)pelem->pvalue)->time.secs != pvarcurrprops->time.secs) ||
    		    (((OV_SINGLE_PV_VEC*)pelem->pvalue)->time.usecs != pvarcurrprops->time.usecs) ) {
        		*changed = TRUE;
        	} else {      		
                /* Wert geaendert? */
                if(veclen > 0) {
                    size = veclen*(sizeof(OV_SINGLE));
                    if( memcmp(pvalue, ((OV_SINGLE_PV_VEC*)pelem->pvalue)->value.value, size) ) {
                        *changed = TRUE;
                    }
                }
            }
        }
    }
    
	/* SET-FNC vorhanden? */
    if( pelem->elemunion.pvar->v_setfnc ) {
        if(pelem->elemunion.pvar->v_vartype == OV_VT_SINGLE_PV_VEC) {
			OV_SINGLE_PV_VEC var;
			var.value.value = pvalue;
			var.value.veclen= veclen;
			var.state = pvarcurrprops->state;
			var.time = pvarcurrprops->time;
			return ((OV_FNCPTR_SETPVVEC(SINGLE))pelem->elemunion.pvar->v_setfnc)(pobj, &var);
        }
        return ((OV_FNCPTR_SETVEC(SINGLE))pelem->elemunion.pvar->v_setfnc)(pobj, pvalue, veclen);
    }
    
    if(*changed == TRUE) {
        size = veclen*(sizeof(OV_SINGLE));
        if(pelem->elemunion.pvar->v_vartype == OV_VT_SINGLE_VEC) {
            return  ov_vector_setdynamicvalue( (OV_GENERIC_VEC*)((OV_SINGLE_PV_VEC*)pelem->pvalue), pvalue,veclen,size,OV_VT_SINGLE);
        }
        
		((OV_SINGLE_PV_VEC*)pelem->pvalue)->state = pvarcurrprops->state;
		((OV_SINGLE_PV_VEC*)pelem->pvalue)->time = pvarcurrprops->time;
        return  ov_vector_setdynamicvalue( (OV_GENERIC_VEC*)(&(((OV_SINGLE_PV_VEC*)pelem->pvalue)->value)), pvalue,veclen,size,OV_VT_SINGLE);
    }

    return OV_ERR_OK;
}
/*	----------------------------------------------------------------------	*/
OV_RESULT fb_set_dynvec_single_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {
	OV_SINGLE           *pval;
    OV_UINT             i,veclen;
    OV_RESULT           res;
	OV_DOUBLE           dval;
	OV_UINT     ksTyp;

	pval = NULL;
	
	/* Typ gleich? */
	ksTyp = pvarcurrprops->value.vartype & OV_VT_KSMASK;
	if(ksTyp == OV_VT_SINGLE_VEC) {
	    pval = (OV_SINGLE*)(pvarcurrprops->value.valueunion.val_single_vec.value);
        veclen  = pvarcurrprops->value.valueunion.val_single_vec.veclen;
        
        return fb_set_dynvec_single_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);
	}
    switch(ksTyp) {
        case OV_VT_VOID:
            veclen = 0;
            break;
        case OV_VT_BYTE:
            veclen = 1;
            pval = (OV_SINGLE*)malloc(sizeof(OV_SINGLE));
            if(!pval) {
                return OV_ERR_DBOUTOFMEMORY;
            }
            pval[0] = (OV_SINGLE)pvarcurrprops->value.valueunion.val_byte;
            break;   
        case OV_VT_BOOL:
            veclen = 1;
            pval = (OV_SINGLE*)malloc(sizeof(OV_SINGLE));
            if(!pval) {
                return OV_ERR_DBOUTOFMEMORY;
            }
            pval[0] = (OV_SINGLE)pvarcurrprops->value.valueunion.val_bool;
            break;
        case OV_VT_INT: {
                veclen = 1;
                pval = (OV_SINGLE*)malloc(sizeof(OV_SINGLE));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                pval[0] = (OV_SINGLE)pvarcurrprops->value.valueunion.val_int;
            }
            break;
        case OV_VT_UINT: {
                veclen = 1;
                pval = (OV_SINGLE*)malloc(sizeof(OV_SINGLE));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                pval[0] = (OV_SINGLE)pvarcurrprops->value.valueunion.val_uint;
            }
            break;
        case OV_VT_SINGLE: {
                veclen = 1;
                pval = (OV_SINGLE*)malloc(sizeof(OV_SINGLE));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                pval[0] = pvarcurrprops->value.valueunion.val_single;
            }
            break;
        case OV_VT_DOUBLE: {
                veclen = 1;
                pval = (OV_SINGLE*)malloc(sizeof(OV_SINGLE));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                dval = pvarcurrprops->value.valueunion.val_double;
                pval[0] = (OV_SINGLE)dval;
            }
            break;
        case OV_VT_TIME: {
                veclen = 1;
                pval = (OV_SINGLE*)malloc(sizeof(OV_SINGLE));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                pval[0] = (OV_SINGLE)pvarcurrprops->value.valueunion.val_time.secs;
                pval[0] += (OV_SINGLE)(1.0*pvarcurrprops->value.valueunion.val_time.usecs / 1000000);
            }
            break;
        case OV_VT_TIME_SPAN: {
                veclen = 1;
                pval = (OV_SINGLE*)malloc(sizeof(OV_SINGLE));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                pval[0] = (OV_SINGLE)pvarcurrprops->value.valueunion.val_time_span.secs;
                pval[0] += (OV_SINGLE)(1.0*pvarcurrprops->value.valueunion.val_time_span.usecs / 1000000);
            }
            break;
        case OV_VT_STRING: {
                OV_STRING str;
                veclen = 1;
                pval = (OV_SINGLE*)malloc(sizeof(OV_SINGLE));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                str = pvarcurrprops->value.valueunion.val_string;
                if(!ov_string_compare(str,"")) {
                    pval[0] = 0;
                } else {
                    fb_fs2double(str, &dval);
                    pval[0] = (OV_SINGLE)dval;
                }
            }
            break;
        /* Vectoren */
        case OV_VT_BYTE_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_byte_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_SINGLE*)malloc(veclen*sizeof(OV_SINGLE));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = (OV_SINGLE)(((OV_BYTE*)(pvarcurrprops->value.valueunion.val_byte_vec.value))[i]);
                    }
                }
            }
            break;
        case OV_VT_BOOL_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_bool_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_SINGLE*)malloc(veclen*sizeof(OV_SINGLE));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = (OV_SINGLE)(((OV_BOOL*)(pvarcurrprops->value.valueunion.val_bool_vec.value))[i]);
                    }
                }
            }
            break;
        case OV_VT_INT_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_int_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_SINGLE*)malloc(veclen*sizeof(OV_SINGLE));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = (OV_SINGLE)(((OV_INT*)(pvarcurrprops->value.valueunion.val_int_vec.value))[i]);
                    }
                }
            }
            break;
        case OV_VT_UINT_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_uint_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_SINGLE*)malloc(veclen*sizeof(OV_SINGLE));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = ((OV_SINGLE*)(pvarcurrprops->value.valueunion.val_uint_vec.value))[i];
                    }
                }
            }
            break;
        case OV_VT_SINGLE_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_single_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_SINGLE*)malloc(veclen*sizeof(OV_SINGLE));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = ((OV_SINGLE*)(pvarcurrprops->value.valueunion.val_single_vec.value))[i];
                    }
                }
            }
            break;
        case OV_VT_DOUBLE_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_double_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_SINGLE*)malloc(veclen*sizeof(OV_SINGLE));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        dval = ((OV_DOUBLE*)(pvarcurrprops->value.valueunion.val_double_vec.value))[i];
                        pval[i] = (OV_SINGLE)dval;
                    }
                }
            }
            break;
        case OV_VT_TIME_VEC:  {
                veclen  = pvarcurrprops->value.valueunion.val_time_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_SINGLE*)malloc(veclen*sizeof(OV_SINGLE));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = (OV_SINGLE)(((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[i].secs);
                        pval[i] += (OV_SINGLE)(1.0 * (((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[i].usecs) / 1000000);
                    }
                }
            }
            break;
        case OV_VT_TIME_SPAN_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_time_span_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_SINGLE*)malloc(veclen*sizeof(OV_SINGLE));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = (OV_SINGLE)(((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[i].secs);
                        pval[i] += (OV_SINGLE)(1.0 * (((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[i].usecs) / 1000000);
                    }
                }
            }
            break;
        case OV_VT_STRING_VEC: {
                OV_STRING str;
                veclen  = pvarcurrprops->value.valueunion.val_time_span_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_SINGLE*)malloc(veclen*sizeof(OV_SINGLE));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        str = ((OV_STRING*)(pvarcurrprops->value.valueunion.val_string_vec.value))[i];
                        if(!ov_string_compare(str,"")) {
                            pval[i] = 0;
                        } else {
                            fb_fs2double(str, &dval);
                            pval[i] = (OV_SINGLE)dval;
                        }
                    }
                }
            }
            break;
        default:
	           return OV_ERR_BADTYPE;
    }

    res = fb_set_dynvec_single_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);

    /* Speicher freigeben */
    if(pval) { free(pval); }

    return res;
}
/*	======================================================================	*/
OV_RESULT fb_set_dynvec_double_varvalue_eval(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed,
	OV_DOUBLE                   *pvalue,
    OV_UINT                      veclen
) {
    OV_UINT   size;

    /* PV? */
    if(pelem->elemunion.pvar->v_vartype == OV_VT_DOUBLE_VEC) {
        /* Laenge geaendert? */
        if(	((OV_DOUBLE_VEC*)pelem->pvalue)->veclen != veclen ) {
            *changed = TRUE;
        } else {        
            /* Wert geaendert? */
            if(veclen > 0) {
                size = veclen*(sizeof(OV_DOUBLE));
                if( memcmp(pvalue, ((OV_DOUBLE_VEC*)pelem->pvalue)->value, size) ) {
                    *changed = TRUE;
                }
            }
        }
    } else {
        /* Laenge geaendert? */
        if(	((OV_DOUBLE_PV_VEC*)pelem->pvalue)->value.veclen != veclen ) {
            *changed = TRUE;
        } else {        
            /* PV-Status geaendert? */
    		if(  ((OV_DOUBLE_PV_VEC*)pelem->pvalue)->state != pvarcurrprops->state ||
    		    (((OV_DOUBLE_PV_VEC*)pelem->pvalue)->time.secs != pvarcurrprops->time.secs) ||
    		    (((OV_DOUBLE_PV_VEC*)pelem->pvalue)->time.usecs != pvarcurrprops->time.usecs) ) {
        		*changed = TRUE;
        	} else {      		
                /* Wert geaendert? */
                if(veclen > 0) {
                    size = veclen*(sizeof(OV_DOUBLE));
                    if( memcmp(pvalue, ((OV_DOUBLE_PV_VEC*)pelem->pvalue)->value.value, size) ) {
                        *changed = TRUE;
                    }
                }
            }
        }
    }
    
	/* SET-FNC vorhanden? */
    if( pelem->elemunion.pvar->v_setfnc ) {
        if(pelem->elemunion.pvar->v_vartype == OV_VT_DOUBLE_PV_VEC) {
			OV_DOUBLE_PV_VEC var;
			var.value.value = pvalue;
			var.value.veclen= veclen;
			var.state = pvarcurrprops->state;
			var.time = pvarcurrprops->time;
			return ((OV_FNCPTR_SETPVVEC(DOUBLE))pelem->elemunion.pvar->v_setfnc)(pobj, &var);
        }
        return ((OV_FNCPTR_SETVEC(DOUBLE))pelem->elemunion.pvar->v_setfnc)(pobj, pvalue, veclen);
    }
    
    if(*changed == TRUE) {
        size = veclen*(sizeof(OV_DOUBLE));
        if(pelem->elemunion.pvar->v_vartype == OV_VT_DOUBLE_VEC) {
            return  ov_vector_setdynamicvalue( (OV_GENERIC_VEC*)((OV_DOUBLE_PV_VEC*)pelem->pvalue), pvalue,veclen,size,OV_VT_DOUBLE);
        }
        
		((OV_DOUBLE_PV_VEC*)pelem->pvalue)->state = pvarcurrprops->state;
		((OV_DOUBLE_PV_VEC*)pelem->pvalue)->time = pvarcurrprops->time;
        return  ov_vector_setdynamicvalue( (OV_GENERIC_VEC*)(&(((OV_DOUBLE_PV_VEC*)pelem->pvalue)->value)), pvalue,veclen,size,OV_VT_DOUBLE);
    }

    return OV_ERR_OK;
}
/*	----------------------------------------------------------------------	*/
OV_RESULT fb_set_dynvec_double_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {
    OV_DOUBLE           *pval;
    OV_DOUBLE           dval;
    OV_UINT             i,veclen;
    OV_RESULT           res;
	OV_UINT     ksTyp;

	pval = NULL;
	
	/* Typ gleich? */
	ksTyp = pvarcurrprops->value.vartype & OV_VT_KSMASK;
	if(ksTyp == OV_VT_DOUBLE_VEC) {
	    pval = (OV_DOUBLE*)(pvarcurrprops->value.valueunion.val_double_vec.value);
        veclen  = pvarcurrprops->value.valueunion.val_double_vec.veclen;
        
        return fb_set_dynvec_double_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);
	}
	
    switch(ksTyp) {
        case OV_VT_VOID:
            veclen = 0;
            break;
        case OV_VT_BYTE:
            veclen = 1;
            pval = (OV_DOUBLE*)malloc(sizeof(OV_DOUBLE));
            if(!pval) {
                return OV_ERR_DBOUTOFMEMORY;
            }
            pval[0] = (OV_DOUBLE)pvarcurrprops->value.valueunion.val_byte;
            break;   
        case OV_VT_BOOL:
            veclen = 1;
            pval = (OV_DOUBLE*)malloc(sizeof(OV_DOUBLE));
            if(!pval) {
                return OV_ERR_DBOUTOFMEMORY;
            }
            pval[0] = (OV_DOUBLE)pvarcurrprops->value.valueunion.val_bool;
            break;
        case OV_VT_INT: {
                veclen = 1;
                pval = (OV_DOUBLE*)malloc(sizeof(OV_DOUBLE));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                pval[0] = (OV_DOUBLE)pvarcurrprops->value.valueunion.val_int;
            }
            break;
        case OV_VT_UINT: {
                veclen = 1;
                pval = (OV_DOUBLE*)malloc(sizeof(OV_DOUBLE));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                pval[0] = (OV_DOUBLE)pvarcurrprops->value.valueunion.val_uint;
            }
            break;
        case OV_VT_SINGLE: {
                veclen = 1;
                pval = (OV_DOUBLE*)malloc(sizeof(OV_DOUBLE));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                pval[0] = (OV_DOUBLE)pvarcurrprops->value.valueunion.val_single;
            }
            break;
        case OV_VT_DOUBLE: {
                veclen = 1;
                pval = (OV_DOUBLE*)malloc(sizeof(OV_DOUBLE));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                pval[0] = pvarcurrprops->value.valueunion.val_double;
            }
            break;
        case OV_VT_TIME: {
                veclen = 1;
                pval = (OV_DOUBLE*)malloc(sizeof(OV_DOUBLE));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                pval[0] = (OV_DOUBLE)pvarcurrprops->value.valueunion.val_time.secs;
                pval[0] += (OV_DOUBLE)(1.0*pvarcurrprops->value.valueunion.val_time.usecs / 1000000);
            }
            break;
        case OV_VT_TIME_SPAN: {
                veclen = 1;
                pval = (OV_DOUBLE*)malloc(sizeof(OV_DOUBLE));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                pval[0] = (OV_DOUBLE)pvarcurrprops->value.valueunion.val_time_span.secs;
                pval[0] += (OV_DOUBLE)(1.0*pvarcurrprops->value.valueunion.val_time_span.usecs / 1000000);
            }
            break;
        case OV_VT_STRING: {
                OV_STRING str;
                veclen = 1;
                pval = (OV_DOUBLE*)malloc(sizeof(OV_DOUBLE));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                str = pvarcurrprops->value.valueunion.val_string;
                if(!ov_string_compare(str,"")) {
                    pval[0] = 0;
                } else {
                    fb_fs2double(str, &dval);
                    pval[0] = dval;
                }
            }
            break;
        /* Vectoren */
        case OV_VT_BYTE_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_byte_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_DOUBLE*)malloc(veclen*sizeof(OV_DOUBLE));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = (OV_DOUBLE)(((OV_BYTE*)(pvarcurrprops->value.valueunion.val_byte_vec.value))[i]);
                    }
                }
            }
            break;
        case OV_VT_BOOL_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_bool_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_DOUBLE*)malloc(veclen*sizeof(OV_DOUBLE));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = (OV_DOUBLE)(((OV_BOOL*)(pvarcurrprops->value.valueunion.val_bool_vec.value))[i]);
                    }
                }
            }
            break;
        case OV_VT_INT_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_int_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_DOUBLE*)malloc(veclen*sizeof(OV_DOUBLE));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = (OV_DOUBLE)(((OV_INT*)(pvarcurrprops->value.valueunion.val_int_vec.value))[i]);
                    }
                }
            }
            break;
        case OV_VT_UINT_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_uint_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_DOUBLE*)malloc(veclen*sizeof(OV_DOUBLE));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = ((OV_DOUBLE*)(pvarcurrprops->value.valueunion.val_uint_vec.value))[i];
                    }
                }
            }
            break;
        case OV_VT_SINGLE_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_single_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_DOUBLE*)malloc(veclen*sizeof(OV_DOUBLE));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = (OV_DOUBLE)(((OV_SINGLE*)(pvarcurrprops->value.valueunion.val_single_vec.value))[i]);
                    }
                }
            }
            break;
        case OV_VT_DOUBLE_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_double_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_DOUBLE*)malloc(veclen*sizeof(OV_DOUBLE));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = ((OV_DOUBLE*)(pvarcurrprops->value.valueunion.val_double_vec.value))[i];
                    }
                }
            }
            break;
        case OV_VT_TIME_VEC:  {
                veclen  = pvarcurrprops->value.valueunion.val_time_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_DOUBLE*)malloc(veclen*sizeof(OV_DOUBLE));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = (OV_DOUBLE)(((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[i].secs);
                        pval[i] += (OV_DOUBLE)(1.0 * (((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[i].usecs) / 1000000);
                    }
                }
            }
            break;
        case OV_VT_TIME_SPAN_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_time_span_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_DOUBLE*)malloc(veclen*sizeof(OV_DOUBLE));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = (OV_DOUBLE)(((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[i].secs);
                        pval[i] += (OV_DOUBLE)(1.0 * (((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[i].usecs) / 1000000);
                    }
                }
            }
            break;
        case OV_VT_STRING_VEC: {
                OV_STRING str;
                veclen  = pvarcurrprops->value.valueunion.val_time_span_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_DOUBLE*)malloc(veclen*sizeof(OV_DOUBLE));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        str = ((OV_STRING*)(pvarcurrprops->value.valueunion.val_string_vec.value))[i];
                        if(!ov_string_compare(str,"")) {
                            pval[i] = 0;
                        } else {
                            fb_fs2double(str, &dval);
                            pval[i] = dval;
                        }
                    }
                }
            }
            break;
        default:
	           return OV_ERR_BADTYPE;
    }

    res = fb_set_dynvec_double_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);

    /* Speicher freigeben */
    if(pval) { free(pval); }

    return res;
}

/*	======================================================================	*/
OV_RESULT fb_set_dynvec_time_varvalue_eval(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed,
	OV_TIME                     *pvalue,
    OV_UINT                      veclen
) {
    OV_UINT   size;

    /* PV? */
    if(pelem->elemunion.pvar->v_vartype == OV_VT_TIME_VEC) {
        /* Laenge geaendert? */
        if(	((OV_TIME_VEC*)pelem->pvalue)->veclen != veclen ) {
            *changed = TRUE;
        } else {        
            /* Wert geaendert? */
            if(veclen > 0) {
                size = veclen*(sizeof(OV_TIME));
                if( memcmp(pvalue, ((OV_TIME_VEC*)pelem->pvalue)->value, size) ) {
                    *changed = TRUE;
                }
            }
        }
    } else {
        /* Laenge geaendert? */
        if(	((OV_TIME_PV_VEC*)pelem->pvalue)->value.veclen != veclen ) {
            *changed = TRUE;
        } else {        
            /* PV-Status geaendert? */
    		if(  ((OV_TIME_PV_VEC*)pelem->pvalue)->state != pvarcurrprops->state ||
    		    (((OV_TIME_PV_VEC*)pelem->pvalue)->time.secs != pvarcurrprops->time.secs) ||
    		    (((OV_TIME_PV_VEC*)pelem->pvalue)->time.usecs != pvarcurrprops->time.usecs) ) {
        		*changed = TRUE;
        	} else {      		
                /* Wert geaendert? */
                if(veclen > 0) {
                    size = veclen*(sizeof(OV_TIME));
                    if( memcmp(pvalue, ((OV_TIME_PV_VEC*)pelem->pvalue)->value.value, size) ) {
                        *changed = TRUE;
                    }
                }
            }
        }
    }
    
	/* SET-FNC vorhanden? */
    if( pelem->elemunion.pvar->v_setfnc ) {
        if(pelem->elemunion.pvar->v_vartype == OV_VT_TIME_PV_VEC) {
			OV_TIME_PV_VEC var;
			var.value.value = pvalue;
			var.value.veclen= veclen;
			var.state = pvarcurrprops->state;
			var.time = pvarcurrprops->time;
			return ((OV_FNCPTR_SETPVVEC(TIME))pelem->elemunion.pvar->v_setfnc)(pobj, &var);
        }
        return ((OV_FNCPTR_SETVEC(TIME))pelem->elemunion.pvar->v_setfnc)(pobj, pvalue, veclen);
    }
    
    if(*changed == TRUE) {
        size = veclen*(sizeof(OV_TIME));
        if(pelem->elemunion.pvar->v_vartype == OV_VT_TIME_VEC) {
            return  ov_vector_setdynamicvalue( (OV_GENERIC_VEC*)((OV_TIME_PV_VEC*)pelem->pvalue), pvalue,veclen,size,OV_VT_TIME);
        }
        
		((OV_TIME_PV_VEC*)pelem->pvalue)->state = pvarcurrprops->state;
		((OV_TIME_PV_VEC*)pelem->pvalue)->time = pvarcurrprops->time;
        return  ov_vector_setdynamicvalue( (OV_GENERIC_VEC*)(&(((OV_TIME_PV_VEC*)pelem->pvalue)->value)), pvalue,veclen,size,OV_VT_TIME);
    }

    return OV_ERR_OK;
}
/*	----------------------------------------------------------------------	*/
OV_RESULT fb_set_dynvec_time_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {
	OV_TIME             *pval;
    OV_UINT             i,veclen;
    OV_RESULT           res;

    
	OV_INT    ival;
	OV_UINT   uival;
	OV_SINGLE sval;
	OV_DOUBLE dval;
	OV_UINT     ksTyp;

	pval = NULL;
	
	/* Typ gleich? */
	ksTyp = pvarcurrprops->value.vartype & OV_VT_KSMASK;
	if(ksTyp == OV_VT_TIME_VEC) {
	    pval = (OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value);
        veclen  = pvarcurrprops->value.valueunion.val_time_vec.veclen;
        
        return fb_set_dynvec_time_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);
	}
	
    switch(ksTyp) {
        case OV_VT_VOID:
            veclen = 0;
            break;
        case OV_VT_BYTE:
            veclen = 1;
            pval = (OV_TIME*)malloc(sizeof(OV_TIME));
            if(!pval) {
                return OV_ERR_DBOUTOFMEMORY;
            }
            pval[0].secs = (OV_UINT)pvarcurrprops->value.valueunion.val_byte;
            pval[0].usecs= 0;
            break;   
        case OV_VT_BOOL:
            veclen = 1;
            pval = (OV_TIME*)malloc(sizeof(OV_TIME));
            if(!pval) {
                return OV_ERR_DBOUTOFMEMORY;
            }
            pval[0].secs = (OV_UINT)pvarcurrprops->value.valueunion.val_bool;
            pval[0].usecs= 0;
            break;
        case OV_VT_INT: {
                veclen = 1;
                pval = (OV_TIME*)malloc(sizeof(OV_TIME));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                pval[0].secs = (OV_UINT)pvarcurrprops->value.valueunion.val_int;
                pval[0].usecs= 0;
            }
            break;
        case OV_VT_UINT: {
                veclen = 1;
                pval = (OV_TIME*)malloc(sizeof(OV_TIME));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                uival = pvarcurrprops->value.valueunion.val_uint;
                pval[0].secs = uival;
                pval[0].usecs= 0;
            }
            break;
        case OV_VT_SINGLE: {
                veclen = 1;
                pval = (OV_TIME*)malloc(sizeof(OV_TIME));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                sval = pvarcurrprops->value.valueunion.val_single;
                pval[0].secs = (OV_UINT)sval;
                pval[0].usecs = (OV_UINT)( (sval - (OV_SINGLE)pval[0].secs) * 1000000);
            }
            break;
        case OV_VT_DOUBLE: {
                veclen = 1;
                pval = (OV_TIME*)malloc(sizeof(OV_TIME));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                dval = pvarcurrprops->value.valueunion.val_double;
                pval[0].secs = (OV_UINT)dval;
                pval[0].usecs = (OV_UINT)( (dval - (OV_DOUBLE)pval[0].secs) * 1000000);
            }
            break;
        case OV_VT_TIME: {
                veclen = 1;
                pval = (OV_TIME*)malloc(sizeof(OV_TIME));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                pval[0].secs = pvarcurrprops->value.valueunion.val_time.secs;
                pval[0].usecs= pvarcurrprops->value.valueunion.val_time.usecs;
            }
            break;
        case OV_VT_TIME_SPAN: {
                veclen = 1;
                pval = (OV_TIME*)malloc(sizeof(OV_TIME));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                pval[0].secs = (OV_UINT)pvarcurrprops->value.valueunion.val_time_span.secs;
                pval[0].usecs = (OV_UINT)pvarcurrprops->value.valueunion.val_time_span.usecs;
            }
            break;
        case OV_VT_STRING: {
                OV_STRING str;
                veclen = 1;
                pval = (OV_TIME*)malloc(sizeof(OV_TIME));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                str = pvarcurrprops->value.valueunion.val_string;
                if(!ov_string_compare(str,"")) {
                    pval[0].secs = 0;
                } else {
                    if(fb_ascii2time(str, &pval[0]) != OV_ERR_OK) {
                        pval[0].secs = 0;
                        pval[0].usecs = 0;
                    }
                }
            }
            break;
        /* Vectoren */
        case OV_VT_BYTE_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_byte_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_TIME*)malloc(veclen*sizeof(OV_TIME));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i].secs = (OV_UINT)(((OV_BYTE*)(pvarcurrprops->value.valueunion.val_byte_vec.value))[i]);
                        pval[i].usecs= 0;
                    }
                }
            }
            break;
        case OV_VT_BOOL_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_bool_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_TIME*)malloc(veclen*sizeof(OV_TIME));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i].secs = (OV_UINT)(((OV_BOOL*)(pvarcurrprops->value.valueunion.val_bool_vec.value))[i]);
                        pval[i].usecs= 0;
                    }
                }
            }
            break;
        case OV_VT_INT_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_int_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_TIME*)malloc(veclen*sizeof(OV_TIME));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i].secs = (OV_UINT)(((OV_INT*)(pvarcurrprops->value.valueunion.val_int_vec.value))[i]);
                        pval[i].usecs= 0;
                    }
                }
            }
            break;
        case OV_VT_UINT_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_uint_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_TIME*)malloc(veclen*sizeof(OV_TIME));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        uival = ((OV_UINT*)(pvarcurrprops->value.valueunion.val_uint_vec.value))[i];
                        pval[i].secs = (OV_UINT)uival;
                        pval[i].usecs = 0;
                    }
                }
            }
            break;
        case OV_VT_SINGLE_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_single_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_TIME*)malloc(veclen*sizeof(OV_TIME));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        sval = ((OV_SINGLE*)(pvarcurrprops->value.valueunion.val_single_vec.value))[i];
                        pval[i].secs = (OV_UINT)sval;
                        pval[i].usecs = (OV_UINT)( (sval - (OV_SINGLE)pval[i].secs) * 1000000);
                    }
                }
            }
            break;
        case OV_VT_DOUBLE_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_double_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_TIME*)malloc(veclen*sizeof(OV_TIME));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        dval = ((OV_DOUBLE*)(pvarcurrprops->value.valueunion.val_double_vec.value))[i];
                        pval[i].secs = (OV_UINT)dval;
                        pval[i].usecs = (OV_UINT)( (dval - (OV_DOUBLE)pval[i].secs) * 1000000);
                    }
                }
            }
            break;
        case OV_VT_TIME_VEC:  {
                veclen  = pvarcurrprops->value.valueunion.val_time_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_TIME*)malloc(veclen*sizeof(OV_TIME));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i].secs = ((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[i].secs;
                        pval[i].usecs = ((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[i].usecs;
                    }
                }
            }
            break;
        case OV_VT_TIME_SPAN_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_time_span_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_TIME*)malloc(veclen*sizeof(OV_TIME));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        ival = ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[i].secs;
                        pval[i].secs = (OV_UINT)ival;
                        ival = ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[i].usecs;
                        pval[i].usecs = (OV_UINT)ival;
                    }
                }
            }
            break;
        case OV_VT_STRING_VEC: {
                OV_STRING str;
                veclen  = pvarcurrprops->value.valueunion.val_time_span_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_TIME*)malloc(veclen*sizeof(OV_TIME));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        str = ((OV_STRING*)(pvarcurrprops->value.valueunion.val_string_vec.value))[i];
                        if(!ov_string_compare(str,"")) {
                            pval[i].secs = 0;
                            pval[i].usecs = 0;
                        } else {
                            if(fb_ascii2time(str, &pval[i]) != OV_ERR_OK) {
                                pval[i].secs = 0;
                                pval[i].usecs = 0;
                            }
                        }
                    }
                }
            }
            break;
        default:
	           return OV_ERR_BADTYPE;
    }

    res = fb_set_dynvec_time_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);

    /* Speicher freigeben */
    if(pval) { free(pval); }

    return res;
}

/*	======================================================================	*/
OV_RESULT fb_set_dynvec_time_span_varvalue_eval(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed,
	OV_TIME_SPAN                *pvalue,
    OV_UINT                      veclen
) {
    OV_UINT   size;

    /* PV? */
    if(pelem->elemunion.pvar->v_vartype == OV_VT_TIME_SPAN_VEC) {
        /* Laenge geaendert? */
        if(	((OV_TIME_SPAN_VEC*)pelem->pvalue)->veclen != veclen ) {
            *changed = TRUE;
        } else {        
            /* Wert geaendert? */
            if(veclen > 0) {
                size = veclen*(sizeof(OV_TIME_SPAN));
                if( memcmp(pvalue, ((OV_TIME_SPAN_VEC*)pelem->pvalue)->value, size) ) {
                    *changed = TRUE;
                }
            }
        }
    } else {
        /* Laenge geaendert? */
        if(	((OV_TIME_SPAN_PV_VEC*)pelem->pvalue)->value.veclen != veclen ) {
            *changed = TRUE;
        } else {        
            /* PV-Status geaendert? */
    		if(  ((OV_TIME_SPAN_PV_VEC*)pelem->pvalue)->state != pvarcurrprops->state ||
    		    (((OV_TIME_SPAN_PV_VEC*)pelem->pvalue)->time.secs != pvarcurrprops->time.secs) ||
    		    (((OV_TIME_SPAN_PV_VEC*)pelem->pvalue)->time.usecs != pvarcurrprops->time.usecs) ) {
        		*changed = TRUE;
        	} else {      		
                /* Wert geaendert? */
                if(veclen > 0) {
                    size = veclen*(sizeof(OV_TIME_SPAN));
                    if( memcmp(pvalue, ((OV_TIME_SPAN_PV_VEC*)pelem->pvalue)->value.value, size) ) {
                        *changed = TRUE;
                    }
                }
            }
        }
    }
    
	/* SET-FNC vorhanden? */
    if( pelem->elemunion.pvar->v_setfnc ) {
        if(pelem->elemunion.pvar->v_vartype == OV_VT_TIME_SPAN_PV_VEC) {
			OV_TIME_SPAN_PV_VEC var;
			var.value.value = pvalue;
			var.value.veclen= veclen;
			var.state = pvarcurrprops->state;
			var.time = pvarcurrprops->time;
			return ((OV_FNCPTR_SETPVVEC(TIME_SPAN))pelem->elemunion.pvar->v_setfnc)(pobj, &var);
        }
        return ((OV_FNCPTR_SETVEC(TIME_SPAN))pelem->elemunion.pvar->v_setfnc)(pobj, pvalue, veclen);
    }
    
    if(*changed == TRUE) {
        size = veclen*(sizeof(OV_TIME_SPAN));
        if(pelem->elemunion.pvar->v_vartype == OV_VT_TIME_SPAN_VEC) {
            return  ov_vector_setdynamicvalue( (OV_GENERIC_VEC*)((OV_TIME_SPAN_VEC*)pelem->pvalue), pvalue,veclen,size,OV_VT_TIME_SPAN);
        }
        
		((OV_TIME_SPAN_PV_VEC*)pelem->pvalue)->state = pvarcurrprops->state;
		((OV_TIME_SPAN_PV_VEC*)pelem->pvalue)->time = pvarcurrprops->time;
        return  ov_vector_setdynamicvalue( (OV_GENERIC_VEC*)(&(((OV_TIME_SPAN_PV_VEC*)pelem->pvalue)->value)), pvalue,veclen,size,OV_VT_TIME_SPAN);
    }

    return OV_ERR_OK;
}
/*	----------------------------------------------------------------------	*/
OV_RESULT fb_set_dynvec_time_span_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {
	OV_TIME_SPAN        *pval;
	OV_TIME_SPAN        val;
    OV_UINT             i,veclen;
    OV_RESULT           res;
    
	OV_UINT   uival;
	OV_SINGLE sval;
	OV_DOUBLE dval;
	OV_UINT     ksTyp;

	pval = NULL;
	
	/* Typ gleich? */
	ksTyp = pvarcurrprops->value.vartype & OV_VT_KSMASK;
	if(ksTyp == OV_VT_TIME_SPAN_VEC) {
	    pval = (OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value);
        veclen  = pvarcurrprops->value.valueunion.val_time_span_vec.veclen;
        
        return fb_set_dynvec_time_span_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);
	}
	
    switch(ksTyp) {
        case OV_VT_VOID:
            veclen = 0;
            break;
        case OV_VT_BYTE:
            veclen = 1;
            pval = (OV_TIME_SPAN*)malloc(sizeof(OV_TIME_SPAN));
            if(!pval) {
                return OV_ERR_DBOUTOFMEMORY;
            }
            pval[0].secs = (OV_INT)pvarcurrprops->value.valueunion.val_byte;
            pval[0].usecs= 0;
            break;   
        case OV_VT_BOOL:
            veclen = 1;
            pval = (OV_TIME_SPAN*)malloc(sizeof(OV_TIME_SPAN));
            if(!pval) {
                return OV_ERR_DBOUTOFMEMORY;
            }
            pval[0].secs = (OV_INT)pvarcurrprops->value.valueunion.val_bool;
            pval[0].usecs= 0;
            break;
        case OV_VT_INT: {
                veclen = 1;
                pval = (OV_TIME_SPAN*)malloc(sizeof(OV_TIME_SPAN));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                pval[0].secs = pvarcurrprops->value.valueunion.val_int;
                pval[0].usecs= 0;
            }
            break;
        case OV_VT_UINT: {
                veclen = 1;
                pval = (OV_TIME_SPAN*)malloc(sizeof(OV_TIME_SPAN));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                uival = pvarcurrprops->value.valueunion.val_uint;
                pval[0].secs = (OV_INT)uival;
                pval[0].usecs= 0;
            }
            break;
        case OV_VT_SINGLE: {
                veclen = 1;
                pval = (OV_TIME_SPAN*)malloc(sizeof(OV_TIME_SPAN));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                sval = pvarcurrprops->value.valueunion.val_single;
                pval[0].secs = (OV_INT)sval;
                pval[0].usecs = (OV_INT)( (sval - (OV_SINGLE)pval[0].secs) * 1000000);
            }
            break;
        case OV_VT_DOUBLE: {
                veclen = 1;
                pval = (OV_TIME_SPAN*)malloc(sizeof(OV_TIME_SPAN));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                dval = pvarcurrprops->value.valueunion.val_double;
                pval[0].secs = (OV_INT)dval;
                pval[0].usecs = (OV_INT)( (dval - (OV_DOUBLE)pval[0].secs) * 1000000);
            }
            break;
        case OV_VT_TIME: {
                veclen = 1;
                pval = (OV_TIME_SPAN*)malloc(sizeof(OV_TIME_SPAN));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                pval[0].secs = (OV_INT)pvarcurrprops->value.valueunion.val_time.secs;
                pval[0].usecs= (OV_INT)pvarcurrprops->value.valueunion.val_time.usecs;
            }
            break;
        case OV_VT_TIME_SPAN: {
                veclen = 1;
                pval = (OV_TIME_SPAN*)malloc(sizeof(OV_TIME_SPAN));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                pval[0].secs = pvarcurrprops->value.valueunion.val_time_span.secs;
                pval[0].usecs = pvarcurrprops->value.valueunion.val_time_span.usecs;
            }
            break;
        case OV_VT_STRING: {
                OV_STRING str;
                veclen = 1;
                pval = (OV_TIME_SPAN*)malloc(sizeof(OV_TIME_SPAN));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                str = pvarcurrprops->value.valueunion.val_string;
                if(!ov_string_compare(str,"")) {
                    pval[0].secs = 0;
                    pval[0].usecs = 0;
                } else {
                    if(fb_ascii2timespan(str, &val) != OV_ERR_OK) {
                        val.secs = 0;
                        val.usecs = 0;
                    }
                    pval[0].secs = val.secs;
                    pval[0].usecs = val.usecs;
                }
            }
            break;
        /* Vectoren */
        case OV_VT_BYTE_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_byte_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_TIME_SPAN*)malloc(veclen*sizeof(OV_TIME_SPAN));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i].secs = (OV_INT)(((OV_BYTE*)(pvarcurrprops->value.valueunion.val_byte_vec.value))[i]);
                        pval[i].usecs= 0;
                    }
                }
            }
            break;
        case OV_VT_BOOL_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_bool_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_TIME_SPAN*)malloc(veclen*sizeof(OV_TIME_SPAN));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i].secs = (OV_INT)(((OV_BOOL*)(pvarcurrprops->value.valueunion.val_bool_vec.value))[i]);
                        pval[i].usecs= 0;
                    }
                }
            }
            break;
        case OV_VT_INT_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_int_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_TIME_SPAN*)malloc(veclen*sizeof(OV_TIME_SPAN));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i].secs = ((OV_INT*)(pvarcurrprops->value.valueunion.val_int_vec.value))[i];
                        pval[i].usecs= 0;
                    }
                }
            }
            break;
        case OV_VT_UINT_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_uint_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_TIME_SPAN*)malloc(veclen*sizeof(OV_TIME_SPAN));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        uival = ((OV_UINT*)(pvarcurrprops->value.valueunion.val_uint_vec.value))[i];
                        pval[i].secs = (OV_INT)uival;
                        pval[i].usecs = 0;
                    }
                }
            }
            break;
        case OV_VT_SINGLE_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_single_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_TIME_SPAN*)malloc(veclen*sizeof(OV_TIME_SPAN));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        sval = ((OV_SINGLE*)(pvarcurrprops->value.valueunion.val_single_vec.value))[i];
                        pval[i].secs = (OV_INT)sval;
                        pval[i].usecs = (OV_INT)( (sval - (OV_SINGLE)pval[i].secs) * 1000000);
                    }
                }
            }
            break;
        case OV_VT_DOUBLE_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_double_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_TIME_SPAN*)malloc(veclen*sizeof(OV_TIME_SPAN));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        dval = ((OV_DOUBLE*)(pvarcurrprops->value.valueunion.val_double_vec.value))[i];
                        pval[i].secs = (OV_INT)dval;
                        pval[i].usecs = (OV_INT)( (dval - (OV_DOUBLE)pval[i].secs) * 1000000);
                    }
                }
            }
            break;
        case OV_VT_TIME_VEC:  {
                veclen  = pvarcurrprops->value.valueunion.val_time_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_TIME_SPAN*)malloc(veclen*sizeof(OV_TIME_SPAN));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i].secs = (OV_INT)(((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[i].secs);
                        pval[i].usecs = (OV_INT)(((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[i].usecs);
                    }
                }
            }
            break;
        case OV_VT_TIME_SPAN_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_time_span_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_TIME_SPAN*)malloc(veclen*sizeof(OV_TIME_SPAN));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i].secs = ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[i].secs;
                        pval[i].usecs = ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[i].usecs;
                    }
                }
            }
            break;
        case OV_VT_STRING_VEC: {
                OV_STRING str;
                veclen  = pvarcurrprops->value.valueunion.val_time_span_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_TIME_SPAN*)malloc(veclen*sizeof(OV_TIME_SPAN));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        str = ((OV_STRING*)(pvarcurrprops->value.valueunion.val_string_vec.value))[i];
                        if(!ov_string_compare(str,"")) {
                            pval[i].secs = 0;
                            pval[i].usecs = 0;
                        } else {
                            if(fb_ascii2timespan(str, &val) != OV_ERR_OK) {
                                val.secs = 0;
                                val.usecs = 0;
                            }
                            pval[i].secs =  val.secs;
                            pval[i].usecs = val.usecs;
                        }
                    }
                }
            }
            break;
        default:
	           return OV_ERR_BADTYPE;
    }

    res = fb_set_dynvec_time_span_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);

    /* Speicher freigeben */
    if(pval) { free(pval); }

    return res;
}

/*	======================================================================	*/
OV_RESULT fb_set_dynvec_string_varvalue_eval(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed,
	OV_STRING                   *pvalue,
    OV_UINT                      veclen
) {
    OV_UINT   i;
    OV_UINT   size;
    
    /* PV? */
    if(pelem->elemunion.pvar->v_vartype == OV_VT_STRING_VEC) {
        /* Laenge geaendert? */
        if(	((OV_STRING_VEC*)pelem->pvalue)->veclen != veclen ) {
            *changed = TRUE;
        } else {        
            /* Wert geaendert? */
            for(i=0; i<veclen; i++) {
                if( ov_string_compare(pvalue[i] , ((OV_STRING_VEC*)pelem->pvalue)->value[i]) ) {
                    *changed = TRUE;
                    break;                
                }
            }
        }
    } else {
        /* Laenge geaendert? */
        if(	((OV_STRING_PV_VEC*)pelem->pvalue)->value.veclen != veclen ) {
            *changed = TRUE;
        } else {        
            /* PV-Status geaendert? */
    		if(  ((OV_STRING_PV_VEC*)pelem->pvalue)->state != pvarcurrprops->state ||
    		    (((OV_STRING_PV_VEC*)pelem->pvalue)->time.secs != pvarcurrprops->time.secs) ||
    		    (((OV_STRING_PV_VEC*)pelem->pvalue)->time.usecs != pvarcurrprops->time.usecs) ) {
        		*changed = TRUE;
        	} else {      		
                /* Wert geaendert? */
                for(i=0; i<veclen; i++) {
                    if( ov_string_compare(pvalue[i] , ((OV_STRING_PV_VEC*)pelem->pvalue)->value.value[i]) ) {
                        *changed = TRUE;
                        break;                
                    }
                }
            }
        }
    }
    
	/* Wert setzen */
    if( pelem->elemunion.pvar->v_setfnc ) {
        if(pelem->elemunion.pvar->v_vartype == OV_VT_STRING_PV_VEC) {
			OV_STRING_PV_VEC var;
			var.value.value = pvalue;
			var.value.veclen= veclen;
			var.state = pvarcurrprops->state;
			var.time = pvarcurrprops->time;
			return ((OV_FNCPTR_SETPVVEC(STRING))pelem->elemunion.pvar->v_setfnc)(pobj, &var);
        }
        return ((OV_FNCPTR_SETVEC(STRING))pelem->elemunion.pvar->v_setfnc)(pobj, pvalue, veclen);
    }
    
    if(*changed == TRUE) {
        size = veclen*(sizeof(OV_STRING));
        if(pelem->elemunion.pvar->v_vartype == OV_VT_STRING_VEC) {
            return  ov_vector_setdynamicvalue( (OV_GENERIC_VEC*)((OV_STRING_VEC*)pelem->pvalue), pvalue,veclen,size,OV_VT_STRING);
        }
		((OV_STRING_PV_VEC*)pelem->pvalue)->state = pvarcurrprops->state;
		((OV_STRING_PV_VEC*)pelem->pvalue)->time = pvarcurrprops->time;
        return  ov_vector_setdynamicvalue( (OV_GENERIC_VEC*)(&(((OV_STRING_PV_VEC*)pelem->pvalue)->value)), pvalue,veclen,size,OV_VT_STRING);
    }

    return OV_ERR_OK;
}
/*	----------------------------------------------------------------------	*/
void fb_free_string_struct(OV_STRING *pval, OV_UINT veclen) {
    OV_UINT i;
    
    if(pval) {
        for(i=0; i<veclen; i++) {
            if(pval[i]) {
                free(pval[i]);
            }
        }
        free(pval);
    }
}
OV_RESULT fb_set_dynvec_string_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {
	OV_STRING           *pval;
    OV_UINT             i,veclen;
    OV_RESULT           res;
	OV_UINT     ksTyp;

	pval = NULL;
	
	/* Typ gleich? */
	ksTyp = pvarcurrprops->value.vartype & OV_VT_KSMASK;
	if(ksTyp == OV_VT_STRING_VEC) {
	    pval = (OV_STRING*)(pvarcurrprops->value.valueunion.val_string_vec.value);
        veclen  = pvarcurrprops->value.valueunion.val_string_vec.veclen;
        
        return fb_set_dynvec_string_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);
	}

    switch(ksTyp) {
        case OV_VT_VOID:
            veclen = 0;
            break;
        case OV_VT_BYTE:
            veclen = 1;
            pval = (OV_STRING*)malloc(sizeof(OV_STRING));
            if(!pval) {
                return OV_ERR_DBOUTOFMEMORY;
            }
            pval[0] = (OV_STRING)malloc(32*sizeof(char));
            if(!pval[0]) {
                fb_free_string_struct(pval, veclen);
                return OV_ERR_DBOUTOFMEMORY;
            }            
            sprintf(pval[0], "%d", pvarcurrprops->value.valueunion.val_byte);
            break;   
        case OV_VT_BOOL:
            veclen = 1;
            pval = (OV_STRING*)malloc(sizeof(OV_STRING));
            if(!pval) {
                return OV_ERR_DBOUTOFMEMORY;
            }
            pval[0] = (OV_STRING)malloc(32*sizeof(char));
            if(!pval[0]) {
                fb_free_string_struct(pval, veclen);
                return OV_ERR_DBOUTOFMEMORY;
            }            
            if(pvarcurrprops->value.valueunion.val_bool) {
                strcpy(pval[0] , "TRUE");
            } else {
                strcpy(pval[0] , "FALSE");
            }
            break;
        case OV_VT_INT: {
                veclen = 1;
                pval = (OV_STRING*)malloc(sizeof(OV_STRING));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                pval[0] = (OV_STRING)malloc(64*sizeof(char));
                if(!pval[0]) {
                    fb_free_string_struct(pval, veclen);
                    return OV_ERR_DBOUTOFMEMORY;
                }            
                sprintf(pval[0], "%ld", pvarcurrprops->value.valueunion.val_int);
            }
            break;
        case OV_VT_UINT: {
                veclen = 1;
                pval = (OV_STRING*)malloc(sizeof(OV_STRING));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                pval[0] = (OV_STRING)malloc(64*sizeof(char));
                if(!pval[0]) {
                    fb_free_string_struct(pval, veclen);
                    return OV_ERR_DBOUTOFMEMORY;
                }            
                sprintf(pval[0], "%lu", pvarcurrprops->value.valueunion.val_uint);
            }
            break;
        case OV_VT_SINGLE: {
                veclen = 1;
                pval = (OV_STRING*)malloc(sizeof(OV_STRING));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                pval[0] = (OV_STRING)malloc(64*sizeof(char));
                if(!pval[0]) {
                    fb_free_string_struct(pval, veclen);
                    return OV_ERR_DBOUTOFMEMORY;
                }            
                sprintf(pval[0], "%f", pvarcurrprops->value.valueunion.val_single);
            }
            break;
        case OV_VT_DOUBLE: {
                veclen = 1;
                pval = (OV_STRING*)malloc(sizeof(OV_STRING));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                pval[0] = (OV_STRING)malloc(64*sizeof(char));
                if(!pval[0]) {
                    fb_free_string_struct(pval, veclen);
                    return OV_ERR_DBOUTOFMEMORY;
                }            
                sprintf(pval[0], "%f", pvarcurrprops->value.valueunion.val_double);
            }
            break;
        case OV_VT_TIME: {
                OV_TIME t;
                veclen = 1;
                pval = (OV_STRING*)malloc(sizeof(OV_STRING));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                pval[0] = (OV_STRING)malloc(64*sizeof(char));
                if(!pval[0]) {
                    fb_free_string_struct(pval, veclen);
                    return OV_ERR_DBOUTOFMEMORY;
                }
                t = pvarcurrprops->value.valueunion.val_time;
                fb_time2ascii(&t, pval[0]);
            }
            break;
        case OV_VT_TIME_SPAN: {
                OV_TIME_SPAN ts;
                veclen = 1;
                pval = (OV_STRING*)malloc(sizeof(OV_STRING));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                pval[0] = (OV_STRING)malloc(64*sizeof(char));
                if(!pval[0]) {
                    fb_free_string_struct(pval, veclen);
                    return OV_ERR_DBOUTOFMEMORY;
                }
                ts = pvarcurrprops->value.valueunion.val_time_span;
                fb_timespan2ascii(&ts, pval[0]);
            }
            break;
        case OV_VT_STRING: {
                OV_STRING str;
                veclen = 1;
                pval = (OV_STRING*)malloc(sizeof(OV_STRING));
                if(!pval) {
                    return OV_ERR_DBOUTOFMEMORY;
                }
                str = pvarcurrprops->value.valueunion.val_string;
                if( !ov_string_compare(str, "") ) {
                    pval[0] = NULL;
                } else {
                    pval[0] = (OV_STRING)malloc(sizeof(char) * ov_string_getlength(str) + 1);
                    if(!pval[0]) {
                        fb_free_string_struct(pval, veclen);
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    strcpy(pval[0] , str);
                }
            }
            break;
        /* Vectoren */
        case OV_VT_BYTE_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_byte_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_STRING*)malloc(veclen*sizeof(OV_STRING));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = NULL;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = (OV_STRING)malloc(16*sizeof(char));
                        if(!pval[i]) {
                            fb_free_string_struct(pval, veclen);
                            return OV_ERR_DBOUTOFMEMORY;
                        }
                        sprintf(pval[i], "%d", ((OV_BYTE*)(pvarcurrprops->value.valueunion.val_byte_vec.value))[i]);
                    }
                }
            }
            break;
        case OV_VT_BOOL_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_bool_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_STRING*)malloc(veclen*sizeof(OV_STRING));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = NULL;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = (OV_STRING)malloc(16*sizeof(char));
                        if(!pval[i]) {
                            fb_free_string_struct(pval, veclen);
                            return OV_ERR_DBOUTOFMEMORY;
                        }
                        if(((OV_BOOL*)(pvarcurrprops->value.valueunion.val_bool_vec.value))[i]) {
                            strcpy(pval[i], "TRUE");
                        } else {
                            strcpy(pval[i], "FALSE");
                        } 
                    }
                }
            }
            break;
        case OV_VT_INT_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_int_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_STRING*)malloc(veclen*sizeof(OV_STRING));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = NULL;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = (OV_STRING)malloc(64*sizeof(char));
                        if(!pval[i]) {
                            fb_free_string_struct(pval, veclen);
                            return OV_ERR_DBOUTOFMEMORY;
                        }
                        sprintf(pval[i] , "%ld", ((OV_INT*)(pvarcurrprops->value.valueunion.val_int_vec.value))[i]);
                    }
                }
            }
            break;
        case OV_VT_UINT_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_uint_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_STRING*)malloc(veclen*sizeof(OV_STRING));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = NULL;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = (OV_STRING)malloc(64*sizeof(char));
                        if(!pval[i]) {
                            fb_free_string_struct(pval, veclen);
                            return OV_ERR_DBOUTOFMEMORY;
                        }
                        sprintf(pval[i] , "%lu", ((OV_UINT*)(pvarcurrprops->value.valueunion.val_uint_vec.value))[i]);
                    }
                }
            }
            break;
        case OV_VT_SINGLE_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_single_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_STRING*)malloc(veclen*sizeof(OV_STRING));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = NULL;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = (OV_STRING)malloc(64*sizeof(char));
                        if(!pval[i]) {
                            fb_free_string_struct(pval, veclen);
                            return OV_ERR_DBOUTOFMEMORY;
                        }
                        sprintf(pval[i] , "%f", ((OV_SINGLE*)(pvarcurrprops->value.valueunion.val_single_vec.value))[i]);
                    }
                }
            }
            break;
        case OV_VT_DOUBLE_VEC: {
                veclen  = pvarcurrprops->value.valueunion.val_double_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_STRING*)malloc(veclen*sizeof(OV_STRING));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = NULL;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = (OV_STRING)malloc(64*sizeof(char));
                        if(!pval[i]) {
                            fb_free_string_struct(pval, veclen);
                            return OV_ERR_DBOUTOFMEMORY;
                        }
                        sprintf(pval[i] , "%f", ((OV_DOUBLE*)(pvarcurrprops->value.valueunion.val_double_vec.value))[i]);
                    }
                }
            }
            break;
        case OV_VT_TIME_VEC:  {
                OV_TIME t;
                veclen  = pvarcurrprops->value.valueunion.val_time_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_STRING*)malloc(veclen*sizeof(OV_STRING));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = NULL;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = (OV_STRING)malloc(64*sizeof(char));
                        if(!pval[i]) {
                            fb_free_string_struct(pval, veclen);
                            return OV_ERR_DBOUTOFMEMORY;
                        }
                        t = ((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[i];
                        fb_time2ascii( &t , pval[i]);
                    }
                }
            }
            break;
        case OV_VT_TIME_SPAN_VEC: {
                OV_TIME_SPAN ts;
                veclen  = pvarcurrprops->value.valueunion.val_time_span_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_STRING*)malloc(veclen*sizeof(OV_STRING));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = NULL;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = (OV_STRING)malloc(64*sizeof(char));
                        if(!pval[i]) {
                            fb_free_string_struct(pval, veclen);
                            return OV_ERR_DBOUTOFMEMORY;
                        }
                        ts = ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[i];
                        fb_timespan2ascii( &ts , pval[i]);
                    }
                }
            }
            break;
        case OV_VT_STRING_VEC: {
                OV_STRING str;
                veclen  = pvarcurrprops->value.valueunion.val_time_span_vec.veclen;
                if(veclen > 0) {
                    pval = (OV_STRING*)malloc(veclen*sizeof(OV_STRING));
                    if(!pval) {
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    for(i=0; i<veclen; i++) {
                        pval[i] = NULL;
                    }
                    for(i=0; i<veclen; i++) {
                        str = ((OV_STRING*)(pvarcurrprops->value.valueunion.val_string_vec.value))[i];
                        if( ov_string_compare(str, "") ) {
                            pval[i] = (OV_STRING)malloc(sizeof(char)*ov_string_getlength(str));
                            if(!pval[i]) {
                                fb_free_string_struct(pval, veclen);
                                return OV_ERR_DBOUTOFMEMORY;
                            }
                            strcpy(pval[i], str);
                        }
                    }
                }
            }
            break;
        default:
	           return OV_ERR_BADTYPE;
    }

    res = fb_set_dynvec_string_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);
    
    /* Speicher freigeben */
    if(pval) { fb_free_string_struct(pval, veclen); }

    return res;
}


OV_RESULT fb_set_dynvec_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {
    switch(pelem->elemunion.pvar->v_vartype) {
        case OV_VT_BYTE_VEC:
    		return fb_set_dynvec_byte_varvalue(pobj, pelem, pvarcurrprops, changed);
        case OV_VT_BOOL_VEC:
        case OV_VT_BOOL_PV_VEC:
    		return fb_set_dynvec_bool_varvalue(pobj, pelem, pvarcurrprops, changed);
        case OV_VT_INT_VEC:
        case OV_VT_INT_PV_VEC:
    		return fb_set_dynvec_int_varvalue(pobj, pelem, pvarcurrprops, changed);
        case OV_VT_UINT_VEC:
        case OV_VT_UINT_PV_VEC:
    		return fb_set_dynvec_uint_varvalue(pobj, pelem, pvarcurrprops, changed);
        case OV_VT_SINGLE_VEC:
        case OV_VT_SINGLE_PV_VEC:
    		return fb_set_dynvec_single_varvalue(pobj, pelem, pvarcurrprops, changed);
        case OV_VT_DOUBLE_VEC:
        case OV_VT_DOUBLE_PV_VEC:
    		return fb_set_dynvec_double_varvalue(pobj, pelem, pvarcurrprops, changed);
        case OV_VT_TIME_VEC:
        case OV_VT_TIME_PV_VEC:
    		return fb_set_dynvec_time_varvalue(pobj, pelem, pvarcurrprops, changed);
        case OV_VT_TIME_SPAN_VEC:
        case OV_VT_TIME_SPAN_PV_VEC:
    		return fb_set_dynvec_time_span_varvalue(pobj, pelem, pvarcurrprops, changed);
    	case OV_VT_STRING_VEC :
        case OV_VT_STRING_PV_VEC:
    		return fb_set_dynvec_string_varvalue(pobj, pelem, pvarcurrprops, changed);
                    
		default:
			break;
    }

	return OV_ERR_BADTYPE;
}


/*	----------------------------------------------------------------------
*   Static vector variable
*	------------------------------------------------------------------- */
OV_RESULT fb_set_vec_byte_varvalue_eval(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed,
	OV_BYTE                     *pvalue,
	OV_UINT                      veclen
) {
    OV_UINT size;
    
    size = veclen*(sizeof(OV_BYTE));
    /* Wert geaendert? */
    if(ov_vector_compare((const OV_POINTER)(pelem->pvalue),
            (const OV_POINTER)(pvalue),
            veclen, size, OV_VT_BYTE)) {
        *changed = TRUE;
    }
    
	/* SET-FNC forhanden? */
    if( pelem->elemunion.pvar->v_setfnc ) {
        return ((OV_FNCPTR_SETVEC(BYTE))pelem->elemunion.pvar->v_setfnc)(pobj, pvalue, veclen);
    }
    
    if(*changed == TRUE) {
        return ov_vector_setstaticvalue((OV_BYTE*)pelem->pvalue,pvalue,veclen,size,OV_VT_BYTE);
    }
    
    return OV_ERR_OK;
}
/*	------------------------------------------------------------------- */
OV_RESULT fb_set_vec_byte_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {
	OV_BYTE            *pval;
    OV_UINT             i,veclen;
    OV_RESULT           res;

	OV_INT    ival;
	OV_UINT   uival;
	OV_SINGLE sval;
	OV_DOUBLE dval;
    OV_UINT   size;
	OV_UINT     ksTyp;

    veclen = pelem->elemunion.pvar->v_veclen;
	
	/* Typ gleich? */
	ksTyp = pvarcurrprops->value.vartype & OV_VT_KSMASK;
    if(ksTyp == OV_VT_BYTE_VEC) {
        /* Laenge gleich? */
        if( veclen == pvarcurrprops->value.valueunion.val_byte_vec.veclen) {
            pval = (OV_BYTE*)(pvarcurrprops->value.valueunion.val_byte_vec.value);
    	    return fb_set_vec_byte_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);
    	}
    }
    
    pval = (OV_BYTE*)malloc(veclen * sizeof(OV_BYTE));
    if(!pval) {
        return OV_ERR_DBOUTOFMEMORY;
    }
	for(i=0; i<veclen; i++) {
        pval[i] = 0;
    }
    
    switch(ksTyp) {
        case OV_VT_VOID:
            break;
        case OV_VT_BYTE:
            pval[0] = pvarcurrprops->value.valueunion.val_byte;
            break;   
        case OV_VT_BOOL:
            pval[0] = pvarcurrprops->value.valueunion.val_bool ? 1 : 0;
            break;
        case OV_VT_INT: {
                ival = pvarcurrprops->value.valueunion.val_int;
                pval[0] = (OV_BYTE)ival;
            }
            break;
        case OV_VT_UINT: {
                uival = pvarcurrprops->value.valueunion.val_uint;
                pval[0] = (OV_BYTE)uival;
            }
            break;
        case OV_VT_SINGLE: {
                sval = pvarcurrprops->value.valueunion.val_single;
                pval[0] = (OV_BYTE)sval;
            }
            break;
        case OV_VT_DOUBLE: {
                dval = pvarcurrprops->value.valueunion.val_double;
                pval[0] = (OV_BYTE)dval;
            }
            break;
        case OV_VT_TIME: {
                uival = pvarcurrprops->value.valueunion.val_time.secs;
                pval[0] = (OV_BYTE)uival;
            }
            break;
        case OV_VT_TIME_SPAN: {
                ival = pvarcurrprops->value.valueunion.val_time_span.secs;
                pval[0] = (OV_BYTE)ival;
            }
            break;
        case OV_VT_STRING: {
                if(!ov_string_compare(pvarcurrprops->value.valueunion.val_string,"")) {
                    ival = 0;
                } else {
                    ival = atol(pvarcurrprops->value.valueunion.val_string);
                }
                pval[0] = (OV_BYTE)ival;
            }
            break;
        /* Vectoren */
        case OV_VT_BYTE_VEC: {
                size = pvarcurrprops->value.valueunion.val_byte_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    pval[i] = ((OV_BYTE*)(pvarcurrprops->value.valueunion.val_byte_vec.value))[i];
                }
            }
            break;
        case OV_VT_BOOL_VEC: {
                size = pvarcurrprops->value.valueunion.val_bool_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    pval[i] = ((OV_BOOL*)(pvarcurrprops->value.valueunion.val_bool_vec.value))[i] ? 1 : 0;
                }
            }
            break;
        case OV_VT_INT_VEC: {
                size = pvarcurrprops->value.valueunion.val_int_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    ival = ((OV_INT*)(pvarcurrprops->value.valueunion.val_int_vec.value))[i];
                    pval[i] = (OV_BYTE)ival;
                }
            }
            break;
        case OV_VT_UINT_VEC: {
                size = pvarcurrprops->value.valueunion.val_uint_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    uival = ((OV_UINT*)(pvarcurrprops->value.valueunion.val_uint_vec.value))[i];
                    pval[i] = (OV_BYTE)uival;
                }
            }
            break;
        case OV_VT_SINGLE_VEC: {
                size = pvarcurrprops->value.valueunion.val_single_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    sval = ((OV_SINGLE*)(pvarcurrprops->value.valueunion.val_single_vec.value))[i];
                    pval[i] = (OV_BYTE)sval;
                }
            }
            break;
        case OV_VT_DOUBLE_VEC: {
                size = pvarcurrprops->value.valueunion.val_double_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    dval = ((OV_DOUBLE*)(pvarcurrprops->value.valueunion.val_double_vec.value))[i];
                    pval[i] = (OV_BYTE)dval;
                }
            }
            break;
        case OV_VT_TIME_VEC:  {
                size = pvarcurrprops->value.valueunion.val_time_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    uival = ((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[i].secs;
                    pval[i] = (OV_BYTE)uival;
                }
            }
            break;
        case OV_VT_TIME_SPAN_VEC: {
                size = pvarcurrprops->value.valueunion.val_time_span_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    ival = ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[i].secs;
                    pval[i] = (OV_BYTE)ival;
                }
            }
            break;
        case OV_VT_STRING_VEC: {
                OV_STRING str;
                size = pvarcurrprops->value.valueunion.val_string_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    str = ((OV_STRING*)(pvarcurrprops->value.valueunion.val_string_vec.value))[i];
                    if(!ov_string_compare(str, "")) {
                        ival = 0;
                    } else {
                        ival = (OV_INT)atol(str);
                    }
                    pval[i] = (OV_BYTE)ival;
                }
            }
            break;
        default:
	           return OV_ERR_BADTYPE;
    }

    res = fb_set_vec_byte_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);
    
    /* Speicher freigeben */
    if(pval) { free(pval); }

    return res;
}
/*	======================================================================	*/
OV_RESULT fb_set_vec_bool_varvalue_eval(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed,
	OV_BOOL                     *pvalue,
	OV_UINT                      veclen
) {
    OV_UINT size;
    
    size = veclen*(sizeof(OV_BOOL));
    /* PV? */
	if(pelem->elemunion.pvar->v_vartype == OV_VT_BOOL_VEC) {
		if( ov_vector_compare((const OV_POINTER)(pelem->pvalue),
            (const OV_POINTER)(pvalue),
            veclen,	size, OV_VT_BOOL)) {
			*changed = TRUE;
        }
    } else {
        /* PV */
        if(((OV_BOOL_PV_VEC*)pelem->pvalue)->state != pvarcurrprops->state ||
		    (((OV_BOOL_PV_VEC*)pelem->pvalue)->time.secs != pvarcurrprops->time.secs) ||
		    (((OV_BOOL_PV_VEC*)pelem->pvalue)->time.usecs != pvarcurrprops->time.usecs) ) {
    		*changed = TRUE;
    	} else {
		    if( ov_vector_compare((const OV_POINTER)((OV_BOOL_PV_VEC*)pelem->pvalue)->value.value,
                (const OV_POINTER)(pvalue),
                veclen,	size, OV_VT_BOOL)) {
    			*changed = TRUE;
            }
        }
    }

	/* SET-FNC forhanden? */
    if( pelem->elemunion.pvar->v_setfnc ) {
        if(pelem->elemunion.pvar->v_vartype == OV_VT_BOOL_PV_VEC) {
			OV_BOOL_PV_VEC var;
			var.value.value = pvalue;
			var.value.veclen= veclen;
			var.state = pvarcurrprops->state;
			var.time = pvarcurrprops->time;
			return ((OV_FNCPTR_SETPVVEC(BOOL))pelem->elemunion.pvar->v_setfnc)(pobj, &var);
        }
        return ((OV_FNCPTR_SETVEC(BOOL))pelem->elemunion.pvar->v_setfnc)(pobj, pvalue, veclen);
    }
    
    if(*changed == TRUE) {
        if(pelem->elemunion.pvar->v_vartype == OV_VT_BOOL_VEC) {
            return ov_vector_setstaticvalue((OV_BOOL*)pelem->pvalue,pvalue,veclen,size,OV_VT_BOOL);
        }
        
        ((OV_BOOL_PV_VEC*)pelem->pvalue)->state = pvarcurrprops->state;
	    ((OV_BOOL_PV_VEC*)pelem->pvalue)->time.secs = pvarcurrprops->time.secs;
	    ((OV_BOOL_PV_VEC*)pelem->pvalue)->time.usecs = pvarcurrprops->time.usecs;
        return ov_vector_setstaticvalue(((OV_BOOL_PV_VEC*)pelem->pvalue)->value.value ,pvalue,veclen,size,OV_VT_BOOL);
    }
    
    return OV_ERR_OK;
}
/*	------------------------------------------------------------------- */
OV_RESULT fb_set_vec_bool_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {
	OV_BOOL            *pval;
    OV_UINT             i,veclen;
    OV_RESULT           res;

	OV_INT    ival;
	OV_UINT   uival;
	OV_SINGLE sval;
	OV_DOUBLE dval;
    OV_UINT   size;
	OV_UINT   ksTyp;

    veclen = pelem->elemunion.pvar->v_veclen;
	
	/* Typ gleich? */
	ksTyp = pvarcurrprops->value.vartype & OV_VT_KSMASK;
    if(ksTyp == OV_VT_BOOL_VEC) {
        /* Laenge gleich? */
        if( veclen == pvarcurrprops->value.valueunion.val_bool_vec.veclen) {
            pval = (OV_BOOL*)(pvarcurrprops->value.valueunion.val_bool_vec.value);
    	    return fb_set_vec_bool_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);
    	}
    }
    
    pval = (OV_BOOL*)malloc(veclen * sizeof(OV_BOOL));
    if(!pval) {
        return OV_ERR_DBOUTOFMEMORY;
    }
	for(i=0; i<veclen; i++) {
        pval[i] = FALSE;
    }
    
    switch(ksTyp) {
        case OV_VT_VOID:
            break;
        case OV_VT_BYTE:
            pval[0] = pvarcurrprops->value.valueunion.val_byte ? TRUE : FALSE;
            break;   
        case OV_VT_BOOL:
            pval[0] = pvarcurrprops->value.valueunion.val_bool;
            break;
        case OV_VT_INT: {
                ival = pvarcurrprops->value.valueunion.val_int;
                pval[0] = (OV_BOOL)ival;
            }
            break;
        case OV_VT_UINT: {
                uival = pvarcurrprops->value.valueunion.val_uint;
                pval[0] = (OV_BOOL)uival;
            }
            break;
        case OV_VT_SINGLE: {
                sval = pvarcurrprops->value.valueunion.val_single;
                pval[0] = (OV_BOOL)sval;
            }
            break;
        case OV_VT_DOUBLE: {
                dval = pvarcurrprops->value.valueunion.val_double;
                pval[0] = (OV_BOOL)dval;
            }
            break;
        case OV_VT_TIME: {
                uival = pvarcurrprops->value.valueunion.val_time.secs;
                pval[0] = (OV_BOOL)uival;
            }
            break;
        case OV_VT_TIME_SPAN: {
                ival = pvarcurrprops->value.valueunion.val_time_span.secs;
                pval[0] = (OV_BOOL)ival;
            }
            break;
        case OV_VT_STRING: {
                OV_STRING str;
                str = pvarcurrprops->value.valueunion.val_string;
                if( !ov_string_compare(str, "") ) {
                    ival = 0;
                } else {
                    if( (!ov_string_compare(str, "TRUE")) || (!ov_string_compare(str, "true")) ) {
                        ival = 1;
                    } else {
                        ival = atol(str);
                    }
                }
                pval[0] = (OV_BOOL)ival;
            }
            break;
        /* Vectoren */
        case OV_VT_BYTE_VEC: {
                size = pvarcurrprops->value.valueunion.val_byte_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    pval[i] = ((OV_BYTE*)(pvarcurrprops->value.valueunion.val_byte_vec.value))[i] ? TRUE : FALSE;
                }
            }
            break;
        case OV_VT_BOOL_VEC: {
                size = pvarcurrprops->value.valueunion.val_bool_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    pval[i] = ((OV_BOOL*)(pvarcurrprops->value.valueunion.val_bool_vec.value))[i];
                }
            }
            break;
        case OV_VT_INT_VEC: {
                size = pvarcurrprops->value.valueunion.val_int_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    ival = ((OV_INT*)(pvarcurrprops->value.valueunion.val_int_vec.value))[i];
                    pval[i] = (OV_BOOL)ival;
                }
            }
            break;
        case OV_VT_UINT_VEC: {
                size = pvarcurrprops->value.valueunion.val_uint_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    uival = ((OV_UINT*)(pvarcurrprops->value.valueunion.val_uint_vec.value))[i];
                    pval[i] = (OV_BOOL)uival;
                }
            }
            break;
        case OV_VT_SINGLE_VEC: {
                size = pvarcurrprops->value.valueunion.val_single_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    sval = ((OV_SINGLE*)(pvarcurrprops->value.valueunion.val_single_vec.value))[i];
                    pval[i] = (OV_BOOL)sval;
                }
            }
            break;
        case OV_VT_DOUBLE_VEC: {
                size = pvarcurrprops->value.valueunion.val_double_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    dval = ((OV_DOUBLE*)(pvarcurrprops->value.valueunion.val_double_vec.value))[i];
                    pval[i] = (OV_BOOL)dval;
                }
            }
            break;
        case OV_VT_TIME_VEC:  {
                size = pvarcurrprops->value.valueunion.val_time_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    uival = ((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[i].secs;
                    pval[i] = (OV_BOOL)uival;
                }
            }
            break;
        case OV_VT_TIME_SPAN_VEC: {
                size = pvarcurrprops->value.valueunion.val_time_span_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    ival = ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[i].secs;
                    pval[i] = (OV_BOOL)ival;
                }
            }
            break;
        case OV_VT_STRING_VEC: {
                OV_STRING str;
                size = pvarcurrprops->value.valueunion.val_string_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    str = ((OV_STRING*)(pvarcurrprops->value.valueunion.val_string_vec.value))[i];
                    if(!ov_string_compare(str, "")) {
                        ival = 0;
                    } else {
                        if( (!ov_string_compare(str, "TRUE")) || (!ov_string_compare(str, "true")) ) {
                            ival = 1;
                        } else {
                            ival = (OV_INT)atol(str);
                        }
                    }
                    pval[i] = (OV_BOOL)ival;
                }
            }
            break;
        default:
	           return OV_ERR_BADTYPE;
    }

    res = fb_set_vec_bool_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);
    
    /* Speicher freigeben */
    if(pval) { free(pval); }

    return res;
}

/*	======================================================================	*/
OV_RESULT fb_set_vec_int_varvalue_eval(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed,
	OV_INT                      *pvalue,
	OV_UINT                      veclen
) {
    OV_UINT size;
    
    size = veclen*(sizeof(OV_INT));
    /* PV? */
	if(pelem->elemunion.pvar->v_vartype == OV_VT_INT_VEC) {
		if( ov_vector_compare((const OV_POINTER)(pelem->pvalue),
            (const OV_POINTER)(pvalue),
            veclen,	size, OV_VT_INT)) {
			*changed = TRUE;
        }
    } else {
        /* PV */
        if(  ((OV_INT_PV_VEC*)pelem->pvalue)->state != pvarcurrprops->state ||
		    (((OV_INT_PV_VEC*)pelem->pvalue)->time.secs != pvarcurrprops->time.secs) ||
		    (((OV_INT_PV_VEC*)pelem->pvalue)->time.usecs != pvarcurrprops->time.usecs) ) {
    		*changed = TRUE;
    	} else {
		    if( ov_vector_compare((const OV_POINTER)((OV_INT_PV_VEC*)pelem->pvalue)->value.value,
                (const OV_POINTER)(pvalue),
                veclen,	size, OV_VT_INT)) {
    			*changed = TRUE;
            }
        }
    }

	/* SET-FNC forhanden? */
    if( pelem->elemunion.pvar->v_setfnc ) {
        if(pelem->elemunion.pvar->v_vartype == OV_VT_INT_PV_VEC) {
			OV_INT_PV_VEC var;
			var.value.value = pvalue;
			var.value.veclen= veclen;
			var.state = pvarcurrprops->state;
			var.time = pvarcurrprops->time;
			return ((OV_FNCPTR_SETPVVEC(INT))pelem->elemunion.pvar->v_setfnc)(pobj, &var);
        }
        return ((OV_FNCPTR_SETVEC(INT))pelem->elemunion.pvar->v_setfnc)(pobj, pvalue, veclen);
    }
    
    if(*changed == TRUE) {
        if(pelem->elemunion.pvar->v_vartype == OV_VT_INT_VEC) {
            return ov_vector_setstaticvalue( (OV_INT*)pelem->pvalue ,pvalue,veclen,size,OV_VT_INT);
        }
        
        ((OV_INT_PV_VEC*)pelem->pvalue)->state = pvarcurrprops->state;
	    ((OV_INT_PV_VEC*)pelem->pvalue)->time.secs = pvarcurrprops->time.secs;
	    ((OV_INT_PV_VEC*)pelem->pvalue)->time.usecs = pvarcurrprops->time.usecs;
        return ov_vector_setstaticvalue( ((OV_INT_PV_VEC*)pelem->pvalue)->value.value ,pvalue,veclen,size,OV_VT_INT);
    }
    
    return OV_ERR_OK;
}
/*	------------------------------------------------------------------- */
OV_RESULT fb_set_vec_int_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {
	OV_INT             *pval;
    OV_UINT             i,veclen;
    OV_RESULT           res;

	OV_INT    ival;
	OV_UINT   uival;
	OV_SINGLE sval;
	OV_DOUBLE dval;
    OV_UINT   size;
	OV_UINT   ksTyp;

    veclen = pelem->elemunion.pvar->v_veclen;
	
	/* Typ gleich? */
	ksTyp = pvarcurrprops->value.vartype & OV_VT_KSMASK;
    if(ksTyp == OV_VT_INT_VEC) {
        /* Laenge gleich? */
        if( veclen == pvarcurrprops->value.valueunion.val_int_vec.veclen) {
            pval = (OV_INT*)(pvarcurrprops->value.valueunion.val_int_vec.value);
    	    return fb_set_vec_int_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);
    	}
    }
    
    pval = (OV_INT*)malloc(veclen * sizeof(OV_INT));
    if(!pval) {
        return OV_ERR_DBOUTOFMEMORY;
    }
	for(i=0; i<veclen; i++) {
        pval[i] = 0;
    }
    
    switch(ksTyp) {
        case OV_VT_VOID:
            break;
        case OV_VT_BYTE:
            pval[0] = (OV_INT)pvarcurrprops->value.valueunion.val_byte;
            break;   
        case OV_VT_BOOL:
            pval[0] = (OV_INT)pvarcurrprops->value.valueunion.val_bool;
            break;
        case OV_VT_INT: {
                ival = pvarcurrprops->value.valueunion.val_int;
                pval[0] = ival;
            }
            break;
        case OV_VT_UINT: {
                uival = pvarcurrprops->value.valueunion.val_uint;
                pval[0] = (OV_INT)uival;
            }
            break;
        case OV_VT_SINGLE: {
                sval = pvarcurrprops->value.valueunion.val_single;
                pval[0] = (OV_INT)sval;
            }
            break;
        case OV_VT_DOUBLE: {
                dval = pvarcurrprops->value.valueunion.val_double;
                pval[0] = (OV_INT)dval;
            }
            break;
        case OV_VT_TIME: {
                uival = pvarcurrprops->value.valueunion.val_time.secs;
                pval[0] = (OV_INT)uival;
            }
            break;
        case OV_VT_TIME_SPAN: {
                ival = pvarcurrprops->value.valueunion.val_time_span.secs;
                pval[0] = (OV_INT)ival;
            }
            break;
        case OV_VT_STRING: {
                OV_STRING str;
                str = pvarcurrprops->value.valueunion.val_string;
                if( !ov_string_compare(str, "") ) {
                    ival = 0;
                } else {
                    ival = atol(str);
                }
                pval[0] = (OV_INT)ival;
            }
            break;
        /* Vectoren */
        case OV_VT_BYTE_VEC: {
                size = pvarcurrprops->value.valueunion.val_byte_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    pval[i] = (OV_INT)(((OV_BYTE*)(pvarcurrprops->value.valueunion.val_byte_vec.value))[i]);
                }
            }
            break;
        case OV_VT_BOOL_VEC: {
                size = pvarcurrprops->value.valueunion.val_bool_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    pval[i] = (OV_INT)(((OV_BOOL*)(pvarcurrprops->value.valueunion.val_bool_vec.value))[i]);
                }
            }
            break;
        case OV_VT_INT_VEC: {
                size = pvarcurrprops->value.valueunion.val_int_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    ival = ((OV_INT*)(pvarcurrprops->value.valueunion.val_int_vec.value))[i];
                    pval[i] = ival;
                }
            }
            break;
        case OV_VT_UINT_VEC: {
                size = pvarcurrprops->value.valueunion.val_uint_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    uival = ((OV_UINT*)(pvarcurrprops->value.valueunion.val_uint_vec.value))[i];
                    pval[i] = (OV_INT)uival;
                }
            }
            break;
        case OV_VT_SINGLE_VEC: {
                size = pvarcurrprops->value.valueunion.val_single_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    sval = ((OV_SINGLE*)(pvarcurrprops->value.valueunion.val_single_vec.value))[i];
                    pval[i] = (OV_INT)sval;
                }
            }
            break;
        case OV_VT_DOUBLE_VEC: {
                size = pvarcurrprops->value.valueunion.val_double_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    dval = ((OV_DOUBLE*)(pvarcurrprops->value.valueunion.val_double_vec.value))[i];
                    pval[i] = (OV_INT)dval;
                }
            }
            break;
        case OV_VT_TIME_VEC:  {
                size = pvarcurrprops->value.valueunion.val_time_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    uival = ((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[i].secs;
                    pval[i] = (OV_INT)uival;
                }
            }
            break;
        case OV_VT_TIME_SPAN_VEC: {
                size = pvarcurrprops->value.valueunion.val_time_span_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    ival = ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[i].secs;
                    pval[i] = ival;
                }
            }
            break;
        case OV_VT_STRING_VEC: {
                OV_STRING str;
                size = pvarcurrprops->value.valueunion.val_string_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    str = ((OV_STRING*)(pvarcurrprops->value.valueunion.val_string_vec.value))[i];
                    if(!ov_string_compare(str, "")) {
                        ival = 0;
                    } else {
                        ival = (OV_INT)atol(str);
                    }
                    pval[i] = ival;
                }
            }
            break;
        default:
	           return OV_ERR_BADTYPE;
    }

    res = fb_set_vec_int_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);
    
    /* Speicher freigeben */
    if(pval) { free(pval); }

    return res;
}

/*	======================================================================	*/
OV_RESULT fb_set_vec_uint_varvalue_eval(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed,
	OV_UINT                     *pvalue,
	OV_UINT                      veclen
) {
    OV_UINT size;
    
    size = veclen*(sizeof(OV_UINT));
    /* PV? */
	if(pelem->elemunion.pvar->v_vartype == OV_VT_UINT_VEC) {
		if( ov_vector_compare((const OV_POINTER)(pelem->pvalue),
            (const OV_POINTER)(pvalue),
            veclen,	size, OV_VT_UINT)) {
			*changed = TRUE;
        }
    } else {
        /* PV */
        if(  ((OV_UINT_PV_VEC*)pelem->pvalue)->state != pvarcurrprops->state ||
		    (((OV_UINT_PV_VEC*)pelem->pvalue)->time.secs != pvarcurrprops->time.secs) ||
		    (((OV_UINT_PV_VEC*)pelem->pvalue)->time.usecs != pvarcurrprops->time.usecs) ) {
    		*changed = TRUE;
    	} else {
		    if( ov_vector_compare((const OV_POINTER)((OV_UINT_PV_VEC*)pelem->pvalue)->value.value,
                (const OV_POINTER)(pvalue),
                veclen,	size, OV_VT_UINT)) {
    			*changed = TRUE;
            }
        }
    }

	/* SET-FNC forhanden? */
    if( pelem->elemunion.pvar->v_setfnc ) {
        if(pelem->elemunion.pvar->v_vartype == OV_VT_UINT_PV_VEC) {
			OV_UINT_PV_VEC var;
			var.value.value = pvalue;
			var.value.veclen= veclen;
			var.state = pvarcurrprops->state;
			var.time = pvarcurrprops->time;
			return ((OV_FNCPTR_SETPVVEC(UINT))pelem->elemunion.pvar->v_setfnc)(pobj, &var);
        }
        return ((OV_FNCPTR_SETVEC(UINT))pelem->elemunion.pvar->v_setfnc)(pobj, pvalue, veclen);
    }
    
    if(*changed == TRUE) {
        if(pelem->elemunion.pvar->v_vartype == OV_VT_UINT_VEC) {
            return ov_vector_setstaticvalue( (OV_UINT*)pelem->pvalue ,pvalue,veclen,size,OV_VT_UINT);
        }
        
        ((OV_UINT_PV_VEC*)pelem->pvalue)->state = pvarcurrprops->state;
	    ((OV_UINT_PV_VEC*)pelem->pvalue)->time.secs = pvarcurrprops->time.secs;
	    ((OV_UINT_PV_VEC*)pelem->pvalue)->time.usecs = pvarcurrprops->time.usecs;
        return ov_vector_setstaticvalue( ((OV_UINT_PV_VEC*)pelem->pvalue)->value.value ,pvalue,veclen,size,OV_VT_UINT);
    }
    
    return OV_ERR_OK;
}
/*	------------------------------------------------------------------- */
OV_RESULT fb_set_vec_uint_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {
	OV_UINT            *pval;
    OV_UINT             i,veclen;
    OV_RESULT           res;

	OV_INT    ival;
	OV_UINT   uival;
	OV_SINGLE sval;
	OV_DOUBLE dval;
    OV_UINT   size;
	OV_UINT   ksTyp;

    veclen = pelem->elemunion.pvar->v_veclen;
	
	/* Typ gleich? */
	ksTyp = pvarcurrprops->value.vartype & OV_VT_KSMASK;
    if(ksTyp == OV_VT_UINT_VEC) {
        /* Laenge gleich? */
        if( veclen == pvarcurrprops->value.valueunion.val_uint_vec.veclen) {
            pval = (OV_UINT*)(pvarcurrprops->value.valueunion.val_uint_vec.value);
    	    return fb_set_vec_uint_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);
    	}
    }
    
    pval = (OV_UINT*)malloc(veclen * sizeof(OV_UINT));
    if(!pval) {
        return OV_ERR_DBOUTOFMEMORY;
    }
	for(i=0; i<veclen; i++) {
        pval[i] = 0;
    }
    
    switch(ksTyp) {
        case OV_VT_VOID:
            break;
        case OV_VT_BYTE:
            pval[0] = (OV_UINT)pvarcurrprops->value.valueunion.val_byte;
            break;   
        case OV_VT_BOOL:
            pval[0] = (OV_UINT)pvarcurrprops->value.valueunion.val_bool;
            break;
        case OV_VT_INT: {
                ival = pvarcurrprops->value.valueunion.val_int;
                pval[0] = (OV_UINT)ival;
            }
            break;
        case OV_VT_UINT: {
                uival = pvarcurrprops->value.valueunion.val_uint;
                pval[0] = uival;
            }
            break;
        case OV_VT_SINGLE: {
                sval = pvarcurrprops->value.valueunion.val_single;
                pval[0] = (OV_UINT)sval;
            }
            break;
        case OV_VT_DOUBLE: {
                dval = pvarcurrprops->value.valueunion.val_double;
                pval[0] = (OV_UINT)dval;
            }
            break;
        case OV_VT_TIME: {
                uival = pvarcurrprops->value.valueunion.val_time.secs;
                pval[0] = uival;
            }
            break;
        case OV_VT_TIME_SPAN: {
                ival = pvarcurrprops->value.valueunion.val_time_span.secs;
                pval[0] = (OV_UINT)ival;
            }
            break;
        case OV_VT_STRING: {
                OV_STRING str;
                str = pvarcurrprops->value.valueunion.val_string;
                if( !ov_string_compare(str, "") ) {
                    ival = 0;
                } else {
                    ival = atol(str);
                }
                pval[0] = (OV_UINT)ival;
            }
            break;
        /* Vectoren */
        case OV_VT_BYTE_VEC: {
                size = pvarcurrprops->value.valueunion.val_byte_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    pval[i] = (OV_UINT)(((OV_BYTE*)(pvarcurrprops->value.valueunion.val_byte_vec.value))[i]);
                }
            }
            break;
        case OV_VT_BOOL_VEC: {
                size = pvarcurrprops->value.valueunion.val_bool_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    pval[i] = (OV_UINT)(((OV_BOOL*)(pvarcurrprops->value.valueunion.val_bool_vec.value))[i]);
                }
            }
            break;
        case OV_VT_INT_VEC: {
                size = pvarcurrprops->value.valueunion.val_int_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    ival = ((OV_INT*)(pvarcurrprops->value.valueunion.val_int_vec.value))[i];
                    pval[i] = (OV_UINT)ival;
                }
            }
            break;
        case OV_VT_UINT_VEC: {
                size = pvarcurrprops->value.valueunion.val_uint_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    uival = ((OV_UINT*)(pvarcurrprops->value.valueunion.val_uint_vec.value))[i];
                    pval[i] = uival;
                }
            }
            break;
        case OV_VT_SINGLE_VEC: {
                size = pvarcurrprops->value.valueunion.val_single_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    sval = ((OV_SINGLE*)(pvarcurrprops->value.valueunion.val_single_vec.value))[i];
                    pval[i] = (OV_UINT)sval;
                }
            }
            break;
        case OV_VT_DOUBLE_VEC: {
                size = pvarcurrprops->value.valueunion.val_double_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    dval = ((OV_DOUBLE*)(pvarcurrprops->value.valueunion.val_double_vec.value))[i];
                    pval[i] = (OV_UINT)dval;
                }
            }
            break;
        case OV_VT_TIME_VEC:  {
                size = pvarcurrprops->value.valueunion.val_time_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    uival = ((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[i].secs;
                    pval[i] = uival;
                }
            }
            break;
        case OV_VT_TIME_SPAN_VEC: {
                size = pvarcurrprops->value.valueunion.val_time_span_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    ival = ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[i].secs;
                    pval[i] = (OV_UINT)ival;
                }
            }
            break;
        case OV_VT_STRING_VEC: {
                OV_STRING str;
                size = pvarcurrprops->value.valueunion.val_string_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    str = ((OV_STRING*)(pvarcurrprops->value.valueunion.val_string_vec.value))[i];
                    if(!ov_string_compare(str, "")) {
                        ival = 0;
                    } else {
                        ival = (OV_INT)atol(str);
                    }
                    pval[i] = (OV_UINT)ival;
                }
            }
            break;
        default:
	           return OV_ERR_BADTYPE;
    }

    res = fb_set_vec_uint_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);
    
    /* Speicher freigeben */
    if(pval) { free(pval); }

    return res;
}

/*	======================================================================	*/
OV_RESULT fb_set_vec_single_varvalue_eval(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed,
	OV_SINGLE                   *pvalue,
	OV_UINT                      veclen
) {
    OV_UINT size;
    
    size = veclen*(sizeof(OV_SINGLE));
    /* PV? */
	if(pelem->elemunion.pvar->v_vartype == OV_VT_SINGLE_VEC) {
		if( ov_vector_compare((const OV_POINTER)(pelem->pvalue),
            (const OV_POINTER)(pvalue),
            veclen,	size, OV_VT_SINGLE)) {
			*changed = TRUE;
        }
    } else {
        /* PV */
        if(  ((OV_SINGLE_PV_VEC*)pelem->pvalue)->state != pvarcurrprops->state ||
		    (((OV_SINGLE_PV_VEC*)pelem->pvalue)->time.secs != pvarcurrprops->time.secs) ||
		    (((OV_SINGLE_PV_VEC*)pelem->pvalue)->time.usecs != pvarcurrprops->time.usecs) ) {
    		*changed = TRUE;
    	} else {
		    if( ov_vector_compare((const OV_POINTER)((OV_SINGLE_PV_VEC*)pelem->pvalue)->value.value,
                (const OV_POINTER)(pvalue),
                veclen,	size, OV_VT_SINGLE)) {
    			*changed = TRUE;
            }
        }
    }

	/* SET-FNC forhanden? */
    if( pelem->elemunion.pvar->v_setfnc ) {
        if(pelem->elemunion.pvar->v_vartype == OV_VT_SINGLE_PV_VEC) {
			OV_SINGLE_PV_VEC var;
			var.value.value = pvalue;
			var.value.veclen= veclen;
			var.state = pvarcurrprops->state;
			var.time = pvarcurrprops->time;
			return ((OV_FNCPTR_SETPVVEC(SINGLE))pelem->elemunion.pvar->v_setfnc)(pobj, &var);
        }
        return ((OV_FNCPTR_SETVEC(SINGLE))pelem->elemunion.pvar->v_setfnc)(pobj, pvalue, veclen);
    }
    
    if(*changed == TRUE) {
        if(pelem->elemunion.pvar->v_vartype == OV_VT_SINGLE_VEC) {
            return ov_vector_setstaticvalue( (OV_SINGLE*)pelem->pvalue ,pvalue,veclen,size,OV_VT_SINGLE);
        }
        
        ((OV_SINGLE_PV_VEC*)pelem->pvalue)->state = pvarcurrprops->state;
	    ((OV_SINGLE_PV_VEC*)pelem->pvalue)->time.secs = pvarcurrprops->time.secs;
	    ((OV_SINGLE_PV_VEC*)pelem->pvalue)->time.usecs = pvarcurrprops->time.usecs;
        return ov_vector_setstaticvalue( ((OV_SINGLE_PV_VEC*)pelem->pvalue)->value.value ,pvalue,veclen,size,OV_VT_SINGLE);
    }
    
    return OV_ERR_OK;
}
/*	------------------------------------------------------------------- */
OV_RESULT fb_set_vec_single_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {
	OV_SINGLE           *pval;
    OV_UINT             i,veclen;
    OV_RESULT           res;

	OV_INT    ival;
	OV_UINT   uival;
	OV_SINGLE sval;
	OV_DOUBLE dval;
    OV_UINT   size;
	OV_UINT   ksTyp;

    veclen = pelem->elemunion.pvar->v_veclen;
	
	/* Typ gleich? */
	ksTyp = pvarcurrprops->value.vartype & OV_VT_KSMASK;
    if(ksTyp == OV_VT_SINGLE_VEC) {
        /* Laenge gleich? */
        if( veclen == pvarcurrprops->value.valueunion.val_single_vec.veclen) {
            pval = (OV_SINGLE*)(pvarcurrprops->value.valueunion.val_single_vec.value);
    	    return fb_set_vec_single_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);
    	}
    }
    
    pval = (OV_SINGLE*)malloc(veclen * sizeof(OV_SINGLE));
    if(!pval) {
        return OV_ERR_DBOUTOFMEMORY;
    }
	for(i=0; i<veclen; i++) {
        pval[i] = 0;
    }
    
    switch(ksTyp) {
        case OV_VT_VOID:
            break;
        case OV_VT_BYTE:
            pval[0] = (OV_SINGLE)pvarcurrprops->value.valueunion.val_byte;
            break;   
        case OV_VT_BOOL:
            pval[0] = (OV_SINGLE)pvarcurrprops->value.valueunion.val_bool;
            break;
        case OV_VT_INT: {
                ival = pvarcurrprops->value.valueunion.val_int;
                pval[0] = (OV_SINGLE)ival;
            }
            break;
        case OV_VT_UINT: {
                uival = pvarcurrprops->value.valueunion.val_uint;
                pval[0] = (OV_SINGLE)uival;
            }
            break;
        case OV_VT_SINGLE: {
                sval = pvarcurrprops->value.valueunion.val_single;
                pval[0] = sval;
            }
            break;
        case OV_VT_DOUBLE: {
                dval = pvarcurrprops->value.valueunion.val_double;
                pval[0] = (OV_SINGLE)dval;
            }
            break;
        case OV_VT_TIME: {
                uival = pvarcurrprops->value.valueunion.val_time.secs;
                pval[0] = (OV_SINGLE)uival;
                uival = pvarcurrprops->value.valueunion.val_time.usecs;
                pval[0] += (OV_SINGLE)(1.0 * uival / 1000000);
            }
            break;
        case OV_VT_TIME_SPAN: {
                ival = pvarcurrprops->value.valueunion.val_time_span.secs;
                pval[0] = (OV_SINGLE)ival;
                ival = pvarcurrprops->value.valueunion.val_time_span.usecs;
                pval[0] += (OV_SINGLE)(1.0 * ival / 1000000);
            }
            break;
        case OV_VT_STRING: {
                OV_STRING str;
                str = pvarcurrprops->value.valueunion.val_string;
                if( !ov_string_compare(str, "") ) {
                    sval = 0;
                } else {
                    fb_fs2double(str, &dval);
                    sval = (OV_SINGLE)dval;
                }
                pval[0] = sval;
            }
            break;
        /* Vectoren */
        case OV_VT_BYTE_VEC: {
                size = pvarcurrprops->value.valueunion.val_byte_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    pval[i] = (OV_SINGLE)(((OV_BYTE*)(pvarcurrprops->value.valueunion.val_byte_vec.value))[i]);
                }
            }
            break;
        case OV_VT_BOOL_VEC: {
                size = pvarcurrprops->value.valueunion.val_bool_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    pval[i] = (OV_SINGLE)(((OV_BOOL*)(pvarcurrprops->value.valueunion.val_bool_vec.value))[i]);
                }
            }
            break;
        case OV_VT_INT_VEC: {
                size = pvarcurrprops->value.valueunion.val_int_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    ival = ((OV_INT*)(pvarcurrprops->value.valueunion.val_int_vec.value))[i];
                    pval[i] = (OV_SINGLE)ival;
                }
            }
            break;
        case OV_VT_UINT_VEC: {
                size = pvarcurrprops->value.valueunion.val_uint_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    uival = ((OV_UINT*)(pvarcurrprops->value.valueunion.val_uint_vec.value))[i];
                    pval[i] = (OV_SINGLE)uival;
                }
            }
            break;
        case OV_VT_SINGLE_VEC: {
                size = pvarcurrprops->value.valueunion.val_single_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    sval = ((OV_SINGLE*)(pvarcurrprops->value.valueunion.val_single_vec.value))[i];
                    pval[i] = sval;
                }
            }
            break;
        case OV_VT_DOUBLE_VEC: {
                size = pvarcurrprops->value.valueunion.val_double_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    dval = ((OV_DOUBLE*)(pvarcurrprops->value.valueunion.val_double_vec.value))[i];
                    pval[i] = (OV_SINGLE)dval;
                }
            }
            break;
        case OV_VT_TIME_VEC:  {
                size = pvarcurrprops->value.valueunion.val_time_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    uival = ((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[i].secs;
                    pval[i] = (OV_SINGLE)uival;
                    uival = ((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[i].usecs;
                    pval[i] += (OV_SINGLE)(1.0 * uival / 1000000);
                }
            }
            break;
        case OV_VT_TIME_SPAN_VEC: {
                size = pvarcurrprops->value.valueunion.val_time_span_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    ival = ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[i].secs;
                    pval[i] = (OV_SINGLE)ival;
                    ival = ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[i].usecs;
                    pval[i] += (OV_SINGLE)(1.0 * ival / 1000000);
                }
            }
            break;
        case OV_VT_STRING_VEC: {
                OV_STRING str;
                size = pvarcurrprops->value.valueunion.val_string_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    str = ((OV_STRING*)(pvarcurrprops->value.valueunion.val_string_vec.value))[i];
                    if(!ov_string_compare(str, "")) {
                        sval = 0;
                    } else {
                        fb_fs2double(str, &dval);
                        sval = (OV_SINGLE)dval;
                    }
                    pval[i] = sval;
                }
            }
            break;
        default:
	           return OV_ERR_BADTYPE;
    }

    res = fb_set_vec_single_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);
    
    /* Speicher freigeben */
    if(pval) { free(pval); }

    return res;
}

/*	======================================================================	*/
OV_RESULT fb_set_vec_double_varvalue_eval(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed,
	OV_DOUBLE                   *pvalue,
	OV_UINT                      veclen
) {
    OV_UINT size;
    
    size = veclen*(sizeof(OV_DOUBLE));
    /* PV? */
	if(pelem->elemunion.pvar->v_vartype == OV_VT_DOUBLE_VEC) {
		if( ov_vector_compare((const OV_POINTER)(pelem->pvalue),
            (const OV_POINTER)(pvalue),
            veclen,	size, OV_VT_DOUBLE)) {
			*changed = TRUE;
        }
    } else {
        /* PV */
        if(  ((OV_DOUBLE_PV_VEC*)pelem->pvalue)->state != pvarcurrprops->state ||
		    (((OV_DOUBLE_PV_VEC*)pelem->pvalue)->time.secs != pvarcurrprops->time.secs) ||
		    (((OV_DOUBLE_PV_VEC*)pelem->pvalue)->time.usecs != pvarcurrprops->time.usecs) ) {
    		*changed = TRUE;
    	} else {
		    if( ov_vector_compare((const OV_POINTER)((OV_DOUBLE_PV_VEC*)pelem->pvalue)->value.value,
                (const OV_POINTER)(pvalue),
                veclen,	size, OV_VT_DOUBLE)) {
    			*changed = TRUE;
            }
        }
    }

	/* SET-FNC forhanden? */
    if( pelem->elemunion.pvar->v_setfnc ) {
        if(pelem->elemunion.pvar->v_vartype == OV_VT_DOUBLE_PV_VEC) {
			OV_DOUBLE_PV_VEC var;
			var.value.value = pvalue;
			var.value.veclen= veclen;
			var.state = pvarcurrprops->state;
			var.time = pvarcurrprops->time;
			return ((OV_FNCPTR_SETPVVEC(DOUBLE))pelem->elemunion.pvar->v_setfnc)(pobj, &var);
        }
        return ((OV_FNCPTR_SETVEC(DOUBLE))pelem->elemunion.pvar->v_setfnc)(pobj, pvalue, veclen);
    }
    
    if(*changed == TRUE) {
        if(pelem->elemunion.pvar->v_vartype == OV_VT_DOUBLE_VEC) {
            return ov_vector_setstaticvalue( (OV_DOUBLE*)pelem->pvalue ,pvalue,veclen,size,OV_VT_DOUBLE);
        }
        
        ((OV_DOUBLE_PV_VEC*)pelem->pvalue)->state = pvarcurrprops->state;
	    ((OV_DOUBLE_PV_VEC*)pelem->pvalue)->time.secs = pvarcurrprops->time.secs;
	    ((OV_DOUBLE_PV_VEC*)pelem->pvalue)->time.usecs = pvarcurrprops->time.usecs;
        return ov_vector_setstaticvalue( ((OV_DOUBLE_PV_VEC*)pelem->pvalue)->value.value ,pvalue,veclen,size,OV_VT_DOUBLE);
    }
    
    return OV_ERR_OK;
}
/*	------------------------------------------------------------------- */
OV_RESULT fb_set_vec_double_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {
	OV_DOUBLE           *pval;
    OV_UINT             i,veclen;
    OV_RESULT           res;

	OV_INT    ival;
	OV_UINT   uival;
	OV_SINGLE sval;
	OV_DOUBLE dval;
    OV_UINT   size;
	OV_UINT   ksTyp;

    veclen = pelem->elemunion.pvar->v_veclen;
	
	/* Typ gleich? */
	ksTyp = pvarcurrprops->value.vartype & OV_VT_KSMASK;
    if(ksTyp == OV_VT_DOUBLE_VEC) {
        /* Laenge gleich? */
        if( veclen == pvarcurrprops->value.valueunion.val_double_vec.veclen) {
            pval = (OV_DOUBLE*)(pvarcurrprops->value.valueunion.val_double_vec.value);
    	    return fb_set_vec_double_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);
    	}
    }
    
    pval = (OV_DOUBLE*)malloc(veclen * sizeof(OV_DOUBLE));
    if(!pval) {
        return OV_ERR_DBOUTOFMEMORY;
    }
	for(i=0; i<veclen; i++) {
        pval[i] = 0;
    }
    
    switch(ksTyp) {
        case OV_VT_VOID:
            break;
        case OV_VT_BYTE:
            pval[0] = (OV_DOUBLE)pvarcurrprops->value.valueunion.val_byte;
            break;   
        case OV_VT_BOOL:
            pval[0] = (OV_DOUBLE)pvarcurrprops->value.valueunion.val_bool;
            break;
        case OV_VT_INT: {
                ival = pvarcurrprops->value.valueunion.val_int;
                pval[0] = (OV_DOUBLE)ival;
            }
            break;
        case OV_VT_UINT: {
                uival = pvarcurrprops->value.valueunion.val_uint;
                pval[0] = (OV_DOUBLE)uival;
            }
            break;
        case OV_VT_SINGLE: {
                sval = pvarcurrprops->value.valueunion.val_single;
                pval[0] = (OV_DOUBLE)sval;
            }
            break;
        case OV_VT_DOUBLE: {
                dval = pvarcurrprops->value.valueunion.val_double;
                pval[0] = dval;
            }
            break;
        case OV_VT_TIME: {
                uival = pvarcurrprops->value.valueunion.val_time.secs;
                pval[0] = (OV_DOUBLE)uival;
                uival = pvarcurrprops->value.valueunion.val_time.usecs;
                pval[0] += (OV_DOUBLE)(1.0 * uival / 1000000);
            }
            break;
        case OV_VT_TIME_SPAN: {
                ival = pvarcurrprops->value.valueunion.val_time_span.secs;
                pval[0] = (OV_DOUBLE)ival;
                ival = pvarcurrprops->value.valueunion.val_time_span.usecs;
                pval[0] = (OV_DOUBLE)(1.0 * ival / 1000000);
            }
            break;
        case OV_VT_STRING: {
                OV_STRING str;
                str = pvarcurrprops->value.valueunion.val_string;
                if( !ov_string_compare(str, "") ) {
                    dval = 0;
                } else {
                    fb_fs2double(str, &dval);
                }
                pval[0] = dval;
            }
            break;
        /* Vectoren */
        case OV_VT_BYTE_VEC: {
                size = pvarcurrprops->value.valueunion.val_byte_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    pval[i] = (OV_DOUBLE)(((OV_BYTE*)(pvarcurrprops->value.valueunion.val_byte_vec.value))[i]);
                }
            }
            break;
        case OV_VT_BOOL_VEC: {
                size = pvarcurrprops->value.valueunion.val_bool_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    pval[i] = (OV_DOUBLE)(((OV_BOOL*)(pvarcurrprops->value.valueunion.val_bool_vec.value))[i]);
                }
            }
            break;
        case OV_VT_INT_VEC: {
                size = pvarcurrprops->value.valueunion.val_int_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    ival = ((OV_INT*)(pvarcurrprops->value.valueunion.val_int_vec.value))[i];
                    pval[i] = (OV_DOUBLE)ival;
                }
            }
            break;
        case OV_VT_UINT_VEC: {
                size = pvarcurrprops->value.valueunion.val_uint_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    uival = ((OV_UINT*)(pvarcurrprops->value.valueunion.val_uint_vec.value))[i];
                    pval[i] = (OV_DOUBLE)uival;
                }
            }
            break;
        case OV_VT_SINGLE_VEC: {
                size = pvarcurrprops->value.valueunion.val_single_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    sval = ((OV_SINGLE*)(pvarcurrprops->value.valueunion.val_single_vec.value))[i];
                    pval[i] = (OV_DOUBLE)sval;
                }
            }
            break;
        case OV_VT_DOUBLE_VEC: {
                size = pvarcurrprops->value.valueunion.val_double_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    dval = ((OV_DOUBLE*)(pvarcurrprops->value.valueunion.val_double_vec.value))[i];
                    pval[i] = dval;
                }
            }
            break;
        case OV_VT_TIME_VEC:  {
                size = pvarcurrprops->value.valueunion.val_time_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    uival = ((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[i].secs;
                    pval[i] = (OV_DOUBLE)uival;
                    uival = ((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[i].usecs;
                    pval[i] += (OV_DOUBLE)(1.0 * uival / 1000000);
                }
            }
            break;
        case OV_VT_TIME_SPAN_VEC: {
                size = pvarcurrprops->value.valueunion.val_time_span_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    ival = ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[i].secs;
                    pval[i] = (OV_DOUBLE)ival;
                    ival = ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[i].usecs;
                    pval[i] = (OV_DOUBLE)(1.0 * ival / 1000000);
                }
            }
            break;
        case OV_VT_STRING_VEC: {
                OV_STRING str;
                size = pvarcurrprops->value.valueunion.val_string_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    str = ((OV_STRING*)(pvarcurrprops->value.valueunion.val_string_vec.value))[i];
                    if(!ov_string_compare(str, "")) {
                        dval = 0;
                    } else {
                        fb_fs2double(str, &dval);
                    }
                    pval[i] = dval;
                }
            }
            break;
        default:
	           return OV_ERR_BADTYPE;
    }

    res = fb_set_vec_double_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);
    
    /* Speicher freigeben */
    if(pval) { free(pval); }

    return res;
}

/*	======================================================================	*/
OV_RESULT fb_set_vec_time_varvalue_eval(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed,
	OV_TIME                     *pvalue,
	OV_UINT                      veclen
) {
    OV_UINT size;
    
    size = veclen*(sizeof(OV_TIME));
    /* PV? */
	if(pelem->elemunion.pvar->v_vartype == OV_VT_TIME_VEC) {
		if( ov_vector_compare((const OV_POINTER)(pelem->pvalue),
            (const OV_POINTER)(pvalue),
            veclen,	size, OV_VT_TIME)) {
			*changed = TRUE;
        }
    } else {
        /* PV */
        if(  ((OV_TIME_PV_VEC*)pelem->pvalue)->state != pvarcurrprops->state ||
		    (((OV_TIME_PV_VEC*)pelem->pvalue)->time.secs != pvarcurrprops->time.secs) ||
		    (((OV_TIME_PV_VEC*)pelem->pvalue)->time.usecs != pvarcurrprops->time.usecs) ) {
    		*changed = TRUE;
    	} else {
		    if( ov_vector_compare((const OV_POINTER)((OV_TIME_PV_VEC*)pelem->pvalue)->value.value,
                (const OV_POINTER)(pvalue),
                veclen,	size, OV_VT_TIME)) {
    			*changed = TRUE;
            }
        }
    }

	/* SET-FNC forhanden? */
    if( pelem->elemunion.pvar->v_setfnc ) {
        if(pelem->elemunion.pvar->v_vartype == OV_VT_TIME_PV_VEC) {
			OV_TIME_PV_VEC var;
			var.value.value = pvalue;
			var.value.veclen= veclen;
			var.state = pvarcurrprops->state;
			var.time = pvarcurrprops->time;
			return ((OV_FNCPTR_SETPVVEC(TIME))pelem->elemunion.pvar->v_setfnc)(pobj, &var);
        }
        return ((OV_FNCPTR_SETVEC(TIME))pelem->elemunion.pvar->v_setfnc)(pobj, pvalue, veclen);
    }
    
    if(*changed == TRUE) {
        if(pelem->elemunion.pvar->v_vartype == OV_VT_TIME_VEC) {
            return ov_vector_setstaticvalue( (OV_TIME*)pelem->pvalue ,pvalue,veclen,size,OV_VT_TIME);
        }
        
        ((OV_TIME_PV_VEC*)pelem->pvalue)->state = pvarcurrprops->state;
	    ((OV_TIME_PV_VEC*)pelem->pvalue)->time.secs = pvarcurrprops->time.secs;
	    ((OV_TIME_PV_VEC*)pelem->pvalue)->time.usecs = pvarcurrprops->time.usecs;
        return ov_vector_setstaticvalue( ((OV_TIME_PV_VEC*)pelem->pvalue)->value.value ,pvalue,veclen,size,OV_VT_TIME);
    }
    
    return OV_ERR_OK;
}
/*	------------------------------------------------------------------- */
OV_RESULT fb_set_vec_time_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {
	OV_TIME            *pval;
    OV_UINT             i,veclen;
    OV_RESULT           res;

	OV_INT    ival;
	OV_UINT   uival;
	OV_SINGLE sval;
	OV_DOUBLE dval;
    OV_UINT   size;
	OV_UINT   ksTyp;

    veclen = pelem->elemunion.pvar->v_veclen;
	
	/* Typ gleich? */
	ksTyp = pvarcurrprops->value.vartype & OV_VT_KSMASK;
    if(ksTyp == OV_VT_TIME_VEC) {
        /* Laenge gleich? */
        if( veclen == pvarcurrprops->value.valueunion.val_time_vec.veclen) {
            pval = (OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value);
    	    return fb_set_vec_time_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);
    	}
    }
    
    pval = (OV_TIME*)malloc(veclen * sizeof(OV_TIME));
    if(!pval) {
        return OV_ERR_DBOUTOFMEMORY;
    }
	for(i=0; i<veclen; i++) {
        pval[i].secs = 0;
        pval[i].usecs = 0;
    }
    
    switch(ksTyp) {
        case OV_VT_VOID:
            break;
        case OV_VT_BYTE:
            pval[0].secs = (OV_UINT)pvarcurrprops->value.valueunion.val_byte;
            break;   
        case OV_VT_BOOL:
            pval[0].secs = (OV_UINT)pvarcurrprops->value.valueunion.val_bool;
            break;
        case OV_VT_INT: {
                ival = pvarcurrprops->value.valueunion.val_int;
                pval[0].secs = (OV_UINT)ival;
            }
            break;
        case OV_VT_UINT: {
                uival = pvarcurrprops->value.valueunion.val_uint;
                pval[0].secs = uival;
            }
            break;
        case OV_VT_SINGLE: {
                sval = pvarcurrprops->value.valueunion.val_single;
                pval[0].secs = (OV_UINT)sval;
                pval[0].usecs = (OV_UINT)( (sval - (OV_SINGLE)pval[0].secs) * 1000000);
            }
            break;
        case OV_VT_DOUBLE: {
                dval = pvarcurrprops->value.valueunion.val_double;
                pval[0].secs = (OV_UINT)dval;
                pval[0].usecs = (OV_UINT)( (dval - (OV_DOUBLE)pval[0].secs) * 1000000);
            }
            break;
        case OV_VT_TIME: {
                pval[0].secs = pvarcurrprops->value.valueunion.val_time.secs;
                pval[0].usecs = pvarcurrprops->value.valueunion.val_time.usecs;
            }
            break;
        case OV_VT_TIME_SPAN: {
                ival = pvarcurrprops->value.valueunion.val_time_span.secs;
                pval[0].secs = (OV_UINT)ival;
                ival = pvarcurrprops->value.valueunion.val_time_span.usecs;
                pval[0].usecs = (OV_UINT)(1.0 * ival / 1000000);
            }
            break;
        case OV_VT_STRING: {
                OV_STRING str;
                str = pvarcurrprops->value.valueunion.val_string;
                if( !ov_string_compare(str, "") ) {
                    pval[0].secs = 0;
                } else {
                    OV_TIME t;
                    fb_ascii2time(str, &t);
                    pval[0].secs = t.secs;
                    pval[0].usecs = t.usecs;
                }
            }
            break;
        /* Vectoren */
        case OV_VT_BYTE_VEC: {
                size = pvarcurrprops->value.valueunion.val_byte_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    pval[i].secs = (OV_UINT)(((OV_BYTE*)(pvarcurrprops->value.valueunion.val_byte_vec.value))[i]);
                }
            }
            break;
        case OV_VT_BOOL_VEC: {
                size = pvarcurrprops->value.valueunion.val_bool_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    pval[i].secs = (OV_UINT)(((OV_BOOL*)(pvarcurrprops->value.valueunion.val_bool_vec.value))[i]);
                }
            }
            break;
        case OV_VT_INT_VEC: {
                size = pvarcurrprops->value.valueunion.val_int_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    ival = ((OV_INT*)(pvarcurrprops->value.valueunion.val_int_vec.value))[i];
                    pval[i].secs = (OV_UINT)ival;
                }
            }
            break;
        case OV_VT_UINT_VEC: {
                size = pvarcurrprops->value.valueunion.val_uint_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    uival = ((OV_UINT*)(pvarcurrprops->value.valueunion.val_uint_vec.value))[i];
                    pval[i].secs = uival;
                }
            }
            break;
        case OV_VT_SINGLE_VEC: {
                size = pvarcurrprops->value.valueunion.val_single_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    sval = ((OV_SINGLE*)(pvarcurrprops->value.valueunion.val_single_vec.value))[i];
                    pval[i].secs = (OV_UINT)sval;
                    pval[i].usecs = (OV_UINT)( (sval - (OV_SINGLE)pval[i].secs) * 1000000);
                }
            }
            break;
        case OV_VT_DOUBLE_VEC: {
                size = pvarcurrprops->value.valueunion.val_double_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    dval = ((OV_DOUBLE*)(pvarcurrprops->value.valueunion.val_double_vec.value))[i];
                    pval[i].secs = (OV_UINT)dval;
                    pval[i].usecs = (OV_UINT)( (dval - (OV_DOUBLE)pval[i].secs) * 1000000);
                }
            }
            break;
        case OV_VT_TIME_VEC:  {
                size = pvarcurrprops->value.valueunion.val_time_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    pval[i].secs = ((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[i].secs;
                    pval[i].usecs = ((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[i].usecs;
                }
            }
            break;
        case OV_VT_TIME_SPAN_VEC: {
                size = pvarcurrprops->value.valueunion.val_time_span_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    ival = ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[i].secs;
                    pval[i].secs = (OV_UINT)ival;
                    ival = ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[i].usecs;
                    pval[i].usecs = (OV_UINT)ival;
                }
            }
            break;
        case OV_VT_STRING_VEC: {
                OV_STRING str;
                size = pvarcurrprops->value.valueunion.val_string_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    str = ((OV_STRING*)(pvarcurrprops->value.valueunion.val_string_vec.value))[i];
                    if(!ov_string_compare(str, "")) {
                        pval[i].secs = 0;
                    } else {
                        OV_TIME t;
                        fb_ascii2time(str, &t);
                        pval[i].secs = t.secs;
                        pval[i].usecs = t.usecs;
                    }
                }
            }
            break;
        default:
	           return OV_ERR_BADTYPE;
    }

    res = fb_set_vec_time_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);
    
    /* Speicher freigeben */
    if(pval) { free(pval); }

    return res;
}

/*	======================================================================	*/
OV_RESULT fb_set_vec_time_span_varvalue_eval(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed,
	OV_TIME_SPAN                *pvalue,
	OV_UINT                      veclen
) {
    OV_UINT size;
    
    size = veclen*(sizeof(OV_TIME_SPAN));
    /* PV? */
	if(pelem->elemunion.pvar->v_vartype == OV_VT_TIME_SPAN_VEC) {
		if( ov_vector_compare((const OV_POINTER)(pelem->pvalue),
            (const OV_POINTER)(pvalue),
            veclen,	size, OV_VT_TIME_SPAN)) {
			*changed = TRUE;
        }
    } else {
        /* PV */
        if(  ((OV_TIME_SPAN_PV_VEC*)pelem->pvalue)->state != pvarcurrprops->state ||
		    (((OV_TIME_SPAN_PV_VEC*)pelem->pvalue)->time.secs != pvarcurrprops->time.secs) ||
		    (((OV_TIME_SPAN_PV_VEC*)pelem->pvalue)->time.usecs != pvarcurrprops->time.usecs) ) {
    		*changed = TRUE;
    	} else {
		    if( ov_vector_compare((const OV_POINTER)((OV_TIME_SPAN_PV_VEC*)pelem->pvalue)->value.value,
                (const OV_POINTER)(pvalue),
                veclen,	size, OV_VT_TIME_SPAN)) {
    			*changed = TRUE;
            }
        }
    }

	/* SET-FNC forhanden? */
    if( pelem->elemunion.pvar->v_setfnc ) {
        if(pelem->elemunion.pvar->v_vartype == OV_VT_TIME_SPAN_PV_VEC) {
			OV_TIME_SPAN_PV_VEC var;
			var.value.value = pvalue;
			var.value.veclen= veclen;
			var.state = pvarcurrprops->state;
			var.time = pvarcurrprops->time;
			return ((OV_FNCPTR_SETPVVEC(TIME_SPAN))pelem->elemunion.pvar->v_setfnc)(pobj, &var);
        }
        return ((OV_FNCPTR_SETVEC(TIME_SPAN))pelem->elemunion.pvar->v_setfnc)(pobj, pvalue, veclen);
    }
    
    if(*changed == TRUE) {
        if(pelem->elemunion.pvar->v_vartype == OV_VT_TIME_SPAN_VEC) {
            return ov_vector_setstaticvalue( (OV_TIME_SPAN*)pelem->pvalue ,pvalue,veclen,size,OV_VT_TIME_SPAN);
        }
        
        ((OV_TIME_SPAN_PV_VEC*)pelem->pvalue)->state = pvarcurrprops->state;
	    ((OV_TIME_SPAN_PV_VEC*)pelem->pvalue)->time.secs = pvarcurrprops->time.secs;
	    ((OV_TIME_SPAN_PV_VEC*)pelem->pvalue)->time.usecs = pvarcurrprops->time.usecs;
        return ov_vector_setstaticvalue( ((OV_TIME_SPAN_PV_VEC*)pelem->pvalue)->value.value ,pvalue,veclen,size,OV_VT_TIME_SPAN);
    }
    
    return OV_ERR_OK;
}
/*	------------------------------------------------------------------- */
OV_RESULT fb_set_vec_time_span_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {
	OV_TIME_SPAN       *pval;
    OV_UINT             i,veclen;
    OV_RESULT           res;

	OV_INT    ival;
	OV_UINT   uival;
	OV_SINGLE sval;
	OV_DOUBLE dval;
    OV_UINT   size;
	OV_UINT   ksTyp;

    veclen = pelem->elemunion.pvar->v_veclen;
	
	/* Typ gleich? */
	ksTyp = pvarcurrprops->value.vartype & OV_VT_KSMASK;
    if(ksTyp == OV_VT_TIME_SPAN_VEC) {
        /* Laenge gleich? */
        if( veclen == pvarcurrprops->value.valueunion.val_time_span_vec.veclen) {
            pval = (OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value);
    	    return fb_set_vec_time_span_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);
    	}
    }
    
    pval = (OV_TIME_SPAN*)malloc(veclen * sizeof(OV_TIME_SPAN));
    if(!pval) {
        return OV_ERR_DBOUTOFMEMORY;
    }
	for(i=0; i<veclen; i++) {
        pval[i].secs = 0;
        pval[i].usecs = 0;
    }
    
    switch(ksTyp) {
        case OV_VT_VOID:
            break;
        case OV_VT_BYTE:
            pval[0].secs = (OV_INT)pvarcurrprops->value.valueunion.val_byte;
            break;   
        case OV_VT_BOOL:
            pval[0].secs = (OV_INT)pvarcurrprops->value.valueunion.val_bool;
            break;
        case OV_VT_INT: {
                ival = pvarcurrprops->value.valueunion.val_int;
                pval[0].secs = ival;
            }
            break;
        case OV_VT_UINT: {
                uival = pvarcurrprops->value.valueunion.val_uint;
                pval[0].secs = (OV_INT)uival;
            }
            break;
        case OV_VT_SINGLE: {
                sval = pvarcurrprops->value.valueunion.val_single;
                pval[0].secs = (OV_INT)sval;
                pval[0].usecs = (OV_INT)( (sval - (OV_SINGLE)pval[0].secs) * 1000000);
            }
            break;
        case OV_VT_DOUBLE: {
                dval = pvarcurrprops->value.valueunion.val_double;
                pval[0].secs = (OV_INT)dval;
                pval[0].usecs = (OV_INT)( (dval - (OV_DOUBLE)pval[0].secs) * 1000000);
            }
            break;
        case OV_VT_TIME: {
                pval[0].secs = (OV_INT)pvarcurrprops->value.valueunion.val_time.secs;
                pval[0].usecs = (OV_INT)pvarcurrprops->value.valueunion.val_time.usecs;
            }
            break;
        case OV_VT_TIME_SPAN: {
                pval[0].secs = pvarcurrprops->value.valueunion.val_time_span.secs;
                pval[0].usecs = pvarcurrprops->value.valueunion.val_time_span.usecs;
            }
            break;
        case OV_VT_STRING: {
                OV_STRING str;
                str = pvarcurrprops->value.valueunion.val_string;
                if( !ov_string_compare(str, "") ) {
                    pval[0].secs = 0;
                } else {
                    OV_TIME_SPAN t;
                    fb_ascii2timespan(str, &t);
                    pval[0].secs = t.secs;
                    pval[0].usecs = t.usecs;
                }
            }
            break;
        /* Vectoren */
        case OV_VT_BYTE_VEC: {
                size = pvarcurrprops->value.valueunion.val_byte_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    pval[i].secs = (OV_INT)(((OV_BYTE*)(pvarcurrprops->value.valueunion.val_byte_vec.value))[i]);
                }
            }
            break;
        case OV_VT_BOOL_VEC: {
                size = pvarcurrprops->value.valueunion.val_bool_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    pval[i].secs = (OV_INT)(((OV_BOOL*)(pvarcurrprops->value.valueunion.val_bool_vec.value))[i]);
                }
            }
            break;
        case OV_VT_INT_VEC: {
                size = pvarcurrprops->value.valueunion.val_int_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    ival = ((OV_INT*)(pvarcurrprops->value.valueunion.val_int_vec.value))[i];
                    pval[i].secs = ival;
                }
            }
            break;
        case OV_VT_UINT_VEC: {
                size = pvarcurrprops->value.valueunion.val_uint_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    uival = ((OV_UINT*)(pvarcurrprops->value.valueunion.val_uint_vec.value))[i];
                    pval[i].secs = (OV_INT)uival;
                }
            }
            break;
        case OV_VT_SINGLE_VEC: {
                size = pvarcurrprops->value.valueunion.val_single_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    sval = ((OV_SINGLE*)(pvarcurrprops->value.valueunion.val_single_vec.value))[i];
                    pval[i].secs = (OV_INT)sval;
                    pval[i].usecs = (OV_INT)( (sval - (OV_SINGLE)pval[i].secs) * 1000000);
                }
            }
            break;
        case OV_VT_DOUBLE_VEC: {
                size = pvarcurrprops->value.valueunion.val_double_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    dval = ((OV_DOUBLE*)(pvarcurrprops->value.valueunion.val_double_vec.value))[i];
                    pval[i].secs = (OV_INT)dval;
                    pval[i].usecs = (OV_INT)( (dval - (OV_DOUBLE)pval[i].secs) * 1000000);
                }
            }
            break;
        case OV_VT_TIME_VEC:  {
                size = pvarcurrprops->value.valueunion.val_time_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    pval[i].secs = (OV_INT)(((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[i].secs);
                    pval[i].usecs = (OV_INT)(((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[i].usecs);
                }
            }
            break;
        case OV_VT_TIME_SPAN_VEC: {
                size = pvarcurrprops->value.valueunion.val_time_span_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    ival = ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[i].secs;
                    pval[i].secs = ival;
                    ival = ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[i].usecs;
                    pval[i].usecs = ival;
                }
            }
            break;
        case OV_VT_STRING_VEC: {
                OV_STRING str;
                size = pvarcurrprops->value.valueunion.val_string_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    str = ((OV_STRING*)(pvarcurrprops->value.valueunion.val_string_vec.value))[i];
                    if(!ov_string_compare(str, "")) {
                        pval[i].secs = 0;
                    } else {
                        OV_TIME_SPAN t;
                        fb_ascii2timespan(str, &t);
                        pval[i].secs = t.secs;
                        pval[i].usecs = t.usecs;
                    }
                }
            }
            break;
        default:
	           return OV_ERR_BADTYPE;
    }

    res = fb_set_vec_time_span_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);
    
    /* Speicher freigeben */
    if(pval) { free(pval); }

    return res;
}

/*	======================================================================	*/
OV_RESULT fb_set_vec_string_varvalue_eval(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed,
	OV_STRING                   *pvalue,
	OV_UINT                      veclen
) {
    OV_UINT i;
    OV_UINT size;
    
    /* PV? */
	if(pelem->elemunion.pvar->v_vartype == OV_VT_STRING_VEC) {
        for(i=0; i<veclen; i++) {
            if( ov_string_compare(pvalue[i] , ((OV_STRING_VEC*)pelem->pvalue)->value[i]) ) {
                *changed = TRUE;
                break;                
            }
        }
    } else {
        /* PV */
        if(  ((OV_STRING_PV_VEC*)pelem->pvalue)->state != pvarcurrprops->state ||
		    (((OV_STRING_PV_VEC*)pelem->pvalue)->time.secs != pvarcurrprops->time.secs) ||
		    (((OV_STRING_PV_VEC*)pelem->pvalue)->time.usecs != pvarcurrprops->time.usecs) ) {
    		*changed = TRUE;
    	} else {
            /* Wert geaendert? */
            for(i=0; i<veclen; i++) {
                if( ov_string_compare(pvalue[i] , ((OV_STRING_PV_VEC*)pelem->pvalue)->value.value[i]) ) {
                    *changed = TRUE;
                    break;                
                }
            }
        }
    }

	/* SET-FNC forhanden? */
    if( pelem->elemunion.pvar->v_setfnc ) {
        if(pelem->elemunion.pvar->v_vartype == OV_VT_TIME_SPAN_PV_VEC) {
			OV_STRING_PV_VEC var;
			var.value.value = pvalue;
			var.value.veclen= veclen;
			var.state = pvarcurrprops->state;
			var.time = pvarcurrprops->time;
			return ((OV_FNCPTR_SETPVVEC(STRING))pelem->elemunion.pvar->v_setfnc)(pobj, &var);
        }
        return ((OV_FNCPTR_SETVEC(STRING))pelem->elemunion.pvar->v_setfnc)(pobj, pvalue, veclen);
    }
    
    if(*changed == TRUE) {
        size = veclen*sizeof(OV_STRING);
        if(pelem->elemunion.pvar->v_vartype == OV_VT_STRING_VEC) {
            return ov_vector_setstaticvalue( (OV_STRING*)pelem->pvalue ,pvalue,veclen,size,OV_VT_STRING);
        }
        
        ((OV_STRING_PV_VEC*)pelem->pvalue)->state = pvarcurrprops->state;
	    ((OV_STRING_PV_VEC*)pelem->pvalue)->time.secs = pvarcurrprops->time.secs;
	    ((OV_STRING_PV_VEC*)pelem->pvalue)->time.usecs = pvarcurrprops->time.usecs;
        return ov_vector_setstaticvalue( ((OV_STRING_PV_VEC*)pelem->pvalue)->value.value ,pvalue,veclen,size,OV_VT_STRING);
    }
    
    return OV_ERR_OK;
}
/*	------------------------------------------------------------------- */
OV_RESULT fb_set_vec_string_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {
	OV_STRING          *pval;
    OV_UINT             i,veclen;
    OV_RESULT           res;

    OV_UINT   size;
	OV_UINT   ksTyp;

    veclen = pelem->elemunion.pvar->v_veclen;
	
	/* Typ gleich? */
	ksTyp = pvarcurrprops->value.vartype & OV_VT_KSMASK;
    if(ksTyp == OV_VT_STRING_VEC) {
        /* Laenge gleich? */
        if( veclen == pvarcurrprops->value.valueunion.val_string_vec.veclen) {
            pval = (OV_STRING*)(pvarcurrprops->value.valueunion.val_string_vec.value);
    	    return fb_set_vec_string_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);
    	}
    }
    
    pval = (OV_STRING*)malloc(veclen * sizeof(OV_STRING));
    if(!pval) {
        return OV_ERR_DBOUTOFMEMORY;
    }
	for(i=0; i<veclen; i++) {
        pval[i] = NULL;
    }
    
    switch(ksTyp) {
        case OV_VT_VOID:
            break;
        case OV_VT_BYTE:
            pval[0] = (OV_STRING)malloc(32*sizeof(char));
            if(!pval[0]) {
                fb_free_string_struct(pval, veclen);
                return OV_ERR_DBOUTOFMEMORY;
            }            
            sprintf(pval[0], "%d", pvarcurrprops->value.valueunion.val_byte);
            break;   
        case OV_VT_BOOL:
            pval[0] = (OV_STRING)malloc(6*sizeof(char));
            if(!pval[0]) {
                fb_free_string_struct(pval, veclen);
                return OV_ERR_DBOUTOFMEMORY;
            }
            if(pvarcurrprops->value.valueunion.val_bool) {       
                strcpy(pval[0], "TRUE");
            } else {
                strcpy(pval[0], "FALSE");
            }
            break;
        case OV_VT_INT: {
                pval[0] = (OV_STRING)malloc(32*sizeof(char));
                if(!pval[0]) {
                    fb_free_string_struct(pval, veclen);
                    return OV_ERR_DBOUTOFMEMORY;
                }            
                sprintf(pval[0], "%ld", pvarcurrprops->value.valueunion.val_int);
            }
            break;
        case OV_VT_UINT: {
                pval[0] = (OV_STRING)malloc(32*sizeof(char));
                if(!pval[0]) {
                    fb_free_string_struct(pval, veclen);
                    return OV_ERR_DBOUTOFMEMORY;
                }            
                sprintf(pval[0], "%lu", pvarcurrprops->value.valueunion.val_uint);
            }
            break;
        case OV_VT_SINGLE: {
                pval[0] = (OV_STRING)malloc(32*sizeof(char));
                if(!pval[0]) {
                    fb_free_string_struct(pval, veclen);
                    return OV_ERR_DBOUTOFMEMORY;
                }            
                sprintf(pval[0], "%f", pvarcurrprops->value.valueunion.val_single);
            }
            break;
        case OV_VT_DOUBLE: {
                pval[0] = (OV_STRING)malloc(64*sizeof(char));
                if(!pval[0]) {
                    fb_free_string_struct(pval, veclen);
                    return OV_ERR_DBOUTOFMEMORY;
                }            
                sprintf(pval[0], "%f", pvarcurrprops->value.valueunion.val_single);
            }
            break;
        case OV_VT_TIME: {
                OV_TIME t;
                pval[0] = (OV_STRING)malloc(32*sizeof(char));
                if(!pval[0]) {
                    fb_free_string_struct(pval, veclen);
                    return OV_ERR_DBOUTOFMEMORY;
                }
                t = pvarcurrprops->value.valueunion.val_time;
                fb_time2ascii(&t, pval[0]);       
            }
            break;
        case OV_VT_TIME_SPAN: {
                OV_TIME_SPAN t;
                pval[0] = (OV_STRING)malloc(64*sizeof(char));
                if(!pval[0]) {
                    fb_free_string_struct(pval, veclen);
                    return OV_ERR_DBOUTOFMEMORY;
                }
                t = pvarcurrprops->value.valueunion.val_time_span;
                fb_timespan2ascii(&t, pval[0]);       
            }
            break;
        case OV_VT_STRING: {
                OV_STRING str;
                str = pvarcurrprops->value.valueunion.val_string;
                if( ov_string_compare(str, "") ) {
                    pval[0] = (OV_STRING)malloc(ov_string_getlength(str)*sizeof(char) + 1);
                    if(!pval[0]) {
                        fb_free_string_struct(pval, veclen);
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    strcpy(pval[0], str);
                }
            }
            break;
        /* Vectoren */
        case OV_VT_BYTE_VEC: {
                size = pvarcurrprops->value.valueunion.val_byte_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    pval[i] = (OV_STRING)malloc(32*sizeof(char));
                    if(!pval[i]) {
                        fb_free_string_struct(pval, veclen);
                        return OV_ERR_DBOUTOFMEMORY;
                    }            
                    sprintf(pval[i], "%d", ((OV_BYTE*)(pvarcurrprops->value.valueunion.val_byte_vec.value))[i]);
                }
            }
            break;
        case OV_VT_BOOL_VEC: {
                size = pvarcurrprops->value.valueunion.val_bool_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    pval[i] = (OV_STRING)malloc(6*sizeof(char));
                    if(!pval[i]) {
                        fb_free_string_struct(pval, veclen);
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    if( ((OV_BOOL*)(pvarcurrprops->value.valueunion.val_byte_vec.value))[i] ) {
                        strcpy(pval[i], "TRUE");
                    } else {
                        strcpy(pval[i], "FALSE");
                    }
                }
            }
            break;
        case OV_VT_INT_VEC: {
                size = pvarcurrprops->value.valueunion.val_int_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    pval[i] = (OV_STRING)malloc(32*sizeof(char));
                    if(!pval[i]) {
                        fb_free_string_struct(pval, veclen);
                        return OV_ERR_DBOUTOFMEMORY;
                    }            
                    sprintf(pval[i], "%ld", ((OV_INT*)(pvarcurrprops->value.valueunion.val_int_vec.value))[i]);
                }
            }
            break;
        case OV_VT_UINT_VEC: {
                size = pvarcurrprops->value.valueunion.val_uint_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    pval[i] = (OV_STRING)malloc(32*sizeof(char));
                    if(!pval[i]) {
                        fb_free_string_struct(pval, veclen);
                        return OV_ERR_DBOUTOFMEMORY;
                    }            
                    sprintf(pval[i], "%ld", ((OV_UINT*)(pvarcurrprops->value.valueunion.val_uint_vec.value))[i]);
                }
            }
            break;
        case OV_VT_SINGLE_VEC: {
                size = pvarcurrprops->value.valueunion.val_single_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    pval[i] = (OV_STRING)malloc(32*sizeof(char));
                    if(!pval[i]) {
                        fb_free_string_struct(pval, veclen);
                        return OV_ERR_DBOUTOFMEMORY;
                    }            
                    sprintf(pval[i], "%f", ((OV_SINGLE*)(pvarcurrprops->value.valueunion.val_single_vec.value))[i]);
                }
            }
            break;
        case OV_VT_DOUBLE_VEC: {
                size = pvarcurrprops->value.valueunion.val_double_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    pval[i] = (OV_STRING)malloc(32*sizeof(char));
                    if(!pval[i]) {
                        fb_free_string_struct(pval, veclen);
                        return OV_ERR_DBOUTOFMEMORY;
                    }            
                    sprintf(pval[i], "%f", ((OV_SINGLE*)(pvarcurrprops->value.valueunion.val_double_vec.value))[i]);
                }
            }
            break;
        case OV_VT_TIME_VEC:  {
                OV_TIME t;
                size = pvarcurrprops->value.valueunion.val_time_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    pval[i] = (OV_STRING)malloc(32*sizeof(char));
                    if(!pval[i]) {
                        fb_free_string_struct(pval, veclen);
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    t = ((OV_TIME*)(pvarcurrprops->value.valueunion.val_time_vec.value))[i];
                    fb_time2ascii(&t, pval[i]);       
                }
            }
            break;
        case OV_VT_TIME_SPAN_VEC: {
                OV_TIME_SPAN t;
                size = pvarcurrprops->value.valueunion.val_time_span_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    pval[i] = (OV_STRING)malloc(64*sizeof(char));
                    if(!pval[i]) {
                        fb_free_string_struct(pval, veclen);
                        return OV_ERR_DBOUTOFMEMORY;
                    }
                    t = ((OV_TIME_SPAN*)(pvarcurrprops->value.valueunion.val_time_span_vec.value))[i];
                    fb_timespan2ascii(&t, pval[i]);       
                }
            }
            break;
        case OV_VT_STRING_VEC: {
                OV_STRING str;
                size = pvarcurrprops->value.valueunion.val_string_vec.veclen;
                if(size > veclen) {
                    size = veclen;
                }
                for(i=0; i<size; i++) {
                    str = ((OV_STRING*)(pvarcurrprops->value.valueunion.val_string_vec.value))[i];
                    if(ov_string_compare(str, "")) {
                        pval[i] = (OV_STRING)malloc(ov_string_getlength(str)*sizeof(char) + 1);
                        if(!pval[i]) {
                            fb_free_string_struct(pval, veclen);
                            return OV_ERR_DBOUTOFMEMORY;
                        }
                        strcpy(pval[i], str);
                    }
                }
            }
            break;
        default:
	           return OV_ERR_BADTYPE;
    }

    res = fb_set_vec_string_varvalue_eval(pobj,pelem,pvarcurrprops,changed,pval,veclen);
    
    /* Speicher freigeben */
    if(pval) { fb_free_string_struct(pval, veclen); }

    return res;
}



OV_RESULT fb_set_vec_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {
    
    switch(pelem->elemunion.pvar->v_vartype) {
        case OV_VT_BYTE_VEC:
            return fb_set_vec_byte_varvalue(pobj,pelem,pvarcurrprops,changed);
        case OV_VT_BOOL_VEC:
        case OV_VT_BOOL_PV_VEC:
            return fb_set_vec_bool_varvalue(pobj,pelem,pvarcurrprops,changed);
        case OV_VT_INT_VEC:
        case OV_VT_INT_PV_VEC:
            return fb_set_vec_int_varvalue(pobj,pelem,pvarcurrprops,changed);
        case OV_VT_UINT_VEC:
        case OV_VT_UINT_PV_VEC:
            return fb_set_vec_uint_varvalue(pobj,pelem,pvarcurrprops,changed);
        case OV_VT_SINGLE_VEC:
        case OV_VT_SINGLE_PV_VEC:
            return fb_set_vec_single_varvalue(pobj,pelem,pvarcurrprops,changed);
        case OV_VT_DOUBLE_VEC:
        case OV_VT_DOUBLE_PV_VEC:
            return fb_set_vec_double_varvalue(pobj,pelem,pvarcurrprops,changed);
        case OV_VT_TIME_VEC:
        case OV_VT_TIME_PV_VEC:
            return fb_set_vec_time_varvalue(pobj,pelem,pvarcurrprops,changed);
        case OV_VT_TIME_SPAN_VEC:
        case OV_VT_TIME_SPAN_PV_VEC:
            return fb_set_vec_time_span_varvalue(pobj,pelem,pvarcurrprops,changed);
        case OV_VT_STRING_VEC:
        case OV_VT_STRING_PV_VEC:
            return fb_set_vec_string_varvalue(pobj,pelem,pvarcurrprops,changed);
        
       	default:
            break;
    }
	return OV_ERR_BADTYPE;
}


/*	----------------------------------------------------------------------	*/
OV_DLLFNCEXPORT OV_RESULT fb_set_varvalue(
    OV_INSTPTR_ov_object         pobj,
    const OV_ELEMENT            *pelem,
    const OV_ANY                *pvarcurrprops,
    OV_BOOL                     *changed
) {
    OV_UINT varLen;

    /* ANY? */
    if( (pelem->elemunion.pvar->v_vartype & OV_VT_KSMASK) == OV_VT_ANY ) {
        return fb_set_any_varvalue(pobj, pelem, pvarcurrprops, changed);
    }
				
    /* Scalar/Vector/Dynamicvector? */
    varLen = pelem->elemunion.pvar->v_veclen;
	
    /* Dynamic vector variable ? */
	if(varLen == 0) {
	   return fb_set_dynvec_varvalue(pobj, pelem, pvarcurrprops, changed);
	}
	
    /* Scalar variable ? */
	if(varLen == 1) {
	   return fb_set_scalar_varvalue(pobj, pelem, pvarcurrprops, changed);
	}
    		
    /* Static vector variable */
	return fb_set_vec_varvalue(pobj, pelem, pvarcurrprops, changed);	
}


/*
*	End of file
*/
