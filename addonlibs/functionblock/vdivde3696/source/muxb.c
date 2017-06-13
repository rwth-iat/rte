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
*    functionblock                              			    *
*                                                       		    *
*    multiplexer for BOOL (VDI/VDE 3696)                                    *
*                                                                           *
****************************************************************************/
/*
*   type: muxb
*   ==========
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
*   Similar to standardfunction MUX in IEC 1131-3. Uses BOOL-inputs and is extended
*   by input I0.
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

OV_RESULT OV_DLLFNCEXPORT vdivde3696_muxb_constructor(
	OV_INSTPTR_ov_object	pobj
) {
	OV_RESULT res;
	OV_INSTPTR_vdivde3696_muxb pmuxb;

	res = fb_functionblock_constructor(pobj);
	if (Ov_Fail(res)) return res;
	pmuxb = Ov_StaticPtrCast(vdivde3696_muxb, pobj);
	pmuxb->v_en = TRUE;
	return OV_ERR_OK;
}

void OV_DLLFNCEXPORT vdivde3696_muxb_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_vdivde3696_muxb port = Ov_StaticPtrCast(vdivde3696_muxb, pfb);

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
	  /* set enable output to TRUE */
	  port->v_eno = TRUE;

	  /* write value of "Ik" in output q */
	  switch (port->v_k)
	  {
	    case 1 : port->v_q = port->v_i1;
	             break;
	    case 2 : port->v_q = port->v_i2;
	             break;
	    case 3 : port->v_q = port->v_i3;
	             break;
	    case 4 : port->v_q = port->v_i4;
	             break;
	    case 5 : port->v_q = port->v_i5;
	             break;
	    case 6 : port->v_q = port->v_i6;
	             break;
	    case 7 : port->v_q = port->v_i7;
	             break;
	    case 8 : port->v_q = port->v_i8;
	             break;
	    default : port->v_q = port->v_i0;
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










