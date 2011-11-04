/*
============================================================
coder.h
Codierung und Dekodierung von Strings bzw. String-Bitfeldern

199-08-27
H.-J. Mehnert
============================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "strops.h"

#ifdef __BORLANDC__
#include <string.h>
#endif

#ifndef _LIB_CODER_  
#define _LIB_CODER_


unsigned int stupidfunc(int x,int y) {
  int counter;
  unsigned int result;
  result=x;
  for (counter=2;counter<=y;counter++) {
    result=result*x;
  }
  return(result);
}


unsigned char inttochar6bit(int code)
{
    //printf("%i: ",(int) code);
    if (code < 26) {
        return (unsigned char)('a' +code);
    } else {
        if (code < (26+26)) {
            return (unsigned char)('A'+(code-26));
        } else {
            if (code < (26+26+10)) {
                return  (unsigned char)('0'+(code-26-26));
            } else {
	            if (code==62) {
	                return '&';
	            } else {
	                if (code==63) {
	                    return '*';
	                }
	            }
            }
        }
    }
    EXITMSG("inttochar6bit(): code out of range\n");
    return 0;
}


unsigned char inttochar5bit(int code)
{
  switch (code) {
  //kein '0' (null), J, Q und W
  case  0: return 'I';
  case  1: return 'E';
  case  2: return '9';
  case  3: return 'N';
  case  4: return 'U';
  case  5: return '4';
  case  6: return 'D';
  case  7: return '3';
  case  8: return 'H';
  case  9: return '7';
  case 10: return 'T';
  case 11: return 'M';
  case 12: return 'K';
  case 13: return 'A';
  case 14: return 'P';
  case 15: return 'V';
  case 16: return '5';
  case 17: return 'Y';
  case 18: return '1';
  case 19: return 'O';
  case 20: return 'F';
  case 21: return '8';
  case 22: return 'Z';
  case 23: return 'B';
  case 24: return 'L';
  case 25: return '6';
  case 26: return 'R';
  case 27: return 'G';
  case 28: return 'X';
  case 29: return '2';
  case 30: return 'S';
  case 31: return 'C';
  }
  EXITMSG("inttochar5bit(): code out of range\n");
  return 0;
}


int char5bittoint(unsigned char zeichen)
{
  switch (zeichen) {
  //kein '0' (null), J, Q und W
  case 'I': return  0;
  case 'E': return  1;
  case '9': return  2;
  case 'N': return  3;
  case 'U': return  4;
  case '4': return  5;
  case 'D': return  6;
  case '3': return  7;
  case 'H': return  8;
  case '7': return  9;
  case 'T': return 10;
  case 'M': return 11;
  case 'K': return 12;
  case 'A': return 13;
  case 'P': return 14;
  case 'V': return 15;
  case '5': return 16;
  case 'Y': return 17;
  case '1': return 18;
  case 'O': return 19;
  case 'F': return 20;
  case '8': return 21;
  case 'Z': return 22;
  case 'B': return 23;
  case 'L': return 24;
  case '6': return 25;
  case 'R': return 26;
  case 'G': return 27;
  case 'X': return 28;
  case '2': return 29;
  case 'S': return 30;
  case 'C': return 31;
  }
  EXITMSG("char5bittoint(): char out of range\n");
  return 0;
}


void strbitsinvert(char *string) {
  int counter,slen;
  slen = strlen(string);
  for (counter=0;counter<slen;counter++) {
    if (string[counter]=='1')
       string[counter]='0';
    else
       string[counter]='1';
  }
  return;
}


void strbitsfill2rnd(char *string) {
  int charloop,rndbit, slen = strlen(string);
  char *buffer=(char*)malloc(slen*2+1);
  time_t akttime;
  strcpy(buffer,"");
  time(&akttime);
  srand(50+akttime-(akttime/100)*100);
  for (charloop=0;charloop<slen;charloop++) {
    rndbit=(int) (2.0*rand() / (1.0*RAND_MAX));
    mysprintf(buffer,"%s%c%i\0",buffer,string[charloop],rndbit);
  }
  strcopy(buffer,string);
  free(buffer);
  return;
}


int strbitsexclor(char *source,char *pwd,char *dest) {
  int charloop,databit,pwdbit,resultbit,pwdpos, slen = strlen(source);
  char *buffer=(char*)malloc(slen+1);
  strcpy(buffer,"");
  pwdpos=0;
  //alle Datenbits durchlaufen
  for (charloop=0;charloop<slen;charloop++) {
    //Datenbit auslesen
    if (source[charloop]=='1') databit=1; else databit=0;
    //Passwortbit auslesen
    if (pwd[pwdpos]=='1') pwdbit=1; else pwdbit=0;   
    //Ergebnisbit errechnen
    resultbit=(databit ^ pwdbit);                     
    //Bits in Feld eintragen
    mysprintf(buffer,"%s%i\0",buffer,resultbit);
    //Zeiger auf aktuelles Passwort-Bit erhöhen
    pwdpos++;                                 
    //Passwort wiederholen
    if (pwdpos==(int)strlen(pwd)) pwdpos=0;
  }
  strcopy(buffer,dest);
  free(buffer);
  return 0;
}


void strbitsinsmagnum(char *string,int magicnum,int from,int spacing) {
  int bitloop,aktbit, slen = strlen(string) ;
  unsigned int factor;
  
  //  printf("strbitsinsmagnum(): string=[%s]\n",string);
  if ((from+15*spacing+1)>slen)
     EXITMSG("strbitsinsmagnum(): i-string too short\n");

  factor=stupidfunc(2,15);
  //  printf("strbitsinsmagnum(): factor=%u\n",factor);
  for (bitloop=0;bitloop<=15;bitloop++) {
    if (magicnum>=(int)factor) {
      aktbit=1;
      magicnum=magicnum-factor;
    } else aktbit=0;
    if (aktbit==1) string[from+bitloop*spacing]='1'; else string[from+bitloop*spacing]='0';
    //    printf("%i",aktbit);
    factor=factor/2;
  }
  //  printf("\n");
  return;
}


unsigned int strbitsextrmagnum(char *string,int from,int spacing) {
  int bitloop,aktbit,slen = strlen(string);
  unsigned int resultbyte,factor;

  if ((from+15*spacing+1)>slen) EXITMSG("strbitsextrmagnum(): i-string too short\n");
  
  factor=stupidfunc(2,15);
  resultbyte=0;
  for (bitloop=0;bitloop<=15;bitloop++) {
    if (string[from+(bitloop*spacing)]=='1') aktbit=1; else aktbit=0;
    if (aktbit==1) resultbyte=resultbyte+factor;
    factor=factor/2;
  }
  
  return(resultbyte);
}


int strbitstobytes(char *source,char *dest,int bitsperbyte) {
  int charloop,bitloop,resultbyte,factor,outbytes=0,slen = strlen(source);
 
  for (charloop=0;charloop<(slen/bitsperbyte);charloop++) {
    factor=stupidfunc(2,bitsperbyte-1);
    resultbyte=0;
    for (bitloop=0;bitloop<bitsperbyte;bitloop++) {
      //wenn aktuelles Bit=1...
      if (source[(charloop*bitsperbyte)+bitloop]=='1') {
	//..Faktor zu Ergebnisbyte addieren
	resultbyte=resultbyte+factor;
      }
      //das nächste Bit weiter rechts hat den halben Faktor
      factor=factor/2;
    }
    //Ergebnisbyte in öffentliches Feld output schreiben
    switch (bitsperbyte) {
      case 5: resultbyte=(int) inttochar5bit((unsigned char) resultbyte);break;
      case 6: resultbyte=(int) inttochar6bit((unsigned char) resultbyte);break;
      case 8: break;
      default: return 1;
    }    
    dest[charloop]=(char)resultbyte;
    outbytes++;
  }
  return(outbytes);
}


void bytestostrbits(char *source,char *dest,int inbytes,int bitsperbyte) {
  int charloop,bitloop,aktbit;
  char aktvalue;
  char buffer[4096];
  strcpy(dest,"");
#ifdef CODE_DEBUG
  printf("bytestostrbits(): inbytes=%i\n",inbytes);
#endif
  for (charloop=0;charloop<inbytes;charloop++) { //alle zeichen durchgehen
    for (bitloop=(bitsperbyte-1);bitloop>=0;bitloop--) { //alle bits durchgehen
      aktvalue=source[charloop];
      switch(bitsperbyte) {
        case 5: aktvalue = (char)char5bittoint((unsigned char)aktvalue); break;
        case 6: EXITMSG("bytestostrbits(): 6 bits per byte not supported yet\n");
        case 8: aktvalue=aktvalue; break;
      }
      aktbit=(int) ((aktvalue >> bitloop) & 1);
      // ...und in das Feld schreiben
      mysprintf(buffer,"%s%i\0",dest,aktbit);
      mysprintf(dest,"%s\0",buffer);
    }
  }
#ifdef CODE_DEBUG
  printf("bytestostrbits(): dest=[%s]\n",dest);
#endif
  return;
}

void coder_decode (unsigned char *input,char *output,int inbytes,char *pwd) {
  char inputfield  [4096];
  char pwdfield    [4096];
  char decodedfield[4096];
  char prebuffer   [4096];
  int outbytes;
#ifdef CODE_DEBUG
  printf("coder_decode(): got %i bytes\n",inbytes);
#endif
  bytestostrbits((char*)input,inputfield,inbytes,8);
#ifdef CODE_DEBUG
  printf("coder_decode(): inputfield=[%s]\n",inputfield);
#endif
  strshrink     (inputfield,2,2);
#ifdef CODE_DEBUG
  printf("coder_decode(): inputfield=[%s]\n",inputfield);
#endif
  bytestostrbits(pwd,pwdfield,strlen(pwd),8);
#ifdef CODE_DEBUG
  printf("coder_decode(): pwdfield=[%s]\n",pwdfield);
#endif
  strbitsexclor (inputfield,pwdfield,decodedfield);
#ifdef CODE_DEBUG
  printf("coder_decode(): decodedfield=[%s]\n",decodedfield);
#endif
  strbitsinvert (decodedfield);
#ifdef CODE_DEBUG
  printf("coder_decode(): decodedfield=[%s]\n",decodedfield);
#endif
  outbytes=strbitstobytes(decodedfield,prebuffer,8);
#ifdef CODE_DEBUG
  printf("coder_decode(): prebuffer=[%s]\n",prebuffer);
#endif
  prebuffer[outbytes]='\0';
  strcopy(prebuffer,output);
  return;
}

int bytescopy(char* source,char* dest,int tocopy)
{
  int counter;
  for (counter=0;counter<tocopy;counter++) {
    dest[counter]=source[counter];
  }
  return(tocopy);
}

int coder_encode (char *input,unsigned char *output,char *pwd) {
  char inputfield   [4096];
  char pwdfield     [4096];
  char encodedfield [4096];
  char prebuffer [4096];
  int  outbytes;

#ifdef CODE_DEBUG
  printf("coder_encode(): input=[%s]\n",input);
#endif
  bytestostrbits(input,inputfield,strlen(input),8);
#ifdef CODE_DEBUG
  printf("coder_encode(): inputfield=[%s]\n",inputfield);
#endif

  bytestostrbits(pwd,pwdfield,strlen(pwd),8);
#ifdef CODE_DEBUG
  printf("coder_encode(): pwdfield=[%s]\n",pwdfield);
#endif

  strbitsexclor(inputfield,pwdfield,encodedfield);
#ifdef CODE_DEBUG
  printf("coder_encode(): encodedfield=[%s]\n",encodedfield);
#endif

  strbitsinvert(encodedfield);
  strbitsfill2rnd(encodedfield);
#ifdef CODE_DEBUG
  printf("coder_encode(): encodedfield=[%s]\n",encodedfield);
#endif

  outbytes=strbitstobytes(encodedfield,prebuffer,8);
  prebuffer[outbytes]='\0';
  bytescopy(prebuffer,(char*)output,outbytes);
#ifdef CODE_DEBUG
  printf("coder_encode(): outbytes=%i\n",outbytes);
#endif
  return(outbytes);
}

void fillkeystep3(char *key,char *source) {
  int bitloop,looptype,loopstep,aktbit,aktkeybit,slen = strlen(source);
  looptype=0;
  loopstep=0;
  for (bitloop=0;bitloop<slen;bitloop++) {
    if (source[bitloop]=='1') aktbit=1; else aktbit=0;
    if (key[loopstep]=='1') aktkeybit=1; else aktkeybit=0;
    if ((aktbit^aktkeybit)==1) key[loopstep]='1'; else key[loopstep]='0';

    loopstep=loopstep+3;
    if (loopstep>=100) {
      looptype++;
      if (looptype==3) {
	looptype=1;
      }
      loopstep=looptype;
    }
  }
  return;
}


unsigned int getmagicnumfrom5x4key(char *key) {
  unsigned int magicnum;
  char keyfield[4096];

  strfilter(key,'-');
  bytestostrbits(key,keyfield,strlen(key),5);
  magicnum=strbitsextrmagnum(keyfield,5,6);

  return(magicnum);
}


void generate5x4Key(char *output,char *input,char *pwd,unsigned int magicnum) {
  int outlen;
  char inputfield[4096];
  char keyfield[100+1];

  if (pwd[0]==123) {}

#ifdef CODE_DEBUG
  printf("generate5x4key(): input=[%s]\n",input);
#endif
  bytestostrbits(input,inputfield,strlen(input),8);
#ifdef CODE_DEBUG
  printf("generate5x4key(): inputfield=[%s]\n",inputfield);
#endif
  strmirror(inputfield);
#ifdef CODE_DEBUG
  printf("generate5x4key(): inputfield=[%s]\n",inputfield);
#endif
  strbitsinvert(inputfield);
#ifdef CODE_DEBUG
  printf("generate5x4key(): inputfield=[%s]\n",inputfield);
#endif
  strfill(keyfield,100,'0');
#ifdef CODE_DEBUG
  printf("generate5x4key(): keyfield=[%s]\n",keyfield);
#endif
  fillkeystep3(keyfield,inputfield);
#ifdef CODE_DEBUG
  printf("generate5x4key(): keyfield=[%s]\n",keyfield);
#endif
  strcircle(keyfield,magicnum);
#ifdef CODE_DEBUG
  printf("generate5x4key(): keyfield=[%s]\n",keyfield);
#endif
  strbitsinsmagnum(keyfield,magicnum,5,6);
#ifdef CODE_DEBUG
  printf("generate5x4key(): keyfield=[%s]\n",keyfield);
#endif
  outlen=strbitstobytes(keyfield,output,5);
  output[outlen]='\0';
#ifdef CODE_DEBUG
  printf("generate5x4key(): outlen=%i\n",outlen);
  printf("generate5x4key(): output=[%s]\n",output);
#endif
  strsplit(output,5,4,'-');
#ifdef CODE_DEBUG
  printf("generate5x4key(): output=[%s]\n",output);
#endif

  return;
} 


//Ende des Präprozessor-If-Rumpfes
#endif










