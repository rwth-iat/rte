#ifndef KS_KS_INCLUDED
#define KS_KS_INCLUDED
/*
 * Copyright (c) 1996, 1997
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


/*
 * Currently defined ACPLT/KS error codes.
 */
enum KS_RESULT {
    KS_ERR_OK                       = 0x0000,
    KS_ERR_GENERIC                  = 0x0001,
    KS_ERR_BADAUTH                  = 0x0002,
    KS_ERR_UNKNOWNAUTH              = 0x0005,
    KS_ERR_NOTIMPLEMENTED           = 0x0003,
    KS_ERR_BADPARAM                 = 0x0004,
    
    KS_ERR_BADNAME                  = 0x0010,
    KS_ERR_BADPATH                  = 0x0011,
    KS_ERR_BADMASK                  = 0x0012,
    KS_ERR_NOACCESS                 = 0x0013,
    KS_ERR_BADTYPE                  = 0x0014,
    KS_ERR_CANTSYNC                 = 0x0015,
    
    KS_ERR_NOREMOTE                 = 0x0020,
    KS_ERR_SERVERUNKNOWN            = 0x0021
};

/*
 * A/V stuff
 */
enum KS_AUTH_TYPE {
    KS_AUTH_NONE   = 0, /* NONE-A/V-Module   */
    KS_AUTH_SIMPLE = 1  /* SIMPLE-A/V-Module */
};

/*
 * Object types (flags). When browsing through the object tree, these flags
 * can be combined (or-ed) to find any object belonging to one of the types
 * listed in the flag set.
 */
enum KS_OBJTYPE {
    KS_OT_DOMAIN   = 0x0001, /* object is a domain   */
    KS_OT_VARIABLE = 0x0002, /* object is a variable */
    KS_OT_HISTORY  = 0x0004, /* object is a history  */
    KS_OT_ANY      = 0xFFFF  /* don't care */
};

enum KS_ACCESS {
    KS_AC_NONE      = 0, /* no access at all      */
    KS_AC_READ      = 1, /* read access           */
    KS_AC_WRITE     = 2, /* write access          */
    KS_AC_READWRITE = 3  /* read and write access */
};

enum KS_STATE {
    KS_ST_NOTSUPPORTED = 0, /* no state available          */
    KS_ST_UNKNOWN      = 1, /* state unknown at this time  */
    KS_ST_BAD          = 2, /* information is bad          */
    KS_ST_QUESTIONABLE = 3, /* information is questionable */
    KS_ST_GOOD         = 4  /* information is good         */
};

enum KS_VAR_TYPE {
    KS_VT_VOID       = 0x00,
    KS_VT_INT        = 0x10,
    KS_VT_UINT       = 0x11,
    KS_VT_SINGLE     = 0x20,
    KS_VT_DOUBLE     = 0x21,
    KS_VT_STRING     = 0x30,
    KS_VT_TIME       = 0x31,
    KS_VT_BYTE_VEC   = 0x81,
    KS_VT_INT_VEC    = 0x90,
    KS_VT_UINT_VEC   = 0x91,
    KS_VT_SINGLE_VEC = 0xA0,
    KS_VT_DOUBLE_VEC = 0xA1,
    KS_VT_STRING_VEC = 0xB0,
    KS_VT_TIME_VEC   = 0xB1
};

#endif












