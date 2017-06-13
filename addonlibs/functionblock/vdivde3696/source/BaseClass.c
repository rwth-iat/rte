/*
*   Copyright (C) 2004
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
