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
*   functionblock                       				    *
*                                                       		    *
*   nonlinear static function (VDI/VDE 3696)                                *
*                                                                           *
****************************************************************************/
/*
*   type: nonlin
*   ============
*/
/*
*   history
*   -------
*   created : 18.5.1999
*   author  : Michael Klamann, Chair of Process Control Engineering, RWTH Aachen
*/
/*
*   description
*   -----------
*   At first the value pairs "U1","V1 to "Un","Vn" will be converted 
*   into two vectors (u_field,v_field). "n" is specified by
*   the port of same denominator.
*   Then the u_field-vector will be sorted (u0<u1<u2...).
*   Now the input value u is compared with this vector :
*	- if the value is not in the domain [u0,un], the correlating
*         limit (v0 or vn ) will be the result. 
*       - if the value matches a checkpoint the correlating
*         v_field-value will be the result.
*       - if the value is between two checkpoints, the result
*         will be caclculated by linear interpolation.
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

OV_RESULT OV_DLLFNCEXPORT vdivde3696_nonlin_constructor(
	OV_INSTPTR_ov_object	pobj
) {
	OV_RESULT res;
	OV_INSTPTR_vdivde3696_nonlin pnonlin;

	res = fb_functionblock_constructor(pobj);
	if (Ov_Fail(res)) return res;
	pnonlin = Ov_StaticPtrCast(vdivde3696_nonlin, pobj);
	pnonlin->v_en = TRUE;
	return OV_ERR_OK;
}

void OV_DLLFNCEXPORT vdivde3696_nonlin_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_vdivde3696_nonlin port = Ov_StaticPtrCast(vdivde3696_nonlin, pfb);

	/*
	*   local variables
	*   ---------------
	*/
		float u_field[10],v_field[10],u_temp,v_temp;
	        int i,j,count;
       
	/*
	*   instructions
	*   ------------
	*/
 
        if (port->v_n > 1 && port->v_en) /* functionblock only enabled when at least two checkpoints exist */
        {
	  /* set enable output to TRUE */
	  port->v_eno = TRUE;

	  /* readout of checkpoints */
	  u_field[0] = port->v_u1;
	  v_field[0] = port->v_v1;
	  u_field[1] = port->v_u2;
	  v_field[1] = port->v_v2;
	  count = 2;
	  if (port->v_n > count)
	  {
	    u_field[2] = port->v_u3;
	    v_field[2] = port->v_v3;
	    count = 3;
	    if (port->v_n > count)
	    {
	      u_field[3] = port->v_u4;
	      v_field[3] = port->v_v4;
	      count = 4;
	      if (port->v_n > count)
	      {
 		u_field[4] = port->v_u5;
		v_field[4] = port->v_v5;
		count = 5;
		if (port->v_n > count)
		{
		  u_field[5] = port->v_u6;
		  v_field[5] = port->v_v6;
		  count = 6;
		  if (port->v_n > count)
		  {
		    u_field[6] = port->v_u7;
		    v_field[6] = port->v_v7;
		    count = 7;
		    if (port->v_n > count)
		    {
		      u_field[7] = port->v_u8;
		      v_field[7] = port->v_v8;
		      count = 8;
		      if (port->v_n > count)
		      {
			u_field[8] = port->v_u9;
			v_field[8] = port->v_v9;
			count = 9;
			if (port->v_n > count)
			{
			  u_field[9] = port->v_u10;
			  v_field[9] = port->v_v10;
			  if (port->v_n > 10)
			    port->v_n = 10;
			}
		      }
		    }
		  }
		}
	      }
	    }
	  }
	} /* end readout */
        else /* functionblock NOT enable or less than two checkpoints */
        {
	  port->v_eno = FALSE;
	  goto BREAK_OFF;
	}

/*** sorting of checkpoint vector ***/
        for (i = 1;i < port->v_n;i++)
        {
	  for (j = 1;j < port->v_n;j++)
	  {
	    if (u_field[j-1] > u_field[j])
	    {
	      u_temp = u_field[j-1];
	      v_temp = v_field[j-1];
	      u_field[j-1] = u_field[j];
	      v_field[j-1] = v_field[j];
	      u_field[j] = u_temp;
	      v_field[j] = v_temp;
	    }
	  }
	}
/*** end sorting ***/

/*** compare "u" with u_field ***/
        if (port->v_u < u_field[0])
        {
	  /* u smaller than low limit */
	  port->v_v = v_field[0];
	  goto BREAK_OFF;
	}
        else if (port->v_u > u_field[port->v_n -1])
             {
	       /* u greater than high limit */
	       port->v_v = v_field [port->v_n -1];
	       goto BREAK_OFF;
	     }
             else for (i = 1;i < port->v_n;i++) 
	            if (port->v_u == u_field[i-1]) /* check if u matches a checkpoint */
	            {
		      port->v_v = v_field[i-1];
		      goto BREAK_OFF;
		    }
	            else if (port->v_u > u_field[i-1] && port->v_u < u_field[i]) /* check if u is between two consecutive u_field values */ 
                         {
			   j = i;
			 }
/*** end comparison ***/

	/* linear interpolation */
        port->v_v = v_field[j-1] + (v_field[j] - v_field[j-1]) * (port->v_u - u_field[j-1])/(u_field[j] - u_field[j-1]);

/* goto TARGET */ 
BREAK_OFF:

        /* add 1 to counter */
	port->v_counter = (port->v_counter+1)%10000;

/*******  end  *******/
	return;
	}



