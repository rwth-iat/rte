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
*   fb_dbinfo.c                                                               *
*                                                                             *
*                                                                             *
*   Historie                                                                  *
*   --------                                                                  *
*   1999-07-08 Alexander Neugebauer: Erstellung, LTSoft GmbH, Kerpen          *
*                                    Lehrstuhl fuer Prozessleittechnik, Aachen*
*   2001-06-12 Major revision.       A.Neugebauer                             *
*                                                                             *
*   Beschreibung                                                              *
*   ------------                                                              *
*   Implementierung der FB-Klasse dbinfo                                      *
*                                                                             *
******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_fb
#define OV_COMPILE_LIBRARY_fb
#endif

#include "fb.h"
#include "fb_namedef.h"
#include "fb_macros.h"
#include "fb_database.h"
#include "libov/ov_string.h"
#include "libov/ov_path.h"
#include "libov/ov_macros.h"
#include "libov/ov_vendortree.h"
#include "fb_av.h"

/* Deklarationen */
char* lts_encode_string(
    char *pOut
    ,char *str
);

/*
*	Licence
*/
#if FB_OEM_LICENCE == 1
#else
#include "checklic.h"
#endif

/*
*	Global variables
*/
//Sten: was never used so I removed it
//static int fb_startup = 0;

/*	----------------------------------------------------------------------	*/
/*
*	Start up of dbinfo objects
*/
OV_DLLFNCEXPORT void fb_dbinfoclass_startup(
	OV_INSTPTR_ov_object pobj
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_fb_dbinfoclass	pdbi = Ov_StaticPtrCast(fb_dbinfoclass, pobj);

	ov_object_startup(pobj);
	ov_string_setvalue(&pdbi->v_serversystem, SERVER_SYSTEM);
	ov_string_setvalue(&pdbi->v_version, "");
	ov_string_setvalue(&pdbi->v_licinfo, "");
}

/*	----------------------------------------------------------------------	*/
/*
*	Constructor of db-info object
*/
OV_DLLFNCEXPORT OV_RESULT fb_dbinfoclass_constructor(
	OV_INSTPTR_ov_object pobj
) {
	OV_STRING					path;
	char						help[32];
    OV_RESULT                   result;
    
    
	result = ov_object_constructor(pobj);
	if(Ov_Fail(result))
		return result;

    /*
    *	test if we are the "/dbinfo"
    */
	sprintf(help, "/%s", FB_DBINFO);
	ov_memstack_lock();
	path = ov_path_getcanonicalpath(pobj,KS_VERSION);
	if(!ov_string_compare(path, help)) {
		result = OV_ERR_OK;
	} else {
	    result = OV_ERR_BADPATH;
	}
	ov_memstack_unlock();
	
	return result;
}

/*	----------------------------------------------------------------------	*/
OV_DLLFNCEXPORT void fb_dbinfoclass_destructor(
	OV_INSTPTR_ov_object pobj
) {
	ov_object_destructor(pobj);	
    fb_database_nildbinfo();
}

/*	----------------------------------------------------------------------	*/
OV_DLLFNCEXPORT void fb_dbinfoclass_shutdown(
	OV_INSTPTR_ov_object pobj
) {
	ov_object_shutdown(pobj);
    fb_database_nildbinfo();
}

/*	----------------------------------------------------------------------	*/
/*
*	Get the access rights for the dbinfo object or its variables
*/
OV_DLLFNCEXPORT OV_ACCESS fb_dbinfoclass_getaccess(
	OV_INSTPTR_ov_object		pobj,
	const OV_ELEMENT			*pelem,
	const OV_TICKET				*pticket
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_fb_dbinfoclass	pdbi = Ov_StaticPtrCast(fb_dbinfoclass, pobj);

	/*
	*	switch based on the element's type
	*/
	switch(pelem->elemtype) {
		case OV_ET_VARIABLE:
			if(pelem->elemunion.pvar->v_offset >= offsetof(OV_INST_ov_object,__classinfo)) {

		        if(IsFlagSet(pelem->elemunion.pvar->v_flags, 'i')) {
					return OV_AC_READWRITE;
				}
				return OV_AC_READ;
			}
			break;
		case OV_ET_OBJECT:
			/* Gibt es schon FB-Instanzen? */
			if( pdbi->v_instnumber || pdbi->v_tasknumber || pdbi->v_connnumber ) {
			    return OV_AC_READWRITE;
			}
			return (OV_AC_READWRITE | OV_AC_DELETEABLE);
		default:
			break;
	}
	return ov_object_getaccess(pobj, pelem, pticket);
}


/*	----------------------------------------------------------------------	*/
OV_DLLFNCEXPORT OV_STRING fb_dbinfoclass_licinfo_get(
    OV_INSTPTR_fb_dbinfoclass pobj
) {

	OV_INSTPTR_fb_dbinfoclass	pdbi = Ov_StaticPtrCast(fb_dbinfoclass, pobj);
	if(!ov_string_compare(pdbi->v_licinfo, "") ) {
	    fb_setlicinfo(NULL, &pdbi->v_licinfo, &pdbi->v_version);
	}
	return pdbi->v_licinfo;

}

/*	----------------------------------------------------------------------	*/
OV_DLLFNCEXPORT OV_STRING fb_dbinfoclass_version_get(
    OV_INSTPTR_fb_dbinfoclass pobj
) {

	OV_INSTPTR_fb_dbinfoclass	pdbi = Ov_StaticPtrCast(fb_dbinfoclass, pobj);
	if(!ov_string_compare(pdbi->v_version, "") ) {
	    fb_setlicinfo(NULL, &pdbi->v_licinfo, &pdbi->v_version);
	}
	return pdbi->v_version;

}

/*	----------------------------------------------------------------------	*/
OV_DLLFNCEXPORT OV_INT fb_dbinfoclass_flush_get(
	OV_INSTPTR_fb_dbinfoclass	pdbi
) {
	return pdbi->v_flush;
}
OV_DLLFNCEXPORT OV_RESULT fb_dbinfoclass_flush_set(
	OV_INSTPTR_fb_dbinfoclass	pdbi,
	const OV_INT				value
) {
    OV_ANY			pvarcurrprops;
    OV_RESULT       res;
    char            help[256];
    static char     dbsav[256];
    char            *ph;
    
	if(!value) {
	    return OV_ERR_OK;
	}
	
	/* Datenbasis sichern */
	fb_database_flush();
	
	res = ov_vendortree_getdatabasename(&pvarcurrprops, NULL);
	if(res != OV_ERR_OK) {
	    return res;
	}
	strcpy(help, pvarcurrprops.value.valueunion.val_string);
	ph = help;
	while(ph && (*ph)) ph++;
	while( (ph != help) && (*ph != '.')) ph--;
	if(*ph != '.') {
        return OV_ERR_BADPATH;
    }
	*ph = '\0';

	sprintf(dbsav, "%s%s", help, DB_FILEBAC_EX);

	return ov_database_write(dbsav);
}

/*	----------------------------------------------------------------------	*/
OV_DLLFNCEXPORT OV_STRING fb_dbinfoclass_ticket_get(
    OV_INSTPTR_fb_dbinfoclass pobj
) {
    static char lock[] = "locked";
    static char none[] = "none";

    if( ov_string_compare(pdb->serverpassword, "") ) {
        return lock;
    }
    return none;
}

/*	----------------------------------------------------------------------	*/
OV_DLLFNCEXPORT OV_RESULT fb_dbinfoclass_ticket_set(
	OV_INSTPTR_fb_dbinfoclass	pobj,
	const OV_STRING				value
) {
    ov_vendortree_setserverpassword(value);
    if(value && (!pdb->serverpassword)) {
        return OV_ERR_DBOUTOFMEMORY;
    }
    return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/
OV_DLLFNCEXPORT OV_RESULT fb_dbinfoclass_command_set(
	OV_INSTPTR_fb_dbinfoclass	pdbi,
	const OV_STRING				value
) {
    /* Lokale Variablen */
    OV_RESULT res = OV_ERR_OK;
    
#if 0
    char      help[512];

    do {
        if(!ov_string_compare(value, "p1w")) {
            /* Passwort */
            if(!ov_string_compare(pdb->serverpassword, "")) {
                ov_string_setvalue(&pdbi->v_command, NULL);
                break;
            }
            lts_encode_string(help, (char*)(pdb->serverpassword));
            help[511] = '\0';
            if(ov_string_getlength(help) < 4) {
                res = OV_ERR_GENERIC;
                break;
            }
            res = ov_string_setvalue(&pdbi->v_command, help);
            
            break;
        }
        
        /* Commando nicht definiert */
        res = OV_ERR_BADVALUE;
        
    } while(1==0); /* Nur 1 Mal durchlaufen */
#endif

    return res;
}

/*	----------------------------------------------------------------------	*/
OV_DLLFNCEXPORT OV_STRING fb_dbinfoclass_command_get(
    OV_INSTPTR_fb_dbinfoclass pdbi
) {
	/*
	*	local variables
	*/
	static char  help[512];
	
	if(!ov_string_compare(pdbi->v_command, NULL)) {
	   help[0] = '\0';
	} else {
        strncpy(help, pdbi->v_command, 511);
    }
    
    /* Alten Ergebnis loeschen */
    ov_string_setvalue(&pdbi->v_command, NULL);
    
	return help;
}

/*	----------------------------------------------------------------------	*/
/*
*	Set licence infos
*   -----------------
*/
OV_DLLFNCEXPORT void fb_setlicinfo(
     char*      filename
    ,OV_STRING* licinfo
    ,OV_STRING* version
) {
}

/*	----------------------------------------------------------------------	*/
/*
*	Get FB start state
*   ------------------
*/
int fb_started() {
    return 1;
}

/*
*	End of file
*/
