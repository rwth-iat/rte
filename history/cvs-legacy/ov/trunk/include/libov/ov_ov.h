/*
*   $Id: ov_ov.h,v 1.2 1999-07-26 16:14:07 dirk Exp $
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
*	08-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

#ifndef OV_OV_H_INCLUDED
#define OV_OV_H_INCLUDED

#include "libov/ov_config.h"
#include "libov/ov_version.h"

#include "ks/ks.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef OV_COMPILE_LIBOV
#define OV_COMPILE_LIBRARY_ov
#endif

/*
*	Basic data types
*	----------------
*	Note: most data types are identical with the datatypes used in the 
*	XDR routines generated for use with ACPLT/KS.
*/
typedef void*	OV_POINTER;		/* generic pointer */
typedef enum_t	OV_ENUM;		/* enumeration value */

typedef	bool_t	OV_BOOL;		/* bool value */
typedef long	OV_INT;			/* integer value */
typedef u_long	OV_UINT;		/* unsigned integer value */
typedef float	OV_SINGLE;		/* single precision floating value */
typedef double	OV_DOUBLE;		/* double precision floating value */
typedef char*	OV_STRING;		/* string value */
typedef char	OV_BYTE;		/* used with C-type variables */

typedef struct {				/* time/date value */
	OV_UINT	secs;
	OV_UINT	usecs;
}	OV_TIME;

typedef struct {				/* time span (duration) value */
	OV_INT	secs;
	OV_INT	usecs;
}	OV_TIME_SPAN;

/*
*	OV_STATE:
*	---------
*   States a value may have.
*	Note: some codes are identical with the ACPLT/KS codes of KS_STATE.
*/
#define OV_ST_NOTSUPPORTED	KS_ST_NOTSUPPORTED	/* no state available */
#define OV_ST_UNKNOWN		KS_ST_UNKNOWN		/* state unknown at this time */
#define OV_ST_BAD			KS_ST_BAD			/* information is bad */
#define OV_ST_QUESTIONABLE	KS_ST_QUESTIONABLE	/* information is questionable */
#define OV_ST_GOOD			KS_ST_GOOD			/* information is good */

typedef OV_ENUM OV_STATE;

/*
*	Process value datatypes (PVs)
*/
typedef struct {				/* boolean process value */
	OV_BOOL		value;
	OV_STATE	state;
	OV_TIME		time;
}	OV_BOOL_PV;

typedef struct {				/* integer process value */
	OV_INT		value;
	OV_STATE	state;
	OV_TIME		time;
}	OV_INT_PV;

typedef struct {				/* single precision floating point process value */
	OV_SINGLE	value;
	OV_STATE	state;
	OV_TIME		time;
}	OV_SINGLE_PV;

/*
*	OV_VAR_TYPE:
*	------------
*	Variable types (indicate the datatypes of variables).
*	Note: some codes are identical with the ACPLT/KS codes of OV_VAR_TYPE.
*/
#define OV_VT_BOOL			KS_VT_BOOL
#define OV_VT_INT			KS_VT_INT
#define OV_VT_UINT			KS_VT_UINT
#define OV_VT_SINGLE		KS_VT_SINGLE
#define OV_VT_DOUBLE		KS_VT_DOUBLE
#define OV_VT_STRING		KS_VT_STRING
#define OV_VT_TIME			KS_VT_TIME
#define OV_VT_TIME_SPAN		KS_VT_TIME_SPAN

#define OV_VT_BOOL_VEC		KS_VT_BOOL_VEC
#define OV_VT_INT_VEC		KS_VT_INT_VEC
#define OV_VT_UINT_VEC		KS_VT_UINT_VEC
#define OV_VT_SINGLE_VEC	KS_VT_SINGLE_VEC
#define OV_VT_DOUBLE_VEC	KS_VT_DOUBLE_VEC
#define OV_VT_STRING_VEC	KS_VT_STRING_VEC
#define OV_VT_TIME_VEC		KS_VT_TIME_VEC
#define OV_VT_TIME_SPAN_VEC	KS_VT_TIME_SPAN_VEC

#define OV_VT_STRUCT		KS_VT_STRUCT

#define OV_VT_VOID			KS_VT_VOID			/* not used in ACPLT/OV */
#define OV_VT_BYTE_VEC		KS_VT_BYTE_VEC		/* used for C-type variables */

#define OV_VT_HAS_STATE		ENUMVAL(KS_VAR_TYPE, 0x00010000)
#define OV_VT_HAS_TIMESTAMP	ENUMVAL(KS_VAR_TYPE, 0x00020000)
#define OV_VT_KSMASK		ENUMVAL(KS_VAR_TYPE, 0x0000ffff)

#define OV_VT_BOOL_PV		(KS_VT_BOOL | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP)
#define OV_VT_INT_PV		(KS_VT_INT | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP)
#define OV_VT_SINGLE_PV		(KS_VT_SINGLE | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP)

typedef OV_ENUM OV_VAR_TYPE;

/*
*	OV_VAR_PROPS:
*	-------------
*	Variable properties.
*/
#define OV_VP_GETACCESSOR	0x00000001			/* variable has a get accessor */
#define OV_VP_SETACCESSOR	0x00000002			/* variable has a set accessor */
#define OV_VP_ACCESSORS		(OV_VP_GETACCESSOR | OV_VP_SETACCESSOR)

typedef OV_ENUM OV_VAR_PROPS;

/*
*	OV_VAR_VALUE:
*	-------------
*	Current value of a variable.
*/
typedef struct {
	OV_VAR_TYPE		vartype;
	OV_UINT			veclen;						/* if vartype == OV_VT_XXX_VEC */
	union {
		OV_BOOL				val_bool;			/* if vartype == OV_VT_BOOL */
		OV_INT				val_int;			/* if vartype == OV_VT_INT */
		OV_UINT				val_uint;			/* if vartype == OV_VT_UINT */
		OV_SINGLE			val_single;			/* if vartype == OV_VT_SINGLE */
		OV_DOUBLE			val_double;			/* if vartype == OV_VT_DOUBLE */
		OV_STRING			val_string;			/* if vartype == OV_VT_STRING */
		OV_TIME				val_time;			/* if vartype == OV_VT_TIME */
		OV_TIME_SPAN		val_time_span;		/* if vartype == OV_VT_TIME_SPAN */
		OV_BOOL				*val_bool_vec;		/* if vartype == OV_VT_BOOL_VEC */
		OV_INT				*val_int_vec;		/* if vartype == OV_VT_INT_VEC */
		OV_UINT				*val_uint_vec;		/* if vartype == OV_VT_UINT_VEC */
		OV_SINGLE			*val_single_vec;	/* if vartype == OV_VT_SINGLE_VEC */
		OV_DOUBLE			*val_double_vec;	/* if vartype == OV_VT_DOUBLE_VEC */
		OV_STRING			*val_string_vec;	/* if vartype == OV_VT_STRING_VEC */
		OV_TIME				*val_time_vec;		/* if vartype == OV_VT_TIME_VEC */
		OV_TIME_SPAN		*val_time_span_vec;	/* if vartype == OV_VT_TIME_SPAN_VEC */
		char				*val_byte_vec;		/* if vartype == OV_VT_BYTE_VEC */
	}	valueunion;
}	OV_VAR_VALUE;

/*
*	OV_VAR_CURRENT_PROPS:
*	---------------------
*	Current properties of a variable; includes the variable value, the
*	variable's state and a time stamp (compare ACPLT/KS).
*/
typedef struct {
	OV_VAR_VALUE	value;
	OV_STATE		state;
	OV_TIME			time;
}	OV_VAR_CURRENT_PROPS;

/*
*	OV_CLASS_PROPS:
*	---------------
*	Class properties.
*/
#define OV_CP_INSTANTIABLE	0x00000001			/* class is instantiable */
#define OV_CP_FINAL			0x00000002			/* class cannot be subclassed */

typedef OV_ENUM OV_CLASS_PROPS;

/*
*	OV_ASSOC_PROPS:
*	---------------
*	Association properties.
*/
#define OV_AP_NAMEBINDING	0x00000001			/* association is namebinding */

typedef OV_ENUM OV_ASSOC_PROPS;

/*
*	OV_OP_PROPS:
*	---------------
*	Operation properties.
*/
#define OV_OP_ABSTRACT	0x00000001			/* operation is not implemented */

typedef OV_ENUM OV_OP_PROPS;

/*
*	OV_TICKET_TYPE:
*	---------------
*	Type of an authentification/verification ticket (see ACPLT/KS)
*/
#define OV_TT_NONE			KS_AUTH_NONE		/* no ticket */
#define OV_TT_SIMPLE		KS_AUTH_SIMPLE		/* a simple ticket */

typedef OV_ENUM OV_TICKET_TYPE;

/*
*	OV_TICKET:
*	----------
*	Tickets are used for authentification/verification
*/
typedef struct {
	OV_TICKET_TYPE	tickettype;
	OV_STRING		id;							/* only, if tickettype == OV_TT_SIMPLE */
}	OV_TICKET;

/*
*	limits of values
*	----------------
*/
#define OV_VL_MAXINT	(((unsigned long)-1) >> 1)
#define OV_VL_MAXUINT	((unsigned long)-1)

/*
* 	OV_RESULT:
*	----------
*	Error codes used in ACPLT/OV.
*	Note: some codes are identical with the ACPLT/KS codes of KS_RESULT.
*/
#define OV_ERR_OK					KS_ERR_OK
#define OV_ERR_GENERIC				KS_ERR_GENERIC
#define OV_ERR_TARGETGENERIC		KS_ERR_TARGETGENERIC
#define OV_ERR_BADAUTH				KS_ERR_BADAUTH
#define OV_ERR_UNKNOWNAUTH			KS_ERR_UNKNOWNAUTH
#define OV_ERR_NOTIMPLEMENTED		KS_ERR_NOTIMPLEMENTED
#define OV_ERR_BADPARAM				KS_ERR_BADPARAM
#define OV_ERR_BADOBJTYPE			KS_ERR_BADOBJTYPE
   
#define OV_ERR_BADNAME				KS_ERR_BADNAME
#define OV_ERR_BADPATH				KS_ERR_BADPATH
#define OV_ERR_BADMASK				KS_ERR_BADMASK
#define OV_ERR_NOACCESS				KS_ERR_NOACCESS
#define OV_ERR_BADTYPE				KS_ERR_BADTYPE
#define OV_ERR_BADSELECTOR			KS_ERR_BADSELECTOR
#define OV_ERR_BADVALUE				KS_ERR_BADVALUE
    
#define OV_ERR_BADFACTORY			KS_ERR_BADFACTORY
#define OV_ERR_ALREADYEXISTS		KS_ERR_ALREADYEXISTS
#define OV_ERR_BADINITPARAM			KS_ERR_BADINITPARAM
#define OV_ERR_BADPLACEMENT			KS_ERR_BADPLACEMENT
#define OV_ERR_CANTMOVE				KS_ERR_CANTMOVE

#define OV_ERR_FILEALREADYEXISTS	0x00010000
#define OV_ERR_CANTCREATEFILE		0x00010001
#define OV_ERR_CANTOPENFILE			0x00010002
#define OV_ERR_CANTLOCKFILE			0x00010003
#define OV_ERR_CANTREADFROMFILE		0x00010004
#define OV_ERR_CANTWRITETOFILE		0x00010005
#define OV_ERR_CANTMAPFILE			0x00010006
#define OV_ERR_BADDATABASE			0x00010007

#define OV_ERR_CANTOPENLIBRARY		0x00010010
	
#define OV_ERR_LIBDEFMISMATCH		0x00010020
#define OV_ERR_STRUCTDEFMISMATCH	0x00010021
#define OV_ERR_CLASSDEFMISMATCH		0x00010022
#define OV_ERR_ASSOCDEFMISMATCH		0x00010023
#define OV_ERR_VARDEFMISMATCH		0x00010024
#define OV_ERR_PARTDEFMISMATCH		0x00010025
#define OV_ERR_OPDEFMISMATCH		0x00010026
#define OV_ERR_UNKNOWNSTRUCTDEF		0x00010027
#define OV_ERR_UNKNOWNCLASSDEF		0x00010028
#define OV_ERR_UNKNOWNASSOCDEF		0x00010029

#define OV_ERR_DBOUTOFMEMORY		0x00010030
#define OV_ERR_HEAPOUTOFMEMORY		0x00010031

typedef OV_ENUM OV_RESULT;

#define Ov_OK(result)	((result) == KS_ERR_OK)
#define Ov_Fail(result)	((result) != KS_ERR_OK)

/*
*	OV_ACCESS:
*	----------
*	Access modes of objects and their parts.
*	Note: the codes are identical with the ACPLT/KS codes of KS_ACCESS.
*/
#define OV_AC_NONE			KS_AC_NONE			/* no access at all, element is not visible */
#define OV_AC_READ			KS_AC_READ			/* read access */
#define OV_AC_WRITE			KS_AC_WRITE			/* write access */
#define OV_AC_READWRITE		KS_AC_READWRITE		/* both read and write access */
#define OV_AC_INSTANTIABLE	KS_AC_INSTANTIABLE	/* object can act as a factory */
#define OV_AC_PART			KS_AC_PART			/* object is part of another object */
#define OV_AC_DELETEABLE	KS_AC_DELETEABLE	/* object can be deleted */
#define OV_AC_RENAMEABLE	KS_AC_RENAMEABLE	/* object can be renamed */
#define OV_AC_LINKABLE		KS_AC_LINKABLE		/* parent/child can be linked */
#define OV_AC_UNLINKABLE	KS_AC_UNLINKABLE	/* parent/child can be unlinked */

typedef OV_ENUM OV_ACCESS;

/*
*	OV_ASSOC_TYPE:
*	------------
*	Association types.
*/
#define OV_AT_ORDERED_LIST	0x00000001			/* ordered list */

typedef OV_ENUM OV_ASSOC_TYPE;

/*
*	OV_PLACEMENT_HINT:
*	------------------
*	Placement hint used with associations/links.
*	Note: some codes are identical with the ACPLT/KS codes of KS_PLACEMENT_HINT.
*/
#define OV_PMH_DEFAULT	KS_PMH_DEFAULT
#define OV_PMH_BEGIN	KS_PMH_BEGIN
#define OV_PMH_END		KS_PMH_END
#define OV_PMH_BEFORE	KS_PMH_BEFORE
#define OV_PMH_AFTER	KS_PMH_AFTER

typedef OV_ENUM OV_PLACEMENT_HINT;

/*
*	OV_OBJ_STATE:
*	-------------
*	Possible macros state of an object during its lifetime
*	(values may be OR-ed)
*/
#define OV_OS_NONE		0x00000000		/* object is not yet initialized */
#define OV_OS_INIT		0x00000001		/* object is initialized */
#define OV_OS_STARTED	0x00000002		/* object is started up */
#define OV_OS_ACTIVE	0x00000004		/* object is active */

typedef OV_ENUM OV_OBJ_STATE;

/*
*	OV_TIME_TYPE:
*	-------------
*	Time types for use with ACPLT/KS histories
*/
#define KS_TT_ABSOLUTE	0x00000000		/* TODO! should be defined in ks.h... */
#define KS_TT_RELATIVE	0x00000001		/* ...but is currently defined in selector.h */

#define OV_TT_ABSOLUTE	KS_TT_ABSOLUTE
#define OV_TT_RELATIVE	KS_TT_RELATIVE

typedef OV_ENUM OV_TIME_TYPE;

/*
*	OV_HSEL_TYPE:
*	-------------
*	Selector types for use with ACPLT/KS histories
*/
#define KS_HSELT_NONE	0x00000000		/* TODO! should be defined in ks.h... */
#define KS_HSELT_TIME	0x00000001		/* ...but is currently defined in selector.h */
#define KS_HSELT_STRING	0x00000002		/* dto. */

#define OV_HSELT_NONE	KS_HSELT_NONE
#define OV_HSELT_TIME	KS_HSELT_TIME
#define OV_HSELT_STRING	KS_HSELT_STRING

typedef OV_ENUM OV_HSEL_TYPE;

/*
*	OV_HIST_TYPE:
*	-------------
*	History types for use with ACPLT/KS histories
*/
#define OV_HT_LOG			KS_HT_LOG
#define OV_HT_BOOL			KS_HT_BOOL
#define OV_HT_INT			KS_HT_INT
#define OV_HT_UINT			KS_HT_UINT
#define OV_HT_SINGLE		KS_HT_SINGLE
#define OV_HT_DOUBLE		KS_HT_DOUBLE
#define OV_HT_STRING		KS_HT_STRING
#define OV_HT_TIME			KS_HT_TIME
#define OV_HT_TIME_SPAN		KS_HT_TIME_SPAN
#define OV_HT_TYPE_MASK		KS_HT_TYPE_MASK
#define OV_HT_TIME_DRIVEN	KS_HT_TIME_DRIVEN
#define OV_HT_CHANGE_DRIVEN	KS_HT_CHANGE_DRIVEN

typedef OV_ENUM OV_HIST_TYPE;

/*
*	OV_INTERPOLATION_MODE:
*	----------------------
*	Interpolation modes for use with ACPLT/KS histories
*/
#define OV_IPM_NONE		KS_IPM_NONE
#define OV_IPM_LINEAR	KS_IPM_LINEAR
#define OV_IPM_MIN		KS_IPM_MIN
#define OV_IPM_MAX		KS_IPM_MAX
#define OV_IPM_HOLD		KS_IPM_HOLD
#define OV_IPM_DEFAULT	KS_IPM_DEFAULT

typedef OV_ENUM OV_INTERPOLATION_MODE;

/*
*	OV_MSG_TYPE:
*	------------
*	Type of a logfile message
*/
#define OV_MT_UNKNOWN	0x00000000
#define OV_MT_INFO		0x00000001
#define OV_MT_DEBUG		0x00000002
#define OV_MT_WARNING	0x00000003
#define OV_MT_ERROR		0x00000004
#define OV_MT_ALERT		0x00000005

typedef OV_ENUM OV_MSG_TYPE;

/*
*	different object name constants
*	-------------------------------
*/
#define OV_OBJNAME_ROOT					"/"
#define OV_OBJNAME_ACPLT				"acplt"
#define OV_OBJNAME_OV					"ov"
#define OV_OBJNAME_OBJECT 				"object"
#define OV_OBJNAME_DOMAIN				"domain"
#define OV_OBJNAME_LIBRARY				"library"
#define OV_OBJNAME_STRUCTURE			"structure"
#define OV_OBJNAME_CLASS				"class"
#define OV_OBJNAME_ELEMENT				"element"
#define OV_OBJNAME_VARIABLE				"variable"
#define OV_OBJNAME_PART					"part"
#define OV_OBJNAME_OPERATION			"operation"
#define OV_OBJNAME_ASSOCIATION			"association"
#define OV_OBJNAME_CONTAINMENT			"containment"

#define OV_CONST_OPENFNC_PREFIX			"ov_library_open_"

#ifdef __cplusplus
}	/* extern "C" */
#endif

/*
*	Include debugging stuff
*/
#include "libov/ov_debug.h"

#endif
/*
*	End of file
*/

