/** @file 
 * Managersendrecv takes over the sending and receiving to/from the
 * ks_manager.
 *
 * This class is used by the managercom-class to send/receive the
 * generated xdrs.
 ******************************************************************************/
#ifndef OV_COMPILE_LIBRARY_ksservtcp
#define OV_COMPILE_LIBRARY_ksservtcp
#endif

#include "ksservtcp.h"
#include "config.h"
#include "ksserv_logfile.h"

#include "libov/ov_macros.h"
#include "libov/ov_path.h"

#include "assert.h"

#include <fcntl.h>

#if !OV_SYSTEM_NT
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#endif

//#include <time.h>
#define h_addr h_addr_list[0]
/**
 * Returns the port which this class uses to communicate
 * with the ks_manager.
 */
OV_DLLFNCEXPORT OV_UINT ksservtcp_managersendrecv_mngport_get(
		OV_INSTPTR_ksservtcp_managersendrecv pobj) {
	return pobj->v_mngport;
}

/**
 * Sets the port which this class uses to communicate
 * with the ks_manager.
 */
OV_DLLFNCEXPORT OV_RESULT ksservtcp_managersendrecv_mngport_set(
		OV_INSTPTR_ksservtcp_managersendrecv pobj, const OV_UINT value) {
	OV_INSTPTR_ksservtcp_managercom
			pmc =
					(OV_INSTPTR_ksservtcp_managercom) Ov_GetParent(ov_containment, pobj);
	OV_INT sock = pobj->v_socket;
	OV_INT sockmc = pmc->v_socket;

	ksserv_logfile_error("@@@@@@@@@@@@@@@@@@@@@@@ managersendrecv/mngport set: got new port");

	CLOSE_SOCKET(sock);
	CLOSE_SOCKET(sockmc);
	pobj->v_socket = -1;
	pmc->v_socket = -1;
	pobj->v_mngport = value;
	pmc->v_mngport = value;
	return OV_ERR_OK;
}

/**
 * This method is called on startup.
 */
OV_DLLFNCEXPORT void ksservtcp_managersendrecv_startup(
		OV_INSTPTR_ov_object pobj) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_ksservtcp_managersendrecv pinst =
			Ov_StaticPtrCast(ksservtcp_managersendrecv, pobj);
	/* do what the base class does first */
	ov_object_startup(pobj);

	pinst->v_actimode = 0;

	return;
}


/**
 * This method is called on shutdown.
 * It calls the typemethod until state is finished (most propaply we sent a UNREGISTER)
 * afterwards closes open socket
 */
OV_DLLFNCEXPORT void ksservtcp_managersendrecv_shutdown(
		OV_INSTPTR_ov_object pobj) {
	OV_INSTPTR_ksservtcp_managersendrecv pinst = Ov_StaticPtrCast(ksservtcp_managersendrecv, pobj);
	//struct timespec req;

	ksserv_logfile_error("managersendrecv/shutdown: shutting down - BLOCKING shutdown until unused (sending unregister)");
	while(pinst->v_state != STATUS_MANAGERSENDRECV_UNUSED && pinst->v_shutdowncounter < 5) { // sth is to do, not yet closing
		ksserv_logfile_error("managersendrecv/shutdown: waiting for sending ...");
		//wait a bit for not not eating the cpu
#if !OV_SYSTEM_NT
		//this would be the uptodate solution ... but we want to stick for c99 ....
		//req.tv_sec=0;
		//req.tv_nsec=1000000;
		//nanosleep(req, req);
		usleep(1000);
#else
    	Sleep(1000);
#endif
		ksservtcp_managersendrecv_typemethod((OV_INSTPTR_ksserv_ComTask)pinst);
		pinst->v_shutdowncounter++;
	}
	ksserv_logfile_error("managersendrecv/shutdown: will shut down...");
	CLOSE_SOCKET(pinst->v_socket);
	pinst->v_socket = -1;
	pinst->v_mngport = 0;
	pinst->v_operatingfunction = 0;
	pinst->v_actimode = 0;
	pinst->v_state = STATUS_MANAGERSENDRECV_UNUSED;

	/* set the object's state to "shut down" */
	//ov_object_shutdown((OV_INSTPTR_ov_object)pinst);
}

/**
 * This method send the in the variable pobj->v_xdr saved
 * xdr to the ks_manger and registers the receive-method
 * at the ov_scheduler.
 */
OV_DLLFNCEXPORT void ksservtcp_managersendrecv_sendxdr(
		OV_INSTPTR_ksservtcp_managersendrecv pobj) {
	int sock, flags;
	struct sockaddr_in server;
	struct hostent *hp;
#if OV_SYSTEM_NT
	unsigned long  dummy;
#endif   
	if (pobj->v_mngport <= 0) {
		ov_logfile_error("no manager port set");
		return;
	}

	//~ ov_logfile_info("sendxdr");

	//print xdr
	//~ int j;
	//~ printf("\n\nxdr:\n");
	//~ for (j = 0; j < pobj->v_xdrlength; j=j+4)
	//~ printf("%X %X %X %X     ", pobj->v_xdr[j], pobj->v_xdr[j+1], pobj->v_xdr[j+2], pobj->v_xdr[j+3]);
	//~ printf("\n\n");
	//~ for (j = 5; j < pobj->v_xdrlength; j=j+4)
	//~ printf("%c %c %c %c     ", pobj->v_xdr[j], pobj->v_xdr[j+1], pobj->v_xdr[j+2], pobj->v_xdr[j+3]);
	//~ printf("\n\n");
	//~ printf("%s\n\n", pobj->v_xdr);
	ksserv_logfile_error("managersendrecv/sendxdr: Check XDR which has to be sent: %p w/ length %d", pobj->v_xdr, pobj->v_xdrlength);

	if(pobj->v_actimode == 1 || pobj->v_state != STATUS_MANAGERSENDRECV_UNUSED) { //something is wrong...we seem to be INUSE!
		ov_logfile_error("managersendrecv/sendxdr called on a running instance! ignoring this request!");
		free(pobj->v_xdr);
		pobj->v_xdr = 0;
		return;
	}

	if (pobj->v_socket >= 0) { //cleanup, if not done already
		CLOSE_SOCKET(pobj->v_socket);
		pobj->v_socket = -1;
	}

	//create socket
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket(ksservtcp) failed");
		return;
	}
#if OV_SYSTEM_NT
    ioctlsocket(sock,FIONBIO,&dummy);              // Set non-blocking
#else
    flags = fcntl(sock,F_GETFL,0);              // Get socket flags
	fcntl(sock,F_SETFL,flags | O_NONBLOCK);   // Add non-blocking flag
#endif


	// conencting - building server adr struct
	server.sin_family = AF_INET;
	hp = gethostbyname("127.0.0.1");
	if (hp == 0) {
		ov_logfile_error("Unknown own host (ksservtcp)");
		CLOSE_SOCKET(sock);
		return;
	}
	memcpy((char *) &server.sin_addr, (char *) hp->h_addr, hp->h_length);
	server.sin_port = htons(pobj->v_mngport);
	//connect socket
	ov_logfile_error("ksservtcp_managersendrecv: calling connect to port %d", pobj->v_mngport);
	if (connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
		perror("connect(ksservtcp_managersendrecv) failed");
		//CLOSE_SOCKET(sock);
		//return;
	}
	pobj->v_state = STATUS_MANAGERSENDRECV_WAITINGFORSENDING; //ready for waiting to send
	pobj->v_socket = sock; //store the successfully nonblocking socket
	pobj->v_actimode = 1; //start waiting for sending XDR on nonblocking socket
	ov_logfile_info("managersendrecv: sendxdr - prepared socket %d sending %p and enabled typemethod", sock, pobj->v_xdr);
	return;
}

/**
 * This method receives the answer of the ks_manager.
 */
void ksservtcp_managersendrecv_typemethod(
		OV_INSTPTR_ksserv_ComTask cTask
) {
	OV_INSTPTR_ksservtcp_managersendrecv pinst = Ov_StaticPtrCast(ksservtcp_managersendrecv, cTask);
	OV_INSTPTR_ksservtcp_managercom	pmc = (OV_INSTPTR_ksservtcp_managercom) Ov_GetParent(ov_containment, cTask);
	int sock = pinst->v_socket;
	int err; // holds return values
	int bytes = 0; 
	char buffer[4098];
	fd_set read_flags,write_flags;
	struct timeval waitd;

	ov_logfile_info("managersendrecv: typemethod called - state %d using socket %d for sending %p", pinst->v_state, sock, pinst->v_xdr);
	//check socket
	if (sock < 0) {
		ov_logfile_error("managersendrecv: no socket but cycling?? - disabling typemethod");
		pinst->v_actimode = 0;
		return;
	}

	if(pinst->v_shutdowncounter > 5) {//give up
		//reset everything back
		ov_logfile_debug("Managersendrecv waited too long ... giving up on socket %d ", sock);
		free(pinst->v_xdr);
		pinst->v_xdr = 0;
		CLOSE_SOCKET(sock);
		pinst->v_socket = -1;
		pinst->v_operatingfunction = 0;
		pmc->v_operatingfunction = 0;
		pinst->v_shutdowncounter = 0;

		pinst->v_actimode = 0;
		pinst->v_state = STATUS_MANAGERSENDRECV_UNUSED;
		return;
	}


	waitd.tv_sec = 1;     // Make select wait up to 1 second for data TODO shorter
    waitd.tv_usec = 0;    // and 0 milliseconds.
    FD_ZERO(&read_flags); // Zero the flags ready for using
    FD_ZERO(&write_flags);
    FD_SET(sock, &read_flags);
    //just get write permissions if waiting for sending
    //if(pinst->v_state == STATUS_MANAGERSENDRECV_WAITINGFORSENDING)
    	FD_SET(sock, &write_flags);
    err = select(sock + 1, &read_flags,&write_flags, (fd_set*)0,&waitd);
	ov_logfile_debug("Managersendrecv: socket select returned %d", err);
	if(err < 0) {
		perror("Managersendrecv: error sets actimode=0");
		pinst->v_actimode = 0;
		return;
	}
	//ok, something arrived...
	if(pinst->v_state == STATUS_MANAGERSENDRECV_WAITINGFORSENDING) {
		if(! FD_ISSET(sock, &write_flags)){
			ov_logfile_debug("Managersendrecv waiting for sending (tried %d times)...",pinst->v_shutdowncounter );
			pinst->v_shutdowncounter ++;
			if(FD_ISSET(sock, &read_flags)) ov_logfile_debug("..but we could read! (state mismatch)");
			return;
		}
		FD_CLR(sock, &write_flags);
		bytes = send(sock, pinst->v_xdr, pinst->v_xdrlength, 0); //send it!
		if(bytes == -1) {
			ov_logfile_error("Managersendrecv/typemethod: write hasnt sent anything (returned -1) retry to send %p w/ %d bytes", pinst->v_xdr, pinst->v_xdrlength);
#if !OV_SYSTEM_NT
			perror("Managersendrecv: error while sending");
#else
			ov_logfile_error("Managersendrecv: error while sending: %d", WSAGetLastError());
#endif
			return;
		}
		ov_logfile_error("Managersendrecv/typemethod: write has sent %d bytes", bytes);
		pinst->v_state = STATUS_MANAGERSENDRECV_WAITINGFORANSWER; //wait for answer now
		pinst->v_shutdowncounter = 0;
		return;
	} else if (pinst->v_state == STATUS_MANAGERSENDRECV_WAITINGFORANSWER) {
		if(! FD_ISSET(sock, &read_flags)){
			ov_logfile_debug("Managersendrecv waiting for answer  (tried %d times)...",pinst->v_shutdowncounter);
			pinst->v_shutdowncounter ++;
			if(FD_ISSET(sock, &write_flags)) ov_logfile_debug("..but we could write! (state mismatch)");
			return;
		}
		FD_CLR(sock, &read_flags);
		if (recv(sock, buffer, sizeof(buffer)-1, 0) <= 0) {
			ov_logfile_info("Mgrsendreceived: got response but smaller 0? - once more waiting");
			pinst->v_shutdowncounter ++;
		} else { //cool, we got the answer
			//ov_logfile_info("Mgrsendreceived: got response!");
			// 		auswertung des empfangenen XDRs nicht moeglich,
			//		da keine register/unregister-fehlgeschlagen Befehle erzeugt werden konnten
			if (pinst->v_operatingfunction == 1) {
				//~ printf("\n\n\nregistered\n\n\n");
				ov_logfile_info("Mgrsendreceived: got response - registered at manager");
			} else if (pinst->v_operatingfunction == 2) {
				ov_logfile_info("Mgrsendreceived: got response - unregistered at manager");
			} else {
				ov_logfile_info("Mgrsendreceived: got response - unknown operatingfunction");
			}
			//ready for new work
			pinst->v_actimode = 0;
			pinst->v_state = STATUS_MANAGERSENDRECV_UNUSED;

			//reset everything back
			free(pinst->v_xdr);
			pinst->v_xdr = 0;
			CLOSE_SOCKET(sock);
			pinst->v_socket = -1;
			pinst->v_operatingfunction = 0;
			pmc->v_operatingfunction = 0;
			pinst->v_shutdowncounter = 0;
		} //end read success
	} else { //socket neither read or writeable or we are in a bad mood
		ov_logfile_debug("Managersendrecv waiting?!... obj state: %d", pinst->v_state);
	}


}
