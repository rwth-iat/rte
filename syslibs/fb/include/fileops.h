/*
==============================================
fileops.h
Transfer von Daten zwischen Arrays und Dateien
in bzw. aus Dateien

1999-08-30
H.-J. Mehnert
==============================================
*/


#include <stdio.h>
#include <stdlib.h>


#ifndef _LIB_FILEOPS_
#define _LIB_FILEOPS_


const int APPEND=1;
const int NOAPPEND=0;


int getfilesize(char *filename) {
  FILE *thisfile; /* = stdin; */
  int anzbytes=0;
/*  int buffer; */

  thisfile=fopen(filename,"rb");
  if (thisfile==NULL) EXITMSG("getfilesize(): could not assign file\n");

  while (( fgetc(thisfile) ) != (-1)) anzbytes++;
  fclose(thisfile);
/*  buffer=buffer; //for ANSI */
  
  return anzbytes;
}


void fieldtofile (char* field,char* filename,int bytes,int writemode) {
  FILE *outfile = stdout;
#ifdef CODE_DEBUG
  int counter;
#endif
  
#ifdef CODE_DEBUG
  printf("fieldtofile(): filename=[%s]\n",filename);
#endif

#ifdef CODE_DEBUG
  printf("fieldtofile(): field=|");
  for (counter=0;counter<bytes;counter++) {
    printf("%i|",(int) (unsigned char) field[counter]); 
  }
  printf("\n");
#endif
  
  if (writemode==APPEND) {
    outfile=fopen(filename,"ab");
  } else if (writemode==NOAPPEND) {
    outfile=fopen(filename,"wb");
  } else EXITMSG("fieldtofile(): unknown writemode\n");
  if (outfile==NULL) EXITMSG("fieldtofile(): could not assign file\n");
  //printf("fieldtofile(): field=[%s]\n",field);
  fwrite(field,bytes,1,outfile);
#ifdef CODE_DEBUG
  printf("fieldtofile(): bytes=%i\n",bytes);
#endif
  fclose(outfile);
  return;
}


int fieldfromfile (char* field,char* filename) {
  FILE *infile; /* = stdout; */
  int bytes;
#ifdef CODE_DEBUG
  int counter;
#endif

  infile=fopen(filename,"rb");
  if (infile==NULL) EXITMSG("fieldfromfile(): could not assign file\n");

#ifdef _LIB_CHECKLIC_
  if (liccheckerror!=0) return(0);
#endif

  bytes=getfilesize(filename);
#ifdef CODE_DEBUG
  printf("fieldfromfile(): bytes=%i\n",bytes);
#endif
  fread(field,bytes,1,infile);
  fclose(infile);

#ifdef CODE_DEBUG
  printf("fieldfromfile(): field=|");
  for (counter=0;counter<bytes;counter++) {
    printf("%i|",(int) (unsigned char) field[counter]); 
  }
  printf("\n");
#endif

  return(bytes);
}


//Ende des Praeprozessor-If-Rumpfes
#endif






