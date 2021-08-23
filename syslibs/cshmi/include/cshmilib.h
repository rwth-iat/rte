/*
*   Copyright (C) 2015
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
#include "ov_macros.h"
#include "ov_logfile.h"
#include "ov_path.h"
#include "ov_result.h"
#include "ov_class.h"
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
