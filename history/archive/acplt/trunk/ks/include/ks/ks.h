/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/ks.h,v 1.52 2008-09-22 08:26:09 henning Exp $ */
#ifndef KS_KS_INCLUDED
#define KS_KS_INCLUDED
/*
 * Copyright (c) 1996, 1997, 1998, 1999, 2000
 * Lehrstuhl fuer Prozessleittechnik, RWTH Aachen
 * D-52064 Aachen, Germany.
 * All rights reserved.
 *
 * This file is part of the ACPLT/KS Package which is licensed as open
 * source under the Artistic License; you can use, redistribute and/or
 * modify it under the terms of that license.
 *
 * You should have received a copy of the Artistic License along with
 * this Package; see the file ARTISTIC-LICENSE. If not, write to the
 * Copyright Holder.
 *
 * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
 * OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
/* This mess was made by: Harald Albrecht <harald@plt.rwth-aachen.de> */

/* ----------------------------------------------------------------------------
 * In case this code is included from the C++ communication library code, then
 * use the rpc header file provided by the C++ wrapper, because that particular
 * header file encapsulates different RPC header mumbo jumbo especially on
 * OpenVMS. If not using C++, then include the ordinary rpc headers. For
 * instance, if compiling protocol functions with rpcgen, then the protocol
 * specification can use the constants defined in this header file. Thus no
 * need to maintain separate header files containing constant definitions.
 */

#include "ks/rpc.h"

#ifdef __cplusplus
extern "C" {
#endif


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


/* ----------------------------------------------------------------------------
 * Version string for use with banners, messages, etc. This is used for example
 * in the managers as well as in the test servers for the /vendor/version
 * communication object. You might want to use this version string to provide
 * information about with which version of the C++ communication library your
 * server was linked with.
 */
#define KS_VERSION_STRING "1.2.5"

	 
/* ----------------------------------------------------------------------------
 * RPC program number reserved for ACPLT/KS library. This had been aquired
 * from Sun Microsystems.
 */
#define KS_RPC_PROGRAM_NUMBER 0x49678ul


/* ----------------------------------------------------------------------------
 * Default ACPLT/KS protocol version used in this implementation.
 * KS_MINPROTOCOL_VERSION indicates the oldest ACPLT/KS protocol version
 * supported by this implementation.
 */ 
#define KS_PROTOCOL_VERSION 2ul
#define KS_MINPROTOCOL_VERSION 1ul


/* ----------------------------------------------------------------------------
 * ACPLT/KS error codes defined by the ACPLT/KS core protocol...
 */
ENUMDEF(KS_RESULT)
#define KS_ERR_OK                        ENUMVAL(KS_RESULT, 0x0000)
#define KS_ERR_GENERIC                   ENUMVAL(KS_RESULT, 0x0001)
#define KS_ERR_TARGETGENERIC             ENUMVAL(KS_RESULT, 0x0006)
#define KS_ERR_BADAUTH                   ENUMVAL(KS_RESULT, 0x0002)
#define KS_ERR_UNKNOWNAUTH               ENUMVAL(KS_RESULT, 0x0005)
#define KS_ERR_NOTIMPLEMENTED            ENUMVAL(KS_RESULT, 0x0003)
#define KS_ERR_BADPARAM                  ENUMVAL(KS_RESULT, 0x0004)
#define KS_ERR_BADOBJTYPE                ENUMVAL(KS_RESULT, 0x0007)
   
#define KS_ERR_BADNAME                   ENUMVAL(KS_RESULT, 0x0010)
#define KS_ERR_BADPATH                   ENUMVAL(KS_RESULT, 0x0011)
#define KS_ERR_BADMASK                   ENUMVAL(KS_RESULT, 0x0012)
#define KS_ERR_NOACCESS                  ENUMVAL(KS_RESULT, 0x0013)
#define KS_ERR_BADTYPE                   ENUMVAL(KS_RESULT, 0x0014)
#define KS_ERR_CANTSYNC                  ENUMVAL(KS_RESULT, 0x0015)
#define KS_ERR_BADSELECTOR               ENUMVAL(KS_RESULT, 0x0016)
#define KS_ERR_BADVALUE                  ENUMVAL(KS_RESULT, 0x0017)
    
#define KS_ERR_NOREMOTE                  ENUMVAL(KS_RESULT, 0x0020)
#define KS_ERR_SERVERUNKNOWN             ENUMVAL(KS_RESULT, 0x0021)

#define KS_ERR_BADFACTORY                ENUMVAL(KS_RESULT, 0x0030)
#define KS_ERR_ALREADYEXISTS             ENUMVAL(KS_RESULT, 0x0031)
#define KS_ERR_BADINITPARAM              ENUMVAL(KS_RESULT, 0x0032)
#define KS_ERR_BADPLACEMENT              ENUMVAL(KS_RESULT, 0x0033)
#define KS_ERR_CANTMOVE                  ENUMVAL(KS_RESULT, 0x0034)

/*  
 * ... and errors which can occur in the client part of the
 * C++ Communication Library. These error codes have been put into
 * the range 0x1000-0x1FFF to distinguish them from core protocol
 * errors.
 */
#define KS_ERR_MALFORMEDPATH             ENUMVAL(KS_RESULT, 0x1001)
#define KS_ERR_NETWORKERROR              ENUMVAL(KS_RESULT, 0x1002)
#define KS_ERR_TYPEMISMATCH              ENUMVAL(KS_RESULT, 0x1003)
#define KS_ERR_HOSTUNKNOWN               ENUMVAL(KS_RESULT, 0x1004)
#define KS_ERR_CANTCONTACT               ENUMVAL(KS_RESULT, 0x1005)
#define KS_ERR_TIMEOUT                   ENUMVAL(KS_RESULT, 0x1006)
#define KS_ERR_NOMANAGER                 ENUMVAL(KS_RESULT, 0x1007)


/* ----------------------------------------------------------------------------
 * Authentification & verification (aka A/V) stuff.
 */
ENUMDEF(KS_AUTH_TYPE)
#define KS_AUTH_NONE    ENUMVAL(KS_AUTH_TYPE, 0) /* NONE-A/V-Module   */
#define KS_AUTH_SIMPLE  ENUMVAL(KS_AUTH_TYPE, 1) /* SIMPLE-A/V-Module */


/* ----------------------------------------------------------------------------
 * Object types (flags). When browsing through the object tree, these flags
 * can be combined (or-ed) to find any object belonging to one of the types
 * listed in the flag set.
 */
ENUMDEF(KS_OBJ_TYPE)
#define KS_OT_DOMAIN     ENUMVAL(KS_OBJ_TYPE, 0x0001) /* object is a domain    */
#define KS_OT_VARIABLE   ENUMVAL(KS_OBJ_TYPE, 0x0002) /* object is a variable  */
#define KS_OT_HISTORY    ENUMVAL(KS_OBJ_TYPE, 0x0004) /* object is a history   */
#define KS_OT_LINK       ENUMVAL(KS_OBJ_TYPE, 0x0008) /* object is a link      */
#define KS_OT_STRUCTURE  ENUMVAL(KS_OBJ_TYPE, 0x0010) /* object is a structure */
#define KS_OT_ANY        ENUMVAL(KS_OBJ_TYPE, \
                                     KS_OT_DOMAIN | KS_OT_VARIABLE | \
                                     KS_OT_HISTORY | \
                                     KS_OT_LINK) /* | KS_OT_STRUCTURE) */
	/* restricted to the currently implemented types */

/* ----------------------------------------------------------------------------
 * Access modes -- this does not only embrace the "classical" access modes like
 * read & write, but also accessing parts, instantiability, etc...
 */
ENUMDEF(KS_ACCESS)
#define KS_AC_NONE          ENUMVAL(KS_ACCESS, 0) /* no access at all      */
#define KS_AC_READ          ENUMVAL(KS_ACCESS, 1) /* read access           */
#define KS_AC_WRITE         ENUMVAL(KS_ACCESS, 2) /* write access          */
#define KS_AC_READWRITE     (KS_AC_READ | KS_AC_WRITE)

#define KS_AC_DELETEABLE    ENUMVAL(KS_ACCESS, 0x10000000)
#define KS_AC_RENAMEABLE    ENUMVAL(KS_ACCESS, 0x08000000)
#define KS_AC_LINKABLE      ENUMVAL(KS_ACCESS, 0x04000000)
#define KS_AC_UNLINKABLE    ENUMVAL(KS_ACCESS, 0x02000000)

    /* object can act as a factory */
#define KS_AC_INSTANTIABLE  ENUMVAL(KS_ACCESS, 0x20000000)
    /* object is part of another object */
#define KS_AC_PART          ENUMVAL(KS_ACCESS, 0x80000000)


/* ----------------------------------------------------------------------------
 * Option flags for the GETEP service.
 */
ENUMDEF(KS_EP_FLAGS)
#define KS_EPF_PARTS     ENUMVAL(KS_EP_FLAGS, 0x80000000)
#define KS_EPF_CHILDREN  ENUMVAL(KS_EP_FLAGS, 0x40000000)
#define KS_EPF_FLATTEN   ENUMVAL(KS_EP_FLAGS, 0x00000001)
#define KS_EPF_DEFAULT   ENUMVAL(KS_EP_FLAGS, KS_EPF_PARTS | KS_EPF_CHILDREN)


/* ----------------------------------------------------------------------------
 * States a value can have, so clients can decive whether they want to trust
 * a value.
 */
ENUMDEF(KS_STATE)
#define KS_ST_NOTSUPPORTED  ENUMVAL(KS_STATE, 0) /* no state available          */
#define KS_ST_UNKNOWN       ENUMVAL(KS_STATE, 1) /* state unknown at this time  */
#define KS_ST_BAD           ENUMVAL(KS_STATE, 2) /* information is bad          */
#define KS_ST_QUESTIONABLE  ENUMVAL(KS_STATE, 3) /* information is questionable */
#define KS_ST_GOOD          ENUMVAL(KS_STATE, 4) /* information is good         */


/* ----------------------------------------------------------------------------
 * ACPLT/KS data types / variable data types.
 */
ENUMDEF(KS_VAR_TYPE)
#define KS_VT_VOID           ENUMVAL(KS_VAR_TYPE, 0x00)
#define KS_VT_BOOL           ENUMVAL(KS_VAR_TYPE, 0x02)
#define KS_VT_INT            ENUMVAL(KS_VAR_TYPE, 0x10)
#define KS_VT_UINT           ENUMVAL(KS_VAR_TYPE, 0x11)
#define KS_VT_SINGLE         ENUMVAL(KS_VAR_TYPE, 0x20)
#define KS_VT_DOUBLE         ENUMVAL(KS_VAR_TYPE, 0x21)
#define KS_VT_STRING         ENUMVAL(KS_VAR_TYPE, 0x30)
#define KS_VT_TIME           ENUMVAL(KS_VAR_TYPE, 0x31)
#define KS_VT_TIME_SPAN      ENUMVAL(KS_VAR_TYPE, 0x32)
#define KS_VT_STATE          ENUMVAL(KS_VAR_TYPE, 0x38)
#define KS_VT_STRUCT         ENUMVAL(KS_VAR_TYPE, 0x40)
#define KS_VT_BYTE_VEC       ENUMVAL(KS_VAR_TYPE, 0x81)
#define KS_VT_BOOL_VEC       ENUMVAL(KS_VAR_TYPE, 0x82)
#define KS_VT_INT_VEC        ENUMVAL(KS_VAR_TYPE, 0x90)
#define KS_VT_UINT_VEC       ENUMVAL(KS_VAR_TYPE, 0x91)
#define KS_VT_SINGLE_VEC     ENUMVAL(KS_VAR_TYPE, 0xA0)
#define KS_VT_DOUBLE_VEC     ENUMVAL(KS_VAR_TYPE, 0xA1)
#define KS_VT_STRING_VEC     ENUMVAL(KS_VAR_TYPE, 0xB0)
#define KS_VT_TIME_VEC       ENUMVAL(KS_VAR_TYPE, 0xB1)
#define KS_VT_TIME_SPAN_VEC  ENUMVAL(KS_VAR_TYPE, 0xB2)
#define KS_VT_TIME_SERIES    ENUMVAL(KS_VAR_TYPE, 0xB3)
#define KS_VT_STATE_VEC      ENUMVAL(KS_VAR_TYPE, 0xB8)


/* ----------------------------------------------------------------------------
 *
 */
ENUMDEF(KS_HIST_TYPE)
#define KS_HT_LOG            ENUMVAL(KS_HIST_TYPE, 0x00)
#define KS_HT_TABLE          ENUMVAL(KS_HIST_TYPE, 0x01)
#define KS_HT_BOOL           ENUMVAL(KS_HIST_TYPE, 0x02)
#define KS_HT_INT            ENUMVAL(KS_HIST_TYPE, 0x10)
#define KS_HT_UINT           ENUMVAL(KS_HIST_TYPE, 0x11)
#define KS_HT_SINGLE         ENUMVAL(KS_HIST_TYPE, 0x20)
#define KS_HT_DOUBLE         ENUMVAL(KS_HIST_TYPE, 0x21)
#define KS_HT_STRING         ENUMVAL(KS_HIST_TYPE, 0x30)
#define KS_HT_TIME           ENUMVAL(KS_HIST_TYPE, 0x31)
#define KS_HT_TIME_SPAN      ENUMVAL(KS_HIST_TYPE, 0x32)

#define KS_HT_TYPE_MASK      ENUMVAL(KS_HIST_TYPE, 0x0000FFFF)

#define KS_HT_TIME_DRIVEN    ENUMVAL(KS_HIST_TYPE, 0x80000000)
#define KS_HT_CHANGE_DRIVEN  ENUMVAL(KS_HIST_TYPE, 0x40000000)


/* ----------------------------------------------------------------------------
 * Link types can be either 1:1, 1:m or m:n and in addition with local scope or
 * global scope ("global" means currently "within the same ACPLT/KS server").
 */
ENUMDEF(KS_LINK_TYPE)
#define KS_LT_LOCAL_1_1         ENUMVAL(KS_LINK_TYPE, 1)
#define KS_LT_LOCAL_1_MANY      ENUMVAL(KS_LINK_TYPE, 2)
#define KS_LT_LOCAL_MANY_MANY   ENUMVAL(KS_LINK_TYPE, 5)
#define KS_LT_LOCAL_MANY_1      ENUMVAL(KS_LINK_TYPE, 6)
#define KS_LT_GLOBAL_1_1        ENUMVAL(KS_LINK_TYPE, 3)
#define KS_LT_GLOBAL_1_MANY     ENUMVAL(KS_LINK_TYPE, 4)
#define KS_LT_GLOBAL_MANY_MANY  ENUMVAL(KS_LINK_TYPE, 7)
#define KS_LT_GLOBAL_MANY_1     ENUMVAL(KS_LINK_TYPE, 8)


/* ----------------------------------------------------------------------------
 * Placement hint used with object management systems which support ordered
 * sets of children/parts.
 */
ENUMDEF(KS_PLACEMENT_HINT)
#define KS_PMH_DEFAULT  ENUMVAL(KS_PLACEMENT_HINT, 0)
#define KS_PMH_BEGIN    ENUMVAL(KS_PLACEMENT_HINT, 1)
#define KS_PMH_END      ENUMVAL(KS_PLACEMENT_HINT, 2)
#define KS_PMH_BEFORE   ENUMVAL(KS_PLACEMENT_HINT, 3)
#define KS_PMH_AFTER    ENUMVAL(KS_PLACEMENT_HINT, 4)


/* ----------------------------------------------------------------------------
 *
 */
ENUMDEF(KS_INTERPOLATION_MODE)
#define KS_IPM_NONE     ENUMVAL(KS_INTERPOLATION_MODE, 0x0000)
#define KS_IPM_LINEAR   ENUMVAL(KS_INTERPOLATION_MODE, 0x0001)
#define KS_IPM_MIN      ENUMVAL(KS_INTERPOLATION_MODE, 0x0002)
#define KS_IPM_MAX      ENUMVAL(KS_INTERPOLATION_MODE, 0x0004)
#define KS_IPM_HOLD     ENUMVAL(KS_INTERPOLATION_MODE, 0x0008)
#define KS_IPM_DEFAULT  ENUMVAL(KS_INTERPOLATION_MODE, 0x8000)


/* ----------------------------------------------------------------------------
 *
 */
ENUMDEF(KS_STRUCTURE_FLAGS)
#define KS_STF_ATOMICWRITE ENUMVAL(KS_STRUCTURE_FLAGS, 0x0001)


/* ----------------------------------------------------------------------------
 * The so-called "semantic flags" allow ACPLT/KS servers to indicate server-
 * specific semantics with individual ACPLT/KS communication objects. For
 * instance, function block system servers might want to indicate that par-
 * ticular variables are input or output ports of function blocks, etc.
 */
typedef u_long KS_SEMANTIC_FLAGS;


/* ----------------------------------------------------------------------------
 * Service codes. Dial 1-800-ACPLTKS for more information =:)
 */
ENUMDEF(KS_SVC)
#define KS_NULL           ENUMVAL(KS_SVC, 0)
    /*
     * Now for the manager part of the KS protocol
     */
#define KS_REGISTER       ENUMVAL(KS_SVC, 0x0000FF01)
#define KS_UNREGISTER     ENUMVAL(KS_SVC, 0x0000FF02)
#define KS_GETSERVER      ENUMVAL(KS_SVC, 0x0000FF03)
    
    /*
     * The GetPP/GetEP services group.
     */
#define KS_GETPP          ENUMVAL(KS_SVC, 0x00000001)
#define KS_GETEP          ENUMVAL(KS_SVC, 0x00000002)
#define KS_GETCANONICALPATH ENUMVAL(KS_SVC, 0x00000003)
    
    /*
     * The variable access service group.
     */
#define KS_GETVAR         ENUMVAL(KS_SVC, 0x00000101)
#define KS_SETVAR         ENUMVAL(KS_SVC, 0x00000102)
#define KS_EXGDATA        ENUMVAL(KS_SVC, 0x00000103)

    /*
     * Object management service group.
     */
#define KS_CREATEOBJECT   ENUMVAL(KS_SVC, 0x00000201)
#define KS_DELETEOBJECT   ENUMVAL(KS_SVC, 0x00000202)
#define KS_RENAMEOBJECT   ENUMVAL(KS_SVC, 0x00000203)

    /*
     * Structure management service group.
     */
#define KS_LINK           ENUMVAL(KS_SVC, 0x00000301)
#define KS_UNLINK         ENUMVAL(KS_SVC, 0x00000302)

    /*
     * History access service group.
     */
#define KS_GETHIST        ENUMVAL(KS_SVC, 0x00000401)


/*
 * Please note that although an objects name (identifier) can't be longer than
 * 255 characters in length, there is no limit on the path to an object. Thus,
 * a full path and identifier name can be of arbitrary length.
 */
#define KS_NAME_MAXLEN 255
#define KS_COMMENT_MAXLEN 4095
#define KS_TECHUNIT_MAXLEN 63
#define KS_SIMPLEID_MAXLEN 255

#ifdef __cplusplus
}; /* extern "C" */
#endif
    
#endif
/* End of ks/ks.h */
