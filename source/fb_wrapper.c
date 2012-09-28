/******************************************************************************
***                                                                         ***
***   iFBSpro   -   Funktionsbaustein-Model                                 ***
***   #####################################                                 ***
***                                                                         ***
***   L T S o f t                                                           ***
***   Agentur f�r Leittechnik Software GmbH                                 ***
***   Brabanterstr. 13                                                      ***
***   D-50171 Kerpen                                                        ***
***   Tel : 02237/92869-2                                                   ***
***   Fax : 02237/92869-9                                                   ***
***   e-Mail   : ltsoft@ltsoft.de                                           ***
***   Internet : http://www.ltsoft.de                                       ***
***                                                                         ***
***   -------------------------------------------------------------------   ***
***                                                                         ***
***   Implementierung des Funktionsbausteinsystems IFBSpro                  ***
***   RWTH,   Aachen                                                        ***
***   LTSoft, Kerpen                                                        ***
***                                                                         ***
*******************************************************************************
*                                                                             *
*   Datei                                                                     *
*   -----                                                                     *
*   fb_wraper.cpp                                                             *
*                                                                             *
*                                                                             *
*   Historie                                                                  *
*   --------                                                                  *
*   2010-10-14 Alexander Neugebauer: Erstellung, LTSoft GmbH, Kerpen          *
*                                                                             *
*   Beschreibung                                                              *
*   ------------                                                              *
*   C++/Try-Catch wrapper fuer FB-FNC                                         *
*                                                                             *
******************************************************************************/

#ifdef OV_COMPILE_LIBRARY_fb
#undef OV_COMPILE_LIBRARY_fb
#endif

#if OV_SYSTEM_LINUX
	#define _XOPEN_SOURCE 500
#endif

#include "libov/ov_database.h"
#include "libov/ov_logfile.h"

#if OV_SYSTEM_LINUX
	#include <unistd.h>
#endif
#if OV_SYSTEM_SOLARIS
    #include <stdlib.h>
#endif

#define SRV_DEBUG 0

#define IFBS_INC_NAMES_ONLY 1
#include "fb_namedef.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "fb.h"

/*	----------------------------------------------------------------------	*/
/*
*	Map/unmap an existing database
*/
void fb_database_wrapunmap(void) {

#if FB_CATCH_EXCEPTIONS == 1
    try {
#endif
        ov_database_unmap();

#if FB_CATCH_EXCEPTIONS == 1
    } catch(...) {
    }
#endif

    return;
}
/*	----------------------------------------------------------------------	*/
OV_RESULT fb_database_wrapmap(OV_STRING	filename) {
    OV_RESULT res;
        
#if FB_CATCH_EXCEPTIONS == 1
    try {
#endif

        res = ov_database_map(filename);

#if FB_CATCH_EXCEPTIONS == 1
    } catch(...) {
        res = OV_ERR_TARGETGENERIC;        
        fb_database_wrapunmap();
    }
#endif

    return res;
}

/*	----------------------------------------------------------------------	*/
/*
*	Wrap typemethod
*/
OV_RESULT fb_functionblock_wraptypemethod(
    OV_VTBLPTR_fb_functionblock	pfbvtable,
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME				        *pltc
) {

#if FB_CATCH_EXCEPTIONS == 1
    try {
#endif
        pfbvtable->m_typemethod(pfb, pltc);
        
#if FB_CATCH_EXCEPTIONS == 1
    } catch(...) {
        return OV_ERR_GENERIC;
    }
#endif

    return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/
/*
*	Start up the database
*/
OV_RESULT fb_database_wrapstartup(void) {
    OV_RESULT res;
    
#if FB_CATCH_EXCEPTIONS == 1
    try {
#endif

        res = ov_database_startup();

#if FB_CATCH_EXCEPTIONS == 1
    } catch(...) {
        res = OV_ERR_TARGETGENERIC;
    }
#endif

    return res;
}

#define EXEC_PATH_SIZE 512
/*	----------------------------------------------------------------------	*/
/*
*   Get/Set the envinroment
*/
#if OV_SYSTEM_NT
#define ENV_DIRPATHSEPARATOR ";"
#elif OV_SYSTEM_OPENVMS
#define ENV_DIRPATHSEPARATOR ","
#else
#define ENV_DIRPATHSEPARATOR ":"
#endif

void fb___setenv(const char *pEnvVar, const char *pth) {
    char *hStr;
    
    if(!pEnvVar) {
        return;
    }
    if(!pth) {
        return;
    }
    
    hStr = (char*)malloc( strlen(pEnvVar) + strlen(pth) + 2);
    if(!hStr) {
        return;
    }
    sprintf(hStr, "%s=%s", pEnvVar, pth);
    putenv(hStr);
    free(hStr);
    
    return;
}
OV_DLLFNCEXPORT char* fb___getenv(OV_STRING exepth) {
    char 		*ph;
    char 		*penv;
    size_t       pos;
    static char  hStr[EXEC_PATH_SIZE];

#if OV_SYSTEM_NT == 1
    DWORD        result;
    DWORD        path_size = EXEC_PATH_SIZE;    
#endif

#if OV_SYSTEM_LINUX == 1
    long    PID;
    ssize_t ssiz;
    char    path[64];
#endif

#if OV_SYSTEM_OPENVMS == 1
    error --- Funktion fb___getenv() erweitern! ---
#endif
    
    /* IFBS_HOME */
    penv = IFBS_HOME_ENVPATH;
    ph=getenv(penv);
    if(ph) {
        /* OK, gesetzt */
        strncpy(hStr, ph, EXEC_PATH_SIZE-1);
        hStr[EXEC_PATH_SIZE-1] = '\0';
        ph = hStr;
        
        return ph;
    }
    
    /* ACPLT_HOME */
    penv = ACPLT_HOME_ENVPATH;
    ph=getenv(penv);
    if(ph) {
        /* OK, gesetzt */
        strncpy(hStr, ph, EXEC_PATH_SIZE-1);
        hStr[EXEC_PATH_SIZE-1] = '\0';
        ph = hStr;
        
        return ph;
    }
    
    
    /* Dann aus dem Pfad "extrairen" */
    strncpy(hStr, exepth, EXEC_PATH_SIZE-1);
    hStr[511] = '\0';
    
#if OV_SYSTEM_NT == 1
    /* Alle  '\\' mit '/' ersetzen */
    ph = hStr;
    while(ph && (*ph)) {
        if( (*ph) == '\\') {
            *ph = '/';
        }
        ph++;
    }
#endif

    /* Relativen oder fehlerhaften Pfad? */
    pos = 0;
    ph = strstr(hStr, "..");
    if(ph) {
        pos = 1;
    } else {
        ph = strstr(hStr, "//");
        if(ph) {
            pos = 1;
        }
    }
    
    /* Pfad valid? */
    if(pos == 0) {
        /* String-Ende suchen */
        ph = hStr;
        while(ph && (*ph)) {
            ph++;
        }
        
        /* Pfad der Exe '.../server/bin/fb_*.exe' */
        pos = 0;
        while(ph != hStr) {
            if( *ph == '/' ) {
                pos++;
                if(pos == 3) {
                    /* Gefunden */
                    *ph = '\0';
                    ph = hStr;
                
#if OV_SYSTEM_NT == 1
                    while(ph && (*ph)) {
                        if( (*ph) == '/') {
                            *ph = '\\';
                        }
                        ph++;
                    }
                    ph = hStr;
#endif

                    fb___setenv(IFBS_HOME_ENVPATH, ph);
                    return ph;
                }
            }
            ph--;
        }
    }
    
    /* Aufruf ohne Pfad */
    
#if OV_SYSTEM_LINUX == 1
    PID = getpid();
    sprintf(path, "/opt/%ld/exe", PID);
    ssiz = readlink(path, hStr, sizeof(hStr)-1);
    if(ssiz == -1) {
        hStr[0] = '\0';
    } else {
        hStr[ssiz] = '\0';
        hStr[EXEC_PATH_SIZE-1] = '\0';
        
        /* String-Ende suchen */
        ph = hStr;
        while(ph && (*ph)) {
            ph++;
        }
        /* Pfad der Exe '.../server/bin/fb_*.exe' */
        pos = 0;
        while(ph != hStr) {
            if( *ph == '/' ) {
                pos++;
                if(pos == 3) {
                    /* Gefunden */
                    *ph = '\0';
                    ph = hStr;
                    fb___setenv(IFBS_HOME_ENVPATH, ph);
                    return ph;
                }
            }
            ph--;
        }
    }
#endif

#if OV_SYSTEM_SOLARIS == 1
    ph = getexecname();
    if(ph) {
        strncpy(hStr, ph, EXEC_PATH_SIZE-1);
        hStr[EXEC_PATH_SIZE-1] = '\0';
        
        /* String-Ende suchen */
        ph = hStr;
        while(ph && (*ph)) {
            ph++;
        }
        /* Pfad der Exe '.../server/bin/fb_*.exe' */
        pos = 0;
        while(ph != hStr) {
            if( *ph == '/' ) {
                pos++;
                if(pos == 3) {
                    /* Gefunden */
                    *ph = '\0';
                    ph = hStr;
                    fb___setenv(IFBS_HOME_ENVPATH, ph);
                    return ph;
                }
            }
            ph--;
        }
    }
#endif

#if OV_SYSTEM_NT == 1
    memset(hStr, 0, path_size);
    result     = GetModuleFileName(0, hStr, sizeof(hStr)-1);

    if (result == 0) {
        hStr[0] = '\0';
    } else {
        hStr[EXEC_PATH_SIZE-1] = '\0';
        
        if( (result == EXEC_PATH_SIZE) || (result == EXEC_PATH_SIZE-1) ) {
            /* EXE-Path laenger als 512 Bytes? */
        }  else  {
            hStr[EXEC_PATH_SIZE-1] = '\0';

            /* String-Ende suchen */
            ph = hStr;
            while(ph && (*ph)) {
                if( (*ph) == '\\') {
                    *ph = '/';
                }
                ph++;
            }
            /* Pfad der Exe '.../server/bin/fb_*.exe' */
            pos = 0;
            while(ph != hStr) {
                if( *ph == '/' ) {
                    pos++;
                    if(pos == 3) {
                        /* Gefunden */
                        *ph = '\0';
                        ph = hStr;

                        while(ph && (*ph)) {
                            if( (*ph) == '/') {
                                *ph = '\\';
                            }
                            ph++;
                        }
                        ph = hStr;

                        fb___setenv(IFBS_HOME_ENVPATH, ph);
                        return ph;
                    }
                }
                ph--;
            }
        }
    }
#endif

    return 0;
}

#ifdef __cplusplus
}
#endif

/*
*	End of file
*/
