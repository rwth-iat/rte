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
*   fb_functionblock.c                                                        *
*                                                                             *
*                                                                             *
*   Historie                                                                  *
*   --------                                                                  *
*   1999-08-07 Alexander Neugebauer: Erstellung, LTSoft GmbH, Kerpen          *
*                                    Lehrstuhl fuer Prozessleittechnik, Aachen*
*                                                                             *
*   1999-10-25 V1.21 Aufruf von Unterobjekten mit ltc, bei cyctime == 0       *
*   2000-02-11 V1.21 Uebertragung des unveraenderten Status bei SetVar (PV)   *
*   2001-04-27 V1.4  Keine Begrenzung des Method-Counters                     *
*   2006-10-10 V2.0  setVar FNC ueberarbeitet                                 *
*                                                                             *
*   Beschreibung                                                              *
*   ------------                                                              *
*   Implementierung der FB-Klasse  functionblock                              *
*                                                                             *
******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_fb
#define OV_COMPILE_LIBRARY_fb
#endif

#include "fb.h"
#include "fb_av.h"
#include "fb_namedef.h"
#include "fb_macros.h"
#include "fb_database.h"
#include "libov/ov_string.h"
#include "libov/ov_time.h"
#include "libov/ov_path.h"
#include "libov/ov_supervise.h"
#include "libov/ov_macros.h"
#include "libov/ov_logfile.h"
#include "fb_log.h"

/*	----------------------------------------------------------------------	*/
/*
*   Deklarationen:
*/
void fb_set_proctime( OV_INSTPTR_fb_task	ptask);

/*	----------------------------------------------------------------------	*/
/* Hilfs-FNC: Prueft, ob Var-Typ von FB unterstuetz ist */
int fb_vartype_implemented(OV_VAR_TYPE typ) {

    switch(typ) {
        case OV_VT_ANY:
        case OV_VT_BYTE:
        case OV_VT_BOOL:
        case OV_VT_INT:
        case OV_VT_UINT:
        case OV_VT_SINGLE:
        case OV_VT_DOUBLE:
        case OV_VT_STRING:
        case OV_VT_TIME:
        case OV_VT_TIME_SPAN:
        
        case OV_VT_BYTE_VEC:
        case OV_VT_BOOL_VEC:
        case OV_VT_INT_VEC:
        case OV_VT_UINT_VEC:
        case OV_VT_SINGLE_VEC:
        case OV_VT_DOUBLE_VEC:
        case OV_VT_STRING_VEC:
        case OV_VT_TIME_VEC:
        case OV_VT_TIME_SPAN_VEC:

        case OV_VT_BOOL_PV:
        case OV_VT_INT_PV:
        case OV_VT_UINT_PV:
        case OV_VT_SINGLE_PV:
        case OV_VT_DOUBLE_PV:
        case OV_VT_STRING_PV:
        case OV_VT_TIME_PV:
        case OV_VT_TIME_SPAN_PV:

        case OV_VT_BOOL_PV_VEC:
        case OV_VT_INT_PV_VEC:
        case OV_VT_UINT_PV_VEC:
        case OV_VT_SINGLE_PV_VEC:
        case OV_VT_DOUBLE_PV_VEC:
        case OV_VT_STRING_PV_VEC:
        case OV_VT_TIME_PV_VEC:
        case OV_VT_TIME_SPAN_PV_VEC:

            return 1;
        default:
            break;
	}
    return 0;
}

/*	----------------------------------------------------------------------	*/
/*
*	Get the flags for the functionblock object or its variables
*/
OV_DLLFNCEXPORT OV_UINT fb_functionblock_getflags(
	OV_INSTPTR_ov_object 	pobj,
	const OV_ELEMENT		*pelem
) {
	return ov_object_getflags(pobj, pelem );
}

/*	----------------------------------------------------------------------	*/
/*
*	Get the access rights for the functionblock object or its variables
*/
OV_DLLFNCEXPORT OV_ACCESS fb_functionblock_getaccess(
	OV_INSTPTR_ov_object		pobj,
	const OV_ELEMENT			*pelem,
	const OV_TICKET				*pticket
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_fb_functionblock	pfb = Ov_StaticPtrCast(fb_functionblock, pobj);
	OV_INSTPTR_fb_connection	pconn;

	/*
	*	switch based on the element's type
	*/
	switch(pelem->elemtype) {
		case OV_ET_OBJECT:
			pconn = Ov_GetFirstChild(fb_inputconnections, pfb);
            if(pconn) {
                return (OV_AC_READWRITE | OV_AC_RENAMEABLE | OV_AC_LINKABLE | OV_AC_UNLINKABLE);
            }
            pconn = Ov_GetFirstChild(fb_outputconnections, pfb);
            if(pconn) {
                return (OV_AC_READWRITE | OV_AC_RENAMEABLE | OV_AC_LINKABLE | OV_AC_UNLINKABLE);
            }
            /* Keine Verbindungen am Objekt. Pruefe Task-Zugriffsrechte */
            break;
		default:
			break;
	}
	return fb_task_getaccess(pobj, pelem, pticket);
}

/*	----------------------------------------------------------------------	*/
/*
*	Trigger input get connections
*/
OV_DLLFNCEXPORT void fb_functionblock_triggerInpGetConnections(
	OV_INSTPTR_fb_functionblock	pfb
) {
	OV_INSTPTR_fb_connection				pconn;
	OV_VTBLPTR_fb_connection				pconnvtable;

    /* Logging */
    FbSvcLog_printexecitem((OV_INSTPTR_ov_object)pfb, "trigger input get connections");

    /* Ueber alle Input-Verbindungen */
	Ov_ForEachChild(fb_inputconnections, pfb, pconn) {
		if(!pconn->v_sourcetrig) {
			Ov_GetVTablePtr(fb_connection, pconnvtable, pconn);
      
			if(pconnvtable) {
			    pconnvtable->m_trigger(pconn);
			} else {
			    pconn->v_on = FALSE;
			    
			    /* Logging */
                FbSvcLog_printexecitem((OV_INSTPTR_ov_object)pconn, "ERROR: method table not found");
			    ov_logfile_error("Connection %s: method table not found.", pconn->v_identifier);
			}
		}
	}

    return;
}

/*	----------------------------------------------------------------------	*/
/*
*	Trigger output send connections
*/
OV_DLLFNCEXPORT void fb_functionblock_triggerOutSendConnections(
	OV_INSTPTR_fb_functionblock	pfb
) {
	OV_INSTPTR_fb_connection				pconn;
	OV_VTBLPTR_fb_connection				pconnvtable;

    /* Logging */
    FbSvcLog_printexecitem((OV_INSTPTR_ov_object)pfb, "trigger output send connections");

    /* Ueber alle Output-Verbindungen */
	Ov_ForEachChild(fb_outputconnections, pfb, pconn) {
		if(pconn->v_sourcetrig) {
			Ov_GetVTablePtr(fb_connection, pconnvtable, pconn);
    
			if(pconnvtable) {
			    pconnvtable->m_trigger(pconn);
			} else {
			    pconn->v_on = FALSE;
			    ov_logfile_error("Connection %s: method table not found.", 
			        pconn->v_identifier);
			}
		}
	}
	
    return;
}

/*	----------------------------------------------------------------------	*/
/*
*	Call typemethod of functionblock
*/
OV_DLLFNCEXPORT OV_BOOL fb_functionblock_execTypeMethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME				        *pltc
) {
	volatile OV_VTBLPTR_fb_functionblock	pfbvtable;
	OV_JUMPBUFFER							jumpbuffer;
    OV_BOOL                                 ret;

	FbSvcLog_printexecitem((OV_INSTPTR_ov_object)pfb, "execute typemethod");

	pfb->v_eexreq = 0;
	ret = TRUE;
	
	Ov_GetVTablePtr(fb_functionblock, pfbvtable, pfb);
	if(!pfbvtable) {
		pfb->v_actimode = 0;	/* FIXME! */
		pfb->v_ErrState = 1;	/* FIXME! */
		
		/* Logging */
		FbSvcLog_printexecitem((OV_INSTPTR_ov_object)pfb, "ERROR: method table not found");		
		ov_logfile_error("Functionblock %s: method table not found.", pfb->v_identifier);
		    
		return FALSE;
	}

	/* supervised? */
	if( (pfb->v_maxcalctime.secs >= 0) && (pfb->v_maxcalctime.usecs >= 0) &&
	    (pfb->v_maxcalctime.secs || pfb->v_maxcalctime.usecs) ) {
	   
		if(ov_supervise_setjmp(jumpbuffer) == 0) {
			if(ov_supervise_start(&pfb->v_maxcalctime, &jumpbuffer)) {
				pfbvtable->m_typemethod(pfb, pltc);
				ov_supervise_end();
			} else {
			    FbSvcLog_printexecitem((OV_INSTPTR_ov_object)pfb, "ERROR: can't start supervise");
				pfbvtable->m_typemethod(pfb, pltc);
			}
		} else {
			/* timer timed out, here's code handling this error */
			FbSvcLog_printexecitem((OV_INSTPTR_ov_object)pfb, "ERROR: max. calculation time exceeded");
			
			ov_memstack_lock();
			ov_logfile_warning("Functionblock %s: max. calculation time exceeded.", 
				ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object, pfb), KS_VERSION));
			ov_memstack_unlock();
			
			pfb->v_actimode = 0;	/* FIXME! */
			pfb->v_ErrState = 1;	/* FIXME! */

			ret = FALSE;
		}
	} else {
	    /* Not supervised */
		pfbvtable->m_typemethod(pfb, pltc);
	}

	/*
	*	increment the counter
	*/
	if(ret) {
    	pfb->v_methcount++;
    	if(!pfb->v_methcount) {
    		pfb->v_methcount = 1;
        }
    }
    return ret;
}

/*	----------------------------------------------------------------------	*/
/*
*	Execute the functionblock
*/
OV_DLLFNCEXPORT void fb_functionblock_execute(
	OV_INSTPTR_fb_task	ptask,
	OV_TIME				*pltc
) {
	/*
	*	local variables
	*/
	char                           help[128];
	OV_TIME				           t0;
	OV_TIME				           t1;
	OV_BOOL				           ret;
	OV_INSTPTR_fb_functionblock    pfb = Ov_StaticPtrCast(fb_functionblock, ptask);
	
	
	/*
	*	is otc >= ltc or function off?
	*/
	if((ov_time_compare(&pfb->v_proctime, pltc) > 0) || (pfb->v_actimode == 0)) {
		return;
	}

    /* Logging */
    FbSvcLog_incrIndent();
    sprintf(help, "executing (method counter %lu)", pfb->v_methcount);
    FbSvcLog_printexecitem((OV_INSTPTR_ov_object)pfb, help);

	/* trigger input get connections */
    fb_functionblock_triggerInpGetConnections(pfb);
    
	/* test if we need to calculate new outputs */
	if(pfb->v_iexreq || pfb->v_eexreq) {
		
		/* call typemethod and supervise if appropriate */
		ov_time_gettime(&t0);
        ret = fb_functionblock_execTypeMethod(pfb, pltc);
		ov_time_gettime(&t1);
		ov_time_diff(&pfb->v_calctime, &t1, &t0);
		
		/* Typemethod ausgefuehrt? */
		if(ret == FALSE) {
            /* Logging */
            FbSvcLog_decrIndent();
            return;
        }
        
		/* trigger output send connections */
		fb_functionblock_triggerOutSendConnections(pfb);
	}
	
	/* execute the child objects */
	fb_task_execChildObjects(ptask, pltc);

	/* calculate new value of otc */
	fb_task_setNextProcTime(ptask, pltc);

    /* Logging */
    FbSvcLog_decrIndent();
}


/*	----------------------------------------------------------------------	*/

OV_DLLFNCEXPORT OV_RESULT fb_functionblock_getvar(
	OV_INSTPTR_ov_object 	pobj, 
	const OV_ELEMENT		*pelem,
	OV_ANY              	*pvarcurrprops
) {
    /* Typ unterstuetzt? */
    if( !fb_vartype_implemented(pelem->elemunion.pvar->v_vartype) ) {
        return OV_ERR_NOTIMPLEMENTED;
    }

    return ov_object_getvar(pobj, pelem, pvarcurrprops);
}


/*	----------------------------------------------------------------------	*/
int fb_functionblock_compare_dynvec(
	const OV_POINTER	pvalue1,
	const OV_POINTER	pvalue2,
	const OV_UINT		veclen,
	const OV_UINT		size,
	const OV_VAR_TYPE	vartype
) {
	/*
	*	local variables
	*/
	OV_UINT		    i;
	OV_STRING	   *pstring1;
    OV_STRING      *pstring2;
	OV_INT		    result;
    OV_GENERIC_VEC* pvector = (OV_GENERIC_VEC*)pvalue1;
    
    /* Port gesetzt? */
    if(!pvalue1) {
        /* Vector leer. Wert uebergeben? */
        if(veclen == 0) {
            /* Nichts zu setzen */
            return 0;
        }
		return 1;
	}
	/* Vector-Laenge geaendert? */
    if(pvector->veclen != veclen) {
        return 1;
    }
	/* Vector leer? */
    if(veclen == 0) {
        /*
        *  Port-Vector ist auch leer, da die Laenge gleich ist.        
        */
        return 0;
    }

	/*
	*	compare values
	*/
	if(pvalue2) {
		if((vartype & OV_VT_KSMASK) == OV_VT_STRING) {
		    pstring1=(OV_STRING*)pvector->value;
		    pstring2=(OV_STRING*)pvalue2;
			for(i=0; i<veclen; i++) {
				result = ov_string_compare(pstring1[i], pstring2[i]);
				if(result) {
					return result;
				}
			}
			return 0;
		}
		return memcmp(pvector->value, pvalue2, size);
	}
    
    return 1;
}

#define Fb_CompareDynVectorValues(pvalue1, pvalue2, veclen, type)			\
	fb_functionblock_compare_dynvec((const OV_POINTER)(pvalue1),			\
	(const OV_POINTER)(pvalue2), (veclen),									\
    (veclen)*sizeof(OV_##type), OV_VT_##type)

/*	----------------------------------------------------------------------	*/
/*
*	Helper macro for implementing fb_functionblock_setvar()
*/
#define FB_CheckDynVecValue(vartype, VARTYPE)									\
	case OV_VT_##VARTYPE##_VEC:													\
		if( Fb_CompareDynVectorValues(pelem->pvalue,							\
                pvarcurrprops->value.valueunion.val_##vartype##_vec.value,		\
                pvarcurrprops->value.valueunion.val_##vartype##_vec.veclen,		\
                VARTYPE)) {														\
            *changed = TRUE;											 	    \
        }																		\
		return OV_ERR_OK
		
#define FB_CheckDynVecPVValue(vartype, VARTYPE)														\
	case OV_VT_##VARTYPE##_PV_VEC:																	\
		if( (((OV_##VARTYPE##_PV_VEC*)pelem->pvalue)->state != pvarcurrprops->state) || 			\
		    (((OV_##VARTYPE##_PV_VEC*)pelem->pvalue)->time.secs != pvarcurrprops->time.secs) ||		\
		    (((OV_##VARTYPE##_PV_VEC*)pelem->pvalue)->time.usecs != pvarcurrprops->time.usecs) ) {	\
    		*changed = TRUE;																		\
    		return OV_ERR_OK;																		\
    	}																							\
		if( Fb_CompareDynVectorValues(((OV_##VARTYPE##_PV_VEC*)pelem->pvalue)->value.value,			\
                pvarcurrprops->value.valueunion.val_##vartype##_vec.value,							\
                pvarcurrprops->value.valueunion.val_##vartype##_vec.veclen,							\
                VARTYPE) ) {																		\
			*changed = TRUE;																		\
        }																							\
        return OV_ERR_OK

#define FB_CheckValue(vartype, VARTYPE)									\
	case OV_VT_##VARTYPE:												\
		if(	*((OV_##VARTYPE*)pelem->pvalue)	!=							\
				 pvarcurrprops->value.valueunion.val_##vartype) {		\
			*changed = TRUE;											\
		}																\
		return OV_ERR_OK

#define FB_CheckPVValue(vartype, VARTYPE)								\
	case OV_VT_##VARTYPE##_PV:											\
		if(((OV_##VARTYPE##_PV*)pelem->pvalue)->state != pvarcurrprops->state || \
		    (((OV_##VARTYPE##_PV_VEC*)pelem->pvalue)->time.secs != pvarcurrprops->time.secs) ||		\
		    (((OV_##VARTYPE##_PV_VEC*)pelem->pvalue)->time.usecs != pvarcurrprops->time.usecs) ) {	\
    		*changed = TRUE;													\
    		return OV_ERR_OK;													\
    	}																		\
		if(((OV_##VARTYPE##_PV*)pelem->pvalue)->value !=						\
		      pvarcurrprops->value.valueunion.val_##vartype) {			\
			*changed = TRUE;											\
		}																\
		return OV_ERR_OK


#define FB_CheckVecValue(vartype, VARTYPE)													\
	case OV_VT_##VARTYPE##_VEC:																\
		if( ov_vector_compare((const OV_POINTER)(pelem->pvalue),							\
            (const OV_POINTER)(pvarcurrprops->value.valueunion.val_##vartype##_vec.value),	\
            pvarcurrprops->value.valueunion.val_##vartype##_vec.veclen,						\
            pvarcurrprops->value.valueunion.val_##vartype##_vec.veclen*sizeof(OV_##VARTYPE),\
            OV_VT_##VARTYPE)) {																\
			*changed = TRUE;																\
        }																					\
        return OV_ERR_OK

#define FB_CheckVecPVValue(vartype, VARTYPE)								\
	case OV_VT_##VARTYPE##_PV_VEC:												\
		if(((OV_##VARTYPE##_PV_VEC*)pelem->pvalue)->state != pvarcurrprops->state || \
		    (((OV_##VARTYPE##_PV_VEC*)pelem->pvalue)->time.secs != pvarcurrprops->time.secs) ||		\
		    (((OV_##VARTYPE##_PV_VEC*)pelem->pvalue)->time.usecs != pvarcurrprops->time.usecs) ) {	\
    		*changed = TRUE;														 \
    		return OV_ERR_OK;													\
    	}																		\
		if( Fb_CompareDynVectorValues(((OV_##VARTYPE##_PV_VEC*)pelem->pvalue)->value.value, \
                pvarcurrprops->value.valueunion.val_##vartype##_vec.value,		\
                pvarcurrprops->value.valueunion.val_##vartype##_vec.veclen,		\
                VARTYPE)) {														\
			*changed = TRUE;													\
        }																		\
        return OV_ERR_OK

#define FB_CheckAnyValue(vartype, VARTYPE)										\
	case OV_VT_##VARTYPE:														\
		if(	pany->value.valueunion.val_##vartype	!=							\
			pvarcurrprops->value.valueunion.val_##vartype) {					\
			*changed = TRUE;													\
		}																		\
 		return OV_ERR_OK
		
#define FB_CheckAnyDynVecValue(vartype, VARTYPE)								\
	case OV_VT_##VARTYPE##_VEC:													\
		if( pany->value.valueunion.val_##vartype##_vec.veclen !=				\
            pvarcurrprops->value.valueunion.val_##vartype##_vec.veclen) {		\
			*changed = TRUE;													\
			return OV_ERR_OK;													\
		}																		\
		if( Ov_CompareVectorValues(pany->value.valueunion.val_##vartype##_vec.value,\
                pvarcurrprops->value.valueunion.val_##vartype##_vec.value,		\
                pvarcurrprops->value.valueunion.val_##vartype##_vec.veclen,		\
                VARTYPE)) {														\
            *changed = TRUE;											 	    \
        }																		\
		return OV_ERR_OK

/*	----------------------------------------------------------------------	*/
OV_RESULT fb_compareAnyValue(
	const OV_ANY       	    *pany,
	const OV_ANY            *pvarcurrprops,
	OV_BOOL                 *changed
) {

    if(!pany) {
        *changed = 1;
        return 1;
    }
    
    if( (pvarcurrprops->value.vartype  & OV_VT_KSMASK) !=
        (pany->value.vartype  & OV_VT_KSMASK) ) {

        *changed = TRUE;
        return OV_ERR_OK;        
    }
   
    switch(pvarcurrprops->value.vartype & OV_VT_KSMASK) {
        
        FB_CheckAnyValue(byte,BYTE);
        FB_CheckAnyValue(bool,BOOL);
        FB_CheckAnyValue(int,INT);
        FB_CheckAnyValue(uint,UINT);
        FB_CheckAnyValue(single,SINGLE);
        FB_CheckAnyValue(double,DOUBLE);
        
    	case OV_VT_TIME:
    		if( pany->value.valueunion.val_time.secs !=
    		    pvarcurrprops->value.valueunion.val_time.secs) {
    			*changed = TRUE;
    		}
    		if( pany->value.valueunion.val_time.usecs !=
    		    pvarcurrprops->value.valueunion.val_time.usecs) {
    			*changed = TRUE;
    		}
    		return OV_ERR_OK;
    	case OV_VT_TIME_SPAN:
    		if( pany->value.valueunion.val_time_span.secs !=
    		    pvarcurrprops->value.valueunion.val_time_span.secs) {
    			*changed = TRUE;
    		}
    		if( pany->value.valueunion.val_time_span.usecs !=
    		    pvarcurrprops->value.valueunion.val_time_span.usecs) {
    			*changed = TRUE;
    		}
    		return OV_ERR_OK;
		case OV_VT_STRING :
			if( ov_string_compare( pany->value.valueunion.val_string,
					pvarcurrprops->value.valueunion.val_string) ) {
				*changed = TRUE;
			}
			return OV_ERR_OK;
                
        
		FB_CheckAnyDynVecValue(byte, BYTE);
		FB_CheckAnyDynVecValue(bool, BOOL);
		FB_CheckAnyDynVecValue(int, INT);
		FB_CheckAnyDynVecValue(uint, UINT);
		FB_CheckAnyDynVecValue(single, SINGLE);
		FB_CheckAnyDynVecValue(double, DOUBLE);
		FB_CheckAnyDynVecValue(string, STRING);
		FB_CheckAnyDynVecValue(time, TIME);
		FB_CheckAnyDynVecValue(time_span, TIME_SPAN);
		
        default:
            break;
	}
    return OV_ERR_BADVALUE;

}

/*	----------------------------------------------------------------------	*/
OV_RESULT fb_varvalue_changed(
	const OV_ELEMENT			*pelem,
	const OV_ANY            	*pvarcurrprops,
	OV_BOOL                     *changed
) {
    /* Annahme: Wert nicht geaendert */
    *changed = FALSE;
    
    /* Wert vergleichen */    
	switch(pelem->elemunion.pvar->v_veclen) {
		case 0:
			/*
			*	dynamic vector variable
			*/
			switch(pelem->elemunion.pvar->v_vartype) {

				FB_CheckDynVecValue(byte, BYTE);
				FB_CheckDynVecValue(bool, BOOL);
				FB_CheckDynVecValue(int, INT);
				FB_CheckDynVecValue(uint, UINT);
				FB_CheckDynVecValue(single, SINGLE);
				FB_CheckDynVecValue(double, DOUBLE);
				FB_CheckDynVecValue(string, STRING);
				FB_CheckDynVecValue(time, TIME);
				FB_CheckDynVecValue(time_span, TIME_SPAN);
				
				FB_CheckDynVecPVValue(bool, BOOL);
				FB_CheckDynVecPVValue(int, INT);
				FB_CheckDynVecPVValue(uint, UINT);
				FB_CheckDynVecPVValue(single, SINGLE);
				FB_CheckDynVecPVValue(double, DOUBLE);
				FB_CheckDynVecPVValue(string, STRING);
				FB_CheckDynVecPVValue(time, TIME);
				FB_CheckDynVecPVValue(time_span, TIME_SPAN);
				
                default:
			         break;
			}
			break;
			
		case 1:
			/*
			*	scalar variables
			*/
    		switch(pelem->elemunion.pvar->v_vartype) {
    		
    			FB_CheckValue(byte,BYTE);
    			FB_CheckValue(bool,BOOL);
    			FB_CheckValue(int,INT);
    			FB_CheckValue(uint,UINT);
    			FB_CheckValue(single,SINGLE);
    			FB_CheckValue(double,DOUBLE);
    			
            	case OV_VT_TIME:
            		if(	((((OV_TIME*)pelem->pvalue)->secs) != pvarcurrprops->value.valueunion.val_time.secs) ||
                        ((((OV_TIME*)pelem->pvalue)->usecs) != pvarcurrprops->value.valueunion.val_time.usecs) ) {
            			*changed = TRUE;
            		}
            		return OV_ERR_OK;
            	case OV_VT_TIME_SPAN:
            		if(	((((OV_TIME_SPAN*)pelem->pvalue)->secs) != pvarcurrprops->value.valueunion.val_time_span.secs) ||
                        ((((OV_TIME_SPAN*)pelem->pvalue)->usecs) != pvarcurrprops->value.valueunion.val_time_span.usecs) ) {
            			*changed = TRUE;
            		}
            		return OV_ERR_OK;
    			case OV_VT_STRING :
    				if( ov_string_compare( *((OV_STRING*)pelem->pvalue),
    						pvarcurrprops->value.valueunion.val_string) ) {
    					*changed = TRUE;
    				}
    				return OV_ERR_OK;


				FB_CheckPVValue(bool, BOOL);
				FB_CheckPVValue(int, INT);
				FB_CheckPVValue(uint, UINT);
				FB_CheckPVValue(single, SINGLE);
				FB_CheckPVValue(double, DOUBLE);

                case OV_VT_STRING_PV:
            		if( (((OV_STRING_PV*)pelem->pvalue)->state != pvarcurrprops->state) ||
            		    (((OV_STRING_PV*)pelem->pvalue)->time.secs != pvarcurrprops->time.secs) ||
            		    (((OV_STRING_PV*)pelem->pvalue)->time.usecs != pvarcurrprops->time.usecs) ) {
                		*changed = TRUE;
                		return OV_ERR_OK;
                	}
    				if( ov_string_compare( ((OV_STRING_PV*)pelem->pvalue)->value,
    						pvarcurrprops->value.valueunion.val_string) ) {
    					*changed = TRUE;
    				}
		            return OV_ERR_OK;
                case OV_VT_TIME_PV:
            		if(((OV_TIME_PV*)pelem->pvalue)->state != pvarcurrprops->state ||
            		    (((OV_TIME_PV*)pelem->pvalue)->time.secs != pvarcurrprops->time.secs) ||
            		    (((OV_TIME_PV*)pelem->pvalue)->time.usecs != pvarcurrprops->time.usecs) ) {
                		*changed = TRUE;
                		return OV_ERR_OK;
                	}
    				if( (((OV_TIME_PV*)pelem->pvalue)->value.secs != pvarcurrprops->value.valueunion.val_time.secs) ||
                        (((OV_TIME_PV*)pelem->pvalue)->value.usecs != pvarcurrprops->value.valueunion.val_time.usecs) ) {
    					*changed = TRUE;
    				}
		            return OV_ERR_OK;
                case OV_VT_TIME_SPAN_PV:
            		if(((OV_TIME_SPAN_PV*)pelem->pvalue)->state != pvarcurrprops->state ||
            		    (((OV_TIME_SPAN_PV*)pelem->pvalue)->time.secs != pvarcurrprops->time.secs) ||
            		    (((OV_TIME_SPAN_PV*)pelem->pvalue)->time.usecs != pvarcurrprops->time.usecs) ) {
                		*changed = TRUE;
                		return OV_ERR_OK;
                	}
    				if( (((OV_TIME_SPAN_PV*)pelem->pvalue)->value.secs != pvarcurrprops->value.valueunion.val_time_span.secs) ||
                        (((OV_TIME_SPAN_PV*)pelem->pvalue)->value.usecs != pvarcurrprops->value.valueunion.val_time_span.usecs) ) {
    					*changed = TRUE;
    				}
		            return OV_ERR_OK;
		            
		        case OV_VT_ANY:
		            return fb_compareAnyValue((OV_ANY*)pelem->pvalue, pvarcurrprops, changed);
                    
    			default:
    				break;
    		}
    		break;
    		
	  default:
            /*
            *	static vector variable
            */
            if(pelem->elemunion.pvar->v_veclen
            	!= pvarcurrprops->value.valueunion.val_generic_vec.veclen
            ) {
            	return OV_ERR_BADVALUE;
            }
            switch(pelem->elemunion.pvar->v_vartype) {
            	FB_CheckVecValue(byte, BYTE);
            	FB_CheckVecValue(bool, BOOL);
            	FB_CheckVecValue(int, INT);
            	FB_CheckVecValue(uint, UINT);
            	FB_CheckVecValue(single, SINGLE);
            	FB_CheckVecValue(double, DOUBLE);
            	FB_CheckVecValue(string, STRING);
                FB_CheckVecValue(time, TIME);
            	FB_CheckVecValue(time_span, TIME_SPAN);

				FB_CheckVecPVValue(bool, BOOL);
				FB_CheckVecPVValue(int, INT);
				FB_CheckVecPVValue(uint, UINT);
				FB_CheckVecPVValue(single, SINGLE);
				FB_CheckVecPVValue(double, DOUBLE);
				FB_CheckVecPVValue(string, STRING);
				FB_CheckVecPVValue(time, TIME);
				FB_CheckVecPVValue(time_span, TIME_SPAN);

            	default:
            		break;
            }
            
            break;
	}
	
	return OV_ERR_BADTYPE;
}

/*	----------------------------------------------------------------------	*/

OV_DLLFNCEXPORT OV_RESULT fb_functionblock_setvar(
	OV_INSTPTR_ov_object 		pobj,
	const OV_ELEMENT			*pelem,
	const OV_ANY            	*pvarcurrprops
) {
	OV_RESULT                   result;
	OV_BOOL                     runSetVar;
	OV_INSTPTR_fb_functionblock	pfb = Ov_StaticPtrCast(fb_functionblock, pobj);

	switch(pelem->elemtype) {
		case OV_ET_VARIABLE:
		
	    	/*
    		*	Check the variable type
		    */
		    if( !fb_vartype_implemented(pelem->elemunion.pvar->v_vartype) ) {
                return OV_ERR_BADTYPE;
			}
			
			/* ANY? Passt alles rein */
			if((pelem->elemunion.pvar->v_vartype & OV_VT_KSMASK) != OV_VT_ANY) {
			    /* Sonnst nur vom gleichen Typ */
    			if((pvarcurrprops->value.vartype & OV_VT_KSMASK)
    			    != (pelem->elemunion.pvar->v_vartype & OV_VT_KSMASK)) {
    	    		return OV_ERR_BADTYPE;
        		}
        	}
            
            /* SetVar accessor? */
    		if(pelem->elemunion.pvar->v_setfnc) {
    			runSetVar = TRUE;
    		} else {
        		/* Compare values */
        		result = fb_varvalue_changed(pelem, pvarcurrprops, &runSetVar);
        		if(result != OV_ERR_OK) {
                    return result;
                }
            }
            if(runSetVar) {
                pfb->v_eexreq = TRUE;
                return ov_object_setvar(pobj, pelem, pvarcurrprops);
            }
			return OV_ERR_OK;
		default:
			break;
	}
	return OV_ERR_BADPARAM;
}
/*	----------------------------------------------------------------------	*/

OV_DLLFNCEXPORT void fb_functionblock_startup(
	OV_INSTPTR_ov_object pobj
) {

	OV_INSTPTR_fb_functionblock	pfb = Ov_StaticPtrCast(fb_functionblock, pobj);

	ov_object_startup(pobj);

    if(	pfb->v_actimode == 1) {
        fb_set_proctime( Ov_StaticPtrCast(fb_task, pobj) );
    }
    
	if( IsFlagSet(Ov_GetParent(ov_instantiation, pobj)->v_flags, 'i') )
		pfb->v_iexreq = TRUE;
}


/*	----------------------------------------------------------------------	*/

OV_DLLFNCEXPORT void fb_functionblock_shutdown(
	OV_INSTPTR_ov_object pobj
) {
	ov_object_shutdown(pobj);
}


/*	----------------------------------------------------------------------	*/

/*
*	Constructor of functionblock objects
*/
OV_DLLFNCEXPORT OV_RESULT fb_functionblock_constructor(
	OV_INSTPTR_ov_object pobj
) {
	/*
	*	local variables
	*/
	OV_RESULT					result;
	OV_ELEMENT				    parent, child;
	OV_INSTPTR_fb_dbinfoclass	pdbinfo ;
	OV_INSTPTR_fb_functionblock	pfb = Ov_StaticPtrCast(fb_functionblock, pobj);

	result = ov_object_constructor(pobj);
	if(Ov_Fail(result)) {
		return result;
    }
    
    /*
    *   Set FB-Flags
    */    
	parent.elemtype = OV_ET_OBJECT;
	parent.pobj = pobj;
	child.elemtype = OV_ET_NONE;
	
	while(TRUE) {
		if(Ov_OK(ov_element_getnextpart(&parent, &child, OV_ET_VARIABLE))) {
            /* Finished? */
    		if(child.elemtype == OV_ET_NONE) {
    			break;
    		}
    		/* Is part a variable? */
    		if (child.elemtype == OV_ET_VARIABLE) {
    			if(child.elemunion.pvar->v_veclen == 0) {
    				SetNewFlag(child.elemunion.pvar->v_flags, 'd');
    			}
            	if(child.elemunion.pvar->v_vartype & OV_VT_HAS_STATE ) {
    				SetNewFlag(child.elemunion.pvar->v_flags, 's');
    			}
        	}
        } else {
            break;
        }
    }
    
	/*
	*	initialize otc
	*/
	ov_time_gettime(&pfb->v_proctime);

	pdbinfo = (OV_INSTPTR_fb_dbinfoclass)fb_database_getdbinfo();
	if(pdbinfo) {
		if( (!fb_started()) && (pdbinfo->v_instnumber >= 10) ) {
			return OV_ERR_NOACCESS;
		}
		pdbinfo->v_instnumber++;
	} else {
	    return OV_ERR_NOACCESS;
	}
	
	return OV_ERR_OK;
	
}


/*	----------------------------------------------------------------------	*/

OV_DLLFNCEXPORT void fb_functionblock_destructor(
	OV_INSTPTR_ov_object pobj
) {
	OV_INSTPTR_fb_dbinfoclass	pdbinfo;
	
	pdbinfo = (OV_INSTPTR_fb_dbinfoclass)fb_database_getdbinfo();
	if(pdbinfo){
        pdbinfo->v_instnumber = (pdbinfo->v_instnumber > 0) ? (pdbinfo->v_instnumber - 1) : 0 ;
	}

	ov_object_destructor(pobj);
}


/*	----------------------------------------------------------------------	*/

OV_DLLFNCEXPORT OV_RESULT fb_functionblock_checkinit(
	OV_INSTPTR_ov_object pobj
) {
    return 	ov_object_checkinit(pobj);
}



/*
*	End of file
*/
