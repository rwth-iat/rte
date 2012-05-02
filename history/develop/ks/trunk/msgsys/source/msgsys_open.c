#ifndef OV_COMPILE_LIBRARY_msgsys
#define OV_COMPILE_LIBRARY_msgsys
#endif

#include "msgsys.h"
#include "msgconfig.h"
#include "libov/ov_result.h"

#ifdef ov_library_open_msgsys
#undef ov_library_open_msgsys
#endif

/*
*       This function will be called, after the library is loaded.
*       ----------------------------------------------------------
*/
OV_RESULT ov_library_setglobalvars_msgsys_new(void) {
	/*
	*       local variables
	*/
	OV_RESULT								result    = OV_ERR_OK;

	OV_INSTPTR_msgsys_MsgComponent			component  = NULL;
	OV_INSTPTR_msgsys_ServiceRegistry		services  = NULL;

	OV_INSTPTR_ov_domain 					domain = NULL;
	OV_INSTPTR_fb_task						urtaskdom = NULL;

	/*
	*       set the global variables and if successfull go on
	*/
	result = ov_library_setglobalvars_msgsys();

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
		if(!Ov_OK((Ov_CreateObject(msgsys_MsgComponent, component, domain, MSGCOMPONENTCONTAINER)))){
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
		if(!Ov_OK((Ov_CreateObject(msgsys_ServiceRegistry, services, domain, "Services")))){
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
OV_DLLFNCEXPORT OV_LIBRARY_DEF *ov_library_open_msgsys(void) {
	/*
	*       local variables
	*/
	static OV_LIBRARY_DEF OV_LIBRARY_DEF_msgsys_new;


	        OV_INSTPTR_ov_library pNewOvLib = NULL;
	        OV_RESULT fr = OV_ERR_OK;
	        OV_INSTPTR_ov_domain pTarget;
	        //this is executed at database start, too

	        fr = OV_ERR_OK;
	        pTarget = Ov_DynamicPtrCast(ov_domain, ov_path_getobjectpointer("/acplt", 2));
	        if (pTarget != NULL){
	                fr = Ov_CreateObject(ov_library, pNewOvLib, pTarget, "ksapi");
	                if (fr != OV_ERR_ALREADYEXISTS && Ov_Fail(fr))
	                {
	                        ov_logfile_error("msgsys needs the library %s which is not available (%s).",
	                                        "ksapi",
	                                        ov_result_getresulttext(fr));

	                }
	        } else {
	                ov_logfile_error("domain /acplt does not exist");
	        }


	/*
	*       replace the "setglobalvars" function created by the code generator
	*       with a new one.
	*/
	OV_LIBRARY_DEF_msgsys_new = *ov_library_open_msgsys_old();
	OV_LIBRARY_DEF_msgsys_new.setglobalvarsfnc = ov_library_setglobalvars_msgsys_new;
	return &OV_LIBRARY_DEF_msgsys_new;
}

