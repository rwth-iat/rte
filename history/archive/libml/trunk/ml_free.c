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
*   ml_free.h
*
*   Historie
*   --------
*   2008-04-30 Alexander Neugebauer: Erstellung, LTSoft GmbH, Kerpen
*
******************************************************************************/
#include "ml_malloc.h"


/* Freie Segmente (um)ordnen. */
void
ml_reorde_block(struct __ml_list *ptrLst, __ml_size_t size) {

    struct __ml_list     *curr;
    __ml_size_t           firstAddr;
    __ml_size_t           nextAddr;

#if DBG_TO_STDERR
fprintf(stderr, "ml_reorde_block %lu mit %lu Bytes\n", (__ml_size_t)(__ml_byte_t*)ptrLst, size);
#endif

if( (((__ml_size_t)(__ml_byte_t*)ptrLst - ML_STARTADDRESS) % BLOCKSIZE) != 0 ) {
#if DBG_TO_STDERR
    fprintf(stderr, "!!! Adresse  %lu nicht an der Block-Ausrichtung (start: %lu) !!!\n", (__ml_size_t)(__ml_byte_t*)ptrLst, ML_STARTADDRESS);
#endif
    return;
}

    /* Block-Groesse vermerkt? */
    ptrLst->len = size;

    /* Freie Bloecke bereits vorhanden? */
    curr = _ml_info_ptr->free_blocks.next;
    if(curr) {
    
        /* Nachfolger vorhanden? */
        firstAddr = (__ml_size_t)((__ml_byte_t*)ptrLst);
        nextAddr  = firstAddr + size;
        while(curr) {
            if(nextAddr == (__ml_size_t)((__ml_byte_t*)curr)) {
                /* Nachfolger gefunden */
#if DBG_TO_STDERR
fprintf(stderr, "ml_reorde_block: nachfolger %lu (%lu Bytes) gefunden\n", (__ml_size_t)(__ml_byte_t*)curr, curr->len);
#endif
                ptrLst->next = curr->next;
                ptrLst->prev = curr->prev;
                ptrLst->prev->next = ptrLst;
                ptrLst->len += curr->len;
                if(ptrLst->next) {
                    ptrLst->next->prev = ptrLst;
                }
                curr->prev = NULL;
                curr->next = NULL;
                curr->len  = 0;

#if DBG_TO_STDERR
fprintf(stderr, "ml_reorde_block: Block %lu hat nun %lu Bytes\n", (__ml_size_t)(__ml_byte_t*)ptrLst, ptrLst->len);
#endif
                
                /* Luecke geschlossen? */
                if(ptrLst != _ml_info_ptr->free_blocks.next) {
                    /* Nicht am Anfang der Liste */
                    curr = ptrLst->prev;
                    if(firstAddr == (__ml_size_t)((__ml_byte_t*)curr + (curr->len)) ) {
#if DBG_TO_STDERR
fprintf(stderr, "ml_reorde_block: Luecke zu %lu (%lu Bytes) geschlossen\n", (__ml_size_t)(__ml_byte_t*)curr, curr->len);
#endif
                        curr->next = ptrLst->next;
                        curr->len += ptrLst->len;
                        if(ptrLst->next) {
                            ptrLst->next->prev = curr;
                        }
                        ptrLst->next = NULL;
                        ptrLst->prev = NULL;
                        ptrLst->len  = 0;

#if DBG_TO_STDERR
fprintf(stderr, "ml_reorde_block: Block %lu hat nun %lu Bytes\n", (__ml_size_t)(__ml_byte_t*)curr, curr->len);
#endif
                    }
                }
                return;
            }
            curr = curr->next;
        }
        /* Vorgaenger vorhanden? */
        curr = _ml_info_ptr->free_blocks.next;
        while(curr) {
            if((__ml_size_t)((__ml_byte_t*)curr + (curr->len)) == firstAddr) {
                /* Vorgaenger gefunden */
#if DBG_TO_STDERR
fprintf(stderr, "ml_reorde_block: vorgaenger %lu (%lu Bytes) gefunden\n", (__ml_size_t)(__ml_byte_t*)curr, curr->len);
#endif
                curr->len += ptrLst->len;
                ptrLst->next = NULL;
                ptrLst->prev = NULL;
                ptrLst->len  = 0;

                /* Luecke geschlossen? */
                if(curr->next) {
                    nextAddr  = (__ml_size_t)((__ml_byte_t*)curr + (curr->len));
                    if(nextAddr == (__ml_size_t)((__ml_byte_t*)curr->next) ) {
                        ptrLst = curr->next;
#if DBG_TO_STDERR
fprintf(stderr, "ml_reorde_block: Luecke zu %lu (%lu Bytes) geschlossen\n", (__ml_size_t)(__ml_byte_t*)ptrLst, ptrLst->len);
#endif
                        if(ptrLst->next) {
                            ptrLst->next->prev = curr;
                        }
                        curr->next = ptrLst->next;
                        curr->len += ptrLst->len;
                        
                        ptrLst->next = NULL;
                        ptrLst->prev = NULL;
                        ptrLst->len  = 0;
                    }
                }

#if DBG_TO_STDERR
fprintf(stderr, "ml_reorde_block: Block %lu hat nun %lu Bytes\n", (__ml_size_t)(__ml_byte_t*)curr, curr->len);
#endif
                
                return;
            }
            curr = curr->next;
        }
        
        /* 
            Na ja, ... (s.u.)
        */
        curr = _ml_info_ptr->free_blocks.next;
        while(curr) {
            if(nextAddr < (__ml_size_t)((__ml_byte_t*)curr)) {
#if DBG_TO_STDERR
fprintf(stderr, "ml_reorde_block: haenge vor %lu (%lu Bytes) an\n", (__ml_size_t)(__ml_byte_t*)curr, curr->len);
#endif
                /* Nachfolger gefunden */
                ptrLst->next = curr;
                ptrLst->prev = curr->prev;
                curr->prev->next = ptrLst;
                curr->prev = ptrLst;
                
                return;
            }
            curr = curr->next;
        }
        
#if DBG_TO_STDERR
fprintf(stderr, "ml_reorde_block: haenge am Ende der Liste\n");
#endif
        /* Dann eben am Ende der Liste */
        curr = _ml_info_ptr->free_blocks.next;
        while(curr->next) {
            curr = curr->next;
        }
        ptrLst->next = NULL;
        ptrLst->prev = curr;
        curr->next = ptrLst;
        
        return;
    }
    
#if DBG_TO_STDERR
fprintf(stderr, "ml_reorde_block: noch keine Bloecke vorhanden\n");
#endif
    /* Keine freie Bloecke vorhanden */
    ptrLst->next = NULL;
    ptrLst->prev = &_ml_info_ptr->free_blocks;
    _ml_info_ptr->free_blocks.next = ptrLst;
    
    return;

}

void
ml_reorde(__ml_size_t log) {
    struct __ml_list *curr;
    struct __ml_list *startElem;
    __ml_size_t       anzFrags;
    __ml_size_t       pos;
    __ml_size_t       startAddr;
    __ml_size_t       nextAddr;
    __ml_size_t       size;
    
    /* Anzahl Fragmenten und Groesse eines Fragmentes im Bereich */
    size = BLOCKSIZE / (BLOCKSIZE >> log);
    anzFrags = (__ml_size_t) (BLOCKSIZE / size);

    startElem = _ml_info_ptr->fraghead[log].next;
    while(startElem) {
        /* Fragment am "Block-Anfang" Position? */
        startAddr = (__ml_size_t)((__ml_byte_t*)startElem);
        
        if( ((startAddr - ML_STARTADDRESS) % BLOCKSIZE) == 0 ) {
            /* Anfang-Position eines Blocks */
            curr = startElem;
            pos = 0;
            while(curr) {
                pos++;
                /* Ergibt Gesammt-Laenge ein Block? */
                if(pos == anzFrags) {
                    /* Block komplett */
                    startElem->prev->next = curr->next;
                    if(curr->next) {
                        curr->next->prev = startElem->prev;
                    }
                    
                    ml_reorde_block(startElem, BLOCKSIZE);
                    return;
                }
                if(curr->next) {
                    /* Nachfolge-Element vorhanden. Gehoeren die zusammen? */
                    nextAddr = (__ml_size_t)((__ml_byte_t*)curr + size);
                    
                    if( nextAddr != (__ml_size_t)((__ml_byte_t*)curr->next) ) {
                        /* Luecke */
                        break;
                    }
                } else {
                    // Nachfolge-Element fellt
                    break;
                }
                curr = curr->next;
            }
        }
        
        /* Naechsten Block holen */
        startElem = startElem->next;
    }

    return;
}

/* Zeiger auf Anfang des Fragments setzen und Groesse ermitteln */
__ml_ptr
ml_getFragmentData(__ml_ptr ptr, __ml_size_t *size) {
    __ml_size_t       offs;
    __ml_byte_t      *pc;
    __ml_size_t      *pPtr;

    /*
    Zurueckgegeben wurde nicht der eigentliche Block, sondern Bereich ab
    pBlock + offset.
    Zeiger auf Anfang des Blocks setzen.
    */
    offs = ml_getOffset();
    pc = (__ml_byte_t *)ptr;
    pc -= offs;
    ptr = (__ml_ptr)pc;
    pPtr = (__ml_size_t*)ptr;
    *size = *pPtr;

    return ptr;
}

/* Reservierten Bereich freigeben.  */
void
ml_free (__ml_ptr ptr) {
    __ml_size_t       size;
    __ml_size_t       firstAddr;
    __ml_size_t       nextAddr;
    struct __ml_list *curr;
    struct __ml_list *ptrLst;
    __ml_size_t       log;
    
    if (!ptr) {
        return;
    }
    
    /* Zeiger auf Anfang des Blocks setzen. */
    ptr = ml_getFragmentData(ptr, &size);
    ptrLst = (struct __ml_list *)ptr;

    /* Liegt die Adresse in Memory-Pool? */
    if( ((__ml_byte_t*)ptr < _ml_info_ptr->heapbase) || ((__ml_byte_t*)ptr >= _ml_info_ptr->heapend) ) {
#if DBG_TO_STDERR
fprintf(stderr, "  ml_free Bereich %lu ausserhalb Memory-Pools!\n", (__ml_size_t)(__ml_byte_t*)ptrLst);
#endif
        return;    
    }
    /* Bereich bereits freigegeben? */
    if (size <= BLOCKSIZE / 2) {
        /* Small allocation */
        log = _ml_getfrag_log(size);

        curr = _ml_info_ptr->fraghead[log].next;
        while(curr) {
            if(ptrLst == curr) {
#if DBG_TO_STDERR
fprintf(stderr, "  ml_free Bereich %lu bereits freigegeben!\n", (__ml_size_t)(__ml_byte_t*)ptrLst);
#endif
                return;
            }
            curr = curr->next;
        }
    }
    
    /* Groesse des Bereichs merken */
    ptrLst->len = size;

#if DBG_TO_STDERR
fprintf(stderr, "  ml_free %lu size %lu\n", (__ml_size_t)(__ml_byte_t*)ptrLst, size);
#endif
  
    /* Update the statistics.  */
    _ml_info_ptr->bytes_free += size;

   
    /* Wie gross ist der reservierten Bereich ? */
    if (size <= BLOCKSIZE / 2) {
        /* Small allocation */
        log = _ml_getfrag_log(size);

        /* In entsprechenden frag einordnen */
        curr = _ml_info_ptr->fraghead[log].next;
        if(curr) {
            /* Nachfolger vorhanden? */
            firstAddr = (__ml_size_t)((__ml_byte_t*)ptr);
            nextAddr  = firstAddr + ptrLst->len;
            while(curr) {
                if(nextAddr == (__ml_size_t)((__ml_byte_t*)curr)) {
                    /* Nachfolger gefunden */
                    ptrLst->next = curr;
                    ptrLst->prev = curr->prev;
                    curr->prev->next = ptrLst;
                    curr->prev = ptrLst;
                    
                    /* Pruefe ob frag-Segmente einen ganzen Block ergeben */
                    ml_reorde(log);
                    
                    return;
                }
                curr = curr->next;
            }
            /* Vorgaenger vorhanden? */
            curr = _ml_info_ptr->fraghead[log].next;
            while(curr) {
                if((__ml_size_t)((__ml_byte_t*)curr + size) == firstAddr) {
                    /* Vorgaenger gefunden */
                    ptrLst->next = curr->next;
                    if(curr->next) {
                        curr->next->prev = ptrLst;
                    }
                    curr->next = ptrLst;
                    ptrLst->prev = curr;
                    
                    /* Pruefe ob frag-Segmente einen ganzen Block ergeben */
                    ml_reorde(log);
                    
                    return;
                }
                curr = curr->next;
            }
            
            /* 
                Na ja, es war ein Versuch Wert, die Bloeckchen zusammen zu bringen.
                Die Fragmentierung laesst gruessen. Suche Platz/Position fuer
                freizugebenden Block.
            */
            curr = _ml_info_ptr->fraghead[log].next;
            while(curr) {
                if(nextAddr < (__ml_size_t)((__ml_byte_t*)curr)) {
                    /* Nachfolger gefunden */
                    ptrLst->next = curr;
                    ptrLst->prev = curr->prev;
                    curr->prev->next = ptrLst;
                    curr->prev = ptrLst;
    
                    return;
                }
                curr = curr->next;
            }
    
            /* Dann eben am Ende der Liste */
            curr = _ml_info_ptr->fraghead[log].next;
            while(curr->next) {
                curr = curr->next;
            }
            ptrLst->next = NULL;
            ptrLst->prev = curr;
            curr->next = ptrLst;
            
            return;
            
        } else {
            /* Keine Blocke in frag */
            ptrLst->next = NULL;
            _ml_info_ptr->fraghead[log].next = ptrLst;
            ptrLst->prev = &_ml_info_ptr->fraghead[log];
    
            return;
        }
    } else {
        /* Block allocation */
        ml_reorde_block(ptrLst, size);
    }
  
    return;
}

