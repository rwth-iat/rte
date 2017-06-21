/*
*   $Id: shorten.c,v 1.1 1999-07-19 15:01:53 dirk Exp $
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
