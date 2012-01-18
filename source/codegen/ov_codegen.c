/*
*   $Id: ov_codegen.c,v 1.29 2010-12-20 13:23:06 martin Exp $
*
*   Copyright (C) 1998-1999
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
*	15-Jun-1998 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	16-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*	04-Nov-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: variable type ANY added.
*	07-Mai-2001 Ansgar Münnemann <ansgar@plt.rwth-aachen.de>: Bugfix: get and set parameters of static vectors.
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ov_codegen.h"

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
	OV_INT i;
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
			printf("ACPLT/OV C-Code Generator " OV_VER_CODEGEN "\n");
			return EXIT_FAILURE;
		}
		/*
		*	display help option
		*/
		else if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
HELP:		fprintf(stderr, "Usage: ov_codegen [arguments]\n"
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
			exit_status = ov_codegen_backend();
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
int ov_codegen_backend(void) {
	/*
	*	local variables
	*/
	OV_OVM_LIBRARY_DEF	*plib;
	int 				exit_status;
	/*
	*	only output for one library?
	*/
	if(libname) {
		for(plib=libraries; plib; plib=plib->pnext) {
			if(!strcmp(plib->identifier, libname)) {
				/*
				*	library found, create output for this library
				*/
				exit_status = ov_codegen_createheaderfile(plib);
				if(exit_status != EXIT_SUCCESS) {
					return exit_status;
				}
				exit_status = ov_codegen_createsourcefile(plib);
				return exit_status;
			}
		}
		/*
		*	library not found
		*/
		fprintf(stderr, "no such library: \"%s\".\n", libname);
		return EXIT_FAILURE;
	}
	/*
	*	create output for all libraries
	*/
	for(plib=libraries; plib; plib=plib->pnext) {
		/*
		*	create header file *.h
		*/
		exit_status = ov_codegen_createheaderfile(plib);
		if(exit_status != EXIT_SUCCESS) {
			return exit_status;
		}
		/*
		*	create source file *.c
		*/
		exit_status = ov_codegen_createsourcefile(plib);
		if(exit_status != EXIT_SUCCESS) {
			return exit_status;
		}
	}
	return EXIT_SUCCESS;
}
	
/*	----------------------------------------------------------------------	*/

/*
*	Create header file of a library
*/
int ov_codegen_createheaderfile(
	OV_OVM_LIBRARY_DEF*	plib
) {
	/*
	*	local variables
	*/
	FILE					*fp;
	OV_OVM_LIBRARY_DEF		*plib2;
	OV_OVM_STRUCTURE_DEF	*pstruct;
	OV_OVM_CLASS_DEF		*pclass;
	OV_OVM_ASSOCIATION_DEF	*passoc;
	OV_BOOL					found;
	OV_OVM_OPERATION_DEF	*pop;
	OV_OVM_VARIABLE_DEF		*pvar;
	/*
	*	create output file
	*/
	fp = ov_codegen_createfile(plib, ".h");
	if(!fp) {
		return EXIT_FAILURE;
	}
	/*
	*	create enclosing #ifndef ... #define ... #endif
	*/
	fprintf(fp, "#ifndef OV_LIBRARY_%s_H_INCLUDED\n", plib->identifier);
	fprintf(fp, "#define OV_LIBRARY_%s_H_INCLUDED\n", plib->identifier);
	fprintf(fp, "\n");
	/*
	*	insert include files
	*/
	fprintf(fp, "#include \"libov/ov_library.h\"\n");
	fprintf(fp, "#include \"libov/ov_structure.h\"\n");
	fprintf(fp, "#include \"libov/ov_class.h\"\n");
	fprintf(fp, "#include \"libov/ov_association.h\"\n");
	fprintf(fp, "#include \"libov/ov_variable.h\"\n");
	fprintf(fp, "#include \"libov/ov_part.h\"\n");
	fprintf(fp, "#include \"libov/ov_operation.h\"\n");
	for(plib2=libraries; plib2!=plib; plib2=plib2->pnext) {
		fprintf(fp, "#include \"%s.h\"\n", plib2->identifier);
	}
	fprintf(fp, "\n");
	/*
	*	if there could be a new C-typed variable, include *.ovt file
	*/
	found = FALSE;
	for(pstruct=plib->structures; pstruct; pstruct=pstruct->pnext) {
		for(pvar=pstruct->members; pvar; pvar=pvar->pnext) {
			if(pvar->vartype == OV_VT_CTYPE) {
				found = TRUE;
				break;
			}
		}
		if(found) {
			break;
		}
	}
	if(!found) {
		for(pclass=plib->classes; pclass; pclass=pclass->pnext) {
			for(pvar=pclass->variables; pvar; pvar=pvar->pnext) {
				if(pvar->vartype == OV_VT_CTYPE) {
					found = TRUE;
					break;
				}
			}
			if(found) {
				break;
			}
		}
	}
	if(found) {
		fprintf(fp, "#include \"%s.ovt\"\n", plib->identifier);
		fprintf(fp, "\n");
	}
	/*
	*	local definitions
	*/
	fprintf(fp, "#ifdef __cplusplus\n");
	fprintf(fp, "extern \"C\" {\n");
	fprintf(fp, "#endif\n");
	fprintf(fp, "\n");

	fprintf(fp, "#ifdef OV_COMPILE_LIBRARY_%s\n", plib->identifier);
	/* Manual fix */
	fprintf(fp, "#if OV_COMPILER_BORLAND\n");
	fprintf(fp, "#define OV_VAREXTERN extern\n");
	fprintf(fp, "#else\n");
	fprintf(fp, "#define OV_VAREXTERN OV_DLLVAREXPORT\n");
	fprintf(fp, "#endif\n");
	fprintf(fp, "#define OV_FNCEXTERN OV_DLLFNCEXPORT\n");
	fprintf(fp, "#else\n");
	fprintf(fp, "#define OV_VAREXTERN OV_DLLVARIMPORT\n");
	fprintf(fp, "#define OV_FNCEXTERN OV_DLLVARIMPORT\n");
	fprintf(fp, "#endif\n");
	fprintf(fp, "\n");
	/*
	*	print typedefs associated with all structures
	*/
	for(pstruct=plib->structures; pstruct; pstruct=pstruct->pnext) {
		ov_codegen_printstructtypedef(plib, pstruct, fp);
		fprintf(fp, "\n");
	}
	/*
	*	print defines associated with all associations
	*/
	for(passoc=plib->associations; passoc; passoc=passoc->pnext) {
		ov_codegen_printassocdefines(plib, passoc, fp);
		fprintf(fp, "\n");
	}
	/*
	*	print instance defines associated with all classes
	*/
	for(pclass=plib->classes; pclass; pclass=pclass->pnext) {
		ov_codegen_printclassinstdefines(plib, pclass, fp);
		fprintf(fp, "\n");
	}
	/*
	*	print instance typedefs associated with all classes
	*/
	for(pclass=plib->classes; pclass; pclass=pclass->pnext) {
		fprintf(fp, "OV_TYPEDEF_INSTANCE(%s_%s);\n", plib->identifier,
			pclass->identifier);
	}
	if(plib->classes) {
		fprintf(fp, "\n");
	}
	/*
	*	if there could be a new function prototype, include *.ovf file
	*/
	found = FALSE;
	for(pclass=plib->classes; pclass; pclass=pclass->pnext) {
		for(pop=pclass->operations; pop; pop=pop->pnext) {
			if(!pop->overloaded) {
				found = TRUE;
				break;
			}
		}
		if(found) {
			break;
		}
	}
	if(found) {
		fprintf(fp, "#include \"%s.ovf\"\n", plib->identifier);
		fprintf(fp, "\n");
	}
	/*
	*	print function declarations associated with all classes
	*/
	for(pclass=plib->classes; pclass; pclass=pclass->pnext) {
		ov_codegen_printclassfncdecls(plib, pclass, fp);
		if(pclass->operations) {
			fprintf(fp, "\n");
		}
	}
	/*
	*	print vtable defines associated with all classes
	*/
	for(pclass=plib->classes; pclass; pclass=pclass->pnext) {
		ov_codegen_printclassvtbldefines(plib, pclass, fp);
		fprintf(fp, "\n");
	}
	/*
	*	print vtable typedefs associated with all classes
	*/
	for(pclass=plib->classes; pclass; pclass=pclass->pnext) {
		fprintf(fp, "OV_TYPEDEF_VTABLE(%s_%s);\n", plib->identifier,
			pclass->identifier);
	}
	if(plib->classes) {
		fprintf(fp, "\n");
	}
	/*
	*	print accessor function declarations associated with all classes
	*/
	for(pclass=plib->classes; pclass; pclass=pclass->pnext) {
		ov_codegen_printclassaccessorfncdecls(plib, pclass, fp);
	}
	/*
	*	print function declarations associated with all associations
	*/
	for(passoc=plib->associations; passoc; passoc=passoc->pnext) {
		ov_codegen_printassocfncdecls(plib, passoc, fp);
	}
	/*
	*	print global variable declarations associated with all structures
	*/
	for(pstruct=plib->structures; pstruct; pstruct=pstruct->pnext) {
		fprintf(fp, "extern OV_STRUCTURE_DEF OV_STRUCTURE_DEF_%s_%s;\n", plib->identifier,
			pstruct->identifier);
		fprintf(fp, "OV_VAREXTERN OV_INSTPTR_ov_structure pstruct_%s_%s;\n",
			plib->identifier, pstruct->identifier);
		fprintf(fp, "\n");
	}
	/*
	*	print global variable declarations associated with all classes
	*/
	for(pclass=plib->classes; pclass; pclass=pclass->pnext) {
		fprintf(fp, "extern OV_CLASS_DEF OV_CLASS_DEF_%s_%s;\n", plib->identifier,
			pclass->identifier);
		fprintf(fp, "OV_VAREXTERN OV_INSTPTR_ov_class pclass_%s_%s;\n",
			plib->identifier, pclass->identifier);
		fprintf(fp, "\n");
	}
	/*
	*	print global variable declarations associated with all associations
	*/
	for(passoc=plib->associations; passoc; passoc=passoc->pnext) {
		fprintf(fp, "extern OV_ASSOCIATION_DEF OV_ASSOCIATION_DEF_%s_%s;\n", plib->identifier,
			passoc->identifier);
		fprintf(fp, "OV_VAREXTERN OV_INSTPTR_ov_association passoc_%s_%s;\n",
			plib->identifier, passoc->identifier);
		fprintf(fp, "\n");
	}
	/*
	*	print global variable declaration associated with the library
	*/
	fprintf(fp, "extern OV_LIBRARY_DEF OV_LIBRARY_DEF_%s;\n", plib->identifier);
	fprintf(fp, "\n");
	/*
	*	print function prototype for setting global vars and opening library
	*/
	fprintf(fp, "OV_RESULT ov_library_setglobalvars_%s(void);\n",
		plib->identifier);
	fprintf(fp, "\n");
	fprintf(fp, "OV_DLLFNCEXPORT OV_LIBRARY_DEF *" OV_CONST_OPENFNC_PREFIX "%s(void);\n",
		plib->identifier);
	fprintf(fp, "\n");
	/*
	*	local definitions
	*/
	fprintf(fp, "#undef OV_VAREXTERN\n");
	fprintf(fp, "#undef OV_FNCEXTERN\n");
	fprintf(fp, "\n");
	fprintf(fp, "#ifdef __cplusplus\n");
	fprintf(fp, "}\n");
	fprintf(fp, "#endif\n");
	fprintf(fp, "\n");
	/*
	*	Create enclosing #ifndef ... #define ... #endif
	*/
	fprintf(fp, "#endif\n");
	/*
	*	free temporary memory and close file
	*/
	return ov_codegen_closefile(fp);
}

/*	----------------------------------------------------------------------	*/

/*
*	Create initialvalues subfunctions in setglobalvars function in source file of a library
*/
void ov_codegen_printinitvalueelemdefobj(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	OV_OVM_VARIABLE_DEF	*pvar,
	OV_VAR_VALUE		*pvalue,
	OV_UINT			num,
	FILE			*fp
) {
	if (!plib || !pclass || !pvar || !fp) exit(-1);
	switch (pvalue->vartype) {
		case OV_VT_SINGLE:
			if (pvalue) fprintf(fp, " %f ", (OV_SINGLE) pvalue->valueunion.val_double);
			break;
		case OV_VT_DOUBLE:
			if (pvalue) fprintf(fp, " %e ", pvalue->valueunion.val_double);
			break;
		case OV_VT_INT:
			if (pvalue) fprintf(fp, " %ld ", pvalue->valueunion.val_int);
			break;
		case OV_VT_UINT:
			if (pvalue) fprintf(fp, " %lu ", pvalue->valueunion.val_uint);
			break;
		case OV_VT_BOOL:
			if (pvalue) fprintf(fp, pvalue->valueunion.val_bool ? " 1 " : " 0 ");
			break;
		case OV_VT_TIME:
			if (pvalue) fprintf(fp, " { %lu , %lu } ", pvalue->valueunion.val_time.secs,pvalue->valueunion.val_time.usecs);
			break;
		case OV_VT_TIME_SPAN:
			if (pvalue) fprintf(fp, " { %ld , %ld } ", pvalue->valueunion.val_time.secs,pvalue->valueunion.val_time.usecs);
			break;
		case OV_VT_STRING:
			fprintf(fp, " OV_INITSTRING_DEF_%s_%s_%s_%ld ", plib->identifier,
				pclass->identifier, pvar->identifier, num);
			break;
		default:
			break;
	}
}

void ov_codegen_printinitvaluedef(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	OV_OVM_VARIABLE_DEF	*pvar,
	FILE			*fp
) {
	OV_OVM_STRUCTURE_DEF	*pstruct;

	if (pvar->pinitvalue->value.vartype == OV_VT_STRING) {
		fprintf(fp, "            pvar->v_initialvalue.value.vartype = OV_VT_STRING;\n");
		fprintf(fp, "            pvar->v_initialvalue.value.valueunion.val_string = ");
		ov_codegen_printinitvalueelemdefobj(plib, pclass, pvar, &pvar->pinitvalue->value, pvar->pinitvalue->num, fp);
		fprintf(fp, ";\n");
	}
	else if (pvar->pinitvalue->pstructelem) {
		pstruct = ov_codegen_getstructdef(plib, pvar->structurename);
		fprintf(fp, "            pvar->v_initialvalue.value.vartype = OV_VT_STRUCT;\n");
		fprintf(fp, "            pvar->v_initialvalue.value.valueunion.val_byte_vec.veclen = sizeof(OV_STRUCT_%s_%s);\n", pstruct->libname, pstruct->identifier);
		fprintf(fp, "            pvar->v_initialvalue.value.valueunion.val_byte_vec.value  = &OV_INITSTRUCT_DEF_%s_%s_%s_%lu;\n",plib->identifier, pclass->identifier, pvar->identifier, pvar->pinitvalue->num);
	}
	else if (pvar->pinitvalue->pvectorelem) {
		fprintf(fp, "            pvar->v_initialvalue.value.vartype = %s;\n", ov_codegen_getfulltypetext( (pvar->vartype & OV_VT_KSMASK) ) );
		fprintf(fp, "            pvar->v_initialvalue.value.valueunion.val_%s.veclen = %lu;\n", ov_codegen_getvartypetextsmall( (pvar->vartype & OV_VT_KSMASK) ), pvar->pinitvalue->value.valueunion.val_generic_vec.veclen);
		fprintf(fp, "            pvar->v_initialvalue.value.valueunion.val_%s.value  = OV_INITVECTOR_DEF_%s_%s_%s_%ld;\n", ov_codegen_getvartypetextsmall(pvar->pinitvalue->value.vartype),plib->identifier, pclass->identifier, pvar->identifier, pvar->pinitvalue->num);
	}
	else {
		fprintf(fp, "            pvar->v_initialvalue.value.vartype = %s;\n", ov_codegen_getfulltypetext( (pvar->vartype & OV_VT_KSMASK) ) );
		if( (pvar->pinitvalue->value.vartype == OV_VT_TIME)||(pvar->pinitvalue->value.vartype == OV_VT_TIME_SPAN)) {
			fprintf(fp, "            pvar->v_initialvalue.value.valueunion.val_%s.secs = %ld;\n", ov_codegen_getvartypetextsmall( (pvar->vartype & OV_VT_KSMASK) ),pvar->pinitvalue->value.valueunion.val_time.secs );
			fprintf(fp, "            pvar->v_initialvalue.value.valueunion.val_%s.usecs = %ld", ov_codegen_getvartypetextsmall(pvar->pinitvalue->value.vartype),pvar->pinitvalue->value.valueunion.val_time.usecs );
		}
		else {
			fprintf(fp, "            pvar->v_initialvalue.value.valueunion.val_%s =", ov_codegen_getvartypetextsmall( (pvar->vartype & OV_VT_KSMASK) ) );
			ov_codegen_printinitvalueelemdefobj(plib, pclass, pvar, &pvar->pinitvalue->value, 0, fp);
		}
		fprintf(fp, ";\n");
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	Create source file of a library
*/
int ov_codegen_createsourcefile(
	OV_OVM_LIBRARY_DEF*	plib
) {
	/*
	*	local variables
	*/
	FILE					*fp;
	OV_OVM_STRUCTURE_DEF	*pstruct;
	OV_OVM_CLASS_DEF		*pclass;
	OV_OVM_VARIABLE_DEF		*pvar;
	OV_OVM_ASSOCIATION_DEF	*passoc;
	OV_BOOL			 usedinitvalue=FALSE;
	/*
	*	create output file
	*/
	fp = ov_codegen_createfile(plib, ".c");
	if(!fp) {
		return EXIT_FAILURE;
	}
	/*
	*	insert include files
	*/
	fprintf(fp, "#ifndef OV_COMPILE_LIBRARY_%s\n", plib->identifier);
	fprintf(fp, "#define OV_COMPILE_LIBRARY_%s\n", plib->identifier);
	fprintf(fp, "#endif\n");
	fprintf(fp, "\n");
	fprintf(fp, "#include \"%s.h\"\n", plib->identifier);
	fprintf(fp, "\n");
	/*
	*	print global variable definitions associated with all structures
	*/
	for(pstruct=plib->structures; pstruct; pstruct=pstruct->pnext) {
		fprintf(fp, "OV_DLLVAREXPORT OV_INSTPTR_ov_structure pstruct_%s_%s = NULL;\n",
			plib->identifier, pstruct->identifier);
	}
	if(plib->structures) {
		fprintf(fp, "\n");
	}
	/*
	*	print global variable definitions associated with all classes
	*/
	for(pclass=plib->classes; pclass; pclass=pclass->pnext) {
		fprintf(fp, "OV_DLLVAREXPORT OV_INSTPTR_ov_class pclass_%s_%s = NULL;\n",
			plib->identifier, pclass->identifier);
	}
	if(plib->classes) {
		fprintf(fp, "\n");
	}
	/*
	*	print global variable definitions associated with all associations
	*/
	for(passoc=plib->associations; passoc; passoc=passoc->pnext) {
		fprintf(fp, "OV_DLLVAREXPORT OV_INSTPTR_ov_association passoc_%s_%s = NULL;\n",
			plib->identifier, passoc->identifier);
	}
	if(plib->associations) {
		fprintf(fp, "\n");
	}
	/*
	*	print vtable definions associated with all classes
	*/
	for(pclass=plib->classes; pclass; pclass=pclass->pnext) {
		ov_codegen_printclassvtbldef(plib, pclass, fp);
		fprintf(fp, "\n");
	}
	/*
	*	print definition objects associated with all structures
	*/
	for(pstruct=plib->structures; pstruct; pstruct=pstruct->pnext) {
		ov_codegen_printstructdefobjs(plib, pstruct, fp);
		fprintf(fp, "\n");
	}
	/*
	*	print definition objects associated with all classes
	*/
	for(pclass=plib->classes; pclass; pclass=pclass->pnext) {
		ov_codegen_printclassdefobjs(plib, pclass, fp);
		fprintf(fp, "\n");
	}
	/*
	*	print definition objects associated with all associations
	*/
	for(passoc=plib->associations; passoc; passoc=passoc->pnext) {
		ov_codegen_printassocdefobj(plib, passoc, fp);
		fprintf(fp, "\n");
	}
	/*
	*	print library definition object associated with the library
	*/
	fprintf(fp, "OV_LIBRARY_DEF OV_LIBRARY_DEF_%s = {\n", plib->identifier);
	fprintf(fp, "    \"%s\",\n", plib->identifier);
	fprintf(fp, "    %s,\n", ov_codegen_getstringtext(plib->version));
	fprintf(fp, "    \"%s\",\n", OV_VER_LIBOV);
	fprintf(fp, "    %s,\n", ov_codegen_getstringtext(plib->author));
	fprintf(fp, "    %s,\n", ov_codegen_getstringtext(plib->copyright));
	fprintf(fp, "    %s,\n", ov_codegen_getstringtext(plib->comment));
	if(plib->structures) {
		fprintf(fp, "    &OV_STRUCTURE_DEF_%s_%s,\n", plib->identifier,
			plib->structures->identifier);
	} else {
		fprintf(fp, "    (OV_STRUCTURE_DEF*)0,\n");
	}
	if(plib->classes) {
		fprintf(fp, "    &OV_CLASS_DEF_%s_%s,\n", plib->identifier,
			plib->classes->identifier);
	} else {
		fprintf(fp, "    (OV_CLASS_DEF*)0,\n");
	}
	if(plib->associations) {
		fprintf(fp, "    &OV_ASSOCIATION_DEF_%s_%s,\n", plib->identifier,
			plib->associations->identifier);
	} else {
		fprintf(fp, "    (OV_ASSOCIATION_DEF*)0,\n");
	}
	fprintf(fp, "    ov_library_setglobalvars_%s\n", plib->identifier);
	fprintf(fp, "};\n");
	fprintf(fp, "\n");
	/*
	*	insert include
	*/
	fprintf(fp, "#include \"libov/ov_macros.h\"\n");
	fprintf(fp, "\n");
	/*
	*	print implementation of function setting global variables
	*/
	fprintf(fp, "OV_RESULT ov_library_setglobalvars_%s(void) {\n", plib->identifier);
	fprintf(fp, "    OV_INSTPTR_ov_library plib;\n");
	for(pclass=plib->classes; pclass; pclass=pclass->pnext) {
		for (pvar=pclass->variables; pvar; pvar=pvar->pnext) {
			if (pvar->pinitvalue) {
				usedinitvalue = TRUE;
				fprintf(fp, "    OV_INSTPTR_ov_variable pvar;\n\n");
			}
			if (usedinitvalue) break;
		}
		if (usedinitvalue) break;
	}
	fprintf(fp, "    Ov_ForEachChildEx(ov_instantiation, pclass_ov_library, plib, ov_library) {\n");
	fprintf(fp, "        if(!strcmp(plib->v_identifier, \"%s\")) {\n", plib->identifier);
	for(pstruct=plib->structures; pstruct; pstruct=pstruct->pnext) {
		fprintf(fp, "            pstruct_%s_%s = Ov_SearchChildEx(ov_containment, plib, \"%s\", ov_structure);\n",
			plib->identifier, pstruct->identifier, pstruct->identifier);
		fprintf(fp, "            if(!pstruct_%s_%s) {\n",
			plib->identifier, pstruct->identifier);
		fprintf(fp, "                return OV_ERR_GENERIC;\n");
		fprintf(fp, "            }\n");
	}
	for(pclass=plib->classes; pclass; pclass=pclass->pnext) {
		fprintf(fp, "            pclass_%s_%s = Ov_SearchChildEx(ov_containment, "
			"plib, \"%s\", ov_class);\n", plib->identifier, pclass->identifier, 
			pclass->identifier);
		fprintf(fp, "            if(!pclass_%s_%s) {\n", plib->identifier, 
			pclass->identifier);
		fprintf(fp, "                return OV_ERR_GENERIC;\n");
		fprintf(fp, "            }\n");
		for (pvar=pclass->variables; pvar; pvar=pvar->pnext) {
			if (pvar->pinitvalue) {
				fprintf(fp, "            pvar = Ov_SearchChildEx(ov_containment, "
					"pclass_%s_%s, \"%s\", ov_variable);\n", plib->identifier, pclass->identifier, 
					pvar->identifier);
				ov_codegen_printinitvaluedef(plib, pclass, pvar, fp);
			}
		}
	}
	for(passoc=plib->associations; passoc; passoc=passoc->pnext) {
		fprintf(fp, "            passoc_%s_%s = Ov_SearchChildEx(ov_containment, "
			"plib, \"%s\", ov_association);\n", plib->identifier, 
			passoc->identifier, passoc->identifier);
		fprintf(fp, "            if(!passoc_%s_%s) {\n",
			plib->identifier, passoc->identifier);
		fprintf(fp, "                return OV_ERR_GENERIC;\n");
		fprintf(fp, "            }\n");
	}
	fprintf(fp, "            return OV_ERR_OK;\n");
	fprintf(fp, "        }\n");
	fprintf(fp, "    }\n");
	fprintf(fp, "    return OV_ERR_GENERIC;\n");
	fprintf(fp, "}\n");
	fprintf(fp, "\n");
	/*
	*	print implementation of function opening the library
	*/
	fprintf(fp, "OV_DLLFNCEXPORT OV_LIBRARY_DEF *" OV_CONST_OPENFNC_PREFIX "%s(void) {\n", plib->identifier);
	fprintf(fp, "    return &OV_LIBRARY_DEF_%s;\n", plib->identifier);
	fprintf(fp, "}\n");
	/*
	*	close file
	*/
	return ov_codegen_closefile(fp);
}

/*	----------------------------------------------------------------------	*/

/*
*	Create a new file with a given extension
*/
FILE *ov_codegen_createfile(
	OV_OVM_LIBRARY_DEF	*plib,
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
		+strlen(plib->identifier)+strlen(extension)+1);
	sprintf(filename, "%s" OV_DIRPATHDELIMITER "%s%s", outputpath,
		plib->identifier, extension);
	fp = fopen(filename, "w");
	free(filename);
	if(!fp) {
		fprintf(stderr, "unable to open file for writing: \"%s%s\".\n",
			plib->identifier, extension);
		return NULL;
	}
	/*
	*	print file information
	*/
	fprintf(fp, "/*\n");
	fprintf(fp, "*   This file was generated by the ACPLT/OV C-Code Generator.\n");
	fprintf(fp, "*\n");
	fprintf(fp, "*   C-Code Generator version: " OV_VER_CODEGEN "\n");
	fprintf(fp, "*   ACPLT/OV library version: " OV_VER_LIBOV "\n");
	fprintf(fp, "*   Date of file generation:  %2d-%s-%4.4d (%2d:%2.2d:%2.2d)\n",
		ptime->tm_mday, months[ptime->tm_mon], ptime->tm_year+1900,
		ptime->tm_hour, ptime->tm_min, ptime->tm_sec);
	fprintf(fp, "*\n");
	fprintf(fp, "*   DO NOT EDIT THIS FILE!\n");
	fprintf(fp, "*/\n");
	fprintf(fp, "\n");
	/*
	*	finished
	*/
	return fp;
}

/*	----------------------------------------------------------------------	*/

/*
*	Close an existing file
*/
int ov_codegen_closefile(
	FILE	*fp
) {
	fprintf(fp, "\n");
	fprintf(fp, "/*\n");
	fprintf(fp, "*   End of file\n");
	fprintf(fp, "*/\n");
	if(fclose(fp)) {
		fprintf(stderr, "error closing file.\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

/*	----------------------------------------------------------------------	*/

/*
*	Print typedef associated with a structure
*/
void ov_codegen_printstructtypedef(
	OV_OVM_LIBRARY_DEF		*plib,
	OV_OVM_STRUCTURE_DEF	*pstruct,
	FILE					*fp
) {
	/*
	*	local variables
	*/
	OV_OVM_VARIABLE_DEF	*pvar;
	/*
	*	print typedef of instance data of a structure
	*/
	fprintf(fp, "typedef struct {\n");
	for(pvar=pstruct->members; pvar; pvar=pvar->pnext) {
		if(pvar->veclen) {
			/*
			*	scalar or vector of fixed length
			*/
			switch(pvar->vartype) {
			case OV_VT_STRUCT:
				fprintf(fp, "    OV_STRUCT_%s v_%s", ov_codegen_replace(pvar->structurename),
					pvar->identifier);
				break;
			case OV_VT_CTYPE:
				fprintf(fp, "    %s v_%s", pvar->ctypename, pvar->identifier);
				break;
			default:
				fprintf(fp, "    %s v_%s", ov_codegen_getvartypetext(pvar->vartype),
					pvar->identifier);
				break;
			}
			if(pvar->veclen > 1) {
				fprintf(fp, "[%lu]", pvar->veclen);
			}
			fprintf(fp, ";\n");
		} else {
			/*
			*	vector of dynamic length
			*/
			fprintf(fp, "    %s_VEC v_%s;\n", ov_codegen_getvartypetext(pvar->vartype),
				pvar->identifier);
		}
	}
	fprintf(fp, "}   OV_STRUCT_%s_%s;\n", plib->identifier, pstruct->identifier);
}

/*	----------------------------------------------------------------------	*/

/*
*	Print defines associated with an associations
*/
void ov_codegen_printassocdefines(
	OV_OVM_LIBRARY_DEF		*plib,
	OV_OVM_ASSOCIATION_DEF	*passoc,
	FILE					*fp
) {
	/*
	*	information about parent and child pointer types and stuff
	*/
	fprintf(fp, "#define OV_PPT_%s_%s struct OV_INST_%s*\n", plib->identifier,
		passoc->identifier, ov_codegen_replace(passoc->parentclassname));
	fprintf(fp, "#define OV_CPT_%s_%s struct OV_INST_%s*\n", plib->identifier,
		passoc->identifier, ov_codegen_replace(passoc->childclassname));
	fprintf(fp, "#define OV_PCI_%s_%s is_of_class_%s\n", plib->identifier,
		passoc->identifier, ov_codegen_replace(passoc->parentclassname));
	fprintf(fp, "#define OV_CCI_%s_%s is_of_class_%s\n", plib->identifier,
		passoc->identifier, ov_codegen_replace(passoc->childclassname));
}

/*	----------------------------------------------------------------------	*/

/*
*	Print instance defines associated with a class
*/
void ov_codegen_printclassinstdefines(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	FILE				*fp
) {
	/*
	*	local variables
	*/
	OV_OVM_VARIABLE_DEF		*pvar;
	OV_OVM_PART_DEF			*ppart;
	/*
	*	information about baseclass instance and vtable pointers
	*/
	if(pclass->baseclassname) {
		fprintf(fp, "#define OV_BIPT_%s_%s OV_INSTPTR_%s\n", plib->identifier,
			pclass->identifier, ov_codegen_replace(pclass->baseclassname));
		fprintf(fp, "#define OV_BVPT_%s_%s OV_VTBLPTR_%s\n", plib->identifier,
			pclass->identifier, ov_codegen_replace(pclass->baseclassname));
		fprintf(fp, "\n");
	}
	/*
	*	defines for realizing single inheritance
	*/
	fprintf(fp, "#define OV_INSTBODY_%s_%s", plib->identifier, pclass->identifier);
	if(pclass->baseclassname) {
		fprintf(fp, " \\\n    OV_INSTBODY_%s", ov_codegen_replace(pclass->baseclassname));
	}
	/*
	*	include new variables in define
	*/
	for(pvar = pclass->variables; pvar; pvar=pvar->pnext) {
		if(!(pvar->varprops & (OV_VP_DERIVED | OV_VP_STATIC))) {
			if(pvar->veclen) {
				/*
				*	scalar or vector of fixed length
				*/
				switch(pvar->vartype) {
				case OV_VT_STRUCT:
					fprintf(fp, " \\\n    OV_STRUCT_%s v_%s",
						 ov_codegen_replace(pvar->structurename), pvar->identifier);
					break;
				case OV_VT_CTYPE:
					fprintf(fp, " \\\n    %s v_%s", pvar->ctypename, pvar->identifier);
					break;
				default:
					fprintf(fp, " \\\n    %s v_%s",
						 ov_codegen_getvartypetext(pvar->vartype),	pvar->identifier);
					break;
				}
				if(pvar->veclen > 1) {
					fprintf(fp, "[%lu]", pvar->veclen);
				}
				fprintf(fp,";");
			} else {
				/*
				*	vector of dynamic length
				*/
				fprintf(fp, " \\\n    %s_VEC v_%s;", ov_codegen_getvartypetext(pvar->vartype),
					pvar->identifier);
			}
		}
	}
	/*
	*	include instance data of new parts in define
	*/
	for(ppart=pclass->parts; ppart; ppart=ppart->pnext) {
		fprintf(fp, " \\\n    OV_INST_%s p_%s;",
			ov_codegen_replace(ppart->partclassname), ppart->identifier);
	}
	fprintf(fp, "\n");
	fprintf(fp, "\n");
	/*
	*	defines with class information
	*/
	fprintf(fp, "#define OV_CIB_%s_%s \\\n", plib->identifier, pclass->identifier);
	if(pclass->baseclassname) {
		fprintf(fp, "    OV_CIB_%s \\\n", ov_codegen_replace(pclass->baseclassname));
	}
	fprintf(fp, "    int is_of_class_%s_%s:1;\n", plib->identifier, pclass->identifier);
	fprintf(fp, "\n");
	/*
	*	defines for realizing static variables
	*/
	fprintf(fp, "#define OV_STATICINSTBODY_%s_%s", plib->identifier, pclass->identifier);
	/*
	*	include new static variables in define
	*/
	for(pvar = pclass->variables; pvar; pvar=pvar->pnext) {
		if(pvar->varprops & OV_VP_STATIC) {
			if(pvar->veclen) {
				/*
				*	scalar or vector of fixed length
				*/
				switch(pvar->vartype) {
				case OV_VT_STRUCT:
					fprintf(fp, " \\\n    OV_STRUCT_%s v_%s", ov_codegen_replace(
						pvar->structurename), pvar->identifier);
					break;
				case OV_VT_CTYPE:
					fprintf(fp, " \\\n    %s v_%s", pvar->ctypename, pvar->identifier);
					break;
				default:
					fprintf(fp, " \\\n    %s v_%s", ov_codegen_getvartypetext(
						pvar->vartype),	pvar->identifier);
					break;
				}
				if(pvar->veclen > 1) {
					fprintf(fp, "[%lu]", pvar->veclen);
				}
				fprintf(fp,";");
			} else {
				/*
				*	vector of dynamic length
				*/
				fprintf(fp, " \\\n    %s_VEC v_%s;", ov_codegen_getvartypetext(pvar->vartype),
					pvar->identifier);
			}
		}
	}
	fprintf(fp, "\n");
}

/*	----------------------------------------------------------------------	*/

/*
*	Print function declaration associated with a class
*/
void ov_codegen_printclassfncdecls(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	FILE				*fp
) {
	/*
	*	local variables
	*/
	OV_OVM_OPERATION_DEF	*pop;
	/*
	*	function prototypes
	*/
	for(pop=pclass->operations; pop; pop=pop->pnext) {
		fprintf(fp, "OV_FNCEXTERN %s %s_%s_%s;\n", pop->cfnctypename,
			plib->identifier, pclass->identifier, pop->identifier);
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	Print vtable defines associated with a class
*/
void ov_codegen_printclassvtbldefines(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	FILE				*fp
) {
	/*
	*	local variables
	*/
	OV_OVM_OPERATION_DEF	*pop;
	/*
	*	defines for realizing single inheritance
	*/
	fprintf(fp, "#define OV_VTBLBODY_%s_%s", plib->identifier, pclass->identifier);
	if(pclass->baseclassname) {
		fprintf(fp, " \\\n    OV_VTBLBODY_%s", ov_codegen_replace(pclass->baseclassname));
	}
	for(pop=pclass->operations; pop; pop=pop->pnext) {
		if(!(pop->overloaded)) {
			fprintf(fp, " \\\n    %s* m_%s;", pop->cfnctypename, pop->identifier);
		}
	}
	fprintf(fp, "\n");
}

/*	----------------------------------------------------------------------	*/

/*
*	Print accessor function declarations associated with a class
*/
void ov_codegen_printclassaccessorfncdecls(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	FILE				*fp
) {
	/*
	*	local variables
	*/
	OV_OVM_VARIABLE_DEF	*pvar;
	/*
	*	iterate of all variables of the class
	*/
	for(pvar=pclass->variables; pvar; pvar=pvar->pnext) {
		/*
		*	declare get accessor if appropriate
		*/
		if(pvar->varprops & OV_VP_GETACCESSOR) {
			fprintf(fp, "OV_DLLFNCEXPORT ");
			switch(pvar->vartype) {
			case OV_VT_STRUCT:
				fprintf(fp, "OV_STRUCT_%s *", ov_codegen_replace(pvar->structurename));
				break;
			case OV_VT_CTYPE:
				fprintf(fp, "%s *", pvar->ctypename);
				break;
			case OV_VT_TIME:
			case OV_VT_TIME_SPAN:
			case OV_VT_BOOL_PV:
			case OV_VT_INT_PV:
			case OV_VT_UINT_PV:
			case OV_VT_SINGLE_PV:
			case OV_VT_DOUBLE_PV:
			case OV_VT_STRING_PV:
			case OV_VT_TIME_PV:
			case OV_VT_TIME_SPAN_PV:
			case OV_VT_ANY:
				fprintf(fp, "%s *", ov_codegen_getvartypetext(pvar->vartype));
				break;
			default:
				fprintf(fp, "%s", ov_codegen_getvartypetext(pvar->vartype));
				if(pvar->veclen != 1) {
				    if( pvar->vartype & OV_VT_HAS_STATE ) {
					   fprintf(fp, "_VEC");
					}
				}
				fprintf(fp, " ");
				if(pvar->veclen != 1) {
					fprintf(fp, "*");
				}
				break;
			}
			fprintf(fp, "%s_%s_%s_get(\n", plib->identifier,
				pclass->identifier, pvar->identifier);
			fprintf(fp, "    OV_INSTPTR_%s_%s pobj", plib->identifier,
				pclass->identifier);
			if( (pvar->veclen!=1) && (!(pvar->vartype & OV_VT_HAS_STATE)) ) {
				fprintf(fp, ",\n    OV_UINT *pveclen");
			}
			fprintf(fp, "\n);\n");
			fprintf(fp, "\n");
		}
		/*
		*	declare set accessor if apropriate
		*/
		if(pvar->varprops & OV_VP_SETACCESSOR) {
			fprintf(fp, "OV_DLLFNCEXPORT OV_RESULT %s_%s_%s_set(\n",
				plib->identifier, pclass->identifier, pvar->identifier);
			fprintf(fp, "    OV_INSTPTR_%s_%s pobj,\n", plib->identifier,
				pclass->identifier);
			switch(pvar->vartype) {
			case OV_VT_STRUCT:
				fprintf(fp, "    const OV_STRUCT_%s *p", ov_codegen_replace(pvar->structurename));
				break;
			case OV_VT_TIME:
			case OV_VT_TIME_SPAN:
			case OV_VT_BOOL_PV:
			case OV_VT_INT_PV:
			case OV_VT_UINT_PV:
			case OV_VT_SINGLE_PV:
			case OV_VT_DOUBLE_PV:
			case OV_VT_STRING_PV:
			case OV_VT_TIME_PV:
			case OV_VT_TIME_SPAN_PV:
			case OV_VT_ANY:
				fprintf(fp, "    const %s *p", ov_codegen_getvartypetext(pvar->vartype));
				break;
			case OV_VT_CTYPE:
				fprintf(fp, "    const %s *p", pvar->ctypename);
				break;
			default:
				fprintf(fp, "    const %s", ov_codegen_getvartypetext(pvar->vartype));
				if(pvar->veclen != 1) {
    			    if( pvar->vartype & OV_VT_HAS_STATE ) {
    				   fprintf(fp, "_VEC");
    				}
    			}
    			fprintf(fp, " ");
				if(pvar->veclen != 1) {
					fprintf(fp, "*p");
				}
				break;
			}
			fprintf(fp, "value");
			if( (pvar->veclen!=1) && (!(pvar->vartype & OV_VT_HAS_STATE)) ) {
				fprintf(fp, ",\n    const OV_UINT veclen");
			}
			fprintf(fp, "\n);\n");
			fprintf(fp, "\n");
		}
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	Print function declarations associated with an association
*/
void ov_codegen_printassocfncdecls(
	OV_OVM_LIBRARY_DEF		*plib,
	OV_OVM_ASSOCIATION_DEF	*passoc,
	FILE					*fp
) {
	fprintf(fp, "OV_DECL_LINK(%s_%s);\n",  plib->identifier, passoc->identifier);
	fprintf(fp, "OV_DECL_UNLINK(%s_%s);\n",  plib->identifier, passoc->identifier);
	fprintf(fp, "OV_DECL_GETACCESS(%s_%s);\n",  plib->identifier, passoc->identifier);
	fprintf(fp, "\n");
}

/*	----------------------------------------------------------------------	*/

/*
*	Print vtable definion associated with a class
*/
void ov_codegen_printclassvtbldef(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	FILE				*fp
) {
	/*
	*	local variables
	*/
	OV_UINT					i;
	OV_OVM_OPERATION_DEF	*pop;
	OV_OVM_CLASS_DEF		*pclass2;
	OV_OVM_LIBRARY_DEF		*plib2;
	/*
	*	definition of the vtable of the class
	*/
	fprintf(fp, "OV_VTBL_%s_%s OV_VTABLE_%s_%s = {\n", plib->identifier,
		pclass->identifier, plib->identifier, pclass->identifier);
	for(i=0; i<pclass->numoperations; i++) {
		pop = ov_codegen_getopdefbynum(pclass, i);
		if(pop->opprops & OV_OP_ABSTRACT) {
			fprintf(fp, "    (%s*)0", pop->cfnctypename);
		} else {
			pclass2 = ov_codegen_getclassdef(NULL, pop->classname);
			if(!pclass2) {
				fprintf(stderr, "Class '%s' not found.\n", pop->classname);
				exit(EXIT_FAILURE);
			}
			plib2 = ov_codegen_getlibdef(pclass2->libname);
			if(!plib2) {
				fprintf(stderr, "Library '%s' not found.\n", pclass2->libname);
				exit(EXIT_FAILURE);
			}
			fprintf(fp, "    %s_%s_%s", plib2->identifier, pclass2->identifier,
				pop->identifier);
		}
		if(i < pclass->numoperations-1) {
			fprintf(fp, ",\n");
		} else {
			fprintf(fp, "\n");
		}
	}
	fprintf(fp, "};\n");
}

/*	----------------------------------------------------------------------	*/

/*
*	Print definition objects associated with a structure
*/
void ov_codegen_printstructdefobjs(
	OV_OVM_LIBRARY_DEF		*plib,
	OV_OVM_STRUCTURE_DEF	*pstruct,
	FILE					*fp
) {
	/*
	*	local variables
	*/
	OV_OVM_VARIABLE_DEF	*pvar;
	OV_UINT				i;
	/*
	*	print definition objects associated with all members
	*/
	i = 0;
	fprintf(fp, "OV_VARIABLE_DEF OV_VARIABLE_DEF_%s_%s[] = {{\n", plib->identifier,
		pstruct->identifier);
	for(pvar=pstruct->members; pvar; pvar=pvar->pnext) {
		i++;
		ov_codegen_printmemberdefobj(plib, pstruct, pvar, i, fp);
		if(pvar->pnext) {
			fprintf(fp, "}, {\n");
		}
	}
	fprintf(fp, "}};\n");
	fprintf(fp, "\n");
	/*
	*	print definition object of the structure
	*/
	fprintf(fp, "OV_STRUCTURE_DEF OV_STRUCTURE_DEF_%s_%s = {\n", plib->identifier,
		pstruct->identifier);
	if(pstruct->pnext) {
		fprintf(fp, "    &OV_STRUCTURE_DEF_%s_%s,\n", plib->identifier,
		pstruct->pnext->identifier);
	} else {
		fprintf(fp, "    (OV_STRUCTURE_DEF*)0,\n");
	}
	fprintf(fp, "    \"%s\",\n", pstruct->identifier);
	fprintf(fp, "    sizeof(OV_STRUCT_%s_%s),\n", plib->identifier,
		pstruct->identifier);
	fprintf(fp, "    &OV_VARIABLE_DEF_%s_%s[0]\n", plib->identifier,
		pstruct->identifier); 
	fprintf(fp, "};\n");
}

/*	----------------------------------------------------------------------	*/

/*
*	Print definition objects associated with a class
*/
void ov_codegen_printclassdefobjs(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	FILE				*fp
) {
	/*
	*	local variables
	*/
	OV_OVM_VARIABLE_DEF		*pvar;
	OV_OVM_PART_DEF			*ppart;
	OV_OVM_OPERATION_DEF	*pop;
	OV_UINT					i;
	/*
	*	print definition objects associated with all initialvalues of the variables
	*/
	ov_codegen_printinitstringsdefobj(plib, pclass, fp);
	ov_codegen_printinitvecdefobj(plib, pclass, fp);
	ov_codegen_printinitstructdefobj(plib, pclass, fp);

	/*
	*	print definition objects associated with all variables
	*/
	if(pclass->variables) {
		i = 0;
		fprintf(fp, "OV_VARIABLE_DEF OV_VARIABLE_DEF_%s_%s[] = {{\n", plib->identifier,
			pclass->identifier);
		for(pvar=pclass->variables; pvar; pvar=pvar->pnext) {
			i++;
			ov_codegen_printvardefobj(plib, pclass, pvar, i, fp);
			if(pvar->pnext) {
				fprintf(fp, "}, {\n");
			}
		}
		fprintf(fp, "}};\n");
		fprintf(fp, "\n");
	}
	/*
	*	print definition objects associated with all parts
	*/
	if(pclass->parts) {
		i = 0;
		fprintf(fp, "OV_PART_DEF OV_PART_DEF_%s_%s[] = {{\n", plib->identifier,
		pclass->identifier);
		for(ppart=pclass->parts; ppart; ppart=ppart->pnext) {
			i++;
			ov_codegen_printpartdefobj(plib, pclass, ppart, i, fp);
			if(ppart->pnext) {
				fprintf(fp, "}, {\n");
			}
		}
		fprintf(fp, "}};\n");
		fprintf(fp, "\n");
	}
	/*
	*	print definition objects associated with all operations
	*/
	if(pclass->operations) {
		i = 0;
		fprintf(fp, "OV_OPERATION_DEF OV_OPERATION_DEF_%s_%s[] = {{\n", plib->identifier,
			pclass->identifier);
		for(pop=pclass->operations; pop; pop=pop->pnext) {
			i++;
			ov_codegen_printopdefobj(plib, pclass, pop, i, fp);
			if(pop->pnext) {
				fprintf(fp, "}, {\n");
			}
		}
		fprintf(fp, "}};\n");
		fprintf(fp, "\n");
	}
	/*
	*	print definition object of the class
	*/
	fprintf(fp, "OV_CLASS_DEF OV_CLASS_DEF_%s_%s = {\n", plib->identifier,
		pclass->identifier);
	if(pclass->pnext) {
		fprintf(fp, "    &OV_CLASS_DEF_%s_%s,\n", plib->identifier,
			pclass->pnext->identifier);
	} else {
		fprintf(fp, "    (OV_CLASS_DEF*)0,\n");
	}
	fprintf(fp, "    \"%s\",\n", pclass->identifier);
	if(pclass->baseclassname) {
		fprintf(fp, "    \"%s\",\n", pclass->baseclassname); 
	} else {
		fprintf(fp, "    (OV_STRING)NULL,\n");
	}
	fprintf(fp, "    %d,\n", pclass->classprops);
	fprintf(fp, "    %lu,\n", pclass->flags);
	fprintf(fp, "    %s,\n", ov_codegen_getstringtext(pclass->comment));
	fprintf(fp, "    Ov_GetInstSize(%s_%s),\n", plib->identifier,
		pclass->identifier);
	fprintf(fp, "    Ov_GetStaticInstSize(%s_%s),\n", plib->identifier,
		pclass->identifier);
	fprintf(fp, "    (OV_POINTER)&OV_VTABLE_%s_%s,\n", plib->identifier,
		pclass->identifier);
	if(pclass->variables) {
		fprintf(fp, "    &OV_VARIABLE_DEF_%s_%s[0],\n", plib->identifier,
			pclass->identifier); 
	} else {
		fprintf(fp, "    (OV_VARIABLE_DEF*)0,\n");
	}
	if(pclass->parts) {
		fprintf(fp, "    &OV_PART_DEF_%s_%s[0],\n", plib->identifier,
			pclass->identifier); 
	} else {
		fprintf(fp, "    (OV_PART_DEF*)0,\n");
	}
	if(pclass->operations) {
		fprintf(fp, "    &OV_OPERATION_DEF_%s_%s[0]\n", plib->identifier,
			pclass->identifier); 
	} else {
		fprintf(fp, "    (OV_OPERATION_DEF*)0\n");
	}
	fprintf(fp, "};\n");
}

/*	----------------------------------------------------------------------	*/

/*
*	Print definition object associated with an association
*/
void ov_codegen_printassocdefobj(
	OV_OVM_LIBRARY_DEF		*plib,
	OV_OVM_ASSOCIATION_DEF	*passoc,
	FILE					*fp
) {
	fprintf(fp, "OV_ASSOCIATION_DEF OV_ASSOCIATION_DEF_%s_%s = {\n",
		plib->identifier, passoc->identifier);
	if(passoc->pnext) {
		fprintf(fp, "    &OV_ASSOCIATION_DEF_%s_%s,\n", plib->identifier,
			passoc->pnext->identifier);
	} else {
		fprintf(fp, "    (OV_ASSOCIATION_DEF*)0,\n");
	}
	fprintf(fp, "    \"%s\",\n", passoc->identifier);
	fprintf(fp, "    %d,\n", passoc->assoctype);
	fprintf(fp, "    %d,\n", passoc->assocprops);
	fprintf(fp, "    \"%s\",\n", passoc->childrolename);
	fprintf(fp, "    \"%s\",\n", passoc->parentclassname);
	fprintf(fp, "    \"%s\",\n", passoc->parentrolename);
	fprintf(fp, "    \"%s\",\n", passoc->childclassname);
	fprintf(fp, "    %s,\n", ov_codegen_getstringtext(passoc->parentcomment));
	fprintf(fp, "    %s,\n", ov_codegen_getstringtext(passoc->childcomment));
	fprintf(fp, "    0,\n");
	fprintf(fp, "    0,\n");
	fprintf(fp, "    %lu,\n", passoc->parentflags);
	fprintf(fp, "    %lu,\n", passoc->childflags);
	fprintf(fp, "    (OV_FNCPTR_LINK)%s_%s_link,\n", plib->identifier,
		passoc->identifier);
	fprintf(fp, "    (OV_FNCPTR_UNLINK)%s_%s_unlink,\n", plib->identifier,
		passoc->identifier);
	fprintf(fp, "    (OV_FNCPTR_GETACCESS)%s_%s_getaccess\n", plib->identifier,
		passoc->identifier);
	fprintf(fp, "};\n");
}

/*	----------------------------------------------------------------------	*/

/*
*	Print definition object associated with a member of a structure
*/
void ov_codegen_printmemberdefobj(
	OV_OVM_LIBRARY_DEF		*plib,
	OV_OVM_STRUCTURE_DEF	*pstruct,
	OV_OVM_VARIABLE_DEF		*pvar,
	OV_UINT					i,
	FILE					*fp
) {
	if(pvar->pnext) {
		fprintf(fp, "    &OV_VARIABLE_DEF_%s_%s[%lu],\n", plib->identifier,
			pstruct->identifier, i);
	} else {
		fprintf(fp, "    (OV_VARIABLE_DEF*)0,\n");
	}
	fprintf(fp ,"    \"%s\",\n", pvar->identifier);
	fprintf(fp ,"    %lu,\n", pvar->veclen);
	fprintf(fp ,"    %d,\n", pvar->varprops);
	fprintf(fp ,"    %d,\n", pvar->vartype);
	switch(pvar->vartype) {
	case OV_VT_STRUCT:
		fprintf(fp, "    (OV_STRING)NULL,\n");
		fprintf(fp, "    \"%s\",\n", pvar->structurename);
		fprintf(fp, "    %lu*sizeof(OV_STRUCT_%s),\n", pvar->veclen,
			ov_codegen_replace(pvar->structurename));
		break;
	case OV_VT_CTYPE:
		fprintf(fp, "    \"%s\",\n", pvar->ctypename);
		fprintf(fp, "    (OV_STRING)NULL,\n");
		fprintf(fp, "    %lu*sizeof(%s),\n", pvar->veclen, pvar->ctypename);
		break;
	default:
		if (pvar->veclen == 0) {
			fprintf(fp, "    (OV_STRING)NULL,\n");
			fprintf(fp, "    (OV_STRING)NULL,\n");
			fprintf(fp, "    sizeof(%s_VEC),\n",
				ov_codegen_getvartypetext(pvar->vartype));
		}
		else {
			fprintf(fp, "    (OV_STRING)NULL,\n");
			fprintf(fp, "    (OV_STRING)NULL,\n");
			fprintf(fp, "    %lu*sizeof(%s),\n", pvar->veclen,
				ov_codegen_getvartypetext(pvar->vartype));
		}
		break;
	}
	fprintf(fp, "    offsetof(OV_STRUCT_%s_%s, v_%s),\n", plib->identifier,
		pstruct->identifier, pvar->identifier);
	fprintf(fp, "    %lu,\n", pvar->flags);
	fprintf(fp, "    %s,\n", ov_codegen_getstringtext(pvar->tech_unit));
	fprintf(fp, "    %s,\n", ov_codegen_getstringtext(pvar->comment));
	fprintf(fp, "    NULL,\n");
	fprintf(fp, "    NULL\n");
}

/*	----------------------------------------------------------------------	*/

/*
*	Print definition objects associated with the initialvalues of a variable of a class
*/
void ov_codegen_iterate_strings(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	OV_OVM_VARIABLE_DEF	*pvar,
	OV_OVM_INITVALUE_DEF	*pval,
	OV_UINT			*num,
	FILE			*fp
) {
	while (pval) {
		if (pval->value.vartype == OV_VT_STRING) {
			fprintf(fp, "char OV_INITSTRING_DEF_%s_%s_%s_%lu[] = %s;\n", plib->identifier,
				pclass->identifier, pvar->identifier, *num, pval->value.valueunion.val_string);
			pval->num = *num;
			(*num)++;
		}
		if (pval->pstructelem) ov_codegen_iterate_strings(plib, pclass, pvar, pval->pstructelem, num, fp);
		if (pval->pvectorelem) ov_codegen_iterate_strings(plib, pclass, pvar, pval->pvectorelem, num, fp);
		pval=pval->pnext;
	}
}

void ov_codegen_printinitstringsdefobj(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	FILE			*fp
) {
	OV_OVM_VARIABLE_DEF	*pvar;
	OV_OVM_INITVALUE_DEF	*pvalue;
	OV_UINT			num;

	if(pclass->variables) {
		for(pvar=pclass->variables; pvar; pvar=pvar->pnext) {
			if (pvar->pinitvalue) {
				num = 1;
				pvalue = pvar->pinitvalue;
				ov_codegen_iterate_strings(plib, pclass, pvar, pvalue, &num, fp);
			}
		}
	}
	fprintf(fp, "\n");
}

void ov_codegen_iterate_vectors(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	OV_OVM_VARIABLE_DEF	*pvar,
	OV_OVM_INITVALUE_DEF	*pval,
	OV_UINT			*num,
	FILE			*fp
) {
	OV_OVM_INITVALUE_DEF		*pvec;

	while (pval) {
		if (pval->pvectorelem) {
			fprintf(fp, "%s OV_INITVECTOR_DEF_%s_%s_%s_%lu [] = {\n", ov_codegen_getvartypetext(pval->value.vartype) ,plib->identifier,
				pclass->identifier, pvar->identifier, *num);
			pval->num = *num;
			(*num)++;
			pvec = pval->pvectorelem;
			while (pvec) {
				if (pvec->value.vartype == OV_VT_STRING) ov_codegen_printinitvalueelemdefobj(plib, pclass, pvar, &pvec->value, pvec->num, fp);
				else ov_codegen_printinitvalueelemdefobj(plib, pclass, pvar, &pvec->value, 0, fp);
				if (pvec->pnext) fprintf(fp, " ,\n");
				else fprintf(fp, "\n");
				pvec = pvec->pnext;
			}
			fprintf(fp, "};\n\n");
		}
		if (pval->pstructelem) ov_codegen_iterate_vectors(plib, pclass, pvar, pval->pstructelem, num, fp);
		pval=pval->pnext;
	}
}

void ov_codegen_printinitvecdefobj(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	FILE			*fp
) {
	OV_OVM_VARIABLE_DEF		*pvar;
	OV_OVM_INITVALUE_DEF		*pvalue;
	OV_UINT				num;

	if(pclass->variables) {
		for(pvar=pclass->variables; pvar; pvar=pvar->pnext) {
			if (pvar->pinitvalue) {
				pvalue = pvar->pinitvalue;
				num = 1;
				ov_codegen_iterate_vectors(plib, pclass, pvar, pvalue, &num, fp);
			}
		}
	}
}

void ov_codegen_iterate_structs (
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	OV_OVM_VARIABLE_DEF	*pvar,
	OV_OVM_STRUCTURE_DEF	*pstruct,
	OV_OVM_INITVALUE_DEF	*pval,
	OV_UINT			*num,
	FILE			*fp
) {
	OV_OVM_VARIABLE_DEF		*pmem;
	OV_OVM_INITVALUE_DEF		*pstr;
	OV_OVM_INITVALUE_DEF		*pvec;
	OV_UINT				i;

	while (pval) {
		if (pval->pstructelem) {
			fprintf(fp, "OV_STRUCT_%s_%s OV_INITSTRUCT_DEF_%s_%s_%s_%lu\n", pstruct->libname, pstruct->identifier,
				plib->identifier, pclass->identifier, pvar->identifier, *num);
			pval->num = *num;
			(*num)++;
			pstr = pval->pstructelem;
			while (pstr) {
				if (pstr->value.vartype == OV_VT_STRING) ov_codegen_printinitvalueelemdefobj(plib, pclass, pvar, &pstr->value, pstr->num, fp);
				else if (pstr->pvectorelem) {
					pmem = ov_codegen_getmemberdef(plib, pstruct, pstr->identifier);
					if (pmem->veclen > 1) {
						pvec = pstr->pvectorelem;
						for (i=0; (i<pmem->veclen) && (pvec);i++, pvec = pvec->pnext)
							ov_codegen_printinitvalueelemdefobj(plib, pclass, pvar, &pvec->value, 0, fp);
					}
					else 	{
							fprintf(fp, "    { %lu , ", pstr->value.valueunion.val_generic_vec.veclen);
							fprintf(fp, "OV_INITVECTOR_DEF_%s_%s_%s_%lu }", plib->identifier, pclass->identifier, pvar->identifier, pstr->num);
						}
				     }
				else ov_codegen_printinitvalueelemdefobj(plib, pclass, pvar, &pstr->value, 0, fp);
				if (pstr->pnext) fprintf(fp, " ,\n");
				else fprintf(fp, "\n");
				pstr = pstr->pnext;
			}
			fprintf(fp, "};\n\n");
		}
		pval=pval->pnext;
	}
}

void ov_codegen_printinitstructdefobj(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	FILE			*fp
) {
	OV_OVM_VARIABLE_DEF		*pvar;
	OV_OVM_INITVALUE_DEF		*pvalue;
	OV_OVM_STRUCTURE_DEF		*pstruct;
	OV_UINT				num;


	if(pclass->variables) {
		for(pvar=pclass->variables; pvar; pvar=pvar->pnext) {
			if (pvar->pinitvalue) {
				pvalue = pvar->pinitvalue;
				if (pvar->structurename) {
					pstruct = ov_codegen_getstructdef(plib, pvar->structurename);
					ov_codegen_iterate_structs(plib, pclass, pvar, pstruct, pvalue, &num, fp);
				}
			}
		}
	}
}


/*	----------------------------------------------------------------------	*/

/*
*	Print definition object associated with a variable of a class
*/
void ov_codegen_printvardefobj(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	OV_OVM_VARIABLE_DEF	*pvar,
	OV_UINT				i,
	FILE				*fp
) {
	if(pvar->pnext) {
		fprintf(fp, "    &OV_VARIABLE_DEF_%s_%s[%lu],\n", plib->identifier,
			pclass->identifier, i);
	} else {
		fprintf(fp, "    (OV_VARIABLE_DEF*)0,\n");
	}
	fprintf(fp ,"    \"%s\",\n", pvar->identifier);
	fprintf(fp ,"    %lu,\n", pvar->veclen);
	fprintf(fp ,"    %d,\n", pvar->varprops);
	fprintf(fp ,"    %d,\n", pvar->vartype);
	switch(pvar->vartype) {
	case OV_VT_STRUCT:
		fprintf(fp, "    (OV_STRING)NULL,\n");
		fprintf(fp, "    \"%s\",\n", pvar->structurename);
		fprintf(fp, "    %lu*sizeof(OV_STRUCT_%s),\n", pvar->veclen,
			ov_codegen_replace(pvar->structurename));
		break;
	case OV_VT_CTYPE:
		fprintf(fp, "    \"%s\",\n", pvar->ctypename);
		fprintf(fp, "    (OV_STRING)NULL,\n");
		fprintf(fp, "    %lu*sizeof(%s),\n", pvar->veclen, pvar->ctypename);
		break;
	default:
		if (pvar->veclen == 0) {
			fprintf(fp, "    (OV_STRING)NULL,\n");
			fprintf(fp, "    (OV_STRING)NULL,\n");
			fprintf(fp, "    sizeof(%s_VEC),\n",
				ov_codegen_getvartypetext(pvar->vartype));
		}
		else {
			fprintf(fp, "    (OV_STRING)NULL,\n");
			fprintf(fp, "    (OV_STRING)NULL,\n");
			fprintf(fp, "    %lu*sizeof(%s),\n", pvar->veclen,
				ov_codegen_getvartypetext(pvar->vartype));
		}
		break;
	}
	if(pvar->varprops & OV_VP_DERIVED) {
		fprintf(fp, "    0,\n");
	} else if(pvar->varprops & OV_VP_STATIC) {
		fprintf(fp ,"    offsetof(OV_STATICINST_%s_%s, v_%s),\n", plib->identifier,
			pclass->identifier, pvar->identifier);
	} else {
		fprintf(fp ,"    offsetof(OV_INST_%s_%s, v_%s),\n", plib->identifier,
			pclass->identifier, pvar->identifier);
	}
	fprintf(fp, "    %lu,\n", pvar->flags);
	fprintf(fp ,"    %s,\n", ov_codegen_getstringtext(pvar->tech_unit));
	fprintf(fp ,"    %s,\n", ov_codegen_getstringtext(pvar->comment));
	if(pvar->varprops & OV_VP_GETACCESSOR) {
		fprintf(fp, "    (OV_FNCPTR_GET)%s_%s_%s_get,\n", plib->identifier,
			pclass->identifier, pvar->identifier);
	} else {
		fprintf(fp, "    NULL,\n");
	}
	if(pvar->varprops & OV_VP_SETACCESSOR) {
		fprintf(fp, "    (OV_FNCPTR_SET)%s_%s_%s_set\n", plib->identifier,
			pclass->identifier, pvar->identifier);
	} else {
		fprintf(fp, "    NULL\n");
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	Print definition object associated with a part of a class
*/
void ov_codegen_printpartdefobj(
	OV_OVM_LIBRARY_DEF	*plib,
	OV_OVM_CLASS_DEF	*pclass,
	OV_OVM_PART_DEF		*ppart,
	OV_UINT				i,
	FILE				*fp
) {
	if(ppart->pnext) {
		fprintf(fp, "    &OV_PART_DEF_%s_%s[%lu],\n", plib->identifier,
			pclass->identifier, i);
	} else {
		fprintf(fp, "    (OV_PART_DEF*)0,\n");
	}
	fprintf(fp ,"    \"%s\",\n", ppart->identifier);
	fprintf(fp ,"    \"%s\",\n", ppart->partclassname);
	fprintf(fp ,"     %lu,\n", ppart->flags);
	fprintf(fp ,"    offsetof(OV_INST_%s_%s, p_%s)\n", plib->identifier,
		pclass->identifier, ppart->identifier);
}

/*	----------------------------------------------------------------------	*/

/*
*	Print definition object associated with an operation of a class
*/
void ov_codegen_printopdefobj(
	OV_OVM_LIBRARY_DEF		*plib,
	OV_OVM_CLASS_DEF		*pclass,
	OV_OVM_OPERATION_DEF	*pop,
	OV_UINT					i,
	FILE					*fp
) {
	if(pop->pnext) {
		fprintf(fp, "    &OV_OPERATION_DEF_%s_%s[%lu],\n", plib->identifier,
			pclass->identifier, i);
	} else {
		fprintf(fp, "    (OV_OPERATION_DEF*)0,\n");
	}
	fprintf(fp ,"    \"%s\",\n", pop->identifier);
	fprintf(fp ,"    %d,\n", pop->opprops);
	fprintf(fp ,"    \"%s\"\n", pop->cfnctypename);
}

/*	----------------------------------------------------------------------	*/

/*
*	Return variable type name defined in ov_ov.h for a given vartype
*/
OV_STRING ov_codegen_getvartypetext(
	OV_VAR_TYPE	vartype
) {
	switch(vartype) {
	case OV_VT_BOOL:
	case OV_VT_BOOL_VEC:
		return "OV_BOOL";
	case OV_VT_INT:
	case OV_VT_INT_VEC:
		return "OV_INT";
	case OV_VT_UINT:
	case OV_VT_UINT_VEC:
		return "OV_UINT";
	case OV_VT_SINGLE:
	case OV_VT_SINGLE_VEC:
		return "OV_SINGLE";
	case OV_VT_DOUBLE:
	case OV_VT_DOUBLE_VEC:
		return "OV_DOUBLE";
	case OV_VT_STRING:
	case OV_VT_STRING_VEC:
		return "OV_STRING";
	case OV_VT_TIME:
	case OV_VT_TIME_VEC:
		return "OV_TIME";
	case OV_VT_TIME_SPAN:
	case OV_VT_TIME_SPAN_VEC:
		return "OV_TIME_SPAN";
		
	case OV_VT_ANY:
		return "OV_ANY";
		
	case OV_VT_BOOL_PV:
		return "OV_BOOL_PV";
	case OV_VT_INT_PV:
		return "OV_INT_PV";
	case OV_VT_UINT_PV:
		return "OV_UINT_PV";
	case OV_VT_SINGLE_PV:
		return "OV_SINGLE_PV";
	case OV_VT_DOUBLE_PV:
		return "OV_DOUBLE_PV";
	case OV_VT_STRING_PV:
		return "OV_STRING_PV";
	case OV_VT_TIME_PV:
		return "OV_TIME_PV";
	case OV_VT_TIME_SPAN_PV:
		return "OV_TIME_SPAN_PV";
    
    /* PV vectors are not supported */
    
	default:
		fprintf(stderr, "unknown variable type.\n");
		exit(EXIT_FAILURE);
	}
	return NULL;
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
		
	case OV_VT_BOOL_VEC:
		return "bool_vec";
	case OV_VT_INT_VEC:
		return "int_vec";
	case OV_VT_UINT_VEC:
		return "uint_vec";
	case OV_VT_SINGLE_VEC:
		return "single_vec";
	case OV_VT_DOUBLE_VEC:
		return "double_vec";
	case OV_VT_STRING_VEC:
		return "string_vec";
	case OV_VT_TIME_VEC:
		return "time_vec";
	case OV_VT_TIME_SPAN_VEC:
		return "time_span_vec";

	case OV_VT_BOOL_PV:
		return "bool";
	case OV_VT_INT_PV:
		return "int";
	case OV_VT_UINT_PV:
		return "uint";
	case OV_VT_SINGLE_PV:
		return "single";
	case OV_VT_DOUBLE_PV:
		return "double";
	case OV_VT_STRING_PV:
		return "string";
	case OV_VT_TIME_PV:
		return "time";
	case OV_VT_TIME_SPAN_PV:
		return "time_span";

	default:
		fprintf(stderr, "unknown variable type.\n");
		exit(EXIT_FAILURE);
	}
	return NULL;
}

OV_STRING ov_codegen_getfulltypetext(
	OV_VAR_TYPE	vartype
) {
	switch(vartype) {
	case OV_VT_BOOL:
		return "OV_VT_BOOL";
	case OV_VT_INT:
		return "OV_VT_INT";
	case OV_VT_UINT:
		return "OV_VT_UINT";
	case OV_VT_SINGLE:
		return "OV_VT_SINGLE";
	case OV_VT_DOUBLE:
		return "OV_VT_DOUBLE";
	case OV_VT_STRING:
		return "OV_VT_STRING";
	case OV_VT_TIME:
		return "OV_VT_TIME";
	case OV_VT_TIME_SPAN:
		return "OV_VT_TIME_SPAN";
		
	case OV_VT_BOOL_VEC:
		return "OV_VT_BOOL_VEC";
	case OV_VT_INT_VEC:
		return "OV_VT_INT_VEC";
	case OV_VT_UINT_VEC:
		return "OV_VT_UINT_VEC";
	case OV_VT_SINGLE_VEC:
		return "OV_VT_SINGLE_VEC";
	case OV_VT_DOUBLE_VEC:
		return "OV_VT_DOUBLE_VEC";
	case OV_VT_STRING_VEC:
		return "OV_VT_STRING_VEC";
	case OV_VT_TIME_VEC:
		return "OV_VT_TIME_VEC";
	case OV_VT_TIME_SPAN_VEC:
		return "OV_VT_TIME_SPAN_VEC";
			
	case OV_VT_ANY:
		return "OV_VT_ANY";
		
		
	case OV_VT_BOOL_PV:
		return "OV_VT_BOOL_PV";
	case OV_VT_INT_PV:
		return "OV_VT_INT_PV";
	case OV_VT_UINT_PV:
		return "OV_VT_UINT_PV";
	case OV_VT_SINGLE_PV:
		return "OV_VT_SINGLE_PV";
	case OV_VT_DOUBLE_PV:
		return "OV_VT_DOUBLE_PV";
	case OV_VT_STRING_PV:
		return "OV_VT_STRING_PV";
	case OV_VT_TIME_PV:
		return "OV_VT_TIME_PV";
	case OV_VT_TIME_SPAN_PV:
		return "OV_VT_TIME_SPAN_PV";
		
	case OV_VT_BOOL_PV_VEC:
		return "OV_VT_BOOL_PV_VEC";
	case OV_VT_INT_PV_VEC:
		return "OV_VT_INT_PV_VEC";
	case OV_VT_UINT_PV_VEC:
		return "OV_VT_UINT_PV_VEC";
	case OV_VT_SINGLE_PV_VEC:
		return "OV_VT_SINGLE_PV_VEC";
	case OV_VT_DOUBLE_PV_VEC:
		return "OV_VT_DOUBLE_PV_VEC";
	case OV_VT_STRING_PV_VEC:
		return "OV_VT_STRING_PV_VEC";
	case OV_VT_TIME_PV_VEC:
		return "OV_VT_TIME_PV_VEC";
	case OV_VT_TIME_SPAN_PV_VEC:
		return "OV_VT_TIME_SPAN_PV_VEC";
		
	default:
		fprintf(stderr, "unknown variable type.\n");
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
    	case OV_VT_BOOL_PV:
    	case OV_VT_BOOL_PV_VEC:
    		return OV_VT_BOOL;
    	case OV_VT_INT:
    	case OV_VT_INT_VEC:
    	case OV_VT_INT_PV:
    	case OV_VT_INT_PV_VEC:
    		return OV_VT_INT;
    	case OV_VT_UINT:
    	case OV_VT_UINT_VEC:
    	case OV_VT_UINT_PV:
    	case OV_VT_UINT_PV_VEC:
    		return OV_VT_UINT;
    	case OV_VT_SINGLE:
    	case OV_VT_SINGLE_VEC:
    	case OV_VT_SINGLE_PV:
    	case OV_VT_SINGLE_PV_VEC:
    		return OV_VT_SINGLE;
    	case OV_VT_DOUBLE:
    	case OV_VT_DOUBLE_VEC:
    	case OV_VT_DOUBLE_PV:
    	case OV_VT_DOUBLE_PV_VEC:
    		return OV_VT_DOUBLE;
    	case OV_VT_STRING:
    	case OV_VT_STRING_VEC:
    	case OV_VT_STRING_PV:
    	case OV_VT_STRING_PV_VEC:
    		return OV_VT_STRING;
    	case OV_VT_TIME:
    	case OV_VT_TIME_VEC:
    	case OV_VT_TIME_PV:
    	case OV_VT_TIME_PV_VEC:
    		return OV_VT_TIME;
    	case OV_VT_TIME_SPAN:
    	case OV_VT_TIME_SPAN_VEC:
    	case OV_VT_TIME_SPAN_PV:
    	case OV_VT_TIME_SPAN_PV_VEC:
    		return OV_VT_TIME_SPAN;		
    	case OV_VT_ANY:
    		return OV_VT_ANY;
    	default:
    		fprintf(stderr, "! not an OV-datatype ! -- internal error -- .\n");
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
	case OV_VT_BOOL_PV:
	case OV_VT_BOOL_PV_VEC:
		return OV_VT_BOOL_VEC;
	case OV_VT_INT:
	case OV_VT_INT_VEC:
	case OV_VT_INT_PV:
	case OV_VT_INT_PV_VEC:
		return OV_VT_INT_VEC;
	case OV_VT_UINT:
	case OV_VT_UINT_VEC:
	case OV_VT_UINT_PV:
	case OV_VT_UINT_PV_VEC:
		return OV_VT_UINT_VEC;
	case OV_VT_SINGLE:
	case OV_VT_SINGLE_VEC:
	case OV_VT_SINGLE_PV:
	case OV_VT_SINGLE_PV_VEC:
		return OV_VT_SINGLE_VEC;
	case OV_VT_DOUBLE:
	case OV_VT_DOUBLE_VEC:
	case OV_VT_DOUBLE_PV:
	case OV_VT_DOUBLE_PV_VEC:
		return OV_VT_DOUBLE_VEC;
	case OV_VT_STRING:
	case OV_VT_STRING_VEC:
	case OV_VT_STRING_PV:
	case OV_VT_STRING_PV_VEC:
		return OV_VT_STRING_VEC;
	case OV_VT_TIME:
	case OV_VT_TIME_VEC:
	case OV_VT_TIME_PV:
	case OV_VT_TIME_PV_VEC:
		return OV_VT_TIME_VEC;
	case OV_VT_TIME_SPAN:
	case OV_VT_TIME_SPAN_VEC:
	case OV_VT_TIME_SPAN_PV:
	case OV_VT_TIME_SPAN_PV_VEC:
		return OV_VT_TIME_SPAN_VEC;
	case OV_VT_ANY:
		return OV_VT_ANY;
	default:
		fprintf(stderr, "! not an OV-datatype ! -- internal error -- .\n");
		exit(EXIT_FAILURE);
	}
	return OV_VT_VOID;
}

/*	----------------------------------------------------------------------	*/

/*
*	Return text defining a string
*/
OV_STRING ov_codegen_getstringtext(
	OV_STRING	string
) {
	if(string) {
		return string;
	}
	return "(OV_STRING)NULL";
}

/*	----------------------------------------------------------------------	*/

/*
*	Replace '/' with '_' in an identifier
*/
OV_STRING ov_codegen_replace(
	OV_STRING	identifier
) {
	/*
	*	local variables
	*/
	OV_STRING	id, pc;
	/*
	*	replace '/' with '_'
	*/
	if(!identifier) {
		return NULL;
	}
	id = ov_codegen_getstring(identifier, strlen(identifier));
	for(pc=id; *pc; pc++) {
		if(*pc == '/') {
			*pc ='_';
			break;
		}
	}
	return id;
}

/*
*	Get Library part in the lib/class-name 
*/
OV_STRING ov_codegen_getlibname(
	OV_STRING	identifier
) {
	/*
	*	local variables
	*/
	OV_STRING	id, pc;
	/*
	*	search for '/'
	*/
	id = ov_codegen_getstring(identifier, strlen(identifier));
	for(pc = id; *pc; pc++) {
		if(*pc == '/') {
			*pc = 0;
			break;
		}
	}
	return id;
}
/*
*	Get Class part in the lib/class-name 
*/
OV_STRING ov_codegen_getclassname(
	OV_STRING	identifier
) {
	/*
	*	local variables
	*/
	OV_STRING	id, pc;
	/*
	*	search for '/'
	*/
	id = ov_codegen_getstring(identifier, strlen(identifier));
	for(pc = id; *pc; pc++) {
		if(*pc == '/') {
			pc++;
			break;
		}
	}
	return pc;
}
/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

