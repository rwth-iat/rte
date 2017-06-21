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







