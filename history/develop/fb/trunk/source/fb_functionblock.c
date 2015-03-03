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
OV_RESULT fb_functionblock_wraptypemethod(
    OV_VTBLPTR_fb_functionblock	pfbvtable,
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME				        *pltc
);

/*	----------------------------------------------------------------------	*/
/*
*	Get the flags for the functionblock object or its variables
*/
OV_DLLFNCEXPORT OV_UINT fb_functionblock_getflags(
	OV_INSTPTR_ov_object 	pobj,
	const OV_ELEMENT		*pelem
) {
	return fb_object_getflags(pobj, pelem );
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
	return fb_task_getaccess(pobj, pelem, pticket);
}

/*	----------------------------------------------------------------------	*/
/*
*	Trigger input get connections
*/
OV_DLLFNCEXPORT void fb_functionblock_triggerInpGetConnections(
	OV_INSTPTR_fb_functionblock	pfb
) {
    fb_object_triggerInpGetConnections( (OV_INSTPTR_fb_object)pfb );
}

/*	----------------------------------------------------------------------	*/
/*
*	Trigger output send connections
*/
OV_DLLFNCEXPORT void fb_functionblock_triggerOutSendConnections(
	OV_INSTPTR_fb_functionblock	pfb
) {
    fb_object_triggerOutSendConnections( (OV_INSTPTR_fb_object)pfb );
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

	pfb->v_eexreq = FALSE;
	ret = TRUE;
	
	Ov_GetVTablePtr(fb_functionblock, pfbvtable, pfb);
	if(!pfbvtable) {
		pfb->v_actimode = FB_AM_OFF;	/* FIXME! */
		pfb->v_ErrState = 1;	/* FIXME! */
		
		/* Logging */
		FbSvcLog_printexecitem((OV_INSTPTR_ov_object)pfb, "ERROR: method table not found");		
		ov_logfile_error("Functionblock %s: method table not found.", pfb->v_identifier);
		    
		return FALSE;
	}

	/* supervised? */
	if( (pfb->v_maxcalctime.secs || pfb->v_maxcalctime.usecs) &&
		(pfb->v_maxcalctime.secs >= 0) && (pfb->v_maxcalctime.usecs >= 0)	) {
	   
		if(ov_supervise_setjmp(jumpbuffer) == 0) {
			if(ov_supervise_start(&pfb->v_maxcalctime, &jumpbuffer)) {
                /* pfbvtable->m_typemethod(pfb, pltc); */
				if(fb_functionblock_wraptypemethod(pfbvtable,pfb,pltc) != OV_ERR_OK) {
				
				  pfb->v_actimode = FB_AM_OFF;	/* FIXME! */
    			  pfb->v_ErrState = 1;	/* FIXME! */
    			  ret = FALSE;
    			  
			      FbSvcLog_printexecitem((OV_INSTPTR_ov_object)pfb, "ERROR: exception in typemethod");
                }
				ov_supervise_end();
			} else {
			    FbSvcLog_printexecitem((OV_INSTPTR_ov_object)pfb, "ERROR: can't start supervise");
                /* pfbvtable->m_typemethod(pfb, pltc); */
				if(fb_functionblock_wraptypemethod(pfbvtable,pfb,pltc) != OV_ERR_OK) {
				
				  pfb->v_actimode = FB_AM_OFF;	/* FIXME! */
    			  pfb->v_ErrState = 1;	/* FIXME! */
    			  ret = FALSE;
    			  
			      FbSvcLog_printexecitem((OV_INSTPTR_ov_object)pfb, "ERROR: exception in typemethod");
                }
			}
		} else {
			/* timer timed out, here's code handling this error */
			FbSvcLog_printexecitem((OV_INSTPTR_ov_object)pfb, "ERROR: max. calculation time exceeded");
			
			ov_memstack_lock();
			ov_logfile_warning("Functionblock %s: max. calculation time exceeded.", 
				ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object, pfb), KS_VERSION));
			ov_memstack_unlock();
			
			pfb->v_actimode = FB_AM_OFF;	/* FIXME! */
			pfb->v_ErrState = 1;	/* FIXME! */

			ret = FALSE;
		}
	} else {
	    /* Not supervised */
		if(fb_functionblock_wraptypemethod(pfbvtable,pfb,pltc) != OV_ERR_OK) {
		
		  pfb->v_actimode = FB_AM_OFF;	/* FIXME! */
		  pfb->v_ErrState = 1;	/* FIXME! */
		  ret = FALSE;
		  
	      FbSvcLog_printexecitem((OV_INSTPTR_ov_object)pfb, "ERROR: exception in typemethod");
        }
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
	if((ov_time_compare(&pfb->v_proctime, pltc) == OV_TIMECMP_AFTER) || (pfb->v_actimode == FB_AM_OFF)) {
		return;
	}

	/* Logging */
	FbSvcLog_incrIndent();
	sprintf(help, "executing (method counter %" OV_PRINT_UINT ")", pfb->v_methcount);
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
    return fb_object_getvar(pobj, pelem, pvarcurrprops);
}


/*	----------------------------------------------------------------------	*/

OV_DLLFNCEXPORT OV_RESULT fb_functionblock_setvar(
	OV_INSTPTR_ov_object 		pobj,
	const OV_ELEMENT			*pelem,
	const OV_ANY            	*pvarcurrprops
) {
	return fb_object_setvar(pobj, pelem, pvarcurrprops);
}

/*	----------------------------------------------------------------------	*/

OV_DLLFNCEXPORT void fb_functionblock_startup(
	OV_INSTPTR_ov_object pobj
) {

	OV_INSTPTR_fb_functionblock	pfb = Ov_StaticPtrCast(fb_functionblock, pobj);

	ov_object_startup(pobj);

    if(	pfb->v_actimode == FB_AM_ON) {
        fb_set_proctime( Ov_StaticPtrCast(fb_task, pobj) );
    }
    
	if( IsFlagSet(Ov_GetParent(ov_instantiation, pobj)->v_flags, 'i') ) {
		pfb->v_iexreq = TRUE;
	}
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
	OV_INSTPTR_fb_dbinfoclass	pdbinfo ;
	OV_INSTPTR_fb_functionblock	pfb = Ov_StaticPtrCast(fb_functionblock, pobj);

	result = fb_object_constructor(pobj);
	if(Ov_Fail(result)) {
		return result;
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
