
/******************************************************************************
 *
 *   FILE
 *   ----
 *   TCPChannel.c
 *
 *   History
 *   -------
 *   2013-01-17   File created
 *
 *******************************************************************************
 *
 *   This file is generated by the 'acplt_builder' command
 *
 ******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_TCPbind
#define OV_COMPILE_LIBRARY_TCPbind
#endif


#include "TCPbind.h"
#include "libov/ov_macros.h"
#include "libov/ov_malloc.h"
#include "libov/ov_result.h"
#include "libov/ov_memstack.h"
#include "TCPbind_helper.h"
#include "ks_logfile.h"
#include "TCPbind_config.h"
#include "ksbase_helper.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#if !OV_SYSTEM_NT
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
/* for select */
#include <sys/select.h>
#else
#include <winsock2.h>
#endif


OV_DLLFNCEXPORT OV_INT TCPbind_TCPChannel_socket_get(
		OV_INSTPTR_TCPbind_TCPChannel          pobj
) {
	return pobj->v_socket;
}

OV_DLLFNCEXPORT OV_RESULT TCPbind_TCPChannel_socket_set(
		OV_INSTPTR_TCPbind_TCPChannel          pobj,
		const OV_INT  value
) {
	OV_INT socket;

	socket = TCPbind_TCPChannel_socket_get(pobj);
	if(socket >= 0 && socket != value)
	{
		CLOSE_SOCKET(socket);
		ks_logfile_debug("TCPChannel/socket %s closing socket %d", pobj->v_identifier, socket);
	}
	pobj->v_socket = value;

	//activate typemethod if a socket is there
	if(value >= 0)
		pobj->v_actimode = 1;

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT TCPbind_TCPChannel_SendData(
		OV_INSTPTR_ksbase_Channel this
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_TCPbind_TCPChannel thisCh = Ov_StaticPtrCast(TCPbind_TCPChannel, this);
	OV_INT socket = -1;
	fd_set write_flags;
	struct timeval waitd;
	int err = 0;
	OV_UINT sentChunkSize;
	OV_UINT sendlength = 0;

	if(thisCh->v_outData.length)	//check if there is data to send
	{
		socket = TCPbind_TCPChannel_socket_get(thisCh);

		//Check if socket is ok
		if (socket < 0 || thisCh->v_ConnectionState == TCPbind_CONNSTATE_CLOSED) { // check if the socket might be OK.
			ks_logfile_debug("%s/SendData: no socket set, nothing sent",this->v_identifier);
			thisCh->v_ConnectionState = TCPbind_CONNSTATE_CLOSED;
			return OV_ERR_GENERIC;
		}

		//send one chunk

		// Zero the flags ready for using
		FD_ZERO(&write_flags);
		FD_SET(socket, &write_flags); // get write flags
		waitd.tv_sec = 0;     // Set Timeout
		waitd.tv_usec = 1000;    //  1 millisecond
		err = select(socket + 1, (fd_set*) 0,&write_flags, (fd_set*)0,&waitd);

		ks_logfile_debug("select returned: %d; line %d", err, __LINE__);

		//determine how many bytes have to be sent
		sendlength = thisCh->v_outData.length;
		sendlength -= (thisCh->v_outData.readPT - thisCh->v_outData.data);

		if(!sendlength) //nothing more to send
		{
			ksbase_free_KSDATAPACKET(&(thisCh->v_outData));
			return OV_ERR_OK;
		}
			//issue send command
		sentChunkSize = send(socket, (char*)thisCh->v_outData.readPT, sendlength, 0);
		if (sentChunkSize == -1)
		{
			ks_logfile_error("%s: send() failed", thisCh->v_identifier);
			return OV_ERR_GENERIC;
		}

		thisCh->v_outData.writePT += sentChunkSize;

		if((thisCh->v_outData.writePT - thisCh->v_outData.data) >= thisCh->v_outData.length)
			ksbase_free_KSDATAPACKET(&(thisCh->v_outData));
	}
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT void TCPbind_TCPChannel_startup(
		OV_INSTPTR_ov_object 	pobj
) {
	/*
	 *   local variables
	 */

	/* do what the base class does first */
	ksbase_Channel_startup(pobj);

	/* do what */
	//get called every 5th cycle before receiving first data (we have some time here)
	Ov_StaticPtrCast(TCPbind_TCPChannel, pobj)->v_cycInterval = 5;

	//set time of creation of the connection
	ov_time_gettime(&(Ov_StaticPtrCast(TCPbind_TCPChannel, pobj)->v_LastReceiveTime));

	return;
}

OV_DLLFNCEXPORT void TCPbind_TCPChannel_shutdown(
		OV_INSTPTR_ov_object 	pobj
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_TCPbind_TCPChannel this = Ov_StaticPtrCast(TCPbind_TCPChannel, pobj);
	int socket;
	/* do what */

	ks_logfile_debug("tcpclient/shutdown: %s", pobj->v_identifier);
	socket = TCPbind_TCPChannel_socket_get(this);
	if(socket >= 0)
	{
		CLOSE_SOCKET(socket);
		ks_logfile_debug("TCPChannel/shutdown %s closing socket %d", pobj->v_identifier, socket);
		TCPbind_TCPChannel_socket_set(this, -1);
		this->v_ConnectionState = TCPbind_CONNSTATE_CLOSED;
	}

	ksbase_Channel_shutdown(pobj);
	return;

}

OV_DLLFNCEXPORT void TCPbind_TCPChannel_typemethod (
		OV_INSTPTR_ksbase_ComTask	this
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_TCPbind_TCPChannel thisCh = Ov_StaticPtrCast(TCPbind_TCPChannel, this);
	OV_INSTPTR_ksbase_ClientHandler pClientHandler = NULL;
	OV_VTBLPTR_ksbase_ClientHandler pVTBLClientHandler = NULL;
	OV_INSTPTR_ksbase_DataHandler pDataHandler = NULL;
	OV_VTBLPTR_ksbase_DataHandler pVTBLDataHandler = NULL;
	OV_INT socket = -1;
	fd_set read_flags;
	struct timeval waitd;
	int err = 0;
	OV_TIME now;
	OV_TIME_SPAN tstemp;
	OV_TIME ttemp;
	OV_RESULT result;
	OV_BYTE* tempdata = NULL;

	ks_logfile_debug("TCPChannel typemethod called ");

	/*******************************************************************************************************************************************************
	 *	check timeouts
	 ******************************************************************************************************************************************************/
	ov_time_gettime(&now);

	//TimeOut of connection
	tstemp.secs = thisCh->v_ConnectionTimeOut;
	tstemp.usecs = 0;
	ov_time_add(&ttemp, &(thisCh->v_LastReceiveTime), &tstemp);
	if((thisCh->v_ConnectionState == TCPbind_CONNSTATE_OPEN) && (ov_time_compare(&now, &ttemp) == OV_TIMECMP_AFTER))
	{
		if(thisCh->v_ClientHandlerAssociated == TCPbind_CH_NOTNEEDED)
		{
			ks_logfile_info("%s: received nothing for %u seconds. Closing connection.", this->v_identifier, thisCh->v_ConnectionTimeOut);
			CLOSE_SOCKET(socket);
			TCPbind_TCPChannel_socket_set(thisCh, -1);
			thisCh->v_ConnectionState = TCPbind_CONNSTATE_CLOSED;
		}
		else
		{
			ks_logfile_info("%s: received nothing for %u seconds. Deleting channel.", this->v_identifier, thisCh->v_ConnectionTimeOut);
			Ov_DeleteObject(Ov_StaticPtrCast(ov_object, this));
		}
	}

	//Timeout of Data in inData (in Buffer)
	tstemp.secs = thisCh->v_UnusedDataTimeOut;
	tstemp.usecs = 0;
	ov_time_add(&ttemp, &(thisCh->v_LastReceiveTime), &tstemp);
	if((thisCh->v_ConnectionState == TCPbind_CONNSTATE_CLOSED)
			|| ((thisCh->v_ConnectionState == TCPbind_CONNSTATE_OPEN) && (ov_time_compare(&now, &ttemp) == OV_TIMECMP_AFTER)))
	{
		ks_logfile_info("%s: received nothing for %u seconds. Deleting inData.", this->v_identifier, thisCh->v_UnusedDataTimeOut);
		ksbase_free_KSDATAPACKET(&(thisCh->v_inData));
	}

	/**********************************************************************************************************************************************************
	 *	Associate ClientHandler if needed
	 *********************************************************************************************************************************************************/
	if(thisCh->v_ClientHandlerAssociated == TCPbind_CH_NOTASSOCATIED)
	{
		TCPbind_TCPChannel_AssociateClientHandler(thisCh);
	}

	/***********************************************************************************************************************************************************************************
	 *	Handle incoming data
	 **********************************************************************************************************************************************************************************/

	socket = TCPbind_TCPChannel_socket_get(thisCh);

	if (socket < 0 || thisCh->v_ConnectionState == TCPbind_CONNSTATE_CLOSED) { // check if the socket might be OK.
		ks_logfile_debug("%s/typemethod: no socket set, disabling typemethod",this->v_identifier);
		this->v_actimode = 0;
		thisCh->v_ConnectionState = TCPbind_CONNSTATE_CLOSED;
		return;
	}

	//receive data in chunks (we dont know how much it will be)
	do
	{
		FD_ZERO(&read_flags);
		FD_SET(socket, &read_flags); // get read flags
		waitd.tv_sec = 0;     // Set Timeout
		waitd.tv_usec = 0;    //  do not wait
		err = select(socket + 1, &read_flags, (fd_set*) 0, (fd_set*)0, &waitd);

		ks_logfile_debug("select returned: %d; line %d", err, __LINE__);

		//check if data arrived
		if((err > 0) && FD_ISSET(socket, &read_flags))
		{
			//Data arrived
			//reallocate memory for receiving data. Note the temp-pointer: if realloc fails, the original pointer is NOT freed
			tempdata = ov_realloc(thisCh->v_inData.data, thisCh->v_inData.length + TCPbind_CHUNKSIZE);
			if(!tempdata)
			{
				ks_logfile_error("%s: failed to allocate memory for received data (length: %u)", this->v_identifier, thisCh->v_inData.length+TCPbind_CHUNKSIZE);
				ksbase_free_KSDATAPACKET(&(thisCh->v_inData));
				return;
			}
			else
				thisCh->v_inData.data = tempdata;

			err = recv(socket, (char*) thisCh->v_inData.writePT, TCPbind_CHUNKSIZE, 0);		//receive data
			if(err < TCPbind_CHUNKSIZE)
			{
				if(err == 0)
				{
					ks_logfile_debug("%s: nothing received. connection was gracefully closed", this->v_identifier);
					thisCh->v_ConnectionState = TCPbind_CONNSTATE_CLOSED;
					break;
				}
				else if (err == -1)
				{
					ks_logfile_error("%s: error receiving. Deleting data and closing socket.", this->v_identifier);
					ksbase_free_KSDATAPACKET(&(thisCh->v_inData));
					CLOSE_SOCKET(socket);
					TCPbind_TCPChannel_socket_set(thisCh, -1);
					thisCh->v_ConnectionState = TCPbind_CONNSTATE_CLOSED;
					return;
				}

			}

			//update data length
			thisCh->v_inData.length += err;
			//move writept to end of data
			thisCh->v_inData.writePT += err;

		}
	}while(FD_ISSET(socket, &read_flags));

	//update receivetime
	ov_time_gettime(&(thisCh->v_LastReceiveTime));

	/*****************************************************************************************************************************************************************************
	 *	Process received data
	 ****************************************************************************************************************************************************************************/

	if(thisCh->v_ClientHandlerAssociated == TCPbind_CH_ASSOCIATED)
	{	//there is a ClientHandler associated. Call its HandleRequest function.
		pClientHandler = Ov_GetChild(ksbase_AssocChannelClientHandler, thisCh);
		if(pClientHandler)
		{
			Ov_GetVTablePtr(ksbase_ClientHandler, pVTBLClientHandler, pClientHandler);
			if(pVTBLClientHandler)
			{
				ks_logfile_debug("%s: handing over data to %s to handle it.", thisCh->v_identifier, pClientHandler->v_identifier);
				result = pVTBLClientHandler->m_HandleRequest(pClientHandler, &(thisCh->v_inData), &(thisCh->v_outData));
				if(Ov_Fail(result))
				{
					ov_memstack_lock();
					ks_logfile_error("%s: processing of received data by %s failed: %s", thisCh->v_identifier, pClientHandler->v_identifier, ov_result_getresulttext(result));
					ov_memstack_unlock();
					ksbase_free_KSDATAPACKET(&(thisCh->v_inData));
					ksbase_free_KSDATAPACKET(&(thisCh->v_outData));
					return;
				}
				else
				{
					TCPbind_TCPChannel_SendData(Ov_StaticPtrCast(ksbase_Channel, thisCh));
					return;
				}
			}
			else
			{
				ks_logfile_error("%s: no Vtable found for ClientHandler %s. Shutting down", thisCh->v_identifier, pClientHandler->v_identifier);
				Ov_DeleteObject(Ov_StaticPtrCast(ov_object, thisCh));
				return;
			}
		}
		else
		{
			ks_logfile_error("%s: no ClientHandler associated. Shutting down", thisCh->v_identifier);
			Ov_DeleteObject(Ov_StaticPtrCast(ov_object, thisCh));
			return;
		}
	}
	else if(thisCh->v_ClientHandlerAssociated == TCPbind_CH_NOTNEEDED)	//if acting on the client side, check if a "callback" has to be done
	{
		pDataHandler = Ov_GetChild(ksbase_AssocChannelDataHandler, thisCh);
		if(pDataHandler)
		{	//there is a DataHandler --> call its HandleData Function
			Ov_GetVTablePtr(ksbase_DataHandler, pVTBLDataHandler, pDataHandler);
			if(pVTBLDataHandler)
			{
				ks_logfile_debug("%s: handing over data to %s to handle it.", thisCh->v_identifier, pDataHandler->v_identifier);
				result = pVTBLDataHandler->m_HandleData(pDataHandler, &(thisCh->v_inData), &(thisCh->v_outData));
				if(Ov_Fail(result))
				{
					ov_memstack_lock();
					ks_logfile_error("%s: processing of received data by %s failed: %s", thisCh->v_identifier, pDataHandler->v_identifier, ov_result_getresulttext(result));
					ov_memstack_unlock();
					ksbase_free_KSDATAPACKET(&(thisCh->v_inData));
					ksbase_free_KSDATAPACKET(&(thisCh->v_outData));
					return;
				}
				else
				{
					TCPbind_TCPChannel_SendData(Ov_StaticPtrCast(ksbase_Channel, thisCh));
					return;
				}
			}
			else
			{
				ks_logfile_error("%s: no Vtable found for DataHandler %s. Shutting down", thisCh->v_identifier, pDataHandler->v_identifier);
				Ov_DeleteObject(Ov_StaticPtrCast(ov_object, thisCh));
				return;
			}
		}
	}

	/*****************************************************************************************************************************************************************************
	 * Send data if there is still something in the buffer
	 ****************************************************************************************************************************************************************************/
	if(thisCh->v_outData.length)
		TCPbind_TCPChannel_SendData(Ov_StaticPtrCast(ksbase_Channel, thisCh));

	return;
}

OV_DLLFNCEXPORT OV_RESULT TCPbind_TCPChannel_OpenConnection(
		OV_INSTPTR_TCPbind_TCPChannel this,
		OV_STRING host,
		OV_INT port,
		OV_INT* socket
) {

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT TCPbind_TCPChannel_AssociateClientHandler(
		OV_INSTPTR_TCPbind_TCPChannel this
) {
	OV_INSTPTR_ov_class pClassProtIdent = NULL;
	OV_INSTPTR_ksbase_ProtocolIdentificator pProtIdent = NULL;
	OV_VTBLPTR_ksbase_ProtocolIdentificator pVTBLProtIdent = NULL;
	OV_RESULT result;

	//iterate over all classes derived from ksbase_ProtocolIdentificator
	pClassProtIdent = (Ov_StaticPtrCast(ov_class, Ov_GetFirstChild(ov_inheritance, pclass_ksbase_ProtocolIdentificator)));
	while(pClassProtIdent)
	{
		//iterate over all instances of the identificator class
		pProtIdent = Ov_StaticPtrCast(ksbase_ProtocolIdentificator, Ov_GetFirstChild(ov_instantiation, pClassProtIdent));
		while(pProtIdent)
		{
			//get VTable of protocol identificator
			Ov_GetVTablePtr(ksbase_ProtocolIdentificator, pVTBLProtIdent, pProtIdent);
			if(!pVTBLProtIdent)
			{
				ks_logfile_error("Could not determine VTable of ProtocolIdentificator %s - Cancelling operation.", pProtIdent->v_identifier);
				return OV_ERR_NOTIMPLEMENTED;
			}
			else
			{
				//check if protocol is recognized by this Identificator
				if(pVTBLProtIdent->m_identify(pProtIdent, Ov_StaticPtrCast(ksbase_Channel, this)))
				{	//if so, create ClientHandler
					ks_logfile_debug("Protocol identified by %s. Creating Clienthandler", pProtIdent->v_identifier);
					result = pVTBLProtIdent->m_createClientHandler(pProtIdent, Ov_StaticPtrCast(ksbase_Channel, this));
					if(Ov_Fail(result))
					{
						ov_memstack_lock();
						ks_logfile_error("ClientHandler could not be created. Reason: %s", ov_result_getresulttext(result));
						ov_memstack_unlock();
						return result;
					}
					ks_logfile_debug("ClientHandler created.");
					this->v_ClientHandlerAssociated = TCPbind_CH_ASSOCIATED;
					this->v_ConnectionTimeOut = TCPbind_TTL_AFTER_ASSOC;
					break;
				}
			}
			pProtIdent = Ov_StaticPtrCast(ksbase_ProtocolIdentificator, Ov_GetNextChild(ov_instantiation, pProtIdent));
		}
		//if ClientHandler could be Associated, do not go on
		if(this->v_ClientHandlerAssociated == TCPbind_CH_ASSOCIATED)
			break;
		else
			pClassProtIdent = Ov_StaticPtrCast(ov_class, Ov_GetNextChild(ov_inheritance, pClassProtIdent));
	}
	//if no ClientHanlder could be associated Delete Channel
	if(this->v_ClientHandlerAssociated == TCPbind_CH_NOTASSOCATIED)
	{
		this->v_ClientHandlerAssociated = TCPbind_CH_NOTFOUND;
		ks_logfile_error("No ClientHandler could be associated. Deleting TCPChannel");
		Ov_DeleteObject(Ov_StaticPtrCast(ov_object, this));
	}
	return OV_ERR_OK;
}

