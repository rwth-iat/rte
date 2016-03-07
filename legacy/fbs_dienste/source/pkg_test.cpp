/*
*	KS-Includes
*	-----------
*/
#include "ifbslibdef.h"


/*****************************************************************************/
void searchAndAddingVariables(KscServerBase *Server, PltString &path
              ,KscPackage *pkg,size_t NumVars, size_t retries, FILE* fil) {
/*****************************************************************************/
    KsGetEPParams         param;
    KsGetEPResult         result;      // Ergebnis des Lesens der Variablen-Daten
    size_t                retNr = 0;
    bool                  ok;
    
    KsString    	root = Server->getHostAndName();
    KsString    	Var;

    PltTime   start;
    PltTime   end;
    
    if(pkg->sizeVariables() >= NumVars) {
        return;
    }
    
    if(fil) {
        fprintf(fil, "\n Durchsuche Domain '%s' ", (const char*)path);
    } else {
        fprintf(stderr, "\n Durchsuche Domain '%s' ", (const char*)path);
    }
    
    // Alle Objekte auflisten :
    param.path = path;
    param.type_mask = (KS_OT_VARIABLE | KS_OT_DOMAIN);
    param.name_mask = "*";
    param.scope_flags = KS_EPF_DEFAULT;

    start = PltTime::now();
    while(retNr < retries) {
        retNr++;
        if(fil) {
            fprintf(fil, ".");
        } else {
            fprintf(stderr, ".");
        }

        
        ok = Server->getEP(0, param, result);
        if ( !ok ) {
            continue;
        }
        if ( result.result != KS_ERR_OK ) {
            continue;
        }
        if(result.items.size() ) {
            break;
        }
    }
   
    end = start - PltTime::now();
    end.normalize();
    
    if(fil) {
        fprintf(fil, "  %ld,%6.6ld sec", end.tv_sec, end.tv_usec);
    } else {
        fprintf(stderr, "  %ld,%6.6ld sec", end.tv_sec, end.tv_usec);
    }
    while ( result.items.size() ) {
        KsEngPropsHandle hpp = result.items.removeFirst();
        if(hpp->xdrTypeCode() == KS_OT_DOMAIN) {
            PltString newPath = path;
            if(path != "/") {
                newPath += "/";
            }
            newPath += (const char*)hpp->identifier;
            
            searchAndAddingVariables(Server, newPath, pkg, NumVars, retries, fil);
        } else {
            if( hpp->xdrTypeCode() == KS_OT_VARIABLE) {
                Var = path;
                if(hpp->access_mode & KS_AC_PART) {
                    Var += ".";
                } else {
                    Var += "/";
                }
                Var += hpp->identifier;
                    
                KscVariable *ksvar = new KscVariable(root+Var);
                pkg->add(KscVariableHandle(ksvar, PltOsNew));
            }
        }
        
        if(pkg->sizeVariables() >= NumVars) {
            return;
        }
    }
}

/*****************************************************************************/
int main(int argc, char **argv) {
/*****************************************************************************/

    PltString HaS("");
    KS_RESULT err;
    PltString StartDom("/");
    unsigned long  upd = 1;
    size_t   NumVars = 100;
    size_t   retries = 1;
    size_t   retNr = 1;
    FILE*    fil = 0;
    int      i;
    
    KscPackage	pkg;
    PltTime   start;
    PltTime   end;
    time_t    timer;
    struct tm* t;
    char help[32];
    PltTime   aft;
    
    for(i=1; i<argc; i++) {
    	if( !strcmp(argv[i], "-s") ) {
    	    i++;
    		if(i<argc) {
    		    HaS = argv[i];
    		} else {
    		    goto HELP;
    		}
    	}
    	else if( !strcmp(argv[i], "-d") ) {
    	    i++;
    	    if(i<argc) {
    		    StartDom = argv[i];
    	    } else {
    		    goto HELP;
    	    } 
    	}
    	else if( !strcmp(argv[i], "-u") ) {
    	    i++;
    	    if(i<argc) {
    		    upd = (unsigned long)atoi(argv[i]);
    	    } else {
    		    goto HELP;
    	    } 
    	}
    	else if( !strcmp(argv[i], "-n") ) {
    	    i++;
    	    if(i<argc) {
    		    NumVars = (size_t)atoi(argv[i]);;
    	    } else {
    		    goto HELP;
    	    }
    	}
    	else if( !strcmp(argv[i], "-l") ) {
    	    i++;
    	    if(i<argc) {
    		    fil = fopen(argv[i], "w");
    		    if(!fil) {
    		        fprintf(stderr, "\n Can't open file '%s' for write.\n", argv[i]);
    		        goto HELP;
    		    }
    	    } else {
    		    goto HELP;
    	    }
    	}
    	else if( !strcmp(argv[i], "-r") ) {
    	    i++;
    	    if(i<argc) {
    	        retries = (size_t)atoi(argv[i]);
    	    } else {
    		    goto HELP;
    	    }
    	}
    	else if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
HELP: 
            fprintf(stderr, "Usage: pkg_test.exe [arguments]\n"
            "\n"
            "-s            HOST/SERVER  Set host and server name\n"
            "-d            NAME         Set start domain (default \"/\")\n"
            "-u            SECS         Set update time  (default 1 sec)\n"
            "-n            NUM          Set max number of the updating variables (default 100)\n"
            "-l            LOGFILE      Set logfile name, you may use stderr or stdout\n"
            "-r            NUM          Set number of search retries (if variables not found or other error)\n"
            "-h OR --help               Display this help message and exit\n");
    	    return 1;
    	}
    }
    
    if(HaS == "") {
        fprintf(stderr, "\n Bad host and server name.\n");
        goto HELP;
    }
    
    KscServerBase* Server = GetServerByName(HaS, err);
    if(err != KS_ERR_OK) {
        fprintf(stderr, "\n Can't contact to server.\n");
        return 0;
    }
    if(!Server) {
        fprintf(stderr, "\n Can't contact to server.\n");
        return 0;
    }
    
    if(!retries) {
        fprintf(stderr, "\n Bad number of retries. Default = 1.\n");
        retries = 1;
    }

    if(!upd) {
        fprintf(stderr, "\n Bad update time. Default = 1 sec.\n");
        upd = 1;
    }

    if(fil) {
        fprintf(fil, "\n");
    } else {
        fprintf(stderr, "\n");
    }
    
    searchAndAddingVariables(Server, StartDom, &pkg, NumVars, retries, fil);
    
    // Update
    aft = PltTime(upd,0);

    while( !(pkg.sizeVariables()) ) {
        if(retNr < retries) {
            retNr++;
            if(fil) {
                fprintf(fil, "\n No Variables found. Start search now");
            } else {
                fprintf(stderr, "\n No Variables found. Start search now");
            }
            searchAndAddingVariables(Server, StartDom, &pkg, NumVars, retries, fil);
        } else {
            break;
        }
        aft.sleep();
    }
    
    if(!pkg.sizeVariables() ) {
        if(fil) {
            fprintf(fil, "\n No Variables found.");
        } else {
            fprintf(stderr, "\n No Variables found.");
        }
        
        return 0;
    }
    
    
    upd = pkg.sizeVariables();
    
    if(fil) {
        fprintf(fil, "\n");
    } else {
        fprintf(stderr, "\n");
    }

    while(upd) {
        err = KS_ERR_OK;
        start = PltTime::now();
        if(!pkg.getUpdate() ) {
            err = pkg.getLastResult();
            if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
        }
        
        end = PltTime::now() - start;
        end.normalize();
    
		timer = (time_t)start.tv_sec;
		t = localtime(&timer);
		if(!t) {
		    strcpy(help, "[1970-01-01 00:00:00.000000]");
		} else {
            sprintf(help, "[%4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d.%6.6ld]",
					t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min,
					t->tm_sec, start.tv_usec );
	    }
        if(fil) {
            fprintf(fil, "%s KS_GETVAR %s %ld Variable(s) %ld.,%6.6ld sec\n"
                    , help, ((err != KS_ERR_OK) ? "Bad" : "Ok"), upd, end.tv_sec, end.tv_usec );
        } else {
            fprintf(stderr, "%s KS_GETVAR %s %ld Variable(s) %ld.%6.6ld sec\n"
                    , help, ((err != KS_ERR_OK) ? "Bad" : "Ok"), upd, end.tv_sec, end.tv_usec );
        }

        aft.sleep();
    }

    return 0;
} /* main() */