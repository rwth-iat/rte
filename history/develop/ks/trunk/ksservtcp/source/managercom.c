/** @file 
 * Managercom provides functionallity to communicate with the ks_manager.
 *
 * This class uses the class managersendrecv to
 * submit/receive the generated xdr-files
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
OV_DLLFNCEXPORT OV_UINT ksservtcp_managercom_mngport_get(
		OV_INSTPTR_ksservtcp_managercom pobj) {
	return pobj->v_mngport;
}

/**
 * Sets the port which this class uses to communicate
 * with the ks_manager
 */
OV_DLLFNCEXPORT OV_RESULT ksservtcp_managercom_mngport_set(
		OV_INSTPTR_ksservtcp_managercom pobj, const OV_UINT value) {
	OV_INSTPTR_ksservtcp_managersendrecv
			pmsr =
					(OV_INSTPTR_ksservtcp_managersendrecv) Ov_SearchChild(ov_containment, pobj, "managersendrecv");
	OV_INT sock = pobj->v_socket;
	OV_INT sockmsr = pmsr->v_socket;

	CLOSE_SOCKET(sock);
	CLOSE_SOCKET(sockmsr);
	pobj->v_socket = -1;
	pmsr->v_socket = -1;
	pobj->v_mngport = value;
	pmsr->v_mngport = value;
	return OV_ERR_OK;
}

/**
 * Returns the name with which this class registers the
 * server running on at the ks_manger
 */
OV_DLLFNCEXPORT OV_STRING ksservtcp_managercom_sname_get(
		OV_INSTPTR_ksservtcp_managercom pobj) {
	return pobj->v_sname;
}

/**
 * Sets the name with which this class registers the
 * server running on at the ks_manger.
 * 
 * This method unregisters the old servername and
 * registers the new one at the manager.
 */
OV_DLLFNCEXPORT OV_RESULT ksservtcp_managercom_sname_set(
		OV_INSTPTR_ksservtcp_managercom pobj, const OV_STRING value) {
	ksservtcp_managercom_mngunregister(pobj, pobj->v_sname, 2);
	ksservtcp_managercom_mngregister(pobj, value, 2, pobj->v_sport,
			pobj->v_sttl);

	return ov_string_setvalue(&pobj->v_sname, value);
}

/**
 * Returns the timetolife which will submitted to ks_manager.
 */
OV_DLLFNCEXPORT OV_UINT ksservtcp_managercom_sttl_get(
		OV_INSTPTR_ksservtcp_managercom pobj) {
	return pobj->v_sttl;
}

/**
 * Sets the timetolife which will submitted to ks_manager.
 * 
 * Timetolife is the time after which the ks_manager will
 * set the server to inactive state if no acknowledgement
 * is sent by the server.
 */
OV_DLLFNCEXPORT OV_RESULT ksservtcp_managercom_sttl_set(
		OV_INSTPTR_ksservtcp_managercom pobj, const OV_UINT value) {
	pobj->v_sttl = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_BOOL ksservtcp_managercom_activatetypemethod_get(
		OV_INSTPTR_ksservtcp_managercom pobj) {
	return pobj->v_activatetypemethod;
}

/**
 * Choose whether typemethod should be active or not
 */
OV_DLLFNCEXPORT OV_RESULT ksservtcp_managercom_activatetypemethod_set(
		OV_INSTPTR_ksservtcp_managercom pobj, const OV_BOOL value) {
	OV_VAR_CURRENT_PROPS srvnameprops;
	char servername[256];

	if (value) {
		//get own Servername
		ov_vendortree_getservername(&srvnameprops, NULL);
		sprintf(servername, "%s", srvnameprops.value.valueunion.val_string);
		ov_string_setvalue(&pobj->v_sname, servername);

		ksserv_logfile_info("OWNNAME: %s", pobj->v_sname);

		pobj->v_cycInterval = 60000; //every minute send REGISTER
		pobj->v_cycIntervalCount = 0;
		pobj->v_actimode = 1;
		ksserv_logfile_info(
				"managercom_activatetypemethod_set: typemethod activated");
		//~ ksservtcp_managercom_typemethod((OV_INSTPTR_ksserv_ComTask)pobj);
	} else {
		pobj->v_actimode = 0;
		ksserv_logfile_info("typemethod deactivated");
	}

	pobj->v_activatetypemethod = value;
	return OV_ERR_OK;
}

/**
 * This method is called on startup.
 * 
 * This method creates the underlying object managersendrecv
 * which submits the xdrs to the ks_manager.
 * 
 * It also gets the port of the ks_manager from the portmapper
 * (if not set through the external variable MNGPORT).
 * 
 * If no servername is set through the external variable
 * OWNNAME it gets the name of the server running on.
 * 
 * The registration at the ks_manager is performed with this data.
 */
OV_DLLFNCEXPORT void ksservtcp_managercom_startup(OV_INSTPTR_ov_object pobj) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_ksservtcp_managercom pinst =
			Ov_StaticPtrCast(ksservtcp_managercom, pobj);
	OV_INSTPTR_ov_domain pinstdomain = Ov_StaticPtrCast(ov_domain, pobj);
	OV_INSTPTR_ksserv_ComTask ctask =
			Ov_StaticPtrCast(ksserv_ComTask, pobj);
	OV_INSTPTR_ksservtcp_managersendrecv
			pmsr =
					(OV_INSTPTR_ksservtcp_managersendrecv) Ov_SearchChild(ov_containment, pinstdomain, "managersendrecv");

	/* do what the base class does first */
	ov_object_startup(pobj);

	pinst->v_xdr = NULL;
	pinst->v_actimode = 0;

	ctask->v_cycInterval = MANAGER_REGISTER_INTERVALL;

	//create managersendrecv
	if (pmsr) {
		Ov_DeleteObject(pmsr);
	}
	if (!Ov_OK((Ov_CreateObject(ksservtcp_managersendrecv, pmsr, pinstdomain, "managersendrecv")))) {
		printf("\nFatal: Couldnt create Object managersendrecv\n");
		return;
	}

	//get managerport
	if (getenv("MNGPORT")) {
		pinst->v_mngport = atoi(getenv("MNGPORT"));
		pmsr->v_mngport = atoi(getenv("MNGPORT"));
		ksserv_logfile_info("MNGPORT: %d", pinst->v_mngport);
	} else {
		pinst->v_mngport = 7509;
		pmsr->v_mngport = 7509;
		ksserv_logfile_info("mngport: %d", pinst->v_mngport);
	}

	return;
}

/**
 * This method is called on shutdown.
 * 
 * It unregisters the server at the ks_manager, closes open socket
 * and deletes the created managersendrecv-object.
 */
OV_DLLFNCEXPORT void ksservtcp_managercom_shutdown(OV_INSTPTR_ov_object pobj) {

	OV_INSTPTR_ksservtcp_managercom pinst =			Ov_StaticPtrCast(ksservtcp_managercom, pobj);
	//OV_INSTPTR_ov_domain pinstdomain = Ov_StaticPtrCast(ov_domain, pobj);
	//OV_INSTPTR_ksservtcp_managersendrecv pmsr =	(OV_INSTPTR_ksservtcp_managersendrecv) Ov_SearchChild(ov_containment, pinstdomain, "managersendrecv");

	/* do what */
	ksserv_logfile_info("managercom/shutdown ... sending unregister and deleting oneself");

	if (pinst->v_sname) {
		ksservtcp_managercom_mngunregister(pinst, pinst->v_sname, 2);
		//CLOSE_SOCKET(pinst->v_socket);
		//pinst->v_socket = -1;
		//pinst->v_mngport = 0;
		//pinst->v_status = 1;
		//pinst->v_operatingfunction = 0;
		//if(pinst->v_xdr) {
		//  free(pinst->v_xdr);
		//}
		//pinst->v_xdr = NULL;
		//Ov_DeleteObject(pmsr);

		/* set the object's state to "shut down" */
		ov_object_shutdown(pobj);
	}

	return;
}

/**
 * This method is called frequently after the with
 * the variable CYCTIME set time.
 * 
 * It takes over the frequently acknowledge at
 * the ks_manager.
 */
void ksservtcp_managercom_typemethod(OV_INSTPTR_ksserv_ComTask cTask
//ComTask OV_INSTPTR_ov_object	pfb
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_ksservtcp_managercom pinst =
			Ov_StaticPtrCast(ksservtcp_managercom, cTask);
	OV_INSTPTR_ov_domain pinstdomain = Ov_StaticPtrCast(ov_domain, cTask);
	OV_INSTPTR_ksservtcp_managersendrecv
			pmsr =
					(OV_INSTPTR_ksservtcp_managersendrecv) Ov_SearchChild(ov_containment, pinstdomain, "managersendrecv");
	//~ printf("\n\n\nMANAGERCOM TYPEMETHOD\n\n\n");
	if (pinst->v_sport <= 0) {
		ksserv_logfile_info("ownport not set --> not registered at manager");
		return;
	}

	if (getenv("MNGPORT")) {
		pinst->v_mngport = atoi(getenv("MNGPORT"));
		pmsr->v_mngport = atoi(getenv("MNGPORT"));
		//KSDEVEL ksserv_logfile_info("MNGPORT: %d", pinst->v_mngport);
	} else {
		pinst->v_mngport = 7509;
		pmsr->v_mngport = 7509;
		//KSDEVEL ksserv_logfile_info("mngport: %d", pinst->v_mngport);
	}

	ksservtcp_managercom_mngregister(pinst, pinst->v_sname, 2, pinst->v_sport,
			pinst->v_sttl);

	return;
}

/**
 * This method gets the port of with the variables
 * NAME and VERSION declared server from the ks_manager.
 * 
 * The received port is stored in the variable
 * pobj->v_receivedsport.
 */
/* OV_DLLFNCEXPORT OV_INT ksservtcp_managercom_mnggetserver(
 OV_INSTPTR_ksservtcp_managercom	pobj,
 OV_STRING								name,
 OV_UINT									version
 ) {
 OV_INSTPTR_ksservtcp_managersendrecv pmsr = (OV_INSTPTR_ksservtcp_managersendrecv)Ov_SearchChild(ov_containment, pobj, "managersendrecv");
 OV_UINT namelength = strlen(name);
 OV_UINT xdrnamelength;
 OV_INT rcvdserverport;
 OV_INT c;
 OV_INT bytes, i;
 char *tmp;
 int sock;
 struct sockaddr_in server;
 struct hostent *hp;
 char temp[4];
 char buffer[4098];

 if(pobj->v_mngport <= 0)
 {
 ksserv_logfile_error("no manager port set (mnggetserver)");
 return -1;
 }

 //set xdrlength
 xdrnamelength = strlen(name);
 while ((xdrnamelength%4) != 0)
 xdrnamelength++;
 pobj->v_xdrlength = xdrnamelength+52;
 //~ if(pobj->v_xdr) {
 //~ free(pobj->v_xdr);
 //~ }
 pobj->v_xdr = (char*)malloc(pobj->v_xdrlength);
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
 tmp = (char*)&namelength;
 for (c=0; c<4; c++)
 pobj->v_xdr[44+c] = tmp[3-c];
 for (c=0; c<namelength; c++)
 pobj->v_xdr[48+c] = name[c];

 //set serverversion
 tmp = (char*)&version;
 for (c=0; c<4; c++)
 pobj->v_xdr[48+xdrnamelength+c] = tmp[3-c];

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

 //~ ksserv_logfile_info("sendxdr port: %d", port);

 //create socket
 if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
 {
 perror ("socket(mnggetserver) failed");
 return -2;
 }
 pobj->v_socket = sock;

 //~ memset(&server, 0, sizeof(server));
 server.sin_family = AF_INET;
 hp = gethostbyname("127.0.0.1");
 if (hp==0)
 {
 ksserv_logfile_info("Unknown host (mnggetserver)");
 CLOSE_SOCKET(sock);
 pobj->v_socket = -1;
 return -3;
 }
 memcpy((char *)&server.sin_addr,(char *)hp->h_addr,hp->h_length);
 server.sin_port = htons(pobj->v_mngport);

 //send data
 if ((connect(sock,(struct sockaddr *)&server,sizeof(server))) == -1) {
 perror("connect(ksservtcp_managercom) failed");
 CLOSE_SOCKET(sock);
 pobj->v_socket = -1;
 return -4;
 }

 bytes = send(sock,pobj->v_xdr,pobj->v_xdrlength,0);
 if (bytes < 0) {
 ksserv_logfile_info("send(mnggetserver) failed");
 CLOSE_SOCKET(sock);
 pobj->v_socket = -1;
 return -5;
 }

 //receive
 memset (buffer, 0, sizeof(buffer));
 bytes = recv(sock,buffer,4096,0);
 if (bytes < 0) {
 return -6;
 } else if (bytes == 0) {
 return -7;
 } else {
 //~ ksserv_logfile_info("recv(ksservtcp) ok");
 pobj->v_xdrlength = bytes;
 if(pobj->v_xdr) {
 free(pobj->v_xdr);
 }
 pobj->v_xdr = (char*)malloc(pobj->v_xdrlength);
 memset(pobj->v_xdr, 0, pobj->v_xdrlength);
 memcpy(pobj->v_xdr, buffer, pobj->v_xdrlength);


 CLOSE_SOCKET(sock);
 pobj->v_socket = -1;

 //print xdr
 //~ int j;
 //~ printf("\n\nxdr received from manager:\n");
 //~ for (j = 0; j < pobj->v_xdrlength; j=j+4)
 //~ printf("%X %X %X %X     ", pobj->v_xdr[j], pobj->v_xdr[j+1], pobj->v_xdr[j+2], pobj->v_xdr[j+3]);
 //~ printf("\n\n");
 //~ for (j = 0; j < pobj->v_xdrlength; j=j+4)
 //~ printf("%c%c%c%c ", pobj->v_xdr[j], pobj->v_xdr[j+1], pobj->v_xdr[j+2], pobj->v_xdr[j+3]);
 //~ printf("\n\n");

 if(pobj->v_xdr[31] == 0)
 {
 for (i = 3; i >= 0; i--)
 temp[3-i] = pobj->v_xdr[40+xdrnamelength+i];
 memcpy(&rcvdserverport, temp, 4);
 pobj->v_receivedsport = rcvdserverport;
 //~ ksserv_logfile_info("receivedsport: %d", pobj->v_receivedsport);
 }
 else
 {
 ksserv_logfile_info("mnggetserver failed");
 pobj->v_receivedsport = 0;
 }
 }

 pobj->v_operatingfunction = 0;
 pmsr->v_operatingfunction = 0;

 CLOSE_SOCKET(sock);
 pobj->v_socket = -1;

 return pobj->v_receivedsport;
 }
 */

 /**
 * This method takes over the registration at the ks_manager.
 * 
 * Transmitted variables are:
 * NAME:		name of the server to register
 * VERSION:	version of the server to register
 * PORT:		port at which the server can be reached
 * TTL:		time after which the server should acknowledge
 * 			at the ks_manager
 */
OV_DLLFNCEXPORT void ksservtcp_managercom_mngregister(
		OV_INSTPTR_ksservtcp_managercom pobj, OV_STRING name, OV_UINT version,
		OV_UINT port, OV_UINT ttl) {
	OV_INSTPTR_ksservtcp_managersendrecv
			pmsr =
					(OV_INSTPTR_ksservtcp_managersendrecv) Ov_SearchChild(ov_containment, pobj, "managersendrecv");
	OV_UINT namelength = strlen(name);
	OV_UINT xdrnamelength;
	OV_INT c;
	char *tmpfragment;
	char *tmp;
    OV_INSTPTR_ov_domain managerdomain;
    OV_INSTPTR_ksservtcp_register preg;
    
	pobj->v_operatingfunction = 1;
	pmsr->v_operatingfunction = 1;

	//If server is MANAGER on its own (check by retrieving register obj), registration
	//is done locally
	managerdomain =
			(OV_INSTPTR_ov_domain) ov_path_getobjectpointer(
					"/communication/ksservtcp/manager", 2);
	preg =
					(OV_INSTPTR_ksservtcp_register) Ov_SearchChild(ov_containment, managerdomain, "register");
	if (preg) {
		//ksserv_logfile_info(
		//		"REGISTER-OBJECT found - server is MANAGER, local distribution of REGISTER cmd");
		ksservtcp_register_name_set(preg, name);
		ksservtcp_register_version_set(preg, version);
		ksservtcp_register_port_set(preg, port);
		ksservtcp_register_timetolife_set(preg, ttl);
		ksservtcp_register_process_set(preg, 1);
		return;
	}
	ksserv_logfile_info(
			"NO REGISTER-OBJECT, thus server is NOT Manager - remote sending...");

	if (pobj->v_mngport <= 0) {
		ksserv_logfile_info("no manager port set");
		return;
	}

	//set xdrlength
	xdrnamelength = strlen(name);
	while ((xdrnamelength % 4) != 0)
		xdrnamelength++;
	pobj->v_xdrlength = xdrnamelength + 64;//martin60
	pobj->v_xdr = (char*) malloc(pobj->v_xdrlength);
	memset(pobj->v_xdr, 0, pobj->v_xdrlength);

	//add tcp-fragment-rpcheader
	tmpfragment = (char*)&(pobj->v_xdrlength);
	for (c=0; c<4; c++)
		pobj->v_xdr[3-c] = tmpfragment[c];
	pobj->v_xdr[0] = 0x80;

	//set xid (martin+=4)
	pobj->v_xdr[4] = 0x4a;
	pobj->v_xdr[5] = 0x5d;
	pobj->v_xdr[6] = 0x4f;
	pobj->v_xdr[7] = 0xe4;

	//set message type call --> 0000

	//set rpc version (martin=11)
	pobj->v_xdr[15] = 0x02;

	//set programm nr.(martin+=4)
	pobj->v_xdr[17] = 0x04;
	pobj->v_xdr[18] = 0x96;
	pobj->v_xdr[19] = 0x78;

	//set programm version martin19
	pobj->v_xdr[23] = 0x02;

	//set procedure martin 22/23
	pobj->v_xdr[26] = 0xff;
	pobj->v_xdr[27] = 0x01;

	//set servername
	tmp = (char*) &namelength;
	for (c = 0; c < 4; c++)
		pobj->v_xdr[48 + c] = tmp[3 - c];//martin 44
	for (c = 0; c < namelength; c++)
		pobj->v_xdr[52 + c] = name[c];//martin 48

	//set serverversion
	tmp = (char*) &version;
	for (c = 0; c < 4; c++)
		pobj->v_xdr[52 + xdrnamelength + c] = tmp[3 - c]; //martin48

	//set serverport
	tmp = (char*) &port;
	for (c = 0; c < 4; c++)
		pobj->v_xdr[56 + xdrnamelength + c] = tmp[3 - c];//martin52

	//set serverttl
	tmp = (char*) &ttl;
	for (c = 0; c < 4; c++)
		pobj->v_xdr[60 + xdrnamelength + c] = tmp[3 - c];//martin 56

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

	//if (pmsr->v_xdr) {
	//	free(pmsr->v_xdr);
	//}
	pmsr->v_xdr = (char*) malloc(pobj->v_xdrlength);
	memcpy(pmsr->v_xdr, pobj->v_xdr, pobj->v_xdrlength);
	free(pobj->v_xdr);
	pobj->v_xdr = 0;
	pmsr->v_xdrlength = pobj->v_xdrlength;
	pobj->v_status = STMNGRQSND;
	ksservtcp_managersendrecv_sendxdr(pmsr);
	//~ ksserv_logfile_info("registered at manager");
	return;
}

/**
 * This method takes over the unregistration at the ks_manager.
 * 
 * It unregisters the with variables NAME and VERSION
 * declared server at the ks_manager.
 */
OV_DLLFNCEXPORT void ksservtcp_managercom_mngunregister(
		OV_INSTPTR_ksservtcp_managercom pobj, OV_STRING name, OV_UINT version) {
	OV_INSTPTR_ksservtcp_managersendrecv
			pmsr =
					(OV_INSTPTR_ksservtcp_managersendrecv) Ov_SearchChild(ov_containment, pobj, "managersendrecv");
	OV_UINT namelength = strlen(name);
	OV_UINT xdrnamelength;
	OV_INSTPTR_ov_domain managerdomain;
	OV_INSTPTR_ksservtcp_unregister	pureg;
	OV_INT c;
	char *tmp;
	char *tmpfragment;

	pobj->v_operatingfunction = 2;
	pmsr->v_operatingfunction = 2;

	//If server is MANAGER on its own (check by retrieving register obj), registration
	//is done locally
	//this doesnt really make sense to unregister by oneself...
	managerdomain =	(OV_INSTPTR_ov_domain) ov_path_getobjectpointer("/communication/ksservtcp/manager", 2);
	pureg =	(OV_INSTPTR_ksservtcp_unregister) Ov_SearchChild(ov_containment, managerdomain, "unregister");
	if (pureg) {
		//ksserv_logfile_info("UNREGISTER-OBJECT found - server is MANAGER, local distribution of UNREGISTER cmd");
		ksservtcp_unregister_name_set(pureg, name);
		ksservtcp_unregister_version_set(pureg, version);
		ksservtcp_unregister_process_set(pureg, 1);
		return;
	}
	ksserv_logfile_info(
			"NO UNREGISTER-OBJECT, thus server is NOT Manager - remote sending...");

	if (pobj->v_mngport <= 0) {
		ksserv_logfile_info("no manager port set");
		return;
	}

	//set xdrlength
	xdrnamelength = strlen(name);
	while ((xdrnamelength % 4) != 0)
		xdrnamelength++;
	pobj->v_xdrlength = xdrnamelength + 56;
	//~ if(pobj->v_xdr) {
	//~ free(pobj->v_xdr);
	//~ }
	pobj->v_xdr = (char*) malloc(pobj->v_xdrlength);
	memset(pobj->v_xdr, 0, pobj->v_xdrlength);

	//add tcp-fragment-rpcheader
	tmpfragment = (char*)&(pobj->v_xdrlength);
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
	pobj->v_xdr[27] = 0x02;

	//set servername
	tmp = (char*) &namelength;
	for (c = 0; c < 4; c++)
		pobj->v_xdr[48 + c] = tmp[3 - c];
	for (c = 0; c < namelength; c++)
		pobj->v_xdr[52 + c] = name[c];

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

	//~ ksservtcp_managercom_sendxdr(pobj);

	//~ if(pmsr->v_xdr) {
	//~ free(pmsr->v_xdr);
	//~ }
	pmsr->v_xdr = (char*) malloc(pobj->v_xdrlength);
	memcpy(pmsr->v_xdr, pobj->v_xdr, pobj->v_xdrlength);
	//ksserv_logfile_info("pobj->v_xdr: %p", pobj->v_xdr);
	free(pobj->v_xdr);
	pobj->v_xdr = 0;
	pmsr->v_xdrlength = pobj->v_xdrlength;
	ksservtcp_managersendrecv_sendxdr(pmsr);
	//ksserv_logfile_info("pmsr->v_xdr: %p", pmsr->v_xdr);
	free(pobj->v_xdr);
	pobj->v_xdr = 0;
	ksserv_logfile_info("managercom/mngunregister: prepared UNREGISTER to manager");

	return;
}

