/*
*	Copyright (C) 2005
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
