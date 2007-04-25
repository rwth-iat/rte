/*
 * $Id: dbparse.h,v 1.10 2007-04-25 13:59:03 martin Exp $
 *
 * Copyright (c) 1996-2002
 * Lehrstuhl fuer Prozessleittechnik, RWTH Aachen
 * D-52064 Aachen, Germany.
 * All rights reserved.
 *
 * This file is part of the ACPLT/KS Package which is licensed as open
 * source under the Artistic License; you can use, redistribute and/or
 * modify it under the terms of that license.
 *
 * You should have received a copy of the Artistic License along with
 * this Package; see the file ARTISTIC-LICENSE. If not, write to the
 * Copyright Holder.
 *
 * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
 * OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

// Includes and declarations for ACPLT/OV text file parser 
//
// Author : Christian Poensgen <chris@plt.rwth-aachen.de>
// dbparse.h
// last change: Nov 19, 2002

//-------------------------------------------------------------------------------

#ifndef KS_DBPARSE_H_INCLUDED
#define KS_DBPARSE_H_INCLUDED

#include <ctype.h>
#if PLT_USE_DEPRECIATED_HEADER
    #include <iostream.h>
#else
    #include <iostream>
#endif

#include <iostream.h>
#include <malloc.h>
#include <fstream.h>
#include "ks/commobject.h"
#include "ks/avmodule.h"
#include "ks/avsimplemodule.h"
#include "ks/objmgrparams.h"
#include "ks/path.h"
//#include "ks/stdconnectionmgr.h"


#if PLT_SYSTEM_NT
#include <time.h>
#include <stdio.h>
#include <dos.h>
#define uint unsigned int
#define timezone _timezone
#endif

#if PLT_COMPILER_MSVC
#define sleep Sleep
#include <io.h>
#endif

//-------------------------------------------------------------------------------
// defines
#if PLT_USE_DEPRECIATED_HEADER
    #define OUT_STREAM  cerr
    #define NEW_LINE    endl
#else
    #define OUT_STREAM  STDNS::cerr
    #define NEW_LINE    STDNS::endl
#endif

//-------------------------------------------------------------------------------

enum value_types {		// defines possible value types for internal representation
	DB_VT_NONE,			// of variables
	DB_VT_BOOL,
	DB_VT_INT,
	DB_VT_DOUBLE,
	DB_VT_STRING,
	DB_VT_TIME,
	DB_VT_TIME_SPAN,
	DB_VT_VECTOR,
	DB_VT_STRUCTURE,
	DB_VT_VOID,
	DB_VT_UINT,
	DB_VT_SINGLE,
	DB_VT_INT_TO_SINGLE,
	DB_VT_INT_TO_DOUBLE,
	DB_VT_DOUBLE_TO_SINGLE,
	DB_VT_TIME_TO_TIME_SPAN,
	DB_VT_TIME_SPAN_TO_TIME
};

//-------------------------------------------------------------------------------

// These options specify the behaviour when loading a textfile into an existing
// database

#define LO_ADD_CLASSES				0x1		// add missing classes to target database
#define LO_OVERWRITE_EXACT			0x2		// overwrite exactly matching types
#define LO_OVERWRITE_INHERITED		0x4		// overwrite inherited types
#define LO_OVERWRITE_INCOMPATIBLE	0x8		// overwrite incompatible types
#define LO_ERROR_ON_INCOMPATIBLE	0x10	// generate error if incompatible types
#define LO_ADD_DOMAINS				0x20	// add missing parent domains to target database
#define LO_DEFAULT					(LO_ADD_CLASSES|LO_OVERWRITE_EXACT| \
									LO_ADD_DOMAINS)

//-------------------------------------------------------------------------------

enum create_options {		// possible values for attribute "cr_opts" of class instance
	CO_TREE,				// domain only needed for tree structure
	CO_NONE,				// initial value before compatibility check
	CO_CREATE,				// domain not present in server, create it
	CO_CHANGE,				// domain present and of same type, set values
	CO_OVERWRITE			// domain present and of different type, delete and recreate it
};

//-------------------------------------------------------------------------------

typedef bool (*exec_function) (class instance *);		// function prototype

//-------------------------------------------------------------------------------
//
//	Class declarations
//
//-------------------------------------------------------------------------------

// representation of an object path, modified KsPath to support parts
class LogPath: public KsPath
{
public:
	LogPath(const PltString & str);
	LogPath();
	LogPath(const LogPath & path, size_t first, size_t last);
    operator PltString () const;
	PltString getHead() const;
	LogPath getTail() const;
	bool headIsPart();
	bool headIsRootPart();
	bool isPart();
protected:
    size_t checkAndCount();
    void findSlashes();
};

// root of and operations on the parse tree
class parsetree {
	public:
	parsetree();
	~parsetree();
	class instance *getRoot();
	bool add(class instance *inst);
	bool remove(class instance *inst);
	bool remove(LogPath *name);
	bool inTree(LogPath *name);
	bool forEach(exec_function func);
	class instance *search(LogPath *search_path);
	
	protected:
	bool depthFirstSearch(class instance *start, exec_function func);
	
	class instance *root;	// root of the parse tree
};

// one vector element
class vector {
	public:
	vector();
	~vector();

	class value			*content;	// value of this vector element
	enum value_types	type;
	vector				*next;		// next element of vector
};

// one structure element (structures are not yet supported!)
class structure {
	public:
	structure();
	~structure();

	class value		*content;		// value of this structure element
	structure		*next;			// next element of structure
};

// value of a variable
class value {
	public:
	value();
	~value();

	enum value_types	type;		// determines type of value stored in v
	union {
		bool			bool_val;
		int				int_val;
		double			double_val;
		void			*pvoid_val;
		KsString		*pstring_val;
		KsTime			*ptime_val;
		KsTimeSpan		*ptime_span_val;
		vector			*pvector_val;
		structure		*pstructure_val;
	}	v;
};

// information about a variable
class variable_value {
	public:
	variable_value();
	~variable_value();

	KsString			*ident;
	value				*val;
	KsTime				*var_time;
	KS_STATE			var_state;
	uint				var_flags;			// semantic flags
	KsString			*var_unit;
	KsString			*var_comment;
};


// value of a link
class link_value {
	public:
	link_value();
	~link_value();

	KsString			*ident;
	PltList<LogPath>	*link_paths;		// targets of the link
};

// representation of an instance (object) in the parse tree
class instance {
	public:

	instance();
	~instance();
	variable_value *search_var(KsString *name);
	link_value *search_link(KsString *name);
	instance *search_part(KsString *name);
	instance *search_child(KsString *name);

	LogPath										*ident;
	bool										is_part;			// true = Part
	LogPath										*class_ident;
	KsTime										*creation_time;
	uint										sem_flags;			// Semantic flags
	KsString									*comment;
	PltList<variable_value *>					*var_block_list;
	PltList<link_value *>						*link_block_list;
	PltList<instance *>							*part_block_list;
	PltHashTable<PltString, instance *>			*children;
	PltList<instance *>							*children_list;
	enum create_options							cr_opts;
};

//-------------------------------------------------------------------------------

// global variables
extern ofstream parselog;
extern parsetree *parse_tree;
extern int lopts;								// load options
extern KscPath *path;
extern bool	relative;

//-------------------------------------------------------------------------------
//
//	Function prototypes
//
//-------------------------------------------------------------------------------

// parsing routines
int yyparse(void);
int yylex(void);
int yyerror(char *s);
//-------------------------------------------------------------------------------

// output operators
ostream &operator << (ostream &log, PltString str);
ostream &operator << (ostream &log, KsString *str);
ostream &operator << (ostream &log, LogPath path);
ostream &operator << (ostream &log, KsTime *time);
ostream &operator << (ostream &log, KsTimeSpan *timesp);
ostream &operator << (ostream &log, enum value_types vtyp);
ostream &operator << (ostream &log, vector *vec);
ostream &operator << (ostream &log, structure *struc);
ostream &operator << (ostream &log, value *val);
ostream &operator << (ostream &log, variable_value *varval);
ostream &operator << (ostream &log, link_value *linkval);
ostream &operator << (ostream &log, PltList<variable_value*> *var_block_list);
ostream &operator << (ostream &log, PltList<link_value*> *link_block_list);
ostream &operator << (ostream &log, PltList<instance*> *children_list);
ostream &operator << (ostream &log, instance *inst);

//-------------------------------------------------------------------------------

// Convert KsTime to KsString
KsString TimeToAscii(const KsTime ptime);

// Convert KsTimeSpan to KsString
KsString TimeSpanToAscii(const KsTimeSpan ptimesp);

// Get time from string
KsTime GetTime(char *arg);

// Get timespan from string
KsTimeSpan GetTimeSpan(char *arg);

// Get semantic flags from string
KS_SEMANTIC_FLAGS GetSemFlags(KsString *ksarg);

#endif	// KS_DBPARSE_H_INCLUDED
