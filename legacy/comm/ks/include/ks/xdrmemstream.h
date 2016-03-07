/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/xdrmemstream.h,v 1.10 2007-04-25 12:57:20 martin Exp $ */
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
                                unsigned long *fragment_count,
                                unsigned long *total_size);

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
