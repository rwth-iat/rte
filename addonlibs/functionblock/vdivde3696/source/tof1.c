/*
*	Copyright (C) 2004
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
