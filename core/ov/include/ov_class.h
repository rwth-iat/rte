/*
*   $Id: ov_class.h,v 1.6 2002-01-23 13:44:14 ansgar Exp $
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
*	10-Aug-1998 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	08-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/

#ifndef OV_CLASS_H_INCLUDED1
#define OV_CLASS_H_INCLUDED1

#include "libov/ov_ov.h"

/*
*	OV_CLASS_DEF:
*	-------------
*	Class definition, included in the library's code
*/
struct OV_CLASS_DEF {
	struct OV_CLASS_DEF		*pnext;
	OV_STRING				identifier;
	OV_STRING				baseclassname;
	OV_CLASS_PROPS			classprops;
	OV_UINT					flags;
	OV_STRING				comment;
	OV_UINT					size;
	OV_UINT					staticsize;
	OV_POINTER				pvtable;
	struct OV_VARIABLE_DEF	*variables;
	struct OV_PART_DEF		*parts;
	struct OV_OPERATION_DEF	*operations;
};
typedef struct OV_CLASS_DEF OV_CLASS_DEF;

/*
*	Define the instance data of a class
*/
#define OV_TYPEDEF_INSTANCE(class)								\
	struct OV_INST_##class {									\
		OV_INSTBODY_##class										\
		struct {												\
			OV_CIB_##class										\
		}	__classinfo;										\
	};															\
	typedef struct OV_INST_##class OV_INST_##class;				\
	typedef OV_INST_##class* OV_INSTPTR_##class;				\
	struct OV_STATICINST_##class {								\
		OV_STATICINSTBODY_##class								\
		struct {												\
			OV_CIB_##class										\
		}	__classinfo;										\
	};															\
	typedef struct OV_STATICINST_##class OV_STATICINST_##class;	\
	typedef OV_STATICINST_##class* OV_STATICINSTPTR_##class

/*
*	Define the vtable of a class
*/
#define OV_TYPEDEF_VTABLE(class)								\
	struct OV_VTBL_##class {									\
		OV_VTBLBODY_##class										\
	};															\
	typedef struct OV_VTBL_##class OV_VTBL_##class;				\
	typedef OV_VTBL_##class* OV_VTBLPTR_##class


/*
*   Alignment macro, align must be a power of 2
*/
#define Ov_AlignTo(align, x) \
	(((x)+(align-1))&~(align-1))

/*
*	Get size of an instance of a given class
*/
#define Ov_GetInstSize(class)									\
	Ov_AlignTo(sizeof(double), offsetof(OV_INST_##class,		\
	__classinfo))

/*
*	Get static size of an instance of a given class
*/
#define Ov_GetStaticInstSize(class)								\
	Ov_AlignTo(sizeof(double), offsetof(OV_STATICINST_##class,	\
	__classinfo))

#endif

#include "ov.h"

#ifdef OV_INSTBODY_ov_object

#ifndef OV_CLASS_H_INCLUDED2
#define OV_CLASS_H_INCLUDED2

#ifdef __cplusplus
extern "C" {
#endif

/*
*	OV_FNC_INITOBJ:
*	---------------
*	Function prototype for function initializing an object
*/
typedef OV_RESULT OV_FNC_INITOBJ(
	OV_INSTPTR_ov_object	pobj,
	OV_POINTER				userdata
);

/*
*	Search for a class object with given identifier
*/
OV_DLLFNCEXPORT OV_INSTPTR_ov_class ov_class_search(
	OV_STRING	identifier
);

/*
*	Load a class into the database
*/
OV_RESULT ov_class_load(
	OV_CLASS_DEF*				pclassdef,
	OV_INSTPTR_ov_domain		pparent
);

/*
*	Compare a class with its definition
*/
OV_RESULT ov_class_compare(
	OV_INSTPTR_ov_class			pclass,
	OV_CLASS_DEF*				pclassdef
);

/*
*	Test if we can unload a class from the database
*/
OV_BOOL ov_class_canunload(
	OV_INSTPTR_ov_class		pclass
);

/**
*	Test if a pointer to an instance of a class can be casted to another one
*	if you want to test the object itself you should use the macro Ov_CanCastTo()
*/
OV_DLLFNCEXPORT OV_BOOL ov_class_cancastto(
	const OV_INSTPTR_ov_class	pclassfrom,
	const OV_INSTPTR_ov_class	pclassto
);
	
/**
*	Create an instance of the class
*	if the class is known to the library and you want default placing you should use the macro Ov_CreateObject()
*/
OV_DLLFNCEXPORT OV_RESULT ov_class_createobject(
	const OV_INSTPTR_ov_class	pclass,
	const OV_INSTPTR_ov_domain	pparent,
	const OV_STRING				identifier,
	const OV_PLACEMENT_HINT		hint,
	const OV_INSTPTR_ov_object	prelobj,
	OV_FNC_INITOBJ				*initobjfnc,
	OV_POINTER					userdata,
	OV_INSTPTR_ov_object		*ppobj
);

/**
*	Create an instance of the class
*	if the class is known to the library and you want default placing you should use the macro Ov_CreateIDedObject()
*	the instances identifier will consist of 16 chars representing the instance's id in hex notation 
*	and an optional given postfix
*	this function wraps around ov_class_createobject
*/
OV_DLLFNCEXPORT OV_RESULT ov_class_createIDedObject(
	const OV_INSTPTR_ov_class	pclass,
	const OV_INSTPTR_ov_domain	pparent,
	const OV_STRING				identifierPostfix,
	const OV_PLACEMENT_HINT		hint,
	const OV_INSTPTR_ov_object	prelobj,
	OV_FNC_INITOBJ				*initobjfnc,
	OV_POINTER					userdata,
	OV_INSTPTR_ov_object		*ppobj
);

/**
*	Delete an instance of the class
*	you should use Ov_DeleteObject() in your code
*/
OV_DLLFNCEXPORT OV_RESULT ov_class_deleteobject(
	const OV_INSTPTR_ov_object	pobj
);

/**
*	Rename or move an instance of the class
*	@param pobj	object to rename
*	@param pparent pointer to the (new?) parent domain (right now NOT optional)
*	@param identifier new name for the object (could be NULL if no change is requested)
*	@param hint placement hint for a new parent domain
*	@param prelobj placement partner if needed for the placement hint
*/
OV_DLLFNCEXPORT OV_RESULT ov_class_renameobject(
	const OV_INSTPTR_ov_object	pobj,
	const OV_INSTPTR_ov_domain	pparent,
	const OV_STRING				identifier,
	const OV_PLACEMENT_HINT		hint,
	const OV_INSTPTR_ov_object	prelobj
);

/*
*	Preinitialization of an object during instantiation (subroutine)
*/
OV_RESULT ov_class_createobject_preinit(
	const OV_INSTPTR_ov_class	pclass,
	const OV_INSTPTR_ov_object	pobj,
	const OV_STRING				identifier,
	const OV_TIME				*ptime,
	const OV_INSTPTR_ov_object	pouterobj
);

/*
*	Set object state to initialized during instantiation (subroutine)
*/
void ov_class_createobject_setinit(
	const OV_INSTPTR_ov_object	pobj
);

/*
*	Clean up of an object during deletion (subroutine)
*/
void ov_class_deleteobject_cleanupinst(
	OV_INSTPTR_ov_object		pobj
);

/*
*	Clean up of the static part of an object during deletion (subroutine)
*/
void ov_class_deleteobject_cleanupstaticinst(
	OV_INSTPTR_ov_class			pclass
);

/*
*	Clean up of a structure during deletion (subroutine)
*/
void ov_class_deleteobject_cleanupstruct(
	OV_ELEMENT	*pelem
);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif

#endif
/*
*	End of file
*/

