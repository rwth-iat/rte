/*
 * Copyright (c) 1996, 1997, 1998, 1999, 2000, 2001
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

// Author : Christian Poensgen <chris@plt.rwth-aachen.de>
// dbparse.h
// last change: Nov 26, 2001

//-------------------------------------------------------------------------------

#ifndef KS_DBPARSE_H_INCLUDED
#define KS_DBPARSE_H_INCLUDED

#include <ctype.h>
#include <iostream.h>
#include <malloc.h>
#include <fstream.h>
#include "ks/commobject.h"
#include "ks/avmodule.h"
#include "ks/avsimplemodule.h"
#include "ks/objmgrparams.h"
#include "ks/path.h"

#if PLT_SYSTEM_NT
#include <time.h>
#include <stdio.h>
#include <dos.h>
#define uint unsigned int
#define timezone _timezone
#endif

//-------------------------------------------------------------------------------

enum value_types {
	DB_VT_NONE,
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
#define LO_DEFAULT					(LO_ADD_CLASSES|LO_OVERWRITE_EXACT|LO_ERROR_ON_INCOMPATIBLE| \
									LO_ADD_DOMAINS)

//-------------------------------------------------------------------------------

enum create_options {
	CO_TREE,				// domain only needed for tree structure
	CO_NONE,				// initial value before compatibility check
	CO_CREATE,				// domain not present in server, create it
	CO_CHANGE,				// domain present and of same type, set values
	CO_OVERWRITE			// domain present and of different type, delete and recreate it
};

//-------------------------------------------------------------------------------

typedef bool (*exec_function) (class instance *);

//-------------------------------------------------------------------------------
//
//	Class declarations
//
//-------------------------------------------------------------------------------

class LogPath: public KsPath
{
public:
	LogPath(const PltString & str);
	LogPath();
	LogPath(const LogPath & path, size_t first, size_t last);
    operator PltString () const;
	PltString getHead() const;
	LogPath getTail() const;
	bool isPart();
	bool isRootPart();
protected:
    size_t checkAndCount();
    void findSlashes();
};


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
	
	class instance *root;
};

class vector {
	public:
	vector();
	~vector();

	class value			*content;
	enum value_types	type;
	vector				*next;
};

class structure {
	public:
	structure();
	~structure();

	class value		*content;
	structure		*next;
};

class value {
	public:
	value();
	~value();

	enum value_types	type;
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

class variable_value {
	public:
	variable_value();
	~variable_value();

	KsString			*ident;
	value				*val;
	KsTime				*var_time;
	KS_STATE			var_state;
	uint				var_flags;
	KsString			*var_unit;
	KsString			*var_comment;
};

class link_value {
	public:
	link_value();
	~link_value();

	KsString			*ident;
	PltList<LogPath>		*link_paths;
};

class instance {
	public:

	instance();
	~instance();
	variable_value *search_var(KsString *name);
	link_value *search_link(KsString *name);
	instance *search_part(KsString *name);
	instance *search_child(KsString *name);

	LogPath										*ident;
	bool										is_part;
	LogPath										*class_ident;
	KsTime										*creation_time;
	uint										sem_flags;
	KsString									*comment;
	PltHashTable<PltString, variable_value *>	*var_block;
	PltHashTable<PltString, link_value *>		*link_block;
	PltHashTable<PltString, instance	*>		*part_block;
	PltHashTable<PltString, instance *>			*children;
	enum create_options							cr_opts;
};

//-------------------------------------------------------------------------------

// global variables
extern ofstream parselog;
extern parsetree *parse_tree;
extern int lopts;								// load options

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
ostream &operator << (ostream &log, PltHashTable<PltString, variable_value*> *var_block);
ostream &operator << (ostream &log, PltHashTable<PltString, link_value*> *link_block);
ostream &operator << (ostream &log, PltHashTable<PltString, instance*> *children);
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