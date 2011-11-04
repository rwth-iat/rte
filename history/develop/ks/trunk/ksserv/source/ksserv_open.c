/** @file 
* Startup procedure on library loading 
*/

#ifndef OV_COMPILE_LIBRARY_ksserv
#define OV_COMPILE_LIBRARY_ksserv
#endif


#include "ksserv.h"
#include "libov/ov_macros.h"
#include "libov/ov_result.h"
#include "libov/ov_path.h"
#include "ksserv_logfile.h"

#ifdef ov_library_open_ksserv
#undef ov_library_open_ksserv
#endif

/**
* This function will be called, after the library is loaded.
*/
OV_RESULT ov_library_setglobalvars_ksserv_new(void) {
	/*
	*       local variables
	*/
	OV_RESULT   		    		result    = OV_ERR_OK;

	OV_INSTPTR_ksserv_RootComTask		rcTask  = NULL;
	OV_INSTPTR_ov_domain        		domain = NULL;
	OV_INSTPTR_ov_object        		pobj = NULL;
	OV_INSTPTR_ksserv_comcontainer				pcommunication  = NULL;
	/*
	*       set the global variables and if successful go on
	*/
	result = ov_library_setglobalvars_ksserv();

	if(Ov_OK(result)) {
		// get "/" pointer
		domain = (OV_INSTPTR_ov_domain)ov_path_getobjectpointer("/", 2);
		if(!domain) {
			ksserv_logfile_error("Fatal: Could not find Domain '/'! - Fatal Error");
			return OV_ERR_GENERIC;
		}
		// create ComTasks object (instance of RootComTask)
		pobj = Ov_SearchChild(ov_containment, domain, "ComTasks");
		if(!pobj) {
	    		result = Ov_CreateObject(ksserv_RootComTask, rcTask, domain, "ComTasks");
	    		if(Ov_Fail(result)) {
				ksserv_logfile_error("Fatal: Couldnt create Object 'ComTasks'");
	    			return result;
	    		}
		}
		// create "communication" container for collecting all types of communication principles
		pcommunication = (OV_INSTPTR_ksserv_comcontainer)Ov_SearchChild(ov_containment, domain, "communication");
		if(!pcommunication) {
	    		result = Ov_CreateObject(ksserv_comcontainer, pcommunication, domain, "communication");
	    		if(Ov_Fail(result)) {
				ksserv_logfile_error("Fatal: Couldnt create Object 'communication':%s", ov_result_getresulttext(result));
	    			return result;
	    		}
		}


	}
	return result;
}


/**
* Replace the "setglobalvars" function of a library with this
* previous one, which additionally creates instances.
* this is called by the OV system upon library load.
*/
OV_DLLFNCEXPORT OV_LIBRARY_DEF *ov_library_open_ksserv(void) {
	/*
	*       local variables
	*/
	static OV_LIBRARY_DEF OV_LIBRARY_DEF_ksserv_new;
	/*
	*       replace the "setglobalvars" function created by the code generator
	*       with a new one.
	*/
	OV_LIBRARY_DEF_ksserv_new = *ov_library_open_ksserv_old();
	OV_LIBRARY_DEF_ksserv_new.setglobalvarsfnc = ov_library_setglobalvars_ksserv_new;
	return &OV_LIBRARY_DEF_ksserv_new;
}
