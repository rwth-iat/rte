/*
*   $Id: ov_vector.c,v 1.1 1999-07-28 15:59:57 dirk Exp $
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
*	20-Jan-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	13-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/

#define OV_COMPILE_LIBOV

#include "libov/ov_vector.h"
#include "libov/ov_database.h"
#include "libov/ov_string.h"
#include "libov/ov_macros.h"

#if OV_SYSTEM_MC164
#define memcpy xmemcpy
#define memset xmemset
#endif

/*	----------------------------------------------------------------------	*/

OV_RESULT OV_DLLFNCEXPORT ov_vector_setstaticvalue(
	OV_STATIC_VECTOR	*pvector,
	const OV_POINTER	pvalue,
	const OV_UINT		veclen,
	const OV_UINT		size,
	const OV_VAR_TYPE	vartype
) {
	/*
	*	check parameters
	*/
	if(!pvector || !pvalue) {
		return OV_ERR_BADPARAM;
	}
	/*
	*	set the value
	*/
	if(vartype == OV_VT_STRING) {
		return ov_string_setvecvalue(pvector->valueunion.string_vec,
			(OV_STRING*)pvalue, veclen);
	}
	memcpy(pvector, pvalue, size);
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Set the value of a dynamic vector variable
*/
OV_RESULT OV_DLLFNCEXPORT ov_vector_setdynamicvalue(
	OV_DYNAMIC_VECTOR	**pvector,
	const OV_POINTER	pvalue,
	const OV_UINT		veclen,
	const OV_UINT		offset,
	const OV_UINT		size,
	const OV_VAR_TYPE	vartype
) {
	/*
	*	local variables
	*/
	OV_DYNAMIC_VECTOR	*pnewvector;
	OV_RESULT			result;
	OV_UINT				oldveclen;
	/*
	*	check parameters
	*/
	if(!pvector) {
		return OV_ERR_BADPARAM;
	}
	/*
	*	manipulate memory and set the actual vector value
	*/
	oldveclen = (*pvector)?((*pvector)->valueunion.veclen):(0);
	if(oldveclen && (vartype == OV_VT_STRING)) {
		/*
		*	free strings
		*/
		Ov_WarnIfNot(Ov_OK(ov_string_setvecvalue(
			(*pvector)->valueunion.string_vec.value, NULL, oldveclen)));
	}
	if(!veclen) {
		if(*pvector) {
			ov_database_free(*pvector);
			*pvector = NULL;
		}
	} else {
		if(oldveclen != veclen) {
			/*
			*	vector length has changed, reallocate memory
			*/
			pnewvector = (OV_DYNAMIC_VECTOR*)ov_database_realloc(*pvector,
				offset+size);
			if(!pnewvector) {
				return OV_ERR_DBOUTOFMEMORY;
			}
			pnewvector->valueunion.veclen = veclen;
			*pvector = pnewvector;
		}
		if(vartype == OV_VT_STRING) {
			memset(((OV_BYTE*)*pvector)+offset, 0, size);
			result = ov_string_setvecvalue((*pvector)->valueunion.string_vec.value,
				(OV_STRING*)pvalue, veclen);
			if(Ov_Fail(result)) {
				Ov_WarnIfNot(Ov_OK(ov_vector_setdynamicvalue(pvector, NULL, 0,	0, 0, 0)));
				return result;
			}
		} else {
			memcpy(((OV_BYTE*)*pvector)+offset, pvalue, size);
		}
	}
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

