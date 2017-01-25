/*
*	Copyright (C) 1998, 1999
*	Chair of Process Control Engineering,
*	Aachen University of Technology.
*	All rights reserved.
*
*	Redistribution and use in source and binary forms, with or without
*	modification, are permitted provided that the following conditions
*	are met:
*	1. Redistributions of source code must retain the above copyright
*	   notice, this list of conditions and the following disclaimer.
*	2. Redistributions in binary form must print or display the above
*	   copyright notice either during startup or must have a means for
*	   the user to view the copyright notice.
*	3. Redistributions in binary form must reproduce the above copyright
*	   notice, this list of conditions and the following disclaimer in the
*	   documentation and/or other materials provided with the distribution.
*	4. Neither the name of the Chair of Process Control Engineering nor the
*	   name of the Aachen University of Technology may be used to endorse or
*	   promote products derived from this software without specific prior
*	   written permission.
*
*	THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
*	``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
*	TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
*	PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE CHAIR OF PROCESS CONTROL
*	ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*	EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
*	PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
*	OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
*	WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
*	OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*	ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/****************************************************************************
*									    *
*    F u n k t i o n s b a u s t e i n t y p				    *
*    Standard-Funktionsbaustein nach VDI/VDE 3696            		    *
*		                                                            *
*    Arcussinus (arcus sine)                                                *
****************************************************************************/
/*
*   type: asin
*   ==========
*/
/*
*   history
*   -------
*   created : 21.4.1999
*   author  : Michael Klamann, Chair of Process Control Engineering, RWTH Aachen
*/
/*
*   description
*   -----------
*   Equal to standardfunction ASIN in IEC 1131-3.
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

OV_RESULT OV_DLLFNCEXPORT vdivde3696_asin_constructor(
	OV_INSTPTR_ov_object	pobj
) {
	OV_RESULT res;
	OV_INSTPTR_vdivde3696_asin pasin;

	res = fb_functionblock_constructor(pobj);
	if (Ov_Fail(res)) return res;
	pasin = Ov_StaticPtrCast(vdivde3696_asin, pobj);
	pasin->v_en = TRUE;
	return OV_ERR_OK;
}

void OV_DLLFNCEXPORT vdivde3696_asin_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_vdivde3696_asin port = Ov_StaticPtrCast(vdivde3696_asin, pfb);

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
	  if (port->v_u > 1) /* input to large */
	  {
	    port->v_v = 1.5708;
	    port->v_eno = FALSE;
	  }
	  else if (port->v_u < -1) /* input to small */
	       {
		 port->v_v = -1.5708;
		 port->v_eno = FALSE;
	       }
	       else /* compute asin (u) */
	       {
		 port->v_v = asin(port->v_u);
		 port->v_eno = TRUE;
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







