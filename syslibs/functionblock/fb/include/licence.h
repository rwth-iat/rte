/*
==========================================
licence.h
Ein-, Ausgabe und Bearbeitung von Lizenzen

1999-08-30
H.-J. Mehnert
==========================================
*/

#ifndef _LIB_CHECKLIC_
#define EXITMSG(msg) {printf("EXITMSG: "msg); exit(0);}
#else
#define EXITMSG(msg) {liccheckerror=1;}
/*
#define EXITMSG(msg) {printf("EXITMSG: "msg); liccheckerror=1;}
*/
#endif

/* #ifdef __BORLANDC__ */
#include <string.h>
/* #endif */

#include "fileops.h"
#include "strops.h"
#include "coder.h"
#include <ctype.h>

#ifndef _LIB_LICENCE_  
#define _LIB_LICENCE_

typedef struct {
  char         Firma       [255];// !
  char         Abteilung   [255];
  char         Name        [255];// !
  char         Strasse     [255];// !
  char         PLZ         [255];// !
  char         Ort         [255];// !
  char         Tel         [255];
  char         Fax         [255];
  char         eMail       [255];
  unsigned int Typ;              // !
  char         erstellt    [255];
  char         validiert   [255];
  char         Schluessel  [255];// !
} licence_t;


void licprintuserinfo (licence_t *licence) {
  printf("Firma               : %s\n",(*licence).Firma);
  printf("Abteilung    (opt.) : %s\n",(*licence).Abteilung);
  printf("Name                : %s\n",(*licence).Name);
  printf("Strasse             : %s\n",(*licence).Strasse);
  printf("PLZ                 : %s\n",(*licence).PLZ);
  printf("Ort                 : %s\n",(*licence).Ort);
  printf("Tel.         (opt.) : %s\n",(*licence).Tel);
  printf("Fax          (opt.) : %s\n",(*licence).Fax);
  printf("eMail        (opt.) : %s\n",(*licence).eMail);

  return;
}

void licgetuserinfo (licence_t *licence) {
  

  smscans("Firma               : ",(*licence).Firma);
  smscans("Abteilung    (opt.) : ",(*licence).Abteilung);
  smscans("Name                : ",(*licence).Name);
  smscans("Strasse             : ",(*licence).Strasse);
  smscans("PLZ                 : ",(*licence).PLZ);
  smscans("Ort                 : ",(*licence).Ort);
  smscans("Tel.         (opt.) : ",(*licence).Tel);
  smscans("Fax          (opt.) : ",(*licence).Fax);
  smscans("eMail        (opt.) : ",(*licence).eMail);

  return;
}


void licgetcreated (licence_t*licence) {
  
  strgetisotime((*licence).erstellt);
  return;
}

void licprintcreated (licence_t *licence) {
  
  printf( "erstellt            : %s\n",(*licence).erstellt);
  return;
}

void licgetvalidated (licence_t*licence) {
  
  strgetisotime((*licence).validiert);
  return;
}


void licprintvalidated (licence_t *licence) {
  
  printf( "validiert           : %s\n",(*licence).validiert);
  return;
}

void licgetkey (licence_t *licence) {
  int counter,slen = strlen((*licence).Schluessel);
  smscans("Schluessel:         : ",(*licence).Schluessel);
  //  printf("licgetkey(): key=[%s]\n",(*licence).Schluessel);
  for (counter=0;counter<slen;counter++) {
    (*licence).Schluessel[counter]=(char) toupper((char) (*licence).Schluessel[counter]);    
  }
  //  printf("licgetkey(): key=[%s]\n",(*licence).Schluessel);
  return;
}

void licgettypefromkey (licence_t *licence) {
  
  char strbuf[255];
  sprintf(strbuf,"%s",(*licence).Schluessel);
  (*licence).Typ=getmagicnumfrom5x4key(strbuf);
  return;
}

void licgettype (licence_t *licence) {
  
  printf( "Lizenztyp (0-65535) : ");
  scanf("%i",&(*licence).Typ);
  getchar();
  return;
}


void licprinttype (licence_t *licence) {
  
  printf( "Lizenztyp (0-65535) : %i\n",(*licence).Typ);
  return;
}

void setInitPwdString(char *pwd) {
  pwd[0]= (char)82;pwd[1]= (char)16;pwd[2]=(char)217;pwd[3]=(char)192;
  pwd[4]= (char)2; pwd[5]= (char)61;pwd[6]= (char)49;pwd[7]=(char)247;
  pwd[8]='\0';
}

void licmakekey (licence_t *licence) {
  char pwd[9];
  char strforkey[1200];
  char firmatmp[255];
  char nametmp[255];
  char strassetmp[255];
  char plztmp[255];
  char orttmp[255];
  
  //irgendein Passwort erstellen
  setInitPwdString(pwd);

  strcpy(firmatmp,(*licence).Firma);
  strcpy(nametmp,(*licence).Name);
  strcpy(strassetmp,(*licence).Strasse);
  strcpy(plztmp,(*licence).PLZ);
  strcpy(orttmp,(*licence).Ort);

  strfilter(firmatmp,' ');
  strfilter(nametmp,' ');
  strfilter(strassetmp,' ');
  strfilter(plztmp,' ');
  strfilter(orttmp,' ');

  sprintf(strforkey,"%s%s%s%s%s",firmatmp,nametmp,strassetmp,plztmp,orttmp);
  generate5x4Key((*licence).Schluessel,strforkey,pwd,(*licence).Typ);
  return;
}

void licprintkey (licence_t *licence) {
  printf("Key                 : %s\n",(*licence).Schluessel);
  return;
}

void lictofilesingletxt (licence_t *licence,char *directory,char *filenm,char *fileext) {
  
  
  char strforfile[2550];
  char filename[255];

  mysprintf(filename,"%s%s.%s\0",directory,filenm,fileext);
#ifdef CODE_DEBUG
  printf("lictofilesingletxt(): filename=[%s]\n",filename);
#endif
  mysprintf(strforfile,"\0");
  mysprintf(strforfile,"%serstellt   : %s\n\0",strforfile,(*licence).erstellt);
  mysprintf(strforfile,"%sFirma      : %s\n\0",strforfile,(*licence).Firma);
  mysprintf(strforfile,"%sAbteilung  : %s\n\0",strforfile,(*licence).Abteilung);
  mysprintf(strforfile,"%sName       : %s\n\0",strforfile,(*licence).Name);
  mysprintf(strforfile,"%sStrasse    : %s\n\0",strforfile,(*licence).Strasse);
  mysprintf(strforfile,"%sPLZ        : %s\n\0",strforfile,(*licence).PLZ);
  mysprintf(strforfile,"%sOrt        : %s\n\0",strforfile,(*licence).Ort);
  mysprintf(strforfile,"%sTel        : %s\n\0",strforfile,(*licence).Tel);
  mysprintf(strforfile,"%sFax        : %s\n\0",strforfile,(*licence).Fax);
  mysprintf(strforfile,"%seMail      : %s\n\0",strforfile,(*licence).eMail);
  mysprintf(strforfile,"%sSchluessel : %s\n\0",strforfile,(*licence).Schluessel);
  mysprintf(strforfile,"%s\n\0",strforfile);
  
  fieldtofile(strforfile,filename,strlen(strforfile),NOAPPEND);
  
  return;
}


void lictofilesingledat (licence_t *licence,char *directory,char *filenm,char *fileext) {
  char pwd[9];
  
  char strforfile[2550];
  char filename[255];
  unsigned char buffer[4096];
  int bytes;

  mysprintf(filename,"%s%s.%s\0",directory,filenm,fileext);
#ifdef CODE_DEBUG
  printf("lictofilesingledat(): filename=[%s]\n",filename);
#endif   
  mysprintf(strforfile,  "%s|\0",           (*licence).Firma     );
  mysprintf(strforfile,"%s%s|\0",strforfile,(*licence).Abteilung );
  mysprintf(strforfile,"%s%s|\0",strforfile,(*licence).Name      );
  mysprintf(strforfile,"%s%s|\0",strforfile,(*licence).Strasse   );
  mysprintf(strforfile,"%s%s|\0",strforfile,(*licence).PLZ       );
  mysprintf(strforfile,"%s%s|\0",strforfile,(*licence).Ort       );
  mysprintf(strforfile,"%s%s|\0",strforfile,(*licence).Tel       );
  mysprintf(strforfile,"%s%s|\0",strforfile,(*licence).Fax       );
  mysprintf(strforfile,"%s%s|\0",strforfile,(*licence).eMail     );
  mysprintf(strforfile,"%s%s|\0",strforfile,(*licence).Schluessel);
  mysprintf(strforfile,"%s%s|\0",strforfile,(*licence).validiert );
#ifdef CODE_DEBUG
  printf("lictofilesingledat(): strforfile=[%s]\n",strforfile);
#endif
  //irgendein Passwort erstellen
  setInitPwdString(pwd);
  
  bytes=coder_encode(strforfile,buffer,pwd);
  fieldtofile((char*)buffer,filename,bytes,NOAPPEND);

  return;
}


void lictofilemultitxt (licence_t *licence,char *directory,char *filenm,char *fileext) {
  
  
  char strforfile[2550];
  char filename[255];

  mysprintf(filename,"%s%s.%s\0",directory,filenm,fileext);
#ifdef CODE_DEBUG
  printf("lictofilemultitxt(): filename=[%s]\n",filename);
#endif  
  mysprintf(strforfile,"\0");
  mysprintf(strforfile,"%serstellt   : %s\n\0",strforfile,(*licence).erstellt);
  mysprintf(strforfile,"%sFirma      : %s\n\0",strforfile,(*licence).Firma);
  mysprintf(strforfile,"%sAbteilung  : %s\n\0",strforfile,(*licence).Abteilung);
  mysprintf(strforfile,"%sName       : %s\n\0",strforfile,(*licence).Name);
  mysprintf(strforfile,"%sStrasse    : %s\n\0",strforfile,(*licence).Strasse);
  mysprintf(strforfile,"%sPLZ        : %s\n\0",strforfile,(*licence).PLZ);
  mysprintf(strforfile,"%sOrt        : %s\n\0",strforfile,(*licence).Ort);
  mysprintf(strforfile,"%sTel        : %s\n\0",strforfile,(*licence).Tel);
  mysprintf(strforfile,"%sFax        : %s\n\0",strforfile,(*licence).Fax);
  mysprintf(strforfile,"%seMail      : %s\n\0",strforfile,(*licence).eMail);
  mysprintf(strforfile,"%sLizenztyp  : %i\n\0",strforfile,(*licence).Typ);
  mysprintf(strforfile,"%sSchluessel : %s\n\0",strforfile,(*licence).Schluessel);
  mysprintf(strforfile,"%s- - - - - - - - - - - - - - - - - - -\n\0",strforfile);
  
  fieldtofile(strforfile,filename,strlen(strforfile),APPEND);
  return;
}


void licfromfilesingledat (licence_t *licence,char *directory,char *filenm,char *fileext) {
  char pwd[9],zeichen;
  int counter,strpos;
  char strfromfile[2550];
  char filename[255],splitstr[255];
  char buffer[4096];
  int bytes;

  sprintf(filename,"%s%s.%s",directory,filenm,fileext);
  if (((strcmp(filenm,"")==0)&&(strcmp(fileext,"")==0))||(strcmp(fileext,"")==0)) {
    filename[strlen(filename)-1]='\0';
  }
#ifdef CODE_DEBUG
  printf("licfromfilesingledat(): filename=[%s]\n",filename);
#endif

  // Passwort erstellen
  setInitPwdString(pwd);
  
    
  bytes=fieldfromfile(buffer,filename);

#ifdef _LIB_CHECKLIC_
  if (liccheckerror!=0) return;
#endif

  coder_decode((unsigned char*)buffer,strfromfile,bytes,pwd);
#ifdef CODE_DEBUG
  printf("licfromfilesingledat(): strfromfile=[%s]\n",strfromfile);
#endif
  strpos=0;
  for (counter=1;counter<=11;counter++) {
    zeichen=' ';
    strcpy(splitstr,"");
    while (zeichen!='|') {
      zeichen=strfromfile[strpos];
      strpos++;
      if (strpos>=4096) EXITMSG("licfromfilesingledat(): error in sequence: no | found or file bigger than 4096 bytes\n");

#ifdef _LIB_CHECKLIC_
  if (liccheckerror!=0) return;
#endif

      if (zeichen!='|') mysprintf(splitstr,"%s%c\0",splitstr,zeichen);
    }
    switch(counter) {
      case  1: sprintf((*licence).Firma     ,"%s",splitstr); break;
      case  2: sprintf((*licence).Abteilung ,"%s",splitstr); break;
      case  3: sprintf((*licence).Name      ,"%s",splitstr); break;
      case  4: sprintf((*licence).Strasse   ,"%s",splitstr); break;
      case  5: sprintf((*licence).PLZ       ,"%s",splitstr); break;
      case  6: sprintf((*licence).Ort       ,"%s",splitstr); break;
      case  7: sprintf((*licence).Tel       ,"%s",splitstr); break;
      case  8: sprintf((*licence).Fax       ,"%s",splitstr); break;
      case  9: sprintf((*licence).eMail     ,"%s",splitstr); break;
      case 10: sprintf((*licence).Schluessel,"%s",splitstr); break;
      case 11: sprintf((*licence).validiert ,"%s",splitstr); break;
      default: EXITMSG("licfromfilesingledat(): error while splitting strfromfile\n");
    }

#ifdef _LIB_CHECKLIC_
  if (liccheckerror!=0) return;
#endif

  }

  return;
}



void licfromfilesingletxt (licence_t *licence,char *directory,char *filenm,char *fileext) {
  char zeichen;
  int counter,strpos;
  char strfromfile[4096];
  char filename[255],splitstr[255];
//  unsigned char buffer[4096];
  int bytes;
  
  sprintf(filename,"%s%s.%s",directory,filenm,fileext);
  if (((strcmp(filenm,"")==0)&&(strcmp(fileext,"")==0))||(strcmp(fileext,"")==0)) {
    filename[strlen(filename)-1]='\0';
  }
#ifdef CODE_DEBUG
  printf("licfromfilesingletxt(): filename=[%s]\n",filename);
#endif

  bytes=fieldfromfile(strfromfile,filename);
  strfromfile[bytes]='\0';
  strpos=0;
  for (counter=1;counter<=11;counter++) {
    zeichen=' ';
    mysprintf(splitstr,"\0");

    while (zeichen!='\n') {
      zeichen=strfromfile[strpos];
#ifdef CODE_DEBUG
  printf("%c",zeichen);
#endif
      strpos++;
      if (strpos>=4096) EXITMSG("licfromfilesingletxt(): error in sequence: no NEWLINE found of file bigger than 4096 bytes\n");
      if (zeichen!='\n') mysprintf(splitstr,"%s%c\0",splitstr,zeichen);
    }
    strcircle(splitstr,strlen(splitstr)-13);
    splitstr[strlen(splitstr)-13]='\0';
    switch(counter) {
      case  1: break;
      case  2: sprintf((*licence).Firma     ,"%s",splitstr); break;
      case  3: sprintf((*licence).Abteilung ,"%s",splitstr); break;
      case  4: sprintf((*licence).Name      ,"%s",splitstr); break;
      case  5: sprintf((*licence).Strasse   ,"%s",splitstr); break;
      case  6: sprintf((*licence).PLZ       ,"%s",splitstr); break;
      case  7: sprintf((*licence).Ort       ,"%s",splitstr); break;
      case  8: sprintf((*licence).Tel       ,"%s",splitstr); break;
      case  9: sprintf((*licence).Fax       ,"%s",splitstr); break;
      case 10: sprintf((*licence).eMail     ,"%s",splitstr); break;
      case 11: sprintf((*licence).Schluessel,"%s",splitstr); break;
      default: EXITMSG("licfromfilesingletxt(): error while splitting strfromfile\n"); break;
    }
  }

  return;
}

//Ende des Präprozessor-If-Rumpfes
#endif










