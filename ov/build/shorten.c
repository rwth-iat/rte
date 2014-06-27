/*
*   $Id: shorten.c,v 1.1 1999-07-19 15:01:53 dirk Exp $
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


#include <string.h>
#include <stdio.h>

int main(int argc, char** argv) {
	int i;
	char result[9];
	char *pc1, *pc2;
	for(i=1; i<argc; i++) {
		pc1 = argv[i];
		pc2 = strstr(pc1, "ov_ksserver_");
		if(pc2) {
			pc1 = pc2+12;
			strcpy(result, "ks");
			strncat(result, pc1, 6);
		}
		else {
			pc2 = strstr(pc1, "ov_");
			if(pc2) {
				pc1 = pc2+3;
				strcpy(result, "");
				strncat(result, pc1, 8);			
			}
			else {
				pc2 = strstr(pc1, "fb_"); 
				if(pc2) {
					pc1 = pc2+3;
					strcpy(result, "");
					strncat(result, pc1, 8);
				} 
				else{
					pc2 = strstr(pc1, "mp_"); 
					if(pc2) {
						pc1 = pc2+3;
						strcpy(result, "mp");
						strncat(result, pc1, 6);
					}					
					else {
						strcpy(result, "mc");
						strncat(result, pc1, 6);
					}			  
				}
			}
		}

		printf("%s ", result);
	}
	return 0;
}
