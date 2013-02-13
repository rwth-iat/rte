/*
*   $Id: ov_ksserver_exgdata.c,v 1.1.1.1 2011-01-31 07:47:43 henning Exp $
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
*	23-Jul-1998 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	21-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/

#include "ov_ksserver_backend.h"

#include "libov/ov_path.h"
#include "libov/ov_macros.h"

/*	----------------------------------------------------------------------	*/

/*
*	Execute the DataExchange service (subroutine)
*/
OV_DLLFNCEXPORT void ov_ksserver_exgdata(
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

