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
*   ml_malloc.c
*
*   Historie
*   --------
*   2008-04-30 Alexander Neugebauer: Erstellung, LTSoft GmbH, Kerpen
*
******************************************************************************/

#include "ml_malloc.h"

/* Memory pool info. */
ml_info *_ml_info_ptr;

/* Get more memory.  */
__ml_ptr (*__ml_morecore)(__ml_size_t __size) = NULL;


/* Get neatly aligned memory, initializing or
   growing the heap info table as necessary. */
static __ml_ptr
morecore (__ml_size_t size) {

    __ml_ptr     result;
    
    result = (*__ml_morecore) (size);
    if(!result) {
#if DBG_TO_STDERR
fprintf(stderr, "__morecore returned NULL\n");
#endif
        return result;
    }
    
    _ml_info_ptr->heapend = (__ml_byte_t*)result + size;
    _ml_info_ptr->bytes_free  += size;
    _ml_info_ptr->bytes_used  += size;

#if DBG_TO_STDERR
fprintf(stderr, "__morecore returned %lu\n", (__ml_size_t)(__ml_byte_t*)result);
#endif
    
    return result;
}

/* Set everything up and remember that we have.  */
int
ml_initialize(
    ml_info     *ml_info_ptr,
    __ml_byte_t *heapbase,
    __ml_ptr     (*func)(__ml_size_t __size)
) {
    __ml_size_t i;
    
    if (!ml_info_ptr || !heapbase || !func) {
        return 0;
    }
    
    _ml_info_ptr           = ml_info_ptr;
    _ml_info_ptr->heapbase = heapbase;
    _ml_info_ptr->heapend  = NULL;
    __ml_morecore          = func;
    
    _ml_info_ptr->free_blocks.prev = NULL;
    _ml_info_ptr->free_blocks.next = NULL;
    for (i=0; i<BLOCKLOG; i++) {
        _ml_info_ptr->fraghead[i].next = 0;
        _ml_info_ptr->fraghead[i].prev = 0;
    }

    /* Statistics.  */
    _ml_info_ptr->bytes_used  = 0;
    _ml_info_ptr->bytes_free  = 0;

    return 1;
}


/* Set everything up again.  */
int
ml_restart (
    ml_info     *ml_info_ptr,
    __ml_byte_t *heapbase,
    __ml_ptr     (*func)(__ml_size_t __size)
) {
    if (!ml_info_ptr || !heapbase || !func) {
        return 0;
    }
    
    _ml_info_ptr           = ml_info_ptr;
    _ml_info_ptr->heapbase = heapbase;
    __ml_morecore          = func;
    
    return 1;
}



/* Position in "Small"-Fragmenten bestimmen */ 
__ml_size_t
_ml_getfrag_log(__ml_size_t size) {
    __ml_size_t log = 1;
    
    size--;
    while ((size /= 2) != 0) {
        log++;
    }
    
    return log;
}

/* Offset fuer Datenpool */
__ml_size_t ml_getOffset(void) {
    /* Byte-Ausrichtung beachten (min. CHAR_BIT) ! */
    #define ML_WBIT CHAR_BIT
    
    __ml_size_t offs;

    offs = sizeof(__ml_size_t);
    offs = (offs + ML_WBIT -1)&~(ML_WBIT-1);
    
    return offs;
}

/* Search free block. */
__ml_ptr
ml_malloc_getfreeblock(__ml_size_t size) {
    struct __ml_list *curr;
    struct __ml_list *ret;

#if DBG_TO_STDERR
fprintf(stderr, "   getfreeblock() suche %lu Byte gross\n", size);
#endif
    
    /* Freie Bloecke vorhanden? */
    curr = _ml_info_ptr->free_blocks.next;
    if(curr) {
        /* Gibt es passenden ? */
        while(curr) {

#if DBG_TO_STDERR
fprintf(stderr, "   getfreeblock(%lu : %lu) suche passend\n", (__ml_size_t)(__ml_byte_t*)curr, curr->len);
#endif
        
            if(curr->len == size) {
#if DBG_TO_STDERR
fprintf(stderr, "   getfreeblock(%lu : %lu) passend gefunden\n", (__ml_size_t)(__ml_byte_t*)curr, curr->len);
#endif
                curr->prev->next = curr->next;
                if(curr->next) {
                    curr->next->prev = curr->prev;
                }
                return (__ml_ptr)curr;
            }
            curr = curr->next;
        }
        /* Gibt es groesseren? */
        curr = _ml_info_ptr->free_blocks.next;
        while(curr) {
#if DBG_TO_STDERR
fprintf(stderr, "   getfreeblock(%lu : %lu) suche groesseren\n", (__ml_size_t)(__ml_byte_t*)curr, curr->len);
#endif
            if(curr->len > size) {
                ret = (struct __ml_list *)((__ml_byte_t*)curr + (curr->len - size));
                curr->len -= size;                
                
#if DBG_TO_STDERR
fprintf(stderr, "   getfreeblock(%lu) vom groesseren abgeschnitten. Rest %lu Bytes\n", (__ml_size_t)(__ml_byte_t*)ret, curr->len);
#endif
                return (__ml_ptr)(ret);
            }
            curr = curr->next;
        }
    }
    
    return morecore(size);
}

/* Allocate memory from the heap.  */
__ml_ptr
ml_malloc(__ml_size_t size) {
    __ml_ptr          result;
    __ml_size_t       blocks;
    __ml_size_t       i;
    __ml_size_t       log;
    __ml_size_t       offs;
    __ml_size_t      *pResult;
    __ml_byte_t      *pc;
    struct __ml_list *curr;

    if (size == 0) {
        return NULL;
    }
    /* + Head for allocated block */
    offs = ml_getOffset();
    size += offs;
    if(size < sizeof(struct __ml_list)) {
        size = sizeof(struct __ml_list);
    }
  
    /* Determine the allocation policy based on the request size.  */
    if (size <= BLOCKSIZE / 2) {
        /* Small allocation to receive a fragment of a block.
           Determine the logarithm to base two of the fragment size. */
        log = _ml_getfrag_log(size);
        size = BLOCKSIZE / (BLOCKSIZE >> log);

#if DBG_TO_STDERR
        i = 0;
        curr = _ml_info_ptr->fraghead[log].next;
        while(curr) {
            i++;
            curr = curr->next;
        }
fprintf(stderr, "    ==> %lu Bytes: log %lu (anz %lu)\n", size, log, i);
#endif
      
        /* Look in the fragment lists for a free fragment of the desired size.*/
        curr = _ml_info_ptr->fraghead[log].next;
        if (curr) {
            /* There are free fragments of this size.
               Pop a fragment out of the fragment list and return it.
               Update the counters. */
            result = (__ml_ptr) curr;
            curr->prev->next = curr->next;
            if (curr->next != NULL) {
                curr->next->prev = curr->prev;
            }
            /* Update the statistics.  */
            _ml_info_ptr->bytes_free -= size;
        } else {
            /* No free fragments of the desired size, so get a new block
               and break it into fragments, returning the first.  */
            result = ml_malloc_getfreeblock(BLOCKSIZE);
            if (!result) {
                 return NULL;
            }
          
            /* Link all fragments but the first into the free list.  */
            blocks = (__ml_size_t) (BLOCKSIZE / size) - 1;
            for (i = blocks; i > 0 ; i--) {
                curr = (struct __ml_list *) ((__ml_byte_t*) result + (i << log));
                curr->next = _ml_info_ptr->fraghead[log].next;
                curr->prev = &_ml_info_ptr->fraghead[log];
                curr->prev->next = curr;
                if (curr->next != NULL) {
                    curr->next->prev = curr;
                }              
            }

            /* Initialize the counter for free blocks.  */
            _ml_info_ptr->bytes_free -= size;
        }
      
    } else {
        /* Large allocation to receive one or more blocks.
           Search the free list in a circle starting at the last place visited.
           If we loop completely around without finding a large enough
           space we will have to get more memory from the system.  */
        blocks = BLOCKIFY (size);
        size = blocks * BLOCKSIZE;

        result = ml_malloc_getfreeblock(size);
        if (!result) {
            return NULL;
        }
        _ml_info_ptr->bytes_free -= size;
    }

    /* Reservierten Bereich initialisieren und Groesse des Bereichs merken */
    pc = (__ml_byte_t*)result;
    for(i=0; i<size; i++) {
        *pc = '\0';
        pc++;      
    }

    pResult = (__ml_size_t*)result;
    *pResult = size;
    pc = (__ml_byte_t*)result + offs;
    result = (__ml_ptr)pc;

#if DBG_TO_STDERR
fprintf(stderr, "  ml_malloc %lu (%lu) size %lu\n\n"
                , (__ml_size_t)(__ml_byte_t*)result
                , (__ml_size_t)(__ml_byte_t*)pResult
                , size);
#endif
  
    return result;
}
