/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/ks.h,v 1.26 1999-02-26 13:28:04 harald Exp $ */
#ifndef KS_KS_INCLUDED
#define KS_KS_INCLUDED
/*
 * Copyright (c) 1996, 1997, 1998, 1999
 * Chair of Process Control Engineering,
 * Aachen University of Technology.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must print or display the above
 *    copyright notice either during startup or must have a means for
 *    the user to view the copyright notice.
 * 3. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the Chair of Process Control Engineering nor the
 *    name of the Aachen University of Technology may be used to endorse or
 *    promote products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE CHAIR OF PROCESS CONTROL
 * ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
#ifdef __cplusplus
#include "ks/rpc.h"
#else
#include <rpc/rpc.h>
#include <rpc/xdr.h>
#endif


/* ----------------------------------------------------------------------------
 * When using some brain-damaged C compilers for embedded systems, we need to
 * circumvent enums being 16bit wide on some plattforms. So we define constants
 * to be 32bit wide but loose enum type checking... Sigh.
 */
#if 0

#define ENUMBEGIN(t) typedef enum_t t;
#define ENUMEND /* */
#define ENUM(e,v) const enum_t e = v;

#else

#define ENUMBEGIN(t) typedef enum_t t; enum t##_ENUM {
#define ENUMEND };
#define ENUM(e,v) e = v,

#endif


/* ----------------------------------------------------------------------------
 * Version string for use with banners, messages, etc. This is used for example
 * in the managers as well as in the test servers for the /vendor/version
 * communication object. You might want to use this version string to provide
 * information about with which version of the C++ communication library your
 * server was linked with.
 */
#define KS_VERSION_STRING "1.0.5pre7"

	 
/* ----------------------------------------------------------------------------
 * RPC program number reserved for ACPLT/KS library. This had been aquired
 * from Sun Microsystems.
 */
const unsigned long KS_RPC_PROGRAM_NUMBER = 0x49678; 


/* ----------------------------------------------------------------------------
 * Default ACPLT/KS protocol version.
 */ 
const unsigned long KS_PROTOCOL_VERSION = 1;


/* ----------------------------------------------------------------------------
 * ACPLT/KS error codes defined by the ACPLT/KS core protocol...
 */
ENUMBEGIN(KS_RESULT)
    ENUM(KS_ERR_OK,                        0x0000)
    ENUM(KS_ERR_GENERIC,                   0x0001)
    ENUM(KS_ERR_TARGETGENERIC,             0x0006)
    ENUM(KS_ERR_BADAUTH,                   0x0002)
    ENUM(KS_ERR_UNKNOWNAUTH,               0x0005)
    ENUM(KS_ERR_NOTIMPLEMENTED,            0x0003)
    ENUM(KS_ERR_BADPARAM,                  0x0004)
    ENUM(KS_ERR_BADOBJTYPE,                0x0007)
   
    ENUM(KS_ERR_BADNAME,                   0x0010)
    ENUM(KS_ERR_BADPATH,                   0x0011)
    ENUM(KS_ERR_BADMASK,                   0x0012)
    ENUM(KS_ERR_NOACCESS,                  0x0013)
    ENUM(KS_ERR_BADTYPE,                   0x0014)
    ENUM(KS_ERR_CANTSYNC,                  0x0015)
    ENUM(KS_ERR_BADSELECTOR,               0x0016)
    ENUM(KS_ERR_BADVALUE,                  0x0017)
    
    ENUM(KS_ERR_NOREMOTE,                  0x0020)
    ENUM(KS_ERR_SERVERUNKNOWN,             0x0021)

    ENUM(KS_ERR_BADFACTORY,                0x0030)
    ENUM(KS_ERR_ALREADYEXISTS,             0x0031)
    ENUM(KS_ERR_BADINITPARAM,              0x0032)
    ENUM(KS_ERR_BADPLACEMENT,              0x0033)

/*  
 * ... and errors which can occur in the client part of the
 * C++ Communication Library. These error codes have been put into
 * the range 0x1000-0x1FFF to distinguish them from core protocol
 * errors.
 */
    ENUM(KS_ERR_MALFORMEDPATH,             0x1001)
    ENUM(KS_ERR_NETWORKERROR,              0x1002)
    ENUM(KS_ERR_TYPEMISMATCH,              0x1003)
    ENUM(KS_ERR_HOSTUNKNOWN,               0x1004)
    ENUM(KS_ERR_CANTCONTACT,               0x1005)
    ENUM(KS_ERR_TIMEOUT,                   0x1006)
    ENUM(KS_ERR_NOMANAGER,                 0x1007)
ENUMEND


/* ----------------------------------------------------------------------------
 * Authentification & verification (aka A/V) stuff.
 */
ENUMBEGIN(KS_AUTH_TYPE)
    ENUM(KS_AUTH_NONE,    0) /* NONE-A/V-Module   */
    ENUM(KS_AUTH_SIMPLE,  1) /* SIMPLE-A/V-Module */
ENUMEND


/* ----------------------------------------------------------------------------
 * Object types (flags). When browsing through the object tree, these flags
 * can be combined (or-ed) to find any object belonging to one of the types
 * listed in the flag set.
 */
ENUMBEGIN(KS_OBJ_TYPE)
    ENUM(KS_OT_DOMAIN,     0x0001) /* object is a domain    */
    ENUM(KS_OT_VARIABLE,   0x0002) /* object is a variable  */
    ENUM(KS_OT_HISTORY,    0x0004) /* object is a history   */
    ENUM(KS_OT_LINK,       0x0008) /* object is a link      */
    ENUM(KS_OT_STRUCTURE,  0x0010) /* object is a structure */
    ENUM(KS_OT_ANY,        KS_OT_DOMAIN | KS_OT_VARIABLE | KS_OT_HISTORY | \
                           KS_OT_LINK)
                    /* FIXME  KS_OT_STRUCTURE */
	/* restricted to the currently implemented types */
ENUMEND

/* ----------------------------------------------------------------------------
 * Access modes -- this does not only embrace the "classical" access modes like
 * read & write, but also accessing parts, instantiability, etc...
 */
ENUMBEGIN(KS_ACCESS)
    ENUM(KS_AC_NONE,          0) /* no access at all      */
    ENUM(KS_AC_READ,          1) /* read access           */
    ENUM(KS_AC_WRITE,         2) /* write access          */
    ENUM(KS_AC_READWRITE,     KS_AC_READ | KS_AC_WRITE)

    /* object can act as a factory */
    ENUM(KS_AC_INSTANTIABLE,  0x20000000)
    /* object is part of another object */
    ENUM(KS_AC_PART,          0x80000000)
ENUMEND


/* ----------------------------------------------------------------------------
 * Option flags for the GETEP service.
 */
ENUMBEGIN(KS_EP_FLAGS)
    ENUM(KS_EPF_PARTS,     0x80000000)
    ENUM(KS_EPF_CHILDREN,  0x40000000)
    ENUM(KS_EPF_FLATTEN,   0x00000001)
ENUMEND


/* ----------------------------------------------------------------------------
 * States a value can have, so clients can decive whether they want to trust
 * a value.
 */
ENUMBEGIN(KS_STATE)
    ENUM(KS_ST_NOTSUPPORTED,  0) /* no state available          */
    ENUM(KS_ST_UNKNOWN,       1) /* state unknown at this time  */
    ENUM(KS_ST_BAD,           2) /* information is bad          */
    ENUM(KS_ST_QUESTIONABLE,  3) /* information is questionable */
    ENUM(KS_ST_GOOD,          4) /* information is good         */
ENUMEND


/* ----------------------------------------------------------------------------
 * ACPLT/KS data types / variable data types.
 */
ENUMBEGIN(KS_VAR_TYPE)
    ENUM(KS_VT_VOID,           0x00)
    ENUM(KS_VT_BOOL,           0x02)
    ENUM(KS_VT_INT,            0x10)
    ENUM(KS_VT_UINT,           0x11)
    ENUM(KS_VT_SINGLE,         0x20)
    ENUM(KS_VT_DOUBLE,         0x21)
    ENUM(KS_VT_STRING,         0x30)
    ENUM(KS_VT_TIME,           0x31)
    ENUM(KS_VT_TIME_SPAN,      0x32)
    ENUM(KS_VT_STATE,          0x38)
    ENUM(KS_VT_STRUCT,         0x40)
    ENUM(KS_VT_BYTE_VEC,       0x81)
    ENUM(KS_VT_BOOL_VEC,       0x82)
    ENUM(KS_VT_INT_VEC,        0x90)
    ENUM(KS_VT_UINT_VEC,       0x91)
    ENUM(KS_VT_SINGLE_VEC,     0xA0)
    ENUM(KS_VT_DOUBLE_VEC,     0xA1)
    ENUM(KS_VT_STRING_VEC,     0xB0)
    ENUM(KS_VT_TIME_VEC,       0xB1)
    ENUM(KS_VT_TIME_SPAN_VEC,  0xB2)
    ENUM(KS_VT_TIME_SERIES,    0xB3)
    ENUM(KS_VT_STATE_VEC,      0xB8)
ENUMEND


/* ----------------------------------------------------------------------------
 *
 */
ENUMBEGIN(KS_HIST_TYPE)
    ENUM(KS_HT_LOG,            0x00)
    ENUM(KS_HT_BOOL,           0x02)
    ENUM(KS_HT_INT,            0x10)
    ENUM(KS_HT_UINT,           0x11)
    ENUM(KS_HT_SINGLE,         0x20)
    ENUM(KS_HT_DOUBLE,         0x21)
    ENUM(KS_HT_STRING,         0x30)
    ENUM(KS_HT_TIME,           0x31)
    ENUM(KS_HT_TIME_SPAN,      0x32)

    ENUM(KS_HT_TYPE_MASK,      0x0000FFFF)

    ENUM(KS_HT_TIME_DRIVEN,    0x80000000)
    ENUM(KS_HT_CHANGE_DRIVEN,  0x40000000)
ENUMEND


/* ----------------------------------------------------------------------------
 * Link types can be either 1:1 or 1:m and in addition with local scope or
 * global scope (global = within the same ACPLT/KS server).
 */
ENUMBEGIN(KS_LINK_TYPE)
    ENUM(KS_LT_LOCAL_1_1,      1)
    ENUM(KS_LT_LOCAL_1_MANY,   2)
    ENUM(KS_LT_GLOBAL_1_1,     3)
    ENUM(KS_LT_GLOBAL_1_MANY,  4)
ENUMEND


/* ----------------------------------------------------------------------------
 * Placement hint used with object management systems which support ordered
 * sets of children/parts.
 */
ENUMBEGIN(KS_PLACEMENT_HINT)
    ENUM(KS_PMH_DEFAULT,  0)
    ENUM(KS_PMH_BEGIN,    1)
    ENUM(KS_PMH_END,      2)
    ENUM(KS_PMH_BEFORE,   3)
    ENUM(KS_PMH_AFTER,    4)
ENUMEND


/* ----------------------------------------------------------------------------
 *
 */
ENUMBEGIN(KS_INTERPOLATION_MODE)
    ENUM(KS_IPM_NONE,     0x0000)
    ENUM(KS_IPM_LINEAR,   0x0001)
    ENUM(KS_IPM_MIN,      0x0002)
    ENUM(KS_IPM_MAX,      0x0004)
    ENUM(KS_IPM_HOLD,     0x0008)
    ENUM(KS_IPM_DEFAULT,  0x8000)
ENUMEND


/* ----------------------------------------------------------------------------
 * Service codes. Dial 1-800-ACPLTKS for more information =:)
 */
ENUMBEGIN(KS_SVC)
    ENUM(KS_NULL,           0)
    /*
     * Now for the manager part of the KS protocol
     */
    ENUM(KS_REGISTER,       0x0000FF01)
    ENUM(KS_UNREGISTER,     0x0000FF02)
    ENUM(KS_GETSERVER,      0x0000FF03)
    
    /*
     * The GetPP services group.
     */
    ENUM(KS_GETPP,          0x00000001)
    ENUM(KS_GETEP,          0x00000002)
    
    /*
     * The variable access service group.
     */
    ENUM(KS_GETVAR,         0x00000101)
    ENUM(KS_SETVAR,         0x00000102)
    ENUM(KS_EXGDATA,        0x00000103)

    /*
     * Object management service group.
     */
    ENUM(KS_CREATEOBJECT,   0x00000201)
    ENUM(KS_DELETEOBJECT,   0x00000202)
    ENUM(KS_RENAMEOBJECT,   0x00000203)

    /*
     * Structure management service group.
     */
    ENUM(KS_LINK,           0x00000301)
    ENUM(KS_UNLINK,         0x00000302)

    /*
     * History access service group.
     */
    ENUM(KS_GETHIST,        0x00000401)
ENUMEND


/*
 * Please note that although an objects name (identifier) can't be longer than
 * 255 characters in length, there is no limit on the path to an object. Thus,
 * a full path and identifier name can be of arbitrary length.
 */
const unsigned long KS_NAME_MAXLEN     =  255;
const unsigned long KS_COMMENT_MAXLEN  = 4095;
const unsigned long KS_TECHUNIT_MAXLEN =   63;
const unsigned long KS_SIMPLEID_MAXLEN =  255;

    
#endif
/* End of ks/ks.h */
