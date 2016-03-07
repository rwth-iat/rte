/*
*	KS-Includes
*	-----------
*/
#include "stdio.h"
#include "stdlib.h"
#include "string.h"



/*****************************************************************************/
int main(int argc, char **argv)
/*****************************************************************************/
{
    char help[256];
    char *ph;
    
    help[255] = '\0'; //make sure the string is terminated
    strncpy(help, argv[0], 255);
    ph = help;
    while( ph && (*ph) ) {
        if( (*ph) == '\\') {
            *ph = '/';
        }
        ph++;
    }
fprintf(stderr, "\nPfad: >%s<\n", argv[0]);
fprintf(stderr, "Neue: >%s<\n", help);

    return 0;
} /* main() */
