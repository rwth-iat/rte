/******************************************************************************
***                                                                         ***
***   iFBSpro   -   Funktionsbaustein-Model                                 ***
***   #####################################                                 ***
***                                                                         ***
***   L T S o f t                                                           ***
***   Agentur f�r Leittechnik Software GmbH                                 ***
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

/**
*	Set init proctime, to stay in the old cycle
*	called on task reactivation and db startup
*/
void fb_set_proctime(
	OV_INSTPTR_fb_task	ptask
) {
    
    OV_DOUBLE 			nowDouble,proctimeDouble,cyctimeDouble;
    OV_TIME				nowTime;
    OV_TIME_SPAN		procTimeDiff;
    OV_INT			cycMisses;

    ov_time_gettime(&nowTime);

    Ov_TimeToDouble(nowTime, nowDouble);
    Ov_TimeToDouble(ptask->v_proctime, proctimeDouble);

    if(proctimeDouble < nowDouble) {
        //the next execution time is in the past
        Ov_TimeSpanToDouble(ptask->v_cyctime, cyctimeDouble);
        if((proctimeDouble >= 0) && (cyctimeDouble > 0)) {
            cycMisses = (OV_INT)( (nowDouble - proctimeDouble)/cyctimeDouble );
            cyctimeDouble *= cycMisses;
            cycMisses = (OV_INT)cyctimeDouble;
            procTimeDiff.secs = cycMisses;
            procTimeDiff.usecs= (OV_INT)( (cyctimeDouble-cycMisses)*1000000 );
            ov_time_add(&ptask->v_proctime, &ptask->v_proctime, &procTimeDiff);
        } else {
            //set to now, to call this as soon as possible
            ptask->v_proctime = nowTime;
        }
    }
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
					//make sure the next proctime is in the future
					ov_time_add(&ptask->v_proctime, &ptask->v_proctime, &ptask->v_cyctime);
				} while(ov_time_compare(&ptask->v_proctime, pltc) == OV_TIMECMP_BEFORE);
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
			ptask->v_actimode = FB_AM_OFF;
			break;
	}
	return;
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
            /* no break */
        case FB_AM_UNLINK:
        case FB_AM_ONCE:
        case FB_AM_CATCHUP:
                ptask->v_ErrState = 0;
                /* no break */
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

	result = fb_object_constructor(pobj);
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
	
    if(ptask->v_actimode == FB_AM_ON) {
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
	OV_ACCESS           access;
	
	/*
	*	switch based on the element's type
	*/
	switch(pelem->elemtype) {
		case OV_ET_OBJECT:
			/* Default access */
			access = (OV_AC_READWRITE | OV_AC_LINKABLE );

			/*
			 *	test if we are the "urtask"
			 */
			if(!fb_task_is_urtask(ptask)) {
				access |= OV_AC_RENAMEABLE;
			}

			/* Task als Unterobjekt? */
			ptaskchild = Ov_GetParent(fb_tasklist, ptask);
			if(ptaskchild) {
				access |= OV_AC_UNLINKABLE;
			}

			/* Gibt es Task-Unterobjekte ? */
			ptaskchild = Ov_GetFirstChild(fb_tasklist, ptask);
			if(ptaskchild) {
				access |= OV_AC_UNLINKABLE;
			}
			access &= fb_object_getaccess(pobj, pelem, pticket);

			//OV does not allow deletion if there are links
			//all tasks have a task parent link, so fb_object_getaccess probably forbid deletion
			//override this as we have no easy way checking for the type of a delete preventing link 8-/

			/* Ist Task eingeschaltet, darf Task nicht geloescht werden */
			if(ptask->v_actimode == FB_AM_OFF) {
				//only inactive tasks are deletable

				if(!ptaskchild) {
					/* Keine Unterobjekte. Dann ist Task loeschbar. Auch UrTask */
					access |= OV_AC_DELETEABLE;
				}
			}

			return access;
		default:
			break;
	}
	return fb_object_getaccess(pobj, pelem, pticket);
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
	OV_INSTPTR_fb_task	pnexttaskchild;
	OV_VTBLPTR_fb_task	pvtable;

    FbSvcLog_printexecitem((OV_INSTPTR_ov_object)ptask, "executing child objects");

	/*
	*	execute the child objects
	*/
	ptaskchild=Ov_GetFirstChild(fb_tasklist, ptask);
	while(ptaskchild) {
		pnexttaskchild=Ov_GetNextChild(fb_tasklist, ptaskchild);
		Ov_GetVTablePtr(fb_task, pvtable, ptaskchild);
    
		if(!pvtable) {

			ptaskchild->v_actimode = FB_AM_OFF;	/* FIXME! */
			ptaskchild->v_ErrState = 1;	/* FIXME! */
			
			/* Logging */
            FbSvcLog_printexecitem((OV_INSTPTR_ov_object)ptaskchild, "ERROR: method table not found");
			ov_logfile_error(" %s: method table not found.", ptaskchild->v_identifier);
			
		} else {
    		pvtable->m_execute(ptaskchild, pltc);
		}
		ptaskchild = pnexttaskchild;
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
	if((ov_time_compare(&ptask->v_proctime, pltc) == OV_TIMECMP_AFTER) || (ptask->v_actimode == FB_AM_OFF)) {
		return;
	}

    ov_time_gettime(&t0);

    /* Logging */
    FbSvcLog_incrIndent();

	/* trigger output send connections */
    fb_object_triggerInpGetConnections(Ov_PtrUpCast(fb_object, ptask));
    
	/* execute the child objects */
    fb_task_execChildObjects(ptask, pltc);

	/* trigger output send connections */
	fb_object_triggerOutSendConnections(Ov_PtrUpCast(fb_object, ptask));
	
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
			ptask->v_proctime.secs = time.secs + 1;
    	}
    } else {
        if((ptask->v_cyctime.secs) || (ptask->v_cyctime.usecs)) {
			do {
				ov_time_add(&ptask->v_proctime, &ptask->v_proctime, &ptask->v_cyctime);
			} while(ov_time_compare(&ptask->v_proctime, &time) <= 0); //OV_TIMECMP_EQUAL or BEFORE
		} else {
			ptask->v_proctime.secs = time.secs + 1;
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
