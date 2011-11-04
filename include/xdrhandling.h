#ifndef XDRHANLING_H_INCLUDED
#define XDRHANLING_H_INCLUDED

#include <stdlib.h>

void generategetxdr(char *xdr[], int *length, char *path);
void generategetheader(char *xdr[], int *length);
void generategetbody(char *xdr[], int *length, char *path);
void generatesetheader(char *xdr[], int *length);
void addrpcheader(char *xdr[], int *length);
void generatesetstringxdr(char *xdr[], int *length, char *path, char *setstring);
void generatesetstringbody(char *xdr[], int *length, char *path, char *setstring);
void generatesetstringvecxdr(char *xdr[], int *length, char *path, char **setstringvec, int veclength);
void generatesetstringvecbody(char *xdr[], int *length, char *path, char **setstringvec, int veclength);
void generatesetboolxdr(char *xdr[], int *length, char *path, int setbool);
void generatesetboolbody(char *xdr[], int *length, char *path, int setbool);
void generatesetboolvecxdr(char *xdr[], int *length, char *path, int setboolvec[], int veclength);
void generatesetboolvecbody(char *xdr[], int *length, char *path, int setboolvec[], int veclength);
void generatesetdoublexdr(char *xdr[], int *length, char *path, double setdouble);
void generatesetdoublebody(char *xdr[], int *length, char *path, double setdouble);
void generatesetdoublevecxdr(char *xdr[], int *length, char *path, double setdoublevec[], int veclength);
void generatesetdoublevecbody(char *xdr[], int *length, char *path, double setdoublevec[], int veclength);
void generatesetintxdr(char *xdr[], int *length, char *path, int setint);
void generatesetintbody(char *xdr[], int *length, char *path, int setint);
void generatesetintvecxdr(char *xdr[], int *length, char *path, int setintvec[], int veclength);
void generatesetintvecbody(char *xdr[], int *length, char *path, int setintvec[], int veclength);
void generatesetsinglexdr(char *xdr[], int *length, char *path, float setsingle);
void generatesetsinglebody(char *xdr[], int *length, char *path, float setsingle);
void generatesetsinglevecxdr(char *xdr[], int *length, char *path, float setsinglevec[], int veclength);
void generatesetsinglevecbody(char *xdr[], int *length, char *path, float setsinglevec[], int veclength);
void generatesetuintxdr(char *xdr[], int *length, char *path, unsigned int setuint);
void generatesetuintbody(char *xdr[], int *length, char *path, unsigned int setuint);
void generatesetuintvecxdr(char *xdr[], int *length, char *path, unsigned int setuintvec[], int veclength);
void generatesetuintvecbody(char *xdr[], int *length, char *path, unsigned int setuintvec[], int veclength);


void generatedeleteobjectxdr(char *xdr[], int *length, char *path);
void generatecreateobjectxdr(char *xdr[], int *length, char *path, char *librarypath, int position, char *refpath);
void generaterenameobjectxdr(char *xdr[], int *length, char *oldpath, char *newpath);
void generateunlinkobjectxdr(char *xdr[], int *length, char *path, char *linkedpath);
void generatelinkobjectxdr(char *xdr[], int *length, char *path, char *linkedpath, int position, char *refpath);


void generategetepxdr(char *xdr[], int *length, char *path, int ksobjecttype, char *namemask, int epflags);




int analysegetstringreply(char xdr[], int xdrlength, char *reply[]);
int analysegetstringvecreply(char xdr[], int xdrlength, char ***reply, int *replyveclength);
int analysegetboolreply(char xdr[], int xdrlength, int *reply);
int analysegetboolvecreply(char xdr[], int xdrlength, int *reply[], int *veclength);
int analysegetdoublereply(char xdr[], int xdrlength, double *reply);
int analysegetdoublevecreply(char xdr[], int xdrlength, double *reply[], int *veclength);
int analysegetintreply(char xdr[], int xdrlength, int *reply);
int analysegetintvecreply(char xdr[], int xdrlength, int *reply[], int *veclength);
int analysegetsinglereply(char xdr[], int xdrlength, float *reply);
int analysegetsinglevecreply(char xdr[], int xdrlength, float *reply[], int *veclength);
int analysegetuintreply(char xdr[], int xdrlength, unsigned int *reply);
int analysegetuintvecreply(char xdr[], int xdrlength, unsigned int *reply[], int *veclength);


int analysedeleteobjectreply(char xdr[], int xdrlength);
int analysecreateobjectreply(char xdr[], int xdrlength);
int analyserenameobjectreply(char xdr[], int xdrlength);
int analyselinkobjectreply(char xdr[], int xdrlength);
int analyseunlinkobjectreply(char xdr[], int xdrlength);


int analysesetxdr(char xdr[], int xdrlength);

#endif
