/*
*   $Id: example_testclass.c,v 1.8 2000-02-10 13:06:59 dirk Exp $
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
*	04-May-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

#ifndef OV_COMPILE_LIBRARY_example
#define OV_COMPILE_LIBRARY_example
#endif

#include "example.h"
#include "libov/ov_macros.h"

/*	----------------------------------------------------------------------	*/

/*
*	Accessor functions for "Single"
*/
OV_DLLFNCEXPORT OV_SINGLE example_testclass_Single_get(
	OV_INSTPTR_example_testclass	ptest
) {
	ov_logfile_info("You asked for Single!");
	return ptest->v_Single;
}

OV_DLLFNCEXPORT OV_RESULT example_testclass_Single_set(
	OV_INSTPTR_example_testclass	ptest,
	const OV_SINGLE					value
) {
	ptest->v_Single = value;
	ov_logfile_info("You set Single!");
	return OV_ERR_OK;
}

/*
*	Accessor functions for "String"
*/
OV_DLLFNCEXPORT OV_STRING example_testclass_String_get(
	OV_INSTPTR_example_testclass	ptest
) {
	ov_logfile_info("You asked for String!");
	return ptest->v_String;
}

OV_DLLFNCEXPORT OV_RESULT example_testclass_String_set(
	OV_INSTPTR_example_testclass	ptest,
	OV_STRING						value
) {
	ov_string_setvalue(&ptest->v_String, value);
	ov_logfile_info("You set String!");
	return OV_ERR_OK;
}

/*
*	Accessor functions for "Time"
*/
OV_DLLFNCEXPORT OV_TIME example_testclass_Time_get(
	OV_INSTPTR_example_testclass	ptest
) {
	OV_STATICINSTPTR_example_testclass	pstatictest
		= Ov_GetStaticInstPtr(example_testclass, ptest);
	ov_logfile_info("You asked for Time!");
	return pstatictest->v_Time;
}

OV_DLLFNCEXPORT OV_RESULT example_testclass_Time_set(
	OV_INSTPTR_example_testclass	ptest,
	OV_TIME							value
) {
	OV_STATICINSTPTR_example_testclass	pstatictest
		= Ov_GetStaticInstPtr(example_testclass, ptest);
	pstatictest->v_Time = value;
	ov_logfile_info("You set Time!");
	return OV_ERR_OK;
}

/*
*	Accessor functions for "IntPV"
*/
OV_DLLFNCEXPORT OV_INT_PV *example_testclass_IntPV_get(
	OV_INSTPTR_example_testclass	ptest
) {
	ov_logfile_info("You asked for IntPV!");
	return &ptest->v_IntPV;
}

OV_DLLFNCEXPORT OV_RESULT example_testclass_IntPV_set(
	OV_INSTPTR_example_testclass	ptest,
	const OV_INT_PV					*pvalue
) {
	ptest->v_IntPV = *pvalue;
	ov_logfile_info("You set IntPV!");
	return OV_ERR_OK;
}

/*
*	Accessor functions for "UIntVec"
*/
OV_DLLFNCEXPORT OV_UINT *example_testclass_UIntVec_get(
	OV_INSTPTR_example_testclass	ptest,
	OV_UINT							*pveclen
) {
	ov_logfile_info("You asked for UIntVec!");
	*pveclen = ptest->v_UIntVec.veclen;
	return ptest->v_UIntVec.value;
}

OV_DLLFNCEXPORT OV_RESULT example_testclass_UIntVec_set(
	OV_INSTPTR_example_testclass	ptest,
	const OV_UINT					*pvalue,
	const OV_UINT					veclen
) {
	ov_logfile_info("You set UIntVec!");
	return Ov_SetDynamicVectorValue(&ptest->v_UIntVec,
		pvalue, veclen, UINT);
}

/*
*	Accessor functions for "StringVec"
*/
OV_DLLFNCEXPORT OV_STRING *example_testclass_StringVec_get(
	OV_INSTPTR_example_testclass	ptest,
	OV_UINT							*pveclen
) {
	/*
	*	local variables
	*/
	static OV_STRING stringvec[] = {
		"This string is virtual.",
		"It is not stored in the object."
	};
	/*
	*	instructions
	*/
	ov_logfile_info("You asked for StringVec!");
	*pveclen = sizeof(stringvec)/sizeof(OV_STRING);
	return stringvec;
}

OV_DLLFNCEXPORT OV_RESULT example_testclass_StringVec_set(
	OV_INSTPTR_example_testclass	ptest,
	const OV_STRING*				pvalue,
	const OV_UINT					veclen
) {
	/*
	*	local variables
	*/
	OV_UINT	i;
	/*
	*	instructions
	*/
	ov_logfile_info("You (tried to) set StringVec!");
	ov_logfile_info("The value is:");
	for(i=0; i<veclen; i++) {
		ov_logfile_info("StringVec[%ld] = \"%s\".", i, pvalue[i]);
	}
	return OV_ERR_OK;
}

/*
*	Accessor functions for "AnyVariable"
*/
OV_DLLFNCEXPORT OV_ANY *example_testclass_AnyVariable_get(
	OV_INSTPTR_example_testclass	ptest
) {
	ov_logfile_info("You asked for AnyVariable!");
	return &ptest->v_AnyVariable;
}

OV_DLLFNCEXPORT OV_RESULT example_testclass_AnyVariable_set(
	OV_INSTPTR_example_testclass	ptest,
	const OV_ANY					*pvalue
) {
	ov_logfile_info("You set AnyVariable!");
	return Ov_SetAnyValue(&ptest->v_AnyVariable, pvalue);
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

