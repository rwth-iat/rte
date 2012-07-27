/** @file 
 * xdrhandling generates XDRs and analyses the reply XDRs
 ******************************************************************************/
#include "xdrhandling.h"

#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include <string.h>
#include "assert.h"
#include "config.h"

#if !OV_SYSTEM_NT
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include "libov/ov_ov.h"

#include "ksapi_logfile.h"
#define logfile_error ksapi_logfile_error
#define logfile_debug ksapi_logfile_debug


void printxdr(char xdr[], int xdrlength) {
	int i;
	int j;
	int temp;

	for(i=0; i < xdrlength; i +=4)
	{
		for(j=0; j<4; j++)
		{
			((char*) &temp)[3-j] = xdr[i + j];

		}
		logfile_debug("ab %d: %u - %x", i, temp, temp);
		logfile_debug("%c%c%c%c\n", xdr[i], xdr[i+1], xdr[i+2], xdr[i+3]);

	}
	return;

}


/**
 * Generates the GETVAR xdr
 * input variables are:
 * char *xdr[] --> here the generated xdr is stored
 * int *length --> here the length of the xdr is stored
 * char *path --> path to the variable which should be retrieved
 */
void generategetxdr(char *xdr[], int *length, char *path)
{
	generateheader(KS_GETVAR, xdr, length);
	generategetbody(xdr, length, path);
	addrpcheader(xdr, length);
	return;
}

/**
 * Generates the SETVAR for STRING variable
 * input variables are:
 * char *xdr[] : here the generated xdr is stored
 * int *length : here the length of the xdr is stored
 * char *path : path to the variable which should be set
 * char *setstring : string which should be set to dedicated variable
 */
void generatesetstringxdr(char *xdr[], int *length, char *path, char *setstring,
		unsigned long varTsSecs, unsigned long varTsUsecs, unsigned long varQState)
{
	generateheader(KS_SETVAR, xdr, length);
	generatesetbody(OV_VT_STRING, xdr, length, path, &setstring, varTsSecs, varTsUsecs, varQState);
	addrpcheader(xdr, length);

	return;
}

/**
 * Generates the SETVAR for STRINGVEC variable
 * input variables are:
 * char *xdr[] : here the generated xdr is stored
 * int *length : here the length of the xdr is stored
 * char *path : path to the variable which should be set
 * char **setstringvec : a vector of strings which should be set to dedicated variable
 * int veclength : length of setstringvec
 */
void generatesetstringvecxdr(char *xdr[], int *length, char *path, char **setstringvec, int veclength,
		unsigned long varTsSecs, unsigned long varTsUsecs, unsigned long varQState)
{
	generateheader(KS_SETVAR, xdr, length);
	generatesetvecbody(OV_VT_STRING_VEC, xdr, length, path, &setstringvec, veclength, varTsSecs, varTsUsecs, varQState);
	addrpcheader(xdr, length);

	return;
}

/**
 * Generates the SETVAR for BOOL variable
 * input variables are:
 * char *xdr[] : here the generated xdr is stored
 * int *length : here the length of the xdr is stored
 * char *path : path to the variable which should be set
 * int setbool : bool value (0 or 1) which should be set to dedicated variable
 */
void generatesetboolxdr(char *xdr[], int *length, char *path, int setbool,
		unsigned long varTsSecs, unsigned long varTsUsecs, unsigned long varQState)
{
	generateheader(KS_SETVAR, xdr, length);
	generatesetbody(OV_VT_BOOL, xdr, length, path, &setbool, varTsSecs, varTsUsecs, varQState);
	addrpcheader(xdr, length);	

	return;
}

/**
 * Generates the SETVAR for BOOLVEC variable
 * input variables are:
 * char *xdr[] : here the generated xdr is stored
 * int *length : here the length of the xdr is stored
 * char *path : path to the variable which should be set
 * int setboolvec[] : a vector of bool values which should be set to dedicated variable
 * int veclength : length of setboolvec
 */
void generatesetboolvecxdr(char *xdr[], int *length, char *path, int setboolvec[], int veclength,
		unsigned long varTsSecs, unsigned long varTsUsecs, unsigned long varQState)
{
	generateheader(KS_SETVAR, xdr, length);
	generatesetvecbody(OV_VT_BOOL_VEC, xdr, length, path, &setboolvec, veclength, varTsSecs, varTsUsecs, varQState);
	addrpcheader(xdr, length);

	return;
}

/**
 * Generates the SETVAR for DOUBLE variable
 * input variables are:
 * char *xdr[] : here the generated xdr is stored
 * int *length : here the length of the xdr is stored
 * char *path : path to the variable which should be set
 * double setdouble : double value which should be set to dedicated variable
 */
void generatesetdoublexdr(char *xdr[], int *length, char *path, double setdouble,
		unsigned long varTsSecs, unsigned long varTsUsecs, unsigned long varQState)
{
	generateheader(KS_SETVAR, xdr, length);
	generatesetbody(OV_VT_DOUBLE, xdr, length, path, &setdouble, varTsSecs, varTsUsecs, varQState);
	addrpcheader(xdr, length);	

	return;
}

/**
 * Generates the SETVAR for DOUBLEVEC variable
 * input variables are:
 * char *xdr[] : here the generated xdr is stored
 * int *length : here the length of the xdr is stored
 * char *path : path to the variable which should be set
 * double setdoublevec[] : a vector of double values which should be set to dedicated variable
 * int veclength : length of setdoublevec
 */
void generatesetdoublevecxdr(char *xdr[], int *length, char *path, double setdoublevec[], int veclength,
		unsigned long varTsSecs, unsigned long varTsUsecs, unsigned long varQState)
{
	generateheader(KS_SETVAR, xdr, length);
	generatesetvecbody(OV_VT_DOUBLE_VEC, xdr, length, path, &setdoublevec, veclength, varTsSecs, varTsUsecs, varQState);
	addrpcheader(xdr, length);	

	return;
}

/**
 * Generates the SETVAR for INT variable
 * input variables are:
 * char *xdr[] : here the generated xdr is stored
 * int *length : here the length of the xdr is stored
 * char *path : path to the variable which should be set
 * int setint : integer value which should be set to dedicated variable
 */
void generatesetintxdr(char *xdr[], int *length, char *path, int setint,
		unsigned long varTsSecs, unsigned long varTsUsecs, unsigned long varQState)
{
	generateheader(KS_SETVAR, xdr, length);
	generatesetbody(OV_VT_INT, xdr, length, path, &setint, varTsSecs, varTsUsecs, varQState);
	addrpcheader(xdr, length);

	return;
}

/**
 * Generates the SETVAR for INTVEC variable
 * input variables are:
 * char *xdr[] : here the generated xdr is stored
 * int *length : here the length of the xdr is stored
 * char *path : path to the variable which should be set
 * int setintvec[] : a vector of integer values which should be set to dedicated variable
 * int veclength : length of setintvec
 */
void generatesetintvecxdr(char *xdr[], int *length, char *path, int setintvec[], int veclength,
		unsigned long varTsSecs, unsigned long varTsUsecs, unsigned long varQState)
{
	generateheader(KS_SETVAR, xdr, length);
	generatesetvecbody(OV_VT_INT_VEC, xdr, length, path, &setintvec, veclength, varTsSecs, varTsUsecs, varQState);
	addrpcheader(xdr, length);	

	return;
}

/**
 * Generates the SETVAR for SINGLE variable
 * input variables are:
 * char *xdr[] : here the generated xdr is stored
 * int *length : here the length of the xdr is stored
 * char *path : path to the variable which should be set
 * float setsingle : float value which should be set to dedicated variable
 */
void generatesetsinglexdr(char *xdr[], int *length, char *path, float setsingle,
		unsigned long varTsSecs, unsigned long varTsUsecs, unsigned long varQState)
{
	generateheader(KS_SETVAR, xdr, length);
	generatesetbody(OV_VT_SINGLE, xdr, length, path, &setsingle, varTsSecs, varTsUsecs, varQState);
	addrpcheader(xdr, length);

	return;
}

/**
 * Generates the SETVAR for SINGLEVEC variable
 * input variables are:
 * char *xdr[] : here the generated xdr is stored
 * int *length : here the length of the xdr is stored
 * char *path : path to the variable which should be set
 * float setsinglevec[] : a vector of float values which should be set to dedicated variable
 * int veclength : length of setsinglevec
 */
void generatesetsinglevecxdr(char *xdr[], int *length, char *path, float setsinglevec[], int veclength,
		unsigned long varTsSecs, unsigned long varTsUsecs, unsigned long varQState)
{
	generateheader(KS_SETVAR, xdr, length);
	generatesetvecbody(OV_VT_SINGLE_VEC, xdr, length, path, &setsinglevec, veclength, varTsSecs, varTsUsecs, varQState);
	addrpcheader(xdr, length);
	return;
}

/**
 * Generates the SETVAR for UINT variable
 * input variables are:
 * char *xdr[] : here the generated xdr is stored
 * int *length : here the length of the xdr is stored
 * char *path : path to the variable which should be set
 * uint setuint : unsigned integer value which should be set to dedicated variable
 */
void generatesetuintxdr(char *xdr[], int *length, char *path, unsigned int setuint,
		unsigned long varTsSecs, unsigned long varTsUsecs, unsigned long varQState)
{
	generateheader(KS_SETVAR, xdr, length);
	generatesetbody(OV_VT_UINT, xdr, length, path, &setuint, varTsSecs, varTsUsecs, varQState);
	addrpcheader(xdr, length);

	return;
}

/**
 * Generates the SETVAR for UINTVEC variable
 * input variables are:
 * char *xdr[] : here the generated xdr is stored
 * int *length : here the length of the xdr is stored
 * char *path : path to the variable which should be set
 * uint setuintvec[] : a vector of unsigned integer values which should be set to dedicated variable
 * int veclength : length of setuintvec
 */
void generatesetuintvecxdr(char *xdr[], int *length, char *path, unsigned int setuintvec[], int veclength,
		unsigned long varTsSecs, unsigned long varTsUsecs, unsigned long varQState)
{
	generateheader(KS_SETVAR, xdr, length);
	generatesetvecbody(OV_VT_UINT_VEC, xdr, length, path, &setuintvec, veclength, varTsSecs, varTsUsecs, varQState);
	addrpcheader(xdr, length);

	return;
}

/**
 * Generates the xdr for DELETEOBJECT
 * input variables are:
 * char *xdr[] : here the generated xdr is stored
 * int *length : here the length of the xdr is stored
 * char *path : path to the object which should be deleted
 */
void generatedeleteobjectxdr(char *xdr[], int *length, char *path)
{
	int c, xdrposition;
	int pathlength;
	int pathlengthmod4;

	generateheader(KS_DELETEOBJECT, xdr, length);
	xdrposition = *length;

	//set pathlength
	pathlength = strlen(path);
	pathlengthmod4 = pathlength;
	while(pathlengthmod4 % 4)
		pathlengthmod4++;

	//set xdrlenght
	*length += pathlengthmod4+56;
	*xdr = realloc(*xdr, *length);
	memset(&((*xdr)[xdrposition]), 0, (*length)-xdrposition);

	//set path
	for(c=0; c < 4; c++)
		(*xdr)[xdrposition+c] = ((char*)(&pathlength))[3-c];
	xdrposition += 4;
	memcpy(&((*xdr)[xdrposition]), path, pathlength);
	xdrposition += pathlengthmod4;

	addrpcheader(xdr, length);
	return;
}

/**
 * Generates the xdr for CREATEOBJECT
 * input variables are:
 * char *xdr[] : here the generated xdr is stored
 * int *length : here the length of the xdr is stored
 * char *path : path where the object should be created
 * char *librarypath : path to the type defining object
 * int position : position to place the object in the hierarchy
 * 					(0: default, 1: begin, 2: end, 3: before, 4: after)
 * char *refpath : path to object before or after refers to
 */
void generatecreateobjectxdr(char *xdr[], int *length, char *path, char *librarypath, int position, char *refpath)
{
	int c;
	int librarypathlength, pathlength, refpathlength, xdrposition;
	int librarypathlengthmod4, pathlengthmod4, refpathlengthmod4;

	generateheader(KS_CREATEOBJECT, xdr, length);
	xdrposition = *length;

	//set librarypathlength
	librarypathlength = strlen(librarypath);
	librarypathlengthmod4 = librarypathlength;
	while(librarypathlengthmod4 % 4)
		librarypathlengthmod4++;
	//set pathlength
	pathlength = strlen(path);
	pathlengthmod4 = pathlength;
	while(pathlengthmod4 % 4)
		pathlengthmod4++;
	//set refpathlength
	if(refpath)
		refpathlength = strlen(refpath);
	else
		refpathlength = 0;
	refpathlengthmod4 = refpathlength;
	while(refpathlengthmod4 % 4)
		refpathlengthmod4++;
	//set xdrlenght
	*length += librarypathlengthmod4+pathlengthmod4+refpathlengthmod4+68;
	//if relative placement: add space for PMH-length
	if ((position == KS_PMH_BEFORE) | (position == KS_PMH_AFTER))
		*length += 4;

	*xdr = realloc(*xdr, *length);
	memset(&((*xdr)[xdrposition]), 0, (*length)-xdrposition);


	//set librarypath
	for(c=0; c < 4; c++)
		(*xdr)[xdrposition+c] = ((char*)(&librarypathlength))[3-c];
	xdrposition += 4;
	memcpy(&((*xdr)[xdrposition]), librarypath, librarypathlength);
	xdrposition += librarypathlengthmod4;

	//set path
	for(c=0; c < 4; c++)
		(*xdr)[xdrposition+c] = ((char*)(&pathlength))[3-c];
	xdrposition += 4;
	memcpy(&((*xdr)[xdrposition]), path, pathlength);
	xdrposition += pathlengthmod4;

	//set PMH
	(*xdr)[xdrposition+3] = position;
	xdrposition += 4;

	//set refpath if PMH indicates a relative position
	if ((position == KS_PMH_BEFORE) | (position == KS_PMH_AFTER))
	{
		for(c=0; c < 4; c++)
			(*xdr)[xdrposition+c] = ((char*)(&refpathlength))[3-c];
		xdrposition += 4;
		memcpy(&((*xdr)[xdrposition]), refpath, refpathlength);
		xdrposition += refpathlengthmod4;
	}	

	//add rpcheader
	addrpcheader(xdr, length);

	return;
}

/**
 * Generates the xdr for RENAMEOBJECT
 * input variables are:
 * char *xdr[] : here the generated xdr is stored
 * int *length : here the length of the xdr is stored
 * char *oldpath : path to the object which should be renamed
 * char *newpath : new path of the renamed object
 */
void generaterenameobjectxdr(char *xdr[], int *length, char *oldpath, char *newpath)
{
	int c, xdrposition;
	int oldpathlength, newpathlength;
	int oldpathlengthmod4, newpathlengthmod4;

	generateheader(KS_RENAMEOBJECT, xdr, length);
	xdrposition = *length;

	//set oldpathlength
	oldpathlength = strlen(oldpath);
	oldpathlengthmod4 = oldpathlength;
	while(oldpathlengthmod4 % 4)
		oldpathlengthmod4++;

	//set oldpathlength
	newpathlength = strlen(newpath);
	newpathlengthmod4 = newpathlength;
	while(newpathlengthmod4 % 4)
		newpathlengthmod4++;

	//set xdrlenght
	*length += oldpathlengthmod4+newpathlengthmod4+60;
	*xdr = realloc(*xdr, *length);
	memset(&((*xdr)[xdrposition]), 0, (*length)-xdrposition);

	//set oldpath
	for(c=0; c < 4; c++)
		(*xdr)[xdrposition+c] = ((char*)(&oldpathlength))[3-c];
	xdrposition += 4;
	memcpy(&((*xdr)[xdrposition]), oldpath, oldpathlength);
	xdrposition += oldpathlengthmod4;

	//set newpath
	for(c=0; c < 4; c++)
		(*xdr)[xdrposition+c] = ((char*)(&newpathlength))[3-c];
	xdrposition += 4;
	memcpy(&((*xdr)[xdrposition]), newpath, newpathlength);
	xdrposition += newpathlengthmod4;

	addrpcheader(xdr, length);
	return;
}

/**
 * Generates the xdr for UNLINKOBJECT
 * input variables are:
 * char *xdr[] : here the generated xdr is stored
 * int *length : here the length of the xdr is stored
 * char *path : path to the object - with anchor - which should be unlinked
 * char *linkedpath : path of the (child) object - without anchor -  which should be unlinked with selected object
 */
void generateunlinkobjectxdr(char *xdr[], int *length, char *path, char *linkedpath)
{
	int c, xdrposition;
	int pathlength, linkedpathlength;
	int pathlengthmod4, linkedpathlengthmod4;

	generateheader(KS_UNLINK, xdr, length);
	xdrposition = *length;

	//set pathlength
	pathlength = strlen(path);
	pathlengthmod4 = pathlength;
	while(pathlengthmod4 % 4)
		pathlengthmod4++;

	//set linkedpathlength
	linkedpathlength = strlen(linkedpath);
	linkedpathlengthmod4 = linkedpathlength;
	while(linkedpathlengthmod4 % 4)
		linkedpathlengthmod4++;

	//set xdrlenght
	*length += pathlengthmod4+linkedpathlengthmod4+60;
	*xdr = realloc(*xdr, *length);
	memset(&((*xdr)[xdrposition]), 0, (*length)-xdrposition);

	//set path
	for(c=0; c < 4; c++)
		(*xdr)[xdrposition+c] = ((char*)(&pathlength))[3-c];
	xdrposition += 4;
	memcpy(&((*xdr)[xdrposition]), path, pathlength);
	xdrposition += pathlengthmod4;

	//set linkedpath
	for(c=0; c < 4; c++)
		(*xdr)[xdrposition+c] = ((char*)(&linkedpathlength))[3-c];
	xdrposition += 4;
	memcpy(&((*xdr)[xdrposition]), linkedpath, linkedpathlength);
	xdrposition += linkedpathlengthmod4;

	addrpcheader(xdr, length);
	return;
}

/**
 * Generates the xdr for LINKOBJECT
 * input variables are:
 * char *xdr[] : here the generated xdr is stored
 * int *length : here the length of the xdr is stored
 * char *path : path to the object - with anchor - which should be linked
 * char *linkedpath : path of the (child) object - without anchor -  which should be linked with selected object
 * int position : position to place the object in the hierarchy
 * 					(0: default, 1: begin, 2: end, 3: before, 4: after)
 * char *refpath : path to object before or after refers to
 */
void generatelinkobjectxdr(char *xdr[], int *length, char *path, char *linkedpath, int position, char *refpath)
{
	int c, xdrposition;
	int pathlength, linkedpathlength;
	int pathlengthmod4, linkedpathlengthmod4;

	generateheader(KS_LINK, xdr, length);
	xdrposition = *length;

	//set pathlength
	pathlength = strlen(path);
	pathlengthmod4 = pathlength;
	while(pathlengthmod4 % 4)
		pathlengthmod4++;

	//set linkedpathlength
	linkedpathlength = strlen(linkedpath);
	linkedpathlengthmod4 = linkedpathlength;
	while(linkedpathlengthmod4 % 4)
		linkedpathlengthmod4++;

	//set xdrlenght
	*length += pathlengthmod4+linkedpathlengthmod4+60;
	*xdr = realloc(*xdr, *length);
	memset(&((*xdr)[xdrposition]), 0, (*length)-xdrposition);

	//set path
	for(c=0; c < 4; c++)
		(*xdr)[xdrposition+c] = ((char*)(&pathlength))[3-c];
	xdrposition += 4;
	memcpy(&((*xdr)[xdrposition]), path, pathlength);
	xdrposition += pathlengthmod4;

	//set linkedpath
	for(c=0; c < 4; c++)
		(*xdr)[xdrposition+c] = ((char*)(&linkedpathlength))[3-c];
	xdrposition += 4;
	memcpy(&((*xdr)[xdrposition]), linkedpath, linkedpathlength);
	xdrposition += linkedpathlengthmod4;

	addrpcheader(xdr, length);
	return;
}


/**
 * Generates the xdr for GETEP
 * input variables are:
 * char *xdr[] : here the generated xdr is stored
 * int *length : here the length of the xdr is stored
 * char *path : path to the object EPs should be retrieved from
 * int ksobjecttype : ???
 * char *namemask : filter applied on existing EPs before reply
 * int epflags : ???
 */
void generategetepxdr(char *xdr[], int *length, char *path, int ksobjecttype, char *namemask, int epflags)
{
	int c, xdrposition;
	int pathlength, namemasklength;
	int pathlengthmod4, namemasklengthmod4;
	generateheader(KS_GETEP, xdr, length);
	xdrposition = (*length) - 4;		//ovwewrite last 4 bytes of header in GetEP (for whatever reason^^)

	//set pathlength
	pathlength = strlen(path);
	pathlengthmod4 = pathlength;
	while(pathlengthmod4 % 4)
		pathlengthmod4++;

	//set namemasklength
	namemasklength = strlen(namemask);
	namemasklengthmod4 = namemasklength;
	while(namemasklengthmod4 % 4)
		namemasklengthmod4++;

	//set xdrlenght
	*length += pathlengthmod4+namemasklengthmod4+12;
	*xdr = realloc(*xdr, *length);
	memset(&((*xdr)[xdrposition]), 0, (*length)-xdrposition);

	//set path
	for(c=0; c < 4; c++)
		(*xdr)[xdrposition+c] = ((char*)(&pathlength))[3-c];
	xdrposition += 4;
	memcpy(&((*xdr)[xdrposition]), path, pathlength);
	xdrposition += pathlengthmod4;

	//ksobjecttype
	for(c=0; c < 4; c++)
		(*xdr)[xdrposition+c] = ((char*)(&ksobjecttype))[3-c];
	xdrposition += 4;

	//set namemask
	for(c=0; c < 4; c++)
		(*xdr)[xdrposition+c] = ((char*)(&namemasklength))[3-c];
	xdrposition += 4;
	memcpy(&((*xdr)[xdrposition]), namemask, namemasklength);
	xdrposition += namemasklengthmod4;

	//epflags
	for(c=0; c < 4; c++)
		(*xdr)[xdrposition+c] = ((char*)(&epflags))[3-c];
	xdrposition += 4;

	addrpcheader(xdr, length);

	return;
}


/**
 * Analyses GET reply
 * return value is errorcode (0: OK)
 * input variables are:
 * int type : OV_VT_* defines type of variable to decode
 * char xdr[] : received xdr which should be analysed
 * int xdrlength : length of xdr
 * void *reply : here received VAR is stored
 * unsigned long *repTsSecs : here the seconds part of the timestamp is saved
 * unsigned long *repTsUsecs : here the microseconds part of the timestamp is saved
 * unsigned long *repState : here the quality-state of the variable is saved
 */
int analysegetreply(int type, char xdr[], int xdrlength, void *reply, unsigned long *repTsSecs, unsigned long *repTsUsecs, unsigned long *repState)
{
	int errorcode, vartype;
	char *str=NULL;
	int strlength;
	int i;
	char temp[4];
	char tempd[8];
	int j;

	//decoding
	if(xdr[39] == 0) // OK, found
	{
		for(i=0; i<4; i++)
			((char*) &vartype)[3-i] = xdr[44+i]; // copy type


		if((vartype & OV_VT_KSMASK) == type && type == OV_VT_STRING)	//decoding STRING
		{
			logfile_debug("\ndecoding STRING\n");
			for(j=0; j<4; j++)
				((char*) &strlength)[3-j] = xdr[48+j];

			str = (char*)malloc((strlength+1)*sizeof(char));
			for (i = 0; i < strlength; i++)
				str[i] = xdr[52+i]; //Ok, start reading
			*(char**)reply = malloc((strlength+1)*sizeof(char));
			memcpy(*(char**)reply, str, strlength+1);
			(*(char**) reply)[strlength] = '\0';
			free(str);

			while(strlength%4)
				strlength++;
			//decode TimeStamps seconds
			if(repTsSecs)
			{
				for (i = 3; i >= 0; i--)
					temp[3-i] = xdr[52+strlength+i];
				memcpy(repTsSecs, temp, 4);
			}
			//decode TimeStamps microseconds
			if(repTsUsecs)
			{
				for (i = 3; i >= 0; i--)

					temp[3-i] = xdr[56+strlength+i];
				memcpy(repTsUsecs, temp, 4);
			}
			//decode state
			if(repState)
			{
				for (i = 3; i >= 0; i--)
					temp[3-i] = xdr[60+strlength+i];
				memcpy(repState, temp, 4);
			}
		}
		else if((vartype & OV_VT_KSMASK) == type && type == OV_VT_DOUBLE)	//decoding DOUBLE
		{
			logfile_debug("\ndecoding DOUBLE\n");
			for (i = 7; i >= 0; i--)
				tempd[7-i] = xdr[48+i];
			memcpy(reply, tempd, 8);

			//decode TimeStamps seconds
			if(repTsSecs)
			{
				for (i = 3; i >= 0; i--)
					temp[3-i] = xdr[56+i];
				memcpy(repTsSecs, temp, 4);
			}
			//decode TimeStamps microseconds
			if(repTsUsecs)
			{
				for (i = 3; i >= 0; i--)
					temp[3-i] = xdr[60+i];
				memcpy(repTsUsecs, temp, 4);
			}
			//decode state
			if(repState)
			{
				for (i = 3; i >= 0; i--)
					temp[3-i] = xdr[64+i];
				memcpy(repState, temp, 4);
			}
		}
		else if((vartype & OV_VT_KSMASK) == type)		//decoding INT, UINT, SINGLE and BOOL
		{
			logfile_debug("\ndecoding INT, UINT, SINGLE and BOOL\n");
			for (i = 3; i >= 0; i--)
				temp[3-i] = xdr[48+i];
			memcpy(reply, temp, 4);
			//decode TimeStamps seconds
			if(repTsSecs)
			{
				for (i = 3; i >= 0; i--)

					temp[3-i] = xdr[52+i];
				memcpy(repTsSecs, temp, 4);
			}
			//decode TimeStamps microseconds
			if(repTsUsecs)
			{
				for (i = 3; i >= 0; i--)
					temp[3-i] = xdr[56+i];
				memcpy(repTsUsecs, temp, 4);
			}
			//decode state
			for (i = 3; i >= 0; i--)
				temp[3-i] = xdr[60+i];
			memcpy(repState, temp, 4);
		}
		else
		{
			logfile_error("\nvartype mismatch\n");
			return KS_ERR_BADTYPE;
		}

		return KS_ERR_OK;

	}
	else //bad luck - no result
	{
		errorcode = xdr[xdrlength-1];
		switch (errorcode)
		{
		case KS_ERR_BADPATH:
			logfile_error("\nwrong path\n");
			break;
		case KS_ERR_NOACCESS:
			logfile_error("\nno access\n");
			break;
		default:
			logfile_error("\nERROR\n");
		}
	}


	return errorcode;
}

/**
 * Analyses GETVEC reply
 * return value is errorcode (0: OK)
 * input variables are:
 * int type : OV_VT_* defines type of variable to decode
 * char xdr[] : received xdr which should be analysed
 * int xdrlength : length of xdr
 * void **reply : here received VARVEC is stored
 * int *veclength: length of received vector is stored here
 * unsigned long *repTsSecs : here the seconds part of the timestamp is saved
 * unsigned long *repTsUsecs : here the microseconds part of the timestamp is saved
 * unsigned long *repState : here the quality-state of the variable is saved
 */
int analysegetvecreply(int type, char xdr[], int xdrlength, void **reply, int *veclength, unsigned long *repTsSecs, unsigned long *repTsUsecs, unsigned long *repState)
{
	int errorcode, vartype = 0;
	int strlength;
	int i;
	int j;
	int xdrposition;
	char temp[4];

	logfile_debug("\nanalysing getvec reply\n");

	//decoding
	if(xdr[39] == 0) // OK, found
	{
		for(i=0; i<4; i++)
			((char*) &vartype)[3-i] = xdr[44+i]; // copy type

		logfile_debug("vartype: %x", vartype);

		if((vartype & OV_VT_KSMASK) == type && type == OV_VT_STRING_VEC)	//decoding STRINGVEC
		{
			for(j=0; j<4; j++) // read length of vec
				((char*) veclength)[3-j] = xdr[48+j];
			*reply = (char**)malloc(*veclength*sizeof(char*)); // get space for vector length
			xdrposition = 52; //start decode, will be incremented per read string
			for(i=0; i<*veclength; i++) // iterate over strings
			{
				for(j=0; j<4; j++) //read length of current string
					((char*) &strlength)[3-j] = xdr[xdrposition+j];
				((char**) *reply)[i] = malloc(strlength+1); //get space for current string
				xdrposition += 4; // go to position of string start
				memcpy(((char**) *reply)[i], xdr+xdrposition, strlength); //save current string
				((char**) *reply)[i][strlength] = 0; //0terminate current string

				xdrposition += strlength; // go over to next string
				while(xdrposition%4) //...but take care of modulo4 encoding in XDRs!
					xdrposition++;
			}

			//decode TimeStamps seconds
			if(repTsSecs)
			{
				for (i = 3; i >= 0; i--)
					temp[3-i] = xdr[xdrposition+i];
				memcpy(repTsSecs, temp, 4);
			}
			xdrposition+=4;
			//decode TimeStamps microseconds
			if(repTsUsecs)
			{
				for (i = 3; i >= 0; i--)
					temp[3-i] = xdr[xdrposition+i];
				memcpy(repTsUsecs, temp, 4);
			}
			xdrposition+=4;
			//decode state
			if(repState)
			{
				for (i = 3; i >= 0; i--)
					temp[3-i] = xdr[xdrposition+i];
				memcpy(repState, temp, 4);
			}
			xdrposition+=4;

		}
		else if((vartype & OV_VT_KSMASK) == type && type == OV_VT_DOUBLE_VEC)	//decoding DOUBLEVEC
		{																		//TODO untested since no DOUBLE_VECs available
			for(j=0; j<4; j++)
				((char*) veclength)[3-j] = xdr[48+j];

			*reply = malloc(*veclength*8);
			for (j = 0; j < *veclength; j++)
			{
				for (i = 7; i >= 0; i--)
					((char*) *reply)[j*8+7-i] = xdr[52+j*8+i];
			}
			//decode TimeStamps seconds
			if(repTsSecs)
			{
				for (i = 3; i >= 0; i--)
					temp[3-i] = xdr[56+*veclength*8+i];
				memcpy(repTsSecs, temp, 4);
			}
			//decode TimeStamps microseconds
			if(repTsUsecs)
			{
				for (i = 3; i >= 0; i--)
					temp[3-i] = xdr[60+*veclength*8+i];
				memcpy(repTsUsecs, temp, 4);
			}
			//decode state
			if(repState)
			{
				for (i = 3; i >= 0; i--)
					temp[3-i] = xdr[64+*veclength*8+i];
				memcpy(repState, temp, 4);
			}
		}
		else if((vartype & OV_VT_KSMASK) == type)		//decoding INTVEC, UINTVEC, SINGLEVEC and BOOLVEC
		{												//TODO tested just for BOOLVEC
			for(j=0; j<4; j++)
				((char*) veclength)[3-j] = xdr[48+j];

			*reply = malloc(*veclength*4);
			for (j = 0; j < *veclength; j++)
			{
				for (i = 3; i >= 0; i--)
					((char*) *reply)[j*4+3-i] = xdr[52+j*4+i];
			}
			//decode TimeStamps seconds
			if(repTsSecs)
			{
				for (i = 3; i >= 0; i--)
					temp[3-i] = xdr[56+*veclength*4+i];
				memcpy(repTsSecs, temp, 4);
			}
			//decode TimeStamps microseconds
			if(repTsSecs)
			{
				for (i = 3; i >= 0; i--)
					temp[3-i] = xdr[60+*veclength*4+i];
				memcpy(repTsUsecs, temp, 4);
			}
			//decode state
			if(repState)
			{
				for (i = 3; i >= 0; i--)
					temp[3-i] = xdr[64+*veclength*4+i];
				memcpy(repState, temp, 4);
			}
		}
		else
		{
			logfile_error("\nvartype mismatch\n");
			return KS_ERR_BADTYPE;
		}

		return KS_ERR_OK;

	}
	else //bad luck - no result
	{
		errorcode = xdr[xdrlength-1];
		switch (errorcode)
		{
		case KS_ERR_BADPATH:
			logfile_error("\nwrong path\n");
			break;
		case KS_ERR_NOACCESS:
			logfile_error("\nno access\n");
			break;
		default:
			logfile_error("\nERROR\n");
		}
	}


	return errorcode;
}

/**
 * Analyses SETVAR and DELETEOBJECT, CREATEOBJECT,RENAMEOBJECT,LINKOBJECT, UNLINKOBJECT replies
 * return value is errorcode
 * input variables are:
 * char xdr[] : received xdr which should be analysed
 * int xdrlength : length of xdr
 */
int analyseresultonlyreply(char xdr[], int xdrlength)
{
	int errorcode;

	errorcode = xdr[39];
	switch (errorcode)
	{
	case KS_ERR_OK:
		return errorcode;
	case KS_ERR_GENERIC:
		logfile_error("\nKS_ERR_GENERIC received\n");
		return errorcode;
	case KS_ERR_TARGETGENERIC:
		logfile_error("\nKS_ERR_TARGETGENERIC received\n");
		return errorcode;
	case KS_ERR_BADAUTH:
		logfile_error("\nKS_ERR_BADAUTH received\n");
		return errorcode;
	case KS_ERR_UNKNOWNAUTH:
		logfile_error("\nKS_ERR_UNKNOWNAUTH received\n");
		return errorcode;
	case KS_ERR_NOTIMPLEMENTED:
		logfile_error("\nKS_ERR_NOTIMPLEMENTED received\n");
		return errorcode;
	case KS_ERR_BADPARAM:
		logfile_error("\nKS_ERR_BADPARAM received\n");
		return errorcode;
	case KS_ERR_BADOBJTYPE:
		logfile_error("\nKS_ERR_BADOBJTYPE received\n");
		return errorcode;
	case KS_ERR_BADNAME:
		logfile_error("\nKS_ERR_BADNAME received\n");
		return errorcode;
	case KS_ERR_BADPATH:
		logfile_error("\nKS_ERR_BADPATH received\n");
		return errorcode;
	case KS_ERR_BADMASK:
		logfile_error("\nKS_ERR_BADMASK received\n");
		return errorcode;
	case KS_ERR_NOACCESS:
		logfile_error("\nKS_ERR_NOACCESS received\n");
		return errorcode;
	case KS_ERR_BADTYPE:
		logfile_error("\nKS_ERR_BADTYPE received\n");
		return errorcode;
	case KS_ERR_CANTSYNC:
		logfile_error("\nKS_ERR_CANTSYNC received\n");
		return errorcode;
	case KS_ERR_BADSELECTOR:
		logfile_error("\nKS_ERR_BADSELECTOR received\n");
		return errorcode;
	case KS_ERR_BADVALUE:
		logfile_error("\nKS_ERR_BADVALUE received\n");
		return errorcode;
	case KS_ERR_NOREMOTE:
		logfile_error("\nKS_ERR_NOREMOTE received\n");
		return errorcode;
	case KS_ERR_SERVERUNKNOWN:
		logfile_error("\nKS_ERR_SERVERUNKNOWN received\n");
		return errorcode;
	case KS_ERR_BADFACTORY:
		logfile_error("\nKS_ERR_BADFACTORY received\n");
		return errorcode;
	case KS_ERR_ALREADYEXISTS:
		logfile_error("\nKS_ERR_ALREADYEXISTS received\n");
		return errorcode;
	case KS_ERR_BADINITPARAM:
		logfile_error("\nKS_ERR_BADINITPARAM received\n");
		return errorcode;
	case KS_ERR_BADPLACEMENT:
		logfile_error("\nKS_ERR_BADPLACEMENT received\n");
		return errorcode;
	case KS_ERR_CANTMOVE:
		logfile_error("\nKS_ERR_CANTMOVE received\n");
		return errorcode;

	default:
		logfile_error("\nunknown errorcode received: %d\n", errorcode);
		return errorcode;
	}


	return errorcode;
}

void generateheader(int procedureNumber, char *xdr[], int *length)
{
	int c;
	char xdrdata[48];

	//make header
	xdrdata[0] = 61;				//xid
	xdrdata[1] = 50;				//xid
	xdrdata[2] = -13;			//xid
	xdrdata[3] = 74;				//xid

	for (c=4; c<11; c++)
		xdrdata[c] = 0;
	xdrdata[11] = 2;
	xdrdata[12] = 0;
	xdrdata[13] = 4;				//prog.-nr.
	xdrdata[14] = -106;		//prog.-nr.
	xdrdata[15] = 120;			//prog.-nr.
	for (c=16; c<19; c++)
		xdrdata[c] = 0;
	xdrdata[19] = 2;				//verion-nr.
	for (c=0; c<4; c++)
			xdrdata[20+c] = ((char*)(&procedureNumber))[3-c];

	for (c=24; c<46; c++)
		xdrdata[c] = 0;
	xdrdata[46] = 0;
	xdrdata[47] = 1;

	*xdr = (char*)malloc(52*sizeof(char));		//rpcheader (4bytes) + xdrheader
	memcpy(*xdr+4, xdrdata, 48);				//rpcheader (first 4 bytes) is set later
	*length = 52;

	return;
}

void addrpcheader(char *xdr[], int *length)
{
	char *tmp;
	int c;
	int xdrlength = *length - 4;

	//make rpcheader
	tmp = (char*)&(xdrlength);
	for (c=0; c<4; c++)
		(*xdr)[3-c] = tmp[c];		//memory was allocated during generate*header()
	(*xdr)[0] = 0x80;


	return;
}



void generatesetbody(int type, char *xdr[], int *length, char *path, void* setvalue, unsigned long varTsSecs, unsigned long varTsUsecs, unsigned long varQState)
{
	int pathlength;
	int i;
	int strlength;
	int xdrposition;
	int setstringlength;

	pathlength = strlen(path);
	strlength = pathlength;
	while(strlength%4)		//make length%4 == 0 for allocation
		strlength++;

	xdrposition = *length;

	switch(type)
	{
	case OV_VT_STRING:
		setstringlength = strlen((*(char**)setvalue));
		*length += 4+strlength+24+setstringlength;		//4 = length of path; 24 = setvalue-encoding
		while(*length%4)
			(*length)++;
		*xdr = realloc(*xdr, *length);
		memset(&((*xdr)[xdrposition]), 0, (*length)-xdrposition);
		for(i=0; i<4; i++)
			(*xdr)[xdrposition+i] = ((char*)&pathlength)[3-i];
		memcpy(&((*xdr)[xdrposition+4]), path, pathlength);
		(*xdr)[xdrposition+strlength+4+3] = 0x2;		//whatever 0x2 stands for^^
		for(i=0; i<4; i++)
			(*xdr)[xdrposition+strlength+8+i] = ((char*)&type)[3-i];		//set vartype
		for(i=0; i<4; i++)
			(*xdr)[xdrposition+strlength+12+i] = ((char*)&setstringlength)[3-i];		//set stringlength
		memcpy(&((*xdr)[xdrposition+strlength+16]), (*(char**)setvalue), setstringlength);		//set value
		xdrposition+=strlength+16+setstringlength;
		while(xdrposition%4)
			xdrposition++;
		break;
	case OV_VT_DOUBLE:
		*length += 4+strlength+28;		//4 = length of path; 28 = setvalue-encoding
		*xdr = realloc(*xdr, *length);
		memset(&((*xdr)[xdrposition]), 0, 4+strlength+28);
		for(i=0; i<4; i++)
			(*xdr)[xdrposition+i] = ((char*)&pathlength)[3-i];
		memcpy(&((*xdr)[xdrposition+4]), path, pathlength);
		(*xdr)[xdrposition+strlength+4+3] = 0x2;		//whatever 0x2 stands for^^
		for(i=0; i<4; i++)
			(*xdr)[xdrposition+strlength+8+i] = ((char*)&type)[3-i];		//set vartype
		for(i=0; i<8; i++)
			(*xdr)[xdrposition+strlength+12+i] = ((char*)setvalue)[7-i];		//set value
		xdrposition+=strlength+20;
		break;
	default:
		*length += 4+strlength+24;		//4 = length of path; 24 = setvalue-encoding
		*xdr = realloc(*xdr, *length);
		memset(&((*xdr)[xdrposition]), 0, 4+strlength+24);
		for(i=0; i<4; i++)
			(*xdr)[xdrposition+i] = ((char*)&pathlength)[3-i];
		memcpy(&((*xdr)[xdrposition+4]), path, pathlength);
		(*xdr)[xdrposition+strlength+4+3] = 0x2;		//whatever 0x2 stands for^^
		for(i=0; i<4; i++)
			(*xdr)[xdrposition+strlength+8+i] = ((char*)&type)[3-i];		//set vartype
		for(i=0; i<4; i++)
			(*xdr)[xdrposition+strlength+12+i] = ((char*)setvalue)[3-i];		//set value
		xdrposition+=strlength+16;
		break;

	}

	//set Time-Stamps seconds
	for(i=0; i<4; i++)
		(*xdr)[xdrposition+i] = ((char*)&varTsSecs)[3-i];
	xdrposition += 4;
	//set Time-Stamps useconds
	for(i=0; i<4; i++)
		(*xdr)[xdrposition+i] = ((char*)&varTsUsecs)[3-i];
	xdrposition += 4;
	//set quality state
	for(i=0; i<4; i++)
		(*xdr)[xdrposition+i] = ((char*)&varQState)[3-i];
	xdrposition += 4;
	//set xdr end
	//(*xdr)[*length-1] = 0x04;					//whatever 0x04 stands for^^
	return;
}


void generatesetvecbody(int type, char *xdr[], int *length, char *path, void* setvalue, int veclength,
		unsigned long varTsSecs, unsigned long varTsUsecs, unsigned long varQState)
{
	int pathlength;
	int i, j;
	int strlength;
	int xdrposition;
	int setstringlength = 0;

	pathlength = strlen(path);
	strlength = pathlength;
	while(strlength%4)		//make length%4 == 0 for allocation
		strlength++;

	xdrposition = *length;

	switch(type)
	{


	case OV_VT_DOUBLE_VEC:
		*length += 4+strlength+24+8*veclength;		//4 = length of path; 24 = setvalue-encoding + 8* veclen
		*xdr = realloc(*xdr, *length);
		memset(&((*xdr)[xdrposition]), 0, 4+strlength+24);
		for(i=0; i<4; i++)
			(*xdr)[xdrposition+i] = ((char*)&pathlength)[3-i];
		memcpy(&((*xdr)[xdrposition+4]), path, pathlength);
		(*xdr)[xdrposition+strlength+4+3] = 0x2;		//whatever 0x2 stands for^^
		for(i=0; i<4; i++)
			(*xdr)[xdrposition+strlength+8+i] = ((char*)&type)[3-i];		//set vartype
		for(i=0; i<4; i++)
			(*xdr)[xdrposition+strlength+12+i] = ((char*)(&veclength))[3-i];		//set veclength
		for(i=0; i<veclength; i++)
		{
			for(j=0; j<8; j++)
				(*xdr)[xdrposition+strlength+16+i*8+j] = ((char*)(&((int**)setvalue)[i]))[7-j];		//set values
		}
		xdrposition+=strlength+veclength*8+16;
		break;
	case OV_VT_STRING_VEC:
		for(i=0; i< veclength; i++)		//calculating overall size
		{
			setstringlength += strlen((*(char***)setvalue)[i]);
			while(setstringlength%4)
				setstringlength++;
		}
		*length += 4+strlength+24+4*veclength+setstringlength;		//4 = length of path; 24 = setvalue-encoding + 4* veclen (lwength of each string) + setstringlength (all strings)
		*xdr = realloc(*xdr, *length);
		memset(&((*xdr)[xdrposition]), 0, *length-xdrposition);
		for(i=0; i<4; i++)
			(*xdr)[xdrposition+i] = ((char*)&pathlength)[3-i];
		memcpy(&((*xdr)[xdrposition+4]), path, pathlength);
		(*xdr)[xdrposition+strlength+4+3] = 0x2;		//whatever 0x2 stands for^^
		for(i=0; i<4; i++)
			(*xdr)[xdrposition+strlength+8+i] = ((char*)&type)[3-i];		//set vartype
		for(i=0; i<4; i++)
			(*xdr)[xdrposition+strlength+12+i] = ((char*)(&veclength))[3-i];		//set veclength
		xdrposition += strlength + 16;						//xdrposition is incremented from now on
		for(i=0; i<veclength; i++)
		{
			strlength = strlen((*(char***)setvalue)[i]);
			for(j=0; j<4; j++)
				(*xdr)[xdrposition+j] = ((char*)(&strlength))[3-j];		//set length of current string
			memcpy(&((*xdr)[xdrposition+4]), (*(char***)setvalue)[i], strlength);
			xdrposition+=4+strlength;
			while(xdrposition%4)
				xdrposition++;
		}
		break;
	default:
		*length += 4+strlength+24+4*veclength;		//4 = length of path; 24 = setvalue-encoding + 4* veclen
		*xdr = realloc(*xdr, *length);
		memset(&((*xdr)[xdrposition]), 0, 4+strlength+24);
		for(i=0; i<4; i++)
			(*xdr)[xdrposition+i] = ((char*)&pathlength)[3-i];
		memcpy(&((*xdr)[xdrposition+4]), path, pathlength);
		(*xdr)[xdrposition+strlength+4+3] = 0x2;		//whatever 0x2 stands for^^
		for(i=0; i<4; i++)
			(*xdr)[xdrposition+strlength+8+i] = ((char*)&type)[3-i];		//set vartype
		for(i=0; i<4; i++)
			(*xdr)[xdrposition+strlength+12+i] = ((char*)(&veclength))[3-i];		//set veclength
		for(i=0; i<veclength; i++)
		{
			for(j=0; j<4; j++)
				(*xdr)[xdrposition+strlength+16+i*4+j] = ((char*)(&((int**)setvalue)[i]))[3-j];		//set values
		}
		xdrposition+=strlength+veclength*4+16;
		break;

	}

	//set Time-Stamps seconds
	for(i=0; i<4; i++)
		(*xdr)[xdrposition+i] = ((char*)&varTsSecs)[3-i];
	xdrposition += 4;
	//set Time-Stamps useconds
	for(i=0; i<4; i++)
		(*xdr)[xdrposition+i] = ((char*)&varTsUsecs)[3-i];
	xdrposition += 4;
	//set quality state
	for(i=0; i<4; i++)
		(*xdr)[xdrposition+i] = ((char*)&varQState)[3-i];
	xdrposition += 4;

	//set xdr end
	//(*xdr)[*length-1] = 0x4;					//whatever 0x04 stands for^^
	return;
}



void generategetbody(char *xdr[], int *length, char *path)
{
	char *temp = (char*)malloc(*length*sizeof(char));
	int xdrlength;
	int c;

	memcpy(temp, *xdr, *length);	//save first part of xdr

	//calc xdrlength
	xdrlength = strlen(path);
	while ((xdrlength%4)!=0)
		xdrlength++;

	*xdr = (char*)realloc(*xdr, (*length+xdrlength+4)*sizeof(char));
	memcpy(*xdr, temp, *length);		//copy in first part of xdr

	for (c=0; c<4; c++)
		(*xdr)[*length+3-c] = ((char*)&xdrlength)[c];		//copy in xdrlength (length of path)

	memcpy(*xdr+*length+4, path, xdrlength);		//copy in path
	*length = *length+xdrlength+4;

	free(temp);
	return;
}

