/******************************************************************************
***                                                                         ***
***   iFBSpro   -   Funktionsbaustein-Model                                 ***
***   #####################################                                 ***
***                                                                         ***
***   L T S o f t                                                           ***
***   Agentur für Leittechnik Software GmbH                                 ***
***   Brabanterstr. 13                                                      ***
***   D-50171 Kerpen                                                        ***
***   Tel : 02237/92869-2                                                   ***
***   Fax : 02237/92869-9                                                   ***
***   e-Mail   : ltsoft@ltsoft.de                                           ***
***   Internet : http://www.ltsoft.de                                       ***
***                                                                         ***
*******************************************************************************
*                                                                             *
*   Datei                                                                     *
*   -----                                                                     *
*   lts_cfnc.c                                                                *
*                                                                             *
*   Historie                                                                  *
*   --------                                                                  *
*   2007-01-25 Erstellung, LTSoft GmbH, Kerpen            A.Neugebauer        *
*                                                                             *
*   Beschreibung                                                              *
*   ------------                                                              *
*   Hilfsfunktionen fuer FileUpload                                           *
*                                                                             *
*                                                                             *
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Deklarationen */
char* fileUpload_getFullPath(const char * fileName);

/* Passwort fuer 'replace' aus der RC-Datei lesen   */
/* FileName = $IFBS_HOME/server/user/libs/upload.rc */
#define UPLOAD_RC_FILE "libs/fileupload.rc"

/* DEBUG-Ausgaben? */
#define NGB_DEBUG 0
/*	----------------------------------------------------------------------	*/

int ___string_compare(
	const char*		string1,
	const char*		string2
) {
	if(string1) {
		if(string2) {
			return strcmp(string1, string2);
		}
		if(!*string1) {
			return 0;		/* "" == NULL */
		}
		return 1;			/* "xxx" > NULL */
	}
	if(string2) {
		if(!*string2) {
			return 0;		/* NULL == "" */
		}
		return -1;			/* NULL < "xxx" */
	}
	return 0;				/* NULL == NULL */
}

/*	----------------------------------------------------------------------	*/

void lts_encodeblock( unsigned char in[3], unsigned char out[4], int len )
{
    static const char cb64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    
    out[0] = cb64[ in[0] >> 2 ];
    out[1] = cb64[ ((in[0] & 0x03) << 4) + (in[1] >> 4) ];
    out[2] = (unsigned char) (len > 1 ? cb64[ ((in[1] & 0x0f) << 2) + (in[2] >> 6) ] : '=');
    out[3] = (unsigned char) (len > 2 ? cb64[ in[2] & 0x3f ] : '=');
}

/*	----------------------------------------------------------------------	*/
/* Rueckgabe - Zeiger auf letzten encodierten Zeichen (Bei OK - '\0') */

char* lts_encode_string(
    char *pOut
    ,char *str
) {
    unsigned char in[3], out[4];
    int i, len;
    char *ph;

    if(str && (*str)) {
        ph = str;
        while( ph && (*ph)) {
            len = 0;
            for( i = 0; i < 3; i++ ) {
                in[i] = (unsigned char)(*ph);
                if( ph && (*ph) ) {
                    len++;
                    ph++;
                }
                else {
                    in[i] = 0;
                }
            }
            if( len ) {
                lts_encodeblock( in, out, len );
                for( i = 0; i < 4; i++ ) {
                    *pOut = (char)(out[i]);
                    pOut++;                
                }
            }
        }
    }
    /* String abschliessen */
    *pOut = '\0';
        
    return pOut;
}

void lts_encode(
    FILE* infile
    ,char **outbuf
    ,char *cmd
    ,char *fileName
    ,char *pwd
) {
    unsigned char in[3], out[4];
    int i, len;
    char *pOut;
    
    /* Output-Buffer */
    pOut = *outbuf;
    
    /* Merke CMD */
    *pOut = *cmd;
    pOut++;

    /* Platzhalter */
    *pOut = ' ';
    pOut++;
    
    /* Passwort */
    *pOut = '_';
    pOut++;

    pOut = lts_encode_string(pOut, pwd);
    
    *pOut = '_';
    pOut++;

    /* Platzhalter */
    *pOut = ' ';
    pOut++;
    
    /* Merke fileName */
    pOut = lts_encode_string(pOut, fileName);
    
    /* Platzhalter */
    *pOut = ' ';
    pOut++;

    /* Datei-Inhalt */
    while( !feof( infile ) ) {
        len = 0;
        for( i = 0; i < 3; i++ ) {
            in[i] = (unsigned char) getc( infile );
            if( !feof( infile ) ) {
                len++;
            }
            else {
                in[i] = 0;
            }
        }
        if( len ) {
            lts_encodeblock( in, out, len );
            for( i = 0; i < 4; i++ ) {
                *pOut = (char)(out[i]);
                pOut++;
            }
        }
    }
    *pOut = '\0';
}

/*	----------------------------------------------------------------------	*/

void lts_decodeblock( unsigned char in[4], unsigned char out[3] )
{   
    out[0] = (unsigned char ) ((in[0] << 2) | (in[1] >> 4));
    out[1] = (unsigned char ) ((in[1] << 4) | (in[2] >> 2));
    out[2] = (unsigned char ) (((in[2] << 6) & 0xc0) | in[3]);
}

/*	----------------------------------------------------------------------	*/

void lts_decodePwd(
    const char*    encPwd
    ,char*         decPwd
) {
    /* Locale Variablen */
    static const char cd64[]="|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";
    unsigned char in[4], out[3], v;
    int i, len;

    char    *pf;            /* Laufzeiger dekodierten String   */
    char    *pes;           /* Laufzeiger encodierten String   */

    /* Passwort decodieren */
    pf = decPwd;
    pes = (char*)encPwd;
    
    *pf = '\0';
    
    while( pes && (*pes)) {
        len=0;
        for(i=0; (i<4) && pes && (*pes); i++) {
            v = 0;
            /* Zeilenumbruch und Steuerzeichen weg */
            while(pes && (*pes) && (v==0)) {
                v = (unsigned char)(*pes);
                pes++;
                v = (unsigned char) ((v < 43 || v > 122) ? 0 : cd64[ v - 43 ]);
                if( v ) {
                    v = (unsigned char) ((v == '$') ? 0 : v - 61);
                }
            }
            if( (pes && (*pes)) || v) {
	           len++;
                if(v) {
                    in[i] = (unsigned char) (v - 1);
                } else {
                    in[i] = 0;
                }
            } else {
                in[i] = 0;
            }
        }
        if( len ) {
            lts_decodeblock( in, out );
            for( i = 0; i < len - 1; i++ ) {
                *pf = out[i];
                pf++;                
            }
        }
    }
    *pf = '\0';
}

/*	----------------------------------------------------------------------	*/
int lts_checkRcPwdOk(
    char *pwd
) {
    /* Wird FNC von iFBS-Lib benutzt? */
#if COMPILE_FB_DLL    
    /* Lokale Variablen */
    FILE*   fil;
    char    cmd[256];
    char    *pf;
    char    *ph;

#if NGB_DEBUG
fprintf(stderr, "decode(checkPwd): >%s<\n", (pwd ? pwd : ""));
#endif

    pf = fileUpload_getFullPath(UPLOAD_RC_FILE);
    if(!pf) {
	    return 0;
    }

    fil = fopen(pf, "r");
    if(!fil) {
#if NGB_DEBUG
fprintf(stderr, "  Can't open file >%s< for read\n", (pf ? pf : ""));
#endif
        return 0;
    }
    
    while(fgets(cmd, 255, fil) != NULL) {
#if NGB_DEBUG
fprintf(stderr, " .... %s", cmd);
#endif
        if(cmd[0] != '#') {
            /* Kein Kommentar : bearbeiten */
            /* Token suchen */
            pf = strstr(cmd, "PASSWORD");
            if(pf) {
#if NGB_DEBUG
fprintf(stderr, "  Zeile gefunden: >%s<\n", pf);
#endif
                pf += 8;
                while(pf && (*pf) && ((*pf == ' ') || (*pf == '\t') || (*pf == '=')) ) pf++;
#if NGB_DEBUG
fprintf(stderr, "  TrimLeft: >%s<\n", (pf ? pf : ""));
#endif
                
                if(!pf) {
                    /* Kein Passwort eingetragen? */
                    fclose(fil);
                    return 1;
                }
                ph = pf;
                /* Trim Right */
                while(ph && (*ph)) ph++;
                ph--;
                while( ((*ph == '\n') || (*ph == '\r') || (*ph == ' ') || (*ph == '\t')) ) {
                    *ph = '\0';
                    if(ph == pf) {
                        break;
                    }
                    ph--;
                }
                
#if NGB_DEBUG
fprintf(stderr, "  TrimRight: >%s<\n", (pf ? pf : ""));
#endif
                /* Kein Passwort eingetragen? */
                if(!pf) {
#if NGB_DEBUG
fprintf(stderr, "  Kein Passwort gesetzt\n");
#endif
                    fclose(fil);
                    return 1;
                }
                if(!___string_compare(pf, "")) {
#if NGB_DEBUG
fprintf(stderr, "  Kein Passwort gesetzt\n");
#endif
                    fclose(fil);
                    return 1;
                }

                                
#if NGB_DEBUG
fprintf(stderr, "  Vergleiche >%s< und >%s<\n", (pwd ? pwd : ""), (pf ? pf : ""));
#endif
                /* Strings vergleichen */
                if(!___string_compare(pwd, pf)) {
                    fclose(fil);
                    return 1;
                }
            }
        }
    }
    
    /* RC-Datei schliessen */
    fclose(fil);
    
    
#else
    if(!___string_compare(pwd, "admin")) {
        return 1;
    }
#endif

    return 0;
}

/*	----------------------------------------------------------------------	*/
char* lts_decode(
    const char*     cmd
    ,const char*    encFileName
    ,const char*    encFileData
    ,char*          decFileName
    ,char*          pwd
) {
    /* Locale Variablen */
    static const char cd64[]="|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";
    unsigned char in[4], out[3], v;
    int i, len;

    int fileExists = 0;     /* Merker: Datei bereits vorhanden */
    char    *pf;            /* Laufzeiger dekodierten FileName */
    char    *pes;           /* Laufzeiger encodierten String   */
    FILE*   outfile;
    char    outFileName[255];
    
    /* Datei-Name decodieren */
    pf = decFileName;
    pes = (char*)encFileName;
    while( pes && (*pes)) {
        len=0;
        for(i=0; (i<4) && pes && (*pes); i++) {
            v = 0;
            /* Zeilenumbruch und Steuerzeichen weg */
            while(pes && (*pes) && (v==0)) {
                v = (unsigned char)(*pes);
                pes++;
                v = (unsigned char) ((v < 43 || v > 122) ? 0 : cd64[ v - 43 ]);
                if( v ) {
                    v = (unsigned char) ((v == '$') ? 0 : v - 61);
                }
            }
            if( (pes && (*pes)) || v) {
	           len++;
                if(v) {
                    in[i] = (unsigned char) (v - 1);
                } else {
                    in[i] = 0;
                }
            } else {
                in[i] = 0;
            }
        }
        if( len ) {
            lts_decodeblock( in, out );
            for( i = 0; i < len - 1; i++ ) {
                *pf = out[i];
                pf++;                
            }
        }
    }
    *pf = '\0';
    
    /* Datei-Name decodiert ? */    
    if(!___string_compare(decFileName, "")) {
	    return "Bad file name.";
    }

    /* Wird FNC von iFBS-Lib benutzt? */
#if COMPILE_FB_DLL    
    pf = fileUpload_getFullPath(decFileName);
    if(!pf) {
	    return "No environment variable.";
    }
#else
    pf = decFileName;
#endif
    /* Datei-Name merken */ 
    strcpy(outFileName, pf);
    
    /* Datei bereits vorhanden ? */
    outfile = fopen(pf, "rb");
    if(outfile) {
        fclose(outfile);
        fileExists = 1;
    }
    
    /* Kopieren oder ersetzen? */
    switch(*cmd) {
        case 'c':
            if(fileExists == 1) {
                return "File already exists.";
            }
            break;
        case 'r':
        case 'R':
            /* Datei vorhanden? */
            if(fileExists == 1) {
                /* RC-Datei vorhanden und Pwd stimmt? */
                if( !lts_checkRcPwdOk(pwd)) {
                    return "Bad password.";
                }
            }
            break;
        default:
            return "Bad value";
    }

    /* Dann ueberschreiben oder neue anlegen */
    outfile = fopen(outFileName, "wb");
    if(!outfile) {        
        return "Can't open file for write.";
    }
    
    /* Daten dekodieren */
    pes = (char*)encFileData;
    while( pes && (*pes)) {
        len=0;
        for(i=0; (i<4) && pes && (*pes); i++) {
            v = 0;
            /* Zeilenumbruch und Steuerzeichen weg */
            while(pes && (*pes) && (v==0)) {
                v = (unsigned char)(*pes);
                pes++;
                v = (unsigned char) ((v < 43 || v > 122) ? 0 : cd64[ v - 43 ]);
                if( v ) {
                    v = (unsigned char) ((v == '$') ? 0 : v - 61);
                }
            }
            if((pes && (*pes)) || v) {
	            len++;
                if(v) {
                    in[i] = (unsigned char) (v - 1);
                } else {
                    in[i] = 0;
                }
            } else {
                in[i] = 0;
            }
        }
        if( len ) {
            lts_decodeblock( in, out );
            for( i = 0; i < len - 1; i++ ) {
                putc( out[i], outfile );
            }
        }
    }
    
    fclose(outfile);
    
    return NULL;
}

