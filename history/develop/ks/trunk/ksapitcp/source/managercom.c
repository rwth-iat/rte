/** @file 
 * Managercom provides functionallity to communicate with the ks_manager.
 ******************************************************************************/
#ifndef OV_COMPILE_LIBRARY_ksapitcp
#define OV_COMPILE_LIBRARY_ksapitcp
#endif

#include "ksapitcp.h"
#include "config.h"
#include "ksserv.h"
#include "ksservtcp.h"
#include "ksapi_logfile.h"

#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include "libov/ov_scheduler.h"

#include "assert.h"
#if !OV_SYSTEM_NT
#include <unistd.h>
#include <sys/ioctl.h>
#endif
#define h_addr h_addr_list[0]

const int STOK = STATUS_STOK;
const int STPMSOCKCR = STATUS_STPMSOCKCR;
const int STPMRQSND = STATUS_STPMRQSND;
const int STPMRPLYRCV = STATUS_STPMRPLYRCV;
const int STMNGRQSND = STATUS_STMNGRQSND;

/**
 * Returns the port which this class uses to communicate
 * with the ks_manager
 */
OV_DLLFNCEXPORT OV_UINT ksapitcp_managercom_mngport_get(
		OV_INSTPTR_ksapitcp_managercom pobj) {
	return pobj->v_mngport;
}

/**
 * Sets the port which this class uses to communicate
 * with the ks_manager
 */
OV_DLLFNCEXPORT OV_RESULT ksapitcp_managercom_mngport_set(
		OV_INSTPTR_ksapitcp_managercom pobj, const OV_UINT value) {
	OV_INT udpsock = pobj->v_udpsocket;
	OV_INT tcpsock = pobj->v_tcpsocket;

	CLOSE_SOCKET(udpsock);
	CLOSE_SOCKET(tcpsock);
	pobj->v_udpsocket = -1;
	pobj->v_tcpsocket = -1;
	pobj->v_mngport = value;

	return OV_ERR_OK;
}

/**
 * This method is called on startup.
 * 
 * It also gets the port of the ks_manager from the portmapper
 * (if not set through the external variable MNGPORT).
 */
OV_DLLFNCEXPORT void ksapitcp_managercom_startup(OV_INSTPTR_ov_object pobj) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_ksapitcp_managercom pinst =
			Ov_StaticPtrCast(ksapitcp_managercom, pobj);

	/* do what the base class does first */
	ov_object_startup(pobj);
	//~ printf("\n\n\nksapitcp_MANAGERCOM STARTUP\n\n\n");
	pinst->v_xdr = NULL;

	//get managerport
	if (getenv("MNGPORT")) {
		pinst->v_mngport = atoi(getenv("MNGPORT"));
		ksapi_logfile_info("MNGPORT: %d", pinst->v_mngport);
	} else {
		ksapitcp_managercom_getmngport(pinst);
		ksapi_logfile_info("mngport: %d", pinst->v_mngport);
	}

	return;
}

/**
 * This method is called on shutdown.
 * 
 * It unregisters the server at the ks_manager and closes open socket
 */
OV_DLLFNCEXPORT void ksapitcp_managercom_shutdown(OV_INSTPTR_ov_object pobj) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_ksapitcp_managercom pinst =
			Ov_StaticPtrCast(ksapitcp_managercom, pobj);

	/* do what */
	//KSDEVEL ksapi_logfile_info("SHUTDOWN MANAGERCOM");

	CLOSE_SOCKET(pinst->v_udpsocket);
	CLOSE_SOCKET(pinst->v_tcpsocket);
	pinst->v_udpsocket = -1;
	pinst->v_tcpsocket = -1;
	pinst->v_mngport = 0;
	pinst->v_status = 1;

	/* set the object's state to "shut down" */
	ov_object_shutdown(pobj);

	return;
}

/**
 * This method gets the port of with the variables
 * NAME and VERSION declared server from the ks_manager.
 * 
 * The received port is stored in the variable
 * pobj->v_receivedsport.
 */
OV_DLLFNCEXPORT void ksapitcp_managercom_mnggetserver(
		OV_INSTPTR_ksapitcp_managercom pobj, OV_STRING name, OV_UINT version) {
	OV_UINT namelength = strlen(name);
	OV_UINT xdrnamelength;
	OV_INT c;
	OV_INT bytes;
	char *tmp;
	int sock;
	struct sockaddr_in server;
	struct hostent *hp;
	OV_INSTPTR_ov_domain managerdomain;
	OV_INSTPTR_ksservtcp_getserverdata pgsd;
	OV_INSTPTR_ksapitcp_TCPChannel channel;
	OV_VTBLPTR_ksapitcp_TCPChannel channelVTBL;
	OV_INSTPTR_ksapi_KSCommon  kscommon;
	
	//~ printf("\n\n\nksapitcp_MANAGAERCOM_MNGGETSERVER\n\n\n");
	pobj->v_receivedsport = 0;

	/** This means that ksapitcp depends on ksservtcp, and is the ONLY reason!
	 * THUS all ksapitcp servers are required to have a incoming tcp socket...
	 * At least Currently...one could change libraryopen code to prevent this
	 */
	//If server is MANAGER on its own (check by retrieving register obj), registration
	//is done locally
	managerdomain =
			(OV_INSTPTR_ov_domain) ov_path_getobjectpointer(
					"/communication/ksservtcp/manager", 2);
	pgsd =
					(OV_INSTPTR_ksservtcp_getserverdata) Ov_SearchChild(ov_containment, managerdomain, "getserverdata");
	channel =
			(OV_INSTPTR_ksapitcp_TCPChannel) Ov_GetParent(ov_containment, pobj);
	if (pgsd) {
		ov_logfile_info(
				"getserverdata-OBJECT found - server is MANAGER, local distribution of GETSERVER cmd");
		ksservtcp_getserverdata_name_set(pgsd, name);
		ksservtcp_getserverdata_version_set(pgsd, version);
		ksservtcp_getserverdata_process_set(pgsd, 1);
		if (pgsd->v_res == 1) { // we successfully found sth
			ov_logfile_info(
					"ManagerCom local GETSERVER: ANSWER w/ name: %s, version: %d, port: %d, secs: %d, usecs: %d",
					name, pgsd->v_version, pgsd->v_port,
					pgsd->v_expirationtime.secs, pgsd->v_expirationtime.usecs);
			pobj->v_receivedsport = pgsd->v_port;
			channel->v_serverport = pgsd->v_port;
			ov_logfile_info("local retrieved serverport: %d",
					pobj->v_receivedsport);
		} else { // cant locate server, but we are the Manager! Non registered!
			ov_logfile_info(
					"serverport: NONE, not found loally registered server for %s",
					pobj->v_receivedsport);
			pobj->v_receivedsport = 0;
			channel->v_serverport = 0;
		}
		channelVTBL = NULL;
		kscommon =
						(OV_INSTPTR_ksapi_KSCommon) Ov_GetParent(ov_containment, channel);
		Ov_GetVTablePtr(ksapitcp_TCPChannel, channelVTBL, channel);
		channelVTBL->m_sendxdr((OV_INSTPTR_ksapi_Channel) channel, kscommon,
				NULL, 0);
		return;
	}
	ov_logfile_info(
			"NO getserverdata-OBJECT, thus server is NOT Manager - remote sending...");

	if (pobj->v_mngport <= 0) {
		ksapi_logfile_error("no manager port set (mnggetserver)");
		return;
	}

	//set xdrlength
	xdrnamelength = strlen(name);
	while ((xdrnamelength % 4) != 0)
		xdrnamelength++;
	pobj->v_xdrlength = xdrnamelength + 52;
	//~ if(pobj->v_xdr) {
	//~ free(pobj->v_xdr);
	//~ }
	pobj->v_xdr = (char*) malloc(pobj->v_xdrlength);
	memset(pobj->v_xdr, 0, pobj->v_xdrlength);

	//set xid
	pobj->v_xdr[0] = 0x4a;
	pobj->v_xdr[1] = 0x5d;
	pobj->v_xdr[2] = 0x4f;
	pobj->v_xdr[3] = 0xe4;

	//set message type call --> 0000

	//set rpc version
	pobj->v_xdr[11] = 0x02;

	//set programm nr.
	pobj->v_xdr[13] = 0x04;
	pobj->v_xdr[14] = 0x96;
	pobj->v_xdr[15] = 0x78;

	//set programm version
	pobj->v_xdr[19] = 0x02;

	//set procedure
	pobj->v_xdr[22] = 0xff;
	pobj->v_xdr[23] = 0x03;

	//set servername
	tmp = (char*) &namelength;
	for (c = 0; c < 4; c++)
		pobj->v_xdr[44 + c] = tmp[3 - c];
	for (c = 0; c < namelength; c++)
		pobj->v_xdr[48 + c] = name[c];

	//set serverversion
	tmp = (char*) &version;
	for (c = 0; c < 4; c++)
		pobj->v_xdr[48 + xdrnamelength + c] = tmp[3 - c];

	//print xdr
	//~ int j;
	//~ printf("\n\nxdr:\nlength: %d\n", pobj->v_xdrlength);
	//~ for (j = 0; j < pobj->v_xdrlength; j=j+4)
	//~ {
	//~ if(((j!=0) && (j%20)) == 0)
	//~ printf("\n");
	//~ printf("%X %X %X %X     ", pobj->v_xdr[j], pobj->v_xdr[j+1], pobj->v_xdr[j+2], pobj->v_xdr[j+3]);
	//~ }
	//~ printf("\n");
	//~ for (j = 0; j < pobj->v_xdrlength; j=j+4)
	//~ {
	//~ if(((j!=0) && (j%20)) == 0)
	//~ printf("\n");
	//~ printf("%c %c %c %c     ", pobj->v_xdr[j], pobj->v_xdr[j+1], pobj->v_xdr[j+2], pobj->v_xdr[j+3]);
	//~ }
	//~ printf("\n\n");
	//~ printf("%s\n\n", pobj->v_xdr);

	//~ ksapi_logfile_info("sendxdr port: %d", port);

	//create socket
	//~ printf("\n\n\nMANAGAERCOM_MNGGETSERVER create socket\n\n\n");
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket(mnggetserver) failed");
		return;
	}
	pobj->v_tcpsocket = sock;

	//~ memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	hp = gethostbyname("127.0.0.1");
	if (hp == 0) {
		ksapi_logfile_info("Unknown host (mnggetserver)");
		CLOSE_SOCKET(sock);
		pobj->v_tcpsocket = -1;
		return;
	}
	memcpy((char *) &server.sin_addr, (char *) hp->h_addr, hp->h_length);
	server.sin_port = htons(pobj->v_mngport);

	//send data

	//~ printf("\n\n\nMANAGAERCOM_MNGGETSERVER connect\n\n\n");
	if ((connect(sock, (struct sockaddr *) &server, sizeof(server))) == -1) {
		perror("connect(ksapi_managercom) failed");
		CLOSE_SOCKET(sock);
		pobj->v_tcpsocket = -1;
		return;
	}
	//~ printf("\n\n\nMANAGAERCOM_MNGGETSERVER send\n\n\n");
	bytes = send(sock, pobj->v_xdr, pobj->v_xdrlength, 0);
	if (bytes < 0) {
		ksapi_logfile_info("send(mnggetserver) failed");
		CLOSE_SOCKET(sock);
		pobj->v_tcpsocket = -1;
		return;
	}

	//activate typemethod for receiving
	//~ printf("\n\n\nMANAGAERCOM_MNGGETSERVER activate typemethode\n\n\n");
	pobj->v_cycIntervalCount = 0;
	pobj->v_actimode = 1;

	return;
}

/**
 * This method gets the port of the ks_manager from the portmapper.
 * DEPRICATED
 */
OV_DLLFNCEXPORT void ksapitcp_managercom_getmngport(
		OV_INSTPTR_ksapitcp_managercom pobj) {
	OV_INT bytes;
	int sock;
	struct sockaddr_in server;
	struct hostent *hp;
	//~ printf("\n\n\nksapitcp_MANAGERCOM GETMNGPORT\n\n\n");
	//set xdrlength
	pobj->v_xdrlength = 56;
	if (pobj->v_xdr) {
		free(pobj->v_xdr);
	}
	pobj->v_xdr = (char*) malloc(pobj->v_xdrlength);
	memset(pobj->v_xdr, 0, pobj->v_xdrlength);

	//set xid
	pobj->v_xdr[0] = 0x4a;
	pobj->v_xdr[1] = 0x5d;
	pobj->v_xdr[2] = 0x4f;
	pobj->v_xdr[3] = 0xe4;

	//set message type call --> 0000

	//set rpc version
	pobj->v_xdr[11] = 0x02;

	//set programm nr.
	pobj->v_xdr[13] = 0x01;
	pobj->v_xdr[14] = 0x86;
	pobj->v_xdr[15] = 0xa0;

	//set programm version
	pobj->v_xdr[19] = 0x02;

	//set procedure
	pobj->v_xdr[23] = 0x03;

	//set programm nr. to get port from
	pobj->v_xdr[41] = 0x04;
	pobj->v_xdr[42] = 0x96;
	pobj->v_xdr[43] = 0x78;

	//set programm version to get port from
	pobj->v_xdr[47] = 0x02;

	//set protocoll
	//pobj->v_xdr[51] = 0x11;  // --> udp
	pobj->v_xdr[51] = 0x06; // --> tcp

	//print xdr
	//~ int j;
	//~ printf("\n\nxdr:\nlength: %d\n", pobj->v_xdrlength);
	//~ for (j = 0; j < pobj->v_xdrlength; j=j+4)
	//~ {
	//~ if(((j!=0) && (j%20)) == 0)
	//~ printf("\n");
	//~ printf("%X %X %X %X     ", pobj->v_xdr[j], pobj->v_xdr[j+1], pobj->v_xdr[j+2], pobj->v_xdr[j+3]);
	//~ }
	//~ printf("\n");
	//~ for (j = 0; j < pobj->v_xdrlength; j=j+4)
	//~ {
	//~ if(((j!=0) && (j%20)) == 0)
	//~ printf("\n");
	//~ printf("%c %c %c %c     ", pobj->v_xdr[j], pobj->v_xdr[j+1], pobj->v_xdr[j+2], pobj->v_xdr[j+3]);
	//~ }
	//~ printf("\n\n");
	//~ printf("%s\n\n", pobj->v_xdr);

	//create socket

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//if (sock == 0) {
#if OV_SYSTEM_NT
	if (sock == INVALID_SOCKET) {
		perror ("socket(getmngport) failed");
#else
	if (sock < 0) {
		perror("socket(getmngport) failed");
#endif
		return;
	}
	pobj->v_udpsocket = sock;
	pobj->v_status = STPMSOCKCR;

	server.sin_family = AF_INET;
	hp = gethostbyname("127.0.0.1");
	if (hp == 0) {
		ksapi_logfile_info("Unknown host (ksapitcp_managercom)");
		CLOSE_SOCKET(sock);
		pobj->v_udpsocket = -1;
		return;
	}
	memcpy((char *) &server.sin_addr, (char *) hp->h_addr, hp->h_length);
	server.sin_port = htons(111);

	//send data
	bytes = sendto(sock, pobj->v_xdr, pobj->v_xdrlength, 0,
			(struct sockaddr *) &server, sizeof(server));
	if (bytes < 0) {
		ksapi_logfile_info("send(getmngport) failed");
		CLOSE_SOCKET(sock);
		pobj->v_udpsocket = -1;
		return;
	}
	pobj->v_status = STPMRQSND;
	//~ ksapi_logfile_info("send(getmngport) ok");

	//activate typemethod for receiving
	pobj->v_cycIntervalCount = 0;
	pobj->v_actimode = 1;

	return;
}

OV_DLLFNCEXPORT void ksapitcp_managercom_typemethod(
		OV_INSTPTR_ksserv_ComTask cTask) {
	OV_INSTPTR_ksapitcp_managercom pobj =
			Ov_StaticPtrCast(ksapitcp_managercom, cTask);
	OV_INSTPTR_ksapitcp_TCPChannel channel =
			(OV_INSTPTR_ksapitcp_TCPChannel) Ov_GetParent(ov_containment, pobj);
	OV_VTBLPTR_ksapitcp_TCPChannel channelVTBL = NULL;
	OV_INSTPTR_ksapi_KSCommon kscommon =
			(OV_INSTPTR_ksapi_KSCommon) Ov_GetParent(ov_containment, channel);
	OV_INT bytes, c;
	OV_UINT mngport;
	OV_UINT xdrnamelength = 0;
	OV_INT rcvdserverport;
	char buffer[4096];
	int udpsocket = pobj->v_udpsocket;
	int tcpsocket = pobj->v_tcpsocket;
#if OV_SYSTEM_NT
	int size_from;
#else
	socklen_t size_from;
#endif
	struct sockaddr_in from;
	char temp[4];

	Ov_GetVTablePtr(ksapitcp_TCPChannel, channelVTBL, channel);
	//~ printf("\n\n\nksapitcp_MANAGERCOM TYPEMETHOD\nudpsocket: %d\ntcpsocket: %d\n\n\n", udpsocket, tcpsocket);
	if (udpsocket >= 0) {//retrieves manager port from portmapper
		//receive
		memset(buffer, 0, sizeof(buffer));
		//~ printf("\n\n\nksapitcp_MANAGERCOM TYPEMETHOD RECEIVE\n\n\n");
		bytes = recvfrom(udpsocket, buffer, sizeof(buffer), 0,
				(struct sockaddr *) &from, &size_from);
		//~ printf("\n\n\nksapitcp_MANAGERCOM TYPEMETHOD RECEIVED %d bytes\n\n\n", bytes);
		if (bytes <= 0) {
			CLOSE_SOCKET(udpsocket);
			pobj->v_udpsocket = -1;
			return;
		}
		pobj->v_status = STPMRPLYRCV;
		ksapi_logfile_info("recv(mngport) ok");

		pobj->v_xdrlength = bytes;
		if (pobj->v_xdr) {
			free(pobj->v_xdr);
		}
		pobj->v_xdr = (char*) malloc(pobj->v_xdrlength);
		memset(pobj->v_xdr, 0, pobj->v_xdrlength);
		memcpy(pobj->v_xdr, buffer, pobj->v_xdrlength);

		//print xdr
		//~ int j;
		//~ printf("\n\nxdr udp:\nlength: %d\n", pobj->v_xdrlength);
		//~ for (j = 0; j < pobj->v_xdrlength; j=j+4)
		//~ {
		//~ if(((j!=0) && (j%20)) == 0)
		//~ printf("\n");
		//~ printf("%X %X %X %X     ", pobj->v_xdr[j], pobj->v_xdr[j+1], pobj->v_xdr[j+2], pobj->v_xdr[j+3]);
		//~ }
		//~ printf("\n");
		//~ for (j = 0; j < pobj->v_xdrlength; j=j+4)
		//~ {
		//~ if(((j!=0) && (j%20)) == 0)
		//~ printf("\n");
		//~ printf("%c %c %c %c     ", pobj->v_xdr[j], pobj->v_xdr[j+1], pobj->v_xdr[j+2], pobj->v_xdr[j+3]);
		//~ }
		//~ printf("\n\n");
		//~ printf("%s\n\n", pobj->v_xdr);

		CLOSE_SOCKET(udpsocket);
		pobj->v_udpsocket = -1;

		for (c = 0; c < 4; c++)
			temp[c] = pobj->v_xdr[pobj->v_xdrlength - 1 - c];
		memcpy(&mngport, temp, 4);
		pobj->v_mngport = mngport;
	}

	if (pobj->v_mngport <= 0) {//close, if we dont have a mng port to communicate with
		CLOSE_SOCKET(udpsocket);
		CLOSE_SOCKET(tcpsocket);
		ksapitcp_managercom_getmngport(pobj);
		pobj->v_cycIntervalCount = 0;
		return;
	}

	if (tcpsocket >= 0) { //receive getserver answer
		memset(buffer, 0, sizeof(buffer));
		bytes = recv(tcpsocket, buffer, 4096, 0);
		if (bytes <= 0) {
			CLOSE_SOCKET(tcpsocket);
			pobj->v_tcpsocket = -1;
			return;
		} else {
			ksapi_logfile_info(
					"(ksapi_managercom/typemethod: getserver answer analysing");
			pobj->v_xdrlength = bytes;
			if (pobj->v_xdr) {
				free(pobj->v_xdr);
			}
			pobj->v_xdr = (char*) malloc(pobj->v_xdrlength);
			memset(pobj->v_xdr, 0, pobj->v_xdrlength);
			memcpy(pobj->v_xdr, buffer, pobj->v_xdrlength);

			CLOSE_SOCKET(tcpsocket);
			pobj->v_tcpsocket = -1;

			//print xdr
			//~ int j;
			//~ printf("\n\nxdr tcp:\nlength: %d\n", pobj->v_xdrlength);
			//~ for (j = 0; j < pobj->v_xdrlength; j=j+4)
			//~ {
			//~ if(((j!=0) && (j%20)) == 0)
			//~ printf("\n");
			//~ printf("%X %X %X %X     ", pobj->v_xdr[j], pobj->v_xdr[j+1], pobj->v_xdr[j+2], pobj->v_xdr[j+3]);
			//~ }
			//~ printf("\n");
			//~ for (j = 0; j < pobj->v_xdrlength; j=j+4)
			//~ {
			//~ if(((j!=0) && (j%20)) == 0)
			//~ printf("\n");
			//~ printf("%c %c %c %c     ", pobj->v_xdr[j], pobj->v_xdr[j+1], pobj->v_xdr[j+2], pobj->v_xdr[j+3]);
			//~ }
			//~ printf("\n\n");
			//~ printf("%s\n\n", pobj->v_xdr);

			if (pobj->v_xdr[31] == 0) {
				for (c = 0; c < 4; c++) {
					temp[c] = pobj->v_xdr[35 - c];
				}
				memcpy(&xdrnamelength, temp, 4);
				while ((xdrnamelength % 4) != 0)
					xdrnamelength++;
				for (c = 3; c >= 0; c--)
					temp[3 - c] = pobj->v_xdr[40 + xdrnamelength + c];
				memcpy(&rcvdserverport, temp, 4);
				pobj->v_receivedsport = rcvdserverport;
				channel->v_serverport = rcvdserverport;
				ksapi_logfile_info("receivedsport: %d", pobj->v_receivedsport);
			} else { // no server found
				ksapi_logfile_info("mnggetserver failed");
				pobj->v_receivedsport = 0;
				channel->v_serverport = 0;
			}
		}
		channelVTBL->m_sendxdr((OV_INSTPTR_ksapi_Channel) channel, kscommon,
				NULL, 0);
	}

	if ((pobj->v_udpsocket < 0) && (pobj->v_tcpsocket < 0))
		pobj->v_actimode = 0;

	return;
}
