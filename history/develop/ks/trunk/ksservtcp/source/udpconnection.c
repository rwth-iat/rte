/** @file 
 * This class takes over listening on UDP-port.
 * It handles incoming REGISTER, UNREGISTER and GETSERVER commands.
 ******************************************************************************/
#ifndef OV_COMPILE_LIBRARY_ksservtcp
#define OV_COMPILE_LIBRARY_ksservtcp
#endif

#include "ksservtcp.h"
#include "libov/ov_macros.h"
#include "libov/ov_scheduler.h"
#include "ksserv_logfile.h"
#include "config.h"
#include <errno.h>

#if !OV_SYSTEM_NT
#include <sys/time.h>
#else
#include <time.h>
#include <windows.h>
#endif

#include <stdio.h>
#if !OV_SYSTEM_NT
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/ioctl.h>
#endif
#define h_addr h_addr_list[0]
//ComTask void ksservtcp_udpconnection_typemethod(OV_INSTPTR_ov_object	pobj);

/**
 * Returns value of variable udpport.
 */
OV_DLLFNCEXPORT OV_INT ksservtcp_udpconnection_udpport_get(
		OV_INSTPTR_ksservtcp_udpconnection          pobj
) {
	return pobj->v_udpport;
}

/**
 * Sets value of variable udpport.
 *
 */
OV_DLLFNCEXPORT OV_RESULT ksservtcp_udpconnection_udpport_set(
		OV_INSTPTR_ksservtcp_udpconnection          pobj,
		const OV_INT           value
) {
	pobj->v_udpport = value;
	return OV_ERR_OK;
}

/**
 * Returns value of variable tcpport.
 */
OV_DLLFNCEXPORT OV_INT ksservtcp_udpconnection_tcpport_get(
		OV_INSTPTR_ksservtcp_udpconnection          pobj
) {
	return pobj->v_tcpport;
}

/**
 * Sets value of variable tcpport.
 *
 */
OV_DLLFNCEXPORT OV_RESULT ksservtcp_udpconnection_tcpport_set(
		OV_INSTPTR_ksservtcp_udpconnection          pobj,
		const OV_INT           value
) {

	pobj->v_tcpport = value;
	return OV_ERR_OK;
}

/**
 * Returns value of variable socket.
 */
OV_DLLFNCEXPORT OV_INT ksservtcp_udpconnection_socket_get(
		OV_INSTPTR_ksservtcp_udpconnection          pobj
) {
	return pobj->v_socket;
}

/**
 * Sets value of variable udpport.
 */
OV_DLLFNCEXPORT OV_RESULT ksservtcp_udpconnection_socket_set(
		OV_INSTPTR_ksservtcp_udpconnection          pobj,
		const OV_INT           value
) {
	pobj->v_socket = value;
	return OV_ERR_OK;
}

/**
 * Returns status.
 */
OV_DLLFNCEXPORT OV_INT ksservtcp_udpconnection_status_get(
		OV_INSTPTR_ksservtcp_udpconnection          pobj
) {
	return pobj->v_status;
}

/**
 * Sets status.
 */
OV_DLLFNCEXPORT OV_RESULT ksservtcp_udpconnection_status_set(
		OV_INSTPTR_ksservtcp_udpconnection          pobj,
		const OV_INT           value
) {
	pobj->v_status = value;
	return OV_ERR_OK;
}

/**
 * Procedure called on object startup.
 * Gets UDP-port to listen on.
 */
OV_DLLFNCEXPORT void ksservtcp_udpconnection_startup(
		OV_INSTPTR_ov_object 	pobj
) {
	OV_INSTPTR_ksservtcp_udpconnection this = Ov_StaticPtrCast(ksservtcp_udpconnection, pobj);
	OV_TIME_SPAN CYCTIME;
	int port;

	ksserv_logfile_info("UDPConnection (for backwardcompatibility) startup.");
	CYCTIME.secs = 0;
	this->v_cycsecs = CYCTIME.secs;
	CYCTIME.usecs = 1;
	this->v_cycusecs = CYCTIME.usecs;

	//get OWNPORT
	if(getenv("MNGLIBPORT"))
	{
		port = atoi(getenv("MNGLIBPORT"));
		ksserv_logfile_info("MNGLIBPORT: %d", port);
	}
	else
	{
		port = 7509;
	}
	ksservtcp_udpconnection_udpport_set(this, port);

	//register typemethod
	//ComTask   Ov_WarnIfNot(Ov_OK(ov_scheduler_register(pobj, ksservtcp_udpconnection_typemethod)));
	//ComTask	ov_scheduler_setreleventtime(pobj, &CYCTIME);
	return;
}

/**
 * Procedure called on object shutdown. 
 * It closes open socket and unregisters manager at portmapper.
 */
OV_DLLFNCEXPORT void ksservtcp_udpconnection_shutdown(
		OV_INSTPTR_ov_object 	pobj
) {
	OV_INSTPTR_ksservtcp_udpconnection this = Ov_StaticPtrCast(ksservtcp_udpconnection, pobj);
	int sock;
	char unsetxdr[56];
	int xdrlength = 56;
	struct sockaddr_in server;
	unsigned int server_len = sizeof(server);
	struct hostent *hp;
	int bytes;
	char buffer[4096];

	sock = ksservtcp_udpconnection_socket_get(this);

#if OV_SYSTEM_NT
	if((CLOSE_SOCKET(sock)))
	{
		errno = WSAGetLastError();
#else
	if(!(CLOSE_SOCKET(sock)))
	{
#endif
		perror("shutdown listen");
	}

	ksservtcp_udpconnection_socket_set(this, -1);

	return;
}

/**
 * Procedure periodically called by ov_scheduler.
 * It takes over the listening on UDP-port and handles incoming
 * REGISTER, UNREGISTER and GETSERVER commands.
 */
void ksservtcp_udpconnection_typemethod(
		//	OV_INSTPTR_ov_object	pobj
		OV_INSTPTR_ksserv_ComTask	cTask
) {
	OV_INSTPTR_ksservtcp_udpconnection this = Ov_StaticPtrCast(ksservtcp_udpconnection, cTask);
	OV_INSTPTR_ov_domain ksservtcpdomain = (OV_INSTPTR_ov_domain)Ov_GetParent(ov_containment, cTask);
	OV_INSTPTR_ksservtcp_register preg = (OV_INSTPTR_ksservtcp_register)Ov_SearchChild(ov_containment, ksservtcpdomain, "register");
	OV_INSTPTR_ksservtcp_unregister pureg = (OV_INSTPTR_ksservtcp_unregister)Ov_SearchChild(ov_containment, ksservtcpdomain, "unregister");
	OV_INSTPTR_ksservtcp_getserverdata pgsd = (OV_INSTPTR_ksservtcp_getserverdata)Ov_SearchChild(ov_containment, ksservtcpdomain, "getserverdata");
	OV_TIME_SPAN CYCTIME;
	int sock = ksservtcp_udpconnection_socket_get(this);
	int optval = 1; //used by setsockopt for reuseage of tcp connection port
	struct sockaddr_in server_addr, client_addr;
	unsigned int client_addr_len = sizeof(client_addr);
	int port = ksservtcp_udpconnection_udpport_get(this);
	char buffer[4096];
	int bytes = 0;
	int procedure;
	char temp[4];
	int j;
	int on = 1;
	char *receivedname;
	int receivednamelength, receivednamelengthxdr;
	int receivedversion;
	int receivedport;
	int receivedttl;
	unsigned int secs, usecs;
	char xid[4];
	char *replyxdr = NULL;
	int replyxdrlength = 0;
	char *buf;

	//check if register and unregister objects exist
	if((!preg) || (!pureg))
	{
		ksserv_logfile_error("ksservtcp_register or ksservtcp_unregister not found");
		return;
	}

	CYCTIME.secs = this->v_cycsecs;
	CYCTIME.usecs = this->v_cycusecs;

	//create socket
	if(sock <= 0)
	{
		//open socket
		sock = socket(AF_INET, SOCK_DGRAM, 0);
		ksservtcp_udpconnection_socket_set(this, sock);
		if (sock == -1)
		{
			perror("socket(udpconnection) failed");
			//ov_scheduler_setreleventtime(pobj, &CYCTIME);
			return;
		}

		//non-blocking
		if ((IOCTL_SOCKET(sock, FIONBIO, (char*)&on)) == -1)
		{
			perror("ioctl(udpconnection) failed");
			CLOSE_SOCKET(sock);
			ksservtcp_udpconnection_socket_set(this, -1);
			//ov_scheduler_setreleventtime(pobj, &CYCTIME);
			return;
		}

		//bind
		memset(&server_addr, 0, sizeof(server_addr));
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		server_addr.sin_port = htons(port);

		//test resueage!
		ksserv_logfile_info("########## reuseage of used port by setsockopt ");
		setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

		if ((bind(sock, (struct sockaddr*) &server_addr, sizeof(server_addr))) == -1)
		{
			perror("bind(udpconnection) failed");
			CLOSE_SOCKET(sock);
			ksservtcp_udpconnection_socket_set(this, -1);
			//ov_scheduler_setreleventtime(pobj, &CYCTIME);
			return;
		}
	}

	//recv
	memset(buffer, 0, sizeof(buffer));
	bytes = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr*) &client_addr, &client_addr_len);
	if(bytes == -1)
	{
		//~ ksserv_logfile_error("recv(udpclient) failed");
	}
	else if(bytes > 0)
	{
		ov_logfile_info("udpconnection/typemethod recv(udpclient) > 0");

		//get size_received
		this->v_xdrlength = bytes;

		//set size of xdr
		this->v_xdr = (char*)malloc(this->v_xdrlength);
		memset(this->v_xdr, 0, this->v_xdrlength);
		memcpy(this->v_xdr, buffer, this->v_xdrlength);

		//print xdr
		//KSEDEVEL printf("\nxdr:\nlength: %d\n", this->v_xdrlength);
		//KSEDEVEL for (j = 0; j < this->v_xdrlength; j=j+4)
		//KSEDEVEL printf("%X %X %X %X     ", this->v_xdr[j], this->v_xdr[j+1], this->v_xdr[j+2], this->v_xdr[j+3]);
		//KSEDEVEL printf("\n\n");

		//get procedure
		for(j=0;j<4;j++)
			temp[3-j] = this->v_xdr[20+j];
		memcpy(&procedure, temp, 4);
		ksserv_logfile_info("UDPConnection/typemethod - procedure: %d", procedure);

		switch (procedure)
		{
		case 65281:	//register
			//get xid
			for(j=0;j<4;j++)
				xid[j] = this->v_xdr[j];
			//get name
			for(j=0;j<4;j++)
				temp[3-j] = this->v_xdr[44+j];
			memcpy(&receivednamelength, temp, 4);
			receivednamelengthxdr = receivednamelength;
			while ((receivednamelengthxdr%4)!=0)
				receivednamelengthxdr++;
			receivedname = (char*)malloc(receivednamelength+1);
			memset(receivedname, 0, receivednamelength+1);
			for(j=0;j<receivednamelength;j++)
				receivedname[j]= this->v_xdr[48+j];
			//get version
			for(j=0;j<4;j++)
				temp[3-j] = this->v_xdr[48+receivednamelengthxdr+j];
			memcpy(&receivedversion, temp, 4);
			//get port
			for(j=0;j<4;j++)
				temp[3-j] = this->v_xdr[52+receivednamelengthxdr+j];
			memcpy(&receivedport, temp, 4);
			//get ttl
			for(j=0;j<4;j++)
				temp[3-j] = this->v_xdr[56+receivednamelengthxdr+j];
			memcpy(&receivedttl, temp, 4);
			//set register object
			ksserv_logfile_info("UDPConnection: REGISTER cmd w/ name: %s, version %d, port: %d, ttl: %d",
					receivedname, receivedversion, receivedport,receivedttl);
			ksservtcp_register_name_set(preg, receivedname);
			ksservtcp_register_version_set(preg, receivedversion);
			ksservtcp_register_port_set(preg, receivedport);
			ksservtcp_register_timetolife_set(preg, receivedttl);
			ksservtcp_register_process_set(preg, 1);
			//generate answer
			replyxdrlength = 32;
			replyxdr = (char*)malloc(replyxdrlength);
			memset(replyxdr, 0, replyxdrlength);
			for(j=0;j<4;j++)
				replyxdr[j] = xid[j];
			replyxdr[7] = 1;
			break;
		case 65282:	//unregister
			//get xid
			for(j=0;j<4;j++)
				xid[j] = this->v_xdr[j];
			//get name
			for(j=0;j<4;j++)
				temp[3-j] = this->v_xdr[44+j];
			memcpy(&receivednamelength, temp, 4);
			receivednamelengthxdr = receivednamelength;
			while ((receivednamelengthxdr%4)!=0)
				receivednamelengthxdr++;
			receivedname = (char*)malloc(receivednamelength+1);
			memset(receivedname, 0, receivednamelength+1);
			for(j=0;j<receivednamelength;j++)
				receivedname[j]= this->v_xdr[48+j];
			//get version
			for(j=0;j<4;j++)
				temp[3-j] = this->v_xdr[48+receivednamelengthxdr+j];
			memcpy(&receivedversion, temp, 4);
			//set unregister object
			ksserv_logfile_info("UDPConnection: UNREGISTER cmd w/ name: %s, version %d", receivedname, receivedversion);
			ksservtcp_unregister_name_set(pureg, receivedname);
			ksservtcp_unregister_version_set(pureg, receivedversion);
			ksservtcp_unregister_process_set(pureg, 1);
			//generate answer
			replyxdrlength = 32;
			replyxdr = (char*)malloc(replyxdrlength);
			memset(replyxdr, 0, replyxdrlength);
			for(j=0;j<4;j++)
				replyxdr[j] = xid[j];
			replyxdr[7] = 1;
			if(pureg->v_res == -2)
				replyxdr[31] = 0x21;
			break;
		case 65283:	//getserver
			//get xid
			for(j=0;j<4;j++)
				xid[j] = this->v_xdr[j];
			//get name
			for(j=0;j<4;j++)
				temp[3-j] = this->v_xdr[44+j];
			memcpy(&receivednamelength, temp, 4);
			receivednamelengthxdr = receivednamelength;
			while ((receivednamelengthxdr%4)!=0)
				receivednamelengthxdr++;
			receivedname = (char*)malloc(receivednamelength+1);
			memset(receivedname, 0, receivednamelength+1);
			for(j=0;j<receivednamelength;j++)
				receivedname[j]= this->v_xdr[48+j];
			//get version
			for(j=0;j<4;j++)
				temp[3-j] = this->v_xdr[48+receivednamelengthxdr+j];
			memcpy(&receivedversion, temp, 4);
			//set getserverdata object
			ksserv_logfile_info("UDPConnection: GETSERVER cmd w/ name: %s, version %d", receivedname, receivedversion);
			ksservtcp_getserverdata_name_set(pgsd, receivedname);
			ksservtcp_getserverdata_version_set(pgsd, receivedversion);
			ksservtcp_getserverdata_process_set(pgsd, 1);
			//generate answer
			if(pgsd->v_res == 1)
			{
				receivedversion = pgsd->v_version;
				receivedport = pgsd->v_port;
				secs = pgsd->v_expirationtime.secs;
				usecs = pgsd->v_expirationtime.usecs;
				ksserv_logfile_info("UDPconnection: ANSWER GETSERVER w/ name: %s, version: %d, port: %d, secs: %d, usecs: %d",
						receivedname, receivedversion, receivedport, secs, usecs);
				replyxdrlength = 56+receivednamelengthxdr;
				replyxdr = (char*)malloc(replyxdrlength);
				memset(replyxdr, 0, replyxdrlength);
				for(j=0;j<4;j++)
					replyxdr[j] = xid[j];
				replyxdr[7] = 1;
				buf = (char*)&receivednamelength;
				memcpy(temp, buf, 4);
				for (j=0; j<4; j++)
					replyxdr[35-j] = temp[j];
				for(j=0;j<receivednamelength;j++)
					replyxdr[36+j] = receivedname[j];
				buf = (char*)&receivedversion;
				memcpy(temp, buf, 4);
				for (j=0; j<4; j++)
					replyxdr[39+receivednamelengthxdr-j] = temp[j];
				buf = (char*)&receivedport;
				memcpy(temp, buf, 4);
				for (j=0; j<4; j++)
					replyxdr[43+receivednamelengthxdr-j] = temp[j];
				buf = (char*)&secs;
				memcpy(temp, buf, 4);
				for (j=0; j<4; j++)
					replyxdr[47+receivednamelengthxdr-j] = temp[j];
				buf = (char*)&usecs;
				memcpy(temp, buf, 4);
				for (j=0; j<4; j++)
					replyxdr[51+receivednamelengthxdr-j] = temp[j];
				replyxdr[replyxdrlength-1] = 1;
			}
			else
			{
				replyxdrlength = 32;
				replyxdr = (char*)malloc(replyxdrlength);
				memset(replyxdr, 0, replyxdrlength);
				for(j=0;j<4;j++)
					replyxdr[j] = xid[j];
				replyxdr[7] = 1;
			}
			replyxdr[replyxdrlength-1] = 0x21;
			//print xdr
			//KSDEVEL printf("\nxdr (reply udpconnection):\nlength: %d\n", replyxdrlength);
			//KSDEVEL for (j = 0; j < replyxdrlength; j=j+4)
			//KSDEVEL printf("%X %X %X %X     ", replyxdr[j], replyxdr[j+1], replyxdr[j+2], replyxdr[j+3]);
			//KSDEVEL printf("\n\n");
			break;
		default:
			ksserv_logfile_error("UNKNOWN PROCEDURE");
			break;
		}

		//send reply
		sendto(sock, replyxdr, replyxdrlength, 0, (struct sockaddr*) &client_addr, client_addr_len);

	}

	//~ free(receivedname);

	//ov_scheduler_setreleventtime(pobj, &CYCTIME);




	return;
}
