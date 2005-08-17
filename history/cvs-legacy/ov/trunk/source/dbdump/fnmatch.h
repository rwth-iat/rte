/*
 * $Id: fnmatch.h,v 1.4 2005-08-17 09:40:10 markus Exp $
 *
 * Copyright (c) 1996-2002
 * Lehrstuhl fuer Prozessleittechnik, RWTH Aachen
 * D-52064 Aachen, Germany.
 * All rights reserved.
 *
 * This file is part of the ACPLT/KS Package which is licensed as open
 * source under the Artistic License; you can use, redistribute and/or
 * modify it under the terms of that license.
 *
 * You should have received a copy of the Artistic License along with
 * this Package; see the file ARTISTIC-LICENSE. If not, write to the
 * Copyright Holder.
 *
 * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
 * OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/* Data structures and prototypes for string matching routines
 *
 * Author : Christian Poensgen <chris@plt.rwth-aachen.de>
 * fnmatch.h
 * last change: Feb 8, 2002
 */

#ifndef FNMATCH_H_INCLUDED
#define FNMATCH_H_INCLUDED

/******************************************************************************************/
/* Datenstrukturen */

typedef enum error_type {ERR_OK,											/* Errorcodes */
						 ERR_INVALID_MASK_FORMAT,
						 ERR_NO_HEX,
						 ERR_INVALID_MASK_CHAR,
						 ERR_NO_MEM,
						 ERR_INVALID_NAME
						} error_tt;

enum list_el_type {str_type, set_type, quest_type, ast_type};	/* Typ des Listenelements */

typedef struct list_struct {						   /* Typ für Elemente der Tokenliste */
	enum list_el_type	type_id;
	struct list_struct	*next;
	union {
		char			*string;
		unsigned char	char_field[32];
		int				number;
	} contents;
} list_t;

/******************************************************************************************/
/* Prozeduren */

#ifdef __cplusplus
extern "C" {
#endif

list_t *Acplt_Compile_Mask (char *mask, error_tt *err_c);
/* Wandelt Eingabestring in internes Maskenformat um */

int Acplt_Compare(list_t *cmask, char *name);
/* Vergleich der Maske mit gegebenem String */

void Acplt_Clear_Mask(list_t *cmask);
/* Löschen des Speicherplatzes der Maske */

int Acplt_FNMatch (char *mask, char *name, int no_case, error_tt *err_c);
/* Vergleich der Maske mit gegebenem String "name"; falls no_case <> 0, keine Unter- */
/* scheidung Gross- / Kleinschreibung. */
/* Fuehrt alle obigen Prozeduren automatisch hintereinander aus. */
/* Liefert 1 zurueck, falls Strings gleich, sonst 0. */

#ifdef __cplusplus
}
#endif

#endif /* FNMATCH_H_INCLUDED */
