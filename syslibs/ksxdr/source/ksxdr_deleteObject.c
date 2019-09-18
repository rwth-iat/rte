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
 * xdr routine for decoding delete object parameters
 */

OV_RESULT ksxdr_deleteObject_decodeparams(KS_DATAPACKET* dataReceived, OV_DELETEOBJECT_PAR* params)
{
	return KS_DATAPACKET_read_xdr_array_tomemstack(dataReceived, (void**) &params->paths_val, sizeof(OV_STRING), &params->paths_len,
			(xdr_readfncptr) &KS_DATAPACKET_read_xdr_string_tomemstack_wolength);
}



/*
 * routine to encode the results
 */

OV_RESULT ksxdr_deleteObject_encoderesults(KS_DATAPACKET* serviceAnswer, OV_DELETEOBJECT_RES* results)
{
	switch(results->result)
	{
	case OV_ERR_OK:
		return KS_DATAPACKET_write_xdr_array(serviceAnswer, (void**) &results->results_val, sizeof(OV_RESULT), &results->results_len,
				(xdr_writefncptr) &KS_DATAPACKET_write_xdr_int);
	default:
		break;
	}

	return OV_ERR_OK;
}

/*
 * this routine decodes the params, executes the service and encodes the results
 */

OV_RESULT ksxdr_deleteObject(const OV_UINT version, const OV_TICKET* pticket, KS_DATAPACKET* dataReceived, KS_DATAPACKET* serviceAnswer, OV_UINT* msgState, OV_UINT* ksErrCode)
{
	/*
	 *	parameter and result objects
	 */
	OV_DELETEOBJECT_PAR	params;
	OV_DELETEOBJECT_RES	results;
	OV_RESULT		fncresult;
	/*
	 *	decode the parameters
	 */
	ov_memstack_lock();
	fncresult = ksxdr_deleteObject_decodeparams(dataReceived, &params);
	if(Ov_OK(fncresult))
	{
		/*
		 *	properly decoded, call service function and send reply
		 */
		ov_ksserver_deleteobject(version, pticket, &params, &results);
		*ksErrCode = results.result;
		fncresult = ksxdr_deleteObject_encoderesults(serviceAnswer, &results);
		KS_logfile_debug(("ksxdr_deleteObject: %s", ov_result_getresulttext(fncresult)));
#if LOG_KS || LOG_KS_DEBUG
		{
			OV_UINT i;
			for(i=0; i<results.results_len; i++){
				KS_logfile_debug(("ksxdr_deleteObject: item %lu: %s", i, ov_result_getresulttext(results.results_val[0])));
			}
		}
#endif
		ov_memstack_unlock();
		return OV_ERR_OK;
	}
	ov_memstack_unlock();

	return fncresult;
}
