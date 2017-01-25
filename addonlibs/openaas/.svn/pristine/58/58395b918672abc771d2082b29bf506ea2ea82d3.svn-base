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
 *    einfacher pi-Regler (Epple)                                              *
 *                                                                           *
 ****************************************************************************/
/*
 *   type: pi
 *   ============
 */
/*
 *   history
 *   -------
 *   created : 23.04.2013
 *   author  : Ulrich Epple, Chair of Process Control Engineering, RWTH Aachen
 *
 *   modified : 17.10.2014
 *   author  : Constantin Wagner, Chair of Process Control Engineering, RWTH Aachen
 */
/*
 *   description
 *   -----------
 *   This functionblock realizes a simple PI-Control schema
 *
 *   operating states ENO:
 *   EN = 0		:	RESET		OFF-Mode
 *	EN = 1 		:	RUN		 	Running Mode
 *	EN = 2		:	TRACK   	Tracking-Mode
 *	EN = 3		:	HOLD   		Hold-Mode
 *
 *	limiting violation states LIMV:
 *	LIMV = 0	:	ok			no limitation violation
 *	LIMV = 11	:	YPMAX		Limiting YP = YMAX (YI set to 0)
 *	LIMV = 12	:	YPMIN		Limiting YP = YMIN (YI set to 0)
 *	LIMV = 21	:	YMAX		Limiting YI = YMAX-YP
 *	LIMV = 22	:	YMIN		Limiting YI = YMIN-YP
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

OV_RESULT OV_DLLFNCEXPORT ACPLTlab003lindyn_pi_constructor(OV_INSTPTR_ov_object pobj) {
	OV_RESULT res;
	OV_INSTPTR_ACPLTlab003lindyn_pi pint;

	res = fb_functionblock_constructor(pobj);
	if (Ov_Fail(res))
		return res;
	pint = Ov_StaticPtrCast(ACPLTlab003lindyn_pi, pobj);
	pint->v_YMAX = 100;
	pint->v_YMIN = -100;
	pint->v_DTM = 10;
	pint->v_KI = 1;
	pint->v_KP = 1;
	pint->v_W = 0;
	pint->v_X = 0;
	pint->v_Y = 0;
	pint->v_YI = 0;
	pint->v_YP = 0;
	pint->v_YTR = 0;
	pint->v_T = 0;
	pint->v_DT = 0;
	pint->v_EN = 1;
	pint->v_ENO = 0;
	pint->v_iexreq = 1;
	pint->v_LIMV = 0;
	pint->v_TIMV = 0;

	return OV_ERR_OK;
}

void OV_DLLFNCEXPORT ACPLTlab003lindyn_pi_typemethod(OV_INSTPTR_fb_functionblock pfb, OV_TIME *pltc) {
	//OV_INSTPTR_fb_task		ptask;
	OV_INSTPTR_ACPLTlab003lindyn_pi port = Ov_StaticPtrCast(ACPLTlab003lindyn_pi, pfb);

	/*
	 *   local variables
	 *   ---------------
	 */
	OV_DOUBLE t = 0;
	OV_SINGLE XD = 0;
	OV_DOUBLE temp = 0;
	/*
	 *   instructions
	 *   ------------
	 */
	port->v_TIMV = 0;
	port->v_LIMV = 0;
	Ov_TimeToDouble(*pltc, t);
	/* out of use, just switches on or just switched off:  */
	/* in these cases the algorithm will not be executed   */
	if (port->v_ENO == 0 || port->v_EN == 0) {
		port->v_Y = port->v_YTR;
		port->v_YI = 0;
		goto finish;
	}
	/* controler is on */
	port->v_DT = t - port->v_T;
	/* if DT is not within the allowed limits, the algorithm will not be executed */
	if (port->v_DT > port->v_DTM) {
		port->v_TIMV = 1;
		goto finish;
	}
	/* calculation of YP (P-channel-output */
	XD = port->v_W - port->v_X; /* control difference */
	port->v_YP = port->v_KP * XD; /* p-channel output          */
	/* exception in case of YMAX,YMIN violation by YP only  */
//		if (port->v_YP > port->v_YMAX)
//			{port->v_LIMV = 11 ; port->v_YI = 0; port->v_Y = port->v_YMAX;
//			goto finish;}
//		if (port->v_YP < port->v_YMIN)
//			{port->v_LIMV = 12 ; port->v_YI = 0; port->v_Y = port->v_YMIN;
//			goto finish;}
	/* calculation of YI (I-channel-output */
	/* case 3: hold mode */
	if (port->v_EN == 3) {
		port->v_YI = port->v_Y - port->v_YP;
		port->v_Y = port->v_Y;
		goto finish;
	}
	/* case 2: tracking mode */
	if (port->v_EN == 2) {
		port->v_YI = port->v_YTR - port->v_YP;
		port->v_Y = port->v_YTR;
		goto finish;
	}
	/* case 1: run mode */
	if (port->v_EN == 1) {
		temp = (XD * port->v_DT);
		port->v_YI = port->v_YI + temp;
		/* limitation in case of YP+YI limit violation */
		if (port->v_YI * port->v_KI + port->v_YP > port->v_YMAX) {
			if(port->v_YI<0 && temp>0){port->v_YI= port->v_YI + temp; }
			port->v_LIMV = 21; port->v_YI = port->v_YI - temp;
			port->v_Y = port->v_YMAX;
			goto finish;
		}
		if (port->v_YI * port->v_KI + port->v_YP < port->v_YMIN) {
			port->v_LIMV = 22;
			if(port->v_YI>0 && temp<0){port->v_YI= port->v_YI + temp; }
			port->v_YI = port->v_YI - temp;
			port->v_Y = port->v_YMIN;
			goto finish;
		}
		port->v_Y = port->v_YP + port->v_YI * port->v_KI;
	}
	finish: port->v_T = t;
	port->v_ENO = port->v_EN;
	/*******  end  *******/
	return;
}

