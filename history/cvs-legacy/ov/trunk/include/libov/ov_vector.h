/*
*   $Id: ov_vector.h,v 1.1 1999-07-28 15:59:56 dirk Exp $
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
*	28-Jul-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

#ifndef OV_VECTOR_H_INCLUDED
#define OV_VECTOR_H_INCLUDED

#include "libov/ov_ov.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
*	OV_STATIC_VECTOR:
*	-----------------
*	Generic dynamic vector value
*/
typedef struct {
	union {
		OV_BOOL				bool_vec[1];
		OV_INT				int_vec[1];
		OV_UINT				uint_vec[1];
		OV_SINGLE			single_vec[1];
		OV_DOUBLE			double_vec[1];
		OV_STRING			string_vec[1];
		OV_TIME				time_vec[1];
		OV_TIME_SPAN		time_span_vec[1];
		OV_BYTE				byte_vec[1];
	}	valueunion;
}	OV_STATIC_VECTOR;

/*
*	OV_DYNAMIC_VECTOR:
*	------------------
*	Generic dynamic vector value
*/
typedef struct {
	union {
		OV_UINT				veclen;
		OV_BOOL_VEC			bool_vec;
		OV_INT_VEC			int_vec;
		OV_UINT_VEC			uint_vec;
		OV_SINGLE_VEC		single_vec;
		OV_DOUBLE_VEC		double_vec;
		OV_STRING_VEC		string_vec;
		OV_TIME_VEC			time_vec;
		OV_TIME_SPAN_VEC	time_span_vec;
	}	valueunion;
}	OV_DYNAMIC_VECTOR;

/*
*	Set the value of a static vector variable
*/
OV_RESULT OV_DLLFNCEXPORT ov_vector_setstaticvalue(
	OV_STATIC_VECTOR	*pvector,
	const OV_POINTER	pvalue,
	const OV_UINT		veclen,
	const OV_UINT		size,
	const OV_VAR_TYPE	vartype
);

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
);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif
/*
*	End of file
*/

