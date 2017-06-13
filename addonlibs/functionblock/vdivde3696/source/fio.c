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
*    functionblock                       				    *
*                                                       		    *
*    1st order, lead lag (VDI/VDE 3696)                                     *
*                                                                           *
****************************************************************************/
/*
*   type: fio
*   =========
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
*   This functionblock approximizes the following equation of first degree :
*
*   T1 * vold + V = KP * [U + TD * uold]
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

OV_RESULT OV_DLLFNCEXPORT vdivde3696_fio_constructor(
	OV_INSTPTR_ov_object	pobj
) {
	OV_RESULT res;
	OV_INSTPTR_vdivde3696_fio pfio;

	res = fb_functionblock_constructor(pobj);
	if (Ov_Fail(res)) return res;
	pfio = Ov_StaticPtrCast(vdivde3696_fio, pobj);
	pfio->v_en = TRUE;
	pfio->v_kp = 1;
	return OV_ERR_OK;
}

void OV_DLLFNCEXPORT vdivde3696_fio_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_fb_task		ptask;
	OV_INSTPTR_vdivde3696_fio port = Ov_StaticPtrCast(vdivde3696_fio, pfb);

	/*
	*   local variables
	*   ----------------
	*/
		float alpha,beta;
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

	  if (port->v_reset) /* reset active ? */
	  {
	    /* set output to reset value */
	    port->v_v = port->v_v0;
	  }
	  else /* approximate calculation */
	  {
	    alpha = port->v_t1 / cyctime;
	    beta = port->v_td / cyctime;
	    port->v_v = (alpha * port->v_vold + port->v_kp * (1+beta) * port->v_u - port->v_kp * beta * port->v_uold)/(1+alpha);
	  }
       	}
        else /* functionblock NOT enable */
        {
	  port->v_eno = FALSE;
	}

	/* store values for use in next cycle */
	port->v_uold = port->v_u;
        port->v_vold = port->v_v;

        /* add 1 to counter */
	port->v_counter = (port->v_counter+1)%10000;

/*******  end  *******/
	return;
	}





