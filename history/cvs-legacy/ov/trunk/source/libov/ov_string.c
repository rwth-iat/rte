/*
*   $Id: ov_string.c,v 1.1 1999-07-19 15:02:14 dirk Exp $
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
*/

#define OV_COMPILE_LIBOV

#include "libov/ov_string.h"
#include "libov/ov_database.h"

/*	----------------------------------------------------------------------	*/

/*
*	Set value of a string in the database
*/
OV_RESULT OV_DLLFNCEXPORT ov_string_setvalue(
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
OV_RESULT OV_DLLFNCEXPORT ov_string_setvecvalue(
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
	if(!pstringvec || !pvalue) {
		return OV_ERR_BADPARAM;
	}
	/*
	*	set strings
	*/
	for(i=0, pstring = pstringvec, pcurrvalue = pvalue; 
		i<veclen; 
		i++, pstring++, pcurrvalue++
	) {
		result = ov_string_setvalue(pstring, *pcurrvalue);
		if(Ov_Fail(result)) {
			return result;
		}
	}
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Compare two strings, result is -1, 0 or 1.
*/
OV_INT OV_DLLFNCEXPORT ov_string_compare(
	const OV_STRING		string1,
	const OV_STRING		string2
) {
	if(string1) {
		if(string2) {
			return strcmp(string1, string2);
		}
		if(!*string1) {
			return 0;						/* "" == NULL */
		}
		return 1;							/* "xxx" > NULL */
	}
	if(string2) {
		if(!*string2) {
			return 0;						/* NULL == "" */
		}
		return -1;							/* NULL < "xxx" */
	}
	return 0;								/* NULL == NULL */
}

/*	----------------------------------------------------------------------	*/

/*
*	Test if a string matches a regular expression
*/
OV_BOOL OV_DLLFNCEXPORT ov_string_match(
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
	while(mask[i] && string[j]) {
		switch(mask[i]) {
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
*	End of file
*/

