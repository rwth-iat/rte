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
*    Standard-Funktionsbaustein nach VDI/VDE 3696            		    *
*		                                                            *
*    Minimalwertauswahl (minimum selection)                                 *
****************************************************************************/
/*
*   Typ: MIN_
*   =========
*/
/*
*   history
*   -------
*   created : 11.5.1999
*   author  : Michael Klamann, Chair of Process Control Engineering, RWTH Aachen
*/
/*
*   description
*   -----------
*   Equal to standardfunction MIN in IEC 1131-3.
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

OV_RESULT OV_DLLFNCEXPORT vdivde3696_min_constructor(
	OV_INSTPTR_ov_object	pobj
) {
	OV_RESULT res;
	OV_INSTPTR_vdivde3696_min pmin;

	res = fb_functionblock_constructor(pobj);
	if (Ov_Fail(res)) return res;
	pmin = Ov_StaticPtrCast(vdivde3696_min, pobj);
	pmin->v_en = TRUE;
	pmin->v_u2 = 1e19;
	pmin->v_u3 = 1e19;
	pmin->v_u4 = 1e19;
	pmin->v_u5 = 1e19;
	pmin->v_u6 = 1e19;
	pmin->v_u7 = 1e19;
	pmin->v_u8 = 1e19;
	return OV_ERR_OK;
}

void OV_DLLFNCEXPORT vdivde3696_min_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_vdivde3696_min port = Ov_StaticPtrCast(vdivde3696_min, pfb);

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

	  /* check all eight inputs for minimum value */
	  port->v_v = port->v_u1;
	  if (port->v_u2 < port->v_v)
	  {
	    port->v_v = port->v_u2;
	  }
	  if (port->v_u3 < port->v_v)
	  {
	    port->v_v = port->v_u3;
	  }
	  if (port->v_u4 < port->v_v)
	  {
	    port->v_v = port->v_u4;
	  }
	  if (port->v_u5 < port->v_v)
	  {
	    port->v_v = port->v_u5;
	  }
	  if (port->v_u6 < port->v_v)
	  {
	    port->v_v = port->v_u6;
	  }
	  if (port->v_u7 < port->v_v)
	  {
	    port->v_v = port->v_u7;
	  }
	  if (port->v_u8 < port->v_v)
	  {
	    port->v_v = port->v_u8;
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










