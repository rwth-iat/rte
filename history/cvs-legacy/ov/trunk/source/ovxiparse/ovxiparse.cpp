/*
 * $Id: ovxiparse.cpp,v 1.7 2007-04-24 14:11:29 martin Exp $
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

// ACPLT/OV OVI text file parser generating a XML textfile (ovxi.xsd)
// (used source code of dbparse1.cpp)
// Author : Ansgar Münnemann <ansgar@plt.rwth-aachen.de>
// date: Jan 24, 2005

//-------------------------------------------------------------------------------
// includes

#if PLT_SYSTEM_LINUX
#include <unistd.h>
#endif

#include <time.h>
#include "dbparse.h"
#include "libov/ov_version.h"

#include "db_y.h"

//-------------------------------------------------------------------------------
// global variables

ofstream			parselog;						// file logging parse results
parsetree			*parse_tree;			     	// parse tree
bool				verbose;						// print verbose status info?
FILE*				fout;						// the output file
bool				relative;						// write relative pathes to PATH
KscPath  			*path;							// target path in server

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
			act_inst->children->add(*(new_inst->ident), new_inst);
			if (!act_inst->children_list) {			// create new hash table for children
				act_inst->children_list = new(PltList<instance *>);
			}
			act_inst->children_list->addLast(new_inst);
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
		if (!act_inst->part_block_list) {		// no part list exists
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
		if (!act_inst->children_list) {			// no child list exists
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
	    // func execution failed
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
		return ret;
	} else {
		OUT_STREAM << "Error: Invalid semantic flags." << NEW_LINE;
		free(arg);
		exit(-1);
	}
} // GetSemFlags

//-------------------------------------------------------------------------------

// write the time of a variable
bool write_time(KsTime *ptime)
{
	fprintf(fout, "%s", (const char*) TimeToAscii(*ptime));
	return TRUE;
}

//-------------------------------------------------------------------------------

// write the state of a variable
bool write_state(KS_STATE state)
{
	switch (state) {
		case KS_ST_GOOD:
			fprintf(fout, "GOOD");
			break;
		case KS_ST_BAD:
			fprintf(fout, "BAD");
			break;
		case KS_ST_QUESTIONABLE:
			fprintf(fout, "QUESTIONABLE");
			break;
		default:
			fprintf(fout, "UNKNOWN");
	}	
	return TRUE;
}

//-------------------------------------------------------------------------------

// write the semantic falgs of a variable
bool write_flags(uint flags)
{
	uint				i,j;
	char				*arg;

	arg = (char *)malloc(33);
	j = 0;
	for (i=0;i<26;i++) {
		if (flags & (1 << i)) {
			arg[j]=('A'+i);
			j++;
		}
	}

	for (i=26;i<33;i++) {
		if (flags & (1 << i)) {
			arg[j]=('a'+i-26);
			j++;
		}
	}
	arg[j]=0;
	fprintf(fout, "%s", arg);
	return TRUE;
}

//-------------------------------------------------------------------------------

// write the value of a variable
bool write_value(value *val, enum value_types type)
{
	vector *act_vec;
	enum value_types vec_t;
	
			switch(type) {
				case DB_VT_VOID:					// skip void variables
					break;
				case DB_VT_BOOL:
					if (val->v.bool_val) fprintf(fout, "TRUE");
					else fprintf(fout, "FALSE");
					break;
				case DB_VT_INT:
					fprintf(fout, "%i", val->v.int_val);
					break;
				case DB_VT_DOUBLE:
					fprintf(fout, "%e", val->v.double_val);
					break;
				case DB_VT_STRING:
					fprintf(fout, "\"%s\"", (const char*) *val->v.pstring_val);
					break;
				case DB_VT_TIME:
					fprintf(fout, "%s", (const char*) TimeToAscii(*val->v.ptime_val));
					break;
				case DB_VT_TIME_SPAN:
					fprintf(fout, "%s", (const char*) TimeSpanToAscii(*val->v.ptime_span_val));
					break;
				case DB_VT_VECTOR:
					act_vec = val->v.pvector_val;
					vec_t = act_vec->content->type;
					fprintf(fout, "{");
					while (act_vec) {	
						write_value(act_vec->content, vec_t);
						act_vec = act_vec->next;
						if (act_vec) fprintf(fout, ",");
					}
					fprintf(fout, "}");
					break;
				default:
					OUT_STREAM << "Error: unknown variable type!" << type << NEW_LINE;
					return false;
			} // switch
			return TRUE;
}

//-------------------------------------------------------------------------------

// write the instance "node" in the target file

bool write_instance_base(instance *node, u_int cnt)
{
	u_int i,j;
	bool ok;
	
	if (verbose) OUT_STREAM << "write instance" << *node->ident << NEW_LINE;
	
	for (i=0;i<cnt;i++) fprintf(fout, "\t");
	fprintf(fout, "<Instance ID = \"%s\" ClassID = \"%s\"", (const char*) PltString(*node->ident), (const char*) PltString(*node->class_ident));
	if (node->creation_time) {
		fprintf(fout, " CreationTime=\"");
		write_time(node->creation_time);
		fprintf(fout, "\"");
	}
	if (node->sem_flags) {
		fprintf(fout, " Flags=\"");
		write_flags(node->sem_flags);
		fprintf(fout, "\"");
	}
	if (node->comment) {
		fprintf(fout, " Comment=\"%s\"", (const char*) *(node->comment));
	}
	fprintf(fout, ">\n");
	
	if (node->var_block_list) {
		PltListIterator<variable_value *> *it = (PltListIterator<variable_value *> *) node->var_block_list->newIterator();
		while (*it) {
			if (strncmp(node->ident->getHead(), "vendor", 6) != 0) {	//do not write variables of vendor tree
				if (verbose) OUT_STREAM << "write variable " << *((**it)->ident) << NEW_LINE;
				for (i=0;i<cnt;i++) fprintf(fout, "\t");
				fprintf(fout, "\t<VariableValue ID=\"%s\"", (const char *) *((**it)->ident) );
				if ((**it)->var_time) {
					fprintf(fout, " Time=\"");
					write_time((**it)->var_time);
					fprintf(fout, "\"");
				}
				if ((**it)->var_state != KS_ST_UNKNOWN) {
					fprintf(fout, " State=\"");
					write_state((**it)->var_state);
					fprintf(fout, "\"");
				}
				if ((**it)->var_flags) {
					fprintf(fout, " Flags=\"");
					write_flags((**it)->var_flags);
					fprintf(fout, "\"");
				}
				if ((**it)->var_unit) fprintf(fout," Unit=\"%s\"",(const char *) *((**it)->var_unit) );
				if ((**it)->var_comment) fprintf(fout," Comment=\"%s\"",(const char *) *((**it)->var_comment) );
				fprintf(fout,">");
				write_value((**it)->val, (**it)->val->type );
				fprintf(fout, "</VariableValue>\n");
			}
			++*it;
		}
		delete it;
	}

	if (node->part_block_list) {    	// now iterate over parts
		PltListIterator<instance *> *itpart = (PltListIterator<instance *> *) node->part_block_list->newIterator();
		while(*itpart) {								
			if (verbose) OUT_STREAM << "write part " << *((**itpart)->ident) << NEW_LINE;
			ok = write_instance_base( (**itpart) , cnt+1);
			if (!ok) {
			    delete itpart;
				return ok;
   			}
			++*itpart;
		}
		delete itpart;
	}

	if (node->link_block_list) {
		PltListIterator<link_value *> *itlink = (PltListIterator<link_value *> *) node->link_block_list->newIterator();
		while (*itlink) {
			if (verbose) OUT_STREAM << "write link " << *((**itlink)->ident) << NEW_LINE;
			for (i=0;i<cnt;i++) fprintf(fout, "\t");
			fprintf(fout, "\t<LinkValue ID=\"%s\">", (const char *) *(**itlink)->ident);
			
			// iterate over link targets
			if ((**itlink)->link_paths) {
				PltListIterator<LogPath> *link_it = (PltListIterator<LogPath> *) (**itlink)->link_paths->newIterator();
				for (j = 0; j < (**itlink)->link_paths->size(); j++) {
				    fprintf(fout, "%s", (const char *) PltString(**link_it));		// link target
					++*link_it;
					if (*link_it) fprintf(fout, ",");
				}
				delete link_it;
			}

			fprintf(fout, "</LinkValue>\n");
			
			++*itlink;
		}
		delete itlink;
	}
			
	for (i=0;i<cnt;i++) fprintf(fout, "\t");
	fprintf(fout, "</Instance>\n");
	return TRUE;

} // write_instance_base

//-------------------------------------------------------------------------------

// write the instance "node" in the target file
bool write_instance(instance *node)
{

	if (node->is_part) return true;   // skip parts here
	return write_instance_base(node, 1);
}

//-------------------------------------------------------------------------------

// write XML file
bool write_file()
{
	bool ok = true;

	fprintf(fout, "<ACPLT_OV_InstanceModel xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
				  "xsi:noNamespaceSchemaLocation=\"www.plt.rwth-aachen.de/ov/xml/ovi.xsd\">\n");
	// Write instances into database
	if (!parse_tree->forEach(&write_instance)) {
		OUT_STREAM << "Warning: Could not write all objects in OVXI/XML file!" << NEW_LINE;
		ok = false;
	}
	fprintf(fout, "</ACPLT_OV_InstanceModel>\n");
	return ok;
	
} // write_file

//-------------------------------------------------------------------------------

int main(int argc, char **argv)
{
	int						ret;						// parse result
	bool					ok = true;					// returns error code
	extern FILE*			yyin;						// the input file
	KsString				infile;						// name of the input file
	KsString				outfile;					// name of the input file
	int						i;
//	extern int				yydebug;					// enable parser trace facilities
//	yydebug = 1;

	// print info on startup
	OUT_STREAM << "** ACPLT/OVXI text file parser, version " << OV_VER_OVXIPARSE << " **" << NEW_LINE
		 << "(c) 2005 Lehrstuhl fuer Prozessleittechnik, RWTH Aachen" << NEW_LINE
		 << NEW_LINE;

	if ((argc == 2) && (strcmp(argv[1], "-h") == 0)) {
		OUT_STREAM << "OVI text file parser generates a OVXI/XML file as output." << NEW_LINE
			 << "Usage: " << argv[0] << " input_file output_file" << NEW_LINE
			 << "[-h] print help information" << NEW_LINE
			 << "[-pPATH] convert relative pathes to PATH/relative path" << NEW_LINE;
		return 0;
	}

	if (argc < 3){
		OUT_STREAM << "Error: Missing arguments." << NEW_LINE << NEW_LINE
			 << "Usage: " << argv[0] << " input_file output_file" << NEW_LINE
			 << "[-v] print verbose information" << NEW_LINE
			 << "[-h] print help information" << NEW_LINE
			 << "[-pPATH] convert relative pathes to PATH/relative path" << NEW_LINE;
		return -1;
	}

	infile = argv[1];
	outfile = argv[2];

	verbose = false;
	path = new(KscPath);
	*path = KscPath("/");

	for (i=3; i<argc; i++) {						// read command line parameters
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
			case 'v':
			case 'V': verbose = true;
					  break;
			case 'p':
			case 'P': relative = true;
					  *path =  KscPath(KsString(&argv[i][2]));
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
	
	yyin = fopen(infile, "r");						// open the input file read-only
	if(!yyin) {
		OUT_STREAM << "Error: File not found!" << NEW_LINE;
		return -1;
	}
	ret = yyparse();								// call parser

	if (ret != 0) {									// parse error
		ok = false;
	}

	if (verbose) {
		parselog.open("ovxiparse.log");						// dump parse tree ..
		parselog << parse_tree->getRoot();					// .. to "dbparse.log"
		parselog.close();
	}

	fout = fopen(outfile, "w");						// open the output file for writing
	if(!fout) {
		OUT_STREAM << "Error: Could not create file!" << NEW_LINE;
		return -1;
	}

	if (ok) ok = write_file();						  		// write parse tree into OVXI/XML target file

	if (ok) {
		OUT_STREAM << "Success." << NEW_LINE;
	} else {
		OUT_STREAM << "Some Warnings have occurred." << NEW_LINE;
	}
	delete(parse_tree);									// clean up
	return ret;											// return parser result
} // main


