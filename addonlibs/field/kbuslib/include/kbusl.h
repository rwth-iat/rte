/*
*   Copyright (C) 2005
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
*	kbusl.h
*
*	Autoren:
*	--------
*	LE						Lars Evertz		<l.evertz@plt.rwth-aachen.de>
*
*	CVS:
*	----
*	$Revision: 
*
*	Historie:
*	--------
*	07th-February-2011	LE
*		-	file created
*
***********************************************************************/

#ifndef KBUSL_H_INCLUDED
#define KBUSL_H_INCLUDED

/***********************************************************************
	Includes
***********************************************************************/

#include "libov/ov_macros.h"

enum kbuslib_MBState{
	MB_idle,
	MB_startSend,
	MB_sending,
	MB_stopSend,
	MB_startRead,
	MB_startRead2,
	MB_reading,
	MB_stopRead,
	MB_finished,
	MB_errorState
};


enum
{
	KBUS_ERROR_NOERROR = 0,
	KBUS_ERROR_MANAGER = -10,
	KBUS_ERROR_NO_MANAGER = -20,
	KBUS_ERROR_CRMANAGER = -30,
	KBUS_ERROR_IDENTIFIER_EXISTS = -35,
	KBUS_ERROR_INIT = -40,
	KBUS_ERROR_AUTODETECT = -50,
	KBUS_ERROR_NOCLAMP = -60,
	KBUS_CLAMP_DISABLED = -70,
	KBUS_ERROR_OUT_OF_RANGE = -80,
	KBUS_ERROR_OUTPUT_EXISTS = -90,
	KBUS_ERROR_PARAM_CHANGE_NOT_ALLOWED = -100,
	KBUS_ERROR_BAD_PARAM = -110,
	KBUS_ERROR_FAULT = -14,
	KBUS_ERROR_BUSY = -16,
	KBUS_ERROR_NODEV = -19,
	KBUS_ERROR_IO = -5,
	KBUS_ERROR_TIME = -62,
	KBUS_ERROR_PERM = -1
	
};

#define KBUS_ERRORSTR_NOERROR "<no error>"
#define KBUS_ERRORSTR_MANAGER "more than one Manager found"
#define KBUS_ERRORSTR_NO_MANAGER "No Manager found"
#define KBUS_ERRORSTR_CRMANAGER "Manager already exists"
#define KBUS_ERRORSTR_IDENTIFIER_EXISTS "this identifier already exists inside the managers containment"
#define KBUS_ERRORSTR_INIT "kbus not initializable"
#define KBUS_ERRORSTR_AUTODETECT "autodetection of clamps failed"
#define KBUS_ERRORSTR_NOCLAMP "this clamp does not exist"
#define KBUS_STR_CLAMP_DISABLED "clamp disabled"
#define KBUS_ERRORSTR_OUT_OF_RANGE "value out of given range (UpperLimit / LowerLimit)"
#define KBUS_ERRORSTR_OUTPUT_EXISTS "space in output image is already in use"
#define KBUS_ERRORSTR_PARAM_CHANGE_NOT_ALLOWED "parameter cannot be changed for this type of clamp"
#define KBUS_ERRORSTR_BAD_PARAM "bad parameter value"
#define KBUS_ERRORSTR_FAULT "address error"
#define KBUS_ERRORSTR_BUSY "kbus busy"
#define KBUS_ERRORSTR_NODEV "no device found or DPram not intitialized"
#define KBUS_ERRORSTR_IO "kbus answer failed"
#define KBUS_ERRORSTR_TIME "communication timed out"
#define KBUS_ERRORSTR_PERM "operation not permitted"
#define MISC_ERRORSTR_INIT "misc functionality not initializable"

#endif
