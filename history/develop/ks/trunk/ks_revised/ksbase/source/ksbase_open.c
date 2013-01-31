
/******************************************************************************
*
*   FILE
*   ----
*   library_open.c
*
*   History
*   -------
*   2013-01-15   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_ksbase
#define OV_COMPILE_LIBRARY_ksbase
#endif

#include "ksbase.h"
#include "libov/ov_macros.h"
#include "libov/ov_result.h"
#include "libov/ov_logfile.h"
#include "libov/ov_memstack.h"

#ifdef ov_library_open_ksbase
#undef ov_library_open_ksbase
#endif

/*
* This function will be called, when the library is loaded.
* It could generate components and initializes the startup procedure
*/
OV_RESULT ov_library_setglobalvars_ksbase_new(void) {
	OV_RESULT result;
	OV_INSTPTR_ov_domain pcommunication = NULL;
	OV_INSTPTR_ksbase_RootComTask rcTask = NULL;
	OV_INSTPTR_ksbase_ServerRep	pThisServer = NULL;
	/*
	 *    set the global variables of the original version
	 *    and if successful, load other libraries
	 *    and create some objects
	 */
	result = ov_library_setglobalvars_ksbase();
	if(Ov_Fail(result))
		return result;

	// create "communication" container for collecting all types of communication objects
	pcommunication = (OV_INSTPTR_ov_domain)Ov_SearchChild(ov_containment, &(pdb->root), "communication");
	if(!pcommunication) {
		result = Ov_CreateObject(ov_domain, pcommunication, &(pdb->root), "communication");
		if(Ov_Fail(result)) {
			ov_memstack_lock();
			ov_logfile_error("Fatal: Could not create Object 'communication': %s", ov_result_getresulttext(result));
			ov_memstack_unlock();
			return result;
		}
	}
	else if(!Ov_CanCastTo(ov_domain, (OV_INSTPTR_ov_object) pcommunication))
	{
		ov_logfile_error("Fatal: communication object found but not domain (or derived)");
		return OV_ERR_GENERIC;
	}


	// create RootComTask
	rcTask = (OV_INSTPTR_ksbase_RootComTask) Ov_GetFirstChild(ov_instantiation, pclass_ksbase_RootComTask);
	if(!rcTask) {
		result = Ov_CreateObject(ksbase_RootComTask, rcTask, pcommunication, "RootComTask");
		if(Ov_Fail(result)) {
			ov_memstack_lock();
			ov_logfile_error("Fatal: Could not create RootComTask: %s", ov_result_getresulttext(result));
			ov_memstack_unlock();
			return result;
		}
	}

	// create thisServer representative
	pThisServer = (OV_INSTPTR_ksbase_ServerRep)Ov_SearchChild(ov_containment, pcommunication, "thisServer");
	if(pThisServer)
		Ov_DeleteObject(pThisServer);

	result = Ov_CreateObject(ksbase_ServerRep, pThisServer, pcommunication, "thisServer");
	if(Ov_Fail(result)) {
		ov_memstack_lock();
		ov_logfile_error("Fatal: Could not create Object 'thisServer': %s", ov_result_getresulttext(result));
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
OV_DLLFNCEXPORT OV_LIBRARY_DEF *ov_library_open_ksbase(void) {
	/* local variables */
	static OV_LIBRARY_DEF *OV_LIBRARY_DEF_ksbase_new;
	/*
	*       replace the 'setglobalvars' function created by the code generator
	*       with a new one.
	*/
	OV_LIBRARY_DEF_ksbase_new = ov_library_open_ksbase_old();
	OV_LIBRARY_DEF_ksbase_new->setglobalvarsfnc = ov_library_setglobalvars_ksbase_new;
	return OV_LIBRARY_DEF_ksbase_new;
}
