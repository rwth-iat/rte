#ifndef __OV_KS_MISSING_SYMBOLS_H__
#define __OV_KS_MISSING_SYMBOLS_H__
/* -*-plt-c++-*- */
#ifndef KS_RPC_INCLUDED
#define KS_RPC_INCLUDED
/*
 * Copyright (c) 1996, 1997, 1998, 1999
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
/*
 * Author: "Martin Kneissl" <martin@plt.rwth-aachen.de>
 */

#include "plt/debug.h"
#include "libov/ov_config.h"

#if PLT_SYSTEM_HPUX
extern "C" {
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <rpc/rpc.h>
#include <rpc/pmap_clnt.h>
#include <rpc/pmap_prot.h>
extern int get_myaddress(struct sockaddr_in *);
};
#endif

#if PLT_SYSTEM_LINUX || PLT_SYSTEM_IRIX
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <rpc/rpc.h>
#include <rpc/pmap_clnt.h>
#include <rpc/pmap_prot.h>
#endif

/*
 * But now for the "industrial standard" -- well, looks like our industry
 * likes this incapable, instable, insecure, insane, inappropriate and
 * inadequate operating system. Maybe just because of the same "in" prefix.
 */
#if PLT_SYSTEM_NT

/*
 * Make using WinSock2 optional, as it can be sometimes a pain in the ass.
 */
#if PLT_USE_WINSOCK2
#include <winsock2.h>
#endif

/*
 * Cygnus' Win32 compiler better than the average Windooze compiler -- no
 * wonder, as it is based on gcc technology.
 */
#if !PLT_COMPILER_CYGWIN
typedef unsigned char u_char;
#endif

#include <rpc/rpc.h>
#include <rpc/pmap_pro.h>
#include <rpc/pmap_cln.h>

#if PLT_COMPILER_MSVC
#pragma warning (disable : 4237 )  /* disable warning about defining bool... */
#endif


#ifdef __cplusplus

class KsNTOncRpc
{
public:
    KsNTOncRpc();
    ~KsNTOncRpc();
private:
    static int refcount;
};

static KsNTOncRpc ks_nt_oncrpc;

#endif

#endif /* PLT_SYSTEM_NT */

#if PLT_SYSTEM_SOLARIS
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <rpc/rpc.h>
#include <rpc/svc.h>
#include <rpc/svc_soc.h>
#include <rpc/clnt_soc.h>
#include <rpc/pmap_clnt.h>
#include <rpc/pmap_prot.h>

/* missing in solaris: */
#define INADDR_NONE ((u_long)-1)

#endif

#if PLT_SYSTEM_OPENVMS
/*
*  This seems to be very order dependant!!!
*  Don�t put the rpc below the socket headers.
*/
#include <ucx$rpc:rpc.h>
#include <ucx$rpc:pmap_clnt.h>
#include <ucx$rpc:pmap_prot.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
/* #include <rpc/clnt.h> */
#endif

#if PLT_SYSTEM_MC164
#include <rpc/xdr.h>
#include <rpc/rpc.h>
#endif

/*
 * The Never Ending Story: glic2 update: now the x_inline function
 * pointer returns an int32_t on glibc2.1-based plattforms. Well, why,
 * but in the end that's more precise than the dreaded long *, which
 * is not very 64bit compliant at all. But Sun surely never dreamt of
 * 64 bit when developing ONC/RPC...
 *
 * Note: glibc 1.x systems don't define __GLIBC__ but instead the
 * __GNULIBRARY__ symbol. Because these old glibc systems don't have
 * the int32_t, we do automatically fall back to the old long.
 *
 * Yet Another Note: FreeBSD also uses int32_t (guess where the glibc
 * header files might have come from) -- at least newer FreeBSD releases.
 */
#if PLT_SYSTEM_FREEBSD

typedef int32_t *XDR_INLINE_PTR;

#else

#if defined(PLT_RUNTIME_GLIBC) && (PLT_RUNTIME_GLIBC >= 0x20001)
typedef int32_t *XDR_INLINE_PTR;
#else
typedef long *XDR_INLINE_PTR;
#endif

#endif


#endif /* KS_RPC_INCLUDED */

/* End of ks/rpc.h */



/*
 * Copyright (c) 1996, 1997, 1998, 1999
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

/*
 * xdrmemstream.h -- This module implements memory based XDR streams
 *                   which can grow (and shrink) as necessary. In contrast
 *                   to this Sun's genuine memory streams work only on
 *                   fixed-sized, pre-allocated memory buffers.
 *
 * Written by Harald Albrecht <harald@plt.rwth-aachen.de>
 */

#ifndef __XDRDYNAMICMEMORYSTREAM_H
#define __XDRDYNAMICMEMORYSTREAM_H

//#include "ks/xdr.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ---------------------------------------------------------------------------
 * The information contained in a XDR memory stream is split up and
 * stored in fragments. These fragments form a linked list with the
 * XDR structure pointing to the first and current one.
 */
typedef struct MemoryStreamFragmentTag {
    struct MemoryStreamFragmentTag *next;  /* next fragment in chain           */
    u_int                           used;  /* # of bytes used in this fragment */
    double                          dummy; /* -- just for alignment --         */
} MemoryStreamFragment;


/* ---------------------------------------------------------------------------
 * With every XDR structure we're associating this tiny block of
 * information, so we can easily find the last block as well as we
 * can keep some accounting data. We need this extra block as the
 * XDR structure doesn't provide enough space, just the pointer to
 * this info block, a read/write pointer and a length word indicating
 * how much bytes are still free in the current fragment.
 */
typedef struct {
    MemoryStreamFragment *first;   /* points to first fragment in chain   */
    MemoryStreamFragment *current; /* points to current fragment in chain */
    u_int                 fragment_count; /* # of allocated fragments     */
    u_int                 length;
} MemoryStreamInfo;

/*
 * Create and manage XDR dynamic memory streams.
 */
OV_DLLFNCEXPORT bool_t xdrmemstream_create(XDR *xdrs);
void   xdrmemstream_clear(XDR *xdrs);
OV_DLLFNCEXPORT void   xdrmemstream_rewind(XDR *xdrs, enum xdr_op op);

/*
 * External I/O for filling and draining XDR dynamic memory streams.
 */
bool_t xdrmemstream_get_length(XDR *xdrs, int *len);
bool_t xdrmemstream_write_to_fd(XDR *xdrs, int fd, int *max, int *err);
bool_t xdrmemstream_read_from_fd(XDR *xdrs, int fd, int *max, int *err);

/*
 * Optimized reading and writing directly from the underlying buffering/
 * fragments mechanism.
 */
typedef struct {
    char          *fragment; /* points to begin of fragment */
    unsigned long  length;   /* length of fragment */
} xdrmemstream_fragment_description;
void xdrmemstream_get_fragments(XDR *xdrs, 
                                xdrmemstream_fragment_description *desc,
                                unsigned long *fragment_count,
                                unsigned long *total_size);

/*
 * Some statistics and control functions...
 */
void   xdrmemstream_getusage(u_int *total, u_int *freepool);
bool_t xdrmemstream_controlusage(u_int fragmentsize, u_int quota,
	                         u_int watermark, u_int freepercentage);
void   xdrmemstream_freegarbage();

//
OV_DLLFNCEXPORT void   MemStreamDestroy(XDR *xdrs);

#ifdef __cplusplus
}
#endif

#endif

/* End of xdrmemstream.h */
#endif
