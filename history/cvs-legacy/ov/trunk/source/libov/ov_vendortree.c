/*
*   $Id: ov_vendortree.c,v 1.7 2002-01-29 15:36:07 ansgar Exp $
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
#include "libov/ov_string.h"
#include "libov/ov_time.h"
#include "libov/ov_database.h"
#include "libov/ov_macros.h"
#include "libov/ov_path.h"


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
OV_DLLVAREXPORT OV_BOOL activitylock;
/*
*	Global variables
*/
OV_DLLVAREXPORT OV_VENDORTREE_INFO vendorinfo[OV_NUM_VENDOROBJECTS] = {
	{ "associations",			NULL,	ov_vendortree_getassociations, NULL },
	{ "classes",				NULL,	ov_vendortree_getclasses, NULL },
	{ "database_fragmentation",	"%",	ov_vendortree_getdatabasefrag, NULL },
	{ "database_free",			"Byte",	ov_vendortree_getdatabasefree, NULL },
	{ "database_name",			NULL,	ov_vendortree_getdatabasename, NULL },
	{ "database_size",			"Byte",	ov_vendortree_getdatabasesize, NULL },
	{ "database_started",		NULL,	ov_vendortree_getdatabasestarted, NULL },
	{ "database_used",			"Byte",	ov_vendortree_getdatabaseused, NULL },
	{ "name", 					NULL,	ov_vendortree_getname, NULL },
	{ "libks_version",			NULL,	ov_vendortree_getlibksversion, NULL },
	{ "libov_version",			NULL,	ov_vendortree_getlibovversion, NULL },
	{ "libovks_version",		NULL,	ov_vendortree_getlibovksversion, NULL },
	{ "libraries",				NULL,	ov_vendortree_getlibraries, NULL },
	{ "semantic_flags",			NULL,	ov_vendortree_getsemanticflags, NULL },
	{ "server_description",		NULL,	ov_vendortree_getserverdescription, NULL },
	{ "server_name",			NULL,	ov_vendortree_getservername, NULL },
	{ "server_time",			"UTC",	ov_vendortree_getservertime, NULL },
	{ "server_version",			NULL,	ov_vendortree_getserverversion, NULL },
	{ "startup_time",			"UTC",	ov_vendortree_getstartuptime, NULL },
	{ "structures",				NULL,	ov_vendortree_getstructures, NULL },
	{ "activitiy_lock",			NULL,	ov_vendortree_getactivitylock, ov_vendortree_setactivitylock },
	{ "server_password",			NULL,	ov_vendortree_getserverpassword, NULL },
	{ "ov_time_offset",			NULL,	ov_vendortree_gettimeoffset, ov_vendortree_settimeoffset }
};

/*	----------------------------------------------------------------------	*/

/*
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
	memset(pdb->vendordom.v_linktable, 0, pclass_ov_domain->v_linktablesize);
	Ov_Link(ov_containment, &pdb->root, &pdb->vendordom);
	Ov_Link(ov_instantiation, pclass_ov_domain, &pdb->vendordom);
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
		Ov_Link(ov_containment, &pdb->vendordom, &pdb->vendorobj[i]);
		Ov_Link(ov_instantiation, pclass_ov_object, &pdb->vendorobj[i]);
	}
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
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

/*
*	Get vendor variable
*/
OV_RESULT ov_vendortree_getvar(
	OV_INSTPTR_ov_object	pobj,
	OV_ANY					*pvarcurrprops,
	const OV_TICKET			*pticket
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
*	Set vendor variable
*/
OV_RESULT ov_vendortree_setvar(
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

/*
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

/*
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

/*
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

/*
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

/*
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

/*
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

/*
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

/*
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

/*
*	Get list of classes in the database
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getclasses(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	/*
	*	local variables
	*/
	OV_UINT					classes = 0;
	OV_INSTPTR_ov_class		pclass;
	OV_STRING				*pstring = NULL;
	OV_INSTPTR_ov_object	pclassobj;
	OV_ELEMENT				element;
	/*
	*	count instantiable classes
	*/
	Ov_ForEachChildEx(ov_instantiation, pclass_ov_class, pclass, ov_class) {
		pclassobj = Ov_PtrUpCast(ov_object, pclass);
		element.elemtype = OV_ET_OBJECT;
		element.pobj = pclassobj;
		if(ov_object_getaccess(pclassobj, &element, NULL) & OV_AC_INSTANTIABLE) {
			classes++;
		}
	}
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
	Ov_ForEachChildEx(ov_instantiation, pclass_ov_class, pclass, ov_class) {
		pclassobj = Ov_PtrUpCast(ov_object, pclass);
		element.elemtype = OV_ET_OBJECT;
		element.pobj = pclassobj;
		if(ov_object_getaccess(pclassobj, &element, NULL) & OV_AC_INSTANTIABLE) {
			*pstring = ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object, pclass), 2);
			if(!*pstring) {
				return OV_ERR_HEAPOUTOFMEMORY;
			}
			pstring++;
		}
	}
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
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

/*
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

/*
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

/*
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

/*
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

/*
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

/*
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

/*
*	Get libks version
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getlibksversion(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_STRING;
	pvarcurrprops->value.valueunion.val_string = KS_VERSION_STRING;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
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

/*
*	Get libovks version
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getlibovksversion(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_STRING;
	pvarcurrprops->value.valueunion.val_string = OV_VER_LIBOVKS;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
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
	OV_STRING				*pstring = NULL;
	/*
	*	count associations
	*/
	Ov_ForEachChildEx(ov_instantiation, pclass_ov_library, plib, ov_library) {
		libs++;
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
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
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

/*
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

/*
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

/*
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

/*
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

/*
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

/*
*	Get list of structures in the database
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getstructures(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	/*
	*	local variables
	*/
	OV_UINT					structs = 0;
	OV_INSTPTR_ov_structure	pstruct;
	OV_STRING				*pstring = NULL;
	/*
	*	count associations
	*/
	Ov_ForEachChildEx(ov_instantiation, pclass_ov_structure, pstruct, ov_structure) {
		structs++;
	}
	/*
	*	allocate memory for the structure paths
	*/
	if(structs) {
		pstring = (OV_STRING*)ov_memstack_alloc(structs*sizeof(OV_STRING));
		if(!pstring) {
			return OV_ERR_HEAPOUTOFMEMORY;
		}
	}
	/*
	*	enter structure paths into a string vector
	*/
	pvarcurrprops->value.vartype = OV_VT_STRING_VEC;
	pvarcurrprops->value.valueunion.val_string_vec.veclen = structs;
	pvarcurrprops->value.valueunion.val_string_vec.value = pstring;
	Ov_ForEachChildEx(ov_instantiation, pclass_ov_structure, pstruct, ov_structure) {
		*pstring = ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object, pstruct), 2);
		if(!*pstring) {
			return OV_ERR_HEAPOUTOFMEMORY;
		}
		pstring++;
	}
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get activitylock
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getactivitylock(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_BOOL;
	pvarcurrprops->value.valueunion.val_bool = activitylock;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Set activitylock
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_setactivitylock(
	const OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	if (pvarcurrprops->value.vartype == OV_VT_BOOL) {
		activitylock = pvarcurrprops->value.valueunion.val_bool;
		return OV_ERR_OK;
	}
	return OV_ERR_BADTYPE;
}

/*	----------------------------------------------------------------------	*/

/*
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
		strcpy(pdb->serverpassword, password);
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	Get serverpassword
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getserverpassword(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
) {
	if (pdb->serverpassword) {
		if (pticket->type == OV_TT_SIMPLE) {
			if(!strcmp(pticket->ticketunion.simpleticket.id, pdb->serverpassword)) goto CONTINUE1;
		}
		return OV_ERR_NOACCESS;
	}
CONTINUE1:
	pvarcurrprops->value.vartype = OV_VT_STRING;
	pvarcurrprops->value.valueunion.val_string = pdb->serverpassword;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
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

/*
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

/*
*	End of file
*/

