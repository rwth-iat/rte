/*
*   $Id: ov_time.h,v 1.2 1999-08-18 13:11:23 dirk Exp $
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
*	Get the current system time
*/
void OV_DLLFNCEXPORT ov_time_gettime(
	OV_TIME				*ptime
);

/*
*	Calculate the sum of a time and a time span
*/
void OV_DLLFNCEXPORT ov_time_add(
	OV_TIME				*psum,
	const OV_TIME		*padd1,
	const OV_TIME_SPAN	*padd2
);

/*
*	Caluculate the difference of two times
*/
void OV_DLLFNCEXPORT ov_time_diff(
	OV_TIME_SPAN		*pdiff,
	const OV_TIME		*psub1,
	const OV_TIME		*psub2
);

/*
*	Compare two times, result is -1, 0 or 1.
*/
OV_INT OV_DLLFNCEXPORT ov_time_compare(
	const OV_TIME		*ptime1,
	const OV_TIME		*ptime2
);

/*
*	Convert a time into an ASCII string
*/
OV_STRING OV_DLLFNCEXPORT ov_time_timetoascii(
	const OV_TIME		*ptime
);

/*
*	Convert an ASCII string into a time
*/
OV_RESULT OV_DLLFNCEXPORT ov_time_asciitotime(
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

