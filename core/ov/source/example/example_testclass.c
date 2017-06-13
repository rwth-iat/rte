/*
*   $Id: example_testclass.c,v 1.10 2010-12-20 13:23:06 martin Exp $
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
	const OV_STRING			value
) {
	ov_string_setvalue(&ptest->v_String, value);
	ov_logfile_info("You set String!");
	return OV_ERR_OK;
}

/*
*	Accessor functions for "Time"
*/
OV_DLLFNCEXPORT OV_TIME* example_testclass_Time_get(
	OV_INSTPTR_example_testclass	ptest
) {
	OV_STATICINSTPTR_example_testclass	pstatictest
		= Ov_GetStaticInstPtr(example_testclass, ptest);
	ov_logfile_info("You asked for Time!");
	return &pstatictest->v_Time;
}

OV_DLLFNCEXPORT OV_RESULT example_testclass_Time_set(
	OV_INSTPTR_example_testclass	ptest,
	const OV_TIME			*value
) {
	OV_STATICINSTPTR_example_testclass	pstatictest
		= Ov_GetStaticInstPtr(example_testclass, ptest);
	pstatictest->v_Time = *value;
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

