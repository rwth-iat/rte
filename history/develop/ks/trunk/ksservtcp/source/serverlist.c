/** @file
* This class is part of the manager.
* Object for storing server data.
* When server registers object of this type with
* the servername is created.
*/
#ifndef OV_COMPILE_LIBRARY_ksservtcp
#define OV_COMPILE_LIBRARY_ksservtcp
#endif


#include "ksservtcp.h"
#include "ksserv.h"
#include "libov/ov_macros.h"
#include "libov/ov_path.h"


/**
* Returns name of registered server.
*/
OV_DLLFNCEXPORT OV_STRING ksservtcp_serverlist_name_get(
    OV_INSTPTR_ksservtcp_serverlist          pobj
) {
    return pobj->v_name;
}

/**
* Sets name of registered server.
* This variable has no effect on the name under which
* the server is registered.
* Therefor only the object name is relevant.
*/
OV_DLLFNCEXPORT OV_RESULT ksservtcp_serverlist_name_set(
    OV_INSTPTR_ksservtcp_serverlist          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_name,value);
}

/**
* Returns version of registered server.
*/
OV_DLLFNCEXPORT OV_INT ksservtcp_serverlist_version_get(
    OV_INSTPTR_ksservtcp_serverlist          pobj
) {
    return pobj->v_version;
}

/**
* Sets version of registered server.
*/
OV_DLLFNCEXPORT OV_RESULT ksservtcp_serverlist_version_set(
    OV_INSTPTR_ksservtcp_serverlist          pobj,
    const OV_INT  value
) {
    pobj->v_version = value;
    return OV_ERR_OK;
}

/**
* Returns port of registered server.
*/
OV_DLLFNCEXPORT OV_INT ksservtcp_serverlist_port_get(
    OV_INSTPTR_ksservtcp_serverlist          pobj
) {
    return pobj->v_port;
}

/**
* Sets port of registered server.
*/
OV_DLLFNCEXPORT OV_RESULT ksservtcp_serverlist_port_set(
    OV_INSTPTR_ksservtcp_serverlist          pobj,
    const OV_INT  value
) {
    pobj->v_port = value;
    return OV_ERR_OK;
}

/**
* Returns timetolife of registered server.
*/
OV_DLLFNCEXPORT OV_UINT ksservtcp_serverlist_timetolife_get(
    OV_INSTPTR_ksservtcp_serverlist          pobj
) {
    return pobj->v_timetolife;
}

/**
* Sets timetolife of registered server.
* By setting this variable the expiration time is updated also.
*/
OV_DLLFNCEXPORT OV_RESULT ksservtcp_serverlist_timetolife_set(
    OV_INSTPTR_ksservtcp_serverlist          pobj,
    const OV_UINT  value
) {
    OV_TIME currenttime;
    OV_TIME exptime;
    OV_TIME_SPAN ttl;
    
    pobj->v_timetolife = value;
    
    ttl.secs = value;
    ttl.usecs = 0;
    
    ov_time_gettime(&currenttime);
    ov_time_add(&exptime, &currenttime, &ttl);
    ksservtcp_serverlist_expirationtime_set(pobj, &exptime);
    
    return OV_ERR_OK;
}

/**
* Returns expiration time of registered server.
*/
OV_DLLFNCEXPORT OV_TIME* ksservtcp_serverlist_expirationtime_get(
    OV_INSTPTR_ksservtcp_serverlist          pobj
) {
    return &pobj->v_expirationtime;
}

/**
* Sets expiration time of registered server.
*/
OV_DLLFNCEXPORT OV_RESULT ksservtcp_serverlist_expirationtime_set(
    OV_INSTPTR_ksservtcp_serverlist          pobj,
    const OV_TIME*  value
) {
    pobj->v_expirationtime = *value;
    return OV_ERR_OK;
}

/**
* Returns state of registered server.
* 1: online; 2: inactiv
*/
OV_DLLFNCEXPORT OV_INT ksservtcp_serverlist_state_get(
    OV_INSTPTR_ksservtcp_serverlist          pobj
) {
    return pobj->v_state;
}

/**
* Sets state of registered server.
*/
OV_DLLFNCEXPORT OV_RESULT ksservtcp_serverlist_state_set(
    OV_INSTPTR_ksservtcp_serverlist          pobj,
    const OV_INT  value
) {
    pobj->v_state = value;
    return OV_ERR_OK;
}

/**
* Procedure called on object startup.
* Registers typemethod and sets registration time
* of registered server.
*/
OV_DLLFNCEXPORT void ksservtcp_serverlist_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_ksservtcp_serverlist pinst = Ov_StaticPtrCast(ksservtcp_serverlist, pobj);
    OV_TIME timenow;
    OV_TIME_SPAN CYCTIME;
    
    

    CYCTIME.secs = 5;
    pinst->v_cycsecs = CYCTIME.secs;
    CYCTIME.usecs = 0;
    pinst->v_cycusecs = CYCTIME.usecs;

    
    /* do what */	 


    //set registration time of registered server
    ov_time_gettime(&timenow);
	 pinst->v_regtime.secs = timenow.secs;
	 pinst->v_regtime.usecs = timenow.usecs;
	 ksservtcp_serverlist_state_set(pinst, 1);


    return;
}

/**
* Procedure called on object shutdown.
*/
OV_DLLFNCEXPORT void ksservtcp_serverlist_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    
    /* do what */

    return;
}

/**
* Procedure periodically called by RootComTask
* Checks if timetolife of server has expired.
* When expired server first is set as inactive
* and after another 300 secs object will be deleted.
*/
OV_DLLFNCEXPORT void ksservtcp_serverlist_typemethod(
	OV_INSTPTR_ksserv_ComTask	cTask
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_ksservtcp_serverlist pinst = Ov_StaticPtrCast(ksservtcp_serverlist, cTask);
	OV_TIME timenow;
    OV_TIME_SPAN timediff;
        
    ov_time_gettime(&timenow);
    ov_time_diff(&timediff, &timenow, &pinst->v_regtime);
            
    if(timediff.secs >= (int)ksservtcp_serverlist_timetolife_get(pinst)) {
    	ksservtcp_serverlist_state_set(pinst, 2);
    }
    if(timediff.secs >= ((int)ksservtcp_serverlist_timetolife_get(pinst)+300)) {
		 Ov_DeleteObject(pinst);
    }
	    
    return;
}

