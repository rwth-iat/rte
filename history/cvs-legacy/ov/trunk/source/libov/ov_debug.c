/*
*   $Id: ov_debug.c,v 1.1 1999-07-19 15:02:12 dirk Exp $
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
*	15-Jan-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	08-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/

#define OV_COMPILE_LIBOV

#include "libov/ov_debug.h"

/*	----------------------------------------------------------------------	*/

/*
*	Return filename from a path
*/
OV_STRING OV_DLLFNCEXPORT ov_debug_filename(
	OV_STRING	path
) {
	/*
	*	local variables
	*/
	OV_STRING	filename;
	/*
	*	find out filename
	*/
	if(path) {
		for(filename=path+strlen(path); filename>=path; filename--) {
			if((*filename == '/') || (*filename == '\\')) {
				return filename+1;
			}
		}
	}
	return NULL;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

