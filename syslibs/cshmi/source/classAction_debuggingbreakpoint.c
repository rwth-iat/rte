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
*	classAction_debuggingbreakpoint.c
*
*	Editors:
*	--------
*	Je							Holger Jeromin <jeromin@plt.rwth-aachen.de>
*
*	SVN:
*	----
*	$Revision: $
*	$Date: $
*
*	History:
*	--------
*	16-May-2012			Je		V0.1.0
*		-	File created
*
***********************************************************************/

#ifndef OV_COMPILE_LIBRARY_cshmi
#define OV_COMPILE_LIBRARY_cshmi
#endif

#include "cshmilib.h"

OV_DLLFNCEXPORT OV_RESULT cshmi_debuggingbreakpoint_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
	//	local variables
	//
	OV_RESULT    result;

	/* do what the base class does first */
	result = cshmi_Action_constructor(pobj);
	if(Ov_Fail(result))
		return result;

	//debugger is most useful before the existing statements
	cshmi_Object_zindex_set(Ov_StaticPtrCast(cshmi_Object, pobj), CSHMI_ZINDEX_BOTTOM);

	return OV_ERR_OK;
}

