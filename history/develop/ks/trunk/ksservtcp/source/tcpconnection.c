/** @file 
 * This class takes over listening on TCP-port.
 * If connection-request is comming in it creates a tcpclient object
 * which handles the communication with requesting client.
 ******************************************************************************/
#ifndef OV_COMPILE_LIBRARY_ksservtcp
#define OV_COMPILE_LIBRARY_ksservtcp
#endif

#include "ksservtcp.h"
#include "xdrexec.h"
#include "config.h"
#include "ksserv_logfile.h"
#include "libov/ov_scheduler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/xdr.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>

#if !OV_SYSTEM_NT 
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#include "libov/ov_macros.h"
#include "ov_ks_missing_symbols.h"
#include "libov/ov_path.h"

//ComTask void ksservtcp_tcpconnection_typemethod(OV_INSTPTR_ov_object	pfb);
OV_RESULT ksservtcp_tcpconnection_createmanager();

/**
 * Returns the port on which the object is listening.
 */
OV_DLLFNCEXPORT OV_INT ksservtcp_tcpconnection_port_get(
		OV_INSTPTR_ksservtcp_tcpconnection pobj) {
	return pobj->v_port;
}

/**
 * Sets the port on which the object is listening.
 */
OV_DLLFNCEXPORT OV_RESULT ksservtcp_tcpconnection_port_set(
		OV_INSTPTR_ksservtcp_tcpconnection pobj, const OV_INT value) {
	CLOSE_SOCKET(pobj->v_listensocket);
	ksservtcp_tcpconnection_listensocket_set(pobj, -1);

	pobj->v_port = value;
	return OV_ERR_OK;
}

/**
 * Returns the socket on which the object is listening.
 */
OV_DLLFNCEXPORT OV_INT ksservtcp_tcpconnection_listensocket_get(
		OV_INSTPTR_ksservtcp_tcpconnection pobj) {
	return pobj->v_listensocket;
}

/**
 * Sets the socket on which the object is listening.
 */
OV_DLLFNCEXPORT OV_RESULT ksservtcp_tcpconnection_listensocket_set(
		OV_INSTPTR_ksservtcp_tcpconnection pobj, const OV_INT value) {
	pobj->v_listensocket = value;
	return OV_ERR_OK;
}

/**
 * Procedure called on object startup.
 * It creates a managercom-object which takes over the
 * communication with the ks_manager. 
 * It starts the listening on with variable OWNPORT
 * declared port or by default on port 7509.
 */
OV_DLLFNCEXPORT void ksservtcp_tcpconnection_startup(OV_INSTPTR_ov_object pobj) {
	OV_INSTPTR_ksservtcp_tcpconnection this =
			Ov_StaticPtrCast(ksservtcp_tcpconnection, pobj);
	OV_INSTPTR_ov_domain thisdomain = Ov_StaticPtrCast(ov_domain, pobj);
	OV_INSTPTR_ksservtcp_managercom
			pmc =
					(OV_INSTPTR_ksservtcp_managercom) Ov_SearchChild(ov_containment, thisdomain, "managercom");
	OV_INSTPTR_ov_domain
			pclients =
					(OV_INSTPTR_ov_domain) Ov_SearchChild(ov_containment, thisdomain, "clients");
	OV_INSTPTR_ksservtcp_tcpclient ptcpc = NULL;
	OV_INSTPTR_ksservtcp_udpconnection pudpc;
	OV_VAR_CURRENT_PROPS srvnameprops;
	OV_RESULT res;
	int port;

	//KSDEVEL ksserv_logfile_info("STARTUP TCPCONNECTION");

	//decide Port to use
	if (getenv("OWNPORT")) {//OWNPORT set
		port = atoi(getenv("OWNPORT"));
	} else {//OWNPORT not set --> default port 7509 --> create manager-objects
		port = 7509;
	}
	//decide Name to use 
	if (port == 7509) {//OWNPORT not set --> if port 7509 --> MANAGER
		ov_vendortree_setservername("MANAGER");
		ov_logfile_info("Forced KS servername to: MANAGER. Could be different in vendortree!");
	}// else: use default of libov (--> ov_server_USERNAME)

	//decide if Manager --> create components
	ov_vendortree_getservername(&srvnameprops, NULL);
	if (port == 7509 || ov_string_compare(
			srvnameprops.value.valueunion.val_string, "MANAGER") == OV_STRCMP_EQUAL) {
		res = ksservtcp_tcpconnection_createmanager();
		if ((!Ov_OK(res)))
			ov_logfile_error("Cannot load manager library");
		pudpc = (OV_INSTPTR_ksservtcp_udpconnection) ov_path_getobjectpointer(
				"/communication/ksservtcp/manager/udpconnection", 2);
		if (!pudpc) {
			ksserv_logfile_error("Cannot locate 'udpconnection' domain!");
		} else {
			ksservtcp_udpconnection_tcpport_set(pudpc, port);
		}
	}

	//store port to object
	ksservtcp_tcpconnection_port_set(this, port);

	//create clients domain
	if (pclients) {
		ptcpc
				= (OV_INSTPTR_ksservtcp_tcpclient) Ov_GetFirstChild(ov_containment, pclients);
		while (ptcpc) {
			ksservtcp_tcpclient_shutdown((OV_INSTPTR_ov_object) ptcpc);
			ptcpc
					= (OV_INSTPTR_ksservtcp_tcpclient) Ov_GetNextChild(ov_containment, pclients);
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

	//register typemethod
	//ComTask Ov_WarnIfNot(Ov_OK(ov_scheduler_register(pobj, ksservtcp_tcpconnection_typemethod)));

	//create managercom
	if (pmc) {
		Ov_DeleteObject(pmc);
	}
	if (!Ov_OK((Ov_CreateObject(ksservtcp_managercom, pmc, thisdomain, "managercom")))) {
		ksserv_logfile_error("Fatal: Couldnt create Object managercom");
		return;
	} else {
		pmc->v_sport = this->v_port;
		ksservtcp_managercom_activatetypemethod_set(pmc, TRUE);
	}

	return;
}

/**
 * Procedure called on object shutdown. 
 * It closes open socket and deletes created
 * managercom- and manager-objects.
 */
OV_DLLFNCEXPORT void ksservtcp_tcpconnection_shutdown(OV_INSTPTR_ov_object pobj) {
	int listensocket;
	OV_INSTPTR_ksservtcp_tcpconnection this =
			Ov_StaticPtrCast(ksservtcp_tcpconnection, pobj);
	OV_INSTPTR_ov_domain thisdomain = Ov_StaticPtrCast(ov_domain, pobj);
	OV_INSTPTR_ksservtcp_managercom
			pmc =
					(OV_INSTPTR_ksservtcp_managercom) Ov_SearchChild(ov_containment, thisdomain, "managercom");
	OV_INSTPTR_ov_domain pksserv =
			(OV_INSTPTR_ov_domain) ov_path_getobjectpointer(
					"/communication/ksservtcp", 2);
	OV_INSTPTR_ov_domain proot =
			(OV_INSTPTR_ov_domain) ov_path_getobjectpointer("/", 2);
	OV_INSTPTR_ov_domain
			pmanager =
					(OV_INSTPTR_ov_domain) Ov_SearchChild(ov_containment, pksserv, "manager");
	OV_INSTPTR_ov_domain
			pserverlist =
					(OV_INSTPTR_ov_domain) Ov_SearchChild(ov_containment, proot, "servers");
	OV_INSTPTR_ksservtcp_register pmr = NULL;
	OV_INSTPTR_ksservtcp_unregister pmur = NULL;
	OV_INSTPTR_ksservtcp_getserverdata pmgsd = NULL;
	OV_INSTPTR_ksservtcp_udpconnection pmudp = NULL;

	//KSDEVEL ksserv_logfile_info("SHUTDOWN TCPCONNECTION");

	//ComTask ov_scheduler_unregister(pobj);

	//close socket
	listensocket = ksservtcp_tcpconnection_listensocket_get(this);
#if OV_SYSTEM_NT
	if((CLOSE_SOCKET(listensocket)))
	{
		errno = WSAGetLastError();
#else
	if(!(CLOSE_SOCKET(listensocket)))
	{
#endif
		perror("shutdown listen");
	}

	ksservtcp_tcpconnection_listensocket_set(this, -1);

	//delete managercom-object if existing
	if (pmc)
		Ov_DeleteObject(pmc);

	//delete manager-objects if existing
	if (pmanager) {
		pmr
				= (OV_INSTPTR_ksservtcp_register) Ov_SearchChild(ov_containment, pmanager, "register");
		if (pmr)
			Ov_DeleteObject(pmr);
		pmur
				= (OV_INSTPTR_ksservtcp_unregister) Ov_SearchChild(ov_containment, pmanager, "unregister");
		if (pmur)
			Ov_DeleteObject(pmur);
		pmgsd
				= (OV_INSTPTR_ksservtcp_getserverdata) Ov_SearchChild(ov_containment, pmanager, "getserverdata");
		if (pmgsd)
			Ov_DeleteObject(pmgsd);
		pmudp
				= (OV_INSTPTR_ksservtcp_udpconnection) Ov_SearchChild(ov_containment, pmanager, "udpconnection");
		if (pmudp)
			Ov_DeleteObject(pmudp);
		Ov_DeleteObject(pmanager);
	}
	if (pserverlist)
		Ov_DeleteObject(pserverlist);

	return;
}

/**
 * Procedure periodically called by Root ComTask //ov_scheduler.
 * It takes over the connection-management.
 * If there is an incomming connection-request on
 * listensocket it creates an tcpclient-object which
 * will take over the communication with requesting client.
 */
void ksservtcp_tcpconnection_typemethod(OV_INSTPTR_ksserv_ComTask cTask
//ComTask OV_INSTPTR_ov_object	pfb
) {
	OV_INSTPTR_ksservtcp_tcpconnection this =
			Ov_StaticPtrCast(ksservtcp_tcpconnection, cTask);
	OV_INSTPTR_ov_domain thisdomain = Ov_StaticPtrCast(ov_domain, cTask);
	OV_INSTPTR_ksservtcp_managercom
			pmc =
					(OV_INSTPTR_ksservtcp_managercom) Ov_SearchChild(ov_containment, thisdomain, "managercom");
	OV_INSTPTR_ksservtcp_tcpclient ptcpc = NULL;
	OV_INSTPTR_ov_domain
			pclients =
					(OV_INSTPTR_ov_domain) Ov_SearchChild(ov_containment, thisdomain, "clients");
	int listensocket = ksservtcp_tcpconnection_listensocket_get(this);
	int receivesocket;
	char clientname[256];
	int cnr = 0;
	int on = 1;
	int optval = 1; //used by setsockopt for reuseage of tcp connection port
	struct sockaddr_in client_addr;
	int port = ksservtcp_tcpconnection_port_get(this);

	//check if managercom-object exists
	if (!pmc) {
		ksserv_logfile_error("ERROR: no managercom-object");
		return;
	}

	//check if we have already an open socket - otherwise create socket
	if (listensocket == -1) {
		//open socket
		listensocket = socket(PF_INET, SOCK_STREAM, 0);
		ksservtcp_tcpconnection_listensocket_set(this, listensocket);
		if (listensocket == -1) {
			perror("socket(tcpconnection) failed");
			this->v_status = STATUS_TCPCON_SOCKOPENFAILED;
			return;
		}

		//non-blocking
		if ((IOCTL_SOCKET(listensocket, FIONBIO, (char*) &on)) == -1) {
			perror("ioctl(tcpconnection) failed");
			CLOSE_SOCKET(listensocket);
			ksservtcp_tcpconnection_listensocket_set(this, -1);
			this->v_status = STATUS_TCPCON_SOCKNONBLOCKFAILED;
			return;
		}

		//bind
		memset(&client_addr, 0, sizeof(client_addr));
		client_addr.sin_family = AF_INET;
		client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		client_addr.sin_port = htons(port);

		//test resueage!
		ksserv_logfile_info("########## reuseage of used port by setsockopt ");
		setsockopt(listensocket, SOL_SOCKET, SO_REUSEADDR, &optval,
				sizeof optval);

		if ((bind(listensocket, (struct sockaddr*) &client_addr,
				sizeof(client_addr))) == -1) {
			perror("bind(tcpconnection) failed");
			CLOSE_SOCKET(listensocket);
			ksservtcp_tcpconnection_listensocket_set(this, -1);
			this->v_status = STATUS_TCPCON_SOCKBINDFAILED;
			return;
		}
		//listen
		if ((listen(listensocket, 3)) == -1) {
			perror("listen(tcpconnection) failed");
			CLOSE_SOCKET(listensocket);
			ksservtcp_tcpconnection_listensocket_set(this, -1);
			this->v_status = STATUS_TCPCON_SOCKLISTENFAILED;
			return;
		}


	} //end create socket

	listensocket = ksservtcp_tcpconnection_listensocket_get(this);

	//accept
	if ((receivesocket = accept(listensocket, NULL, NULL)) < 0) {
		//ksserv_logfile_info("No client waiting to connect (accept(tcpconnection) < 0)");
		this->v_status = STATUS_TCPCON_SOCKACCEPTFAILED;
		return;
	}

	//non-blocking
	if ((IOCTL_SOCKET(receivesocket, FIONBIO, (char*) &on)) == -1) {
		perror("ioctl(receivesocket) failed");
		CLOSE_SOCKET(receivesocket);
		this->v_status = STATUS_TCPCON_SOCKNONBLOCKCLIENTFAILED;
		return;
	}

	//KSDEVEL ksserv_logfile_info("clientname: %s", clientname);
	//KSDEVEL ksserv_logfile_info("socket: %d", receivesocket);

	//get first free "tcpclient"-name
	do {
		ptcpc = NULL;
		cnr++;
		sprintf(clientname, "tcpclient%i", cnr);
		ptcpc
				= (OV_INSTPTR_ksservtcp_tcpclient) Ov_SearchChild(ov_containment, pclients, clientname);
	} while (ptcpc);

	//create receiving tcpclient
	if (Ov_OK(Ov_CreateObject(ksservtcp_tcpclient, ptcpc, pclients, clientname))) {
		ksserv_logfile_info("New client connected, socket %d", receivesocket);
		//copy socket to created tcpclient-object
		ksservtcp_tcpclient_receivesocket_set(ptcpc, receivesocket);
		this->v_status = STATUS_TCPCON_OK;
	} else {
		ksserv_logfile_error("Creating of TCPClient failed while New client connected, socket %d ", receivesocket);
	}
	return;
}

/**
 * Procedure which creates manager-objects.
 */
OV_RESULT ksservtcp_tcpconnection_createmanager() {
	OV_INSTPTR_ov_domain pksserv =
			(OV_INSTPTR_ov_domain) ov_path_getobjectpointer(
					"/communication/ksservtcp", 2);
	OV_INSTPTR_ov_domain proot =
			(OV_INSTPTR_ov_domain) ov_path_getobjectpointer("/", 2);
	OV_INSTPTR_ov_domain
			pmanager =
					(OV_INSTPTR_ov_domain) Ov_SearchChild(ov_containment, pksserv, "manager");
	OV_INSTPTR_ov_domain
			pserverlist =
					(OV_INSTPTR_ov_domain) Ov_SearchChild(ov_containment, proot, "servers");
	OV_INSTPTR_ksservtcp_register pmr = NULL;
	OV_INSTPTR_ksservtcp_unregister pmur = NULL;
	OV_INSTPTR_ksservtcp_getserverdata pmgsd = NULL;
	OV_INSTPTR_ksservtcp_udpconnection pmudp = NULL;

	ksserv_logfile_info("Server is MANAGER - creating Manager components");
	//check if pointer to "/" and "ksservtcp" exist
	if ((!pksserv) || (!proot)) {
		ksserv_logfile_error(
				"Fatal: Coudnt find Domain '%s' or '%s'! - Fatal Error", "/",
				"/communication/ksservtcp");
		return OV_ERR_GENERIC;
	}

	if (pmanager) {//manager domain already exists --> delete existing objects

		pmr
				= (OV_INSTPTR_ksservtcp_register) Ov_SearchChild(ov_containment, pmanager, "register");
		if (pmr)
			Ov_DeleteObject(pmr);
		pmur
				= (OV_INSTPTR_ksservtcp_unregister) Ov_SearchChild(ov_containment, pmanager, "unregister");
		if (pmur)
			Ov_DeleteObject(pmur);
		pmgsd
				= (OV_INSTPTR_ksservtcp_getserverdata) Ov_SearchChild(ov_containment, pmanager, "getserverdata");
		if (pmgsd)
			Ov_DeleteObject(pmgsd);
		pmudp
				= (OV_INSTPTR_ksservtcp_udpconnection) Ov_SearchChild(ov_containment, pmanager, "udpconnection");
		if (pmudp)
			Ov_DeleteObject(pmudp);
		Ov_DeleteObject(pmanager);
	}
	if (pserverlist)
		Ov_DeleteObject(pserverlist);

	//create manager domain
	if (!Ov_OK((Ov_CreateObject(ov_domain, pmanager, pksserv, "manager")))) {
		ksserv_logfile_error("Fatal: Couldnt create Object '%s'", "manager");
		return OV_ERR_GENERIC;
	}

	//create servers domain --> here all registered server will be stored in
	if (!Ov_OK((Ov_CreateObject(ov_domain, pserverlist, proot, "servers")))) {
		ksserv_logfile_error("Fatal: Couldnt create Object '%s'", "serverlist");
		return OV_ERR_GENERIC;
	}

	//create register object
	if (!Ov_OK((Ov_CreateObject(ksservtcp_register, pmr, pmanager, "register")))) {
		ksserv_logfile_error("Fatal: Couldnt create Object '%s'", "register");
		return OV_ERR_GENERIC;
	}

	//create unregister object
	if (!Ov_OK((Ov_CreateObject(ksservtcp_unregister, pmur, pmanager, "unregister")))) {
		ksserv_logfile_error("Fatal: Couldnt create Object '%s'", "unregister");
		return OV_ERR_GENERIC;
	}

	//create getserverdata object
	if (!Ov_OK((Ov_CreateObject(ksservtcp_getserverdata, pmgsd, pmanager, "getserverdata")))) {
		ksserv_logfile_error("Fatal: Couldnt create Object '%s'",
				"getserverdata");
		return OV_ERR_GENERIC;
	}

	//create udpconnection object
	if (!Ov_OK((Ov_CreateObject(ksservtcp_udpconnection, pmudp, pmanager, "udpconnection")))) {
		ksserv_logfile_error("Fatal: Couldnt create Object '%s'", "unregister");
		return OV_ERR_GENERIC;
	}

	return OV_ERR_OK;
}
