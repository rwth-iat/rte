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
*    functionblock                                                          *
*                                                                           *
*    moving window average (VDI/VDE 3696)                                   *
*                                                                           *
****************************************************************************/
/*
*   type: aver
*   ==========
*/
/*
*   history
*   -------
*   created : 31.5.1999
*   author  : Michael Klamann, Chair of Process Control Engineering, RWTH Aachen
*/
/*
*   description
*   -----------
*   Similar to standardfunction AVERAGE in the annex of IEC 1131-3.
*   Instead of n, ta is denounced.
*/
/*
*   C-Includes
*   ----------
*/
#include <math.h>
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

OV_RESULT OV_DLLFNCEXPORT vdivde3696_aver_constructor(
	OV_INSTPTR_ov_object	pobj
) {
	OV_RESULT res;
	OV_INSTPTR_vdivde3696_aver paver;

	res = fb_functionblock_constructor(pobj);
	if (Ov_Fail(res)) return res;
	paver = Ov_StaticPtrCast(vdivde3696_aver, pobj);
	paver->v_en = TRUE;
	paver->v_ta = 1;
	paver->v_mxtatotc = 10;
	paver->v_averon = 1;
	return OV_ERR_OK;
}

void OV_DLLFNCEXPORT vdivde3696_aver_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_fb_task		ptask;
	OV_INSTPTR_vdivde3696_aver port = Ov_StaticPtrCast(vdivde3696_aver, pfb);

	/*
	*   local variables
	*   ---------------
	*/
		int i,n,deltak;
		float sum;
		double cyctime;
	/*
	*   instructions
	*   ------------
	*/

        if (port->v_en) /* functionblock enable */
        {
	  /* get the cyctime of functionblock or parenttask */
	  
	  ptask=Ov_PtrUpCast(fb_task, port);
	  while (ptask && (ptask->v_cyctime.secs==0) && (ptask->v_cyctime.usecs==0)) {
		ptask = Ov_GetParent(fb_tasklist, ptask);
	  }
 	  if (!ptask) cyctime = 1.0; 
	  else cyctime = TimeToDouble(ptask->v_cyctime);

	  port->v_qon = port->v_averon;
	  if (port->v_averon == FALSE) /* average not running */
	  {
	    /* set enable output to TRUE, ouput to input and averono to FALSE */ 
	    port->v_v = port->v_u;
	    port->v_averono = FALSE;
	    port->v_eno = TRUE;
	  }
	  else if (port->v_averono == FALSE) /* starting average */
	       {
		 /* fill buffer with current input value */
		 for (i=0;i < port->v_mxtatotc;i++)
		 {
		   port->v_ringbuffer[i] = port->v_u;
		 }

		 /* set averono to TRUE, enable output to TRUE and output to input */
		 port->v_averono = TRUE;
		 port->v_v = port->v_u;
		 port->v_eno = TRUE;
	       }
	       else /* running average */
	       {
		 /* compute relevant numbers of old cycles */
		 n = floor(port->v_ta / cyctime + 0.5);

		 if (n > port->v_mxtatotc) /* n to large ? */
		 {
		   /* set n to largest allowed number and enable output to FALSE */
		   n = port->v_mxtatotc;
		   port->v_eno = FALSE;
		 }
		 else if (n < 1) /* n to small */
		      {
			/* set n to 1 and enable output to FALSE */
			n = 1;
			port->v_eno = FALSE;
		      }
		      else /* n OK */
		      {
			/* set enable output to TRUE */
			port->v_eno = TRUE;
		      }

		 /* store current input value in ringbuffer */
		 port->v_ringbuffer[port->v_k] = port->v_u;

		 /* building average */
		 sum = 0;
		 for (deltak=0;deltak < n;deltak++)
		 {
		   i = port->v_k - deltak - port->v_mxtatotc * floor ((float)(port->v_k - deltak) / port->v_mxtatotc);
		   sum = sum + port->v_ringbuffer[i];
		 }
		 port->v_v = sum/n;

		 /* update bufferindex k */
		 port->v_k = fmod ((float)(port->v_k + 1),port->v_mxtatotc);
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







