/*
*   $Id: ov_builder.c,v 1.4 2002-08-21 12:46:09 ansgar Exp $
*
*   Copyright (C) 1998-1999
*   Lehrstuhl fuer Prozessleittechnik,
*   RWTH Aachen, D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Ansgar Münnemann<ansgar@plt.rwth-aachen.de>
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
*	10-Feb-2001 Ansgar Münnemann <ansgar@plt.rwth-aachen.de>: File created.
*/

#include <ctype.h>
#include <time.h>

#include "ov_builder.h"

/*	----------------------------------------------------------------------	*/

/*
*	Global variables
*/

#if OV_SYSTEM_OPENVMS
OV_STRING		outputpath = "";
#else
OV_STRING		outputpath = ".";
#endif
OV_STRING		libname = NULL;
extern int		yydebug;
extern FILE*	yyin;

/*	----------------------------------------------------------------------	*/

/*
*   Main program
*/
int main(int argc, char **argv) {
	/*
	*	local variables
	*/
	OV_UINT i;
	int		exit_status = EXIT_FAILURE;
	/*
	*	initialization
	*/
	yydebug = 0;
	/*
	*	parse command line arguments
	*/
	for(i=1; i<argc; i++) {
		/*
		*	set model filename option
		*/
		if(!strcmp(argv[i], "-f") || !strcmp(argv[i], "--file")) {
			i++;
			if(i<argc) {
				filename = ov_codegen_getstring(argv[i], strlen(argv[i]));
				if(strcmp(filename, "stdin")) {
					yyin = fopen(filename, "r");
					if(!yyin) {
						fprintf(stderr, "File not found: \"%s\".\n", filename);
						return EXIT_FAILURE;
					}
				} else {
					yyin = stdin;
				}
			} else {
				goto HELP;
			}
		}
		/*
		*	set output path option
		*/
		else if(!strcmp(argv[i], "-o") || !strcmp(argv[i], "--output-path")) {
			i++;
			if(i<argc) {
				OV_UINT len = strlen(argv[i]);
				outputpath = argv[i];
				if((outputpath[len-1] == '/')
					|| (outputpath[len-1] == '\\')
				) {
					outputpath[len-1] = 0;
				}
			} else {
				goto HELP;
			}
		}
		/*
		*	set include path option
		*/
		else if(!strcmp(argv[i], "-I") || !strcmp(argv[i], "--include-path")) {
			i++;
			if(i<argc) {
				OV_UINT len = strlen(argv[i]);
				if(includepath_ptr >= MAX_INCLUDEPATHS) {
					fprintf(stderr, "Too many include paths specified.\n");
					return EXIT_FAILURE;
				}
				if((argv[i][len-1] == '/') || (argv[i][len-1] == '\\')) {
					argv[i][len-1] = 0;
				}
				includepath[includepath_ptr] = argv[i];
				includepath_ptr++;
			} else {
				goto HELP;
			}
		}
		/*
		*	set library option
		*/
		else if(!strcmp(argv[i], "-l") || !strcmp(argv[i], "--library")) {
			i++;
			if(i<argc) {
				libname = argv[i];
			} else {
				goto HELP;
			}
		}
		/*
		*	turn on debug mode option
		*/
		else if(!strcmp(argv[i], "-d") || !strcmp(argv[i], "--debug-mode")) {
			yydebug = 1;
		}
		/*
		*	display version option
		*/
		else if(!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version")) {
			printf("ACPLT/OV Frame Work Builder " OV_VER_BUILDER "\n");
			return EXIT_FAILURE;
		}
		/*
		*	display help option
		*/
		else if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
HELP:		fprintf(stderr, "Usage: ov_builder [arguments]\n"
				"\n"
				"The following arguments are mandatory:\n"
				"-f FILE, --file FILE            Set model filename (*.ovm), you may use stdin\n"
				"\n"
				"The following arguments are optional:\n"
				"-o PATH, --output-path PATH     Set path for output files (*.h and *.c)\n"
				"-I PATH, --include-path PATH    Set include path, may be used multiple times\n"
				"-l LIBRARY, --library LIBRARY   Only create output for this library\n"
				"-d, --debug-mode                Turn on parser debug mode\n"
				"-v, --version                   Display version information\n"
				"-h, --help                      Display this help message\n");
			return EXIT_FAILURE;
		} else {
			goto HELP;
		}
	}
	/*
	*	test if mandatory arguments are set
	*/
	if(!filename) {
		goto HELP;
	}
	/*
	*	syntactically parse the input
	*/
	if(yyparse() == EXIT_SUCCESS) {
		/*
		*	Check the input semantically
		*/
		if(ov_codegen_checksemantics()) {
			/*
			*	Create output
			*/
			exit_status = ov_builder_backend();
		}
	}
	/*
	*	clean up memory and close input file
	*/
	ov_codegen_cleanup();
	if(yyin != stdin) {
		fclose(yyin);
	}
	/*
	*	return exit status
	*/
	return exit_status;
}

/*	----------------------------------------------------------------------	*/

/*
*	Backend of the code generator
*/
int ov_builder_backend(void) {
	/*
	*	local variables
	*/
	OV_OVM_LIBRARY_DEF	*plib;
	int 				exit_status;
	/*
	*	create output for all libraries
	*/
	for(plib=libraries; plib; plib=plib->pnext) {
		/*
		*	create source files *.c
		*/
		if (libname) {
			if (strcmp(plib->identifier,libname)==0) {
				exit_status = ov_builder_createsourcefiles(plib);
				if(exit_status != EXIT_SUCCESS) {
					return exit_status;
				}
			}
		}
		else {
			exit_status = ov_builder_createsourcefiles(plib);
			if(exit_status != EXIT_SUCCESS) 
				return exit_status;
		}
	}
	return EXIT_SUCCESS;
}
	

/*	----------------------------------------------------------------------	*/

/*
*	Create source files of a library
*/
int ov_builder_createsourcefiles(
	OV_OVM_LIBRARY_DEF*	plib
) {
	/*
	*	local variables
	*/
	FILE			*fp;
	FILE			*rfp;
	OV_OVM_CLASS_DEF	*pclass;
	OV_OVM_ASSOCIATION_DEF	*passoc;
	OV_OVM_VARIABLE_DEF 	*pvar;
	OV_OVM_OPERATION_DEF	*pop;
	OV_STRING		libid;
	OV_STRING		classid;
	OV_OVM_OPERATION_DEF	*pop2;
	OV_STRING		lib_ident;
	OV_STRING		class_ident;
	char			text1[256];
	char			text2[256];
	OV_INT			pathcount;
	int			i;
	/*
	*	create output files
	*/
	for(pclass=plib->classes; pclass; pclass=pclass->pnext) {
		fp = ov_builder_createfile(pclass->identifier, ".c");
		if(!fp) {
			return EXIT_FAILURE;
		}
		/*
		*	insert OV_COMPILE_LIBRARY option
		*/
		fprintf(fp, "\n");
		fprintf(fp, "#ifndef OV_COMPILE_LIBRARY_%s\n", plib->identifier);
		fprintf(fp, "#define OV_COMPILE_LIBRARY_%s\n", plib->identifier);
		fprintf(fp, "#endif\n");
		fprintf(fp, "\n");

		/*
		*	insert include files
		*/
		fprintf(fp, "\n");
		fprintf(fp, "#include \"%s.h\"\n", plib->identifier);
		fprintf(fp,"#include \"libov/ov_macros.h\"\n\n");

		fprintf(fp, "\n");
		for(pvar=pclass->variables;pvar;pvar=pvar->pnext) {
			if (pvar->varprops & OV_VP_GETACCESSOR) {
				fprintf(fp,"OV_DLLFNCEXPORT %s %s_%s_%s_get(\n",ov_builder_getvartypetext(pvar->vartype),
					plib->identifier,pclass->identifier,pvar->identifier);
				fprintf(fp,"             OV_INSTPTR_%s_%s          pobj",plib->identifier,pclass->identifier);
				if (pvar->veclen != 1) {
			     		fprintf(fp,",\n             OV_UINT *pveclen");
					fprintf(fp,"\n) {\n");
					if (pvar->varprops & (OV_VP_DERIVED | OV_VP_STATIC)) {
						fprintf(fp,"             return (%s) 0;\n",ov_builder_getvartypetext(pvar->vartype));
						fprintf(fp,"}\n\n");
					}
					else {
						if (pvar->veclen > 1) {
							fprintf(fp,"             *pveclen = %d;\n",pvar->veclen);
							fprintf(fp,"             return pobj->v_%s;\n",pvar->identifier);
							fprintf(fp,"}\n\n");
						}
						else {
							fprintf(fp,"             *pveclen = pobj->v_%s.veclen;\n",pvar->identifier);
							fprintf(fp,"             return pobj->v_%s.value;\n",pvar->identifier);
							fprintf(fp,"}\n\n");
						}
					}
				}
				else {
					fprintf(fp,"\n");
					fprintf(fp,") {\n");
					if (pvar->varprops & (OV_VP_DERIVED | OV_VP_STATIC)) {
						fprintf(fp,"             return (%s) 0;\n",ov_builder_getvartypetext(pvar->vartype));
						fprintf(fp,"}\n\n");
					}
					else {
						if (pvar->vartype >= OV_VT_BOOL_PV) {
							fprintf(fp,"             return &pobj->v_%s;\n",pvar->identifier);
							fprintf(fp,"}\n\n");
						}
						else {
							fprintf(fp,"             return pobj->v_%s;\n",pvar->identifier);
							fprintf(fp,"}\n\n");
						}
					}
				}
			}	
			if (pvar->varprops & OV_VP_SETACCESSOR) {
				fprintf(fp,"OV_DLLFNCEXPORT OV_RESULT %s_%s_%s_set(\n",
					plib->identifier,pclass->identifier,pvar->identifier);
				fprintf(fp,"             OV_INSTPTR_%s_%s          pobj,\n",plib->identifier,pclass->identifier);
				if (pvar->veclen != 1) {
					fprintf(fp,"             const %spvalue,\n",ov_builder_getvartypetext(pvar->vartype));
					fprintf(fp,"             const OV_UINT veclen\n");
					fprintf(fp,") {\n");

					if (pvar->varprops & (OV_VP_DERIVED | OV_VP_STATIC)) {
						fprintf(fp,"             return OV_ERR_OK;\n");
						fprintf(fp,"}\n\n");
					}
					else {
						if (pvar->veclen > 1) {
							fprintf(fp,"             return Ov_SetStaticVectorValue(pobj->v_%s,pvalue,veclen,%s);\n",pvar->identifier,
								ov_builder_getvartypevectext(pvar->vartype));
							fprintf(fp,"}\n\n");
						}
						else {
							fprintf(fp,"             return Ov_SetDynamicVectorValue(&pobj->v_%s,pvalue,veclen,%s);\n",pvar->identifier,
								ov_builder_getvartypevectext(pvar->vartype));
							fprintf(fp,"}\n\n");
						}
					}
				}
				else {
					if (pvar->vartype >= OV_VT_BOOL_PV) {
						fprintf(fp,"             const %spvalue,\n",ov_builder_getvartypetext(pvar->vartype));
					}
					else {
						fprintf(fp,"             const %s           value\n",ov_builder_getvartypetext(pvar->vartype));
					}
					fprintf(fp,") {\n");

					if (pvar->varprops & (OV_VP_DERIVED | OV_VP_STATIC)) {
						fprintf(fp,"             return OV_ERR_OK;\n");
						fprintf(fp,"}\n\n");
					}
					else {
						if (pvar->vartype >= OV_VT_BOOL_PV) {
							fprintf(fp,"             pobj->v_%s = *pvalue;\n",pvar->identifier);
							fprintf(fp,"             return OV_ERR_OK;\n");
						}
						else {
							if (pvar->vartype == OV_VT_STRING) {
								fprintf(fp,"             return ov_string_setvalue(&pobj->v_%s,value);\n",pvar->identifier);
							}
							else {
								fprintf(fp,"             pobj->v_%s = value;\n",pvar->identifier);
								fprintf(fp,"             return OV_ERR_OK;\n");
							}
						}
						fprintf(fp,"}\n\n");
					}
				}
			}
		}
		for(pop=pclass->operations; pop; pop=pop->pnext) {
			lib_ident = plib->identifier;
			class_ident = pclass->identifier;
			if (pop->overloaded) {
				pop2 = ov_codegen_getopdef(NULL, pclass, pop->identifier);
				if(pop2) {
					libid = ov_codegen_getstring(pop2->classname, strlen(pop2->classname));
					for(classid = libid; *classid; classid++) {
						if(*classid == '/') {
							*classid = 0;
							classid++;
							break;
						}
					}
					lib_ident = libid;
					class_ident = classid;
				}		
			}
			rfp = NULL;
			pathcount=includepath_ptr;
			while ((pathcount > 0) && (!rfp)) {
				rfp = ov_builder_openreadfile(includepath[pathcount-1], lib_ident, ".ovf");
				pathcount--;
			}	
			if(!rfp) {
				fprintf(fp,"OV_DLLFNCEXPORT OV_RESULT %s_%s_%s(\n",
					plib->identifier,pclass->identifier,pop->identifier);
				fprintf(fp,"             OV_INSTPTR_%s_%s          pobj\n",lib_ident,class_ident);
				fprintf(fp,") {\n");
				fprintf(fp,"             return OV_ERR_OK;\n");
				fprintf(fp,"}\n\n");
			}
			else {
				while (fscanf(rfp, "%s", text1)!=EOF) {
					if (strncmp(text1, pop->cfnctypename,strlen(pop->cfnctypename))==0) {
						fprintf(fp,"OV_DLLFNCEXPORT %s %s_%s_%s",
							text2,plib->identifier,pclass->identifier,pop->identifier);
						if (text1[strlen(text1)-1] == '(') fprintf(fp,"(");
						while ( (fscanf(rfp, "%c", &text1[0])!=EOF) && (text1[0]!=';')) {
							fprintf(fp,"%c",text1[0]);
						}
						fprintf(fp," {\n");
						fprintf(fp,"             return (%s) 0;\n",text2);
						fprintf(fp,"}\n\n");
					}
					for (i=0; i<=strlen(text1);i++) text2[i]=text1[i];
				}	
				ov_builder_closefile(rfp);
			}
		}
		ov_builder_closefile(fp);
	}
	for(passoc=plib->associations; passoc; passoc=passoc->pnext) {
		fp = ov_builder_createfile(passoc->identifier, ".c");
		if(!fp) {
			return EXIT_FAILURE;
		}
		/*
		*	insert OV_COMPILE_LIBRARY option
		*/
		fprintf(fp, "\n");
		fprintf(fp, "#ifndef OV_COMPILE_LIBRARY_%s\n", plib->identifier);
		fprintf(fp, "#define OV_COMPILE_LIBRARY_%s\n", plib->identifier);
		fprintf(fp, "#endif\n");
		fprintf(fp, "\n");

		/*
		*	insert include files
		*/
		fprintf(fp, "\n");
		fprintf(fp, "#include \"%s.h\"\n", plib->identifier);
		fprintf(fp,"#include \"libov/ov_association.h\"\n\n");
		fprintf(fp,"#include \"libov/ov_macros.h\"\n\n");

		fprintf(fp,"OV_DLLFNCEXPORT OV_RESULT %s_%s_link(\n",plib->identifier,passoc->identifier);
		fprintf(fp,"        const OV_PPT_%s_%s                 pparent,\n",plib->identifier,passoc->identifier);
		fprintf(fp,"        const OV_CPT_%s_%s                 pchild,\n",plib->identifier,passoc->identifier);
		fprintf(fp,"        const OV_PLACEMENT_HINT            parenthint,\n",plib->identifier,passoc->identifier);
		fprintf(fp,"        const OV_CPT_%s_%s                 prelparent,\n",plib->identifier,passoc->identifier);
		fprintf(fp,"        const OV_PLACEMENT_HINT            childhint,\n",plib->identifier,passoc->identifier);
		fprintf(fp,"        const OV_CPT_%s_%s                 prelchild\n",plib->identifier,passoc->identifier);
		fprintf(fp,") {\n");
		fprintf(fp,"        return ov_association_link(passoc_%s_%s, Ov_PtrUpCast\n",plib->identifier,passoc->identifier);
		fprintf(fp,"                       (ov_object, pparent), Ov_PtrUpCast(ov_object, pchild),\n");
		fprintf(fp,"                      parenthint, Ov_PtrUpCast(ov_object, prelparent),\n");
		fprintf(fp,"                      childhint, Ov_PtrUpCast(ov_object, prelchild));\n");
		fprintf(fp,"}\n\n");

		fprintf(fp,"OV_DLLFNCEXPORT void %s_%s_unlink(\n",plib->identifier,passoc->identifier);
		fprintf(fp,"        const OV_PPT_%s_%s                 pparent,\n",plib->identifier,passoc->identifier);
		fprintf(fp,"        const OV_CPT_%s_%s                 pchild\n",plib->identifier,passoc->identifier);
		fprintf(fp,") {\n");
		fprintf(fp,"        ov_association_unlink(passoc_%s_%s, Ov_PtrUpCast\n",plib->identifier,passoc->identifier);
		fprintf(fp,"                      (ov_object, pparent), Ov_PtrUpCast(ov_object, pchild));\n");
		fprintf(fp,"}\n\n");

		fprintf(fp,"OV_DLLFNCEXPORT OV_ACCESS %s_%s_getaccess(\n",plib->identifier,passoc->identifier);
		fprintf(fp,"        const OV_PPT_%s_%s                 pparent,\n",plib->identifier,passoc->identifier);
		fprintf(fp,"        const OV_CPT_%s_%s                 pchild,\n",plib->identifier,passoc->identifier);
		fprintf(fp,"        const OV_TICKET                    *pticket\n");
		fprintf(fp,") {\n");
		fprintf(fp,"        return OV_AC_READ | OV_AC_LINKABLE | OV_AC_UNLINKABLE;\n");
		fprintf(fp,"}\n\n");

		ov_builder_closefile(fp);
	}

	return EXIT_SUCCESS;
}

/*	----------------------------------------------------------------------	*/

/*
*	Opens a file for reading
*/
FILE *ov_builder_openreadfile(
	OV_STRING			path,
	OV_STRING			name,
	OV_STRING			extension
) {
	FILE		*rfp;
	OV_STRING	filename;

	filename = (OV_STRING)ov_codegen_malloc(strlen(path)+1
		+strlen(name)+strlen(extension)+1);
	sprintf(filename, "%s" OV_DIRPATHDELIMITER "%s%s", path,
		name, extension);
	rfp = fopen(filename, "r");
	free(filename);
	if(!rfp) {
		return NULL;
	}
	return rfp;
}

/*	----------------------------------------------------------------------	*/

/*
*	Create a new file with a given extension
*/
FILE *ov_builder_createfile(
	OV_STRING			name,
	OV_STRING			extension
) {
	/*
	*	local variables
	*/
	FILE		*fp;
	OV_STRING	filename;
	const char  months[][4] = {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};
	time_t		current_time = time(NULL);
	struct tm	*ptime = localtime(&current_time);
	/*
	*	open file for writing
	*/
	filename = (OV_STRING)ov_codegen_malloc(strlen(outputpath)+1
		+strlen(name)+strlen(extension)+1);
	sprintf(filename, "%s" OV_DIRPATHDELIMITER "%s%s", outputpath,
		name, extension);
	fp = fopen(filename, "w");
	free(filename);
	if(!fp) {
		fprintf(stderr, "unable to open file for writing: \"%s%s\".\n",
			name, extension);
		return NULL;
	}
	/*
	*	print file information
	*/
	fprintf(fp, "/*\n");
	fprintf(fp, "*   The Framework was generated by the ACPLT/OV Builder.\n");
	fprintf(fp, "*\n");
	fprintf(fp, "*   Builder version: " OV_VER_BUILDER "\n");
	fprintf(fp, "*   Date of file generation:  %2d-%s-%4.4d (%2d:%2.2d:%2.2d)\n",
		ptime->tm_mday, months[ptime->tm_mon], ptime->tm_year+1900,
		ptime->tm_hour, ptime->tm_min, ptime->tm_sec);
	fprintf(fp, "*/\n");
	/*
	*	finished
	*/
	return fp;
}

/*	----------------------------------------------------------------------	*/

/*
*	Close an existing file
*/
int ov_builder_closefile(
	FILE	*fp
) {
	if(fclose(fp)) {
		fprintf(stderr, "error closing file.\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

/*	----------------------------------------------------------------------	*/

/*
*	Return variable type name defined in ov_ov.h for a given vartype
*/
OV_STRING ov_builder_getvartypetext(
	OV_VAR_TYPE	vartype
) {
	switch(vartype) {
	case OV_VT_BOOL:
		return "OV_BOOL";
	case OV_VT_BOOL_VEC:
		return "OV_BOOL *";
	case OV_VT_INT:
		return "OV_INT";
	case OV_VT_INT_VEC:
		return "OV_INT *";
	case OV_VT_UINT:
		return "OV_UINT";
	case OV_VT_UINT_VEC:
		return "OV_UINT *";
	case OV_VT_SINGLE:
		return "OV_SINGLE";
	case OV_VT_SINGLE_VEC:
		return "OV_SINGLE *";
	case OV_VT_DOUBLE:
		return "OV_DOUBLE";
	case OV_VT_DOUBLE_VEC:
		return "OV_DOUBLE *";
	case OV_VT_STRING:
		return "OV_STRING";
	case OV_VT_STRING_VEC:
		return "OV_STRING *";
	case OV_VT_TIME:
		return "OV_TIME";
	case OV_VT_TIME_VEC:
		return "OV_TIME *";
	case OV_VT_TIME_SPAN:
		return "OV_TIME_SPAN";
	case OV_VT_TIME_SPAN_VEC:
		return "OV_TIME_SPAN *";
	case OV_VT_BOOL_PV:
		return "OV_BOOL_PV *";
	case OV_VT_INT_PV:
		return "OV_INT_PV *";
	case OV_VT_SINGLE_PV:
		return "OV_SINGLE_PV *";
	case OV_VT_ANY:
		return "OV_ANY *";
	default:
		fprintf(stderr, "internal error -- sorry.\n");
		exit(EXIT_FAILURE);
	}
	return NULL;
}


OV_STRING ov_builder_getvartypevectext(
	OV_VAR_TYPE	vartype
) {
	switch(vartype) {
	case OV_VT_BOOL_VEC:
		return "BOOL";
	case OV_VT_INT_VEC:
		return "INT";
	case OV_VT_UINT_VEC:
		return "UINT";
	case OV_VT_SINGLE_VEC:
		return "SINGLE";
	case OV_VT_DOUBLE_VEC:
		return "DOUBLE";
	case OV_VT_STRING_VEC:
		return "STRING";
	case OV_VT_TIME_VEC:
		return "TIME";
	case OV_VT_TIME_SPAN_VEC:
		return "TIME_SPAN";
	case OV_VT_ANY:
		return "ANY";
	default:
		fprintf(stderr, "internal error -- sorry.\n");
		exit(EXIT_FAILURE);
	}
	return NULL;
}

OV_VAR_TYPE ov_codegen_getvarelementtype(
	OV_VAR_TYPE	vartype
) {
	switch(vartype) {
	case OV_VT_BOOL:
	case OV_VT_BOOL_VEC:
		return OV_VT_BOOL;
	case OV_VT_INT:
	case OV_VT_INT_VEC:
		return OV_VT_INT;
	case OV_VT_UINT:
	case OV_VT_UINT_VEC:
		return OV_VT_UINT;
	case OV_VT_SINGLE:
	case OV_VT_SINGLE_VEC:
		return OV_VT_SINGLE;
	case OV_VT_DOUBLE:
	case OV_VT_DOUBLE_VEC:
		return OV_VT_DOUBLE;
	case OV_VT_STRING:
	case OV_VT_STRING_VEC:
		return OV_VT_STRING;
	case OV_VT_TIME:
	case OV_VT_TIME_VEC:
		return OV_VT_TIME;
	case OV_VT_TIME_SPAN:
	case OV_VT_TIME_SPAN_VEC:
		return OV_VT_TIME_SPAN;
	case OV_VT_BOOL_PV:
		return OV_VT_BOOL_PV;
	case OV_VT_INT_PV:
		return OV_VT_INT_PV;
	case OV_VT_SINGLE_PV:
		return OV_VT_SINGLE_PV;
	case OV_VT_ANY:
		return OV_VT_ANY;
	default:
		fprintf(stderr, "! not an OV-datatype ! -- internal error -- sorry.\n");
		exit(EXIT_FAILURE);
	}
	return OV_VT_VOID;
}

OV_VAR_TYPE ov_codegen_getvarvectortype(
	OV_VAR_TYPE	vartype
) {
	switch(vartype) {
	case OV_VT_BOOL:
	case OV_VT_BOOL_VEC:
		return OV_VT_BOOL_VEC;
	case OV_VT_INT:
	case OV_VT_INT_VEC:
		return OV_VT_INT_VEC;
	case OV_VT_UINT:
	case OV_VT_UINT_VEC:
		return OV_VT_UINT_VEC;
	case OV_VT_SINGLE:
	case OV_VT_SINGLE_VEC:
		return OV_VT_SINGLE_VEC;
	case OV_VT_DOUBLE:
	case OV_VT_DOUBLE_VEC:
		return OV_VT_DOUBLE_VEC;
	case OV_VT_STRING:
	case OV_VT_STRING_VEC:
		return OV_VT_STRING_VEC;
	case OV_VT_TIME:
	case OV_VT_TIME_VEC:
		return OV_VT_TIME_VEC;
	case OV_VT_TIME_SPAN:
	case OV_VT_TIME_SPAN_VEC:
		return OV_VT_TIME_SPAN_VEC;
	case OV_VT_BOOL_PV:
		return OV_VT_BOOL_PV;
	case OV_VT_INT_PV:
		return OV_VT_INT_PV;
	case OV_VT_SINGLE_PV:
		return OV_VT_SINGLE_PV;
	case OV_VT_ANY:
		return OV_VT_ANY;
	default:
		fprintf(stderr, "! not an OV-datatype ! -- internal error -- sorry.\n");
		exit(EXIT_FAILURE);
	}
	return OV_VT_VOID;
}

OV_STRING ov_codegen_getvartypetextsmall(
	OV_VAR_TYPE	vartype
) {
	switch(vartype) {
	case OV_VT_BOOL:
		return "bool";
	case OV_VT_INT:
		return "int";
	case OV_VT_UINT:
		return "uint";
	case OV_VT_SINGLE:
		return "single";
	case OV_VT_DOUBLE:
		return "double";
	case OV_VT_STRING:
		return "string";
	case OV_VT_TIME:
		return "time";
	case OV_VT_TIME_SPAN:
		return "time_span";
	case OV_VT_BOOL_PV:
		return "bool_pv";
	case OV_VT_INT_PV:
		return "int_pv";
	case OV_VT_SINGLE_PV:
		return "single_pv";
	case OV_VT_ANY:
		return "any";
	default:
		fprintf(stderr, "internal error -- sorry.\n");
		exit(EXIT_FAILURE);
	}
	return NULL;
}
/*
*	End of file
*/

