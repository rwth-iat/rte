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
*	ksapi.h
*
*	Editors:
*	--------
*	MS						Markus Schluetter <Markus.Schluetter@plt.rwth-aachen.de>
*	AS						Andreas Schueller	<Andreas.Schueller@plt.rwth-aachen.de>
*
*	CVS:
*	----
*	$Revision: 1.1.1.1 $
*	$Date: 2011-01-31 07:47:43 $
*
*	History:
*	--------
*	07-December-2007	MS		V0.1.0
*		-	File created
*	06-March-2008		AS		V0.2.0
*		-	seperated sending object from functionblock
*	19-May-2008			AS		V0.3.0
*		-	different libraries (sendapi, sendlib) for user interface and api
*	05-Sep-2008			AS		V0.3.1
*		-	cleaning and commenting the code
*	17-Sep-2008			AS		V0.4.0
*		-	renaming to ksapi, seperate KSCommon from SetString
*
***********************************************************************/

#ifndef KSAPI_H_INCLUDED
#define KSAPI_H_INCLUDED

/***********************************************************************
	Includes
***********************************************************************/

#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include "libov/ov_memstack.h"
#include "libov/ov_result.h"
#include "libov/ov_logfile.h"


/***********************************************************************
	LOGFILE - Defines
***********************************************************************/
#define log_info(log_msg) ov_logfile_info("%s %d: %s", __FILE__, __LINE__, log_msg)
#define log_debug(log_msg) ov_logfile_debug("%s %d: %s", __FILE__, __LINE__, log_msg)
#define log_warning(log_msg) ov_logfile_warning("%s %d: %s", __FILE__, __LINE__, log_msg)
#define log_error(log_msg) ov_logfile_error("%s %d: %s", __FILE__, __LINE__, log_msg)


/****************** DEFINITIONEN ZUSTANDSMACHINEN *******************/


#endif
