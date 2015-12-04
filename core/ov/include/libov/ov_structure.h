/*
*   $Id: ov_structure.h,v 1.1 1999-07-19 15:02:04 dirk Exp $
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

