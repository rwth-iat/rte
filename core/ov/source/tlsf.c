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

/*
 * Code contributions:
 *
 * (Jul 28 2007)  Herman ten Brugge <hermantenbrugge@home.nl>:
 *
 * - Add 64 bit support. It now runs on x86_64 and solaris64.
 * - I also tested this on vxworks/32and solaris/32 and i386/32 processors.
 * - Remove assembly code. I could not measure any performance difference 
 *   on my core2 processor. This also makes the code more portable.
 * - Moved defines/typedefs from tlsf.h to tlsf.c
 * - Changed MIN_BLOCK_SIZE to sizeof (free_ptr_t) and BHDR_OVERHEAD to 
 *   (sizeof (bhdr_t) - MIN_BLOCK_SIZE). This does not change the fact 
 *    that the minumum size is still sizeof 
 *   (bhdr_t).
 * - Changed all C++ comment style to C style. (// -> /.* ... *./)
 * - Used ls_bit instead of ffs and ms_bit instead of fls. I did this to 
 *   avoid confusion with the standard ffs function which returns 
 *   different values.
 * - Created set_bit/clear_bit fuctions because they are not present 
 *   on x86_64.
 * - Added locking support + extra file target.h to show how to use it.
 * - Added get_used_size function (REMOVED in 2.4)
 * - Added rtl_realloc and rtl_calloc function
 * - Implemented realloc clever support.
 * - Added some test code in the example directory.
 * - Bug fixed (discovered by the rockbox project: www.rockbox.org).       
 *
 * (Oct 23 2006) Adam Scislowicz: 
 *
 * - Support for ARMv5 implemented
 *
 */

/*#define USE_SBRK        (0) */
/*#define USE_MMAP        (0) */

#define _BSD_SOURCE
#define _DEFAULT_SOURCE
#include "ov_config.h"

#ifndef USE_PRINTF
#define USE_PRINTF      (1)
#endif

#include <string.h>

#ifndef TLSF_USE_LOCKS
#define	TLSF_USE_LOCKS 	(0)
#endif

#ifndef TLSF_STATISTIC
#define	TLSF_STATISTIC 	(0)
#endif

#ifndef USE_MMAP
#define	USE_MMAP 	(0)
#endif

#ifndef USE_SBRK
#define	USE_SBRK 	(0)
#endif

#ifndef USE_VIRTALLOC
#define USE_VIRTALLOC (0)
#endif

#ifndef OV_RT
#define OV_RT (0)
#endif

#if TLSF_USE_LOCKS
#include <tlsf_target.h>
#else
#define TLSF_CREATE_LOCK(_unused_)   do{}while(0)
#define TLSF_DESTROY_LOCK(_unused_)  do{}while(0) 
#define TLSF_ACQUIRE_LOCK(_unused_)  do{}while(0)
#define TLSF_RELEASE_LOCK(_unused_)  do{}while(0)
#endif

#if TLSF_STATISTIC
#define	TLSF_ADD_SIZE(tlsf, b) do {									\
		tlsf->used_size += (b->size & BLOCK_SIZE) + BHDR_OVERHEAD;	\
		if (tlsf->used_size > tlsf->max_size) 						\
			tlsf->max_size = tlsf->used_size;						\
		tlsf->free_size = tlsf->pool_size - tlsf->used_size;		\
		} while(0)

#define	TLSF_REMOVE_SIZE(tlsf, b) do {								\
		tlsf->used_size -= (b->size & BLOCK_SIZE) + BHDR_OVERHEAD;	\
		tlsf->free_size = tlsf->pool_size - tlsf->used_size;		\
	} while(0)
#else
#define	TLSF_ADD_SIZE(tlsf, b)	     do{}while(0)
#define	TLSF_REMOVE_SIZE(tlsf, b)    do{}while(0)
#endif

#if USE_MMAP || USE_SBRK
#include <unistd.h>
#endif

#if USE_MMAP
#include <sys/mman.h>
#endif

#if USE_VIRTALLOC
#include <windows.h>
#endif

#include "tlsf.h"

#undef tlsf_t

#if !defined(__GNUC__)
#ifndef __inline__
#define __inline__
#endif
#endif

/* The  debug functions  only can  be used  when _DEBUG_TLSF_  is set. */
#ifndef _DEBUG_TLSF_
#define _DEBUG_TLSF_  (0)
#endif

/*************************************************************************/
/* Definition of the structures used by TLSF */


/* Some IMPORTANT TLSF parameters */
/* Unlike the preview TLSF versions, now they are statics */
#define BLOCK_ALIGN (sizeof(void *) * 2)

#define MAX_FLI		(30)
#define MAX_LOG2_SLI	(5)
#define MAX_SLI		(1 << MAX_LOG2_SLI)     /* MAX_SLI = 2^MAX_LOG2_SLI */

#define FLI_OFFSET	(6)     /* tlsf structure just will manage blocks bigger */
/* than 128 bytes */
#define SMALL_BLOCK	(128)
#define REAL_FLI	(MAX_FLI - FLI_OFFSET)
#define MIN_BLOCK_SIZE	(sizeof (free_ptr_t))
#define BHDR_OVERHEAD	(sizeof (bhdr_t) - MIN_BLOCK_SIZE)
#define FREE_BLOCK_SIZE	(sizeof (bhdr_t))
#define TLSF_SIGNATURE	(0x2A59FA59)

#define	PTR_MASK	(sizeof(void *) - 1)
#define BLOCK_SIZE	(0xFFFFFFFF - PTR_MASK)

#define GET_BLOCK_HEADER(_buffer) ((bhdr_t *) (((char *) (_buffer)) - BHDR_OVERHEAD))
#define GET_NEXT_BLOCK(_addr, _r) ((bhdr_t *) ((char *) (_addr) + (_r)))
#define	MEM_ALIGN		  ((BLOCK_ALIGN) - 1)
#define ROUNDUP_SIZE(_r)          (((_r) + MEM_ALIGN) & ~MEM_ALIGN)
#define ROUNDDOWN_SIZE(_r)        ((_r) & ~MEM_ALIGN)
#define ROUNDUP(_x, _v)           ((((~(_x)) + 1) & ((_v)-1)) + (_x))

#define BLOCK_STATE	(0x1)
#define PREV_STATE	(0x2)

/* bit 0 of the block size */
#define FREE_BLOCK	(0x1)
#define USED_BLOCK	(0x0)

/* bit 1 of the block size */
#define PREV_FREE	(0x2)
#define PREV_USED	(0x0)


#define DEFAULT_AREA_SIZE (1024*128)

#ifdef USE_MMAP
#define PAGE_SIZE (getpagesize())
#endif

#if USE_VIRTALLOC
#define VIRTALLOC_RESERVE (1024*1024*32)	/* reserved continuous memory */
static LPTSTR lpNextPage = NULL;
static DWORD dpagesize = 0; /* doubled pagesize */
#endif

/*
 * Valgrind integration defines
 */
#if OV_VALGRIND
#include <valgrind/memcheck.h>
#define TLSF_VG_UNLOCK_BHDR(_b) VALGRIND_MAKE_MEM_DEFINED((_b), BHDR_OVERHEAD)
#define TLSF_VG_UNLOCK_FBLOCK(_b) VALGRIND_MAKE_MEM_DEFINED((_b), FREE_BLOCK_SIZE)
#define TLSF_VG_LOCK_BHDR(_b) VALGRIND_MAKE_MEM_NOACCESS((_b), BHDR_OVERHEAD)
#define TLSF_VG_LOCK_FBLOCK(_b) VALGRIND_MAKE_MEM_NOACCESS((_b), FREE_BLOCK_SIZE)

#else
// define dummy macros for valgrind macros
#define TLSF_VG_UNLOCK_BHDR(_b)
#define TLSF_VG_UNLOCK_FBLOCK(_b)
#define TLSF_VG_LOCK_BHDR(_b)
#define TLSF_VG_LOCK_FBLOCK(_b)

#define VALGRIND_CREATE_MEMPOOL(a,b,c)
#define VALGRIND_MEMPOOL_ALLOC(a,b,c)
#define VALGRIND_MEMPOOL_FREE(a,b)
#define VALGRIND_MEMPOOL_CHANGE(a,b,c,d)

#define VALGRIND_MAKE_MEM_DEFINED(a,b)
#define VALGRIND_MAKE_MEM_UNDEFINED(a,b)
#define VALGRIND_MAKE_MEM_NOACCESS(a,b)

#define VALGRIND_DISABLE_ERROR_REPORTING
#define VALGRIND_ENABLE_ERROR_REPORTING

#endif

#ifdef USE_PRINTF
#include <stdio.h>
# define PRINT_MSG(fmt, ...) printf(fmt, ##__VA_ARGS__)
# define ERROR_MSG(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
# if !defined(PRINT_MSG)
#  define PRINT_MSG(fmt, args...)
# endif
# if !defined(ERROR_MSG)
#  define ERROR_MSG(fmt, args...)
# endif
#endif

typedef unsigned int u32_t;     /* NOTE: Make sure that this type is 4 bytes long on your computer */
typedef unsigned char u8_t;     /* NOTE: Make sure that this type is 1 byte on your computer */

typedef struct free_ptr_struct {
    struct bhdr_struct *prev;
    struct bhdr_struct *next;
} free_ptr_t;

typedef struct bhdr_struct {
    /* This pointer is just valid if the first bit of size is set */
    struct bhdr_struct *prev_hdr;
    /* The size is stored in bytes */
    size_t size;                /* bit 0 indicates whether the block is used and */
    /* bit 1 allows to know whether the previous block is free */
    union {
        struct free_ptr_struct free_ptr;
        u8_t buffer[1];         /*sizeof(struct free_ptr_struct)]; */
    } ptr;
} bhdr_t;

/* This structure is embedded at the beginning of each area, giving us
 * enough information to cope with a set of areas */

typedef struct area_info_struct {
    bhdr_t *end;
    struct area_info_struct *next;
} area_info_t;

typedef struct TLSF_struct {
    /* the TLSF's structure signature */
    u32_t tlsf_signature;

    u8_t isStatic;

#if TLSF_USE_LOCKS
    TLSF_MLOCK_T lock;
#endif

#if TLSF_STATISTIC
    /* These can not be calculated outside tlsf because we
     * do not know the sizes when freeing/reallocing memory. */
    size_t pool_size;
    size_t used_size;
    size_t free_size;
    size_t free_fragment;
    size_t max_size;
#endif

    /* A linked list holding all the existing areas */
    area_info_t *area_head;

    /* the first-level bitmap */
    /* This array should have a size of REAL_FLI bits */
    u32_t fl_bitmap;

    /* the second-level bitmap */
    u32_t sl_bitmap[REAL_FLI];

    bhdr_t *matrix[REAL_FLI][MAX_SLI];
} tlsf_t;


/*
 * function definitions for valgrind based functions
 */
#if OV_VALGRIND
static size_t tlsf_valgrind_getAllocedSize(bhdr_t* b);
void tlsf_valgrind_reinit_access(void* mem_pool);
void tlsf_valgrind_reinit_alloc(void* mem_pool);
int tlsf_valgrind_check_access(void* mem_pool);

#else
#define tlsf_valgrind_getAllocedSize(_b) ((_b)->size&BLOCK_SIZE)
#define tlsf_valgrind_reinit_access(mem_pool)
#define tlsf_valgrind_reinit_alloc(mem_pool)
#define tlsf_valgrind_check_access(mem_pool)
#endif


/******************************************************************/
/**************     Helping functions    **************************/
/******************************************************************/
static __inline__ void set_bit(int nr, u32_t * addr);
static __inline__ void clear_bit(int nr, u32_t * addr);
static __inline__ int ls_bit(int x);
static __inline__ int ms_bit(int x);
static __inline__ void MAPPING_SEARCH(size_t * _r, int *_fl, int *_sl);
static __inline__ void MAPPING_INSERT(size_t _r, int *_fl, int *_sl);
static __inline__ bhdr_t *FIND_SUITABLE_BLOCK(tlsf_t * _tlsf, int *_fl, int *_sl);
static __inline__ bhdr_t *process_area(void *area, size_t size);
#if USE_SBRK || USE_MMAP || USE_VIRTALLOC
static __inline__ void *get_new_area(size_t * size);
#endif

static const int table[] = {
    -1, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4,
    4, 4,
    4, 4, 4, 4, 4, 4, 4,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    5,
    5, 5, 5, 5, 5, 5, 5,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6,
    6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6,
    6, 6, 6, 6, 6, 6, 6,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7,
    7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7,
    7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7,
    7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7,
    7, 7, 7, 7, 7, 7, 7
};

static __inline__ int ls_bit(int i)
{
    unsigned int a;
    unsigned int x = i & -i;

    a = x <= 0xffff ? (x <= 0xff ? 0 : 8) : (x <= 0xffffff ? 16 : 24);
    return table[x >> a] + a;
}

static __inline__ int ms_bit(int i)
{
    unsigned int a;
    unsigned int x = (unsigned int) i;

    a = x <= 0xffff ? (x <= 0xff ? 0 : 8) : (x <= 0xffffff ? 16 : 24);
    return table[x >> a] + a;
}

static __inline__ void set_bit(int nr, u32_t * addr)
{
    addr[nr >> 5] |= 1 << (nr & 0x1f);
}

static __inline__ void clear_bit(int nr, u32_t * addr)
{
    addr[nr >> 5] &= ~(1 << (nr & 0x1f));
}

static __inline__ void MAPPING_SEARCH(size_t * _r, int *_fl, int *_sl)
{
    int _t;

    if (*_r < SMALL_BLOCK) {
        *_fl = 0;
        *_sl = *_r / (SMALL_BLOCK / MAX_SLI);
    } else {
        _t = (1 << (ms_bit(*_r) - MAX_LOG2_SLI)) - 1;
        *_r = *_r + _t;
        *_fl = ms_bit(*_r);
        *_sl = (*_r >> (*_fl - MAX_LOG2_SLI)) - MAX_SLI;
        *_fl -= FLI_OFFSET;
        /*if ((*_fl -= FLI_OFFSET) < 0) // FL wil be always >0!
         *_fl = *_sl = 0;
         */
        *_r &= ~_t;
    }
}

static __inline__ void MAPPING_INSERT(size_t _r, int *_fl, int *_sl)
{
    if (_r < SMALL_BLOCK) {
        *_fl = 0;
        *_sl = _r / (SMALL_BLOCK / MAX_SLI);
    } else {
        *_fl = ms_bit(_r);
        *_sl = (_r >> (*_fl - MAX_LOG2_SLI)) - MAX_SLI;
        *_fl -= FLI_OFFSET;
    }
}


static __inline__ bhdr_t *FIND_SUITABLE_BLOCK(tlsf_t * _tlsf, int *_fl, int *_sl)
{
    u32_t _tmp = _tlsf->sl_bitmap[*_fl] & (~0 << *_sl);
    bhdr_t *_b = NULL;

    if (_tmp) {
        *_sl = ls_bit(_tmp);
        _b = _tlsf->matrix[*_fl][*_sl];
    } else {
        *_fl = ls_bit(_tlsf->fl_bitmap & (~0 << (*_fl + 1)));
        if (*_fl > 0) {         /* likely */
            *_sl = ls_bit(_tlsf->sl_bitmap[*_fl]);
            _b = _tlsf->matrix[*_fl][*_sl];
        }
    }
    return _b;
}


#define EXTRACT_BLOCK_HDR(_b, _tlsf, _fl, _sl) do {					\
		_tlsf -> matrix [_fl] [_sl] = _b -> ptr.free_ptr.next;		\
		if (_tlsf -> matrix[_fl][_sl]) {							\
			TLSF_VG_UNLOCK_FBLOCK(_tlsf->matrix[_fl][_sl]);			\
			_tlsf -> matrix[_fl][_sl] -> ptr.free_ptr.prev = NULL;	\
			TLSF_VG_LOCK_FBLOCK(_tlsf->matrix[_fl][_sl]);			 \
		} else {													\
			clear_bit (_sl, &_tlsf -> sl_bitmap [_fl]);				\
			if (!_tlsf -> sl_bitmap [_fl])							\
				clear_bit (_fl, &_tlsf -> fl_bitmap);				\
		}															\
		_b -> ptr.free_ptr.prev =  NULL;				\
		_b -> ptr.free_ptr.next =  NULL;				\
	}while(0)


#define EXTRACT_BLOCK(_b, _tlsf, _fl, _sl) do {							\
		if (_b -> ptr.free_ptr.next){									\
			TLSF_VG_UNLOCK_FBLOCK(_b->ptr.free_ptr.next);				\
			_b -> ptr.free_ptr.next -> ptr.free_ptr.prev = _b -> ptr.free_ptr.prev; \
			TLSF_VG_LOCK_FBLOCK(_b->ptr.free_ptr.next);					\
		} 																\
		if (_b -> ptr.free_ptr.prev){									\
			TLSF_VG_UNLOCK_FBLOCK(_b->ptr.free_ptr.prev);				\
			_b -> ptr.free_ptr.prev -> ptr.free_ptr.next = _b -> ptr.free_ptr.next; \
			TLSF_VG_LOCK_FBLOCK(_b->ptr.free_ptr.prev);					\
		}																\
		if (_tlsf -> matrix [_fl][_sl] == _b) {							\
			_tlsf -> matrix [_fl][_sl] = _b -> ptr.free_ptr.next;		\
			if (!_tlsf -> matrix [_fl][_sl]) {							\
				clear_bit (_sl, &_tlsf -> sl_bitmap[_fl]);				\
				if (!_tlsf -> sl_bitmap [_fl])							\
					clear_bit (_fl, &_tlsf -> fl_bitmap);				\
			}															\
		}																\
		_b -> ptr.free_ptr.prev = NULL;					\
		_b -> ptr.free_ptr.next = NULL;					\
	} while(0)

#define INSERT_BLOCK(_b, _tlsf, _fl, _sl) do {							\
		_b -> ptr.free_ptr.prev = NULL;									\
		_b -> ptr.free_ptr.next = _tlsf -> matrix [_fl][_sl]; 			\
		if (_tlsf -> matrix [_fl][_sl]){								\
			TLSF_VG_UNLOCK_FBLOCK(_tlsf->matrix[_fl][_sl]);				\
			_tlsf -> matrix [_fl][_sl] -> ptr.free_ptr.prev = _b;		\
			TLSF_VG_LOCK_FBLOCK(_tlsf->matrix[_fl][_sl]);				\
		}																\
		_tlsf -> matrix [_fl][_sl] = _b;								\
		set_bit (_sl, &_tlsf -> sl_bitmap [_fl]);						\
		set_bit (_fl, &_tlsf -> fl_bitmap);								\
	} while(0)

#if USE_SBRK || USE_MMAP || USE_VIRTALLOC
static __inline__ void *get_new_area(size_t * size) 
{
    void *area;

#if USE_VIRTALLOC
    if(!lpNextPage){
    	SYSTEM_INFO sysInf;
    	GetSystemInfo(&sysInf);
    	dpagesize = 2 * sysInf.dwPageSize;
    	lpNextPage = VirtualAlloc(NULL, VIRTALLOC_RESERVE,
    			MEM_RESERVE, PAGE_NOACCESS);
    }

    *size = ROUNDUP(*size, dpagesize);
    area = VirtualAlloc(lpNextPage, *size, MEM_COMMIT, PAGE_READWRITE);

    if(!area){
    	/*
    	 * remaining reserved block too small
    	 * allocate separate block
    	 */
    	area = VirtualAlloc(NULL, *size, MEM_COMMIT, PAGE_READWRITE);
    }

    if(area){
    	lpNextPage = (LPTSTR)(((char*)area)+*size);
    	return area;
    }
#endif

#if USE_SBRK
    area = (void *)sbrk(0);
    if (((void *)sbrk(*size)) != ((void *) -1))
        return area;
#endif

#if USE_MMAP
#ifndef MAP_ANONYMOUS
/* https://dev.openwrt.org/ticket/322 */
# define MAP_ANONYMOUS MAP_ANON
#endif

    *size = ROUNDUP(*size, PAGE_SIZE);
    if ((area = mmap(0, *size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) != MAP_FAILED)
        return area;
#endif
    return ((void *) ~0);
}
#endif

static __inline__ bhdr_t *process_area(void *area, size_t size)
{
    bhdr_t *b, *lb, *ib;
    area_info_t *ai;

    ib = (bhdr_t *) area;
    ib->size =
        (sizeof(area_info_t) <
         MIN_BLOCK_SIZE) ? MIN_BLOCK_SIZE : ROUNDUP_SIZE(sizeof(area_info_t)) | USED_BLOCK | PREV_USED;
    b = (bhdr_t *) GET_NEXT_BLOCK(ib->ptr.buffer, ib->size & BLOCK_SIZE);
    b->size = ROUNDDOWN_SIZE(size - 3 * BHDR_OVERHEAD - (ib->size & BLOCK_SIZE)) | USED_BLOCK | PREV_USED;
    b->ptr.free_ptr.prev = b->ptr.free_ptr.next = 0;
    lb = GET_NEXT_BLOCK(b->ptr.buffer, b->size & BLOCK_SIZE);
    lb->prev_hdr = b;
    lb->size = 0 | USED_BLOCK | PREV_FREE;
    ai = (area_info_t *) ib->ptr.buffer;
    ai->next = 0;
    ai->end = lb;
    VALGRIND_MAKE_MEM_NOACCESS(b, (b->size&BLOCK_SIZE) + BHDR_OVERHEAD);
    TLSF_VG_LOCK_BHDR(lb);
    return ib;
}

/******************************************************************/
/******************** Begin of the allocator code *****************/
/******************************************************************/

static char* mp[2] = {NULL, NULL};         /* Memory pools */

/******************************************************************/
size_t init_memory_pool(size_t mem_pool_size, void *mem_pool)
{
/******************************************************************/
    tlsf_t *tlsf;
    bhdr_t *b, *ib;
    size_t size = 0;

    if (!mem_pool || !mem_pool_size || mem_pool_size < sizeof(tlsf_t) + BHDR_OVERHEAD * 8) {
        ERROR_MSG("init_memory_pool (): memory_pool invalid\n");
        return -1;
    }

    if (((uintptr_t) mem_pool & PTR_MASK)) {
        ERROR_MSG("init_memory_pool (): mem_pool must be aligned to a word\n");
        return -1;
    }
    tlsf = (tlsf_t *) mem_pool;
    VALGRIND_CREATE_MEMPOOL(tlsf, 0, FALSE);
    /* Check if already initialised */
    VALGRIND_MAKE_MEM_DEFINED(&tlsf->tlsf_signature, sizeof(tlsf->tlsf_signature));
    if (tlsf->tlsf_signature == TLSF_SIGNATURE) {
#if TLSF_USE_LOCKS
        // reset lock
        memset(&tlsf->lock, 0, sizeof(tlsf->lock));
        TLSF_CREATE_LOCK(&tlsf->lock);
#endif
        VALGRIND_MAKE_MEM_DEFINED(tlsf, sizeof(tlsf_t));
        b = GET_NEXT_BLOCK(mem_pool, ROUNDUP_SIZE(sizeof(tlsf_t)));
        size = b->size & BLOCK_SIZE;
        tlsf_valgrind_reinit_alloc(mem_pool);
        tlsf_valgrind_reinit_access(mem_pool);
        return size;
    }

    /* Zeroing the memory pool */
    memset(mem_pool, 0, sizeof(tlsf_t));

    tlsf->tlsf_signature = TLSF_SIGNATURE;

    TLSF_CREATE_LOCK(&tlsf->lock);

    ib = process_area(GET_NEXT_BLOCK
                      (mem_pool, ROUNDUP_SIZE(sizeof(tlsf_t))), ROUNDDOWN_SIZE(mem_pool_size - sizeof(tlsf_t)));
    b = GET_NEXT_BLOCK(ib->ptr.buffer, ib->size & BLOCK_SIZE);
    VALGRIND_MEMPOOL_ALLOC(mem_pool, b->ptr.buffer, MIN_BLOCK_SIZE); // pseudo alloc so we can free
    free_ex(b->ptr.buffer, tlsf);
    tlsf->area_head = (area_info_t *) ib->ptr.buffer;
    VALGRIND_MAKE_MEM_NOACCESS(ib, BHDR_OVERHEAD+sizeof(area_info_t));

#if TLSF_STATISTIC
    TLSF_VG_UNLOCK_BHDR(b);
    tlsf->pool_size = mem_pool_size;
    tlsf->free_size = (b->size & BLOCK_SIZE);
    tlsf->free_fragment = tlsf->free_size;
    tlsf->used_size = mem_pool_size - tlsf->free_size;
    tlsf->max_size = tlsf->used_size;
#endif

    size = (b->size & BLOCK_SIZE);
    TLSF_VG_LOCK_BHDR(b);
    return size;
}

/******************************************************************/
size_t add_new_area(void *area, size_t area_size, void *mem_pool)
{
/******************************************************************/
    tlsf_t *tlsf = (tlsf_t *) mem_pool;
    area_info_t *ptr, *ptr_prev, *ai;
    bhdr_t *ib0, *b0, *lb0, *ib1, *b1, *lb1, *next_b;
    size_t size = 0;

    memset(area, 0, area_size);
    ptr = tlsf->area_head;
    ptr_prev = 0;

    ib0 = process_area(area, area_size);
    b0 = GET_NEXT_BLOCK(ib0->ptr.buffer, ib0->size & BLOCK_SIZE);

    TLSF_VG_UNLOCK_BHDR(b0);
    lb0 = GET_NEXT_BLOCK(b0->ptr.buffer, b0->size & BLOCK_SIZE);
    TLSF_VG_UNLOCK_BHDR(lb0);

    /* Before inserting the new area, we have to merge this area with the
       already existing ones */

    while (ptr) {
        VALGRIND_MAKE_MEM_DEFINED(ptr, sizeof(area_info_t));

        ib1 = (bhdr_t *) ((char *) ptr - BHDR_OVERHEAD);
        TLSF_VG_UNLOCK_BHDR(ib1);
        b1 = GET_NEXT_BLOCK(ib1->ptr.buffer, ib1->size & BLOCK_SIZE);
        TLSF_VG_UNLOCK_BHDR(b1);
        lb1 = ptr->end;
        TLSF_VG_UNLOCK_BHDR(lb1);

        /* Merging the new area with the next physically contigous one */
        if ((uintptr_t) ib1 == (uintptr_t) lb0 + BHDR_OVERHEAD) {
            if (tlsf->area_head == ptr) {
                tlsf->area_head = ptr->next;
                ai = ptr;
                ptr = ptr->next;
                VALGRIND_MAKE_MEM_NOACCESS(ai, sizeof(area_info_t));
            } else {
                ptr_prev->next = ptr->next;
                ai = ptr;
                ptr = ptr->next;
                VALGRIND_MAKE_MEM_NOACCESS(ai, sizeof(area_info_t));
            }

            b0->size =
                ROUNDDOWN_SIZE((b0->size & BLOCK_SIZE) +
                               (ib1->size & BLOCK_SIZE) + 2 * BHDR_OVERHEAD) | USED_BLOCK | PREV_USED;

            b1->prev_hdr = b0;
            lb0 = lb1;
            TLSF_VG_LOCK_BHDR(ib1);
            TLSF_VG_LOCK_BHDR(b1);
            //old lb1 is in b0
            continue;
        }

        /* Merging the new area with the previous physically contigous
           one */
        if ((uintptr_t) lb1->ptr.buffer == (uintptr_t) ib0) {
            if (tlsf->area_head == ptr) {
                tlsf->area_head = ptr->next;
                ptr = ptr->next;
            } else {
                ptr_prev->next = ptr->next;
                ptr = ptr->next;
            }

            lb1->size =
                ROUNDDOWN_SIZE((b0->size & BLOCK_SIZE) +
                               (ib0->size & BLOCK_SIZE) + 2 * BHDR_OVERHEAD) | USED_BLOCK | (lb1->size & PREV_STATE);
            next_b = GET_NEXT_BLOCK(lb1->ptr.buffer, lb1->size & BLOCK_SIZE);
            TLSF_VG_UNLOCK_BHDR(next_b);
            next_b->prev_hdr = lb1;
            TLSF_VG_LOCK_BHDR(next_b);
            b0 = lb1;
            ib0 = ib1;
            TLSF_VG_LOCK_BHDR(b1);
            // lb1 and ib1 need not be changed to no access
            continue;
        }
        TLSF_VG_LOCK_BHDR(ib1);
        TLSF_VG_LOCK_BHDR(b1);
        TLSF_VG_LOCK_BHDR(lb1);
        VALGRIND_MAKE_MEM_NOACCESS(ptr_prev, sizeof(area_info_t));
        ptr_prev = ptr;
        ptr = ptr->next;
    }
    VALGRIND_MAKE_MEM_NOACCESS(ptr_prev, sizeof(area_info_t));

    /* Inserting the area in the list of linked areas */
    ai = (area_info_t *) ib0->ptr.buffer;
    VALGRIND_MAKE_MEM_DEFINED(ai, sizeof(area_info_t));
    ai->next = tlsf->area_head;
    ai->end = lb0;
    VALGRIND_MAKE_MEM_NOACCESS(ai, sizeof(area_info_t));
    tlsf->area_head = ai;
    VALGRIND_MEMPOOL_ALLOC(mem_pool, b0->ptr.buffer, MIN_BLOCK_SIZE); // pseudo alloc so we can free
    free_ex(b0->ptr.buffer, mem_pool);
#if TLSF_STATISTIC
    // add area size to pool size
    tlsf->pool_size += area_size;
    // adjust used size changed free_ex
    tlsf->used_size += area_size;
#endif
    TLSF_VG_UNLOCK_BHDR(b0);
    size = (b0->size & BLOCK_SIZE);
    VALGRIND_MAKE_MEM_NOACCESS(ib0, BHDR_OVERHEAD+sizeof(area_info_t));
    TLSF_VG_LOCK_BHDR(b0);
    TLSF_VG_LOCK_BHDR(lb0);
    return size;
}


/******************************************************************/
size_t get_used_size(void *mem_pool)
{
/******************************************************************/
#if TLSF_STATISTIC
	if(!mem_pool)
		return 0;
    return ((tlsf_t *) mem_pool)->used_size;
#else
    return 0;
#endif
}


/******************************************************************/
size_t get_free_size(void *mem_pool)
{
/******************************************************************/
#if TLSF_STATISTIC
	if(!mem_pool)
		return 0;
    return ((tlsf_t *) mem_pool)->free_size;
#else
    return 0;
#endif
}


/******************************************************************/
double get_fragmentation(void *mem_pool)
{
/******************************************************************/
#if TLSF_STATISTIC
	tlsf_t* tlsf = (tlsf_t*) mem_pool;
	size_t fragment_size = 0;
	bhdr_t* bh = NULL;
	bhdr_t* bhnext = NULL;
	int fl, sl;

	if(!mem_pool)
		return 0.0;

	TLSF_ACQUIRE_LOCK(&tlsf->lock);

	fl = ms_bit(tlsf->fl_bitmap);
	sl = ms_bit(tlsf->sl_bitmap[fl]);

	bh = tlsf->matrix[fl][sl];
	while(bh){
		TLSF_VG_UNLOCK_FBLOCK(bh);
		if(fragment_size<(bh->size&BLOCK_SIZE))
			fragment_size = (bh->size&BLOCK_SIZE);
#if OV_RT
		// break loop to maintain real time
		// fragmentation may not be exact
		bh = NULL;
		TLSF_VG_LOCK_FBLOCK(bh);
#else
		bhnext = bh->ptr.free_ptr.next;
		TLSF_VG_LOCK_FBLOCK(bh);
		bh = bhnext;
#endif
	}

	TLSF_RELEASE_LOCK(&tlsf->lock);

    return 1.0 - ((double)fragment_size)/((double)(tlsf->free_size));
#else
    return 0.0;
#endif
}


/******************************************************************/
size_t get_pool_size(void *mem_pool)
{
/******************************************************************/
#if TLSF_STATISTIC
	if(!mem_pool)
		return 0;
    return ((tlsf_t *) mem_pool)->pool_size;
#else
    return 0;
#endif
}


/******************************************************************/
size_t get_max_size(void *mem_pool)
{
/******************************************************************/
#if TLSF_STATISTIC
	if(!mem_pool)
		return 0;
    return ((tlsf_t *) mem_pool)->max_size;
#else
    return 0;
#endif
}

/******************************************************************/
void destroy_memory_pool(void *mem_pool)
{
/******************************************************************/
    tlsf_t *tlsf = (tlsf_t *) mem_pool;

    tlsf->tlsf_signature = 0;

    TLSF_DESTROY_LOCK(&tlsf->lock);

//    VALGRIND_DESTROY_MEMPOOL(mem_pool);

}


__inline__ void* tlsf_activate(void) {
#if USE_MMAP || USE_SBRK || USE_VIRTALLOC
    if (!mp[0]) {
        size_t area_size;
        void *area;

        area_size = sizeof(tlsf_t) + BHDR_OVERHEAD * 8; /* Just a safety constant */
        area_size = (area_size > DEFAULT_AREA_SIZE) ? area_size : DEFAULT_AREA_SIZE;
        area = get_new_area(&area_size);
        if (area == ((void *) ~0))
        	return NULL; /* Not enough system memory */
        init_memory_pool(area_size, area);
        mp[0] = area;
    }
#endif
    return mp[0];
}

/******************************************************************/
int tlsf_set_pool(ov_tlsf_pool pool, void* mem_pool)
{
/******************************************************************/
	switch(pool) {
	case ov_heap:
	case ov_database:
		mp[pool] = mem_pool;
		break;
	default:
		return 1;
	}
	return 0;
}


/******************************************************************/
void tlsf_set_static(bool_t isStatic, void* mem_pool)
{
/******************************************************************/
	((tlsf_t*) mem_pool)->isStatic = isStatic;
}

/******************************************************************/
void *tlsf_malloc(size_t size, ov_tlsf_pool pool)
{
/******************************************************************/
    void *ret;

    switch(pool) {
    case ov_heap:
    case ov_database:
    	break;
    default:
    	return NULL;
    }

    if(!mp[pool]
#if USE_MMAP || USE_SBRK || USE_VIRTALLOC
		   && pool!=ov_heap
#endif
    )
    	return NULL;


    tlsf_activate();

    TLSF_ACQUIRE_LOCK(&((tlsf_t *)mp[pool])->lock);

    ret = malloc_ex(size, mp[pool]);

    TLSF_RELEASE_LOCK(&((tlsf_t *)mp[pool])->lock);

    return ret;
}

/******************************************************************/
void tlsf_free(void *ptr, ov_tlsf_pool pool)
{
/******************************************************************/

    switch(pool) {
    case ov_heap:
    case ov_database:
    	break;
    default:
    	return;
    }

	if(!mp[pool])
		return;

    tlsf_activate();

    TLSF_ACQUIRE_LOCK(&((tlsf_t *)mp[pool])->lock);

    free_ex(ptr, mp[pool]);

    TLSF_RELEASE_LOCK(&((tlsf_t *)mp[pool])->lock);

}

/******************************************************************/
void *tlsf_realloc(void *ptr, size_t size, ov_tlsf_pool pool)
{
/******************************************************************/
    void *ret;

    switch(pool) {
    case ov_heap:
    case ov_database:
    	break;
    default:
    	return NULL;
    }

    if(!mp[pool])
    	return NULL;

#if USE_MMAP || USE_SBRK
	if (!mp[pool]) {
		return tlsf_malloc(size, pool);
	}
#endif

    TLSF_ACQUIRE_LOCK(&((tlsf_t *)mp[pool])->lock);

    ret = realloc_ex(ptr, size, mp[pool]);

    TLSF_RELEASE_LOCK(&((tlsf_t *)mp[pool])->lock);

    return ret;
}

/******************************************************************/
void *tlsf_calloc(size_t nelem, size_t elem_size, ov_tlsf_pool pool)
{
/******************************************************************/
    void *ret;

    switch(pool) {
    case ov_heap:
    case ov_database:
    	break;
    default:
    	return NULL;
    }

    if(!mp[pool])
    	return NULL;

    TLSF_ACQUIRE_LOCK(&((tlsf_t *)mp[pool])->lock);

    ret = calloc_ex(nelem, elem_size, mp[pool]);

    TLSF_RELEASE_LOCK(&((tlsf_t *)mp[pool])->lock);

    return ret;
}

/******************************************************************/
void *malloc_ex(size_t size, void *mem_pool)
{
/******************************************************************/
    tlsf_t *tlsf = (tlsf_t *) mem_pool;
    bhdr_t *b, *b2, *next_b;
    int fl, sl;
    size_t tmp_size;

    size = (size < MIN_BLOCK_SIZE) ? MIN_BLOCK_SIZE : ROUNDUP_SIZE(size);

    /* Rounding up the requested size and calculating fl and sl */
    MAPPING_SEARCH(&size, &fl, &sl);

    /* Searching a free block, recall that this function changes the values of fl and sl,
       so they are not longer valid when the function fails */
    b = FIND_SUITABLE_BLOCK(tlsf, &fl, &sl);
#if USE_MMAP || USE_SBRK || USE_VIRTALLOC
    if (!b && !tlsf->isStatic) {
        size_t area_size;
        void *area;
        /* Growing the pool size when needed */
        area_size = (size+(size>>1)) + BHDR_OVERHEAD * 8;   /* size plus enough room for the requered headers. */
        area_size = (area_size > DEFAULT_AREA_SIZE) ? area_size : DEFAULT_AREA_SIZE;
        area = get_new_area(&area_size);        /* Call sbrk or mmap */
        if (area == ((void *) ~0))
            return NULL;        /* Not enough system memory */
        add_new_area(area, area_size, mem_pool);
        /* Rounding up the requested size and calculating fl and sl */
        MAPPING_SEARCH(&size, &fl, &sl);
        /* Searching a free block */
        b = FIND_SUITABLE_BLOCK(tlsf, &fl, &sl);
    }
#endif
    if (!b)
        return NULL;            /* Not found */

    TLSF_VG_UNLOCK_FBLOCK(b);
    EXTRACT_BLOCK_HDR(b, tlsf, fl, sl);

    /*-- found: */
    next_b = GET_NEXT_BLOCK(b->ptr.buffer, b->size & BLOCK_SIZE);
    TLSF_VG_UNLOCK_BHDR(next_b);
    /* Should the block be split? */
    tmp_size = (b->size & BLOCK_SIZE) - size;
    if (tmp_size >= sizeof(bhdr_t)) {
        tmp_size -= BHDR_OVERHEAD;
        b2 = GET_NEXT_BLOCK(b->ptr.buffer, size);
        TLSF_VG_UNLOCK_FBLOCK(b2);
        b2->size = tmp_size | FREE_BLOCK | PREV_USED;
        next_b->prev_hdr = b2;
        MAPPING_INSERT(tmp_size, &fl, &sl);
        INSERT_BLOCK(b2, tlsf, fl, sl);
        VALGRIND_MAKE_MEM_NOACCESS(b2, (b2->size&BLOCK_SIZE) + BHDR_OVERHEAD);

        b->size = size | (b->size & PREV_STATE);
    } else {
        next_b->size &= (~PREV_FREE);
        b->size &= (~FREE_BLOCK);       /* Now it's used */
    }
    TLSF_ADD_SIZE(tlsf, b);
    TLSF_VG_LOCK_BHDR(b);
    TLSF_VG_LOCK_BHDR(next_b);

    VALGRIND_MAKE_MEM_UNDEFINED(b->ptr.buffer, size);
    VALGRIND_MEMPOOL_ALLOC(mem_pool, b->ptr.buffer, size);

    return (void *) b->ptr.buffer;
}

/* EDB Added. */


size_t tlsf_get_object_size (void * ptr);
void tlsf_lock (void);
void tlsf_unlock (void);

size_t get_object_size_ex (void * ptr, void * mem_pool);

size_t get_object_size_ex (void * ptr, void * mem_pool)
{
  bhdr_t *b;

  if (!ptr) {
    return 0;
  }
  b = (bhdr_t *) ((char *) ptr - BHDR_OVERHEAD);
  return b->size & (size_t) ~1UL;
}

size_t tlsf_get_object_size (void * ptr)
{
  tlsf_activate();
  return get_object_size_ex (ptr, mp);
}

void tlsf_lock (void) {
    TLSF_ACQUIRE_LOCK(&((tlsf_t *)mp)->lock);
}

void tlsf_unlock (void) {
    TLSF_RELEASE_LOCK(&((tlsf_t *)mp)->lock);
}


/**/


/******************************************************************/
void free_ex(void *ptr, void *mem_pool)
{
/******************************************************************/
    tlsf_t *tlsf = (tlsf_t *) mem_pool;
    bhdr_t *b, *tmp_b;
    int fl = 0, sl = 0;

    if (!ptr) {
        return;
    }
    b = (bhdr_t *) ((char *) ptr - BHDR_OVERHEAD);
    TLSF_VG_UNLOCK_FBLOCK(b);
    VALGRIND_MAKE_MEM_NOACCESS(b->ptr.buffer+MIN_BLOCK_SIZE, (b->size&BLOCK_SIZE)-MIN_BLOCK_SIZE);
    b->size |= FREE_BLOCK;

    TLSF_REMOVE_SIZE(tlsf, b);

    b->ptr.free_ptr.prev = NULL;
    b->ptr.free_ptr.next = NULL;
    tmp_b = GET_NEXT_BLOCK(b->ptr.buffer, b->size & BLOCK_SIZE);
    TLSF_VG_UNLOCK_BHDR(tmp_b);
    if (tmp_b->size & FREE_BLOCK) {
        MAPPING_INSERT(tmp_b->size & BLOCK_SIZE, &fl, &sl);
        TLSF_VG_UNLOCK_FBLOCK(tmp_b);
        EXTRACT_BLOCK(tmp_b, tlsf, fl, sl);
        b->size += (tmp_b->size & BLOCK_SIZE) + BHDR_OVERHEAD;
        TLSF_VG_LOCK_FBLOCK(tmp_b);
    }
    TLSF_VG_LOCK_BHDR(tmp_b);
    if (b->size & PREV_FREE) {
        tmp_b = b->prev_hdr;
        TLSF_VG_UNLOCK_FBLOCK(tmp_b);
        MAPPING_INSERT(tmp_b->size & BLOCK_SIZE, &fl, &sl);
        EXTRACT_BLOCK(tmp_b, tlsf, fl, sl);
        tmp_b->size += (b->size & BLOCK_SIZE) + BHDR_OVERHEAD;
        b = tmp_b;
    }
    MAPPING_INSERT(b->size & BLOCK_SIZE, &fl, &sl);
    INSERT_BLOCK(b, tlsf, fl, sl);

    tmp_b = GET_NEXT_BLOCK(b->ptr.buffer, b->size & BLOCK_SIZE);
    TLSF_VG_UNLOCK_BHDR(tmp_b);
    tmp_b->size |= PREV_FREE;
    tmp_b->prev_hdr = b;
    TLSF_VG_LOCK_BHDR(tmp_b);
    VALGRIND_MAKE_MEM_NOACCESS(b, BHDR_OVERHEAD + (b->size&BLOCK_SIZE));
    VALGRIND_MEMPOOL_FREE(mem_pool, ptr);
}

/******************************************************************/
void *realloc_ex(void *ptr, size_t new_size, void *mem_pool)
{
/******************************************************************/
    tlsf_t *tlsf = (tlsf_t *) mem_pool;
    void *ptr_aux;
    unsigned int cpsize;
    bhdr_t *b, *tmp_b, *next_b;
    int fl, sl;
    size_t tmp_size, alloced_size;

    if (!ptr) {
        if (new_size)
            return (void *) malloc_ex(new_size, mem_pool);
        if (!new_size)
            return NULL;
    } else if (!new_size) {
        free_ex(ptr, mem_pool);
        return NULL;
    }

    b = (bhdr_t *) ((char *) ptr - BHDR_OVERHEAD);
    TLSF_VG_UNLOCK_BHDR(b);
    next_b = GET_NEXT_BLOCK(b->ptr.buffer, b->size & BLOCK_SIZE);
    TLSF_VG_UNLOCK_BHDR(next_b);
    new_size = (new_size < MIN_BLOCK_SIZE) ? MIN_BLOCK_SIZE : ROUNDUP_SIZE(new_size);
    tmp_size = (b->size & BLOCK_SIZE);
    alloced_size = tlsf_valgrind_getAllocedSize(b);
    (void)alloced_size; // avoid unused warning
    if (new_size <= tmp_size) {
        TLSF_REMOVE_SIZE(tlsf, b);
        if (next_b->size & FREE_BLOCK) {
            TLSF_VG_UNLOCK_FBLOCK(next_b);
            MAPPING_INSERT(next_b->size & BLOCK_SIZE, &fl, &sl);
            EXTRACT_BLOCK(next_b, tlsf, fl, sl);
            tmp_size += (next_b->size & BLOCK_SIZE) + BHDR_OVERHEAD;
            next_b = GET_NEXT_BLOCK(next_b->ptr.buffer, next_b->size & BLOCK_SIZE);
            TLSF_VG_UNLOCK_BHDR(next_b);
            /* We allways reenter this free block because tmp_size will
               be greater then sizeof (bhdr_t) */
        }
        tmp_size -= new_size;
        if (tmp_size >= sizeof(bhdr_t)) {
            tmp_size -= BHDR_OVERHEAD;
            tmp_b = GET_NEXT_BLOCK(b->ptr.buffer, new_size);
            TLSF_VG_UNLOCK_FBLOCK(tmp_b);
            tmp_b->size = tmp_size | FREE_BLOCK | PREV_USED;
            next_b->prev_hdr = tmp_b;
            next_b->size |= PREV_FREE;
            MAPPING_INSERT(tmp_size, &fl, &sl);
            INSERT_BLOCK(tmp_b, tlsf, fl, sl);
            b->size = new_size | (b->size & PREV_STATE);
            VALGRIND_MAKE_MEM_NOACCESS(tmp_b, BHDR_OVERHEAD+(tmp_b->size&BLOCK_SIZE));
        }
        TLSF_ADD_SIZE(tlsf, b);
        TLSF_VG_LOCK_BHDR(next_b);
        TLSF_VG_LOCK_BHDR(b);
        VALGRIND_MEMPOOL_CHANGE(tlsf, ptr, b->ptr.buffer, new_size);

#if OV_VALGRIND
        if(alloced_size<new_size)
            VALGRIND_MAKE_MEM_UNDEFINED(b->ptr.buffer+alloced_size, new_size-alloced_size);
#endif
        return (void *) b->ptr.buffer;
    }
    if ((next_b->size & FREE_BLOCK)) {
        if (new_size <= (tmp_size + (next_b->size & BLOCK_SIZE))) {
            TLSF_VG_UNLOCK_FBLOCK(next_b);
            TLSF_REMOVE_SIZE(tlsf, b);
            MAPPING_INSERT(next_b->size & BLOCK_SIZE, &fl, &sl);
            EXTRACT_BLOCK(next_b, tlsf, fl, sl);
            b->size += (next_b->size & BLOCK_SIZE) + BHDR_OVERHEAD;
            next_b = GET_NEXT_BLOCK(b->ptr.buffer, b->size & BLOCK_SIZE);
            TLSF_VG_UNLOCK_BHDR(next_b);
            next_b->prev_hdr = b;
            next_b->size &= ~PREV_FREE;
            tmp_size = (b->size & BLOCK_SIZE) - new_size;
            if (tmp_size >= sizeof(bhdr_t)) {
                tmp_size -= BHDR_OVERHEAD;
                tmp_b = GET_NEXT_BLOCK(b->ptr.buffer, new_size);
                TLSF_VG_UNLOCK_FBLOCK(tmp_b);
                tmp_b->size = tmp_size | FREE_BLOCK | PREV_USED;
                next_b->prev_hdr = tmp_b;
                next_b->size |= PREV_FREE;
                MAPPING_INSERT(tmp_size, &fl, &sl);
                INSERT_BLOCK(tmp_b, tlsf, fl, sl);
                VALGRIND_MAKE_MEM_NOACCESS(tmp_b, BHDR_OVERHEAD+(tmp_b->size&BLOCK_SIZE));
                b->size = new_size | (b->size & PREV_STATE);
            }
            TLSF_ADD_SIZE(tlsf, b);
            TLSF_VG_LOCK_BHDR(b);
            TLSF_VG_LOCK_BHDR(next_b);
            VALGRIND_MEMPOOL_CHANGE(tlsf, ptr, b->ptr.buffer, new_size);
            VALGRIND_MAKE_MEM_UNDEFINED(b->ptr.buffer+alloced_size, new_size-alloced_size);
            return (void *) b->ptr.buffer;
        }
    }

    TLSF_VG_LOCK_BHDR(next_b);

    if (!(ptr_aux = malloc_ex(new_size, mem_pool))){
        TLSF_VG_LOCK_BHDR(b);
        return NULL;
    }
    TLSF_VG_UNLOCK_BHDR(b); // may have been locked by malloc_ex

    cpsize = ((b->size & BLOCK_SIZE) > new_size) ? new_size : (b->size & BLOCK_SIZE);
#if OV_VALGRIND
    if(alloced_size<cpsize){
        cpsize = alloced_size;
    }
#endif
    TLSF_VG_LOCK_BHDR(b);

    memcpy(ptr_aux, ptr, cpsize);

    free_ex(ptr, mem_pool);
    return ptr_aux;
}


/******************************************************************/
void *calloc_ex(size_t nelem, size_t elem_size, void *mem_pool)
{
/******************************************************************/
    void *ptr;

    if (nelem <= 0 || elem_size <= 0)
        return NULL;

    if (!(ptr = malloc_ex(nelem * elem_size, mem_pool)))
        return NULL;
    memset(ptr, 0, nelem * elem_size);

    return ptr;
}


#define AdjustPointer(ptr, diff) (ptr) = ((void*) ((ptr)?(((unsigned char*) (ptr)) + diff):(NULL)))

int tlsf_move_pool(void* _tlsf, ptrdiff_t diff){
	tlsf_t* tlsf = (tlsf_t*) _tlsf;
	area_info_t* ai = NULL;
	bhdr_t* bh = NULL;
	int i, j;

	// adjust areas
	AdjustPointer(tlsf->area_head, diff);
	ai = tlsf->area_head;
	if(ai->next){
		// tlsf with several areas not supported
		AdjustPointer(ai, -diff);
		return 1;
	}
	while(ai){
		AdjustPointer(ai->end, diff);
		AdjustPointer(ai->next, diff);

		// adjust blocks
		bh = GET_BLOCK_HEADER(ai);

		while(bh){
			if((bh->size & BLOCK_STATE) == FREE_BLOCK){
				AdjustPointer(bh->ptr.free_ptr.prev, diff);
				AdjustPointer(bh->ptr.free_ptr.next, diff);
			}

			if((bh->size & PREV_STATE) == PREV_FREE){
				AdjustPointer(bh->prev_hdr, diff);
			}

			if ((bh->size & BLOCK_SIZE))
				bh = GET_NEXT_BLOCK(bh->ptr.buffer, bh->size & BLOCK_SIZE);
			else
				bh = NULL;
		}
		//ai = ai->next;
		// move only the first area
		ai = NULL;
	}

	// adjust tlsf matrix
	for(i = 0; i<REAL_FLI; i++){
		for(j = 0; j<MAX_SLI; j++){
			AdjustPointer(tlsf->matrix[i][j], diff);
		}
	}

	return 0;
}

#if OV_DEBUG || OV_VALGRIND

void tlsf_getPointerList(void* mem_pool, void*** list, size_t** sizes){
	tlsf_t* tlsf = (tlsf_t*) mem_pool;

	bhdr_t* b;

	unsigned int count = 0;
	unsigned int size = 1000;
	void**  ptrList = malloc(size*sizeof(void*));
	*sizes = malloc(size*sizeof(size_t));

	VALGRIND_DISABLE_ERROR_REPORTING;

	b = GET_BLOCK_HEADER(tlsf->area_head);
	b = GET_NEXT_BLOCK(b->ptr.buffer, b->size&BLOCK_SIZE);

	while(b){

		if(count>=size){
			size += 1000;
			ptrList = realloc(ptrList, size*sizeof(void*));
			*sizes = realloc(*sizes, size*sizeof(size_t));
		}

		if((b->size&BLOCK_STATE)==USED_BLOCK){
			ptrList[count] = b->ptr.buffer;
			(*sizes)[count] = b->size&BLOCK_SIZE;
			count++;
		}

		b = GET_NEXT_BLOCK(b->ptr.buffer, b->size&BLOCK_SIZE);
		if(!(b->size&BLOCK_SIZE))
			b = NULL;
	}

	memset(ptrList+count, 0, (size-count)*sizeof(void*));

	VALGRIND_ENABLE_ERROR_REPORTING;

	*list = ptrList;
}
#endif

#if OV_VALGRIND

static size_t tlsf_valgrind_getAllocedSize(bhdr_t* b){
	void* pNoAccess = NULL;
	size_t size = 0;

	VALGRIND_DISABLE_ERROR_REPORTING;

	size = b->size&BLOCK_SIZE;
	pNoAccess = (void*)VALGRIND_CHECK_MEM_IS_ADDRESSABLE(b->ptr.buffer, size);
	if(pNoAccess)
		size = (u8_t*)pNoAccess-b->ptr.buffer;

	VALGRIND_ENABLE_ERROR_REPORTING;

	return size;
}

void tlsf_valgrind_reinit_alloc(void* mem_pool) {
	tlsf_t* tlsf = (tlsf_t*) mem_pool;
	area_info_t* ai = NULL;
	bhdr_t* b = NULL;

	VALGRIND_DISABLE_ERROR_REPORTING;

	ai = tlsf->area_head;

	while(ai){
		b = GET_BLOCK_HEADER(ai);
		// skip area info block
		b = GET_NEXT_BLOCK(b->ptr.buffer, b->size&BLOCK_SIZE);
		while(b){
			if ((b->size&BLOCK_STATE)==USED_BLOCK) {
				VALGRIND_MEMPOOL_ALLOC(tlsf, b->ptr.buffer, b->size&BLOCK_SIZE);
			}

			b = GET_NEXT_BLOCK(b->ptr.buffer, b->size&BLOCK_SIZE);
			if(!(b->size&BLOCK_SIZE))
				b = NULL;
		}
		ai = ai->next;
	}

	VALGRIND_ENABLE_ERROR_REPORTING;
}

void tlsf_valgrind_reinit_access(void* mem_pool){
	tlsf_t* tlsf = (tlsf_t*) mem_pool;
	area_info_t* ai = NULL;
	area_info_t* ainext = NULL;
	bhdr_t* b = NULL;
	bhdr_t* bnext = NULL;

	VALGRIND_DISABLE_ERROR_REPORTING;

	VALGRIND_MAKE_MEM_DEFINED(tlsf, sizeof(tlsf));

	ai = tlsf->area_head;

	while(ai){
		VALGRIND_MAKE_MEM_DEFINED(ai, sizeof(area_info_t));
		b = GET_BLOCK_HEADER(ai);
		while(b){
			TLSF_VG_UNLOCK_BHDR(b);
			bnext = GET_NEXT_BLOCK(b->ptr.buffer, b->size&BLOCK_SIZE);

			if((b->size&BLOCK_STATE)==FREE_BLOCK){
				VALGRIND_MAKE_MEM_NOACCESS(b->ptr.buffer, b->size&BLOCK_SIZE);
			}else if((b->size&BLOCK_STATE)==USED_BLOCK) {
				VALGRIND_MAKE_MEM_DEFINED(b->ptr.buffer, b->size&BLOCK_SIZE);
			}

			TLSF_VG_LOCK_BHDR(b);

			b = bnext;

			if(!(b->size&BLOCK_SIZE)){
				TLSF_VG_LOCK_BHDR(b);
				b = NULL;
			}
		}
		ainext = ai->next;
		VALGRIND_MAKE_MEM_NOACCESS(ai, sizeof(area_info_t));
		ai = ainext;
	}

	VALGRIND_ENABLE_ERROR_REPORTING;
}

int tlsf_valgrind_check_access(void* mem_pool) {
	tlsf_t* tlsf = (tlsf_t*) mem_pool;
	area_info_t* ai = NULL;
	bhdr_t* b = NULL;
	uint8_t* pNA = NULL;
	int iarea = 0;
	int iblock = 0;
	int res = 0;

	VALGRIND_DISABLE_ERROR_REPORTING;

	if(VALGRIND_CHECK_MEM_IS_DEFINED(mem_pool, sizeof(tlsf_t))){
		fprintf(stderr, "tlsf header is not defined\n");
		res |= 0x1;
	}

	ai = tlsf->area_head;
	while(ai){
		if(!VALGRIND_CHECK_MEM_IS_ADDRESSABLE(ai, sizeof(area_info_t))){
			fprintf(stderr, "area head is not protected (%i)\n", iarea);
			res |= 0x2;
		}

		b = GET_BLOCK_HEADER(ai);
		iblock = 0;
		while(b){
			if(!VALGRIND_CHECK_MEM_IS_ADDRESSABLE(b, BHDR_OVERHEAD)){
				fprintf(stderr, "block head at %p not protected (%i, %i)\n", b, iarea, iblock);
				res |= 0x4;
			}


			if((b->size&BLOCK_STATE)==FREE_BLOCK &&
					!VALGRIND_CHECK_MEM_IS_ADDRESSABLE(b->ptr.buffer, (b->size&BLOCK_SIZE))){
				fprintf(stderr, "free block %p is addressable (%i, %i)\n", b, iarea, iblock);
				res |= 0x8;
			}else if((b->size&BLOCK_STATE)==USED_BLOCK) {
				pNA = (void*) VALGRIND_CHECK_MEM_IS_ADDRESSABLE(b->ptr.buffer, b->size&BLOCK_SIZE);
				if(pNA && b->size-(pNA-b->ptr.buffer)>FREE_BLOCK_SIZE){
					fprintf(stderr, "used block %p is not addressable (%i, %i); %lu byte in block of %lu\n", b, iarea, iblock, pNA-b->ptr.buffer, b->size&BLOCK_SIZE);
					res |= 0x10;
				}

			}

			b = GET_NEXT_BLOCK(b->ptr.buffer, b->size&BLOCK_SIZE);
			if(!(b->size&BLOCK_SIZE)){
				b = NULL;
				continue;
			}
			iblock++;
		}
		ai = ai->next;
		iarea++;
	}

	VALGRIND_ENABLE_ERROR_REPORTING;

	return res;
}

#endif

#if _DEBUG_TLSF_

/***************  DEBUG FUNCTIONS   **************/

/* The following functions have been designed to ease the debugging of */
/* the TLSF  structure.  For non-developing  purposes, it may  be they */
/* haven't too much worth.  To enable them, _DEBUG_TLSF_ must be set.  */

static void print_block(bhdr_t * b);

extern int tlsf_check_pool(void* _tlsf);
extern void dump_memory_region(unsigned char *mem_ptr, unsigned int size);
extern void print_tlsf(void* _tlsf);
extern void print_all_blocks(void* _tlsf);

static int find_block(bhdr_t** list, int len, bhdr_t* hdr){
	for(int i = 0; i<len; i++){
		if(list[i] == hdr){
			return i;
		}
	}
	return -1;
}

int tlsf_check_pool(void* _tlsf){

	tlsf_t* tlsf = (tlsf_t*) _tlsf;
	area_info_t* ai = NULL;
	bhdr_t* pool_start = NULL;
	bhdr_t* pool_end = NULL;
	bhdr_t* bh = NULL;

	int n_block = 0;
	int c_block = 0;
	int n_free = 0;
	int c_free = 0;
	bhdr_t** blocklist;
	bhdr_t** blocklist_free;
	char* check_free;

	ai = tlsf->area_head;

	while(ai){
		pool_start = GET_BLOCK_HEADER(ai);
		pool_end = ai->end;

		bh = pool_start;
		while(bh){
			n_block++;
			if((bh->size & BLOCK_STATE) == FREE_BLOCK)
				n_free++;
			if(bh->size&BLOCK_SIZE){
				bh = GET_NEXT_BLOCK(bh->ptr.buffer, bh->size & BLOCK_SIZE);
				if(bh > pool_end){
					// pointer outside of area
					return 1;
				}
			} else {
				// check if end matches area end
				if(bh!=pool_end){
					return 2;
				}
				bh = NULL;
			}
		}

		ai = ai->next;

	}

	blocklist = calloc(n_block, sizeof(void*));
	blocklist_free = calloc(n_free, sizeof(void*));
	check_free = calloc(n_free, sizeof(char));

	ai = tlsf->area_head;
	c_block = 0;
	c_free = 0;

	while(ai){
		pool_start = GET_BLOCK_HEADER(ai);

		bh = pool_start;
		while(bh){
			// add to list of blocks
			blocklist[c_block] = bh;
			c_block++;
			// add to list of free blocks
			if((bh->size & BLOCK_STATE) == FREE_BLOCK){
				blocklist_free[c_free] = bh;
				c_free++;
			}
			if(bh->size&BLOCK_SIZE){
				bh = GET_NEXT_BLOCK(bh->ptr.buffer, bh->size & BLOCK_SIZE);
			} else {
				bh = NULL;
			}
		}

		ai = ai->next;
	}

	// check previous block pointer
	for(int i = 0; i < n_block; i++){
		if((blocklist[i]->size & PREV_STATE)==PREV_FREE){
			// previous block pointer should be set
			if(blocklist[i]->prev_hdr != blocklist[i-1]){
				free(blocklist);
				free(blocklist_free);
				free(check_free);
				return 3;
			}
		}
	}

	// check free blocks for consistency
	for(int i = 1; i < n_free; i++){
		bh = blocklist_free[i];

		if(	(bh->ptr.free_ptr.prev&&(find_block(blocklist_free, n_free, bh->ptr.free_ptr.prev)==-1)) ||
			(bh->ptr.free_ptr.next&&(find_block(blocklist_free, n_free, bh->ptr.free_ptr.next)==-1)) ){
			// previous / next points to invalid block
			free(blocklist);
			free(blocklist_free);
			free(check_free);
			return 4;
		}
	}

	// check matrix for consistency
	for(int fl = 0; fl < REAL_FLI; fl++){
		for(int sl = 0; sl < MAX_SLI; sl++){
			int pos;
			bh=tlsf->matrix[fl][sl];
			while(bh){
				pos = find_block(blocklist_free, n_free, bh);
				if(pos<0){
					// invalid block in matrix
					free(blocklist);
					free(blocklist_free);
					free(check_free);
					return 5;
				}
				check_free[pos]=1;
				bh = bh->ptr.free_ptr.next;
			}
		}
	}

	for(int i = 0; i < n_free; i++){
		if(!check_free[i]){
			// not all free blocks in matrix
			free(blocklist);
			free(blocklist_free);
			free(check_free);
			return 6;
		}
	}

	free(blocklist);
	free(blocklist_free);
	free(check_free);
	return 0;
}

void dump_memory_region(unsigned char *mem_ptr, unsigned int size)
{

	uintptr_t begin = (uintptr_t) mem_ptr;
	uintptr_t end = (uintptr_t) mem_ptr + size;
    int column = 0;

    begin >>= 2;
    begin <<= 2;

    end >>= 2;
    end++;
    end <<= 2;

    PRINT_MSG("\nMemory region dumped: 0x%lx - 0x%lx\n\n", begin, end);

    column = 0;
    PRINT_MSG("0x%lx ", begin);

    while (begin < end) {
        if (((unsigned char *) begin)[0] == 0)
            PRINT_MSG("00");
        else
            PRINT_MSG("%02x", ((unsigned char *) begin)[0]);
        if (((unsigned char *) begin)[1] == 0)
            PRINT_MSG("00 ");
        else
            PRINT_MSG("%02x ", ((unsigned char *) begin)[1]);
        begin += 2;
        column++;
        if (column == 8) {
            PRINT_MSG("\n0x%lx ", begin);
            column = 0;
        }

    }
    PRINT_MSG("\n\n");
}

void print_block(bhdr_t * b)
{
    if (!b)
        return;
    PRINT_MSG(">> [%p] (", b);
    if ((b->size & BLOCK_SIZE))
        PRINT_MSG("%lu bytes, ", (unsigned long) (b->size & BLOCK_SIZE));
    else
        PRINT_MSG("sentinel, ");
    PRINT_MSG("buf: [%p] ", b->ptr.buffer);
    if ((b->size & BLOCK_STATE) == FREE_BLOCK)
        PRINT_MSG("free [%p, %p], ", b->ptr.free_ptr.prev, b->ptr.free_ptr.next);
    else
        PRINT_MSG("used, ");
    if ((b->size & PREV_STATE) == PREV_FREE)
        PRINT_MSG("prev. free [%p])\n", b->prev_hdr);
    else
        PRINT_MSG("prev used)\n");
}

void print_tlsf(void* _tlsf)
{
	tlsf_t * tlsf = (tlsf_t*) _tlsf;
    bhdr_t *next;
    int i, j;

    PRINT_MSG("\nTLSF at %p\n", tlsf);

    PRINT_MSG("FL bitmap: 0x%x\n\n", (unsigned) tlsf->fl_bitmap);

    for (i = 0; i < REAL_FLI; i++) {
        if (tlsf->sl_bitmap[i])
            PRINT_MSG("SL bitmap 0x%x\n", (unsigned) tlsf->sl_bitmap[i]);
        for (j = 0; j < MAX_SLI; j++) {
            next = tlsf->matrix[i][j];
            if (next)
                PRINT_MSG("-> [%d][%d]\n", i, j);
            while (next) {
                print_block(next);
                next = next->ptr.free_ptr.next;
            }
        }
    }
}

void print_all_blocks(void* _tlsf)
{
	tlsf_t * tlsf = (tlsf_t*) _tlsf;
    area_info_t *ai;
    bhdr_t *next;
    PRINT_MSG("\nTLSF at %p\nALL BLOCKS\n\n", tlsf);
    ai = tlsf->area_head;
    while (ai) {
    	next = GET_BLOCK_HEADER(ai);
        while (next) {
            print_block(next);
            if ((next->size & BLOCK_SIZE))
                next = GET_NEXT_BLOCK(next->ptr.buffer, next->size & BLOCK_SIZE);
            else
                next = NULL;
        }
        ai = ai->next;
    }
}

#endif
