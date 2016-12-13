/*
*   Copyright (C) 2004
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
*/
/***********************************************************************
*
*	File
*	-----
*	tof1.c					Ausschaltverzögerung (off delay)
*
*	Autoren:
*	--------
*	St							Stefan Schmitz <StefanS@plt.rwth-aachen.de>
*
*	Historie:
*	--------
*	23-September-2004		St
*		-	Erstellung
*
***********************************************************************/

#ifndef OV_COMPILE_LIBRARY_vdivde3696
#define OV_COMPILE_LIBRARY_vdivde3696
#endif

/***********************************************************************
	Includes
***********************************************************************/

#include "vdivde3696.h"
#include "libov/ov_macros.h"

/***********************************************************************
	Typemethod
***********************************************************************/

void OV_DLLFNCEXPORT vdivde3696_tof1_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME								*pltc
)	{
	//	Local Pointer
	//
	OV_INSTPTR_fb_task
		ptask = NULL;
	OV_INSTPTR_vdivde3696_tof1
		port = Ov_StaticPtrCast(vdivde3696_tof1, pfb);
		
	//	Local Variables
	//
	double cyctime;
	
	//	Logic
	//
	port->v_eno = port->v_en;
	
	if (port->v_en == TRUE)
	{
		//	get the cyctime of functionblock or parenttask
		//
		ptask = Ov_PtrUpCast(fb_task, pfb);
		
		while (ptask && (ptask->v_cyctime.secs==0) && (ptask->v_cyctime.usecs==0)) {
			ptask = Ov_GetParent(fb_tasklist, ptask);
		}
	 	if (ptask == NULL)
	 		cyctime = 1.0; 
		else
			Ov_TimeToDouble(ptask->v_cyctime, cyctime);
			
		switch (port->v_i)
		{
			case TRUE:
				port->v_etime	= 0.0;
				port->v_q		= TRUE;
				break;
				
			case FALSE:
				if (port->v_etime >= port->v_time)
					port->v_q = FALSE;
				else
				{
					port->v_etime = port->v_etime + cyctime;
				};
				break;
				
			default:
				port->v_eno = FALSE;
				break;
		};
	};

	//	add 1 to counter
	//
	port->v_counter = (port->v_counter + 1) % 10000;
	
	return;
};
