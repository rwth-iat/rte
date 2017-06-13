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
*    boolean hand control value with switch (VDI/VDE 3696)                  *
*                                                        	   	    *
****************************************************************************/
/*
*   type: hb
*   ========
*/
/*
*   history
*   -------
*   created : 5.5.1999
*   author  : Michael Klamann, Chair of Process Control Engineering, RWTH Aachen
*/
/*
*   description
*   -----------
*   Is QEXTON active the ouput value Q will be the external control value, otherwise the 
*   hand control value will be given. QQEXT shows which value is active.
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

OV_RESULT OV_DLLFNCEXPORT vdivde3696_hb_constructor(
	OV_INSTPTR_ov_object	pobj
) {
	OV_RESULT res;
	OV_INSTPTR_vdivde3696_hb phb;

	res = fb_functionblock_constructor(pobj);
	if (Ov_Fail(res)) return res;
	phb = Ov_StaticPtrCast(vdivde3696_hb, pobj);
	phb->v_en = TRUE;
	return OV_ERR_OK;
}

void OV_DLLFNCEXPORT vdivde3696_hb_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_vdivde3696_hb port = Ov_StaticPtrCast(vdivde3696_hb, pfb);

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

	  /* determine if qexton is active */	  	
	  if (port->v_qexton)
	  {
	    /* active -> output is set to external control value */
	    /*        -> qqext is set to true */
	    port->v_q = port->v_qext;
	    port->v_qqext = TRUE;
	  }
	  else /* NOT active -> output is set to hand control value and qqext to false */
	  {
	    port->v_q = port->v_qman;
	    port->v_qqext = FALSE;
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







