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
*    functionblock                       				    *
*                                                       		    *
*    integrator (Epple)                                              *
*                                                                           *
****************************************************************************/
/*
*   type: int
*   ============
*/
/*
*   history
*   -------
*   created : 23.04.2013
*   author  : Ulrich Epple, Chair of Process Control Engineering, RWTH Aachen
*    modified : 17.10.2014
*   author  : Constantin Wagner, Chair of Process Control Engineering, RWTH Aachen
*/
/*
*   description
*   -----------
*   This functionblock realizes a step by step integration
*   
*   operating states:
*   EN = 0:	TRACK:   	X=XTR          no integration
*	EN = 1:	RUN: 		X(T) = X(T-DT) + KI*DT*DX
*			ERROR 1:	DT < DTM:		X=X(Talt)
*			ERROR 2:	X > XM	:		X=XMAX
*			ERROR 3:	X < XL	:		X=XMIN
*/
/*
*   C-Includes
*   ----------
*/
/*
*   Module-declaration
*   -----------------
*/
#ifndef OV_COMPILE_LIBRARY_ACPLTlab003lindyn
#define OV_COMPILE_LIBRARY_ACPLTlab003lindyn
#endif

#include "ACPLTlab003lindyn.h"
#include "libov/ov_macros.h"

OV_RESULT OV_DLLFNCEXPORT ACPLTlab003lindyn_int_constructor(
	OV_INSTPTR_ov_object	pobj
) {
	OV_RESULT res;
	OV_INSTPTR_ACPLTlab003lindyn_int pint;

	res = fb_functionblock_constructor(pobj);
	if (Ov_Fail(res)) return res;
	pint = Ov_StaticPtrCast(ACPLTlab003lindyn_int, pobj);
	pint->v_XM = 1e19;
	pint->v_XL = -1e19;
	pint->v_DTM = 10;
	pint->v_KI = 1;
	pint->v_XTR = 0;
	pint->v_X  = 0;
	pint->v_DX = 0;
	pint->v_T = 0;
	pint->v_DT = 0;
	pint->v_EN = 1;
	pint->v_ENO = 0;
	pint->v_iexreq = 1;
	return OV_ERR_OK;
}

void OV_DLLFNCEXPORT ACPLTlab003lindyn_int_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	//OV_INSTPTR_fb_task		ptask;
	OV_INSTPTR_ACPLTlab003lindyn_int port = Ov_StaticPtrCast(ACPLTlab003lindyn_int, pfb);

	/*
	*   local variables
	*   ---------------
	*/
	OV_DOUBLE t=0;
	/*
	*   instructions
	*   ------------
	*/
	Ov_TimeToDouble(*pltc, t);
	if (port->v_ENO == 1 && port->v_EN == 1)
		{
		port->v_DT = t - port->v_T ;
		if	(port->v_DT > port->v_DTM) {port->v_ERROR = 3;}
		else
			{port->v_X = port->v_X + port->v_KI * port->v_DT * port->v_DX;
	        if (port->v_X > port->v_XM) {port->v_X = port->v_XM; port->v_ERROR = 1;};
	        if (port->v_X < port->v_XL) {port->v_X = port->v_XL; port->v_ERROR = 2;};
	        }
	      }
	port->v_T = t;
	port->v_ENO = port->v_EN;
	if (port->v_EN == 0){port->v_X = port->v_XTR; port->v_ERROR = 0;};

/*******  end  *******/
	return;
	}






