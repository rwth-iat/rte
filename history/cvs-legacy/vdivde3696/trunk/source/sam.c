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
*                                                         		    *
*    limit switch and alarm or message storing (VDI/VDE 3696)               *
*                                                                           *
****************************************************************************/
/*
*   type: sam
*   =========
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
*   The input pvmode determines in which way pvmod is generated:
*	- 0 : pvmod = pv
*	- 1 : pvmod = pv - pvref
*	- 2 : pvmod = pv / pvref
*
*   The pvmod value is then checked for limit violation. Low decdides if the 
*   value should be larger (low = TRUE) or less (low = FALSE) than limit s.
*   A limit violation ludes to a TRUE qs output.
*  
*   If aon is TRUE the ouptut signal qs is watched as described in functionblock am.
*/
/*
*   C-Includes
*   ----------
*   none
*/
/*  
*   defines
*   -------
*   none
*/
/*
*   modul-declaration
*   -----------------
*/
#include "vdivde3696.h"
#include "libov/ov_macros.h"

OV_RESULT OV_DLLFNCEXPORT vdivde3696_sam_constructor(
	OV_INSTPTR_ov_object	pobj
) {
	OV_RESULT res;
	OV_INSTPTR_vdivde3696_sam psam;

	res = fb_functionblock_constructor(pobj);
	if (Ov_Fail(res)) return res;
	psam = Ov_StaticPtrCast(vdivde3696_sam, pobj);
	psam->v_en = TRUE;
	psam->v_smin = -1e19;
	psam->v_smax = 1e19;
	ov_string_setvalue (&psam->v_amode , "A1");
	return OV_ERR_OK;
}

void OV_DLLFNCEXPORT vdivde3696_sam_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_vdivde3696_sam port = Ov_StaticPtrCast(vdivde3696_sam, pfb);

	/*
	*   local variables
	*   ---------------
	*/
 	       float smod;
	/*
	*   instructions
	*   ------------
	*/
        if (port->v_en) /* functionblock enable */
        {
	  /* set enable ouput to TRUE */
	  port->v_eno = TRUE;

	  /* generate pvmod */
	  switch (port->v_pvmode)
	  {
	  case 0 : port->v_pvmod = port->v_pv;
	           break;
	  case 1 : port->v_pvmod = port->v_pv - port->v_pvref;
	           break;
	  case 2 : if (port->v_pvref == 0) /* division by zero ? */
	           {
		     port->v_eno = FALSE;
		     port->v_pvmod = 0;
		   }
	           else
		   {
		     port->v_pvmod = port->v_pv / port->v_pvref;
		   }
	           break;
	  default : port->v_eno = FALSE; /* not defined pvmode status */
	  }

	  /* check limit s for high and low limit */
	  if (port->v_s < port->v_smin)
	  {
	    smod = port->v_smin;
	  }
	  else if (port->v_s > port->v_smax)
	       {
		 smod = port->v_smax;
	       }
	       else
	       {
		 smod = port->v_s;
	       }
	  
	  /* limit check : compare PVMOD und smod */
	  if (port->v_low) /* s is low limit */
	  {
	    if (port->v_qsold == 0 && port->v_pvmod < smod) /* new low limit violation ? */
	    {
	      /* set qs to TRUE */
	      port->v_qs = TRUE;
	    }
	    else if (port->v_qsold && (port->v_pvmod > smod + port->v_shys)) /* old limit violation over ? */
	         {
		   /* set qs to FALSE */
		   port->v_qs = FALSE;
		 }
	         else /* no change in qs port */
		 {
		   port->v_qs = port->v_qsold;
		 }
	  }
	  else /* s is high limit */
	    {
	      if (port->v_qsold == 0 && port->v_pvmod > smod) /* new high limit violation ? */
	      {
		/* set qs to TRUE */
		port->v_qs = TRUE;
	      }
	      else if (port->v_qsold && (port->v_pvmod < smod - port->v_shys)) /* old limit violation over ? */
		   {
		     /* set qs to FALSE */
		     port->v_qs = FALSE;
		   }
	           else /* no change in qs port */
		   {
		     port->v_qs = port->v_qsold;
		   }
	    }
	  
	  /* alarm or message generator */
	  if (port->v_aon)
	  {
	    /* overwrite old alarm/message */
	    port->v_v = FALSE;

	    if (strcmp(port->v_amode,"A1") == 0)
	    {
	      /* alarm if qs is TRUE */
	      if (port->v_qs)
	      { 
		port->v_v = TRUE;
	      }
	    }
	    else if (strcmp(port->v_amode,"A0") == 0)
	         {
		   /* alarm if qs is FALSE */
		   if (port->v_qs == FALSE)
		   {
		     port->v_v = TRUE;
		   }  
		 }
	         else if (strcmp(port->v_amode,"MR") == 0)
		      {
			/* message if rising edge */
			if ((port->v_qs - port->v_qsold) > 0)
		        {
			  port->v_v = TRUE;
			}
		      }
	              else if (strcmp(port->v_amode,"MF") == 0)
		           {
			     /* message if falling edge */
			     if ((port->v_qs - port->v_qsold) < 0)
			     {
			       port->v_v = TRUE;
			     }
			   }
	                   else if (strcmp(port->v_amode,"MB") == 0)
			        {
				  /* message on both edges */
				  if ((port->v_qs - port->v_qsold) != 0)
				  {
				    port->v_v = TRUE;
				  }
				}
	                        else /* not defined amode status */
				{
				  port->v_eno = FALSE;
				}
	  }
	}
        else /* functionblock NOT enable */
        {
	  port->v_eno = FALSE;
	}

	/* store qs as qsold for next cycle */
        port->v_qsold = port->v_qs;

        /* add 1 to counter */
	port->v_counter = (port->v_counter+1)%10000;

/*******  end  *******/
	return;
	}







