/*
*   Copyright (C) 2007
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
