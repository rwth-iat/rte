/*
*   $Id: ov_path.c,v 1.7 2003-11-07 09:33:00 ansgar Exp $
*
*   Copyright (C) 1998-1999
*   Lehrstuhl fuer Prozessleittechnik,
*   RWTH Aachen, D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
*
*   This file is part of the ACPLT/OV Package which is licensed as open
*   source under the Artistic License; you can use, redistribute and/or
*   modify it under the terms of that license.
*
*   You should have received a copy of the Artistic License along with
*   this Package; see the file ARTISTIC-LICENSE. If not, write to the
*   Copyright Holder.
*
*   THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
*   WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
*   OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/
/*
*	History:
*	--------
*	22-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

#define OV_COMPILE_LIBOV

#include "libov/ov_path.h"
#include "libov/ov_logfile.h"
#include "libov/ov_memstack.h"
#include "libov/ov_database.h"
#include "libov/ov_vendortree.h"
#include "libov/ov_macros.h"

#if OV_SYSTEM_MC164
#define memcpy xmemcpy
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Resolve a path using a given path name
*	Note: the memory for the path elements is allocated on the memory
*	stack, use ov_memstack_lock()/unlock() outside of this function
*/
OV_DLLFNCEXPORT OV_RESULT ov_path_resolve(
	OV_PATH			*ppath,
	const OV_PATH	*prelpath,
	const OV_STRING	pathname,
	const OV_UINT	version
) {
	/*
	*	local variables
	*/
	OV_UINT		size, relsize;
	OV_STRING	pc;
	OV_STRING       pathcopy;
	char		del, nextdel;
	OV_UINT		i;
	OV_UINT		pathlen;
	OV_STRING	identifier;
	OV_RESULT	result;
	OV_STRING	pcundo;
	char		cundo;
	/*
	*	check parameters
	*/
	if(!ppath || !pathname) {
		return OV_ERR_BADPARAM;
	}
	if(!*pathname) {
		return OV_ERR_BADPATH;
	}
	/*
	*       copy static pathname in memstack (beware of memory access to the codesegments!)
	*/
	pathcopy = ov_memstack_alloc(strlen(pathname)+1);
	strcpy(pathcopy, pathname);

	/*
	*	determine the count of path elements needed (relative part)
	*/
	pc = pathcopy;
	if(*pc == '/') {
		/*
		*	path is absolute
		*/
		relsize = 0;
		size = 1;
		pc++;
		del = '/';
	} else {
		/*
		*	path is relative
		*/
		if(!prelpath) {
			return OV_ERR_BADPATH;
		}
		size = prelpath->size;
		/*
		*	skip a leading "./"
		*/
		if((pc[0]=='.') && (pc[1]=='/')) {
			pc += 2;
		}
		/*
		*	go to current domain (object/structure)
		*/
		while(TRUE) {
			if(prelpath->elements[size-1].elemtype == OV_ET_OBJECT) {
				/*
				*	vendor objects must be handeled specially
				*/
				if((prelpath->elements[size-1].pobj >= &pdb->vendorobj[OV_NUM_VENDOROBJECTS])
					|| (prelpath->elements[size-1].pobj < &pdb->vendorobj[0])
				) {
					break;
				}
			}
			if((prelpath->elements[size-1].elemtype == OV_ET_VARIABLE)
				|| (prelpath->elements[size-1].elemtype == OV_ET_MEMBER)
			) {
				if((prelpath->elements[size-1].elemunion.pvar->v_vartype
					& OV_VT_KSMASK) == OV_VT_STRUCT
				) {
					break;
				}
			}
			size--;
			if(!size) {
				return OV_ERR_BADPATH;
			}
		}	/* while */
		/*
		*	process "../"
		*/
		while(*pc == '.') {
			if(pc[1] == '.') {
				if(pc[2] == '/') {
					/*
					*	go up one level of hierarchie
					*/
					pc += 3;
					while(TRUE) {
						size--;
						if(!size) {
							return OV_ERR_BADPATH;
						}
						if(prelpath->elements[size-1].elemtype == OV_ET_OBJECT) {
							break;
						}
						if((prelpath->elements[size-1].elemtype == OV_ET_VARIABLE)
							|| (prelpath->elements[size-1].elemtype == OV_ET_MEMBER)
						) {
							if((prelpath->elements[size-1].elemunion.pvar->v_vartype
								& OV_VT_KSMASK) == OV_VT_STRUCT
							) {
								break;
							}
						}
					}	/* while */
				} else {
					break;
				}
			} else {
				break;
			}
		}	/* while */
		/*
		*	determine current delimiter
		*/
		if(*pc == '.') {
			del = '.';
			pc++;
		} else {
			del = '/';
		}
		/*
		*	set size of relative path part
		*/
		relsize = size;
	}	/* else */
	/*
	*	determine the count of path elements needed (further part)
	*/
	pathlen = strlen(pc);
	if(pathlen) {
		size++;
	}
	for(i=0; i<pathlen; i++) {
		if((pc[i]=='/') || (pc[i]=='.')) {
			size++;
			if(version == 1) {
				if(pc[i+1] == '.') {
					i++;
				}
			}
			i++;
			if((pc[i]=='/') || (pc[i]=='.')) {
				return OV_ERR_BADPATH;
			}
		}
	}
	/*
	*	allocate memory for the new path
	*/
	ppath->elements = (OV_ELEMENT*)ov_memstack_alloc(size*sizeof(OV_ELEMENT));
	if(!ppath->elements) {
		ppath->size = 0;
		return OV_ERR_HEAPOUTOFMEMORY;
	}
	/*
	*	copy relative part or create root element
	*/
	if(relsize) {
		memcpy(ppath->elements, prelpath->elements, relsize*sizeof(OV_ELEMENT));
		ppath->size = relsize;
	} else {
		ppath->elements[0].elemtype = OV_ET_OBJECT;
		ppath->elements[0].pobj = Ov_PtrUpCast(ov_object, &pdb->root);
		ppath->size = 1;
	}
	if(*pc) {
		/*
		*	parse the rest of the path
		*/
		do {
			/*
			*	get next identifier and next delimiter
			*/
			identifier = pc;
			nextdel = 0;
			pcundo = pc;
			cundo = *pc;
			while(*pc) {
				if(*pc == '/') {
					pcundo = pc;
					cundo = *pc;
					*pc = 0;
					nextdel = '/';
					if(version == 1) {
						if(pc[1]=='.') {
							nextdel = '.';
							pc++;
						}
					}
					break;
				} else if(*pc == '.') {
					pcundo = pc;
					cundo = *pc;
					*pc = 0;
					nextdel = '.';
					break;
				}
				pc++;
			}
			switch(del) {
			case '/':
				/*
				*	next element is a child
				*/
				result = ov_element_searchchild(&ppath->elements[ppath->size-1],
					&ppath->elements[ppath->size], ov_path_frompercent(identifier));
				break;
			case '.':
				/*
				*	next element is a part
				*/
				result = ov_element_searchpart(&ppath->elements[ppath->size-1],
					&ppath->elements[ppath->size], OV_ET_ANY, ov_path_frompercent(identifier));
				break;
			default:
				/*
				*	this is strange...
				*/
				ov_logfile_warning("internal error: del = '%d'\n", (int)del);
				return OV_ERR_GENERIC;
			}
			*pcundo = cundo;
			if(Ov_Fail(result)) {
				return OV_ERR_BADPATH;
			}
			if(ppath->elements[ppath->size].elemtype == OV_ET_NONE) {
				return OV_ERR_BADPATH;
			}
			/*
			*	go on with next element
			*/
			ppath->size++;
			del = nextdel;
			pc++;
		} while(nextdel);
	}
	/*
	*	finished
	*/
	if(ppath->size != size) {
		return OV_ERR_BADPATH;
	}
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get the canonical path of an element
*	Note: the memory for the path name is allocated on the memory
*	stack, use ov_memstack_lock()/unlock() outside of this function
*/
OV_DLLFNCEXPORT OV_STRING ov_path_getcanonicalpath(
	OV_INSTPTR_ov_object	pobj,
	const OV_UINT			version
) {
	/*
	*	local variables
	*/
	OV_UINT					size = 0;
	OV_UINT					currsize;
	OV_STRING				pathname;
	OV_INSTPTR_ov_object	pcurrobj;
	/*
	*	check parameters
	*/
	if(!pobj) {
		return NULL;
	}
	/*
	*	root must be handeled specially
	*/
	if(pobj == Ov_PtrUpCast(ov_object, &pdb->root)) {
		return "/";
	}
	/*
	*	determine the size of the pathname
	*/
	for(pcurrobj=pobj; pcurrobj != Ov_PtrUpCast(ov_object, &pdb->root);
		pcurrobj=((pcurrobj->v_pouterobject)?(pcurrobj->v_pouterobject):
		(Ov_PtrUpCast(ov_object, Ov_GetParent(ov_containment, pcurrobj))))
	) {
		/*
		*	add space for the identifier and allow for version 1
		*	path names which may contain "/." as part delimiter
		*/
		size += ov_path_percentsize(pcurrobj->v_identifier)+2;
	}
	/*
	*	allocate memory for the path name on the memory stack
	*/
	pathname = (OV_STRING)ov_memstack_alloc(size+1);
	if(!pathname) {
		return NULL;
	}
	/*
	*	insert the identifiers and delimiters
	*/
	pathname += size;
	*pathname = 0;
	pcurrobj = pobj;
	while(pcurrobj != Ov_PtrUpCast(ov_object, &pdb->root)) {
		currsize = ov_path_percentsize(pcurrobj->v_identifier);
		pathname -= currsize;
		strncpy(pathname, ov_path_topercent(pcurrobj->v_identifier), currsize);
		pathname--;
		if(pcurrobj->v_pouterobject) {
			*pathname = '.';
			if(version == 1) {
				pathname--;
				*pathname = '/';
			}
			pcurrobj = pcurrobj->v_pouterobject;
		} else {
			*pathname = '/';
			pcurrobj = Ov_PtrUpCast(ov_object, Ov_GetParent(ov_containment, pcurrobj));
		}
	}
	return pathname;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get the pointer to an object with given path name
*	Note: you need NOT call ov_memstack_lock()/unlock() outside of this function
*/
OV_DLLFNCEXPORT OV_INSTPTR_ov_object ov_path_getobjectpointer(
	const OV_STRING			pathname,
	const OV_UINT			version
) {
	/*
	*	local variables
	*/
	OV_PATH		path;
	OV_ELEMENT	element;
	/*
	*	instructions
	*/
	ov_memstack_lock();
	if(Ov_Fail(ov_path_resolve(&path, NULL, pathname, version))) {
		/*
		*	bad path
		*/
		ov_memstack_unlock();
		return NULL;
	}
	element = path.elements[path.size-1];
	ov_memstack_unlock();
	if(element.elemtype == OV_ET_OBJECT) {
		return element.pobj;
	}
	/*
	*	not an object path
	*/
	return NULL;
}

/*	----------------------------------------------------------------------	*/

/*
*	Checks if characters in a valid one
*/

OV_DLLFNCEXPORT OV_BOOL ov_path_isvalidchar (
                char pcurr
) {
	if((!((pcurr >= 'a') && (pcurr <= 'z')))		/* no lower char */
		&& (!((pcurr >= 'A') && (pcurr <= 'Z')))	/* no upper char */
		&& (!((pcurr >= '0') && (pcurr <= '9')))	/* no digit */
		&& (pcurr != '_')				/* and no underscore */
	) return FALSE;						/* is not a valid char */
        return TRUE;                                            /* is a valid char */
}

/*	----------------------------------------------------------------------	*/

/*
*	returns size of a string in respect to a percent conversion
*/

OV_DLLFNCEXPORT OV_UINT ov_path_percentsize (
                OV_STRING org
) {
    OV_UINT size;
    char *d;

    size = strlen(org);
    d = org;
    while (*d) {
          if (!ov_path_isvalidchar(*d)) size = size + 2;
          d++;
    }
    return size;
}

/*	----------------------------------------------------------------------	*/

/*
*	Converts characters in an identifier-string to their percent representation
*	Note: the memory for the returned string is allocated on the memory
*	stack, use ov_memstack_lock()/unlock() outside of this function
*/

OV_DLLFNCEXPORT OV_STRING ov_path_topercent (
                OV_STRING org
) {
    OV_STRING newstring;
    unsigned char *s,*d;
    unsigned int upper, lower;

    newstring = (OV_STRING) ov_memstack_alloc(ov_path_percentsize(org)+1);
    d = org;
    s = newstring;
    while (*d) {
          if (!ov_path_isvalidchar(*d)) {
              upper = (*d) >> 4;
              lower = (*d) % 16;
              s[0] = '%';
              s[1] = (unsigned char)upper + (upper < 10 ? '0' : '7');
              s[2] = (unsigned char)lower + (lower < 10 ? '0' : '7');
              s = s + 2;
          }
          else *s = *d;
          s++;
          d++;
    }
    *s = 0;
    return newstring;
}

/*	----------------------------------------------------------------------	*/

/*
*	Converts percent characters in asci characters
*	Note: the memory for the returned string is allocated on the memory
*	stack, use ov_memstack_lock()/unlock() outside of this function
*/

OV_DLLFNCEXPORT OV_STRING ov_path_frompercent (
                OV_STRING org
) {
    OV_UINT val, size;
    OV_STRING newstring;
    char *s,*d;

    size = strlen(org);
    d = org;
    while (*d) {
          if (*d == '%') size = size - 2;
          d++;
    }

    newstring = (OV_STRING) ov_memstack_alloc(size+1);
    d = org;
    s = newstring;

    while (*d) {
          if (*d == '%') {
              val = 0;
              if( d[1] <= '9' ) {
                  val = d[1] - '0';
                  val <<= 4;
              } else if( d[1] <= 'F' ) {
                  val = d[1] - '7';
                  val <<= 4;
              } else if( d[1] <= 'f' )  {
                  val = d[1] - 'W';
                  val <<= 4;
              }
              if( d[2] <= '9' ) {
                  val += d[2] - '0';
              } else if( d[2] <= 'F' ) {
                  val += d[2] - '7';
              } else if( d[2] <= 'f' ) {
                  val += d[2] - 'W';
              }
              d = d + 2;
              *s = (char) val;
          }
          else *s = *d;
          s++;
          d++;
    }
    *s = 0;

    return newstring;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

