#ifndef OV_COMPILE_LIBRARY_ksserv
#define OV_COMPILE_LIBRARY_ksserv
#endif


#include "ksserv.h"
#include "libov/ov_macros.h"
#include "libov/ov_scheduler.h"
#include "libov/ov_macros.h"
#include "ksserv_logfile.h"

void ksserv_RootComTask_execute(OV_INSTPTR_ov_object	pobj);

/**
 * Seconds for the ov_scheduler to call this RootComTask
 */
OV_DLLFNCEXPORT OV_INT ksserv_RootComTask_cycsecs_get(
    OV_INSTPTR_ksserv_RootComTask          pobj
) {
    return pobj->v_cycsecs;
}

/**
 * Seconds for the ov_scheduler to call this RootComTask
 */
OV_DLLFNCEXPORT OV_RESULT ksserv_RootComTask_cycsecs_set(
    OV_INSTPTR_ksserv_RootComTask          pobj,
    const OV_INT  value
) {
    pobj->v_cycsecs = value;
    return OV_ERR_OK;
}

/**
 * USeconds for the ov_scheduler to call this RootComTask
 */
OV_DLLFNCEXPORT OV_INT ksserv_RootComTask_cycusecs_get(
    OV_INSTPTR_ksserv_RootComTask          pobj
) {
    return pobj->v_cycusecs;
}

/**
 * USeconds for the ov_scheduler to call this RootComTask
 */
OV_DLLFNCEXPORT OV_RESULT ksserv_RootComTask_cycusecs_set(
    OV_INSTPTR_ksserv_RootComTask          pobj,
    const OV_INT  value
) {
    pobj->v_cycusecs = value;
    return OV_ERR_OK;
}

/**
 * Registers the Task initially at the ov_scheduler
 */
OV_DLLFNCEXPORT void ksserv_RootComTask_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	OV_TIME_SPAN		t;
	OV_INSTPTR_ksserv_RootComTask	rcTask;
	
	rcTask = Ov_StaticPtrCast(ksserv_RootComTask, pobj);

	t.secs = rcTask->v_cycsecs;
	t.usecs = rcTask->v_cycusecs;
	ov_scheduler_register(pobj, ksserv_RootComTask_execute);
	ov_scheduler_setreleventtime(pobj, &t);
	ksserv_logfile_debug("RootComTask registered at ov_scheduler with default intervall %d %d", t.secs, t.usecs);
	return;
}

/**
 * Deregisters the Task at the ov_scheduler
 */
OV_DLLFNCEXPORT void ksserv_RootComTask_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
	ov_scheduler_unregister((OV_INSTPTR_ov_object)pobj);
	ksserv_logfile_debug("RootComTask UNregistered at ov_scheduler");
	return;
}

/**
 * gets called by the ov_scheduler and iterates over all existing ComTask Objs
 * Reregisters itself at the ov_scheduler
 */
void ksserv_RootComTask_execute(
	OV_INSTPTR_ov_object	pobj
) {
	OV_TIME_SPAN		t;
        OV_INSTPTR_ksserv_RootComTask	rcTask;
	OV_INSTPTR_ksserv_ComTask 	childTask = NULL;
	OV_INSTPTR_ksserv_ComTask firstChild = NULL;
	OV_VTBLPTR_ksserv_ComTask	pvtable;
	OV_TIME time_next, now, earliestChild;
	OV_TIME_SPAN time_left, ts;
	rcTask = Ov_StaticPtrCast(ksserv_RootComTask, pobj);

	//get time_span until next event
	time_left = *(ov_scheduler_getnexteventtime());
	//if next event is too far in the future limit looping to 2 seconds
	if(time_left.secs > 1)
		time_left.secs = 1;
	//get current time
	ov_time_gettime(&now);
	//calculate time of next event
	ov_time_add(&time_next, &now, &time_left);


	do{//loop until next event in ov_scheduler

		//startvalue to estimate time until next (child-)event
		earliestChild = time_next;
		//lets see if something is todo...
		firstChild = Ov_GetFirstChild(ksserv_AssocComTaskList, rcTask);
		childTask = firstChild;
		while(childTask) {
			if(ksserv_ComTask_calcExec(childTask)) {//if TRUE, its time to execute this object
				//go via the methodtable to call the "real" implementation of the typemethod
				//ksserv_logfile_debug("RootComTask: %s was executed", childTask->v_identifier);
				Ov_GetVTablePtr(ksserv_ComTask, pvtable, childTask);
				pvtable->m_typemethod(childTask);
				//calculate and set next execution time of child task
				ov_time_gettime(&now);
				ts.secs = rcTask->v_cycsecs * childTask->v_cycInterval;
				ts.usecs = rcTask->v_cycusecs * childTask->v_cycInterval;
				if(ts.usecs >= 1000000)
				{
					ts.secs += (ts.usecs / 1000000);
					ts.usecs %= 1000000;
				}
				ov_time_add(&(childTask->v_NextExecTime), &(now), &ts);

				//get the earliest child task to be run again; just to estimate possible sleep time
				//sequence of child-task, however, remains
				if(ov_time_compare(&(childTask->v_NextExecTime), &(earliestChild)) < 0)
				{
					earliestChild = childTask->v_NextExecTime;

				}
			}


			childTask = Ov_GetNextChild(ksserv_AssocComTaskList, childTask);

		}

		ov_time_gettime(&now);
		ov_time_diff(&time_left, &earliestChild, &now);
		//sleep until next task is to be executed
		if((time_left.secs > 0) || ((time_left.secs == 0) && (time_left.usecs > 0)))
		{

		//	ksserv_logfile_debug("sleepin %d usecs", time_left.usecs);
#if !OV_SYSTEM_NT
		usleep(time_left.secs * 1000000 + time_left.usecs);
        // Not work on LINUX: select(0,  0,0,0,  &delay);
#else
        if( (time_left.secs == 0) && (time_left.usecs == 0) ) {
            /* Windows does not sleep iff 0 is param, but linux usleep drops timslot of thread */
            time_left.usecs = 1000;
        }
    	Sleep(time_left.secs * 1000 + time_left.usecs / 1000);
#endif
		}
	}while(ov_time_compare(&time_next, &now) > 0);


	//ksserv_logfile_debug("leaving loop");
	//get called again in a few moments
	t.secs = rcTask->v_cycsecs;
	t.usecs = rcTask->v_cycusecs;
	//ksserv_logfile_debug("RootComTask reschedule with intervall %d %d", t.secs, t.usecs);
        ov_scheduler_setreleventtime(pobj, &t);
        return;
}

