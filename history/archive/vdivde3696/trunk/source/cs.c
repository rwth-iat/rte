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
*   Functionblock							    *
*                                         				    *
*   standard controller (VDI/VDE 3696)                                      *
*   	                                                                    *
****************************************************************************/
/*
*   Typ: cs
*   =======
*/
/*
*   History
*   -------
*   date : 08.04.1999
*   author : Felix Uecker, Chair of Process Control Engineering, RWTH Aachen	
*/
/*
*   Description
*   -----------
*	This functionblock realizes a standard pid_controller from VDI/VDE 3696.
*
*	This functionblock is equal to the FB C but misses the following features :
*		- limitation of sp
*		- up rate limit of sp and mv
*		- dead zone
*		- on/off switch for gain and derivative component
*		- reset of integrative component
*		- port mvmanfc (force hand)
*		- external control value mvext
*		- disturbance variable feedforward z
*		- port sptron (automatically reacts as sptron=TRUE)
*		- port qmvman
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
#define TI_MIN 0.001
#define TimeToDouble(time) \
((OV_DOUBLE)(time).secs + (OV_DOUBLE)(time).usecs/(OV_DOUBLE)1e6)

/*
*   modul declaration
*   -----------------
*/
#include "vdivde3696.h"
#include "libov/ov_macros.h"

OV_RESULT OV_DLLFNCEXPORT vdivde3696_cs_constructor(
	OV_INSTPTR_ov_object	pobj
) {
	OV_RESULT res;
	OV_INSTPTR_vdivde3696_cs pcs;

	res = fb_functionblock_constructor(pobj);
	if (Ov_Fail(res)) return res;
	pcs = Ov_StaticPtrCast(vdivde3696_cs, pobj);
	pcs->v_en = TRUE;
	pcs->v_reverse = TRUE;
	pcs->v_ion = TRUE;
	pcs->v_kp = 1;
	pcs->v_ti = 100;
	pcs->v_t1totd = 0.1;
	pcs->v_mvhl = 100;
	return OV_ERR_OK;
}

void OV_DLLFNCEXPORT vdivde3696_cs_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_fb_task		ptask;
	OV_INSTPTR_vdivde3696_cs port = Ov_StaticPtrCast(vdivde3696_cs, pfb);

	/*
	*   local variables
	*   ---------------
	*/
            int factor;		/* reverse or direct action */
	    float mv_pid;	/* variable to add P, I and D terms */
	    float mv_p;		/* P term */
	    float mv_d;		/* D term */
	    float mv_i;		/* I term */
	    float t1,alpha;	/* needed for first order lag */
            float mv_pred;	/* MV predicted */
	    double cyctime;

	/*
	*   instructions
	*   ------------
	*/
        if (port->v_en) /* function block enable */
        {
	  /* set eno (enable output) to TRUE */
	  port->v_eno = TRUE;

	  /* get the cyctime of functionblock or parenttask */
	  
	  ptask=Ov_PtrUpCast(fb_task, port);
	  while (ptask && (ptask->v_cyctime.secs==0) && (ptask->v_cyctime.usecs==0)) {
		ptask = Ov_GetParent(fb_tasklist, ptask);
	  }
 	  if (!ptask) cyctime = 1.0; 
	  else cyctime = TimeToDouble(ptask->v_cyctime);

	  /* select setpoint from external setpoint or internal setpoint */
	  if (port->v_spexton) /* external setpoint */
	  {
	    /* setpoint-output equals external setpoint-input */
	    port->v_sp = port->v_spext;

	    /* set qspint (internal setpoint active) to FALSE */
	    port->v_qspint = FALSE;
	    /* overwrite spint with spext */
	    port->v_spint = port->v_spext;
	  }
	  else /* internal setpoint */
	  {
	    /* setpoint-output equals internal setpoint-input */
	    port->v_sp = port->v_spint;
	    /* set qspint (internal setpoint active) to TRUE */
	    port->v_qspint = TRUE;
	  }
	  	
	  /* computation of error signal */
	  port->v_e = port->v_sp - port->v_pv;
	  
	  /* sign factor for reverse or direct action */
	  if (port->v_reverse)
	  {
	    factor = 1;
	  }
	  else
	  {
	    factor = -1;
	  }

	  /* initialize MV_PID for addition of terms */
	  mv_pid = 0;

	  /* compute and add P term */
	  mv_p = factor * port->v_kp * port->v_e;
	  mv_pid = mv_p;
	  
	  /* compute and add D term */
	  switch (port->v_dmode)
	  {
	  case 0 : /* SP - PV */
	           mv_d = factor * port->v_kp * port->v_td * (port->v_e - port->v_eold)/cyctime;
		   break;
	  case 1 : /* -PV */
	           mv_d = factor * port->v_kp * port->v_td * (port->v_pvold - port->v_pv)/cyctime;
		   break;
	  case 2:  /* SP */
	           mv_d = factor * port->v_kp * port->v_td * (port->v_sp - port->v_spold)/cyctime;
		   break;
	  default : mv_d = 0; 
	            port->v_eno = FALSE;
	  }
	  mv_pid = mv_pid + mv_d;
	 
          /* compute and add I term */
	  if (port->v_mvmanon)	/* track mode in case of manual MV */
	  {
	    /* compute mv_i so that mv_i+mv_d+mv_p equals mvold */
	    /* for preparation of bumpless switch back to automatic mode */
	    mv_i = port->v_mvold - mv_p - mv_d;
	  }
	  else if (port->v_ion) /* ION=TRUE, I term activated */
	       {
		 /* normal computation of I term */
	  	 /* check T_I for absolute minimal value of TI_MIN (e.g.: 0.0001) */
		 if ((port->v_ti < TI_MIN) && (port->v_ti > -TI_MIN))
		 {
		   port->v_ti = TI_MIN;
		   port->v_eno = FALSE;
		 }
		 mv_i = port->v_mviold + (factor * port->v_kp / port->v_ti) * cyctime * port->v_e;
		 
		 /* anti preset windup */
		 mv_pred = mv_p + mv_i + mv_d;	/* prediction of MV */
		 if (((mv_pred > port->v_mvhl) && (mv_i > port->v_mviold))||((mv_pred < port->v_mvll) && (mv_i < port->v_mviold)))
		 {
		   mv_i = port->v_mviold;
		 }
	       }
	       else /* ION = FALSE, I term NOT activated and not man. or ext. MV */
	       {
		 mv_i = port->v_mviold;
	       }
	  /* add I term */
	  mv_pid = mv_pid + mv_i;
	  
	  /* first oder lag */
	  t1 = port->v_t1totd * port->v_td;
	  alpha = 1.0/(1.0 + t1/cyctime);
	  mv_pid = (1.0 - alpha) * port->v_mvpidold + alpha * mv_pid;

	  /* select MV from MVMAN ? */
	  if (port->v_mvmanon)
	  {
	    /* manual MV */
	    port->v_mv = port->v_mvman;
	  }
	  else 
	  {
	    port->v_mv = mv_pid;
	    port->v_mvman = mv_pid;	/* MVMAN follows MV */
	  }
	  
	  /* limitaion of MV */
	  if (port->v_mv < port->v_mvll)
	  {
	    port->v_mv = port->v_mvll;
	  }
	  else if (port->v_mv > port->v_mvhl)
	       {
		 port->v_mv = port->v_mvhl;
	       }

	  /* store values for use in next cycle as old values */
	  /* before start, all these values should be 0 */
	  port->v_spold = port->v_sp;
	  port->v_eold = port->v_e;
	  port->v_mvold = port->v_mv;
	  port->v_mviold = mv_i;
	  port->v_mvpidold = mv_pid;
	}
	else /* function block NOT enable */
        {
	  port->v_eno = FALSE;
	}
	
	/* add 1 to counter */
	port->v_counter = (port->v_counter+1)%10000;

/*******  end  *******/
	return;
	}







