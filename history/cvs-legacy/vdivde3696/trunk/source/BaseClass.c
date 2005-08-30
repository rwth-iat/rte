/*
*	Copyright (C) 2004
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
*	BaseClass.c				Basisklasse
*
*	Autoren:
*	--------
*	St							Stefan Schmitz <StefanS@plt.rwth-aachen.de>
*
*	Historie:
*	--------
*	21-Dezember-2004		St
*		-	Erstellung
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
	getAccessor counter
***********************************************************************/

OV_INT OV_DLLFNCEXPORT vdivde3696_BaseClass_counter_get(
	OV_INSTPTR_vdivde3696_BaseClass	pfb
)	{
	return pfb->v_methcount;
	//	Local Method Tables
	//
	//OV_VTBLPTR_vdivde3696_BaseClass	pTable;
	
	//	get Method Table
	//
	//Ov_GetVTablePtr(vdivde3696_BaseClass, pTable, pfb);
	
	//	return specific eno
	//
	//return pTable->m_getcounter(pfb);
};

/***********************************************************************
	getcounter
***********************************************************************/

/*OV_INT OV_DLLFNCEXPORT vdivde3696_BaseClass_getcounter(
	OV_INSTPTR_vdivde3696_BaseClass	pfb
)	{
	return pfb->v_methcount;
};*/

/***********************************************************************
	getAccessor eno
***********************************************************************/

OV_BOOL OV_DLLFNCEXPORT vdivde3696_BaseClass_eno_get(
	OV_INSTPTR_vdivde3696_BaseClass	pfb
)	{
	return pfb->v_en;
	//	Local Method Tables
	//
	//OV_VTBLPTR_vdivde3696_BaseClass	pTable;
	
	//	get Method Table
	//
	//Ov_GetVTablePtr(vdivde3696_BaseClass, pTable, pfb);
	
	//	return specific eno
	//
	//return pTable->m_geteno(pfb);
};

/***********************************************************************
	geteno
***********************************************************************/

/*OV_BOOL OV_DLLFNCEXPORT vdivde3696_BaseClass_geteno(
	OV_INSTPTR_vdivde3696_BaseClass	pfb
)	{
	return pfb->v_en;
};*/
