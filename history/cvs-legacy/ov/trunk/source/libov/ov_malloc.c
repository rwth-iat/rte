/*
*   $Id: ov_malloc.c,v 1.1 1999-07-19 15:02:13 dirk Exp $
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
*	13-Jan-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	13-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/
/*
*	Description:
*	------------
*	The routines included in this file are a wrapper for the standard
*	malloc(), realloc() and free() functions, which may cause problems
*	with DLLs/shared libraries, if they have an own allocator.
*	Usage: free any heap memory allocated in function of LIBOV using the
*	function ov_free() and (re)allocate any heap memory freed later in
*	LIBOV using ov_malloc() or ov_realloc().
*/

#define OV_COMPILE_LIBOV

#include "libov/ov_malloc.h"

#if OV_SYSTEM_MC164
#define malloc	xmalloc
#define free	xfree
#define realloc	xrealloc
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Allocate memory on the heap
*/
OV_POINTER OV_DLLFNCEXPORT ov_malloc(
	OV_UINT		size
) {
	return malloc(size);
}

/*	----------------------------------------------------------------------	*/

/*
*	Free memory allocated in the heap
*/
void OV_DLLFNCEXPORT ov_free(
	OV_POINTER	ptr
) {
	free(ptr);
}

/*	----------------------------------------------------------------------	*/

/*
*	Reallocate memory on the heap
*/
OV_POINTER OV_DLLFNCEXPORT ov_realloc(
	OV_POINTER	ptr,
	OV_UINT		size
) {
	return realloc(ptr, size);
}

/*	----------------------------------------------------------------------	*/

/*
*	Duplicate a string on the heap using malloc
*/
OV_STRING ov_strdup(
	OV_STRING	string
) {
	/*
	*	local variables
	*/
    OV_STRING result;
	result = (OV_STRING)malloc(strlen(string)+1);
    if(!result) {
        return NULL;
    }
    strcpy(result, string);
    return result;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

