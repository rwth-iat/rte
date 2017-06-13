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






