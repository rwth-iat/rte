/*
*	Copyright (C) 2011
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
*
***********************************************************************
*
* CSHMI - Client-Side Human-Machine Interface
*
*	File:
*	------
*	classEvent_OperatorEvent.c
*
*	Editors:
*	--------
*	Je							Holger Jeromin <jeromin@plt.rwth-aachen.de>
*	GQ							Gustavo Quiros <g.quiros@plt.rwth-aachen.de>
*
*	SVN:
*	----
*	$Revision$
*	$Date$
*
*	History:
*	--------
*	18-August-2011			GQ		V0.1.0
*		-	File created
*
***********************************************************************/

#ifndef OV_COMPILE_LIBRARY_cshmi
#define OV_COMPILE_LIBRARY_cshmi
#endif

#include "cshmilib.h"

OV_DLLFNCEXPORT OV_RESULT cshmi_OperatorEvent_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
	/*    
	*   local variables
	*/
	OV_RESULT    result;
	OV_STRING erroroutput;
	
	/* do what the base class does first */
	result = ov_object_constructor(pobj);
	if(Ov_Fail(result))
		return result;
	
	//todo check for move and correct to aftermove

	//force our keywords
	if (	ov_string_compare(pobj->v_identifier, "click") == OV_STRCMP_EQUAL
			||	ov_string_compare(pobj->v_identifier, "doubleclick") == OV_STRCMP_EQUAL
			||	ov_string_compare(pobj->v_identifier, "rightclick") == OV_STRCMP_EQUAL
			||	ov_string_compare(pobj->v_identifier, "aftermove") == OV_STRCMP_EQUAL
		){
		return OV_ERR_OK;
	}else{
		ov_memstack_lock();
		ov_string_print(&erroroutput, "object %s had wrong identifier. Rejecting construction.", ov_path_getcanonicalpath(Ov_StaticPtrCast(ov_object, pobj), 2));
		ov_memstack_unlock();
		ov_logfile_error(erroroutput);
		ov_string_print(&erroroutput, NULL);
		return OV_ERR_BADPARAM;
	}
}

