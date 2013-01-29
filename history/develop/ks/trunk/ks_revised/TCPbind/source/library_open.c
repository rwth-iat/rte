
/******************************************************************************
 *
 *   FILE
 *   ----
 *   library_open.c
 *
 *   History
 *   -------
 *   2013-01-18   File created
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
#include "ks_logfile.h"
#include "libov/ov_logfile.h"
#include "libov/ov_vendortree.h"
#include "libov/ov_memstack.h"
#include "libov/ov_macros.h"


#ifdef ov_library_open_TCPbind
#undef ov_library_open_TCPbind
#endif

/*
 * This function will be called, when the library is loaded.
 * It could generate components and initializes the startup procedure
 */
/**
 * During Library open it is determined whether there should be a TCPListener i.e. whether this runtimeserver should listen on a TCP-port.
 * A Listener will be created unless the option "KS_NOSERVER" is set. The portnumber to listen on is determined by the values set either in
 * 1. the "PORT" option of the runtimeserver
 * 2. the environment variable "OWNPORT" (for compatibility with "old" ks2nd)
 * 3. the options "TCPBIND_LISTENPORT" or "KS_PORT" The value of TCPBIND_LISTENPORT will override KS_PORT.
 * If the options are set without value a random port is chosen.
 * If the port is 7509 (registered at IANA for acplt-ks) a manager will be created.
 * If there is no portnumber set in either way, the library open function will check if it can connect to localhost:7509.
 * 		If yes, a random port is chosen. If no, 7509 will be set as port.
 */

OV_RESULT ov_library_setglobalvars_TCPbind_new(void) {
	OV_RESULT result;
	OV_INT port = -1;
	OV_STRING OptValTemp = NULL;
	OV_INSTPTR_TCPbind_TCPChannel ptestchannel = NULL;
	OV_INSTPTR_TCPbind_TCPListener pListener = NULL;
	OV_INT testsocket = 0;
	OV_INSTPTR_ov_domain pcommunication = NULL;
	OV_INSTPTR_ov_domain pTCPbindDom = NULL;

	OV_INSTPTR_ksbase_Manager pManager = NULL;

	/*
	 * set the global variables of the original version
	 * and if successful, load other libraries
	 * and create some objects
	 */
	result = ov_library_setglobalvars_TCPbind();
#if OV_SYSTEM_NT
	{
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData)){
			ov_logfile_error(("TCPbind library open: Fatal: initializing Winsock failed"));
			return OV_ERR_GENERIC;
		}
	}
#endif

	pcommunication = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, &(pdb->root), "communication"));
	if(!pcommunication)
	{
		ov_logfile_error("TCPbind library open: Fatal: communication domain not found");
		return OV_ERR_BADFACTORY;
	}
	else if(!Ov_CanCastTo(ov_domain, (OV_INSTPTR_ov_object) pcommunication))
	{
		ov_logfile_error("TCPbind library open: Fatal: communication object found but not domain (or derived)");
		return OV_ERR_BADFACTORY;
	}

	pTCPbindDom = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, pcommunication, "TCPbind"));
	if(!pTCPbindDom)
	{
		result = Ov_CreateObject(ov_domain, pTCPbindDom, pcommunication, "TCPbind");
		if(Ov_Fail(result))
		{
			ov_logfile_error("TCPbind library open: Fatal: could not create TCPbind domain");
			return result;
		}
	}
	ov_memstack_lock();	//needed for ov_vendortree_getcmdlineoption_value

	if(!ov_vendortree_getcmdlineoption_value("KS_NOSERVER"))	//check for "KS_NOSERVER"
	{
		/*
		 * Determine port
		 */
		port = ov_vendortree_getport();
		if(port < 0 )// PORT not set
		{
			//check environment Variable OWNPORT (for compatibility reasons)
			if (getenv("OWNPORT")) //OWNPORT set
				port = atoi(getenv("OWNPORT"));
			else
			{	//check option TCPBIND_LISTENPORT
				OptValTemp = ov_vendortree_getcmdlineoption_value("TCPBIND_LISTENPORT");
				if(OptValTemp && *OptValTemp)
					port = atoi(OptValTemp);
				else
				{
					//check option KS_PORT
					OptValTemp = ov_vendortree_getcmdlineoption_value("KS_PORT");
					if(OptValTemp && *OptValTemp)
						port = atoi(OptValTemp);
				}
			}
		}

		if(port <0)	//no port number set --> check if we can connect to 7509
		{
			KS_logfile_debug(("TCPbind library open: no port specified. trying to connect to localhost:7509 to check for a manager"));
			if(Ov_OK(Ov_CreateObject(TCPbind_TCPChannel, ptestchannel, pTCPbindDom, "porttest")))
			{
				if(Ov_Fail(TCPbind_TCPChannel_OpenConnection(ptestchannel, "localhost", "7509", &testsocket)))
				{
					port = 7509;
					TCPbind_TCPChannel_CloseConnection(ptestchannel);
					KS_logfile_debug(("TCPbind library open: could not connect to 7509. Setting port to 7509."));
				}
				Ov_DeleteObject(ptestchannel);
			}
			else
				ov_logfile_info("TCPbind library open: No port set and not manager. Using random port.");
		}

		/*
		 * Create Listener
		 */

		pListener = Ov_StaticPtrCast(TCPbind_TCPListener, Ov_SearchChild(ov_containment, pTCPbindDom, "TCPListener"));
		if(pListener)
			Ov_DeleteObject(pListener);

		result = Ov_CreateObject(TCPbind_TCPListener, pListener, pTCPbindDom, "TCPListener");
		if(Ov_Fail(result))
		{
			ov_logfile_error("TCPbind library open: could not create TCPListener");
			ov_memstack_unlock();
			return result;
		}

		//configure port and set active
		TCPbind_TCPListener_port_set(pListener, port);
		pListener->v_actimode = 1;
		pListener->v_ChannelNeedsClientHandler = TRUE;
		//run typemethod once to create socket
		TCPbind_TCPListener_typemethod(Ov_StaticPtrCast(ksbase_ComTask, pListener));
		//read out port and print it
		ov_logfile_info("TCPbind library open: listening on port %d", pListener->v_port);

		/*
		 * If port is 7509 create a Manager
		 */
		if(port == 7509)
		{
			result = Ov_CreateObject(ksbase_Manager, pManager, &(pdb->root), "servers");
			if(Ov_Fail(result))
			{
				ov_logfile_error("TCPbind library open: could not create Manager");
				ov_memstack_unlock();
				return result;
			}
		}
	}
	ov_memstack_unlock();


	return result;
}
/*
 *  Replace the 'setglobalvars' function of a library with this
 *  previous one, which additionally creates instances.
 * 	This is called by the OV system upon library load.
 */
OV_DLLFNCEXPORT OV_LIBRARY_DEF *ov_library_open_TCPbind(void) {
	/* local variables */
	static OV_LIBRARY_DEF *OV_LIBRARY_DEF_TCPbind_new;
	/*
	 * replace the 'setglobalvars' function created by the code generator
	 * with a new one.
	 */
	OV_LIBRARY_DEF_TCPbind_new = ov_library_open_TCPbind_old();
	OV_LIBRARY_DEF_TCPbind_new->setglobalvarsfnc = ov_library_setglobalvars_TCPbind_new;
	return OV_LIBRARY_DEF_TCPbind_new;
}
