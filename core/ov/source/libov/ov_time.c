/*
*   $Id: ov_time.c,v 1.10 2006-01-12 14:10:13 markus Exp $
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
#include "libov/ov_database.h"
#include "libov/ov_ov.h"

#if OV_SYSTEM_MC164
#include "mc164/time.h"
#else
//	Set _BSD_SOURCE for timegm
#define _BSD_SOURCE
#include <time.h>
#endif

#if OV_SYSTEM_UNIX
#include <sys/time.h>
#endif

#if OV_SYSTEM_NT
#include <sys/timeb.h>
#include "windows.h"	//for GetTimeZoneInformation
#endif

#if OV_SYSTEM_OPENVMS
#include <sys/timeb.h>
#endif

#if OV_SYSTEM_RMOS
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
OV_DLLFNCEXPORT void ov_time_gettime(
	OV_TIME			*ptime
) {
#if OV_SYSTEM_UNIX
	struct timeval t;
	gettimeofday(&t, NULL);
	ptime->secs = t.tv_sec;
	ptime->usecs = t.tv_usec;
#endif
#if OV_SYSTEM_OPENVMS
	struct timeb t;
	ftime(&t);
	ptime->secs = t.time;
	ptime->usecs = t.millitm*1000;
#endif
#if OV_SYSTEM_NT
#define TICKS_PER_SECOND 10000000
#define TICKS_PER_MICROSECOND 10
#define EPOCH_DIFFERENCE 11644473600LL
	FILETIME ft;	/*	64bit integer counting 100ns-ticks from 01.01.1601	*/
	ULARGE_INTEGER ticks;
	GetSystemTimeAsFileTime(&ft);
	ticks.LowPart = ft.dwLowDateTime;
	ticks.HighPart = ft.dwHighDateTime;
	ptime->secs = (OV_UINT) ((ticks.QuadPart / TICKS_PER_SECOND) - EPOCH_DIFFERENCE);
	ptime->usecs = (OV_UINT) ((ticks.QuadPart - (EPOCH_DIFFERENCE * TICKS_PER_SECOND)) - (ptime->secs * TICKS_PER_SECOND)) / TICKS_PER_MICROSECOND;
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
	if (pdb) ov_time_add(ptime, ptime, &pdb->timeoffset);
}

/*	----------------------------------------------------------------------	*/

/*
*	Calculate the sum of a time and a time span
*/
OV_DLLFNCEXPORT void ov_time_add(
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
OV_DLLFNCEXPORT void ov_time_diff(
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
OV_DLLFNCEXPORT OV_INT ov_time_compare(
	const OV_TIME		*ptime1,
	const OV_TIME		*ptime2
) {
	if(ptime1->secs > ptime2->secs) {
		return OV_TIMECMP_AFTER;
	}
	if(ptime1->secs < ptime2->secs) {
		return OV_TIMECMP_BEFORE;
	}
	if(ptime1->usecs > ptime2->usecs) {
		return OV_TIMECMP_AFTER;
	}
	if(ptime1->usecs < ptime2->usecs) {
		return OV_TIMECMP_BEFORE;
	}
	return OV_TIMECMP_EQUAL;
}

/*	----------------------------------------------------------------------	*/

/*
*	Convert a time into an ASCII string
*/
static OV_STRING ov_time_timetoascii_internal(
	const OV_TIME		*ptime,
	const OV_BOOL uselocaltime
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
	if(uselocaltime){
		//POSIX C
		ptm = localtime(&secs);
	}else{
		//POSIX C
		ptm = gmtime(&secs);
	}
	sprintf(timestring, "%04d/%02d/%02d %02d:%02d:%02d.%06" OV_PRINT_UINT,
		ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour,
		ptm->tm_min, ptm->tm_sec, ptime->usecs);
	return timestring;
}

/*
*	Convert a time into an ASCII string
*/
OV_DLLFNCEXPORT OV_STRING ov_time_timetoascii(
	const OV_TIME		*ptime
) {
	return ov_time_timetoascii_internal(ptime, FALSE);
}

/*
*	Convert a time into an ASCII string
*/
OV_DLLFNCEXPORT OV_STRING ov_time_timetoascii_utc(
	const OV_TIME		*ptime
) {
	return ov_time_timetoascii_internal(ptime, FALSE);
}

/*
*	Convert a time into an ASCII string
*/
OV_DLLFNCEXPORT OV_STRING ov_time_timetoascii_local(
	const OV_TIME		*ptime
) {
	return ov_time_timetoascii_internal(ptime, TRUE);
}


/*	----------------------------------------------------------------------	*/

/*
*	Convert a time span into an ASCII string
*/
OV_DLLFNCEXPORT OV_STRING ov_time_timespantoascii(
             const OV_TIME_SPAN  *ptimespan
){
	/*
	*	local variables
	*/
	static char 		timestring[]="hhhh:mm:ss.uuuuuu";
	unsigned int 		seconds, minutes, hours;
	time_t      		secs = ptimespan->secs;

	/*
	*	convert the time span to a string
	*/
	seconds = secs % 60;
	secs -= seconds;
	minutes = (secs % 3600)/60;
	secs -= minutes * 60;
	hours = secs / 3600;

	sprintf(timestring, "%04d:%02d:%02d.%06" OV_PRINT_UINT,
		hours, minutes, seconds, ptimespan->usecs);
	return timestring;
}

/*	----------------------------------------------------------------------	*/

/*
*	Convert an ASCII string into a time
*/
static OV_RESULT ov_time_asciitotime_internal(
	OV_TIME				*ptime,
	const OV_STRING		timestring,
	const OV_BOOL uselocaltime
) {
	/*
	*	local variables
	*/
	static OV_STRING	format = "0000/00/00 00:00:00.000000";
	struct tm			tm;
	time_t				secs;
	OV_UINT				usecs;
	OV_STRING			pc1, pc2;
	#if OV_SYSTEM_NT
	TIME_ZONE_INFORMATION tzinfo;
	#endif

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
	sscanf(timestring, "%d/%d/%d %d:%d:%d.%" OV_PRINT_UINT, &tm.tm_year, &tm.tm_mon,
		&tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec, &usecs);
	tm.tm_year -= 1900;
	tm.tm_mon--;
	if(uselocaltime){
		//POSIX C
		//time_t mktime (struct tm*);
		secs = mktime(&tm);
	}else{
		//POSIX forgot the UTC version of mktime
		#if OV_ARCH_NOMMU
			// uclibc does not always know timegm or similar (uclibc has the L_timegm define)
			return OV_ERR_NOTIMPLEMENTED;
		#else
			#if OV_SYSTEM_UNIX
				//unix has
				//time_t timegm(struct tm *);
				// Lars E.: According to timegm manpage: "These functions are nonstandard GNU extensions that are also present on the BSDs. Avoid their use"...
				//		"For a portable version of timegm(), set the TZ environment variable to UTC, call mktime(3) and restore the value of TZ."
				//	Feature Test Macro added.
				secs = timegm(&tm);
			#endif

			#if OV_SYSTEM_NT
				//POSIX forgot the utc version of mktime
				//Microsoft invented _mkgmtime (but this is not always available in minGW)
				#if OV_COMPILER_MSVC
					secs = _mkgmtime(&tm);
				#else
					//so we have to use the localtime version and calculate the offset by hand
					secs = mktime(&tm);

					// Obtain timezone information from operating system.
					memset(&tzinfo, 0, sizeof(tzinfo));
					if (GetTimeZoneInformation(&tzinfo) == TIME_ZONE_ID_INVALID) {
						// If we cannot get timezone information we fall back to Central European Time.
						tzinfo.Bias = -60;
						tzinfo.StandardDate.wMonth = 10;
						tzinfo.StandardDate.wDay = 5;
						tzinfo.StandardDate.wHour = 3;
						tzinfo.StandardBias = 0;
						tzinfo.DaylightDate.wMonth = 3;
						tzinfo.DaylightDate.wDay = 5;
						tzinfo.DaylightDate.wHour = 2;
						tzinfo.DaylightBias = -60;
					}
					//UTC = local time + bias
					if(tm.tm_isdst == 1){
						secs += (tzinfo.Bias + tzinfo.DaylightBias)*60;
					}else{
						secs += (tzinfo.Bias)*60;
					}
				#endif
			#endif

			#if OV_SYSTEM_UNIX + OV_SYSTEM_NT != 1
				#ifdef OV_DEBUG
					#pragma message "no timegm or _mkgmtime available on this platform"
				#endif
				return OV_ERR_NOTIMPLEMENTED;
			#endif
		#endif
	}
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

/*
*	Convert an ASCII string into a time (local)
*/
OV_DLLFNCEXPORT OV_RESULT ov_time_asciitotime_local(
	OV_TIME				*ptime,
	const OV_STRING		timestring
) {
	return ov_time_asciitotime_internal(ptime, timestring, TRUE);
}

/*
*	Convert an ASCII string into a time (UTC)
*	Warning: this is NOTIMPLEMENTED on some uclibc compilers or platforms other than Windows and Unix!
*/
OV_DLLFNCEXPORT OV_RESULT ov_time_asciitotime_utc(
	OV_TIME				*ptime,
	const OV_STRING		timestring
) {
	return ov_time_asciitotime_internal(ptime, timestring, FALSE);
}

/*
*	Convert an ASCII string into a time (local time, backward compatibility function)
*/
OV_DLLFNCEXPORT OV_RESULT ov_time_asciitotime(
	OV_TIME				*ptime,
	const OV_STRING		timestring
) {
	return ov_time_asciitotime_internal(ptime, timestring, TRUE);
}

/*	----------------------------------------------------------------------	*/
/*
 * conversion between ov_time and time values as 64bit integers with 100ns resolution starting at 01.01.1601
 */
#define EPOCHDIFFERENCE_SECONDS	11644473600LL
#define TOSECONDS				10000000LL
#define TOMICROSECONDS			10LL

OV_DLLFNCEXPORT OV_TIME ov_1601nsTimeToOvTime(OV_INT64 time){
	OV_TIME timeTemp;
	OV_INT64 tempInt64;
	tempInt64 = (time / TOSECONDS) - EPOCHDIFFERENCE_SECONDS;	/*	convert to seconds and subtract the difference in epoch	*/
	if((tempInt64 < 0) || (time < 0)){
		timeTemp.secs = 0;
		timeTemp.usecs = 0;
	} else if(tempInt64 > OV_VL_MAXUINT || time == OV_VL_MAXINT64){
		timeTemp.secs = OV_VL_MAXUINT;
		timeTemp.usecs = OV_VL_MAXUINT;
	} else {
		timeTemp.secs = tempInt64;
		timeTemp.usecs = (time - ((timeTemp.secs + EPOCHDIFFERENCE_SECONDS) * TOSECONDS)) / TOMICROSECONDS; /*	subtract the seconds-part and convert to microseconds	*/
	}
	return timeTemp;
}

OV_DLLFNCEXPORT OV_INT64 ov_ovTimeTo1601nsTime(OV_TIME time){
	OV_INT64 result;
	if((time.secs == 0) && (time.usecs == 0)){
		return 0;
	} else if((time.secs == OV_VL_MAXUINT) && (time.usecs >= 999999)){
		return OV_VL_MAXINT64;
	} else {
		result = (time.secs + EPOCHDIFFERENCE_SECONDS) * TOSECONDS + time.usecs * TOMICROSECONDS;
		return result;
	}
}

OV_DLLFNCEXPORT OV_TIME_SPAN ov_1601nsTimeToOvTimeSpan(OV_INT64 time){
	OV_TIME_SPAN timeTemp;
	OV_INT64 tempInt64;
	tempInt64 = (time / TOSECONDS);	/*	convert to seconds	*/
	if(tempInt64 < (-OV_VL_MAXINT)){
		timeTemp.secs = -OV_VL_MAXINT;
		timeTemp.usecs = -OV_VL_MAXINT;
	} else if(tempInt64 > OV_VL_MAXINT){
		timeTemp.secs = OV_VL_MAXINT;
		timeTemp.usecs = OV_VL_MAXINT;
	} else {
		timeTemp.secs = tempInt64;
		timeTemp.usecs = (time - (timeTemp.secs * TOSECONDS)) / TOMICROSECONDS; /*	subtract the seconds-part and convert to microseconds	*/
	}
	return timeTemp;
}

OV_DLLFNCEXPORT OV_INT64 ov_ovTimeSpanTo1601nsTime(OV_TIME_SPAN time){
	OV_INT64 result;
	if((time.secs == (-OV_VL_MAXINT)) && (time.usecs == (-OV_VL_MAXINT))){
		return 0;
	} else if((time.secs == OV_VL_MAXINT) && (time.usecs >= 999999)){
		return OV_VL_MAXINT64;
	} else {
		result = time.secs  * TOSECONDS + time.usecs * TOMICROSECONDS;
		return result;
	}
}

/*
*	End of file
*/

