/*
*   $Id: mkrules.c,v 1.1 1999-07-19 15:01:53 dirk Exp $
*
*   Copyright (C) 1998
*   Lehrstuhl fuer Prozessleittechnik,
*   D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
*
*   This file is part of the ACPLT/OV Package 
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
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
