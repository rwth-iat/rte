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
*    functionblock                      				    *
*                                                       		    *
*    dead time (VDI/VDE 3696)                                               *
*                                                                           *
****************************************************************************/
/*
*   type: deadt
*   ===========
*/
/*
*   history
*   -------
*   created : 1.6.1999
*   author  : Michael Klamann, Chair of Process Control Engineering, RWTH Aachen
*/
/*
*   description
*   -----------
*   Similar to standardfunction DELAY in the annex of IEC 1131-3.
*   Instead of N (number of cycles) TD (dead time) is denounced.
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

OV_RESULT OV_DLLFNCEXPORT vdivde3696_deadt_constructor(
	OV_INSTPTR_ov_object	pobj
) {
	OV_RESULT res;
	OV_INSTPTR_vdivde3696_deadt pdeadt;

	res = fb_functionblock_constructor(pobj);
	if (Ov_Fail(res)) return res;
	pdeadt = Ov_StaticPtrCast(vdivde3696_deadt, pobj);
	pdeadt->v_en = TRUE;
	pdeadt->v_mxtdtotc = 10;
	return OV_ERR_OK;
}

void OV_DLLFNCEXPORT vdivde3696_deadt_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_fb_task		ptask;
	OV_INSTPTR_vdivde3696_deadt port = Ov_StaticPtrCast(vdivde3696_deadt, pfb);

	/*
	*   local variables
	*   ---------------
	*/
		int n,i;
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

	  /* output qon follows input deadton */
	  port->v_qon = port->v_deadton;

	  if (port->v_deadton == FALSE) /* deadt time NOT active ? */
	  {
	    /* set output to input, deadtono to FALSE and enable output to TRUE */
	    port->v_v = port->v_u;
	    port->v_deadtono = FALSE;
	    port->v_eno = TRUE;
	  }
	  else if (port->v_deadtono == FALSE) /* dead time activation ? */
	       {
		 /* fill ringbuffer with current input value */
		 for (i=0;i < port->v_mxtdtotc;i++)
		 {
		   port->v_ringbuffer[i] = port->v_u;
		 }

		 /* set output to input, deaton_o to TRUE and enable output to TRUE */
		 port->v_deadtono = TRUE;
		 port->v_v = port->v_u;
		 port->v_eno = TRUE;
	       }
	       else /* running dead time */
	       {
		 /* compute relevant numbers of old cycles */
		 n = floor (port->v_td / cyctime + 0.5) - 1;
		 
		 if (n >= port->v_mxtdtotc) /* n to large ? */
		 {
		   /* set n to largest allowed number and enable output to FALSE */
		   n = port->v_mxtdtotc - 1;
		   port->v_eno = FALSE;
		 }
		 else if (n < 0) /* n to small */
		      {
			/* set n to 0 and enable output to FALSE */
			n = 0;
			port->v_eno = FALSE;
		      }
		      else /* n OK */
		      {
			/* set enable output to TRUE */
			port->v_eno = TRUE;
		      }

	         /* caclulate v */
		 if (n == 0)
		 {
		   port->v_v = port->v_u;
		 }
		 else
		 {
		   i = port->v_k - n - port->v_mxtdtotc * floor ((float)((port->v_k - n)) / port->v_mxtdtotc);
		   port->v_v = port->v_ringbuffer[i];
		 }

		 /* store input u and update bufferindex k */
		 port->v_ringbuffer[port->v_k] = port->v_u;
		 port->v_k = fmod ((float)(port->v_k + 1),port->v_mxtdtotc);
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







