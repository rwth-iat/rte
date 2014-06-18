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
*   ml_realloc.h
*
*   Historie
*   --------
*   2008-04-30 Alexander Neugebauer: Erstellung, LTSoft GmbH, Kerpen
*   
******************************************************************************/
#include "../../include/libml/ml_malloc.h"
#ifdef ML_SYSTEM_WIN
#include <windows.h>
#include <winbase.h>
#endif

/* Resize the given region to the new size, returning a pointer
   to the (possibly moved) region.  This is optimized for speed;
   some benchmarks seem to indicate that greater compactness is
   achieved by unconditionally allocating and copying to a
   new region.  This module has incestuous knowledge of the
   internals of both ml_free and ml_malloc. */
__ml_ptr
ml_realloc (__ml_ptr ptr, __ml_size_t size) {
  __ml_ptr     result;
  __ml_size_t  newsize, oldsize, copysize;
  __ml_size_t  offs;


  if (size == 0) {
      ml_free(ptr);
      return NULL;
  }
  
  result = ml_malloc(size);
  if(!ptr) {
      return result;
  }
  if(!result) {
      /* DISCUS FIXME AND TODO: ml_free(ptr); ? */
      return result;
  }
  
  /* Offset */
  offs = ml_getOffset();

  /* Groesse des neuen und des alten Bloeks ermitteln. */
  ml_getFragmentData(ptr, &oldsize);
  ml_getFragmentData(result, &newsize);

#if DBG_TO_STDERR
fprintf(stderr, "  ml_realloc oldsize %" PRIuPTR " Bytes\n", oldsize);
fprintf(stderr, "  ml_realloc newsize %" PRIuPTR " Bytes\n", newsize);
#endif
  
  copysize = ((oldsize < newsize) ? oldsize : newsize) - offs;

#if DBG_TO_STDERR
fprintf(stderr, "  ml_realloc copiere %" PRIuPTR " Bytes\n", copysize);
#endif
  
  memcpy(result, ptr, copysize);

#if DBG_TO_STDERR
fprintf(stderr, "  ml_realloc gebe adresse %" PRIuPTR " frei\n", (__ml_size_t)ptr);
#endif

  /* Alten Speicher freigeben */
  ml_free(ptr);
  
  return result;
}

/*	----------------------------------------------------------------------	*/
/*
    !!! Achtung !!!
    ---------------
    Alle NFS-Systeme haben Problem mit Mapping der Dateien, deren Groessen
    nicht seiten-ausgerichtet sind. Seiten-Groesse auf Linux-Systemen ist
    4096 Byte. Solaris dagegen - 8192.
    Mit
        int pageSize = getpagesize();
        fprintf(stderr, "PageSize: %d\n", pageSize);
    kann die Groesse ermittelt werden.
*/
__ml_size_t
ml_rundupFileSize(__ml_size_t size) {
    __ml_size_t newSize;
    int         pageSize;

#if ML_SYSTEM_UNIX    
    /* Seiten-Groesse ermitteln */
    pageSize = getpagesize();
#else
#ifdef ML_SYSTEM_WIN
    SYSTEM_INFO system_info;
    GetSystemInfo (&system_info);
    pageSize = (int)system_info.dwPageSize;
#else
    pageSize = 8192;
#endif
#endif

#if DBG_TO_STDERR
fprintf(stderr, "PageSize: %d\n", pageSize);
#endif

    if(pageSize < BLOCKSIZE) {
        pageSize = 2 * BLOCKSIZE;
    }
    newSize = (size + pageSize - 1)&~(pageSize-1);
    
    return newSize;
}
