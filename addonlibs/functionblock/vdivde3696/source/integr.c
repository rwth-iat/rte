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
*   limitations under the License..
*/
/****************************************************************************
*									    *
*    functionblock                       				    *
*                                                       		    *
*    integrator (VDI/VDE 3696)                                              *
*                                                                           *
****************************************************************************/
/*
*   type: integr
*   ============
*/
/*
*   history
*   -------
*   created : 28.5.1999
*   author  : Michael Klamann, Chair of Process Control Engineering, RWTH Aachen
*/
/*
*   description
*   -----------
*   This functionblock approximizes the integral : 
*
*   V = V0 + KI*  0{t U dt
*
*   On startup v is set to v0.
*
*   Reset is prior to hold/track mode. Hold is prior to track mode.
*
*   note : The input digits exists but is not used.
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

OV_RESULT OV_DLLFNCEXPORT vdivde3696_integr_constructor(
	OV_INSTPTR_ov_object	pobj
) {
	OV_RESULT res;
	OV_INSTPTR_vdivde3696_integr pint;

	res = fb_functionblock_constructor(pobj);
	if (Ov_Fail(res)) return res;
	pint = Ov_StaticPtrCast(vdivde3696_integr, pobj);
	pint->v_hl = 1e19;
	pint->v_hhs = 1e19;
	pint->v_ll = -1e19;
	pint->v_lls = -1e19;
	pint->v_digits = 6;
	return OV_ERR_OK;
}

void OV_DLLFNCEXPORT vdivde3696_integr_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_fb_task		ptask;
	OV_INSTPTR_vdivde3696_integr port = Ov_StaticPtrCast(vdivde3696_integr, pfb);

	/*
	*   local variables
	*   ---------------
	*/
		double cyctime;
	/*
	*   instructions
	*   ------------
	*/

	/* special instruction on initilization */
	if (port->v_counter == 0)
	{
	  port->v_vold = port->v_v0;
	}
	/* end special instruction */
	
        if (port->v_en) /* functionblock enbale */
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

	  if (port->v_reset) /* reset active ? */
	  {
	    port->v_v = port->v_v0;
	  }
	  else if (port->v_hold) /* hold mode active ? */
	       {
		 port->v_v = port->v_vold;	
	       }	
	       else if (port->v_track) /* track mode active ? */
		    {
		      port->v_v = port->v_u;
		    }
		    else /* no special mode active */
		    {
		      if (port->v_thron && fabs(port->v_u) < fabs(port->v_thr)) /* check threshold */
		      {
			port->v_v = port->v_vold; /* dead zone */
		      }
		      else /* approximate calculation */
		      {
			port->v_v = port->v_vold + port->v_ki * cyctime * port->v_u;
		      }
		    }

	  /* limiting of v */
	  if (port->v_v >= port->v_hl)	/* to large ? */
	  {
	    /* set v to hl and qhl to TRUE */
	    port->v_v = port->v_hl;
	    port->v_qhl = TRUE;
	  }
	  else
	  {
	    /* set qhl to FALSE */
	    port->v_qhl = FALSE;
	  }

	  if (port->v_v <= port->v_ll) /* to small ?*/
	  {
	    /* set v to ll and qll to TRUE */
	    port->v_v = port->v_ll;
	    port->v_qll = TRUE;
	  }
	  else
	  {
	    /* set qll to FALSE */
	    port->v_qll = FALSE;
	  }
	  
	  /* limit and pre-limit checks */
	  if (port->v_v > port->v_hhs) /* v larger than hhs ? */
	  {
	    port->v_qhhs = TRUE;
	  }
	  else
	  {
	    port->v_qhhs = FALSE;
	  }

	  if (port->v_v > (port->v_hhs - port->v_deltahs)) /* v larger than hs ? */
	  {
	    port->v_qhs = TRUE;
	  }
	  else
	  {
	    port->v_qhs = FALSE;
	  }

	  if (port->v_v < (port->v_lls + port->v_deltals)) /* v smaller than ls ? */
	  {
	    port->v_qls = TRUE;
	  }
	  else
	  {
	    port->v_qls = FALSE;
	  }

	  if (port->v_v < port->v_lls) /* v smaller than lls ? */
	  {
	    port->v_qlls = TRUE;
	  }
	  else
	  {
	    port->v_qlls = FALSE;
	  }
	  
	  /* check if v is equal to initilization value v0 */
	  if (port->v_v == port->v_v0)
	  {
	    port->v_qv0 = TRUE;
	  }
	  else
	  {
	    port->v_qv0 = FALSE;
	  }
       	}
        else /* functionblock NOT enable */
        {
	  port->v_eno = FALSE;
	}
	
	/* store value for use in next cycle */
        port->v_vold = port->v_v;

        /* add 1 to counter */
	port->v_counter = (port->v_counter+1)%10000;

/*******  end  *******/
	return;
	}






