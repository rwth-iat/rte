/******************************************************************************
***                                                                         ***
***   iFBSpro   -   Funktionsbaustein-Model                                 ***
***   #####################################                                 ***
***                                                                         ***
***   L T S o f t                                                           ***
***   Agentur für Leittechnik Software GmbH                                 ***
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
*   fb_namedef.h                                                              *
*                                                                             *
*   Historie                                                                  *
*   --------                                                                  *
*   1998-02-22  Alexander Neugebauer: Erstellung, LTSoft, Kerpen              *
*                                                                             *
*   Beschreibung                                                              *
*   ------------                                                              *
*   Name-Definitionen der FB-Objekten                                         *
*                                                                             *
******************************************************************************/

#include "libov/ov_config.h"

#ifndef _FB_NAMEDEF_H_
#define _FB_NAMEDEF_H_

#define  KS_VERSION				2

#define  ANZ_MAX_ARRAY_ELEM          75
#define  FB_INSTANZ_CONTAINER        "TechUnits"
#define  FB_INSTANZ_CONTAINER_PATH   "/TechUnits"
#define  FB_TASK_CONTAINER           "Tasks"
#define  FB_TASK_CONTAINER_PATH      "/Tasks"
#define  FB_CONN_CONTAINER           "Cons"
#define  FB_CONN_CONTAINER_PATH      "/Cons"
#define  FB_URTASK                   "UrTask"
#define  FB_URTASK_PATH              "/Tasks/UrTask"
#define  FB_LIBRARIES_CONTAINER      "Libraries"
#define  FB_LIBRARIES_CONTAINER_PATH "/Libraries"
#define  FB_TOOL_LIBRARY             "fbtool"
#define  FB_TOOL_CONTAINER           "fbtool"
#define  FB_TOOL_CONTAINER_PATH      "/fbtool"
#define  FB_FB_PATH                  "/fb/functionblock"
#define  OV_LIB_PATH                 "/acplt/ov/"
#define  OV_ASSOC_PATH               "/acplt/ov/association"
#define  OV_CLASS_PATH               "/acplt/ov/class"
#define  OV_VARLIBS_PATH             "/vendor/libraries"
#define  FB_DBINFO                   "dbinfo"
#define  FB_SERV_INFO_VAR            "licinfo"

/* Logger */ 
#define  FB_LOGGER_CONTAINER        "serverinfo"
#define  FB_LOGGER_CONTAINER_PATH   "/serverinfo"
#define  FB_LOGGER_NAME             "logging"
#define  FB_LOGGER_PATH             "/serverinfo/logging"

/* FileUpload */ 
#define  FB_UPLOAD_NAME             "upload"
#define  FB_UPLOAD_PATH             "/serverinfo/upload"

/* DB and backup file extensions */ 
#define  DB_FILEOVD_EX          ".ovd"
#define  DB_FILEBAC_EX          ".bac"

#define  IFBS_HOME_ENVPATH      "IFBS_HOME"
#define  ACPLT_HOME_ENVPATH     "ACPLT_HOME"

/* fb/task: actimode */
#define FB_AM_OFF       0
#define FB_AM_ON        1
#define FB_AM_UNLINK    2
#define FB_AM_ONCE      3
#define FB_AM_CATCHUP   10


/* Error codes */
#define FB_ERR_ATPARSE          0x00020000
#define IFBS_OK_FLAG            "OK"
#define IFBS_ERR_FLAG           "ERR"
#define IFBS_WARNING_FLAG       "WARNING"

/* Init output value */
#define IFBS_INIT_VALUE     -989898

#ifndef IFBS_INC_NAMES_ONLY
#include "fb.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
*	Execute the child objects
*/
OV_DLLFNCEXPORT void fb_task_execChildObjects(
	OV_INSTPTR_fb_task	ptask,
	OV_TIME				*pltc
);

/*
*	Set next proctime of the task object
*/
OV_DLLFNCEXPORT void fb_task_setNextProcTime(
	OV_INSTPTR_fb_task	ptask,
	OV_TIME				*pltc
);

/*
*   Check whether the variable type is supportive of FB
*/
OV_DLLFNCEXPORT int fb_vartype_implemented(
    OV_VAR_TYPE typ
);

/*
*   Set variable value
*/
OV_DLLFNCEXPORT OV_RESULT fb_set_varvalue(
    OV_INSTPTR_ov_object  pobj,
    const OV_ELEMENT     *pelem,
    const OV_ANY         *pvarcurrprops,
    OV_BOOL              *changed
);

/*
*	Trigger input get connections
*/
OV_DLLFNCEXPORT void fb_functionblock_triggerInpGetConnections(
	OV_INSTPTR_fb_functionblock	pfb
);
OV_DLLFNCEXPORT void fb_object_triggerInpGetConnections(
	OV_INSTPTR_fb_object	pfb
);

/*
*	Trigger output send connections
*/
OV_DLLFNCEXPORT void fb_functionblock_triggerOutSendConnections(
	OV_INSTPTR_fb_functionblock	pfb
);
OV_DLLFNCEXPORT void fb_object_triggerOutSendConnections(
	OV_INSTPTR_fb_object	pfb
);

/*
*	Call typemethod of functionblock
*/
OV_DLLFNCEXPORT OV_BOOL fb_functionblock_execTypeMethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME				        *pltc
);

/*
* Create a connection object
* --------------------------
* OV_INSTPTR_fb_connection  pcon;
* result = fb_connection_create("myconobj","/TechUnits/sum1","out","/TechUnits/sum2","in1",&pcon);
* if(result != OV_ERR_OK) {
*     return result;
* }
* // turn on
* pcon->v_on = TRUE;
*/
OV_DLLFNCEXPORT OV_RESULT fb_connection_create(
	OV_STRING				  identifier,         /* Connection identifier "myconobj" */
	OV_STRING				  sourceFB,           /* Path source FB "/TechUnits/sum1" */
	OV_STRING				  sourcePort,         /* Source port identifier "out"     */
	OV_STRING				  targetFB,           /* Path target FB "/TechUnits/sum2" */
	OV_STRING				  targetPort,         /* Target port identifier "in1"     */
    OV_INSTPTR_fb_connection  *pcon               /* Pointer connection object        */
);

/*
*   Get the envinroment
*/
OV_DLLFNCEXPORT char* fb___getenv(OV_STRING exepth);
OV_DLLFNCEXPORT void fb___addenvpath(const char *pEnvVar, const char *pth);

#ifdef __cplusplus
} // Extern "C"
#endif

#endif /* IFBS_INC_NAMES_ONLY */

/*
*	Plattformen...
*	--------------
*/

#if OV_SYSTEM_HPUX == 1
#define SERVER_SYSTEM "hpux"
#endif

#if OV_SYSTEM_LINUX == 1
#define SERVER_SYSTEM "linux"
#endif

#if OV_SYSTEM_NT == 1
#define SERVER_SYSTEM "nt"
#endif

#if OV_SYSTEM_OPENVMS == 1
#define SERVER_SYSTEM "openvms"
#endif

#if OV_SYSTEM_SOLARIS == 1
#define SERVER_SYSTEM "solaris"
#endif

#if OV_SYSTEM_MC164 == 1
#define SERVER_SYSTEM "mc164"
#endif

#endif
