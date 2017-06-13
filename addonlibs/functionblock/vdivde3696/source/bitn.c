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







