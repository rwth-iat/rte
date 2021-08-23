/*
*   $Id: ov_part.h,v 1.3 2002-01-23 13:44:14 ansgar Exp $
*
*   Copyright (C) 1998
*   Lehrstuhl fuer Prozessleittechnik,
*   D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
*
*   This file is part of the ACPLT/OV Package 
*   
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
/*
*	History:
*	--------
*	09-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

#ifndef OV_PART_H_INCLUDED1
#define OV_PART_H_INCLUDED1

#include "ov_ov.h"
#include "ov_debug.h"

/*
*	OV_PART_DEF:
*	------------
*	Part definition, included in the library's code
*/
struct OV_PART_DEF {
	struct OV_PART_DEF	*pnext;
	OV_STRING			identifier;
	OV_STRING			partclassname;
	OV_UINT				flags;
	OV_UINT				offset;
};
typedef struct OV_PART_DEF OV_PART_DEF;

#endif

#include "ov.h"

#ifdef OV_INSTBODY_ov_object

#ifndef OV_PART_H_INCLUDED2
#define OV_PART_H_INCLUDED2

#ifdef __cplusplus
extern "C" {
#endif

/*
*	Load a part into the database
*/
OV_RESULT ov_part_load(
	OV_PART_DEF				*ppartdef,
	OV_INSTPTR_ov_domain	pparent
);

/*
*	Compare a part with its definition
*/
OV_RESULT ov_part_compare(
	OV_INSTPTR_ov_part		ppart,
	OV_PART_DEF				*ppartdef
);

/*
*	Test if we can unload a part from the database
*/
OV_BOOL ov_part_canunload(
	OV_INSTPTR_ov_part	ppart
);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif

#endif
/*
*	End of file
*/

