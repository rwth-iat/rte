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
    
    strcpy(help, argv[0]);
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
