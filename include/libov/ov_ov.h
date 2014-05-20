/*
*   $Id: ov_ov.h,v 1.22 2011-01-18 14:27:34 martin Exp $
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
*	04-Nov-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: variable type ANY added.
*	02-Apr-2001 Ansgar Münnemann <ansgar@plt.rwth-aachen.de>: adjustings for KsHistory-objects.
*/

#ifndef OV_OV_H_INCLUDED
#define OV_OV_H_INCLUDED

#include "libov/ov_config.h"
#include "libov/ov_version.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef OV_COMPILE_LIBOV
#define OV_COMPILE_LIBRARY_ov
#endif

/*
*	Generic datatypes
*	-----------------
*/
typedef void*		OV_POINTER;		/* generic pointer */
typedef enum_t		OV_ENUM;		/* enumeration value */
typedef unsigned char	OV_BYTE;		/* used with C-type variables */


/* ----------------------------------------------------------------------------
 * When using some brain-damaged C compilers for embedded systems, we need to
 * circumvent enums being 16bit wide on some plattforms. So we define #defines
 * as usual and drop clean enum type checking. Sigh. Shit happens. Damn, now
 * access to this indecent file must be restricted within Australia.
 */
#define ENUMDEF(t) typedef enum_t t; typedef enum_t t##_ENUM;
#ifdef __C166__
#define ENUMVAL(t,v) ((t)v##ul)
#else
#define ENUMVAL(t,v) ((t)v)
#endif

/*
*	OV_STATE:
*	---------
*   States a value may have.
*	Note: some codes are identical with the ACPLT/KS codes of KS_STATE.
*/
ENUMDEF(OV_STATE)
#define OV_ST_NOTSUPPORTED  ENUMVAL(OV_STATE, 0) /* no state available          */
#define OV_ST_UNKNOWN       ENUMVAL(OV_STATE, 1) /* state unknown at this time  */
#define OV_ST_BAD           ENUMVAL(OV_STATE, 2) /* information is bad          */
#define OV_ST_QUESTIONABLE  ENUMVAL(OV_STATE, 3) /* information is questionable */
#define OV_ST_GOOD          ENUMVAL(OV_STATE, 4) /* information is good         */



/*
*	OV_VAR_TYPE:
*	------------
*	Variable types (indicate the datatypes of variables).
*	Note: some codes are identical with the ACPLT/KS codes of OV_VAR_TYPE.
*/
ENUMDEF(OV_VAR_TYPE)
#define OV_VT_VOID           ENUMVAL(OV_VAR_TYPE, 0x00)	/* used with the ANY datatype */
#define OV_VT_BYTE           ENUMVAL(KS_VAR_TYPE, 0x01)	/*not in KS */
#define OV_VT_BOOL           ENUMVAL(OV_VAR_TYPE, 0x02)
#define OV_VT_INT            ENUMVAL(OV_VAR_TYPE, 0x10)
#define OV_VT_UINT           ENUMVAL(OV_VAR_TYPE, 0x11)
#define OV_VT_SINGLE         ENUMVAL(OV_VAR_TYPE, 0x20)
#define OV_VT_DOUBLE         ENUMVAL(OV_VAR_TYPE, 0x21)
#define OV_VT_STRING         ENUMVAL(OV_VAR_TYPE, 0x30)
#define OV_VT_TIME           ENUMVAL(OV_VAR_TYPE, 0x31)
#define OV_VT_TIME_SPAN      ENUMVAL(OV_VAR_TYPE, 0x32)
#define OV_VT_STATE          ENUMVAL(OV_VAR_TYPE, 0x38)
#define OV_VT_STRUCT         ENUMVAL(OV_VAR_TYPE, 0x40)

#define OV_VT_CTYPE          ENUMVAL(OV_VAR_TYPE, 0x41)	/*not in KS */
#define OV_VT_POINTER        ENUMVAL(KS_VAR_TYPE, 0x42)	/*not in KS */
#define OV_VT_ISVECTOR       ENUMVAL(KS_VAR_TYPE, 0x80)	/*not in KS; OV_VT_type_VEC = (OV_VT_type | OV_VT_ISVECTOR) */

#define OV_VT_BYTE_VEC       ENUMVAL(OV_VAR_TYPE, 0x81)
#define OV_VT_BOOL_VEC       ENUMVAL(OV_VAR_TYPE, 0x82)
#define OV_VT_INT_VEC        ENUMVAL(OV_VAR_TYPE, 0x90)
#define OV_VT_UINT_VEC       ENUMVAL(OV_VAR_TYPE, 0x91)
#define OV_VT_SINGLE_VEC     ENUMVAL(OV_VAR_TYPE, 0xA0)
#define OV_VT_DOUBLE_VEC     ENUMVAL(OV_VAR_TYPE, 0xA1)
#define OV_VT_STRING_VEC     ENUMVAL(OV_VAR_TYPE, 0xB0)
#define OV_VT_TIME_VEC       ENUMVAL(OV_VAR_TYPE, 0xB1)
#define OV_VT_TIME_SPAN_VEC  ENUMVAL(OV_VAR_TYPE, 0xB2)
#define OV_VT_TIME_SERIES    ENUMVAL(OV_VAR_TYPE, 0xB3)
#define OV_VT_STATE_VEC      ENUMVAL(OV_VAR_TYPE, 0xB8)

#define OV_VT_STRUCT_VEC     ENUMVAL(OV_VAR_TYPE, 0x000000C0)	/*not in KS */

#define OV_VT_HAS_STATE      ENUMVAL(OV_VAR_TYPE, 0x00010000)	/*not in KS */
#define OV_VT_HAS_TIMESTAMP  ENUMVAL(OV_VAR_TYPE, 0x00020000)	/*not in KS */
#define OV_VT_KSMASK         ENUMVAL(OV_VAR_TYPE, 0x0000ffff)	/*not in KS */

#define OV_VT_BOOL_PV        (OV_VT_BOOL | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP)
#define OV_VT_INT_PV         (OV_VT_INT | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP)
#define OV_VT_UINT_PV        (OV_VT_UINT | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP)
#define OV_VT_SINGLE_PV      (OV_VT_SINGLE | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP)
#define OV_VT_DOUBLE_PV      (OV_VT_DOUBLE | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP)
#define OV_VT_STRING_PV      (OV_VT_STRING | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP)
#define OV_VT_TIME_PV        (OV_VT_TIME | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP)
#define OV_VT_TIME_SPAN_PV   (OV_VT_TIME_SPAN | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP)

#define OV_VT_BOOL_PV_VEC    (OV_VT_BOOL_VEC | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP)
#define OV_VT_INT_PV_VEC     (OV_VT_INT_VEC | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP)
#define OV_VT_UINT_PV_VEC    (OV_VT_UINT_VEC | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP)
#define OV_VT_SINGLE_PV_VEC  (OV_VT_SINGLE_VEC | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP)
#define OV_VT_DOUBLE_PV_VEC  (OV_VT_DOUBLE_VEC | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP)
#define OV_VT_STRING_PV_VEC  (OV_VT_STRING_VEC | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP)
#define OV_VT_TIME_PV_VEC    (OV_VT_TIME_VEC | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP)
#define OV_VT_TIME_SPAN_PV_VEC (OV_VT_TIME_SPAN_VEC | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP)

#define OV_VT_ANY            ENUMVAL(OV_VAR_TYPE, 0x000000ff)


/*
*	OV_VAR_PROPS:
*	-------------
*	Variable properties.
*/
#define OV_VP_GETACCESSOR	0x00000001			/* variable has a get accessor */
#define OV_VP_SETACCESSOR	0x00000002			/* variable has a set accessor */
#define OV_VP_ACCESSORS		(OV_VP_GETACCESSOR | OV_VP_SETACCESSOR)
#define OV_VP_DERIVED		0x00000004			/* variable is derived (virtual) */
#define OV_VP_STATIC		0x00000008			/* variable is static (class variable) */

typedef OV_ENUM OV_VAR_PROPS;

/*
*	Basic data types
*	----------------
*	Note: most data types are identical with the datatypes used in the
*	XDR routines generated for use with ACPLT/KS.
*
*	Use of C99 fixed width data types of intstd.h and inttypes.h
*/
#ifdef _STDINT_H
	typedef	int32_t	OV_BOOL;		/* bool value */
	typedef int32_t	OV_INT;			/* integer value */
	typedef uint32_t	OV_UINT;	/* unsigned integer value */
#else
	typedef	boot_t	OV_BOOL;		/* bool value */
	typedef long	OV_INT;			/* integer value */
	typedef u_long	OV_UINT;	/* unsigned integer value */
#endif
typedef float	OV_SINGLE;		/* single precision floating value */
typedef double	OV_DOUBLE;		/* double precision floating value */
typedef char*	OV_STRING;		/* string value */

#if !OV_SYSTEM_UNIX
	#if OV_SYSTEM_MC164
		#define	OV_PRINT_BOOL "l"
	#else
		#define	OV_PRINT_BOOL "i"
	#endif
#else
	#define	OV_PRINT_BOOL "i"
#endif

#ifdef PRId32
	#define	OV_PRINT_INT	PRId32		/* integer value */
	#define	OV_PRINT_UINT	PRIu32		/* unsigned integer value */
#else
	#define	OV_PRINT_INT	"i"		/* integer value */
	#define	OV_PRINT_UINT	"lu"		/* unsigned integer value */
#endif
#define	OV_PRINT_SINGLE	"f"	/* single precision floating value */
#define	OV_PRINT_DOUBLE	"lf"	/* double precision floating value */
#define	OV_PRINT_STRING	"s"	/* string value */


typedef struct {				/* time/date value */
	OV_UINT	secs;
	OV_UINT	usecs;
}	OV_TIME;

typedef struct {				/* time span (duration) value */
	OV_INT	secs;
	OV_INT	usecs;
}	OV_TIME_SPAN;

/*
*	Datatypes of dynamic vectors
*	----------------------------
*/
typedef struct {				/* dynamic byte value vector, used for C-typed variables */
	OV_UINT			veclen;
	OV_BYTE			*value;
}	OV_BYTE_VEC;

typedef struct {				/* dynamic bool value vector */
	OV_UINT			veclen;
	OV_BOOL			*value;
}	OV_BOOL_VEC;

typedef struct {				/* dynamic integer value vector */
	OV_UINT			veclen;
	OV_INT			*value;
}	OV_INT_VEC;

typedef struct {				/* dynamic unsigned integer value vector */
	OV_UINT			veclen;
	OV_UINT			*value;
}	OV_UINT_VEC;

typedef struct {				/* dynamic single precision floating point value vector */
	OV_UINT			veclen;
	OV_SINGLE		*value;
}	OV_SINGLE_VEC;

typedef struct {				/* dynamic double precision floating point value vector */
	OV_UINT			veclen;
	OV_DOUBLE		*value;
}	OV_DOUBLE_VEC;

typedef struct {				/* dynamic string value vector */
	OV_UINT			veclen;
	OV_STRING		*value;
}	OV_STRING_VEC;

typedef struct {				/* dynamic time/date value vector */
	OV_UINT			veclen;
	OV_TIME			*value;
}	OV_TIME_VEC;

typedef struct {				/* dynamic time span (duration) value vector */
	OV_UINT			veclen;
	OV_TIME_SPAN	*value;
}	OV_TIME_SPAN_VEC;

typedef struct {				/* dynamic time span (duration) value vector */
	OV_UINT			veclen;
	OV_STATE		*value;
}	OV_STATE_VEC;

typedef struct {				/* generic dynamic value vector (internal use) */
	OV_UINT			veclen;
	OV_POINTER		value;
}	OV_GENERIC_VEC;

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

typedef struct {				/* unsigned integer process value */
	OV_UINT		value;
	OV_STATE	state;
	OV_TIME		time;
}	OV_UINT_PV;

typedef struct {				/* single precision floating point process value */
	OV_SINGLE	value;
	OV_STATE	state;
	OV_TIME		time;
}	OV_SINGLE_PV;

typedef struct {				/* double precision floating point process value */
	OV_DOUBLE	value;
	OV_STATE	state;
	OV_TIME		time;
}	OV_DOUBLE_PV;

typedef struct {				/* string process value */
	OV_STRING	value;
	OV_STATE	state;
	OV_TIME		time;
}	OV_STRING_PV;

typedef struct {				/* time process value */
	OV_TIME		value;
	OV_STATE	state;
	OV_TIME		time;
}	OV_TIME_PV;

typedef struct {				/* time span process value */
	OV_TIME_SPAN	value;
	OV_STATE	state;
	OV_TIME		time;
}	OV_TIME_SPAN_PV;

/*
*	Process vector value datatypes (PV-vectors)
*/
typedef struct {				/* boolean process value */
	OV_BOOL_VEC	value;
	OV_STATE	state;
	OV_TIME		time;
}	OV_BOOL_PV_VEC;

typedef struct {				/* integer process value */
	OV_INT_VEC	value;
	OV_STATE	state;
	OV_TIME		time;
}	OV_INT_PV_VEC;

typedef struct {				/* unsigned integer process value */
	OV_UINT_VEC	value;
	OV_STATE	state;
	OV_TIME		time;
}	OV_UINT_PV_VEC;

typedef struct {				/* single precision floating point process value */
	OV_SINGLE_VEC	value;
	OV_STATE	state;
	OV_TIME		time;
}	OV_SINGLE_PV_VEC;

typedef struct {				/* double precision floating point process value */
	OV_DOUBLE_VEC	value;
	OV_STATE	state;
	OV_TIME		time;
}	OV_DOUBLE_PV_VEC;

typedef struct {				/* string process value */
	OV_STRING_VEC	value;
	OV_STATE	state;
	OV_TIME		time;
}	OV_STRING_PV_VEC;

typedef struct {				/* time process value */
	OV_TIME_VEC	value;
	OV_STATE	state;
	OV_TIME		time;
}	OV_TIME_PV_VEC;

typedef struct {				/* time span process value */
	OV_TIME_SPAN_VEC value;
	OV_STATE	 state;
	OV_TIME		 time;
}	OV_TIME_SPAN_PV_VEC;

/*
*	OV_STRUCT datatype
*	------------------
*/
typedef struct {
	OV_UINT			elements;
	OV_BYTE			*value;
}       OV_STRUCT;


/*
*	OV_STRUCT_VEC datatype
*	----------------------
*/
typedef struct {
	OV_UINT			veclen;
	OV_BYTE*		value;
} 	OV_STRUCT_VEC;

/*
*	OV_VAR_VALUE:
*	-------------
*	Current value of a variable.
*/
typedef struct {
	OV_VAR_TYPE				vartype;			/* actual vartype, may be OV_VT_VOID */
	union {
		/* basic datatypes */
		OV_BOOL				val_bool;			/* if vartype == OV_VT_BOOL */
		OV_BYTE				val_byte;			/* if vartype == OV_VT_BYTE */
		OV_INT				val_int;			/* if vartype == OV_VT_INT */
		OV_UINT				val_uint;			/* if vartype == OV_VT_UINT */
		OV_SINGLE			val_single;			/* if vartype == OV_VT_SINGLE */
		OV_DOUBLE			val_double;			/* if vartype == OV_VT_DOUBLE */
		OV_STRING			val_string;			/* if vartype == OV_VT_STRING */
		OV_TIME				val_time;			/* if vartype == OV_VT_TIME */
		OV_TIME_SPAN		val_time_span;		/* if vartype == OV_VT_TIME_SPAN */
		OV_STATE			val_state;			/* if vartype == OV_VT_TIME_SPAN */
		OV_STRUCT			val_struct;			/* if vartype == OV_VT_STRUCT */
		/* vector datatypes */
		OV_BYTE_VEC			val_byte_vec;		/* if vartype == OV_VT_BYTE_VEC or OV_VT_CTYPE*/
		OV_BOOL_VEC			val_bool_vec;		/* if vartype == OV_VT_BOOL_VEC */
		OV_INT_VEC			val_int_vec;		/* if vartype == OV_VT_INT_VEC */
		OV_UINT_VEC			val_uint_vec;		/* if vartype == OV_VT_UINT_VEC */
		OV_SINGLE_VEC		val_single_vec;		/* if vartype == OV_VT_SINGLE_VEC */
		OV_DOUBLE_VEC		val_double_vec;		/* if vartype == OV_VT_DOUBLE_VEC */
		OV_STRING_VEC		val_string_vec;		/* if vartype == OV_VT_STRING_VEC */
		OV_TIME_VEC			val_time_vec;		/* if vartype == OV_VT_TIME_VEC */
		OV_TIME_SPAN_VEC	val_time_span_vec;	/* if vartype == OV_VT_TIME_SPAN_VEC */
		OV_STATE_VEC		val_state_vec;		/* if vartype == OV_VT_TIME_SPAN */
		OV_GENERIC_VEC		val_generic_vec;	/* for internal use only */
		OV_STRUCT_VEC		val_struct_vec;		/* if vartype == OV_VT_STRUCT_VEC */
	}	valueunion;
}	OV_VAR_VALUE;

/*
*	ANY datatype
*	------------
*/
typedef struct {
	OV_VAR_VALUE			value;				/* the actual value */
	OV_STATE				state;				/* state of the value */
	OV_TIME					time;				/* time stamp of the value */
}	OV_ANY;

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
#define OV_AP_LOCAL		0x00000001			/* association has local name scope */

typedef OV_ENUM OV_ASSOC_PROPS;

/*
*	OV_OP_PROPS:
*	---------------
*	Operation properties.
*/
#define OV_OP_ABSTRACT	0x00000001			/* operation is not implemented */
#define OV_OP_CFUNCTION	0x00000002			/* operation is implemented by a c-function */

typedef OV_ENUM OV_OP_PROPS;

/*
*	OV_TICKET_TYPE:
*	---------------
*	Type of an authentification/verification ticket (as used in ACPLT/KS)
*/
ENUMDEF(OV_AUTH_TYPE)
#define OV_TT_NONE    ENUMVAL(OV_AUTH_TYPE, 0) /* NONE-A/V-Module   */
#define OV_TT_SIMPLE  ENUMVAL(OV_AUTH_TYPE, 1) /* SIMPLE-A/V-Module */

typedef OV_ENUM OV_TICKET_TYPE;

/*
*	limits of values
*	----------------
*/
#define OV_VL_MAXINT	(((OV_UINT)-1) >> 1)
#define OV_VL_MAXUINT	((OV_UINT)-1)

/*
* 	OV_RESULT:
*	----------
*	Error codes used in ACPLT/OV.
*	Note: some codes are identical with the ACPLT/KS codes of KS_RESULT.
*/
ENUMDEF(OV_RESULT)
#define OV_ERR_OK                        ENUMVAL(OV_RESULT, 0x0000)
#define OV_ERR_GENERIC                   ENUMVAL(OV_RESULT, 0x0001)
#define OV_ERR_TARGETGENERIC             ENUMVAL(OV_RESULT, 0x0006)
#define OV_ERR_BADAUTH                   ENUMVAL(OV_RESULT, 0x0002)
#define OV_ERR_UNKNOWNAUTH               ENUMVAL(OV_RESULT, 0x0005)
#define OV_ERR_NOTIMPLEMENTED            ENUMVAL(OV_RESULT, 0x0003)
#define OV_ERR_BADPARAM                  ENUMVAL(OV_RESULT, 0x0004)
#define OV_ERR_BADOBJTYPE                ENUMVAL(OV_RESULT, 0x0007)

#define OV_ERR_BADNAME                   ENUMVAL(OV_RESULT, 0x0010)
#define OV_ERR_BADPATH                   ENUMVAL(OV_RESULT, 0x0011)
#define OV_ERR_BADMASK                   ENUMVAL(OV_RESULT, 0x0012)
#define OV_ERR_NOACCESS                  ENUMVAL(OV_RESULT, 0x0013)
#define OV_ERR_BADTYPE                   ENUMVAL(OV_RESULT, 0x0014)
#define OV_ERR_BADSELECTOR               ENUMVAL(OV_RESULT, 0x0016)
#define OV_ERR_BADVALUE                  ENUMVAL(OV_RESULT, 0x0017)

#define OV_ERR_BADFACTORY                ENUMVAL(OV_RESULT, 0x0030)
#define OV_ERR_ALREADYEXISTS             ENUMVAL(OV_RESULT, 0x0031)
#define OV_ERR_BADINITPARAM              ENUMVAL(OV_RESULT, 0x0032)
#define OV_ERR_BADPLACEMENT              ENUMVAL(OV_RESULT, 0x0033)
#define OV_ERR_CANTMOVE                  ENUMVAL(OV_RESULT, 0x0034)

#define OV_ERR_FILEALREADYEXISTS	0x00010000
#define OV_ERR_CANTCREATEFILE		0x00010001
#define OV_ERR_CANTOPENFILE		0x00010002
#define OV_ERR_CANTLOCKFILE		0x00010003
#define OV_ERR_CANTREADFROMFILE		0x00010004
#define OV_ERR_CANTWRITETOFILE		0x00010005
#define OV_ERR_CANTMAPFILE		0x00010006
#define OV_ERR_BADDATABASE		0x00010007

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

#define OV_ERR_UNKNOWNOPTYPEDEF		0x00010032
#define OV_ERR_OPTYPEDEFMISMATCH 	0x00010033

#define Ov_OK(result)	((result) == OV_ERR_OK)
#define Ov_Fail(result)	((result) != OV_ERR_OK)

/*
*	OV_ACCESS:
*	----------
*	Access modes of objects and their parts.
*	this does not only embrace the "classical" access modes like
*	read & write, but also accessing parts, instantiability, etc...
*/
ENUMDEF(OV_ACCESS)
#define OV_AC_NONE          ENUMVAL(OV_ACCESS, 0) /* no access at all, element is not visible */
#define OV_AC_READ          ENUMVAL(OV_ACCESS, 1) /* read access           */
#define OV_AC_WRITE         ENUMVAL(OV_ACCESS, 2) /* write access          */
#define OV_AC_READWRITE     (OV_AC_READ | OV_AC_WRITE) /* both read and write access */

#define OV_AC_DELETEABLE    ENUMVAL(OV_ACCESS, 0x10000000) /* object can be deleted */
#define OV_AC_RENAMEABLE    ENUMVAL(OV_ACCESS, 0x08000000) /* object can be renamed */
#define OV_AC_LINKABLE      ENUMVAL(OV_ACCESS, 0x04000000) /* parent/child can be linked */
#define OV_AC_UNLINKABLE    ENUMVAL(OV_ACCESS, 0x02000000) /* parent/child can be unlinked */

#define OV_AC_INSTANTIABLE  ENUMVAL(OV_ACCESS, 0x20000000) /* object can act as a factory */
#define OV_AC_PART          ENUMVAL(OV_ACCESS, 0x80000000) /* object is part of another object */

/* ----------------------------------------------------------------------------
 * Object types (flags). When browsing through the object tree, these flags
 * can be combined (or-ed) to find any object belonging to one of the types
 * listed in the flag set.
 */
ENUMDEF(OV_OBJ_TYPE)
#define OV_OT_DOMAIN     ENUMVAL(OV_OBJ_TYPE, 0x0001) /* object is a domain    */
#define OV_OT_VARIABLE   ENUMVAL(OV_OBJ_TYPE, 0x0002) /* object is a variable  */
#define OV_OT_HISTORY    ENUMVAL(OV_OBJ_TYPE, 0x0004) /* object is a history   */
#define OV_OT_LINK       ENUMVAL(OV_OBJ_TYPE, 0x0008) /* object is a link      */
#define OV_OT_STRUCTURE  ENUMVAL(OV_OBJ_TYPE, 0x0010) /* object is a structure */
#define OV_OT_ANY        ENUMVAL(OV_OBJ_TYPE, \
                                     OV_OT_DOMAIN | OV_OT_VARIABLE | \
                                     OV_OT_HISTORY | \
                                     OV_OT_LINK) /* | OV_OT_STRUCTURE) */
/* restricted to the currently implemented types */

/*
*	OV_ASSOC_TYPE:
*	------------
*	Association types.
*/
#define OV_AT_ONE_TO_MANY	0x00000001			/* 1:n association */
#define OV_AT_MANY_TO_MANY	0x00000002			/* n:m association */
#define OV_AT_ONE_TO_ONE	0x00000003			/* 1:1 association */

typedef OV_ENUM OV_ASSOC_TYPE;


/* ----------------------------------------------------------------------------
 * Link types can be either 1:1, 1:m or m:n and in addition with local scope or
 * global scope ("global" means currently "within the same ACPLT/KS server").
 */
ENUMDEF(OV_LINK_TYPE)
#define OV_LT_LOCAL_1_1         ENUMVAL(OV_LINK_TYPE, 1)
#define OV_LT_LOCAL_1_MANY      ENUMVAL(OV_LINK_TYPE, 2)
#define OV_LT_LOCAL_MANY_MANY   ENUMVAL(OV_LINK_TYPE, 5)
#define OV_LT_LOCAL_MANY_1      ENUMVAL(OV_LINK_TYPE, 6)
#define OV_LT_GLOBAL_1_1        ENUMVAL(OV_LINK_TYPE, 3)
#define OV_LT_GLOBAL_1_MANY     ENUMVAL(OV_LINK_TYPE, 4)
#define OV_LT_GLOBAL_MANY_MANY  ENUMVAL(OV_LINK_TYPE, 7)
#define OV_LT_GLOBAL_MANY_1     ENUMVAL(OV_LINK_TYPE, 8)

/*
*	OV_PLACEMENT_HINT:
*	------------------
*	Placement hint used with associations/links.
*/
ENUMDEF(OV_PLACEMENT_HINT)
#define OV_PMH_DEFAULT  ENUMVAL(OV_PLACEMENT_HINT, 0)
#define OV_PMH_BEGIN    ENUMVAL(OV_PLACEMENT_HINT, 1)
#define OV_PMH_END      ENUMVAL(OV_PLACEMENT_HINT, 2)
#define OV_PMH_BEFORE   ENUMVAL(OV_PLACEMENT_HINT, 3)
#define OV_PMH_AFTER    ENUMVAL(OV_PLACEMENT_HINT, 4)

/*
*	OV_OBJ_STATE:
*	-------------
*	Possible macros state of an object during its lifetime
*	(values may be OR-ed)
*/
#define OV_OS_NONE	0x00000000		/* object is not yet initialized */
#define OV_OS_INIT	0x00000001		/* object is initialized */
#define OV_OS_STARTED	0x00000002		/* object is started up */
#define OV_OS_ACTIVE	0x00000004		/* object is active */

typedef OV_ENUM OV_OBJ_STATE;

/* ----------------------------------------------------------------------------
 * Services. Most are used with KS
 */
ENUMDEF(OV_SVC)

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


/* ----------------------------------------------------------------------------
 * The so-called "semantic flags" allow ACPLT/KS servers to indicate server-
 * specific semantics with individual ACPLT/KS communication objects. For
 * instance, function block system servers might want to indicate that par-
 * ticular variables are input or output ports of function blocks, etc.
 */
typedef uint32_t OV_SEMANTIC_FLAGS;


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
ENUMDEF(OV_HIST_TYPE)
#define OV_HT_LOG            ENUMVAL(OV_HIST_TYPE, 0x00)
#define KS_HT_TABLE          ENUMVAL(OV_HIST_TYPE, 0x01) /* only in KS */
#define OV_HT_BOOL           ENUMVAL(OV_HIST_TYPE, 0x02)
#define OV_HT_INT            ENUMVAL(OV_HIST_TYPE, 0x10)
#define OV_HT_UINT           ENUMVAL(OV_HIST_TYPE, 0x11)
#define OV_HT_SINGLE         ENUMVAL(OV_HIST_TYPE, 0x20)
#define OV_HT_DOUBLE         ENUMVAL(OV_HIST_TYPE, 0x21)
#define OV_HT_STRING         ENUMVAL(OV_HIST_TYPE, 0x30)
#define OV_HT_TIME           ENUMVAL(OV_HIST_TYPE, 0x31)
#define OV_HT_TIME_SPAN      ENUMVAL(OV_HIST_TYPE, 0x32)

#define OV_HT_TYPE_MASK      ENUMVAL(OV_HIST_TYPE, 0x0000FFFF)

#define OV_HT_TIME_DRIVEN    ENUMVAL(OV_HIST_TYPE, 0x80000000)
#define OV_HT_CHANGE_DRIVEN  ENUMVAL(OV_HIST_TYPE, 0x40000000)
typedef OV_ENUM OV_HIST_TYPE;

/*
*	OV_INTERPOLATION_MODE:
*	----------------------
*	Interpolation modes for use with ACPLT/KS histories
*/
ENUMDEF(OV_INTERPOLATION_MODE)
#define OV_IPM_NONE     ENUMVAL(OV_INTERPOLATION_MODE, 0x0000)
#define OV_IPM_LINEAR   ENUMVAL(OV_INTERPOLATION_MODE, 0x0001)
#define OV_IPM_MIN      ENUMVAL(OV_INTERPOLATION_MODE, 0x0002)
#define OV_IPM_MAX      ENUMVAL(OV_INTERPOLATION_MODE, 0x0004)
#define OV_IPM_HOLD     ENUMVAL(OV_INTERPOLATION_MODE, 0x0008)
#define OV_IPM_DEFAULT  ENUMVAL(OV_INTERPOLATION_MODE, 0x8000)

/*
*	OV_TICKET:
*	----------
*	Tickets are used for authentification/verification
*/
typedef struct {
	struct OV_TICKET_VTBL	*vtbl;
	OV_TICKET_TYPE			type;
	union {
		struct {
			OV_STRING		id;				/* only, if tickettype == OV_TT_SIMPLE */
		}	simpleticket;
	}	ticketunion;
}	OV_TICKET;

/*
*	OV_TICKET_VTBL:
*	---------------
*	VTable associated with an authentification/verification ticket (see ACPLT/KS)
*/

/*
 * since version 1.9 we are not limited to ks-xdr. Hence we need a more flexible way to work with tickets.
 * To use the new definition old code does not need many changes. A simple typecast inside the respective functions should be enough.
 */
#ifndef OV_XDR_TICKET
struct OV_TICKET_VTBL {
	OV_TICKET	*(* createticket)(void *data, OV_TICKET_TYPE type);
	void 		(* deleteticket)(OV_TICKET *pticket);
	OV_BOOL		(* encodereply)(void *data, OV_TICKET *pticket);
	OV_ACCESS	(* getaccess)(const OV_TICKET *pticket);
};
#else
struct OV_TICKET_VTBL {
	OV_TICKET	*(* createticket)(XDR *xdr, OV_TICKET_TYPE type);
	void 		(* deleteticket)(OV_TICKET *pticket);
	OV_BOOL		(* encodereply)(XDR *xdr, OV_TICKET *pticket);
	OV_ACCESS	(* getaccess)(const OV_TICKET *pticket);
};
#endif
typedef struct OV_TICKET_VTBL	OV_TICKET_VTBL;

/*
*	OV_MSG_TYPE:
*	------------
*	Type of a logfile message
*/
#define OV_MT_UNKNOWN		0x00000000
#define OV_MT_INFO		0x00000001
#define OV_MT_DEBUG		0x00000002
#define OV_MT_WARNING		0x00000003
#define OV_MT_ERROR		0x00000004
#define OV_MT_ALERT		0x00000005

typedef OV_ENUM OV_MSG_TYPE;

/*
*	different object name constants
*	-------------------------------
*/
#define OV_OBJNAME_ROOT				"/"
#define OV_OBJNAME_ACPLT			"acplt"
#define OV_OBJNAME_OV				"ov"
#define OV_OBJNAME_OBJECT 			"object"
#define OV_OBJNAME_DOMAIN			"domain"
#define OV_OBJNAME_LIBRARY			"library"
#define OV_OBJNAME_STRUCTURE			"structure"
#define OV_OBJNAME_CLASS			"class"
#define OV_OBJNAME_ELEMENT			"element"
#define OV_OBJNAME_VARIABLE			"variable"
#define OV_OBJNAME_PART				"part"
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

