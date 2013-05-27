
/******************************************************************************
 *
 *   FILE
 *   ----
 *   TCPListener.c
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
#include "TCPbind_helper.h"
#include "ks_logfile.h"
#include "TCPbind_config.h"
#include "ksbase_helper.h"
#include "libov/ov_vendortree.h"


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
#include <netinet/tcp.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
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
#ifndef IPV6_V6ONLY
#define IPV6_V6ONLY 27
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

OV_DLLFNCEXPORT OV_INT TCPbind_TCPListener_port_get(
		OV_INSTPTR_TCPbind_TCPListener          pobj
) {
	return pobj->v_port;
}

OV_DLLFNCEXPORT OV_RESULT TCPbind_TCPListener_port_set(
		OV_INSTPTR_TCPbind_TCPListener          pobj,
		const OV_INT  value
) {

	if(pobj->v_socket[0] != -1)
	{
		CLOSE_SOCKET(pobj->v_socket[0]);
		pobj->v_socket[0] = -1;
	}

	if(pobj->v_socket[1] != -1)
	{
		CLOSE_SOCKET(pobj->v_socket[1]);
		pobj->v_socket[1] = -1;
	}

	pobj->v_port = value;
	return OV_ERR_OK;
}



OV_DLLFNCEXPORT OV_RESULT TCPbind_TCPListener_constructor(
		OV_INSTPTR_ov_object 	pobj
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_TCPbind_TCPListener pinst = Ov_StaticPtrCast(TCPbind_TCPListener, pobj);
	OV_RESULT result;
	/* do what the base class does first */
	result = ksbase_ComTask_constructor(pobj);
	if(Ov_Fail(result))
		return result;
	/* do what */

	//start inactive at construction. Begin listening in typemethod
	pinst->v_actimode = 0;

	return result;
}

OV_DLLFNCEXPORT void TCPbind_TCPListener_shutdown(
		OV_INSTPTR_ov_object 	pobj
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_TCPbind_TCPListener pinst = Ov_StaticPtrCast(TCPbind_TCPListener, pobj);
	OV_INSTPTR_TCPbind_TCPChannel pChannel = NULL;
	OV_INSTPTR_TCPbind_TCPChannel pNextChannel = NULL;

	/* do what */
	//close socket
	if(pinst->v_socket[0] != -1)
	{
		CLOSE_SOCKET(pinst->v_socket[0]);
		pinst->v_socket[0] = -1;
	}

	if(pinst->v_socket[1] != -1)
	{
		CLOSE_SOCKET(pinst->v_socket[1]);
		pinst->v_socket[1] = -1;
	}
	pinst->v_SocketState = 0;
	pinst->v_actimode = 0;

	/*
	 * iterate over all TCPChannels in containment and delete them if they act as Servers (probably all of them)
	 * we cannot use Ov_ForEachChildEx here since we would delete an object before getting the next child
	 */
	pNextChannel = Ov_StaticPtrCast(TCPbind_TCPChannel, Ov_GetFirstChild(ov_containment, Ov_StaticPtrCast(ov_domain, pinst)));
	while(pNextChannel)
	{
		if(Ov_CanCastTo(TCPbind_TCPChannel, pNextChannel))
		{
			pChannel = pNextChannel;
			pNextChannel = Ov_StaticPtrCast(TCPbind_TCPChannel, Ov_GetNextChild(ov_containment, Ov_StaticPtrCast(ov_object, pChannel)));
			if(pChannel->v_ClientHandlerAssociated != KSBASE_CH_NOTNEEDED)
				Ov_DeleteObject(pChannel);
		}
		else
			pNextChannel = Ov_StaticPtrCast(TCPbind_TCPChannel, Ov_GetNextChild(ov_containment, Ov_StaticPtrCast(ov_object, pNextChannel)));

	}


	/* set the object's state to "shut down" */
	ov_object_shutdown(pobj);

	return;
}

OV_DLLFNCEXPORT void TCPbind_TCPListener_typemethod (
		OV_INSTPTR_ksbase_ComTask	this
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_TCPbind_TCPListener thisLi = Ov_StaticPtrCast(TCPbind_TCPListener, this);
	OV_INSTPTR_TCPbind_TCPChannel pNewChannel = NULL;
	OV_INSTPTR_ksbase_ProtocolIdentificator pProtIdent = NULL;
	OV_VTBLPTR_ksbase_ProtocolIdentificator pVTBLProtIdent = NULL;
	struct addrinfo *res;
	struct addrinfo hints;
	int ret;
	char portbuf [12];
	int fd = 0, n = 0;
#define NFDS 2
	int sockfds[NFDS]={-1,-1};
	struct addrinfo *walk;
	struct sockaddr_storage sa_stor;
	socklen_t sas = sizeof(sa_stor);
	struct sockaddr* sa = (struct sockaddr*) &sa_stor;
	char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
	int flags = NI_NUMERICHOST | NI_NUMERICSERV;
	fd_set fds;
	int highest;
	int i;
	struct timeval waitd;
	struct sockaddr_storage peer;
	socklen_t peers = sizeof(struct sockaddr_storage);
	int cfd = -1;
	char buf[NI_MAXHOST];
	int on = 1;
	OV_UINT namecounter = 0;
	char ChannelNameBuffer[21];	//name for TCPChannelxxxxxxxxxx (MAX_UINT Channels should be enough...)

	/*
	 * If no socket is open, open one and start listening.
	 * This code was copied from ct 19/12. If is meant to make the sockets IPv6 compatible.
	 */
	if(!thisLi->v_SocketState)	//no socket open
	{

		if(!ov_vendortree_getcmdlineoption_value("TCPbind_NO_IPv6"))
		{
			memset(&hints, 0, sizeof(struct addrinfo));
			hints.ai_family = PF_UNSPEC;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_flags = AI_PASSIVE;

			sprintf(portbuf, "%lu", TCPbind_TCPListener_port_get(thisLi));

			if((ret = getaddrinfo(NULL, portbuf, &hints, &res)) != 0)
			{
				KS_logfile_error(("%s: getaddrinfo failed: %d", this->v_identifier, ret));
				thisLi->v_SocketState = TCPbind_CONNSTATE_COULDNOTOPEN;
				return;
			}

			for (walk = res; walk != NULL; walk = walk->ai_next)
			{
				fd = socket(walk->ai_family, walk->ai_socktype, walk->ai_protocol);
#if OV_SYSTEM_NT
				if((fd==-1) || (fd==INVALID_SOCKET))
#else
				if (fd == -1)
#endif
				{
					continue;
				}

				if (walk->ai_family == AF_INET6)
				{

					KS_logfile_debug(("%s: found IPv6 socket: %d", thisLi->v_identifier, fd));
					if (setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&on, sizeof(on)) == -1)
					{
						continue;
					}
				}
				else if(walk->ai_family != AF_INET)
				{		//Whitelisting IPV4 and IPV6
					KS_logfile_debug(("%s: found non INET-socket: %d. closing socket", thisLi->v_identifier, fd));
					CLOSE_SOCKET(fd);
					continue;
				}
				else
					KS_logfile_debug(("%s: found IPv4 socket: %d", thisLi->v_identifier, fd));

				if (bind(fd, walk->ai_addr, walk->ai_addrlen))
				{
					continue;
				}

				if (listen(fd, 5) == -1)
				{
					continue;
				}


				if(getsockname(fd, sa, &sas))
				{
					KS_logfile_error(("%s: getsockname failed", this->v_identifier));
					KS_logfile_print_sysMsg();
					thisLi->v_SocketState = TCPbind_CONNSTATE_COULDNOTOPEN;
					return;
				}

				if(getnameinfo( sa, sas, hbuf, sizeof(hbuf), sbuf, sizeof(sbuf), flags))
				{
					KS_logfile_error(("%s: getnameinfo failed", this->v_identifier));
					KS_logfile_print_sysMsg();
					thisLi->v_SocketState = TCPbind_CONNSTATE_COULDNOTOPEN;
					return;
				}


				KS_logfile_debug(("%s: listening on %s on port %s (socket: %d)",this->v_identifier, hbuf, sbuf, fd));

				sockfds[n++] = fd;
				if(n>=NFDS)
					break;
			}

			freeaddrinfo(res);
			if(thisLi->v_port == -1)
			{
				TCPbind_TCPListener_port_set(thisLi, atoi(sbuf));
			}
			thisLi->v_socket[0] = sockfds[0];
			thisLi->v_socket[1] = sockfds[1];
			thisLi->v_SocketState = TCPbind_CONNSTATE_OPEN;
		}
		else
		{
			memset(&hints, 0, sizeof(struct addrinfo));
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_flags = AI_PASSIVE;

			sprintf(portbuf, "%lu", TCPbind_TCPListener_port_get(thisLi));

			if((ret = getaddrinfo(NULL, portbuf, &hints, &res)) != 0)
			{
				KS_logfile_error(("%s: getaddrinfo failed: %d", this->v_identifier, ret));
				thisLi->v_SocketState = TCPbind_CONNSTATE_COULDNOTOPEN;
				return;
			}

			fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
#if OV_SYSTEM_NT
			if((fd==-1) || (fd==INVALID_SOCKET))
			{
				errno = WSAGetLastError();
#else
			if (fd == -1)
			{
#endif
				KS_logfile_error(("%s: failed to open socket: %d", thisLi->v_identifier, errno));
				ks_logfile_print_sysMsg();
				thisLi->v_SocketState = TCPbind_CONNSTATE_COULDNOTOPEN;
				return;
			}

			if (bind(fd, res->ai_addr, res->ai_addrlen))
			{
#if OV_SYSTEM_NT
				errno = WSAGetLastError();
#endif
				KS_logfile_error(("%s: failed to bind to socket: %d", thisLi->v_identifier, errno));
				ks_logfile_print_sysMsg();
				thisLi->v_SocketState = TCPbind_CONNSTATE_COULDNOTOPEN;
				return;
			}
			if (listen(fd, 5) == -1)
			{
				KS_logfile_error(("%s: failed to start listening on socket: %d", thisLi->v_identifier, errno));
				ks_logfile_print_sysMsg();
				thisLi->v_SocketState = TCPbind_CONNSTATE_COULDNOTOPEN;
				return;
			}

			if(thisLi->v_port == -1)
			{
				TCPbind_TCPListener_port_set(thisLi, atoi(sbuf));
			}
			thisLi->v_socket[0] = fd;
			thisLi->v_socket[1] = -1;
			thisLi->v_SocketState = TCPbind_CONNSTATE_OPEN;
		}
			KS_logfile_debug(("%s: sockets are: %d and %d", thisLi->v_identifier, thisLi->v_socket[0], thisLi->v_socket[1]));
	}
		/**
		 * Socket is open now
		 */

	if(thisLi->v_SocketState == TCPbind_CONNSTATE_OPEN)
	{

		sockfds[0] = thisLi->v_socket[0];
		sockfds[1] = thisLi->v_socket[1];
		highest = 0;
		FD_ZERO(&fds);
		for (i = 0; i < 2; i++) {
#if OV_SYSTEM_NT
			if((sockfds[i] != -1) && (sockfds[i] != INVALID_SOCKET))
#else
			if(sockfds[i] != -1)
#endif
			{

				FD_SET(sockfds[i], &fds);
				highest = (sockfds[i] > highest ? sockfds[i] : highest);

			}
		}

		waitd.tv_sec = 0;     // Set Timeout
		waitd.tv_usec = 0;    //  do not wait
		ret = select(highest+1, &fds, NULL, NULL, &waitd);
		if(ret)
			KS_logfile_debug(("%s: select returned: %d; line %d",this->v_identifier, ret, __LINE__));
#if OV_SYSTEM_NT
		if(ret == SOCKET_ERROR)
		{
			errno = WSAGetLastError();
#else
		if(ret == -1)
		{
#endif
			KS_logfile_error(("%s: select returned error %d", this->v_identifier, errno));
			KS_logfile_print_sysMsg();

		}

		if(ret>0)	//if there is activity on the socket(s)
		{
			for (i = 0; (i < 2); i++) {
				if ((sockfds[i] != -1) && (!FD_ISSET(sockfds[i], &fds)))
					continue;

				cfd = accept(sockfds[i], (struct sockaddr*)&peer, &peers);

				if (getnameinfo((struct sockaddr*)&peer, peers, buf, sizeof(buf), NULL, 0, NI_NUMERICHOST))
				{
					KS_logfile_error(("%s: getnameinfo for newly connected client failed", this->v_identifier));
					KS_logfile_print_sysMsg();
				}

				KS_logfile_debug(("%s: new client connected: %s", this->v_identifier, buf));

				//disable nagle for the receivesocket
				setsockopt(cfd, IPPROTO_TCP, TCP_NODELAY, (char *) &on, sizeof(on));


				//get first free "TCPChannel"-name
				do {
					pNewChannel = NULL;
					namecounter++;
					sprintf(ChannelNameBuffer, "TCPChannel%lu", namecounter);
					pNewChannel	= (OV_INSTPTR_TCPbind_TCPChannel) Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, this), ChannelNameBuffer);
				} while (pNewChannel);

				//create receiving TCPChannel
				if (Ov_OK(Ov_CreateObject(TCPbind_TCPChannel, pNewChannel, Ov_StaticPtrCast(ov_domain, this), ChannelNameBuffer))) {
					KS_logfile_debug(("%s: New Channel created: %s to handle client %s", this->v_identifier, ChannelNameBuffer, buf));
					//copy socket to created object
					TCPbind_TCPChannel_socket_set(pNewChannel, cfd);
					if(Ov_Fail(ov_string_setvalue(&(pNewChannel->v_address), buf)))
							KS_logfile_error(("%s: failed to set address for TCHChannel %s", this->v_identifier, pNewChannel->v_identifier));
					pNewChannel->v_ConnectionState = TCPbind_CONNSTATE_OPEN;
					/*	check for local connection and set flag appropriately	*/
					if((ov_string_compare(buf, "127.0.0.1") == OV_STRCMP_EQUAL)
							|| (ov_string_compare(buf, "::1") == OV_STRCMP_EQUAL))
						pNewChannel->v_isLocal = TRUE;

					if(thisLi->v_ChannelNeedsClientHandler)
					{
						pNewChannel->v_ClientHandlerAssociated = KSBASE_CH_NOTASSOCATIED;
						pProtIdent = Ov_GetChild(TCPbind_AssocSpecificClientHandler, thisLi);
						if(pProtIdent)
						{
							Ov_GetVTablePtr(ksbase_ProtocolIdentificator, pVTBLProtIdent, pProtIdent);
							if(!pVTBLProtIdent)
							{
								KS_logfile_error(("%s: error getting VTablePtr for %s", this->v_identifier, pProtIdent->v_identifier));
							}
							else
							{
								if(Ov_Fail(pVTBLProtIdent->m_createClientHandler(pProtIdent, Ov_StaticPtrCast(ksbase_Channel, pNewChannel))))
									KS_logfile_error(("%s: error creating ClientHandler for %s. (ProtocolIdentificator %s)", this->v_identifier, pNewChannel->v_identifier, pProtIdent->v_identifier));
							}
						}
					}
					else
						pNewChannel->v_ClientHandlerAssociated = KSBASE_CH_NOTNEEDED;

				} else {
					KS_logfile_error(("%s: Creation of TCPChannel for %s failed (socket %d).", this->v_identifier, buf, cfd));
				}
			}
		}
	}
	return;
}

