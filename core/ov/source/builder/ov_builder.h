/*
*   $Id: ov_builder.h,v 1.2 2001-12-10 14:28:37 ansgar Exp $
*
*   Copyright (C) 1998
*   Lehrstuhl fuer Prozessleittechnik,
*   D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
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
/*
*	History:
*	--------
*	9-Jul-2001 Ansgar Münnemann <ansgar@plt.rwth-aachen.de>: File created.
*/

#ifndef OV_BUILDER_H_INCLUDED
#define OV_BUILDER_H_INCLUDED

#include "../codegen/ov_codegen.h"
#include <stdlib.h>

#if OV_COMPILER_BORLAND
#pragma warn -pro
#pragma warn -nod
#endif

#ifdef WIN32
#undef sparc
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
*	Backend of the framework builder
*/
int ov_builder_backend(void);

int ov_builder_createsourcefiles(
	OV_OVM_LIBRARY_DEF*	plib
);

FILE *ov_builder_createfile(
	OV_STRING			name,
	OV_STRING			extension
);

FILE *ov_builder_openreadfile(
	OV_STRING			path,
	OV_STRING			name,
	OV_STRING			extension
);

int ov_builder_closefile(
	FILE	*fp
);

/*
*	Return variable type name defined in ov/ov.h for a given vartype
*/
OV_STRING ov_builder_getvartypetext(
	OV_VAR_TYPE	vartype
);

OV_STRING ov_builder_getvartypevectext(
	OV_VAR_TYPE	vartype
);

OV_VAR_TYPE ov_codegen_getvarelementtype(
	OV_VAR_TYPE	vartype
);

OV_VAR_TYPE ov_codegen_getvarvectortype(
	OV_VAR_TYPE	vartype
);
#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif
/*
*	End of file
*/

