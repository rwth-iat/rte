/*
 *   $Id: ov_vendortree.c,v 1.22 2010-12-20 13:23:06 martin Exp $
 *
 *   Copyright (C) 1998-1999
 *   Lehrstuhl fuer Prozessleittechnik,
 *   RWTH Aachen, D-52056 Aachen, Germany.
 *   All rights reserved.
 *
 *   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
 *
 *   This file is part of the ACPLT/OV Package which is licensed as open
 *   source under the Artistic License; you can use, redistribute and/or
 *   modify it under the terms of that license.
 *
 *   You should have received a copy of the Artistic License along with
 *   this Package; see the file ARTISTIC-LICENSE. If not, write to the
 *   Copyright Holder.
 *
 *   THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
 *   WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
 *   OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
/*
 *	History:
 *	--------
 *	05-May-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
 */

#define OV_COMPILE_LIBOV

#include "libov/ov_vendortree.h"
#include "libov/ov_association.h"
#include "libov/ov_string.h"
#include "libov/ov_time.h"
#include "libov/ov_database.h"
#include "libov/ov_macros.h"
#include "libov/ov_path.h"

#if OV_SYSTEM_UNIX
#include <sys/types.h>
#include <unistd.h>
#endif

#if OV_SYSTEM_NT
#include <winbase.h>
#endif

/*	----------------------------------------------------------------------	*/

/*
 *	Global variables
 */
static OV_STRING	databasename = NULL;
static OV_STRING	vendorname = NULL;
static OV_STRING	serverdescription = NULL;
static OV_STRING	servername = NULL;
static OV_STRING	serverversion = NULL;
static OV_TIME		startuptime;
static OV_STRING	semantic_flag[32] = {
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};
static OV_STRING	cmdlineoptions = NULL;
static OV_STRING	ksVersion = NULL;
static OV_STRING	emptyStr = "";
static OV_UINT		ks_maxItemsPerRequest = 0;
static OV_UINT		maxStringLength = 0;
static OV_UINT		maxVectorLength = 0;
static OV_UINT		maxNameLength = 255;
static OV_UINT		maxHierarchyDepth = 64;
static OV_UINT		ov_scheduler_allowedJitter = 0;
static OV_UINT		ov_scheduler_numberOfExceeds = 0;
static OV_UINT		ov_serverPID = 0;



OV_DLLVAREXPORT OV_BOOL ov_activitylock;
OV_DLLVAREXPORT OV_BOOL ov_backup;
OV_DLLVAREXPORT OV_BOOL ov_explain;
OV_DLLVAREXPORT OV_STRING db_backup_filename;
OV_DLLVAREXPORT OV_BOOL ov_server_run = TRUE;

/*
 *	Global variables
 *
 *	OV_STRING			identifier;
 *	OV_STRING			unit;
 *	OV_FNC_GETVENDORVAR	*getvarfnc;
 *	OV_FNC_SETVENDORVAR	*setvarfnc;
 *
 */
OV_DLLVAREXPORT OV_VENDORTREE_INFO vendorinfo[OV_NUM_VENDOROBJECTS] = {
		{ "libraries",				NULL,	ov_vendortree_getlibraries, NULL },
		{ "classes",				NULL,	ov_vendortree_getclasses, NULL },
		{ "associations",			NULL,	ov_vendortree_getassociations, NULL },
		{ "instanceCount",			NULL,	ov_vendortree_getInstanceCount, NULL },
		{ "database_fragmentation",	"%",	ov_vendortree_getdatabasefrag, NULL },
		{ "database_free",			"Byte",	ov_vendortree_getdatabasefree, NULL },
		{ "database_name",			NULL,	ov_vendortree_getdatabasename, NULL },
		{ "database_size",			"Byte",	ov_vendortree_getdatabasesize, NULL },
		{ "database_started",		NULL,	ov_vendortree_getdatabasestarted, NULL },
		{ "database_used",			"Byte",	ov_vendortree_getdatabaseused, NULL },
		{ "name", 					NULL,	ov_vendortree_getname, NULL },
		{ "ks_version",				NULL,	ov_vendortree_getksversion, NULL },
		{ "libov_version",			NULL,	ov_vendortree_getlibovversion, NULL },
		{ "semantic_flags",			NULL,	ov_vendortree_getsemanticflags, NULL },
		{ "server_description",		NULL,	ov_vendortree_getserverdescription, NULL },
		{ "server_name",			NULL,	ov_vendortree_getservername, NULL },
		{ "server_time",			"UTC",	ov_vendortree_getservertime, NULL },
		{ "server_version",			NULL,	ov_vendortree_getserverversion, NULL },
		{ "server_configuration",	NULL,	ov_vendortree_getserverconfiguration, ov_vendortree_setserverconfiguration },
		{ "server_run",				NULL,	ov_vendortree_getserverrun, ov_vendortree_setserverrun },
		{ "server_password",		NULL,	ov_vendortree_getserverpassword, ov_vendortree_setserverpassword_ext },
		{ "serverPID",				NULL,	ov_vendortree_getserverPID, NULL },
		{ "startup_time",			"UTC",	ov_vendortree_getstartuptime, NULL },
		{ "running_db_backup",		NULL,	ov_vendortree_getbackup, NULL },
		{ "write_db_backup",		NULL,	ov_vendortree_writebackup, NULL },
		{ "ov_time_offset",			NULL,	ov_vendortree_gettimeoffset, ov_vendortree_settimeoffset },
		{ "ks_maxitemsperrequest",	NULL,	ov_vendortree_getKsMaxItems, ov_vendortree_setKsMaxItems },
		{ "maxstringlength",		NULL,	ov_vendortree_getMaxStringLength, ov_vendortree_setMaxStringLength },
		{ "maxvectorlength",		NULL,	ov_vendortree_getMaxVectorLength, ov_vendortree_setMaxVectorLength },
		{ "maxnamelength",			NULL,	ov_vendortree_getMaxNameLength, ov_vendortree_setMaxNameLength },
		{ "maxhierarchydepth",		NULL,	ov_vendortree_getMaxHierarchyDepth, ov_vendortree_setMaxHierarchyDepth },
		{ "ov_scheduler_allowedjitter",	"usecs",	ov_vendortree_getAllowedJitter, ov_vendortree_setAllowedJitter },
		{ "ov_scheduler_numexceeds",	NULL,	ov_vendortree_getNumExceeds, NULL },
		{ "cmdline_options",		NULL,	ov_vendortree_getcmdlineoptions, NULL }
};

/*	----------------------------------------------------------------------	*/


/**
 *	Initialize the vendor tree
 */
OV_RESULT ov_vendortree_init(void) {
	/*
	 *	local variables
	 */
	OV_UINT	i;
	/*
	 *	initialize the vendor domain object
	 */
	ov_string_setvalue(&pdb->vendordom.v_identifier, "vendor");
	ov_time_gettime(&pdb->vendordom.v_creationtime);
	pdb->vendordom.v_pouterobject = NULL;
	pdb->vendordom.v_objectstate = OV_OS_INIT;
	pdb->vendordom.v_linktable = ov_database_malloc(pclass_ov_domain->v_linktablesize);
	if(!pdb->vendordom.v_linktable) {
		return OV_ERR_DBOUTOFMEMORY;
	}
	pdb->vendordom.v_idH = 0;
	pdb->vendordom.v_idL = 1;
	
	memset(pdb->vendordom.v_linktable, 0, pclass_ov_domain->v_linktablesize);
	DoLink(ov_containment, &pdb->root, &pdb->vendordom);
	DoLink(ov_instantiation, pclass_ov_domain, &pdb->vendordom);
	/*
	 *	initialize the vendor objects
	 */
	for(i=0; i<OV_NUM_VENDOROBJECTS; i++) {
		ov_string_setvalue(&pdb->vendorobj[i].v_identifier, vendorinfo[i].identifier);
		ov_time_gettime(&pdb->vendorobj[i].v_creationtime);
		pdb->vendorobj[i].v_pouterobject = NULL;
		pdb->vendorobj[i].v_objectstate = OV_OS_INIT;
		pdb->vendorobj[i].v_linktable = ov_database_malloc(pclass_ov_object->v_linktablesize);
		if(!pdb->vendorobj[i].v_linktable) {
			return OV_ERR_DBOUTOFMEMORY;
		}
		memset(pdb->vendorobj[i].v_linktable, 0, pclass_ov_object->v_linktablesize);
		DoLink(ov_containment, &pdb->vendordom, &pdb->vendorobj[i]);
		DoLink(ov_instantiation, pclass_ov_object, &pdb->vendorobj[i]);
	}
	return OV_ERR_OK;
}
/**
 * frees the heap memory used by the vendor tree variables
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_free(void) {
	//it is save to pass a NULL pointer
	Ov_HeapFree(databasename);
	databasename = NULL;
	Ov_HeapFree(vendorname);
	vendorname = NULL;
	Ov_HeapFree(serverdescription);
	serverdescription = NULL;
	Ov_HeapFree(servername);
	servername = NULL;
	Ov_HeapFree(serverversion);
	serverversion = NULL;
	Ov_HeapFree(ksVersion);
	ksVersion = NULL;
	Ov_HeapFree(cmdlineoptions);
	cmdlineoptions = NULL;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/**
 *	Get unit of a vendor object
 */
OV_DLLFNCEXPORT OV_STRING ov_vendortree_getunit(
	OV_INSTPTR_ov_object	pobj
) {
	/*
	 *	local variables
	 */
	OV_UINT	i;
	/*
	 *	get number of the vendor object
	 */
	i = pobj-&pdb->vendorobj[0];
	if(i >= OV_NUM_VENDOROBJECTS) {
		return NULL;
	}
	/*
	 *	return unit
	 */
	return vendorinfo[i].unit;
}

/*	----------------------------------------------------------------------	*/

/**
 *	Get variable of a vendor object
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getvar(
	OV_INSTPTR_ov_object	pobj,
	OV_ANY			*pvarcurrprops,
	const OV_TICKET		*pticket
) {
	/*
	 *	local variables
	 */
	OV_UINT	i;
	/*
	 *	get number of the vendor object
	 */
	i = pobj-pdb->vendorobj;
	if(i >= OV_NUM_VENDOROBJECTS) {
		return OV_ERR_GENERIC;
	}
	/*
	 *	set time and state and get value
	 */
	ov_time_gettime(&pvarcurrprops->time);
	pvarcurrprops->state = OV_ST_GOOD;
	return vendorinfo[i].getvarfnc(pvarcurrprops, pticket);
}

/*	----------------------------------------------------------------------	*/

/* 
 *	Set variable of a vendor object
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_setvar(
	OV_INSTPTR_ov_object	pobj,
	const OV_ANY		*pvarcurrprops,
	const OV_TICKET		*pticket
) {
	/*
	 *	local variables
	 */
	OV_UINT	i;
	/*
	 *	get number of the vendor object
	 */
	i = pobj-pdb->vendorobj;
	if(i >= OV_NUM_VENDOROBJECTS) {
		return OV_ERR_GENERIC;
	}
	/*
	 *	call the setfnc of the vendor object
	 */
	if (vendorinfo[i].setvarfnc) 
		return vendorinfo[i].setvarfnc(pvarcurrprops, pticket);
	else return OV_ERR_NOACCESS;
}

/*	----------------------------------------------------------------------	*/

/**
 *	Set database name
 */
OV_DLLFNCEXPORT void ov_vendortree_setdatabasename(
	OV_STRING	name
) {
	if(databasename) {
		Ov_HeapFree(databasename);
		databasename = NULL;
	}
	if(name) {
		databasename = (OV_STRING)Ov_HeapMalloc(strlen(name)+1);
		strcpy(databasename, name);
	}
}

/*	----------------------------------------------------------------------	*/

/**
 *	Set vendor name
 */
OV_DLLFNCEXPORT void ov_vendortree_setname(
	OV_STRING	name
) {
	if(vendorname) {
		Ov_HeapFree(vendorname);
		vendorname = NULL;
	}
	if(name) {
		vendorname = (OV_STRING)Ov_HeapMalloc(strlen(name)+1);
		strcpy(vendorname, name);
	}
}

/*	----------------------------------------------------------------------	*/

/**
 *	Set semantic flag
 */
OV_DLLFNCEXPORT void ov_vendortree_setsemanticflag(
	OV_UINT		flagnum,
	OV_STRING	flagvalue
) {
	if(flagnum < sizeof(semantic_flag)/sizeof(semantic_flag[0])) {
		if(semantic_flag[flagnum]) {
			Ov_HeapFree(semantic_flag[flagnum]);
			semantic_flag[flagnum] = NULL;
		}
		if(flagvalue) {
			semantic_flag[flagnum] = (OV_STRING)Ov_HeapMalloc(strlen(flagvalue)+1);
			strcpy(semantic_flag[flagnum], flagvalue);
		}
	}
}

/*	----------------------------------------------------------------------	*/

/**
 *	Set server description
 */
OV_DLLFNCEXPORT void ov_vendortree_setserverdescription(
	OV_STRING	name
) {
	if(serverdescription) {
		Ov_HeapFree(serverdescription);
		serverdescription = NULL;
	}
	if(name) {
		serverdescription = (OV_STRING)Ov_HeapMalloc(strlen(name)+1);
		strcpy(serverdescription, name);
	}
}

/*	----------------------------------------------------------------------	*/

/**
 *	Set server name
 */
OV_DLLFNCEXPORT void ov_vendortree_setservername(
	OV_STRING	name
) {
	if(servername) {
		Ov_HeapFree(servername);
		servername = NULL;
	}
	if(name) {
		servername = (OV_STRING)Ov_HeapMalloc(strlen(name)+1);
		strcpy(servername, name);
	}
}

/*	----------------------------------------------------------------------	*/

/**
 *	Set server version
 */
OV_DLLFNCEXPORT void ov_vendortree_setserverversion(
	OV_STRING	name
) {
	if(serverversion) {
		Ov_HeapFree(serverversion);
		serverversion = NULL;
	}
	if(name) {
		serverversion = (OV_STRING)Ov_HeapMalloc(strlen(name)+1);
		strcpy(serverversion, name);
	}
}

/*	----------------------------------------------------------------------	*/

/**
 *	Set startup time
 */
OV_DLLFNCEXPORT void ov_vendortree_setstartuptime(
	OV_TIME		*ptime
) {
	if(ptime) {
		startuptime = *ptime;
	} else {
		ov_time_gettime(&startuptime);
	}
}

/*	----------------------------------------------------------------------	*/

/**
 *	Get list of associations in the database
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getassociations(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	/*
	 *	local variables
	 */
	OV_UINT						assocs = 0;
	OV_INSTPTR_ov_association	passoc;
	OV_STRING					*pstring = NULL;
	/*
	 *	count assoiations
	 */
	Ov_ForEachChildEx(ov_instantiation, pclass_ov_association, passoc, ov_association) {
		assocs++;
	}
	/*
	 *	allocate memory for the association paths
	 */
	if(assocs) {
		pstring = (OV_STRING*)ov_memstack_alloc(assocs*sizeof(OV_STRING));
		if(!pstring) {
			return OV_ERR_HEAPOUTOFMEMORY;
		}
	}
	/*
	 *	enter association paths into a string vector
	 */
	pvarcurrprops->value.vartype = OV_VT_STRING_VEC;
	pvarcurrprops->value.valueunion.val_string_vec.veclen = assocs;
	pvarcurrprops->value.valueunion.val_string_vec.value = pstring;
	Ov_ForEachChildEx(ov_instantiation, pclass_ov_association, passoc, ov_association) {
		*pstring = ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object, passoc), 2);
		if(!*pstring) {
			return OV_ERR_HEAPOUTOFMEMORY;
		}
		pstring++;
	}
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/**
 *	Get list of classes in the database
 *
 *	an object in OV is a class if it's objectclass (instanstiation) is ov_class or derived of (inheritance) ov_class
 *	or it's objectclass (instantiation) is an instance of a class whose baseclass (inheritance) is ov_class or derived of ov_class
 */
OV_RESULT ov_vendortree_getderivedclasses(
		OV_INSTPTR_ov_class pstartclass,
		OV_STRING** pstring
) {
	OV_INSTPTR_ov_class		pderivedclass;
	OV_INSTPTR_ov_class		pclass;
	OV_INSTPTR_ov_object		pclassobj;
	OV_VTBLPTR_ov_object		pvtable;
	OV_ELEMENT			element;
	OV_RESULT 			res;

	pclass = pstartclass;
	pderivedclass = Ov_GetFirstChild(ov_inheritance, pclass);
	while (pclass) {
		Ov_ForEachChild(ov_instantiation, pclass, pclassobj) {
			element.elemtype = OV_ET_OBJECT;
			element.pobj = pclassobj;
			Ov_GetVTablePtr(ov_object, pvtable, pclassobj);
			if (pvtable) { 
				if(pvtable->m_getaccess(pclassobj, &element, NULL) & OV_AC_INSTANTIABLE) {
					**pstring = ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object, pclassobj), 2);
					if(!**pstring) {
						return OV_ERR_HEAPOUTOFMEMORY;
					}
					(*pstring)++;
				}
			} 
			else {
				if(ov_object_getaccess(pclassobj, &element, NULL) & OV_AC_INSTANTIABLE) {
					**pstring = ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object, pclassobj), 2);
					if(!**pstring) {
						return OV_ERR_HEAPOUTOFMEMORY;
					}
					(*pstring)++;
				}
			}
		}
		if (Ov_GetFirstChild(ov_inheritance, pderivedclass)) {
			res = ov_vendortree_getderivedclasses(Ov_GetFirstChild(ov_inheritance, pderivedclass), pstring);
			if (Ov_Fail(res)) return res;
		}
		pclass = pderivedclass;
		pderivedclass = Ov_GetNextChild(ov_inheritance, pclass);
	}
	return OV_ERR_OK;
}

OV_UINT ov_vendortree_countderivedclasses(
		OV_INSTPTR_ov_class pstartclass,
		OV_UINT	lastresult
) {
	OV_INSTPTR_ov_class		pclass;
	OV_INSTPTR_ov_class		pderivedclass;
	OV_INSTPTR_ov_object		pclassobj;
	OV_VTBLPTR_ov_object		pvtable;
	OV_UINT classes;
	OV_ELEMENT			element;

	pclass = pstartclass;
	classes = lastresult;
	pderivedclass = Ov_GetFirstChild(ov_inheritance, pclass);
	while (pclass) {
		Ov_ForEachChild(ov_instantiation, pclass, pclassobj) {
			element.elemtype = OV_ET_OBJECT;
			element.pobj = pclassobj;
			Ov_GetVTablePtr(ov_object, pvtable, pclassobj);
			if (pvtable) { 
				if(pvtable->m_getaccess(pclassobj, &element, NULL) & OV_AC_INSTANTIABLE)
					classes++;
			}
			else {
				if(ov_object_getaccess(pclassobj, &element, NULL) & OV_AC_INSTANTIABLE)
					classes++;
			}
		}
		if (Ov_GetFirstChild(ov_inheritance, pderivedclass)) classes = ov_vendortree_countderivedclasses(Ov_GetFirstChild(ov_inheritance, pderivedclass), classes);
		pclass = pderivedclass;
		pderivedclass = Ov_GetNextChild(ov_inheritance, pclass);
	}
	return classes;
}

/**
 *	Get classes of the database
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getclasses(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	/*
	 *	local variables
	 */
	OV_UINT					classes;
	OV_STRING				*pstring = NULL;
	/*
	 *	count instantiable classes
	 */
	classes = ov_vendortree_countderivedclasses(pclass_ov_class, 0);
	/*
	 *	allocate memory for the class paths
	 */
	if(classes) {
		pstring = (OV_STRING*)ov_memstack_alloc(classes*sizeof(OV_STRING));
		if(!pstring) {
			return OV_ERR_HEAPOUTOFMEMORY;
		}
	}
	/*
	 *	enter class paths into a string vector
	 */
	pvarcurrprops->value.vartype = OV_VT_STRING_VEC;
	pvarcurrprops->value.valueunion.val_string_vec.veclen = classes;
	pvarcurrprops->value.valueunion.val_string_vec.value = pstring;

	return ov_vendortree_getderivedclasses(pclass_ov_class, &pstring);
}

/*	----------------------------------------------------------------------	*/

/**
 *	Get fragmentation of the database
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getdatabasefrag(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_UINT;
	pvarcurrprops->value.valueunion.val_uint = ov_database_getfrag();
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/**
 *	Get free storage of the database
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getdatabasefree(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_UINT;
	pvarcurrprops->value.valueunion.val_uint = ov_database_getfree();
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/**
 *	Get database name
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getdatabasename(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_STRING;
	pvarcurrprops->value.valueunion.val_string = databasename;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/**
 *	Get size of the database
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getdatabasesize(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_UINT;
	pvarcurrprops->value.valueunion.val_uint = ov_database_getsize();
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/**
 *	Get whether the database is started or not
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getdatabasestarted(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_BOOL;
	pvarcurrprops->value.valueunion.val_bool = pdb->started;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/**
 *	Get used storage of the database
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getdatabaseused(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_UINT;
	pvarcurrprops->value.valueunion.val_uint = ov_database_getused();
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/**
 *	Get vendor name
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getname(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_STRING;
	pvarcurrprops->value.valueunion.val_string = vendorname;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/**
 *	Get ks version
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getksversion(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_STRING;
	if(ksVersion)
		pvarcurrprops->value.valueunion.val_string = ksVersion;
	else
		pvarcurrprops->value.valueunion.val_string = emptyStr;

	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/**
 *	Set ks version
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_setksversion(
		OV_STRING	version,
		const OV_TICKET	*pticket
) {
	if(ksVersion) {
		Ov_HeapFree(ksVersion);
		ksVersion = NULL;
	}
	if(version) {
		ksVersion = (OV_STRING)Ov_HeapMalloc(strlen(version)+1);
		if(ksVersion) {
			strcpy(ksVersion, version);
		}
	}
	return OV_ERR_OK;
}


/*	----------------------------------------------------------------------	*/

/**
 *	Get libov version
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getlibovversion(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_STRING;
	pvarcurrprops->value.valueunion.val_string = OV_VER_LIBOV;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/


/**
 *	Get list of libraries in the database
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getlibraries(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	/*
	 *	local variables
	 */
	OV_UINT					libs = 0;
	OV_INSTPTR_ov_library	plib;
	OV_INSTPTR_ov_class	pclass;
	OV_STRING				*pstring = NULL;
	/*
	 *	count associations
	 */
	Ov_ForEachChildEx(ov_instantiation, pclass_ov_library, plib, ov_library) {
		libs++;
	}
	Ov_ForEachChild(ov_inheritance, pclass_ov_library, pclass) {
		Ov_ForEachChildEx(ov_instantiation, pclass, plib, ov_library) {
			libs++;
		}
	}
	/*
	 *	allocate memory for the library paths
	 */
	if(libs) {
		pstring = (OV_STRING*)ov_memstack_alloc(libs*sizeof(OV_STRING));
		if(!pstring) {
			return OV_ERR_HEAPOUTOFMEMORY;
		}
	}
	/*
	 *	enter library paths into a string vector
	 */
	pvarcurrprops->value.vartype = OV_VT_STRING_VEC;
	pvarcurrprops->value.valueunion.val_string_vec.veclen = libs;
	pvarcurrprops->value.valueunion.val_string_vec.value = pstring;
	Ov_ForEachChildEx(ov_instantiation, pclass_ov_library, plib, ov_library) {
		*pstring = ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object, plib), 2);
		if(!*pstring) {
			return OV_ERR_HEAPOUTOFMEMORY;
		}
		pstring++;
	}
	Ov_ForEachChild(ov_inheritance, pclass_ov_library, pclass) {
		Ov_ForEachChildEx(ov_instantiation, pclass, plib, ov_library) {
			*pstring = ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object, plib), 2);
			if(!*pstring) {
				return OV_ERR_HEAPOUTOFMEMORY;
			}
			pstring++;
		}
	}
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/**
 *	Get list of semantic flags in the database
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getsemanticflags(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_STRING_VEC;
	pvarcurrprops->value.valueunion.val_string_vec.veclen = sizeof(semantic_flag)/sizeof(semantic_flag[0]);
	pvarcurrprops->value.valueunion.val_string_vec.value = &semantic_flag[0];
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/**
 *	Get server description
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getserverdescription(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_STRING;
	pvarcurrprops->value.valueunion.val_string = serverdescription;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/**
 *	Get server name
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getservername(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_STRING;
	pvarcurrprops->value.valueunion.val_string = servername;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/**
 *	Get server time
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getservertime(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_TIME;
	ov_time_gettime(&pvarcurrprops->value.valueunion.val_time);
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/**
 *	Get server version
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getserverversion(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_STRING;
	pvarcurrprops->value.valueunion.val_string = serverversion;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/**
 *	Get startup time
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getstartuptime(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_TIME;
	pvarcurrprops->value.valueunion.val_time = startuptime;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/**
 *	Get number of instances in database
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getInstanceCount(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	if(!pvarcurrprops){
		return OV_ERR_BADPARAM;
	}
	
	pvarcurrprops->value.vartype = OV_VT_UINT;
	pvarcurrprops->value.valueunion.val_uint = (OV_UINT) pdb->idList->relationCount;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT ov_vendortree_getInstanceCountUINT() {
	return (OV_UINT) pdb->idList->relationCount;
}


/**
 *	Get PID of server
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getserverPID(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	if(!pvarcurrprops){
		return OV_ERR_BADPARAM;
	}
	
	pvarcurrprops->value.vartype = OV_VT_UINT;
	pvarcurrprops->value.valueunion.val_uint = ov_serverPID;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT ov_vendortree_getserverPIDUINT() {
	return ov_serverPID;
}

OV_DLLFNCEXPORT void ov_vendortree_setServerPID(){
#if OV_SYSTEM_UNIX
	ov_serverPID = getpid();
#endif
#if OV_SYSTEM_NT
	ov_serverPID = GetCurrentProcessId();
#endif
}


/*	----------------------------------------------------------------------	*/

/**
 *	Get server configuration
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getserverconfiguration(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_BOOL_VEC;
	pvarcurrprops->value.valueunion.val_bool_vec.veclen = OV_NUM_SERVER_CONFIG;
	pvarcurrprops->value.valueunion.val_bool_vec.value = ov_memstack_alloc(OV_NUM_SERVER_CONFIG*sizeof(OV_BOOL));
	pvarcurrprops->value.valueunion.val_bool_vec.value[0] = ov_activitylock;
	pvarcurrprops->value.valueunion.val_bool_vec.value[1] = ov_explain;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/**
 *	Set server configuration
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_setserverconfiguration(
	const OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	if (pdb->serverpassword) {
		if (pticket->type == OV_TT_SIMPLE) {
			if(!strcmp(pticket->ticketunion.simpleticket.id, pdb->serverpassword)) goto CONTINUE1;
		}
		return OV_ERR_NOACCESS;
	}
	CONTINUE1:
	if (pvarcurrprops->value.vartype == OV_VT_BOOL_VEC) {
		if (pvarcurrprops->value.valueunion.val_bool_vec.veclen == OV_NUM_SERVER_CONFIG) {
			ov_activitylock = pvarcurrprops->value.valueunion.val_bool_vec.value[0];
			ov_explain = pvarcurrprops->value.valueunion.val_bool_vec.value[1];
			return OV_ERR_OK;
		}
	}
	return OV_ERR_BADTYPE;
}

/*	----------------------------------------------------------------------	*/

/**
 *	Get backup
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getbackup(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_BOOL;
	pvarcurrprops->value.valueunion.val_bool = ov_backup;
	return OV_ERR_OK;
}
/*	----------------------------------------------------------------------	*/

/**
 *	Write backup file
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_writebackup(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_BOOL;
	pvarcurrprops->value.valueunion.val_bool = Ov_OK(ov_database_write(db_backup_filename));
	return OV_ERR_OK;
}/*	----------------------------------------------------------------------	*/

/**
 *	Set serverpassword external
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_setserverpassword_ext(
	const OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	if (ov_string_compare(pdb->serverpassword,NULL)) {
		if (pticket->type == OV_TT_SIMPLE) {
			if(!ov_string_compare(pticket->ticketunion.simpleticket.id, pdb->serverpassword)) goto CONTINUE1;
		}
		return OV_ERR_NOACCESS;
	}
	CONTINUE1:
	if (pvarcurrprops->value.vartype == OV_VT_STRING) {
		if (pdb->serverpassword) {
			ov_database_free(pdb->serverpassword);
			pdb->serverpassword = NULL;
		}
		if (ov_string_compare(pvarcurrprops->value.valueunion.val_string, NULL)) {
			pdb->serverpassword = (OV_STRING)ov_database_malloc(ov_string_getlength(pvarcurrprops->value.valueunion.val_string)+1);
			if (!pdb->serverpassword) {
				return OV_ERR_DBOUTOFMEMORY;
			}
			strcpy(pdb->serverpassword, pvarcurrprops->value.valueunion.val_string);
		}
		return OV_ERR_OK;
	}
	return OV_ERR_BADTYPE;
}

/*	----------------------------------------------------------------------	*/

/**
 *	Set serverpassword
 */
OV_DLLFNCEXPORT void ov_vendortree_setserverpassword(
	OV_STRING	password
) {
	if(pdb->serverpassword) {
		ov_database_free(pdb->serverpassword);
		pdb->serverpassword = NULL;
	}
	if(password) {
		pdb->serverpassword = (OV_STRING)ov_database_malloc(strlen(password)+1);
		if(pdb->serverpassword) {
			strcpy(pdb->serverpassword, password);
		}
	}
}

/*	----------------------------------------------------------------------	*/

/**
 *	Get serverpassword
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getserverpassword(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_STRING;
	if (ov_string_compare(pdb->serverpassword,NULL)) {
		if (pticket->type == OV_TT_SIMPLE) {
			if(!ov_string_compare(pticket->ticketunion.simpleticket.id, pdb->serverpassword)) goto CONTINUE1;
		}
		return OV_ERR_NOACCESS;
	}
	CONTINUE1:
	pvarcurrprops->value.valueunion.val_string = pdb->serverpassword;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/**
 *	Get timeoffset
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_gettimeoffset(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_TIME_SPAN;
	pvarcurrprops->value.valueunion.val_time_span = pdb->timeoffset;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/**
 *	Set timeoffset
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_settimeoffset(
	const OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	if (pvarcurrprops->value.vartype == OV_VT_TIME_SPAN) {
		pdb->timeoffset = pvarcurrprops->value.valueunion.val_time_span;
		return OV_ERR_OK;
	}
	return OV_ERR_BADTYPE;
}

/*	----------------------------------------------------------------------	*/

/**
 *	Get commandline options
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getcmdlineoptions(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_STRING;
	pvarcurrprops->value.valueunion.val_string = cmdlineoptions;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/**
 *	Set commandline options
 */
OV_DLLFNCEXPORT void ov_vendortree_setcmdlineoptions(
	OV_STRING	options
) {
	if(cmdlineoptions) {
		Ov_HeapFree(cmdlineoptions);
		cmdlineoptions = NULL;
	}
	if(options) {
		cmdlineoptions = (OV_STRING)Ov_HeapMalloc(strlen(options)+1);
		if(cmdlineoptions) {
			strcpy(cmdlineoptions, options);
		}
	}
}

/*	----------------------------------------------------------------------	*/

/**
 *	Get the value of the specified option
 *	returns a NULL-pointer if option was not found
 *	returns an empty string if option has no value
 *	otherwise returns the value of the specified option
 *	This functions uses the memstack, so call ov_memstack_lock() / ov_memstack_unlock() around
 *	If no memory can be allocated a NULL-pointer is returned
 */

OV_DLLFNCEXPORT OV_STRING ov_vendortree_getcmdlineoption_value(
	OV_STRING option
) {
	OV_STRING begin_option = NULL;	//points to the beginning of the option in cmdlineoptions
	OV_STRING space_ptr = NULL;		//points to the first space after the option in cmdlineoptions
	OV_STRING equ_ptr = NULL;		//points to the first '=' after the option in cmdlineoptions
	OV_STRING value = NULL;

	//check if there are command line options saved
	if(!cmdlineoptions)
		return (OV_STRING) NULL;

	begin_option = strstr(cmdlineoptions, option);

	//options not found?
	if(!begin_option)
		return (OV_STRING) NULL;

	equ_ptr = strchr(begin_option, (int) '=');
	//no "=" found --> empty value
	if(!equ_ptr)
	{
		value = ov_memstack_alloc(1);
		if(value)
		{
			*value = '\0';
			return value;
		}
		else
			return (OV_STRING) NULL;
	}

	space_ptr = strchr(begin_option, (int) ' ');
	//seems to be the last option
	if(!space_ptr)
	{
		equ_ptr++;	//point to character after '='
		value = ov_memstack_alloc(strlen(equ_ptr) + 1);
		if(value)
		{
			strcpy(value, equ_ptr);
			return value;
		}
		else
			return (OV_STRING) NULL;
	}
	else
	{
		if(equ_ptr > space_ptr)
		{	// '=' after next ' ' --> return empty value
			value = ov_memstack_alloc(1);
			if(value)
			{
				*value = '\0';
				return value;
			}
			else
				return (OV_STRING) NULL;
		}
		else
		{
			equ_ptr++; //point to character after '='
			value = ov_memstack_alloc((space_ptr - equ_ptr) + 1);	//allocate memory for value
			if(value)
			{
				strncpy(value, equ_ptr, (space_ptr - equ_ptr));
				value[(space_ptr - equ_ptr)] = '\0';
				return value;
			}
			else
				return (OV_STRING) NULL;
		}
	}
}

/*	----------------------------------------------------------------------	*/

/**
 *	Get the PORT number out of the cmdlineoptions string if it is set
 *	returns -1 if PORT is not set
 *	returns port number on success
 */

OV_DLLFNCEXPORT OV_INT ov_vendortree_getport()
{
	OV_STRING begin_option = NULL;	//points to the beginning of "PORT" in cmdlineoptions
	OV_STRING space_ptr = NULL;		//points to the first space after the option in cmdlineoptions
	OV_STRING equ_ptr = NULL;		//points to the first '=' after the option in cmdlineoptions

	//check if there are command line options saved
	if(!cmdlineoptions)
		return -1;

	begin_option = strstr(cmdlineoptions, "PORT");

	//PORT not set?
	if(!begin_option)
		return -1;

	equ_ptr = strchr(begin_option, (int) '=');
	//no "=" found --> empty value
	if(!equ_ptr)
		return -1;

	space_ptr = strchr(begin_option, (int) ' ');

	//seems to be the last option
	if(!space_ptr)
	{
		equ_ptr++;	//point to character after '='
		return(atoi(equ_ptr));	//return port number
	}
	else
	{
		if(equ_ptr > space_ptr)
		{	// '=' after next ' ' --> return empty value
			return -1;
		}
		else
		{
			equ_ptr++; //point to character after '='
			return(atoi(equ_ptr));	//return port number
		}
	}

}
/*-----------------------------------------------------------------*/
/**
 * Get the running flag
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getserverrun(
		OV_ANY			*pvarcurrprops,
		const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_BOOL;
	pvarcurrprops->value.valueunion.val_bool = ov_server_run;
	return OV_ERR_OK;
}

/*
 *	Set the running flag
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_setserverrun(
		const OV_ANY			*pvarcurrprops,
		const OV_TICKET	*pticket
) {//TODO:ticketing
	if (pvarcurrprops->value.vartype == OV_VT_BOOL) {
		ov_server_run = pvarcurrprops->value.valueunion.val_bool;
		return OV_ERR_OK;
	}
	return OV_ERR_BADTYPE;
}

/*-----------------------------------------------------------------*/
/*
 * Get ks_maxItemsPerRequest
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getKsMaxItems(
		OV_ANY			*pvarcurrprops,
		const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_UINT;
	pvarcurrprops->value.valueunion.val_uint = ks_maxItemsPerRequest;
	return OV_ERR_OK;
}

/**
 *	Get ks_maxItemsPerRequest
 */
OV_DLLFNCEXPORT OV_UINT ov_vendortree_KsMaxItems() {
	return ks_maxItemsPerRequest;
}

/**
 *	Set ks_maxItemsPerRequest
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_setKsMaxItems(
		const OV_ANY			*pvarcurrprops,
		const OV_TICKET	*pticket
) {//TODO:ticketing
	if (pvarcurrprops->value.vartype == OV_VT_UINT) {
		ks_maxItemsPerRequest = pvarcurrprops->value.valueunion.val_uint;
		return OV_ERR_OK;
	}
	return OV_ERR_BADTYPE;
}

/*-----------------------------------------------------------------*/
/*
 * Get maxstringlength
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getMaxStringLength(
		OV_ANY			*pvarcurrprops,
		const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_UINT;
	pvarcurrprops->value.valueunion.val_uint = maxStringLength;
	return OV_ERR_OK;
}

/**
 * Get maxstringlength as UINT
 */
OV_DLLFNCEXPORT OV_UINT ov_vendortree_MaxStringLength() {
	return maxStringLength;
}

/**
 *	Set maxstringlength
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_setMaxStringLength(
		const OV_ANY			*pvarcurrprops,
		const OV_TICKET	*pticket
) {//TODO:ticketing
	if (pvarcurrprops->value.vartype == OV_VT_UINT) {
		maxStringLength = pvarcurrprops->value.valueunion.val_uint;
		return OV_ERR_OK;
	}
	return OV_ERR_BADTYPE;
}
/*-----------------------------------------------------------------*/
/*
 * Get maxvectorlength
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getMaxVectorLength(
		OV_ANY			*pvarcurrprops,
		const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_UINT;
	pvarcurrprops->value.valueunion.val_uint = maxVectorLength;
	return OV_ERR_OK;
}

/**
 *	Get maxvectorlength as UINT
 */
OV_DLLFNCEXPORT OV_UINT ov_vendortree_MaxVectorLength() {
	return maxVectorLength;
}

/**
 *	Set maxvectorlength
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_setMaxVectorLength(
		const OV_ANY			*pvarcurrprops,
		const OV_TICKET	*pticket
) {//TODO:ticketing
	if (pvarcurrprops->value.vartype == OV_VT_UINT) {
		maxVectorLength = pvarcurrprops->value.valueunion.val_uint;
		return OV_ERR_OK;
	}
	return OV_ERR_BADTYPE;
}

/*-----------------------------------------------------------------*/
/*
 * Get maxnamelength
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getMaxNameLength(
		OV_ANY			*pvarcurrprops,
		const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_UINT;
	pvarcurrprops->value.valueunion.val_uint = maxNameLength;
	return OV_ERR_OK;
}

/**
 *	Get maxnamelength as UINT
 */
OV_DLLFNCEXPORT OV_UINT ov_vendortree_MaxNameLength() {
	return maxNameLength;
}

/**
 *	Set maxnamelength
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_setMaxNameLength(
		const OV_ANY			*pvarcurrprops,
		const OV_TICKET	*pticket
) {//TODO:ticketing
	if (pvarcurrprops->value.vartype == OV_VT_UINT) {
		maxNameLength = pvarcurrprops->value.valueunion.val_uint;
		return OV_ERR_OK;
	}
	return OV_ERR_BADTYPE;
}

/*-----------------------------------------------------------------*/
/*
 * Get maxhierarchydepth
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getMaxHierarchyDepth(
		OV_ANY			*pvarcurrprops,
		const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_UINT;
	pvarcurrprops->value.valueunion.val_uint = maxHierarchyDepth;
	return OV_ERR_OK;
}

/**
 *	Get maxhierarchydepth as UINT
 */
OV_DLLFNCEXPORT OV_UINT ov_vendortree_MaxHierarchyDepth() {
	return maxHierarchyDepth;
}

/**
 *	Set maxhierarchydepth
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_setMaxHierarchyDepth(
		const OV_ANY			*pvarcurrprops,
		const OV_TICKET	*pticket
) {//TODO:ticketing
	if (pvarcurrprops->value.vartype == OV_VT_UINT) {
		maxHierarchyDepth = pvarcurrprops->value.valueunion.val_uint;
		return OV_ERR_OK;
	}
	return OV_ERR_BADTYPE;
}

/*-----------------------------------------------------------------*/
/**
 * Get ov_scheduler_allowedjitter
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getAllowedJitter(
		OV_ANY			*pvarcurrprops,
		const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_UINT;
	pvarcurrprops->value.valueunion.val_uint = ov_scheduler_allowedJitter;
	return OV_ERR_OK;
}

/**
 * Get ov_scheduler_allowedjitter as UINT
 */
OV_DLLFNCEXPORT OV_UINT ov_vendortree_schedulerAllowedJitter() {
	return ov_scheduler_allowedJitter;
}

/*
 *	Set ov_scheduler_allowedjitter
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_setAllowedJitter(
		const OV_ANY			*pvarcurrprops,
		const OV_TICKET	*pticket
) {//TODO:ticketing
	if (pvarcurrprops->value.vartype == OV_VT_UINT) {
		if(pvarcurrprops->value.valueunion.val_uint > ov_scheduler_allowedJitter)
			ov_scheduler_numberOfExceeds = 0;	/*	if the allowed jitter is raised, the exceed counter is reset	*/
		ov_scheduler_allowedJitter = pvarcurrprops->value.valueunion.val_uint;
		return OV_ERR_OK;
	}
	return OV_ERR_BADTYPE;
}


/*-----------------------------------------------------------------*/
/**
 * Get ov_scheduler_numexceeds
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getNumExceeds(
		OV_ANY			*pvarcurrprops,
		const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_UINT;
	pvarcurrprops->value.valueunion.val_uint = ov_scheduler_numberOfExceeds;
	return OV_ERR_OK;
}

/**
 * Get ov_scheduler_numexceeds as UINT
 */
OV_DLLFNCEXPORT OV_UINT ov_vendortree_schedulerNumExceeds()
{
	return ov_scheduler_numberOfExceeds;
}

/*
 * Increment numexceeds
 */
OV_DLLFNCEXPORT void ov_vendortree_incrementNumExceeds() {
	ov_scheduler_numberOfExceeds++;
	return;
}


/*
 *	End of file
 */

