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
#include <rpc/xdr.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "assert.h"
#if !OV_SYSTEM_NT
#include <unistd.h>
#include <sys/ioctl.h>
#include <netinet/tcp.h>
#else
#include <winsock2.h>
#endif

#ifndef EWOULDBLOCK
#define EWOULDBLOCK WSAEWOULDBLOCK
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
		pinst->v_mngport = 7509;
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
OV_DLLFNCEXPORT void ksapitcp_managercom_mnggetserver(OV_INSTPTR_ksapitcp_managercom pobj, OV_STRING host, OV_STRING servername, OV_UINT version) {
	OV_UINT namelength = strlen(servername);
	OV_UINT xdrnamelength;
	OV_INT c;
	OV_INT bytes;
	char *tmp;
	char* tmpfragment;
	int sock;
	int optval = 1;
	struct sockaddr_in server;
	struct hostent *hp;
	OV_INSTPTR_ov_domain managerdomain;
	OV_INSTPTR_ksservtcp_getserverdata pgsd;
	OV_INSTPTR_ksapitcp_TCPChannel channel;
	OV_VTBLPTR_ksapitcp_TCPChannel channelVTBL;
	OV_INSTPTR_ksapi_KSCommon  kscommon;
	OV_UINT lengthtemp;
	

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
	if (pgsd && (!ov_string_compare(host, "127.0.0.1") || !ov_string_compare(host, "localhost"))) { //dont ask local manager, if remote server is requested
		ov_logfile_info(
				"getserverdata-OBJECT found - server is MANAGER, local distribution of GETSERVER cmd");
		ksservtcp_getserverdata_name_set(pgsd, servername);
		ksservtcp_getserverdata_version_set(pgsd, version);
		ksservtcp_getserverdata_process_set(pgsd, 1);
		if (pgsd->v_res == 1) { // we successfully found sth
			ov_logfile_info(
					"ManagerCom local GETSERVER: ANSWER w/ name: %s, version: %d, port: %d, secs: %d, usecs: %d",
					servername, pgsd->v_version, pgsd->v_port,
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
	ksapi_logfile_info(
			"NO getserverdata-OBJECT, thus server is NOT Manager - remote sending...");

	if (pobj->v_mngport <= 0) {
		ksapi_logfile_error("no manager port set (mnggetserver)");
		return;
	}

	//set xdrlength
	xdrnamelength = strlen(servername);
	while ((xdrnamelength % 4) != 0)
		xdrnamelength++;
	pobj->v_xdrlength = xdrnamelength + 56;
	//~ if(pobj->v_xdr) {
	//~ free(pobj->v_xdr);
	//~ }
	pobj->v_xdr = (char*) malloc(pobj->v_xdrlength);
	memset(pobj->v_xdr, 0, pobj->v_xdrlength);

	lengthtemp = pobj->v_xdrlength - 4;		//first 4 bytes of xdr code length of the REST
	//add tcp-fragment-rpcheader
	tmpfragment = (char*)&(lengthtemp);
	for (c=0; c<4; c++)
		pobj->v_xdr[3-c] = tmpfragment[c];
	pobj->v_xdr[0] = 0x80;

	//set xid
	pobj->v_xdr[4] = 0x4a;
	pobj->v_xdr[5] = 0x5d;
	pobj->v_xdr[6] = 0x4f;
	pobj->v_xdr[7] = 0xe4;

	//set message type call --> 0000

	//set rpc version
	pobj->v_xdr[15] = 0x02;

	//set programm nr.
	pobj->v_xdr[17] = 0x04;
	pobj->v_xdr[18] = 0x96;
	pobj->v_xdr[19] = 0x78;

	//set programm version
	pobj->v_xdr[23] = 0x02;

	//set procedure
	pobj->v_xdr[26] = 0xff;
	pobj->v_xdr[27] = 0x03;

	//set servername
	tmp = (char*) &namelength;
	for (c = 0; c < 4; c++)
		pobj->v_xdr[48 + c] = tmp[3 - c];
	for (c = 0; c < namelength; c++)
		pobj->v_xdr[52 + c] = servername[c];

	//set serverversion
	tmp = (char*) &version;
	for (c = 0; c < 4; c++)
		pobj->v_xdr[52 + xdrnamelength + c] = tmp[3 - c];

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

	//disable nagle for the socket
	setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char *) &optval, sizeof(optval));

	//~ memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	hp = gethostbyname(host); //hp = gethostbyname("127.0.0.1");
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
	OV_UINT xdrnamelength = 0;
	OV_INT rcvdserverport;
	char buffer[4096];
	int tcpsocket = pobj->v_tcpsocket;

	char temp[4];

	Ov_GetVTablePtr(ksapitcp_TCPChannel, channelVTBL, channel);
	//~ printf("\n\n\nksapitcp_MANAGERCOM TYPEMETHOD\nudpsocket: %d\ntcpsocket: %d\n\n\n", udpsocket, tcpsocket);

	if (tcpsocket >= 0) { //receive getserver answer
		memset(buffer, 0, sizeof(buffer));
		bytes = recv(tcpsocket, buffer, 4096, 0);
#if OV_SYSTEM_NT
		errno = WSAGetLastError();
#endif
		if(bytes == -1 && ( errno != EAGAIN && errno != EWOULDBLOCK ) )
		{
			ksapi_logfile_debug("error receiving?, closing socket!");
			CLOSE_SOCKET(tcpsocket);
			pobj->v_tcpsocket = -1;
			return;
		} else {
			ksapi_logfile_debug(
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

			if (pobj->v_xdr[35] == 0) {
				for (c = 0; c < 4; c++) {
					temp[c] = pobj->v_xdr[39 - c];
				}
				memcpy(&xdrnamelength, temp, 4);
				while ((xdrnamelength % 4) != 0)
					xdrnamelength++;
				for (c = 3; c >= 0; c--)
					temp[3 - c] = pobj->v_xdr[44 + xdrnamelength + c];
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
