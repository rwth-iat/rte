/*
*	Copyright (C) 1998, 1999
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
*	   notice, this list of conditions and the following disclaimer in the
*	   documentation and/or other materials provided with the distribution.
*	4. Neither the name of the Chair of Process Control Engineering nor the
*	   name of the Aachen University of Technology may be used to endorse or
*	   promote products derived from this software without specific prior
*	   written permission.
*
*	THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
*	``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
*	TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
*	PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE CHAIR OF PROCESS CONTROL
*	ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*	EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
*	PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
*	OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
*	WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
*	OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*	ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/****************************************************************************
*									    *
*    functionblock                                			    *
*                                                         		    *
*    on delay (VDI/VDE 3696)                                                *
*                                                                           *
****************************************************************************/
/*
*   type: ton1
*   ==========
*/
/*
*   history
*   -------
*   created : 23.6.1999
*   author  : Michael Klamann, Chair of Process Control Engineering, RWTH Aachen
*/
/*
*   description
*   -----------
*   A rising edge of the input signal i will only be relayed to 
*   the ouput q if the time span "time" has elapsed and the input signal is still TRUE.
*
*   Similar to standardfunction TON in IEC 1131-3.
*/
/*
*   C-Includes
*   ----------
*   none
*/
/*  
*   defines
*   -------
*/
#define TimeToDouble(time) \
((OV_DOUBLE)(time).secs + (OV_DOUBLE)(time).usecs/(OV_DOUBLE)1e6)
/*
*   modul-declaration
*   -----------------
*/
#include "vdivde3696.h"
#include "libov/ov_macros.h"

OV_RESULT OV_DLLFNCEXPORT vdivde3696_ton1_constructor(
	OV_INSTPTR_ov_object	pobj
) {
	OV_RESULT res;
	OV_INSTPTR_vdivde3696_ton1 pton1;

	res = fb_functionblock_constructor(pobj);
	if (Ov_Fail(res)) return res;
	pton1 = Ov_StaticPtrCast(vdivde3696_ton1, pobj);
	pton1->v_en = TRUE;
	return OV_ERR_OK;
}

void OV_DLLFNCEXPORT vdivde3696_ton1_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_fb_task		ptask;
	OV_INSTPTR_vdivde3696_ton1 port = Ov_StaticPtrCast(vdivde3696_ton1, pfb);

	/*
	*   local variables
	*   ---------------
	*/
		double cyctime;
	/*
	*   instructions
	*   ------------
	*/

        if (port->v_en) /* functionblock enable */
        {
	  /* set enable output to TRUE */
	  port->v_eno = TRUE;

	  /* get the cyctime of functionblock or parenttask */
	  
	  ptask=Ov_PtrUpCast(fb_task, port);
	  while (ptask && (ptask->v_cyctime.secs==0) && (ptask->v_cyctime.usecs==0)) {
		ptask = Ov_GetParent(fb_tasklist, ptask);
	  }
 	  if (!ptask) cyctime = 1.0; 
	  else cyctime = TimeToDouble(ptask->v_cyctime);

	  if (port->v_i == FALSE) /* FALSE input ? */
	  {
	    /* reset time counter etime and set output to FALSE */
	    port->v_etime = 0;
	    port->v_q = FALSE;
	  }
	  else if (port->v_etime >= port->v_time) /* elapsed time greater than time delay ? */
	       {
		 /* set output to TRUE */
		 port->v_q = TRUE;
	       }
	       else /* add cycle time to elapsed time */
	       {
		 port->v_etime = port->v_etime + cyctime;
	       }
       	}
        else /* functionblock NOT enable */
        {
	  port->v_eno = FALSE;
	}

        /* add 1 to counter */
	port->v_counter = (port->v_counter+1)%10000;

/*******  end  *******/
	return;
	}





