/*
 *
 *  Created on: 21.10.2014
 *      Author: lars
 */

#ifndef OV_COMPILE_LIBRARY_opcua
#define OV_COMPILE_LIBRARY_opcua
#endif
//
//#ifdef MOZ_VALGRIND
//#  define JS_VALGRIND
//#endif
//#ifdef JS_VALGRIND
//#  include <valgrind/valgrind.h>
//#endif


#include "helpers.h"
#include "ov_string.h"
#include "ov_time.h"
#include "ov_path.h"
#include "ov_macros.h"

#define EPOCHDIFFERENCE_SECONDS	11644473600LL
#define TOSECONDS				10000000LL
#define TOMICROSECONDS			10LL



OV_DLLFNCEXPORT void opcua_helpers_UA_String_append(UA_String * string, const char * append){
	if(string == NULL)
		return;
	size_t length = strlen(append);
	string->data = UA_realloc(string->data, sizeof(UA_Byte) * (length + string->length));
	if(string->data == NULL){
		string->length = 0;
		return;
	}
	memcpy(&string->data[string->length], append, length);
	string->length += length;
}

OV_DLLFNCEXPORT UA_StatusCode opcua_helpers_ovResultToUaStatusCode(OV_RESULT result){
	switch(result){
	case OV_ERR_OK:
		return UA_STATUSCODE_GOOD;
	case OV_ERR_GENERIC:
	case OV_ERR_TARGETGENERIC:
		return UA_STATUSCODE_BADINTERNALERROR;
	case OV_ERR_BADAUTH:
		return UA_STATUSCODE_BADCERTIFICATEUNTRUSTED;
	case OV_ERR_UNKNOWNAUTH:
		return UA_STATUSCODE_BADCERTIFICATEURIINVALID;
	case OV_ERR_NOTIMPLEMENTED:
		return UA_STATUSCODE_BADNOTIMPLEMENTED;
	case OV_ERR_BADPARAM:
	case OV_ERR_BADOBJTYPE:
	case OV_ERR_BADMASK:
	case OV_ERR_BADPLACEMENT:
		return UA_STATUSCODE_BADINVALIDARGUMENT;
	case OV_ERR_BADNAME:
		return UA_STATUSCODE_BADNODEIDREJECTED;
	case OV_ERR_BADPATH:
	case OV_ERR_CANTMOVE:
		return UA_STATUSCODE_BADBROWSENAMEINVALID;
	case OV_ERR_NOACCESS:
		return UA_STATUSCODE_BADSECURITYCHECKSFAILED;
	case OV_ERR_BADTYPE:
		return UA_STATUSCODE_BADTYPEMISMATCH;
	case OV_ERR_BADVALUE:
		return UA_STATUSCODE_BADOUTOFRANGE;
	case OV_ERR_BADFACTORY:
		return UA_STATUSCODE_BADNODECLASSINVALID;
	case OV_ERR_ALREADYEXISTS:
		return UA_STATUSCODE_BADENTRYEXISTS;
	case OV_ERR_DBOUTOFMEMORY:
	case OV_ERR_HEAPOUTOFMEMORY:
		return UA_STATUSCODE_BADOUTOFMEMORY;
	default:
		return UA_STATUSCODE_BADUNEXPECTEDERROR;
	}
}

OV_DLLFNCEXPORT UA_NodeId opcua_helpers_ovVarTypeToNodeId(OV_VAR_TYPE type){
	UA_NodeId varType;
	UA_NodeId_init(&varType);
	varType.namespaceIndex = 0;
	varType.identifierType = UA_NODEIDTYPE_NUMERIC;
	switch(type & (OV_VT_KSMASK & (~OV_VT_ISVECTOR))){
	case OV_VT_ANY:
	case OV_VT_VOID:
		varType.identifier.numeric = 24;
		break;
	case OV_VT_BOOL:
		varType.identifier.numeric = 1;
		break;
	case OV_VT_BYTE:
		varType.identifier.numeric = 3;
		break;
	case OV_VT_CTYPE:
	case OV_VT_POINTER:
	case OV_VT_STRUCT:
		varType.identifier.numeric = 22;
		break;
	case OV_VT_DOUBLE:
		varType.identifier.numeric = 11;
		break;
	case OV_VT_INT:
		varType.identifier.numeric = 6;
		break;
	case OV_VT_SINGLE:
		varType.identifier.numeric = 10;
		break;
	case OV_VT_STATE:
		varType.identifier.numeric = 19;
		break;
	case OV_VT_STRING:
		varType.identifier.numeric = 12;
		break;
	case OV_VT_TIME:
	case OV_VT_TIME_SPAN:
		varType.identifier.numeric = 13;
		break;
	case OV_VT_UINT:
		varType.identifier.numeric = 7;
		break;
	default:
		varType.identifier.numeric = 22;
		break;
	}
	return varType;
}

OV_DLLFNCEXPORT UA_StatusCode opcua_helpers_ovAnyToUAVariant(const OV_ANY* pAny, UA_Variant* pVariant){
	UA_StatusCode result = UA_STATUSCODE_GOOD;
	const void *value = NULL;
	UA_Boolean tempBool;	/*	has different byte size in ov and ua hence we need a temp variable	*/
	UA_String tempString = {.length = 0, .data = NULL};
	UA_DateTime tempTime;
	UA_Double duration;
	UA_Int32	arrayLength;
	UA_Boolean* tempBoolArray = NULL;
	UA_String*	tempStringArray = NULL;
	UA_DateTime*	tempTimeArray = NULL;
	UA_Double*	tempDoubleArray	=	NULL;
	UA_Int32	iterator;

	if(!((pAny->value.vartype & OV_VT_KSMASK) & OV_VT_ISVECTOR)){
		UA_Variant_init(pVariant);
		switch(pAny->value.vartype & OV_VT_KSMASK){
		/*	scalar variable	*/
		case OV_VT_BOOL:
		case OV_VT_BOOL_PV:
			if(pAny->value.valueunion.val_bool == TRUE){
				tempBool = UA_TRUE;
			} else {
				tempBool = UA_FALSE;
			}
			value = &tempBool;
			pVariant->type = &UA_TYPES[UA_TYPES_BOOLEAN];
			pVariant->data = UA_Boolean_new();
			if(!pVariant->data){
				result = UA_STATUSCODE_BADOUTOFMEMORY;
				UA_Variant_clear(pVariant);
				return result;
			}
			UA_Boolean_copy(&tempBool, pVariant->data);
			break;
		case OV_VT_BYTE:
			value = &(pAny->value.valueunion.val_byte);
			pVariant->type = &UA_TYPES[UA_TYPES_BYTE];
			pVariant->data = UA_Byte_new();
			if(!pVariant->data){
				result = UA_STATUSCODE_BADOUTOFMEMORY;
				UA_Variant_clear(pVariant);
				return result;
			}
			UA_Byte_copy(value, pVariant->data);
			break;
		case OV_VT_DOUBLE:
		case OV_VT_DOUBLE_PV:
			value = &(pAny->value.valueunion.val_double);
			pVariant->type = &UA_TYPES[UA_TYPES_DOUBLE];
			pVariant->data = UA_Double_new();
			if(!pVariant->data){
				result = UA_STATUSCODE_BADOUTOFMEMORY;
				UA_Variant_clear(pVariant);
				return result;
			}
			UA_Double_copy(value, pVariant->data);
			break;
		case OV_VT_INT:
		case OV_VT_INT_PV:
			value = &(pAny->value.valueunion.val_int);
			pVariant->type = &UA_TYPES[UA_TYPES_INT32];
			pVariant->data = UA_Int32_new();
			if(!pVariant->data){
				result = UA_STATUSCODE_BADOUTOFMEMORY;
				UA_Variant_clear(pVariant);
				return result;
			}
			UA_Int32_copy(value, pVariant->data);
			break;
		case OV_VT_STATE:
			value = &(pAny->value.valueunion.val_state);
			pVariant->type = &UA_TYPES[UA_TYPES_INT32];
			pVariant->data = UA_Int32_new();
			if(!pVariant->data){
				result = UA_STATUSCODE_BADOUTOFMEMORY;
				UA_Variant_clear(pVariant);
				return result;
			}
			UA_Int32_copy(value, pVariant->data);
			break;
		case OV_VT_SINGLE:
		case OV_VT_SINGLE_PV:
			value = &(pAny->value.valueunion.val_single);
			pVariant->type = &UA_TYPES[UA_TYPES_FLOAT];
			pVariant->data = UA_Float_new();
			if(!pVariant->data){
				result = UA_STATUSCODE_BADOUTOFMEMORY;
				UA_Variant_clear(pVariant);
				return result;
			}
			UA_Float_copy(value, pVariant->data);
			break;
		case OV_VT_STRING:
		case OV_VT_STRING_PV:
			tempString.data = (UA_Byte*) pAny->value.valueunion.val_string;
			if(pAny->value.valueunion.val_string){
				tempString.length = strlen(pAny->value.valueunion.val_string);
			} else {
				tempString.length = 0;
			}
			pVariant->type = &UA_TYPES[UA_TYPES_STRING];
			pVariant->data = UA_String_new();
			if(!pVariant->data){
				result = UA_STATUSCODE_BADOUTOFMEMORY;
				UA_Variant_clear(pVariant);
				return result;
			}
			UA_String_copy(&tempString, pVariant->data);
			break;
		case OV_VT_TIME:
		case OV_VT_TIME_PV:
			tempTime = ov_ovTimeTo1601nsTime(pAny->value.valueunion.val_time);
			pVariant->type = &UA_TYPES[UA_TYPES_DATETIME];
			pVariant->data = UA_DateTime_new();
			if(!pVariant->data){
				result = UA_STATUSCODE_BADOUTOFMEMORY;
				UA_Variant_clear(pVariant);
				return result;
			}
			UA_DateTime_copy(&tempTime, pVariant->data);
			break;
		case OV_VT_TIME_SPAN:
		case OV_VT_TIME_SPAN_PV:
			Ov_TimeSpanToDouble(pAny->value.valueunion.val_time_span, duration);
			duration *= 1000.0;	/*	duration uses ms	*/
			pVariant->type = &UA_TYPES[UA_TYPES_DOUBLE];
			pVariant->data = UA_Double_new();
			if(!pVariant->data){
				result = UA_STATUSCODE_BADOUTOFMEMORY;
				UA_Variant_clear(pVariant);
				return result;
			}
			UA_Double_copy(&duration, pVariant->data);
			break;
		case OV_VT_UINT:
		case OV_VT_UINT_PV:
			value = &(pAny->value.valueunion.val_uint);
			pVariant->type = &UA_TYPES[UA_TYPES_UINT32];
			pVariant->data = UA_UInt32_new();
			if(!pVariant->data){
				result = UA_STATUSCODE_BADOUTOFMEMORY;
				UA_Variant_clear(pVariant);
				return result;
			}
			UA_UInt32_copy(value, pVariant->data);
			break;
		default:
			UA_Variant_clear(pVariant);
			return opcua_helpers_ovResultToUaStatusCode(OV_ERR_BADTYPE);
		}
		return result;
	} else {
		/*	handle vectors	*/
		ov_memstack_lock();
		UA_Variant_init(pVariant);
		switch(pAny->value.vartype & OV_VT_KSMASK){
		case OV_VT_BOOL_VEC:
		case OV_VT_BOOL_PV_VEC:
			arrayLength = pAny->value.valueunion.val_bool_vec.veclen;
			tempBoolArray = ov_memstack_alloc(arrayLength * sizeof(UA_Boolean));
			if(!tempBoolArray){
				ov_memstack_unlock();
				UA_Variant_clear(pVariant);
				return opcua_helpers_ovResultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
			}
			for(iterator = 0; iterator < arrayLength; iterator++){
				if(pAny->value.valueunion.val_bool_vec.value[iterator] == TRUE){
					tempBoolArray[iterator] = UA_TRUE;
				} else {
					tempBoolArray[iterator] = UA_FALSE;
				}
			}
			value = tempBoolArray;
			pVariant->type = &UA_TYPES[UA_TYPES_BOOLEAN];
			if(!pVariant->data){
				ov_memstack_unlock();
				result = UA_STATUSCODE_BADOUTOFMEMORY;
				UA_Variant_clear(pVariant);
				return result;
			}
			result = UA_Array_copy(value, arrayLength, &(pVariant->data), &UA_TYPES[UA_TYPES_BOOLEAN]);
			if(result != UA_STATUSCODE_GOOD){
				ov_memstack_unlock();
				UA_Variant_clear(pVariant);
				return result;
			}
			pVariant->arrayLength = arrayLength;
			break;
		case OV_VT_BYTE_VEC:
			arrayLength = pAny->value.valueunion.val_byte_vec.veclen;
			value = pAny->value.valueunion.val_byte_vec.value;
			pVariant->type = &UA_TYPES[UA_TYPES_BYTE];
			result = UA_Array_copy(value, arrayLength, &(pVariant->data), &UA_TYPES[UA_TYPES_BYTE]);
			if(result != UA_STATUSCODE_GOOD){
				ov_memstack_unlock();
				UA_Variant_clear(pVariant);
				return result;
			}
			pVariant->arrayLength = arrayLength;
			break;
		case OV_VT_DOUBLE_VEC:
		case OV_VT_DOUBLE_PV_VEC:
			arrayLength = pAny->value.valueunion.val_double_vec.veclen;
			value = pAny->value.valueunion.val_double_vec.value;
			pVariant->type = &UA_TYPES[UA_TYPES_DOUBLE];
			result = UA_Array_copy(value, arrayLength, &(pVariant->data), &UA_TYPES[UA_TYPES_DOUBLE]);
			if(result != UA_STATUSCODE_GOOD){
				ov_memstack_unlock();
				UA_Variant_clear(pVariant);
				return result;
			}
			pVariant->arrayLength = arrayLength;
			break;
		case OV_VT_INT_VEC:
		case OV_VT_INT_PV_VEC:
			arrayLength = pAny->value.valueunion.val_int_vec.veclen;
			value = pAny->value.valueunion.val_int_vec.value;
			pVariant->type = &UA_TYPES[UA_TYPES_INT32];
			result = UA_Array_copy(value, arrayLength, &(pVariant->data), &UA_TYPES[UA_TYPES_INT32]);
			if(result != UA_STATUSCODE_GOOD){
				ov_memstack_unlock();
				UA_Variant_clear(pVariant);
				return result;
			}
			pVariant->arrayLength = arrayLength;
			break;
		case OV_VT_STATE_VEC:
			arrayLength = pAny->value.valueunion.val_state_vec.veclen;
			value = pAny->value.valueunion.val_state_vec.value;
			pVariant->type = &UA_TYPES[UA_TYPES_INT32];
			result = UA_Array_copy(value, arrayLength, &(pVariant->data), &UA_TYPES[UA_TYPES_INT32]);
			if(result != UA_STATUSCODE_GOOD){
				ov_memstack_unlock();
				UA_Variant_clear(pVariant);
				return result;
			}
			pVariant->arrayLength = arrayLength;
			break;
		case OV_VT_SINGLE_VEC:
		case OV_VT_SINGLE_PV_VEC:
			arrayLength = pAny->value.valueunion.val_single_vec.veclen;
			value = pAny->value.valueunion.val_single_vec.value;
			pVariant->type = &UA_TYPES[UA_TYPES_FLOAT];
			result = UA_Array_copy(value, arrayLength, &(pVariant->data), &UA_TYPES[UA_TYPES_FLOAT]);
			if(result != UA_STATUSCODE_GOOD){
				ov_memstack_unlock();
				UA_Variant_clear(pVariant);
				return result;
			}
			pVariant->arrayLength = arrayLength;
			break;
		case OV_VT_STRING_VEC:
		case OV_VT_STRING_PV_VEC:
			arrayLength = pAny->value.valueunion.val_string_vec.veclen;
			tempStringArray = ov_memstack_alloc(arrayLength * sizeof(UA_String));
			if(!tempStringArray){
				ov_memstack_unlock();
				UA_Variant_clear(pVariant);
				return opcua_helpers_ovResultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
			}
			for(iterator = 0; iterator < arrayLength; iterator++){
				if(!pAny->value.valueunion.val_string_vec.value[iterator]){
					tempStringArray[iterator].length = 0;
				} else {
					tempStringArray[iterator].length = ov_string_getlength(pAny->value.valueunion.val_string_vec.value[iterator]);
					tempStringArray[iterator].data = (UA_Byte*)pAny->value.valueunion.val_string_vec.value[iterator];
				}
			}
			value = tempStringArray;
			pVariant->type = &UA_TYPES[UA_TYPES_STRING];
			result = UA_Array_copy(value, arrayLength, &(pVariant->data), &UA_TYPES[UA_TYPES_STRING]);
			if(result != UA_STATUSCODE_GOOD){
				ov_memstack_unlock();
				UA_Variant_clear(pVariant);
				return result;
			}
			pVariant->arrayLength = arrayLength;
			break;
		case OV_VT_TIME_VEC:
		case OV_VT_TIME_PV_VEC:
			arrayLength = pAny->value.valueunion.val_time_vec.veclen;
			tempTimeArray = ov_memstack_alloc(arrayLength * sizeof(UA_DateTime));
			if(!tempTimeArray){
				ov_memstack_unlock();
				UA_Variant_clear(pVariant);
				return opcua_helpers_ovResultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
			}
			for(iterator = 0; iterator < arrayLength; iterator++){
				tempTimeArray[iterator] = ov_ovTimeTo1601nsTime(pAny->value.valueunion.val_time_vec.value[iterator]);
			}
			value = tempTimeArray;
			pVariant->type = &UA_TYPES[UA_TYPES_DATETIME];
			result = UA_Array_copy(value, arrayLength, &(pVariant->data), &UA_TYPES[UA_TYPES_DATETIME]);
			if(result != UA_STATUSCODE_GOOD){
				ov_memstack_unlock();
				UA_Variant_clear(pVariant);
				return result;
			}
			pVariant->arrayLength = arrayLength;
			break;
		case OV_VT_TIME_SPAN_VEC:
		case OV_VT_TIME_SPAN_PV_VEC:
			arrayLength = pAny->value.valueunion.val_time_span_vec.veclen;
			tempDoubleArray = ov_memstack_alloc(arrayLength * sizeof(UA_Double));
			if(!tempDoubleArray){
				ov_memstack_unlock();
				UA_Variant_clear(pVariant);
				return opcua_helpers_ovResultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
			}
			for(iterator = 0; iterator < arrayLength; iterator++){
				Ov_TimeSpanToDouble(pAny->value.valueunion.val_time_span_vec.value[iterator], tempDoubleArray[iterator]);
				tempDoubleArray[iterator] *= 1000.0;
			}
			value = tempTimeArray;
			pVariant->type = &UA_TYPES[UA_TYPES_DOUBLE];
			result = UA_Array_copy(value, arrayLength, &(pVariant->data), &UA_TYPES[UA_TYPES_DOUBLE]);
			if(result != UA_STATUSCODE_GOOD){
				ov_memstack_unlock();
				UA_Variant_clear(pVariant);
				return result;
			}
			pVariant->arrayLength = arrayLength;
			break;
		case OV_VT_UINT_VEC:
		case OV_VT_UINT_PV_VEC:
			arrayLength = pAny->value.valueunion.val_uint_vec.veclen;
			value = pAny->value.valueunion.val_uint_vec.value;
			pVariant->type = &UA_TYPES[UA_TYPES_UINT32];
			result = UA_Array_copy(value, arrayLength, &(pVariant->data), &UA_TYPES[UA_TYPES_UINT32]);
			if(result != UA_STATUSCODE_GOOD){
				ov_memstack_unlock();
				UA_Variant_clear(pVariant);
				return result;
			}
			pVariant->arrayLength = arrayLength;
			break;
		default:
			ov_memstack_unlock();
			UA_Variant_clear(pVariant);
			return opcua_helpers_ovResultToUaStatusCode(OV_ERR_BADTYPE);
		}
		ov_memstack_unlock();
		return result;
	}
}

OV_DLLFNCEXPORT UA_StatusCode opcua_helpers_UAVariantToOVAny(const UA_Variant* pVariant, OV_ANY* pAny){
	OV_UINT iterator = 0;
	if(pVariant->arrayLength == 0 && pVariant->data > UA_EMPTY_ARRAY_SENTINEL){
		/*	scalar values	*/
		if(pVariant->type == &UA_TYPES[UA_TYPES_BOOLEAN]){
			pAny->value.vartype = OV_VT_BOOL;
			if((*(UA_Boolean*)pVariant->data) == UA_TRUE){
				pAny->value.valueunion.val_bool = TRUE;
			} else {
				pAny->value.valueunion.val_bool = FALSE;
			}
		} else if(pVariant->type == &UA_TYPES[UA_TYPES_INT32]){
			pAny->value.vartype = OV_VT_INT;
			pAny->value.valueunion.val_int =  *(UA_Int32*)pVariant->data;
		} else if(pVariant->type == &UA_TYPES[UA_TYPES_INT16]){
			pAny->value.vartype = OV_VT_INT;
			pAny->value.valueunion.val_int =  *(UA_Int16*)pVariant->data;
		} else if(pVariant->type == &UA_TYPES[UA_TYPES_UINT32]){
			pAny->value.vartype = OV_VT_UINT;
			pAny->value.valueunion.val_uint =  *(UA_UInt32*)pVariant->data;
		} else if(pVariant->type == &UA_TYPES[UA_TYPES_UINT16]){
			pAny->value.vartype = OV_VT_UINT;
			pAny->value.valueunion.val_uint =  *(UA_UInt16*)pVariant->data;
		} else if(pVariant->type == &UA_TYPES[UA_TYPES_FLOAT]){
			pAny->value.vartype = OV_VT_SINGLE;
			pAny->value.valueunion.val_single =  *(UA_Float*)pVariant->data;
		} else if(pVariant->type == &UA_TYPES[UA_TYPES_DOUBLE]){
			pAny->value.vartype = OV_VT_DOUBLE;
			pAny->value.valueunion.val_double =  *(UA_Double*)pVariant->data;
		} else if(pVariant->type == &UA_TYPES[UA_TYPES_STRING]){
			pAny->value.vartype = OV_VT_STRING;
			if(!(UA_String*)pVariant->data || ((UA_String*)pVariant->data)->length <= 0){
				pAny->value.valueunion.val_string =  NULL;
			} else {
				pAny->value.valueunion.val_string =  ov_database_malloc(((UA_String*)pVariant->data)->length + 1);
				if(!pAny->value.valueunion.val_string){
					return UA_STATUSCODE_BADOUTOFMEMORY;
				}
				for(iterator = 0; iterator < (OV_UINT)((UA_String*)pVariant->data)->length; iterator++){
					pAny->value.valueunion.val_string[iterator] = ((UA_String*)pVariant->data)->data[iterator];
				}
				pAny->value.valueunion.val_string[iterator] = '\0';
			}
		} else if(pVariant->type == &UA_TYPES[UA_TYPES_DATETIME]){
			pAny->value.vartype = OV_VT_TIME;
			pAny->value.valueunion.val_time =  ov_1601nsTimeToOvTime(*(UA_DateTime*)pVariant->data);
		}else if(pVariant->type == &UA_TYPES[UA_TYPES_STATUSCODE]){
			pAny->value.vartype = OV_VT_STATE;
			pAny->value.valueunion.val_int =  *(UA_Int32*)pVariant->data;
		}else {
			return UA_STATUSCODE_BADDATATYPEIDUNKNOWN;
		}
	} else {
		/*	vectors	*/
		if(pVariant->type == &UA_TYPES[UA_TYPES_BOOLEAN]){
			pAny->value.vartype = OV_VT_BOOL_VEC;
			if(pVariant->arrayLength > 0){
				pAny->value.valueunion.val_bool_vec.veclen = pVariant->arrayLength;
				pAny->value.valueunion.val_bool_vec.value = ov_database_malloc(pVariant->arrayLength * sizeof(OV_BOOL));
				if(!pAny->value.valueunion.val_bool_vec.value){
					return UA_STATUSCODE_BADOUTOFMEMORY;
				}
				for(iterator = 0; iterator < (OV_UINT)pVariant->arrayLength; iterator++){
					if(((UA_Boolean*)pVariant->data)[iterator] == UA_TRUE){
						pAny->value.valueunion.val_bool_vec.value[iterator] = TRUE;
					} else {
						pAny->value.valueunion.val_bool_vec.value[iterator] = FALSE;
					}
				}
			} else {
				pAny->value.valueunion.val_bool_vec.veclen = 0;
				pAny->value.valueunion.val_bool_vec.value = NULL;
			}
		} else if(pVariant->type == &UA_TYPES[UA_TYPES_INT32]){
			pAny->value.vartype = OV_VT_INT_VEC;
			if(pVariant->arrayLength > 0){
				pAny->value.valueunion.val_int_vec.veclen = pVariant->arrayLength;
				pAny->value.valueunion.val_int_vec.value = ov_database_malloc(pVariant->arrayLength * sizeof(OV_INT));
				if(!pAny->value.valueunion.val_int_vec.value){
					return UA_STATUSCODE_BADOUTOFMEMORY;
				}
				for(iterator = 0; iterator < (OV_UINT)pVariant->arrayLength; iterator++){
					pAny->value.valueunion.val_int_vec.value[iterator] = ((UA_Int32*)pVariant->data)[iterator];
				}
			} else {
				pAny->value.valueunion.val_int_vec.veclen = 0;
				pAny->value.valueunion.val_int_vec.value = NULL;
			}
		} else if(pVariant->type == &UA_TYPES[UA_TYPES_INT16]){
			pAny->value.vartype = OV_VT_INT_VEC;
			if(pVariant->arrayLength > 0){
				pAny->value.valueunion.val_int_vec.veclen = pVariant->arrayLength;
				pAny->value.valueunion.val_int_vec.value = ov_database_malloc(pVariant->arrayLength * sizeof(OV_INT));
				if(!pAny->value.valueunion.val_int_vec.value){
					return UA_STATUSCODE_BADOUTOFMEMORY;
				}
				for(iterator = 0; iterator < (OV_UINT)pVariant->arrayLength; iterator++){
					pAny->value.valueunion.val_int_vec.value[iterator] = ((UA_Int16*)pVariant->data)[iterator];
				}
			} else {
				pAny->value.valueunion.val_int_vec.veclen = 0;
				pAny->value.valueunion.val_int_vec.value = NULL;
			}
		} else if(pVariant->type == &UA_TYPES[UA_TYPES_UINT32]){
			pAny->value.vartype = OV_VT_UINT_VEC;
			if(pVariant->arrayLength > 0){
				pAny->value.valueunion.val_uint_vec.veclen = pVariant->arrayLength;
				pAny->value.valueunion.val_uint_vec.value = ov_database_malloc(pVariant->arrayLength * sizeof(OV_UINT));
				if(!pAny->value.valueunion.val_uint_vec.value){
					return UA_STATUSCODE_BADOUTOFMEMORY;
				}
				for(iterator = 0; iterator < (OV_UINT)pVariant->arrayLength; iterator++){
					pAny->value.valueunion.val_uint_vec.value[iterator] = ((UA_UInt32*)pVariant->data)[iterator];
				}
			} else {
				pAny->value.valueunion.val_uint_vec.veclen = 0;
				pAny->value.valueunion.val_uint_vec.value = NULL;
			}
		} else if(pVariant->type == &UA_TYPES[UA_TYPES_UINT16]){
			pAny->value.vartype = OV_VT_UINT_VEC;
			if(pVariant->arrayLength > 0){
				pAny->value.valueunion.val_uint_vec.veclen = pVariant->arrayLength;
				pAny->value.valueunion.val_uint_vec.value = ov_database_malloc(pVariant->arrayLength * sizeof(OV_UINT));
				if(!pAny->value.valueunion.val_uint_vec.value){
					return UA_STATUSCODE_BADOUTOFMEMORY;
				}
				for(iterator = 0; iterator < (OV_UINT)pVariant->arrayLength; iterator++){
					pAny->value.valueunion.val_uint_vec.value[iterator] = ((UA_UInt16*)pVariant->data)[iterator];
				}
			} else {
				pAny->value.valueunion.val_uint_vec.veclen = 0;
				pAny->value.valueunion.val_uint_vec.value = NULL;
			}
		} else if(pVariant->type == &UA_TYPES[UA_TYPES_FLOAT]){
			pAny->value.vartype = OV_VT_SINGLE_VEC;
			if(pVariant->arrayLength > 0){
				pAny->value.valueunion.val_single_vec.veclen = pVariant->arrayLength;
				pAny->value.valueunion.val_single_vec.value = ov_database_malloc(pVariant->arrayLength * sizeof(OV_SINGLE));
				if(!pAny->value.valueunion.val_single_vec.value){
					return UA_STATUSCODE_BADOUTOFMEMORY;
				}
				for(iterator = 0; iterator < (OV_UINT)pVariant->arrayLength; iterator++){
					pAny->value.valueunion.val_single_vec.value[iterator] = ((UA_Float*)pVariant->data)[iterator];
				}
			} else {
				pAny->value.valueunion.val_single_vec.veclen = 0;
				pAny->value.valueunion.val_single_vec.value = NULL;
			}
		} else if(pVariant->type == &UA_TYPES[UA_TYPES_DOUBLE]){
			pAny->value.vartype = OV_VT_DOUBLE_VEC;
			if(pVariant->arrayLength > 0){
				pAny->value.valueunion.val_double_vec.veclen = pVariant->arrayLength;
				pAny->value.valueunion.val_double_vec.value = ov_database_malloc(pVariant->arrayLength * sizeof(OV_DOUBLE));
				if(!pAny->value.valueunion.val_double_vec.value){
					return UA_STATUSCODE_BADOUTOFMEMORY;
				}
				for(iterator = 0; iterator < (OV_UINT)pVariant->arrayLength; iterator++){
					pAny->value.valueunion.val_double_vec.value[iterator] = ((UA_Double*)pVariant->data)[iterator];
				}
			} else {
				pAny->value.valueunion.val_double_vec.veclen = 0;
				pAny->value.valueunion.val_double_vec.value = NULL;
			}
		} else if(pVariant->type == &UA_TYPES[UA_TYPES_STRING]){
			pAny->value.vartype = OV_VT_STRING_VEC;
			if(pVariant->arrayLength > 0){
				pAny->value.valueunion.val_string_vec.veclen = pVariant->arrayLength;
				pAny->value.valueunion.val_string_vec.value = ov_database_malloc(pVariant->arrayLength * sizeof(OV_STRING));
				if(!pAny->value.valueunion.val_string_vec.value){
					return UA_STATUSCODE_BADOUTOFMEMORY;
				}
				for(iterator = 0; iterator < (OV_UINT)pVariant->arrayLength; iterator++){
					if((((UA_String*)pVariant->data)[iterator]).length <= 0){
						pAny->value.valueunion.val_string_vec.value[iterator] =  NULL;
					} else {
						OV_UINT iTwo = 0;
						pAny->value.valueunion.val_string_vec.value[iterator] =  ov_database_malloc(((UA_String*)pVariant->data)[iterator].length + 1);
						if(!pAny->value.valueunion.val_string_vec.value[iterator]){
							return UA_STATUSCODE_BADOUTOFMEMORY;
						}
						for(iTwo = 0; iTwo < (OV_UINT)((UA_String*)pVariant->data)[iterator].length; iTwo++){
							(pAny->value.valueunion.val_string_vec.value[iterator])[iTwo] = ((UA_String*)pVariant->data)[iterator].data[iTwo];
						}
						(pAny->value.valueunion.val_string_vec.value[iterator])[iTwo] = '\0';
					}
				}
			} else {
				pAny->value.valueunion.val_int_vec.veclen = 0;
				pAny->value.valueunion.val_int_vec.value = NULL;
			}
		} else if(pVariant->type == &UA_TYPES[UA_TYPES_DATETIME]){
			pAny->value.vartype = OV_VT_TIME_VEC;
			if(pVariant->arrayLength > 0){
				pAny->value.valueunion.val_time_vec.veclen = pVariant->arrayLength;
				pAny->value.valueunion.val_time_vec.value = ov_database_malloc(pVariant->arrayLength * sizeof(OV_TIME));
				if(!pAny->value.valueunion.val_time_vec.value){
					return UA_STATUSCODE_BADOUTOFMEMORY;
				}
				for(iterator = 0; iterator < (OV_UINT)pVariant->arrayLength; iterator++){
					pAny->value.valueunion.val_time_vec.value[iterator] = ov_1601nsTimeToOvTime(((UA_Int32*)pVariant->data)[iterator]);
				}
			} else {
				pAny->value.valueunion.val_int_vec.veclen = 0;
				pAny->value.valueunion.val_int_vec.value = NULL;
			}
		} else if(pVariant->type == &UA_TYPES[UA_TYPES_STATUSCODE]){
			pAny->value.vartype = OV_VT_STATE_VEC;
			if(pVariant->arrayLength > 0){
				pAny->value.valueunion.val_state_vec.veclen = pVariant->arrayLength;
				pAny->value.valueunion.val_state_vec.value = ov_database_malloc(pVariant->arrayLength * sizeof(OV_STATE));
				if(!pAny->value.valueunion.val_state_vec.value){
					return UA_STATUSCODE_BADOUTOFMEMORY;
				}
				for(iterator = 0; iterator < (OV_UINT)pVariant->arrayLength; iterator++){
					pAny->value.valueunion.val_state_vec.value[iterator] = ((UA_StatusCode*)pVariant->data)[iterator];
				}
			} else {
				pAny->value.valueunion.val_state_vec.veclen = 0;
				pAny->value.valueunion.val_state_vec.value = NULL;
			}
		} else {
			return UA_STATUSCODE_BADDATATYPEIDUNKNOWN;
		}

	}
	return UA_STATUSCODE_GOOD;
}

//TODO use memstack instead
OV_DLLFNCEXPORT OV_RESULT opcua_helpers_copyUAStringToOV(UA_String src, OV_STRING *dst) {
	if(src.data == NULL)
		return OV_ERR_OK;
	*dst = ov_database_malloc(sizeof(char)*(src.length+1));
	if (*dst == NULL)
		return OV_ERR_HEAPOUTOFMEMORY;
	memcpy(*dst, src.data, src.length);
	*(*dst + src.length) = '\0';
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT UA_StatusCode opcua_helpers_resolveNodeIdToPath(const UA_NodeId nodeId, OV_PATH* pPath){
	OV_STRING path = NULL;
	OV_RESULT result;
	switch(nodeId.identifierType){
	case UA_NODEIDTYPE_STRING:
		result = opcua_helpers_copyUAStringToOV(nodeId.identifier.string, &path);
		if(Ov_Fail(result)){
			return opcua_helpers_ovResultToUaStatusCode(result);
		}
		break;
	case UA_NODEIDTYPE_NUMERIC:
		ov_string_print(&path, "/.%u", nodeId.identifier.numeric);
		break;
	default:
		return UA_STATUSCODE_BADNODEIDREJECTED;
	}
	result = ov_path_resolve(pPath,NULL,path, 2);
	ov_string_setvalue(&path, NULL);
	if(Ov_Fail(result)){
		return opcua_helpers_ovResultToUaStatusCode(result);
	}
	return UA_STATUSCODE_GOOD;
}

OV_DLLFNCEXPORT OV_INSTPTR_ov_object opcua_helpers_resolveNodeIdToOvObject(const UA_NodeId *nodeId){
	OV_STRING path = NULL;
	OV_INSTPTR_ov_object ptr = NULL;
	switch(nodeId->identifierType){
	case UA_NODEIDTYPE_STRING:
		opcua_helpers_copyUAStringToOV(nodeId->identifier.string, &path);
		ptr = ov_path_getobjectpointer(path, 2);
		ov_string_setvalue(&path, NULL);
		break;
	case UA_NODEIDTYPE_NUMERIC:
		ov_path_getObjectById(0, nodeId->identifier.numeric, &ptr);
		break;
	default:
		break;
	}
	return ptr;
}


OV_DLLFNCEXPORT UA_StatusCode opcua_helpers_getVtblPointerAndCheckAccess(OV_ELEMENT *pelem, OV_INSTPTR_ov_object *pInstance, OV_VTBLPTR_ov_object *ppVtblObj, OV_ACCESS *access){
	switch(pelem->elemtype){
	case OV_ET_OBJECT:
	case OV_ET_OPERATION:
		*pInstance = pelem->pobj;
		break;
	case OV_ET_MEMBER:
	case OV_ET_VARIABLE:
		*pInstance = pelem->elemunion.pobj;
		break;
	case OV_ET_CHILDLINK:
	case OV_ET_PARENTLINK:
		*pInstance = Ov_StaticPtrCast(ov_object, pelem->elemunion.passoc);
		break;
	default:
		*pInstance = pelem->pobj;
		break;
	}
	Ov_GetVTablePtr(ov_object, *ppVtblObj, *pInstance);
	if((!*ppVtblObj) || (ov_activitylock)){
		*ppVtblObj = pclass_ov_object->v_pvtable;
	}
	if(pelem->elemtype == OV_ET_VARIABLE) //FIXME Allow read access for all variables, even if they don't have get/set accesors
		*access = OV_AC_READWRITE;
	else
		*access = (*ppVtblObj)->m_getaccess(pelem->pobj, pelem, NULL);
	return UA_STATUSCODE_GOOD;
}

OV_DLLFNCEXPORT OV_ACCESS opcua_helpers_getAccess(const OV_ELEMENT* pElem){
	OV_VTBLPTR_ov_object	pVtbl	=	NULL;
	if(!pElem->pobj){
		return OV_AC_NONE;
	} else {
		Ov_GetVTablePtr(ov_object, pVtbl, pElem->pobj);
		if(pVtbl){
			return pVtbl->m_getaccess(pElem->pobj, pElem, NULL);
		} else {
			return OV_AC_NONE;
		}
	}
	return OV_AC_NONE;
}

OV_DLLFNCEXPORT UA_NodeClass opcua_helpers_getNodeClass(const OV_ELEMENT* pElem){
	if(pElem->elemtype == OV_ET_OBJECT){
		//	check further since all definitions are objects themselves
		if(!pElem->pobj){
			return UA_NODECLASS_UNSPECIFIED;
		} else if(Ov_GetParent(ov_instantiation, pElem->pobj) == pclass_ov_class){
			return UA_NODECLASS_OBJECTTYPE;
		} else if(Ov_GetParent(ov_instantiation, pElem->pobj) == pclass_ov_variable){
			return UA_NODECLASS_VARIABLETYPE;
		} else if(Ov_GetParent(ov_instantiation, pElem->pobj) == pclass_ov_association){
			return UA_NODECLASS_REFERENCETYPE;
		} else {
				return UA_NODECLASS_OBJECT;
		}
	} else if(pElem->elemtype == OV_ET_VARIABLE || pElem->elemtype == OV_ET_MEMBER) {
		return UA_NODECLASS_VARIABLE;
	} else {
		return UA_NODECLASS_UNSPECIFIED;
	}
}

OV_DLLFNCEXPORT OV_RESULT
opcua_helpers_setRootEntryReference(const OV_STRING newPath, OV_INSTPTR_opcua_interface pobj, OV_STRING * poldPath){
	// Check new path for NULL or zero length
	OV_UINT length = ov_string_getlength(newPath);
	if(length == 0)
		return OV_ERR_BADNAME;

	// Check newPath for valid characters
	OV_STRING *plist = NULL;
	OV_UINT    i,len;
	plist = ov_string_split(newPath,"/",&len);
	for(i = 0 ; i < len ; i++) {
		length = ov_string_getlength(plist[i]);
		for(OV_UINT j = 0 ; j < length ; j++){
			if(!ov_path_isvalidchar(plist[i][j]))
				return OV_ERR_BADNAME;
		}
	}
	ov_string_freelist(plist);

	OV_INSTPTR_opcua_server server = Ov_GetParent(opcua_serverToInterfaces, pobj);
	if(server != NULL && server->v_isRunning){
		UA_StatusCode retval = UA_STATUSCODE_GOOD;
		//Delete old entry reference
		retval = UA_Server_deleteReference(server->v_server, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
				UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_TRUE, UA_EXPANDEDNODEID_STRING(OV_OPCUA_DEFAULTNSINDEX, *poldPath), UA_FALSE);
		if(retval != UA_STATUSCODE_GOOD){
			//Ov_Warning(UA_StatusCode_name(retval));
		}

		//Add reference to new specified entry object
		retval = UA_Server_addReference(server->v_server, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
				UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_EXPANDEDNODEID_STRING(OV_OPCUA_DEFAULTNSINDEX, newPath), true);
		if(retval != UA_STATUSCODE_GOOD){
			//Ov_Warning(UA_StatusCode_name(retval));
		}
	}

	if(poldPath)
		return ov_string_setvalue(poldPath, newPath);
	else
		return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT
opcua_helpers_addReferencesToTrafo(UA_Server* pUaServer, OV_STRING entryPath){
	//Add reference to OV root for generic interface
	UA_StatusCode retval = UA_STATUSCODE_GOOD;
	retval |= UA_Server_addReference(pUaServer, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
			UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_EXPANDEDNODEID_STRING(OV_OPCUA_DEFAULTNSINDEX, entryPath), UA_TRUE);
	//Add reference to ov domain
	retval |= UA_Server_addReference(pUaServer, UA_NODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE),
			UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE), UA_EXPANDEDNODEID_STRING(OV_OPCUA_DEFAULTNSINDEX, "/acplt/ov/domain"), UA_TRUE);
	//Add reference to ov object
	retval |= UA_Server_addReference(pUaServer, UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
			UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE), UA_EXPANDEDNODEID_STRING(OV_OPCUA_DEFAULTNSINDEX, "/acplt/ov/object"), UA_TRUE);
	return retval == UA_STATUSCODE_GOOD ? OV_ERR_OK : OV_ERR_GENERIC;
}

//Make sure that string node ids are allocated!
OV_DLLFNCEXPORT UA_StatusCode
opcua_helpers_addReference(UA_Node* node, UA_Byte refTypeIndex,
		const UA_ExpandedNodeId targetNodeId, OV_STRING targetBrowseName, UA_Boolean isForward){

	UA_QualifiedName targetBrowseNameQualified = UA_QUALIFIEDNAME(targetNodeId.nodeId.namespaceIndex, targetBrowseName);
	return UA_Node_addReference(node, refTypeIndex, isForward, &targetNodeId, UA_QualifiedName_hash(&targetBrowseNameQualified));	
}

// Needed for setting namespace 1
UA_StatusCode opcua_interface_setNamespace(UA_Server* server, const UA_String uriOld, const UA_String uriNew, size_t * indexOut){
	size_t index = 0;
	// Get the namespace index of the old uri
	UA_StatusCode result = UA_STATUSCODE_GOOD;
	result = UA_Server_getNamespaceByName(server, uriOld, &index);
	if(result == UA_STATUSCODE_BADNOTFOUND){
		return result;
	}
	if(indexOut)
		*indexOut = index;

	// Read namespace array from server
	UA_Variant namespaceArray;
	result = UA_Server_readValue(server, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_NAMESPACEARRAY), &namespaceArray);
	if(result != UA_STATUSCODE_GOOD){
		return result;
	}

	// Replace namespace URI
	UA_String_clear(&(((UA_String*)namespaceArray.data)[index]));
	if(uriNew.data == NULL)
		UA_String_init(&(((UA_String*)namespaceArray.data)[index]));
	else{
		result = UA_String_copy(&uriNew, &(((UA_String*)namespaceArray.data)[index]));
		if(result != UA_STATUSCODE_GOOD){
			return result;
		}
	}

	// Write back new namespace array
	result = UA_Server_writeValue(server, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_NAMESPACEARRAY), namespaceArray);
	if(result != UA_STATUSCODE_GOOD){
		return result;
	}

	// Free resources
	UA_Variant_clear(&namespaceArray);
	return result;
}

OV_DLLFNCEXPORT
UA_StatusCode opcua_helpers_getNumericalNodeIdForInputOutputArgs(UA_Server *server,
		UA_NodeId methodId, UA_NodeId* inArgsId, UA_NodeId* outArgsId) {
	UA_BrowseDescription bD;
	UA_BrowseDescription_init(&bD);
	bD.browseDirection = UA_BROWSEDIRECTION_FORWARD;
	bD.nodeId = methodId;
	bD.referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY);
	bD.includeSubtypes = false;
	UA_Boolean found1 = false;
	UA_Boolean found2 = false;

	UA_BrowseResult bR = UA_Server_browse(server, 2, &bD);
	for (size_t i = 0; i < bR.referencesSize; i++) {
		UA_QualifiedName bN;
		UA_QualifiedName_init(&bN);
		UA_Server_readBrowseName(server, bR.references[i].nodeId.nodeId, &bN);
		UA_String tmpBN = UA_String_fromChars("InputArguments");
		if (UA_String_equal(&bN.name, &tmpBN)) {
			*inArgsId = bR.references[i].nodeId.nodeId;
			found1 = true;
			//UA_NodeId_copy(&bR.references[i].nodeId.nodeId,&inArgsId);
			UA_String_clear(&tmpBN);
			UA_QualifiedName_clear(&bN);
			continue;
		}
		UA_String_clear(&tmpBN);
		tmpBN = UA_String_fromChars("OutputArguments");
		if (UA_String_equal(&bN.name, &tmpBN)) {
			*outArgsId = bR.references[i].nodeId.nodeId;
			found2 = true;
			//UA_NodeId_copy(&bR.references[i].nodeId.nodeId,&outArgsId);
			UA_String_clear(&tmpBN);
			UA_QualifiedName_clear(&bN);
			continue;
		}
		UA_String_clear(&tmpBN);
		UA_QualifiedName_clear(&bN);
	}
	UA_BrowseDescription_clear(&bD);
	UA_BrowseResult_clear(&bR);
	if (found1 && found2)
		return UA_STATUSCODE_GOOD;
	return UA_STATUSCODE_BADNOTFOUND;
}
