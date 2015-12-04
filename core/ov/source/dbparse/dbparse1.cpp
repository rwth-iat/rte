/*
 * $Id: dbparse1.cpp,v 1.22 2007-04-25 13:59:03 martin Exp $
 *
 * Copyright (c) 1996-2004
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

// ACPLT/OV text file parser
//
// Author : Christian Poensgen <chris@plt.rwth-aachen.de>
//          Ansgar M�nnemann <ansgar@plt.rwth-aachen.de>

//-------------------------------------------------------------------------------
// includes

#if PLT_SYSTEM_LINUX
#include <unistd.h>
#endif

#include <time.h>
#include "dbparse.h"
#include "libov/ov_version.h"
#include "ks/conversions.h"
#include "libov/ov_ov.h"
//for saving dumps to database dir
#include "libov/ov_database.h"

#include "db_y.h"

//-------------------------------------------------------------------------------
// global variables

ofstream			parselog;						// file logging parse results
parsetree			*parse_tree;			     	// parse tree
int					lopts;							// load options
KsString			server;							// target server name
KscPath  			*path;							// target path in server
KscServerBase		*server_base;					// server base
u_int				lib_wait_time;					// time to wait after loading
													// the libraries
bool				verbose;						// print verbose status info?
bool				relative;						// write relative pathes to PATH
bool				use_activitylock;				// use activitylock of OV-server
PltList<instance *>	liblist;						// list of library instances

//-------------------------------------------------------------------------------

void _DELETE (void *ptr) {			// delete pointers only if not NULL
	if (ptr) {
		delete(ptr);
	}
}

//-------------------------------------------------------------------------------
//
// methods of class LogPath
//
//-------------------------------------------------------------------------------

// Parse the path string into path components, separated by slashes or dots
// depending on whether they denote children or parts. The indices of the
// beginnings of the path components are stored for later (quick) use.
// Note that the "../" and "./" components are not indexed, but rather we
// remember how many components we should go up within the object tree.

void LogPath::findSlashes() {

	if (_slash.size() == 0) {
	//
	// We don't have exceptions (well, we don't want to use them),
	// so this is the only way to indicate failure.
	//
		_valid = false;
	}
	if (isValid() ) {
		const char *s = _str;
		size_t si = 0;
		//
		// Start with skipping the "../../" garbage at the beginning
		// of the path.
		//
		while (s[si] == '/' || s[si] == '.') ++si;
		//
		// Now save the indices of the remaining slashes
		//
		_slash[0] = si;
		size_t ai = 1;
		size_t maxidx = _slash.size() - 1 ;
		while (ai < maxidx) {
			if (s[si] == '/' || s[si] == '.') {
				_slash[ai++] = si+1;
			}
			++si;
		}
		_slash[ai] = PLT_SIZE_T_MAX;
		//
		// and set the range we are interested in.
		// Since this is a construction from a string,
		// set the full range.
		//
		_last = ai-1;
	}
	PLT_POSTCONDITION( 	!isValid()
						|| (_first == 0 && _slash.size() == size() + 1));
} // LogPath::findSlashes

//-------------------------------------------------------------------------------

// Parse the path string, checking for well-formedness, and at the same time
// count the number of components.

size_t LogPath::checkAndCount() {

    const char * p = _str;				// running pointer
    bool was_slash = false;				// was last char == '/' or '.'?
    _valid = true;						// be optimistic

	//
	// Empty paths are not allowed, so bail out as soon as possible.
	//
	if (!*p) {
		_valid = false;
		return 0;
	}
	if (*p == '/' || (*p == '.' && *(p+1) != '/' && *(p+1) != '.')) {
		// The path is absolute.
		_go_up = 0;
		was_slash = true;
		++p;
		if (*p == '.') {
			++p;
		}
	} else {
	//
	// The path is relative, so we need to skip the "stay here" and
	// "go up" components in front of the more interesting part of
	// the path.
	//
		_go_up = 1;
		//
		// handle points
		//
		do {
			if (*p == '.') {
				// first '.'
				++p;
				was_slash=false;
				if (*p == '.') {
					// second '.'
					++p;
					++_go_up;
				}
				if (*p == '/') {
					was_slash = true;
					++p;
				}
			} else {
				break;
			}
		} while (was_slash);
	}

	size_t slashes = 0;
	for ( ; *p; ++p) {
		char ch = *p;
		if (ch == '/' || ch == '.') {
			if (was_slash) {
				// two of '/' and '.' in a row are not allowed
				_valid = false;
				return 0;
			} else {
				was_slash = true;
				++slashes;
			}
		} else {
		// TODO: Check for valid characters in string and bail out
		// if garbage is found.
			was_slash = false;
		}
	}
	return slashes;
} // LogPath::checkAndCount

//-------------------------------------------------------------------------------

// Convert a LogPath object to a PltString

LogPath::operator PltString () const
{
	PltString prefix = "";
	if (isAbsolute()) {	// path is absolute
		// check if path is correct
		if (_slash[_first] >= 2 &&
//			_slash[_first] < 100 &&		// necessary? TODO!
			_str[_slash[_first] - 2] == '/') {
			prefix = "/";
		}
		if (_slash[_first] >= 1 &&
//			_slash[_first] < 100 &&		// necessary? TODO!
			_str[_slash[_first] - 1] == '.') {
			prefix += ".";
		} else {
			prefix = "/";
		}
	} else {	// path is relative
		for (size_t i = 0; i<_go_up-1; ++i) {
			prefix += "../";
		}
	}
	return PltString(prefix,
					 _str.substr(_slash[_first],
					 _slash[_last+1] - _slash[_first]));

} // LogPath::operator PltString

//-------------------------------------------------------------------------------

// Create a LogPath from a PltString

LogPath::LogPath(const PltString & str)
{
	_str = str;
	_slash = checkAndCount() + 2;
	_first = 0;
	findSlashes();
}

//-------------------------------------------------------------------------------

// Create an empty LogPath

LogPath::LogPath()
{
	KsPath();
}

//-------------------------------------------------------------------------------

// Create a new LogPath representing a substring of another LogPath

LogPath::LogPath(const LogPath &path, size_t first, size_t last)
{
	_str = path._str;
	_valid = path._valid;
	_slash = path._slash;
	_go_up = 0;

	PLT_PRECONDITION(path.isValid()
					 && first <= last && last < path.size());
	_first = first + path._first;
	_last = last + path._first;
}

//-------------------------------------------------------------------------------

// Get first path component

PltString LogPath::getHead() const
{
	PLT_ASSERT(isValid());
	return (*this)[0];
}

//-------------------------------------------------------------------------------

// Get the rest of the path

LogPath LogPath::getTail() const
{
	PLT_ASSERT(size() > 1);
	return LogPath(*this, 1, size()-1);
}

//-------------------------------------------------------------------------------

// Check if head of path is a part

bool LogPath::headIsPart() {
	if (isAbsolute() && _str[_slash[_first] - 1] == '.') {
		return true;
	} else {
		return false;
	}
}

//-------------------------------------------------------------------------------

// Check if head of path is a part in the root domain ("/.")

bool LogPath::headIsRootPart() {
	if (isAbsolute() && _str[_slash[_first] - 1] == '.'
		&& _slash[_first] >= 2 && _str[_slash[_first] - 2] == '/') {
		return true;
	} else {
		return false;
	}
}

//-------------------------------------------------------------------------------

// Check if last component of path is a part

bool LogPath::isPart() {
	if (isAbsolute() && _str[_slash[_last - 1] - 1] == '.') {
		return true;
	} else {
		return false;
	}
}

//-------------------------------------------------------------------------------
//
// methods of class parsetree
//
//-------------------------------------------------------------------------------

// public:

// parse tree constructor, creates root domain
parsetree::parsetree() {
	root = new(instance);
	root->ident = new(LogPath);
	*(root->ident) = LogPath("/");
	root->is_part = false;
	root->class_ident = new(LogPath);
	*(root->class_ident) = LogPath("/acplt/ov/domain");
	root->creation_time = new(KsTime);
	*(root->creation_time) = (root->creation_time)->now();
	root->sem_flags = 0;
	root->comment = NULL;
	root->cr_opts = CO_TREE;
}

//-------------------------------------------------------------------------------

// destructor
parsetree::~parsetree() {
	delete root;
}

//-------------------------------------------------------------------------------

// get root instance of parse tree
instance *parsetree::getRoot() {
	return root;
}

//-------------------------------------------------------------------------------

// add an instance to the parse tree, add parent domains if necessary
bool parsetree::add(instance *inst) {

	instance	*act_inst, *new_inst, *child;
	int			i, j;
	KsString	delimiter;

	if (verbose) OUT_STREAM << "Adding " << *inst->ident << " to parsetree!" << NEW_LINE;
	j = inst->ident->size();					// nr. of path components
	if (*(inst->ident) == LogPath("/")) {		// root domain always exists
		*(root->creation_time) = *(inst->creation_time);
		return true;
	}
	act_inst = root;
	LogPath act_tail = inst->ident->getTail();
	if (inst->ident->headIsRootPart() ) {			// NEW: Pfad mit "/." am Anfang
		delimiter = "/.";
	} else {
		delimiter = "/";
	}
	KsString act_name = KsString(delimiter + inst->ident->getHead());

	for (i = 0; i < j-1; i++) {					// for any path level
		new_inst = act_inst->search_child(&act_name);
		if (!new_inst) {
			new_inst = act_inst->search_part(&act_name);
		}
		if (!new_inst) {						// parent domain not (yet) in parse tree,
			new_inst = new(instance);			// so create it
			new_inst->ident = new(LogPath);
			*(new_inst->ident) = LogPath(act_name);
			if (new_inst->ident->isPart() ) {	//	NEW: check if instance is part
				new_inst->is_part = true;
			} else {
				new_inst->is_part = false;
			}
			new_inst->class_ident = new(LogPath);
			*(new_inst->class_ident) = LogPath("/acplt/ov/domain");
			new_inst->creation_time = new(KsTime);
			*(new_inst->creation_time) = (new_inst->creation_time)->now();
			new_inst->sem_flags = 0;
			new_inst->comment = NULL;
			new_inst->var_block_list = NULL;
			new_inst->link_block_list = NULL;
			new_inst->part_block_list = NULL;
			new_inst->cr_opts = CO_TREE;
			if (!act_inst->children) {			// create new hash table for children
				act_inst->children = new(PltHashTable<PltString, instance*>);
			}
			if (!act_inst->children_list) {
			    act_inst->children_list = new(PltList<instance *>);
   			}
   			act_inst->children_list->addLast(new_inst);
			act_inst->children->add(*(new_inst->ident), new_inst);
		}
		act_inst = new_inst;
		if (act_tail.headIsPart() ) {			// NEW
			delimiter = ".";					//
		} else {								//
			delimiter = "/";					//
		}
		act_name = KsString(act_name + delimiter + act_tail.getHead());	// one tree level down
		act_tail = act_tail.getTail();
	} // for

	if (act_inst->children) {
		child = act_inst->search_child(&act_name);
		if (child) {
			if (child->cr_opts == CO_TREE) {	// instance exists with CO_TREE => overwrite it
				*(child->ident) = *(inst->ident);
				child->is_part = inst->is_part;
				*(child->class_ident) = *(inst->class_ident);
				*(child->creation_time) = *(inst->creation_time);
				child->sem_flags = inst->sem_flags;
				*(child->comment) = *(inst->comment);
				child->cr_opts = CO_NONE;
				if (inst->is_part) {
					act_inst->children->remove(*(inst->ident), inst);
					act_inst->children_list->remove(inst);
				} else {
					return true;
				}
			} else {
				OUT_STREAM << "Error: Duplicate instance identifier in input file!" << NEW_LINE;
				return false;
			}
		}
	}

	if (inst->is_part) {					// instance is a part
		if (!act_inst->part_block_list) {	// no part list exists
		    act_inst->part_block_list = new(PltList<instance *>);
  		}
		if (act_inst->part_block_list->addLast(inst)) {
			return true;
		} else {
		       	OUT_STREAM << "Error: Could not add instance " << *(inst->ident) << " to part_block!" << NEW_LINE;
			return false;
		}
	} else {								// instance is a child
		if (!act_inst->children) {			// no child list exists
			act_inst->children = new(PltHashTable<PltString, instance*>);
		}
		if (!act_inst->children_list) {
		    act_inst->children_list = new(PltList<instance *>);
		}
		if (act_inst->children->add(*(inst->ident), inst)) {
			act_inst->children_list->addLast(inst);
			return true;
		} else {
		       	OUT_STREAM << "Error: Could not add instance " << *(inst->ident) << " to children!" << NEW_LINE;
			return false;
		}
	}
} // parsetree::add

//-------------------------------------------------------------------------------

// remove instance with path "name" from parse tree
bool parsetree::remove(LogPath *name)
{
	instance	*inst, *parent;
	LogPath		lp;

	inst = search(name);
	if (!inst) {
		return false;
	}
	lp = LogPath(*name, 0, name->size()-1);
	parent = search(&lp);
	// remove instance from hash table of parent
	if (!parent->children->remove(*name, inst)) {
		OUT_STREAM << "Error: Cannot remove " << *(inst->ident) << " from parse tree!" << NEW_LINE;
		exit(-1);
	}
	if (!parent->children_list->remove(inst)) {
		OUT_STREAM << "Error: Cannot remove " << *(inst->ident) << " from parse tree!" << NEW_LINE;
		exit(-1);
	}
	delete inst;
	return true;
} // parsetree::remove

//-------------------------------------------------------------------------------

// remove instance pointed to by "inst" from parse tree
bool parsetree::remove(instance *inst)
{
	instance	*parent;
	LogPath		lp;

	lp = LogPath(*(inst->ident), 0, inst->ident->size()-1);
	parent = search(&lp);

	if (!parent->children->remove(*(inst->ident), inst)) {
		OUT_STREAM << "Error: Cannot remove " << *(inst->ident) << " from parse tree!" << NEW_LINE;
		exit(-1);
	}
	if (!parent->children_list->remove(inst)) {
		OUT_STREAM << "Error: Cannot remove " << *(inst->ident) << " from parse tree!" << NEW_LINE;
		exit(-1);
	}
	delete inst;
	return true;
} // parsetree::remove

//-------------------------------------------------------------------------------

// is object "name" in parse tree?
bool parsetree::inTree(LogPath *name) {
	return (search(name) != NULL);
}

//-------------------------------------------------------------------------------

// apply "func" on each node of the parse tree
bool parsetree::forEach(exec_function func) {
	return depthFirstSearch(root, func);
} // parsetree::forEach

//-------------------------------------------------------------------------------

// return a pointer to the node with name "search_path"
instance *parsetree::search(LogPath *search_path) {

	instance	*act_inst;
	KsString	act_name;
	LogPath		act_tail;

	if (*search_path == LogPath("/")) {			// no need to search for root
		return root;
	} else {
		act_inst = root;
		act_name = KsString("/" + search_path->getHead());
		act_tail = LogPath(search_path->getTail());
		while (act_tail.isValid() ) {			// descend tree levels
			act_inst = act_inst->search_child(&act_name);
			if (!act_inst) {
				return NULL;
			}
			if (act_tail.getHead() == "") {
				break;
			}
			act_name = KsString(act_name + "/" + act_tail.getHead());
			act_tail = LogPath(act_tail.getTail());
			// TODO: check if "."
		}
		return act_inst;
	}
} // parsetree::search

//-------------------------------------------------------------------------------

// protected:

// visit each node below "start" in depth first order and apply function "func"
bool parsetree::depthFirstSearch(instance *start, exec_function func) {
	bool	ret = true;

	if (!func(start)) {
		//	func execution failed
		return false;
	}
	
	if (start->children_list) {
		PltListIterator<instance *> *it = (PltListIterator<instance *> *) start->children_list->newIterator();
		//	iterate over children
		while(*it) {
			ret &= depthFirstSearch((**it), func);
			++*it;
		}
	}

	if (start->part_block_list) {
		PltListIterator<instance *> *it = (PltListIterator<instance *> *) start->part_block_list->newIterator();
		//	iterate over parts
		while(*it) {
			ret &= depthFirstSearch((**it), func);
			++*it;
		}
	}
	return ret;
}

//-------------------------------------------------------------------------------
//
// methods of class vector
//
//-------------------------------------------------------------------------------

// constructor
vector::vector() {
	type = DB_VT_VOID;
	content = NULL;
	next = NULL;
}

//-------------------------------------------------------------------------------

// destructor
vector::~vector() {
	_DELETE(content);
	_DELETE(next);
}

//-------------------------------------------------------------------------------
//
// methods of class structure
//
//-------------------------------------------------------------------------------

// constructor
structure::structure() {
	content = NULL;
	next = NULL;
}

//-------------------------------------------------------------------------------

// destructor
structure::~structure() {
	delete(content);
	_DELETE(next);
}

//-------------------------------------------------------------------------------
//
// methods of class value
//
//-------------------------------------------------------------------------------

// constructor
value::value() {
	type = DB_VT_VOID;
	v.pvoid_val = NULL;
}

//-------------------------------------------------------------------------------

// destructor
value::~value() {
	switch (type) {
		case DB_VT_STRING				: delete(v.pstring_val); break;
		case DB_VT_TIME					: delete(v.ptime_val); break;
		case DB_VT_TIME_SPAN			: delete(v.ptime_span_val); break;
		case DB_VT_TIME_TO_TIME_SPAN	: delete(v.ptime_val); break;
		case DB_VT_TIME_SPAN_TO_TIME	: delete(v.ptime_span_val); break;
		case DB_VT_VECTOR				: delete(v.pvector_val); break;
		case DB_VT_STRUCTURE			: delete(v.pstructure_val); break;
		default							: break;
	}
}

//-------------------------------------------------------------------------------
//
// methods of class variable_value
//
//-------------------------------------------------------------------------------

// constructor
variable_value::variable_value() {
	ident = NULL;
	val = NULL;
	var_time = NULL;
	var_state = KS_ST_UNKNOWN;
	var_flags = 0;
	var_unit = NULL;
	var_comment = NULL;
}

//-------------------------------------------------------------------------------

// destructor
variable_value::~variable_value() {
	delete(ident);
	delete(val);
	_DELETE(var_time);
	_DELETE(var_unit);
	_DELETE(var_comment);
}

//-------------------------------------------------------------------------------
//
// methods of class link_value
//
//-------------------------------------------------------------------------------

// constructor
link_value::link_value() {
	ident = NULL;
	link_paths = NULL;
}

//-------------------------------------------------------------------------------

// destructor
link_value::~link_value() {
	delete(ident);
	_DELETE(link_paths);
}

//-------------------------------------------------------------------------------
//
// methods of class instance
//
//-------------------------------------------------------------------------------

//public:

// constructor
instance::instance() {
	ident = NULL;
	is_part = false;
	class_ident = NULL;
	creation_time = NULL;
	sem_flags = 0;
	comment = NULL;
	var_block_list = NULL;
	link_block_list = NULL;
	part_block_list = NULL;
	children = NULL;
	children_list = NULL;
	cr_opts = CO_NONE;
}

//-------------------------------------------------------------------------------

// destructor
instance::~instance() {
	delete(ident);
	delete(class_ident);
	_DELETE(creation_time);
	_DELETE(comment);
	_DELETE(var_block_list);
	_DELETE(link_block_list);
	_DELETE(part_block_list);
	_DELETE(children);
	_DELETE(children_list);
}

//-------------------------------------------------------------------------------

// search variable "name" of instance
variable_value *instance::search_var(KsString *name) {
	variable_value *p = NULL;

	if (var_block_list) {
	    PltListIterator<variable_value *> *it = (PltListIterator<variable_value *>*) var_block_list->newIterator();
	    while (*it) {
			p = (**it);
			if (strcmp(*p->ident,*name) == 0) {
				delete it;
				return p;
   			}
			++*it;
     	} // end WHILE there are more variables
     	delete it;
	} // end IF
 	return NULL;
};

//-------------------------------------------------------------------------------

// search link "name" of instance
link_value *instance::search_link(KsString *name) {
	link_value *p = NULL;

	if (link_block_list) {
	    PltListIterator<link_value *> *it = (PltListIterator<link_value *>*) link_block_list->newIterator();
	    while (*it) {
			p = (**it);
			if (strcmp((PltString) *(p->ident),*name) == 0) {
				delete it;
				return p;
   			}
			++*it;
     	} // end WHILE there are more variables
     	delete it;
	} // end IF
 	return NULL;
};

//-------------------------------------------------------------------------------

// search part "name" of instance
instance *instance::search_part(KsString *name) {
	instance *p = NULL;

	if (part_block_list) {
	    PltListIterator<instance *> *it = (PltListIterator<instance *>*) part_block_list->newIterator();
	    while (*it) {
			p = (**it);
            if (strcmp((PltString) *(p->ident),*name) == 0) {
				delete it;
				return p;
   			}
			++*it;
     	} // end WHILE there are more variables
     	delete it;
	} // end IF
 	return NULL;
};

//-------------------------------------------------------------------------------

// search child "name" of instance
instance *instance::search_child(KsString *name) {
	instance *p = NULL;

	if (!children || !children->query(*name, p)) {
		return NULL;
	} else {
		return p;
	}
};

//-------------------------------------------------------------------------------
//
// output operators
//
//-------------------------------------------------------------------------------

ostream &operator << (ostream &log, PltString str) {
	return log << (const char *) str;
}

//-------------------------------------------------------------------------------

ostream &operator << (ostream &log, KsString *str) {
	if (str) {
		return log << *str;
	} else {
		return log;
	}
}

//-------------------------------------------------------------------------------

ostream &operator << (ostream &log, LogPath path) {

	if (path.isValid() ) {
		if (path.headIsRootPart() ) {
			log << "/.";
		} else {
			if (path.headIsPart() ) {
				log << ".";
			} else {
				log << "/";
			}
		}
		log << path.getHead();
		if (!path.isSingle()) {
			log << path.getTail();
		}
	} else {
		OUT_STREAM << "Warning: Invalid path!" << NEW_LINE;
	}
	return log;
}

//-------------------------------------------------------------------------------

ostream &operator << (ostream &log, KsTime *time) {
	if (time) {
		return log << TimeToAscii(*time);
	} else {
		return log;
	}
}

//-------------------------------------------------------------------------------

ostream &operator << (ostream &log, KsTimeSpan *timesp) {
	return log << TimeSpanToAscii(*timesp);
}

//-------------------------------------------------------------------------------

ostream &operator << (ostream &log, enum value_types vtyp) {
	switch (vtyp) {
		case DB_VT_BOOL					: return log << "DB_VT_BOOL";
		case DB_VT_INT					: return log << "DB_VT_INT";
		case DB_VT_DOUBLE				: return log << "DB_VT_DOUBLE";
		case DB_VT_STRING				: return log << "DB_VT_STRING";
		case DB_VT_TIME					: return log << "DB_VT_TIME";
		case DB_VT_TIME_SPAN			: return log << "DB_VT_TIME_SPAN";
		case DB_VT_VECTOR				: return log << "DB_VT_VECTOR";
		case DB_VT_STRUCTURE			: return log << "DB_VT_STRUCTURE";
		case DB_VT_VOID					: return log << "DB_VT_VOID";
		case DB_VT_UINT					: return log << "DB_VT_UINT";
		case DB_VT_SINGLE				: return log << "DB_VT_SINGLE";
		case DB_VT_INT_TO_SINGLE		: return log << "DB_VT_INT_TO_SINGLE";
		case DB_VT_INT_TO_DOUBLE		: return log << "DB_VT_INT_TO_DOUBLE";
		case DB_VT_DOUBLE_TO_SINGLE		: return log << "DB_VT_DOUBLE_TO_SINGLE";
		case DB_VT_TIME_TO_TIME_SPAN	: return log << "DB_VT_TIME_TO_TIME_SPAN";
		case DB_VT_TIME_SPAN_TO_TIME	: return log << "DB_VT_TIME_SPAN_TO_TIME";
		default							: return log << "unknown";
	}
}

//-------------------------------------------------------------------------------

ostream &operator << (ostream &log, value *val) {
	switch (val->type) {
		case DB_VT_BOOL					: return log << "<DB_VT_BOOL> " << val->v.bool_val;
		case DB_VT_INT					: return log << "<DB_VT_INT> " << val->v.int_val;
		case DB_VT_DOUBLE				: return log << "<DB_VT_DOUBLE> " << val->v.double_val;
		case DB_VT_STRING				: return log << "<DB_VT_STRING> " << *(val->v.pstring_val);
		case DB_VT_TIME					: return log << "<DB_VT_TIME> " << val->v.ptime_val;
		case DB_VT_TIME_SPAN			: return log << "<DB_VT_TIME_SPAN> " << val->v.ptime_span_val;
		case DB_VT_VECTOR				: return log << "<DB_VT_VECTOR> {[" << val->v.pvector_val->type << "] "
													 << val->v.pvector_val << "}";
		case DB_VT_STRUCTURE			: return log << "<DB_VT_STRUCTURE> {" << val->v.pstructure_val << "}";
		case DB_VT_VOID					: return log << "<DB_VT_VOID> " << "<void>";
		case DB_VT_UINT					: return log << "<DB_VT_UINT> " << val->v.int_val;
		case DB_VT_SINGLE				: return log << "<DB_VT_SINGLE> " << val->v.double_val;
		case DB_VT_INT_TO_SINGLE		: return log << "<DB_VT_INT_TO_SINGLE> " << val->v.int_val;
		case DB_VT_INT_TO_DOUBLE		: return log << "<DB_VT_INT_TO_DOUBLE> " << val->v.int_val;
		case DB_VT_DOUBLE_TO_SINGLE		: return log << "<DB_VT_DOUBLE_TO_SINGLE> " << val->v.double_val;
		case DB_VT_TIME_TO_TIME_SPAN	: return log << "<DB_VT_TIME_TO_TIME_SPAN> " << val->v.ptime_val;
		case DB_VT_TIME_SPAN_TO_TIME	: return log << "<DB_VT_TIME_SPAN_TO_TIME> " << val->v.ptime_span_val;
		default							: return log << "<unknown>";
	}
}

//-------------------------------------------------------------------------------

ostream &operator << (ostream &log, vector *vec) {
	if (vec && vec->content) {								// vector not empty
		return log << vec->content << "," << vec->next;
	} else {
		return log;
	}
}

//-------------------------------------------------------------------------------

ostream &operator << (ostream &log, structure *struc) {
	if (struc) {
		return log << struc->content << "," << struc->next;
	} else {
		return log;
	}
}

//-------------------------------------------------------------------------------

ostream &operator << (ostream &log, variable_value *varval) {

	if (varval) {
		KsString 	state;

		switch (varval->var_state) {
			case KS_ST_NOTSUPPORTED	: state = KsString("KS_ST_NOTSUPPORTED"); break;
			case KS_ST_UNKNOWN		: state = KsString("KS_ST_UNKNOWN"); break;
			case KS_ST_BAD			: state = KsString("KS_ST_BAD"); break;
			case KS_ST_QUESTIONABLE	: state = KsString("KS_ST_QUESTIONABLE"); break;
			case KS_ST_GOOD			: state = KsString("KS_ST_GOOD"); break;
		}
		return log << "   Identifier: " << *(varval->ident) << NEW_LINE
				   << "   Value: " << varval->val << NEW_LINE
				   << "   Time: " << varval->var_time << NEW_LINE
				   << "   State: " << state << NEW_LINE
				   << "   Flags: " << varval->var_flags << NEW_LINE
				   << "   Unit: " << varval->var_unit << NEW_LINE
				   << "   Comment: " << varval->var_comment << NEW_LINE;
	} else {
		return log;
	}
}
//-------------------------------------------------------------------------------

ostream &operator << (ostream &log, link_value *linkval) {
	PltListIterator<LogPath> *it;

	if (linkval && linkval->link_paths) {
		it = (PltListIterator<LogPath> *) linkval->link_paths->newIterator();
		log << "   Identifier: " << *(linkval->ident) << NEW_LINE
			<< "   Paths: ";
		while(*it) {	// iterate over link targets
			log << LogPath(**it) << ",";
			++*it;
		}
		delete it;
		return log << NEW_LINE;
	} else {
		return log;
	}
}

//-------------------------------------------------------------------------------

ostream &operator << (ostream &log, PltHashTable<PltString, variable_value*> *var_block) {
	PltHashIterator<PltString, variable_value *> *it;

	if (var_block) {
		it = (PltHashIterator<PltString, variable_value *> *) var_block->newIterator();
		while(*it) {	// iterate over variables
			log << (**it).a_value;
			++*it;
		}
		delete it;
	}
	return log;
}

//-------------------------------------------------------------------------------

ostream &operator << (ostream &log, PltList<variable_value *> *var_block_list) {
    PltListIterator<variable_value *> *it;

	if(var_block_list) {
	    it = (PltListIterator<variable_value *> *) var_block_list->newIterator();
	    while(*it) {
	        log << (**it);
	        ++*it;
     	}
     	delete it;
    }
	return log;
}

//-------------------------------------------------------------------------------

ostream &operator << (ostream &log, PltHashTable<PltString, link_value*> *link_block) {
	PltHashIterator<PltString, link_value *> *it;

	if(link_block) {
		it = (PltHashIterator<PltString, link_value *> *) link_block->newIterator();
		while(*it) {	// iterate over links
			log << (**it).a_value;
			++*it;
		}
		delete it;
	}
	return log;
}
//-------------------------------------------------------------------------------

ostream &operator << (ostream &log, PltList<link_value *> *link_block_list) {
    PltListIterator<link_value *> *it;

	if(link_block_list) {
	    it = (PltListIterator<link_value *> *) link_block_list->newIterator();
	    while(*it) {
	        log << (**it);
	        ++*it;
     	}
     	delete it;
    }
	return log;
}

//-------------------------------------------------------------------------------

ostream &operator << (ostream &log, PltHashTable<PltString, instance*> *children) {
	PltHashIterator<PltString, instance *> *it;

	if(children) {
		it = (PltHashIterator<PltString, instance *> *) children->newIterator();
		while(*it) {	// iterate over children
			log << (**it).a_value;
			++*it;
		}
		delete it;
	}
	return log;
}

//-------------------------------------------------------------------------------

ostream &operator << (ostream &log, PltList<instance*> *children_list) {
    PltListIterator<instance *> *it;

	if(children_list) {
	    it = (PltListIterator<instance *> *) children_list->newIterator();
	    while(*it) {
	        log << (**it);
	        ++*it;
     	}
     	delete it;
    }
	return log;
}

//-------------------------------------------------------------------------------

ostream &operator << (ostream &log, instance *inst) {
	if (inst) {
		return log << "Identifier: " << *(inst->ident) << NEW_LINE
				   << "Part: " << inst->is_part << NEW_LINE
				   << "Class Identifier: " << *(inst->class_ident) << NEW_LINE
				   << "Creation Time: " << inst->creation_time << NEW_LINE
				   << "Semantic Flags: " << inst->sem_flags << NEW_LINE
				   << "Comment: " << inst->comment << NEW_LINE
				   << "Variable block list:" << NEW_LINE
				   << inst->var_block_list
				   << "Link block list:" << NEW_LINE
				   << inst->link_block_list
				   << "Part block list:" << NEW_LINE
				   << inst->part_block_list
				   << NEW_LINE
				   << inst->children;
	} else {
		return log;
	}
}

//-------------------------------------------------------------------------------

// Convert a time into an ASCII string
KsString TimeToAscii(const KsTime ptime)
{
	// local variables
	static char		timestring[] = "YYYY/MM/DD hh:mm:ss.uuuuuu";
	struct tm		*ptm;
	time_t			secs = ptime.tv_sec;

	// convert the time to a string
	ptm = gmtime(&secs);
	sprintf(timestring, "%04d/%02d/%02d %02d:%02d:%02d.%06lu",
		ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour,
		ptm->tm_min, ptm->tm_sec, ptime.tv_usec);
	return (KsString)timestring;
} // TimeToAscii

//-------------------------------------------------------------------------------

// Convert a time span into an ASCII string
KsString TimeSpanToAscii(const KsTimeSpan ptimesp)
{
	// local variables
	static char		timestring[] = "hhhh:mm:ss.uuuuuu";
	time_t			secs = ptimesp.tv_sec;
	uint			seconds, minutes, hours;

	// convert the time to a string
	seconds = secs % 60;
	secs -= seconds;
	minutes = secs % 3600;
	secs -= minutes * 60;
	hours = secs / 60;
	sprintf(timestring, "%04d:%02d:%02d.%06lu",
		hours, minutes, seconds, ptimesp.tv_usec);
	return (KsString)timestring;
} // TimeSpanToAscii

//-------------------------------------------------------------------------------

// Get time from string
KsTime GetTime(char *arg)
{
	// local variables
	static char		format[] = "0000/00/00 00:00:00.000000";
	struct tm		tm;
	time_t			secs;
	uint			usecs;
	char			*pc1, *pc2;
	KsTime			ptime;

	// check the format of the given string, which must be either
	// YYYY/MM/DD, YYYY/MM/DD hh:mm:ss or YYYY/MM/DD hh:mm:ss.uuuuuu
	if(!arg) {
		OUT_STREAM << "Error: Bad time format." << NEW_LINE;
		exit (-1);
	}
	for(pc1=format, pc2=arg; *pc1; pc1++, pc2++) {
		if(!*pc2) {
			if((*pc1 == ' ') || (*pc1 == '.')) {
				break;
			}
			OUT_STREAM << "Error: Bad time format." << NEW_LINE;
			exit (-1);
		}
		switch(*pc1) {
		case '0':
			if(!((*pc2 >= '0') && (*pc2 <= '9'))) {
				OUT_STREAM << "Error: Bad time format." << NEW_LINE;
				exit (-1);
			}
			break;
		case '/':
		case ':':
		case ' ':
		case '.':
			if(*pc2 != *pc1) {
				OUT_STREAM << "Error: Bad time format." << NEW_LINE;
				exit (-1);
			}
			break;
		default:
			OUT_STREAM << "Error: Bad time format." << NEW_LINE;
			exit (-1);
		}
	}

	// convert the string to a time
	memset(&tm, 0, sizeof(tm));
	usecs = 0;
	sscanf(arg, "%d/%d/%d %d:%d:%d.%u", &tm.tm_year, &tm.tm_mon,
		&tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec, &usecs);
	tm.tm_year -= 1900;
	tm.tm_mon--;
	secs = mktime(&tm);
	secs -= timezone;	// compensate time zone shift
	if(secs < 0) {
		OUT_STREAM << "Error: Time conversion failed." << NEW_LINE;
		exit (-1);
	}
	// time is OK, return value
	ptime.tv_sec = secs;
	ptime.tv_usec = usecs;
	return ptime;
} // GetTime

//-------------------------------------------------------------------------------

// Get time span from string
KsTimeSpan GetTimeSpan(char *arg)
{
	// local variables
	static char		format[] = "0000:00:00.000000";
	char			*pc1, *pc2;
	KsTimeSpan		ptime;
	uint			hours, minutes, seconds, usecs;

	// check the format of the given string, which must be either
	// hhhh:mm:ss or hhhh:mm:ss.uuuuuu
	if(!arg) {
		OUT_STREAM << "Error: Bad time span format." << NEW_LINE;
		exit (-1);
	}
	for(pc1=format, pc2=arg; *pc1; pc1++, pc2++) {
		if(!*pc2) {
			if(*pc1 == '.') {
				break;
			}
			OUT_STREAM << "Error: Bad time span format." << NEW_LINE;
			exit (-1);
		}
		switch(*pc1) {
		case '0':
			if(!((*pc2 >= '0') && (*pc2 <= '9'))) {
				OUT_STREAM << "Error: Bad time span format." << NEW_LINE;
				exit (-1);
			}
			break;
		case ':':
		case '.':
			if(*pc2 != *pc1) {
				OUT_STREAM << "Error: Bad time span format." << NEW_LINE;
				exit (-1);
			}
			break;
		default:
			OUT_STREAM << "Error: Bad time span format." << NEW_LINE;
			exit (-1);
		}
	}

	// convert the string to a time
	usecs = 0;
	sscanf(arg, "%d:%d:%d.%u", &hours, &minutes, &seconds, &usecs);
	if(!(
			usecs >= 0 && usecs <= 999999 &&
			seconds >= 0 && seconds <= 59 &&
			minutes >= 0 && minutes <= 59 &&
			hours >= 0 && hours <= 9999
	  	)) {
		OUT_STREAM << "Error: Time span conversion failed." << NEW_LINE;
		exit (-1);
	}
	// time is OK, return value
	ptime.tv_sec = hours * 3600 + minutes * 60 + seconds;
	ptime.tv_usec = usecs;
	return ptime;
} // GetTimeSpan

//-------------------------------------------------------------------------------

// Get semantic flags from string
KS_SEMANTIC_FLAGS GetSemFlags(KsString *ksarg)
{
	KS_SEMANTIC_FLAGS	ret;
	uint				i;
	char				*arg;

	arg = (char *)malloc(33);
	strcpy(arg,*ksarg);

	if (arg[0] == '\0') {
		free(arg);
		return 0;
	}
	i = 0;
	ret = 0;
	while ((arg[i] != '\0') &&							// verify parameter format
			(isupper(arg[i])  ||
			(arg[i] >= 'a' && arg[i] <= 'f')) &&
			(arg[i+1] == '\0' || arg[i+1] > arg[i])) {

		if (isupper(arg[i])) {
			ret |= (1 << (arg[i] - 'A'));				// build bit field
		} else {
			ret |= (1 << (26 + arg[i] -'a'));
		}
		i++;
	}
	if (i == strlen(arg)) {
		free(arg);
	} else {
		OUT_STREAM << "Error: Invalid semantic flags." << NEW_LINE;
		free(arg);
		exit(-1);
	}
	
	return ret;
} // GetSemFlags

//-------------------------------------------------------------------------------

// Check variable types for compatibility, class information overwrites parse
// results (parser cannot distinguish int and unsigned int, for example)
bool compat_types(enum value_types &type1, KS_VAR_TYPE type2)
{
	type2 &= 0xFFFF;		// KS type mask
	
	// the variabletype in the database is ANY (defined by OV not by KS)
	// so any variabletype is allowed here
	// for OV_VT_ANY we need ov_ov.h
	if (type2 == OV_VT_ANY) {
	    if (verbose) OUT_STREAM << "Found variabletype: ANY!" << NEW_LINE;
		return true;
 	}
 	
 	// if ANY type is not allowed, you have to check the types
	switch (type1) {
		case DB_VT_BOOL			:	if (type2 == KS_VT_BOOL) {
										return true;
									}
									break;
		case DB_VT_INT			:	if (type2 == KS_VT_INT) {
										return true;
									}
									if (type2 == KS_VT_UINT) {
										type1 = DB_VT_UINT;
										return true;
									}
									if (type2 == KS_VT_SINGLE) {
										type1 = DB_VT_INT_TO_SINGLE;
										return true;
									}
									if (type2 == KS_VT_DOUBLE) {
										type1 = DB_VT_INT_TO_DOUBLE;
										return true;
									}
									break;
		case DB_VT_DOUBLE		:	if (type2 == KS_VT_DOUBLE) {
										return true;
									}
									if (type2 == KS_VT_SINGLE) {
										type1 = DB_VT_DOUBLE_TO_SINGLE;
										return true;
									}
									break;
		case DB_VT_STRING		:	if (type2 == KS_VT_STRING) {
										return true;
									}
									break;
		case DB_VT_TIME			:	if (type2 == KS_VT_TIME) {
										return true;
									}
									if (type2 == KS_VT_TIME_SPAN) {
										type1 = DB_VT_TIME_SPAN;
										return true;
									}
									break;
		case DB_VT_TIME_SPAN	:	if (type2 == KS_VT_TIME_SPAN) {
										return true;
									}
									if (type2 == KS_VT_TIME) {
										type1 = DB_VT_TIME;
										return true;
									}
									break;
		case DB_VT_STRUCTURE	:	if (type2 == KS_VT_STRUCT) {
										return true;
									}
									break;
		case DB_VT_VOID			:	if (type2 == KS_VT_VOID) {
										return true;
									}
									break;
		// added for multiple runs in proof handling
		case DB_VT_DOUBLE_TO_SINGLE : if (type2 == KS_VT_SINGLE) {
										return true;
									}
									break;
		case DB_VT_UINT			:	if (type2 == KS_VT_UINT) {
										return true;
									}
									break;
		case DB_VT_INT_TO_SINGLE:	if (type2 == KS_VT_SINGLE) {
										return true;
									}
									break;
		case DB_VT_INT_TO_DOUBLE:	if (type2 == KS_VT_DOUBLE) {
										return true;
									}
									break;
		default					:	return false;
	}
	return false;
} // compat_types

//-------------------------------------------------------------------------------

// Check vector variable types for compatibility
bool compat_vectors(vector *vec, KS_VAR_TYPE type2)
{
	type2 &= 0xFFFF;			// KS type mask

	switch (type2) {			// get type from class definition
		case KS_VT_BOOL_VEC			:	vec->type = DB_VT_BOOL;
										break;
		case KS_VT_INT_VEC			:	vec->type = DB_VT_INT;
										break;
		case KS_VT_UINT_VEC			:	vec->type = DB_VT_UINT;
										break;
		case KS_VT_SINGLE_VEC		:	vec->type = DB_VT_SINGLE;
										break;
		case KS_VT_DOUBLE_VEC		:	vec->type = DB_VT_DOUBLE;
										break;
		case KS_VT_STRING_VEC		:	vec->type = DB_VT_STRING;
										break;
		case KS_VT_TIME_VEC			:	vec->type = DB_VT_TIME;
										break;
		case KS_VT_TIME_SPAN_VEC	:	vec->type = DB_VT_TIME_SPAN;
										break;
		default						:	return false;
	}

	if (!vec->content) {		// empty vector
		return true;
	}

	vector *act = vec;
	while (act) {				// for all vector elements
		enum value_types *type1 = &(act->content->type);
		switch (*type1) {
			case DB_VT_BOOL			:	if (type2 == KS_VT_BOOL_VEC) {
											act = act->next;
											continue;
										}
										return false;
			case DB_VT_INT			:	if (type2 == KS_VT_INT_VEC) {
											act = act->next;
											continue;
										}
										if (type2 == KS_VT_UINT_VEC) {
											*type1 = DB_VT_UINT;
											act = act->next;
											continue;
										}
										if (type2 == KS_VT_SINGLE_VEC) {
											*type1 = DB_VT_INT_TO_SINGLE;
											act = act->next;
											continue;
										}
										if (type2 == KS_VT_DOUBLE_VEC) {
											*type1 = DB_VT_INT_TO_DOUBLE;
											act = act->next;
											continue;
										}
										return false;
			case DB_VT_DOUBLE		:	if (type2 == KS_VT_DOUBLE_VEC) {
											act = act->next;
											continue;
										}
										if (type2 == KS_VT_SINGLE_VEC) {
											*type1 = DB_VT_DOUBLE_TO_SINGLE;
											act = act->next;
											continue;
										}
										return false;
			case DB_VT_STRING		:	if (type2 == KS_VT_STRING_VEC) {
											act = act->next;
											continue;
										}
										return false;
			case DB_VT_TIME			:	if (type2 == KS_VT_TIME_VEC) {
											act = act->next;
											continue;
										}
										if (type2 == KS_VT_TIME_SPAN_VEC) {
											*type1 = DB_VT_TIME_SPAN;
											act = act->next;
											continue;
										}
										return false;
			case DB_VT_TIME_SPAN	:	if (type2 == KS_VT_TIME_SPAN_VEC) {
											act = act->next;
											continue;
										}
										if (type2 == KS_VT_TIME_VEC) {
											*type1 = DB_VT_TIME;
											act = act->next;
											continue;
										}
										return false;
			// added for multiple runs in proof handling
			case DB_VT_UINT			:	if (type2 == KS_VT_UINT_VEC) {
											act = act->next;
											continue;
										}
										return false;
			case DB_VT_INT_TO_SINGLE :	if (type2 == KS_VT_SINGLE_VEC) {
											act = act->next;
											continue;
										}
										return false;
			case DB_VT_INT_TO_DOUBLE :	if (type2 == KS_VT_DOUBLE_VEC) {
											act = act->next;
											continue;
										}
										return false;
			case DB_VT_DOUBLE_TO_SINGLE:if (type2 == KS_VT_SINGLE_VEC) {
											act = act->next;
											continue;
										}
										return false;
			default					:	return false;
		}
	}
	return true;

} // compat_vectors

//-------------------------------------------------------------------------------

// Check classes for (inheritance) compatibility
bool compat_classes(const LogPath &type1, const KsString &type2)
{
#if 0
// TODO: Welches Kompatibilit�tskriterium ist sinnvoll?
	KsString	parent_path;

	KscVariable start(type2 + ".baseclass");
	if (!start.Update()) {
		KscVariable start(type2 + ".parentclass");
		if (!start.getUpdate()) {
			OUT_STREAM << "Error: Cannot check class compatibility for " << type2
				 << "!" << NEW_LINE;
			return false;
		}
	}
	parent_path = (KsStringValue &) start.getCurrProps()->value;
	KscDomain parent_dom(parent_path);
#endif
	return false;							// require full compatibility by now
} // compat_classes

//-------------------------------------------------------------------------------

// Search and compare objects in target server
bool checkObjs(instance *node)
{
	KsString domname = server + KsString(*node->ident);
	KscDomain dom(domname);
	if (verbose) OUT_STREAM << "..." << *node->ident << NEW_LINE;
	if (node->is_part) {
	        node->cr_opts = CO_CHANGE;
	        return TRUE;
	}
	if (!dom.getEngPropsUpdate()) {					// object not in server
		if (!(lopts & LO_ADD_DOMAINS) && node->cr_opts == CO_TREE) {
			OUT_STREAM << "Error: Parent domain of " << *(node->ident) << " not in server!" << NEW_LINE;
			return false;
		} else {
			node->cr_opts = CO_CREATE;				// => must be created
		}
	} else {										// object in server, check compatibility
		if (strcmp(KsString(*(node->class_ident)), ((KsDomainEngProps*) dom.getEngProps())->class_identifier) == 0) {
													// classes are equal
			if (lopts & LO_OVERWRITE_EXACT) {
				node->cr_opts = CO_CHANGE;
			}
		} else if (compat_classes(*(node->class_ident), ((KsDomainEngProps*) dom.getEngProps())->class_identifier)) {
													// classes are compatible
			if (lopts & LO_OVERWRITE_INHERITED) {
				node->cr_opts = CO_OVERWRITE;
			}
		} else {									// classes are incompatible
			if (lopts & LO_ERROR_ON_INCOMPATIBLE) {
				OUT_STREAM << "Error: Types of " << *(node->ident) << " are incompatible!" << NEW_LINE;
				return false;
			}
			if (lopts & LO_OVERWRITE_INCOMPATIBLE) {	// force overwrite
				node->cr_opts = CO_OVERWRITE;
			}
		}
	}
	return true;										// no errors => success
} // checkObjs

//-------------------------------------------------------------------------------

// Search and compare objects in target server
bool check_objects()
{
 	if (verbose) OUT_STREAM << "Checking objects ..." << NEW_LINE;
 	return (parse_tree->forEach(&checkObjs));
} //

//-------------------------------------------------------------------------------

// Check compatibility of vendor branches in parse tree and server,
// separate check as vendor branch has different variable format (vendor/x instead of .x)
bool check_vendortree()
{
	instance										*vendor;
	PltListIterator<variable_value *>				*act_var;
	bool											ok;
	KS_VAR_TYPE										vartype;
	LogPath											lp;
	
	lp = LogPath("/vendor");
	vendor = parse_tree->search(&lp);

	if (!vendor) {										// no vendor branch in parse tree
		return true;
	}
	act_var = (PltListIterator<variable_value *>	*) vendor->var_block_list->newIterator();

	while (*act_var) {
		KscVariable var(server + "/vendor/" + *((**act_var)->ident));
		if (!var.getEngPropsUpdate()) {					// variable not found in server
			OUT_STREAM << "Error: Cannot access variable /vendor/" << *((**act_var)->ident)
				 << " in server!" << NEW_LINE;
		}
		vartype = ((KsVarEngProps*) var.getEngProps())->type;
		if ((**act_var)->val->type == DB_VT_VECTOR) {
			ok = compat_vectors((**act_var)->val->v.pvector_val, vartype);
		} else {
			ok = compat_types((**act_var)->val->type, vartype);
		}
		if (!ok) {										// variable types are incompatible
			OUT_STREAM << "Error: Conflicting types for variable "
				 << *((**act_var)->ident) << "!" << NEW_LINE;
			return false;
		}
		if (strcmp ( *((**act_var)->ident) , "/vendor/activity_lock" ) == 0) {
			(**act_var)->val->v.bool_val = true;	// always set activity lock in parse tree
		}
		++*act_var;
	}
	return true;

} // check_vendortree

//-------------------------------------------------------------------------------

// check class compatibility of instance "node"
bool checkClassId(instance *node) {

	int												ret;
	PltListIterator<variable_value *>				*act_var;
	PltListIterator<link_value *>					*act_link;
	PltListIterator<instance *>						*act_part;
	int												i;
	bool											found = true;
	bool											ok = false;
	instance										*class_obj, *act_cobj, *var_inst;
	variable_value									*var_type, *var_props; //, *dummy;
	link_value										*bc;
	bool											succ;

	if (strncmp(node->ident->getHead(), "vendor", 6) == 0) {	// skip /vendor branch
		return true;
	}
	KsString domname(server + KsString(*(node->class_ident)));
	KscDomain dom(domname);
	if (verbose) OUT_STREAM << "..." << *node->class_ident << NEW_LINE;
	if (!dom.getEngPropsUpdate()) {								// class not in server
		if (!(lopts & LO_ADD_CLASSES)) {
			OUT_STREAM << "Error: Class " << *(node->class_ident) << " not in server!" << NEW_LINE;
			return false;
		} else {												// search class in parse tree
			class_obj = parse_tree->search(node->class_ident);
			if (!class_obj) {
				OUT_STREAM << "Error: Class " << *(node->class_ident) << " not found!" << NEW_LINE;
				return false;
			}
			if (node->var_block_list) {
				act_var = (PltListIterator<variable_value *>	*) node->var_block_list->newIterator();		// check variables of instance for
																// compliance to class definition
				while (*act_var) {							// for all variables of the instance
					ret = 0;

					LogPath act_class_name(*(class_obj->ident));

					while (!ret && act_class_name.isValid()) {
											// look for variable in the class definition
											// of the object first, if not found search the base classes
						act_cobj = parse_tree->search(&act_class_name);
						if (! act_cobj) {
							OUT_STREAM << "Error: Class " << act_class_name << " not found!" << NEW_LINE;
							return false;
						}
						if (act_cobj->children) {
							if (act_cobj->children->query(act_class_name + "/" + ((PltString) *((**act_var)->ident)), var_inst)) {
								KsString ksvar("vartype");
								var_type = var_inst->search_var(&ksvar);
								if (!var_type) {
									OUT_STREAM << "Error: Class " << *(node->class_ident) << "," << NEW_LINE
										 << "       cannot get type of variable " << *((**act_var)->ident)
										 << "!" << NEW_LINE;
									return false;
								}
								KsString ksvarprop("varprops");
								var_props = var_inst->search_var(&ksvarprop);
								if (!var_props) {
									OUT_STREAM << "Error: Class " << *(node->class_ident) << "," << NEW_LINE
										 << "       cannot get props of variable " << *((**act_var)->ident)
										 << "!" << NEW_LINE;
									return false;
								}
								if (var_props->val->v.int_val & 0x4) {		// variable is derived, must not be set
									node->var_block_list->remove((**act_var));
									ret = 1;
									break;
								}
								if ((**act_var)->val->type == DB_VT_VECTOR) {
									ok = compat_vectors((**act_var)->val->v.pvector_val, var_type->val->v.int_val);
								} else {
									ok = compat_types((**act_var)->val->type, var_type->val->v.int_val);
								}
								if (ok) {					// variable types are compatible
									ret = 1;
									break;
								} else {
									OUT_STREAM << "Error: Class " << *(node->class_ident) << "," << NEW_LINE
										 << "       variable " << *((**act_var)->ident) << " not compatible!" << NEW_LINE;
									return false;
								}
							} // if (var_inst)
						} // if (act_cobj->children)
						KsString ksbcstr("baseclass");
						bc = act_cobj->search_link(&ksbcstr);
						if (bc) {
							if (bc->link_paths) {
								PltListIterator<LogPath> *it = (PltListIterator<LogPath> *) bc->link_paths->newIterator();
								act_class_name = **it;					// path of the base class
								delete it;
							} else {
								act_class_name = LogPath("");
							}
							continue;
						} else {
							OUT_STREAM << "Error: Cannot get base class of class " << act_class_name
								 << "!" << NEW_LINE;
							return false;
						}
					} // while (!ret && act_class_name.isValid())

					if (!ret) {
						OUT_STREAM << "Error: Class " << *(node->class_ident) << "," << NEW_LINE
							 << "       variable " << *((**act_var)->ident) << " not compatible!" << NEW_LINE;
						return false;
					}
					++*act_var;
				} // while (*act_var)
				delete act_var;
			} // if (node->var_block_list)

		} // if (!lopts ..)
		node->cr_opts = CO_CREATE;

// TODO: check links and parts when class not in server

	} else {										// class in server, compatible?

		// check variables
		if (node->var_block_list) {
			act_var = (PltListIterator<variable_value *>	*) node->var_block_list->newIterator();
			while (*act_var) {						// for all variables of the instance
				if (verbose) OUT_STREAM << "      check variable " << *((**act_var)->ident) << NEW_LINE;
				LogPath act_class_name = LogPath(*(node->class_ident));
				KsString domname(server + act_class_name + "/" + *((**act_var)->ident));
				KscDomain var(domname);		// meta information about variable is stored in domain object
				if (!var.getEngPropsUpdate()) {					// class variable not found in server
					found = false;
					while (act_class_name.isValid() ) {			// search the base classes for
																// the variable definition
						KscVariable parent(server + act_class_name + ".baseclass");
						if (! parent.getUpdate() ) {
							OUT_STREAM << "Error: Cannot get base class of " << act_class_name << "!" << NEW_LINE;
							return false;
						}
						const KsVarCurrProps *parentprops = parent.getCurrProps();
						act_class_name = (KsStringValue &) *parentprops->value;
						if (act_class_name.isValid()) {
							KscDomain var(server + act_class_name + "/" + *((**act_var)->ident));
							if (var.getEngPropsUpdate()) {			// variable in class definition
								found = true;
								break;
							}
						}
					} // while
				}
				if (!found) {
					OUT_STREAM << "Error: Class " << *(node->class_ident) << "," << NEW_LINE
						 << "       variable " << var.getName() << " does not exist!" << NEW_LINE;
					return false;
				}
				// variable description is found in server, check compatibility now
				KscVariable vartype(server + act_class_name + "/" + *((**act_var)->ident) + ".vartype");
				if (! (vartype.getEngPropsUpdate() && vartype.getUpdate()) ) {
					OUT_STREAM << "Error: Cannot get variable type of " << NEW_LINE
						 << *(node->class_ident) << "/" << *((**act_var)->ident) << "!" << NEW_LINE;
					return false;
				}
				KscVariable varprops(server + act_class_name + "/" + *((**act_var)->ident) + ".varprops");
				if (! (varprops.getEngPropsUpdate() && varprops.getUpdate()) ) {
					OUT_STREAM << "Error: Cannot get variable props of " << NEW_LINE
						 << *(node->class_ident) << "/" << *((**act_var)->ident) << "!" << NEW_LINE;
					return false;
				}
				const KsVarCurrProps *currprops = varprops.getCurrProps();
				if (((int)((KsIntValue &) *currprops->value)) & 0x4) {		// variable is derived, must not be set
					node->var_block_list->remove((**act_var));
					// create new iterator, because the variable is removed from the list
					// otherwise a nullpointer occurs in act_var
					delete act_var;
					act_var = (PltListIterator<variable_value *>	*) node->var_block_list->newIterator();
					continue;
				}
				currprops = vartype.getCurrProps();
				if ((**act_var)->val->type == DB_VT_VECTOR) {
					// check variable types for compatibility
					ok = compat_vectors((**act_var)->val->v.pvector_val, (int)((KsIntValue &) *currprops->value));
				} else {
					ok = compat_types((**act_var)->val->type, (int)((KsIntValue &) *currprops->value));
				}
				if (!ok) {												// variable type not compatible
					OUT_STREAM << "Error: Class " << *(node->class_ident) << "," << NEW_LINE
						 << "       type of ." << *((**act_var)->ident) << " is not compatible!"
						 << NEW_LINE;
					return false;
				}
				++*act_var;
			} // while (*act_var)
			delete act_var;
		} // if (node->var_block_list);
		// check links
		if (node->link_block_list) {
			act_link = (PltListIterator<link_value *>*) node->link_block_list->newIterator();

			while (*act_link) {								// for all links of the instance
				if (verbose) OUT_STREAM << "      check link " << *((**act_link)->ident) << NEW_LINE;
				succ = false;
				LogPath act_class_name = *(node->class_ident);
				// 1. Get the contents of the variables "childassociation" and "parentassociation"
				//    of the class description, i.e. the association classes the current class can be
				//    linked with.
				// 2. The variables "parentrolename" and "childrolename" of the association class
				//    description contain the names of the parent / child links of the association.
				//    The name of act_link must be one of these names.
				while (!succ && act_class_name.isValid() ) {	// loop over base classes if necessary

					KscVariable childlink(server + act_class_name + ".childassociation");
					if (! childlink.getUpdate() ) {
						OUT_STREAM << "Error: Class " << act_class_name << "," << NEW_LINE
							 << "       cannot get childlink type!" << NEW_LINE
							 << NEW_LINE;
						return false;
					}
					const KsVarCurrProps *childcprops = childlink.getCurrProps();

					KscVariable parentlink(server + act_class_name + ".parentassociation");
					if (! parentlink.getUpdate() ) {
						OUT_STREAM << "Error: Class " << act_class_name << "," << NEW_LINE
							 << "       cannot get parentlink type!" << NEW_LINE
							 << NEW_LINE;
						return false;
					}
					const KsVarCurrProps *parentcprops = parentlink.getCurrProps();

					for(i = 0; (unsigned int) i < ((KsStringVecValue &) *childcprops->value).size(); i++) {
						KscVariable p_role(server + ((KsStringVecValue &) *childcprops->value)[i] + ".parentrolename");
						if (! (p_role.getEngPropsUpdate() && p_role.getUpdate()) ) {
							OUT_STREAM << "Error: Class " << *(node->class_ident) << "," << NEW_LINE
								 << "       cannot get parentrolename!" << NEW_LINE
								 << NEW_LINE;
							return false;
						}
						const KsVarCurrProps *p_role_props = p_role.getCurrProps();
						if (strcmp(*((**act_link)->ident), (KsStringValue &) *p_role_props->value) == 0) {
							succ = true;
							break;
						}
					}

					for(i = 0; !succ && (unsigned int) i < ((KsStringVecValue &) *parentcprops->value).size(); i++) {
						KscVariable c_role(server + ((KsStringVecValue &) *parentcprops->value)[i] + ".childrolename");
						if (! (c_role.getEngPropsUpdate() && c_role.getUpdate()) ) {
							OUT_STREAM << "Error: Class " << *(node->class_ident) << "," << NEW_LINE
								 << "       cannot get childrolename!" << NEW_LINE
								 << NEW_LINE;
							return false;
						}
						const KsVarCurrProps *c_role_props = c_role.getCurrProps();
						if (strcmp(*((**act_link)->ident), (KsStringValue &) *c_role_props->value) == 0) {
							succ = true;
							break;
						}
					}
					// if link name not found try base class
					if (!succ) {
						KscVariable parent(server + act_class_name + ".baseclass");
						if (! parent.getUpdate() ) {
							OUT_STREAM << "Error: Cannot get base class of " << act_class_name << "!" << NEW_LINE;
							return false;
						}
						const KsVarCurrProps *parentprops = parent.getCurrProps();
						act_class_name = (KsStringValue &) *parentprops->value;
					}
				} // while (!succ ...)
				if (!succ) {
					OUT_STREAM << "Error: Class " << *(node->class_ident) << ": link type of " << NEW_LINE
						 << *((**act_link)->ident) << " is not compatible!" << NEW_LINE;
					return false;
				}
				++*act_link;
			} // while (*act_link)
			delete act_link;
		} // if (node->link_block_list)

		// check class type of parts
		if (node->part_block_list) {
			act_part = (PltListIterator<instance *>*) node->part_block_list->newIterator();

			PltString * helpString;

			while (*act_part) {									// for all parts of the instance
				if (verbose) OUT_STREAM << "      check part " << *((**act_part)->ident) << NEW_LINE;
				LogPath act_class_name = LogPath(*(node->class_ident));

				// get part name
				int j;
				helpString = (PltString *) (**act_part)->ident;
				for (j = helpString->len() - 1; j >= 0; j--) {
					if ( ((const char *) *helpString)[j] == '.') {
						break;
					}
				}
				KsString help = KsString(& ((const char *) *helpString)[j+1] );
				// info about part is stored in domain "class_name/part_name"
				KsString domname(server + act_class_name + "/" + help);
				KscDomain part(domname);
				found = true;
				if (!part.getEngPropsUpdate()) {				// class part not found in server
					found = false;
					while (act_class_name.isValid() ) {			// search the base classes for
																// the part definition
						KscVariable parent(server + act_class_name + ".baseclass");
						if (! parent.getUpdate() ) {
							OUT_STREAM << "Error: Cannot get base class of " << act_class_name << "!" << NEW_LINE;
							return false;
						}
						const KsVarCurrProps *parentprops = parent.getCurrProps();
						act_class_name = (KsStringValue &) *parentprops->value;
						if (act_class_name.isValid() ) {
							KscDomain part(server + act_class_name + "/" + help);
							if (part.getEngPropsUpdate()) {
								found = true;
								break;
							}
						}
					} // while
				}
				if (!found) {
					OUT_STREAM << "Error: Class " << *(node->class_ident) << "," << NEW_LINE
						 << "       part " << part.getName() << " does not exist!" << NEW_LINE;
					return false;
				}
				// part found, now compare class identifiers
				KscVariable clid(server + act_class_name + "/" + help + ".partclass");
				if (! clid.getUpdate() ) {
					OUT_STREAM << "Error: Cannot get partclass of " << act_class_name << "/"
						 << help << "!" << NEW_LINE;
					return false;
				}
				const KsVarCurrProps *clidprops = clid.getCurrProps();
				KsString clname = KsString((KsStringValue &) *clidprops->value);

				if (strcmp(clname, (PltString) *((**act_part)->class_ident) ) != 0) {
					OUT_STREAM << "Error: Class " << *(node->class_ident) << ": part class " << NEW_LINE
						 << help << " is not compatible!" << NEW_LINE;
					return false;
				}
				++*act_part;
			} // while (*act_part)
			delete act_part;
		} // if (node->part_block_list)

	} // if (!dom.get ...

	return true;
} // checkClassId

//-------------------------------------------------------------------------------

// Are all necessary classes available in target server?
bool check_class_tree()
{
	if (verbose) OUT_STREAM << "Checking classes..." << NEW_LINE;
	return parse_tree->forEach(&checkClassId);
} // check_class_tree

//-------------------------------------------------------------------------------

// write the instance "node" in the target server
bool write_instance(instance *node)
{
	KsCreateObjParams		create_params;
	KsCreateObjResult		create_result;
	KsDeleteObjParams		delete_params;
	KsDeleteObjResult		delete_result;
	KsPlacementHint			pmh;

	if (verbose) OUT_STREAM << "write instance" << *node->ident << " in mode " << node->cr_opts << NEW_LINE;
	// instance shall be overwritten => delete old instance first
	if (node->cr_opts == CO_OVERWRITE) {
		KsArray<KsString> del_path(1);
		del_path[0] = KsString(*(node->ident));
		delete_params.paths = del_path;
		if (!server_base->requestByOpcode(KS_DELETEOBJECT,
										  0,
										  delete_params,
										  delete_result)) {
			OUT_STREAM << "Error deleting object " << *(node->ident) << "!" << NEW_LINE;
			return false;
		}
		if (delete_result.result != KS_ERR_OK) {
			OUT_STREAM << "Error " << delete_result.result << " deleting object " << *(node->ident)
				 << "!" << NEW_LINE;
			return false;
		}
		if (delete_result.results.size() != 1) {
			OUT_STREAM << "Error: Out of memory in write_instance!" << NEW_LINE;
			return false;
		}
		node->cr_opts = CO_CREATE;		// now new instance can be created
	} // if (node->cr_opts == CO_OVERWRITE)

	// instance not present, must be created
	if (node->cr_opts == CO_CREATE) {
		create_params.items = KsArray<KsCreateObjItem>(1);
		if (create_params.items.size() != 1) {
			OUT_STREAM << "Error: Out of memory in write_instance!" << NEW_LINE;
			return false;
		}
		create_params.items[0].factory_path = PltString(*(node->class_ident));
		create_params.items[0].new_path = PltString(*(node->ident));
		pmh.hint = KS_PMH_DEFAULT;
		create_params.items[0].place = pmh;
		KsArray<KsLinkItem> links(0);
		create_params.items[0].links = links;
		KsArray<KsSetVarItem> cparams(0);
		create_params.items[0].parameters = cparams;

		if (!server_base->requestByOpcode(KS_CREATEOBJECT,
										  0,
										  create_params,
										  create_result)) {
			OUT_STREAM << "Error creating object " << *(node->ident) << "!" << NEW_LINE;
			return false;
		}

		if (create_result.result != KS_ERR_OK && create_result.result != KS_ERR_ALREADYEXISTS) {
			OUT_STREAM << "Error " << create_result.result << " creating object " << *(node->ident)
				 << "!" << NEW_LINE;
			return false;
		}
		if (verbose && create_result.result == KS_ERR_ALREADYEXISTS) {
			OUT_STREAM << "Warning: Object " << *(node->ident) << " already exists!" << NEW_LINE;
		}
		if (create_result.obj_results.size() != 1) {
			OUT_STREAM << "Error: Out of memory!" << NEW_LINE;
			return false;
		}
		if (create_result.obj_results[0].result != KS_ERR_OK &&
				create_result.obj_results[0].result != KS_ERR_ALREADYEXISTS) {
			OUT_STREAM << "Error " << create_result.obj_results[0].result << " creating object " << *(node->ident)
				 << "!" << NEW_LINE;
			return false;
		}
		if (verbose) {
			OUT_STREAM << "Created object " << *(node->ident) << " successfully!" << NEW_LINE;
		}
		node->cr_opts = CO_CHANGE;		// values of new instance must still be set
	} // if (node->cr_opts == CO_CREATE)
	return true;

} // write_instance


//-------------------------------------------------------------------------------

// write the variables of instance "node" in the target server

bool write_variables(instance *node)
{
	KsCreateObjParams		create_params;
	KsCreateObjResult		create_result;
	KsDeleteObjParams		delete_params;
	KsDeleteObjResult		delete_result;
	KsPlacementHint			pmh;
	KsString				dummy;
	vector					*act_vec;
	int						vec_sz;
	enum value_types		vec_t;
	unsigned int						i;
	// iterate over all variables of the instance to be changed
	if (node->cr_opts == CO_CHANGE && node->var_block_list) {
		PltListIterator<variable_value *> *it = (PltListIterator<variable_value *> *) node->var_block_list->newIterator();
		unsigned int nr_v = 0;
		while (*it) {
		    PltString delimiter;
			if (strncmp(node->ident->getHead(), "vendor", 6) != 0) {	//do not write variables of vendor tree // different var. format
//				delimiter = "/";
//			} else {
				delimiter = ".";
//			}
				KscVariable help(server + *(node->ident) + delimiter + *((**it)->ident) );
				if (!help.getEngPropsUpdate()) {
					OUT_STREAM << "Error: Variable " << *(node->ident) << delimiter << *((**it)->ident)
						 << " does not exist!" << NEW_LINE;
					delete it;
					return false;
				}
				const KsVarEngProps *helpprops = (KsVarEngProps *) help.getEngProps();
				if ((helpprops->access_mode & KS_AC_WRITE) || (use_activitylock)) {		// count writable variables
					nr_v++;
				} else {
					(**it)->val->type = DB_VT_NONE;		// ignore read-only variables
				}
			} // end of do not write vendor tree variables
			++*it;
		} // while (*it)

		KsArray<KsSetVarItem> params(nr_v);
		it->toStart();
		i = 0;
		// create and set variable value depending on data type
		while (*it) {
		    KsValue *val;
			switch((**it)->val->type) {
				case DB_VT_NONE:					// skip read-only variables
					++*it;
					continue;
				case DB_VT_BOOL:
					val = new KsBoolValue( (**it)->val->v.bool_val);
					break;
				case DB_VT_INT:
					val = new KsIntValue( (**it)->val->v.int_val);
					break;
				case DB_VT_DOUBLE:
					val = new KsDoubleValue( (**it)->val->v.double_val);
					break;
				case DB_VT_INT_TO_DOUBLE:			// convert to double
					val = new KsDoubleValue((double) (**it)->val->v.int_val);
					break;
				case DB_VT_STRING:
					if (ksStringFromPercent(*((**it)->val->v.pstring_val), dummy)==KS_ERR_OK)
						val = new KsStringValue(dummy);
					else val = new KsStringValue("");
					break;
				case DB_VT_TIME:
					val = new KsTimeValue(*((**it)->val->v.ptime_val) );
					break;
				case DB_VT_TIME_SPAN:
					val = new KsTimeSpanValue(*((**it)->val->v.ptime_span_val) );
					break;
				case DB_VT_UINT:
					val = new KsUIntValue((u_int) (**it)->val->v.int_val);
					break;
				case DB_VT_INT_TO_SINGLE:			// convert to single
					val = new KsSingleValue((float) (**it)->val->v.int_val);
					break;
				case DB_VT_DOUBLE_TO_SINGLE:		// convert to single
					val = new KsSingleValue((float) (**it)->val->v.double_val);
					break;
				case DB_VT_TIME_TO_TIME_SPAN:		// convert to time span
					val = new KsTimeSpanValue( (**it)->val->v.ptime_val->tv_sec,
											   (**it)->val->v.ptime_val->tv_usec);
					break;
				case DB_VT_TIME_SPAN_TO_TIME:		// convert to time
					val = new KsTimeValue( (**it)->val->v.ptime_span_val->tv_sec,
										   (**it)->val->v.ptime_span_val->tv_usec);
					break;
				case DB_VT_VECTOR:
					act_vec = (**it)->val->v.pvector_val;
					vec_t = act_vec->type;
					vec_sz = 0;
					while(act_vec && act_vec->content) {	// count vector elements
						vec_sz++;
						act_vec = act_vec->next;
					}
					act_vec = (**it)->val->v.pvector_val;
					switch(vec_t) {
						case DB_VT_BOOL: {
							KsBoolVecValue *bvec = new KsBoolVecValue(vec_sz);
							for (int j=0; j<vec_sz; j++) {
								(*bvec)[j] = act_vec->content->v.bool_val;
								act_vec = act_vec->next;
							}
							val = bvec;
							break;
						}
						case DB_VT_INT: {
							KsIntVecValue *ivec = new KsIntVecValue(vec_sz);
							for (int j=0; j<vec_sz; j++) {
								(*ivec)[j] = act_vec->content->v.int_val;
								act_vec = act_vec->next;
							}
							val = ivec;
							break;
						}
						case DB_VT_DOUBLE: {
							KsDoubleVecValue *dvec = new KsDoubleVecValue(vec_sz);
							for (int j=0; j<vec_sz; j++) {
								if (act_vec->content->type == DB_VT_DOUBLE) {
									(*dvec)[j] = act_vec->content->v.double_val;
								} else {	// DB_VT_INT_TO_DOUBLE
									(*dvec)[j] = (double)act_vec->content->v.int_val;
								}
								act_vec = act_vec->next;
							}
							val = dvec;
							break;
						}
						case DB_VT_STRING: {
							KsStringVecValue *stvec = new KsStringVecValue(vec_sz);
							for (int j=0; j<vec_sz; j++) {
								if (ksStringFromPercent(*(act_vec->content->v.pstring_val), dummy)==KS_ERR_OK)
									(*stvec)[j] = dummy;
								else (*stvec)[j] = KsString("");
								act_vec = act_vec->next;
							}
							val = stvec;
							break;
						}
						case DB_VT_TIME: {
							KsTimeVecValue *tvec = new KsTimeVecValue(vec_sz);
							for (int j=0; j<vec_sz; j++) {
								if (act_vec->content->type == DB_VT_TIME) {
									(*tvec)[j] = *(act_vec->content->v.ptime_val);
								} else {	// DB_VT_TIME_SPAN_TO_TIME
									(*tvec)[j] = KsTime(act_vec->content->v.ptime_span_val->tv_sec,
														act_vec->content->v.ptime_span_val->tv_usec);
								}
								act_vec = act_vec->next;
							}
							val = tvec;
							break;
						}
						case DB_VT_TIME_SPAN: {
							KsTimeSpanVecValue *tsvec = new KsTimeSpanVecValue(vec_sz);
							for (int j=0; j<vec_sz; j++) {
								if (act_vec->content->type == DB_VT_TIME_SPAN) {
									(*tsvec)[j] = *(act_vec->content->v.ptime_span_val);
								} else {	// DB_VT_TIME_TO_TIME_SPAN
									(*tsvec)[j] = KsTimeSpan(act_vec->content->v.ptime_val->tv_sec,
															 act_vec->content->v.ptime_val->tv_usec);
								}
								act_vec = act_vec->next;
							}
							val = tsvec;
							break;
						}
						case DB_VT_VOID: {
							KsIntVecValue *vvec = new KsIntVecValue(0);
							val = vvec;
							break;
						}
						case DB_VT_UINT: {
							KsUIntVecValue *uivec = new KsUIntVecValue(vec_sz);
							for (int j=0; j<vec_sz; j++) {
								(*uivec)[j] = (u_int)act_vec->content->v.int_val;
								act_vec = act_vec->next;
							}
							val = uivec;
							break;
						}
						case DB_VT_SINGLE: {
							KsSingleVecValue *svec = new KsSingleVecValue(vec_sz);
							for (int j=0; j<vec_sz; j++) {
								if (act_vec->content->type == DB_VT_SINGLE ||
									act_vec->content->type == DB_VT_DOUBLE_TO_SINGLE) {
									(*svec)[j] = (float)act_vec->content->v.double_val;
								} else {	// DB_VT_INT_TO_SINGLE
									(*svec)[j] = (float)act_vec->content->v.int_val;
								}
								act_vec = act_vec->next;
							}
							val = svec;
							break;
						}
						case DB_VT_VECTOR: {
							OUT_STREAM << "Error: nested vectors are not allowed! (" << *((**it)->ident)
								 << ")" << NEW_LINE;
							delete it;
							return false;
						}
						case DB_VT_STRUCTURE: {
							OUT_STREAM << "Error: variables of type structure are not yet supported! ("
								 << *((**it)->ident) << ")" << NEW_LINE;
							delete it;
							return false;
						}
						default: {
							OUT_STREAM << "Error: " << *((**it)->ident) << " has unknown variable type!"
								 << NEW_LINE;
							delete it;
							return false;
						}
					}
					break;
				case DB_VT_STRUCTURE:
					OUT_STREAM << "Error: variables of type structure are not yet supported! ("
						 << *((**it)->ident) << ")" << NEW_LINE;
					delete it;
					return false;
				case DB_VT_VOID:
					val = new KsVoidValue();
					break;
				default:
					OUT_STREAM << "Error: " << *((**it)->ident) << " has unknown variable type!"
						 << NEW_LINE;
					delete it;
					return false;
			} // switch

			KsValueHandle hvalue(val, KsOsNew);
			KsVarCurrProps *cprops;
			if ((**it)->var_time) {
				cprops = new KsVarCurrProps(hvalue, *((**it)->var_time), (**it)->var_state);
			} else {
			    KsTime t;
			    t.tv_sec = 0;
			    t.tv_usec = 0;
				cprops = new KsVarCurrProps(hvalue, t, (**it)->var_state);
            }
			KsCurrPropsHandle hcprops(cprops, KsOsNew);

			PltString delimiter;
			if (strncmp(node->ident->getHead(), "vendor", 6) != 0) { //do not write variables of vendor tree
			//if (strncmp(node->ident->getHead(), "vendor", 6) 0= 0) { //different handling for vendor tree
			//	delimiter = "/";
			//} else {
			//	delimiter = ".";
			//}
				delimiter = ".";
				params[i].path_and_name = *(node->ident) + delimiter + *((**it)->ident);
				params[i].curr_props = hcprops;
  			} // end if not vendor tree

			++*it;
			++i;
		} // while (*it)
		delete it;

		// Finally set variables of instance
		if (nr_v != 0) {
			KsSetVarParams set_params(nr_v);
			KsSetVarResult set_result(nr_v);
			set_params.items = params;
			if (!server_base->setVar(NULL, set_params, set_result)) {
				OUT_STREAM << "Error setting variables of object " << *(node->ident) << "!" << NEW_LINE;
				return false;
			}
			if (set_result.results.size() != nr_v) {
				OUT_STREAM << "Error: Out of memory in write_variables!" << NEW_LINE;
				return false;
			}
			for (i = 0; i < nr_v; i++) {
				if (set_result.results[i].result == KS_ERR_NOACCESS) {	// ignore inaccessible variables TODO: ???
					if (verbose) {
						OUT_STREAM << "Warning: No access to variable " << params[i].path_and_name
							 << " of object " << *(node->ident) << "!" << NEW_LINE;
					}
				} else {
					if (set_result.results[i].result == KS_ERR_BADVALUE) {	// TODO: ???
						if (verbose) {
							OUT_STREAM << "Warning: Bad value for variable " << params[i].path_and_name
								 << " of object " << *(node->ident) << "! Value not set." << NEW_LINE;
						}
					} else {
						if (set_result.results[i].result != KS_ERR_OK) {
							OUT_STREAM << "Error " << set_result.results[i].result << " setting variable "
								 << params[i].path_and_name << " of object " << *(node->ident) << "!" << NEW_LINE;
							return false;
						}
					}
				}
			}
			if (verbose) {
				OUT_STREAM << "Wrote variables of " << *(node->ident) << " successfully!" << NEW_LINE;
			}
		} // if (nr_v != 0)

	} // if (node->cr_opts == CO_CHANGE ...
	return true;

} // write_variables

//-------------------------------------------------------------------------------

// Find and create all libraries before the other objects
bool find_libraries(instance *node)
{
	if ( *(node->class_ident) == LogPath("/acplt/ov/library") ) {
		if (strcmp( (PltString) *(node->ident), "/acplt/ov") == 0) {		// skip /acplt/ov branch
			delete node->children;			// skip contents of library,
			node->children = NULL;
			delete node->children_list;		// which is created automatically
			node->children_list = NULL;
			delete node->var_block_list;
			node->var_block_list = NULL;
			delete node->link_block_list;
			node->link_block_list = NULL;
			delete node->part_block_list;
			node->part_block_list = NULL;
			return true;
		}
		if (! node->class_ident->isSingle() ) {
										// parent domains must be created first
			LogPath act_name = LogPath("/" + node->ident->getHead());
			LogPath act_tail = LogPath(node->ident->getTail());
			while (act_name != *(node->ident)) {
				instance *help = parse_tree->search(&act_name);
				if (!write_instance(help)) {
					return false;
				}
				act_name = LogPath(act_name + "/" + act_tail.getHead());
				act_tail = LogPath(act_tail.getTail());
			}
		}
		delete node->children;			// skip contents of library,
		node->children = NULL;
		delete node->children_list;		// which is created automatically
		node->children_list = NULL;
		delete node->var_block_list;
		node->var_block_list = NULL;
		delete node->link_block_list;
		node->link_block_list = NULL;
		delete node->part_block_list;
		node->part_block_list = NULL;
		if (liblist.addLast(node)) {	// add node to the list of library instances
			return true;
		} else {
			return false;
		}
	} else {
		return true;
	}
} // find_libraries

//-------------------------------------------------------------------------------

// set links between instances
bool write_links(instance *node)
{
	unsigned int					i, j;
	KsPlacementHint		pmh;
	KsLinkParams		link_params;
	KsLinkResult		link_result;

	if (! (node->cr_opts == CO_CHANGE && node->link_block_list)) {
        return true;				// nothing to be done
	}

	PltListIterator<link_value *> *it = (PltListIterator<link_value *> *) node->link_block_list->newIterator();
	while (*it) {
	    KscAnyCommObject help(server + *(node->ident) + "." + *((**it)->ident) );
		if (!help.getEngPropsUpdate()) {
			OUT_STREAM << "Error: Link " << *(node->ident) << "." << *((**it)->ident)
				 << " does not exist!" << NEW_LINE;
			delete it;
			return false;
		}
		//const KsLinkEngProps *helpprops = (const KsLinkEngProps *)help.getEngProps();
		//if (! (helpprops->access_mode & KS_AC_LINKABLE)) {	// link cannot be changed
		//	continue;
		//}
		if (! (**it)->link_paths ) {					// empty link
			++*it;
			continue;
		}
		i = (**it)->link_paths->size();
		KsArray<KsLinkItem> l_items(i);

		PltListIterator<LogPath> *link_it = (PltListIterator<LogPath> *) (**it)->link_paths->newIterator();
		// iterate over link targets
		for (j = 0; j < i; j++) {
			l_items[j].link_path = *(node->ident) + "." + *((**it)->ident);		// link name
			l_items[j].element_path = PltString(**link_it);		// link target
			l_items[j].place = pmh;
			l_items[j].opposite_place = pmh;
			++*link_it;
		}
		link_params.items = l_items;
		if (!server_base->requestByOpcode(KS_LINK,
										  0,
										  link_params,
										  link_result)) {
			OUT_STREAM << "Error setting links of object " << *(node->ident) << "!" << NEW_LINE;
			delete link_it;
			delete it;
			return false;
		}
		if (link_result.results.size() != i) {
			OUT_STREAM << "Error: Out of memory in write_links!" << NEW_LINE;
			delete link_it;
			delete it;
			return false;
		}
		link_it->toStart();
		for (j = 0; j < i; j++) {
			if (link_result.results[j] == KS_ERR_OK || link_result.results[j] == KS_ERR_ALREADYEXISTS) {
				// link may already exist as it is set by each end point object
				++*link_it;
				continue;
			}
			if (link_result.results[j] == KS_ERR_BADPATH) {		// TODO: ???
				if (verbose) {
					OUT_STREAM << "Warning: Link " << *((**it)->ident) << " of object " << *(node->ident)
						 << " could not be set," << NEW_LINE << "    target " << (PltString)**link_it
						 << " does not exist!" << NEW_LINE;
				}
			} else {
				OUT_STREAM << "Error " << link_result.results[j] << " setting link "
					 << *((**it)->ident) << " of object " << *(node->ident)
					 << "!" << NEW_LINE;
				delete link_it;
				delete it;
				return false;
			}
			++*link_it;
		} //for
		delete link_it;

		++*it;
	} // while
	delete it;
	if (verbose) {
		OUT_STREAM << "Wrote links of " << *(node->ident) << " successfully!" << NEW_LINE;
	}
	return true;
} // write_links

//-------------------------------------------------------------------------------

// write the parse tree into the server database
bool write_database()
{
	KsArray<KsString> params(1);
	KsArray<KsSetVarItem> paramsset(1);
	KsGetVarParams get_params(1);
	KsGetVarResult get_result(1);
	KsSetVarParams set_params(1);
	KsSetVarResult set_result(1);
	KsCurrPropsHandle hcprops;
	KsVarCurrProps cprops;
	bool ok = true;

	// identify library objects and add them to "liblist"
	if (!parse_tree->forEach(&find_libraries)) {
		OUT_STREAM << "Error: In function find_libraries!" << NEW_LINE;
		return false;
	}
	// enable activity lock: all activities of the server are suspended
	if (use_activitylock) {

		params[0] = "/vendor/server_configuration";

		get_params.identifiers = params;
	        if (!server_base->getVar(NULL, get_params, get_result)) {
			OUT_STREAM << "Error getting server_configuration!" << NEW_LINE;
			return false;
		}
		if (get_result.items[0].result != KS_ERR_OK) {
			OUT_STREAM << "Error getting server_configuration!" << NEW_LINE;
			return false;
		}
		hcprops = get_result.items[0].item;
		cprops = (KsVarCurrProps &) *hcprops;

		((KsBoolVecValue &) *cprops.value)[0] = TRUE;

		paramsset[0].path_and_name = params[0];
		paramsset[0].curr_props = hcprops;
		set_params.items = paramsset;
		if (!server_base->setVar(NULL, set_params, set_result)) {
			OUT_STREAM << "Error setting activity_lock!" << NEW_LINE;
			return false;
		}
		if (set_result.results.size() != 1) {
			OUT_STREAM << "Error: Out of memory in write_database!" << NEW_LINE;
			return false;
		}
		if (set_result.results[0].result != KS_ERR_OK) {
			OUT_STREAM << "Error setting activity_lock!" << NEW_LINE;
			return false;
		}
 	}
	// create library objects first, repeat "liblist.size()" times because of possible load
	// sequence dependancies
	int nr_libs = liblist.size();
	PltListIterator<instance *> *lib_it = (PltListIterator<instance *> *) liblist.newIterator();	//
	for (int i = nr_libs; nr_libs > 0 && i > 0; i--) {				//
		lib_it->toStart();											//
		while (*lib_it) {											//
			if ((**lib_it)->cr_opts != CO_NONE) {					//
				(**lib_it)->cr_opts = CO_CREATE;					//
				if (write_instance(**lib_it) == true) {				// NEW
					(**lib_it)->cr_opts = CO_NONE;					// ignore in next loops
					nr_libs--;										//
				}
			}
			++*lib_it;
		}
	}

	if (nr_libs != 0) {												// NEW
		OUT_STREAM << "Warning: Could not load all libraries needed!" << NEW_LINE;
		ok = false;
	}

	sleep(lib_wait_time);	// wait specified time after loading libraries TODO: obsolete?

	// Write instances into database
	if (!parse_tree->forEach(&write_instance)) {
		OUT_STREAM << "Warning: Could not write all objects in server!" << NEW_LINE;
		ok = false;
	}

	// Write variables of instances
	if (!parse_tree->forEach(&write_variables)) {
		OUT_STREAM << "Warning: Could not write all variables in server!" << NEW_LINE;
		ok = false;
	}

	// set link values
	if (!parse_tree->forEach(&write_links)) {
		OUT_STREAM << "Warning: Could not write all links in server!" << NEW_LINE;
		ok = false;
	}
	
	// finally disable activity lock
	if (use_activitylock) {
		((KsBoolVecValue &) *cprops.value)[0] = FALSE;

		if (!server_base->setVar(NULL, set_params, set_result)) {
			OUT_STREAM << "Error unsetting activity_lock!" << NEW_LINE;
			return false;
		}
		if (set_result.results.size() != 1) {
			OUT_STREAM << "Error: Out of memory in write_database!" << NEW_LINE;
			return false;
		}
		if (set_result.results[0].result != KS_ERR_OK) {
			OUT_STREAM << "Error unsetting activity_lock!" << NEW_LINE;
			return false;
		}
	}
	return ok;

} // write_database

//-------------------------------------------------------------------------------

// Get load options from command line parameter
int GetLoadOpts(char *arg)
{
	int		ret = 0;
	int		i, j, k;
	char	*constant[7];
	char	x;

	i = 2;
	j = 1;
	do {
		constant[j] = (char *)malloc(30);
		k = sscanf(&arg[i], "%[A-Za-z_]%1[-]", constant[j], &x);	// verify parameter format
		if (k == 0) {
			OUT_STREAM << "Error: Invalid load options parameter." << NEW_LINE;
//			free(constant);											// TODO: crashes why?
			exit(-1);
		}
		if (k == 1) {
			break;
		} else {
			i = i + strlen(constant[j]) + 1;
			j++;
		}
	} while (j < 7);	// at most 7 load options possible

	for (i=1; i<=j; i++) {		// for each parameter found
		if (strcmp(constant[i],"LO_ADD_CLASSES") == 0) {
			ret |= LO_ADD_CLASSES;
			continue;
		}
		if (strcmp(constant[i],"LO_OVERWRITE_EXACT") == 0) {
			ret |= LO_OVERWRITE_EXACT;
			continue;
		}
		if (strcmp(constant[i],"LO_OVERWRITE_INHERITED") == 0) {
			ret |= (LO_OVERWRITE_INHERITED | LO_OVERWRITE_EXACT);
			continue;
		}
		if (strcmp(constant[i],"LO_OVERWRITE_INCOMPATIBLE") == 0) {
			ret |= (LO_OVERWRITE_INCOMPATIBLE | LO_OVERWRITE_INHERITED | LO_OVERWRITE_EXACT);
			continue;
		}
		if (strcmp(constant[i],"LO_ERROR_ON_INCOMPATIBLE") == 0) {
			ret |= LO_ERROR_ON_INCOMPATIBLE;
			continue;
		}
		if (strcmp(constant[i],"LO_ADD_DOMAINS") == 0) {
			ret |= LO_ADD_DOMAINS;
			continue;
		}
		if (strcmp(constant[i],"LO_DEFAULT") == 0) {
			ret |= LO_DEFAULT;
			continue;
		}
		OUT_STREAM << "Error: Invalid load options parameter." << NEW_LINE;
		exit(-1);
	}
//	free (constant);									// TODO: crashes why?
	return ret;
} // GetLoadOpts

//-------------------------------------------------------------------------------

int main(int argc, char **argv)
{
	int						ret;						// parse result
	bool					ok = true;					// returns error code
	extern FILE*			yyin;						// the input file
	KsString				infile;						// name of the input file
	KscCommObjectHandle		hrootdomain;				// handle of server root
	char					*x;
	int						i;
	KsString 				rootname;
	//	extern int				yydebug;					// enable parser trace facilities
	//	yydebug = 1;

	// print info on startup
	OUT_STREAM << "** ACPLT/OV text file parser, version " << OV_VER_DBPARSE << " **" << NEW_LINE
		 << "(c) 2002/2006 Lehrstuhl fuer Prozessleittechnik, RWTH Aachen" << NEW_LINE
		 << NEW_LINE;

	if (argc < 2) {
		OUT_STREAM << "Error: Missing parameter \"//host/server\"." << NEW_LINE
			 << "Usage: " << argv[0] << " //host/server [-finput_file]" << NEW_LINE
			 << "[-lload_option[-load_option][-..]]" << NEW_LINE
			 << "[-wlib_wait_time]" << NEW_LINE
			 << "[-v]" << NEW_LINE
			 << "[-a]" << NEW_LINE
			 << NEW_LINE << "For more help type \"" << argv[0] << " -?\"" << NEW_LINE;
		return -1;
	}

	// print extended usage info
	if ((argc == 2 && (strcmp(argv[1], "-?") == 0))) {							// print help
		OUT_STREAM << "Usage: " << argv[0] << " //host/server [-finput_file]" << NEW_LINE
			 << "[-lload_option[-load_option][-..]]" << NEW_LINE
			 << "[-wlib_wait_time]" << NEW_LINE
			 << "[-v]" << NEW_LINE
			 << "[-a]" << NEW_LINE
			 << NEW_LINE
			 << "Options:" << NEW_LINE
			 << "//host/server : name of the OV host and server" << NEW_LINE
			 << "-finput_file : name of the input file, default: dbdump.txt" << NEW_LINE
			 << "-lload_option[-load_option][-..] :" << NEW_LINE
			 << "   valid options:" << NEW_LINE
			 << "      LO_ADD_CLASSES              add missing classes to server" << NEW_LINE
			 << "      LO_OVERWRITE_EXACT          overwrite objects of exactly matching type" << NEW_LINE
			 << "      LO_OVERWRITE_INHERITED      overwrite objects of inheritance compatible" << NEW_LINE
			 << "                                  type" << NEW_LINE
			 << "      LO_OVERWRITE_INCOMPATIBLE   overwrite objects of incompatible type" << NEW_LINE
			 << "      LO_ERROR_ON_INCOMPATIBLE    generate error when encountering incompatible" << NEW_LINE
			 << "                                  objects" << NEW_LINE
			 << "      LO_ADD_DOMAINS              add missing parent domains" << NEW_LINE
			 << "      LO_DEFAULT                  = LO_ADD_CLASSES | LO_OVERWRITE_EXACT |" << NEW_LINE
			 << "                                    LO_ADD_DOMAINS" << NEW_LINE
			 << "      Options may be combined, separated by \"-\"" << NEW_LINE
			 << "-wlib_wait_time : time to wait after loading the libraries in s, default: 0 s" << NEW_LINE
			 << "-v : print verbose status information" << NEW_LINE
			 << "-pPATH : write instances and links with relative path to PATH" << NEW_LINE
			 << "-a : use activitylock (OV version > 1.6.4)" << NEW_LINE
			 << NEW_LINE;
		return 0;
	}

	if (argc > 7) {
		OUT_STREAM << "Error: Too many parameters." << NEW_LINE;
		return -1;
	}

	x = (char *)malloc(strlen(argv[1])+1);					// dummy variable

															// Check host and server name
	if (sscanf(argv[1], "%1[/]%1[/]%[a-zA-Z0-9_.:-]%1[/]%[a-zA-Z0-9_]%s", x, x, x, x, x, x) != 5) {
		OUT_STREAM << "Error: Invalid host and server name." << NEW_LINE;
		free(x);
		return -1;
	} else {
		free(x);
	}

	// set default options
	infile = KsString("dbdump.txt");
	lopts = LO_DEFAULT;
	server = KsString(argv[1]);
	lib_wait_time = 0;
	verbose = false;
	relative = false;
	path = new(KscPath);
	*path = KscPath("/");

	for (i=2; i<argc; i++) {						// read command line parameters
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
			case 'f':
			case 'F': infile = KsString(&argv[i][2]);
					  break;
			case 'l':
			case 'L': lopts = GetLoadOpts(argv[i]);
					  break;
			case 'w':
			case 'W': if (sscanf(&argv[i][2], "%u", &lib_wait_time) != 1) {
						  OUT_STREAM << "Error: Invalid parameter for lib_wait_time." << NEW_LINE;
						  return -1;
					  }
					  break;
			case 'v':
			case 'V': verbose = true;
					  break;
			case 'p':
			case 'P': relative = true;
					  *path =  KscPath(KsString(&argv[i][2]));
					  break;
			case 'a':
			case 'A': use_activitylock = true;
					  break;
			default	: OUT_STREAM << "Invalid parameter: " << argv[i] << "." << NEW_LINE;
					  return -1;
			}
		} else {
			OUT_STREAM << "Invalid parameter: " << argv[i] << "." << NEW_LINE;
			return -1;
		}
	}

	parse_tree = new(parsetree);

	//append database prefix
	infile = KsString(KsString(getenv("ACPLT_HOME"), DATABASE_PATH), infile);

	yyin = fopen(infile, "r");						// open the input file read-only
	if(!yyin) {
		OUT_STREAM << "Error: File " << infile << " not found!" << NEW_LINE;
		return -1;
	}
	ret = yyparse();								// call parser

	if (ret != 0) {									// parse error
		ok = false;
	}

	if (verbose) {
		parselog.open("dbparse.log");						// dump parse tree ..
		parselog << parse_tree->getRoot();					// .. to "dbparse.log"
		parselog.close();
	}

	rootname = KsString(server + KsString("/"));
	if (ok) {
		KscDomain rootdom(rootname);
		if (!rootdom.getEngPropsUpdate()) {
			OUT_STREAM << "Error: Unable to open server " << server << "!" << NEW_LINE;
			ok = false;
		} else {	// server found
			ok = check_class_tree();	// check class compatibility
			ok = TRUE;
		}
	}
	if (ok) {
		ok = check_objects(); // && check_vendortree();		// check compatibility of objects
		ok = TRUE;
	}

	if (ok) {	// we need a handle of the root domain to get a pointer to the server object
		hrootdomain.bindTo(new KscDomain(rootname), KsOsNew);
		if (hrootdomain && hrootdomain->hasValidPath()) {
			server_base = hrootdomain->getServer();
		} else {
			OUT_STREAM << "Error: Cannot find server " << server << "!" << NEW_LINE;
			return false;
		}
		ok = write_database();							// write parse tree into server database
	}

	if (ok) {
		OUT_STREAM << "Success." << NEW_LINE;
	} else {
		OUT_STREAM << "Some Warnings have occurred." << NEW_LINE;
	}
	delete(parse_tree);									// clean up
	return ret;											// return parser result
} // main


