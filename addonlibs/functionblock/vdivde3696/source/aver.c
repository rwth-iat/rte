/*
*   Copyright (C) 1998
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







