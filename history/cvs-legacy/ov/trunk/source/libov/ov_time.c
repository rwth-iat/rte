/*
*   $Id: ov_time.c,v 1.2 1999-07-26 16:14:14 dirk Exp $
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
*	15-Oct-1998 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	13-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/

#define OV_COMPILE_LIBOV

#include "libov/ov_time.h"

#if OV_SYSTEM_UNIX
#include <sys/time.h>
#endif

#if OV_SYSTEM_NT || OV_SYSTEM_OPENVMS
#include <sys/timeb.h>
#endif

#if OV_SYSTEM_MC164
#include "mc164/time.h"
#endif

#if OV_SYSTEM_RMOS
#include "time.h"
#include "rmtypes.h"
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Global variables for emulating Unix time with milliseconds under RMOS
*	(Any volunteers for reimplementing this without using floating points?)
*/
#if OV_SYSTEM_RMOS
/*
*	wheter the system start time is initialized
*/
static OV_BOOL			time_initialized = FALSE;
/*
*	system start time in milliseconds since 01.01.1970 00:00:00 GMT
*/
static double starttime;
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Helper macro for converting RmAbsTimeStruct to double under RMOS
*	(Any volunteers for reimplementing this without using floating points?)
*/
#if OV_SYSTEM_RMOS
#define Ov_TimeToDouble(time)												\
	((((double)4294967296.0)*((double)time.hitime)+((double)time.lotime))	\
		/((double)1000.0))
#endif

/*	----------------------------------------------------------------------	*/

/*
*	get current system time
*/
void OV_DLLFNCEXPORT ov_time_gettime(
	OV_TIME	*ptime
) {
#if OV_SYSTEM_UNIX
	struct timeval t;
	gettimeofday(&t, NULL);
	ptime->secs = t.tv_sec;
	ptime->usecs = t.tv_usec;
#endif
#if OV_SYSTEM_NT || OV_SYSTEM_OPENVMS
	struct timeb t;
	ftime(&t);
	ptime->secs = t.time;
	ptime->usecs = t.millitm*1000;
#endif
#if OV_SYSTEM_MC164
    MC164_GETTIME(ptime);
#endif
#if OV_SYSTEM_RMOS
	RmAbsTimeStruct	abstime;
	double			now;
	if(!time_initialized) {
		RmGetAbsTime(&abstime);
		starttime = (0.5+(double)time(NULL))-Ov_TimeToDouble(abstime);
		time_initialized = TRUE;
	}
	RmGetAbsTime(&abstime);
	now = starttime+Ov_TimeToDouble(abstime);
	ptime->secs = (OV_UINT)now;
	ptime->usecs = (OV_UINT)((now-ptime->secs)*1000000.0);
#endif
}

/*	----------------------------------------------------------------------	*/

/*
*	Calculate the sum of a time and a time span
*/
void OV_DLLFNCEXPORT ov_time_add(
	OV_TIME				*psum,
	const OV_TIME		*padd1,
	const OV_TIME_SPAN	*padd2
) {
	psum->secs = padd1->secs + padd2->secs;
	psum->usecs = padd1->usecs + padd2->usecs;
	if((OV_INT)psum->usecs > 999999) {
		psum->usecs -= 1000000;
		psum->secs++;
		return;
	}
	if((OV_INT)psum->usecs < 0) {
		psum->usecs += 1000000;
		psum->secs--;
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	Caluculate the difference of two times
*/
void OV_DLLFNCEXPORT ov_time_diff(
	OV_TIME_SPAN		*pdiff,
	const OV_TIME		*psub1,
	const OV_TIME		*psub2
) {
	/*
	*	secs and usecs of the time difference may
	*	only temporarily have a different sign!
	*/
	pdiff->secs = psub1->secs - psub2->secs;
	pdiff->usecs = psub1->usecs - psub2->usecs;
	if((pdiff->secs > 0) && ((OV_INT)pdiff->usecs < 0)) {
		pdiff->usecs += 1000000;
		pdiff->secs--;
		return;
	}
	if((pdiff->secs < 0) && ((OV_INT)pdiff->usecs > 0)) {
		pdiff->usecs -= 1000000;
		pdiff->secs++;
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	Compare two times, result is -1, 0 or 1.
*/
OV_INT OV_DLLFNCEXPORT ov_time_compare(
	const OV_TIME		*ptime1,
	const OV_TIME		*ptime2
) {
	if(ptime1->secs > ptime2->secs) {
		return 1;
	}
	if(ptime1->secs < ptime2->secs) {
		return -1;
	}
	if(ptime1->usecs > ptime2->usecs) {
		return 1;
	}
	if(ptime1->usecs < ptime2->usecs) {
		return -1;
	}
	return 0;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

