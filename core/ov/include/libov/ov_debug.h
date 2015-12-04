/*
*   $Id: ov_debug.h,v 1.3 1999-09-15 10:48:14 dirk Exp $
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
*	15-Jan-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	08-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/

#ifndef OV_DEBUG_H_INCLUDED
#define OV_DEBUG_H_INCLUDED

#include "libov/ov_ov.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
*	Return filename from a path
*/
OV_DLLFNCEXPORT OV_STRING ov_debug_filename(
	OV_STRING	path
);

#ifdef OV_DEBUG

#include "libov/ov_logfile.h"

#include <stdlib.h>
#include <stdio.h>

/*
*	If we are debugging, print information
*/
#define Ov_Info(info) \
	ov_logfile_debug("%s:%d: Info: " info, ov_debug_filename(__FILE__), __LINE__);

/*
*	If we are debugging, print a warning and continue.
*/
#define Ov_Warning(warning) \
	ov_logfile_debug("%s:%d: Warning: " warning, ov_debug_filename(__FILE__), __LINE__);

/*
*	If we are debugging, print an error and abort.
*/
#define Ov_Error(err) \
	ov_logfile_debug("%s:%d: Error: " err, ov_debug_filename(__FILE__), __LINE__);	\
	ov_logfile_close();															\
	exit(EXIT_FAILURE)

#else	/* OV_DEBUG */

/*
*	If we are not debugging, do nothing.
*/
#define Ov_Info(warning)			((void)0)
#define Ov_Warning(warning)			((void)0)
#define Ov_Error(err)				((void)0)

#endif	/* OV_DEBUG */

/*
*	If we are debugging, print a warning if a given condition holds and continue
*/
#define Ov_WarnIf(condition) \
	if(condition) { Ov_Warning("Assertion failed: \"!(" #condition ")\"."); }

/*
*	If we are debugging, print a warning if a given condition does not hold and continue
*/
#define Ov_WarnIfNot(condition) \
	if(!(condition)) { Ov_Warning("Assertion failed: \"" #condition "\"."); }

/*
*	If we are debugging, print an error if a given condition holds and continue
*/
#define Ov_AbortIf(condition) \
	if(condition) { Ov_Error("Assertion failed: \"!(" #condition ")\"."); }

/*
*	If we are debugging, print an error if a given condition does not hold and continue
*/
#define Ov_AbortIfNot(condition) \
	if(!(condition)) { Ov_Error("Assertion failed: \"" #condition "\"."); }

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif

/*
*	End of file
*/

