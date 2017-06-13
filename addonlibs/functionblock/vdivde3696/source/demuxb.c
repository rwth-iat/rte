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
*                                                        		    *
*    demultiplexer for BOOL (VDI/VDE 3696)                                  *
*                                                                           *
****************************************************************************/
/*
*   type: demuxb
*   =============
*/
/*
*   history
*   -------
*   created : 12.5.1999
*   author  : Michael Klamann, Chair of Process Control Engineering, RWTH Aachen
*/
/*
*   description
*   -----------
*   Except "Qk" all output values are set to the correlating input values.
*   "Qk" is set to I.
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

OV_RESULT OV_DLLFNCEXPORT vdivde3696_demuxb_constructor(
	OV_INSTPTR_ov_object	pobj
) {
	OV_RESULT res;
	OV_INSTPTR_vdivde3696_demuxb pdemuxb;

	res = fb_functionblock_constructor(pobj);
	if (Ov_Fail(res)) return res;
	pdemuxb = Ov_StaticPtrCast(vdivde3696_demuxb, pobj);
	pdemuxb->v_en = TRUE;
	pdemuxb->v_i = 1;
	return OV_ERR_OK;
}

void OV_DLLFNCEXPORT vdivde3696_demuxb_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_vdivde3696_demuxb port = Ov_StaticPtrCast(vdivde3696_demuxb, pfb);

	/*
	*   local variables
	*   ---------------
	*   none
	*/
	/*
	*   instructions
	*   ------------
	*/

        if (port->v_en) /* functionblock enable */
        {
	  /* set enable output to true */
	  port->v_eno = TRUE;

	  /* set all outputs to correlating input values */
	  port->v_q0 = port->v_i0;
	  port->v_q1 = port->v_i1;
	  port->v_q2 = port->v_i2;
	  port->v_q3 = port->v_i3;
	  port->v_q4 = port->v_i4;
	  port->v_q5 = port->v_i5;
	  port->v_q6 = port->v_i6;
	  port->v_q7 = port->v_i7;
	  port->v_q8 = port->v_i8;
	  
	  /* modify channel k output */
	  switch(port->v_k)
	  {
	  case 1 : port->v_q1 = port->v_i;
	           break;
	  case 2 : port->v_q2 = port->v_i;
	           break;
	  case 3 : port->v_q3 = port->v_i;
	           break;
	  case 4 : port->v_q4 = port->v_i;
                   break;
	  case 5:  port->v_q5 = port->v_i;
                   break;
	  case 6 : port->v_q6 = port->v_i;
                   break;
	  case 7 : port->v_q7 = port->v_i;
	           break;
	  case 8 : port->v_q8 = port->v_i;
	           break;
	  default : port->v_q0 = port->v_i;
	  }
	}
	else /* functionblock NOT enable */
        {
	  port->v_eno = FALSE;
	}

        /* add 1 to counter */
	port->v_counter = (port->v_counter+1)%10000;

/*******  end  *******/
	return;
	}










