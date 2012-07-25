#ifndef XDRHANLING_H_INCLUDED
#define XDRHANLING_H_INCLUDED

#include <stdlib.h>

void generategetxdr(char *xdr[], int *length, char *path);
void generateheader(int procedureNumber, char *xdr[], int *length);
void generategetbody(char *xdr[], int *length, char *path);
void generatesetbody(int type, char *xdr[], int *length, char *path, void* setvalue, unsigned long varTsSecs, unsigned long varTsUsecs, unsigned long varQState);
void generatesetvecbody(int type, char *xdr[], int *length, char *path, void* setvalue, int veclength, unsigned long varTsSecs, unsigned long varTsUsecs, unsigned long varQState);
void addrpcheader(char *xdr[], int *length);

void generatesetstringxdr(char *xdr[], int *length, char *path, char *setstring, unsigned long varTsSecs, unsigned long varTsUsecs, unsigned long varQState);
void generatesetstringvecxdr(char *xdr[], int *length, char *path, char **setstringvec, int veclength, unsigned long varTsSecs, unsigned long varTsUsecs, unsigned long varQState);
void generatesetstringvecbody(char *xdr[], int *length, char *path, char **setstringvec, int veclength);
void generatesetboolxdr(char *xdr[], int *length, char *path, int setbool, unsigned long varTsSecs, unsigned long varTsUsecs, unsigned long varQState);
void generatesetboolvecxdr(char *xdr[], int *length, char *path, int setboolvec[], int veclength, unsigned long varTsSecs, unsigned long varTsUsecs, unsigned long varQState);
void generatesetboolvecbody(char *xdr[], int *length, char *path, int setboolvec[], int veclength);
void generatesetdoublexdr(char *xdr[], int *length, char *path, double setdouble, unsigned long varTsSecs, unsigned long varTsUsecs, unsigned long varQState);
void generatesetdoublevecxdr(char *xdr[], int *length, char *path, double setdoublevec[], int veclength, unsigned long varTsSecs, unsigned long varTsUsecs, unsigned long varQState);
void generatesetdoublevecbody(char *xdr[], int *length, char *path, double setdoublevec[], int veclength);
void generatesetintxdr(char *xdr[], int *length, char *path, int setint, unsigned long varTsSecs, unsigned long varTsUsecs, unsigned long varQState);
void generatesetintvecxdr(char *xdr[], int *length, char *path, int setintvec[], int veclength, unsigned long varTsSecs, unsigned long varTsUsecs, unsigned long varQState);
void generatesetintvecbody(char *xdr[], int *length, char *path, int setintvec[], int veclength);
void generatesetsinglexdr(char *xdr[], int *length, char *path, float setsingle, unsigned long varTsSecs, unsigned long varTsUsecs, unsigned long varQState);
void generatesetsinglevecxdr(char *xdr[], int *length, char *path, float setsinglevec[], int veclength, unsigned long varTsSecs, unsigned long varTsUsecs, unsigned long varQState);
void generatesetsinglevecbody(char *xdr[], int *length, char *path, float setsinglevec[], int veclength);
void generatesetuintxdr(char *xdr[], int *length, char *path, unsigned int setuint, unsigned long varTsSecs, unsigned long varTsUsecs, unsigned long varQState);
void generatesetuintvecxdr(char *xdr[], int *length, char *path, unsigned int setuintvec[], int veclength, unsigned long varTsSecs, unsigned long varTsUsecs, unsigned long varQState);
void generatesetuintvecbody(char *xdr[], int *length, char *path, unsigned int setuintvec[], int veclength);


void generatedeleteobjectxdr(char *xdr[], int *length, char *path);
void generatecreateobjectxdr(char *xdr[], int *length, char *path, char *librarypath, int position, char *refpath);
void generaterenameobjectxdr(char *xdr[], int *length, char *oldpath, char *newpath);
void generateunlinkobjectxdr(char *xdr[], int *length, char *path, char *linkedpath);
void generatelinkobjectxdr(char *xdr[], int *length, char *path, char *linkedpath, int position, char *refpath);

void generategetepxdr(char *xdr[], int *length, char *path, int ksobjecttype, char *namemask, int epflags);


int analysegetreply(int type, char xdr[], int xdrlength, void *reply, unsigned long *repTsSecs, unsigned long *repTsUsecs, unsigned long *repState);
int analysegetvecreply(int type, char xdr[], int xdrlength, void **reply, int *veclength, unsigned long *repTsSecs, unsigned long *repTsUsecs, unsigned long *repState);


int analyseresultonlyreply(char xdr[], int xdrlength);



int analysesetxdr(char xdr[], int xdrlength);

#endif
