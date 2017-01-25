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







