/*
*   $Id: ov_vector.h,v 1.6 2006-10-17 11:30:12 stefan Exp $
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
*	OV_VECTORCOMPARE:
*	-----------------
*	Result of function ov_vector_compare(s1, s2, ...)
*/
#define	OV_VECCMP_EQUAL	0		/* s1 = s2 */
#define	OV_VECCMP_MORE		1		/* s1 > s2 */
#define	OV_VECCMP_LESS		-1		/* s1 < s2 */

/**
*	Set the value of a static vector variable
*	you should use the macro Ov_SetStaticVectorValue() which is type safe and calculates the size for you
*/
OV_DLLFNCEXPORT OV_RESULT ov_vector_setstaticvalue(
	OV_POINTER			pvector,
	const OV_POINTER	pvalue,
	const OV_UINT		veclen,
	const OV_UINT		size,
	const OV_VAR_TYPE	vartype
);

/**
*	Set the value of a dynamic vector variable
*	you can use the macro Ov_SetDynamicVectorValue() which is type safe and calculates the size for you
*/
OV_DLLFNCEXPORT OV_RESULT ov_vector_setdynamicvalue(
	OV_GENERIC_VEC		*pvector,
	const OV_POINTER	pvalue,
	const OV_UINT		veclen,
	const OV_UINT		size,
	const OV_VAR_TYPE	vartype
);

/**
*	Set the vector length of a dynamic vector variable value
*	you should use the macro Ov_SetDynamicVectorLength() which is type safe and calculates the size for you
*
*	If the new veclen is bigger than the old values will survive.
*	If the new veclen is smaller than part of the memory will be freed.
*	If the new veclen is zero than all the memory will be freed.
*
*/
OV_DLLFNCEXPORT OV_RESULT ov_vector_setdynamicveclen(
	OV_GENERIC_VEC		*pvector,
	const OV_UINT		veclen,
	const OV_UINT		size,
	const OV_VAR_TYPE	vartype
);
	
/**
*	Compare two vector variable values, result is greater than, equal to or less than zero
*	you should use Ov_CompareVectorValues() in your code, as it calculates the size for you
*/
OV_DLLFNCEXPORT OV_INT ov_vector_compare(
	const OV_POINTER	pvalue1,
	const OV_POINTER	pvalue2,
	const OV_UINT		veclen,
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

