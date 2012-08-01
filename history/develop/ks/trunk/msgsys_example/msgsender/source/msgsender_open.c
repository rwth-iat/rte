#include "msgsender.h"
#include "senderconfig.h"

#undef ov_library_open_msgsender


/*
*       This function will be called, after the library is loaded.
*       ----------------------------------------------------------
*/
OV_RESULT ov_library_setglobalvars_msgsender_new(void) {
	/*
	*       local variables
	*/
	OV_RESULT   		    		result    = OV_ERR_OK;

	OV_INSTPTR_msgsender_Sender		sender  = NULL;
	OV_INSTPTR_msgsys_MessageBox	inbox = NULL;
	OV_INSTPTR_msgsys_MessageBox	outbox = NULL;

	OV_INSTPTR_ov_domain 			techunits_domain = NULL;
	OV_INSTPTR_ov_domain 			msgsender_domain = NULL;
	OV_INSTPTR_fb_task			urtaskdom = NULL;
	//	OV_STRING tmpPath;
	
	

	/*
	*       set the global variables and if successfull go on
	*/
	result = ov_library_setglobalvars_msgsender();

	if(Ov_OK(result)) {
	//////////////////////////////////////////////////////////
	//get FB references for the domain ApplicationContainer
       
	  techunits_domain = (OV_INSTPTR_ov_domain)ov_path_getobjectpointer(TECHROOT, 2);
	  if(!techunits_domain)
	    {
	      ov_logfile_error("Fatal: Coudnt find Application-Domain '/TechUnits/'! - Fatal Error");
	     
	      return OV_ERR_GENERIC;
	    }
	  urtaskdom = (OV_INSTPTR_fb_task)ov_path_getobjectpointer("/Tasks/UrTask", 2);
	  if(!urtaskdom)
	    {
	  	ov_logfile_error("Fatal: Couldnt find '/Task/UrTask' - Fatal Error!");
		return OV_ERR_GENERIC;
	    }
	  
	  //////////////////////////////////////////////
	  //create components

	  ov_logfile_debug("Now creating Segfaulttestlib Objects/Components!");

	  //Container
	  ov_logfile_debug("Creating Object 'Sender'");
	  if(!Ov_OK((Ov_CreateObject(msgsender_Sender, sender, techunits_domain, SENDERCONTAINER))))
	    {
	      ov_logfile_error("Fatal: Couldnt create Object 'MsgSys'");
	      return OV_ERR_OK;
	    } 
	  else 
	    {
	      Ov_Link(fb_tasklist, urtaskdom, sender);
	      ov_logfile_debug("Done.");
	    }

	  msgsender_domain = (OV_INSTPTR_ov_domain)ov_path_getobjectpointer("/TechUnits/Sender", 2);
	  ov_logfile_debug("Creating Object 'Message'");
	  if(!Ov_OK((Ov_CreateObject(msgsys_MessageBox, inbox, msgsender_domain, "Inbox"))))
	    {
	      ov_logfile_error("Fatal: Couldnt create Object 'Message'");
	      return OV_ERR_OK;
	    } 
	  else 
	    {
	      Ov_Link(fb_tasklist, urtaskdom, inbox);
	      ov_logfile_debug("Done.");
	    }

	  	  if(!Ov_OK((Ov_CreateObject(msgsys_MessageBox, outbox, msgsender_domain, "Outbox"))))
	    {
	      ov_logfile_error("Fatal: Couldnt create Object 'Message'");
	      return OV_ERR_OK;
	    } 
	  else 
	    {
	      Ov_Link(fb_tasklist, urtaskdom, inbox);
	      ov_logfile_debug("Done.");
	    }
	  ov_logfile_error("Sender Objects loaded!!");
	}	//endif setglobalvalues
	return result;
}



/*
*       Replace the "setglobalvars" function of a library with this
*       previous one, which additionally creates instances.
*       ----------------------------------------------------------------------
*/
OV_DLLFNCEXPORT OV_LIBRARY_DEF *ov_library_open_msgsender(void) {
	/*
	*       local variables
	*/
	static OV_LIBRARY_DEF OV_LIBRARY_DEF_msgsender_new;
	/*
	*       replace the "setglobalvars" function created by the code generator
	*       with a new one.
	*/
	OV_LIBRARY_DEF_msgsender_new = *ov_library_open_msgsender_old();
	OV_LIBRARY_DEF_msgsender_new.setglobalvarsfnc = ov_library_setglobalvars_msgsender_new;
	return &OV_LIBRARY_DEF_msgsender_new;
}

