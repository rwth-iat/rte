/*
*   Copyright (C) 2012
*   Chair of Process Control Engineering,
*   D-52056 Aachen, Germany.
*   All rights reserved.
*
*
*   This file is part of the ACPLT/OV Package 
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

