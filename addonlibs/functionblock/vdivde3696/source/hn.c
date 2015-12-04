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
*    functionblock                      				    *
*                                                       		    *
*    handstation with switch and limitation                                 *
*                                                                           *
****************************************************************************/
/*
*   type: hn
*   ========
*/
/*
*   history
*   -------
*   created : 5.5.1999
*   author  : Michael Klamann, Chair of Process Control Engineering, RWTH Aachen
*/
/*
*   description
*   -----------
*   The functionblock checks the inputsignal "Vx" for high- and low limit violation and
*   also for down rate- and up rate limitation.
*
*   VEXTON decides if VMAN or VEXT is watched.
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

OV_RESULT OV_DLLFNCEXPORT vdivde3696_hn_constructor(
	OV_INSTPTR_ov_object	pobj
) {
	OV_RESULT res;
	OV_INSTPTR_vdivde3696_hn phn;

	res = fb_functionblock_constructor(pobj);
	if (Ov_Fail(res)) return res;
	phn = Ov_StaticPtrCast(vdivde3696_hn, pobj);
	phn->v_en = TRUE;
	phn->v_llon = TRUE;
	phn->v_hlon = TRUE;
	phn->v_hl = 100;
	phn->v_dr = 100;
	phn->v_ur = 100;
	return OV_ERR_OK;
}

void OV_DLLFNCEXPORT vdivde3696_hn_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_fb_task		ptask;
	OV_INSTPTR_vdivde3696_hn port = Ov_StaticPtrCast(vdivde3696_hn, pfb);

	/*
	*   local variables
	*   ---------------
	*/
		double cyctime;
	/*
	*   instructions
	*   ------------
	*/


/*** special case instructions -> functionblock initilization ***/
        
        if (port->v_counter == 0)
        {
	  if (port->v_vexton)
	  {
	    port->v_v = port->v_vext;
	  }
	  else
          {
	    port->v_v = port->v_vman;
	  }
	  port->v_vold = port->v_v;
	}
/**************** end functionblock initilization ***************/

        if (port->v_en) /* functionblock enable */
        {
	  /* set enable output to true */
	  port->v_eno = TRUE;

	  /* get the cyctime of functionblock or parenttask */
	  
	  ptask=Ov_PtrUpCast(fb_task, port);
	  while (ptask && (ptask->v_cyctime.secs==0) && (ptask->v_cyctime.usecs==0)) {
		ptask = Ov_GetParent(fb_tasklist, ptask);
	  }
 	  if (!ptask) cyctime = 1.0; 
	  else cyctime = TimeToDouble(ptask->v_cyctime);

	  /* determine if vext or vman has to be watched */
	  if (port->v_vexton)
	  {
            /* set output v to external control value and qvext to true */
	    port->v_v = port->v_vext;
	    port->v_qvext = TRUE;
	  }
	  else /* set output v to hand control value and qvext to false */
	  {
	    port->v_v = port->v_vman;
	    port->v_qvext = FALSE;
	  }

	  /* check for low limit violation */
	  if (port->v_llon && port->v_v < port->v_ll)
	  {
	    port->v_v = port->v_ll;
	  }

	  /* check for high limit violation */
	  if (port->v_hlon && port->v_v > port->v_hl)
	  {
	    port->v_v = port->v_hl;
	  }

          /* check down rate limitation */
	  if (port->v_dron && (port->v_vold - port->v_v) > port->v_dr * cyctime)
	  {
	    port->v_v = port->v_vold - port->v_dr * cyctime;
	  }
	  /* else check up rate limitation */
          else if (port->v_uron && (port->v_v - port->v_vold) > port->v_ur * cyctime)
	       {
		 port->v_v = port->v_vold + port->v_ur * cyctime;
	       }
       	}
        else /* functionblock NOT enable */
        {
	  port->v_eno = FALSE;
	}

        /* store output value for next cycle */
        port->v_vold = port->v_v;
        
        /* add 1 to counter */
	port->v_counter = (port->v_counter+1)%10000;

/*******  end  *******/
	return;
	}







