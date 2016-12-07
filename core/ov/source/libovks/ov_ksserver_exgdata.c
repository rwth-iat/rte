/*
*   $Id: ov_ksserver_exgdata.c,v 1.2 2000-04-13 09:13:14 dirk Exp $
*
*   Copyright (C) 1998
*   Lehrstuhl fuer Prozessleittechnik,
*   RWTH Aachen, D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
*
*   This file is part of the ACPLT/OV 
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
*	23-Jul-1998 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	21-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/

#define OV_COMPILE_LIBOVKS

#include "libovks/ov_ksserver.h"
#include "libov/ov_path.h"
#include "libov/ov_macros.h"

/*	----------------------------------------------------------------------	*/

/*
*	Execute the DataExchange service (subroutine)
*/
void ov_ksserver_exgdata(
	const OV_UINT			version,
	const OV_TICKET			*pticket,
	const OV_EXGDATA_PAR	*params,
	OV_EXGDATA_RES			*result
) {
	/*
	*	local variables
	*/
	OV_UINT			setvarlen = params->set_vars_len;
	OV_SETVAR_ITEM	*psetvaritem = params->set_vars_val;
	OV_RESULT		*psetvarresult;
	OV_UINT			getvarlen = params->get_vars_len;
	OV_GETVAR_ITEM	*pgetvaritem;
	OV_STRING		*pgetvarid = params->get_vars_val;
	OV_PATH			lastpath, path;
	OV_UINT			i;
	/*
	*	initialization
	*/
	result->result = OV_ERR_OK;
	result->results_len = 0;
	result->results_val = NULL;
	result->items_len = 0;
	result->items_val = NULL;
	/*
	*	allocate memory for the result items
	*/
	if(setvarlen) {
		/*
		*   test (grand) access
		*/
		if(!(pticket->vtbl->getaccess(pticket) & OV_AC_WRITE) ) {
			result->result = OV_ERR_NOACCESS;
			return;
		}
		psetvarresult = (OV_RESULT*)ov_memstack_alloc(setvarlen*sizeof(OV_RESULT));
		if(!psetvarresult) {
			result->result = OV_ERR_TARGETGENERIC;	/* TODO! no heap memory */
			return;
		}
	} else {
		psetvarresult = NULL;
	}
	if(getvarlen) {
		/*
		*   test (grand) access
		*/
		if(!(pticket->vtbl->getaccess(pticket) & OV_AC_READ) ) {
			result->result = OV_ERR_NOACCESS;
			return;
		}
		pgetvaritem = (OV_GETVAR_ITEM*)ov_memstack_alloc(getvarlen*sizeof(OV_GETVAR_ITEM));
		if(!pgetvaritem) {
			result->result = OV_ERR_TARGETGENERIC;	/* TODO! no heap memory */
			return;
		}
	} else {
		pgetvaritem = NULL;
	}
	result->results_val = psetvarresult;
	result->results_len = setvarlen;
	result->items_val = pgetvaritem;
	result->items_len = getvarlen;
	/*
	*	iterate over the setvar items given
	*/
	for(i=0; i<setvarlen; i++, psetvaritem++, psetvarresult++) {
		/*
		*	resolve path to variable item, the first time the path is absolute
		*/
		if(i) {
			*psetvarresult = ov_path_resolve(&path, &lastpath,
				psetvaritem->path_and_name, version);
		} else {
			*psetvarresult = ov_path_resolve(&path, NULL,
				psetvaritem->path_and_name, version);
		}
		if(Ov_OK(*psetvarresult)) {
			/*
			*	path is ok, set variable item
			*/
			*psetvarresult = ov_ksserver_setvar_setitem(pticket, psetvaritem,
				&path.elements[path.size-1], FALSE);
		} else {
			/*
			*	path is bad, skip all following relative paths
			*/
			while(i+1<setvarlen) {
				if(*((psetvaritem+1)->path_and_name) == '/') {
					break;
				}
				i++;
				psetvaritem++;
				psetvarresult++;
				*psetvarresult = OV_ERR_BADPATH;
			}
		}
		lastpath = path;
	}	/* for */
	/*
	*	iterate over the getvar identifiers given
	*/
	for(i=0; i<getvarlen; i++, pgetvarid++, pgetvaritem++) {
		/*
		*	resolve path to variable item, the first time the path is absolute
		*/
		/*
		*	FIXME! & TODO! first getvar path absolute again or relative to last setvar path???
		*/
		if(i) {
			pgetvaritem->result = ov_path_resolve(&path, &lastpath, *pgetvarid, version);
		} else {
			pgetvaritem->result = ov_path_resolve(&path, NULL, *pgetvarid, version);
		}
		if(Ov_OK(pgetvaritem->result)) {
			/*
			*	path is ok, get variable item
			*/
			ov_ksserver_getvar_getitem(version, pticket, pgetvaritem,
				&path.elements[path.size-1]);
		} else {
			/*
			*	path is bad, skip all following relative paths
			*/
			while(i+1<getvarlen) {
				if(**(pgetvarid+1) == '/') {
					break;
				}
				i++;
				pgetvarid++;
				pgetvaritem++;
				pgetvaritem->result = OV_ERR_BADPATH;
			}
		}
		lastpath = path;
	}	/* for */
	/*
	*	we are finished.
	*/
	return;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

