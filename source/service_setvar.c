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
***********************************************************************/

#include "config.h"
#include <ctype.h>
#include "urldecode.h"

#define CHECK_BOOLTRUE(Value)	\
			ov_string_compare(Value, "1")		== OV_STRCMP_EQUAL	\
		||	ov_string_compare(Value, "T")	== OV_STRCMP_EQUAL	\
		||	ov_string_compare(Value, "t")	== OV_STRCMP_EQUAL	\
		||	ov_string_compare(Value, "TRUE")	== OV_STRCMP_EQUAL	\
		||	ov_string_compare(Value, "true")	== OV_STRCMP_EQUAL	\
		||	ov_string_compare(Value, "True")	== OV_STRCMP_EQUAL

#define CHECK_BOOLFALSE(Value)	\
			ov_string_compare(Value, "0")			== OV_STRCMP_EQUAL	\
		||	ov_string_compare(Value, "F")	== OV_STRCMP_EQUAL	\
		||	ov_string_compare(Value, "f")	== OV_STRCMP_EQUAL	\
		||	ov_string_compare(Value, "FALSE")	== OV_STRCMP_EQUAL	\
		||	ov_string_compare(Value, "false")	== OV_STRCMP_EQUAL	\
		||	ov_string_compare(Value, "False")	== OV_STRCMP_EQUAL


#define EXEC_SETVAR_RETURN Ov_SetDynamicVectorLength(&pathmatch,0,STRING);\
		Ov_SetDynamicVectorLength(&newvaluematch,0,STRING);\
		Ov_SetDynamicVectorLength(&vartypematch,0,STRING);\
		ov_string_setvalue(&Temp, NULL);\
		return

OV_RESULT kshttp_exec_setvar(OV_STRING_VEC* args, OV_STRING* message, KSHTTP_RESPONSEFORMAT response_format){
	/*
	*	parameter and result objects
	*/
	OV_STRING_VEC pathmatch = {0,NULL};
	OV_STRING_VEC newvaluematch = {0,NULL};
	OV_STRING_VEC vartypematch = {0,NULL};


	OV_SETVAR_PAR	params;
	OV_SETVAR_RES	result;
	OV_SETVAR_ITEM *addrp = NULL;
	OV_GETVAR_PAR	paramsGet;
	OV_STRING *addrpGet = NULL;
	OV_GETVAR_RES	resultGet;
	OV_GETVAR_ITEM	one_resultGet;

	OV_UINT i = 0;
	OV_UINT len = 0;
	OV_STRING *pArgumentList = NULL;
	OV_STRING Temp = NULL;
	OV_STRING Temp2 = NULL;
	OV_UINT stringOffset = 0;
	OV_DOUBLE tempDouble = 0;
	OV_RESULT fr = OV_ERR_OK;
	OV_VAR_TYPE lastVarType = OV_VT_VOID;
	OV_BOOL isNegative = FALSE;

	OV_TICKET* pticket = NULL;

	//process path
	Ov_SetDynamicVectorLength(&pathmatch,0,STRING);
	kshttp_find_arguments(args, "path", &pathmatch);
	if(pathmatch.veclen<1){
		fr = OV_ERR_BADPARAM;
		kshttp_print_result_array(message, response_format, &fr, 1, ": Variable path not found");
		EXEC_SETVAR_RETURN fr; //400
	}
	//process newvalue
	Ov_SetDynamicVectorLength(&newvaluematch,0,STRING);
	kshttp_find_arguments(args, "newvalue", &newvaluematch);
	if(newvaluematch.veclen< pathmatch.veclen){
		fr = OV_ERR_BADPARAM;
		kshttp_print_result_array(message, response_format, &fr, 1, ": not enough Variables newvalue found");
		EXEC_SETVAR_RETURN fr; //400
	}

	ov_memstack_lock();
	addrp = (OV_SETVAR_ITEM*)ov_memstack_alloc(pathmatch.veclen*sizeof(OV_SETVAR_ITEM));
	if(!addrp) {
		ov_memstack_unlock();
		fr = OV_ERR_TARGETGENERIC;
		kshttp_print_result_array(message, response_format, &fr, 1, ": memory problem");
		EXEC_SETVAR_RETURN fr;
	}

	params.items_val = addrp;
	params.items_len = pathmatch.veclen;

	//create NONE-ticket
	pticket = ksbase_NoneAuth->v_ticket.vtbl->createticket(NULL, OV_TT_NONE);

	//#####################################################################
	//process vartype
	//we have to get the vartypes via GetVar, if not specified in request
	Ov_SetDynamicVectorLength(&vartypematch,0,STRING);
	kshttp_find_arguments(args, "vartype", &vartypematch);
	if(vartypematch.veclen< 1){
		//getVar
		addrpGet = Ov_MemStackAlloc(OV_STRING);
		*addrpGet = (OV_STRING)ov_memstack_alloc(pathmatch.veclen*sizeof(OV_STRING));
		if(!*addrpGet) {
			ov_memstack_unlock();
			fr = OV_ERR_TARGETGENERIC;
			kshttp_print_result_array(message, response_format, &fr, 1, ": memory problem");
			EXEC_SETVAR_RETURN fr;
		}
		paramsGet.identifiers_val = addrpGet;
		paramsGet.identifiers_len = pathmatch.veclen;

		for(i=0;i<pathmatch.veclen;i++){
			*addrpGet = pathmatch.value[i];

			//add one size of a pointer
			addrpGet ++;
		}
		ov_ksserver_getvar(2, pticket, &paramsGet, &resultGet);

		if(Ov_Fail(resultGet.result)){
			//general problem like memory problem or NOACCESS
			kshttp_print_result_array(message, response_format, &resultGet.result, 1, ": general problem with get");
			ov_memstack_unlock();
			EXEC_SETVAR_RETURN fr;
		}
	}

	//#####################################################################
	//process multiple path requests at once
	for(i=0;i<pathmatch.veclen;i++){
		if(vartypematch.veclen< 1){
			one_resultGet = *(resultGet.items_val + i);
			if(Ov_Fail(one_resultGet.result)){
				fr = one_resultGet.result;
				kshttp_print_result_array(message, response_format, &fr, 1, ": problem with get");
				EXEC_SETVAR_RETURN fr;
			}
			addrp->var_current_props.value.vartype = one_resultGet.var_current_props.value.vartype;
			addrp->var_current_props.state = one_resultGet.var_current_props.state;
			/*addrp->var_current_props.time.secs = one_resultGet.var_current_props.time.secs;
			addrp->var_current_props.time.usecs = one_resultGet.var_current_props.time.usecs;
			*/
			ov_time_gettime(&addrp->var_current_props.time);
		}else if(vartypematch.veclen <= pathmatch.veclen && i < vartypematch.veclen){
			if(ov_string_compare(vartypematch.value[i], "KS_VT_VOID") == OV_STRCMP_EQUAL){
				addrp->var_current_props.value.vartype = KS_VT_VOID;
			}else if(ov_string_compare(vartypematch.value[i], "KS_VT_BOOL") == OV_STRCMP_EQUAL){
				addrp->var_current_props.value.vartype = KS_VT_BOOL;
			}else if(ov_string_compare(vartypematch.value[i], "KS_VT_INT") == OV_STRCMP_EQUAL){
				addrp->var_current_props.value.vartype = KS_VT_INT;
			}else if(ov_string_compare(vartypematch.value[i], "KS_VT_UINT") == OV_STRCMP_EQUAL){
				addrp->var_current_props.value.vartype = KS_VT_UINT;
			}else if(ov_string_compare(vartypematch.value[i], "KS_VT_SINGLE") == OV_STRCMP_EQUAL){
				addrp->var_current_props.value.vartype = KS_VT_SINGLE;
			}else if(ov_string_compare(vartypematch.value[i], "KS_VT_DOUBLE") == OV_STRCMP_EQUAL){
				addrp->var_current_props.value.vartype = KS_VT_DOUBLE;
			}else if(ov_string_compare(vartypematch.value[i], "KS_VT_STRING") == OV_STRCMP_EQUAL){
				addrp->var_current_props.value.vartype = KS_VT_STRING;
			}else if(ov_string_compare(vartypematch.value[i], "KS_VT_TIME") == OV_STRCMP_EQUAL){
				addrp->var_current_props.value.vartype = KS_VT_TIME;
			}else if(ov_string_compare(vartypematch.value[i], "KS_VT_TIME_SPAN") == OV_STRCMP_EQUAL){
				addrp->var_current_props.value.vartype = KS_VT_TIME_SPAN;
			}else if(ov_string_compare(vartypematch.value[i], "KS_VT_BYTE_VEC") == OV_STRCMP_EQUAL){
				addrp->var_current_props.value.vartype = KS_VT_BYTE_VEC;
			}else if(ov_string_compare(vartypematch.value[i], "KS_VT_BOOL_VEC") == OV_STRCMP_EQUAL){
				addrp->var_current_props.value.vartype = KS_VT_BOOL_VEC;
			}else if(ov_string_compare(vartypematch.value[i], "KS_VT_INT_VEC") == OV_STRCMP_EQUAL){
				addrp->var_current_props.value.vartype = KS_VT_INT_VEC;
			}else if(ov_string_compare(vartypematch.value[i], "KS_VT_UINT_VEC") == OV_STRCMP_EQUAL){
				addrp->var_current_props.value.vartype = KS_VT_UINT_VEC;
			}else if(ov_string_compare(vartypematch.value[i], "KS_VT_INT_VEC") == OV_STRCMP_EQUAL){
				addrp->var_current_props.value.vartype = KS_VT_INT_VEC;
			}else if(ov_string_compare(vartypematch.value[i], "KS_VT_SINGLE_VEC") == OV_STRCMP_EQUAL){
				addrp->var_current_props.value.vartype = KS_VT_SINGLE_VEC;
			}else if(ov_string_compare(vartypematch.value[i], "KS_VT_DOUBLE_VEC") == OV_STRCMP_EQUAL){
				addrp->var_current_props.value.vartype = KS_VT_DOUBLE_VEC;
			}else if(ov_string_compare(vartypematch.value[i], "KS_VT_STRING_VEC") == OV_STRCMP_EQUAL){
				addrp->var_current_props.value.vartype = KS_VT_STRING_VEC;
			}else if(ov_string_compare(vartypematch.value[i], "KS_VT_TIME_VEC") == OV_STRCMP_EQUAL){
				addrp->var_current_props.value.vartype = KS_VT_TIME_VEC;
			}else if(ov_string_compare(vartypematch.value[i], "KS_VT_TIME_SPAN_VEC") == OV_STRCMP_EQUAL){
				addrp->var_current_props.value.vartype = KS_VT_TIME_SPAN_VEC;
			}else if(ov_string_compare(vartypematch.value[i], "KS_VT_TIME_VEC") == OV_STRCMP_EQUAL){
				addrp->var_current_props.value.vartype = KS_VT_TIME_VEC;
			}
			lastVarType = addrp->var_current_props.value.vartype;
		}else{
			addrp->var_current_props.value.vartype = lastVarType;
		}

		addrp->path_and_name = pathmatch.value[i];
		//Variable = addrp->var_current_props;

		//	OV_VT_x_PV is an ANY which is set to x
		//
		switch (addrp->var_current_props.value.vartype){
			case OV_VT_BOOL:
			case OV_VT_BOOL_PV:
				if (CHECK_BOOLTRUE(newvaluematch.value[i])){
					addrp->var_current_props.value.valueunion.val_bool = TRUE;
				}else if (CHECK_BOOLFALSE(newvaluematch.value[i])){
					addrp->var_current_props.value.valueunion.val_bool = FALSE;
				}else{
					fr = OV_ERR_BADPARAM;
					kshttp_print_result_array(message, response_format, &fr, 1, ": Input not detected as bool");
					EXEC_SETVAR_RETURN fr;
				}
				break;

			case OV_VT_INT:
			case OV_VT_INT_PV:
				addrp->var_current_props.value.valueunion.val_int = (OV_INT) strtol(newvaluematch.value[i],NULL,10);
			break;

			case OV_VT_UINT:
			case OV_VT_UINT_PV:
				addrp->var_current_props.value.valueunion.val_uint = (OV_UINT) strtoul(newvaluematch.value[i],NULL,10);
			break;

			case OV_VT_SINGLE:
			case OV_VT_SINGLE_PV:
				addrp->var_current_props.value.valueunion.val_single = (OV_SINGLE) atof(newvaluematch.value[i]);
			break;

			case OV_VT_DOUBLE:
			case OV_VT_DOUBLE_PV:
				addrp->var_current_props.value.valueunion.val_double = (OV_DOUBLE) atof(newvaluematch.value[i]);
			break;

			case OV_VT_STRING:
			case OV_VT_STRING_PV:
				//setting the content of the pointer to null,
				//otherwise setvalue() crashes as it wants to free memory from a garbage pointer
				//we have a new object, so no memory is allocated and the setting to NULL is save
				addrp->var_current_props.value.valueunion.val_string = NULL;
				fr = ov_string_setvalue(&addrp->var_current_props.value.valueunion.val_string, newvaluematch.value[i]);
				if (Ov_Fail(fr)){
					kshttp_print_result_array(message, response_format, &fr, 1, ": Setting string value failed");
					EXEC_SETVAR_RETURN fr;
				};
				break;

			case OV_VT_VOID:
			case OV_VT_VOID | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP:
				if(ov_string_getlength(newvaluematch.value[i]) > 0){
					fr = OV_ERR_BADTYPE;
					kshttp_print_result_array(message, response_format, &fr, 1, ": Variable is/should be void, but a newvalue is given");
					EXEC_SETVAR_RETURN fr;
				}
				break;

			case OV_VT_TIME:
			case OV_VT_TIME | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP:
				fr = kshttp_asciitotime(&addrp->var_current_props.value.valueunion.val_time, newvaluematch.value[i]);
				if (Ov_Fail(fr)){
					kshttp_print_result_array(message, response_format, &fr, 1, ": Setting time value failed");
					EXEC_SETVAR_RETURN fr;
				};
				break;

			case OV_VT_TIME_SPAN:
			case OV_VT_TIME_SPAN | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP:
				//can be 42.1241 or P42.123456S or -P23.42S
				ov_string_setvalue(&Temp, newvaluematch.value[i]);
				if(Temp[0] == 'P'){
					stringOffset = 1;
				}else if(Temp[0] == '-' && Temp[1] == 'P'){
					stringOffset = 2;
					isNegative = TRUE;
				}
				tempDouble = atof(Temp+stringOffset);
				if(isNegative == TRUE){
					tempDouble = -tempDouble;
				}
				Ov_DoubleToTimeSpan(tempDouble, addrp->var_current_props.value.valueunion.val_time_span);
				break;

			case OV_VT_STRUCT:
			case (OV_VT_STRUCT | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):
				//deprecated as KS2.0r
				fr = OV_ERR_NOTIMPLEMENTED;
				kshttp_print_result_array(message, response_format, &fr, 1, ": STRUCT is deprecated with KS2.0r");
				EXEC_SETVAR_RETURN fr;
			break;

			//****************** VEC: *******************
			/* request could be "{1}%20{10}"
			 * split at "%20", discard the "{" via pointer arithmetic (+1)
			 * the strtol and atof commands ignores the last "}"
			 * with STRING_VEC we have to do some more
			 */

	/*		todo implement is base64encoded
			case OV_VT_BYTE_VEC:
			case (OV_VT_BYTE_VEC | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):
	*/

			case OV_VT_BOOL_VEC:
			case OV_VT_BOOL_PV_VEC:
				pArgumentList = ov_string_split(newvaluematch.value[i], "%20", &len);
				addrp->var_current_props.value.valueunion.val_bool_vec.veclen = 0;
				addrp->var_current_props.value.valueunion.val_bool_vec.value = NULL;
				Ov_SetDynamicVectorLength(&addrp->var_current_props.value.valueunion.val_bool_vec, len, BOOL);
				for(i = 0; i < len; i++){
					//killing the first character
					ov_string_setvalue(&Temp, pArgumentList[i]+1);
					//kill the last character, now we have two null bytes at the end
					Temp[ov_string_getlength(Temp)-1] = '\0';

					if (CHECK_BOOLTRUE(Temp)){
						addrp->var_current_props.value.valueunion.val_bool_vec.value[i] = TRUE;
					}else if (CHECK_BOOLFALSE(Temp)){
						addrp->var_current_props.value.valueunion.val_bool_vec.value[i] = FALSE;
					}else{
						//default
						addrp->var_current_props.value.valueunion.val_bool_vec.value[i] = FALSE;
					}
				}
				ov_string_freelist(pArgumentList);
				break;

			case OV_VT_INT_VEC:
			case OV_VT_INT_PV_VEC:
				pArgumentList = ov_string_split(newvaluematch.value[i], "%20", &len);
				addrp->var_current_props.value.valueunion.val_int_vec.veclen = 0;
				addrp->var_current_props.value.valueunion.val_int_vec.value = NULL;
				Ov_SetDynamicVectorLength(&addrp->var_current_props.value.valueunion.val_int_vec, len, INT);
				for(i = 0; i < len; i++){
					addrp->var_current_props.value.valueunion.val_int_vec.value[i] = (OV_INT) strtol(pArgumentList[i]+1,NULL,10);
				}
				ov_string_freelist(pArgumentList);
				break;

			case OV_VT_UINT_VEC:
			case OV_VT_UINT_PV_VEC:
				pArgumentList = ov_string_split(newvaluematch.value[i], "%20", &len);
				addrp->var_current_props.value.valueunion.val_uint_vec.veclen = 0;
				addrp->var_current_props.value.valueunion.val_uint_vec.value = NULL;
				Ov_SetDynamicVectorLength(&addrp->var_current_props.value.valueunion.val_uint_vec, len, UINT);
				for(i = 0; i < len; i++){
					addrp->var_current_props.value.valueunion.val_uint_vec.value[i] = (OV_UINT) strtoul(pArgumentList[i]+1,NULL,10);
				}
				ov_string_freelist(pArgumentList);
				break;

			case OV_VT_SINGLE_VEC:
			case OV_VT_SINGLE_PV_VEC:
				pArgumentList = ov_string_split(newvaluematch.value[i], "%20", &len);
				addrp->var_current_props.value.valueunion.val_single_vec.veclen = 0;
				addrp->var_current_props.value.valueunion.val_single_vec.value = NULL;
				Ov_SetDynamicVectorLength(&addrp->var_current_props.value.valueunion.val_single_vec, len, SINGLE);
				for(i = 0; i < len; i++){
					addrp->var_current_props.value.valueunion.val_single_vec.value[i] = (OV_SINGLE) atof(pArgumentList[i]+1);
				}
				ov_string_freelist(pArgumentList);
				break;

			case OV_VT_DOUBLE_VEC:
			case OV_VT_DOUBLE_PV_VEC:
				pArgumentList = ov_string_split(newvaluematch.value[i], "%20", &len);
				addrp->var_current_props.value.valueunion.val_double_vec.veclen = 0;
				addrp->var_current_props.value.valueunion.val_double_vec.value = NULL;
				Ov_SetDynamicVectorLength(&addrp->var_current_props.value.valueunion.val_double_vec, len, DOUBLE);
				for(i = 0; i < len; i++){
					addrp->var_current_props.value.valueunion.val_double_vec.value[i] = (OV_SINGLE) atof(pArgumentList[i]+1);
				}
				ov_string_freelist(pArgumentList);
				break;

			case OV_VT_STRING_VEC:
			case OV_VT_STRING_PV_VEC:
				//request could be "{hello}%20{world}"
				pArgumentList = ov_string_split(newvaluematch.value[i], "%20", &len);
				addrp->var_current_props.value.valueunion.val_string_vec.veclen = 0;
				addrp->var_current_props.value.valueunion.val_string_vec.value = NULL;
				Ov_SetDynamicVectorLength(&addrp->var_current_props.value.valueunion.val_string_vec, len, STRING);

				if(*pArgumentList[i] != '{' && len > 2){
					fr = OV_ERR_BADPARAM;
					kshttp_print_result_array(message, response_format, &fr, 1, ": VEC entries should be urlencoded, separated with a space and wrapped with curly brackets");
					EXEC_SETVAR_RETURN fr;
				}

				for(i = 0; i < len; i++){
					//setting the content of the pointers to null
					//otherwise setvalue() crashes as it wants to free memory from a garbage pointer
					addrp->var_current_props.value.valueunion.val_string_vec.value[i] = NULL;
					ov_memstack_lock();
					if(*pArgumentList[i] != '{'){
						Temp2 = pArgumentList[i];
					}else{
						//killing the first character aka {
						ov_string_setvalue(&Temp, pArgumentList[i]+1);
						//kill the last character aka }, now we have two null bytes at the end
						Temp[ov_string_getlength(Temp)-1] = '\0';
						//finally decode URLencoding
						Temp2 = url_decode(Temp);
						//Temp2 = Temp;
					}
					fr = ov_string_setvalue(&addrp->var_current_props.value.valueunion.val_string_vec.value[i], Temp2);
					ov_memstack_unlock();
				}
				Temp2 = NULL; //had a memstack pointer only
				ov_string_freelist(pArgumentList);
				break;

			case OV_VT_STRUCT_VEC:
			case (OV_VT_STRUCT_VEC | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):
				//deprecated as KS2.0r
				fr = OV_ERR_NOTIMPLEMENTED;
				kshttp_print_result_array(message, response_format, &fr, 1, ": STRUCT is deprecated with KS2.0r");
				EXEC_SETVAR_RETURN fr;

	/*	TODO Time* VEC
			case OV_VT_TIME_VEC:
			case OV_VT_TIME_PV_VEC:

			case OV_VT_TIME_SPAN_VEC:
			case OV_VT_TIME_SPAN_PV_VEC:

			case OV_VT_STATE_VEC:
			case (OV_VT_STATE_VEC | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):
	*/
			default:
	/*				ov_logfile_error("%s:%d - GestureReaction - target: %s, Userinput (%s), DataType %u not implemented.", __FILE__, __LINE__,
					*varname,
					newvaluematch.value[i],
					addrp->var_current_props.value.vartype);
	*/
				fr = OV_ERR_NOTIMPLEMENTED;
				kshttp_print_result_array(message, response_format, &fr, 1, ": Vartype not supported");
				EXEC_SETVAR_RETURN fr;
		}

		//add one size of a pointer
		addrp ++;
	}

	ov_ksserver_setvar(2, pticket, &params, &result);

	/*	delete Ticket	*/
	pticket->vtbl->deleteticket(pticket);

	/**
	 * Parse result from KS function
	 */

	if(Ov_Fail(result.result)){
		//general problem like memory problem or NOACCESS
		kshttp_print_result_array(message, response_format, &result.result, 1, ": general problem");
		ov_memstack_unlock();
		EXEC_SETVAR_RETURN result.result;
	}
	fr = kshttp_print_result_array(message, response_format, result.results_val, result.results_len, "");

	ov_memstack_unlock();
	EXEC_SETVAR_RETURN fr;
}
