/*
*	Copyright (C) 2014
*	Chair of Process Control Engineering,
*	Aachen University of Technology.
*	All rights reserved.
*
*	Redistribution and use in source and binary forms, with or without
*	modification, are permitted provided that the following conditions
*	are met:
*	1. Redistributions of source code must retain the above copyright
*	   notice, this list of conditions and the following disclaimer.
*	2. Redistributions in binary form must print or display the above
*	   copyright notice either during startup or must have a means for
*	   the user to view the copyright notice.
*	3. Redistributions in binary form must reproduce the above copyright
*	   notice, this list of conditions and the following disclaimer in
*		the documentation and/or other materials provided with the
*		distribution.
*	4. Neither the name of the Chair of Process Control Engineering nor
*		the name of the Aachen University of Technology may be used to
*		endorse or promote products derived from this software without
*		specific prior written permission.
*
*	THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
*	``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE CHAIR OF
*	PROCESS CONTROL ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT,
*	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
*	OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
*	AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
*	WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*	POSSIBILITY OF SUCH DAMAGE.
*
***********************************************************************
*
* CSHMI - Client-Side Human-Machine Interface
*
*	File:
*	------
*	classCondition_CompareIteratedChild.c
*
*	Editors:
*	--------
*	Je							Holger Jeromin <jeromin@plt.rwth-aachen.de>
*
*	SVN:
*	----
*	$Revision$
*	$Date$
*
*	History:
*	--------
*	28-December-2011			Yannick Rocks		V0.1.0
*		-	File created
*
***********************************************************************/

#ifndef OV_COMPILE_LIBRARY_cshmi
#define OV_COMPILE_LIBRARY_cshmi
#endif


#include "cshmilib.h"

OV_DLLFNCEXPORT OV_RESULT cshmi_CompareIteratedChild_childValue_set(
	OV_INSTPTR_cshmi_CompareIteratedChild          pobj,
	const OV_STRING  value
) {
	OV_STRING erroroutput = NULL;
	//force our keywords
	if (	ov_string_compare(value, "OP_NAME") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "OP_TYPE") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "OP_COMMENT") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "OP_ACCESS") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "OP_SEMANTICS") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "OP_CREATIONTIME") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "OP_CLASS") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "OP_CREATIONTIME") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "OP_TECHUNIT") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "OP_ASSOCIDENT") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "OP_ROLEIDENT") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "OP_DEFAULTINTERP") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "OP_SUPPORTEDINTERP") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "OP_TYPEIDENT") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "OP_VALUE") == OV_STRCMP_EQUAL
	){
		cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
		return ov_string_setvalue(&pobj->v_childValue,value);
	} else if (	ov_string_compare(value, "") == OV_STRCMP_EQUAL){
		//unconfigured must be allowed
		cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
		return ov_string_setvalue(&pobj->v_childValue,value);
	}else{
		ov_memstack_lock();
		ov_string_print(&erroroutput, "object %s had wrong childValue '%s'. Rejecting Variable change.", ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object, pobj), 2), value);
		ov_memstack_unlock();
		ov_logfile_warning(erroroutput);
		ov_string_setvalue(&erroroutput, NULL);
		return OV_ERR_BADPARAM;
	}
}

OV_DLLFNCEXPORT OV_RESULT cshmi_CompareIteratedChild_comptype_set(
	OV_INSTPTR_cshmi_CompareIteratedChild          pobj,
	const OV_STRING  value
) {
	OV_STRING erroroutput = NULL;
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	//force our keywords
	if (	ov_string_compare(value, "<") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "<=") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "==") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "!=") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, ">=") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, ">") == OV_STRCMP_EQUAL){
		return ov_string_setvalue(&pobj->v_comptype,value);
	}else{
		ov_memstack_lock();
		ov_string_print(&erroroutput, "object %s had wrong comptype '%s'. Rejecting Variable change.", ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object, pobj), 2), value);
		ov_memstack_unlock();
		ov_logfile_warning(erroroutput);
		ov_string_setvalue(&erroroutput, NULL);
		return OV_ERR_BADPARAM;
	}
}

