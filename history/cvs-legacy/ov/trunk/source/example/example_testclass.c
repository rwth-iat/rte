/*
*   $Id: example_testclass.c,v 1.1 1999-07-19 15:02:11 dirk Exp $
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

#include <stdio.h>

#include "example.h"
#include "libov/ov_string.h"

/*	----------------------------------------------------------------------	*/

/*
*	Accessor functions
*/
OV_STRING OV_DLLFNCEXPORT example_testclass_GetOrSetMeString_get(
	OV_INSTPTR_example_testclass	ptest
) {
	ov_logfile_info("You asked for GetOrSetMeString!");
	return ptest->v_GetOrSetMeString;
}

OV_RESULT OV_DLLFNCEXPORT example_testclass_GetOrSetMeString_set(
	OV_INSTPTR_example_testclass	ptest,
	OV_STRING						value
) {
	ov_logfile_info("You set GetOrSetMeString!");
	ov_string_setvalue(&ptest->v_GetOrSetMeString, value);
	return OV_ERR_OK;
}

OV_INT_PV* OV_DLLFNCEXPORT example_testclass_GetOrSetMeInt_get(
	OV_INSTPTR_example_testclass	ptest
) {
	ov_logfile_info("You asked for GetOrSetMeInt!");
	return &ptest->v_GetOrSetMeInt;
}

OV_RESULT OV_DLLFNCEXPORT example_testclass_GetOrSetMeInt_set(
	OV_INSTPTR_example_testclass	ptest,
	const OV_INT_PV					*pvalue
) {
	ov_logfile_info("You set GetOrSetMeInt!");
	ptest->v_GetOrSetMeInt = *pvalue;
	return OV_ERR_OK;
}

OV_SINGLE_PV* OV_DLLFNCEXPORT example_testclass_GetOrSetMeSingle_get(
	OV_INSTPTR_example_testclass	ptest
) {
	ov_logfile_info("You asked for GetOrSetMeSingle!");
	return &ptest->v_GetOrSetMeSingle;
}

OV_RESULT OV_DLLFNCEXPORT example_testclass_GetOrSetMeSingle_set(
	OV_INSTPTR_example_testclass	ptest,
	const OV_SINGLE_PV				*pvalue
) {
	ov_logfile_info("You set GetOrSetMeSingle!");
	ptest->v_GetOrSetMeSingle = *pvalue;
	return OV_ERR_OK;
}

OV_BOOL_PV* OV_DLLFNCEXPORT example_testclass_GetOrSetMeBool_get(
	OV_INSTPTR_example_testclass	ptest
) {
	ov_logfile_info("You asked for GetOrSetMeBool!");
	return &ptest->v_GetOrSetMeBool;
}

OV_RESULT OV_DLLFNCEXPORT example_testclass_GetOrSetMeBool_set(
	OV_INSTPTR_example_testclass	ptest,
	const OV_BOOL_PV				*pvalue
) {
	ov_logfile_info("You set GetOrSetMeBool!");
	ptest->v_GetOrSetMeBool = *pvalue;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

