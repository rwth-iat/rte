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







