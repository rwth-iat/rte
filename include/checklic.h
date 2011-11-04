/*
==========================
checklic.h
Ueberpruefung von Lizenzen

199-08-27
H.-J. Mehnert
==========================
*/

#ifndef _LIB_CHECKLIC_  
#define _LIB_CHECKLIC_

int   liccheckerror=0;

#include "licence.h"

int liccheck   (char *filename);
char *licgetinfo (char *filename);

int liccheck(char *filename) {
  licence_t thislicence;
  char backupkey[255];
#ifdef CODE_DEBUG
  fprintf(stderr,"Lizenz-Prüfung...\n");  
#endif
  licfromfilesingledat(&thislicence,filename,"","");
  if (liccheckerror!=0) {
      fprintf(stderr,"Lizenz: Fehler beim Auslesen der Datei %s\n",filename);
      return (-1);
  }

  licgettypefromkey(&thislicence);
  if (liccheckerror!=0) {
      fprintf(stderr,"Lizenz: Schlüssel beschädigt\n");
      return (-1);
  }

  sprintf(backupkey,"%s",thislicence.Schluessel);

  licmakekey(&thislicence);
  if (liccheckerror!=0) {
      fprintf(stderr,"Lizenz: Fehlerhafte Lizenzdaten\n");
      return (-1);
  }
  
  if (strcmp(thislicence.Schluessel,backupkey)==0) return(thislicence.Typ);
  else {
        fprintf(stderr,"Lizenz: Schlüssel falsch\n");
        return(-1);
  }
}

char *licgetinfo(char *filename) {
  licence_t thislicence;
  int result;
  static char licencestr[2500];

  /* Init */
  liccheckerror=0;
  
  result=liccheck(filename);  
//  licencestr=(char*)malloc(2550);
  
  if (result!=(-1)) {
    licfromfilesingledat(&thislicence,filename,"","");
    licgettypefromkey(&thislicence);
    //result=licprintuserinfo(&thislicence);
    mysprintf(licencestr,        "\0");
    //mysprintf(licencestr,  "%s%s%s\0",licencestr,"Validiert  : ",thislicence.validiert );
    //mysprintf(licencestr,"%s\n%s%s\0",licencestr,"Firma      : ",thislicence.Firma     );
    mysprintf(licencestr,  "%s%s%s\0",licencestr,"Firma      : ",thislicence.Firma     );
    mysprintf(licencestr,"%s\n%s%s\0",licencestr,"Abteilung  : ",thislicence.Abteilung );
    mysprintf(licencestr,"%s\n%s%s\0",licencestr,"Name       : ",thislicence.Name      );
    mysprintf(licencestr,"%s\n%s%s\0",licencestr,"Strasse    : ",thislicence.Strasse   );
    mysprintf(licencestr,"%s\n%s%s\0",licencestr,"PLZ        : ",thislicence.PLZ       );
    mysprintf(licencestr,"%s\n%s%s\0",licencestr,"Ort        : ",thislicence.Ort       );
    mysprintf(licencestr,"%s\n%s%s\0",licencestr,"Tel        : ",thislicence.Tel       );
    mysprintf(licencestr,"%s\n%s%s\0",licencestr,"Fax        : ",thislicence.Fax       );
    mysprintf(licencestr,"%s\n%s%s\0",licencestr,"eMail      : ",thislicence.eMail     );

	switch(thislicence.Typ) {
		case 1 :
				mysprintf(licencestr,"%s\n%s\0",licencestr,"Typ        : Vollversion");
				break;
		case 2 :
				mysprintf(licencestr,"%s\n%s\0",licencestr,"Typ        : Hochschulversion");
				break;
		case 3 :
				mysprintf(licencestr,"%s\n%s\0",licencestr,"Typ        : Testversion");
				break;
		default:
				mysprintf(licencestr,"%s\n%s\0",licencestr,"Typ        : Demoversion");
	}

    //mysprintf(licencestr,"%s\n%s%i\0",licencestr,"Typ        : ",thislicence.Typ       ); // !!
    //mysprintf(licencestr,"%s\n%s%s\0",licencestr,"Schluessel : ",thislicence.Schluessel);
  } else {
    mysprintf(licencestr,"Keine gültige Lizenz gefunden\n");
  }

  return(licencestr);
}

#endif

//char *licinfo
//licinfo=licgetinfo(licence.dat)
//free(licinfo)
