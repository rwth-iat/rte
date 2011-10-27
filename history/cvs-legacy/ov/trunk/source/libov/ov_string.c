/*
*   $Id: ov_string.c,v 1.10 2010-12-20 13:23:06 martin Exp $
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
*	20-Jan-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	13-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*	07-Jun-2001 J.Nagelmann <nagelmann@ltsoft.de>: Changes for Sun Solaris.
*/

#define OV_COMPILE_LIBOV

#include "libov/ov_string.h"
#include "libov/ov_database.h"
#include "libov/ov_memstack.h"
#include "libov/ov_macros.h"

#include <stdarg.h>
#include <ctype.h>

/*	----------------------------------------------------------------------	*/

/*
*	Set value of a string in the database
*/
OV_DLLFNCEXPORT OV_RESULT ov_string_setvalue(
	OV_STRING			*pstring,
	const OV_STRING		value
) {
	/*
	*	local variables
	*/
	OV_STRING	string;
	/*
	*	check parameters
	*/
	if(!pstring) {
		return OV_ERR_BADPARAM;
	}
	/*
	*	free string if there's no value
	*/
	if(!value) {
		if(*pstring) {
			ov_database_free(*pstring);
			*pstring = NULL;
		}
		return OV_ERR_OK;
	}
	if(!*value) {
		if(*pstring) {
			ov_database_free(*pstring);
			*pstring = NULL;
		}
		return OV_ERR_OK;
	}
	/*
	*	allocate memory for new string
	*/
	string = ov_database_realloc(*pstring, strlen(value)+1);
	if(!string) {
		return OV_ERR_DBOUTOFMEMORY;
	}
	/*
	*	copy the new value
	*/
	strcpy(string, value);
	/*
	*	finish
	*/
	*pstring = string;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Set value of a string vector in the database
*/
OV_DLLFNCEXPORT OV_RESULT ov_string_setvecvalue(
	OV_STRING			*pstringvec,
	const OV_STRING		*pvalue,
	OV_UINT				veclen
) {
	/*
	*	local variables
	*/
	OV_UINT			i;
	OV_STRING		*pstring;
	const OV_STRING	*pcurrvalue;
	OV_RESULT		result;
	/*
	*	check parameters
	*/
	if(!pstringvec) {
		return OV_ERR_BADPARAM;
	}
	/*
	*	set strings
	*/
	if(!pvalue) {
		/*
		*	clear all strings
		*/
		for(i=0, pstring=pstringvec; i<veclen; i++, pstring++) {
			Ov_WarnIfNot(Ov_OK(ov_string_setvalue(pstring, NULL)));
			Ov_WarnIf(*pstring);
		}
		return OV_ERR_OK;
	}
	for(i=0, pstring=pstringvec, pcurrvalue=pvalue; 
		i<veclen; 
		i++, pstring++, pcurrvalue++
	) {
		result = ov_string_setvalue(pstring, *pcurrvalue);
		if(Ov_Fail(result)) {
			for(i=0, pstring=pstringvec; i<veclen; i++, pstring++) {
				ov_string_setvalue(pstring, NULL);
			}
			return result;
		}
	}
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Compare two strings, result is greater than, equal to or less than zero
*/
OV_DLLFNCEXPORT OV_INT ov_string_compare(
	const OV_STRING		string1,
	const OV_STRING		string2
) {
	if(string1) {
		if(string2) {
			return strcmp(string1, string2);
		}
		if(!*string1) {
			return OV_STRCMP_EQUAL;		/* "" == NULL */
		}
		return OV_STRCMP_MORE;			/* "xxx" > NULL */
	}
	if(string2) {
		if(!*string2) {
			return OV_STRCMP_EQUAL;		/* NULL == "" */
		}
		return OV_STRCMP_LESS;			/* NULL < "xxx" */
	}
	return OV_STRCMP_EQUAL;				/* NULL == NULL */
}

/*	----------------------------------------------------------------------	*/

/*
*	Get the length of a string
*/
OV_DLLFNCEXPORT OV_UINT ov_string_getlength(
	const OV_STRING		string
) {
	if(string) {
		return strlen(string);
	}
	return 0;
}

/*	----------------------------------------------------------------------	*/

/*
*	Append a string to an existing one
*/
OV_DLLFNCEXPORT OV_RESULT ov_string_append(
	OV_STRING			*pstring,
	const OV_STRING		appstring
) {
	/*
	*	local variables
	*/
	OV_UINT		oldlength, newlength;
	OV_STRING	newstring;
	/*
	*	check parameters
	*/
	if(!pstring || !appstring) {
		return OV_ERR_BADPARAM;
	}
	/*
	*	calculate old and new string length and reallocate memory
	*/
	if(*pstring) {
		oldlength = strlen(*pstring);
	} else {
		oldlength = 0;
	}
	newlength = oldlength+strlen(appstring);
	if(!newlength) {
		return ov_string_setvalue(pstring, NULL);
	}
	newstring = (OV_STRING)ov_database_realloc(*pstring, newlength+1);
	if(!newstring) {
		return OV_ERR_DBOUTOFMEMORY;
	}
	/*
	*	append new string
	*/
	strcpy(newstring+oldlength, appstring);
	*pstring = newstring;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Formatted print to a string
*/
OV_DLLFNCEXPORT OV_RESULT ov_string_print(
	OV_STRING		*pstring,
	const OV_STRING	format,
	...
) {
	/*
	*	local variables
	*/
	va_list		args;
	OV_UINT		length;
	OV_STRING	pc;
	OV_RESULT	result;
	/*
	*	check parameters
	*/
	if(!pstring || !format) {
		return OV_ERR_BADPARAM;
	}
	/*
	*	calculate buffer size
	*/
	va_start(args, format);
	length = strlen(format)+1;
	pc = format;
	while(*pc) {
	    /*
	    *	Skip first '%'
	    */	    
		if(*pc++ == '%') {
		    /*
		    *	Ignore '%%'
		    */		    
		    if(*pc == '%') {
		        pc++;
		        continue;
		    }
			/*
			*	handle flag characters
			*/
			while((*pc=='-') || (*pc=='+') || (*pc==' ') || (*pc=='#') || (*pc=='0')) {
				pc++;
			}
			/*
			*	handle width specifiers
			*/
			if(*pc == '*') {
				pc++;
				length += abs(va_arg(args, int));
			} else {
				length += strtoul(pc, &pc, 10);
			}
			/*
			*	handle precision specifiers
			*/
			if(*pc == '.') {
				pc++;
				if(*pc == '*') {
					pc++;
					length += abs(va_arg(args, int));
				} else {
					length += strtoul(pc, &pc, 10);
				}
			}
			/*
			*	handle input size modifiers
			*/
			while((*pc=='h') || (*pc=='l') || (*pc=='L') || (*pc=='F') || (*pc=='N')) {
				pc++;
			}
			/*
			*	handle conversion type character
			*/
			switch(*pc) {
			case 's':
				length += strlen(va_arg(args, char*));
				break;
			case 'c':
			case 'd':
			case 'i':
			case 'o':
			case 'u':
			case 'x':
			case 'X':
				/*
				*	reserve space, 32 should be enough
				*/
				length += 32;
				(void)va_arg(args, int);
				break;
			case 'e':
			case 'f':
			case 'g':
			case 'E':
			case 'G':
				/*
				*	reserve space; apart from the 32 bytes we take into
				*	account another 308 bytes (308 is the largest possible
				*	exponent of an IEEE double)
				*/
				length += 340;
				(void)va_arg(args, double);
				break;
			case 'n':
			case 'p':
				/*
				*	reserve space, 32 should be enough
				*/
				length += 32;
				(void)va_arg(args, char*);
				break;
			case '%':
				break;
			default:
				/*
				*	should never happen, but reserve space (just in case...)
				*/
				length += 32;
				break;
			}
		}
	}
	va_end(args);
	/*
	*	allocate memory for the string on the memory stack
	*/
	ov_memstack_lock();
	pc = (OV_STRING)ov_memstack_alloc(length);
	if(!pc) {
		ov_memstack_unlock();
		return OV_ERR_HEAPOUTOFMEMORY;
	}
	/*
	*	create the actual string
	*/
	va_start(args, format);
#if defined(OV_DEBUG) && OV_SYSTEM_UNIX && !OV_SYSTEM_SOLARIS
	if(vsnprintf(pc, length, format, args) >= length) {
		Ov_Warning("string buffer too small");
		ov_memstack_unlock();
		return OV_ERR_GENERIC;
	}	
#else
	vsprintf(pc, format, args);
#endif
	va_end(args);
	/*
	*	set the string value
	*/
	result = ov_string_setvalue(pstring, pc);
	ov_memstack_unlock();
	return result;
}

/*	----------------------------------------------------------------------	*/

/*
*	Convert a string to lower case
*	Note: you must call ov_memstack_lock/unlock() outside of this function!
*/
OV_DLLFNCEXPORT OV_STRING ov_string_tolower(
	const OV_STRING		string
) {
	/*
	*	local variables
	*/
	OV_STRING plower, pfrom, pto;
	/*
	*	instructions
	*/
	if(!string) {
		return NULL;
	}
	plower = (OV_STRING)ov_memstack_alloc(strlen(string)+1);
	if(!plower) {
		return NULL;
	}
	for(pfrom=string, pto=plower; *pfrom; pfrom++, pto++) {
		*pto = tolower(*pfrom);
	}
	pto = '\0';		/*append terminating '\0'*/
	return plower;
}

/*	----------------------------------------------------------------------	*/

/*
*	Convert a string to upper case
*	Note: you must call ov_memstack_lock/unlock() outside of this function!
*/
OV_DLLFNCEXPORT OV_STRING ov_string_toupper(
	const OV_STRING		string
) {
	/*
	*	local variables
	*/
	OV_STRING pupper, pfrom, pto;
	/*
	*	instructions
	*/
	if(!string) {
		return NULL;
	}
	pupper = (OV_STRING)ov_memstack_alloc(strlen(string)+1);
	if(!pupper) {
		return NULL;
	}
	for(pfrom=string, pto=pupper; *pfrom; pfrom++, pto++) {
		*pto = toupper(*pfrom);
	}
	pto = '\0';		/*append terminating '\0'*/
	return pupper;
}

/*	----------------------------------------------------------------------	*/

/*
*	Test if a string matches a regular expression
*/
OV_DLLFNCEXPORT OV_BOOL ov_string_match(
	const OV_STRING		string,
	const OV_STRING		mask
) {
	/*
	*	local variables
	*/
	int r;
	int i = 0;
	int j = 0;
	/*
	*	implementation
	*/
	if (string == NULL || mask == NULL)
	{
		if (string == NULL)
		{
			if (mask == NULL)
				return TRUE;		/* NULL == NULL */
				
			return FALSE;			/* NULL != "xxx" */
		};
		return FALSE;				/* "xxx" != NULL */
	};
	
	while(mask[i] && string[j])
	{
		switch(mask[i])
		{
			case '[':
				if(mask[i+1] != '^') {
					r = ov_string_match_set(mask, string, &i, &j);
				} else {
					i++;
					r = !(ov_string_match_set(mask, string, &i, &j));
				}
				if(!r) {
					return FALSE;
				}
				i++;
				j++;
				break;
	
			case '*':
				r = ov_string_match_joker(mask, string, &i, &j);
				if(r) {
					return TRUE;
				}
				return FALSE;
	
			case '?':
				i++;
				j++;
				break;
	
			case '\\':
				i++;
				/* no break */
	
			default:
				if(mask[i] == string[j]) {
					i++;
					j++;
				} else {
					return FALSE;
				}
				break;
		} /* switch */
	} /* while */
	
	if(mask[i] == string[j]) {
		return TRUE;
	}
	if(!string[j]) {
        while(mask[i]) {
            if(mask[i] != '*') {
                return FALSE;
            }
            i++;
        }
        return TRUE;
    }
	return FALSE;
}

/*	----------------------------------------------------------------------	*/

/*
*	Helper function of ov_string_match() for sets (subroutine)
*/
int ov_string_match_set(
	const OV_STRING		mask,
	const OV_STRING		chain,
	int					*pm,
	int					*pk
) {
	/*
	*	local variables
	*/
	char ch;
	char ch1;
	char ch2;
	/*
	*	implementation
	*/
	(*pm)++;

	while(mask[*pm] != ']') {
		if(mask[*pm+1] != '-') {
			if(mask[*pm] == chain[*pk]) {
				while(mask[*pm] != ']') {
					(*pm)++;
				}
			 	return 1;
			} else {
				(*pm)++;
			}
		} else {
			if (mask[*pm] < mask[*pm+2]) {
				ch1=mask[*pm];
				ch2=mask[*pm+2];
			} else {
				ch1=mask[*pm+2];
				ch2=mask[*pm];
			}
			for(ch=ch1; ch<=ch2; ch++) {
				if(ch == chain[*pk]) {
					while(mask[*pm]!=']') {
						(*pm)++;
					}
				 	return 1;
				}
			}
		 	(*pm) += 3;
		}
	} /* while */
	return 0;
}

/*	----------------------------------------------------------------------	*/

/*
*	Helper function of ov_string_match() for jokers (subroutine)
*/
int ov_string_match_joker(
	const OV_STRING		mask,
	const OV_STRING		chain,
	int					*pm,
	int					*pk
) {
	/*
	*	local variables
	*/
	int oldposm = *pm;
	int oldposk = *pk;
	int start;
	int init	= 1;
	int r		= 0;
	/*
	*	implementation
	*/
	(*pm)++;

	if(!(mask[*pm])) {
		return 1;
	}

	while(chain[*pk]) {
		switch (mask[*pm]) {
		case '[':
			if(mask[*pm+1]!='^') {
				start = *pm;
				if(init) {
					while((!r) && chain[*pk]) {
						*pm = start;
						r = ov_string_match_set(mask, chain, pm, pk);
						(*pk)++;
					}
					if(!r) {
						return 0;
					}
					init = 0;
					(*pk)--;
				} else {
					r = ov_string_match_set(mask, chain, pm, pk);
					if(r) {
						(*pm)++;
						(*pk)++;
					} else {
						if(chain[*pk]) {
							init = 1;
							r = 0;
							*pm = oldposm;
							oldposk++;
							*pk = oldposk;
						} else {
							return 0;
						}
					}
				}
			} else {
				(*pm)++;
				start = *pm;
				if(init) {
					while((!r) && chain[*pk]) {
						*pm = start;
						r = !(ov_string_match_set(mask, chain, pm, pk));
						(*pk)++;
					}
					if(!r) {
						return 0;
					}
					init = 0;
					(*pk)--;
				} else {
					r = !(ov_string_match_set(mask, chain, pm, pk));
					if(r) {
						(*pm)++;
						(*pk)++;
					} else {
						if(chain[*pk]) {
							init = 1;
							*pm = oldposm;
							oldposk++;
							*pk = oldposk;
							r = 0;
						} else {
							return 0;
						}
					}
				}
			}
			(*pm)++;
			(*pk)++;
			break;
	 
		case '*':
			r = ov_string_match_joker(mask, chain, pm, pk);
			if(r) {
				return 1;
			}
			return 0;
		
		case '?':
			(*pm)++;
			(*pk)++;
			break;
	 
		case '\\':
			(*pm)++;
			/* no break */
	 
		default:
			if(init) {
				while((mask[*pm] != chain[*pk]) && chain[*pk]) {
					(*pk)++;
				}
				init = 0;
			}
			if(mask[*pm] == chain[*pk]) {
				(*pm)++;
				(*pk)++;
			} else {
				if(chain[*pk]) {
					init = 1;
					*pm = oldposm;
					r = 0;
					oldposk++;
					*pk = oldposk;
				} else {
					return 0;
				}
			}
			break;
		} /* switch */
	}	/* while	*/
	if(mask[*pm] == chain[*pk]) {
		return 1;
	}
	return (0);
}

/*	----------------------------------------------------------------------	*/

/*
*   Split a string
*/
OV_DLLFNCEXPORT OV_STRING *ov_string_split(
	const OV_STRING		str,
	const OV_STRING		sep,
	OV_UINT             *len
) {
	/*
	*	local variables
	*/
	OV_STRING  pStr;
	OV_STRING  pC;
	OV_UINT    i,size,sepLen,strLen;
	OV_STRING  *ret;
	/*
	*	instructions
	*/
    *len  = 0;

    if(!str || !(*str)) {
        return (OV_STRING*)0;
    }
    
    /*
    * Figure out how much space to allocate.  There must be enough
    * space for both the array of pointers and also for a copy of
    * the list.
    */
    sepLen = ov_string_getlength(sep);
    strLen = ov_string_getlength(str);
    if(sepLen == 0) {
        *len = strLen;
        size = strLen * (2 + sizeof(OV_STRING)) + 1;
        ret  = (OV_STRING *)Ov_HeapMalloc(size);
        if(!ret) {
            *len = 0;
            return ret;            
        }
        pStr = str;
        pC = (OV_STRING)ret;
        pC += strLen * sizeof(OV_STRING);
        i = 0;
        while(pStr && (*pStr)) {
            *pC = *pStr;
            ret[i] = pC;
            pC++; *pC = '\0';

            i++;
            pC++;
            pStr++;            
        }
        return ret;
    }
    
    size = 1;
    pStr = str;
    while( (pC = strstr(pStr, sep)) != NULL ) {
        size++;
        pStr = pC;
        pStr += sepLen;
    }

    *len = size;
    size = size * sizeof(OV_STRING) + strLen + 1;
    ret  = (OV_STRING *)Ov_HeapMalloc(size);
    if(!ret) {
        *len = 0;
        return ret;            
    }
    pStr = (OV_STRING)ret;
    pStr += (*len) * sizeof(OV_STRING);
    strcpy(pStr, str);
    
    ret[0] = pStr;
    i = 1;
    while( (pC = strstr(pStr, sep)) != NULL ) {
        *pC = '\0';
        pC += sepLen;
        pStr = pC;            
        ret[i++] = pC;
    }
    
    return ret;
}

OV_DLLFNCEXPORT void ov_string_freelist(
    OV_STRING *plist
) {
    if( plist && (*plist) ) {
    	Ov_HeapFree((OV_POINTER)plist);
    }
    return;
}


/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

