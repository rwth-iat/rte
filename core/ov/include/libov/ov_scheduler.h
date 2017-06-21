/*
*   $Id: ov_scheduler.h,v 1.2 1999-09-15 10:48:15 dirk Exp $
*
*   Copyright (C) 1998
*   Lehrstuhl fuer Prozessleittechnik,
*   D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
*
*   This file is part of the ACPLT/OV Package 
*   
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
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
typedef void OV_FNC_EXECUTE(
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
OV_DLLFNCEXPORT OV_RESULT ov_scheduler_register(
	OV_INSTPTR_ov_object	pobj,
	OV_FNC_EXECUTE			*executefnc
);

/*
*	Unregister an active object with the scheduler
*/
OV_DLLFNCEXPORT void ov_scheduler_unregister(
	OV_INSTPTR_ov_object	pobj
);

/*
*	Set absolute event time of a registered active object
*/
OV_DLLFNCEXPORT void ov_scheduler_setabseventtime(
	OV_INSTPTR_ov_object	pobj,
	OV_TIME					*ptime
);

/*
*	Set relative event time of a registered active object (time span from now on)
*/
OV_DLLFNCEXPORT void ov_scheduler_setreleventtime(
	OV_INSTPTR_ov_object	pobj,
	OV_TIME_SPAN			*ptimespan
);

/*
*	Schedule the next event of the event queue if the event is pending
*/
OV_DLLFNCEXPORT OV_TIME_SPAN *ov_scheduler_schedulenextevent(void);


/*
*	Get execution time of the pnext event of the event queue
*/
OV_DLLFNCEXPORT OV_TIME_SPAN *ov_scheduler_getnexteventtime(void);

/*
*	Insert an event into the event queue (subroutine)
*/
void ov_scheduler_insertevent(
	OV_SCHEDULER_EVENT		*pevent
);

/**
*	Execute the first event in the queue that matches concerning identifier, library and class
*	empty strings / NULL-pointers are treated as a wildcard
*	returns TRUE after execution
*	returns FALSE if there was no matching event found
*/
OV_DLLFNCEXPORT OV_BOOL ov_scheduler_execnamedeventonce(
		OV_STRING ident,
		OV_STRING class,
		OV_STRING lib
);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif
/*
*	End of file
*/

