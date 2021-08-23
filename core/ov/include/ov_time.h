/*
*   $Id: ov_time.h,v 1.5 2006-01-12 14:10:13 markus Exp $
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
*	13-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

#ifndef OV_TIME_H_INCLUDED
#define OV_TIME_H_INCLUDED

#include "ov.h"
#include "ov_debug.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
*	OV_TIMECOMPARE:
*	---------------
*	Result of function ov_time_compare(t1, t2)
*/
#define	OV_TIMECMP_EQUAL	0		/* t1 = t2 */
#define	OV_TIMECMP_AFTER	1		/* t1 > t2 */
#define	OV_TIMECMP_BEFORE	(-1)	/* t1 < t2 */

/**
*	Get the current system time
*/
OV_DLLFNCEXPORT void ov_time_gettime(
	OV_TIME				*ptime
);

/**
*	Calculate the sum of a time and a time span
*/
OV_DLLFNCEXPORT void ov_time_add(
	OV_TIME				*psum,
	const OV_TIME		*padd1,
	const OV_TIME_SPAN	*padd2
);

/**
*	Caluculate the difference of two times
*/
OV_DLLFNCEXPORT void ov_time_diff(
	OV_TIME_SPAN		*pdiff,
	const OV_TIME		*psub1,
	const OV_TIME		*psub2
);

/*
*	Compare two times.
*	Please check against OV_TIMECMP_AFTER, OV_TIMECMP_EQUAL and OV_TIMECMP_BEFORE
*	OV_TIMECMP_AFTER occures if ptime1 has more seconds compared to ptime2
*/
OV_DLLFNCEXPORT OV_INT ov_time_compare(
	const OV_TIME		*ptime1,
	const OV_TIME		*ptime2
);

/**
*	Convert a time into an ASCII string (utc, backward compatibility function)
*	the format is YYYY/MM/DD hh:mm:ss.uuuuuu
*	no need to worry about memstack allocation. memory is from a static variable
*/
OV_DLLFNCEXPORT OV_STRING ov_time_timetoascii(
	const OV_TIME		*ptime
);

/**
*	Convert a time into an ASCII string (utc)
*	the format is YYYY/MM/DD hh:mm:ss.uuuuuu
*	no need to worry about memstack allocation. memory is from a static variable
*/
OV_DLLFNCEXPORT OV_STRING ov_time_timetoascii_utc(
	const OV_TIME		*ptime
);

/**
*	Convert a time into an ASCII string (local time)
*	the format is YYYY/MM/DD hh:mm:ss.uuuuuu
*	no need to worry about memstack allocation. memory is from a static variable
*/
OV_DLLFNCEXPORT OV_STRING ov_time_timetoascii_local(
	const OV_TIME		*ptime
);

/**
*	Convert a time span into an ASCII string
*	the format is hhhh:mm:ss.uuuuuu
*	no need to worry about memstack allocation. memory is from a static variable
*/
OV_DLLFNCEXPORT OV_STRING ov_time_timespantoascii(
             const OV_TIME_SPAN  *ptimespan
);

/**
*	Convert an ASCII string into a time (local time, backward compatibility function)
*	checks the format of the given string, which must be either
	YYYY/MM/DD, YYYY/MM/DD hh:mm:ss or YYYY/MM/DD hh:mm:ss.uuuuuu
*/
OV_DLLFNCEXPORT OV_RESULT ov_time_asciitotime(
	OV_TIME				*ptime,
	const OV_STRING		timestring
);

/**
*	Convert an ASCII string into a time (local time)
*	checks the format of the given string, which must be either
	YYYY/MM/DD, YYYY/MM/DD hh:mm:ss or YYYY/MM/DD hh:mm:ss.uuuuuu
*/
OV_DLLFNCEXPORT OV_RESULT ov_time_asciitotime_local(
	OV_TIME				*ptime,
	const OV_STRING		timestring
);

/**
*	Convert an ASCII string into a time (UTC)
*	Warning: this is NOTIMPLEMENTED on some uclibc compilers or platforms other than Windows and Unix!
*	checks the format of the given string, which must be either
	YYYY/MM/DD, YYYY/MM/DD hh:mm:ss or YYYY/MM/DD hh:mm:ss.uuuuuu
*/
OV_DLLFNCEXPORT OV_RESULT ov_time_asciitotime_utc(
	OV_TIME				*ptime,
	const OV_STRING		timestring
);

/**
 * convert a 64bit-int coded time value to an OV_TIME structure
 * assuming a resolution of 100ns starting at 01.01.1601
 * values outside the OV_TIME range are either 0.0 or MAXUINT.MAXUINT
 */
OV_DLLFNCEXPORT OV_TIME ov_1601nsTimeToOvTime(OV_INT64 time);

/**
 * convert an OV_TIME structure to a time value with 100ns resolution
 * starting at 01.01.1601 as a 64bit integer
 * smallest and largest possible OV_TIME values are converted to -MAXINT64 and MAXINT64 respectively
 */
OV_DLLFNCEXPORT OV_INT64 ov_ovTimeTo1601nsTime(OV_TIME time);

/**
 * convert a 64bit integer time with 100ns resolution to an OV_TIME_SPAN structure
 * values outside the OV_TIME_SPAN range are either -MAXINT.-MAXINT or MAXINT.MAXINT
 */
OV_DLLFNCEXPORT OV_TIME_SPAN ov_1601nsTimeToOvTimeSpan(OV_INT64 time);

/**
 * convert an OV_TIMS_SPAN structure to a 64bit integer coded time value with 100ns resolution
 * smallest and largest possible OV_TIME_SPAN values are converted to -MAXINT64 and MAXINT64 respectively
 */
OV_DLLFNCEXPORT OV_INT64 ov_ovTimeSpanTo1601nsTime(OV_TIME_SPAN time);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif
/*
*	End of file
*/

