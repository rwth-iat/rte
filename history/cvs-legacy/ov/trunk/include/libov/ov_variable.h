/*
*   $Id: ov_variable.h,v 1.2 1999-07-27 17:41:11 dirk Exp $
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
*	09-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

#ifndef OV_VARIABLE_H_INCLUDED1
#define OV_VARIABLE_H_INCLUDED1

#include "libov/ov_ov.h"
#include "ov.ovt"

/*
*	OV_VARIABLE_DEF:
*	----------------
*	Variable definition, included in the library's code
*/
struct OV_VARIABLE_DEF {
	struct OV_VARIABLE_DEF	*pnext;
	OV_STRING				identifier;
	OV_UINT					veclen;
	OV_VAR_PROPS			varprops;
	OV_VAR_TYPE				vartype;
	OV_STRING				ctypename;
	OV_STRING				structurename;
	OV_UINT					size;
	OV_UINT					offset;
	OV_UINT					flags;
	OV_STRING				techunit;
	OV_STRING				comment;
	OV_FNCPTR_GET			getfnc;
	OV_FNCPTR_SET			setfnc;
};
typedef struct OV_VARIABLE_DEF OV_VARIABLE_DEF;

/*
*	Function prototype for reading a variable value (scalar)
*/
#define OV_FNCPTR_GET(VARTYPE)				\
	OV_##VARTYPE (*)(						\
		OV_INSTPTR				pobj		\
	)
	
/*
*	Function prototype for reading a variable value (PV)
*/
#define OV_FNCPTR_GETPV(VARTYPE)			\
	OV_##VARTYPE##_PV* (*)(					\
		OV_INSTPTR				pobj		\
	)
	
/*
*	Function prototype for reading a variable value (vector)
*/
#define OV_FNCPTR_GETVEC(VARTYPE)			\
	OV_##VARTYPE *(*)(						\
		OV_INSTPTR				pobj,		\
		OV_UINT					*pveclen	\
	)

/*
*	Function prototype for writing a variable value (scalar)
*/
#define OV_FNCPTR_SET(VARTYPE)				\
	OV_RESULT (*)(							\
		OV_INSTPTR				pobj,		\
		const OV_##VARTYPE		value		\
	)

/*
*	Function prototype for writing a variable value (PV)
*/
#define OV_FNCPTR_SETPV(VARTYPE)			\
	OV_RESULT (*)(							\
		OV_INSTPTR				pobj,		\
		const OV_##VARTYPE##_PV	*pvalue		\
	)

/*
*	Function prototype for writing a variable value (vector)
*/
#define OV_FNCPTR_SETVEC(VARTYPE)			\
	OV_RESULT (*)(							\
		OV_INSTPTR				pobj,		\
		const OV_##VARTYPE		*pvalue,	\
		const OV_UINT			veclen		\
	)

#endif

#include "ov.h"

#ifdef OV_INSTBODY_ov_object

#ifndef OV_VARIABLE_H_INCLUDED2
#define OV_VARIABLE_H_INCLUDED2

#ifdef __cplusplus
extern "C" {
#endif

/*
*	Load a variable into the database
*/
OV_RESULT ov_variable_load(
	OV_VARIABLE_DEF			*pvardef,
	OV_INSTPTR_ov_domain	pparent
);

/*
*	Compare a variable with its definition
*/
OV_RESULT ov_variable_compare(
	OV_INSTPTR_ov_variable	pvar,
	OV_VARIABLE_DEF			*pvardef
);

/*
*	Test if we can unload a variable from the database
*/
OV_BOOL ov_variable_canunload(
	OV_INSTPTR_ov_variable		pvar
);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif

#endif
/*
*	End of file
*/

