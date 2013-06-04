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

OV_RESULT ksxdr_getserver(const OV_UINT version, const OV_TICKET* pticket, KS_DATAPACKET* dataReceived, KS_DATAPACKET* serviceAnswer, OV_UINT* msgState, OV_UINT* ksErrCode)
{
	OV_STRING servername = NULL;
	OV_UINT serverversion;
	OV_INSTPTR_ksbase_Manager pManager = NULL;
	OV_STRING_VEC protocols = {0, NULL};
	OV_STRING_VEC ports = {0, NULL};
	OV_UINT	regTTl;
	OV_TIME ExpTime;
	OV_INT registeredVersion;
	OV_UINT i = 0;
	OV_UINT xdr_port;
	OV_TIME ttemp;
	OV_RESULT result;


	if(Ov_Fail(KS_DATAPACKET_read_xdr_string(dataReceived, &servername, KS_NAME_MAXLEN))
			|| Ov_Fail(KS_DATAPACKET_read_xdr_u_long(dataReceived, &serverversion)))
	{
		KS_logfile_error(("ksxdr_getserver: Error decoding getserver message. Sending answer."));
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
			KS_logfile_info(("ksxdr_getserver: received Manager command but no Manager here. Sending answer."));
			return OV_ERR_OK;
		}

		/*
		 * check if there is a server of the given name and version of the ksxdr protocol (or the whole server if it is the only supported protocol)
		 */
		KS_logfile_debug(("ksxdr_getserver: getserver: getting server data."));
		result = ksbase_Manager_getserverdata(servername, serverversion, &protocols, &ports, &regTTl, &ExpTime, &registeredVersion);
		if(Ov_OK(result))
		{	//server exists, check if ks protocol is supported
			KS_logfile_debug(("ksxdr_getserver: getserver: server exists."));

			for(i=0; i<protocols.veclen; i++)
				if(ov_string_compare(protocols.value[i], KSXDR_IDENTIFIER) == OV_STRCMP_EQUAL)
					break;

			if(i<protocols.veclen)
			{	/*	ksxdr supported	*/
				KS_logfile_debug(("ksxdr_getserver: getserver: ksxdr supported."));
				xdr_port = atoi(ports.value[i]);
				Ov_SetDynamicVectorLength(&protocols, 0, STRING);
				Ov_SetDynamicVectorLength(&ports, 0, STRING);
			}
			else
			{	/*	ksxdr not supported	*/
				KS_logfile_debug(("ksxdr_getserver: getserver: ksxdr not supported."));
				Ov_SetDynamicVectorLength(&protocols, 0, STRING);
				Ov_SetDynamicVectorLength(&ports, 0, STRING);
				ov_free(servername);
				*msgState = XDR_MSGST_SUCCESS;
				*ksErrCode = KS_ERR_SERVERUNKNOWN;
				return OV_ERR_OK;
			}

		}
		else
		{
			if(result == KS_ERR_SERVERUNKNOWN)
			{
				Ov_SetDynamicVectorLength(&protocols, 0, STRING);
				Ov_SetDynamicVectorLength(&ports, 0, STRING);
				ov_free(servername);
				*msgState = XDR_MSGST_SUCCESS;
				*ksErrCode = KS_ERR_SERVERUNKNOWN;
				return OV_ERR_OK;
			}
			else
			{/*	not all values set...how can this happen?	*/
				ov_free(servername);
				*msgState = XDR_MSGST_SYSTEM_ERR;
				*ksErrCode = KS_ERR_TARGETGENERIC;
				KS_logfile_error(("ksxdr_getserver: getserver server: weird: getserverdata returned unknown error. i don't know how this can happen."));
				return OV_ERR_BADPARAM;
			}
		}

	}
	/*	create Answer	*/
	/*	servername	*/
	KS_DATAPACKET_write_xdr_string(serviceAnswer, &servername);
	/*	version	*/
	KS_DATAPACKET_write_xdr_long(serviceAnswer, &registeredVersion);
	/*	port	*/
	KS_DATAPACKET_write_xdr_u_long(serviceAnswer, &xdr_port);
	/*	Expiration Time	*/
	KS_DATAPACKET_write_xdr_u_long(serviceAnswer, &ExpTime.secs);
	KS_DATAPACKET_write_xdr_u_long(serviceAnswer, &ExpTime.usecs);
	/*	set living-state	*/
	ov_time_gettime(&ttemp);
	if(ov_time_compare(&ttemp, &ExpTime) == OV_TIMECMP_BEFORE)
	{
		i=1;	/*	temporary used for server state: 1 means server is alive*/
	}
	else
		i=2;	/*	server state 2: means probably dead	*/

	KS_DATAPACKET_write_xdr_u_long(serviceAnswer, &i);

	ov_free(servername);
	return OV_ERR_OK;

}
