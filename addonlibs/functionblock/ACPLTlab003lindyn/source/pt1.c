/*
*   Copyright (C) 2013
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






