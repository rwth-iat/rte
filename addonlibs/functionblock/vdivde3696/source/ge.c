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
*    greater equal comparison (VID/VDE 3696)                                *
*                                                                           *
****************************************************************************/
/*
*   type: ge
*   ========
*/
/*
*   history
*   -------
*   created : 4.5.1999
*   author  : Michael Klamann, Chair of Process Control Engineering, RWTH Aachen
*/
/*
*   description
*   -----------
*   This functionblock is similar to standardfunction GE in IEC 1131-3.
*   It is limited to two input values but extended by a tolerance input. 
*
*   U1 may be smaller than U2 up to the size of TOL and will still be regarded greater than U2.
*
*   note : Floating point values do not allways match the exact decimal fraction.
*          So equal comparisons may lude to false results.
*          To circumvent the problem DELTA_MIN is defined.
*          If large numbers (above 10000) are compared DELTA_MIN has to be altered (1E-4 or larger). 
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
#define DELTA_MIN 1E-5
/*
*   modul-declaration
*   -----------------
*/
#include "vdivde3696.h"
#include "libov/ov_macros.h"

OV_RESULT OV_DLLFNCEXPORT vdivde3696_ge_constructor(
	OV_INSTPTR_ov_object	pobj
) {
	OV_RESULT res;
	OV_INSTPTR_vdivde3696_ge pge;

	res = fb_functionblock_constructor(pobj);
	if (Ov_Fail(res)) return res;
	pge = Ov_StaticPtrCast(vdivde3696_ge, pobj);
	pge->v_en = TRUE;
	return OV_ERR_OK;
}

void OV_DLLFNCEXPORT vdivde3696_ge_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_vdivde3696_ge port = Ov_StaticPtrCast(vdivde3696_ge, pfb);

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
	  /* set enable ouput to true */
	  port->v_eno = TRUE;

	  /* greater equal comparison with tolerance */
	  if (port->v_u1 >= (port->v_u2 - port->v_tol - DELTA_MIN))
	  {
	    /* greater or equal */
	    port->v_q = TRUE;
	  }
	  else /* NOT greater or equal */
	  {
	    port->v_q = FALSE;
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







