/*
*   $Id: ov_scheduler.c,v 1.2 1999-08-25 13:15:57 dirk Exp $
*
*   Copyright (C) 1998-1999
*   Lehrstuhl fuer Prozessleittechnik,
*   RWTH Aachen, D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
*
*   This file is part of the ACPLT/OV Package which is licensed as open
*   source under the Artistic License; you can use, redistribute and/or
*   modify it under the terms of that license.
*
*   You should have received a copy of the Artistic License along with
*   this Package; see the file ARTISTIC-LICENSE. If not, write to the
*   Copyright Holder.
*
*   THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
*   WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
*   OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/
/*
*	History:
*	--------
*	03-Mrz-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	13-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/

#define OV_COMPILE_LIBOV

#include "libov/ov_scheduler.h"
#include "libov/ov_time.h"
#include "libov/ov_macros.h"

/*	----------------------------------------------------------------------	*/

/*
*	global variables
*/
static OV_SCHEDULER_EVENT	*pfirstevent = NULL;

/*	----------------------------------------------------------------------	*/

/*
*	Register an active object with the scheduler
*/
OV_RESULT OV_DLLFNCEXPORT ov_scheduler_register(
	OV_INSTPTR_ov_object	pobj,
	OV_FNC_EXECUTE			*executefnc
) {
	/*
	*	local variables
	*/
	OV_SCHEDULER_EVENT	*pevent;
	/*
	*	check parameters
	*/
	if(!pobj || !executefnc) {
		return OV_ERR_BADPARAM;
	}
	/*
	*	create a new event object
	*/
	pevent = Ov_HeapAlloc(OV_SCHEDULER_EVENT);
	if(!pevent) {
		return OV_ERR_HEAPOUTOFMEMORY;
	}
	/*
	*	initialize the event object
	*/
	pevent->pnext = NULL;
	pevent->pobj = pobj;
	pevent->executefnc = executefnc;
	ov_time_gettime(&pevent->time);
	/*
	*	insert the event object into the event queue
	*/
	ov_scheduler_insertevent(pevent);
	/*
	*	set the object state to "active"
	*/
	pobj->v_objectstate |= OV_OS_ACTIVE;
	/*
	*	finished.
	*/
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Unregister an active object with the scheduler
*/
void OV_DLLFNCEXPORT ov_scheduler_unregister(
	OV_INSTPTR_ov_object	pobj
) {
	/*
	*	local variables
	*/
	OV_SCHEDULER_EVENT	*pcurr, *plast;
	/*
	*	find the event to unregister and always remember the last event
	*/
	plast = NULL;
	for(pcurr=pfirstevent; pcurr; pcurr=pcurr->pnext) {
		if(pcurr->pobj == pobj) {
			/*
			*	remove the event object from the queue
			*/
			if(plast) {
				plast->pnext = pcurr->pnext;
			} else {
				pfirstevent = pcurr->pnext;
			}
			/*
			*	delete the event object
			*/
			Ov_HeapFree(pcurr);
			/*
			*	set the object state to "inactive"
			*/
			pobj->v_objectstate &= ~OV_OS_ACTIVE;
			return;
		}
		plast = pcurr;
	}
	/*
	*	the event object was NOT found. Nevertheless, return.
	*/
	return;
}

/*	----------------------------------------------------------------------	*/

/*
*	Set absolute event time of a registered active object
*/
void OV_DLLFNCEXPORT ov_scheduler_setabseventtime(
	OV_INSTPTR_ov_object	pobj,
	OV_TIME					*ptime
) {
	/*
	*	local variables
	*/
	OV_SCHEDULER_EVENT	*pcurr, *plast;
	OV_TIME				time;
	/*
	*	check parameters
	*/
	if(!pobj) {
		return;
	}
	if(ptime) {
		time = *ptime;
	} else {
		ov_time_gettime(&time);
	}
	/*
	*	find the event object and always remember the last event
	*/
	plast = NULL;
	for(pcurr=pfirstevent; pcurr; pcurr=pcurr->pnext) {
		if(pcurr->pobj == pobj) {
			/* we found the object, set the event time */
			pcurr->time = time;
			/* remove the event object from the queue */
			if(plast) {
				plast->pnext = pcurr->pnext;
			} else {
				pfirstevent = pcurr->pnext;
			}
			/* reinsert the event object into the queue */
			ov_scheduler_insertevent(pcurr);
			return;
		}
		plast = pcurr;
	}
	/*
	*	the event object was NOT found. Nevertheless, return.
	*/
	return;
}

/*	----------------------------------------------------------------------	*/

/*
*	Set relative event time of a registered active object (time span from now on)
*/
void OV_DLLFNCEXPORT ov_scheduler_setreleventtime(
	OV_INSTPTR_ov_object	pobj,
	OV_TIME_SPAN			*ptimespan
) {
	/*
	*	local variables
	*/
	OV_TIME	time;
	/*
	*	check parameters
	*/
	if(!pobj) {
		return;
	}
	/*
	*	get current time
	*/
	ov_time_gettime(&time);
	/*
	*	add time span
	*/
	if(ptimespan) {
		ov_time_add(&time, &time, ptimespan);
	}
	ov_scheduler_setabseventtime(pobj, &time);
}

/*	----------------------------------------------------------------------	*/

/*
*	Schedule the pnext event of the event queue if the event is pending
*/
OV_TIME_SPAN* OV_DLLFNCEXPORT ov_scheduler_schedulenextevent(void) {
	/*
	*	local variables
	*/
	OV_TIME				time;
	static OV_TIME_SPAN	timespan;
	OV_SCHEDULER_EVENT	*pevent = pfirstevent;
	/*
	*	get current time
	*/
	ov_time_gettime(&time);
	/*
	*	test if it's time for the first scheduled active object
	*/
	if(pevent) {
		if(ov_time_compare(&time, &pevent->time) > 0) {
			/* update the scheduled time of the pnext scheduled active object */
			pevent->time = time;
			/* remove the object from the beginning of the queue... */
			pfirstevent = pevent->pnext;
			/* ...and reinsert it at the appropiate place */
			ov_scheduler_insertevent(pevent);
			/* Call method of the pnext scheduled active object */
			(pevent->executefnc)(pevent->pobj);
			/* update the current time (yes, sounds paradox...) */
			ov_time_gettime(&time);
		}
	}
	/*
	*	calculate time span until pnext event
	*/
	if(pfirstevent) {
		/* substract the time of the pnext event from the current time */
		ov_time_diff(&timespan, &pfirstevent->time, &time);
		if((timespan.secs < 0) || (timespan.usecs < 0)) {
			timespan.secs = 0;
			timespan.usecs = 0;
		}
	} else {
		timespan.secs = OV_VL_MAXINT;
		timespan.usecs = 0;
	}
	return &timespan;
}

/*	----------------------------------------------------------------------	*/

/*
*	Insert an event into the event queue (subroutine)
*/
void ov_scheduler_insertevent(
	OV_SCHEDULER_EVENT	*pevent
) {
	/*
	*	local variables
	*/
	OV_SCHEDULER_EVENT	*plast, *pcurr;
	/*
	*	find the right place and always remember the last event
	*/
	pevent->pnext = NULL;
	plast = NULL;
	for(pcurr=pfirstevent; pcurr; pcurr=pcurr->pnext) {
		if(ov_time_compare(&pcurr->time, &pevent->time) > 0) {
			pevent->pnext = pcurr;
			break;
		}
		plast = pcurr;
	}
	/*
	*	insert the event object
	*/
	if(plast) {
		plast->pnext = pevent;
	} else {
		pfirstevent = pevent;
	}
	return;	
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

