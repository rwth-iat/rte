/*
*   $Id: ov_ksserver_getcanonicalpath.c,v 1.1 1999-07-19 15:02:16 dirk Exp $
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
*	23-Jun-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

#define OV_COMPILE_LIBOVKS

#include "libovks/ov_ksserver.h"
#include "libov/ov_path.h"
#include "libov/ov_string.h"
#include "libov/ov_time.h"
#include "libov/ov_macros.h"

/*	----------------------------------------------------------------------	*/

/*
*	Execute the GetCanonicalPath service (subroutine)
*/
void ov_ksserver_getcanonicalpath(
	const OV_UINT					version,
	const OV_TICKET					*pticket,
	const OV_GETCANONICALPATH_PAR	*params,
	OV_GETCANONICALPATH_RES			*result
) {
	/*
	*	local variables
	*/
	OV_UINT						len = params->paths_len;
	OV_GETCANONICALPATHITEM_RES	*pitem;
	OV_STRING					*ppath = params->paths_val;
	OV_PATH						lastpath, path;
	OV_UINT						i, j, k;
	OV_STRING					canonicalpath;
	OV_UINT						size;
	/*
	*	initialization
	*/
	result->result = OV_ERR_OK;
	result->results_len = 0;
	result->results_val = NULL;
	/*
	*	if there are no items, we are done
	*/
	if(!len) {
		return;
	}
	/*
	*	allocate memory for the result items
	*/
	pitem = (OV_GETCANONICALPATHITEM_RES*)ov_memstack_alloc(len
		*sizeof(OV_GETCANONICALPATHITEM_RES));
	if(!pitem) {
		result->result = OV_ERR_TARGETGENERIC;
		return;
	}
	result->results_val = pitem;
	result->results_len = len;
	/*
	*	iterate over the paths given
	*/
	for(i=0; i<len; i++, ppath++, pitem++) {
		/*
		*	resolve path to path item, the first time the path is absolute
		*/
		if(i) {
			pitem->result = ov_path_resolve(&path, &lastpath, *ppath, version);
		} else {
			pitem->result = ov_path_resolve(&path, NULL, *ppath, version);
		}
		if(Ov_OK(pitem->result)) {
			/*
			*	path is ok, get cononical path item
			*/
			j=path.size-1;
			while(TRUE) {
				if(path.elements[j].elemtype == OV_ET_OBJECT) {
					pitem->canonical_path = ov_path_getcanonicalpath(
						path.elements[path.size-1].pobj, version);
					if(pitem->canonical_path) {
						if(j+1 < path.size) {
							/*
							*	TODO! here we could optimize by dividing the given
							*	path into two parts: part one is the path of the
							*	last object and part two is the path of the
							*	part of the object. We only need to exchange the
							*	1st part with its canonical path.
							*/
							canonicalpath = pitem->canonical_path;
							size = strlen(canonicalpath)+1;
							for(k=j+1; k<path.size; k++) {
								size += strlen(ov_element_getidentifier(&path.elements[k]))+1;
							}
							pitem->canonical_path = (OV_STRING)ov_memstack_alloc(size);
							if(pitem->canonical_path) {
								strcpy(pitem->canonical_path, canonicalpath);
								canonicalpath = pitem->canonical_path+strlen(canonicalpath);
								for(k=j+1; k<path.size; k++) {
									*canonicalpath = '.';
									canonicalpath++;
									strcpy(canonicalpath, ov_element_getidentifier(&path.elements[k]));
									canonicalpath += strlen(ov_element_getidentifier(&path.elements[k]));
								}
							} else {
								pitem->result = OV_ERR_TARGETGENERIC;
							}
						}
					} else {
						pitem->result = OV_ERR_TARGETGENERIC;
					}
					break;
				}
				/*
				*	j should never be zero because we will at least
				*	reach the root domain
				*/
				Ov_WarnIf(!j);
				if(!j) {
					pitem->result = OV_ERR_TARGETGENERIC;
					break;
				}
				j--;
			}
		} else {
			/*
			*	path is bad, skip all following relative paths
			*/
			while(i+1<len) {
				if(**(ppath+1) == '/') {
					break;
				}
				i++;
				ppath++;
				pitem++;
				pitem->result = OV_ERR_BADPATH;
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

