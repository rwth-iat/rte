/*
*   $Id: ov_time.c,v 1.3 1999-08-18 13:11:27 dirk Exp $
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

#if OV_SYSTEM_MC164
#define memset xmemset
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Global variables for emulating Unix time with milliseconds under RMOS
*	(Any volunteers for reimplementing this without using floating points?)
*/
#if OV_SYSTEM_RMOS
/*
*	whether the system start time is initialized
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
*	Get the current system time
*/
void OV_DLLFNCEXPORT ov_time_gettime(
	OV_TIME			*ptime
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
*	Convert a time into an ASCII string
*/
OV_STRING OV_DLLFNCEXPORT ov_time_timetoascii(
	const OV_TIME		*ptime
) {
	/*
	*	local variables
	*/
	static char		timestring[] = "YYYY/MM/DD hh:mm:ss.uuuuuu";
	struct tm		*ptm;
	time_t			secs = ptime->secs;
	/*
	*	convert the time to a string
	*/
	ptm = gmtime(&secs);
	sprintf(timestring, "%04d/%02d/%02d %02d:%02d:%02d.%06lu",
		ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour,
		ptm->tm_min, ptm->tm_sec, ptime->usecs);
	return timestring;
}

/*	----------------------------------------------------------------------	*/

/*
*	Convert an ASCII string into a time
*/
OV_RESULT OV_DLLFNCEXPORT ov_time_asciitotime(
	OV_TIME				*ptime,
	const OV_STRING		timestring
) {
	/*
	*	local variables
	*/
	static OV_STRING	format = "0000/00/00 00:00:00.000000";
	struct tm			tm;
	time_t				secs;
	OV_UINT				usecs;
	OV_STRING			pc1, pc2;
	/*
	*	check the format of the given string, which must be either
	*	YYYY/MM/DD, YYYY/MM/DD hh:mm:ss or YYYY/MM/DD hh:mm:ss.uuuuuu
	*/
	if(!timestring) {
		return OV_ERR_BADPARAM;
	}
	for(pc1=format, pc2=timestring; *pc1; pc1++, pc2++) {
		if(!*pc2) {
			if((*pc1 == ' ') || (*pc1 == '.')) {
				break;
			}
			return OV_ERR_BADPARAM;
		}
		switch(*pc1) {
			case '0':
				if(!((*pc2 >= '0') && (*pc2 <= '9'))) {
					return OV_ERR_BADPARAM;
				}
				break;
			case '/':
			case ':':
			case ' ':
			case '.':
				if(*pc2 != *pc1) {
					return OV_ERR_BADPARAM;
				}
				break;
			default:
				Ov_Error("internal error");
		}
	}
	/*
	*	convert the string to a time
	*/
	memset(&tm, 0, sizeof(tm));
	usecs = 0;
	sscanf(timestring, "%d/%d/%d %d:%d:%d.%lu", &tm.tm_year, &tm.tm_mon,
		&tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec, &usecs);
	tm.tm_year -= 1900;
	tm.tm_mon--;
	secs = mktime(&tm);
	if(secs == -1) {
		return OV_ERR_BADPARAM;
	}
	/*
	*	time is OK, return value
	*/
	ptime->secs = secs;
	ptime->usecs = usecs;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

