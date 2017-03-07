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



