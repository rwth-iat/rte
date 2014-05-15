#ifndef _ML_MALLOC_H
#define _ML_MALLOC_H
/******************************************************************************
***
***   L I B M L 
***   #####################################
***
***   L T S o f t
***   Agentur für Leittechnik Software GmbH
***   Brabanterstr. 13
***   D-50171 Kerpen
***   Tel : 02237/92869-2
***   Fax : 02237/92869-9
***   e-Mail   : ltsoft@ltsoft.de 
***   Internet : http://www.ltsoft.de
*** 
*******************************************************************************
*
*   Datei
*   -----
*   ml_malloc.h
*
*   Historie
*   --------
*   2008-04-30 Alexander Neugebauer: Erstellung, LTSoft GmbH, Kerpen
*
*   Beschreibung
*   ------------ 
*   
******************************************************************************/

#if defined(_LIBC) || defined(STDC_HEADERS) || defined(USG) || (defined(__STDC__) && __STDC__) 
#include <string.h>
#endif

#if defined (__GNU_LIBRARY__) || (defined (__STDC__) && __STDC__)
#include <limits.h>
#endif

#include <stdio.h>

#if ML_SYSTEM_UNIX
#include <unistd.h>
#endif


#ifdef __cplusplus
extern "C"
{
#endif


#ifndef NULL
#define NULL 0
#endif

#if defined (__STDC__) && __STDC__
#include <stddef.h>
#endif

/* long variables are not on all Datamodels / platforms (as an example LLP64 as W64) the same size as a pointer -- Holger
 * http://www.viva64.com/en/a/0050/ */
#if _MSC_VER && _MSC_VER <= 1200
	#define __ml_size_t   unsigned long int
#else
	#define __ml_size_t   uintptr_t
#endif
#define __ml_byte_t   unsigned char
#define __ml_ptr      void*

/* The allocator divides the heap into blocks of fixed size; large
   requests receive one or more whole blocks, and small requests
   receive a fragment of a block.  Fragment sizes are powers of two,
   and all fragments of a block are the same size.  When all the
   fragments in a block have been freed, the block itself is freed.  */
#ifdef  CHAR_BIT
#undef  CHAR_BIT
#endif
#define CHAR_BIT        8

/*
    Anzahl Fragmenten fuer Verwaltung der Blocke (BLOCKLOG) ist
        2^N == BLOCKSIZE
          
    #define INT_BIT          (CHAR_BIT * sizeof(int))
    #define BLOCKLOG         (INT_BIT > 16 ? 12 : 9) 
    #define BLOCKSIZE        (((unsigned long)1) << BLOCKLOG)

    Fuer Linux/Solaris/Win - feste Groesse:
*/
#define BLOCKLOG         12
#define BLOCKSIZE        4096
#define BLOCKIFY(SIZE)   (((SIZE) + BLOCKSIZE - 1) / BLOCKSIZE)
#define ML_STARTADDRESS  ((__ml_size_t)_ml_info_ptr->heapbase)

  
/* Doubly linked lists of free fragments.  */
struct __ml_list {
    __ml_size_t       len;
    struct __ml_list *next;
    struct __ml_list *prev;
};

/* All global memory pool info in one structure. */
typedef struct {
    __ml_byte_t        *heapbase;           /* Pointer to first block of the heap. */
    __ml_byte_t        *heapend;            /* Pointer to last block of the heap.  */
    struct __ml_list   fraghead[BLOCKLOG];  /* Free list headers for each fragment size.  */
    struct __ml_list   free_blocks;         /* Free fragments.   */
    __ml_size_t        bytes_free;          /* Instrumentation.  */
    __ml_size_t        bytes_used;          /* Instrumentation.  */
} ml_info;

extern ml_info *_ml_info_ptr;


/* Set everything up and remember that we have.  */
int ml_initialize (ml_info *ml_info_ptr, __ml_byte_t *heapbase, __ml_ptr (*func)(__ml_size_t __size));

/* Set everything up again.  */
int ml_restart (ml_info *ml_info_ptr, __ml_byte_t *heapbase, __ml_ptr (*func)(__ml_size_t __size));

/* Allocate SIZE bytes of memory.  */
__ml_ptr ml_malloc(__ml_size_t __size);

/* Re-allocate the previously allocated block
   in __ml_ptr, making the new block SIZE bytes long.  */
__ml_ptr ml_realloc(__ml_ptr __ptr, __ml_size_t __size);

/* Free a block allocated by 'ml_malloc' or 'ml_realloc'.  */
void ml_free(__ml_ptr __ptr);

/* Datei-Groesse auf Seiten-Ausrichtung aufrunden */
__ml_size_t ml_rundupFileSize(__ml_size_t size);

/* Offset fuer Datenpool */
__ml_size_t ml_getOffset(void);

/* Zeiger auf Anfang des Fragments setzen und Groesse ermitteln */
__ml_ptr ml_getFragmentData(__ml_ptr ptr, __ml_size_t *size);

/* Position in "Small"-Fragmenten bestimmen */ 
__ml_size_t _ml_getfrag_log(__ml_size_t size);

/* Underlying allocation function; successive calls should
   return contiguous pieces of memory.  */
extern __ml_ptr (*__ml_morecore) (__ml_size_t __size);


#ifdef        __cplusplus
}
#endif

#endif /* ml_malloc.h  */
