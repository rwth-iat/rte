/* -*-plt-c++-*- */
#ifndef KS_RPC_INCLUDED
#define KS_RPC_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/rpc.h,v 1.9 1997-09-02 15:08:27 martin Exp $ */
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
 * Author: "Martin Kneissl" <martin@plt.rwth-aachen.de>
 */

#include "plt/debug.h"

#if PLT_SYSTEM_HPUX
extern "C" {
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <rpc/rpc.h>
#include <rpc/pmap_clnt.h>
extern int get_myaddress(struct sockaddr_in *);
};
#endif

#if PLT_SYSTEM_LINUX
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <rpc/rpc.h>
#include <rpc/pmap_clnt.h>
#endif

#if PLT_SYSTEM_NT
typedef unsigned char u_char;
#include <rpc/rpc.h>
#include <rpc/pmap_pro.h>
#include <rpc/pmap_cln.h>

#if PLT_COMPILER_MSVC
#pragma warning (disable : 4237 )  /* disable warning about defining bool... */
#endif

class KsNTOncRpc
{
public:
    KsNTOncRpc();
    ~KsNTOncRpc();
private:
    static int refcount;
};

static KsNTOncRpc ks_nt_oncrpc;

#endif // PLT_SYSTEM_NT

#if PLT_SYSTEM_SOLARIS
#include <sys/socket.h>
#include <rpc/rpc.h>
#include <rpc/svc.h>
#include <rpc/svc_soc.h>
#include <rpc/clnt_soc.h>
#include <rpc/pmap_clnt.h>
#endif

#if PLT_SYSTEM_OPENVMS
//
// This seems to be very order dependant!!!
// Don't put the rpc below the socket headers.
//
#include <rpc/rpc.h>
#include <rpc/pmap_clnt.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
// #include <rpc/clnt.h>
#endif

#endif // KS_RPC_INCLUDED
