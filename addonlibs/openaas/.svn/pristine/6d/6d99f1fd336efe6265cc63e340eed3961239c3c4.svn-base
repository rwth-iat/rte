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
*    functionblock                               			    *
*                                                       		    *
*    selection by 1-of-N bit (VDI/VDE 3696)                                 *
*                                                                           *
****************************************************************************/
/*
*   type: bitn
*   ==========
*/
/*
*   history
*   -------
*   created : 27.4.1999
*   author  : Michael Klamann, Chair of Process Control Engineering, RWTH Aachen
*/
/*
*   description
*   -----------
*   bitn sets v to u1 or u2 or ... u8, depending 
*   on the first input bit (i1 ... i8) with true status.
*   If no bit is true u0 will be the ouput value.
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

OV_RESULT OV_DLLFNCEXPORT vdivde3696_bitn_constructor(
	OV_INSTPTR_ov_object	pobj
) {
	OV_RESULT res;
	OV_INSTPTR_vdivde3696_bitn pbitn;

	res = fb_functionblock_constructor(pobj);
	if (Ov_Fail(res)) return res;
	pbitn = Ov_StaticPtrCast(vdivde3696_bitn, pobj);
	pbitn->v_en = TRUE;
	pbitn->v_u1 = 1;
	pbitn->v_u2 = 2;
	pbitn->v_u3 = 3;
	pbitn->v_u4 = 4;
	pbitn->v_u5 = 5;
	pbitn->v_u6 = 6;
	pbitn->v_u7 = 7;
	pbitn->v_u8 = 8;
	return OV_ERR_OK;
}


void OV_DLLFNCEXPORT vdivde3696_bitn_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_vdivde3696_bitn port = Ov_StaticPtrCast(vdivde3696_bitn, pfb);

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
	  /* determine the first true input bit and set output v to corelating value */
	  if (port->v_i1)
	      port->v_v = port->v_u1;
	  else if (port->v_i2)
	           port->v_v = port->v_u2;
	    else if (port->v_i3)
		     port->v_v = port->v_u3;
       	      else if (port->v_i4)
	               port->v_v = port->v_u4;
	        else if (port->v_i5)
		         port->v_v = port->v_u5;
	          else if (port->v_i6)
		           port->v_v = port->v_u6;
	            else if (port->v_i7)
		             port->v_v = port->v_u7;
	              else if (port->v_i8)
		               port->v_v = port->v_u8;
	                else port->v_v = port->v_u0;
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







