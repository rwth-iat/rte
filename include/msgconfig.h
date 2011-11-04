
#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include "libov/ov_logfile.h"
#include "libov/ov_string.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

OV_DLLVAREXPORT const OV_STRING TECHROOT;
OV_DLLVAREXPORT const OV_STRING MSGCOMPONENTROOT;
OV_DLLVAREXPORT const OV_STRING MSGCOMPONENTCONTAINER;
OV_DLLVAREXPORT const OV_STRING MESSAGECONTAINER;
OV_DLLVAREXPORT const OV_STRING SENDINGINSTANCEPATH;
OV_DLLVAREXPORT const OV_STRING SERVICEROOT;

OV_INT LOCALMSGCOUNTER;

const OV_INT MSGNEW;
const OV_INT MSGWAITING;
const OV_INT MSGDONE;
const OV_INT MSGRECEIVERSERVICEERROR;
const OV_INT MSGFATALERROR;

OV_DLLVAREXPORT const OV_TIME_SPAN MSGCYCTIME;

OV_BOOL MSG_split(OV_STRING victim, OV_STRING delimiter, OV_INT *position, OV_STRING *token);

#define fbcomlib_CreateAnonymousMessage(class, pobj, pparent, ident, result)                                            \
        {\
	int i = 0;\
        OV_STRING tmpName = NULL	;\
	*result = 0;\
        while(TRUE) {\
                ov_string_print(&tmpName, "%s-%d",ident,i);\
                if(Ov_OK(Ov_CreateObject(class, pobj, pparent, tmpName))){\
					    *result = 1;\
                        break;\
                }\
                if(OV_ERR_DBOUTOFMEMORY == Ov_CreateObject(class, pobj, pparent, tmpName)){\
                        ov_logfile_error("Database is FULL! FAILED to create Anonymous Object");\
                       break;\
                }\
                i++;\
        }\
	ov_string_setvalue(&tmpName, NULL);\
	}
