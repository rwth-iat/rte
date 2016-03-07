#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char cmdline[10240];
char buffer[256];

int main(int argc, char **argv)
{
    if (   argc < 3 ) {
	fprintf(stderr,	"Usage: %s <librarian> libfile objfile...\n", argv[0]);
	exit(1);
    } 
    int i = 2;
    strcpy(cmdline,argv[1]);
    strcat(cmdline," ");

    while (i<argc && argv[i][0] == '/') {
	    sprintf(buffer, " %s ", argv[i]);
	    strcat (cmdline, buffer);
	    ++i;
	}
    if (i<argc) { 
	strcat(cmdline, argv[i]);
	++i;
	while (i<argc) {
	    sprintf(buffer, " +-%s", argv[i]);
	    ++i;	
	    strcat(cmdline, buffer);
	}
    }
    printf(cmdline);
    return system(cmdline);
}
