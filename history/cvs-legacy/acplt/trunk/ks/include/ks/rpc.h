/* -*-plt-c++-*- */
#ifndef KS_RPC_INCLUDED
#define KS_RPC_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/rpc.h,v 1.18 1999-09-16 10:54:41 harald Exp $ */
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

#if PLT_SYSTEM_NT

#if PLT_USE_WINSOCK2
#include <winsock2.h>
#endif

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
*  Don´t put the rpc below the socket headers.
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

#endif /* KS_RPC_INCLUDED */
