/*
 * KSDATAPACKET_xdrhandling_ov_specific.c
 *
 *  Created on: 18.02.2013
 *      Author: lars
 */

/*
 * The functions defined herein handle OV-specific datatypes.
 * there are more (specialized functions) for other datatypes (e.g. GETVAR_ITEM) they can be found in the respective ksxdr_xxx.c files. They were not put here in order
 * 	not to inflate this file too much
 */

#include "ksxdr.h"
#include "libov/ov_malloc.h"
#include "libov/ov_memstack.h"
#include "ksbase_helper.h"
#include <string.h>
#include "KSDATAPACKET_xdrhandling.h"
#include "ov_ksserver_backend.h"

/*
*	XDR routines for OV_TIME
*/

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_OV_TIME(KS_DATAPACKET* datapacket, OV_TIME *ptime) {
	OV_RESULT result;
	if(!ptime)
		return OV_ERR_BADPARAM;
	result = KS_DATAPACKET_read_xdr_u_long(datapacket, &ptime->secs);
	if(Ov_Fail(result))
		return result;

	return KS_DATAPACKET_read_xdr_u_long(datapacket, &ptime->usecs);
}

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_OV_TIME(KS_DATAPACKET* datapacket, const OV_TIME* ptime) {
	OV_RESULT result;
	if(!ptime)
		return OV_ERR_BADPARAM;
	result = KS_DATAPACKET_write_xdr_u_long(datapacket, &ptime->secs);
	if(Ov_Fail(result))
		return result;

	return KS_DATAPACKET_write_xdr_u_long(datapacket, &ptime->usecs);
}

/*
 * XDR routines for OV_TIME_SPAN
 */

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_OV_TIME_SPAN(KS_DATAPACKET* datapacket, OV_TIME_SPAN *ptimespan)
{
	OV_RESULT result;
	if(!ptimespan)
		return OV_ERR_BADPARAM;
	result = KS_DATAPACKET_read_xdr_long(datapacket, &ptimespan->secs);
	if(Ov_Fail(result))
		return result;

	return KS_DATAPACKET_read_xdr_long(datapacket, &ptimespan->usecs);
}

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_OV_TIME_SPAN(KS_DATAPACKET* datapacket, const OV_TIME_SPAN* ptimespan)
{
	OV_RESULT result;
	if(!ptimespan)
		return OV_ERR_BADPARAM;
	result = KS_DATAPACKET_write_xdr_long(datapacket, &ptimespan->secs);
	if(Ov_Fail(result))
		return result;

	return KS_DATAPACKET_write_xdr_long(datapacket, &ptimespan->usecs);
}


/*
*	XDR routine for OV_NAMED_ELEMENT
*	These are used for example for structs
*	The read function allocates memory on the memstack
*/
OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_OV_NAMED_ELEMENT (KS_DATAPACKET* datapacket, OV_NAMED_ELEMENT* pNamedElement)
{
	OV_RESULT result;
	if(!pNamedElement)
		return OV_ERR_BADPARAM;

	result = KS_DATAPACKET_read_xdr_string_tomemstack(datapacket, &pNamedElement->identifier, KS_NAME_MAXLEN);
	if(Ov_Fail(result))
		return result;

	return KS_DATAPACKET_read_xdr_OV_VAR_VALUE(datapacket, &pNamedElement->value);
}

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_OV_NAMED_ELEMENT (KS_DATAPACKET* datapacket, OV_NAMED_ELEMENT* pNamedElement)
{
	OV_RESULT result;
	if(!pNamedElement)
		return OV_ERR_BADPARAM;

	if(pNamedElement && strlen(pNamedElement->identifier) > KS_NAME_MAXLEN)
		return OV_ERR_BADVALUE;
	result = KS_DATAPACKET_write_xdr_string(datapacket, &pNamedElement->identifier);
	if(Ov_Fail(result))
		return result;

	return KS_DATAPACKET_write_xdr_OV_VAR_VALUE(datapacket, &pNamedElement->value);
}



/*
 * The following functions read or write OV_VAR_VALUEs. this is a synonym for ANY variables
 * The read-function allocates memory from the memstack
 */

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_OV_VAR_VALUE(KS_DATAPACKET* datapacket, OV_VAR_VALUE* value)
{
	OV_RESULT result;

	if(!value)
		return OV_ERR_BADPARAM;

	result = KS_DATAPACKET_read_xdr_OV_VAR_TYPE(datapacket, &value->vartype);
	if(Ov_Fail(result))
		return result;

	switch(value->vartype & OV_VT_KSMASK) {
		case OV_VT_BOOL:
			return KS_DATAPACKET_read_xdr_OV_BOOL(datapacket, &value->valueunion.val_bool);
		case OV_VT_INT:
			return KS_DATAPACKET_read_xdr_long(datapacket, &value->valueunion.val_int);
		case OV_VT_STATE:
			return KS_DATAPACKET_read_xdr_OV_STATE(datapacket, &value->valueunion.val_state);
		case OV_VT_UINT:
			return KS_DATAPACKET_read_xdr_u_long(datapacket, &value->valueunion.val_uint);
		case OV_VT_SINGLE:
			return KS_DATAPACKET_read_xdr_single(datapacket, &value->valueunion.val_single);
		case OV_VT_DOUBLE:
			return KS_DATAPACKET_read_xdr_double(datapacket, &value->valueunion.val_double);
		case OV_VT_STRING:
			return KS_DATAPACKET_read_xdr_string_tomemstack_wolength(datapacket, &value->valueunion.val_string);
		case OV_VT_TIME:
			return KS_DATAPACKET_read_xdr_OV_TIME(datapacket, &value->valueunion.val_time);
		case OV_VT_TIME_SPAN:
			return KS_DATAPACKET_read_xdr_OV_TIME_SPAN(datapacket, &value->valueunion.val_time_span);
		case OV_VT_STRUCT:
			return KS_DATAPACKET_read_xdr_array_tomemstack(datapacket, (void**) &value->valueunion.val_struct.value, sizeof(OV_NAMED_ELEMENT),
					&value->valueunion.val_struct.elements, (xdr_readfncptr) &KS_DATAPACKET_read_xdr_OV_NAMED_ELEMENT);
		case OV_VT_BOOL_VEC:
			return KS_DATAPACKET_read_xdr_array_tomemstack(datapacket, (void**) &value->valueunion.val_bool_vec.value, sizeof(OV_BOOL),
					&value->valueunion.val_bool_vec.veclen, (xdr_readfncptr) &KS_DATAPACKET_read_xdr_long);
		case OV_VT_INT_VEC:
			return KS_DATAPACKET_read_xdr_array_tomemstack(datapacket, (void**) &value->valueunion.val_int_vec.value, sizeof(OV_INT),
					&value->valueunion.val_int_vec.veclen, (xdr_readfncptr) &KS_DATAPACKET_read_xdr_long);
		case OV_VT_STATE_VEC:
			return KS_DATAPACKET_read_xdr_array_tomemstack(datapacket, (void**) &value->valueunion.val_state_vec.value, sizeof(OV_STATE),
					&value->valueunion.val_state_vec.veclen, (xdr_readfncptr) &KS_DATAPACKET_read_xdr_u_long);
		case OV_VT_UINT_VEC:
			return KS_DATAPACKET_read_xdr_array_tomemstack(datapacket, (void**) &value->valueunion.val_uint_vec.value, sizeof(OV_UINT),
					&value->valueunion.val_uint_vec.veclen, (xdr_readfncptr) &KS_DATAPACKET_read_xdr_u_long);
		case OV_VT_SINGLE_VEC:
			return KS_DATAPACKET_read_xdr_array_tomemstack(datapacket, (void**) &value->valueunion.val_single_vec.value, sizeof(OV_SINGLE),
					&value->valueunion.val_single_vec.veclen, (xdr_readfncptr) &KS_DATAPACKET_read_xdr_single);
		case OV_VT_DOUBLE_VEC:
			return KS_DATAPACKET_read_xdr_array_tomemstack(datapacket, (void**) &value->valueunion.val_double_vec.value, sizeof(OV_DOUBLE),
					&value->valueunion.val_double_vec.veclen, (xdr_readfncptr) &KS_DATAPACKET_read_xdr_double);
		case OV_VT_STRING_VEC:
			return KS_DATAPACKET_read_xdr_array_tomemstack(datapacket, (void**) &value->valueunion.val_string_vec.value, sizeof(OV_STRING),
					&value->valueunion.val_string_vec.veclen, (xdr_readfncptr) &KS_DATAPACKET_read_xdr_string_tomemstack_wolength);
		case OV_VT_TIME_VEC:
			return KS_DATAPACKET_read_xdr_array_tomemstack(datapacket, (void**) &value->valueunion.val_time_vec.value, sizeof(OV_TIME),
					&value->valueunion.val_time_vec.veclen, (xdr_readfncptr) &KS_DATAPACKET_read_xdr_OV_TIME);
		case OV_VT_TIME_SPAN_VEC:
			return KS_DATAPACKET_read_xdr_array_tomemstack(datapacket, (void**) &value->valueunion.val_time_span_vec.value, sizeof(OV_TIME_SPAN),
					&value->valueunion.val_time_span_vec.veclen, (xdr_readfncptr) &KS_DATAPACKET_read_xdr_OV_TIME_SPAN);
		case OV_VT_VOID:
			return OV_ERR_OK;
		case OV_VT_BYTE_VEC:
			return KS_DATAPACKET_read_xdr_opaque_tomemstack(datapacket, (char**) &value->valueunion.val_byte_vec.value, &value->valueunion.val_byte_vec.veclen, ~0);
		default:
			break;
		}
		return OV_ERR_BADTYPE;
}

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_OV_VAR_VALUE(KS_DATAPACKET* datapacket, OV_VAR_VALUE* value)
{
	OV_RESULT result;
	OV_VAR_TYPE temptype = (value->vartype & OV_VT_KSMASK);

	if(!value)
		return OV_ERR_BADPARAM;

	result = KS_DATAPACKET_write_xdr_OV_VAR_TYPE(datapacket, &temptype);
	if(Ov_Fail(result))
		return result;

	switch(value->vartype & temptype) {
	case OV_VT_BOOL:
		return KS_DATAPACKET_write_xdr_OV_BOOL(datapacket, &value->valueunion.val_bool);
	case OV_VT_INT:
		return KS_DATAPACKET_write_xdr_long(datapacket, &value->valueunion.val_int);
	case OV_VT_STATE:
		return KS_DATAPACKET_write_xdr_OV_STATE(datapacket, &value->valueunion.val_state);
	case OV_VT_UINT:
		return KS_DATAPACKET_write_xdr_u_long(datapacket, &value->valueunion.val_uint);
	case OV_VT_SINGLE:
		return KS_DATAPACKET_write_xdr_single(datapacket, &value->valueunion.val_single);
	case OV_VT_DOUBLE:
		return KS_DATAPACKET_write_xdr_double(datapacket, &value->valueunion.val_double);
	case OV_VT_STRING:
		return KS_DATAPACKET_write_xdr_string(datapacket, &value->valueunion.val_string);
	case OV_VT_TIME:
		return KS_DATAPACKET_write_xdr_OV_TIME(datapacket, &value->valueunion.val_time);
	case OV_VT_TIME_SPAN:
		return KS_DATAPACKET_write_xdr_OV_TIME_SPAN(datapacket, &value->valueunion.val_time_span);
	case OV_VT_STRUCT:
		return KS_DATAPACKET_write_xdr_array(datapacket, (void**) &value->valueunion.val_struct.value, sizeof(OV_NAMED_ELEMENT),
				&value->valueunion.val_struct.elements, (xdr_writefncptr) &KS_DATAPACKET_write_xdr_OV_NAMED_ELEMENT);
	case OV_VT_BOOL_VEC:
		return KS_DATAPACKET_write_xdr_array(datapacket, (void**) &value->valueunion.val_bool_vec.value, sizeof(OV_BOOL),
				&value->valueunion.val_bool_vec.veclen, (xdr_writefncptr) &KS_DATAPACKET_write_xdr_long);
	case OV_VT_INT_VEC:
		return KS_DATAPACKET_write_xdr_array(datapacket, (void**) &value->valueunion.val_int_vec.value, sizeof(OV_INT),
				&value->valueunion.val_int_vec.veclen, (xdr_writefncptr) &KS_DATAPACKET_write_xdr_long);
	case OV_VT_STATE_VEC:
		return KS_DATAPACKET_write_xdr_array(datapacket, (void**) &value->valueunion.val_state_vec.value, sizeof(OV_STATE),
				&value->valueunion.val_state_vec.veclen, (xdr_writefncptr) &KS_DATAPACKET_write_xdr_u_long);
	case OV_VT_UINT_VEC:
		return KS_DATAPACKET_write_xdr_array(datapacket, (void**) &value->valueunion.val_uint_vec.value, sizeof(OV_UINT),
				&value->valueunion.val_uint_vec.veclen, (xdr_writefncptr) &KS_DATAPACKET_write_xdr_u_long);
	case OV_VT_SINGLE_VEC:
		return KS_DATAPACKET_write_xdr_array(datapacket, (void**) &value->valueunion.val_single_vec.value, sizeof(OV_SINGLE),
				&value->valueunion.val_single_vec.veclen, (xdr_writefncptr) &KS_DATAPACKET_write_xdr_single);
	case OV_VT_DOUBLE_VEC:
		return KS_DATAPACKET_write_xdr_array(datapacket, (void**) &value->valueunion.val_double_vec.value, sizeof(OV_DOUBLE),
				&value->valueunion.val_double_vec.veclen, (xdr_writefncptr) &KS_DATAPACKET_write_xdr_double);
	case OV_VT_STRING_VEC:
		return KS_DATAPACKET_write_xdr_array(datapacket, (void**) &value->valueunion.val_string_vec.value, sizeof(OV_STRING),
				&value->valueunion.val_string_vec.veclen, (xdr_writefncptr) &KS_DATAPACKET_write_xdr_string);
	case OV_VT_TIME_VEC:
		return KS_DATAPACKET_write_xdr_array(datapacket, (void**) &value->valueunion.val_time_vec.value, sizeof(OV_TIME),
				&value->valueunion.val_time_vec.veclen, (xdr_writefncptr) &KS_DATAPACKET_write_xdr_OV_TIME);
	case OV_VT_TIME_SPAN_VEC:
		return KS_DATAPACKET_write_xdr_array(datapacket, (void**) &value->valueunion.val_time_span_vec.value, sizeof(OV_TIME_SPAN),
				&value->valueunion.val_time_span_vec.veclen, (xdr_writefncptr) &KS_DATAPACKET_write_xdr_OV_TIME_SPAN);
	case OV_VT_VOID:
		return OV_ERR_OK;
	case OV_VT_BYTE_VEC:
		return KS_DATAPACKET_write_xdr_opaque(datapacket, (char*) value->valueunion.val_byte_vec.value, value->valueunion.val_byte_vec.veclen);
	default:
		break;
	}
	return OV_ERR_BADTYPE;
}

/*
*	XDR routine for OV_VAR_CURRENT_PROPS
*/
OV_RESULT xdr_read_OV_VAR_CURRENT_PROPS(KS_DATAPACKET* dataReceived, OV_VAR_CURRENT_PROPS* currprops)
{
	OV_RESULT result;

	result = KS_DATAPACKET_read_xdr_OV_VAR_VALUE(dataReceived, &currprops->value);
	if(Ov_Fail(result))
		return result;

	result = KS_DATAPACKET_read_xdr_OV_TIME(dataReceived, &currprops->time);
	if(Ov_Fail(result))
		return result;

	return KS_DATAPACKET_read_xdr_OV_STATE(dataReceived, &currprops->state);
}

/*
*	XDR routine for OV_VAR_CURRENT_PROPS
*/
OV_RESULT xdr_write_OV_VAR_CURRENT_PROPS(KS_DATAPACKET* serviceAnswer, OV_VAR_CURRENT_PROPS* pProps)
{
	OV_RESULT result;

	result = KS_DATAPACKET_write_xdr_OV_VAR_VALUE(serviceAnswer, &pProps->value);
	if(Ov_Fail(result))
		return result;

	result = KS_DATAPACKET_write_xdr_OV_TIME(serviceAnswer, &pProps->time);
	if(Ov_Fail(result))
		return result;

	return KS_DATAPACKET_write_xdr_OV_STATE(serviceAnswer, &pProps->state);
}

/*
*	XDR routine for OV_PLACEMENT
*/
OV_RESULT xdr_read_OV_PLACEMENT (KS_DATAPACKET* dataReceived, OV_PLACEMENT* pplacement)
{
	OV_RESULT result;

	result = KS_DATAPACKET_read_xdr_OV_PLACEMENT_HINT(dataReceived, &pplacement->hint);
	if(Ov_Fail(result))
		return result;

	switch(pplacement->hint) {
	case OV_PMH_BEFORE:
	case OV_PMH_AFTER:
		return KS_DATAPACKET_read_xdr_string_tomemstack_wolength(dataReceived, &pplacement->place_path);
	default:
		break;
	}
	return OV_ERR_OK;
}

OV_RESULT xdr_write_OV_PLACEMENT (KS_DATAPACKET* dataPacket, OV_PLACEMENT* pplacement)
{
	OV_RESULT result;

	result = KS_DATAPACKET_write_xdr_OV_PLACEMENT_HINT(dataPacket, &pplacement->hint);
	if(Ov_Fail(result))
		return result;

	switch(pplacement->hint) {
	case OV_PMH_BEFORE:
	case OV_PMH_AFTER:
		return KS_DATAPACKET_write_xdr_string(dataPacket, &pplacement->place_path);
	default:
		break;
	}
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_LINK_ITEM
*/
OV_RESULT xdr_read_OV_LINK_ITEM (KS_DATAPACKET* dataReceived, OV_LINK_ITEM*	pitem)
{
	OV_RESULT result;

	result = KS_DATAPACKET_read_xdr_string_tomemstack_wolength(dataReceived, &pitem->link_path);
	if(Ov_Fail(result))
		return result;

	result = KS_DATAPACKET_read_xdr_string_tomemstack_wolength(dataReceived, &pitem->element_path);
	if(Ov_Fail(result))
		return result;

	result = xdr_read_OV_PLACEMENT(dataReceived, &pitem->place);
	if(Ov_Fail(result))
		return result;

	return xdr_read_OV_PLACEMENT(dataReceived, &pitem->opposite_place);
}

OV_RESULT xdr_write_OV_LINK_ITEM (KS_DATAPACKET* dataPacket, OV_LINK_ITEM*	pitem)
{
	OV_RESULT result;

	result = KS_DATAPACKET_write_xdr_string(dataPacket, &pitem->link_path);
	if(Ov_Fail(result))
		return result;

	result = KS_DATAPACKET_write_xdr_string(dataPacket, &pitem->element_path);
	if(Ov_Fail(result))
		return result;

	result = xdr_write_OV_PLACEMENT(dataPacket, &pitem->place);
	if(Ov_Fail(result))
		return result;

	return xdr_write_OV_PLACEMENT(dataPacket, &pitem->opposite_place);
}

