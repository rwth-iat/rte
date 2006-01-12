/*
*   $Id: ov_string.h,v 1.6 2006-01-12 14:10:13 markus Exp $
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

#include "ov.h"

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

/*
*	Set value of a string in the database
*/
OV_DLLFNCEXPORT OV_RESULT ov_string_setvalue(
	OV_STRING			*pstring,
	const OV_STRING		value
);

/*
*	Set value of a string vector in the database
*/
OV_DLLFNCEXPORT OV_RESULT ov_string_setvecvalue(
	OV_STRING			*pstringvec,
	const OV_STRING		*pvalue,
	OV_UINT				veclen
);

/*
*	Compare two strings, result is greater than, equal to or less than zero
*/
OV_DLLFNCEXPORT OV_INT ov_string_compare(
	const OV_STRING		string1,
	const OV_STRING		string2
);

/*
*	Get the length of a string
*/
OV_DLLFNCEXPORT OV_UINT ov_string_getlength(
	const OV_STRING		string
);

/*
*	Append a string to an existing one
*/
OV_DLLFNCEXPORT OV_RESULT ov_string_append(
	OV_STRING			*pstring,
	const OV_STRING		appstring
);

/*
*	Formatted print to a string
*/
OV_DLLFNCEXPORT OV_RESULT ov_string_print(
	OV_STRING		*pstring,
	const OV_STRING	format,
	...
);

/*
*	Test if a string matches a regular expression
*/
OV_DLLFNCEXPORT OV_BOOL ov_string_match(
	const OV_STRING		string,
	const OV_STRING		mask
);

/*
*	Convert a string to lower case
*	Note: you must call ov_memstack_lock/unlock() outside of this function!
*/
OV_DLLFNCEXPORT OV_STRING ov_string_tolower(
	const OV_STRING		string
);

/*
*	Convert a string to upper case
*	Note: you must call ov_memstack_lock/unlock() outside of this function!
*/
OV_DLLFNCEXPORT OV_STRING ov_string_toupper(
	const OV_STRING		string
);

/*
*	Helper function of ov_string_match() for sets (subroutine)
*/
int ov_string_match_set(
	const OV_STRING		mask,
	const OV_STRING		chain,
	int					*pm,
	int					*pk
);

/*
*	Helper function of ov_string_match() for jokers (subroutine)
*/
int ov_string_match_joker(
	const OV_STRING		mask,
	const OV_STRING		chain,
	int					*pm,
	int					*pk
);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif
/*
*	End of file
*/

