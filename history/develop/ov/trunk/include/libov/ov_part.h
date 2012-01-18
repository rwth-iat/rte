/*
*   $Id: ov_part.h,v 1.3 2002-01-23 13:44:14 ansgar Exp $
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
*	09-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

#ifndef OV_PART_H_INCLUDED1
#define OV_PART_H_INCLUDED1

#include "libov/ov_ov.h"

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

