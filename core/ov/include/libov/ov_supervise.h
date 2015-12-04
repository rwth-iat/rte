/*
*   $Id: ov_supervise.h,v 1.3 2011-07-15 13:36:46 gustavo Exp $
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
*	16-Dec-1998 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	13-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/
/*
*	Description:
*	------------
*	The purpose of these functions is to supervise the duration of execution
*	of methods implemented by the user. Using this mechanism, the duration of
*	execution of a certain function is limited to a certain value, which may
*	by freely chosen. Before the function is actually called, a timer is started
*	with the duration limit as timeout. Then the function is called. If the timer
*	does not time out during the function call, the timer will be stopped and
*	nothing else happens. But if there is a time out, the function will be aborted
*	by brute force and the function will return immediately. Note, that this may
*	have side effects!
*
*	Attention:
*	- the supervision code is NOT REENTRANT. If you call ov_supervise_start()
*	  while the supervision mechanism is already running, you will get an
*	  error.
*	- under certain circumstances variables of the function calling the
*	  supervision functions have to be declared as volatile, because they
*	  may be optimized away when the longjmp() function is called.
*	- under Windows NT each call of the supervise routines has to come from
*	  the SAME THREAD.
*
*	Resources used:
*	- Unix: ITIMER_REAL and SIGALRM
*	- NT: an invisible timer window
*
*	Side effects:
*	currently unknown -- it seems to work, but you should NOT RELY ON IT.
*/

#ifndef OV_SUPERVISE_H_INCLUDED
#define OV_SUPERVISE_H_INCLUDED

/*
*	Example code:
*/
#if 0
	#include "libov/ov_supervise.h"

	OV_TIME_SPAN 	timeout;
	OV_JUMPBUFFER	jumpbuffer;
	timeout.secs = 1;
	timeout.usecs = 500000;
	if(ov_supervise_setjmp(jumpbuffer) == 0) {
		if(!ov_supervise_start(&timeout, &jumpbuffer)) {
			/* error in ov_supervise_start(), e.g. could not start timer */
			printf("error\n");
		}	
		userfunction(); /* call of the user's function */
		if(!ov_supervise_end()) {
			/* error in ov_supervise_end() */
			printf("error\n");
		}
	} else {
		/* timer timed out, here's code handling this error */
		printf("userfunction() aborted\n");
	}
#endif

#include "libov/ov_ov.h"
#include <setjmp.h>

/*
*	OV_JUMPBUFFER:
*	--------------
*	This datatype describes the stack frame before calling the user
*	function an allows to abort it.
*/
#if OV_SYSTEM_UNIX
typedef sigjmp_buf	OV_JUMPBUFFER;
#else
typedef jmp_buf		OV_JUMPBUFFER;
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Wrapper macro for the setjmp function/macro
*/
#if OV_SYSTEM_UNIX
#define ov_supervise_setjmp(jumpbuffer)	sigsetjmp(jumpbuffer, 1)
#else
#define ov_supervise_setjmp(jumpbuffer)	setjmp(jumpbuffer)
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Start the supervision of a user function
*/
OV_DLLFNCEXPORT OV_BOOL ov_supervise_start(
	OV_TIME_SPAN	*ptimeout,
	OV_JUMPBUFFER	*pjumpbuffer
);

/*
*	Finish supervising a user function
*/
OV_DLLFNCEXPORT OV_BOOL ov_supervise_end(void);

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

#endif
