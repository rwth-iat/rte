
#include <stdio.h>
#include <string.h>
#include <iostream.h>
#include <iomanip.h>
#include <time.h>
#include <sys/timeb.h>


// ACPLT/KS
#include "plt/config.h"
#include "ks/history.h"
#include "ks/clntpath.h"
#include "ks/package.h"
#include "ks/avsimplemodule.h"
#include "ks/objmgrparams.h"

#if PLT_COMPILER_GCC || PLT_COMPILER_DECCXX
#include "plt/config.h"
#include "plt/hashtable_impl.h"
#include "plt/handle_impl.h"
#endif

// Templates
#if PLT_SEE_ALL_TEMPLATES
#include "plt/hashtable.h"
#include "plt/priorityqueue.h"
#include "ks/array.h"
#include "ks/list.h"
#include "ks/handle.h"
#include "plt/sort.h"
#include "ks/conversions.h"
#else
#include "plt/hashtable_impl.h"
#include "plt/priorityqueue_impl.h"
#include "ks/array_impl.h"
#include "ks/list_impl.h"
#include "ks/handle_impl.h"
#include "plt/sort_impl.h"
#endif


// Zeit-Formatierung in Form "JJJJ-MM-DD hh:ss:mm"
char* ___printTime(                                       //  |> Funktionsrueckmeldung
					  const KsTime   &tim                    // >|  KS-Time
					  )
{
	/*
	*	Lokale Variablen
	*/
    static char  help[32];
    time_t secs = tim.tv_sec;
    struct tm* t;

    t=localtime(&secs);
    if(t == NULL) {
        strcpy(help, "1970-01-01 00:00:00.000");
    } else {
        sprintf(help, "%4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d",
                t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min,
                t->tm_sec);
    }

    return help;
}

// Werte ausgeben
void ___printValue(									//  |  (void)
					const KsValueHandle	&value	// >|  Value handle
				  )
{
	/*
	*	Lokale Variablen
	*	----------------
	*/
	unsigned int    i;       /* Laufvariable */
	unsigned int    size;    /* Anzahl der Elementen in Array */
	KsTimeSpan      zeit;    /* Hilfsvariable */

	// Value-Handle vorhanden ?
	if( !value) {
		fprintf(stderr, "Keine Werte vorhanden\n");
		return;
	}

	// Werte schreiben
	switch(value->xdrTypeCode() )
	   {
	    case KS_VT_BYTE_VEC:
			{
				size = ((KsByteVecValue &) *value).size();
				// Es folgt array
	            for ( i = 0; i < size; ++i ) {
	      			fprintf(stderr,"%d\n", ((KsByteVecValue &) *value)[i]);
	            }
	        }
	        break;
	    case KS_VT_BOOL_VEC:
			{
				size = ((KsBoolVecValue &) *value).size();
				// Es folgt array
				for ( i = 0; i < size; ++i ) {								
					if( (bool) ((KsBoolVecValue &) *value)[i])
						fprintf(stderr,"TRUE\n");
					else
	               		fprintf(stderr,"FALSE\n");
				}
			}
	        break;
	    case KS_VT_INT_VEC:
			{
				size = ((KsIntVecValue &) *value).size();
				// Es folgt array
				for ( i = 0; i < size; ++i ) {
					fprintf(stderr,"%ld\n",((KsIntVecValue &) *value)[i]);
				}
			}
	        break;
	    case KS_VT_UINT_VEC:
			{
				size = ((KsUIntVecValue &) *value).size();
				// Es folgt array
				for ( i = 0; i < size; ++i ) {
					fprintf(stderr,"%lu\n",((KsUIntVecValue &) *value)[i]);
				}
			}
	        break;
	    case KS_VT_SINGLE_VEC:
			{
				size = ((KsSingleVecValue &) *value).size();
				// Es folgt array
 				for ( i = 0; i < size; ++i ) {
					fprintf(stderr,"%#f\n",(float)(((KsSingleVecValue &) *value)[i]) );
				}
			}
	        break;
	    case KS_VT_DOUBLE_VEC:
			{
				size = ((KsDoubleVecValue &) *value).size();
				// Es folgt array
 				for ( i = 0; i < size; ++i ) {
					fprintf(stderr,"%#g\n",(double)(((KsDoubleVecValue &) *value)[i]));
				}
			}
	        break;
	    case KS_VT_STRING_VEC:
			{
				size = ((KsStringVecValue &) *value).size();
				// Es folgt array
				for ( i = 0; i < size; ++i ) {
					fprintf(stderr,"%s\n",(const char*)((KsStringVecValue &) *value)[i]);
				}
			}
	        break;
	    case KS_VT_TIME_VEC:
			{
				size = ((KsTimeVecValue &) *value).size();
				// Es folgt array
				for ( i = 0; i < size; ++i ) {
					fprintf(stderr, "%s\n", ___printTime(((KsTimeVecValue &) *value)[i]));
	            }
	        }
	        break;
	    case KS_VT_TIME_SPAN_VEC:
			{
				size = ((KsTimeSpanVecValue &) *value).size();
				// Es folgt array
				for ( i = 0; i < size; ++i ) {
					zeit = ((KsTimeSpanVecValue &) *value)[i];
	 				fprintf(stderr,"%ld.%6.6ld", zeit.tv_sec, zeit.tv_usec );
				}
			}
	        break;
	    default:
    	    fprintf(stderr, "Error variable type unknown\n");
	   } /* switch */

    return;
}

void readHistWidthServer(PltString &HostServer, PltString &HistPath) {
    /* 
        Irgendeinen Kommunikationobjekt mit Basis-Klasse KscClient anlegen
        und Server holen
    */
    PltString       domPath;
    KscDomain*      dom;
    KscServerBase*  serv;
    
    domPath = HostServer;
    /* Poot-Domain */
    domPath += "/";
    
    dom  = new KscDomain(domPath);
    serv = dom->getServer();
    if ( !serv ) {
        /* Ups... Server ist nicht erreichbar */
        fprintf(stderr, "Can't connect to server\n");
        return;
    }
    
    /* Anfrage bauen. Zeit-Track: */
    KsGetHistItem	t_item;
    t_item.part_id = "t";
    
    /* Noch keine auswahl-Parameter */
    KsNoneSel *sel1 = new KsNoneSel;;
    t_item.sel = KsSelectorHandle(sel1, KsOsNew);

    /* Value-Track: */
    KsGetHistItem	v_item;
    v_item.part_id = "value";
    
    /* Noch keine auswahl-Parameter */
    KsNoneSel *sel2 = new KsNoneSel;
    v_item.sel = KsSelectorHandle(sel2, KsOsNew);

    // Dienst-Parameter generieren
    KsGetHistParams		param(1, 2);
    KsGetHistResult		res(1);
    
    param.paths[0] = HistPath;
    param.items[0] = t_item;
    param.items[1] = v_item;

    bool ok = serv->requestByOpcode ( KS_GETHIST, 0, param, res);
    /* Dienst abgesetzt? */
    if ( !ok ) {
        fprintf(stderr, "Can't read history\n");
        return;
    }
	// Fehler beim ausfueren des Dienstes ?
	if ( res.result) {
        fprintf(stderr, "Can't read history\n");
        return;
    }
    // Antwort zurueckgegeben ?
    if(!res.replies.size() ) {
        fprintf(stderr, "Can't read history\n");
        return;
    }
    // Antwort OK ?
    if(res.replies[0].result ) {
        fprintf(stderr, "Can't read history\n");
        return;
    }
    // 2 Variablen abgefragt, auch zwei zurueckgegeben?
    if(res.replies[0].items.size() != 2) {
        fprintf(stderr, "Request error\n");
        return;
    }
    // Einzelne Tracks OK ? 
    int i;
	for(i = 0; i < 2; i++) {
		if(res.replies[0].items[i].result) {
            fprintf(stderr, "Track %d error\n", i);
            return;
		}
    }
    /* Daten ausgeben */
    for(i = 0; i < 2; i++) {
        fprintf(stderr, "Track %d:\n", i);
        ___printValue(res.replies[0].items[i].value	// >|  Value handle
					  );
    }
}

int main(int argc, char **argv) {

    PltString HaS("");
    PltString histDom("");
    int i;
    
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
    		    histDom = argv[i];
    	    } else {
    		    goto HELP;
    	    } 
    	}
    	else if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
HELP: 
            fprintf(stderr, "Usage: pkg_test.exe [arguments]\n"
            "\n"
            "-s            //HOST/SERVER  Set host and server name\n"
            "-d            PATH           Set path of history domain \n"
            "-h OR --help                 Display this help message and exit\n");
    	    return 1;
    	}
    }
    
    if(HaS == "") {
        fprintf(stderr, "\n Bad host and server name.\n");
        goto HELP;
    }
    if(histDom == "") {
        fprintf(stderr, "\n Bad path of history domain.\n");
        goto HELP;
    }

    readHistWidthServer(HaS, histDom);
    
    return 0;
}
