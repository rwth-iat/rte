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
*    boolean AND (VDI/VDE 3696)                                             *
*                                                                           *
****************************************************************************/
/*
*   type: and
*   =========
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
*   Equal to standardfunction AND in IEC 1131-3.
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

OV_RESULT OV_DLLFNCEXPORT vdivde3696_and_constructor(
	OV_INSTPTR_ov_object	pobj
) {
	OV_RESULT res;
	OV_INSTPTR_vdivde3696_and pand;

	res = fb_functionblock_constructor(pobj);
	if (Ov_Fail(res)) return res;
	pand = Ov_StaticPtrCast(vdivde3696_and, pobj);
	pand->v_en = TRUE;
	pand->v_i2 = TRUE;
	pand->v_i3 = TRUE;
	pand->v_i4 = TRUE;
	pand->v_i5 = TRUE;
	pand->v_i6 = TRUE;
	pand->v_i7 = TRUE;
	pand->v_i8 = TRUE;
	return OV_ERR_OK;
}

void OV_DLLFNCEXPORT vdivde3696_and_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_vdivde3696_and port = Ov_StaticPtrCast(vdivde3696_and, pfb);

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
        /* output is set to true if all input-port values are true */
	{
	  if (port->v_i1 && port->v_i2 && port->v_i3 && port->v_i4 && port->v_i5 && port->v_i6 && port->v_i7 && port->v_i8)
	  {
	    port->v_q = TRUE;
	  }
	  else 
	  {
	    port->v_q = FALSE;
	  }
	  port->v_eno = TRUE;
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







