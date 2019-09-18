/*
=============================
strops.h
Routinen zur Stringbehandlung

199-08-30
H.-J. Mehnert
=============================
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>


#ifdef __BORLANDC__
#include <string.h>
#endif

#ifndef _LIB_STROPS_  
#define _LIB_STROPS_

void mysprintf(char* dest,char *fmt, ...)
{
   va_list argptr;
   char *buffer=(char*)malloc (4096);
/*   int cnt; */
   
   va_start(argptr, fmt);
   vsprintf(buffer, fmt, argptr);
/*   cnt=cnt; */
   va_end(argptr);

   sprintf(dest,"%s",buffer);
   free(buffer);
}


void strgetisotime(char *dest) {
  time_t tsec;
  struct tm *tblock;
  tsec=time(NULL);
  tblock=localtime(&tsec);
  sprintf(dest,"%04i-%02i-%02i  %02i:%02i",
	  (*tblock).tm_year+1900,
	  (*tblock).tm_mon,
	  (*tblock).tm_mday,
	  (*tblock).tm_hour,
	  (*tblock).tm_min);
  return;
}

void strcopy(char *source,char *dest) {
  sprintf(dest,"%s",source);
  return;
}

void strfill(char *string,int anzbytes,char byte) {
  int counter;
  for (counter=0;counter<anzbytes;counter++)
    string[counter]=byte;
  string[anzbytes]='\0';
  return;
}

void smscans(char *text,char *dest) {
  char buffer;
  buffer=0;
  mysprintf(dest,"\0");
  printf("%s",text);
  while ((buffer!='\n')&&(buffer!='\r')) {
    buffer=(unsigned char) getchar();
    if ((buffer!='\n')&&(buffer!='\r')) mysprintf(dest,"%s%c\0",dest,buffer);
  }
  return;
}

void strshrink(char *string,int tokill,int steplen) {
  int charpos,steppos, slen;
  char *buffer=(char *)malloc(strlen(string)+1);
  strcpy(buffer,"");
 
  slen = strlen(string);
  if (( slen -(slen/steplen)*steplen )!=0) EXITMSG("strshrink(): i-string has wrong length\n");
  
  for (charpos=0;charpos<slen;charpos=charpos+steplen) {
    for (steppos=0;steppos<steplen;steppos++) {
      if (steppos!=tokill-1) {
	mysprintf(buffer,"%s%c\0",buffer,string[charpos+steppos]);
      }
    }
  }
  strcopy(buffer,string);
  free(buffer);
  return;
}

void strmirror(char *string) {
  int uppos,downpos, slen;
  char buffer[4096];

  slen = strlen(string);
  for (uppos=0;uppos<slen;uppos++) {
    downpos=slen - 1 - uppos;
    if (string[downpos]=='1')
      buffer[uppos]='1';
    else
      buffer[uppos]='0';
  }
  buffer[strlen(string)]='\0';

#ifdef CODE_DEBUG
  printf("strmirror(): buffer=[%s]\n",buffer);
#endif

  strcopy(buffer,string);

  return;
}

void strcircle(char *string,int moves) {
  int counter,rotatepos;
  int strlength=strlen(string);
  char buffer[4096];
  int realmoves=moves-(moves/strlength)*strlength;

  if (realmoves>0) {
    realmoves=strlength-realmoves;
    for (counter=0;counter<strlength;counter++) {
      rotatepos=(counter+realmoves)-((counter+realmoves)/strlength)*strlength;
      buffer[counter]=string[rotatepos];
    }
    buffer[strlength]='\0';
    if (strlen(buffer)!=strlen(string)) EXITMSG("strcircle(): i changed strlen, sorry\n")
    sprintf(string,"%s",buffer);
  }
  return;
}

void strfilter(char *string,char tofilter) {
  int counter, slen;
  char *buffer=(char*)malloc(strlen(string)+1);
#ifdef CODE_DEBUG
  printf("strfilter(): string=[%s]\n",string);
#endif

  slen = strlen(string);
  mysprintf(buffer,"\0");
  for (counter=0;counter<slen;counter++) {
    if (string[counter]!=tofilter) mysprintf(buffer,"%s%c\0",buffer,string[counter]);
  }
  strcopy(buffer,string);
  free(buffer);
  return;
}

void strsplit(char *string,int anzparts,int partlen,char splitter) {
  int partloop,charloop,charpos, slen;
  char buffer[128];
  char temp[128];
#ifdef CODE_DEBUG
  printf("strsplit(): string=[%s]\n",string);
#endif
  slen = strlen(string);
  strcpy(buffer,"");
  if (slen !=(anzparts*partlen))
     EXITMSG("strsplit(): i-string has wrong length\n");
     
  charpos=0;
  for (partloop=1;partloop<=anzparts;partloop++) {
    for (charloop=1;charloop<=partlen;charloop++) {
      sprintf(temp,"%s%c",buffer,string[charpos]);
      sprintf(buffer,"%s",temp);
      charpos++;
    }
    if (charpos<(partlen*anzparts)) {
      sprintf(temp,"%s%c",buffer,splitter);
      sprintf(buffer,"%s",temp);
    }    
  }
  strcopy(buffer,string);
  return;
}

//Ende des Präprozessor-If-Rumpfes
#endif











