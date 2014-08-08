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
 * xdr routine for decoding getcanonicalpath parameters
 */

OV_RESULT ksxdr_getCanonicalPath_decodeparams(KS_DATAPACKET* dataReceived, OV_GETCANONICALPATH_PAR* params)
{
	return KS_DATAPACKET_read_xdr_array_tomemstack(dataReceived, (void**) &params->paths_val, sizeof(OV_STRING), &params->paths_len,
			(xdr_readfncptr) &KS_DATAPACKET_read_xdr_string_tomemstack_wolength);
}

/*
*	XDR routine for OV_GETCANONICALPATHITEM_RES
*/
OV_RESULT xdr_write_OV_GETCANONICALPATHITEM(KS_DATAPACKET* datapacket, OV_GETCANONICALPATHITEM_RES* pitem)
{
	OV_RESULT result;

	result = KS_DATAPACKET_write_xdr_OV_RESULT(datapacket, &pitem->result);
	if(Ov_Fail(result))
		return result;

	switch(pitem->result) {
	case OV_ERR_OK:
		return KS_DATAPACKET_write_xdr_string(datapacket, &pitem->canonical_path);
	default:
		break;
	}
	return OV_ERR_OK;
}


/*
 * routine to encode the results
 */

OV_RESULT ksxdr_getCanonicalPath_encoderesults(KS_DATAPACKET* serviceAnswer, OV_GETCANONICALPATH_RES* results)
{
	OV_RESULT result;
	OV_UINT i;

	switch(results->result)
	{
	case OV_ERR_OK:
		result = KS_DATAPACKET_write_xdr_uint(serviceAnswer, &(results->results_len));
		if(Ov_Fail(result))
			return result;
		for(i=0; i<results->results_len; i++)
		{
			result = xdr_write_OV_GETCANONICALPATHITEM(serviceAnswer, &(results->results_val[i]));
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

OV_RESULT ksxdr_getCanonicalPath(const OV_UINT version, const OV_TICKET* pticket, KS_DATAPACKET* dataReceived, KS_DATAPACKET* serviceAnswer, OV_UINT* msgState, OV_UINT* ksErrCode)
{
	/*
	 *	parameter and result objects
	 */
	OV_GETCANONICALPATH_PAR	params;
	OV_GETCANONICALPATH_RES	results;
	OV_RESULT		fncresult;
	/*
	 *	decode the parameters
	 */
	ov_memstack_lock();
	fncresult = ksxdr_getCanonicalPath_decodeparams(dataReceived, &params);
	if(Ov_OK(fncresult))
	{
		/*
		 *	properly decoded, call service function and send reply
		 */
		ov_ksserver_getcanonicalpath(version, pticket, &params, &results);
		*ksErrCode = results.result;
		fncresult = ksxdr_getCanonicalPath_encoderesults(serviceAnswer, &results);
		ov_memstack_unlock();
		return OV_ERR_OK;
	}
	ov_memstack_unlock();

	return fncresult;
}
