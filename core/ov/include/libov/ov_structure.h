/*
*   $Id: ov_structure.h,v 1.1 1999-07-19 15:02:04 dirk Exp $
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
*	09-Mar-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

#ifndef OV_STRUCTURE_H_INCLUDED1
#define OV_STRUCTURE_H_INCLUDED1

#include "libov/ov_ov.h"

/*
*	OV_STRUCTURE_DEF:
*	-----------------
*	Structure definition, included in the library's code
*/
struct OV_STRUCTURE_DEF {
	struct OV_STRUCTURE_DEF	*pnext;
	OV_STRING				identifier;
	OV_UINT					size;
	struct OV_VARIABLE_DEF	*members;
};
typedef struct OV_STRUCTURE_DEF OV_STRUCTURE_DEF;

#endif

#include "ov.h"

#ifdef OV_INSTBODY_ov_object

#ifndef OV_STRUCTURE_H_INCLUDED2
#define OV_STRUCTURE_H_INCLUDED2

#ifdef __cplusplus
extern "C" {
#endif

/*
*	Search for a structure object with given identifier
*/
OV_INSTPTR_ov_structure ov_structure_search(
	OV_STRING	identifier
);

/*
*	Load a structure into the database
*/
OV_RESULT ov_structure_load(
	OV_STRUCTURE_DEF		*pstructdef,
	OV_INSTPTR_ov_domain	pparent
);

/*
*	Compare a structure with its definition
*/
OV_RESULT ov_structure_compare(
	OV_INSTPTR_ov_structure	pstruct,
	OV_STRUCTURE_DEF		*pstructdef
);

/*
*	Test if we can unload a structure from the database
*/
OV_BOOL ov_structure_canunload(
	OV_INSTPTR_ov_structure	pstruct
);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif

#endif
/*
*	End of file
*/

