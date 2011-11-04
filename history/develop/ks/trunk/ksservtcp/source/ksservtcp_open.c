/** @file 
* Startup procedure on library loading 
*/

#ifndef OV_COMPILE_LIBRARY_ksservtcp
#define OV_COMPILE_LIBRARY_ksservtcp
#endif

#include "ksservtcp.h"
#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include "ksserv_logfile.h"

#ifdef ov_library_open_ksservtcp
#undef ov_library_open_ksservtcp
#endif


#if OV_SYSTEM_NT
void sysMsgToError() {
    DWORD       eNum;
    TCHAR       sysMsg[256];
    TCHAR*      p;
    static char smsgstr[2048];
    
    // INIT
    sysMsg[0] = 0;
    
    eNum = GetLastError( );
    FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
         NULL, eNum,
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
         sysMsg, 255, NULL );
    
    // Trim the end of the line and terminate it with a null
    p = sysMsg;
    while( ( *p > 31 ) || ( *p == 9 ) ) {
        ++p;
    }
    do {
        *p-- = 0;
    } while( ( p >= sysMsg ) && ( ( *p == '.' ) || ( *p < 33 ) ) );

    // Display the message
    ksserv_logfile_error(" failed with error %lu (%s)",
         (unsigned long)eNum, sysMsg);
}
#endif

/**
* This function will be called, after the library is loaded.
// 
// name: ov_library_setglobalvars_ksservtcp_new
// @param
// @return
*/
OV_RESULT ov_library_setglobalvars_ksservtcp_new(void) {
	/*
	*       local variables
	*/
	OV_RESULT   		    		result    = OV_ERR_OK;

	OV_INSTPTR_ksservtcp_tcpconnection		ptc  = NULL;

	OV_INSTPTR_ov_domain				pksserv  = NULL;
	OV_INSTPTR_ov_domain 			domain = NULL;
	OV_INSTPTR_ov_domain				pcommunication  = NULL;

#if OV_SYSTEM_LINUX
   OV_INSTPTR_ov_object        	pobj;
#endif

	
#if OV_SYSTEM_NT
        WSADATA wsa;
        if (WSAStartup(MAKEWORD(1, 1), &wsa)) {
            sysMsgToError();
        }
#endif

	/*
	*       set the global variables and if successful go on
	*/
	result = ov_library_setglobalvars_ksservtcp();

	if(Ov_OK(result))
	{
		/*
		*       get "/" pointer
		*/
		domain = (OV_INSTPTR_ov_domain)ov_path_getobjectpointer("/", 2);
		if(!domain)
		{
			ksserv_logfile_error("Fatal: Coudnt find Domain '/'! - Fatal Error");
			return OV_ERR_GENERIC;
		}
		

		
		/*
		*       create "ksservtcp" object
		*/
		//communication container should be provided by ksserv
		pcommunication = (OV_INSTPTR_ov_domain)Ov_SearchChild(ov_containment, domain, "communication");
		pksserv = (OV_INSTPTR_ov_domain)Ov_SearchChild(ov_containment, pcommunication, "ksservtcp");
	   if(!pksserv)
	   {
#if OV_SYSTEM_LINUX
    		result = Ov_CreateObject(ov_domain, pobj, pcommunication, "ksservtcp");
#else
    	   result = Ov_CreateObject(ov_domain, pksserv, pcommunication, "ksservtcp");
#endif
    	   if(Ov_Fail(result))
    	   {
				ksserv_logfile_error("Fatal: Couldnt create Object 'ksservtcp'");
    			return result;
    	   }
#if OV_SYSTEM_LINUX
         pksserv = (OV_INSTPTR_ov_domain)pobj;
#endif
		}

		/*
		*       create "tcpconnection" object
		*/
		ptc = (OV_INSTPTR_ksservtcp_tcpconnection)Ov_SearchChild(ov_containment, pksserv, "tcpconnection");
	   if(!ptc)
	   {
#if OV_SYSTEM_LINUX
    		result = Ov_CreateObject(ksservtcp_tcpconnection, pobj, pksserv, "tcpconnection");
#else
    	   result = Ov_CreateObject(ksservtcp_tcpconnection, ptc, pksserv, "tcpconnection");
#endif
    	   if(Ov_Fail(result))
    	   {
				ksserv_logfile_error("Fatal: Couldnt create Object 'tcpconnection'");
			   return result;
		   }
		}
	}

	return result;
}



/**
*       Replace the "setglobalvars" function of a library with this
*       previous one, which additionally creates instances.
* 	This is called by the OV system upon library load.
*/
OV_DLLFNCEXPORT OV_LIBRARY_DEF *ov_library_open_ksservtcp(void) {
	/*
	*       local variables
	*/
	static OV_LIBRARY_DEF OV_LIBRARY_DEF_ksservtcp_new;
	/*
	*       replace the "setglobalvars" function created by the code generator
	*       with a new one.
	*/
	OV_LIBRARY_DEF_ksservtcp_new = *ov_library_open_ksservtcp_old();
	OV_LIBRARY_DEF_ksservtcp_new.setglobalvarsfnc = ov_library_setglobalvars_ksservtcp_new;
	return &OV_LIBRARY_DEF_ksservtcp_new;
}

