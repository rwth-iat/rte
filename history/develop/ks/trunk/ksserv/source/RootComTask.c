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
	OV_VTBLPTR_ksserv_ComTask	pvtable;

	rcTask = Ov_StaticPtrCast(ksserv_RootComTask, pobj);
	//lets see if something is todo...
	childTask = Ov_GetFirstChild(ksserv_AssocComTaskList, rcTask);
	while(childTask) {
		if(ksserv_ComTask_calcExec(childTask)) {//if TRUE, its time to execute this object
			//go via the methodtable to call the "real" implementation of the typemethod
			//ksserv_logfile_debug("RootComTask: %s was executed", childTask->v_identifier);
			Ov_GetVTablePtr(ksserv_ComTask, pvtable, childTask);
			pvtable->m_typemethod(childTask);
		}
		childTask = Ov_GetNextChild(ksserv_AssocComTaskList, childTask);
	}
   	//get called again in a few moments
	t.secs = rcTask->v_cycsecs;
	t.usecs = rcTask->v_cycusecs;
	//ksserv_logfile_debug("RootComTask reschedule with intervall %d %d", t.secs, t.usecs);
        ov_scheduler_setreleventtime(pobj, &t);
        return;
}

