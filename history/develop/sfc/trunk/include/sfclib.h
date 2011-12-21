/*
*	Copyright (C) 2010
*	Chair of Process Control Engineering,
*	Aachen University of Technology.
*	All rights reserved.
*
*	Redistribution and use in source and binary forms, with or without
*	modification, are permitted provided that the following conditions
*	are met:
*	1. Redistributions of source code must retain the above copyright
*	   notice, this list of conditions and the following disclaimer.
*	2. Redistributions in binary form must print or display the above
*	   copyright notice either during startup or must have a means for
*	   the user to view the copyright notice.
*	3. Redistributions in binary form must reproduce the above copyright
*	   notice, this list of conditions and the following disclaimer in
*		the documentation and/or other materials provided with the
*		distribution.
*	4. Neither the name of the Chair of Process Control Engineering nor
*		the name of the Aachen University of Technology may be used to
*		endorse or promote products derived from this software without
*		specific prior written permission.
*
*	THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
*	``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE CHAIR OF
*	PROCESS CONTROL ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT,
*	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
*	OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
*	AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
*	WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*	POSSIBILITY OF SUCH DAMAGE.
*/
/***********************************************************************
*
*	File:
*	------
*	sfcliblib.h
*
*	Editors:
*	--------

*
*	CVS:
*	----
*
************************************************************************/

#ifndef SFCLIB_H_INCLUDED
#define SFCLIB_H_INCLUDED

/***********************************************************************
	Includes
***********************************************************************/

#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include "libov/ov_memstack.h"
#include "libov/ov_result.h"
#include "libov/ov_logfile.h"

#include "ov_call_macros_10.h"

// TODO:  Cmd mapping

/***********************************************************************
	Enumerations
***********************************************************************/

/***********************************************************************
	command
***********************************************************************/

enum {
	SFCCMD_STOP		= 0,	// execute exit-actions of the active step and stop SFC
	SFCCMD_START	= 1,	// start SFC
	SFCCMD_BREAK	= 2,	// stop SFC and hold on active step
	SFCCMD_RESET	= 3		// stop SFC and reset sfc
};

/***********************************************************************
	workingStates
***********************************************************************/

enum {
	WOST_INIT		= 0,	// initialized
	WOST_START		= 1,	// start
	WOST_BREAK		= 2,	// break
	WOST_STOP		= 3,	// break off
	WOST_TERMINATE	= 4		// terminate (reaches END-step)
};

/***********************************************************************
	action identifier
***********************************************************************/

enum {
	ACT_ENTRY		= 1,
	ACT_DO		    = 2,
	ACT_EXIT		= 3
};

/***********************************************************************
	Ov_GetFirstChildEx
***********************************************************************/

#define Ov_GetFirstChildEx(assoc, pparent, pchild, childclass)	\
	Ov_ForEachChildEx(assoc, (pparent), (pchild), childclass)	\
	{	\
		break;	\
	}

#endif
