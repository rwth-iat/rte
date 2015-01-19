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
*	sscliblib.h
*
*	Editors:
*	--------

*
*	CVS:
*	----
*
************************************************************************/

#ifndef SSCLIB_H_INCLUDED
#define SSCLIB_H_INCLUDED

/***********************************************************************
	Includes
***********************************************************************/

#include "ssc.h"
#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include "libov/ov_memstack.h"
#include "libov/ov_result.h"
#include "libov/ov_logfile.h"

#include "fb_namedef.h"

#include "ov_call_macros_10.h"

// TODO:  Cmd mapping

/***********************************************************************
	Enumerations
***********************************************************************/

/***********************************************************************
	command
***********************************************************************/


#define SSC_CMD_STOP	0	// execute exit-actions of the active step and stop SSC
#define SSC_CMD_START	1	// start SSC
#define SSC_CMD_BREAK	2	// stop SSC and hold on active step
#define SSC_CMD_RESET	3	// stop SSC and reset SSC

/***********************************************************************
	workingStates
***********************************************************************/

#define SSC_WOST_INIT		0	// initialized
#define SSC_WOST_START		1	// start
#define SSC_WOST_BREAK		2	// break
#define SSC_WOST_STOP		3	// break off
#define SSC_WOST_TERMINATE	4	// terminate (reaches END-step)

/***********************************************************************
	action qualifier
***********************************************************************/
#define SSC_QUALIFIER_ENTRY	1
#define SSC_QUALIFIER_DO	2
#define SSC_QUALIFIER_EXIT	3

/***********************************************************************
	phase of the execution
***********************************************************************/
#define SSC_PHASE_ENTRYDO	1	//entry or do phase
#define SSC_PHASE_EXITTRANS	2		//exit or transition phase

/***********************************************************************
	values of the variable internalRole of a step
***********************************************************************/
#define SSC_STEPROLE_START	0	//this step is the start step
#define SSC_STEPROLE_NORMAL	1	//this step is a normal step
#define SSC_STEPROLE_END	999		//this step is the end step

/***********************************************************************
	Ov_GetFirstChildEx
***********************************************************************/

#define Ov_GetFirstChildEx(assoc, pparent, pchild, childclass)	\
	Ov_ForEachChildEx(assoc, (pparent), (pchild), childclass)	\
	{	\
		break;	\
	}

#endif

OV_INSTPTR_ov_object getrelativeobjectpointer(const OV_STRING startPoint, const OV_STRING pathname);
OV_RESULT ssc_setNamedVariable(const OV_INSTPTR_ov_object pTargetObj, const OV_STRING targetVarname, OV_ANY *value);
OV_RESULT ssc_getObjectFromExecute(const OV_INSTPTR_ssc_execute pinst, const OV_STRING nameToCheck, OV_INSTPTR_fb_functionblock *pTargetObj);
