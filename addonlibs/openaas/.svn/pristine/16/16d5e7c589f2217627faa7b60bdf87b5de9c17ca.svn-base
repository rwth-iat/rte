/*
 * opcua_helpers.c
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


#include "opcua_helpers.h"
#include "libov/ov_string.h"
#include "libov/ov_time.h"
#include "libov/ov_path.h"
#include "libov/ov_macros.h"

#define EPOCHDIFFERENCE_SECONDS	11644473600LL
#define TOSECONDS				10000000LL
#define TOMICROSECONDS			10LL

UA_StatusCode ov_resultToUaStatusCode(OV_RESULT result){
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

UA_NodeId ov_varTypeToNodeId(OV_VAR_TYPE type){
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

UA_StatusCode ov_AnyToVariant(const OV_ANY* pAny, UA_Variant* pVariant){
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
				UA_Variant_deleteMembers(pVariant);
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
				UA_Variant_deleteMembers(pVariant);
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
				UA_Variant_deleteMembers(pVariant);
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
				UA_Variant_deleteMembers(pVariant);
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
				UA_Variant_deleteMembers(pVariant);
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
				UA_Variant_deleteMembers(pVariant);
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
				UA_Variant_deleteMembers(pVariant);
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
				UA_Variant_deleteMembers(pVariant);
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
				UA_Variant_deleteMembers(pVariant);
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
				UA_Variant_deleteMembers(pVariant);
				return result;
			}
			UA_UInt32_copy(value, pVariant->data);
			break;
		default:
			UA_Variant_deleteMembers(pVariant);
			return ov_resultToUaStatusCode(OV_ERR_BADTYPE);
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
				UA_Variant_deleteMembers(pVariant);
				return ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
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
				UA_Variant_deleteMembers(pVariant);
				return result;
			}
			result = UA_Array_copy(value, arrayLength, &(pVariant->data), &UA_TYPES[UA_TYPES_BOOLEAN]);
			if(result != UA_STATUSCODE_GOOD){
				ov_memstack_unlock();
				UA_Variant_deleteMembers(pVariant);
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
				UA_Variant_deleteMembers(pVariant);
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
				UA_Variant_deleteMembers(pVariant);
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
				UA_Variant_deleteMembers(pVariant);
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
				UA_Variant_deleteMembers(pVariant);
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
				UA_Variant_deleteMembers(pVariant);
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
				UA_Variant_deleteMembers(pVariant);
				return ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
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
				UA_Variant_deleteMembers(pVariant);
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
				UA_Variant_deleteMembers(pVariant);
				return ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
			}
			for(iterator = 0; iterator < arrayLength; iterator++){
				tempTimeArray[iterator] = ov_ovTimeTo1601nsTime(pAny->value.valueunion.val_time_vec.value[iterator]);
			}
			value = tempTimeArray;
			pVariant->type = &UA_TYPES[UA_TYPES_DATETIME];
			result = UA_Array_copy(value, arrayLength, &(pVariant->data), &UA_TYPES[UA_TYPES_DATETIME]);
			if(result != UA_STATUSCODE_GOOD){
				ov_memstack_unlock();
				UA_Variant_deleteMembers(pVariant);
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
				UA_Variant_deleteMembers(pVariant);
				return ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
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
				UA_Variant_deleteMembers(pVariant);
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
				UA_Variant_deleteMembers(pVariant);
				return result;
			}
			pVariant->arrayLength = arrayLength;
			break;
		default:
			ov_memstack_unlock();
			UA_Variant_deleteMembers(pVariant);
			return ov_resultToUaStatusCode(OV_ERR_BADTYPE);
		}
		ov_memstack_unlock();
		return result;
	}
}

UA_StatusCode ov_VariantToAny(const UA_Variant* pVariant, OV_ANY* pAny){
	OV_UINT iterator = 0;
	ov_memstack_lock();
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
				pAny->value.valueunion.val_string =  ov_memstack_alloc(((UA_String*)pVariant->data)->length + 1);
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
				pAny->value.valueunion.val_bool_vec.value = ov_memstack_alloc(pVariant->arrayLength * sizeof(OV_BOOL));
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
				pAny->value.valueunion.val_int_vec.value = ov_memstack_alloc(pVariant->arrayLength * sizeof(OV_INT));
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
				pAny->value.valueunion.val_int_vec.value = ov_memstack_alloc(pVariant->arrayLength * sizeof(OV_INT));
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
				pAny->value.valueunion.val_uint_vec.value = ov_memstack_alloc(pVariant->arrayLength * sizeof(OV_UINT));
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
				pAny->value.valueunion.val_uint_vec.value = ov_memstack_alloc(pVariant->arrayLength * sizeof(OV_UINT));
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
				pAny->value.valueunion.val_single_vec.value = ov_memstack_alloc(pVariant->arrayLength * sizeof(OV_SINGLE));
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
				pAny->value.valueunion.val_double_vec.value = ov_memstack_alloc(pVariant->arrayLength * sizeof(OV_DOUBLE));
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
				pAny->value.valueunion.val_string_vec.value = ov_memstack_alloc(pVariant->arrayLength * sizeof(OV_STRING));
				if(!pAny->value.valueunion.val_string_vec.value){
					return UA_STATUSCODE_BADOUTOFMEMORY;
				}
				for(iterator = 0; iterator < (OV_UINT)pVariant->arrayLength; iterator++){
					if((((UA_String*)pVariant->data)[iterator]).length <= 0){
						pAny->value.valueunion.val_string_vec.value[iterator] =  NULL;
					} else {
						OV_UINT iTwo = 0;
						pAny->value.valueunion.val_string_vec.value[iterator] =  ov_memstack_alloc(((UA_String*)pVariant->data)[iterator].length + 1);
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
				pAny->value.valueunion.val_time_vec.value = ov_memstack_alloc(pVariant->arrayLength * sizeof(OV_TIME));
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
				pAny->value.valueunion.val_state_vec.value = ov_memstack_alloc(pVariant->arrayLength * sizeof(OV_STATE));
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
	ov_memstack_unlock();
	return UA_STATUSCODE_GOOD;
}

OV_RESULT copyOvStringToOPCUA(OV_STRING src, UA_String* dst) {
	if(!src)
		return OV_ERR_OK;
	*dst = UA_STRING_ALLOC(src);
	return OV_ERR_OK;
}


OV_RESULT copyOPCUAStringToOV(UA_String src, OV_STRING *dst) {
	if(src.data == NULL)
		return OV_ERR_OK;
	*dst = ov_database_malloc(sizeof(char)*(src.length+1));
	if (*dst == NULL)
		return OV_ERR_HEAPOUTOFMEMORY;
	memcpy(*dst, src.data, src.length);
	*(*dst + src.length) = '\0';
	return OV_ERR_OK;
}


OV_RESULT copyOvNodeIdToOPCUA(OV_INSTPTR_opcua_nodeId src, UA_NodeId *dst) {
	switch (src->v_identifierType) {
	case UA_NODEIDTYPE_NUMERIC:
		*dst = UA_NODEID_NUMERIC(src->v_namespaceIndex, src->v_id.value.valueunion.val_uint);
		break;
	case UA_NODEIDTYPE_STRING:
		*dst = UA_NODEID_STRING_ALLOC(src->v_namespaceIndex, src->v_id.value.valueunion.val_string);
		break;
	default:
		return OV_ERR_BADPARAM;
	}
	return OV_ERR_OK;
}

OV_RESULT copyOPCUANodeIdToOV(UA_NodeId *src, OV_INSTPTR_opcua_nodeId dst) {

	dst->v_namespaceIndex = src->namespaceIndex;
	switch (src->identifierType) {
	case UA_NODEIDTYPE_NUMERIC:
		dst->v_identifierType = UA_NODEIDTYPE_NUMERIC;
		dst->v_id.value.valueunion.val_uint = src->identifier.numeric;
		break;
	case UA_NODEIDTYPE_STRING:
		dst->v_identifierType = UA_NODEIDTYPE_STRING;
		copyOPCUAStringToOV(src->identifier.string, &dst->v_id.value.valueunion.val_string);
		break;
	default:
		return OV_ERR_BADPARAM;
	}
	return OV_ERR_OK;
}


OV_RESULT copyOvExpandedNodeIdToOPCUA(OV_INSTPTR_opcua_expandedNodeId src,
		UA_ExpandedNodeId *dst) {
	*dst = UA_EXPANDEDNODEID_NUMERIC(src->p_nodeId.v_namespaceIndex, src->p_nodeId.v_id.value.valueunion.val_uint);
	return OV_ERR_OK;
}

OV_RESULT copyOPCUAExpandedNodeIdToOV(UA_ExpandedNodeId *src,
		OV_INSTPTR_opcua_expandedNodeId dst) {
	OV_RESULT res = copyOPCUAStringToOV(src->namespaceUri,
			&dst->v_namespaceUri);
	dst->v_serverIndex = src->serverIndex;
	res |= copyOPCUANodeIdToOV(&src->nodeId, &dst->p_nodeId);
	return res;
}

OV_RESULT copyOvQualifiedNameToOPCUA(OV_INSTPTR_opcua_qualifiedName src,
		UA_QualifiedName *dst) {
	if(!src->v_name)
		return OV_ERR_OK;
	*dst = UA_QUALIFIEDNAME_ALLOC(src->v_namespaceIndex, src->v_name);
	return OV_ERR_OK;
}


OV_RESULT copyOPCUAQualifiedNameToOV(UA_QualifiedName *src,
		OV_INSTPTR_opcua_qualifiedName dst) {
	dst->v_namespaceIndex = src->namespaceIndex;
	return copyOPCUAStringToOV(src->name, &dst->v_name);
}

void deleteUAString(UA_String *ptr) {
	if (!ptr)
		return;
	if (ptr->length > 0)
		ov_database_free(ptr->data);
	return;
}

OV_RESULT copyOvLocalizedTextToOPCUA(OV_INSTPTR_opcua_localizedText src,
		UA_LocalizedText *dst) {

	*dst = UA_LOCALIZEDTEXT_ALLOC(src->v_locale ? src->v_locale : "", src->v_text ? src->v_text : "");
	return OV_ERR_OK;
}

OV_RESULT copyOPCUALocalizedTextToOV(UA_LocalizedText *src,
		OV_INSTPTR_opcua_localizedText dst) {
	OV_RESULT res = copyOPCUAStringToOV(src->locale, &dst->v_locale);
	res |= copyOPCUAStringToOV(src->text, &dst->v_text);
	return res;
}


OV_RESULT copyOvReferenceNodeToOPCUA(OV_INSTPTR_opcua_reference src,
		UA_ReferenceNode *dst) {
	dst->isInverse = src->v_isInverse;
	OV_RESULT res = copyOvNodeIdToOPCUA(&src->p_referenceTypeNodeId,
			&dst->referenceTypeId);
	res |= copyOvExpandedNodeIdToOPCUA(&src->p_targetNodeId, &dst->targetId);
	return res;
}

OV_RESULT copyOPCUAReferenceNodeToOV(UA_ReferenceNode *src,
		OV_INSTPTR_opcua_reference dst) {
	dst->v_isInverse = src->isInverse;
	OV_RESULT res = copyOPCUANodeIdToOV(&src->referenceTypeId,
			&dst->p_referenceTypeNodeId);
	res |= copyOPCUAExpandedNodeIdToOV(&src->targetId, &dst->p_targetNodeId);
	return res;
}


OV_RESULT createOpcuaReferenceNode(OV_INSTPTR_ov_domain location, OV_INSTPTR_opcua_reference *newReferenceNode, OV_STRING name){
	OV_INSTPTR_opcua_reference tmpRef = NULL;
	OV_RESULT result = 0;
	result = Ov_CreateObject(opcua_reference, tmpRef, location, name);
	*newReferenceNode = tmpRef;
	return result;
}
/**
 * resolves a UA-nodeId to an OV_PATH object
 * the nodeId has to be of type STRING or NUMERIC
 * in the latter case only objects can be addressed (no variables)
 * the STRING nodeIds are treated as a usual path, so the ov-id can be part of them (/.xxx)
 * call ov_memstack_lock() /_unlock() around this one
 */

UA_Int32 opcua_nodeStoreFunctions_resolveNodeIdToPath(UA_NodeId nodeId, OV_PATH* pPath){
	OV_STRING tmpString = NULL;
	OV_RESULT result;
	ov_memstack_lock();
	switch(nodeId.identifierType){
	case UA_NODEIDTYPE_STRING:
		tmpString = ov_memstack_alloc(nodeId.identifier.string.length + 1);
		if(!tmpString){
			ov_memstack_unlock();
			return UA_STATUSCODE_BADOUTOFMEMORY;
		}
		memcpy(tmpString,nodeId.identifier.string.data,nodeId.identifier.string.length);
		tmpString[nodeId.identifier.string.length] = 0;
		result = ov_path_resolve(pPath,NULL,tmpString, 2);
		if(Ov_Fail(result)){
			ov_memstack_unlock();
			return ov_resultToUaStatusCode(result);
		}
		break;
	case UA_NODEIDTYPE_NUMERIC:
		tmpString = ov_memstack_alloc(32);
		if(!tmpString){
			ov_memstack_unlock();
			return UA_STATUSCODE_BADOUTOFMEMORY;
		}
		snprintf(tmpString, 31, "/.%u", nodeId.identifier.numeric);
		result = ov_path_resolve(pPath,NULL,tmpString, 2);
		if(Ov_Fail(result)){
			ov_memstack_unlock();
			return ov_resultToUaStatusCode(result);
		}
		break;
	default:
		return UA_STATUSCODE_BADNODEIDREJECTED;
	}
	ov_memstack_unlock();
	return UA_STATUSCODE_GOOD;
}

OV_INSTPTR_ov_object opcua_nodeStoreFunctions_resolveNodeIdToOvObject(UA_NodeId *nodeId){
	OV_STRING tmpString = NULL;
	OV_INSTPTR_ov_object ptr = NULL;
	ov_memstack_lock();
	switch(nodeId->identifierType){
	case UA_NODEIDTYPE_STRING:
		tmpString = ov_database_malloc(nodeId->identifier.string.length + 1);
		if(!tmpString){
			return NULL;
		}
		memcpy(tmpString,nodeId->identifier.string.data,nodeId->identifier.string.length);
		tmpString[nodeId->identifier.string.length] = 0;
		ptr = ov_path_getobjectpointer(tmpString, 2);
		break;
	case UA_NODEIDTYPE_NUMERIC:
		tmpString = ov_memstack_alloc(sizeof(nodeId->identifier.numeric)+12);
		if(!tmpString){
			return NULL;
		}
		snprintf(tmpString, 99, "/TechUnits/%u", nodeId->identifier.numeric);
		ptr = ov_path_getobjectpointer(tmpString, 2);
		break;
	default:
		break;
	}
	ov_memstack_unlock();
	return ptr;
}


UA_Int32 opcua_nodeStoreFunctions_getVtblPointerAndCheckAccess(OV_ELEMENT *pelem, OV_TICKET* pTicket, OV_INSTPTR_ov_object *pInstance, OV_VTBLPTR_ov_object *ppVtblObj, OV_ACCESS *access){
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
	*access = (*ppVtblObj)->m_getaccess(pelem->pobj, pelem, pTicket);
	if(pTicket){
		(*access) &= pTicket->vtbl->getaccess(pTicket);
	}
	return UA_STATUSCODE_GOOD;
}

UA_Int32 opcua_nsOv_getNodeClassAndAccess(const OV_ELEMENT* pElem, OV_ACCESS* pAccess){
	OV_VTBLPTR_ov_object	pVtbl	=	NULL;
	if(pAccess){
		if(!pElem->pobj){
			*pAccess = OV_AC_NONE;
		} else {
			Ov_GetVTablePtr(ov_object, pVtbl, pElem->pobj);
			if(pVtbl){
				*pAccess = pVtbl->m_getaccess(pElem->pobj, pElem, NULL);
			} else {
				*pAccess = OV_AC_NONE;
			}
		}
	}
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
			if(Ov_CanCastTo(opcua_methodNode, pElem->pobj)){
				return UA_NODECLASS_METHOD;
			} else {
				return UA_NODECLASS_OBJECT;
			}
		}
	} else if(pElem->elemtype == OV_ET_VARIABLE || pElem->elemtype == OV_ET_MEMBER) {
		return UA_NODECLASS_VARIABLE;
	} else {
		return UA_NODECLASS_UNSPECIFIED;
	}
}

OV_BOOL opcua_nsOv_nodeClassMaskMatchAndGetAccess(const OV_ELEMENT* pElem, UA_UInt32 mask, OV_ACCESS* pAccess){
	UA_Int32 nodeClass = opcua_nsOv_getNodeClassAndAccess(pElem, pAccess);
	if(mask == 0){
		return TRUE; //if no bit is set, all attributes should be returned
	}
	switch(nodeClass){
	case UA_NODECLASS_OBJECT:
		return ((mask & (1<<0)) ? TRUE : FALSE);
	case UA_NODECLASS_VARIABLE:
		return ((mask & (1<<1)) ? TRUE : FALSE);
	case UA_NODECLASS_METHOD:
		return ((mask & (1<<2)) ? TRUE : FALSE);
	case UA_NODECLASS_OBJECTTYPE:
		return ((mask & (1<<3)) ? TRUE : FALSE);
	case UA_NODECLASS_VARIABLETYPE:
		return ((mask & (1<<4)) ? TRUE : FALSE);
	case UA_NODECLASS_REFERENCETYPE:
		return ((mask & (1<<5)) ? TRUE : FALSE);
	case UA_NODECLASS_DATATYPE:
		return ((mask & (1<<6)) ? TRUE : FALSE);
	case UA_NODECLASS_VIEW:
		return ((mask & (1<<7)) ? TRUE : FALSE);
	default:
		return FALSE;
	}
}

