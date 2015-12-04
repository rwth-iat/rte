/*
*	Copyright (C) 2013
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
*    PT1-block (Epple)                                              *
*                                                                           *
****************************************************************************/
/*
*   type: pt1
*   ============
*/
/*
*   history
*   -------
*   created : 01.05.2013
*   author  : Ulrich Epple, Chair of Process Control Engineering, RWTH Aachen
*/
/*
*   description
*   -----------
*   This functionblock realizes a simple PI-Control schema
*
*   operating states ENO:
*   EN = 0		:	RESET		OFF-Mode
*	EN = 1 		:	RUN		 	Running Mode
*
*   timing violation states TIMV:
*   TIMV = 0 	:   no error
*   TIMV = 1	:   DT > DTM time step to long
*   
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

OV_RESULT OV_DLLFNCEXPORT ACPLTlab003lindyn_pt1_constructor(
	OV_INSTPTR_ov_object	pobj
) {
	OV_RESULT res;
	OV_INSTPTR_ACPLTlab003lindyn_pt1 pint;

	res = fb_functionblock_constructor(pobj);
	if (Ov_Fail(res)) return res;
	pint = Ov_StaticPtrCast(ACPLTlab003lindyn_pt1, pobj);
	pint->v_DTM = 10;
	pint->v_KP = 1;
	pint->v_T1 = 1;
	pint->v_XIN = 0;
	pint->v_XOUT = 0;
	pint->v_X0 = 0;
	pint->v_T = 0;
	pint->v_DT = 0;
	pint->v_EN = 1;
	pint->v_ENO = 0;
	pint->v_iexreq = 1;
	pint->v_TIMV = 0;

	return OV_ERR_OK;
}

void OV_DLLFNCEXPORT ACPLTlab003lindyn_pt1_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	//OV_INSTPTR_fb_task		ptask;
	OV_INSTPTR_ACPLTlab003lindyn_pt1 port = Ov_StaticPtrCast(ACPLTlab003lindyn_pt1, pfb);

	/*
	*   local variables
	*   ---------------
	*/
	OV_DOUBLE	t	=	0	;
	OV_SINGLE   tT1 =   0	;
	OV_SINGLE	KpW =	0	;
	/*
	*   instructions
	*   ------------
	*/
	port->v_TIMV = 0;
	Ov_TimeToDouble(*pltc, t);
		/* out of use, just switches on or just switched off:  */
		/* in these cases the algorithm will not be executed   */
		if (port->v_ENO == 0 || port->v_EN == 0)
		{port->v_XOUT = port->v_X0; goto finish;}
	/* PT1 is on */
	port->v_DT = t - port->v_T ;
		/* if DT is not within the allowed limits, the algorithm will not be executed */
		if	(port->v_DT > port->v_DTM) {port->v_TIMV = 1; goto finish;}
	/* calculation of XOUT */
	tT1 = port->v_DT / port->v_T1;
	KpW = port->v_KP * port->v_XIN;
	/* port->v_XOUT = port->v_XOUT * (1-tT1) + (KpW * tT1) ; 	 explizit */
	port->v_XOUT = (port->v_XOUT +  KpW*tT1 )/(1 + tT1); 	/* implizit */

	finish:
	port->v_T = t;
	port->v_ENO = port->v_EN;
/*******  end  *******/
	return;
	}






