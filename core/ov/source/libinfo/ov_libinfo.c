/*
*   Copyright (C) 2004
*   Lehrstuhl fuer Prozessleittechnik,
*   RWTH Aachen, D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
*
*   This file is part of the ACPLT/OV Package which is licensed as open
*   source under the Artistic License; you can use, redistribute and/or
*   modify it under the terms of that license.
*
*   You should have received a copy of the Artistic License along with
*   this Package; see the file ARTISTIC-LICENSE. If not, write to the
*   Copyright Holder.
*
*   THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
*   WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
*   OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/
/*
*	History:
*	--------
*	07-Oct-2004 Ansgar Münnemann <ansgar@plt.rwth-aachen.de>: File created.
*/

#include "libov/ov_library.h"
#include "libov/ov_macros.h"

/*	----------------------------------------------------------------------	*/

/*
*	Makro wrapping function to open a DLL/shared library
*/
#if OV_SYSTEM_UNIX
#define Ov_Library_OpenDLL(filename)	dlopen((filename), RTLD_LAZY)
#elif OV_SYSTEM_NT
#define Ov_Library_OpenDLL(filename)	LoadLibrary(filename)
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Makro wrapping function to close a DLL/shared library
*/
#if OV_SYSTEM_UNIX
#define Ov_Library_CloseDLL(handle)	dlclose(handle)
#elif OV_SYSTEM_NT
#define Ov_Library_CloseDLL(handle)	FreeLibrary(handle)
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Makro wrapping the address resolution of a symbol in a DLL/shared library
*/
#if OV_SYSTEM_UNIX
#define Ov_Library_GetAddr(handle, symbolname)	dlsym(handle, symbolname)
#elif OV_SYSTEM_NT
#define Ov_Library_GetAddr(handle, symbolname)	GetProcAddress(handle, symbolname)
#endif

/*	----------------------------------------------------------------------	*/

OV_STRING getfulltypetext(
	OV_VAR_TYPE	vartype
) {
	switch(vartype) {
	case OV_VT_BOOL:
		return "BOOL";
	case OV_VT_BOOL_VEC:
		return "BOOL";
	case OV_VT_INT:
		return "INT";
	case OV_VT_INT_VEC:
		return "INT";
	case OV_VT_UINT:
		return "UINT";
	case OV_VT_UINT_VEC:
		return "UINT";
	case OV_VT_SINGLE:
		return "SINGLE";
	case OV_VT_SINGLE_VEC:
		return "SINGLE";
	case OV_VT_DOUBLE:
		return "DOUBLE";
	case OV_VT_DOUBLE_VEC:
		return "DOUBLE";
	case OV_VT_STRING:
		return "STRING";
	case OV_VT_STRING_VEC:
		return "STRING";
	case OV_VT_TIME:
		return "TIME";
	case OV_VT_TIME_VEC:
		return "TIME";
	case OV_VT_TIME_SPAN:
		return "TIME_SPAN";
	case OV_VT_TIME_SPAN_VEC:
		return "TIME_SPAN";
	case OV_VT_BOOL_PV:
		return "BOOL_PV";
	case OV_VT_INT_PV:
		return "INT_PV";
	case OV_VT_SINGLE_PV:
		return "SINGLE_PV";
	case OV_VT_ANY:
		return "ANY";
	case OV_VT_STRUCT:
		return "STRUCTURE";
	default:
		return "C_TYPE";
	}
	return NULL;
}

/*	----------------------------------------------------------------------	*/

void flagstext(
	OV_UINT	flags,
	char *text
) {
   	OV_UINT j;
   	unsigned char i;

	for (i=0;i<32;i++) {
		j = 1 << i;
		if (flags & j) {
			*text = 'a'+i;
			text++;
		}
	}
	*text = 0;

}

/*	----------------------------------------------------------------------	*/

int main(int argc, char **argv) {
	char 				*libname=NULL;
	char 				*filename=NULL;
	int				i;
	OV_BOOL				classinfo = FALSE;
	OV_BOOL				associnfo = FALSE;
	OV_BOOL				structinfo = FALSE;
	char				text[32];
	OV_LIBRARY_DEF			*plibdef = NULL;
	OV_CLASS_DEF			*pclassdef = NULL;
	OV_VARIABLE_DEF			*pvardef = NULL;
	OV_ASSOCIATION_DEF		*passocdef = NULL;
	OV_STRUCTURE_DEF		*pstructdef = NULL;
	OV_PART_DEF			*ppartdef = NULL;
	OV_OPERATION_DEF		*popdef = NULL;
	OV_FNC_LIBRARY_OPEN		*openfnc = NULL;
	OV_STRING			tmpstring = NULL;
	OV_STRING			tmpenv = NULL;
	OV_STRING			fpath = NULL, path = NULL, nextpath = NULL;
	OV_STRING			delim = NULL;
	OV_DLLHANDLE			handle = 0;
        FILE *fp;

	for(i=1; i<argc; i++) {

		if(!strcmp(argv[i], "-l")) {
			i++;
			if(i<argc) {
				libname = argv[i];
			} else {
				goto HELP;
			}
		}
		if(!strcmp(argv[i], "-f")) {
			i++;
			if(i<argc) {
				filename = argv[i];
			} else {
				goto HELP;
			}
		}
		else if(!strcmp(argv[i], "-c")) {
			classinfo = TRUE;
		}
		else if(!strcmp(argv[i], "-a")) {
			associnfo = TRUE;
		}
		else if(!strcmp(argv[i], "-s")) {
			structinfo = TRUE;
		}
		else if(!strcmp(argv[i], "-v")) {
                        fprintf(stderr, "OV_LIBINFO Version: " OV_VER_LIBINFO "\n");
                        return 0;
		}
		else if(!strcmp(argv[i], "-h")) {
HELP:			fprintf(stderr, "OV-Library-Info: lists author, OV version, library version, comment info of the library.\n"
					"Lists additionally all classes, associations, and structures\n");
                        fprintf(stderr, "Usage: ov_libinfo [arguments]\n"
			"\n"
			"-l  LIBNAME     Set name of the library whose infos are listed\n"
			"-f  FILENAME    Set name of the target file for the listed informations\n"
			"-c              List class definitions\n"
			"-a              List association definitions\n"
			"-s              List structure definitions\n"
			"-v              List version information\n");
			return 0;
		}
	}

	if(!libname) {
		goto HELP;
	}

	if (filename) {
		fp = fopen(filename, "w");
		if(!fp) {
			fprintf(stderr, "unable to open file for writing: %s.\n", filename);
			return -1;
		}
	}
	else fp = stderr;

	/*
	*	look for a DLL/shared library
	*/
	if(!openfnc) {
		/*
		*	create a copy of the search path in which we can modify chars
		*/
		if(getenv(OV_LIBRARY_PATH_ENV)) {
			tmpenv =  Ov_HeapStrdup(getenv(OV_LIBRARY_PATH_ENV));
		} else {
			tmpenv =  Ov_HeapStrdup("");
		}
		if(!tmpenv) {
			fprintf(stderr, "undefined environment variable: %s.\n", OV_LIBRARY_PATH_ENV);
			return 0;
		}
		/*
		*	iterate over all paths in the search paths
		*/
		for(path=tmpenv; path && !openfnc; path=nextpath) {
			nextpath = strstr(path, OV_DIRPATHSEPARATOR);
			if(nextpath) {
				*nextpath = 0;
				nextpath++;
			}
			/*
			*	if there is a path given, determine the path delimiter;
			*	otherwise there is none
			*/
			if(*path) {
				delim = OV_DIRPATHDELIMITER;
			} else {
				delim = "";
			}
			/*
			*	allocate memory for the DLL name and the function name;
			*	we allocate enough memory for both.
			*/
			tmpstring = (OV_STRING)Ov_HeapMalloc(strlen(path)+strlen(delim)
				+strlen(libname)+strlen(OV_DLLFLNSUFFIX)
				+strlen(OV_CONST_OPENFNC_PREFIX)+1);
			if(!tmpstring) {
				fprintf(stderr, "no memory on heap free.\n");
				return 0;
			}
			/*
			*	enter the DLL name into the string and try to open the DLL
			*/
			sprintf(tmpstring, "%s%s%s" OV_DLLFLNSUFFIX, path, delim,
				libname);
			handle = Ov_Library_OpenDLL(tmpstring);
			if(handle) {
				/*
				*	we opened the DLL, get the address of the open function;
				*	for that purpose enter its name into the string
				*/
				sprintf(tmpstring, OV_CONST_OPENFNC_PREFIX "%s",
					libname);
				/*
				*	get the address of the open function
				*/
				openfnc = (OV_FNC_LIBRARY_OPEN*)Ov_Library_GetAddr(
					handle, tmpstring);
				if(!openfnc) {
					fprintf(stderr, "undefined open function: %s.\n", tmpstring);
					/*
					*	we got no open function, close the DLL
					*/
					goto CLOSEDLL;
				}
				fpath = path;
			}
#if OV_SYSTEM_LINUX | OV_SYSTEM_SOLARIS
			else {
				fprintf(stderr,"Can't load library. Reason: %s\n", dlerror());
			}
#endif
		}
	}
	if(openfnc) {
		plibdef = openfnc();
		/*
		*	list library infos
		*/
		fprintf(stderr,"library path: %s\n", fpath);
		fprintf(stderr,"version: %s\n", plibdef->version);
		fprintf(stderr,"compiled wih OV version: %s\n", plibdef->ov_version);
		fprintf(stderr,"author: %s\n", plibdef->author);
		fprintf(stderr,"copyright: %s\n", plibdef->copyright);
		fprintf(stderr,"comment: %s\n\n", plibdef->comment);

		fprintf(fp,"LIBRARY %s\n", plibdef->identifier);
		fprintf(fp,"\t VERSION   = \"%s\";\n", plibdef->version);
		fprintf(fp,"\t AUTHOR    = \"%s\";\n", plibdef->author);
		fprintf(fp,"\t COPYRIGHT = \"%s\";\n", plibdef->copyright);
		fprintf(fp,"\t COMMENT   = \"%s\";\n", plibdef->comment);
		fprintf(fp,"\n");
		if (classinfo) {
			/*
			*	list class definitions
			*/
			pclassdef = plibdef->classes;
			while (pclassdef) {
			        fprintf(fp,"\t CLASS %s : ", pclassdef->identifier);
			        fprintf(fp,"CLASS %s\n", pclassdef->baseclassname);
			        if (pclassdef->classprops & OV_CP_INSTANTIABLE) fprintf(fp,"\t\t IS_INSTANTIABLE;\n");
			        if (pclassdef->classprops & OV_CP_FINAL) fprintf(fp,"\t\t IS_FINAL;\n");
			        fprintf(fp,"\t\t COMMENT = \"%s\";\n", pclassdef->comment);
			        pvardef = pclassdef->variables;
		        	fprintf(fp,"\t\t VARIABLES\n");
			        while (pvardef) {
					fprintf(fp,"\t\t\t %s", pvardef->identifier);
				        if (pvardef->veclen == 0) fprintf(fp,"[]");
				        if (pvardef->veclen > 1) fprintf(fp,"[%lu]", pvardef->veclen);
				        fprintf(fp," : ");
			        	fprintf(fp,"%s", getfulltypetext(pvardef->vartype));
			        	if (pvardef->vartype == OV_VT_STRUCT) {
				        	fprintf(fp," %s", pvardef->structurename);
				        }
			        	if (pvardef->vartype == OV_VT_CTYPE) {
				        	fprintf(fp," <%s>", pvardef->ctypename);
				        }
				        fprintf(fp," ");
				        if (pvardef->varprops & OV_VP_GETACCESSOR) fprintf(fp,"HAS_GET_ACCESSOR ");
				        if (pvardef->varprops & OV_VP_SETACCESSOR) fprintf(fp,"HAS_SET_ACCESSOR ");
				        if (pvardef->varprops & OV_VP_DERIVED) fprintf(fp,"IS_DERIVED ");
				        if (pvardef->varprops & OV_VP_STATIC) fprintf(fp,"IS_STATIC ");
					if (pvardef->flags) {
					        flagstext(pvardef->flags, text);
				        	fprintf(fp," FLAGS=\"%s\"", text);
				 	}
				 	if (pvardef->comment) fprintf(fp," COMMENT=\"%s\"", pvardef->comment);

		        		fprintf(fp,";\n");

				        pvardef = pvardef->pnext;
			        }
		        	fprintf(fp,"\t\t END_VARIABLES;\n");
			        ppartdef = pclassdef->parts;
		        	fprintf(fp,"\t\t PARTS\n");
			        while (ppartdef) {
			        	fprintf(fp,"\t\t\t %s : ", ppartdef->identifier);
			        	fprintf(fp,"CLASS %s ", ppartdef->partclassname);
				        flagstext(ppartdef->flags, text);
			        	fprintf(fp,"FLAGS = \"%s\";\n", text);
			        	ppartdef = ppartdef->pnext;
			        }
		        	fprintf(fp,"\t\t END_PARTS;\n");
			        popdef = pclassdef->operations;
		        	fprintf(fp,"\t\t OPERATIONS\n");
			        while (popdef) {
			        	fprintf(fp,"\t\t\t %s : ", popdef->identifier);
			        	fprintf(fp,"C_FUNCTION <%s>;\n", popdef->cfnctypename);
			        	popdef = popdef->pnext;
			        }
		        	fprintf(fp,"\t\t END_OPERATIONS;\n");
				fprintf(fp,"\t END_CLASS;\n\n");
			        pclassdef = pclassdef->pnext;
			}
		}
		if (associnfo) {
			/*
			*	list association definitions
			*/
			passocdef = plibdef->associations;
                        while (passocdef) {
			        fprintf(fp,"\t ASSOCIATION %s : ", passocdef->identifier);
			        if (passocdef->assoctype == OV_AT_ONE_TO_ONE) fprintf(fp,"ONE_TO_ONE\n");
			        if (passocdef->assoctype == OV_AT_ONE_TO_MANY) fprintf(fp,"ONE_TO_MANY\n");
			        if (passocdef->assoctype == OV_AT_MANY_TO_MANY) fprintf(fp,"MANY_TO_MANY\n");
			        fprintf(fp,"\t\t PARENT %s : ", passocdef->parentrolename);
			        fprintf(fp,"CLASS %s;\n", passocdef->parentclassname);
			        fprintf(fp,"\t\t CHILD  %s : ", passocdef->childrolename);
			        fprintf(fp,"CLASS %s;\n", passocdef->childclassname);
			        fprintf(fp,"\t END_ASSOCIATION;\n\n");
				passocdef = passocdef->pnext;
			}
		}
		if (structinfo) {
			/*
			*	list structure definitions
			*/
			pstructdef = plibdef->structures;
			while (pstructdef) {
                                fprintf(fp,"\t STRUCTURE %s\n", pstructdef->identifier);
			        pvardef = pstructdef->members;
			        while (pvardef) {
					fprintf(fp,"\t\t %s", pvardef->identifier);
				        if (pvardef->veclen == 0) fprintf(fp,"[]");
				        if (pvardef->veclen > 1) fprintf(fp,"[%lu]", pvardef->veclen);
				        fprintf(fp," : ");
			        	fprintf(fp,"%s", getfulltypetext(pvardef->vartype));
			        	if (pvardef->vartype == OV_VT_STRUCT) {
				        	fprintf(fp," %s", pvardef->structurename);
				        }
			        	if (pvardef->vartype == OV_VT_CTYPE) {
				        	fprintf(fp," <%s>", pvardef->ctypename);
				        }
				        fprintf(fp," ");
				        if (pvardef->varprops & OV_VP_GETACCESSOR) fprintf(fp,"HAS_GET_ACCESSOR ");
				        if (pvardef->varprops & OV_VP_SETACCESSOR) fprintf(fp,"HAS_SET_ACCESSOR ");
				        if (pvardef->varprops & OV_VP_DERIVED) fprintf(fp,"IS_DERIVED ");
				        if (pvardef->varprops & OV_VP_STATIC) fprintf(fp,"IS_STATIC ");
					if (pvardef->flags) {
					        flagstext(pvardef->flags, text);
				        	fprintf(fp," FLAGS=\"%s\"", text);
				 	}
				 	if (pvardef->comment) fprintf(fp," COMMENT=\"%s\"", pvardef->comment);

		        		fprintf(fp,";\n");

				        pvardef = pvardef->pnext;
			        }
			        pstructdef = pstructdef->pnext;
				fprintf(fp,"\t END_STRUCTURE;\n\n");
			}
		}
		fprintf(fp,"END_LIBRARY;\n");
	}
CLOSEDLL:
	/*
	*	free the temporary string
	*/
	Ov_HeapFree(tmpstring);
	/*
	*	free the temporary search path
	*/
	Ov_HeapFree(tmpenv);

	if(handle) {
		Ov_Library_CloseDLL((OV_DLLHANDLE)handle);
		handle = 0;
	}
	/*
	*	close file
	*/
	if (filename) {
		if(fclose(fp)) {
			fprintf(stderr, "error closing file.\n");
			return -1;
		}
	}
	return 0;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/


