/*
*   $Id: ov_vendortree.c,v 1.4 1999-09-15 10:48:22 dirk Exp $
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

/*
*	Global variables
*/
static OV_VENDORTREE_INFO vendorinfo[OV_NUM_VENDOROBJECTS] = {
	{ "associations",			NULL,	ov_vendortree_getassociations },
	{ "classes",				NULL,	ov_vendortree_getclasses },
	{ "database_fragmentation",	"%",	ov_vendortree_getdatabasefrag },
	{ "database_free",			"Byte",	ov_vendortree_getdatabasefree },
	{ "database_name",			NULL,	ov_vendortree_getdatabasename },
	{ "database_size",			"Byte",	ov_vendortree_getdatabasesize },
	{ "database_started",		NULL,	ov_vendortree_getdatabasestarted },
	{ "database_used",			"Byte",	ov_vendortree_getdatabaseused },
	{ "name", 					NULL,	ov_vendortree_getname },
	{ "libks_version",			NULL,	ov_vendortree_getlibksversion },
	{ "libov_version",			NULL,	ov_vendortree_getlibovversion },
	{ "libovks_version",		NULL,	ov_vendortree_getlibovksversion },
	{ "libraries",				NULL,	ov_vendortree_getlibraries },
	{ "semantic_flags",			NULL,	ov_vendortree_getsemanticflags },
	{ "server_description",		NULL,	ov_vendortree_getserverdescription },
	{ "server_name",			NULL,	ov_vendortree_getservername },
	{ "server_time",			"UTC",	ov_vendortree_getservertime },
	{ "server_version",			NULL,	ov_vendortree_getserverversion },
	{ "startup_time",			"UTC",	ov_vendortree_getstartuptime },
	{ "structures",				NULL,	ov_vendortree_getstructures }
};

/*	----------------------------------------------------------------------	*/

/*
*	Initialize the vendor tree
*/
void ov_vendortree_init(void) {
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
		Ov_Link(ov_containment, &pdb->vendordom, &pdb->vendorobj[i]);
		Ov_Link(ov_instantiation, pclass_ov_object, &pdb->vendorobj[i]);
	}
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
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
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
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
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
	pvarcurrprops->value.veclen = assocs;
	pvarcurrprops->value.valueunion.val_string_vec = pstring;
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
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
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
	pvarcurrprops->value.veclen = classes;
	pvarcurrprops->value.valueunion.val_string_vec = pstring;
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
OV_RESULT ov_vendortree_getdatabasefrag(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_UINT;
	pvarcurrprops->value.valueunion.val_uint = ov_database_getfrag();
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get free storage of the database
*/
OV_RESULT ov_vendortree_getdatabasefree(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_UINT;
	pvarcurrprops->value.valueunion.val_uint = ov_database_getfree();
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get database name
*/
OV_RESULT ov_vendortree_getdatabasename(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_STRING;
	pvarcurrprops->value.valueunion.val_string = databasename;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get size of the database
*/
OV_RESULT ov_vendortree_getdatabasesize(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
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
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_BOOL;
	pvarcurrprops->value.valueunion.val_bool = pdb->started;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get used storage of the database
*/
OV_RESULT ov_vendortree_getdatabaseused(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_UINT;
	pvarcurrprops->value.valueunion.val_uint = ov_database_getused();
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get vendor name
*/
OV_RESULT ov_vendortree_getname(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_STRING;
	pvarcurrprops->value.valueunion.val_string = vendorname;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get libks version
*/
OV_RESULT ov_vendortree_getlibksversion(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_STRING;
	pvarcurrprops->value.valueunion.val_string = KS_VERSION_STRING;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get libov version
*/
OV_RESULT ov_vendortree_getlibovversion(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_STRING;
	pvarcurrprops->value.valueunion.val_string = OV_VER_LIBOV;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get libovks version
*/
OV_RESULT ov_vendortree_getlibovksversion(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
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
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
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
	pvarcurrprops->value.veclen = libs;
	pvarcurrprops->value.valueunion.val_string_vec = pstring;
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
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_STRING_VEC;
	pvarcurrprops->value.veclen = sizeof(semantic_flag)/sizeof(semantic_flag[0]);
	pvarcurrprops->value.valueunion.val_string_vec = &semantic_flag[0];
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get server description
*/
OV_RESULT ov_vendortree_getserverdescription(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_STRING;
	pvarcurrprops->value.valueunion.val_string = serverdescription;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get server name
*/
OV_RESULT ov_vendortree_getservername(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_STRING;
	pvarcurrprops->value.valueunion.val_string = servername;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get server time
*/
OV_RESULT ov_vendortree_getservertime(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_TIME;
	ov_time_gettime(&pvarcurrprops->value.valueunion.val_time);
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get server version
*/
OV_RESULT ov_vendortree_getserverversion(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
) {
	pvarcurrprops->value.vartype = OV_VT_STRING;
	pvarcurrprops->value.valueunion.val_string = serverversion;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get startup time
*/
OV_RESULT ov_vendortree_getstartuptime(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
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
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
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
	pvarcurrprops->value.veclen = structs;
	pvarcurrprops->value.valueunion.val_string_vec = pstring;
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
*	End of file
*/

