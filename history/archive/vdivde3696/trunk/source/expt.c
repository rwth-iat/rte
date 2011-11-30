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
*    functionblock                      				    *
*                                                         		    *
*    exponentiation u1 ** u2 (VDI/VDE 3696)                                 *
*                                                                           *
****************************************************************************/
/*
*   type: expt
*   ==========
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
*   Allmost equal to standardfunction EXPT in IEC 1131-3.
*/
/*
*   C-Includes
*   ----------
*/
#include <math.h>
#include <float.h>
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

OV_RESULT OV_DLLFNCEXPORT vdivde3696_expt_constructor(
	OV_INSTPTR_ov_object	pobj
) {
	OV_RESULT res;
	OV_INSTPTR_vdivde3696_expt pexpt;

	res = fb_functionblock_constructor(pobj);
	if (Ov_Fail(res)) return res;
	pexpt = Ov_StaticPtrCast(vdivde3696_expt, pobj);
	pexpt->v_en = TRUE;
	pexpt->v_u2 = 1;
	return OV_ERR_OK;
}

void OV_DLLFNCEXPORT vdivde3696_expt_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_vdivde3696_expt port = Ov_StaticPtrCast(vdivde3696_expt, pfb);

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
	  if (port->v_u1 > 0) /* positive u1 */
	  /* set enable output to true and compute u1 ** u2 */
	  {
	    port->v_v = pow(port->v_u1,port->v_u2);
	    port->v_eno = TRUE;
	  }
	  else if (port->v_u1 == 0) /* special case */
	       {
		 if (port->v_u2 > 0)
	         /* defined status -> enable ouput true */
		 {
		   port->v_v = 0;
		   port->v_eno = TRUE;
		 }
		 else if (port->v_u2 == 0)
		      /* undefined status -> enable output false */
		      {
			port->v_v = 1;
			port->v_eno = FALSE;
		      }
		      else /* U2 < 0 */ 
		      /* allthough defined status enable output is set to false ; output to infinite */
		      {
			port->v_v = FLT_MAX; /* float.h defines maximum floating point value */ 
			port->v_eno = FALSE;
		      }
	       }
	       else /* U1 < 0 */
	       /* enable output to false ; calculation with abs(u1) */	
	       {
		 port->v_v = pow(-port->v_u1,port->v_u2);
		 port->v_eno = FALSE;
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







