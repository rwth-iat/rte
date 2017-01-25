/*
*	Copyright (C) 2015
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
*	File:
*	------
*	cshmilib.h
*
*	Editors:
*	--------
*	Je							Holger Jeromin <Holger.Jeromin@plt.rwth-aachen.de>
*
*	CVS:
*	----
*	$Revision$
*	$Date$
*
*	History:
*	--------
*	21-November-2011			Je
*		-	File created
*
************************************************************************/

#ifndef CSHMILIB_H_INCLUDED
#define CSHMILIB_H_INCLUDED

/***********************************************************************
	Includes
***********************************************************************/

#include "cshmi.h"
#include "libov/ov_macros.h"
#include "libov/ov_logfile.h"
#include "libov/ov_path.h"
#include "libov/ov_result.h"
#include "libov/ov_class.h"
#include <limits.h>

#ifndef FB_INSTANZ_CONTAINER
#define  FB_INSTANZ_CONTAINER        "TechUnits"
#endif
#ifndef FB_INSTANZ_CONTAINER_PATH
#define  FB_INSTANZ_CONTAINER_PATH   "/TechUnits"
#endif

#define CSHMI_ZINDEX_BOTTOM	0
#define CSHMI_ZINDEX_TOP		ULONG_MAX
#define CSHMI_ZINDEX_DEFAULT	CSHMI_ZINDEX_TOP

#define CSHMI_INITCLASSCACHEENTRY(classname)	\
	pDownloadApplication->v_ApplicationCache.str##classname = NULL;	\
	pDownloadApplication->v_ApplicationCache.cache##classname##Dirty = TRUE

#define CSHMI_EMPTYCLASSCACHEENTRY(classname)	\
	Ov_HeapFree(pDownloadApplication->v_ApplicationCache.str##classname);	\
	CSHMI_INITCLASSCACHEENTRY(classname)

//fall back defines for old ov servers (they are 32 bit for sure)
#ifndef OV_PRINT_INT
#define OV_PRINT_INT "i"
#endif
#ifndef OV_PRINT_UINT
#define OV_PRINT_UINT "lu"
#endif
#ifndef OV_PRINT_STRING
#define OV_PRINT_STRING "s"
#endif
#ifndef OV_PRINT_SINGLE
#define OV_PRINT_SINGLE "f"
#endif
#ifndef OV_PRINT_DOUBLE
#define OV_PRINT_DOUBLE "lf"
#endif

#endif
