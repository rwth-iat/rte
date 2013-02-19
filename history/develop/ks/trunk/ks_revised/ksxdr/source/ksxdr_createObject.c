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
*	XDR routine for OV_CREATEOBJ_ITEM
*/
OV_RESULT ksxdr_read_OV_CREATEOBJ_ITEM (KS_DATAPACKET* dataReceived, OV_CREATEOBJ_ITEM* pitem)
{
	OV_RESULT result;
	OV_UINT i;

	result = KS_DATAPACKET_read_xdr_string_tomemstack_wolength(dataReceived, &pitem->factory_path);
	if(Ov_Fail(result))
		return result;

	result = KS_DATAPACKET_read_xdr_string_tomemstack_wolength(dataReceived, &pitem->new_path);
	if(Ov_Fail(result))
		return result;

	result = xdr_read_OV_PLACEMENT(dataReceived, &pitem->place);
	if(Ov_Fail(result))
		return result;

	result = KS_DATAPACKET_read_xdr_u_long(dataReceived, &pitem->parameters_len);
	if(Ov_Fail(result))
		return result;
	pitem->parameters_val = ov_memstack_alloc(pitem->parameters_len * sizeof(OV_SETVAR_ITEM));
	if(!pitem->parameters_val)
		return OV_ERR_HEAPOUTOFMEMORY;
	for(i=0; i<pitem->parameters_len; i++)
	{
		result = xdr_read_OV_SETVAR_ITEM(dataReceived, &(pitem->parameters_val[i]));
		if(Ov_Fail(result))
			return result;
	}

	result = KS_DATAPACKET_read_xdr_u_long(dataReceived, &pitem->links_len);
	if(Ov_Fail(result))
		return result;
	pitem->links_val = ov_memstack_alloc(pitem->links_len * sizeof(OV_LINK_ITEM));
	if(!pitem->links_val)
		return OV_ERR_HEAPOUTOFMEMORY;
	for(i=0; i<pitem->links_len; i++)
	{
		result = xdr_read_OV_LINK_ITEM(dataReceived, &(pitem->links_val[i]));
		if(Ov_Fail(result))
			return result;
	}

	return OV_ERR_OK;
}

/*
 * xdr routine for decoding create object parameters
 */

OV_RESULT ksxdr_createObject_decodeparams(KS_DATAPACKET* dataReceived, OV_CREATEOBJECT_PAR* params)
{
	OV_RESULT result;
	OV_UINT i;

	result = KS_DATAPACKET_read_xdr_u_long(dataReceived, &params->items_len);
	if(Ov_Fail(result))
		return result;

	params->items_val = ov_memstack_alloc(params->items_len * sizeof(OV_CREATEOBJ_ITEM));
	if(!params->items_val)
		return OV_ERR_HEAPOUTOFMEMORY;

	for(i=0; i<params->items_len; i++)
	{
		result = ksxdr_read_OV_CREATEOBJ_ITEM(dataReceived, &(params->items_val[i]));
		if(Ov_Fail(result))
			return result;
	}

	return OV_ERR_OK;
}


/*
*	XDR routine for OV_CREATEOBJECTITEM_RES
*/
OV_RESULT xdr_write_OV_CREATEOBJECTITEM_RES (KS_DATAPACKET* datapacket, OV_CREATEOBJECTITEM_RES* pitem)
{
	OV_RESULT result;

	result = KS_DATAPACKET_write_xdr_OV_RESULT(datapacket, &pitem->result);
	if(Ov_Fail(result))
		return result;

	switch(pitem->result)
	{
	case OV_ERR_BADINITPARAM:
		result = KS_DATAPACKET_write_xdr_array(datapacket, (void**) &(pitem->params_results_val), sizeof(OV_RESULT), &pitem->params_results_len,
				(xdr_writefncptr) &KS_DATAPACKET_write_xdr_long);
		if(Ov_Fail(result))
			return result;

		return KS_DATAPACKET_write_xdr_array(datapacket, (void**) &(pitem->link_results_val), sizeof(OV_RESULT), &pitem->link_results_len,
				(xdr_writefncptr) &KS_DATAPACKET_write_xdr_long);
	default:
		break;
	}
	return OV_ERR_OK;
}

/*
 * routine to encode the results
 */

OV_RESULT ksxdr_createObject_encoderesults(KS_DATAPACKET* serviceAnswer, OV_CREATEOBJECT_RES* results)
{
	OV_RESULT result;
	OV_UINT i;

	switch(results->result)
	{
	case OV_ERR_OK:
		result = KS_DATAPACKET_write_xdr_u_long(serviceAnswer, &results->obj_results_len);
		if(Ov_Fail(result))
			return result;
		for(i=0; i<results->obj_results_len; i++)
		{
			result = xdr_write_OV_CREATEOBJECTITEM_RES(serviceAnswer, &(results->obj_results_val[i]));
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

OV_RESULT ksxdr_createObject(const OV_UINT version, const OV_TICKET* pticket, KS_DATAPACKET* dataReceived, KS_DATAPACKET* serviceAnswer, OV_UINT* msgState, OV_UINT* ksErrCode)
{
	/*
	 *	parameter and result objects
	 */
	OV_CREATEOBJECT_PAR	params;
	OV_CREATEOBJECT_RES	results;
	OV_RESULT		fncresult;
	/*
	 *	decode the parameters
	 */
	ov_memstack_lock();
	fncresult = ksxdr_createObject_decodeparams(dataReceived, &params);
	if(Ov_OK(fncresult))
	{
		/*
		 *	properly decoded, call service function and send reply
		 */
		ov_ksserver_createobject(version, pticket, &params, &results);
		*ksErrCode = results.result;
		fncresult = ksxdr_createObject_encoderesults(serviceAnswer, &results);
		ov_memstack_unlock();
		return OV_ERR_OK;
	}
	ov_memstack_unlock();

	return fncresult;
}
