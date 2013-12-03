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
*    functionblock                       				    *
*                                                       		    *
*    universal controller (VDI/VDE 3696)                                    *
*                                                                           *
****************************************************************************/
/*
*   type: c
*   =======
*/
/*
*   history
*   -------
*   created : 27.5.1999
*   author  : Michael Klamann, Chair of Process Control Engineering, RWTH Aachen
*/
/*
*   description
*   -----------
*   This functionblock is an universal controller with the following options :
*	- proportinal,integrative and derivative components can be activated solitary
*	- limits for setpoint and control value
*	- down/up rate limits with activator ports
*	- dead zone
*	- disturbance variable feedforward
*	- reverse or direct action
*	- internal and external setpoint
*	- manual, external or computed control value
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
#define TI_MIN 0.001			/* minimal allowed integrative time */
#define TimeToDouble(time) \
((OV_DOUBLE)(time).secs + (OV_DOUBLE)(time).usecs/(OV_DOUBLE)1e6)
/*
*   modul-declaration
*   -----------------
*/
#include "vdivde3696.h"
#include "libov/ov_macros.h"

OV_RESULT OV_DLLFNCEXPORT vdivde3696_c_constructor(
	OV_INSTPTR_ov_object	pobj
) {
	OV_RESULT res;
	OV_INSTPTR_vdivde3696_c pc;

	res = fb_functionblock_constructor(pobj);
	if (Ov_Fail(res)) return res;
	pc = Ov_StaticPtrCast(vdivde3696_c, pobj);
	pc->v_en = TRUE;
	pc->v_sphl = 100;
	pc->v_spdr = 100;
	pc->v_spur = 100;
	pc->v_reverse = TRUE;
	pc->v_pon = TRUE;
	pc->v_ion = TRUE;
	pc->v_don = TRUE;
	pc->v_kp = 1;
	pc->v_ti = 100;
	pc->v_t1totd = 0.1;
	pc->v_mvhl = 100;
	pc->v_mvdr = 100;
	pc->v_mvur = 100;
	return OV_ERR_OK;
}

void OV_DLLFNCEXPORT vdivde3696_c_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_fb_task		ptask;
	OV_INSTPTR_vdivde3696_c port = Ov_StaticPtrCast(vdivde3696_c, pfb);

	/*
	*   local variables
	*   ---------------
	*/
		float edz,eabs;			/* difference with deadzone and absolut difference */
		float t1,alpha;			/* needed for first order lag */
		float factor;			/* reverse or direct action (-1 or +1) */
		float mv_p,mv_i,mv_d;		/* proportional, integrative and derivative term */
		float mv_pid;			/* sum of i-, p- and d-term */
		float mv_pred;			/* mv predicted */
		float mv_z;			/* mv with disturbance variable feedforward */
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

	  /* external or internal setpoint */
	  if (port->v_spexton)
	  {
	    /* set setpoint output to external setpoint and internal setpoint active to FALSE */
	    port->v_sp = port->v_spext;
	    port->v_qspint = FALSE;

	    if (port->v_sptron) /* track mode active */
	    {
	      /* overwrite internal setpoint with external */
	      port->v_spint = port->v_spext;
	    }
	  }
	  else /* set setpoint output to internal setpoint and internal setpoint active to TRUE */
	  {
	    port->v_sp = port->v_spint;
	    port->v_qspint = TRUE;
	  }

	  /* setpoint limitation */
	  if (port->v_sp < port->v_spll) /* setpoint smaller than low limit ? */
	  {
	    /* set setpoint to low limit value */
	    port->v_sp = port->v_spll;
	    /* set low limit violation to TRUE and high limit violation to FALSE */
	    port->v_qspll = TRUE;
	    port->v_qsphl = FALSE;
	  }
	  else if (port->v_sp > port->v_sphl) /* setpoint larger than high limit ? */
	       {
		 /* set setpoint to high limit */
		 port->v_sp = port->v_sphl;
		 /* set low limit violation to FALSE and high limit violation to TRUE */
		 port->v_qspll = FALSE;
		 port->v_qsphl = TRUE;
	       }
	       else /* set both limitation violation ports to FALSE */
	       {
		 port->v_qspll = FALSE;
		 port->v_qsphl = FALSE;
	       }
	  
	  /* setpoint down/up rate limitation */
	  if (port->v_spdron && ((port->v_spold - port->v_sp) > port->v_spdr * cyctime))
	  {
	    port->v_sp = port->v_spold - port->v_spdr * cyctime;
	  }
	  else if (port->v_spuron && ((port->v_sp - port->v_spold) > port->v_spur * cyctime))
	       {
		 port->v_sp = port->v_spold + port->v_spur * cyctime;
	       }
	  
	  /* compute edz,i.e. port e with dead zone */
	  /* e is difference of setpoint and process value */
	  port->v_e = port->v_sp - port->v_pv;

	  if (port->v_deadz) /* deadzone active ? */
	  {
	    /* build abs(e) */
	    if (port->v_e > 0)
	    {
	      eabs = port->v_e;
	    }
	    else
	    {
	      eabs = -port->v_e;
	    }

	    /* substract half deadzone of eabs */
	    if (eabs < 0.5 * port->v_deadz) /* eabs less than half dead zone -> edz = 0 */
	    {
	      edz = 0;
	    }
	    else 
	    {
	      edz = eabs - 0.5 * port->v_deadz;  
	    }
	  }
	  else /* deadzone NOT active */
	  {
	    edz = port->v_e;
	  }
	
	  /* sign factor for reverse or direct action */
	  if (port->v_reverse)
	  {
	    factor = -1;
	  }
	  else
	  {
	    factor = 1;
	  }

	  /* initialize mv_pid */
	  mv_pid = 0;

	  /* compute and add p-term */
	  if (port->v_pon)
	  {
	    mv_p = factor * port->v_kp * edz;
	    mv_pid = mv_pid + mv_p;
	  }
	  else
	  {
	    mv_p = 0;
          }

	  /* compute and add d-term */
	  if (port->v_don)
	  {
	    switch (port->v_dmode) /* differentiation mode switch */
	    {
	    case 0 : /* setpoint minus process value */
	             mv_d = factor * port->v_kp * port->v_td * (edz - port->v_edzold)/cyctime;
		     break;
	    case 1 : /* minus process value */
	             mv_d = factor * port->v_kp * port->v_td * (port->v_pvold - port->v_pv)/cyctime;
		     break;
	    case 2:  /* setpoint */
	             mv_d = factor * port->v_kp * port->v_td * (port->v_sp - port->v_spold)/cyctime;
		     break;

	    default : /* not defined dmode value */
		      mv_d = 0;
	              port->v_eno = FALSE;
	    }
	    mv_pid = mv_pid + mv_d;
	  }
	  else
	  {
	    mv_d = 0;
	  }

	  /* compute i-term */
	  if (port->v_ireset) /* reset active ? */
	  {
	    /* set i-term to zero */
	    mv_i = 0;
	  }
	  else if (port->v_mvmanon || port->v_mvmanfc || (port->v_mvexton && port->v_mvtron))
	       /* in case of manual control value, force manaul or track mode compute *
		* mv_i so that mv_i + mv_d + mv_p equals mvold for bumpless switch   *
		* back to automatic mode */
	       {
		 mv_i = port->v_mvold - mv_p - mv_d;
		 if (port->v_zon)
		 {
		   mv_i = mv_i - port->v_z;
		 }
	       }
	       else if (port->v_ion)
	            {
		      /* normal computation of i-term */
		      if ((port->v_ti < TI_MIN) && (port->v_ti > -TI_MIN)) /* intergative time near zero ? */
		      {
			port->v_ti = TI_MIN;
			port->v_eno = FALSE;
		      }
		      mv_i = port->v_mviold + (factor * port->v_kp / port->v_ti)*cyctime * port->v_e;
		 
		      /* anti preset windup : keeps i-term constant if mv is near limit violation */
		      mv_pred = mv_p + mv_i + mv_d;	
		      if (port->v_zon)
		      {
			mv_pred = mv_pred + port->v_z;
		      }
		      if (((mv_pred > port->v_mvhl) && (mv_i > port->v_mviold))||((mv_pred < port->v_mvll) && (mv_i < port->v_mviold)))
		      {
			mv_i = port->v_mviold;
		      }
		    }
	            else /* ion = FALSE */
		    {
		      mv_i = port->v_mviold;
		    }
	  /* add i-term */
	  mv_pid = mv_pid + mv_i;
	  
	  /* first oder lag */
	  t1 = port->v_t1totd * port->v_td;
	  alpha = 1/(1 + t1/cyctime);
	  mv_pid = (1 - alpha) * port->v_mvpido + alpha * mv_pid;

	  /* compute mv_z */
	  if (port->v_zon)
	  {
	    mv_z = mv_pid + port->v_z;
	  }
	  else
	  {
	    mv_z = mv_pid;
	  }
	  
	  /* choose control value from mv_z, mvman and mvext */
	  if (port->v_mvmanon || port->v_mvmanfc) /* manual control value prior to external control value */
	  {
	    port->v_mv = port->v_mvman;
	    port->v_qmvman = TRUE;
	    port->v_qmvext = FALSE;
	  }
	  else if (port->v_mvexton) /* external control value prior to computed cotrol value */
	       {
		 port->v_mv = port->v_mvext;
		 port->v_qmvext = TRUE;
		 port->v_qmvman = FALSE;
	         if (port->v_mvtron) /* track mode active ? */
		 {
	 	   /* overwrite manual control value with external */
		   port->v_mvman = port->v_mvext;
		 }
	       }
	       else 
	       {
		 port->v_mv = mv_z;
		 port->v_qmvman = FALSE;
		 port->v_qmvext = FALSE;
		 port->v_mvman = mv_z; /* mvman follows mv_z */
	       }
	  
	  /* limitation of mv */
	  if (port->v_mv < port->v_mvll) /* low limit violation ? */
	  {
	    /* set control value to low limit */
	    port->v_mv = port->v_mvll;
	    /* set low limit violation to TRUE and high limit violation to FALSE */
	    port->v_qmvll = TRUE;
	    port->v_qmvhl = FALSE;
	  }
	  else if (port->v_mv > port->v_mvhl) /* high limit violation ? */
	       {
		 /* set control value to high limit */
		 port->v_mv = port->v_mvhl;
		 /* set lol limit violation to FALSE and high limit violation to TRUE */ 
		 port->v_qmvll = FALSE;
		 port->v_qmvhl = TRUE;
	       }
	       else /* set both limit violation ports to FALSE */
	       {
		 port->v_qmvll = FALSE;
		 port->v_qmvhl = FALSE;
	       }
	  
	  /* rate limitation of MV */
	  if (port->v_mvdron && ((port->v_mvold - port->v_mv) > port->v_mvdr * cyctime))
	  {
	    port->v_mv = port->v_mvold - port->v_mvdr * cyctime;
	  }
	  else if (port->v_mvuron && ((port->v_mv - port->v_mvold) > port->v_mvur * cyctime))
	       {
		 port->v_mv = port->v_mvold + port->v_mvur * cyctime;
	       }
	  /* store values for use in next cycle */
	  port->v_spold = port->v_sp;
	  port->v_edzold = edz;
	  port->v_mvold = port->v_mv;
	  port->v_mviold = mv_i;
	  port->v_mvpido = mv_pid;
	}
	else /* functionblock NOT enable */
        {
	  port->v_eno = FALSE;
	}

	/* store process value for use in next cycle */
        port->v_pvold = port->v_pv;

        /* add 1 to counter */
	port->v_counter = (port->v_counter+1)%10000;

/*******  end  *******/
	return;
	}







