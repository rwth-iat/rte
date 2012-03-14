/*
*   ComTask 
*   Abstract Class - need to be inherited by communication system components, which provides a typemethod.
*   
*/

#ifndef OV_COMPILE_LIBRARY_ksserv
#define OV_COMPILE_LIBRARY_ksserv
#endif


#include "ksserv.h"
#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include "ksserv_logfile.h"

/**
 * enable(1) / disable(0) this task
 */
OV_DLLFNCEXPORT OV_INT ksserv_ComTask_actimode_get(
    OV_INSTPTR_ksserv_ComTask          pobj
) {
    return pobj->v_actimode;
}

/**
 * enable(1) / disable(0) this task
 */
OV_DLLFNCEXPORT OV_RESULT ksserv_ComTask_actimode_set(
    OV_INSTPTR_ksserv_ComTask          pobj,
    const OV_INT  value
) {
    pobj->v_actimode = value;
    return OV_ERR_OK;
}

/**
 * internal variable for counted down by calcExec till next execution
 */

OV_DLLFNCEXPORT OV_INT ksserv_ComTask_cycIntervalCount_get(
    OV_INSTPTR_ksserv_ComTask          pobj
) {
    return pobj->v_cycIntervalCount;
}

/**
 * variable every how many cycles of RootComTask this should be executed
 */
OV_DLLFNCEXPORT OV_INT ksserv_ComTask_cycInterval_get(
    OV_INSTPTR_ksserv_ComTask          pobj
) {
    return pobj->v_cycInterval;
}

/**
 * variable every how many cycles of RootComTask this should be executed
 */
OV_DLLFNCEXPORT OV_RESULT ksserv_ComTask_cycInterval_set(
    OV_INSTPTR_ksserv_ComTask          pobj,
    const OV_INT  value
) {
    pobj->v_cycInterval = value;
    return OV_ERR_OK;
}

/* 
 * Implementing classes need to call this!
 * It links the instance to the RootComTask, thus the task is recognized to be executed.
 */ 
OV_DLLFNCEXPORT OV_RESULT ksserv_ComTask_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
	
	OV_INSTPTR_ksserv_RootComTask 	rcTask = NULL;
	OV_INSTPTR_ksserv_ComTask 	cTask = NULL;
	OV_TIME_SPAN ts;
	OV_TIME t;
	ov_object_constructor(pobj);

	cTask = Ov_StaticPtrCast(ksserv_ComTask, pobj);
	rcTask = (OV_INSTPTR_ksserv_RootComTask)ov_path_getobjectpointer("/ComTasks", 2);
	//link for getting called
	Ov_Link(ksserv_AssocComTaskList,rcTask,cTask);
	//ksserv_logfile_debug("Registered %s at ComTasks", pobj->v_identifier);
	//set time for next execution
	ov_time_gettime(&t);
	ts.secs = rcTask->v_cycsecs * cTask->v_cycInterval;
	ts.usecs = rcTask->v_cycusecs * cTask->v_cycInterval;
	if(ts.usecs >= 1000000)
	{
		ts.secs += (ts.usecs / 1000000);
		ts.usecs %= 1000000;
	}
	ov_time_add(&(cTask->v_NextExecTime), &(t), &ts);
    return (OV_RESULT) 0;
}

/**
 * Called while deconstruction, deleted the Assoc between the implementation task
 * and the RootComTask
 */
OV_DLLFNCEXPORT void ksserv_ComTask_destructor(
	OV_INSTPTR_ov_object 	pobj
) {
	OV_INSTPTR_ksserv_RootComTask 	rcTask = NULL;
	OV_INSTPTR_ksserv_ComTask 	cTask = NULL;

	cTask = Ov_StaticPtrCast(ksserv_ComTask, pobj);
	rcTask = (OV_INSTPTR_ksserv_RootComTask)ov_path_getobjectpointer("/ComTasks", 2);
	//unlink
	Ov_Unlink(ksserv_AssocComTaskList,rcTask,cTask);
        return;
}

/**
 * Called by RootComTask
 * Checks if its time to execute typemethod and counts down the cycIntervalCount otherwise.
 */
OV_DLLFNCEXPORT OV_BOOL ksserv_ComTask_calcExec(
	     OV_INSTPTR_ksserv_ComTask	cTask
) {

	OV_INSTPTR_ksserv_RootComTask rcTask;
	OV_TIME now;

	//ksserv_logfile_debug("ComTask: %s , cyc %d ", cTask->v_identifier, cTask->v_cycInterval);
	rcTask = (OV_INSTPTR_ksserv_RootComTask)ov_path_getobjectpointer("/ComTasks", 2);

	if(cTask->v_actimode == 0) return FALSE; //disabled ComTask

	ov_time_gettime(&now);
	if(ov_time_compare(&(cTask->v_NextExecTime), &now) <= 0) {//its time to get called


		return TRUE;
	} 
	return FALSE;	
}

/**
 * Will be called by calcExec, if it is time
 * Should be overwritten by implementing class (is called by methodtable)
 */
OV_DLLFNCEXPORT void ksserv_ComTask_typemethod(
	     OV_INSTPTR_ksserv_ComTask	cTask
) {
	ksserv_logfile_error("This ComTask hasnt a typemethod - but it should!!!");
	return;	
}
