
#include "ifbslibdef.h"

int main(int argc, char **argv) {
        /*
        *        local variables
        */
        char*                filename = NULL;
        char*                servername = NULL;
    char        help[256];
    char*       ph;

        PltString        fn;
        PltString        hs;
        PltString        logfile;
    int         i;
        /*
        *        parse command line arguments
        */
        for(i=1; i<argc; i++) {
                /*
                *        set database filename option
                */
                if(!strcmp(argv[i], "-f") ) {
                        i++;
                        if(i<argc) {
                                filename = argv[i];
                        } else {
                                goto HELP;
                        }
                }
                /*
                *        set server option
                */
                else if(!strcmp(argv[i], "-s")) {
                        i++;
                        if(i<argc) {
                servername = argv[i];
                        } else {
                                goto HELP;
                        }
                }
                /*
                *        display help option
                */
                else if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
HELP:                fprintf(stderr, "Usage: dbload [arguments]\n"
                                "\n"
                                "The following optional arguments are available:\n"
                                "-s            HOST/SERVER  Set host and server name \n"
                                "-f            NAME         Set filename (NAME.fbd)\n"
                                "-h OR --help               Display this help message\n");
                        return EXIT_FAILURE;
                } else {
                        goto HELP;
                }
        }

/* Trottel-Sicherung */
 if(!filename) {
    fprintf(stderr, "\nBad filename\n");
    goto HELP;
 }
 if(!servername) {
    fprintf(stderr, "\nBad host and server name\n");
    goto HELP;
 }

 fn = filename;
 strcpy(help, filename);
 ph = help;

 while ( (*ph) ) ph ++;
 while(  (ph  != help) &&
       ((*ph) != '.')  &&
       ((*ph) != ']')  &&
       ((*ph) != ':')   ) ph --;

 if( (*ph) == '.' ) {
    *ph = '\0';
 }

 logfile = help;
 logfile += ".log";

 hs = servername;
 KS_RESULT err;

 KscServerBase* Server = GetServerByName(hs, err);
 if(err) {
    fprintf(stderr,"\n Can't open %s.\n     Error 0x%x (%s)\n\n\n",
                                 servername, err, GetErrorCode(err));
    return 1;
 }

 err = IFBS_DBLOAD(Server, fn, logfile);
 if(err) {
    fprintf(stderr,"\n Fehler beim Laden der Datenbasis.\n    Nr. 0x%x (%s)\n\n\n",
                         err, GetErrorCode(err));
 } else{
    fprintf(stderr,"\n Datenbasis geladen.\n\n\n");
 }

 return 0;
} /* main() */