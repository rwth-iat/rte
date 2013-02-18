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
*	XDR routine for decoding OV_SETVAR_ITEM
*/
OV_RESULT xdr_read_OV_SETVAR_ITEM (KS_DATAPACKET* dataReceived, OV_SETVAR_ITEM* item)
{
	OV_OBJ_TYPE	objtype;
	OV_RESULT result;

	result = KS_DATAPACKET_read_xdr_string_tomemstack_wolength(dataReceived, &(item->path_and_name));
	if(Ov_Fail(result))
		return result;

	result = KS_DATAPACKET_read_xdr_OV_OBJ_TYPE(dataReceived, &objtype);
	if(Ov_Fail(result))
		return result;

	if(objtype != KS_OT_VARIABLE)
		return FALSE;

	return xdr_read_OV_VAR_CURRENT_PROPS(dataReceived, &item->var_current_props);
}

/*
 * xdr routine for decoding setVar parameters
 */

OV_RESULT ksxdr_setVar_decodeparams(KS_DATAPACKET* dataReceived, OV_SETVAR_PAR* params)
{
	OV_RESULT result;
	OV_UINT i;

	result = KS_DATAPACKET_read_xdr_u_long(dataReceived, &params->items_len);
	if(Ov_Fail(result))
		return result;

	params->items_val = ov_memstack_alloc(params->items_len * sizeof(OV_SETVAR_ITEM));
	if(!params->items_val)
		return OV_ERR_HEAPOUTOFMEMORY;

	for(i=0; i<params->items_len; i++)
	{
		result = xdr_read_OV_SETVAR_ITEM(dataReceived, &(params->items_val[i]));
		if(Ov_Fail(result))
			return result;
	}

	return OV_ERR_OK;
}


/*
 * routine to encode the results
 */

OV_RESULT ksxdr_setVar_encoderesults(KS_DATAPACKET* serviceAnswer, OV_SETVAR_RES* results)
{

	switch(results->result)
	{
	case OV_ERR_OK:
		return KS_DATAPACKET_write_xdr_array(serviceAnswer, (void**) &results->results_val, sizeof(OV_RESULT), &results->results_len, (xdr_writefncptr) &KS_DATAPACKET_write_xdr_long);
		break;

	default:
		break;
	}

	return OV_ERR_OK;
}

/*
 * this routine decodes the params, executes the service and encodes the results
 */

OV_RESULT ksxdr_setVar(const OV_UINT version, const OV_TICKET* pticket, KS_DATAPACKET* dataReceived, KS_DATAPACKET* serviceAnswer, OV_UINT* msgState, OV_UINT* ksErrCode)
{
	/*
	 *	parameter and result objects
	 */
	OV_SETVAR_PAR	params;
	OV_SETVAR_RES	results;
	OV_RESULT		fncresult;
	/*
	 *	decode the parameters
	 */
	ov_memstack_lock();
	fncresult = ksxdr_setVar_decodeparams(dataReceived, &params);
	if(Ov_OK(fncresult))
	{
		/*
		 *	properly decoded, call service function and send reply
		 */
		ov_ksserver_setvar(version, pticket, &params, &results);
		*ksErrCode = results.result;
		fncresult = ksxdr_setVar_encoderesults(serviceAnswer, &results);
		ov_memstack_unlock();
		return OV_ERR_OK;
	}
	ov_memstack_unlock();

	return fncresult;
}
