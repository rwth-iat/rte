/*
*	Copyright (C) 2012
*	Chair of Process Control Engineering,
*	Aachen University of Technology.
*	All rights reserved.
*
*	Redistribution and use in source and binary forms, with or without
*	modification, are permitted provided that the following conditions
*	are met:
*	1. Redistributions of source code must retain the above copyright
*	   notice, this list of conditions and the following disclaimer.
*	2. Redistributions in binary form must print or display the above
*	   copyright notice either during startup or must have a means for
*	   the user to view the copyright notice.
*	3. Redistributions in binary form must reproduce the above copyright
*	   notice, this list of conditions and the following disclaimer in
*		the documentation and/or other materials provided with the
*		distribution.
*	4. Neither the name of the Chair of Process Control Engineering nor
*		the name of the Aachen University of Technology may be used to
*		endorse or promote products derived from this software without
*		specific prior written permission.
*
*	THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
*	``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE CHAIR OF
*	PROCESS CONTROL ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT,
*	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
*	OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
*	AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
*	WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*	POSSIBILITY OF SUCH DAMAGE.
*
***********************************************************************/

#ifndef OV_COMPILE_LIBRARY_ksservhttp
#define OV_COMPILE_LIBRARY_ksservhttp
#endif

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>

#if !OV_SYSTEM_NT 
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <fcntl.h>
#endif

/**
 * Returns the port on which the object is listening.
 */
OV_DLLFNCEXPORT OV_INT ksservhttp_httpserver_port_get(
		OV_INSTPTR_ksservhttp_httpserver pobj) {
	return pobj->v_port;
}

/**
 * Sets the port on which the object is listening.
 */
OV_DLLFNCEXPORT OV_RESULT ksservhttp_httpserver_port_set(
		OV_INSTPTR_ksservhttp_httpserver pobj, const OV_INT value) {
	CLOSE_SOCKET(pobj->v_listensocket);
	ksservhttp_httpserver_listensocket_set(pobj, -1);

	pobj->v_port = value;
	return OV_ERR_OK;
}

/**
 * Returns the socket on which the object is listening.
 */
OV_DLLFNCEXPORT OV_INT ksservhttp_httpserver_listensocket_get(
		OV_INSTPTR_ksservhttp_httpserver pobj) {
	return pobj->v_listensocket;
}

/**
 * Sets the socket on which the object is listening.
 */
OV_DLLFNCEXPORT OV_RESULT ksservhttp_httpserver_listensocket_set(
		OV_INSTPTR_ksservhttp_httpserver pobj, const OV_INT value) {
	pobj->v_listensocket = value;
	return OV_ERR_OK;
}

/**
 * Procedure called on object startup.
 * It starts the listening on with variable OWNPORT
 * declared port or by default on port 8080.
 */
OV_DLLFNCEXPORT void ksservhttp_httpserver_startup(OV_INSTPTR_ov_object pobj) {
	OV_INSTPTR_ksservhttp_httpserver this =
			Ov_StaticPtrCast(ksservhttp_httpserver, pobj);
	OV_INSTPTR_ov_domain thisdomain = Ov_StaticPtrCast(ov_domain, pobj);
	OV_INSTPTR_ov_domain pclients;
	OV_INSTPTR_ov_domain pstaticfiles;
	OV_INSTPTR_ksservhttp_httpclienthandler ptcpc;

	pclients = Ov_SearchChildEx(ov_containment, thisdomain, "clients", ov_domain);
	pstaticfiles = Ov_SearchChildEx(ov_containment, thisdomain, "staticfiles", ov_domain);
	ptcpc =  NULL;

	//seed random generator for md5 nounce generation during http autorization
	srand(time(NULL));

	//TODO: port should be setable via env variable
	//store port to object
	//ksservhttp_httpserver_port_set(this, this->v_port);
	ksservhttp_httpserver_port_set(this, 8080);

	//create clients domain
	if (pclients) {
		ptcpc = (OV_INSTPTR_ksservhttp_httpclienthandler) Ov_GetFirstChild(ov_containment, pclients);
		while (ptcpc) {
			ksservhttp_httpclienthandler_shutdown((OV_INSTPTR_ov_object) ptcpc);
			ptcpc = (OV_INSTPTR_ksservhttp_httpclienthandler) Ov_GetNextChild(ov_containment, ptcpc);
		}
		Ov_DeleteObject((OV_INSTPTR_ov_object)pclients);
		if (!Ov_OK((Ov_CreateObject(ov_domain, pclients, thisdomain, "clients")))) {
			ksserv_logfile_error("Fatal: Couldnt create Object clients");
			return;
		}
	} else {
		if (!Ov_OK((Ov_CreateObject(ov_domain, pclients, thisdomain, "clients")))) {
			ksserv_logfile_error("Fatal: Couldnt create Object clients");
			return;
		}
	}

	//create staticfiles domain
	if (!pstaticfiles) {
		if (!Ov_OK((Ov_CreateObject(ov_domain, pclients, thisdomain, "staticfiles")))) {
			ksserv_logfile_error("Fatal: Couldnt create Object staticfiles");
			return;
		}
	}

	//register typemethod
	//ComTask Ov_WarnIfNot(Ov_OK(ov_scheduler_register(pobj, ksservhttp_httpserver_typemethod)));

	return;
}

/**
 * Procedure called on object shutdown. 
 * It closes open socket.
 */
OV_DLLFNCEXPORT void ksservhttp_httpserver_shutdown(OV_INSTPTR_ov_object pobj) {
	int listensocket;
	OV_INSTPTR_ksservhttp_httpserver this = Ov_StaticPtrCast(ksservhttp_httpserver, pobj);
	//close socket
	listensocket = ksservhttp_httpserver_listensocket_get(this);
#if OV_SYSTEM_NT
	if(CLOSE_SOCKET(listensocket)==SOCKET_ERROR)
	{
		errno = WSAGetLastError();
#else
	if(CLOSE_SOCKET(listensocket) == -1)
	{
#endif
		perror("tcp shutdown listen");
	}

	ksservhttp_httpserver_listensocket_set(this, -1);

	return;
}

/**
 * Procedure periodically called by Root ComTask //ov_scheduler.
 * It takes over the connection-management.
 * If there is an incomming connection-request on
 * listensocket it creates an tcpclient-object which
 * will take over the communication with requesting client.
 */
void ksservhttp_httpserver_typemethod(OV_INSTPTR_ksserv_ComTask cTask
//ComTask OV_INSTPTR_ov_object	pfb
) {
	OV_INSTPTR_ksservhttp_httpserver this =
			Ov_StaticPtrCast(ksservhttp_httpserver, cTask);
	OV_INSTPTR_ov_domain thisdomain = Ov_StaticPtrCast(ov_domain, cTask);
	OV_INSTPTR_ksservhttp_httpclienthandler ptcpc = NULL;
	OV_INSTPTR_ksservhttp_authenticatedsession psession = NULL;
	OV_INSTPTR_ov_domain
			pclients =
					(OV_INSTPTR_ov_domain) Ov_SearchChild(ov_containment, thisdomain, "clients");
	OV_INSTPTR_ov_domain psessions = (OV_INSTPTR_ov_domain) Ov_SearchChild(ov_containment, thisdomain, "sessions");
	int listensocket = ksservhttp_httpserver_listensocket_get(this);
	int receivesocket;
	char clientname[256];
	int on = 1;
	int cnr = 0;
	//int optval = 1;
	char optval = 1; //used by setsockopt for reuseage of tcp connection port
	struct sockaddr_in client_addr;
	int port = ksservhttp_httpserver_port_get(this);

	OV_TIME time;

	//do some authentication housekeeping - remove expired sessions
	ov_time_gettime(&time); //current time
	//add maximal session livetime
	time.secs -= SESSION_TTL;

	Ov_ForEachChildEx(ov_containment, psessions, psession, ksservhttp_authenticatedsession){
		if(ov_time_compare(&time, &(psession->v_lastactivity)) == 1){
			//the session is dead
			Ov_DeleteObject(psession);
		}
	}

	//check if we have already an open socket - otherwise create socket
	if (listensocket == -1) {
		//open socket
		listensocket = socket(PF_INET, SOCK_STREAM, 0);
		ksservhttp_httpserver_listensocket_set(this, listensocket);
		if (listensocket == -1) {
			perror("socket(tcpconnection) failed");
			this->v_status = STATUS_TCPCON_SOCKOPENFAILED;
			return;
		}

		//non-blocking
		//if ((IOCTL_SOCKET(listensocket, FIONBIO, (char*) &on)) == -1) {
		if ((IOCTL_SOCKET(listensocket, FIONBIO, (u_long*) &on)) == -1) {
			perror("ioctl(tcpconnection) failed");
			CLOSE_SOCKET(listensocket);
			ksservhttp_httpserver_listensocket_set(this, -1);
			this->v_status = STATUS_TCPCON_SOCKNONBLOCKFAILED;
			return;
		}

		//bind
		memset(&client_addr, 0, sizeof(client_addr));
		client_addr.sin_family = AF_INET;
		client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		client_addr.sin_port = htons(port);

		//test reuseage!
		ov_logfile_info("########## reuseage of used port by setsockopt ");
		setsockopt(listensocket, SOL_SOCKET, SO_REUSEADDR, &optval,	sizeof(optval));
		//setsockopt(listensocket, SOL_SOCKET, 0, &optval,	sizeof(optval));

		if ((bind(listensocket, (struct sockaddr*) &client_addr,
				sizeof(client_addr))) == -1) {
			perror("bind(tcpconnection) failed");
			CLOSE_SOCKET(listensocket);
			ksservhttp_httpserver_listensocket_set(this, -1);
			this->v_status = STATUS_TCPCON_SOCKBINDFAILED;
			return;
		}
		//listen
		if ((listen(listensocket, 3)) == -1) {
			perror("listen(tcpconnection) failed");
			CLOSE_SOCKET(listensocket);
			ksservhttp_httpserver_listensocket_set(this, -1);
			this->v_status = STATUS_TCPCON_SOCKLISTENFAILED;
			return;
		}


	} //end create socket

	listensocket = ksservhttp_httpserver_listensocket_get(this);

	//accept
	if ((receivesocket = accept(listensocket, NULL, NULL)) < 0) {
		//ov_logfile_info("No client waiting to connect (accept(tcpconnection) < 0)");
		this->v_status = STATUS_TCPCON_SOCKACCEPTFAILED;
		return;
	}

	//disable nagle for the receivesocket
	setsockopt(receivesocket, IPPROTO_TCP, TCP_NODELAY, (char *) &optval, sizeof(optval));

	//non-blocking
	//if ((IOCTL_SOCKET(receivesocket, FIONBIO, (char*) &on)) == -1) {
	if ((IOCTL_SOCKET(receivesocket, FIONBIO, (u_long*) &on)) == -1) {
		perror("ioctl(receivesocket) failed");
		CLOSE_SOCKET(receivesocket);
		this->v_status = STATUS_TCPCON_SOCKNONBLOCKCLIENTFAILED;
		return;
	}

	//KSDEVEL ov_logfile_info("clientname: %s", clientname);
	//KSDEVEL ov_logfile_info("socket: %d", receivesocket);

	//get first free "tcpclient"-name
	do {
		ptcpc = NULL;
		cnr++;
		sprintf(clientname, "tcpclient%i", cnr);
		ptcpc
				= (OV_INSTPTR_ksservhttp_httpclienthandler) Ov_SearchChild(ov_containment, pclients, clientname);
	} while (ptcpc);

	//create receiving tcpclient
	if (Ov_OK(Ov_CreateObject(ksservhttp_httpclienthandler, ptcpc, pclients, clientname))) {
		ksserv_logfile_info("New client connected, socket %d", receivesocket);
		//copy socket to created tcpclient-object
		ksservhttp_httpclienthandler_receivesocket_set(ptcpc, receivesocket);
		this->v_status = STATUS_TCPCON_OK;
		ptcpc->v_cycInterval = 0;
	} else {
		ksserv_logfile_error("Creating of TCPClient failed while New client connected, socket %d ", receivesocket);
	}
	return;
}

