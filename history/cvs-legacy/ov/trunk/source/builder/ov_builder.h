/*
*   $Id: ov_builder.h,v 1.1 2001-07-09 12:52:02 ansgar Exp $
*
*   Copyright (C) 1998-1999
*   Lehrstuhl fuer Prozessleittechnik,
*   RWTH Aachen, D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
*
*   This file is part of the ACPLT/OV Package which is licensed as open
*   source under the Artistic License; you can use, redistribute and/or
*   modify it under the terms of that license.
*
*   You should have received a copy of the Artistic License along with
*   this Package; see the file ARTISTIC-LICENSE. If not, write to the
*   Copyright Holder.
*
*   THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
*   WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
*   OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
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

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif
/*
*	End of file
*/

