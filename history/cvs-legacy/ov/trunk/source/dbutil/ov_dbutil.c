/*
*   $Id: ov_dbutil.c,v 1.6 2004-10-27 12:09:06 ansgar Exp $
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
*	03-Jul-1998 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	20-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*	11-Oct-2004 Ansgar Münnemann <ansgar@plt.rwth-aachen.de>: integration of extended infos and dbdump functions.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "libov/ov_database.h"
#include "libov/ov_result.h"
#include "libov/ov_logfile.h"
#include "libov/ov_macros.h"
#include "libov/ov_path.h"


/*	----------------------------------------------------------------------	*/

/*
*   Global variables: table of static libraries
*/

#if OV_STATIC_LIBRARIES
Ov_BeginStaticLibraryTable
Ov_EndStaticLibraryTable;
#endif

/*	----------------------------------------------------------------------	*/

void ov_instanceoutput(
        OV_INSTPTR_ov_object pobj,
        FILE *handle,
        int rnum
);

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

/*
*	calculate size of ov data types
*/
OV_UINT ov_sizeof(
	OV_UINT vartype
) {
	switch(vartype & 0x7f){
        	case OV_VT_BOOL:
        		return sizeof(OV_BOOL);
        	case OV_VT_BYTE:
        		return sizeof(OV_BYTE);
                case OV_VT_INT:
        		return sizeof(OV_INT);
                case OV_VT_UINT:
        		return sizeof(OV_UINT);
                case OV_VT_SINGLE:
        		return sizeof(OV_SINGLE);
                case OV_VT_STRING:
        		return sizeof(OV_STRING);
                case OV_VT_DOUBLE:
        		return sizeof(OV_DOUBLE);
                case OV_VT_TIME:
        		return sizeof(OV_TIME);
                case OV_VT_TIME_SPAN:
        		return sizeof(OV_TIME_SPAN);
 	}
 	return 0;
}

/*	----------------------------------------------------------------------	*/

/*
*	new line in respect to rnum
*/
void ov_newline(
	FILE* handle,
	int rnum
) {
   	int i;

   	fprintf(handle, "\n");
   	for (i=0;i<rnum;i++) fprintf(handle,"\t");
}

/*	----------------------------------------------------------------------	*/

/*
*	part output
*/

void ov_partoutput(
        OV_INSTPTR_ov_object pobj,
        FILE *handle,
        int rnum
){
	OV_ELEMENT	parentelem;
	OV_ELEMENT	partelem;

	parentelem.elemtype = OV_ET_OBJECT;
	parentelem.pobj = pobj;
	partelem.elemtype = OV_ET_NONE;
	partelem.pobj = NULL;

	ov_element_getnextpart(&parentelem, &partelem, OV_ET_OBJECT);
	while (partelem.elemtype != OV_ET_NONE) {
	        fprintf(handle, "\t\tPART_INSTANCE ");
        	ov_instanceoutput(partelem.pobj, handle, rnum + 2);
		fprintf(handle, "END_PART_INSTANCE;");
		ov_newline(handle, rnum);
		ov_element_getnextpart(&parentelem, &partelem, OV_ET_OBJECT);
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	variable output
*/

void ov_basevalueoutput(
	OV_UINT vartype,
        OV_BYTE* pvalue,
        FILE *handle
){

	OV_STRING    stringtime;
        OV_STRING    stringtimespan;
        OV_BOOL      b;
        OV_BYTE      byte;
        OV_INT       i;
        OV_UINT      u;
        OV_SINGLE    s;
        OV_DOUBLE    d;
        OV_STRING    st;
        OV_TIME      t;
        OV_TIME_SPAN tsp;

        switch (vartype){
                case OV_VT_VOID:
		      fprintf(handle, "<void>");
		      break;
	        case OV_VT_BOOL:
                      b = *(OV_BOOL*)(pvalue);
                      if (b == 0) fprintf(handle, "FALSE");
                      else fprintf(handle, "TRUE");
                      break;
                case OV_VT_INT:
                      i = *(OV_INT*)(pvalue);
                      fprintf(handle, "%d", i);
                      break;
                case OV_VT_UINT:
                      u = *(OV_UINT*)(pvalue);
                      fprintf(handle, "%d", u);
                      break;
                case OV_VT_SINGLE:
                      s = *(OV_SINGLE*)(pvalue);
                      fprintf(handle, "%f", s);
                      break;
                case OV_VT_DOUBLE:
                      d = *(OV_DOUBLE*)(pvalue);
                      fprintf(handle, "%f", d);
                      break;
                case OV_VT_BYTE:
                      byte =*(OV_BYTE*)(pvalue);
                      fprintf(handle, "%d", byte);
                      break;
                case OV_VT_STRING:
                      st = *(OV_STRING*)(pvalue);
                      if(st){
        	             ov_memstack_lock();
 	                     fprintf(handle, "\"%s\"", st);
                	     ov_memstack_unlock();
                      }
		      else fprintf(handle, "\"\"");
                      break;
                case OV_VT_TIME:
                      t = *(OV_TIME*)(pvalue);
                      stringtime = ov_time_timetoascii(&t);
                      fprintf(handle, "%s", stringtime);
                      break;
                case OV_VT_TIME_SPAN:
                      tsp = *(OV_TIME_SPAN*)(pvalue);
                      stringtimespan = ov_time_timespantoascii(&tsp);
                      fprintf(handle, "%s", stringtimespan);
                      break;
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	variable value output
*/

void ov_valueoutput(
	OV_ELEMENT 	thiselem,
        OV_UINT 	vartype,
        OV_UINT 	veclen,
        OV_BYTE 	*pvalue,
        FILE 		*handle
)
{
        OV_UINT 	veclength;
        OV_UINT 	z;
        OV_BYTE 	*pval;
	OV_ELEMENT	partelem;

	partelem.elemtype = OV_ET_NONE;
	partelem.pobj = NULL;

	switch(vartype){
        	case OV_VT_BOOL:
        	case OV_VT_BYTE:
                case OV_VT_INT:
                case OV_VT_UINT:
                case OV_VT_SINGLE:
                case OV_VT_STRING:
                case OV_VT_DOUBLE:
                case OV_VT_TIME:
                case OV_VT_TIME_SPAN:
                	ov_basevalueoutput(vartype, pvalue, handle);
		        fprintf(handle, ";");
                 	break;
                case OV_VT_BOOL_VEC:
                case OV_VT_BYTE_VEC:
                case OV_VT_INT_VEC:
                case OV_VT_UINT_VEC:
                case OV_VT_DOUBLE_VEC:
                case OV_VT_SINGLE_VEC:
                case OV_VT_STRING_VEC:
                case OV_VT_TIME_VEC:
                case OV_VT_TIME_SPAN_VEC:
	                fprintf(handle, "{ ");
	                /* dynamische Vektoren*/
	                if (veclen==0){
	                	veclength = ((OV_GENERIC_VEC*)(pvalue))->veclen;
	                	pval = ((OV_GENERIC_VEC*)(pvalue))->value;
	                     	for (z=0; z<veclength; z++){
	                        	ov_basevalueoutput(vartype & 0x7f, pval, handle);
	                        	pval = pval + ov_sizeof(vartype & 0x7f);
	                        	if ((z+1) < veclength) fprintf(handle, ", ");
	                     	}
	                }
	                /* statische Vektoren*/
			else{
	                	if (veclen >= 1){
	                	        pval = pvalue;
                      			for(z=0; z<veclen; z++){
                      				ov_basevalueoutput(vartype & 0x7f, pval, handle);
		                        	pval = pval + ov_sizeof(vartype & 0x7f);
		                        	if ((z+1) < veclen) fprintf(handle, ", ");
                      			}
                    		}
                   	}
	                fprintf(handle, " };");
                 	break;
                 case OV_VT_ANY:
                 	if (((OV_ANY*)pvalue)->value.vartype & OV_VT_ISVECTOR) veclength = 0;
                 	else veclength = 1;
                   	ov_valueoutput(partelem, (((OV_ANY*)pvalue)->value.vartype & OV_VT_KSMASK), veclength, &(((OV_ANY*)pvalue)->value.valueunion.val_byte), handle);
	                break;
                        fprintf(handle, ";");
                 case OV_VT_STRUCT:
	                fprintf(handle, "{ ");
			ov_element_getnextpart(&thiselem, &partelem, OV_ET_VARIABLE);
			while (partelem.elemtype != OV_ET_NONE) {
			        fprintf(handle, "%s = ", partelem.elemunion.pvar->v_identifier);
				ov_valueoutput(partelem, partelem.elemunion.pvar->v_vartype, partelem.elemunion.pvar->v_veclen, partelem.pvalue, handle);
				ov_element_getnextpart(&thiselem, &partelem, OV_ET_VARIABLE);
				if (partelem.elemtype != OV_ET_NONE) fprintf(handle, ", ");
			}
	                fprintf(handle, " };");
                 	break;

        }

}

/*	----------------------------------------------------------------------	*/

/*
*	variable output
*/

void ov_variableoutput(
        OV_INSTPTR_ov_object pobj,
        FILE *handle,
        int rnum
){
	OV_ELEMENT	parentelem;
	OV_ELEMENT	partelem;

	parentelem.elemtype = OV_ET_OBJECT;
	parentelem.pobj = pobj;
	partelem.elemtype = OV_ET_NONE;
	partelem.pobj = NULL;

	ov_element_getnextpart(&parentelem, &partelem, OV_ET_VARIABLE);
	while (partelem.elemtype != OV_ET_NONE) {
	        if (((partelem.elemunion.pvar->v_vartype & OV_VT_CTYPE) != OV_VT_CTYPE) &&
		    strcmp(partelem.elemunion.pvar->v_identifier, "identifier") &&
		    strcmp(partelem.elemunion.pvar->v_identifier, "creationtime") &&
		    strcmp(partelem.elemunion.pvar->v_identifier, "objectstate")) {
		        fprintf(handle, "\t\t%s = ", partelem.elemunion.pvar->v_identifier);
			ov_valueoutput(partelem, partelem.elemunion.pvar->v_vartype, partelem.elemunion.pvar->v_veclen, partelem.pvalue, handle);
			ov_newline(handle, rnum);
		}
		ov_element_getnextpart(&parentelem, &partelem, OV_ET_VARIABLE);
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	link output
*/

void ov_linkoutput(
        OV_INSTPTR_ov_object pobj,
        FILE *handle,
        int rnum
){
	OV_ELEMENT		parentelem;
	OV_ELEMENT		partelem;
	OV_INSTPTR_ov_object	pparent;
	OV_INSTPTR_ov_object	pchild;
        OV_STRING 		res;
	Ov_Association_DefineIteratorNM(pit);

	parentelem.elemtype = OV_ET_OBJECT;
	parentelem.pobj = pobj;
	partelem.elemtype = OV_ET_NONE;
	partelem.pobj = NULL;

	ov_element_getnextpart(&parentelem, &partelem, OV_ET_PARENTLINK | OV_ET_CHILDLINK);
	while (partelem.elemtype != OV_ET_NONE) {
	        if ((partelem.elemtype == OV_ET_PARENTLINK) &&
		     strcmp(partelem.elemunion.passoc->v_identifier, "containment")) {
       		        fprintf(handle, "\t\t%s = {", partelem.elemunion.passoc->v_childrolename);
		        switch (partelem.elemunion.passoc->v_assoctype) {
		                case OV_AT_ONE_TO_ONE:
		                	pchild = Ov_Association_GetChild(partelem.elemunion.passoc, pobj);
				        if (pchild) {
				                ov_memstack_lock();
					        res = ov_path_getcanonicalpath(pchild, 2);
					        fprintf(handle, "%s", res);
					        ov_memstack_unlock();
					}
		                	break;
		                case OV_AT_ONE_TO_MANY:
		                	pchild = Ov_Association_GetFirstChild(partelem.elemunion.passoc, pobj);
		                	while (pchild) {
					        ov_memstack_lock();
					        res = ov_path_getcanonicalpath(pchild, 2);
					        fprintf(handle, "%s", res);
					        ov_memstack_unlock();
					        pchild = Ov_Association_GetNextChild(partelem.elemunion.passoc, pchild);
					        if (pchild) fprintf(handle, ",");
					}
		                	break;
		                case OV_AT_MANY_TO_MANY:
		                	pchild = Ov_Association_GetFirstChildNM(partelem.elemunion.passoc, pit, pobj);
		                	while (pchild) {
					        ov_memstack_lock();
					        res = ov_path_getcanonicalpath(pchild, 2);
					        fprintf(handle, " %s", res);
					        ov_memstack_unlock();
					        pchild = Ov_Association_GetNextChildNM(partelem.elemunion.passoc, pit);
					        if (pchild) fprintf(handle, ",");
					}
		                	break;
		        }
	                fprintf(handle, "}");
			ov_newline(handle, rnum);
		}
	        if ((partelem.elemtype == OV_ET_CHILDLINK) &&
		     strcmp(partelem.elemunion.passoc->v_identifier, "containment") &&
		     strcmp(partelem.elemunion.passoc->v_identifier, "instantiation")) {
		        fprintf(handle, "\t\t%s = {", partelem.elemunion.passoc->v_parentrolename);
		        switch (partelem.elemunion.passoc->v_assoctype) {
		                case OV_AT_ONE_TO_ONE:
		                case OV_AT_ONE_TO_MANY:
		                	pparent = Ov_Association_GetParent(partelem.elemunion.passoc, pobj);
				        if (pparent) {
						ov_memstack_lock();
				        	res = ov_path_getcanonicalpath(pparent, 2);
				        	fprintf(handle, " %s", res);
				        	ov_memstack_unlock();
				        }
		                	break;
		                case OV_AT_MANY_TO_MANY:
		                	pparent = Ov_Association_GetFirstParentNM(partelem.elemunion.passoc, pit, pobj);
					while (pparent) {
					        ov_memstack_lock();
					        res = ov_path_getcanonicalpath(pparent, 2);
					        fprintf(handle, " %s", res);
					        ov_memstack_unlock();
					        pparent = Ov_Association_GetNextParentNM(partelem.elemunion.passoc, pit);
					        if (pparent) fprintf(handle, ",");
					}
		                	break;
		        }
	                fprintf(handle, "}");
			ov_newline(handle, rnum);
		}
		ov_element_getnextpart(&parentelem, &partelem, OV_ET_PARENTLINK | OV_ET_CHILDLINK);
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	database dump
*/

void ov_instanceoutput(
        OV_INSTPTR_ov_object pobj,
        FILE *handle,
        int rnum
){
        OV_INSTPTR_ov_class   	pclass;
        OV_STRING 		res;
        OV_STRING 		tstring;
        char			text[32];

	/*liefert den Zeiger der Klasse einer Instanz*/
        pclass = Ov_GetClassPtr(pobj);

	/*liefert den Pfad des Objektes aus*/
        ov_memstack_lock();
        res = ov_path_getcanonicalpath(pobj, 2);
        fprintf(handle, "%s", res);
        ov_memstack_unlock();

	/* schreibt den Namen der zugeörigen Bibliothek und Klasse aus */
        fprintf(handle, " : CLASS ");
        ov_memstack_lock();
        res = ov_path_getcanonicalpath( Ov_StaticPtrCast(ov_object, pclass), 2);
        fprintf(handle,"%s", res);
        ov_memstack_unlock();

	ov_newline(handle, rnum);

	/* Ausgabe der Erzeugungszeit */
	tstring = ov_time_timetoascii(&pobj->v_creationtime);
	fprintf(handle, "\tCREATION_TIME = %s;", tstring);
	ov_newline(handle, rnum);

     	/* Hier werden die Flags zu der Klasse ausgeschrieben */
    	if(pclass->v_flags){
    	        flagstext(pclass->v_flags, text);
        	fprintf(handle, "\tFLAGS = \"%s\";", text);
     	}
     	else {
        	fprintf(handle, "\tFLAGS = \"\";");
     	}
	ov_newline(handle, rnum);

	/* Ausgabe des Kommentars */
	if(pclass->v_comment){
        	fprintf(handle, "\tCOMMENT = \"%s\";", pclass->v_comment);
      	}
      	else {
        	fprintf(handle, "\tCOMMENT = \"\";");
       	}
	ov_newline(handle, rnum);

	/* Ausgabe der Objekt-Variablen */
	fprintf(handle, "\tVARIABLE_VALUES");
	ov_newline(handle, rnum);

	ov_variableoutput(pobj, handle, rnum);

	fprintf(handle, "\tEND_VARIABLE_VALUES;");
	ov_newline(handle, rnum);

	/* Ausgabe der Objekt-Links */
	fprintf(handle, "\tLINK_VALUES");
	ov_newline(handle, rnum);

	ov_linkoutput(pobj, handle, rnum);

	fprintf(handle, "\tEND_LINK_VALUES;");
	ov_newline(handle, rnum);

	/* Ausgabe der Objekt-Parts */
	fprintf(handle, "\tPART_INSTANCES");
	ov_newline(handle, rnum);

	ov_partoutput(pobj, handle, rnum);

	fprintf(handle, "\tEND_PART_INSTANCES;");
	ov_newline(handle, rnum);
}

/*	----------------------------------------------------------------------	*/

/*
*	iterate database objetcs
*/

void ov_dump_db(
	FILE *handle,
	OV_INSTPTR_ov_domain pdom
) {
	OV_INSTPTR_ov_object 	pobj;
	OV_INSTPTR_ov_domain 	pdom2;

	Ov_ForEachChild(ov_containment, pdom, pobj) {
	        fprintf(handle, "INSTANCE ");
	        ov_instanceoutput(pobj, handle, 0);
		fprintf(handle, "END_INSTANCE;");
		ov_newline(handle, 0);
		ov_newline(handle, 0);
		pdom2 = Ov_DynamicPtrCast(ov_domain, pobj);
		if (pdom2) ov_dump_db(handle, pdom2);
	}
}
/*	----------------------------------------------------------------------	*/

/*
*	database dump
*/

OV_RESULT ov_dump(
	OV_STRING name
) {
	FILE 			*handle;
	OV_INSTPTR_ov_object 	pobj;

	handle = fopen(name, "w");
	if(!handle) {
		ov_logfile_error("unable to open file for writing: %s.\n", name);
		return -1;
	}
	passoc_ov_containment = &(pdb->containment);
	passoc_ov_instantiation = &(pdb->instantiation);
	if ((!passoc_ov_containment) || (!passoc_ov_instantiation)) {
	        fclose(handle);
	        return -1;
	}

	// get passocs from database
	Ov_ForEachChild(ov_containment, &pdb->ov, pobj) {
	        if (!strcmp(pobj->v_identifier, "containment")) passoc_ov_containment = (OV_INSTPTR_ov_association) pobj;
	        if (!strcmp(pobj->v_identifier, "instantiation")) passoc_ov_instantiation = (OV_INSTPTR_ov_association) pobj;
	        if (!strcmp(pobj->v_identifier, "inheritance")) passoc_ov_inheritance = (OV_INSTPTR_ov_association) pobj;
	        if (!strcmp(pobj->v_identifier, "parentrelationship")) passoc_ov_parentrelationship = (OV_INSTPTR_ov_association) pobj;
	        if (!strcmp(pobj->v_identifier, "childrelationship")) passoc_ov_childrelationship = (OV_INSTPTR_ov_association) pobj;

	        if (!strcmp(pobj->v_identifier, "object")) pclass_ov_object = (OV_INSTPTR_ov_class) pobj;
	        if (!strcmp(pobj->v_identifier, "domain")) pclass_ov_domain = (OV_INSTPTR_ov_class) pobj;
	        if (!strcmp(pobj->v_identifier, "class")) pclass_ov_class = (OV_INSTPTR_ov_class) pobj;
	        if (!strcmp(pobj->v_identifier, "association")) pclass_ov_association = (OV_INSTPTR_ov_class) pobj;
	        if (!strcmp(pobj->v_identifier, "variable")) pclass_ov_variable = (OV_INSTPTR_ov_class) pobj;
	        if (!strcmp(pobj->v_identifier, "operation")) pclass_ov_operation = (OV_INSTPTR_ov_class) pobj;
	        if (!strcmp(pobj->v_identifier, "part")) pclass_ov_part = (OV_INSTPTR_ov_class) pobj;
	}
	if ((!passoc_ov_inheritance) || (!passoc_ov_parentrelationship) || (!passoc_ov_childrelationship)) {
	        fclose(handle);
	        return -1;
	}

	// start recursive object iteration
	ov_dump_db(handle, &pdb->root);
	fclose(handle);
	return 0;
}

/*	----------------------------------------------------------------------	*/

/*
*	Main program
*/
int main(int argc, char **argv) {
	/*
	*	local variables
	*/
	OV_INT	 		i;
	OV_UINT			size = 0;
	OV_STRING		filename = "database.ovd";
	OV_STRING		dumpfilename = NULL;
	OV_RESULT		result;
	OV_BOOL			extended = FALSE;
	OV_INSTPTR_ov_object	pobj;
	OV_INSTPTR_ov_object	pinst;
	/*
	*	if we are debugging, log to stderr (if not
	*	specified by the command line options)
	*/
#ifdef OV_DEBUG
	ov_logfile_logtostderr(NULL);
#endif
	/*
	*	parse command line arguments
	*/
	for(i=1; i<argc; i++) {
		/*
		*	set database filename option
		*/
		if(!strcmp(argv[i], "-f") || !strcmp(argv[i], "--file")) {
			i++;
			if(i<argc) {
				filename = argv[i];
			} else {
				goto HELP;
			}
		}
		/*
		*	create database option
		*/
		else if(!strcmp(argv[i], "-c") || !strcmp(argv[i], "--create")) {
			i++;
			if(i<argc) {
				size = strtoul(argv[i], NULL, 0);
			} else {
				goto HELP;
			}
		}
		/*
		*	set logfile option
		*/
		else if(!strcmp(argv[i], "-l") || !strcmp(argv[i], "--logfile")) {
			i++;
			if(i<argc) {
				if(!strcmp(argv[i], "stdout")) {
					ov_logfile_logtostdout(NULL);
				} else if(!strcmp(argv[i], "stderr")) {
					ov_logfile_logtostderr(NULL);
#if OV_SYSTEM_NT
				} else if(!strcmp(argv[i], "ntlog")) {
					ov_logfile_logtontlog(NULL);
#endif
				} else {
					if(Ov_Fail(ov_logfile_open(NULL, argv[i], "w"))) {
						fprintf(stderr, "Could not open log file: \"%s\".\n", argv[i]);
						return EXIT_FAILURE;
					}
				}
			} else {
				goto HELP;
			}
		}
		/*
		*	display extended information
		*/
		else if(!strcmp(argv[i], "-e") || !strcmp(argv[i], "--extended")) {
			extended = TRUE;
		}
		/*
		*	display extended information
		*/
		else if(!strcmp(argv[i], "-d") || !strcmp(argv[i], "--dump")) {
			i++;
			if(i<argc) {
				dumpfilename = argv[i];
			} else {
				goto HELP;
			}
		}
		/*
		*	display version option
		*/
		else if(!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version")) {
			printf("ACPLT/OV Database Utility " OV_VER_DBUTIL "\n");
			return EXIT_FAILURE;
		}
		/*
		*	display help option
		*/
		else if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
HELP:		fprintf(stderr, "Usage: ov_dbutil [arguments]\n"
				"\n"
				"The following optional arguments are available:\n"
				"-f FILE, --file FILE            Set database filename (*.ovd)\n"
				"-c SIZE, --create SIZE          Create a new database\n"
				"-l LOGFILE, --logfile LOGFILE   Set logfile name, you may use stdout"
#if OV_SYSTEM_NT
				", stderr\n"
				"                                or ntlog (Windows NT only)\n"
#else
				" or stderr\n"
#endif
				"-d DUMPFILE, --dump DUMPFILE    Create database text dump\n"
				"-e, --extended                  Display extended database information\n"
				"-v, --version                   Display version information\n"
				"-h, --help                      Display this help message\n");
			return EXIT_FAILURE;
		} else {
			goto HELP;
		}
	}
	/*
	*	create new or map existing database
	*/
	if(size) {
		ov_logfile_info("Creating database \"%s\"...", filename);
		result = ov_database_create(filename, size);
		if(Ov_Fail(result)) {
ERRORMSG:	ov_logfile_error("Error: %s (error code 0x%x).",
				ov_result_getresulttext(result), result);
			return EXIT_FAILURE;
		}
		ov_logfile_info("Database created.");
	} else {
		ov_logfile_info("Mapping database \"%s\"...", filename);
		result = ov_database_map_loadfile(filename);
		if(Ov_Fail(result)) {
			goto ERRORMSG;
		}
		ov_logfile_info("Database mapped.");
	 	if (extended) {
			/*
			*	Print extended database information
			*/
			passoc_ov_containment = &pdb->containment;
			passoc_ov_instantiation = &pdb->instantiation;

			pobj = Ov_GetFirstChild(ov_containment, &(pdb->ov));
			while (pobj) {
			        if (!strcmp(pobj->v_identifier, "library")) {
			        	Ov_ForEachChild(ov_instantiation, (OV_INSTPTR_ov_class) pobj, pinst) {
						ov_logfile_info("Defined library %s, version %s", pinst->v_identifier, ((OV_INSTPTR_ov_library) pinst)->v_version);
	                               	}
			        }
			        pobj = Ov_GetNextChild(ov_containment, pobj);
			}
		}
		if (dumpfilename) {
			/*
			*	Create a text dump file of the ov database
			*/
			result = ov_dump(dumpfilename);
			if(Ov_Fail(result)) {
				goto ERRORMSG;
			}

		}
		ov_logfile_info("Loading libraries \"%s\"...", filename);
		result = ov_database_map_loadlib(filename);
		if(Ov_Fail(result)) {
			goto ERRORMSG;
		}
	}
	/*
	*	Print some information
	*/
	ov_logfile_info("Database size is %ld Byte.", ov_database_getsize());
	ov_logfile_info("Used storage size is %ld Byte.", ov_database_getused());
	ov_logfile_info("Free storage size is %ld Byte.", ov_database_getfree());
        ov_logfile_info("Unmapping database \"%s\"...", filename);
	ov_database_unmap();
	ov_logfile_info("Database unmapped.");
	/*
	*	return
	*/
	return EXIT_SUCCESS;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

