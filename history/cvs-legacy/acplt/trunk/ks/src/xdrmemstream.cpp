/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/xdrmemstream.cpp,v 1.6 1999-01-29 12:45:46 harald Exp $ */
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
 * xdrmemstream.c -- This module implements memory based XDR streams
 *                   which can grow (and shrink) as necessary. In contrast
 *                   to this, Sun's genuine memory streams work only on
 *                   fixed-sized, pre-allocated memory buffers.
 *
 * Written by Harald Albrecht <harald@plt.rwth-aachen.de>
 */

/*#define CNXDEBUG*/

#if PLT_USE_BUFFERED_STREAMS

#include "ks/xdrmemstream.h"
#include <stdlib.h>

#if PLT_SYSTEM_NT
/*
 * The usual thing that's different by design: M$. Now why can't they
 * even make a usuable WinSock?!
 */
#include <errno.h>
#define write(fd,p,l) send(fd,p,l,0)
#define read(fd,p,l) recv(fd,p,l,0)
#ifdef  EINVAL
#undef  EINVAL
#endif
#define EINVAL      WSAEINVAL
#ifdef  EINTR
#undef  EINTR
#endif
#define EINTR       WSAEINTR
#ifdef  EWOULDBLOCK
#undef  EWOULDBLOCK
#endif 
#define EWOULDBLOCK WSAEWOULDBLOCK

#else

#include <unistd.h>
#include <errno.h>

#if PLT_SYSTEM_OPENVMS
#include <string.h>
#endif

#endif

#ifdef CNXDEBUG
#include <iostream.h>
#endif


/* ---------------------------------------------------------------------------
 * Some constants which control default settings of the fragment (pool)
 */
#define MINFRAGMENTSIZE 256
#define DEFAULTFRAGMENTSIZE 8192

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


/* ---------------------------------------------------------------------------
 * The following variables hold some module-wide information about the
 * free fragments, fragment size (thus configurable at run-time), memory
 * usage and other unimportant things. You should only set them *BEFORE*
 * creating the first XDR dynamic stream, or you´ll be in deep trouble, pal.
 */
static u_int MemStreamBufferSize      = DEFAULTFRAGMENTSIZE;
static MemoryStreamFragment *FreeList = 0;
static u_int FragmentCount            = 0; /* total number = used + free */
static u_int FreeFragmentCount        = 0; /* number of free fragments   */
static u_int FragmentQuota            = (u_int) -1;
static u_int FragmentWatermark        = 0;
static u_int FreePercentage           = 50;


/* ---------------------------------------------------------------------------
 * Return memory (fragment) usage information. The information returned is
 * in bytes.
 */
void xdrmemstream_getusage(u_int *total, u_int *freepool)
{
    if ( total ) {
        *total = FragmentCount * (MemStreamBufferSize + 
                                  sizeof(MemoryStreamFragment) -
                                  sizeof(double));
    }
    if ( freepool ) {
        *freepool = FreeFragmentCount * (MemStreamBufferSize +
                                         sizeof(MemoryStreamFragment) -
                                         sizeof(double));
    }
} /* xdrmemstream_getusage */


/* ---------------------------------------------------------------------------
 * Set the size of memory fragments, the watermark and the percentage of
 * fragments to free for each freegarbage() step. The watermark indicates the
 * amount of fragments which aren´t free. While fragmentsize is in bytes, the
 * watermark is in fragments and the freepercentage is in percent (100 = 100%)
 * quota is the maximum of fragments that can be allocated at any time.
 */
bool_t xdrmemstream_controlusage(u_int fragmentsize, u_int quota,
	                         u_int watermark, u_int freepercentage)
{
    /*
     * Make sure that the fragment size can not be changed after the first
     * fragments have been allocated.
     */
    if ( (fragmentsize != MemStreamBufferSize) &&
	 FragmentCount ) {
	return false;
    }
    if ( fragmentsize < MINFRAGMENTSIZE ) {
	fragmentsize = MINFRAGMENTSIZE;
    }
    MemStreamBufferSize = fragmentsize;
    FragmentQuota       = quota;
    FragmentWatermark   = watermark;
    FreePercentage      = freepercentage;
    return true;
} /* xdrmemstream_controlusage */


/* ---------------------------------------------------------------------------
 * Allocate a new fragment and add it at the end of the list of fragments
 * of a XDR stream. The read/write pointer etc. is updated too.
 */
static MemoryStreamFragment *AllocateMemoryStreamFragment(XDR *xdrs)
{
    MemoryStreamFragment *fragment;

    if ( FreeList ) {
	/*
	 * There are still old fragments around, so we're recycling them.
	 */
	fragment = FreeList;
	FreeList = FreeList->next;
        --FreeFragmentCount;
    } else {
	/*
	 * We don't have a fragment at hand, so we must allocate a new one.
	 * The way the size of a fragment is allocated may seem odd at
	 * the first glimpse, but this way we can care for alignment
	 * restrictions. The head structure of a fragment therefore
	 * contains a dummy variable which just forces stright alignment.
	 * This dummy isn't used but instead the we'll store the contents
	 * beginning with this memory cell.
	 * But first check for our quotas...
	 */
	if ( FragmentCount >= FragmentQuota ) {
	    return 0;
	}
	fragment = (MemoryStreamFragment *)
	               malloc(sizeof(MemoryStreamFragment) - sizeof(double)
			   + MemStreamBufferSize);
        ++FragmentCount;
    }
    if ( fragment ) {
	/*
	 * If we got a fragment, then we'll add it at the end of the
	 * fragment list for this stream. In addition, the read/write
	 * pointer and the length information is updated.
	 */
	if ( ((MemoryStreamInfo *) xdrs->x_base)->current ) {
	    ((MemoryStreamInfo *) xdrs->x_base)->current->next = fragment;
	}
	((MemoryStreamInfo *) xdrs->x_base)->current = fragment;
	xdrs->x_private = (caddr_t) &(fragment->dummy);
	xdrs->x_handy   = MemStreamBufferSize;
	fragment->next  = 0;
	fragment->used  = MemStreamBufferSize; /* this is only a forecast !! */
        /*
         * Update the statistics too...
         */
        ++((MemoryStreamInfo *) xdrs->x_base)->fragment_count;
    }
    return fragment;
} /* AllocateMemoryStreamFragment */


/* ---------------------------------------------------------------------------
 * Free a fragment. Usually this will only result in the fragment
 * being put to the head of the free fragment list.
 */
static void FreeMemoryStreamFragment(MemoryStreamFragment *fragment)
{
    if ( fragment ) {
	fragment->next = FreeList;
	FreeList       = fragment;
        ++FreeFragmentCount;
    }
} /* FreeMemoryStreamFragment */


/* ---------------------------------------------------------------------------
 * Free up left-over free fragments which are currently lurking around in the
 * free list.
 */
void xdrmemstream_freegarbage()
{
    if ( FreeFragmentCount > FragmentWatermark ) {
	MemoryStreamFragment *fragment;
	u_int toFree = ((FreeFragmentCount - FragmentWatermark)
		          * FreePercentage) / 100;
	if ( toFree == 0 ) {
	    toFree = FreeFragmentCount - FragmentWatermark;
	}
	for ( ; toFree; --toFree ) {
	    fragment = FreeList;
	    FreeList = fragment->next;
	    free(fragment);
	    --FreeFragmentCount;
	    --FragmentCount;
    	}
    }
} /* xdrmemstream_freegarbage */


/* ---------------------------------------------------------------------------
 * This is a helper function to advance the read/write pointer of
 * a XDR stream to the beginning of the next fragment. Call this
 * function only if you're sure that you are currently at the end
 * of a fragment and need to advance to the next one.
 */
static bool_t AdvanceToNextFragment(XDR *xdrs)
{
    /*
     * Let's see whether there's another fragment with data
     * waiting for us.
     */
    MemoryStreamFragment *next =
	((MemoryStreamInfo *) xdrs->x_base)->current->next;
    ((MemoryStreamInfo *) xdrs->x_base)->current = next;
    if ( next == 0 ) {
	xdrs->x_handy = 0; /* make sure that the next
			    * request will fail too!
			    */
	return FALSE;
    }
    xdrs->x_private = (caddr_t) &(next->dummy);
    xdrs->x_handy   = next->used;
    if ( xdrs->x_handy == 0 ) {
	return FALSE;
    }
    return TRUE;
} /* AdvanceToNextFragment */


/* ---------------------------------------------------------------------------
 * Now let's head for the implementation stuff for a XDR memory
 * stream.
 *
 * First, some signatures of the stuff we'll need below...
 */
static bool_t MemStreamGetLong(XDR *xdrs, long *lp);
static bool_t MemStreamPutLong(XDR *xdrs, long *lp);
static bool_t MemStreamGetBytes(XDR *xdrs, caddr_t addr, int len);
static bool_t MemStreamPutBytes(XDR *xdrs, caddr_t addr, int len);
static u_int  MemStreamGetPos(XDR *xdrs);
static bool_t MemStreamSetPos(XDR *xdrs, u_int pos);
static long * MemStreamInline(XDR *xdrs, int len);
static void   MemStreamDestroy(XDR *xdrs);


/* ---------------------------------------------------------------------------
 * XDR streams are objects with method pointers for reading and writing
 * bytes and integers from/to the stream's contents. When creating a new
 * memory stream, the stream object will get linked to this method table
 * (good morning, vtable!).
 */
#if PLT_SYSTEM_OPENVMS || PLT_SYSTEM_NT
#define FUNC(rt) (rt (*)(...))
#else
#define FUNC(rt)
#endif

static
#if defined(__cplusplus)
                   /* Arrrghhh. Someone at Sun was soooo lazy and defined */
    XDR::xdr_ops   /* the xdr operations structure within the XDR struct. */
#else              /* While this is will get into the global namespace    */
    struct xdr_ops /* with C, it will be -- naturally -- in a local name- */
#endif             /* space with C++. Sigh.                               */
        memstream_operations = {
    FUNC(bool_t) MemStreamGetLong,   /* retrieve a 32 bit integer in host order      */
    FUNC(bool_t) MemStreamPutLong,   /* store a 32 bit integer in host order         */
    FUNC(bool_t) MemStreamGetBytes,  /* retrieve some octets (multiple of 4)         */
    FUNC(bool_t) MemStreamPutBytes,  /* store some octets (multiple of 4)            */
    FUNC(u_int)  MemStreamGetPos,    /* */
    FUNC(bool_t) MemStreamSetPos,    /* */
    FUNC(long *) MemStreamInline,    /* get some space in the buffer for fast access */
    FUNC(void)   MemStreamDestroy    /* clean up the mess                            */
}; /* memstream_operations */


/* ---------------------------------------------------------------------------
 * Create a new XDR memory stream. This creation function is slightly
 * different from what Sun's xdrmem_create() does: you don't have to
 * allocate memory for the buffer, because we do this here for you. Also
 * you don't have to worry about the buffer size. We'll take care of
 * this too. Because we're this careful, you can create such a memory
 * stream only in the XDR_ENCODE mode: it'll begin its life empty, thus
 * there's nothing you could retrieve from the stream.
 */
bool_t xdrmemstream_create(XDR *xdrs)
{
    MemoryStreamInfo     *info;
    MemoryStreamFragment *fragment;

    /*
     * Don't get fooled by null pointers! We also allocate an info
     * structure which will then hold some info about the stream and
     * its fragments.
     */
    if ( xdrs == 0 ) {
	return FALSE;
    }
    info = (MemoryStreamInfo *) malloc(sizeof(MemoryStreamInfo));
    if ( info == 0 ) {
	return FALSE;
    }

    /*
     * Now set up the XDR stream object: set up the vtable, the operation mode
     * and the read/write pointers. In addition, allocate the first fragment
     * for the stream as we'll surely need it very soon.
     */
    info->current        = 0;
    info->fragment_count = 0;
    xdrs->x_base         = (caddr_t) info;    
    xdrs->x_op           = XDR_ENCODE;
    xdrs->x_ops          = &memstream_operations;
    fragment = AllocateMemoryStreamFragment(xdrs);
    if ( !fragment ) {
	/*
	 * We couldn't allocate another fragment. So we're destroying the stream
	 * and returning an error to the caller. It doesn't make sense to hope
	 * that there may be fragments available later...
	 */
	free(info);
	return FALSE;
    }    
    /*
     * Remember the start of the fragment chain for later use
     */
    info->first = fragment;
    return TRUE;
} /* xdrmemstream_create */


/* ---------------------------------------------------------------------------
 * Reset a XDR memory stream. This will free all fragments with the
 * exception of the first one and will reset the read/write pointer.
 * The stream mode then will be into the XDR_ENCODE mode, because it
 * doesn't contain any data yet (it's pretty much the same as stream
 * creation).
 */
void xdrmemstream_clear(XDR *xdrs)
{
    MemoryStreamFragment *fragment, *next;
    /*
     * Free all fragments with the exception of the first one. Also,
     * reset the read/write pointer.
     */
    fragment = ((MemoryStreamInfo *) xdrs->x_base)->first;
    ((MemoryStreamInfo *) xdrs->x_base)->current = fragment;

    xdrs->x_op      = XDR_ENCODE;
    xdrs->x_private = (caddr_t) &(fragment->dummy);
    xdrs->x_handy   = MemStreamBufferSize;
    fragment->used  = MemStreamBufferSize; /* this is only a forecast !! */

    fragment = fragment->next;
    while ( fragment ) {
        next = fragment->next;
        FreeMemoryStreamFragment(fragment);
        fragment = next;
    }
    ((MemoryStreamInfo *) xdrs->x_base)->first->next = 0;
    ((MemoryStreamInfo *) xdrs->x_base)->fragment_count = 1;
} /* xdrmemstream_clear */


/* ---------------------------------------------------------------------------
 * Rewind a XDR memory stream and change the operation mode.
 */
void xdrmemstream_rewind(XDR *xdrs, enum xdr_op op)
{
    MemoryStreamInfo *info = (MemoryStreamInfo *) xdrs->x_base;

    if ( op == XDR_ENCODE ) {
	xdrmemstream_clear(xdrs);
	return;
    }
    if ( xdrs->x_op == XDR_ENCODE ) {
	/*
	 * Fix up the length information in the last fragment. We'll
         * do this only if the stream was in XDR_ENCODE mode.
	 */
	info->current->used -= xdrs->x_handy;
	info->length = info->fragment_count * MemStreamBufferSize -
		          xdrs->x_handy;
    }
    /*
     * Reset the read/write pointer to the beginning of the first
     * fragment.
     */
    info->current        = info->first;
    xdrs->x_private      = (caddr_t) &(info->first->dummy);
    xdrs->x_handy        = info->first->used;
    xdrs->x_op           = op;
} /* xdrmemstream_rewind */


/* ---------------------------------------------------------------------------
 * Clean up the mess after a stream has died. This involves freeing the
 * fragments as well as the information block associated with this XDR
 * stream.
 */
static void MemStreamDestroy(XDR *xdrs)
{
    MemoryStreamFragment *fragment, *next;

    fragment = ((MemoryStreamInfo *) xdrs->x_base)->first;
    while ( fragment ) {
	next = fragment->next;
	FreeMemoryStreamFragment(fragment);
	fragment = next;
    }

    free(xdrs->x_base);
} /* MemStreamDestroy */


/* ---------------------------------------------------------------------------
 * Note that the caller *must* request a multiple of four bytes
 * or she/he will get in deep trouble the next time she/he
 * accesses the memory stream (due to misaligned pointers).
 */
static long * MemStreamInline(XDR *xdrs, int len)
{
    if ( xdrs->x_handy == 0 ) {
	/*
	 * If the current fragment has been filled up completely, then
	 * we try to allocate a new fragment. If this fails, then we'll
	 * return a null pointer to indicate this somehow.
	 */
	if ( AllocateMemoryStreamFragment(xdrs) == 0 ) {
	    return 0;
	}
    }
    if ( len <= xdrs->x_handy ) {
	long *space = (long *) xdrs->x_private;
	xdrs->x_private += len;
	xdrs->x_handy   -= len;
	return space;
    } else {
	/*
	 * Can not satisfy the request under any circumstances, as the
	 * caller requested more inline space than will fit into this
	 * fragment.
	 */
	return 0;
    }
} /* MemStreamInline */


/* ---------------------------------------------------------------------------
 * Retrieve a XDR int (that is, 32 bit wide integer) from the stream.
 * It is returned in host order whereas it was stored in the stream
 * in network order.
 */
static bool_t MemStreamGetLong(XDR *xdrs, long *lp)
{
    if ( xdrs->x_handy == 0 ) {
	if ( !AdvanceToNextFragment(xdrs) ) {
	    return FALSE;
	}
    }
    /*
     * The private pointer points now in every case to a 32 bit int
     * in the stream fragment. It is important to use the IXDR_GET_LONG
     * macro here to retrieve the 32 bit int as this macro is the only
     * way to do it platform-independant.
     */
#if PLT_COMPILER_DECCXX
    long *ppp = (long *) xdrs->x_private;
    *lp = IXDR_GET_LONG(ppp);
    xdrs->x_private = (caddr_t) ppp;
#else
    *lp = IXDR_GET_LONG(((long *) xdrs->x_private));
#endif
    xdrs->x_handy -= 4;
    return TRUE;
} /* MemStreamGetLong */


/* ---------------------------------------------------------------------------
 * Write a XDR int (as usual it's 32 bit wide integer) to the stream.
 */
static bool_t MemStreamPutLong(XDR *xdrs, long *lp)
{
    if ( xdrs->x_handy == 0 ) {
	/*
	 * This fragment has been used up. So we have to allocate
	 * a fresh one.
	 */
	MemoryStreamFragment *next = AllocateMemoryStreamFragment(xdrs);
	if ( next == 0 ) {
	    xdrs->x_handy = 0; /* make sure that the next
				* request will fail too!
				*/
	    return FALSE;
	}
    }
    /*
     * It is important to use the IXDR_PUT_LONG macro here to store
     * the 32 bit int as this macro is the only way to do it platform-
     * independant. The only gotch to watch out for is that you must
     * stuff a long * into it, regardless of the platform you're on.
     * The macro will take care on 64 bit platforms to cast that pointer
     * to a suitable pointer type in order to access a 32 bit integer
     * quantity.
     */
#if PLT_COMPILER_DECCXX
    long *ppp = (long *) xdrs->x_private;
    IXDR_PUT_LONG(ppp, *lp);
    xdrs->x_private = (caddr_t) ppp;
#else
    IXDR_PUT_LONG(((long *) xdrs->x_private), *lp);
#endif
    xdrs->x_handy -= 4;
    return TRUE;
} /* MemStreamPutLong */


/* ---------------------------------------------------------------------------
 * Retrieve some bytes (some might prefer the term "octets") from the
 * XDR memory stream. If you're not reading a multiple of 4 bytes, then
 * you *must* read some additional bytes so you end up with reading
 * a multiple of 4 bytes in total. Otherwise the next attempt to retrieve
 * a long might result in a bus error.
 */
static bool_t MemStreamGetBytes(XDR *xdrs, caddr_t addr, int len)
{
    while ( len != 0 ) {
	int count;

	if ( xdrs->x_handy == 0 ) {
	    if ( !AdvanceToNextFragment(xdrs) ) {
		return FALSE;
	    }
	}

	count = xdrs->x_handy;
	if ( len < count ) {
	    count = len;
	}
	memcpy(addr, xdrs->x_private, count);
	xdrs->x_private += count;
	xdrs->x_handy   -= count;
	addr            += count;
	len             -= count;
    }
    return TRUE;
} /* MemStreamGetBytes */


/* ---------------------------------------------------------------------------
 * Store some bytes (some might prefer the term "octets") from the
 * XDR memory stream.
 */
static bool_t MemStreamPutBytes(XDR *xdrs, caddr_t addr, int len)
{
    while ( len != 0 ) {
	int count;
	/*
	 * If we have used up the current fragment, then we'll need to allocate
	 * a fresh one to fill it up too.
	 */
	if ( xdrs->x_handy == 0 ) {
	    MemoryStreamFragment *next = AllocateMemoryStreamFragment(xdrs);
	    if ( next == 0 ) {
		return FALSE;
	    }
	}

	count = xdrs->x_handy;
	if ( len < count ) {
	    count = len;
	}
	memcpy(xdrs->x_private, addr, count);
	xdrs->x_private += count;
	xdrs->x_handy   -= count;
	addr            += count;
	len             -= count;
    }
    return TRUE;
} /* MemStreamPutBytes */


/* ---------------------------------------------------------------------------
 * Yet to be implemented...
 */
static u_int  MemStreamGetPos(XDR *)
{
    return 0;
} /* MemStreamGetPos */

static bool_t MemStreamSetPos(XDR *, u_int)
{
    return FALSE;
} /* MemStreamSetPos */


/* ---------------------------------------------------------------------------
 * Read as much data as possible from a file descriptor. But don´t read more
 * than *max bytes. Upon return *max is updated to reflect the amount of data
 * that couldn´t be read this time. So it´s easy for a caller to call us again
 * the next time with the outcomming of this call.
 */
bool_t xdrmemstream_read_from_fd(XDR *xdrs, int fd, int *max, int *err)
{
#if PLT_USE_XTI
    int flags;
#endif

    if ( xdrs->x_op != XDR_ENCODE ) {
    	if ( err ) {
	    *err = EINVAL;
	}
        return FALSE;
    }
    while ( *max != 0 ) {
	int count, count_read;
	/*
	 * If we have used up the current fragment, then we'll need to allocate
	 * a fresh one to fill it up too.
	 */
	if ( xdrs->x_handy == 0 ) {
	    MemoryStreamFragment *next = AllocateMemoryStreamFragment(xdrs);
	    if ( next == 0 ) {
	    	if ( err ) {
		    *err = ENOMEM;
		}
		return FALSE;
	    }
	}
        /*
         * Now try to read as much as you can into the fragment. The limit here
         * is either the remaining free space within the current fragment or
         * the number of bytes to be read in total with this call.
         */
	count = xdrs->x_handy;
	if ( *max < count ) {
	    count = *max;
	}
#if !PLT_USE_XTI
        count_read = read(fd, xdrs->x_private, count);
#else
    	count_read = t_rcv(fd, xdrs->x_private, count, &flags);
#endif
        if ( count_read < 0 ) {
            /*
             * Do not choke on interrupted system calls and not-yet-available
             * data.
             */
#if PLT_SYSTEM_NT
    	    int myerrno = WSAGetLastError();
#else
            int myerrno = errno;
#endif
#if PLT_USE_XTI
    	    /*
	     * Make sure that a pending event on the XTI endpoint is read.
	     * Especially handle connection release or disconnection. Yeah,
	     * with XTI you'll have to handle all things yourself. What a
	     * great deal.
	     */
    	    switch ( t_errno ) {
	    case TLOOK: {
	    	int events = t_look(fd);
		myerrno = EIO;
		if ( events & T_DISCONNECT ) {
		    t_rcvdis(fd, 0);
		    myerrno = EIO;
		}
		if ( events & T_ORDREL ) {
		    t_rcvrel(fd);
		    myerrno = EIO;
		}
		break;
	    	}
	    case TNODATA:
		myerrno = EWOULDBLOCK;
		return TRUE;
	    case TSYSERR:
		break; /* just take the ordinary errno */
	    default:
	    	myerrno = 0; /* indicate XTI error */
	    }
#endif
    	    if ( err ) {
	    	*err = myerrno;
	    }
	    /*
             * Bark loud, if there was a real socket error when we tried
             * to read data.
             */
            return ((myerrno == EINTR)
                    || (myerrno == EAGAIN) 
                    || (myerrno == EWOULDBLOCK)) ? TRUE : FALSE;
        }
	if ( count_read == 0 ) {
	    if ( err ) {
	    	*err = EIO; /* that's rather a fake error indication... */
	    }
	    return FALSE;
	}

	xdrs->x_private += count_read;
	xdrs->x_handy   -= count_read;
	*max            -= count_read;
        /*
         * If we couldn't read enough data yet, then we will return with
         * no error as the next read might deliver that data. In case of
	 * XTI, the transport has already indicated whether there is more
	 * data waiting to be read (some of the quite few goodies in XTI).
         */
#if !PLT_USE_XTI
        if ( count != count_read ) {
#else
    	if ( !(flags & T_MORE) ) {
#endif
            return TRUE;
        }
    }
    return TRUE;
} /* xdrmemstream_read_from_fd */


/* ---------------------------------------------------------------------------
 *
 */
bool_t xdrmemstream_get_length(XDR *xdrs, int *len)
{
    if ( len == 0 ) {
        return FALSE;
    }
    *len = ((MemoryStreamInfo *) xdrs->x_base)->length;
    return TRUE;
} /* xdrmemstream_get_length */


/* ---------------------------------------------------------------------------
 * Write as much data from the stream as possible to a file descriptor. The
 * upper limit is set by *max. On return, *max is updated to reflect how much
 * data has still not been written and need to be put into the pipe the next
 * time.
 */
bool_t xdrmemstream_write_to_fd(XDR *xdrs, int fd, int *max, int *err)
{
    if ( xdrs->x_op != XDR_DECODE ) {
    	if ( err ) {
	    *err = EINVAL;
	}
        return FALSE;
    }
    while ( *max != 0 ) {
	int count, count_written;
	/*
	 * If we have used up the current fragment, then we'll need to allocate
	 * a fresh one to fill it up too.
	 */
	if ( xdrs->x_handy == 0 ) {
	    if ( !AdvanceToNextFragment(xdrs) ) {
	    	if ( err ) {
		    *err = ENOMEM; /* fake this error... */
		}
		return FALSE;
	    }
	}
        /*
         * Now try to write as much as you can from the fragment. The limit here
         * is either the remaining data within the current fragment or the
         * number of bytes to be write in total with this call.
         */
	count = xdrs->x_handy;
	if ( *max <= count ) {
	    count = *max;
	}
#if !PLT_USE_XTI
        count_written = write(fd, xdrs->x_private, count);
#else
	count_written = t_snd(fd, xdrs->x_private, count, 0);
#endif
        if ( count_written < 0 ) {
            /*
             * Do not choke on interrupted system calls and not-yet-free
             * send buffers.
             */
#if PLT_SYSTEM_NT
    	    int myerrno = WSAGetLastError();
#else
            int myerrno = errno;
#endif
#if PLT_USE_XTI
    	    /*
	     * Make sure that a pending event on the XTI endpoint is read.
	     * Especially handle connection release or disconnection. Yeah,
	     * with XTI you'll have to handle all things yourself. What a
	     * great deal.
	     */
    	    switch ( t_errno ) {
	    case TLOOK: {
	    	int events = t_look(fd);
		myerrno = EIO;
		if ( events & T_DISCONNECT ) {
		    t_rcvdis(fd, 0);
		    myerrno = EPIPE;
		}
		if ( events & T_ORDREL ) {
		    t_rcvrel(fd);
		    myerrno = EPIPE;
		}
		break;
	    	}
	    case TFLOW:
		myerrno = EWOULDBLOCK;
		return TRUE;
	    case TSYSERR:
		break; /* just take the ordinary errno */
	    default:
	    	myerrno = 0; /* indicate XTI error */
	    }
#endif
    	    if ( err ) {
	    	*err = myerrno;
	    }
#ifdef CNXDEBUG
	    cout << "*** " << fd << ": could not send: ";
	    switch ( myerrno ) {
	    case EINTR:
		cout << "EINTR"; break;
	    case EWOULDBLOCK:
		cout << "EWOULDBLOCK"; break;
	    case EAGAIN:
		cout << "EAGAIN"; break;
	    default:
		cout << "Exxx: " << myerrno;
	    }
	    cout << endl;
#endif
	    /*
             * Bark loud, if there was a real socket error when we tried
             * to write data. In case the network buffers are full and the
             * system first needs to get rid off that data, we simply wait
             * for the buffers to become free again.
             */
            return ((myerrno == EINTR) 
                    || (myerrno == EAGAIN)
                    || (myerrno == EWOULDBLOCK)) ? TRUE : FALSE;
        }
	if ( count_written == 0 ) {
#ifdef CNXDEBUG
	    cout << "*** " << fd << ": could not send: 0 bytes sent" << endl;
#endif
	    if ( err ) {
	    	*err = EPIPE; /* that's rather a fake error indication... */
	    }
	    return FALSE;
	}

#ifdef CNXDEBUG
	    cout << "*** " << fd << ": sent " << count_written << " bytes" << endl;
#endif
	xdrs->x_private += count_written;
	xdrs->x_handy   -= count_written;
	*max            -= count_written;
        /*
         * If we couldn't write enough data yet, then we will return with
         * no error as the next write might deliver that data.
         */
        if ( count != count_written ) {
            return TRUE;
        }
    }
    return TRUE;
} /* xdrmemstream_write_to_fd */


#endif /* PLT_USE_BUFFERED_STREAMS */

/* End of xdrmemstream.c */
