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
*	XDR routine for OV_RENAMEOBJECT_ITEM
*/
OV_RESULT xdr_read_OV_RENAMEOBJECT_ITEM (KS_DATAPACKET* datapacket, OV_RENAMEOBJECT_ITEM* pitem)
{
	OV_RESULT result;

	result = KS_DATAPACKET_read_xdr_string_tomemstack_wolength(datapacket, &pitem->old_path);
	if(Ov_Fail(result))
		return result;

	result = KS_DATAPACKET_read_xdr_string_tomemstack_wolength(datapacket, &pitem->new_path);
	if(Ov_Fail(result))
		return result;

	return xdr_read_OV_PLACEMENT(datapacket, &pitem->place);
}

OV_RESULT xdr_write_OV_RENAMEOBJECT_ITEM (KS_DATAPACKET* datapacket, OV_RENAMEOBJECT_ITEM* pitem)
{
	OV_RESULT result;

	result = KS_DATAPACKET_write_xdr_string(datapacket, &pitem->old_path);
	if(Ov_Fail(result))
		return result;

	result = KS_DATAPACKET_write_xdr_string(datapacket, &pitem->new_path);
	if(Ov_Fail(result))
		return result;

	return xdr_write_OV_PLACEMENT(datapacket, &pitem->place);
}

/*
 * xdr routine for decoding renameObject parameters
 */

OV_RESULT ksxdr_renameObject_decodeparams(KS_DATAPACKET* dataReceived, OV_RENAMEOBJECT_PAR* params)
{
	OV_RESULT result;
	OV_UINT i;

	result = KS_DATAPACKET_read_xdr_u_long(dataReceived, &params->items_len);
	if(Ov_Fail(result))
		return result;
	params->items_val = ov_memstack_alloc(params->items_len * sizeof(OV_RENAMEOBJECT_ITEM));
	if(!params->items_val)
		return OV_ERR_HEAPOUTOFMEMORY;

	for(i=0; i<params->items_len; i++)
	{
		result = xdr_read_OV_RENAMEOBJECT_ITEM(dataReceived, &params->items_val[i]);
		if(Ov_Fail(result))
			return result;
	}

	return OV_ERR_OK;
}



/*
 * routine to encode the results
 */

OV_RESULT ksxdr_renameObject_encoderesults(KS_DATAPACKET* serviceAnswer, OV_RENAMEOBJECT_RES* results)
{
	switch(results->result)
	{
	case OV_ERR_OK:
		return KS_DATAPACKET_write_xdr_array(serviceAnswer, (void**) &results->results_val, sizeof(OV_RESULT), &results->results_len,
				(xdr_writefncptr) &KS_DATAPACKET_write_xdr_long);
	default:
		break;
	}
	return OV_ERR_OK;
}

/*
 * this routine decodes the params, executes the service and encodes the results
 */

OV_RESULT ksxdr_renameObject(const OV_UINT version, const OV_TICKET* pticket, KS_DATAPACKET* dataReceived, KS_DATAPACKET* serviceAnswer, OV_UINT* msgState, OV_UINT* ksErrCode)
{
	/*
	 *	parameter and result objects
	 */
	OV_RENAMEOBJECT_PAR	params;
	OV_RENAMEOBJECT_RES	results;
	OV_RESULT		fncresult;
	/*
	 *	decode the parameters
	 */
	ov_memstack_lock();
	fncresult = ksxdr_renameObject_decodeparams(dataReceived, &params);
	if(Ov_OK(fncresult))
	{
		/*
		 *	properly decoded, call service function and send reply
		 */
		ov_ksserver_renameobject(version, pticket, &params, &results);
		*ksErrCode = results.result;
		fncresult = ksxdr_renameObject_encoderesults(serviceAnswer, &results);
		ov_memstack_unlock();
		return OV_ERR_OK;
	}
	ov_memstack_unlock();

	return fncresult;
}
