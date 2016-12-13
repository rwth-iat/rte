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
*                                                       		    *
*    demultiplexer for NUM (VDI/VDE 3696)                                   *
*                                                                           *
****************************************************************************/
/*
*   type: demuxn
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
*   Except "Vk" all output values are set to the correlating input values.
*   "Vk" is set to U.
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

OV_RESULT OV_DLLFNCEXPORT vdivde3696_demuxn_constructor(
	OV_INSTPTR_ov_object	pobj
) {
	OV_RESULT res;
	OV_INSTPTR_vdivde3696_demuxn pdemuxn;

	res = fb_functionblock_constructor(pobj);
	if (Ov_Fail(res)) return res;
	pdemuxn = Ov_StaticPtrCast(vdivde3696_demuxn, pobj);
	pdemuxn->v_en = TRUE;
	pdemuxn->v_u = 1;
	return OV_ERR_OK;
}

void OV_DLLFNCEXPORT vdivde3696_demuxn_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_vdivde3696_demuxn port = Ov_StaticPtrCast(vdivde3696_demuxn, pfb);

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

	  /* set all ouput ports to correlating input values */
	  port->v_v0 = port->v_u0;
	  port->v_v1 = port->v_u1;
	  port->v_v2 = port->v_u2;
	  port->v_v3 = port->v_u3;
	  port->v_v4 = port->v_u4;
	  port->v_v5 = port->v_u5;
	  port->v_v6 = port->v_u6;
	  port->v_v7 = port->v_u7;
	  port->v_v8 = port->v_u8;
	  
	  /* set channel "Vk" to port U value */
	  switch(port->v_k)
	  {
	  case 1 : port->v_v1 = port->v_u;
	           break;
	  case 2 : port->v_v2 = port->v_u;
	           break;
	  case 3 : port->v_v3 = port->v_u;
	           break;
	  case 4 : port->v_v4 = port->v_u;
                   break;
	  case 5:  port->v_v5 = port->v_u;
                   break;
	  case 6 : port->v_v6 = port->v_u;
                   break;
	  case 7 : port->v_v7 = port->v_u;
	           break;
	  case 8 : port->v_v8 = port->v_u;
	           break;
	  default : port->v_v0 = port->v_u;
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










