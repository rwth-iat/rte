/*
 * ksxdr_getserver.c
 *
 *  Created on: 12.02.2013
 *      Author: lars
 */

#include "ksxdr.h"
#include "ksxdr_config.h"
#include "ks_logfile.h"
#include "libov/ov_result.h"
#include "libov/ov_macros.h"
#include "KSDATAPACKET_xdrhandling.h"
#include "ov_ksserver_backend.h"
#include "ksbase_helper.h"

/*
 * xdr routine for decoding getVar parameters
 */

OV_RESULT ksxdr_getVar_decodeparams(KS_DATAPACKET* dataReceived, OV_GETVAR_PAR* params)
{
	return KS_DATAPACKET_read_xdr_array_tomemstack(dataReceived, (void**)&params->identifiers_val,
			sizeof(OV_STRING), &params->identifiers_len, (xdr_readfncptr)KS_DATAPACKET_read_xdr_string_tomemstack_wolength);
}



/*
 * routine to encode the results
 */

OV_RESULT ksxdr_getVar_encoderesults(KS_DATAPACKET* serviceAnswer, OV_GETVAR_RES* results)
{
	/*
	*	local variables
	*/
	OV_RESULT result;
	OV_UINT i;

	switch(results->result)
	{
	case OV_ERR_OK:
		result = KS_DATAPACKET_write_xdr_uint(serviceAnswer, &results->items_len);
		if(Ov_Fail(result))
			return result;

		for(i=0; i<results->items_len; i++)
		{
			result = xdr_write_OV_GETVAR_ITEM (serviceAnswer, &(results->items_val[i]));
			if(Ov_Fail(result))
				return result;
		}
		break;

	default:
		break;
	}

	return OV_ERR_OK;
}

/*
 * this routine decodes the params, executes the service and encodes the results
 */

OV_RESULT ksxdr_getVar(const OV_UINT version, const OV_TICKET* pticket, KS_DATAPACKET* dataReceived, KS_DATAPACKET* serviceAnswer, OV_UINT* msgState, OV_UINT* ksErrCode)
{
	/*
	 *	parameter and result objects
	 */
	OV_GETVAR_PAR	params;
	OV_GETVAR_RES	results;
	OV_RESULT		fncresult;
	/*
	 *	decode the parameters
	 */
	ov_memstack_lock();
	fncresult = ksxdr_getVar_decodeparams(dataReceived, &params);
	if(Ov_OK(fncresult))
	{
		/*
		 *	properly decoded, call service function and send reply
		 */
		ov_ksserver_getvar(version, pticket, &params, &results);
		*ksErrCode = results.result;
		fncresult = ksxdr_getVar_encoderesults(serviceAnswer, &results);
		ov_memstack_unlock();
		return OV_ERR_OK;
	}
	ov_memstack_unlock();

	return fncresult;
}

/*
*	XDR routine for OV_GETVAR_ITEM
*/
OV_RESULT xdr_read_OV_GETVAR_ITEM (KS_DATAPACKET* dataPacket, OV_GETVAR_ITEM *pItem)
{
	OV_OBJ_TYPE	objtype;
	OV_RESULT result;

	result = KS_DATAPACKET_read_xdr_OV_RESULT(dataPacket, &pItem->result);
	if(Ov_Fail(result))
		return result;

	switch(pItem->result) {
	case OV_ERR_OK:
		result = KS_DATAPACKET_read_xdr_OV_OBJ_TYPE(dataPacket, &objtype);
		if(Ov_Fail(result))
			return result;

		if(objtype != KS_OT_VARIABLE)
			return OV_ERR_BADOBJTYPE;

		return xdr_read_OV_VAR_CURRENT_PROPS(dataPacket, &pItem->var_current_props);
	default:
		break;
	}
	return OV_ERR_OK;
}


OV_RESULT xdr_write_OV_GETVAR_ITEM (KS_DATAPACKET* serviceAnswer, OV_GETVAR_ITEM	*pItem)
{
	OV_OBJ_TYPE	objtype = KS_OT_VARIABLE;
	OV_RESULT result;

	result = KS_DATAPACKET_write_xdr_OV_RESULT(serviceAnswer, &pItem->result);
	if(Ov_Fail(result))
		return result;

	switch(pItem->result) {
	case OV_ERR_OK:
		result = KS_DATAPACKET_write_xdr_OV_OBJ_TYPE(serviceAnswer, &objtype);
		if(Ov_Fail(result))
			return result;

		return xdr_write_OV_VAR_CURRENT_PROPS(serviceAnswer, &pItem->var_current_props);
	default:
		break;
	}
	return OV_ERR_OK;
}

