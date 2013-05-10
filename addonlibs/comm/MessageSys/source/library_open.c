
/******************************************************************************
 *
 *   FILE
 *   ----
 *   library_open.c
 *
 *   Author: C. Hansen, D. Kampert
 *
 *   History
 *   -------
 *   2012-10-19   File created
 *
 *******************************************************************************
 *
 *   This file is generated by the 'acplt_builder' command
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_MessageSys
#define OV_COMPILE_LIBRARY_MessageSys
#endif

#include "MessageSys.h"
#include "MessageSys_helpers.h"
#include "libov/ov_macros.h"
#include "libov/ov_logfile.h"
#include "libov/ov_path.h"
#include "libov/ov_time.h"
#include "libov/ov_result.h"
#include "libov/ov_library.h"
#include "ksapitcp.h"

#ifdef ov_library_open_MessageSys
#undef ov_library_open_MessageSys
#endif

/*
 * This function will be called, when the library is loaded.
 * It could generate components and initializes the startup procedure
 */
OV_RESULT ov_library_setglobalvars_MessageSys_new(void) {
	OV_RESULT result;
	OV_INSTPTR_ov_domain domain = NULL;
	OV_INSTPTR_ksapitcp_TCPChannel			tcpChannel = NULL;
	OV_INSTPTR_MessageSys_MsgDelivery				MsgSysDelivery  = NULL;
	/*
	 *    set the global variables of the original version
	 *    and if successful, load other libraries
	 *    and create some objects
	 */

	result = ov_library_setglobalvars_MessageSys();

	ov_memstack_lock();

	//MsgDelivery
	domain = (OV_INSTPTR_ov_domain)ov_path_getobjectpointer(COMPATH, 2);

	result = Ov_CreateObject(MessageSys_MsgDelivery, MsgSysDelivery, domain, "MessageSys");
	if(Ov_Fail(result) && (result != OV_ERR_ALREADYEXISTS)){
		ov_memstack_lock();
		ov_logfile_error("messageSys: Fatal: Couldn't create Object 'MessageSys' Reason: %s", ov_result_getresulttext(result));
		ov_memstack_unlock();
		return result;
	}
	if(Ov_OK(result))
		MsgSysDelivery->v_cycInterval = 1;

	//channel
	domain = (OV_INSTPTR_ov_domain)ov_path_getobjectpointer(SENDINGINSTANCE, 2);
	result = Ov_CreateObject(ksapitcp_TCPChannel, tcpChannel, domain, "channel");
	if(Ov_Fail(result) && (result != OV_ERR_ALREADYEXISTS)){
		ov_memstack_lock();
		ov_logfile_error("messageSys: Fatal: Couldn't create Object 'channel' Reason: %s", ov_result_getresulttext(result));
		ov_memstack_unlock();
		return result;
	}

	return OV_ERR_OK;
}
/*
 *       Replace the 'setglobalvars' function of a library with this
 *       previous one, which additionally creates instances.
 * 	This is called by the OV system upon library load.
 */
OV_DLLFNCEXPORT OV_LIBRARY_DEF *ov_library_open_MessageSys(void) {
	/* local variables */
	static OV_LIBRARY_DEF *OV_LIBRARY_DEF_MessageSys_new;


	/*
	 *       replace the 'setglobalvars' function created by the code generator
	 *       with a new one.
	 */
	OV_LIBRARY_DEF_MessageSys_new = ov_library_open_MessageSys_old();
	OV_LIBRARY_DEF_MessageSys_new->setglobalvarsfnc = ov_library_setglobalvars_MessageSys_new;
	return OV_LIBRARY_DEF_MessageSys_new;
}
