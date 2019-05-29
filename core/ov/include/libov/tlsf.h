/*
 * Two Levels Segregate Fit memory allocator (TLSF)
 * Version 2.4.6
 *
 * Written by Miguel Masmano Tello <mimastel@doctor.upv.es>
 *
 * Thanks to Ismael Ripoll for his suggestions and reviews
 *
 * Copyright (C) 2008, 2007, 2006, 2005, 2004
 *
 * This code is released using a dual license strategy: GPL/LGPL
 * You can choose the licence that better fits your requirements.
 *
 * Released under the terms of the GNU General Public License Version 2.0
 * Released under the terms of the GNU Lesser General Public License Version 2.1
 *
 */

#ifndef _TLSF_H_
#define _TLSF_H_

#include <stddef.h>
#include <sys/types.h>

#if _DEBUG_TLSF_
#include <stdlib.h>
extern int tlsf_check_pool(void* _tlsf);
extern void dump_memory_region(unsigned char *mem_ptr, unsigned int size);
extern void print_tlsf(void* _tlsf);
extern void print_all_blocks(void* _tlsf);
#endif


typedef enum {
	ov_heap = 0,
	ov_database = 1
} ov_tlsf_pool;

extern size_t init_memory_pool(size_t, void *);
extern size_t get_used_size(void *);
extern size_t get_free_size(void *);
extern size_t get_pool_size(void *);
extern size_t get_max_size(void *);
extern double get_fragmentation(void *);
extern void destroy_memory_pool(void *);
extern size_t add_new_area(void *, size_t, void *);
extern void *malloc_ex(size_t, void *);
extern void free_ex(void *, void *);
extern void *realloc_ex(void *, size_t, void *);
extern void *calloc_ex(size_t, size_t, void *);

extern int tlsf_set_pool(ov_tlsf_pool pool, void* mem_pool);
extern void tlsf_set_static(bool_t isStatic, void* mem_pool);
extern int tlsf_move_pool(void* _tlsf, ptrdiff_t diff);

extern void *tlsf_activate(void);
extern void *tlsf_malloc(size_t size, ov_tlsf_pool pool);
extern void tlsf_free(void *ptr, ov_tlsf_pool pool);
extern void *tlsf_realloc(void *ptr, size_t size, ov_tlsf_pool pool);
extern void *tlsf_calloc(size_t nelem, size_t elem_size, ov_tlsf_pool pool);

#ifdef OV_COMPILE_LIBOV
#if OV_DEBUG || OV_VALGRIND
void tlsf_getPointerList(void* mem_pool, void*** list, size_t** sizes);
#else
#define tlsf_getPointerList(mem_pool, list, sizes)
#endif

#if OV_VALGRIND
void tlsf_valgrind_reinit_access(void* mem_pool);
void tlsf_valgrind_reinit_alloc(void* mem_pool);
int tlsf_valgrind_check_access(void* mem_pool);
#else
#define tlfs_valgrind_reinit_access(mem_pool)
#define tlsf_valgrind_reinit_alloc(mem_pool)
#define tlsf_valgrind_check_access(mem_pool)
#endif
#endif

#endif
