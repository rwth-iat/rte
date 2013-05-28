/*
 * gzip.c
 *
 *  Created on: 23.11.2012
 *      Author: george
 */

#ifndef KSHTTPD_DISABLE_GZIP

#include "libov/ov_string.h"
#include "libov/ov_database.h"

#include "ks_logfile.h"
#include "zlib.h"

/**
 *
 * @param body: a string that needs to be send
 * @param output: a string for the encoded-body that is send
 * @param length: a integer that is defined for the length of the body that needs to be compress and send
 * @return
 */

// the function gzip.c is making the compression of the body using the deflate algorithm for compressing it.The function contains
// 1 input and 2 outputs as parameters
// The zlib.h compression library provides in-memory compression and decompression functions,
// including integrity checks of the uncompressed data. This version of the library supports only one compression method (deflation).

OV_RESULT gzip(OV_STRING input, OV_STRING* output, OV_INT* length){
	z_stream zstr2;
	int slen = ov_string_getlength(input), dlen, rc;
	zstr2.zalloc = Z_NULL;               // The fields zalloc, zfree and opaque in strm must be initialized before the call.
	zstr2.zfree = Z_NULL;                // If zalloc and zfree are Z_NULL, then the default library- derived memory allocation routines are used.
	zstr2.opaque = Z_NULL;               // The application must update next_in and avail_in when avail_in has dropped to zero.
	zstr2.next_in = (unsigned char*)input; // It must update next_out and avail_out when avail_out has dropped to zero.
	zstr2.avail_in = slen;               // The application must initialize zalloc, zfree and opaque before calling the deflateinit2 function.


	// The compression level must be Z_DEFAULT_COMPRESSION, or between 0 and 9: 1 gives best speed, 9 gives best compression,
	// 0 gives no compression at all (the input data is simply copied a block at a time).
	// Z_DEFAULT_COMPRESSION requests a default compromise between speed and compression (currently equivalent to level 6).
	// add 16 to MAX_WBITS to enforce gzip format

	// deflateInit2 returns Z_OK if success if not, returns OV_ERR_GENERIC
	if (Z_OK != deflateInit2(&zstr2, Z_DEFAULT_COMPRESSION, Z_DEFLATED, MAX_WBITS + 16, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY)){
		return OV_ERR_GENERIC;
	}

    // deflateBound() returns an upper bound on the compressed size after deflation. It must be called after deflateInit2().
	// This would be used to allocate an output buffer for deflation in a single pass, and so would be called before deflateInit2().
	dlen = deflateBound(&zstr2,slen)+12;

	*output = ov_database_malloc(dlen);
	if (*output == NULL){
		KS_logfile_error(("Out of memory"));
		return OV_ERR_GENERIC;
	}

	zstr2.next_out = (unsigned char*)*output;
	zstr2.avail_out = dlen;
	if(Z_STREAM_END != (rc = deflate(&zstr2, Z_FINISH)))return OV_ERR_GENERIC;

	if(Z_OK != deflateEnd(&zstr2))return OV_ERR_GENERIC;

	*length = zstr2.total_out;

	return OV_ERR_OK;
}
    // deflate alg. compresses as much data as possible, and stops when the input buffer becomes empty or the output buffer becomes full.
	// It may introduce some output latency (reading input without producing any output) except when forced to flush.

#endif
