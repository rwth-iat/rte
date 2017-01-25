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










