/*
*   $Id: example_fnc.c,v 1.2 2004-10-29 09:07:45 ansgar Exp $
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
*	27-Oct-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

#ifndef OV_COMPILE_LIBRARY_example
#define OV_COMPILE_LIBRARY_example
#endif

#include "example.h"
#include "libov/ov_macros.h"

/*	----------------------------------------------------------------------	*/

/*
*	Accessor functions for "expression"
*/
OV_DLLFNCEXPORT OV_STRING example_fnc_expression_get(
	OV_INSTPTR_example_fnc	pobj
) {
	return pobj->v_expression;
}

OV_DLLFNCEXPORT OV_RESULT example_fnc_expression_set(
	OV_INSTPTR_example_fnc	pobj,
	const OV_STRING				value
) {
	/*
	*	local variables
	*/
	OV_EXPRESSION *pexpr;
	/*
	*	instructions
	*/
	if(value) {
		pexpr = ov_expression_create(Ov_PtrUpCast(ov_object, pobj), value);
		if(!pexpr) {
			return OV_ERR_BADVALUE;
		}
	} else {
		pexpr = NULL;
	}
	if(pobj->v_pexpr) {
		ov_expression_delete(pobj->v_pexpr);
	}
	pobj->v_pexpr = pexpr;
	ov_string_setvalue(&pobj->v_expression, value);
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Accessor functions for "in1"
*/
OV_DLLFNCEXPORT OV_DOUBLE example_fnc_in1_get(
	OV_INSTPTR_example_fnc	pobj
) {
	return pobj->v_in1;
}

OV_DLLFNCEXPORT OV_RESULT example_fnc_in1_set(
	OV_INSTPTR_example_fnc	pobj,
	const OV_DOUBLE			value
) {
	pobj->v_in1 = value;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Accessor functions for "in2"
*/
OV_DLLFNCEXPORT OV_DOUBLE example_fnc_in2_get(
	OV_INSTPTR_example_fnc	pobj
) {
	return pobj->v_in2;
}

OV_DLLFNCEXPORT OV_RESULT example_fnc_in2_set(
	OV_INSTPTR_example_fnc	pobj,
	const OV_DOUBLE			value
) {
	pobj->v_in2 = value;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Accessor functions for "out"
*/
OV_DLLFNCEXPORT OV_DOUBLE example_fnc_out_get(
	OV_INSTPTR_example_fnc	pobj
) {
	return (pobj->v_pexpr)?(ov_expression_evaluate(pobj->v_pexpr, NULL)):(0.0);
}

/*	----------------------------------------------------------------------	*/

/*
*	Startup of a fnc object
*/
OV_DLLFNCEXPORT void example_fnc_startup(
	OV_INSTPTR_ov_object	pobj
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_example_fnc	pfnc = Ov_StaticPtrCast(example_fnc, pobj);
	/*
	*	instructions
	*/
	ov_object_startup(pobj);
	if(pfnc->v_expression) {
		pfnc->v_pexpr = ov_expression_create(pobj, pfnc->v_expression);
	} else {
		pfnc->v_pexpr = NULL;
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	Shutdown of a fnc object
*/
OV_DLLFNCEXPORT void example_fnc_shutdown(
	OV_INSTPTR_ov_object	pobj
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_example_fnc	pfnc = Ov_StaticPtrCast(example_fnc, pobj);
	/*
	*	instructions
	*/
	if(pfnc->v_pexpr) {
		ov_expression_delete(pfnc->v_pexpr);
		pfnc->v_pexpr = NULL;
	}
	ov_object_shutdown(pobj);
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

