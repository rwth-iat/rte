/*
*   $Id: ov_vendortree.h,v 1.13 2007-04-24 14:11:29 martin Exp $
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

#ifndef OV_VENDORTREE_H_INCLUDED
#define OV_VENDORTREE_H_INCLUDED

#include "ov.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
*	Number of objects in the vendor tree
*/
#define	OV_NUM_VENDOROBJECTS	34

/*
*	Number of configuration bits
*/
#define	OV_NUM_SERVER_CONFIG	2
//	Boolean[0]: ov_activitylock
//	Boolean[1]: ov_explain

/*
*	Global variables
*/
#ifdef OV_COMPILE_LIBOV
/* Manual fix */
#if OV_COMPILER_BORLAND
#define OV_EXTERN extern
#else
#define OV_EXTERN OV_DLLVAREXPORT
#endif
#else
#define OV_EXTERN OV_DLLVARIMPORT
#endif

OV_EXTERN OV_BOOL ov_activitylock;
OV_EXTERN OV_BOOL ov_explain;
OV_EXTERN OV_BOOL ov_backup;
#undef OV_EXTERN

/*
*	OV_FNC_GETVENDORVAR:
*	--------------------
*	Function prototype for getting vendor variables
*/
typedef OV_RESULT OV_FNC_GETVENDORVAR(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/*
*	OV_FNC_SETVENDORVAR:
*	--------------------
*	Function prototype for setting vendor variables
*/
typedef OV_RESULT OV_FNC_SETVENDORVAR(
	const OV_ANY	*pvarcurrprops,
	const OV_TICKET	*pticket
);

/*
*	OV_VENDORTREE_INFO:
*	-------------------
*	Information to a vendor tree object
*/
typedef struct {
	OV_STRING			identifier;
	OV_STRING			unit;
	OV_FNC_GETVENDORVAR	*getvarfnc;
	OV_FNC_SETVENDORVAR	*setvarfnc;
}	OV_VENDORTREE_INFO;

/**
 *	Initialize the vendor tree
 */
OV_RESULT ov_vendortree_init(void);

/**
 * frees the heap memory used by the vendor tree variables
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_free(void);

/**
 *	Get unit of a vendor object
 */
OV_DLLFNCEXPORT OV_STRING ov_vendortree_getunit(
	OV_INSTPTR_ov_object	pobj
);

/**
 *	Get variable of a vendor object
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getvar(
	OV_INSTPTR_ov_object	pobj,
	OV_ANY			*pvarcurrprops,
	const OV_TICKET		*pticket
);

/* 
 *	Set variable of a vendor object
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_setvar(
	OV_INSTPTR_ov_object	pobj,
	const OV_ANY		*pvarcurrprops,
	const OV_TICKET		*pticket
);

/**
 *	Set database name
 */
OV_DLLFNCEXPORT void ov_vendortree_setdatabasename(
	OV_STRING	name
);

/**
 *	Set vendor name
 */
OV_DLLFNCEXPORT void ov_vendortree_setname(
	OV_STRING	name
);

/**
 *	Set semantic flag
 */
OV_DLLFNCEXPORT void ov_vendortree_setsemanticflag(
	OV_UINT		flagnum,
	OV_STRING	flagvalue
);

/**
 *	Set server description
 */
OV_DLLFNCEXPORT void ov_vendortree_setserverdescription(
	OV_STRING	name
);
/**
 *	Set server name
 */
OV_DLLFNCEXPORT void ov_vendortree_setservername(
	OV_STRING	name
);


/**
 *	Set server version
 */
OV_DLLFNCEXPORT void ov_vendortree_setserverversion(
	OV_STRING	name
);

/**
 *	Set startup time
 */
OV_DLLFNCEXPORT void ov_vendortree_setstartuptime(
	OV_TIME		*ptime
);

/**
 *	Get list of associations in the database
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getassociations(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/**
 *	Get classes of the database
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getclasses(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/**
 *	Get fragmentation of the database
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getdatabasefrag(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/**
 *	Get free storage of the database
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getdatabasefree(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/**
 *	Get database name
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getdatabasename(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/**
 *	Get size of the database
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getdatabasesize(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/**
 *	Get whether the database is started or not
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getdatabasestarted(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/**
 *	Get used storage of the database
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getdatabaseused(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/**
 *	Get vendor name
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getname(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/**
 *	Get ks version
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getksversion(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/**
 *	Set ks version
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_setksversion(
		OV_STRING	version,
		const OV_TICKET	*pticket
);

/**
 *	Get libov version
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getlibovversion(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);


/**
 *	Get list of libraries in the database
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getlibraries(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/**
 *	Get list of semantic flags in the database
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getsemanticflags(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/**
 *	Get server description
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getserverdescription(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/**
 *	Get server name
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getservername(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/**
 *	Get server time
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getservertime(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/**
 *	Get server version
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getserverversion(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/**
 *	Get startup time
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getstartuptime(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/**
 *	Get number of instances in database
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getInstanceCount(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

OV_DLLFNCEXPORT OV_UINT ov_vendortree_getInstanceCountUINT();

/**
 *	Get PID of server
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getserverPID(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

OV_DLLFNCEXPORT OV_UINT ov_vendortree_getserverPIDUINT();
OV_DLLFNCEXPORT void ov_vendortree_setServerPID();
/**
 *	Get server configuration
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getserverconfiguration(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/**
 *	Set server configuration
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_setserverconfiguration(
	const OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/**
 *	Get backup
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getbackup(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/**
 *	Write backup file
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_writebackup(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/**
 *	Set serverpassword external
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_setserverpassword_ext(
	const OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/**
 *	Set serverpassword
 */
OV_DLLFNCEXPORT void ov_vendortree_setserverpassword(
	OV_STRING	password
);

/**
 *	Get serverpassword
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getserverpassword(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/**
 *	Get timeoffset
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_gettimeoffset(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/**
 *	Set timeoffset
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_settimeoffset(
	const OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/**
 *	Get commandline options
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getcmdlineoptions(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/**
 *	Set commandline options
 */
OV_DLLFNCEXPORT void ov_vendortree_setcmdlineoptions(
	OV_STRING	options
);

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
);

/**
 *	Get the PORT number out of the cmdlineoptions string if it is set
 *	returns -1 if PORT is not set
 *	returns port number on success
 */

OV_DLLFNCEXPORT OV_INT ov_vendortree_getport();

/**
 * Get the running flag
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getserverrun(
		OV_ANY			*pvarcurrprops,
		const OV_TICKET	*pticket
);

/*
 *	Set the running flag
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_setserverrun(
		const OV_ANY			*pvarcurrprops,
		const OV_TICKET	*pticket
);

/*
 * Get ks_maxItemsPerRequest
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getKsMaxItems(
		OV_ANY			*pvarcurrprops,
		const OV_TICKET	*pticket
);

/**
 *	Get ks_maxItemsPerRequest
 */

OV_DLLFNCEXPORT OV_UINT ov_vendortree_KsMaxItems();

/**
 *	Set ks_maxItemsPerRequest
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_setKsMaxItems(
		const OV_ANY			*pvarcurrprops,
		const OV_TICKET	*pticket
);

/*
 * Get maxstringlength
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getMaxStringLength(
		OV_ANY			*pvarcurrprops,
		const OV_TICKET	*pticket
);

/**
 * Get maxstringlength as UINT
 */
OV_DLLFNCEXPORT OV_UINT ov_vendortree_MaxStringLength();

/**
 *	Set maxstringlength
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_setMaxStringLength(
		const OV_ANY			*pvarcurrprops,
		const OV_TICKET	*pticket
);

/*
 * Get maxvectorlength
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getMaxVectorLength(
		OV_ANY			*pvarcurrprops,
		const OV_TICKET	*pticket
);

/**
 *	Get maxvectorlength as UINT
 */
OV_DLLFNCEXPORT OV_UINT ov_vendortree_MaxVectorLength();

/**
 *	Set maxvectorlength
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_setMaxVectorLength(
		const OV_ANY			*pvarcurrprops,
		const OV_TICKET	*pticket
);

/*
 * Get maxnamelength
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getMaxNameLength(
		OV_ANY			*pvarcurrprops,
		const OV_TICKET	*pticket
);

/**
 *	Get maxnamelength as UINT
 */
OV_DLLFNCEXPORT OV_UINT ov_vendortree_MaxNameLength();

/**
 *	Set maxnamelength
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_setMaxNameLength(
		const OV_ANY			*pvarcurrprops,
		const OV_TICKET	*pticket
);

/*
 * Get maxhierarchydepth
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getMaxHierarchyDepth(
		OV_ANY			*pvarcurrprops,
		const OV_TICKET	*pticket
);

/**
 *	Get maxhierarchydepth as UINT
 */
OV_DLLFNCEXPORT OV_UINT ov_vendortree_MaxHierarchyDepth();

/**
 *	Set maxhierarchydepth
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_setMaxHierarchyDepth(
		const OV_ANY			*pvarcurrprops,
		const OV_TICKET	*pticket
);
/**
 * Get ov_scheduler_allowedjitter
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getAllowedJitter(
		OV_ANY			*pvarcurrprops,
		const OV_TICKET	*pticket
);

/**
 * Get ov_scheduler_allowedjitter as UINT
 */
OV_DLLFNCEXPORT OV_UINT ov_vendortree_schedulerAllowedJitter();

/*
 *	Set ov_scheduler_allowedjitter
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_setAllowedJitter(
		const OV_ANY			*pvarcurrprops,
		const OV_TICKET	*pticket
);

/**
 * Get ov_scheduler_numexceeds
 */
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getNumExceeds(
		OV_ANY			*pvarcurrprops,
		const OV_TICKET	*pticket
);

OV_DLLFNCEXPORT OV_UINT ov_vendortree_schedulerNumExceeds();

/*
 * Increment numexceeds
 */
OV_DLLFNCEXPORT void ov_vendortree_incrementNumExceeds();



#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif
/*
*	End of file
*/

