#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char cmdline[10240];
char buffer[256];

int main(int argc, char **argv)
{
    if (   argc < 4
	|| (   0!=strcmp(argv[1],"r")
	    && 0!=strcmp(argv[1],"rv"))) {
	fprintf(stderr, "Usage: %s r libfile objfile...\n", argv[0]);
	exit(1);
    }
    sprintf(cmdline, "wlib -q %s", argv[2]);
    for ( int i = 3; i<argc; ++i) {
	sprintf(buffer, " +-%s", argv[i]);
	strcat(cmdline, buffer);
    }
    return system(cmdline);
}
