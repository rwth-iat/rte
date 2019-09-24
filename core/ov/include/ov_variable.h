/*
*   $Id: ov_variable.h,v 1.8 2011-01-18 14:27:34 martin Exp $
*
*   Copyright (C) 1998
*   Lehrstuhl fuer Prozessleittechnik,
*   D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
*
*   This file is part of the ACPLT/OV Package 
*   
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
*	09-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	04-Nov-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: variable type ANY added.
*	06-Dez-2001 Ansgar M�nnemann <ansgar@plt.rwth-aachen.de>: initialvalue of variable added.
*/

#ifndef OV_VARIABLE_H_INCLUDED1
#define OV_VARIABLE_H_INCLUDED1

#include "ov_ov.h"
#include "ov_debug.h"
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
	OV_VAR_PROPS				varprops;
	OV_VAR_TYPE				vartype;
	OV_STRING				ctypename;
	OV_STRING				structurename;
	OV_UINT					size;
	OV_UINT					offset;
	OV_UINT					flags;
	OV_STRING				techunit;
	OV_STRING				comment;
	OV_FNCPTR_GET				getfnc;
	OV_FNCPTR_SET				setfnc;
};
typedef struct OV_VARIABLE_DEF OV_VARIABLE_DEF;

/*
*	Function prototype for reading a variable value (scalar)
*/
#define OV_FNCPTR_GET(VARTYPE)				\
	OV_##VARTYPE (*)(						\
		OV_INSTPTR				pobj		\
	)

#define OV_FNCPTR_GETTIME(VARTYPE)			\
	OV_##VARTYPE* (*)(						\
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
*	Function prototype for reading a variable value (vector-PV)
*/
#define OV_FNCPTR_GETPVVEC(VARTYPE)			\
	OV_##VARTYPE##_PV_VEC* (*)(				\
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
*	Function prototype for reading an ANY value
*/
#define OV_FNCPTR_GETANY					\
	OV_ANY* (*)(							\
		OV_INSTPTR				pobj		\
	)
	
	
/*
*	Function prototype for writing a variable value (scalar)
*/
#define OV_FNCPTR_SET(VARTYPE)				\
	OV_RESULT (*)(							\
		OV_INSTPTR				pobj,		\
		const OV_##VARTYPE		value		\
	)

#define OV_FNCPTR_SETTIME(VARTYPE)			\
	OV_RESULT (*)(							\
		OV_INSTPTR				pobj,		\
		const OV_##VARTYPE		*value		\
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
*	Function prototype for writing a variable value (vector-PV)
*/
#define OV_FNCPTR_SETPVVEC(VARTYPE)			\
	OV_RESULT (*)(							\
		OV_INSTPTR				pobj,		\
		const OV_##VARTYPE##_PV_VEC	*pvalue		\
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

/*
*	Function prototype for writing an ANY value
*/
#define OV_FNCPTR_SETANY					\
	OV_RESULT (*)(							\
		OV_INSTPTR				pobj,		\
		const OV_ANY			*pvalue		\
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

/**
*	Set the value of an ANY variable
*	You should use the macro Ov_SetAnyValue()
*/
OV_DLLFNCEXPORT OV_RESULT ov_variable_setanyvalue(
	OV_ANY			*pany,
	const OV_ANY	*pvalue
);

/**
*	Set the value of an ANY variable by pointer
*	You should use the macro Ov_SetAnyValueByPointer()
*/
OV_DLLFNCEXPORT OV_RESULT ov_variable_setanyvaluebypointer(
	OV_ANY*			pany,
	OV_VAR_TYPE		type,
	const void*		value
);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif

#endif
/*
*	End of file
*/

