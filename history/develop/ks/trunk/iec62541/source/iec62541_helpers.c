/*
 * iec62541_helpers.c
 *
 *  Created on: 21.10.2014
 *      Author: lars
 */



#include "iec62541_helpers.h"
#include "libov/ov_string.h"
#include "libov/ov_time.h"

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
		return UA_STATUSCODE_BADNODEIDINVALID;
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

UA_StatusCode ov_AnyToVariant(OV_ANY* pAny, UA_Variant* pVariant){
	UA_StatusCode result;
	void *value = NULL;
	const UA_VTable_Entry *vt = NULL;
	UA_Boolean tempBool;	/*	has different byte size in ov and ua hence we need a temp variable	*/
	UA_String tempString = {.length = 0, .data = NULL};
	UA_DateTime tempTime;
	UA_Int32	arrayLength;
	UA_Boolean* tempBoolArray = NULL;
	UA_String*	tempStringArray = NULL;
	UA_DateTime*	tempTimeArray = NULL;
	UA_Int32	iterator;

	UA_VTable_Entry *debug = UA_;

	if(!((pAny->value.vartype & OV_VT_KSMASK) & OV_VT_ISVECTOR)){
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
			vt = &UA_[UA_BOOLEAN];
			break;
		case OV_VT_BYTE:
			value = &(pAny->value.valueunion.val_byte);
			vt = &UA_[UA_BYTE];
			break;
		case OV_VT_DOUBLE:
		case OV_VT_DOUBLE_PV:
			value = &(pAny->value.valueunion.val_double);
			vt = &UA_[UA_DOUBLE];
			break;
		case OV_VT_INT:
		case OV_VT_INT_PV:
			value = &(pAny->value.valueunion.val_int);
			vt = &UA_[UA_INT32];
			break;
		case OV_VT_STATE:
			value = &(pAny->value.valueunion.val_state);
			vt = &UA_[UA_INT32];
			break;
		case OV_VT_SINGLE:
		case OV_VT_SINGLE_PV:
			value = &(pAny->value.valueunion.val_single);
			vt = &UA_[UA_FLOAT];
			break;
		case OV_VT_STRING:
		case OV_VT_STRING_PV:
			tempString.data = (UA_Byte*) pAny->value.valueunion.val_string;
			tempString.length = strlen(pAny->value.valueunion.val_string);
			value = &tempString;
			vt = &UA_[UA_STRING];
			break;
		case OV_VT_TIME:
		case OV_VT_TIME_PV:
			tempTime = ov_ovTimeTo1601nsTime(pAny->value.valueunion.val_time);
			value = &tempTime;
			vt = &UA_[UA_DATETIME];
			break;
		case OV_VT_TIME_SPAN:
		case OV_VT_TIME_SPAN_PV:
			tempTime = ov_ovTimeSpanTo1601nsTime(pAny->value.valueunion.val_time_span);
			value = &tempTime;
			vt = &UA_[UA_DATETIME];
			break;
		case OV_VT_UINT:
		case OV_VT_UINT_PV:
			value = &(pAny->value.valueunion.val_uint);
			vt = &UA_[UA_UINT32];
			break;
		default:
			return ov_resultToUaStatusCode(OV_ERR_BADTYPE);
		}
		result = UA_Variant_copySetValue(pVariant, vt, value);
		pVariant->storage.data.arrayDimensions       = UA_NULL;
		pVariant->storage.data.arrayDimensionsLength = 1;
		return result;
	} else {
		/*	handle vectors	*/
		ov_memstack_lock();
		switch(pAny->value.vartype & OV_VT_KSMASK){
		case OV_VT_BOOL_VEC:
		case OV_VT_BOOL_PV_VEC:
			arrayLength = pAny->value.valueunion.val_bool_vec.veclen;
			tempBoolArray = ov_memstack_alloc(arrayLength * sizeof(UA_Boolean));
			if(!tempBoolArray){
				ov_memstack_unlock();
				return ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
			}
			for(iterator = 0; iterator < arrayLength; iterator++){
				if(pAny->value.valueunion.val_bool_vec.value[iterator] == TRUE){
					tempBoolArray[iterator] = UA_TRUE;
				} else {
					tempBoolArray[iterator] = UA_FALSE;
				}
			}
			value = &(tempBoolArray);
			vt = &UA_[UA_BOOLEAN];
			break;
		case OV_VT_BYTE_VEC:
			arrayLength = pAny->value.valueunion.val_byte_vec.veclen;
			value = &(pAny->value.valueunion.val_byte_vec.value);
			vt = &UA_[UA_BYTE];
			break;
		case OV_VT_DOUBLE_VEC:
		case OV_VT_DOUBLE_PV_VEC:
			arrayLength = pAny->value.valueunion.val_double_vec.veclen;
			value = &(pAny->value.valueunion.val_double_vec.value);
			vt = &UA_[UA_DOUBLE];
			break;
		case OV_VT_INT_VEC:
		case OV_VT_INT_PV_VEC:
			arrayLength = pAny->value.valueunion.val_int_vec.veclen;
			value = &(pAny->value.valueunion.val_int_vec.value);
			vt = &UA_[UA_INT32];
			break;
		case OV_VT_STATE_VEC:
			arrayLength = pAny->value.valueunion.val_state_vec.veclen;
			value = &(pAny->value.valueunion.val_state_vec.value);
			vt = &UA_[UA_INT32];
			break;
		case OV_VT_SINGLE_VEC:
		case OV_VT_SINGLE_PV_VEC:
			arrayLength = pAny->value.valueunion.val_single_vec.veclen;
			value = &(pAny->value.valueunion.val_single_vec.value);
			vt = &UA_[UA_FLOAT];
			break;
		case OV_VT_STRING_VEC:
		case OV_VT_STRING_PV_VEC:
			arrayLength = pAny->value.valueunion.val_string_vec.veclen;
			tempStringArray = ov_memstack_alloc(arrayLength * sizeof(UA_String));
			if(!tempStringArray){
				ov_memstack_unlock();
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
			value = &(tempStringArray);
			vt = &UA_[UA_STRING];
			break;
		case OV_VT_TIME_VEC:
		case OV_VT_TIME_PV_VEC:
			arrayLength = pAny->value.valueunion.val_time_vec.veclen;
			tempTimeArray = ov_memstack_alloc(arrayLength * sizeof(UA_DateTime));
			if(!tempTimeArray){
				ov_memstack_unlock();
				return ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
			}
			for(iterator = 0; iterator < arrayLength; iterator++){
				tempTimeArray[iterator] = ov_ovTimeTo1601nsTime(pAny->value.valueunion.val_time_vec.value[iterator]);
			}
			value = &(tempTimeArray);
			vt = &UA_[UA_DATETIME];
			break;
		case OV_VT_TIME_SPAN_VEC:
		case OV_VT_TIME_SPAN_PV_VEC:
			arrayLength = pAny->value.valueunion.val_time_vec.veclen;
			tempTimeArray = ov_memstack_alloc(arrayLength * sizeof(UA_DateTime));
			if(!tempTimeArray){
				ov_memstack_unlock();
				return ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
			}
			for(iterator = 0; iterator < arrayLength; iterator++){
				tempTimeArray[iterator] = ov_ovTimeSpanTo1601nsTime(pAny->value.valueunion.val_time_span_vec.value[iterator]);
			}
			value = &(tempTimeArray);
			vt = &UA_[UA_DATETIME];
			break;
		case OV_VT_UINT_VEC:
		case OV_VT_UINT_PV_VEC:
			arrayLength = pAny->value.valueunion.val_uint_vec.veclen;
			value = &(pAny->value.valueunion.val_uint_vec.value);
			vt = &UA_[UA_UINT32];
			break;
		default:
			return ov_resultToUaStatusCode(OV_ERR_BADTYPE);
		}
		result = UA_Variant_copySetArray(pVariant, vt, arrayLength, value);
		ov_memstack_unlock();
		return result;
	}
}

UA_StatusCode ov_VariantToAny(UA_Variant* pVariant, OV_ANY* pAny){
	//	UA_NodeId typeid = { .namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC,
	//	                         .identifier.numeric = encodingByte & UA_VARIANT_ENCODINGMASKTYPE_TYPEID_MASK };
	//	    UA_Int32 typeNs0Id = UA_ns0ToVTableIndex(&typeid );
	//	    const UA_VTable_Entry *vt = &UA_[typeNs0Id];

	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}
