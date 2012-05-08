#ifndef OV_COMPILE_LIBRARY_ksapitcp
#define OV_COMPILE_LIBRARY_ksapitcp
#endif


#include "ksapitcp.h"
#include "ksapitcp_config.h"
#include "ksapi_logfile.h"
#include "libov/ov_scheduler.h"
#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include "libov/ov_scheduler.h"
//#include "xdrhandling.h"
//#include "xdrexec.h"


#define BUFFER_CHUNK_SIZE 128
#include <errno.h>
#if !OV_SYSTEM_NT
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
/* for select */
#include <sys/select.h>
#endif

OV_DLLFNCEXPORT OV_RESULT ov_xdr_setvalue(OV_STRING *pxdr, const OV_STRING value, OV_INT length);

/**
 * Get TCP Socket to the desitination, if open
 */
OV_DLLFNCEXPORT OV_INT ksapitcp_TCPChannel_socket_get(
	OV_INSTPTR_ksapitcp_TCPChannel	pobj
) {
	return pobj->v_socket;
}

/**
 * Set TCP Socket to the desitination, if open
 */
OV_DLLFNCEXPORT OV_RESULT ksapitcp_TCPChannel_socket_set(
	OV_INSTPTR_ksapitcp_TCPChannel	pobj,
	const OV_INT					value
) {
	pobj->v_socket = value;
	return OV_ERR_OK;
}

/*	ksapitcp_TCPChannel_constate_get
 *	returns the TCP connection state
 */
OV_DLLFNCEXPORT OV_INT ksapitcp_TCPChannel_constate_get(
	OV_INSTPTR_ksapitcp_TCPChannel	pobj
) {
	return pobj->v_constate;
}

/*	ksapitcp_TCPChannel_constate_set
 *	TCP only: sets the connection state
 */
OV_DLLFNCEXPORT OV_RESULT ksapitcp_TCPChannel_constate_set(
	OV_INSTPTR_ksapitcp_TCPChannel	pobj,
	const OV_INT					value
) {
	pobj->v_constate = value;
	return OV_ERR_OK;
}

/*	ksapitcp_TCPChannel_startup
 *	On startup set all values to default and close all connections
 */
OV_DLLFNCEXPORT void ksapitcp_TCPChannel_startup(
	OV_INSTPTR_ov_object	pobj
) {
	OV_INSTPTR_ksapitcp_TCPChannel cobj =	Ov_StaticPtrCast(ksapitcp_TCPChannel, pobj);
	OV_INSTPTR_ov_domain thisdomain = Ov_StaticPtrCast(ov_domain, pobj);
	OV_INSTPTR_ksapitcp_managercom pmc = (OV_INSTPTR_ksapitcp_managercom)Ov_SearchChild(ov_containment, thisdomain, "managercom");

	//doesnt has something linke this currently ksserv_ComTask_startup(pobj);

	//initialize Values
	cobj->v_actimode = 0;
	//ov_time_gettime(&curtime);

	//create managercom
	if(pmc != NULL)	{
		Ov_DeleteObject(pmc);
	}
	if(!Ov_OK((Ov_CreateObject(ksapitcp_managercom, pmc, thisdomain, "managercom")))) {
			ksapi_logfile_info("Fatal: Couldnt create Object managercom");
			return;
	}
	return;
} // end startup


/*	ksapitcp_TCPChannel_shutdown
 *	After closing the service and the connection, shutdown the object.
 */
OV_DLLFNCEXPORT void ksapitcp_TCPChannel_shutdown(
	OV_INSTPTR_ov_object	pobj
) {
	OV_INSTPTR_ksapitcp_TCPChannel cobj =	Ov_StaticPtrCast(ksapitcp_TCPChannel, pobj);
	OV_INSTPTR_ov_domain thisdomain = Ov_StaticPtrCast(ov_domain, pobj);
	OV_INSTPTR_ksapitcp_managercom pmc = (OV_INSTPTR_ksapitcp_managercom)Ov_SearchChild(ov_containment, thisdomain, "managercom");
	ksapitcp_TCPChannel_constate_set(cobj, 0);
	if(pmc != NULL)	{
		Ov_DeleteObject(pmc);
	}
	//doesnt has something like this currently ksserv_ComTask_shutdown(pobj);
	return;
} // end shutdown



/**
 * This sends a given XDR to the host/server as defined in kscommon obj.
 * It enables this ComTask thus the typemethod will be called to see if answer does exist
 */
OV_DLLFNCEXPORT void ksapitcp_TCPChannel_sendxdr(
		OV_INSTPTR_ksapi_Channel	 			pobj,
		OV_INSTPTR_ksapi_KSCommon	 			kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
		OV_INSTPTR_ov_domain thisdomain = Ov_StaticPtrCast(ov_domain, pobj);
		OV_INSTPTR_ksapitcp_TCPChannel tcpchannel = Ov_StaticPtrCast(ksapitcp_TCPChannel, pobj);
		OV_INSTPTR_ksserv_ComTask cTask = Ov_StaticPtrCast(ksserv_ComTask, pobj);
		OV_INSTPTR_ksapitcp_managercom pmc = (OV_INSTPTR_ksapitcp_managercom)Ov_SearchChild(ov_containment, thisdomain, "managercom");
		char *xdrtosend;
		OV_INT xdrlengthtosend;
		int sock;
		struct sockaddr_in server_add;

		//reset counter and serverport, if channel now communicates with other host or servername
		if(ov_string_compare(ksapi_KSCommon_server_get(kscommon), pobj->v_servername) != 0)		{
			ov_string_setvalue(&pobj->v_servername, ksapi_KSCommon_server_get(kscommon));
			pobj->v_serverport = 0;
		}
		if(ov_string_compare(ksapi_KSCommon_host_get(kscommon), pobj->v_host) != 0)	{
			ov_string_setvalue(&pobj->v_host, ksapi_KSCommon_host_get(kscommon));
			pobj->v_serverport = 0;
		}

		//print xdr
		 //~ int j;
		 //~ printf("\n\nsendxdr:\n");
		 //~ for (j = 0; j < xdrlength; j=j+4)
			 //~ printf("%X %X %X %X     ", xdr[j], xdr[j+1], xdr[j+2], xdr[j+3]);
		 //~ printf("\n\n");
		 //~ for (j = 5; j < xdrlength; j=j+4)
			 //~ printf("%c %c %c %c     ", xdr[j], xdr[j+1], xdr[j+2], xdr[j+3]);
		 //~ printf("\n\n");
		 //~ printf("%s\n\n", xdr);

		ksapitcp_TCPChannel_constate_set(tcpchannel, CONSTATE_TCPCHANNEL_SENDING);

		//check serverport
		if(pobj->v_serverport <= 0)	{
			//get serverport by name
			ov_xdr_setvalue(&pobj->v_xdr, xdr, xdrlength);
			pobj->v_xdrlength = xdrlength;
			ksapitcp_managercom_mnggetserver(pmc, ksapi_KSCommon_host_get(kscommon), ksapi_KSCommon_server_get(kscommon), 1);
			//~ printf("\n\n\nCHANNEL_SENDXDR mnggetserver\n\n\n");
			return;
		}

		//create socket
		//~ printf("\n\n\nCHANNEL_SENDXDR create socket\n\n\n");
		sock = ksapitcp_TCPChannel_socket_get(tcpchannel);
		if (sock < 0) {
			if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			{
				perror ("socket(ksapitcp) failed");
				ksapitcp_TCPChannel_constate_set(tcpchannel, CONSTATE_TCPCHANNEL_SOCKETCREATEFAILED);
				return;
			} else {
				//KSDEVEL printf("socket(ksapitcp): %d\n", sock);
				ksapitcp_TCPChannel_socket_set(tcpchannel, sock);
			}
		}

		server_add.sin_family = AF_INET;
		//KSDEVEL printf("\nhost: %s\n", ksapi_KSCommon_host_get(pobj));
#if OV_SYSTEM_NT
        server_add.sin_addr.s_addr = inet_addr( ksapi_KSCommon_host_get(kscommon) );
#else
		inet_aton(ksapi_KSCommon_host_get(kscommon), &server_add.sin_addr);
#endif
		server_add.sin_port = htons(pobj->v_serverport);

		//connect socket
		if (connect(sock, (struct sockaddr*)&server_add, sizeof(server_add)) == -1)	{
			ksapi_logfile_error("connect(ksapitcp) failed");
			CLOSE_SOCKET(sock);
			ksapitcp_TCPChannel_socket_set(tcpchannel, -1);
			ksapitcp_TCPChannel_constate_set(tcpchannel, CONSTATE_TCPCHANNEL_SOCKETCONNECTFAILED);
			return;
		}
		//KSDEVEL else
			//KSDEVEL printf("connected to Server ( %s )\n", inet_ntoa(server_add.sin_addr));

		//send data
		if((xdr == NULL) || (xdrlength <= 0)) {
			//~ ov_xdr_setvalue(&xdrtosend, pobj->v_xdr, pobj->v_xdrlength);
			xdrtosend=(char*)malloc(pobj->v_xdrlength);
			memcpy(xdrtosend, pobj->v_xdr, pobj->v_xdrlength);
			xdrlengthtosend = pobj->v_xdrlength;
		} else {
			xdrtosend=(char*)malloc(xdrlength);
			memcpy(xdrtosend, xdr, xdrlength);
			xdrlengthtosend = xdrlength;
		}
		//~ printf("\n\n\nCHANNEL_SENDXDR send\n\n\n");
		if (send(sock, xdrtosend, xdrlengthtosend, 0) == -1) {
			ksapi_logfile_error("send(ksapitcp) failed");
			CLOSE_SOCKET(sock);
			ksapitcp_TCPChannel_socket_set(tcpchannel, -1);
			ksapitcp_TCPChannel_constate_set(tcpchannel, CONSTATE_TCPCHANNEL_SOCKETSENDFAILED);
			return;
		} else {
			//ready -> wait for answer
			cTask->v_actimode = 1;
			ksapitcp_TCPChannel_constate_set(tcpchannel, CONSTATE_TCPCHANNEL_WAITINGRESPOND);

			//~ printf("\n\n\nCHANNEL TYPEMETHOD ACTIVATED\n\n\n");
			//TODO: Do we need to set this as a status flag???

			//KSDEVEL printf("send(ksapitcp) ok\n");
			//Ov_WarnIfNot(Ov_OK(ov_scheduler_register(pov, KSCommon_receivexdr)));
			//ov_scheduler_setreleventtime(pov, &tsp);
		}

	return;
}

/**
 * this is called regulary by the RootComTask, IFF XDR was sent
 * It looks for received data, stores it and calls back the KSCommon->receivedXDRCallBack
 */
OV_DLLFNCEXPORT void ksapitcp_TCPChannel_typemethod(
		OV_INSTPTR_ksserv_ComTask cTask
) {
	OV_INSTPTR_ksapitcp_TCPChannel	channel = Ov_StaticPtrCast(ksapitcp_TCPChannel, cTask);
	OV_INSTPTR_ksapi_KSCommon	kscommon = NULL;
	OV_VTBLPTR_ksapi_KSCommon   kscommonVTBL = NULL;

 	int recvBytes = 0;
	char *buffer_location = 0; //pointer into the buffer 

	int off = 0;
	fd_set read_flags;
	struct timeval waitd;
	char ckxdrlength[4];
	int err = 0;
	char *xdr_received = 0;
	int timeoutcounter = 0;
	
	int sock;
	int i;
	int size_received, size_receiving;

	kscommon = (OV_INSTPTR_ksapi_KSCommon)Ov_GetParent(ov_containment, channel);
	Ov_GetVTablePtr(ksapi_KSCommon, kscommonVTBL, kscommon);

	sock = ksapitcp_TCPChannel_socket_get(channel);
	//check socket
	if (sock < 0) {
		ksapi_logfile_error("no socket");
		ksapitcp_TCPChannel_constate_set(channel, CONSTATE_TCPCHANNEL_NOSOCKET);
		cTask->v_actimode = 0;
		return;
	}

	//blocking
	if ((IOCTL_SOCKET(sock, FIONBIO, (char*) &off)) == -1) {
		perror("ioctl(tcpcChannel) failed (set to blocking)");
		return;
	}

	FD_ZERO(&read_flags);
	FD_SET(sock, &read_flags); // get read flags
	waitd.tv_sec = 0;     // Set Timeout
	waitd.tv_usec = 1000;    //  1 msec
	err = select(sock + 1, &read_flags, (fd_set*) 0, (fd_set*)0,&waitd);
	#if KSSERVTCP_LOG_DEEP
	ksserv_logfile_debug("select returned: %d; line %d", err, __LINE__);
	#if OV_SYSTEM_UNIX
	ksserv_logfile_debug("select waited: %d secs, %d usecs", waitd.tv_sec, 1000-waitd.tv_usec);	//Windows Systems don't alter waitd
	#endif
	#endif

	//check if data arrived
	if((err > 0) && FD_ISSET(sock, &read_flags))
	{
		recvBytes = recv(sock, ckxdrlength, 4,0);		//first 4 bytes code length of xdr
		if(recvBytes < 4)
		{
			if(recvBytes == 0)		//shutdown by server?
				ksapi_logfile_debug("tcpchannel/typemethod: read 0 bytes - shutdown");
			else			//error
				ksapi_logfile_error("received %d bytes (less than 4) - shutting down", recvBytes);			//on windows machines a closed socket will return -1 here (instead of 0)

	#if OV_SYSTEM_NT
			errno = WSAGetLastError();
	#endif
			perror("tcpchannel - receive");

			ksapitcp_TCPChannel_shutdown((OV_INSTPTR_ov_object)cTask);
			return;
		}
		//decoding size of memstream
		for(i=0; i<4; i++)
			((char*) &size_receiving)[i] = ckxdrlength[3-i];
		((char*) &size_receiving)[3] = 0;

		ksapi_logfile_debug("xdr-size: %d", size_receiving);


		xdr_received = (char*)malloc(size_receiving);
		memset(xdr_received, 0, size_receiving);

		size_received = 0;
		buffer_location = xdr_received;

		do{
			FD_ZERO(&read_flags);
			FD_SET(sock, &read_flags); // get read flags
			waitd.tv_sec = 0;     // Set Timeout
			waitd.tv_usec = 1000;    //  1 msec
			err = select(sock + 1, &read_flags, (fd_set*) 0, (fd_set*)0,&waitd);
	#if KSSERVTCP_LOG_DEEP
			ksapi_logfile_debug("select returned: %d; line %d", err, __LINE__);
	#if OV_SYSTEM_UNIX
			ksapi_logfile_debug("select waited: %d secs, %d usecs", waitd.tv_sec, 1000-waitd.tv_usec);	//Windows Systems don't alter waitd
	#endif
	#endif
			if(err < 1)		//if error or timeout expired
			{
				timeoutcounter++;
				if(timeoutcounter >= 10)
				{
					ksapi_logfile_error("command didnt arrive within reasonable time (10 msecs)");
					ksapitcp_TCPChannel_shutdown((OV_INSTPTR_ov_object)cTask);
					free(xdr_received);
					return;
				}
			}

			recvBytes = recv(sock, buffer_location, size_receiving - size_received, 0);
			size_received += recvBytes;
			buffer_location = &(xdr_received[size_received]);

			ksapi_logfile_debug("%d of %d bytes received, err is %d", size_received, size_receiving, err);

		}while(size_received < size_receiving);

	//evaluate


		//int j;
		//~ printf("\n\nxdr channel receive:\nlength: %d\n", xdrlength);
		//~ for (j = 0; j < xdrlength; j=j+4)
		//~ {
			//~ if(((j!=0) && (j%20)) == 0)
				//~ printf("\n");
			//~ printf("%X %X %X %X     ", xdrdata[j], xdrdata[j+1], xdrdata[j+2], xdrdata[j+3]);
		//~ }
		//~ printf("\n");
		//~ for (j = 0; j < xdrlength; j=j+4)
		//~ {
			//~ if(((j!=0) && (j%20)) == 0)
				//~ printf("\n");
			//~ printf("%c %c %c %c     ", xdrdata[j], xdrdata[j+1], xdrdata[j+2], xdrdata[j+3]);
		//~ }
		//~ printf("\n\n");
		//~ printf("%s\n\n", xdrdata);

		//close connection
		ksapi_logfile_debug("TCPChannel closing socket");
		CLOSE_SOCKET(sock); //todo connection manager for reuseage of socket!
		ksapitcp_TCPChannel_socket_set(channel, -1);
		//call obj to decode XDR!
		ksapi_logfile_debug("TCPChannel handing over control to ksapi obj %s", kscommon->v_identifier);
		kscommonVTBL->m_returnMethodxdr(kscommon, xdr_received, size_received);
		ksapitcp_TCPChannel_constate_set(channel, CONSTATE_CHANNEL_OK); //OK= Frei
		//ready -> go to sleep
		cTask->v_actimode = 0;

	}

	free(xdr_received);
	return;
}

OV_DLLFNCEXPORT OV_RESULT ov_xdr_setvalue(
	OV_STRING				*pxdr,
	const OV_STRING		value,
	OV_INT	length
) {
	/*
	*	local variables
	*/
	OV_STRING	xdr;
	/*
	*	check parameters
	*/
	if(!pxdr) {
		return OV_ERR_BADPARAM;
	}
	/*
	*	free string if there's no value
	*/
	if(!value) {
		if(*pxdr) {
			ov_database_free(*pxdr);
			*pxdr = NULL;
		}
		return OV_ERR_OK;
	}
	if(!*value) {
		if(*pxdr) {
			ov_database_free(*pxdr);
			*pxdr = NULL;
		}
		return OV_ERR_OK;
	}
	/*
	*	allocate memory for new string
	*/
	xdr = ov_database_realloc(*pxdr, length);
	if(!xdr) {
		return OV_ERR_DBOUTOFMEMORY;
	}
	/*
	*	copy the new value
	*/
	memcpy(xdr, value, length);
	/*
	*	finish
	*/
	*pxdr = xdr;
	return OV_ERR_OK;
}

