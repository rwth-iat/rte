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
*                                                          		    *
*    linear scaling (VDI/VDE 3696)                                          *
*                                                                           *
****************************************************************************/
/*
*   type: scal
*   ==========
*/
/*
*   history
*   -------
*   created : 21.5.1999
*   author  : Michael Klamann, Chair of Process Control Engineering, RWTH Aachen
*/
/*
*   description
*   -----------
*   V = SCAL * U + OFFS
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

OV_RESULT OV_DLLFNCEXPORT vdivde3696_scal_constructor(
	OV_INSTPTR_ov_object	pobj
) {
	OV_RESULT res;
	OV_INSTPTR_vdivde3696_scal pscal;

	res = fb_functionblock_constructor(pobj);
	if (Ov_Fail(res)) return res;
	pscal = Ov_StaticPtrCast(vdivde3696_scal, pobj);
	pscal->v_en = TRUE;
	pscal->v_scal = 1;
	return OV_ERR_OK;
}

void OV_DLLFNCEXPORT vdivde3696_scal_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_vdivde3696_scal port = Ov_StaticPtrCast(vdivde3696_scal, pfb);

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

	  /* compute output v */
	  port->v_v = port->v_scal * port->v_u + port->v_offs;
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







