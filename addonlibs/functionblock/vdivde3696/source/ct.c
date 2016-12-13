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
*    counter, flow counter (VDI/VDE 3696)                                   *
*                                                               	    *
****************************************************************************/
/*
*   type: ct
*   ========
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
*   This functionblock is counts pulses of the input signal "i".
*   On every rising edge of "i" the ouput increases/decreases by "incr".
*   if "down" is TRUE the output decreases else it increases. 
*
*   On initialization and on active "reset" the output is set to "v0".
*   The output is also watched for limitation values and limits.
*
*   This functionblock is an extension of the standarfunctions CTU,CTD and CTUD in IEC 1131-3.
*
*   note : To work properly the time difference between 
*          edges of "i" have to be larger than cycle time "ocyc".
*
*          The input digits exists but is not used.
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

OV_RESULT OV_DLLFNCEXPORT vdivde3696_ct_constructor(
	OV_INSTPTR_ov_object	pobj
) {
	OV_RESULT res;
	OV_INSTPTR_vdivde3696_ct pct;

	res = fb_functionblock_constructor(pobj);
	if (Ov_Fail(res)) return res;
	pct = Ov_StaticPtrCast(vdivde3696_ct, pobj);
	pct->v_ll = -1e19;
	pct->v_hl = 1e19;
	pct->v_lls = -1e19;
	pct->v_hhs = 1e19;
	pct->v_digits = 6;
	pct->v_incr = 1;
	pct->v_en = TRUE;
	return OV_ERR_OK;
}

void OV_DLLFNCEXPORT vdivde3696_ct_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_vdivde3696_ct port = Ov_StaticPtrCast(vdivde3696_ct, pfb);
	/*
	*   local variables
	*   ---------------
	*   none
	*/
	/*
	*   instructions
	*   ------------
	*/

/* special instruction on initilization */ 
        if (port->v_counter == 0)
        {
	  port->v_v = port->v_v0;
	}
/* end special instruction */

        if (port->v_en) /* functionblock enable */
        {
	  /* set enable output to TRUE */
          port->v_eno = TRUE;

          if (port->v_reset) /* reset active ? */
	  {
	    port->v_v = port->v_v0;
	  }
	  else if (port->v_i && (port->v_iold == FALSE) && (port->v_hold == FALSE)) /* rising edge and no hold mode ? */
	       {
	         if (port->v_down) /* count down ? */
	         {
		   /* subtract incr */
		   port->v_v = port->v_v - port->v_incr;
	         }
	         else /* count up */
	         {
		   /* add incr */
		   port->v_v = port->v_v + port->v_incr;
	         }
	       }

	  /* check if v is v0 */ 
	  if (port->v_v == port->v_v0)
	  {
	    port->v_qv0 = TRUE;
	  }
	  else
	  {
	    port->v_qv0 = FALSE;
	  }

	  /* limit and limitation checks */
          if (port->v_v >= port->v_hl)
	  {
	    port->v_v = port->v_hl;
	    port->v_qhl = TRUE;
	  }
	  else
	  {
	    port->v_qhl = FALSE;
	    if (port->v_v <= port->v_ll)
	    {
	      port->v_v = port->v_ll;
	      port->v_qll = TRUE;
	    }
	    else
	    {
	      port->v_qll = FALSE;
	    }
	  }

	  if (port->v_v < (port->v_lls + port->v_deltals))
	  {
	    port->v_qls = TRUE;
	    if (port->v_v < port->v_lls)
	    {
	      port->v_qlls = TRUE;
	    }
	    else
	    {
	      port->v_qlls = FALSE;
	    }
	  }
	  else 
	  {
	    port->v_qls = FALSE;
	    port->v_qlls = FALSE;
	    if (port->v_v > (port->v_hhs - port->v_deltahs))
	    {
	      port->v_qhs = TRUE;
	      if (port->v_v > port->v_hhs)
	      {
	        port->v_qhhs = TRUE;
	      }
	      else
	      {
	        port->v_qhhs = FALSE;
	      }
	    }
	    else
	    {
	      port->v_qhs = FALSE;
	      port->v_qhhs = FALSE;
	    }
          }
	}
	else /* functionblock NOT enable */
        {
	  port->v_eno = FALSE;
	}

	/* store input i value for use in next cycle */
        port->v_iold = port->v_i;

        /* add 1 to counter */
	port->v_counter = (port->v_counter+1)%10000;

/*******  end  *******/
	return;
	}








