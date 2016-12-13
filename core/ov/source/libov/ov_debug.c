/*
*   $Id: ov_debug.c,v 1.2 1999-09-15 10:48:21 dirk Exp $
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
*	15-Jan-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	08-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/

#define OV_COMPILE_LIBOV

#include "libov/ov_debug.h"

/*	----------------------------------------------------------------------	*/

/*
*	Return filename from a path
*/
OV_DLLFNCEXPORT OV_STRING ov_debug_filename(
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

