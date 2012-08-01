#include "msgreceiver.h"
#include "receiverconfig.h"

#undef ov_library_open_msgreceiver


/*
*       This function will be called, after the library is loaded.
*       ----------------------------------------------------------
*/
OV_RESULT ov_library_setglobalvars_msgreceiver_new(void) {
	/*
	*       local variables
	*/
	OV_RESULT   		    		result    = OV_ERR_OK;

	OV_INSTPTR_msgreceiver_Receiver		receiver  = NULL;
	OV_INSTPTR_msgsys_MessageBox	inbox = NULL;
	OV_INSTPTR_msgsys_MessageBox	outbox = NULL;

	OV_INSTPTR_ov_domain 			techunits_domain = NULL;
	OV_INSTPTR_ov_domain 			msgreceiver_domain = NULL;
	OV_INSTPTR_fb_task			urtaskdom = NULL;
	//	OV_STRING tmpPath;
	
	

	/*
	*       set the global variables and if successfull go on
	*/
	result = ov_library_setglobalvars_msgreceiver();

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
	  ov_logfile_debug("Creating Object 'Receiver'");
	  if(!Ov_OK((Ov_CreateObject(msgreceiver_Receiver, receiver, techunits_domain, RECEIVERCONTAINER))))
	    {
	      ov_logfile_error("Fatal: Couldnt create Object 'MsgSys'");
	      return OV_ERR_OK;
	    } 
	  else 
	    {
	      Ov_Link(fb_tasklist, urtaskdom, receiver);
	      ov_logfile_debug("Done.");
	    }
	  	  //Messagebox
	  msgreceiver_domain = (OV_INSTPTR_ov_domain)ov_path_getobjectpointer("/TechUnits/Receiver", 2);
	  ov_logfile_debug("Creating Object 'Message'");
	  if(!Ov_OK((Ov_CreateObject(msgsys_MessageBox, inbox, msgreceiver_domain, "Inbox"))))
	    {
	      ov_logfile_error("Fatal: Couldnt create Object 'Message'");
	      return OV_ERR_OK;
	    } 
	  else 
	    {
	      Ov_Link(fb_tasklist, urtaskdom, inbox);
	      ov_logfile_debug("Done.");
	    }

	    if(!Ov_OK((Ov_CreateObject(msgsys_MessageBox, outbox, msgreceiver_domain, "Outbox"))))
	    {
	      ov_logfile_error("Fatal: Couldnt create Object 'Message'");
	      return OV_ERR_OK;
	    } 
	  else 
	    {
	      Ov_Link(fb_tasklist, urtaskdom, inbox);
	      ov_logfile_debug("Done.");
	    }
	  ov_logfile_error("Receiver Objects loaded!!");
	}	//endif setglobalvalues
	return result;
}



/*
*       Replace the "setglobalvars" function of a library with this
*       previous one, which additionally creates instances.
*       ----------------------------------------------------------------------
*/
OV_DLLFNCEXPORT OV_LIBRARY_DEF *ov_library_open_msgreceiver(void) {
	/*
	*       local variables
	*/
	static OV_LIBRARY_DEF OV_LIBRARY_DEF_msgreceiver_new;
	/*
	*       replace the "setglobalvars" function created by the code generator
	*       with a new one.
	*/
	OV_LIBRARY_DEF_msgreceiver_new = *ov_library_open_msgreceiver_old();
	OV_LIBRARY_DEF_msgreceiver_new.setglobalvarsfnc = ov_library_setglobalvars_msgreceiver_new;
	return &OV_LIBRARY_DEF_msgreceiver_new;
}

