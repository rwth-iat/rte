/*
 * ksxdr_getserver.c
 *
 *  Created on: 12.02.2013
 *      Author: lars
 */

/*	the following defines reduce the loglevel to warning and error for this file
 * As soon as the development phase is over these lines should be deleted and the loglevel should be reduced to warning and error globally in the Makefile
 * */
#if LOG_KS
#undef LOG_KS
#define LOG_KS 0
#endif

#if LOG_KS_DEBUG
#undef LOG_KS_DBUG
#define LOG_KS_DEBUG 0
#endif

#if LOG_KS_INFO
#undef LOG_KS_INFO
#define LOG_KS_INFO 0
#endif

#if !LOG_KS_WARNING
#ifdef LOG_KS_WARNING
#undef LOG_KS_WARNING
#endif
#define LOG_KS_WARNING 1
#endif

#if !LOG_KS_ERROR
#ifdef LOG_KS_ERROR
#undef LOG_KS_ERROR
#endif
#define LOG_KS_ERROR 1
#endif

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

	if(Ov_Fail(KS_DATAPACKET_read_xdr_string(dataReceived, &servername, KS_NAME_MAXLEN))
			|| Ov_Fail(KS_DATAPACKET_read_xdr_u_long(dataReceived, &serverversion))
			|| Ov_Fail(KS_DATAPACKET_read_xdr_u_long(dataReceived, &serverport))
			|| Ov_Fail(KS_DATAPACKET_read_xdr_u_long(dataReceived, &serverttl)))
	{
		KS_logfile_error(("ksxdr_register: Error decoding Register message. Sending answer."));
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
			KS_logfile_info(("ksxdr_register: received Manager command but no Manager here. Sending answer."));
			return OV_ERR_OK;
		}

		sprintf(portstr, "%u", serverport);
		if(Ov_Fail(ksbase_Manager_register(servername, serverversion, KSXDR_IDENTIFIER, portstr, serverttl)))
		{
			*msgState = XDR_MSGST_SYSTEM_ERR;
		}

	}
	/*	create Answer	*/
	/*	this service only sends back the error code (after completion KS_ERR_OK)	*/
	return OV_ERR_OK;
}
