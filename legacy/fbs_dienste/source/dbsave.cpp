
#include "ifbslibdef.h"

int main(int argc, char **argv) {
        /*
        *        local variables
        */
        PltString        filename("");
        char*                servername = "localhost/fb_database";
        char*                ph;
    char        help[256];
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
HELP:                fprintf(stderr, "\nUsage: dbsave [arguments]\n"
                                "\n"
                                "The following optional arguments are available:\n"
                                "-s             HOST/SERVER   Set host and server name (default 'localhost/fb_database')\n"
                                "-f             NAME          Set filename NAME.fbd (default HOST_SERVER.fbd)\n"
                                "-h OR --help                 Display this help message\n");
                        return 1;
                } else {
                        goto HELP;
                }
        }

if(filename == "") {
    strcpy(help, servername);
    ph = help;
    while( (*ph) ) {
        if( ((*ph) == '.') || ((*ph) == '/') ) {
            *ph = '_';
        }
        ph++;
    }
    filename = help;
    filename += ".fbd";
}

 PltString  hs(servername);
 KS_RESULT err;

 KscServerBase* Server = GetServerByName(hs, err);
 if(err) {
    fprintf(stderr,"\n Can't open %s.\n     Error 0x%x (%s)\n\n\n",
                                 servername, err, GetErrorCode(err));
    return 1;
 }

 err = IFBS_DBSAVE(Server, filename);
 if(err) {
    fprintf(stderr,"\n Fehler beim Sichern der Datenbasis.\n    Nr. 0x%x (%s)\n    Datei '%s'\n\n\n",
                         err, GetErrorCode(err), (const char*)filename);
 } else{
    fprintf(stderr,"\n Datenbasis gesichert. Dateiname : '%s'\n\n\n", (const char*)filename);
 }

 return 0;
} /* main() */