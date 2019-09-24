
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
#include "ov_macros.h"
#include "ov_malloc.h"
#include "ov_result.h"
#include "ov_memstack.h"
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
#include <netdb.h>
#include <fcntl.h>
#include <netinet/tcp.h>
/*	defines if not defined in netdb.h	*/
#ifndef NI_MAXHOST
#define NI_MAXHOST 1025
#endif
#ifndef NI_MAXSERV
#define NI_MAXSERV 32
#endif
/* for select */
#include <sys/select.h>
#else
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x501
#include <winsock2.h>
#include <ws2tcpip.h>
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
	OV_INT activesocket;
	TCPBIND_SOCKET socket = TCPBIND_INVALID_SOCKET;

	activesocket = TCPbind_TCPChannel_socket_get(pobj);
	if(activesocket >= 0 && activesocket != value)
	{
		OV_TCPBIND_SETINT2SOCKET(activesocket, socket);
		TCPBIND_CLOSE_SOCKET(socket);
		KS_logfile_debug(("TCPChannel/socket %s closing socket %d", pobj->v_identifier, activesocket));
	}
	pobj->v_socket = value;
	if(pobj->v_socket < 0)
		pobj->v_ConnectionState = TCPbind_CONNSTATE_CLOSED;

	//activate typemethod if a socket is there
	if(value >= 0)
		pobj->v_actimode = 1;

	return OV_ERR_OK;
}

/**
 * finishes opening a connection
 * when the getaddressinfo has succeeded open the connection etc.
 * v_addrInfo and v_addrInfoReq are freed and NULLED afterwards
 */
OV_DLLFNCEXPORT OV_RESULT TCPbind_TCPChannel_OpenConnection_afterAddrinfo(
		OV_INSTPTR_TCPbind_TCPChannel thisTCPCh
) {
	struct sockaddr_storage sa_stor;
	socklen_t sas = sizeof(sa_stor);
	struct sockaddr* sa = (struct sockaddr*) &sa_stor;
	char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
	int flags = NI_NUMERICHOST | NI_NUMERICSERV;
	int opt_on = 1;

	KS_logfile_debug(("file %s\nline %u:\tentering OpenConnection_afterAddrinfo", __FILE__, __LINE__));


	if(thisTCPCh->v_addrInfoReq){
		if (thisTCPCh->v_addrInfoReq->socket == TCPBIND_INVALID_SOCKET)
		{
			KS_logfile_info(("%s: could not establish connection", thisTCPCh->v_identifier));
			thisTCPCh->v_ConnectionState = TCPbind_CONNSTATE_COULDNOTOPEN;
			TCPbind_aresWorker_delGetAddrInfoElem(thisTCPCh->v_addrInfoReq);	//	deletion is scheduled, not done yet
			thisTCPCh->v_addrInfoReq = NULL;
			return OV_ERR_GENERIC;
		}

		//resolve connected peer
		if(getpeername(thisTCPCh->v_addrInfoReq->socket, sa, &sas) == TCPBIND_SOCKET_ERROR)
		{
			KS_logfile_error(("%s: could not resolve connected peer", thisTCPCh->v_identifier));
			thisTCPCh->v_ConnectionState = TCPbind_CONNSTATE_COULDNOTOPEN;
			TCPbind_aresWorker_delGetAddrInfoElem(thisTCPCh->v_addrInfoReq);	//	deletion is scheduled, not done yet
			thisTCPCh->v_addrInfoReq = NULL;
			return OV_ERR_GENERIC;
		}

		// resolve peername
		if(getnameinfo( sa, sas, hbuf, sizeof(hbuf), sbuf, sizeof(sbuf), flags) != 0)
		{
			KS_logfile_error(("%s: could not resolve connected peer's address", thisTCPCh->v_identifier));
			thisTCPCh->v_ConnectionState = TCPbind_CONNSTATE_COULDNOTOPEN;
			TCPbind_aresWorker_delGetAddrInfoElem(thisTCPCh->v_addrInfoReq);	//	deletion is scheduled, not done yet
			thisTCPCh->v_addrInfoReq = NULL;
			return OV_ERR_GENERIC;
		}

		ov_string_setvalue(&(thisTCPCh->v_address), hbuf);
		thisTCPCh->v_ConnectionState = TCPbind_CONNSTATE_OPEN;
		TCPbind_TCPChannel_socket_set(thisTCPCh, thisTCPCh->v_addrInfoReq->socket);

		KS_logfile_debug(("%s: connected to %s.", thisTCPCh->v_identifier, thisTCPCh->v_address));
		//set time of Opening the Connection
		ov_time_gettime(&(thisTCPCh->v_LastReceiveTime));

		//disable nagle for the receivesocket
		(void)setsockopt(thisTCPCh->v_addrInfoReq->socket, IPPROTO_TCP, TCP_NODELAY, (char *)&opt_on, sizeof(opt_on));
		TCPbind_aresWorker_delGetAddrInfoElem(thisTCPCh->v_addrInfoReq);	//	deletion is scheduled, not done yet
		thisTCPCh->v_addrInfoReq = NULL;
	}
	return OV_ERR_OK;
}

/**
 * The TCHChannel SendData function is used to trigger the sending of the outData buffer.
 * It takes a pointer to the Channel itself as parameter.
 * The SendData function will to send the complete buffer. If the buffer can be send only partially (to much data or some other reason) the rest of the outData buffer will be send by the typemethod.
 *
 */

OV_DLLFNCEXPORT OV_RESULT TCPbind_TCPChannel_SendData(
		OV_INSTPTR_ksbase_Channel this
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_TCPbind_TCPChannel thisCh = Ov_StaticPtrCast(TCPbind_TCPChannel, this);
	TCPBIND_SOCKET socket = TCPBIND_INVALID_SOCKET;
	fd_set write_flags;
	struct timeval waitd;
	int err_select = 0;
#ifdef OV_SYSTEM_NT
	int sentChunkSize;
#else
	ssize_t sentChunkSize;
#endif
	OV_UINT sendlength = 0;

	if(thisCh->v_outData.length)	//check if there is data to send
	{
		if(thisCh->v_ConnectionState == TCPbind_CONNSTATE_OPEN){
			if(!thisCh->v_outData.readPT)
				thisCh->v_outData.readPT = thisCh->v_outData.data;
			socket = TCPbind_TCPChannel_socket_get(thisCh);

			//Check if socket is ok
			if (socket == TCPBIND_INVALID_SOCKET || thisCh->v_ConnectionState == TCPbind_CONNSTATE_CLOSED) { // check if the socket might be OK.
				KS_logfile_info(("%s/SendData: no socket set, nothing sent",this->v_identifier));
				thisCh->v_ConnectionState = TCPbind_CONNSTATE_CLOSED;
				return OV_ERR_GENERIC;
			}

			//send one chunk

			// Zero the flags ready for using
			FD_ZERO(&write_flags);
			FD_SET(socket, &write_flags); // get write flags
			waitd.tv_sec = 0;     // Set Timeout
			waitd.tv_usec = 0;    //  do not wait
			err_select = select(socket + 1, (fd_set*) 0,&write_flags, (fd_set*)0,&waitd);

			if (err_select == TCPBIND_SOCKET_ERROR) {
				KS_logfile_error(("%s: SendData: select returned: %d; line %d", thisCh->v_identifier, err_select, __LINE__));
				thisCh->v_ConnectionState = TCPbind_CONNSTATE_CLOSED;
				return OV_ERR_GENERIC;
			}

			//determine how many bytes have to be sent
			sendlength = thisCh->v_outData.length;
			sendlength -= (thisCh->v_outData.readPT - thisCh->v_outData.data);
			//DEBUG
			//KS_logfile_debug(("%s SendData: outData.length:\t%u\n\toutData.retPT\t%p\n\toutData.data\t%p", thisCh->v_identifier, thisCh->v_outData.length, thisCh->v_outData.readPT,thisCh->v_outData.data));
			//DEBUG_END
			if(sendlength <= 0) //nothing more to send
			{
				ksbase_free_KSDATAPACKET(&(thisCh->v_outData));
				KS_logfile_debug(("no more send needed"));
				return OV_ERR_OK;
			}
			//issue send command
			KS_logfile_debug(("%s SendData: have to send %" OV_PRINT_UINT " bytes", thisCh->v_identifier, sendlength));
			sentChunkSize = send(socket, (char*)thisCh->v_outData.readPT, sendlength, 0);
			if (sentChunkSize == TCPBIND_SOCKET_ERROR){
				KS_logfile_error(("%s: send() failed...", thisCh->v_identifier));
				ks_logfile_print_sysMsg();
				thisCh->v_ConnectionState = TCPbind_CONNSTATE_SENDERROR;
				return OV_ERR_GENERIC;
			}

			thisCh->v_outData.readPT += sentChunkSize;

			KS_logfile_debug(("%s: sent %" OV_PRINT_UINT " bytes", this->v_identifier, sentChunkSize));

			if((thisCh->v_outData.readPT - thisCh->v_outData.data) >= (OV_INT) thisCh->v_outData.length)
			{
				ksbase_free_KSDATAPACKET(&(thisCh->v_outData));
				KS_logfile_debug(("%s: everything sent", thisCh->v_identifier));
			}
		}
	}
	//set time of sending
	ov_time_gettime(&(thisCh->v_LastReceiveTime));
	thisCh->v_actimode = 1;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT void TCPbind_TCPChannel_startup(
		OV_INSTPTR_ov_object 	pobj
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_TCPbind_TCPChannel this = Ov_StaticPtrCast(TCPbind_TCPChannel, pobj);
	int socket;
	/* do what the base class does first */
	ksbase_Channel_startup(pobj);

	/* do what */
	Ov_StaticPtrCast(TCPbind_TCPChannel, pobj)->v_cycInterval = 1;
	Ov_StaticPtrCast(TCPbind_TCPChannel, pobj)->v_usesStreamProtocol = TRUE;	/*	TCP is a stream oriented protocol	*/

	socket = TCPbind_TCPChannel_socket_get(this);
	if(socket >= 0)
	{
		KS_logfile_info(("TCPChannel/startup: %s starting with socket %d. resetting.", pobj->v_identifier, socket));
		this->v_socket = -1;
	}
	if(this->v_ConnectionState != TCPbind_CONNSTATE_CLOSED)
	{
		KS_logfile_info(("TCPChannel/startup: %s starting with connectionState not CLOSED. resetting.", pobj->v_identifier));
		this->v_ConnectionState = TCPbind_CONNSTATE_CLOSED;
	}
	if(this->v_actimode == 1)
		this->v_actimode = 0;
	this->v_addrInfoReq = NULL;
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
	OV_INSTPTR_ksbase_ClientHandler pClientHandler = NULL;
	int socket;

	KS_logfile_debug(("TCPChannel/shutdown: %s", pobj->v_identifier));
	socket = TCPbind_TCPChannel_socket_get(this);
	if(socket >= 0)
	{
		TCPBIND_CLOSE_SOCKET(socket);
		KS_logfile_debug(("TCPChannel/shutdown %s closing socket %d", pobj->v_identifier, socket));
		TCPbind_TCPChannel_socket_set(this, -1);
		this->v_ConnectionState = TCPbind_CONNSTATE_CLOSED;
	}
	if(this->v_addrInfoReq){
		ov_free(this->v_addrInfoReq);
		this->v_addrInfoReq = NULL;
	}

	/*
	 * when acting on the server side, delete associated ClientHandlers (they won't be used anymore)
	 *  and then delete oneself (connections to a server are not static)
	 */

	if(this->v_ClientHandlerAssociated == KSBASE_CH_ASSOCIATED)
	{
		pClientHandler = Ov_GetChild(ksbase_AssocChannelClientHandler, this);
		if(pClientHandler)
		{
			// activate client handler so it can delete itself
			pClientHandler->v_actimode = 1;
		}
		this->v_ClientHandlerAssociated = KSBASE_CH_NOTASSOCATIED;
	}

	/*
	 * call baseclass' shutdown
	 */
	ksbase_Channel_shutdown(pobj);
	return;

}

/**
 * The TCPChannel typemethod is run cyclically to receive data.
 * The received data is stored in the inData buffer. It is appended to the buffer. When a function has processed the buffer (or a part of it) the processed data should be deleted from it
 * to avoid unnecessary memory usage. When no data has arrived for UnusedDataTimeOut seconds the inData buffer is deleted.
 * When an open connection did not receive any data for ConnectionTimeOut seconds it is closed.
 * After data is received and stored in the inData buffer the typemethod checks if there is a ClientHandler or a DataHandler associated with this channel. If so their respective
 * functions HandleRequest or HandleData are called.
 */

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
	TCPBIND_SOCKET socket = TCPBIND_INVALID_SOCKET;
	int intsocket = -1;
	fd_set read_flags;
	struct timeval waitd;
	int err_select = 0;
#ifdef OV_SYSTEM_NT
	int err_recv = 0;
#else
	ssize_t err_recv = 0;
#endif
	OV_TIME now;
	OV_TIME_SPAN tstemp;
	OV_TIME ttemp;
	OV_RESULT result;
	OV_BYTE* tempdata = NULL;
	OV_BOOL datareceived = FALSE;
	OV_INSTPTR_ksbase_RootComTask RCTask = NULL;

	RCTask = Ov_StaticPtrCast(ksbase_RootComTask, Ov_GetFirstChild(ov_instantiation, pclass_ksbase_RootComTask));
	if(RCTask)
		Ov_Link(ksbase_AssocCurrentChannel, RCTask, Ov_StaticPtrCast(ksbase_Channel, thisCh));

#if OV_SYSTEM_NT || OV_SYSTEM_UNIX
	/*	check connection state and set as open if needed	*/
	if (thisCh->v_ConnectionState == TCPbind_CONNSTATE_OPENING){
		/*	this is an aligned variable. access should be atomic and even if not, we have no problem here as the set is atomic	*/
		if(thisCh->v_addrInfoReq->status == AIESTATUS_DONE){
			if(thisCh->v_addrInfoReq->result || Ov_Fail(TCPbind_TCPChannel_OpenConnection_afterAddrinfo(thisCh))){
				thisCh->v_actimode = 0;
				return;
			}
		}
	}
#endif

	/***********************************************************************************************************************************************************************************
	 *	Handle incoming data
	 **********************************************************************************************************************************************************************************/

	intsocket = TCPbind_TCPChannel_socket_get(thisCh);
	OV_TCPBIND_SETINT2SOCKET(intsocket, socket);
	if (socket != TCPBIND_INVALID_SOCKET && thisCh->v_ConnectionState == TCPbind_CONNSTATE_OPEN)		//if socket ok
	{
		//receive data in chunks (we dont know how much it will be)
		do
		{
			FD_ZERO(&read_flags);
			FD_SET(socket, &read_flags); // get read flags
			waitd.tv_sec = 0;     // Set Timeout

			waitd.tv_usec = 0;    //  do not wait
			err_select = select(socket + 1, &read_flags, (fd_set*) 0, (fd_set*)0, &waitd);
			if(err_select == TCPBIND_SOCKET_ERROR)
			{
				KS_logfile_debug(("%s typemethod: select returned: %d; line %d", thisCh->v_identifier, err_select, __LINE__));
			}

			//check if data arrived
			if((err_select > 0) && FD_ISSET(socket, &read_flags))
			{
				KS_logfile_debug(("%s: typemethod: realloc %u bytes", this->v_identifier, thisCh->v_inData.length + TCPbind_CHUNKSIZE));
				//Data arrived
				//reallocate memory for receiving data. Note the temp-pointer: if realloc fails, the original pointer is NOT freed
				tempdata = ov_realloc(thisCh->v_inData.data, thisCh->v_inData.length + TCPbind_CHUNKSIZE);
				if(!tempdata)
				{
					KS_logfile_error(("%s: failed to allocate memory for received data (length: %u)", this->v_identifier, thisCh->v_inData.length+TCPbind_CHUNKSIZE));
					ksbase_free_KSDATAPACKET(&(thisCh->v_inData));
					Ov_Unlink(ksbase_AssocCurrentChannel, RCTask, Ov_StaticPtrCast(ksbase_Channel, thisCh));
					return;
				}

				//if this is the first chunk of data in the packet, initialize the read pointer
				if(!thisCh->v_inData.readPT)
				{
					thisCh->v_inData.readPT = tempdata;
				}
				else
				{
					/*	get the readPT index and set readPT to this index in the newly allocated memory	*/
					thisCh->v_inData.readPT = tempdata + (thisCh->v_inData.readPT - thisCh->v_inData.data);
				}
				thisCh->v_inData.data = tempdata;
				/*	tempdata has done its job now. not used from here on	*/

				if(!thisCh->v_inData.writePT)
				{
					thisCh->v_inData.writePT = thisCh->v_inData.data;
				}
				else
				{
					thisCh->v_inData.writePT = thisCh->v_inData.data + thisCh->v_inData.length;
				}

				err_recv = recv(socket, (char*) thisCh->v_inData.writePT, TCPbind_CHUNKSIZE, 0);		//receive data
				if (err_recv == TCPBIND_SOCKET_ERROR){
					KS_logfile_debug(("%s: error receiving. Closing socket.", this->v_identifier));
					TCPBIND_CLOSE_SOCKET(socket);
					TCPbind_TCPChannel_socket_set(thisCh, -1);
					thisCh->v_ConnectionState = TCPbind_CONNSTATE_CLOSED;
					if(!thisCh->v_inData.length)	/*	nothing was received --> free memory	*/
					{/*	do a direct free in the datapacket as it frees nothing when length == 0	*/
						Ov_HeapFree(thisCh->v_inData.data);
						thisCh->v_inData.data = NULL;
					}
					/*	if we need a client handler and our inData buffer is empty --> delete channel (prevents lots of dead serverside channels in the database)	*/
					if(thisCh->v_ClientHandlerAssociated != KSBASE_CH_NOTNEEDED
							&& !thisCh->v_inData.length)
					{
						KS_logfile_debug(("%s: we are on the server side and have no data --> deleting channel", this->v_identifier));
						Ov_DeleteObject(thisCh);
					}
					else
					{
						KS_logfile_debug(("%s: Setting ConnectionTimeOut to %u.", this->v_identifier, thisCh->v_UnusedDataTimeOut));
						thisCh->v_ConnectionTimeOut = thisCh->v_UnusedDataTimeOut;
						Ov_Unlink(ksbase_AssocCurrentChannel, RCTask, Ov_StaticPtrCast(ksbase_Channel, thisCh));
					}
					return;
				}

				//last part received or closed
				if(err_recv == 0)
				{	// connection closed (0 bytes received and there is nothing in the buffer)
					KS_logfile_debug(("%s: nothing received. connection was gracefully closed", this->v_identifier));
					thisCh->v_ConnectionState = TCPbind_CONNSTATE_CLOSED;
					TCPbind_TCPChannel_socket_set(thisCh, -1);
					if(!thisCh->v_inData.length)	/*	nothing was received --> free memory	*/
					{	/*	do a direct free in the datapacket as it frees nothing when length == 0	*/
						Ov_HeapFree(thisCh->v_inData.data);
						thisCh->v_inData.data = NULL;
					}
					/*	if we need a client handler and our inData buffer is empty --> delete channel (prevents lots of dead serverside channels in the database)	*/
					if(thisCh->v_ClientHandlerAssociated != KSBASE_CH_NOTNEEDED
							&& !thisCh->v_inData.length)
					{
						KS_logfile_debug(("%s: we are on the server side and have no data --> deleting channel", this->v_identifier));
						Ov_DeleteObject(thisCh);
						return;
					}
					else
					{
						KS_logfile_debug(("%s: Setting ConnectionTimeOut to %u.", this->v_identifier, thisCh->v_UnusedDataTimeOut));
						thisCh->v_ConnectionTimeOut = thisCh->v_UnusedDataTimeOut;
						Ov_Unlink(ksbase_AssocCurrentChannel, RCTask, Ov_StaticPtrCast(ksbase_Channel, thisCh));
					}
					break;
				}

				//update data length
				thisCh->v_inData.length += err_recv;
				//move writept to end of data
				thisCh->v_inData.writePT += err_recv;

				datareceived = TRUE;
			}

		}while((err_select != TCPBIND_SOCKET_ERROR) && (err_recv != TCPBIND_SOCKET_ERROR) && FD_ISSET(socket, &read_flags));

		if(datareceived)
		{

			/**********************************************************************************************************************************************************
			 *	Associate ClientHandler if needed
			 *********************************************************************************************************************************************************/
			if(thisCh->v_ClientHandlerAssociated == KSBASE_CH_NOTASSOCATIED)
			{
				TCPbind_TCPChannel_AssociateClientHandler(thisCh);
			}

			/*****************************************************************************************************************************************************************************
			 *	Process received data
			 ****************************************************************************************************************************************************************************/

			//update receivetime
			ov_time_gettime(&(thisCh->v_LastReceiveTime));

			if(thisCh->v_ClientHandlerAssociated == KSBASE_CH_ASSOCIATED)
			{	//there is a ClientHandler associated. Call its HandleRequest function.
				pClientHandler = Ov_GetChild(ksbase_AssocChannelClientHandler, thisCh);
				if(pClientHandler)
				{
					Ov_GetVTablePtr(ksbase_ClientHandler, pVTBLClientHandler, pClientHandler);
					if(pVTBLClientHandler)
					{
						KS_logfile_debug(("%s: handing over data to %s to handle it.", thisCh->v_identifier, pClientHandler->v_identifier));
						result = pVTBLClientHandler->m_HandleRequest(pClientHandler, Ov_StaticPtrCast(ksbase_Channel, thisCh), &(thisCh->v_inData), &(thisCh->v_outData));
						if(Ov_Fail(result))
						{
							KS_logfile_error(("%s: processing of received data by %s failed: %s", thisCh->v_identifier, pClientHandler->v_identifier, ov_result_getresulttext(result)));
							ksbase_free_KSDATAPACKET(&(thisCh->v_inData));
							ksbase_free_KSDATAPACKET(&(thisCh->v_outData));
							Ov_Unlink(ksbase_AssocCurrentChannel, RCTask, Ov_StaticPtrCast(ksbase_Channel, thisCh));
							return;
						}
						else
						{
							TCPbind_TCPChannel_SendData(Ov_StaticPtrCast(ksbase_Channel, thisCh));
							Ov_Unlink(ksbase_AssocCurrentChannel, RCTask, Ov_StaticPtrCast(ksbase_Channel, thisCh));
							if((thisCh->v_CloseAfterSend == TRUE) && ((thisCh->v_outData.readPT - thisCh->v_outData.data) >= (OV_INT) thisCh->v_outData.length))
							{/*	channel should close after send and everything is sent	*/
								Ov_DeleteObject(thisCh);
							}
							return;
						}
					}
					else
					{
						KS_logfile_error(("%s: no Vtable found for ClientHandler %s. Deleting Channel.", thisCh->v_identifier, pClientHandler->v_identifier));
						Ov_DeleteObject(thisCh);
						return;
					}
				}
				else
				{
					KS_logfile_error(("%s: no ClientHandler associated. Deleting Channel.", thisCh->v_identifier));
					Ov_DeleteObject(thisCh);
					return;
				}
			}
			else if(thisCh->v_ClientHandlerAssociated == KSBASE_CH_NOTNEEDED)	//if acting on the client side, check if a "callback" has to be done
			{
				pDataHandler = Ov_GetChild(ksbase_AssocChannelDataHandler, thisCh);
				if(pDataHandler)
				{	//there is a DataHandler --> call its HandleData Function
					Ov_GetVTablePtr(ksbase_DataHandler, pVTBLDataHandler, pDataHandler);
					if(pVTBLDataHandler)
					{
						KS_logfile_debug(("%s: handing over data to %s to handle it.", thisCh->v_identifier, pDataHandler->v_identifier));
						result = pVTBLDataHandler->m_HandleData(pDataHandler, Ov_StaticPtrCast(ksbase_Channel, thisCh), &(thisCh->v_inData), &(thisCh->v_outData));
						if(Ov_Fail(result))
						{
							KS_logfile_error(("%s: processing of received data by %s failed: %s", thisCh->v_identifier, pDataHandler->v_identifier, ov_result_getresulttext(result)));
							ksbase_free_KSDATAPACKET(&(thisCh->v_inData));
							ksbase_free_KSDATAPACKET(&(thisCh->v_outData));
							Ov_Unlink(ksbase_AssocCurrentChannel, RCTask, Ov_StaticPtrCast(ksbase_Channel, thisCh));
							return;
						}
						else
						{
							TCPbind_TCPChannel_SendData(Ov_StaticPtrCast(ksbase_Channel, thisCh));
							Ov_Unlink(ksbase_AssocCurrentChannel, RCTask, Ov_StaticPtrCast(ksbase_Channel, thisCh));
							if((thisCh->v_CloseAfterSend == TRUE) && ((thisCh->v_outData.readPT - thisCh->v_outData.data) >= (OV_INT) thisCh->v_outData.length))
							{/*	channel should close after send and everything is sent	*/
								Ov_DeleteObject(thisCh);
							}
							return;
						}
					}
					else
					{
						KS_logfile_error(("%s: no Vtable found for DataHandler %s. Deleting Channel.", thisCh->v_identifier, pDataHandler->v_identifier));
						Ov_DeleteObject(thisCh);
						return;
					}
				}
			}
		}
		/*****************************************************************************************************************************************************************************
		 * Send data if there is still something in the buffer
		 ****************************************************************************************************************************************************************************/
		if(thisCh->v_outData.length)
		{
			TCPbind_TCPChannel_SendData(Ov_StaticPtrCast(ksbase_Channel, thisCh));
			if((thisCh->v_CloseAfterSend == TRUE) && ((thisCh->v_outData.readPT - thisCh->v_outData.data) >= (OV_INT) thisCh->v_outData.length))
			{/*	channel should close after send and everything is sent	*/
				Ov_DeleteObject(thisCh);
				return;
			}
		}

	}
	else
	{/*	no socket open	*/
		/*	if we need a client handler and our inData buffer is empty --> delete channel (prevents lots of dead serverside channels in the database)	*/
		if(thisCh->v_ClientHandlerAssociated != KSBASE_CH_NOTNEEDED
				&& !thisCh->v_inData.length)
		{
			KS_logfile_debug(("%s: we are on the server side and have no data --> deleting channel", this->v_identifier));
			Ov_DeleteObject(thisCh);
			return;
		}
		else
		{
			#if LOG_KS || LOG_KS_DEBUG
				if(thisCh->v_ConnectionTimeOut != thisCh->v_UnusedDataTimeOut){
					//too much noise every millisecond if no one feels responsive for this data
					KS_logfile_debug(("%s: Setting ConnectionTimeOut to %u.", this->v_identifier, thisCh->v_UnusedDataTimeOut));
				}
			#endif
			thisCh->v_ConnectionTimeOut = thisCh->v_UnusedDataTimeOut;
			Ov_Unlink(ksbase_AssocCurrentChannel, RCTask, Ov_StaticPtrCast(ksbase_Channel, thisCh));
		}
	}
	/*******************************************************************************************************************************************************
	 *	check timeouts
	 ******************************************************************************************************************************************************/
	ov_time_gettime(&now);

	//TimeOut of connection
	tstemp.secs = thisCh->v_ConnectionTimeOut;
	tstemp.usecs = 0;
	ov_time_add(&ttemp, &(thisCh->v_LastReceiveTime), &tstemp);
	if(ov_time_compare(&now, &ttemp) == OV_TIMECMP_AFTER)
	{
		if(thisCh->v_ClientHandlerAssociated == KSBASE_CH_NOTNEEDED)
		{
			if(thisCh->v_ConnectionState == TCPbind_CONNSTATE_OPEN)
			{
				KS_logfile_info(("%s: received nothing for %u seconds. Closing connection.", this->v_identifier, thisCh->v_ConnectionTimeOut));
				TCPBIND_CLOSE_SOCKET(socket);
				TCPbind_TCPChannel_socket_set(thisCh, -1);
				thisCh->v_ConnectionState = TCPbind_CONNSTATE_CLOSED;
			}
		}
		else
		{
			KS_logfile_info(("%s: received nothing for %u seconds. Deleting TCPChannel", this->v_identifier, thisCh->v_ConnectionTimeOut));
			Ov_DeleteObject(thisCh);
			return;
		}
	}

	//Timeout of Data in inData (in Buffer)
	tstemp.secs = thisCh->v_UnusedDataTimeOut;
	tstemp.usecs = 0;
	ov_time_add(&ttemp, &(thisCh->v_LastReceiveTime), &tstemp);
	if((ov_time_compare(&now, &ttemp) == OV_TIMECMP_AFTER) && thisCh->v_inData.length)
	{
		KS_logfile_info(("%s: received nothing for %u seconds. Deleting inData.", this->v_identifier, thisCh->v_UnusedDataTimeOut));
		ksbase_free_KSDATAPACKET(&(thisCh->v_inData));
	}

	Ov_Unlink(ksbase_AssocCurrentChannel, RCTask, Ov_StaticPtrCast(ksbase_Channel, thisCh));
	return;
}

OV_DLLFNCEXPORT OV_RESULT TCPbind_TCPChannel_OpenLocalConn(
		OV_INSTPTR_ksbase_Channel this,
		OV_STRING port
) {

	return TCPbind_TCPChannel_OpenConnection(this, "localhost", port);
}


/**
 * The TCPChannel OpenConnection function is used to open a socket to another server. It takes a pointer to the channel itself, and two strings representing the host address and host port.
 * It furthermore takes a pointer to an int used to return the socket selector.
 * This functions is IPv6 compatible.
 * It returns OV_ERR_OK on success.
 */

OV_DLLFNCEXPORT OV_RESULT TCPbind_TCPChannel_OpenConnection(
		OV_INSTPTR_ksbase_Channel this,
		OV_STRING host,
		OV_STRING port
) {
#if !OV_SYSTEM_NT && !OV_SYSTEM_UNIX
	int ret;
	struct addrinfo *res;
	struct addrinfo *walk;
	struct sockaddr_storage sa_stor;
	socklen_t sas = sizeof(sa_stor);
	struct sockaddr* sa = (struct sockaddr*) &sa_stor;
	char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
	int flags = NI_NUMERICHOST | NI_NUMERICSERV;
	int opt_on = 1;
	TCPBIND_SOCKET sockfd = TCPBIND_INVALID_SOCKET;
#endif
	struct addrinfo hints;
	OV_INSTPTR_TCPbind_TCPChannel thisTCPCh = Ov_StaticPtrCast(TCPbind_TCPChannel, this);

	if(!host || !(*host) || !port || !(*port))
		return OV_ERR_BADPARAM;

	/*	close old connection	*/
	TCPbind_TCPChannel_CloseConnection(this);
	//set connection information
	memset(&(hints), 0, sizeof(struct addrinfo));
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	KS_logfile_debug(("file %s\nline %u:\tbefore ifdef", __FILE__, __LINE__));
#if OV_SYSTEM_NT || OV_SYSTEM_UNIX
	KS_logfile_debug(("%s: inserting work-item", this->v_identifier));
	thisTCPCh->v_addrInfoReq = TCPbind_aresWorker_insertGetAddrInfo(host, port, &(hints));
	if(!thisTCPCh->v_addrInfoReq){
		KS_logfile_error(("%s: could not insert work item", this->v_identifier));
		thisTCPCh->v_ConnectionState = TCPbind_CONNSTATE_COULDNOTOPEN;
		return OV_ERR_GENERIC;
	}
	thisTCPCh->v_ConnectionState = TCPbind_CONNSTATE_OPENING;
	thisTCPCh->v_actimode = 1;
	return OV_ERR_OK;
#else
	KS_logfile_warning(("%s: using blocking getaddrinfo", this->v_identifier));
	//resolve address
	if ((ret = getaddrinfo(host, port, &(hints), &(res))) != 0)
	{
		KS_logfile_error(("%s: getaddrinfo failed", this->v_identifier));
		thisTCPCh->v_ConnectionState = TCPbind_CONNSTATE_COULDNOTOPEN;
		return OV_ERR_GENERIC;
	}
	for (walk = res; walk != NULL; walk = walk->ai_next) {
		KS_logfile_debug(("file %s\nline %u:\twalking...", __FILE__, __LINE__));
		sockfd = socket(walk->ai_family, walk->ai_socktype, walk->ai_protocol);
		if (sockfd == TCPBIND_INVALID_SOCKET){
			KS_logfile_debug(("%s: address not usable", thisTCPCh->v_identifier));
			continue;
		}
		KS_logfile_debug(("file %s\nline %u:\tconnectiong to %i", __FILE__, __LINE__, sockfd));
		if (connect(sockfd, walk->ai_addr, walk->ai_addrlen) == TCPBIND_SOCKET_ERROR) {
			TCPBIND_CLOSE_SOCKET(sockfd);
			sockfd = TCPBIND_INVALID_SOCKET;
			KS_logfile_debug(("%s: connect failed", thisTCPCh->v_identifier));
			continue;
		}
		break;
	}

	//free structures
	freeaddrinfo(res);
	thisTCPCh->v_socket = sockfd;
	//resolve connected peer
	if(getpeername(sockfd, sa, &sas) == TCPBIND_SOCKET_ERROR)
	{
		KS_logfile_error(("%s: could not resolve connected peer", thisTCPCh->v_identifier));
		thisTCPCh->v_ConnectionState = TCPbind_CONNSTATE_COULDNOTOPEN;
		return OV_ERR_GENERIC;
	}

	// resolve peername
	if(getnameinfo( sa, sas, hbuf, sizeof(hbuf), sbuf, sizeof(sbuf), flags) != 0)
	{
		KS_logfile_error(("%s: could not resolve connected peer's address", thisTCPCh->v_identifier));
		thisTCPCh->v_ConnectionState = TCPbind_CONNSTATE_COULDNOTOPEN;
		return OV_ERR_GENERIC;
	}

	ov_string_setvalue(&(thisTCPCh->v_address), hbuf);
	thisTCPCh->v_ConnectionState = TCPbind_CONNSTATE_OPEN;
	TCPbind_TCPChannel_socket_set(thisTCPCh, thisTCPCh->v_socket);

	KS_logfile_debug(("%s: connected to %s.", thisTCPCh->v_identifier, thisTCPCh->v_address));
	//set time of Opening the Connection
	ov_time_gettime(&(thisTCPCh->v_LastReceiveTime));

	//disable nagle for the receivesocket
	(void)setsockopt(thisTCPCh->v_socket, IPPROTO_TCP, TCP_NODELAY, (char *)&opt_on, sizeof(opt_on));
	return OV_ERR_OK;
#endif
}


OV_DLLFNCEXPORT void TCPbind_TCPChannel_CloseConnection(
		OV_INSTPTR_ksbase_Channel this
) {
	OV_INSTPTR_TCPbind_TCPChannel thisTCPCh = Ov_StaticPtrCast(TCPbind_TCPChannel, this);
	if(thisTCPCh->v_socket >= 0)
	{
		TCPbind_TCPChannel_socket_set(thisTCPCh, -1);
		KS_logfile_debug(("TCPChannel %s socket %d closed", thisTCPCh->v_identifier, thisTCPCh->v_socket));
	}
	return;
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
			if(Ov_GetParent(TCPbind_AssocSpecificClientHandler, pProtIdent) == NULL){
				//get VTable of protocol identificator
				Ov_GetVTablePtr(ksbase_ProtocolIdentificator, pVTBLProtIdent, pProtIdent);
				if(!pVTBLProtIdent)
				{
					KS_logfile_error(("Could not determine VTable of ProtocolIdentificator %s - Cancelling operation.", pProtIdent->v_identifier));
					return OV_ERR_NOTIMPLEMENTED;
				}
				else
				{
					//check if protocol is recognized by this Identificator
					if(pVTBLProtIdent->m_identify(pProtIdent, Ov_StaticPtrCast(ksbase_Channel, this)) == TRUE)
					{	//if so, create ClientHandler
						KS_logfile_debug(("Protocol identified by %s (Class %s). Creating Clienthandler", pProtIdent->v_identifier, Ov_GetParent(ov_instantiation, pProtIdent)->v_identifier));
						result = pVTBLProtIdent->m_createClientHandler(pProtIdent, Ov_StaticPtrCast(ksbase_Channel, this));
						if(Ov_Fail(result))
						{
							KS_logfile_error(("ClientHandler could not be created. Reason: %s", ov_result_getresulttext(result)));
							return result;
						}
						KS_logfile_debug(("ClientHandler created."));
						this->v_ClientHandlerAssociated = KSBASE_CH_ASSOCIATED;
						this->v_ConnectionTimeOut = TCPbind_TTL_AFTER_ASSOC;
						break;
					}
				}
			}
			pProtIdent = Ov_StaticPtrCast(ksbase_ProtocolIdentificator, Ov_GetNextChild(ov_instantiation, pProtIdent));
		}
		//if ClientHandler could be Associated, do not go on
		if(this->v_ClientHandlerAssociated == KSBASE_CH_ASSOCIATED)
			break;
		else
			pClassProtIdent = Ov_StaticPtrCast(ov_class, Ov_GetNextChild(ov_inheritance, pClassProtIdent));
	}
	//if no ClientHanlder could be associated Delete Channel
	if(this->v_ClientHandlerAssociated == KSBASE_CH_NOTASSOCATIED)
	{
		this->v_ClientHandlerAssociated = KSBASE_CH_NOTFOUND;
		KS_logfile_error(("No ClientHandler could be associated. Deleting TCPChannel"));
		Ov_DeleteObject(this);
	}
	return OV_ERR_OK;
}

