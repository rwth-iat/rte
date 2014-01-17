/*
*   $Id: ov_time.h,v 1.5 2006-01-12 14:10:13 markus Exp $
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
*	13-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

#ifndef OV_TIME_H_INCLUDED
#define OV_TIME_H_INCLUDED

#include "ov.h"

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
#define	OV_TIMECMP_BEFORE	-1		/* t1 < t2 */

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
*/
OV_DLLFNCEXPORT OV_STRING ov_time_timetoascii(
	const OV_TIME		*ptime
);

/**
*	Convert a time into an ASCII string (utc)
*	the format is YYYY/MM/DD hh:mm:ss.uuuuuu
*/
OV_DLLFNCEXPORT OV_STRING ov_time_timetoascii_utc(
	const OV_TIME		*ptime
);

/**
*	Convert a time into an ASCII string (local time)
*	the format is YYYY/MM/DD hh:mm:ss.uuuuuu
*/
OV_DLLFNCEXPORT OV_STRING ov_time_timetoascii_localitme(
	const OV_TIME		*ptime
);

/**
*	Convert a time span into an ASCII string
*	the format is hhhh:mm:ss.uuuuuu
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
*	Warning: this is NOTIMPLEMENTED under windows!
*	checks the format of the given string, which must be either
	YYYY/MM/DD, YYYY/MM/DD hh:mm:ss or YYYY/MM/DD hh:mm:ss.uuuuuu
*/
OV_DLLFNCEXPORT OV_RESULT ov_time_asciitotime_utc(
	OV_TIME				*ptime,
	const OV_STRING		timestring
);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif
/*
*	End of file
*/

