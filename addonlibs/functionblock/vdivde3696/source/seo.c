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
*    funtionblock                        				    *
*                                                       		    *
*    2nd order (VDI/VDE 3696)                                               *
*                                                                           *
****************************************************************************/
/*
*   type: seo
*   =========
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
*   This functionblock approximizes the following differential equation :
*
*   A2*V..(t) + A1*V.(t) + V(t) = KP * [U(t) + B1*U.(t) + B2*U..(t)]
*  
*   with A2 = 1/OMEGA0²
*        A1 = 2 * THETA/OMEGA0
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

OV_RESULT OV_DLLFNCEXPORT vdivde3696_seo_constructor(
	OV_INSTPTR_ov_object	pobj
) {
	OV_RESULT res;
	OV_INSTPTR_vdivde3696_seo pseo;

	res = fb_functionblock_constructor(pobj);
	if (Ov_Fail(res)) return res;
	pseo = Ov_StaticPtrCast(vdivde3696_seo, pobj);
	pseo->v_en = TRUE;
	pseo->v_kp = 1;
	pseo->v_omega0 = 1;
	pseo->v_theta = 1;
	return OV_ERR_OK;
}

void OV_DLLFNCEXPORT vdivde3696_seo_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_fb_task		ptask;
	OV_INSTPTR_vdivde3696_seo port = Ov_StaticPtrCast(vdivde3696_seo, pfb);

	/*
	*   local variables
	*   ---------------
	*/
		float a1,a2,c1,c2,d1,d2;
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
	    port->v_v = port->v_v0;
	    port->v_vold1 = port->v_vm1;
	  }
	  else /* approximate calculation */
	  {
	    a2 = 1 / port->v_omega0 / port->v_omega0;
	    a1 = 2 * port->v_theta / port->v_omega0;
	    c1 = a1 / cyctime;
	    c2 = a2 / cyctime / cyctime;
	    d1 = port->v_b1 / cyctime;
	    d2 = port->v_b2 / cyctime / cyctime;
	    port->v_v = ((c1 + 2*c2) * port->v_vold1 - c2*port->v_vold2 + port->v_kp*(1+d1+d2)*port->v_u - port->v_kp*(d1 + 2*d2)*port->v_uold1 + port->v_kp*d2*port->v_uold2)/(1 + c1 +c2);
	  }
       	}
        else /* functionblock NOT enable */
        {
	  port->v_eno = FALSE;
	}

	/* store values for use in next cycles */
        port->v_uold2 = port->v_uold1;
	port->v_uold1 = port->v_u;
        port->v_vold2 = port->v_vold1;
        port->v_vold1 = port->v_v;

        /* add 1 to counter */
	port->v_counter = (port->v_counter+1)%10000;

/*******  end  *******/
	return;
	}







