/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/xdrmemstream.h,v 1.4 1999-02-22 15:13:02 harald Exp $ */
/*
 * Copyright (c) 1998, 1999
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
 * xdrmemstream.h -- This module implements memory based XDR streams
 *                   which can grow (and shrink) as necessary. In contrast
 *                   to this Sun's genuine memory streams work only on
 *                   fixed-sized, pre-allocated memory buffers.
 *
 * Written by Harald Albrecht <harald@plt.rwth-aachen.de>
 */

#ifndef __XDRDYNAMICMEMORYSTREAM_H
#define __XDRDYNAMICMEMORYSTREAM_H

#if PLT_USE_BUFFERED_STREAMS

#include "ks/xdr.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Create and manage XDR dynamic memory streams.
 */
bool_t xdrmemstream_create(XDR *xdrs);
void   xdrmemstream_clear(XDR *xdrs);
void   xdrmemstream_rewind(XDR *xdrs, enum xdr_op op);

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
                                unsigned int *fragment_count);

/*
 * Some statistics and control functions...
 */
void   xdrmemstream_getusage(u_int *total, u_int *freepool);
bool_t xdrmemstream_controlusage(u_int fragmentsize, u_int quota,
	                         u_int watermark, u_int freepercentage);
void   xdrmemstream_freegarbage();

#ifdef __cplusplus
}
#endif

#endif /* PLT_USE_BUFFERED_STREAMS */
#endif

/* End of xdrmemstream.h */
