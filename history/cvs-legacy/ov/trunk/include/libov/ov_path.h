/*
*   $Id: ov_path.h,v 1.3 1999-09-15 10:48:15 dirk Exp $
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
*	22-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

#ifndef OV_PATH_H_INCLUDED
#define OV_PATH_H_INCLUDED

#include "ov.h"
#include "libov/ov_element.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
*	OV_PATH:
*	--------
*	A path is an array of elements corresponding to the identifiers
*	of a path name.
*/
typedef struct {
	OV_UINT		size;		/* number of path elements */
	OV_ELEMENT	*elements;	/* pointer to array of elements */
}	OV_PATH;

/*
*	Resolve a path using a given path name
*	Note: the memory for the path elements is allocated on the memory
*	stack, use ov_memstack_lock()/unlock() outside of this function
*/
OV_DLLFNCEXPORT OV_RESULT ov_path_resolve(
	OV_PATH			*ppath,
	const OV_PATH	*prelpath,
	const OV_STRING	pathname,
	const OV_UINT	version
);

/*
*	Get the canonical path of an element
*	Note: the memory for the path name is allocated on the memory
*	stack, use ov_memstack_lock()/unlock() outside of this function
*/
OV_DLLFNCEXPORT OV_STRING ov_path_getcanonicalpath(
	OV_INSTPTR_ov_object	pobj,
	const OV_UINT			version
);

/*
*	Get the pointer to an object with given path name
*	Note: you need NOT call ov_memstack_lock()/unlock() outside of this function
*/
OV_DLLFNCEXPORT OV_INSTPTR_ov_object ov_path_getobjectpointer(
	const OV_STRING			pathname,
	const OV_UINT			version
);
	
#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif
/*
*	End of file
*/

