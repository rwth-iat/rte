/*
*	Copyright (C) 2012
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
*	baseclass_ContainerElement.c
*
*	Editors:
*	--------
*	Je							Holger Jeromin <jeromin@plt.rwth-aachen.de>
*
*	SVN:
*	----
*	$Revision$
*	$Date$
*
*	History:
*	--------
*	30-November-2011			Je
*		-	File created
*
***********************************************************************/

#ifndef OV_COMPILE_LIBRARY_cshmi
#define OV_COMPILE_LIBRARY_cshmi
#endif


#include "cshmilib.h"


OV_DLLFNCEXPORT OV_RESULT cshmi_ContainerElement_constructor(
	OV_INSTPTR_ov_object 	pobj
)	{
	//	local variables
	//
	OV_INSTPTR_ov_object pParent = NULL;
	OV_RESULT    result;

	/* do what the base class does first */
	result = cshmi_Object_constructor(pobj);
	if(Ov_Fail(result))
		return result;

	//force correct placement
	pParent = Ov_StaticPtrCast(ov_object, Ov_GetParent(ov_containment, pobj));
	if (pParent != NULL){
		if (Ov_CanCastTo(cshmi_Group, pobj) && Ov_CanCastTo(cshmi_Template, pParent)){
			//white list Group under template
		}else if (Ov_CanCastTo(cshmi_Element, pParent) ||
			Ov_CanCastTo(cshmi_Action, pParent) ||
			Ov_CanCastTo(cshmi_Condition, pParent) ){
			//ContainerElements are not allowed under Elements, Actions and Conditions
			ov_logfile_warning("An ContainerElement is not allowed below this parent. ContainerElement: %s, parent: %s", pobj->v_identifier, pParent->v_identifier);
			return OV_ERR_BADPLACEMENT;
		}
	}

	return OV_ERR_OK;
}

