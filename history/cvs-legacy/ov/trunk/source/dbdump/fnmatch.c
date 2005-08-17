/*
 * $Id: fnmatch.c,v 1.6 2005-08-17 09:40:10 markus Exp $
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

/* String matching routines supporting wildcards and sets.
 *
 * Author : Christian Poensgen <chris@plt.rwth-aachen.de>
 * fnmatch.c
 * last change: Sep 7, 2000
 */

#include "fnmatch.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/******************************************************************************************/

int is_valid_char(char c) {							/* prüft, ob c in A..Z, a..z, 0..9, _ */
													/* und liefert entspr. Klasse zurück  */

/******************************************************************************************/

	if (islower(c)) {
		return 1;
	}
	if (isupper(c)) {
		return 2;
	}
	if (isdigit(c)) {
		return 3;
	}
	if (c=='_') {
		return 4;
	}
	return 0;
}

/******************************************************************************************/

int get_hex(char *st, unsigned int j, error_tt *err_c) {	/* liefert Zeichen zu Hexwert st[j]st[j+1] */

/******************************************************************************************/

	char	hexstr[3];
	int		hexzahl;

	if (j+1 >= strlen(st)) {
		*err_c = ERR_INVALID_MASK_FORMAT;
		return(-1);
	}
	if (isxdigit(st[j]) && isxdigit(st[j+1])) {						/* gültige Hexziffer? */
		strncpy(hexstr, &st[j], 2);
		hexstr[2]='\0';
		sscanf(hexstr, "%x", &hexzahl); 						 /* Umwandlung in 1 Char */
		return (hexzahl);
	} else {														    /* keine Hexzahl */
		*err_c = ERR_NO_HEX;
		return(-1);
	}
}

/******************************************************************************************/

int save_set(char *st, list_t **pcmask, list_t **plast, unsigned int j, error_tt *err_c) {
													/* speichert Zeichenmenge als Bitfeld */

/******************************************************************************************/

	list_t	*list_el;
	char	x;
	unsigned int		k;

	if (! (is_valid_char(st[j]) || st[j] == '^' || st[j] == '%')) {		/* falsches Format */
		*err_c = ERR_INVALID_MASK_CHAR;
		return(-1);
	}

	list_el = (list_t*)malloc(sizeof(list_t));						/* neues Listenelement */
	if (list_el == NULL) {
		*err_c = ERR_NO_MEM;
		return(-1);
	}
	list_el->type_id = set_type;
	if (*plast == NULL) {
		*pcmask = list_el;
	} else {
		(*plast)->next=list_el;
	}
	*plast=list_el;
 	list_el->next=NULL;

	if (st[j]=='^') {												/* falls negierte Menge */

		memset(list_el->contents.char_field, 255, sizeof(list_el->contents.char_field));
														/* Initialisierung des Feldes mit 1 */

		while (j < strlen(st) && st[j] != ']') {
			if (j+1 >= strlen(st)) {
				*err_c = ERR_INVALID_MASK_FORMAT;
				return(-1);
			}
			if (st[j] == '%') {												/* falls Hexwert */
				st[j]=(char)get_hex(st,j+1,err_c);				/* einsetzen des ASCII-Wertes */
				if (st[j]==-1) {
					return(-1);
				}
				for(k=j+1; k < strlen(st) - 2; k++) {
					st[k] = st[k+2];
				}
				st[strlen(st) - 2] = '\0';
				if (st[j+1] != '-') {
					list_el->contents.char_field[st[j] >> 3] |= (1 << (7 - (st[j] & 0x07)));
											/* entspr. Bit des Feldes setzen;   & 0x07 = % 8 */
					j++;
					continue;
				} else {
					*err_c = ERR_INVALID_MASK_FORMAT;
					return(-1);
				}
			}
			if (is_valid_char(st[j]) && st[j+1] != '-') {		/* falls einzelne(s) Zeichen */
				list_el->contents.char_field[st[j] >> 3] &= (~(1 << (7 - (st[j] & 0x07))));
				j++;
				continue;
			}
			if (j+2 < strlen(st) && is_valid_char(st[j]) && st[j+1] == '-'
				&& is_valid_char(st[j]) == is_valid_char(st[j+2]) && st[j] < st[j+2]) {
																		/* falls Zeichenbereich */
				for (x=st[j];x<=st[j+2];x++) {
					list_el->contents.char_field[x >> 3] &= (~(1 << (7 - (x & 0x07))));
				}
				j = j + 3;
				continue;
			} else {
				*err_c = ERR_INVALID_MASK_FORMAT;
				return(-1);
			}
		}

	} else { 															/* falls nicht negiert */

		memset(list_el->contents.char_field, 0, sizeof(list_el->contents.char_field));

		while (j < strlen(st) && st[j] != ']') {
			if (j+1 >= strlen(st)) {
				*err_c = ERR_INVALID_MASK_FORMAT;
				return(-1);
			}
			if (st[j] == '%') {
				st[j]=(char)get_hex(st,j+1,err_c);
				if (st[j]==-1) {
					return(-1);
				}
				for(k=j+1; k < strlen(st) - 2; k++) {
					st[k] = st[k+2];
				}
				st[strlen(st) - 2] = '\0';
				if (st[j+1] != '-') {
					list_el->contents.char_field[st[j] >> 3] |= (1 << (7 - (st[j] & 0x07)));
					j++;
					continue;
				} else {
					*err_c = ERR_INVALID_MASK_FORMAT;
					return(-1);
				}
			}
			if (is_valid_char(st[j]) && st[j+1] != '-') {
				list_el->contents.char_field[st[j] >> 3] |= (1 << (7 - (st[j] & 0x07)));
				j++;
				continue;
			}
			if (j+2 < strlen(st) && is_valid_char(st[j]) && st[j+1] == '-'
				&& is_valid_char(st[j]) == is_valid_char(st[j+2]) && st[j] < st[j+2]) {

				for (x=st[j];x<=st[j+2];x++) {
					list_el->contents.char_field[x >> 3] |= (1 << (7 - (x & 0x07)));
				}
				j = j + 3;
				continue;
			} else {
				*err_c = ERR_INVALID_MASK_FORMAT;
				return(-1);
			}
		}
	}

	return (j+1);											/* Position im Eingabestring */
}

/******************************************************************************************/

list_t *Acplt_Compile_Mask (char *mask, error_tt *err_c) {
									/* wandelt Eingabestring in internes Maskenformat um */

/******************************************************************************************/

	list_t	*list_el, *cmask, *last;

	unsigned int		i, j, k;
	char	hex;

	i = 0;													/* Position im Eingabestring */
	cmask = NULL;
	last = NULL;

	j=0;
	while (mask[j]==' ' || mask[j]=='\t') {		/* White space am Anfang und Ende entfernen */
		j++;
	}
	if (j != 0) {
		for (k=0; k+j < strlen(mask); k++) {
			mask[k] = mask[k+j];
		}
		mask[strlen(mask)-j] = '\0';
	}
	j=strlen(mask)-1;
	while (mask[j]==' ' || mask[j]=='\t') {
		j--;
	}
	mask[j+1] = '\0';


	while (i < strlen(mask)) {

		if (is_valid_char(mask[i])) {					/* falls mask[i] normales Zeichen */
			list_el = (list_t*)malloc(sizeof(list_t));				/* neues Listenelement */
			if (list_el == NULL) {
				*err_c = ERR_NO_MEM;
				return(NULL);
			}
			list_el->type_id = str_type;
			list_el->contents.string = (char*)malloc(2);
			if (list_el->contents.string == NULL) {
				*err_c = ERR_NO_MEM;
				return(NULL);
			}
			if (last == NULL) {
				cmask = list_el;
			} else {
				last->next=list_el;
			}
			last=list_el;
			list_el->next=NULL;
			list_el->contents.string[0]=mask[i];	/* Zeichen unverändert in Maske eintragen */
			list_el->contents.string[1]='\0';
			i++;
			while (i<strlen(mask) && is_valid_char(mask[i])) {
				strcat(list_el->contents.string," ");		/* evt. weitere Zeichen anhängen */
            list_el->contents.string[strlen(list_el->contents.string)-1]=mask[i];
				i++;
			}
			continue;
		}

		if (mask[i]=='%') {								/* falls nächstes Zeichen Hexwert %xx */
			list_el = (list_t*)malloc(sizeof(list_t));
			if (list_el == NULL) {
				*err_c = ERR_NO_MEM;
				return(NULL);
			}
			list_el->type_id = str_type;
			list_el->contents.string = (char*)malloc(2);
			if (list_el->contents.string == NULL) {
				*err_c = ERR_NO_MEM;
				return(NULL);
			}
			if (last == NULL) {
				cmask = list_el;
			} else {
				last->next=list_el;
			}
			last=list_el;
			list_el->next=NULL;
			hex=(char)get_hex(mask, i+1,err_c);					/* %xx in ein Char umwandeln */
			if (hex==-1) {
				return(NULL);
			}
			list_el->contents.string[0]=hex;
			list_el->contents.string[1]='\0';
			i = i + 3;
			continue;
		}
		if (mask[i]=='[') {							/* falls Beginn einer Mengenangabe [...] */
			i = save_set(mask, &cmask, &last, i+1, err_c);		/* Menge in Bitfeld speichern */
			if (i==-1) {
				return(NULL);
			}
			continue;
		}
		if (mask[i]=='?') {								/* ? als Platzhalter für ein Zeichen */
			list_el = (list_t*)malloc(sizeof(list_t));
			if (list_el == NULL) {
				*err_c = ERR_NO_MEM;
				return(NULL);
			}
			list_el->type_id = quest_type;
			if (last == NULL) {
				cmask = list_el;
			} else {
				last->next = list_el;
			}
			last=list_el;
			list_el->next = NULL;
			list_el->contents.number = 1;
			i++;
			while (i<strlen(mask) && mask[i]=='?') {
				(list_el->contents.number)++;		/* Anzahl aufeinanderfolgender ? speichern */
				i++;
			}
			continue;
		}
		if (mask[i]=='*') {			/* * als Platzhalter für beliebig viele (oder 0) Zeichen */
			while (i<strlen(mask) && mask[i]=='*') {
				i++;
			}
			list_el = (list_t*)malloc(sizeof(list_t));
			if (list_el == NULL) {
				*err_c = ERR_NO_MEM;
				return(NULL);
			}
		    list_el->type_id = ast_type;
			if (last == NULL) {
				cmask = list_el;
			} else {
				last->next = list_el;
			}
			last = list_el;
			list_el->next = NULL;
		} else {
			*err_c = ERR_INVALID_MASK_CHAR;
			return(NULL);
		}
	}
	return (cmask);														/* kompilierte Maske */
}

/******************************************************************************************/

int comp(list_t *cmask, char *name, unsigned int i) {	/* Vergleich der Maske mit gegebenem String */

/******************************************************************************************/

	list_t	*list_el;
	unsigned int	j;

	list_el = cmask;

	while (list_el != NULL && i <= strlen(name)) {
		switch (list_el->type_id) {
			case str_type : for (j=0; j < strlen(list_el->contents.string); j++) {
								if (list_el->contents.string[j] != name[i+j]) {
									return 0;
								}
							}
							i=i+j;
							break;
			case set_type : if (! (((list_el->contents.char_field[name[i] >> 3]
									>> (7 - (name[i] & 0x07))) << 7) & 128) ) {
																		/* & 0x07 = % 8 */
								return 0;
							}
							i++;
							break;
			case quest_type : for (j=1; (int) j <= list_el->contents.number; j++) {
								  i++;
							  }
							  break;
			case ast_type : if(list_el->next == NULL) {
								return 1;
							}
							for (j=i; j < strlen(name); j++) {
								if(comp(list_el->next,name,j)) {		/* rekursiver Aufruf */
									return 1;
								}
							}
							return 0;
		}
		list_el = list_el->next;
	}

	if (list_el == NULL && i == strlen(name)) {
		return 1;													/* Übereinstimmung */
	} else {
		return 0;												/* keine Übereinstimmung */
	}
}

/******************************************************************************************/

int Acplt_Compare(list_t *cmask, char *name) {

/******************************************************************************************/

	return (comp(cmask, name, 0));					/* Aufruf der rekursiven Proz. comp */
}

/******************************************************************************************/

void Acplt_Clear_Mask(list_t *cmask) {			/* Löschen des Speicherplatzes der Maske */

/******************************************************************************************/

	list_t	*actual, *help;

	actual=cmask;
	while (actual!=NULL) {
		help=actual->next;
		if (actual->type_id == str_type) {
			free(actual->contents.string);
		}
		free(actual);
		actual=help;
	}
}

/******************************************************************************************/

int Acplt_FNMatch (char *mask, char *name, int no_case, error_tt *err_c) {

/******************************************************************************************/

	list_t	*compiled_mask;
	int		result;
	unsigned int	i, j;

	*err_c = ERR_OK;
	if (mask == NULL) {
		*err_c = ERR_INVALID_MASK_FORMAT;
		return(-1);
	}
	if (name == NULL) {
		*err_c = ERR_INVALID_NAME;
		return(-1);
	}
	if (no_case) {						/* falls <> 0, dann Groß-/Kleinschreibung ignorieren */
		for (i=0; i < strlen(mask); i++) {		/* evtl. alles in Kleinschreibung umwandeln */
			mask[i] = tolower(mask[i]);
		}
		for (i=0; i < strlen(name); i++) {
			name[i] = tolower(name[i]);
		}
	}
	for (i=0; i < strlen(name); i++) {			/* Hexwerte %xx im Namen in Char umwandeln */
		if (! (is_valid_char(name[i]) || name[i]=='%')) {
			*err_c = ERR_INVALID_NAME;
			return(-1);
		}
		if (name[i]=='%') {
			name[i] = (char)get_hex(name, i+1, err_c);
			if (name[i]==-1) {
				return(-1);
			}
			for(j=i+1; j < strlen(name) - 2; j++) {
				name[j] = name[j+2];
			}
			name[strlen(name) - 2] = '\0';
		}
	}
	compiled_mask=Acplt_Compile_Mask(mask, err_c);					/* Maske kompilieren */
	if (compiled_mask==NULL) {
		return(-1);
	}
	result=Acplt_Compare(compiled_mask, name);	/* kompilierte Maske mit Name vergleichen */
	Acplt_Clear_Mask(compiled_mask);									/* Maske löschen */
	return (result);						/* 1=Übereinstimmung, 0=keine Übereinstimmung */
}
