#ifndef OV_COMPILE_LIBRARY_fbcomlib
#define OV_COMPILE_LIBRARY_fbcomlib
#endif

#include "fbcomlib.h"
#include "msgconfig.h"

#ifdef ov_library_open_fbcomlib
#undef ov_library_open_fbcomlib
#endif

/*
*       This function will be called, after the library is loaded.
*       ----------------------------------------------------------
*/
OV_RESULT ov_library_setglobalvars_fbcomlib_new(void) {
	/*
	*       local variables
	*/
	OV_RESULT								result    = OV_ERR_OK;

	OV_INSTPTR_fbcomlib_MsgComponent			component  = NULL;
	OV_INSTPTR_fbcomlib_ServiceRegistry		services  = NULL;

	OV_INSTPTR_ov_domain 					domain = NULL;
	OV_INSTPTR_fb_task						urtaskdom = NULL;

	/*
	*       set the global variables and if successfull go on
	*/
	result = ov_library_setglobalvars_fbcomlib();

	if(Ov_OK(result)){
	//////////////////////////////////////////////////////////
	//get FB references for the domain ApplicationContainer
	   
		domain = (OV_INSTPTR_ov_domain)ov_path_getobjectpointer(TECHROOT, 2);
		if(!domain){
			ov_logfile_error("Fatal: Coudnt find Application-Domain '/TechUnits/'! - Fatal Error");
			return OV_ERR_GENERIC;
		}
		urtaskdom = (OV_INSTPTR_fb_task)ov_path_getobjectpointer("/Tasks/UrTask", 2);
		if(!urtaskdom){
			ov_logfile_error("Fatal: Couldnt find '/Task/UrTask' - Fatal Error!");
			return OV_ERR_GENERIC;
		}
	  
		//////////////////////////////////////////////
		//create components

		ov_logfile_debug("Now creating MsgSys Objects/Components!");

		//MsgComponent
		ov_logfile_debug("Creating Object 'MsgSys'");
		if(!Ov_OK((Ov_CreateObject(fbcomlib_MsgComponent, component, domain, MSGCOMPONENTCONTAINER)))){
			ov_logfile_error("Fatal: Couldnt create Object 'MsgSys'");
			return OV_ERR_OK;
		} 
		else{
			Ov_Link(fb_tasklist, urtaskdom, component);
			ov_logfile_debug("Done.");
		}

		domain = (OV_INSTPTR_ov_domain)ov_path_getobjectpointer(MSGCOMPONENTROOT, 2);
		if(!domain){
			ov_logfile_error("Fatal: Coudnt find Application-Domain '/TechUnits/MsgSys/'! - Fatal Error");
			return OV_ERR_GENERIC;
		}
		ov_logfile_debug("Creating Object 'ServiceRegistry'");
		if(!Ov_OK((Ov_CreateObject(fbcomlib_ServiceRegistry, services, domain, "Services")))){
			ov_logfile_error("Fatal: Couldnt create Object 'ServiceRegistry'");
			return OV_ERR_OK;
		} else{
			//no fb functionblock any longer! Ov_Link(fb_tasklist, urtaskdom, services);
			ov_logfile_debug("Done.");
		}
			ov_logfile_error("MsgSys Objects loaded!!");
	}	//endif setglobalvalues
	return result;
}



/*
*       Replace the "setglobalvars" function of a library with this
*       previous one, which additionally creates instances.
*       ----------------------------------------------------------------------
*/
OV_DLLFNCEXPORT OV_LIBRARY_DEF *ov_library_open_fbcomlib(void) {
	/*
	*       local variables
	*/
	static OV_LIBRARY_DEF OV_LIBRARY_DEF_fbcomlib_new;
	/*
	*       replace the "setglobalvars" function created by the code generator
	*       with a new one.
	*/
	OV_LIBRARY_DEF_fbcomlib_new = *ov_library_open_fbcomlib_old();
	OV_LIBRARY_DEF_fbcomlib_new.setglobalvarsfnc = ov_library_setglobalvars_fbcomlib_new;
	return &OV_LIBRARY_DEF_fbcomlib_new;
}

