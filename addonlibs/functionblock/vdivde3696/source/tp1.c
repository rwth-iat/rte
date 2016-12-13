/*
*   Copyright (C) 2007
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
*                                                        		    *
*    monostable element (VDI/VDE 3696)                                      *
*                                                                           *
****************************************************************************/
/*
*   type: tp1
*   =========
*/
/*
*   history
*   -------
*   created : 23.6.1999
*   author  : Michael Klamann, Chair of Process Control Engineering, RWTH Aachen
*/
/*
*   description
*   -----------
*   A rising edge of the input signal i triggers an output impuls with 
*   the length of "time".
*   If there is another rising edge before "time" has elapsed it will be ignored. 
*   
*   Similar to standardfunction TP in IEC 1131-3.
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

OV_RESULT OV_DLLFNCEXPORT vdivde3696_tp1_constructor(
	OV_INSTPTR_ov_object	pobj
) {
	OV_RESULT res;
	OV_INSTPTR_vdivde3696_tp1 ptp1;

	res = fb_functionblock_constructor(pobj);
	if (Ov_Fail(res)) return res;
	ptp1 = Ov_StaticPtrCast(vdivde3696_tp1, pobj);
	ptp1->v_en = TRUE;
	return OV_ERR_OK;
}

void OV_DLLFNCEXPORT vdivde3696_tp1_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_fb_task		ptask;
	OV_INSTPTR_vdivde3696_tp1 port = Ov_StaticPtrCast(vdivde3696_tp1, pfb);

	/*
	*   local variables
	*   ---------------
	*/
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

	  if (port->v_q == FALSE) /* FALSE output signal ? */
	  {
	    /* set time counter to zero */
	    port->v_etime = 0;

	    if (port->v_i && port->v_iold == FALSE) /* rising edge ? */
	    {
	      /* set output to TRUE */
	      port->v_q = TRUE;
	    }
	  }
	  else /* q is TRUE */
	  {
	    if (port->v_etime >= port->v_time) /* elapsed time greater than time span */
	    {
	      /* set output q to FALSE */
	      port->v_q = FALSE;
	    }
	    else /* add cycle time to elased time */
	    {
	      port->v_etime = port->v_etime + cyctime;
	    }
	  }
	}
        else /* functionblock NOT enable */
        {
	  port->v_eno = FALSE;
	}

	/* store input value for use in next cycle */
        port->v_iold = port->v_i;

        /* add 1 to counter */
	port->v_counter = (port->v_counter+1)%10000;

/*******  end  *******/
	return;
	}






