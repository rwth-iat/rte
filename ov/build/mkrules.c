/*
*   $Id: mkrules.c,v 1.1 1999-07-19 15:01:53 dirk Exp $
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
*	26-Nov-1998 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	16-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	int i;
	FILE* fp;
	if(argc & 1) {
		return EXIT_FAILURE;
	}
	fp = fopen(argv[1], "w");
	if(!fp) {
		return EXIT_FAILURE;
	}
	for(i=2; i<=argc/2; i++) {
		fprintf(fp, "%s : %s\n", argv[i], argv[i-1+argc/2]);
		fprintf(fp, "\t$(CPP) -Wall -P -C $(DEFINES) $(INCLUDES) %s > $(addsuffix .c, $(shell ../shorten $(basename $(notdir %s))))\n", argv[i-1+argc/2], argv[i-1+argc/2]); 
		fprintf(fp, "\t$(CC) $(addsuffix .c, $(shell ../shorten $(basename $(notdir %s)))) $(CC_FLAGS) OJ($@)\n\n", argv[i-1+argc/2]);
	}
	fclose(fp);
	return EXIT_SUCCESS;
}
