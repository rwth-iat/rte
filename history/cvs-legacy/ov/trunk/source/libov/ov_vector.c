/*
*   $Id: ov_vector.c,v 1.2 1999-07-29 08:57:53 dirk Exp $
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
	OV_POINTER			pvector,
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
		return ov_string_setvecvalue((OV_STRING*)pvector,
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
	OV_GENERIC_VEC		*pvector,
	const OV_POINTER	pvalue,
	const OV_UINT		veclen,
	const OV_UINT		size,
	const OV_VAR_TYPE	vartype
) {
	/*
	*	local variables
	*/
	OV_POINTER	*pnewvalue;
	OV_RESULT	result;
	/*
	*	check parameters
	*/
	if(!pvector) {
		return OV_ERR_BADPARAM;
	}
	/*
	*	manipulate memory and set the actual vector value
	*/
	if(pvector->veclen && (vartype == OV_VT_STRING)) {
		/*
		*	free strings
		*/
		Ov_WarnIfNot(Ov_OK(ov_string_setvecvalue((OV_STRING*)pvector->value,
			NULL, pvector->veclen)));
	}
	if(!veclen) {
		if(pvector->value) {
			ov_database_free(pvector->value);
			pvector->value = NULL;
		}
		return OV_ERR_OK;
	}
	if(pvector->veclen != veclen) {
		/*
		*	vector length has changed, reallocate memory
		*/
		pnewvalue = ov_database_realloc(pvector->value, size);
		if(!pnewvalue) {
			return OV_ERR_DBOUTOFMEMORY;
		}
		pvector->veclen = veclen;
		pvector->value = pnewvalue;
	}
	if(vartype == OV_VT_STRING) {
		memset(pvector->value, 0, size);
		result = ov_string_setvecvalue((OV_STRING*)pvector->value,
			(OV_STRING*)pvalue, veclen);
		if(Ov_Fail(result)) {
			Ov_WarnIfNot(Ov_OK(ov_vector_setdynamicvalue(pvector, NULL, 0, 0, 0)));
			return result;
		}
	} else {
		memcpy(pvector->value, pvalue, size);
	}
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

