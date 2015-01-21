
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
#include "libov/ov_result.h"
#include "TCPbind_config.h"

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
 * If there is no portnumber set in either way, the library open function will check if the servername equals "MANAGER".
 * 		If no, a random port is chosen. If yes, 7509 will be set as port.
 */

OV_RESULT ov_library_setglobalvars_TCPbind_new(void) {
	OV_RESULT result;
	OV_INT port = -1;
	OV_STRING OptValTemp = NULL;
	OV_INSTPTR_TCPbind_TCPListener pListener = NULL;
	OV_INSTPTR_ov_domain pcommunication = NULL;
	OV_INSTPTR_ov_domain pTCPbindDom = NULL;
	OV_ANY tempAny = {{0,{0}}, 0, {0,0}};


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
			KS_logfile_error(("TCPbind library open: Fatal: initializing Winsock failed"));
			return OV_ERR_GENERIC;
		}
	}
#endif

	pcommunication = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, &(pdb->root), "communication"));
	if(!pcommunication)
	{
		KS_logfile_error(("TCPbind library open: Fatal: communication domain not found"));
		return OV_ERR_BADFACTORY;
	}
	else if(!Ov_CanCastTo(ov_domain, (OV_INSTPTR_ov_object) pcommunication))
	{
		KS_logfile_error(("TCPbind library open: Fatal: communication object found but not domain (or derived)"));
		return OV_ERR_BADFACTORY;
	}

	pTCPbindDom = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, pcommunication, "TCPbind"));
	if(!pTCPbindDom)
	{
		result = Ov_CreateObject(ov_domain, pTCPbindDom, pcommunication, "TCPbind");
		if(Ov_Fail(result))
		{
			KS_logfile_error(("TCPbind library open: Fatal: could not create TCPbind domain"));
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

		if(port <0)	//no port number set --> check if we are MANAGER. If so, set port to 7509.
		{
			result = ov_vendortree_getservername(&tempAny, NULL);
				if(Ov_Fail(result))
				{
					KS_logfile_error(("TCPbind library open: could not get servername: %s", ov_result_getresulttext(result)));
					ov_memstack_unlock();
					return result;
				}

				if(ov_string_compare(tempAny.value.valueunion.val_string, "MANAGER") == OV_STRCMP_EQUAL)
					port = 7509;
				else
					KS_logfile_info(("TCPbind library open: No port set and not manager. Using random port."));
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
			KS_logfile_error(("TCPbind library open: could not create TCPListener"));
			ov_memstack_unlock();
			return result;
		}

		//configure port and set active
		TCPbind_TCPListener_port_set(pListener, port);
		pListener->v_actimode = 1;
		pListener->v_ChannelNeedsClientHandler = TRUE;
		//run typemethod once to create socket
		TCPbind_TCPListener_typemethod(Ov_StaticPtrCast(ksbase_ComTask, pListener));
		if(pListener->v_SocketState == TCPbind_CONNSTATE_COULDNOTOPEN)
		{
			KS_logfile_error(("TCPbind library open: Listener could not open socket"));
			ov_memstack_unlock();
			return result;
		}
		//read out port and print it
		KS_logfile_info(("TCPbind library open: listening on port %d", pListener->v_port));

	}
	ov_memstack_unlock();

	KS_logfile_debug(("leaving TCPbind_open"));
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
