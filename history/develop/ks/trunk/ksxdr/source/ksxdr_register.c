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

OV_RESULT ksxdr_register(const OV_UINT version, const OV_TICKET* pticket, KS_DATAPACKET* dataReceived, KS_DATAPACKET* serviceAnswer, OV_UINT* msgState, OV_UINT* ksErrCode)
{
	OV_STRING servername = NULL;
	OV_UINT serverversion;
	OV_UINT serverport;
	OV_UINT serverttl;
	OV_INSTPTR_ksbase_Manager pManager = NULL;
	char portstr [8];

	ov_memstack_lock();
	if(Ov_Fail(KS_DATAPACKET_read_xdr_string_tomemstack(dataReceived, &servername, KS_NAME_MAXLEN))
			|| Ov_Fail(KS_DATAPACKET_read_xdr_uint(dataReceived, &serverversion))
			|| Ov_Fail(KS_DATAPACKET_read_xdr_uint(dataReceived, &serverport))
			|| Ov_Fail(KS_DATAPACKET_read_xdr_uint(dataReceived, &serverttl)))
	{
		KS_logfile_error(("ksxdr_register: Error decoding Register message. Sending answer."));
		*msgState = XDR_MSGST_GARBAGE_ARGS;
		*ksErrCode = KS_ERR_TARGETGENERIC;
		if(servername)
			ov_free(servername);
		ov_memstack_unlock();
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
			KS_logfile_info(("ksxdr_register: received Manager command but no Manager here. Sending answer."));
			ov_memstack_unlock();
			return OV_ERR_OK;
		}

		sprintf(portstr, "%lu", serverport);
		if(Ov_Fail(ksbase_Manager_register(servername, serverversion, KSXDR_IDENTIFIER, portstr, serverttl)))
		{
			KS_logfile_info(("ksxdr_register: could not register server %s with port %s at manager. Sending answer: SYSTEM_ERR.", servername, portstr));
			*msgState = XDR_MSGST_SYSTEM_ERR;
		}

	}
	/*	create Answer	*/
	/*	this service only sends back the error code (after completion KS_ERR_OK)	*/
	ov_memstack_unlock();
	return OV_ERR_OK;
}
