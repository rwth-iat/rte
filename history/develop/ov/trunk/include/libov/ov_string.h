/*
*   $Id: ov_string.h,v 1.7 2011-01-18 14:27:34 martin Exp $
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

#ifndef OV_STRING_H_INCLUDED
#define OV_STRING_H_INCLUDED

#include "libov/ov_ov.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
*	OV_STRINGCOMPARE:
*	----------------
*	Result of function ov_string_compare(s1, s2)
*/
#define	OV_STRCMP_EQUAL	0		/* s1 = s2 */
#define	OV_STRCMP_MORE		1		/* s1 > s2 */
#define	OV_STRCMP_LESS		-1		/* s1 < s2 */

/**
*	Set value of a string in the database
*/
OV_DLLFNCEXPORT OV_RESULT ov_string_setvalue(
	OV_STRING			*pstring,
	const OV_STRING		value
);

/**
*	Set value of a string vector in the database
*/
OV_DLLFNCEXPORT OV_RESULT ov_string_setvecvalue(
	OV_STRING			*pstringvec,
	const OV_STRING		*pvalue,
	OV_UINT				veclen
);

/**
*	Compare two strings, result is greater than, equal to or less than zero
*	Please check against OV_STRCMP_EQUAL, OV_STRCMP_MORE and OV_STRCMP_LESS
*	OV_STRCMP_MORE indicates that the first character that does not match has a greater value in string1 than in string2
*	Note: NULL is considered equal to ""
*
*/
OV_DLLFNCEXPORT OV_INT ov_string_compare(
	const OV_STRING		string1,
	const OV_STRING		string2
);

/**
 * Same as ov_string_compare but not case_sensitive
 * upper case comparison is used
 */
OV_DLLFNCEXPORT OV_INT ov_string_comparei(const OV_STRING s1, const OV_STRING s2);


/**
*	Get the length of a string (without the null terminator)
*	results zero if got a NULL pointer
*/
OV_DLLFNCEXPORT OV_UINT ov_string_getlength(
	const OV_STRING		string
);

/**
*	Append a string to an existing one
*	appending a NULL pointer results in a OV_ERR_BADPARAM
*/
OV_DLLFNCEXPORT OV_RESULT ov_string_append(
	OV_STRING			*pstring,
	const OV_STRING		appstring
);

/**
*	Formatted print to a string
*	crashes if a used value contains a NULL pointer
*/
OV_DLLFNCEXPORT OV_RESULT ov_string_print(
	OV_STRING		*pstring,
	const OV_STRING	format,
	...
);

/**
*	Test if a string matches a regular expression
*	? is an arbitrary character
*	* is an arbitrary amount of any characters
*/
OV_DLLFNCEXPORT OV_BOOL ov_string_match(
	const OV_STRING		string,
	const OV_STRING		mask
);

/**
*	Convert a string to lower case
*	Note: you must call ov_memstack_lock/unlock() outside of this function!
*/
OV_DLLFNCEXPORT OV_STRING ov_string_tolower(
	const OV_STRING		string
);

/**
*	Convert a string to upper case
*	Note: you must call ov_memstack_lock/unlock() outside of this function!
*/
OV_DLLFNCEXPORT OV_STRING ov_string_toupper(
	const OV_STRING		string
);

/**
*	Helper function of ov_string_match() for sets (subroutine)
*/
int ov_string_match_set(
	const OV_STRING		mask,
	const OV_STRING		chain,
	int					*pm,
	int					*pk
);

/**
*	Helper function of ov_string_match() for jokers (subroutine)
*/
int ov_string_match_joker(
	const OV_STRING		mask,
	const OV_STRING		chain,
	int					*pm,
	int					*pk
);

/**
*	Split a string.
*	ov_string_split("a;b;c",";",&len) => len is 3
*	ov_string_split(";",";",&len) => len is 2
*	ov_string_split("a",";",&len) => len is 1
*	ov_string_split(NULL,";",&len) => len is 0
*	---------------
*	OV_STRING *plist;
*	OV_UINT    i,len;
*	
*	plist = ov_string_split("a;b;c",";",&len);
*	for(i=0; i<len; i++) {
*		fprintf(stderr, "%lu : %s\n", i, plist[i]);
*	}
*	ov_string_freelist(plist);
*/
OV_DLLFNCEXPORT OV_STRING *ov_string_split(
	const OV_STRING		str,
	const OV_STRING		sep,
	OV_UINT             *len
);

/**
 * frees a list
 * checks for a NULL pointer, but does not prevent double freeing
 */
OV_DLLFNCEXPORT void ov_string_freelist(
	OV_STRING *plist
);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif
/*
*	End of file
*/

