/*
*   $Id: ov_scheduler.h,v 1.1 1999-07-19 15:02:04 dirk Exp $
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

#ifndef OV_SCHEDULER_H_INCLUDED
#define OV_SCHEDULER_H_INCLUDED

#include "ov.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
*	OV_FNC_EXECUTE:
*	---------------
*	Function prototype for methods used in active objects
*/
typedef void OV_DLLFNCEXPORT OV_FNC_EXECUTE(
	OV_INSTPTR_ov_object 	pobj
);

/*
*	OV_SCHEDULER_EVENT:
*	-------------------
*	Event in a simple event queue, ordered by time
*/
struct OV_SCHEDULER_EVENT {
	struct OV_SCHEDULER_EVENT	*pnext;			/* Pointer to next scheduled event */
	OV_INSTPTR_ov_object		pobj;			/* Pointer to scheduled active object */
	OV_FNC_EXECUTE				*executefnc;	/* method to call on active object */
	OV_TIME						time;			/* time of this scheduled event */
};	
typedef struct OV_SCHEDULER_EVENT OV_SCHEDULER_EVENT; 

/*
*	Register an active object with the scheduler
*/
OV_RESULT OV_DLLFNCEXPORT ov_scheduler_register(
	OV_INSTPTR_ov_object	pobj,
	OV_FNC_EXECUTE			*executefnc
);

/*
*	Unregister an active object with the scheduler
*/
void OV_DLLFNCEXPORT ov_scheduler_unregister(
	OV_INSTPTR_ov_object	pobj
);

/*
*	Set absolute event time of a registered active object
*/
void OV_DLLFNCEXPORT ov_scheduler_setabseventtime(
	OV_INSTPTR_ov_object	pobj,
	OV_TIME					*ptime
);

/*
*	Set relative event time of a registered active object (time span from now on)
*/
void OV_DLLFNCEXPORT ov_scheduler_setreleventtime(
	OV_INSTPTR_ov_object	pobj,
	OV_TIME_SPAN			*ptimespan
);

/*
*	Schedule the next event of the event queue if the event is pending
*/
OV_TIME_SPAN* OV_DLLFNCEXPORT ov_scheduler_schedulenextevent(void);

/*
*	Insert an event into the event queue (subroutine)
*/
void ov_scheduler_insertevent(
	OV_SCHEDULER_EVENT		*pevent
);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif
/*
*	End of file
*/

