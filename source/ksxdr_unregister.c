/*
 * ksxdr_getserver.c
 *
 *  Created on: 12.02.2013
 *      Author: lars
 */


#include "ksxdr.h"
#include "ksxdr_config.h"
#include "ks_logfile.h"
#include "libov/ov_macros.h"
#include "KSDATAPACKET_xdrhandling.h"
#include "ov_ksserver_backend.h"
#include "ksbase_helper.h"

OV_RESULT ksxdr_unregister(const OV_UINT version, const OV_TICKET* pticket, KS_DATAPACKET* dataReceived, KS_DATAPACKET* serviceAnswer, OV_UINT* msgState, OV_UINT* ksErrCode)
{
	OV_STRING servername = NULL;
	OV_UINT serverversion;
	OV_INSTPTR_ksbase_Manager pManager = NULL;
	OV_RESULT result;

	if(Ov_Fail(KS_DATAPACKET_read_xdr_string(dataReceived, &servername, KS_NAME_MAXLEN))
			|| Ov_Fail(KS_DATAPACKET_read_xdr_u_long(dataReceived, &serverversion)))
	{
		KS_logfile_error(("ksxdr_unregister: Error decoding Unregister message. Sending answer."));
		*msgState = XDR_MSGST_GARBAGE_ARGS;
		*ksErrCode = KS_ERR_TARGETGENERIC;
		if(servername)
			ov_free(servername);
		return OV_ERR_OK;
	}
	else
	{
		pManager = Ov_StaticPtrCast(ksbase_Manager, Ov_GetFirstChild(ov_instantiation, pclass_ksbase_Manager));
		if(!pManager)
		{
			*msgState = XDR_MSGST_SUCCESS;
			*ksErrCode = KS_ERR_NOMANAGER;
			if(servername)
				ov_free(servername);
			KS_logfile_info(("ksxdr_unregister: received Manager command but no Manager here. Sending answer."));
			return OV_ERR_OK;
		}

		/*
		 * issue unregister command
		 */
		KS_logfile_debug(("ksxdr_unregister: unregistering KSXDR for %s (version %lu)", servername, serverversion));
		result = ksbase_Manager_unregister(servername, serverversion, KSXDR_IDENTIFIER);
		if(Ov_Fail(result))
		{
			if(result == OV_ERR_BADVALUE) /*	server not found	*/
				*ksErrCode = KS_ERR_SERVERUNKNOWN;
			else
			{
				*msgState = XDR_MSGST_SYSTEM_ERR;
				*ksErrCode = result;
			}
			KS_logfile_debug(("unregister for %s failed with code %0#6x", servername, result));
		}

	}
	/*	create Answer	*/
	/*	this service only sends back the error code (after completion KS_ERR_OK)	*/
	return OV_ERR_OK;
}
