/* -*-plt-c++-*- */
/******************************************************************************
***                                                                         ***
***   iFBSpro   -   Makefile Generator                                      ***
***   ################################                                      ***
***                                                                         ***
***   L T S o f t                                                           ***
***   Agentur für Leittechnik Software GmbH                                 ***
***   Brabanterstr. 13                                                      ***
***   D-50171 Kerpen                                                        ***
***   Tel : 02237/92869-2                                                   ***
***   Fax : 02237/92869-9                                                   ***
***   e-Mail   : ltsoft@ltsoft.de                                           ***
***   Internet : http://www.ltsoft.de                                       ***
***                                                                         ***
*******************************************************************************
*                                                                             *
*   Datei                                                                     *
*   -----                                                                     *
*   acplt_builder.c                                                           *
*                                                                             *
*                                                                             *
*   Historie                                                                  *
*   --------                                                                  *
*   2007-04-09 Alexander Neugebauer: Erstellung, LTSoft GmbH, Kerpen          *
*   2011-06-21 Sten Gruener: Anpassung an MinGW, bugfixes, ACPLT	      *
*                                                                             *
*   Beschreibung                                                              *
*   ------------                                                              *
*   Generator der Source-Dateien fuer Benutzer-Bibliothek                     *
*                                                                             *
******************************************************************************/

#include <ctype.h>
#include <time.h>

#include "definitions.h"

#include "ov_codegen.h"

/*	----------------------------------------------------------------------	*/

/*
*	Global variables
*/

char            outputpath[MAX_INCLUDED_FILES + 1];

char            *libname = NULL;
extern int	yydebug;
extern FILE*	yyin;
int 		addOpenLib = 0;

char        	*libs[MAX_INCLUDED_FILES + 1];
int        	 anzAddLibs = 0;


/*
 *	Common Functions
 *	------------
 */

#include "common.h"

/*	----------------------------------------------------------------------	*/
/*
*	Opens a file for reading
*/
FILE *fb_builder_openreadfile(
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
FILE *fb_builder_createfile(
	OV_STRING			name,
	OV_STRING			extension
) {
	/*
	*	local variables
	*/
	FILE		*fp;
	OV_STRING	filename;
	time_t		current_time = time(NULL);
	struct tm	*ptime = localtime(&current_time);

	/*
	*	open file for writing
	*/
	filename = (OV_STRING)ov_codegen_malloc(strlen(outputpath)+
		+strlen(name)+strlen(extension)+5);
	if(!filename) {
		fprintf(stderr, "Out of memory.\n");
        return NULL;
    }
	sprintf(filename, "%s/%s%s", outputpath, name, extension);
	compatiblePath(filename);

	fp = fopen(filename, "r");
	if(fp) {
		fprintf(stderr, "File '%s%s' already exists.\n",
			name, extension);
		free(filename);
		fclose(fp);
		return NULL;
	}

	fp = fopen(filename, "w");
	free(filename);
	if(!fp) {
		fprintf(stderr, "unable to open file '%s%s' for writing.\n",
			name, extension);
		return NULL;
	}
	/*
	*	print file information
	*/
	fprintf(fp, "\n");
    fprintf(fp, "/******************************************************************************\n");
    fprintf(fp, "*\n");
    fprintf(fp, "*   FILE\n");
    fprintf(fp, "*   ----\n");
    fprintf(fp, "*   %s%s\n", name, extension);
    fprintf(fp, "*\n");
    fprintf(fp, "*   History\n");
    fprintf(fp, "*   -------\n");
	fprintf(fp, "*   %4.4d-%2.2d-%2.2d   File created\n", ptime->tm_year+1900, ptime->tm_mon+1, ptime->tm_mday);
    fprintf(fp, "*\n");
    fprintf(fp, "*******************************************************************************\n");
    fprintf(fp, "*\n");
    fprintf(fp, "*   This file is generated by the 'acplt_builder' command\n");
    fprintf(fp, "*\n");
    fprintf(fp, "******************************************************************************/\n");
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
int fb_builder_closefile(
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
*	Return value type of getAccessor
*/
OV_STRING fb_builder_getResultValueType(
	OV_VAR_TYPE	vartype
) {
	switch(vartype) {
	case OV_VT_BOOL:
		return "OV_BOOL";
	case OV_VT_INT:
		return "OV_INT";
	case OV_VT_UINT:
		return "OV_UINT";
	case OV_VT_SINGLE:
		return "OV_SINGLE";
	case OV_VT_DOUBLE:
		return "OV_DOUBLE";
	case OV_VT_STRING:
		return "OV_STRING";
	case OV_VT_TIME:
		return "OV_TIME*";
	case OV_VT_TIME_SPAN:
		return "OV_TIME_SPAN*";
		
	case OV_VT_BOOL_VEC:
		return "OV_BOOL*";
	case OV_VT_INT_VEC:
		return "OV_INT*";
	case OV_VT_UINT_VEC:
		return "OV_UINT*";
	case OV_VT_SINGLE_VEC:
		return "OV_SINGLE*";
	case OV_VT_DOUBLE_VEC:
		return "OV_DOUBLE*";
	case OV_VT_STRING_VEC:
		return "OV_STRING*";
	case OV_VT_TIME_VEC:
		return "OV_TIME*";
	case OV_VT_TIME_SPAN_VEC:
		return "OV_TIME_SPAN*";
		
	case OV_VT_BOOL_PV:
		return "OV_BOOL_PV*";
	case OV_VT_INT_PV:
		return "OV_INT_PV*";
	case OV_VT_UINT_PV:
		return "OV_UINT_PV*";
	case OV_VT_SINGLE_PV:
		return "OV_SINGLE_PV*";
	case OV_VT_DOUBLE_PV:
		return "OV_DOUBLE_PV*";
	case OV_VT_STRING_PV:
		return "OV_STRING_PV*";
	case OV_VT_TIME_PV:
		return "OV_TIME_PV*";
	case OV_VT_TIME_SPAN_PV:
		return "OV_TIME_SPAN_PV*";

	case OV_VT_BOOL_PV_VEC:
		return "OV_BOOL_PV_VEC*";
	case OV_VT_INT_PV_VEC:
		return "OV_INT_PV_VEC*";
	case OV_VT_UINT_PV_VEC:
		return "OV_UINT_PV_VEC*";
	case OV_VT_SINGLE_PV_VEC:
		return "OV_SINGLE_PV_VEC*";
	case OV_VT_DOUBLE_PV_VEC:
		return "OV_DOUBLE_PV_VEC*";
	case OV_VT_STRING_PV_VEC:
		return "OV_STRING_PV_VEC*";
	case OV_VT_TIME_PV_VEC:
		return "OV_TIME_PV_VEC*";
	case OV_VT_TIME_SPAN_PV_VEC:
		return "OV_TIME_SPAN_PV_VEC*";

	case OV_VT_ANY:
		return "OV_ANY*";
	default:
		fprintf(stderr, "Unknown value type.\n");
		exit(EXIT_FAILURE);
	}
	return NULL;
}

/*
*	Return variable type name defined in ov_ov.h for a given vartype
*/
OV_STRING fb_builder_getvartypetext(
	OV_VAR_TYPE	vartype
) {
	switch(vartype) {
	case OV_VT_BOOL:
		return "OV_BOOL";
	case OV_VT_INT:
		return "OV_INT";
	case OV_VT_UINT:
		return "OV_UINT";
	case OV_VT_SINGLE:
		return "OV_SINGLE";
	case OV_VT_DOUBLE:
		return "OV_DOUBLE";
	case OV_VT_STRING:
		return "OV_STRING";
		
	case OV_VT_BOOL_VEC:
		return "OV_BOOL *";

	case OV_VT_INT_VEC:
		return "OV_INT *";
	case OV_VT_UINT_VEC:
		return "OV_UINT *";
	case OV_VT_SINGLE_VEC:
		return "OV_SINGLE *";
	case OV_VT_DOUBLE_VEC:
		return "OV_DOUBLE *";
	case OV_VT_STRING_VEC:
		return "OV_STRING *";
	case OV_VT_TIME:
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


OV_STRING fb_builder_getvartypevectext(
	OV_VAR_TYPE	vartype
) {
	switch(vartype) {
	case OV_VT_BOOL_VEC:
	case OV_VT_BOOL_PV_VEC:
		return "BOOL";
	case OV_VT_INT_VEC:
	case OV_VT_INT_PV_VEC:
		return "INT";
	case OV_VT_UINT_VEC:
	case OV_VT_UINT_PV_VEC:
		return "UINT";
	case OV_VT_SINGLE_VEC:
	case OV_VT_SINGLE_PV_VEC:
		return "SINGLE";
	case OV_VT_DOUBLE_VEC:
	case OV_VT_DOUBLE_PV_VEC:
		return "DOUBLE";
	case OV_VT_STRING_VEC:
	case OV_VT_STRING_PV_VEC:
		return "STRING";
	case OV_VT_TIME_VEC:
	case OV_VT_TIME_PV_VEC:
		return "TIME";
	case OV_VT_TIME_SPAN_VEC:
	case OV_VT_TIME_SPAN_PV_VEC:
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

OV_STRING ZZZZ_ov_codegen_getvartypetextsmall(
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

/*	----------------------------------------------------------------------	*/
/*
*	Check base class
*/
int isOfBaseClass(
    OV_OVM_CLASS_DEF	 *pclass,
    const char           *classname
) {
	OV_OVM_CLASS_DEF		*pbaseclass;

    pbaseclass = pclass;
    while(pbaseclass) {
        if(!pbaseclass->baseclassname) {
            return 0;
        }
        if(!strcmp(pbaseclass->baseclassname, classname) ) {
            return 1;
        }
	    pbaseclass = ov_codegen_getclassdef(NULL, pbaseclass->baseclassname);
    }
    
    return 0;
}

/*	----------------------------------------------------------------------	*/
/*
*	Search class of overloaded function
*/
void fb_builder_print_overloadedfnc(
    OV_OVM_LIBRARY_DEF	 *plib,
    OV_OVM_CLASS_DEF	 *pclass,
    OV_OVM_OPERATION_DEF *pop,
    FILE			     *fp
) {
	OV_OVM_CLASS_DEF		*pbaseclass;
	OV_OVM_OPERATION_DEF	*pop2;
	OV_STRING		         libid;
	OV_STRING		         classid;
    OV_UINT                  hasResult = 0;

    fprintf(fp,"    /*    \n    *   local variables\n    */\n");
    
    /* FNC der Basis-Klasse bekannt? */
    if( !strcmp(pop->identifier, "constructor") ||
        !strcmp(pop->identifier, "checkinit")   ||
        !strcmp(pop->identifier, "startup")     ||
        !strcmp(pop->identifier, "setvar")      ||
        !strcmp(pop->identifier, "getvar")      ||
        !strcmp(pop->identifier, "getflags")    ||
        !strcmp(pop->identifier, "getaccess")   ||
        !strcmp(pop->identifier, "shutdown")    ||
        !strcmp(pop->identifier, "destructor")  ) {

        /* OV/Object */
        
        fprintf(fp,"    OV_INSTPTR_%s_%s pinst = Ov_StaticPtrCast(%s_%s, pobj);\n",
                plib->identifier, pclass->identifier,
                plib->identifier, pclass->identifier); 
        
    } else {
        /* FB/Task/Functionblock? */
        if( !strcmp(pop->identifier, "typemethod") ||
            !strcmp(pop->identifier, "execute")    ) {
            
            if( !isOfBaseClass(pclass, "fb/task") ) {
                return;
            }

            if( !strcmp(pop->identifier, "typemethod") ) {
                fprintf(fp,"    OV_INSTPTR_%s_%s pinst = Ov_StaticPtrCast(%s_%s, pfb);\n",
                        plib->identifier, pclass->identifier,
                        plib->identifier, pclass->identifier); 
            }
            if( !strcmp(pop->identifier, "execute") ) {
                fprintf(fp,"    OV_INSTPTR_%s_%s pinst = Ov_StaticPtrCast(%s_%s, ptask);\n",
                        plib->identifier, pclass->identifier,
                        plib->identifier, pclass->identifier); 
            }
            
        } else {
            /* FB/Connection? */
            if( !strcmp(pop->identifier,   "checkelements") ||
                !strcmp(pop->identifier,   "trigger")    ) {
                
                if( !isOfBaseClass(pclass, "fb/connection") ) {
                    return;
                }
                
                fprintf(fp,"    OV_INSTPTR_%s_%s pinst = Ov_StaticPtrCast(%s_%s, pconn);\n",
                        plib->identifier, pclass->identifier,
                        plib->identifier, pclass->identifier);                 
                
            } else {
                return;
            }
        }
    }
    
    /* Suche Typ mit der zu ueberladenden FNC */
    pop2 = NULL;
    pbaseclass = pclass; 
    while(!pop2) {
	    pbaseclass = ov_codegen_getclassdef(NULL, pbaseclass->baseclassname);
	    if(!pbaseclass) {
	        break;
	    }
    	/* search for operation in this class */
    	for(pop2=pbaseclass->operations; pop2; pop2=pop2->pnext) {
    		if(!strcmp(pop2->identifier, pop->identifier)) {
    			break;
    		}
    	}
	}
	
    if(!pop2)  {
        return;
    }  

	libid = ov_codegen_getstring(pop2->classname, strlen(pop2->classname));
	for(classid = libid; *classid; classid++) {
		if(*classid == '/') {
			*classid = '\0';
			classid++;
			break;
		}
	}
    
    if( !libid || !classid) {
        return;
    }
    
    /* FNC mit Rueckgabewert? */
    if( !strcmp(pop->identifier, "constructor") ||
        !strcmp(pop->identifier, "checkinit") ) {
        hasResult = 1;
    }
    
    /* StartUp-FNC? */
    if( !strcmp(pop->identifier, "constructor") ||
        !strcmp(pop->identifier, "startup")     ||
        !strcmp(pop->identifier, "checkinit") ) {
        
		if(hasResult) {
            fprintf(fp,"    OV_RESULT    result;\n");
		}
		
        fprintf(fp,"\n    /* do what the base class does first */\n    ");
        
		if(hasResult) {
            fprintf(fp,"result = ");
		}
        
        fprintf(fp,"%s_%s_%s(pobj);\n", libid, classid, pop->identifier);
        
		if(hasResult) {
		    fprintf(fp,"    if(Ov_Fail(result))\n");
            fprintf(fp,"         return result;\n");
		}
        
        fprintf(fp,"\n    /* do what */\n\n");
     
        return;   
    }
    
    /* Destructor, shutdown */
    if( !strcmp(pop->identifier, "destructor") ||
        !strcmp(pop->identifier, "shutdown") ) {
        fprintf(fp,"\n    /* do what */\n\n");
        if( !strcmp(pop->identifier, "shutdown") ) {
            fprintf(fp,"    /* set the object's state to \"shut down\" */\n");
        }
        if( !strcmp(pop->identifier, "destructor") ) {
            fprintf(fp,"    /* destroy object */\n");
        }
    
        fprintf(fp,"    %s_%s_%s(pobj);\n", libid, classid, pop->identifier);
    }
    
    return;
}

/*	----------------------------------------------------------------------	*/
/*
*	Create source files of a library
*/
int fb_builder_createsourcefiles(
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
	char			*ptext, *ptext2, *ptext3;
	OV_INT			pathcount;
	OV_UINT			i;
	/*
	*	create output files
	*/
	
	for(pclass=plib->classes; pclass; pclass=pclass->pnext) {
		fp = fb_builder_createfile(pclass->identifier, ".c");
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
		fprintf(fp, "#include \"libov/ov_macros.h\"\n\n");

		fprintf(fp, "\n");
		for(pvar=pclass->variables;pvar;pvar=pvar->pnext) {
		
			if (pvar->varprops & OV_VP_GETACCESSOR) {
				fprintf(fp,"OV_DLLFNCEXPORT %s %s_%s_%s_get(\n", fb_builder_getResultValueType(pvar->vartype),
					plib->identifier,pclass->identifier,pvar->identifier);
				fprintf(fp,"    OV_INSTPTR_%s_%s          pobj",plib->identifier,pclass->identifier);
				
				/* Scalar? */ 
				if (pvar->veclen == 1) {
					fprintf(fp,"\n");
					fprintf(fp,") {\n");
					
					if (pvar->varprops & (OV_VP_DERIVED | OV_VP_STATIC)) {
						fprintf(fp,"    return (%s)0;\n",fb_builder_getResultValueType(pvar->vartype));
						fprintf(fp,"}\n\n");
						
					} else {
					    switch(pvar->vartype) {
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
							    fprintf(fp,"    return &pobj->v_%s;\n",pvar->identifier);
							    break;
							default:
							    fprintf(fp,"    return pobj->v_%s;\n",pvar->identifier);
							    break;
                        
                        }
						fprintf(fp,"}\n\n");
					}
					
				/* Vector */
				} else {
				
				    /* Nicht PV ? */
				    if( !(pvar->vartype & OV_VT_HAS_STATE) ) {
                        /* Dann auch Laenge zurueckgeben */
			     	    fprintf(fp,",\n    OV_UINT *pveclen");
                    }
					fprintf(fp,"\n) {\n");
					
					if (pvar->varprops & (OV_VP_DERIVED | OV_VP_STATIC)) {
					
					    if( !(pvar->vartype & OV_VT_HAS_STATE) ) {
    					    fprintf(fp,"    *pveclen = 0;\n");
    					}
						fprintf(fp,"    return (%s)0;\n",fb_builder_getResultValueType(pvar->vartype));
						fprintf(fp,"}\n\n");
						
					} else {
					    
					    /* PV? */
    				    if( pvar->vartype & OV_VT_HAS_STATE ) {
    						fprintf(fp,"    return &pobj->v_%s;\n",pvar->identifier);
    					    fprintf(fp,"}\n");
    					    
    					} else {
    					    /* Static vector */
    						if (pvar->veclen > 1) {
    							fprintf(fp,"    *pveclen = %lu;\n",pvar->veclen);
    							fprintf(fp,"    return pobj->v_%s;\n",pvar->identifier);
    							fprintf(fp,"}\n\n");
    							
    						} else {
    						    /* Dynamic vector */
    							fprintf(fp,"    *pveclen = pobj->v_%s.veclen;\n",pvar->identifier);
    							fprintf(fp,"    return pobj->v_%s.value;\n",pvar->identifier);
    							fprintf(fp,"}\n\n");
    						}
    					}
					}
				}
			}	
			if (pvar->varprops & OV_VP_SETACCESSOR) {
			
				fprintf(fp,"OV_DLLFNCEXPORT OV_RESULT %s_%s_%s_set(\n",
					plib->identifier,pclass->identifier,pvar->identifier);
				fprintf(fp,"    OV_INSTPTR_%s_%s          pobj,\n",plib->identifier,pclass->identifier);
				fprintf(fp,"    const %s  value", fb_builder_getResultValueType(pvar->vartype));
				
				/* Vector? */
				if (pvar->veclen != 1) {
    				if( !(pvar->vartype & OV_VT_HAS_STATE) ) {
    				    fprintf(fp,",\n    const OV_UINT veclen");
                    }
                }
 				fprintf(fp,"\n) {\n");

				if (pvar->varprops & (OV_VP_DERIVED | OV_VP_STATIC)) {
					fprintf(fp,"    return OV_ERR_OK;\n");
					fprintf(fp,"}\n\n");
				} else {
				    
				    /* Scalar? */
				    if (pvar->veclen == 1) {
					    switch(pvar->vartype) {
					        case OV_VT_STRING:
    							fprintf(fp,"    return ov_string_setvalue(&pobj->v_%s,value);\n",pvar->identifier);
    							break;
                			case OV_VT_ANY:
							     fprintf(fp,"    return ov_variable_setanyvalue(&pobj->v_%s, value);\n",pvar->identifier);
                			     break;
                			     
                 			case OV_VT_STRING_PV:
   							    fprintf(fp,"    pobj->v_%s.time = value->time;\n",pvar->identifier);
    							fprintf(fp,"    pobj->v_%s.state= value->state;\n",pvar->identifier);
    							fprintf(fp,"    return ov_string_setvalue(&pobj->v_%s.value,value->value);\n",pvar->identifier);
    							break;
                			case OV_VT_TIME:
                			case OV_VT_TIME_SPAN:
                			case OV_VT_BOOL_PV:
                			case OV_VT_INT_PV:
                			case OV_VT_UINT_PV:
                			case OV_VT_SINGLE_PV:
                			case OV_VT_DOUBLE_PV:
                			case OV_VT_TIME_PV:
                			case OV_VT_TIME_SPAN_PV:
							    fprintf(fp,"    pobj->v_%s = *value;\n",pvar->identifier);
							    fprintf(fp,"    return OV_ERR_OK;\n");
							    break;
							default:
							    fprintf(fp,"    pobj->v_%s = value;\n",pvar->identifier);
							    fprintf(fp,"    return OV_ERR_OK;\n");
							    break;
                        
                        }
						fprintf(fp,"}\n\n");
				        
					} else {
                        /* Vector */
                        
                        /* Static vector? */
						if (pvar->veclen > 1) {
						    				        
							fprintf(fp,"    return Ov_SetStaticVectorValue(pobj->v_%s,value,veclen,%s);\n",pvar->identifier,
								fb_builder_getvartypevectext(pvar->vartype));
							fprintf(fp,"}\n\n");
							
						} else {
							fprintf(fp,"    return Ov_SetDynamicVectorValue(&pobj->v_%s,value,veclen,%s);\n",pvar->identifier,
								fb_builder_getvartypevectext(pvar->vartype));
							fprintf(fp,"}\n\n");
						}
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
				rfp = fb_builder_openreadfile(includepath[pathcount-1], lib_ident, ".ovf");
				pathcount--;
			}	
			if(!rfp) {
		        if(pop->opprops != OV_OP_ABSTRACT) {
		        
    				fprintf(fp,"OV_DLLFNCEXPORT OV_RESULT %s_%s_%s(\n",
    					plib->identifier,pclass->identifier,pop->identifier);
    				fprintf(fp,"    OV_INSTPTR_%s_%s          pobj\n",lib_ident,class_ident);
    				fprintf(fp,") {\n");
    				fprintf(fp,"    return OV_ERR_OK;\n");
    				fprintf(fp,"}\n\n");
			    }
			}
			else {
				while (fscanf(rfp, "%s", text1)!=EOF) {
					ptext2 = strstr(text1,pop->cfnctypename);
					if (ptext2!=NULL) {
						ptext=strstr(ptext2, "(");
						ptext3 = ptext;
						if (ptext) {
 					        ptext3++;
					    	*ptext=0;
					    	ptext--;
        					while (((*ptext=='\t') || (*ptext==' ')) && (ptext > ptext2)) {
						        *ptext=0;
						        ptext--;
					    	}
						}
						if (strcmp(ptext2, pop->cfnctypename)==0)  {
		                    if(pop->opprops != OV_OP_ABSTRACT) {
		                        
		                        /* Manual Fix */
		                        if( !strcmp(pop->identifier, "checkelements") ) {
		                            if( isOfBaseClass(pclass, "fb/connection") ) {
		                                 fprintf(fp,"/* Return true if the connection has been initialized, otherwise false. */\n");
		                            }
		                        }
		                        
    							fprintf(fp,"OV_DLLFNCEXPORT %s %s_%s_%s",
    								text2,plib->identifier,pclass->identifier,pop->identifier);
    							if (ptext3) fprintf(fp,"(%s",ptext3);
    							while ( (fscanf(rfp, "%c", &text1[0])!=EOF) && (text1[0]!=';')) {
    								fprintf(fp,"%c",text1[0]);
    							}
    							fprintf(fp," {\n");
    							
	                            /* FNC ueberladet? */
                                if( (lib_ident != plib->identifier) ||
                                    (class_ident != pclass->identifier) ) {

                                    fb_builder_print_overloadedfnc(plib, pclass, pop, fp);
                                }
                                
                                fprintf(fp,"\n");
                                
    							if(!strcmp(text2, "void")) {
                                    fprintf(fp,"    return;\n");
                                } else {
                                    if(!strcmp(text2, "OV_RESULT")) {
    							        fprintf(fp,"    return OV_ERR_OK;\n");
    							    } else {
                                        fprintf(fp,"    return (%s)0;\n",text2);
                                    }
    							}
            						
        						fprintf(fp,"}\n\n");
    						}
						}
					}
					for (i=0; i<=strlen(text1);i++) text2[i]=text1[i];
				}
				fb_builder_closefile(rfp);
			}
		}
		fb_builder_closefile(fp);
	}
	for(passoc=plib->associations; passoc; passoc=passoc->pnext) {
		fp = fb_builder_createfile(passoc->identifier, ".c");
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
		fprintf(fp, "#include \"libov/ov_association.h\"\n\n");
		fprintf(fp, "#include \"libov/ov_macros.h\"\n\n");

		fprintf(fp,"OV_IMPL_LINK(%s_%s)\n", plib->identifier,passoc->identifier);
		fprintf(fp,"OV_IMPL_UNLINK(%s_%s)\n",plib->identifier,passoc->identifier);
		fprintf(fp,"OV_IMPL_GETACCESS(%s_%s)\n",plib->identifier,passoc->identifier);
		fprintf(fp,"\n");

		fb_builder_closefile(fp);
	}


	
	return EXIT_SUCCESS;
}

/*	----------------------------------------------------------------------	*/
/*
*	Backend of the code generator
*/
int fb_builder_backend(void) {
	/*
	*	local variables
	*/
	OV_OVM_LIBRARY_DEF	*plib;
	/*
	*	create output for all libraries
	*/
	for(plib=libraries; plib; plib=plib->pnext) {
		/*
		*	create source files *.c
		*/
		if ( !strcmp(plib->identifier, libname) ) {
			return fb_builder_createsourcefiles(plib);
		}
	}
	
    fprintf(stderr, "Library '%s' not found.\n", libname);
	return EXIT_FAILURE;
}
	

/*	----------------------------------------------------------------------	*/
/*
*   Main program
*/
int main(int argc, char **argv) {
	/*
	*	local variables
	*/
	int     i;
	int	exit_status = EXIT_FAILURE;
	char    *penv;
	char    *ph;
	char    *ifbsEnvVar = IFBS_HOME_ENVPATH;
	char    *acpltEnvVar = ACPLT_HOME_ENVPATH;
	char	userLibPath[512];
	
	/* Aux variables for creation of openlib.c*/
	FILE			*fp;
	int	addOpenLib = 0;
	
	int	acplt = 1; /* acplt or ifbspro server */

	/*
	*	initialization
	*/
	yydebug = 0;
	/*
	*	parse command line arguments
	*/
	for(i=1; i<argc; i++) {
		/*
		*	set library option
		*/
		if(!strcmp(argv[i], "-l") || !strcmp(argv[i], "--library")) {
			i++;
			if(i<argc) {
				libname = argv[i];
			} else {
				goto HELP;
			}
		}
    	/*
    	 *	'open lib' option
    	 */
    	else if( !strcmp(argv[i], "-o") || !strcmp(argv[i], "--userdefined-open") ) {
    	   addOpenLib = 1;
    	}
		/*
		*	display help option
		*/
		else if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
HELP:
			fprintf(stderr, "Source builder: creates '*.c' files from '*.ovm' for compiling an user library.\n");
            fprintf(stderr, "Each library directory contains the 'build', 'source', 'model' and (optional) 'include' directory.\n");
            fprintf(stderr, "_ LIBNAME\n");
            fprintf(stderr, "  |_ build\n");
            fprintf(stderr, "  |_ include\n");
            fprintf(stderr, "  |_ model\n");
            fprintf(stderr, "  |_ source\n");
            fprintf(stderr, "\n");

            fprintf(stderr, "Usage: acplt_builder [arguments]\n"
                            "\n"
                            "The following arguments are mandatory:\n"
                            "-l  LIBNAME                 Create source files for this library\n"
            				"\n"
                            "The following arguments are optional:\n"
                            "-o  OR --userdefined-open   Add user defined 'openlib' option\n"
                            "-h  OR --help               Display this help message and exit\n");
			return EXIT_FAILURE;
		} else {
			goto HELP;
		}
	}
	/*
	*	test if mandatory arguments are set
	*/
	if(!libname) {
		goto HELP;
	}

    	/* Enviroment */
    	/*if ((penv=getenv(pEnvVar)) == NULL) {
	    fprintf(stderr,"No environment variable >%s<", pEnvVar);
	    return 1;
	}*/
	if(getenv(acpltEnvVar) != NULL && getenv(ifbsEnvVar) == NULL){
		fprintf(stdout,"ACPLT server detected\n");
		penv = getenv(acpltEnvVar);
		acplt = 1;
	}else if(getenv(ifbsEnvVar) != NULL && getenv(acpltEnvVar) == NULL){
		fprintf(stdout,"IFBS server detected\n");
		penv = getenv(ifbsEnvVar);
		acplt = 0;
	}else if(getenv(ifbsEnvVar) == NULL && getenv(acpltEnvVar) == NULL){
		fprintf(stderr,"Neither %s nor %s are set. No runtime server detected.\n", acpltEnvVar, ifbsEnvVar);
		return 1;
	}else{
		fprintf(stdout,"ACPLT and IFBS servers detected. ACPLT server selected.\n");
		penv = getenv(acpltEnvVar);
		acplt = 1;
	}

	/* Difference between OV and IFBS */
	if(acplt == 1){
		sprintf(userLibPath, "%s/user", penv);	
	}else{
		sprintf(userLibPath, "%s/server/user", penv);
	}
	
	/*
	*  Set include paths
	*/
	//includepath_ptr = 0;
	//fb_builder_setincludepaths(penv, libname);
	//we reuse a function of makmak
	fb_makmak_searbaselibs(userLibPath, libname, NULL);
	//copy the output of searbaselibs
	includepath_ptr = anzAddLibs;
	for(i=0; i<anzAddLibs; i++) {
	    sprintf(outputpath, "%s/%s/model/", userLibPath, libs[i]);
	    compatiblePath(outputpath);
	    /* Hinzufuegen */
	    ph = (char*)malloc(strlen(outputpath) + 1);
	    if(!ph) {
		fprintf(stderr, "Out of memory\n");
		exit(1);
	    }
	    strcpy(ph, outputpath);
	    includepath[i] = ph;
	}
	//add root model library
	if(acplt == 1){
		sprintf(outputpath, "%s/model", penv);	
	}else{
		sprintf(outputpath, "%s/server/base/model", penv);
	}
	compatiblePath(outputpath);
	/* Hinzufuegen */
	ph = (char*)malloc(strlen(outputpath) + 1);
	if(!ph) {
		fprintf(stderr, "Out of memory\n");
		exit(1);
	}
	strcpy(ph, outputpath);
	includepath[includepath_ptr++] = ph;

	/*
	*	Input file 
	*/
	sprintf(outputpath, "%s/%s/model/%s.ovm", userLibPath, libname, libname);
	compatiblePath(outputpath);

	yyin = fopen(outputpath, "r");
	if(!yyin) {
		fprintf(stderr, "File '%s' not found.\n", outputpath);
		return EXIT_FAILURE;
	}

	/*
	*	Create output path
	*/
	sprintf(outputpath, "%s/%s/source", userLibPath, libname);
	compatiblePath(outputpath);

	/* Check if outputpath */
	if(stat(outputpath, &st) != 0){
			fprintf(stdout,"Creating directory '%s'... \n", outputpath);
			acplt_mkdir(outputpath);
	}

	/* User defined open-lib option? */
	if(addOpenLib == 1) {
		fp = fb_builder_createfile("library_open", ".c");
		if(!fp) {
			return EXIT_FAILURE;
		}

		/*
		*	undef OV_LIBRARY_OPEN option
		*/
		fprintf(fp, "#include \"%s.h\"\n", libname);
		fprintf(fp, "#ifdef ov_library_open_%s\n", libname);
		fprintf(fp, "#undef ov_library_open_%s\n", libname);
		fprintf(fp, "#endif\n");
		fprintf(fp, "\n");


		fprintf(fp, "/*\n");
		fprintf(fp, "* This function will be called, when the library is loaded.\n");
		fprintf(fp, "* It could generate compnents and initializes the startup procedure\n");
		fprintf(fp, "*/\n");
		fprintf(fp, "OV_RESULT ov_library_setglobalvars_%s_new(void) {\n", libname);
		fprintf(fp, "	OV_RESULT result;\n");
		fprintf(fp, "	/*\n");
		fprintf(fp, "	 *    set the global variables of the original version\n");
		fprintf(fp, "	 *    and if successful, load other libraries\n");
		fprintf(fp, "	 *    and create some objects\n");
		fprintf(fp, "	 */\n");
		fprintf(fp, "	result = ov_library_setglobalvars_%s();\n", libname);
		fprintf(fp, "	return result;\n");
		fprintf(fp, "}\n");

		fprintf(fp, "/*\n");
		fprintf(fp, "*       Replace the 'setglobalvars' function of a library with this\n");
		fprintf(fp, "*       previous one, which additionally creates instances.\n");
		fprintf(fp, "* 	This is called by the OV system upon library load.\n");
		fprintf(fp, "*/\n");
		fprintf(fp, "OV_DLLFNCEXPORT OV_LIBRARY_DEF *ov_library_open_%s(void) {\n", libname);
		fprintf(fp, "	/* local variables */\n");
		fprintf(fp, "	static OV_LIBRARY_DEF *OV_LIBRARY_DEF_%s_new;\n", libname);
		fprintf(fp, "	/*\n");
		fprintf(fp, "	*       replace the 'setglobalvars' function created by the code generator\n");
		fprintf(fp, "	*       with a new one.\n");
		fprintf(fp, "	*/\n");
		fprintf(fp, "	OV_LIBRARY_DEF_%s_new = ov_library_open_%s_old();\n", libname, libname);
		fprintf(fp, "	OV_LIBRARY_DEF_%s_new->setglobalvarsfnc = ov_library_setglobalvars_%s_new;\n", libname, libname);
		fprintf(fp, "	return OV_LIBRARY_DEF_%s_new;\n", libname);
		fprintf(fp, "}\n");

		fb_builder_closefile(fp);
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
			exit_status = fb_builder_backend();
		}
	}
	/*
	*	clean up memory and close input file
	*/
	ov_codegen_cleanup();
	fclose(yyin);

	/*
	*	done
	*/
	return exit_status;
}

/*
*	End of file
*/

