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
*   fb_task.c                                                                 *
*                                                                             *
*                                                                             *
*   Historie                                                                  *
*   --------                                                                  *
*   1999-08-07 Alexander Neugebauer: Erstellung, LTSoft GmbH, Kerpen          *
*                                    Lehrstuhl fuer Prozessleittechnik, Aachen*
*                                                                             *
*   1999-10-11 V1.21 cyctime in Task-Objekt                                   *
*   1999-10-25 V1.21 Aufruf von Unterobjekten mit ltc, bei cyctime == 0       *
*   2001-06-12 Major revision.        A.Neugebauer                            *
*                                                                             *
*   Beschreibung                                                              *
*   ------------                                                              *
*   Implementierung der FB-Klasse task                                        *
*                                                                             *
******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_fb
#define OV_COMPILE_LIBRARY_fb
#endif

#include "fb.h"
#include "libov/ov_time.h"
#include "libov/ov_memstack.h"
#include "libov/ov_path.h"
#include "libov/ov_scheduler.h"
#include "libov/ov_macros.h"
#include "libov/ov_logfile.h"
#include "fb_namedef.h"
#include "fb_macros.h"
#include "fb_database.h"
#include "fb_av.h"
#include "fb_log.h"

/*	----------------------------------------------------------------------	*/
/*
*	Forward-declaration: Non-virtual method of the urtask: run
*/
OV_DLLFNCEXPORT void fb_task_run(
	OV_INSTPTR_ov_object 	pobj
);
/* internal use */
int fb_vartype_implemented(OV_VAR_TYPE typ);

/*	----------------------------------------------------------------------	*/
/*
*	Test if we are the "urtask"
*/
OV_UINT fb_task_is_urtask(
	OV_INSTPTR_fb_task	ptask
) {
    /*
    *   Lokale Variablen 
    */
    char                help[255];
	OV_STRING           path;
    OV_UINT             res;
    OV_INSTPTR_fb_task  purtask;
    
    /*
    *	Test
    */
    purtask = (OV_INSTPTR_fb_task)fb_database_geturtask();
    if(purtask) {
        if(purtask == ptask) {
            return 1;
        }
        
        return 0;
    }
    
    res = 0;
	ov_memstack_lock();

	sprintf(help, "/%s/%s", FB_TASK_CONTAINER, FB_URTASK);
	path = ov_path_getcanonicalpath((OV_INSTPTR_ov_object)ptask,KS_VERSION);
	if(!ov_string_compare(path, help)) {
		res = 1;
	}
	ov_memstack_unlock();

    return res;
}

/*	----------------------------------------------------------------------	*/

/*
*	Set proctime
*/
void fb_set_proctime(
	OV_INSTPTR_fb_task	ptask
) {
    
    double 				d1,d2,d3;
    OV_TIME				t0;
    OV_TIME_SPAN		dt;
    long int			n;

    ov_time_gettime(&t0);

    d1 = (double)t0.secs + ((double)t0.usecs)/1000000;
    d2 = (double)ptask->v_proctime.secs + ((double)ptask->v_proctime.usecs)/1000000;

    if(d2 < d1) {
        d3 = (double)ptask->v_cyctime.secs + ((double)ptask->v_cyctime.usecs)/1000000;
        if((d2 >= 0) && (d3 > 0)) {
            n=(long int)( (d1 - d2)/d3 );
            d3 *= n; n = (long int)d3;
            dt.secs = n;
            dt.usecs= (long int)( (d3-n)*1000000 );
            ov_time_add(&ptask->v_proctime, &ptask->v_proctime, &dt);
        } else {
            ptask->v_proctime = t0;
        }
    }
}

/*	----------------------------------------------------------------------	*/

/*
*	Set accessor of variable "actimode"
*/
OV_DLLFNCEXPORT OV_RESULT fb_task_actimode_set(
	OV_INSTPTR_fb_task	ptask,
	const OV_INT		actimode
) {

    if(ptask->v_actimode == actimode) {
        return OV_ERR_OK;
    }
    
    switch(actimode) {
        case FB_AM_ON:
		    fb_set_proctime(ptask);
        case FB_AM_UNLINK:
        case FB_AM_ONCE:
        case FB_AM_CATCHUP:
                ptask->v_ErrState = 0;
        case FB_AM_OFF:
                ptask->v_actimode = actimode;
                return OV_ERR_OK;
        default :
                break;
    }

	return OV_ERR_BADVALUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	Set accessor of variable "cyctime"
*/
OV_DLLFNCEXPORT OV_RESULT fb_task_cyctime_set(
	OV_INSTPTR_fb_task	ptask,
	const OV_TIME_SPAN	*ocyc
) {
	if((ocyc->secs >= 0) && (ocyc->usecs >= 0)) {
		ptask->v_cyctime = *ocyc;
		return OV_ERR_OK;
	}
	return OV_ERR_BADVALUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	Set accessor of variable "proctime"
*/
OV_DLLFNCEXPORT OV_RESULT fb_task_proctime_set(
	OV_INSTPTR_fb_task	ptask,
	const OV_TIME       *tim
) {
	if((tim->secs == 0) && (tim->usecs == 0)) {
		return OV_ERR_BADVALUE;
	}
	
	ptask->v_proctime.secs = tim->secs;
	ptask->v_proctime.usecs = tim->usecs;
	
	/*
	*	test if we are the "urtask"
	*/
	if( fb_task_is_urtask(ptask) ) {
	    /* Unregister ... */
        ov_scheduler_unregister((OV_INSTPTR_ov_object)ptask);
        /* Set proc time ... */
        fb_set_proctime(ptask);
        /* Register now ... */
	    ov_scheduler_register((OV_INSTPTR_ov_object)ptask, fb_task_run);
	    /* And set next event time */
	    ov_scheduler_setabseventtime((OV_INSTPTR_ov_object)ptask, &ptask->v_proctime);
	}
	
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Constructor of task objects
*/
OV_DLLFNCEXPORT OV_RESULT fb_task_constructor(
	OV_INSTPTR_ov_object pobj
) {
	/*
	*	local variables
	*/
	OV_RESULT					result;
	OV_INSTPTR_fb_dbinfoclass	pdbinfo ;
	OV_INSTPTR_fb_task			ptask = Ov_StaticPtrCast(fb_task, pobj);

	result = ov_object_constructor(pobj);
	if(Ov_Fail(result))
		return result;

	/*
	*	initialize otc
	*/
	ov_time_gettime(&ptask->v_proctime);

	pdbinfo = (OV_INSTPTR_fb_dbinfoclass)fb_database_getdbinfo();
	if(pdbinfo)
		pdbinfo->v_tasknumber++;

	return OV_ERR_OK;
	
}

/*	----------------------------------------------------------------------	*/

/*
*	Destructor of task object
*/
OV_DLLFNCEXPORT void fb_task_destructor(
	OV_INSTPTR_ov_object 		pobj
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_fb_dbinfoclass	pdbinfo ;
	pdbinfo = (OV_INSTPTR_fb_dbinfoclass)fb_database_getdbinfo();
	if(pdbinfo) {
        pdbinfo->v_tasknumber = (pdbinfo->v_tasknumber > 0) ? (pdbinfo->v_tasknumber - 1) : 0 ;
	}
	ov_object_destructor(pobj);
	return;
}
/*	----------------------------------------------------------------------	*/

/*
*	Start up the task object
*/
OV_DLLFNCEXPORT void fb_task_startup(
	OV_INSTPTR_ov_object pobj
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_fb_task			ptask    = Ov_StaticPtrCast(fb_task, pobj);

	ov_object_startup(pobj);
	
    if(ptask->v_actimode == 1) {
        fb_set_proctime(ptask);
    }
    
	/*
	*	test if we are the "urtask"
	*/
	if(fb_task_is_urtask(ptask)) {
	   /* In Scheduler eintragen */
       ov_scheduler_register(pobj, fb_task_run);
	   ov_scheduler_setabseventtime(pobj, &ptask->v_proctime);
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	Shut down the task object
*/
OV_DLLFNCEXPORT void fb_task_shutdown(
	OV_INSTPTR_ov_object pobj
) {
	/*
	*	test if we are the "urtask"
	*/
	if(fb_task_is_urtask((OV_INSTPTR_fb_task)pobj)) {
		ov_scheduler_unregister(pobj);
	}
	ov_object_shutdown(pobj);
}

/*	----------------------------------------------------------------------	*/

/*
*	Get the access rights for the task object or its variables
*/
OV_DLLFNCEXPORT OV_ACCESS fb_task_getaccess(
	OV_INSTPTR_ov_object		pobj,
	const OV_ELEMENT			*pelem,
	const OV_TICKET				*pticket
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_fb_task	ptask = Ov_StaticPtrCast(fb_task, pobj);
	OV_INSTPTR_fb_task	ptaskchild;
	OV_ACCESS           acces;
	
	/*
	*	switch based on the element's type
	*/
	switch(pelem->elemtype) {
		case OV_ET_VARIABLE:
			if(pelem->elemunion.pvar->v_offset >= offsetof(OV_INST_ov_object,__classinfo)) {
			    /* Typ unterstuetzt? */
			    if( !fb_vartype_implemented(pelem->elemunion.pvar->v_vartype)) {
                    return OV_AC_NONE;
				}
		        /* Rest der Variablen setzen, nur wenn Input-Flags gesetzt */
		        if(IsFlagSet(pelem->elemunion.pvar->v_flags, 'p') ||
				   IsFlagSet(pelem->elemunion.pvar->v_flags, 'i')) {
					return OV_AC_READWRITE;
				}
        		if(pelem->elemunion.pvar->v_varprops & OV_VP_SETACCESSOR) {
        			return OV_AC_READWRITE;
        		}
				return OV_AC_READ;
			}
			break;
		case OV_ET_OBJECT:
		    /* Default access */
		    acces = (OV_AC_READWRITE | OV_AC_LINKABLE );
		    
        	/*
        	*	test if we are the "urtask"
        	*/
	        if(!fb_task_is_urtask(ptask)) {
				acces |= OV_AC_RENAMEABLE;
			}
			 
			/* Task als Unterobjekt? */
   	    	ptaskchild = Ov_GetParent(fb_tasklist, ptask);
		    if(ptaskchild) {
		        acces |= OV_AC_UNLINKABLE;
		    }
		    
		    /* Gibt es Task-Unterobjekte ? */
   	    	ptaskchild = Ov_GetFirstChild(fb_tasklist, ptask);
		    if(ptaskchild) {
		        acces |= OV_AC_UNLINKABLE;
		    }
		    
		    /* Ist Task eingeschaltet, darf Task nicht geloescht werden */
		    if(ptask->v_actimode == 0) {
    		    if(!ptaskchild) {
        		    /* Keine Unterobjekte. Dann ist Task loeschbar. Auch UrTask */
        		    acces |= OV_AC_DELETEABLE;
    		    }
		    }
		    
			return acces;
		default:
			break;
	}
	return ov_object_getaccess(pobj, pelem, pticket);
}

/*	----------------------------------------------------------------------	*/
/*
*	Set next proctime of the task object
*/
OV_DLLFNCEXPORT void fb_task_setNextProcTime(
	OV_INSTPTR_fb_task	ptask,
	OV_TIME				*pltc
) {
	OV_INSTPTR_fb_task	ptaskparent;
	
	switch(ptask->v_actimode) {
		case FB_AM_ON:
			if((ptask->v_cyctime.secs) || (ptask->v_cyctime.usecs)) {
				do {
					ov_time_add(&ptask->v_proctime, &ptask->v_proctime, &ptask->v_cyctime);
				} while(ov_time_compare(&ptask->v_proctime, pltc) <= 0);
			}
			break;
		case FB_AM_UNLINK:
			ptaskparent = Ov_GetParent(fb_tasklist, ptask);
			if(ptaskparent) {
			    Ov_Unlink(fb_tasklist, ptaskparent, ptask);
			}
			break;
		case FB_AM_CATCHUP:
			ov_time_add(&ptask->v_proctime, &ptask->v_proctime, &ptask->v_cyctime);
			break;
		case FB_AM_ONCE:
		default :
			ptask->v_actimode = 0;
			break;
	}
	return;
}

/*	----------------------------------------------------------------------	*/
/*
*	Execute the child objects
*/
OV_DLLFNCEXPORT void fb_task_execChildObjects(
	OV_INSTPTR_fb_task	ptask,
	OV_TIME				*pltc
) {
	OV_INSTPTR_fb_task	ptaskchild;
	OV_VTBLPTR_fb_task	pvtable;

    FbSvcLog_printexecitem((OV_INSTPTR_ov_object)ptask, "executing child objects");

	/*
	*	execute the child objects
	*/
	ptaskchild=Ov_GetFirstChild(fb_tasklist, ptask);
	while(ptaskchild) {
		Ov_GetVTablePtr(fb_task, pvtable, ptaskchild);
    
		if(!pvtable) {

			ptaskchild->v_actimode = 0;	/* FIXME! */
			ptaskchild->v_ErrState = 1;	/* FIXME! */
			
			/* Logging */
            FbSvcLog_printexecitem((OV_INSTPTR_ov_object)ptaskchild, "ERROR: method table not found");
			ov_logfile_error(" %s: method table not found.", ptaskchild->v_identifier);
			
		} else {
    		pvtable->m_execute(ptaskchild, pltc);
		}
		ptaskchild = Ov_GetNextChild(fb_tasklist, ptaskchild);
	}

	return;
}

/*	----------------------------------------------------------------------	*/
/*
*	Execute the task object
*/
OV_DLLFNCEXPORT void fb_task_execute(
	OV_INSTPTR_fb_task	ptask,
	OV_TIME				*pltc
) {
	/*
	*	local variables
	*/
	OV_TIME				t0;
	OV_TIME				t1;
	
	/*
	*	is otc >= ltc or task object off?
	*/
	if((ov_time_compare(&ptask->v_proctime, pltc) > 0) || (ptask->v_actimode == 0)) {
		return;
	}

    ov_time_gettime(&t0);

    /* Logging */
    FbSvcLog_incrIndent();
    
	/* execute the child objects */
    fb_task_execChildObjects(ptask, pltc);
	
    /* Logging */
    FbSvcLog_decrIndent();
    
	/* calculate new value of proctime */
    fb_task_setNextProcTime(ptask, pltc);
    
    
	/* calctime */
	ov_time_gettime(&t1);
	ov_time_diff(&ptask->v_calctime, &t1, &t0);
}


/*	----------------------------------------------------------------------	*/

/*
*	Non-virtual method of the urtask: run
*/
OV_DLLFNCEXPORT void fb_task_run(
	OV_INSTPTR_ov_object 	pobj
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_fb_task	ptask = Ov_StaticPtrCast(fb_task, pobj);
	OV_TIME				time;
	
	/*
	*	call urtask's execute method with current time
	*/
	ov_time_gettime(&time);

    if(ptask->v_actimode) {
        /* Logging */
        FbSvcLog_startexeclog(pobj, "start executing");
        
        /* Execute */
    	fb_task_execute(ptask, &time);
    	
    	/* Set next execute time */
    	if( (!ptask->v_cyctime.secs) && (!ptask->v_cyctime.usecs) ) {
            do {
				ptask->v_proctime.secs += 1;
			} while(ov_time_compare(&ptask->v_proctime, &time) <= 0);
    	}
    } else {
        if((ptask->v_cyctime.secs) || (ptask->v_cyctime.usecs)) {
			do {
				ov_time_add(&ptask->v_proctime, &ptask->v_proctime, &ptask->v_cyctime);
			} while(ov_time_compare(&ptask->v_proctime, &time) <= 0);
		} else {
            do {
				ptask->v_proctime.secs += 1;
			} while(ov_time_compare(&ptask->v_proctime, &time) <= 0);
		}
    }
	/*
	*	set new event time
	*/
	ov_scheduler_setabseventtime(pobj, &ptask->v_proctime);
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/
