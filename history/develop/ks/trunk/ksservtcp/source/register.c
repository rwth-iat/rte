/** @file
* This class is part of the manager.
* Object for registering server at manager.
*/
#ifndef OV_COMPILE_LIBRARY_ksservtcp
#define OV_COMPILE_LIBRARY_ksservtcp
#endif


#include "ksservtcp.h"
#include "libov/ov_macros.h"
#include "ksserv_logfile.h"
#include <string.h>
#include "libov/ov_path.h"

/**
* Returns name of server to be registered.
*/
OV_DLLFNCEXPORT OV_STRING ksservtcp_register_name_get(
    OV_INSTPTR_ksservtcp_register          pobj
) {
    return pobj->v_name;
}

/**
* Sets name of server to be registered.
*/
OV_DLLFNCEXPORT OV_RESULT ksservtcp_register_name_set(
    OV_INSTPTR_ksservtcp_register          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_name,value);
}

/**
* Returns version of server to be registered
*/
OV_DLLFNCEXPORT OV_INT ksservtcp_register_version_get(
    OV_INSTPTR_ksservtcp_register          pobj
) {
    return pobj->v_version;
}

/**
* Sets version of server to be registered
*/
OV_DLLFNCEXPORT OV_RESULT ksservtcp_register_version_set(
    OV_INSTPTR_ksservtcp_register          pobj,
    const OV_INT value
) {
    pobj->v_version = value;
    return OV_ERR_OK;
}

/**
* Returns port of server to be registered
*/
OV_DLLFNCEXPORT OV_INT ksservtcp_register_port_get(
    OV_INSTPTR_ksservtcp_register          pobj
) {
    return pobj->v_port;
}

/**
* Sets port of server to be registered
*/
OV_DLLFNCEXPORT OV_RESULT ksservtcp_register_port_set(
    OV_INSTPTR_ksservtcp_register          pobj,
    const OV_INT  value
) {
    pobj->v_port = value;
    return OV_ERR_OK;
}

/**
* Returns timetolife of server to be registered.
*/
OV_DLLFNCEXPORT OV_UINT ksservtcp_register_timetolife_get(
    OV_INSTPTR_ksservtcp_register          pobj
) {
    return pobj->v_timetolife;
}

/**
* Sets timetolife of server to be registered.
* Timetolife is the time in which server has to do a call back
* at manager.
* Else server is set as inactive.
*/
OV_DLLFNCEXPORT OV_RESULT ksservtcp_register_timetolife_set(
    OV_INSTPTR_ksservtcp_register          pobj,
    const OV_UINT  value
) {
    pobj->v_timetolife = value;
    return OV_ERR_OK;
}

/**
* Returns value of process variable.
*/
OV_DLLFNCEXPORT OV_BOOL ksservtcp_register_process_get(
    OV_INSTPTR_ksservtcp_register          pobj
) {
    return pobj->v_process;
}

/**
* Sets process variable.
* By setting this variable as true server registration starts
* if all variables (name, version, port, timetolife) are set.
*/
OV_DLLFNCEXPORT OV_RESULT ksservtcp_register_process_set(
    OV_INSTPTR_ksservtcp_register          pobj,
    const OV_BOOL  value
) {
	 OV_INSTPTR_ov_domain pslnd=NULL;
	 OV_INSTPTR_ov_domain psled=NULL;
	 OV_INSTPTR_ov_domain sldomain = (OV_INSTPTR_ov_domain)ov_path_getobjectpointer("/servers",2);
	 OV_INSTPTR_ksservtcp_serverlist psln = NULL;
	 OV_INSTPTR_ksservtcp_serverlist psle = NULL;
	 OV_TIME timenow;
	 int cnt = 1;
	 char cntstr[11];
	 
	 //KSDEVEL printf("\n\n\nregister:\nname: %s\nversion: %d\nttl: %d\nport: %d\n\n\n", pobj->v_name, pobj->v_version, pobj->v_timetolife, pobj->v_port);
	 
    pobj->v_process = value;
    
    if(value == TRUE)
    {
		 if((pobj->v_name) && (pobj->v_version) && (pobj->v_port) && (pobj->v_timetolife) && (sldomain))
		 {//all values are set
		 
		 	if(Ov_OK(Ov_CreateObject(ov_domain, pslnd, sldomain, pobj->v_name)))
		 	{//domain with servername created
			
				if(Ov_OK(Ov_CreateObject(ksservtcp_serverlist, psln, pslnd, "2")))
				{//object "2" created (this structure is for compatibility with magellan and xfbspro)
				
					ksservtcp_serverlist_version_set(psln, pobj->v_version);
					ksservtcp_serverlist_port_set(psln, pobj->v_port);
					ksservtcp_serverlist_timetolife_set(psln, pobj->v_timetolife);
					ksservtcp_serverlist_state_set(psln, 1);
					ksservtcp_register_res_set(pobj, 1);
				}
				else
				{
					ksservtcp_register_res_set(pobj, -1);
					return OV_ERR_GENERIC;
				}
		 	}
		 	else
		 	{//domain with servername already exists

				//get pointer to domain with servername
				psled = (OV_INSTPTR_ov_domain)Ov_SearchChild(ov_containment, sldomain, pobj->v_name);
				do
				{//check if object with matching port already exist
				 //if so --> update expiration time
				 //if not create object with new port
				 
					psle = NULL;
					cnt++;
					sprintf(cntstr, "%i", cnt);
					psle = (OV_INSTPTR_ksservtcp_serverlist)Ov_SearchChild(ov_containment, psled, cntstr);
					if(psle)
					{
						//check if sever values match to existing ones
						if((pobj->v_version == psle->v_version) && (pobj->v_port == psle->v_port))
						{//values match --> update expiration time
						
							ksservtcp_serverlist_timetolife_set(psle, pobj->v_timetolife);
							ov_time_gettime(&timenow);
							psle->v_regtime.secs = timenow.secs;
							psle->v_regtime.usecs = timenow.usecs;
							ksservtcp_serverlist_state_set(psle, 1);
							ksservtcp_register_res_set(pobj, 1);
							psle = NULL;
							return OV_ERR_OK;
						}
						else if(pobj->v_version != psle->v_version)
						{//version does not match --> error
							ksservtcp_register_res_set(pobj, -1);
							ksserv_logfile_error("register: server with this name but different version already exits.");
							psle = NULL;
							return OV_ERR_GENERIC;
						}
					}
				}while(psle);
				
				//KSDEVEL printf("\n\nCNTSTR: %s\n\n", cntstr);
				if(Ov_OK(Ov_CreateObject(ksservtcp_serverlist, psle, psled, cntstr)))
				{//object with new port created
				
		 			ksservtcp_serverlist_version_set(psle, pobj->v_version);
					ksservtcp_serverlist_port_set(psle, pobj->v_port);
					ksservtcp_serverlist_timetolife_set(psle, pobj->v_timetolife);
					ksservtcp_register_res_set(pobj, 1);
				}
				else
				{
					ksserv_logfile_error("register: unknown error.");
					return OV_ERR_GENERIC;
				}
			}
		}
		else
		{//not all values are set
		
			ksservtcp_register_res_set(pobj, -1);
			ksserv_logfile_error("register: Not all values are set.");
			return OV_ERR_GENERIC;
		}
	 }
	 pobj->v_process = FALSE;
    
    return OV_ERR_OK;
}

/**
* Returns value of result variable.
*/
OV_DLLFNCEXPORT OV_INT ksservtcp_register_res_get(
    OV_INSTPTR_ksservtcp_register          pobj
) {
    return pobj->v_res;
}

/**
* Sets value of result variable.
*/
OV_DLLFNCEXPORT OV_RESULT ksservtcp_register_res_set(
    OV_INSTPTR_ksservtcp_register          pobj,
    const OV_INT  value
) {
    pobj->v_res = value;
    return OV_ERR_OK;
}

/**
* Procedure called on object startup.
*/
OV_DLLFNCEXPORT void ksservtcp_register_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    
    /* do what the base class does first */
    ov_object_startup(pobj);

    /* do what */


    return;
}

/**
* Procedure called on object shutdown.
*/
OV_DLLFNCEXPORT void ksservtcp_register_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    
    /* do what */

    /* set the object's state to "shut down" */
    ov_object_shutdown(pobj);

    return;
}
