/*
*	Copyright (C) 2007
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
*	   notice, this list of conditions and the following disclaimer in
*		the documentation and/or other materials provided with the
*		distribution.
*	4. Neither the name of the Chair of Process Control Engineering nor
*		the name of the Aachen University of Technology may be used to
*		endorse or promote products derived from this software without
*		specific prior written permission.
*
*	THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
*	``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE CHAIR OF
*	PROCESS CONTROL ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT,
*	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
*	OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
*	AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
*	WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*	POSSIBILITY OF SUCH DAMAGE.
*/
/***********************************************************************
*
*	File
*	-----
*	xor.c						boolean exclusive OR
*
*	Autoren:
*	--------
*	MK							Michal Klamann
*	St							Stefan Schmitz <StefanS@plt.rwth-aachen.de>
*
*	Historie:
*	--------
*	27-Mai-1999				MK
*		-	Erstellung
*	
*	23-September-2004		St
*		-	Logik gemäß IEC 1131-3
*
***********************************************************************/

#ifndef OV_COMPILE_LIBRARY_vdivde3696
#define OV_COMPILE_LIBRARY_vdivde3696
#endif

/***********************************************************************
	Includes
***********************************************************************/

#include "vdivde3696.h"
#include "libov/ov_macros.h"

/***********************************************************************
	Typemethod
***********************************************************************/

void OV_DLLFNCEXPORT vdivde3696_xor_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME								*pltc
)	{
	//	Local pointer
	//
	OV_INSTPTR_vdivde3696_xor
		port = Ov_StaticPtrCast(vdivde3696_xor, pfb);
		
	//	Local Vairables
	//
	int count = 0;
	
	//	Logic
	//
	if (port->v_en == TRUE)
	{
		//	check all eight inputs for TRUE status
		//
		if (port->v_i1 == TRUE) count++;
		if (port->v_i2 == TRUE) count++;
		if (port->v_i3 == TRUE) count++;
		if (port->v_i4 == TRUE) count++;
		if (port->v_i5 == TRUE) count++;
		if (port->v_i6 == TRUE) count++;
		if (port->v_i7 == TRUE) count++;
		if (port->v_i8 == TRUE) count++;
		
		//	even number of TRUE
		//
		if (count == 0 || count == 2 || count == 4 || count == 6 || count == 8)
		{
			port->v_q = FALSE;
	  	} else {
	    port->v_q = TRUE;
	   };
	};
	
	return;
};
