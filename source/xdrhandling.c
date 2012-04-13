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
		logfile_debug("ab %d: %d\n", i, temp, &temp);
		logfile_debug("%c%c%c%c", xdr[i], xdr[i+1], xdr[i+2], xdr[i+3]);

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
	generategetheader(xdr, length);
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
void generatesetstringxdr(char *xdr[], int *length, char *path, char *setstring)
{
	generatesetheader(xdr, length);
	generatesetstringbody(xdr, length, path, setstring);
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
void generatesetstringvecxdr(char *xdr[], int *length, char *path, char **setstringvec, int veclength)
{
	generatesetheader(xdr, length);
	generatesetstringvecbody(xdr, length, path, setstringvec, veclength);
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
void generatesetboolxdr(char *xdr[], int *length, char *path, int setbool)
{
	generatesetheader(xdr, length);
	generatesetbody(OV_VT_BOOL, xdr, length, path, &setbool);
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
void generatesetboolvecxdr(char *xdr[], int *length, char *path, int setboolvec[], int veclength)
{
	generatesetheader(xdr, length);
	generatesetboolvecbody(xdr, length, path, setboolvec, veclength);
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
void generatesetdoublexdr(char *xdr[], int *length, char *path, double setdouble)
{
	generatesetheader(xdr, length);
	generatesetbody(OV_VT_DOUBLE, xdr, length, path, &setdouble);
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
void generatesetdoublevecxdr(char *xdr[], int *length, char *path, double setdoublevec[], int veclength)
{
	generatesetheader(xdr, length);
	generatesetdoublevecbody(xdr, length, path, setdoublevec, 5);
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
void generatesetintxdr(char *xdr[], int *length, char *path, int setint)
{
	generatesetheader(xdr, length);
	generatesetbody(OV_VT_INT, xdr, length, path, &setint);
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
void generatesetintvecxdr(char *xdr[], int *length, char *path, int setintvec[], int veclength)
{
	generatesetheader(xdr, length);
	generatesetintvecbody(xdr, length, path, setintvec, veclength);
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
void generatesetsinglexdr(char *xdr[], int *length, char *path, float setsingle)
{
	generatesetheader(xdr, length);
	generatesetbody(OV_VT_SINGLE, xdr, length, path, &setsingle);
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
void generatesetsinglevecxdr(char *xdr[], int *length, char *path, float setsinglevec[], int veclength)
{
	char xdrdata[4096];
	char header[48];
	int c, d;
	char rpcheader[4];
	int pathlength, xdrlength;
	char temp[4096];
	char *tmp;
	char *buf;

	memset(xdrdata, 0, 4096);

	//set xdrlenght
	pathlength = strlen(path);
	while ((pathlength%4) != 0)
		pathlength++;
	xdrlength = pathlength+76+4*veclength;

	//make header
	header[0] = 61;				//xid
	header[1] = 50;				//xid
	header[2] = -13;			//xid
	header[3] = 74;				//xid
	for (c=4; c<11; c++)
		header[c] = 0;
	header[11] = 2;
	header[12] = 0;
	header[13] = 4;				//prog.-nr.
	header[14] = -106;		//prog.-nr.
	header[15] = 120;			//prog.-nr.
	for (c=16; c<19; c++)
		header[c] = 0;
	header[19] = 2;				//verion-nr.
	for (c=24; c<46; c++)
		header[c] = 0;
	header[46] = 0;
	header[47] = 1;

	//copy header to xdr
	memset(xdrdata, 0, 4096);
	memcpy(xdrdata, header, 48);


	//set procedure
	xdrdata[20] = 0;
	xdrdata[21] = 0;
	xdrdata[22] = 1;
	xdrdata[23] = 2;

	//set path
	xdrdata[51] = strlen(path);
	for (c=52; c<(strlen(path)+52); c++)
		xdrdata[c] = path[c-52];
	for (c=(strlen(path)+52); c<xdrlength; c++)
		xdrdata[c] = 0;

	//add rpcheader
	tmp = (char*)&(xdrlength);
	for (c=0; c<4; c++)
		rpcheader[3-c] = tmp[c];
	rpcheader[0] = 0x80;

	memset (temp,'\0',xdrlength+5);
	memcpy(temp, rpcheader, 4);
	for (c = 0; c < xdrlength; c++)
		temp[4+c] = xdrdata[c];
	xdrlength = xdrlength+4;
	assert(xdrlength+4 < 4096);
	memcpy(xdrdata, temp, xdrlength);

	//make xdr end
	xdrdata[59+pathlength] = 0x2;
	xdrdata[xdrlength-12] = 0;
	xdrdata[xdrlength-11] = 0;
	xdrdata[xdrlength-10] = 0;
	xdrdata[xdrlength-9] = 0;
	xdrdata[xdrlength-8] = 0;
	xdrdata[xdrlength-7] = 0;
	xdrdata[xdrlength-6] = 0;
	xdrdata[xdrlength-5] = 0;
	xdrdata[xdrlength-4] = 0;
	xdrdata[xdrlength-3] = 0;
	xdrdata[xdrlength-2] = 0;
	xdrdata[xdrlength-1] = 0x4;

	//make specific part of xdr
	xdrdata[63+pathlength] = -96;
	xdrdata[67+pathlength] = veclength;
	for (c=0; c<veclength; c++)
		for (d=0; d<4; d++)
		{
			buf = (char*)&setsinglevec[c];
			//~ memcpy(tc[c], buf, 4);
			xdrdata[71+pathlength+4*c-d] = buf[d];
		}

	*xdr = (char*)malloc(xdrlength*sizeof(char));
	memset(*xdr, 1, xdrlength);
	memcpy(*xdr, xdrdata, xdrlength);
	*length = xdrlength;


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
void generatesetuintxdr(char *xdr[], int *length, char *path, unsigned int setuint)
{
	generatesetheader(xdr, length);
	generatesetbody(OV_VT_UINT, xdr, length, path, &setuint);
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
void generatesetuintvecxdr(char *xdr[], int *length, char *path, unsigned int setuintvec[], int veclength)
{
	generatesetheader(xdr, length);
	generatesetuintvecbody(xdr, length, path, setuintvec, veclength);
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
	char xdrdata[4096];
	char header[48];
	int c;
	char rpcheader[4];
	int pathlength, xdrlength;
	char temp[4096];
	char *tmp;

	memset(xdrdata, 0, 4096);	

	//set pathlength
	pathlength = strlen(path);
	while ((pathlength%4) != 0)
		pathlength++;

	//set xdrlenght
	xdrlength = pathlength+52;

	//make header
	header[0] = 61;				//xid
	header[1] = 50;				//xid
	header[2] = -13;			//xid
	header[3] = 74;				//xid
	for (c=4; c<11; c++)
		header[c] = 0;
	header[11] = 2;
	header[12] = 0;
	header[13] = 4;				//prog.-nr.
	header[14] = -106;		//prog.-nr.
	header[15] = 120;			//prog.-nr.
	for (c=16; c<19; c++)
		header[c] = 0;
	header[19] = 2;				//verion-nr.
	for (c=24; c<46; c++)
		header[c] = 0;
	header[46] = 0;
	header[47] = 1;

	//copy header to xdr
	memcpy(xdrdata, header, 48);


	//set procedure
	xdrdata[20] = 0;
	xdrdata[21] = 0;
	xdrdata[22] = 2;
	xdrdata[23] = 2;

	//set path
	xdrdata[51] = strlen(path);
	for (c=0; c<(strlen(path)); c++)
		xdrdata[52+c] = path[c];
	for (c=(strlen(path)); c<xdrlength-52; c++)
		xdrdata[52+c] = 0;

	//add rpcheader
	tmp = (char*)&(xdrlength);
	for (c=0; c<4; c++)
		rpcheader[3-c] = tmp[c];
	rpcheader[0] = 0x80;

	memset (temp,'\0',xdrlength+5);
	memcpy(temp, rpcheader, 4);
	for (c = 0; c < xdrlength; c++)
		temp[4+c] = xdrdata[c];
	xdrlength = xdrlength+4;
	assert(xdrlength+4 < 4096);
	memcpy(xdrdata, temp, xdrlength);

	*xdr = (char*)malloc(xdrlength*sizeof(char));
	memset(*xdr, 1, xdrlength);
	memcpy(*xdr, xdrdata, xdrlength);
	*length = xdrlength;


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
	char xdrdata[4096];
	char header[48];
	int c;
	char rpcheader[4];
	char *tmp;
	int librarypathlength, pathlength, refpathlength, xdrlength;
	char temp[4096];

	memset(xdrdata, 0, 4096);	

	//set librarypathlength
	librarypathlength = strlen(librarypath);
	while ((librarypathlength%4) != 0)
		librarypathlength++;

	//set pathlength
	pathlength = strlen(path);
	while ((pathlength%4) != 0)
		pathlength++;

	//set refpathlength
	if ((position == 3) | (position == 4))
	{
		refpathlength = strlen(refpath);
		while ((refpathlength%4) != 0)
			refpathlength++;
	}

	//set xdrlenght
	if ((position == 3) | (position == 4))
		xdrlength = librarypathlength+pathlength+refpathlength+72;
	else
		xdrlength = librarypathlength+pathlength+68;

	//make header
	header[0] = 61;				//xid
	header[1] = 50;				//xid
	header[2] = -13;			//xid
	header[3] = 74;				//xid
	for (c=4; c<11; c++)
		header[c] = 0;
	header[11] = 2;
	header[12] = 0;
	header[13] = 4;				//prog.-nr.
	header[14] = -106;		//prog.-nr.
	header[15] = 120;			//prog.-nr.
	for (c=16; c<19; c++)
		header[c] = 0;
	header[19] = 2;				//verion-nr.
	for (c=24; c<46; c++)
		header[c] = 0;
	header[46] = 0;
	header[47] = 1;

	//copy header to xdr
	memcpy(xdrdata, header, 48);


	//set procedure
	xdrdata[20] = 0;
	xdrdata[21] = 0;
	xdrdata[22] = 2;
	xdrdata[23] = 1;

	//set librarypath
	xdrdata[51] = strlen(librarypath);
	for (c=0; c<(strlen(librarypath)); c++)
		xdrdata[52+c] = librarypath[c];
	for (c=(strlen(librarypath)); c<xdrlength-52; c++)
		xdrdata[52+c] = 0;

	//set path
	xdrdata[51+librarypathlength+4] = strlen(path);
	for (c=0; c<strlen(path); c++)
		xdrdata[51+librarypathlength+5+c] = path[c];
	for (c=strlen(path); c<xdrlength-(51+librarypathlength+5); c++)
		xdrdata[51+librarypathlength+5+c] = 0;

	//set position
	xdrdata[51+librarypathlength+4+pathlength+4] = position;

	//set refpath
	if ((position == 3) | (position == 4))
	{
		xdrdata[51+librarypathlength+4+pathlength+8] = strlen(refpath);
		for (c=0; c<strlen(refpath); c++)
			xdrdata[51+librarypathlength+4+pathlength+9+c] = refpath[c];
		for (c=strlen(refpath); c<xdrlength-(51+librarypathlength+4+pathlength+9); c++)
			xdrdata[51+librarypathlength+4+pathlength+9+c] = 0;
	}	

	//add rpcheader
	tmp = (char*)&(xdrlength);
	for (c=0; c<4; c++)
		rpcheader[3-c] = tmp[c];
	rpcheader[0] = 0x80;

	memset (temp,'\0',xdrlength+5);
	memcpy(temp, rpcheader, 4);
	for (c = 0; c < xdrlength; c++)
		temp[4+c] = xdrdata[c];
	xdrlength = xdrlength+4;
	assert(xdrlength+4 < 4096);
	memcpy(xdrdata, temp, xdrlength);

	*xdr = (char*)malloc(xdrlength*sizeof(char));
	memset(*xdr, 1, xdrlength);
	memcpy(*xdr, xdrdata, xdrlength);
	*length = xdrlength;


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
	char xdrdata[4096];
	char header[48];
	int c;
	char rpcheader[4];
	int oldpathlength, newpathlength, xdrlength;
	char temp[4096];
	char *tmp;

	memset(xdrdata, 0, 4096);	

	//set oldpathlength
	oldpathlength = strlen(oldpath);
	while ((oldpathlength%4) != 0)
		oldpathlength++;

	//set namelength
	newpathlength = strlen(newpath);
	while ((newpathlength%4) != 0)
		newpathlength++;

	//set xdrlenght
	xdrlength = oldpathlength+newpathlength+60;

	//make header
	header[0] = 61;				//xid
	header[1] = 50;				//xid
	header[2] = -13;			//xid
	header[3] = 74;				//xid
	for (c=4; c<11; c++)
		header[c] = 0;
	header[11] = 2;
	header[12] = 0;
	header[13] = 4;				//prog.-nr.
	header[14] = -106;		//prog.-nr.
	header[15] = 120;			//prog.-nr.
	for (c=16; c<19; c++)
		header[c] = 0;
	header[19] = 2;				//verion-nr.
	for (c=24; c<46; c++)
		header[c] = 0;
	header[46] = 0;
	header[47] = 1;

	//copy header to xdr
	memcpy(xdrdata, header, 48);


	//set procedure
	xdrdata[20] = 0;
	xdrdata[21] = 0;
	xdrdata[22] = 2;
	xdrdata[23] = 3;

	//set oldpath
	xdrdata[51] = strlen(oldpath);
	for (c=0; c<(strlen(oldpath)); c++)
		xdrdata[52+c] = oldpath[c];
	for (c=(strlen(oldpath)); c<xdrlength-52; c++)
		xdrdata[52+c] = 0;

	//set newpath
	xdrdata[51+oldpathlength+4] = strlen(newpath);
	for (c=0; c<strlen(newpath); c++)
		xdrdata[51+oldpathlength+5+c] = newpath[c];
	for (c=strlen(newpath); c<xdrlength-52; c++)
		xdrdata[51+oldpathlength+5+c] = 0;

	//add rpcheader
	tmp = (char*)&(xdrlength);
	for (c=0; c<4; c++)
		rpcheader[3-c] = tmp[c];
	rpcheader[0] = 0x80;

	memset (temp,'\0',xdrlength+5);
	memcpy(temp, rpcheader, 4);
	temp[3] = xdrlength;
	for (c = 0; c < xdrlength; c++)
		temp[4+c] = xdrdata[c];
	xdrlength = xdrlength+4;
	assert(xdrlength+4 < 4096);
	memcpy(xdrdata, temp, xdrlength);

	*xdr = (char*)malloc(xdrlength*sizeof(char));
	memset(*xdr, 1, xdrlength);
	memcpy(*xdr, xdrdata, xdrlength);
	*length = xdrlength;


	return;
}

/**
 * Generates the xdr for UNLINKOBJECT
 * input variables are:
 * char *xdr[] : here the generated xdr is stored
 * int *length : here the length of the xdr is stored
 * char *path : path to the object which should be unlinked
 * char *linkedpath : path of the object which is linked with selected object
 */
void generateunlinkobjectxdr(char *xdr[], int *length, char *path, char *linkedpath)
{
	char xdrdata[4096];
	char header[48];
	int c;
	char rpcheader[4];
	int pathlength, linkedpathlength, xdrlength;
	char temp[4096];
	char *tmp;

	memset(xdrdata, 0, 4096);	

	//set linkedpathlength
	linkedpathlength = strlen(linkedpath);
	while ((linkedpathlength%4) != 0)
		linkedpathlength++;

	//set pathlength
	pathlength = strlen(path);
	while ((pathlength%4) != 0)
		pathlength++;

	//set xdrlenght
	xdrlength = linkedpathlength+pathlength+64;

	//make header
	header[0] = 61;				//xid
	header[1] = 50;				//xid
	header[2] = -13;			//xid
	header[3] = 74;				//xid
	for (c=4; c<11; c++)
		header[c] = 0;
	header[11] = 2;
	header[12] = 0;
	header[13] = 4;				//prog.-nr.
	header[14] = -106;		//prog.-nr.
	header[15] = 120;			//prog.-nr.
	for (c=16; c<19; c++)
		header[c] = 0;
	header[19] = 2;				//verion-nr.
	for (c=24; c<46; c++)
		header[c] = 0;
	header[46] = 0;
	header[47] = 1;

	//copy header to xdr
	memcpy(xdrdata, header, 48);


	//set procedure
	xdrdata[20] = 0;
	xdrdata[21] = 0;
	xdrdata[22] = 3;
	xdrdata[23] = 2;

	//set path
	xdrdata[51] = strlen(path);
	for (c=0; c<strlen(path); c++)
		xdrdata[52+c] = path[c];
	for (c=strlen(path); c<xdrlength-52; c++)
		xdrdata[52+c] = 0;

	//set linkedpath
	xdrdata[51+pathlength+4] = strlen(linkedpath);
	for (c=0; c<(strlen(linkedpath)); c++)
		xdrdata[52+pathlength+4+c] = linkedpath[c];
	for (c=(strlen(linkedpath)); c<xdrlength-(52+pathlength+4); c++)
		xdrdata[52+pathlength+4+c] = 0;

	//add rpcheader
	tmp = (char*)&(xdrlength);
	for (c=0; c<4; c++)
		rpcheader[3-c] = tmp[c];
	rpcheader[0] = 0x80;

	memset (temp,'\0',xdrlength+5);
	memcpy(temp, rpcheader, 4);
	temp[3] = xdrlength;
	for (c = 0; c < xdrlength; c++)
		temp[4+c] = xdrdata[c];
	xdrlength = xdrlength+4;
	assert(xdrlength+4 < 4096);
	memcpy(xdrdata, temp, xdrlength);

	*xdr = (char*)malloc(xdrlength*sizeof(char));
	memset(*xdr, 1, xdrlength);
	memcpy(*xdr, xdrdata, xdrlength);
	*length = xdrlength;


	return;
}

/**
 * Generates the xdr for LINKOBJECT
 * input variables are:
 * char *xdr[] : here the generated xdr is stored
 * int *length : here the length of the xdr is stored
 * char *path : path to the object which should be linked
 * char *linkedpath : path of the object which should be linked with selected object
 * int position : position to place the object in the hierarchy
 * 					(0: default, 1: begin, 2: end, 3: before, 4: after)
 * char *refpath : path to object before or after refers to
 */
void generatelinkobjectxdr(char *xdr[], int *length, char *path, char *linkedpath, int position, char *refpath)
{
	char xdrdata[4096];
	char header[48];
	int c;
	char rpcheader[4];
	int pathlength, linkedpathlength, refpathlength, xdrlength;
	char temp[4096];
	char *tmp;

	memset(xdrdata, 0, 4096);	

	//set linkedpathlength
	linkedpathlength = strlen(linkedpath);
	while ((linkedpathlength%4) != 0)
		linkedpathlength++;

	//set pathlength
	pathlength = strlen(path);
	while ((pathlength%4) != 0)
		pathlength++;

	//set refpathlength
	if ((position == 3) | (position == 4))
	{
		refpathlength = strlen(refpath);
		while ((refpathlength%4) != 0)
			refpathlength++;
	}

	//set xdrlenght
	if ((position == 3) | (position == 4))
		xdrlength = linkedpathlength+pathlength+refpathlength+68;
	else
		xdrlength = linkedpathlength+pathlength+64;

	//make header
	header[0] = 61;				//xid
	header[1] = 50;				//xid
	header[2] = -13;			//xid
	header[3] = 74;				//xid
	for (c=4; c<11; c++)
		header[c] = 0;
	header[11] = 2;
	header[12] = 0;
	header[13] = 4;				//prog.-nr.
	header[14] = -106;		//prog.-nr.
	header[15] = 120;			//prog.-nr.
	for (c=16; c<19; c++)
		header[c] = 0;
	header[19] = 2;				//verion-nr.
	for (c=24; c<46; c++)
		header[c] = 0;
	header[46] = 0;
	header[47] = 1;

	//copy header to xdr
	memcpy(xdrdata, header, 48);


	//set procedure
	xdrdata[20] = 0;
	xdrdata[21] = 0;
	xdrdata[22] = 3;
	xdrdata[23] = 1;

	//set path
	xdrdata[51] = strlen(path);
	for (c=0; c<strlen(path); c++)
		xdrdata[52+c] = path[c];
	for (c=strlen(path); c<xdrlength-52; c++)
		xdrdata[52+c] = 0;

	//set elementpath
	xdrdata[51+pathlength+4] = strlen(linkedpath);
	for (c=0; c<(strlen(linkedpath)); c++)
		xdrdata[52+pathlength+4+c] = linkedpath[c];
	for (c=(strlen(linkedpath)); c<xdrlength-(52+pathlength+4); c++)
		xdrdata[52+pathlength+4+c] = 0;

	//set position
	xdrdata[51+pathlength+4+linkedpathlength+4] = position;

	//set place
	if ((position == 3) | (position == 4))
	{
		xdrdata[51+pathlength+4+linkedpathlength+8] = strlen(refpath);
		for (c=0; c<strlen(refpath); c++)
			xdrdata[51+pathlength+4+linkedpathlength+9+c] = refpath[c];
		for (c=strlen(refpath); c<xdrlength-(51+pathlength+4+linkedpathlength+9); c++)
			xdrdata[51+pathlength+4+linkedpathlength+9+c] = 0;
	}

	//add rpcheader
	tmp = (char*)&(xdrlength);
	for (c=0; c<4; c++)
		rpcheader[3-c] = tmp[c];
	rpcheader[0] = 0x80;

	memset (temp,'\0',xdrlength+5);
	memcpy(temp, rpcheader, 4);
	temp[3] = xdrlength;
	for (c = 0; c < xdrlength; c++)
		temp[4+c] = xdrdata[c];
	xdrlength = xdrlength+4;
	assert(xdrlength+4 < 4096);
	memcpy(xdrdata, temp, xdrlength);

	*xdr = (char*)malloc(xdrlength*sizeof(char));
	memset(*xdr, 1, xdrlength);
	memcpy(*xdr, xdrdata, xdrlength);
	*length = xdrlength;


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
	char xdrdata[4096];
	char header[48];
	int c;
	char rpcheader[4];
	int pathlength, namemasklength, xdrlength;
	char temp[4096];
	char *tmp;
	char *buf;
	char *buf2;

	memset(xdrdata, 0, 4096);	

	//set pathlength
	pathlength = strlen(path);
	while ((pathlength%4) != 0)
		pathlength++;

	//set namemasklength
	namemasklength = strlen(namemask);
	while ((namemasklength%4) != 0)
		namemasklength++;

	//set xdrlenght
	xdrlength = pathlength+namemasklength+60;

	//make header
	header[0] = 61;				//xid
	header[1] = 50;				//xid
	header[2] = -13;			//xid
	header[3] = 74;				//xid
	for (c=4; c<11; c++)
		header[c] = 0;
	header[11] = 2;
	header[12] = 0;
	header[13] = 4;				//prog.-nr.
	header[14] = -106;		//prog.-nr.
	header[15] = 120;			//prog.-nr.
	for (c=16; c<19; c++)
		header[c] = 0;
	header[19] = 2;				//verion-nr.
	for (c=24; c<46; c++)
		header[c] = 0;
	header[46] = 0;
	header[47] = 1;

	//copy header to xdr
	memcpy(xdrdata, header, 48);


	//set procedure
	xdrdata[20] = 0;
	xdrdata[21] = 0;
	xdrdata[22] = 0;
	xdrdata[23] = 2;

	//set path
	xdrdata[47] = strlen(path);
	for (c=0; c<(strlen(path)); c++)
		xdrdata[48+c] = path[c];
	for (c=(strlen(path)); c<xdrlength-48; c++)
		xdrdata[48+c] = 0;

	//set ksobjecttype
	buf = (char*)&ksobjecttype;
	memcpy(temp, buf, 4);
	for (c=0; c<4; c++)
		xdrdata[51+pathlength-c] = temp[c];

	//set namemask
	xdrdata[55+pathlength] = strlen(namemask);
	for (c=0; c<strlen(namemask); c++)
		xdrdata[56+pathlength+c] = namemask[c];
	for (c=strlen(namemask); c<xdrlength-(56+pathlength+namemasklength); c++)
		xdrdata[56+pathlength+namemasklength+c] = 0;

	//set epflags
	buf2 = (char*)&epflags;
	memcpy(temp, buf2, 4);
	for (c=0; c<4; c++)
		xdrdata[56+pathlength+namemasklength-c] = temp[c];

	//add rpcheader
	tmp = (char*)&(xdrlength);
	for (c=0; c<4; c++)
		rpcheader[3-c] = tmp[c];
	rpcheader[0] = 0x80;

	memset (temp,'\0',xdrlength+5);
	memcpy(temp, rpcheader, 4);
	temp[3] = xdrlength;
	for (c = 0; c < xdrlength; c++)
		temp[4+c] = xdrdata[c];
	xdrlength = xdrlength+4;
	assert(xdrlength+4 < 4096);
	memcpy(xdrdata, temp, xdrlength);

	*xdr = (char*)malloc(xdrlength*sizeof(char));
	memset(*xdr, 1, xdrlength);
	memcpy(*xdr, xdrdata, xdrlength);
	*length = xdrlength;


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
 */
int analysegetreply(int type, char xdr[], int xdrlength, void *reply)
{
	int errorcode, vartype;
	char *str;
	int strlength;
	int i;
	char temp[4];
	char tempd[8];
	int j;

	logfile_debug("\nanalysing get reply\n");

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
			(*(char**) reply)[strlength] = 0;
			free(str);
		}
		else if((vartype & OV_VT_KSMASK) == type && type == OV_VT_DOUBLE)	//decoding DOUBLE
		{
			logfile_debug("\ndecoding DOUBLE\n");
			for (i = 7; i >= 0; i--)
				tempd[7-i] = xdr[48+i];
			memcpy(reply, tempd, 8);
		}
		else if((vartype & OV_VT_KSMASK) == type)		//decoding INT, UINT, SINGLE and BOOL
		{
			logfile_debug("\ndecoding INT, UINT, SINGLE and BOOL\n");
			for (i = 3; i >= 0; i--)
				temp[3-i] = xdr[48+i];
			memcpy(reply, temp, 4);
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
 */
int analysegetvecreply(int type, char xdr[], int xdrlength, void **reply, int *veclength)
{
	int errorcode, vartype = 0;
	int strlength;
	int i;
	int j;
	int xdrposition;

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

void generatesetheader(char *xdr[], int *length)
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
	xdrdata[20] = 0;			//procedure
	xdrdata[21] = 0;			//procedure
	xdrdata[22] = 1;			//procedure
	xdrdata[23] = 2;			//procedure
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

void generatesetboolvecbody(char *xdr[], int *length, char *path, int setboolvec[], int veclength)
{
	int pathlength;
	int xdrlength;
	int c, d;
	char xdrdata[4096];
	char *buf;
	char *temp = (char*)malloc(*length*sizeof(char));

	memset(xdrdata, 0, 4096);
	memset(temp, 0, *length);
	memcpy(temp, *xdr, *length);

	//set xdrlenght
	pathlength = strlen(path);
	while ((pathlength%4) != 0)
		pathlength++;
	xdrlength = 4+pathlength+24+4*veclength;

	//set path
	xdrdata[3] = strlen(path);
	for (c=0; c<strlen(path); c++)
		xdrdata[4+c] = path[c];
	xdrdata[7+pathlength] = 0x2;

	//make specific part of xdr
	xdrdata[11+pathlength] = -126;
	xdrdata[15+pathlength] = veclength;
	for (c=0; c<veclength; c++)
		for (d=0; d<4; d++)
		{
			buf = (char*)&setboolvec[c];
			//~ memcpy(tc[c], buf, 4);
			xdrdata[19+pathlength+4*c-d] = buf[d];
		}

	//make xdr end
	xdrdata[xdrlength-1] = 0x4;

	*xdr = (char*)malloc((*length+xdrlength)*sizeof(char));
	memcpy(*xdr, temp, *length);
	memcpy(*xdr+*length, xdrdata, xdrlength);
	*length = *length+xdrlength;

	return;
}


void generatesetdoublevecbody(char *xdr[], int *length, char *path, double setdoublevec[], int veclength)
{
	int pathlength;
	int xdrlength;
	int c, d;
	char xdrdata[4096];
	char *buf;
	char *temp = (char*)malloc(*length*sizeof(char));

	memset(xdrdata, 0, 4096);
	memset(temp, 0, *length);
	memcpy(temp, *xdr, *length);

	//set xdrlenght
	pathlength = strlen(path);
	while ((pathlength%4) != 0)
		pathlength++;
	xdrlength = 4+pathlength+24+8*veclength;

	//set path
	xdrdata[3] = strlen(path);
	for (c=0; c<strlen(path); c++)
		xdrdata[4+c] = path[c];
	xdrdata[7+pathlength] = 0x2;

	//make specific part of xdr
	xdrdata[11+pathlength] = -95;
	xdrdata[15+pathlength] = veclength;
	for (c=0; c<veclength; c++)
		for (d=0; d<8; d++)
		{
			buf = (char*)&setdoublevec[c];
			//~ memcpy(tc[c], buf, 4);
			xdrdata[23+pathlength+8*c-d] = buf[d];
		}

	//make xdr end
	xdrdata[xdrlength-1] = 0x4;

	*xdr = (char*)malloc((*length+xdrlength)*sizeof(char));
	memcpy(*xdr, temp, *length);
	memcpy(*xdr+*length, xdrdata, xdrlength);
	*length = *length+xdrlength;

	return;
}


void generatesetintvecbody(char *xdr[], int *length, char *path, int setintvec[], int veclength)
{
	int pathlength;
	int xdrlength;
	int c, d;
	char xdrdata[4096];
	char *buf;
	char *temp = (char*)malloc(*length*sizeof(char));

	memset(xdrdata, 0, 4096);
	memset(temp, 0, *length);
	memcpy(temp, *xdr, *length);

	//set xdrlenght
	pathlength = strlen(path);
	while ((pathlength%4) != 0)
		pathlength++;
	xdrlength = 4+pathlength+24+4*veclength;

	//set path
	xdrdata[3] = strlen(path);
	for (c=0; c<strlen(path); c++)
		xdrdata[4+c] = path[c];
	xdrdata[7+pathlength] = 0x2;

	//make specific part of xdr
	xdrdata[11+pathlength] = -112;
	xdrdata[15+pathlength] = veclength;
	for (c=0; c<veclength; c++)
		for (d=0; d<4; d++)
		{
			buf = (char*)&setintvec[c];
			//~ memcpy(tc[c], buf, 4);
			xdrdata[19+pathlength+4*c-d] = buf[d];
		}

	//make xdr end
	xdrdata[xdrlength-1] = 0x4;

	*xdr = (char*)malloc((*length+xdrlength)*sizeof(char));
	memcpy(*xdr, temp, *length);
	memcpy(*xdr+*length, xdrdata, xdrlength);
	*length = *length+xdrlength;

	return;
}

void generatesetbody(int type, char *xdr[], int *length, char *path, void* setvalue)
{
	int pathlength;
	int i, j;
	int strlength;
	int xdrposition;

	pathlength = strlen(path);
	strlength = pathlength;
	while(strlength%4)		//make length%4 == 0 for allocation
		strlength++;

	xdrposition = *length;

	switch(type)
	{
	case OV_VT_STRING:
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
		break;

	}

	//set xdr end
	(*xdr)[*length-1] = 0x4;					//whatever 0x04 stands for^^
	return;
}


void generatesetsinglevecbody(char *xdr[], int *length, char *path, float setsinglevec[], int veclength)
{
	int pathlength;
	int xdrlength;
	int c, d;
	char xdrdata[4096];
	char *buf;
	char *temp = (char*)malloc(*length*sizeof(char));

	memset(xdrdata, 0, 4096);
	memset(temp, 0, *length);
	memcpy(temp, *xdr, *length);

	//set xdrlenght
	pathlength = strlen(path);
	while ((pathlength%4) != 0)
		pathlength++;
	xdrlength = 4+pathlength+24+4*veclength;

	//set path
	xdrdata[3] = strlen(path);
	for (c=0; c<strlen(path); c++)
		xdrdata[4+c] = path[c];
	xdrdata[7+pathlength] = 0x2;

	//make specific part of xdr
	xdrdata[11+pathlength] = -96;
	xdrdata[15+pathlength] = veclength;
	for (c=0; c<veclength; c++)
		for (d=0; d<4; d++)
		{
			buf = (char*)&setsinglevec[c];
			//~ memcpy(tc[c], buf, 4);
			xdrdata[19+pathlength+4*c-d] = buf[d];
		}

	//make xdr end
	xdrdata[xdrlength-1] = 0x4;

	*xdr = (char*)malloc((*length+xdrlength)*sizeof(char));
	memcpy(*xdr, temp, *length);
	memcpy(*xdr+*length, xdrdata, xdrlength);
	*length = *length+xdrlength;

	return;
}


void generatesetstringvecbody(char *xdr[], int *length, char *path, char **setstringvec, int veclength)
{
	int pathlength;
	int xdrlength;
	int c, d, k;
	char xdrdata[4096];
	char *temp = (char*)malloc(*length*sizeof(char));
	int *strlength, strlengthtemp;

	strlength = (int*)malloc(veclength*sizeof(int));

	memset(xdrdata, 0, 4096);
	memset(temp, 0, *length);
	memcpy(temp, *xdr, *length);

	//set xdrlenght
	pathlength = strlen(path);
	while ((pathlength%4) != 0)
		pathlength++;
	xdrlength = 4+pathlength+24+4*veclength;
	for (c=0; c<veclength; c++)
	{
		strlength[c] = strlen(setstringvec[c]);
		while ((strlength[c]%4) != 0)
			strlength[c]++;
		xdrlength = xdrlength+strlength[c];
	}

	//set path
	xdrdata[3] = strlen(path);
	for (c=0; c<strlen(path); c++)
		xdrdata[4+c] = path[c];
	xdrdata[7+pathlength] = 0x2;

	//make specific part of xdr
	xdrdata[11+pathlength] = -80;
	xdrdata[15+pathlength] = veclength;
	k = 19+pathlength;
	for (c=0; c<veclength; c++)
	{
		xdrdata[k] = strlen(setstringvec[c]);
		k++;
		for (d=0; d<strlen(setstringvec[c]); d++)
			xdrdata[k+d] = setstringvec[c][d];
		strlengthtemp = strlength[c];
		while((strlengthtemp%4) != 0)
			strlengthtemp++;
		k = k+strlength[c]+3;
	}

	//make xdr end
	xdrdata[xdrlength-1] = 0x4;

	*xdr = (char*)malloc((*length+xdrlength)*sizeof(char));
	memcpy(*xdr, temp, *length);
	memcpy(*xdr+*length, xdrdata, xdrlength);
	*length = *length+xdrlength;

	return;
}

void generatesetstringbody(char *xdr[], int *length, char *path, char *setstring)
{
	int pathlength;
	int xdrlength;
	int c;
	char xdrdata[4096];
	char *temp = (char*)malloc(*length*sizeof(char));
	int strlength;

	memset(xdrdata, 0, 4096);
	memset(temp, 0, *length);
	memcpy(temp, *xdr, *length);

	//set xdrlenght
	pathlength = strlen(path);
	strlength = strlen(setstring);
	while ((pathlength%4) != 0)
		pathlength++;
	while ((strlength%4) != 0)
		strlength++;
	xdrlength = 4+pathlength+24+strlength;

	//set path
	xdrdata[3] = strlen(path);
	for (c=0; c<strlen(path); c++)
		xdrdata[4+c] = path[c];
	xdrdata[7+pathlength] = 0x2;

	//make specific part of xdr
	xdrdata[11+pathlength] = 48;
	xdrdata[15+pathlength] = strlen(setstring);
	for (c=0; c<strlen(setstring); c++)
		xdrdata[16+pathlength+c] = setstring[c];

	//make xdr end
	xdrdata[xdrlength-1] = 0x4;

	*xdr = (char*)malloc((*length+xdrlength)*sizeof(char));
	memcpy(*xdr, temp, *length);
	memcpy(*xdr+*length, xdrdata, xdrlength);
	*length = *length+xdrlength;

	return;
}

void generatesetuintvecbody(char *xdr[], int *length, char *path, unsigned int setuintvec[], int veclength)
{
	int pathlength;
	int xdrlength;
	int c, d;
	char xdrdata[4096];
	char *buf;
	char *temp = (char*)malloc(*length*sizeof(char));

	memset(xdrdata, 0, 4096);
	memset(temp, 0, *length);
	memcpy(temp, *xdr, *length);

	//set xdrlenght
	pathlength = strlen(path);
	while ((pathlength%4) != 0)
		pathlength++;
	xdrlength = 4+pathlength+24+4*veclength;

	//set path
	xdrdata[3] = strlen(path);
	for (c=0; c<strlen(path); c++)
		xdrdata[4+c] = path[c];
	xdrdata[7+pathlength] = 0x2;

	//make specific part of xdr
	xdrdata[11+pathlength] = -111;
	xdrdata[15+pathlength] = veclength;
	for (c=0; c<veclength; c++)
		for (d=0; d<4; d++)
		{
			buf = (char*)&setuintvec[c];
			//~ memcpy(tc[c], buf, 4);
			xdrdata[19+pathlength+4*c-d] = buf[d];
		}

	//make xdr end
	xdrdata[xdrlength-1] = 0x4;

	*xdr = (char*)malloc((*length+xdrlength)*sizeof(char));
	memcpy(*xdr, temp, *length);
	memcpy(*xdr+*length, xdrdata, xdrlength);
	*length = *length+xdrlength;

	return;
}


void generategetheader(char *xdr[], int *length)
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
	xdrdata[20] = 0;			//procedure
	xdrdata[21] = 0;			//procedure
	xdrdata[22] = 1;			//procedure
	xdrdata[23] = 1;			//procedure
	for (c=24; c<46; c++)
		xdrdata[c] = 0;
	xdrdata[46] = 0;
	xdrdata[47] = 1;

	*xdr = (char*)malloc(52*sizeof(char));		//rpcheader (4bytes) + xdrheader
	memcpy(*xdr+4, xdrdata, 48);				//rpcheader (first 4 bytes) is set later
	*length = 52;

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

void test(char *xdr[], int *length, char *path)
{
	generategetheader(xdr, length);
	generategetbody(xdr, length, path);
	addrpcheader(xdr, length);
}
