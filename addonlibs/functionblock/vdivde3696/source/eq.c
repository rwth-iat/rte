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
*    functionblock                                                          *
*                                                                           *
*    equal comparison (VDI/VDE 3696)                                        *
*                                                                           *
****************************************************************************/
/*
*   type: eq
*   ========
*/
/*
*   history
*   -------
*   created : 28.4.1999
*   author  : Michael Klamann, Chair of Process Control Engineering, RWTH Aachen
*/
/*
*   description
*   -----------
*   This functionblock is similar to standarfunctionn EQ in IEC 1131-3.
*   It is limited to two input values but extended by a tolerance input.
*/
/*
*   C-Includes
*   ----------
*/
#include <math.h>
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

OV_RESULT OV_DLLFNCEXPORT vdivde3696_eq_constructor(
	OV_INSTPTR_ov_object	pobj
) {
	OV_RESULT res;
	OV_INSTPTR_vdivde3696_eq peq;

	res = fb_functionblock_constructor(pobj);
	if (Ov_Fail(res)) return res;
	peq = Ov_StaticPtrCast(vdivde3696_eq, pobj);
	peq->v_en = TRUE;
	return OV_ERR_OK;
}

void OV_DLLFNCEXPORT vdivde3696_eq_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_vdivde3696_eq port = Ov_StaticPtrCast(vdivde3696_eq, pfb);

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
	/* set enable output to true and compare u1 and u2 */
        {
	  port->v_eno = TRUE;
          if (fabs(port->v_u1 - port->v_u2) <= port->v_tol)
	  {
	    port->v_q = TRUE; /* u1 and u2 are equal */
	  }
	  else
	  {
	    port->v_q = FALSE; /* u1 and u2 are NOT equal */
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







