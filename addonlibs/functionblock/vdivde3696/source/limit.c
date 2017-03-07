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
*    limiter (VDI/VDE 3696)                                                 *
*                                                                           *
****************************************************************************/
/*
*   type: limit
*   ===========
*/
/*
*   history
*   -------
*   created : 10.5.1999
*   author  : Michael Klamann, Chair of Process Control Engineering, RWTH Aachen
*/
/*
*   description
*   -----------
*   Similar to standardfunction LIMIT in IEC 1131-3.
*   Extended by the outputs QHL and QLL.
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

OV_RESULT OV_DLLFNCEXPORT vdivde3696_limit_constructor(
	OV_INSTPTR_ov_object	pobj
) {
	OV_RESULT res;
	OV_INSTPTR_vdivde3696_limit plimit;

	res = fb_functionblock_constructor(pobj);
	if (Ov_Fail(res)) return res;
	plimit = Ov_StaticPtrCast(vdivde3696_limit, pobj);
	plimit->v_en = TRUE;
	plimit->v_hl = 100;
	return OV_ERR_OK;
}

void OV_DLLFNCEXPORT vdivde3696_limit_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_vdivde3696_limit port = Ov_StaticPtrCast(vdivde3696_limit, pfb);

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

	  /* check high limit violation */
	  if (port->v_u >= port->v_hl)
	  {
	    /* set output to high limit, qhl to true and qll to false */
	    port->v_v = port->v_hl;
	    port->v_qhl = TRUE;
	    port->v_qll = FALSE;
	  }
	  else /* no high limit violation */
	  {
	    /* set qhl to false */
	    port->v_qhl = FALSE;

	    /* check low limit violation */ 
	    if (port->v_u <= port->v_ll)
	    {
	      /* set output to low limit and qll to true */
	      port->v_v = port->v_ll;
	      port->v_qll = TRUE;
	    }
	    else /* no limit violation */
	    {
	      /* set output to input value and qll to false */
	      port->v_qll = FALSE;
	      port->v_v = port->v_u;
	    }
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







