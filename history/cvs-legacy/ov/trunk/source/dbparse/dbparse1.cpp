/*
 * Copyright (c) 1996, 1997, 1998, 1999, 2000, 2001, 2002
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
// dbparse1.cpp
// Version : 1.12
// last change: Aug 6, 2002

//-------------------------------------------------------------------------------
// includes

#if PLT_SYSTEM_LINUX
#include <unistd.h>
#endif

#include <time.h>
#include "dbparse.h"

#if PLT_SYSTEM_NT
#include "db_y.h"
#else
#include "db.y.h"
#endif

#define version "1.12"

//-------------------------------------------------------------------------------
// global variables

ofstream			parselog;						// file logging parse results
parsetree			*parse_tree = new(parsetree);	// parse tree
int					lopts;							// load options
KsString			server;							// target server
KscServerBase		*server_base;					// server base
u_int				lib_wait_time;					// time to wait after loading
													// the libraries
bool				verbose;						// print verbose status info?
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
			if (s[si] == '/' || s[si] == '.') {			// CHANGED
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
    bool was_slash = false;				// was last char == '/' or '.'?	// CHANGED
    _valid = true;						// be optimistic

	//
	// Empty paths are not allowed, so bail out as soon as possible.
	//
	if (!*p) {
		_valid = false;
		return 0;
	}
	if (*p == '/' || (*p == '.' && *(p+1) != '/' && *(p+1) != '.')) {	// CHANGED
		// The path is absolute.
		_go_up = 0;
		was_slash = true;
		++p;
		if (*p == '.') {			// CHANGED
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
				}	// CHANGED
			} else {
				break;
			}
		} while (was_slash);
	}

	size_t slashes = 0;
	for ( ; *p; ++p) {
		char ch = *p;
		if (ch == '/' || ch == '.') { 		// CHANGED
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
		// if garbage is found...
			was_slash = false;
		}
	}
	return slashes;
} // LogPath::checkAndCount

//-------------------------------------------------------------------------------

LogPath::operator PltString () const
{
	PltString prefix = "";
	if (isAbsolute()) {
		if (_slash[_first] >= 2 &&
			_slash[_first] < 100 &&		// !!!
			_str[_slash[_first] - 2] == '/') {	// CHANGED
			prefix = "/";
		}
		if (_slash[_first] >= 1 &&
			_slash[_first] < 100 &&		// !!!
			_str[_slash[_first] - 1] == '.') {		// CHANGED
			prefix += ".";
		} else {
			prefix = "/";
		}
	} else {
		for (size_t i = 0; i<_go_up-1; ++i) {
			prefix += "../";
		}
	}
	return PltString(prefix,
					 _str.substr(_slash[_first],
					 _slash[_last+1] - _slash[_first]));

} // LogPath::operator PltString

//-------------------------------------------------------------------------------

LogPath::LogPath(const PltString & str)
{
	_str = str;
	_slash = checkAndCount() + 2;
	_first = 0;
	findSlashes();
}

//-------------------------------------------------------------------------------

LogPath::LogPath()
{
	KsPath();
}

//-------------------------------------------------------------------------------

LogPath::LogPath(const LogPath &path, size_t first, size_t last)
{
	_str = path._str;
	_valid = path._valid;
	_slash = path._slash;
	_go_up = 0;		// CHANGED from 1

	PLT_PRECONDITION(path.isValid()
					 && first <= last && last < path.size());
	_first = first + path._first;
	_last = last + path._first;
}

//-------------------------------------------------------------------------------

PltString LogPath::getHead() const
{
	PLT_ASSERT(isValid());
	return (*this)[0];
}

//-------------------------------------------------------------------------------

LogPath LogPath::getTail() const
{
	PLT_ASSERT(size() > 1);
	return LogPath(*this, 1, size()-1);
}

//-------------------------------------------------------------------------------

bool LogPath::isPart() {
	if (isAbsolute() && _str[_slash[_first] - 1] == '.') {
		return true;
	} else {
		return false;
	}
}

//-------------------------------------------------------------------------------

bool LogPath::isRootPart() {
	if (isAbsolute() && _str[_slash[_first] - 1] == '.'
		&& _slash[_first] >= 2 && _str[_slash[_first] - 2] == '/') {
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

parsetree::~parsetree() {
	delete root;
}

//-------------------------------------------------------------------------------

instance *parsetree::getRoot() {
	return root;
}

//-------------------------------------------------------------------------------

// add an instance to the parse tree, add parent domains if necessary
bool parsetree::add(instance *inst) {

	instance	*act_inst, *new_inst, *child;
	int			i, j;

	j = inst->ident->size();					// nr. of path components
	if (*(inst->ident) == LogPath("/")) {		// root domain always exists
		*(root->creation_time) = *(inst->creation_time);
		return true;
	}
	act_inst = root;
	LogPath act_tail = inst->ident->getTail();
	KsString act_name = KsString("/" + inst->ident->getHead());
	// TODO: Pfad mit "/." am Anfang
	for (i = 0; i < j-1; i++) {					// for any path level
		new_inst = act_inst->search_child(&act_name);
		if (!new_inst) {
			new_inst = act_inst->search_part(&act_name);
		}
		if (!new_inst) {						// parent domain not (yet) in parse tree,
			new_inst = new(instance);			// so create it
			new_inst->ident = new(LogPath);
			*(new_inst->ident) = LogPath(act_name);
//	TODO: check if instance is part
			new_inst->is_part = false;
			new_inst->class_ident = new(LogPath);
			*(new_inst->class_ident) = LogPath("/acplt/ov/domain");
			new_inst->creation_time = new(KsTime);
			*(new_inst->creation_time) = (new_inst->creation_time)->now();
			new_inst->sem_flags = 0;
			new_inst->comment = NULL;
			new_inst->var_block = NULL;
			new_inst->link_block = NULL;
			new_inst->part_block = NULL;
			new_inst->cr_opts = CO_TREE;
			if (!act_inst->children) {
				act_inst->children = new(PltHashTable<PltString, instance*>);
			}
			act_inst->children->add(*(new_inst->ident), new_inst);
		}
		act_inst = new_inst;
		act_name = KsString(act_name + "/" + act_tail.getHead());	// one tree level down
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
				} else {
					return true;
				}
			} else {
				cout << "Error: Duplicate instance identifier in input file!" << endl;
				return false;
			}
		}
	}

	if (inst->is_part) {					// instance is a part
		if (!act_inst->part_block) {		// no part list exists
			act_inst->part_block = new(PltHashTable<PltString, instance*>);
		}
		if (act_inst->part_block->add(*(inst->ident), inst)) {
			return true;
		} else {
			return false;
		}
	} else {								// instance is a child
		if (!act_inst->children) {			// no child list exists
			act_inst->children = new(PltHashTable<PltString, instance*>);
		}
		if (act_inst->children->add(*(inst->ident), inst)) {
			return true;
		} else {
			return false;
		}
	}
} // parsetree::add

//-------------------------------------------------------------------------------

// remove instance with path "name" from parse tree
bool parsetree::remove(LogPath *name)
{
	instance	*inst, *parent;

	inst = search(name);
	if (!inst) {
		return false;
	}
	parent = search(&LogPath(*name, 0, name->size()-1));
	if (!parent->children->remove(*name, inst)) {
		cout << "Error: Cannot remove " << *(inst->ident) << " from parse tree!" << endl;
		exit;
	}
	delete inst;
	return true;
} // parsetree::remove

//-------------------------------------------------------------------------------

// remove instance pointed to by "inst" from parse tree
bool parsetree::remove(instance *inst)
{
	instance	*parent;

	parent = search(&LogPath(*(inst->ident), 0, inst->ident->size()-1));
	if (!parent->children->remove(*(inst->ident), inst)) {
		cout << "Error: Cannot remove " << *(inst->ident) << " from parse tree!" << endl;
		exit;
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

	if (*search_path == LogPath("/")) {			// no need to search root
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

	if (!func(start)) {								// func execution failed
		return false;
	}
	if (start->children) {
		PltHashIterator<PltString, instance *> *it = start->children->newIterator();
		while(*it) {								// iterate over children
			ret &= depthFirstSearch((**it).a_value, func);
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

vector::vector() {
	type = DB_VT_VOID;
	content = NULL;
	next = NULL;
}

//-------------------------------------------------------------------------------

vector::~vector() {
	_DELETE(content);
	_DELETE(next);
}

//-------------------------------------------------------------------------------
//
// methods of class structure
//
//-------------------------------------------------------------------------------

structure::structure() {
	content = NULL;
	next = NULL;
}

//-------------------------------------------------------------------------------

structure::~structure() {
	delete(content);
	_DELETE(next);
}

//-------------------------------------------------------------------------------
//
// methods of class value
//
//-------------------------------------------------------------------------------

value::value() {
	type = DB_VT_VOID;
	v.pvoid_val = NULL;
}

//-------------------------------------------------------------------------------

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

link_value::link_value() {
	ident = NULL;
	link_paths = NULL;
}

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
	var_block = NULL;
	link_block = NULL;
	part_block = NULL;
	children = NULL;
	cr_opts = CO_NONE;
}

//-------------------------------------------------------------------------------

// destructor
instance::~instance() {
	delete(ident);
	delete(class_ident);
	_DELETE(creation_time);
	_DELETE(comment);
	_DELETE(var_block);
	_DELETE(link_block);
	_DELETE(part_block);
	_DELETE (children);
}

//-------------------------------------------------------------------------------

// search variable "name" of instance
variable_value *instance::search_var(KsString *name) {
	variable_value *p = NULL;

	if (!var_block || !var_block->query(*name, p)) {
		return NULL;
	} else {
		return p;
	}
};

//-------------------------------------------------------------------------------

// search link "name" of instance
link_value *instance::search_link(KsString *name) {
	link_value *p = NULL;

	if (!link_block || !link_block->query(*name, p)) {
		return NULL;
	} else {
		return p;
	}
};

//-------------------------------------------------------------------------------

// search part "name" of instance
instance *instance::search_part(KsString *name) {
	instance *p = NULL;

	if (!part_block || !part_block->query(*name, p)) {
		return NULL;
	} else {
		return p;
	}
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

	if (path.isValid() ) { // && path.isAbsolute() ) {
		if (path.isRootPart() ) {
			log << "/.";
		} else {
			if (path.isPart() ) {
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
		cout << "Warning: Invalid path!" << endl;
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
		return log << "   Identifier: " << *(varval->ident) << endl
				   << "   Value: " << varval->val << endl
				   << "   Time: " << varval->var_time << endl
				   << "   State: " << state << endl
				   << "   Flags: " << varval->var_flags << endl
				   << "   Unit: " << varval->var_unit << endl
				   << "   Comment: " << varval->var_comment << endl;
	} else {
		return log;
	}
}
//-------------------------------------------------------------------------------

ostream &operator << (ostream &log, link_value *linkval) {
	PltListIterator<LogPath> *it;

	if (linkval && linkval->link_paths) {
		it = linkval->link_paths->newIterator();
		log << "   Identifier: " << *(linkval->ident) << endl
			<< "   Paths: ";
		while(*it) {
			log << LogPath(**it) << ",";
			++*it;
		}
		delete it;
		return log << endl;
	} else {
		return log;
	}
}

//-------------------------------------------------------------------------------

ostream &operator << (ostream &log, PltHashTable<PltString, variable_value*> *var_block) {
	PltHashIterator<PltString, variable_value *> *it;

	if (var_block) {
		it = var_block->newIterator();
		while(*it) {
			log << (**it).a_value;
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
		it = link_block->newIterator();
		while(*it) {
			log << (**it).a_value;
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
		it = children->newIterator();
		while(*it) {
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
		return log << "Identifier: " << *(inst->ident) << endl
				   << "Part: " << inst->is_part << endl
				   << "Class Identifier: " << *(inst->class_ident) << endl
				   << "Creation Time: " << inst->creation_time << endl
				   << "Semantic Flags: " << inst->sem_flags << endl
				   << "Comment: " << inst->comment << endl
				   << "Variable block:" << endl
				   << inst->var_block
				   << "Link block:" << endl
				   << inst->link_block
				   << "Part block:" << endl
				   << inst->part_block
				   << endl
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
	sprintf(timestring, "%04d/%02d/%02d %02d:%02d:%02d.%06u",
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
	sprintf(timestring, "%04d:%02d:%02d.%06u",
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
		cout << "Error: Bad time format.\n";
		exit (-1);
	}
	for(pc1=format, pc2=arg; *pc1; pc1++, pc2++) {
		if(!*pc2) {
			if((*pc1 == ' ') || (*pc1 == '.')) {
				break;
			}
			cout << "Error: Bad time format.\n";
			exit (-1);
		}
		switch(*pc1) {
		case '0':
			if(!((*pc2 >= '0') && (*pc2 <= '9'))) {
				cout << "Error: Bad time format.\n";
				exit (-1);
			}
			break;
		case '/':
		case ':':
		case ' ':
		case '.':
			if(*pc2 != *pc1) {
				cout << "Error: Bad time format.\n";
				exit (-1);
			}
			break;
		default:
			cout << "Error: Bad time format.\n";
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
	secs -= timezone;
	if(secs < 0) {
		cout << "Error: Time conversion failed.\n";
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
		cout << "Error: Bad time span format.\n";
		exit (-1);
	}
	for(pc1=format, pc2=arg; *pc1; pc1++, pc2++) {
		if(!*pc2) {
			if(*pc1 == '.') {
				break;
			}
			cout << "Error: Bad time span format.\n";
			exit (-1);
		}
		switch(*pc1) {
		case '0':
			if(!((*pc2 >= '0') && (*pc2 <= '9'))) {
				cout << "Error: Bad time span format.\n";
				exit (-1);
			}
			break;
		case ':':
		case '.':
			if(*pc2 != *pc1) {
				cout << "Error: Bad time span format.\n";
				exit (-1);
			}
			break;
		default:
			cout << "Error: Bad time span format.\n";
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
		cout << "Error: Time span conversion failed.\n";
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

		if isupper(arg[i]) {
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
		cout << "Error: Invalid semantic flags." << endl;
		free(arg);
		exit(-1);
	}
} // GetSemFlags

//-------------------------------------------------------------------------------

// Check variable types for compatibility, class information overwrites parse
// results (parser cannot distinguish int and unsigned int, for example)
bool compat_types(enum value_types &type1, KS_VAR_TYPE type2)
{
	type2 &= 0xFFFF;		// KS type mask
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
		default					:	return false;
	}
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
	while (act) {
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
// TODO: Welches Kompatibilitätskriterium ist sinnvoll?
	KsString	parent_path;

	KscVariable start(type2 + ".baseclass");
	if (!start.Update()) {
		KscVariable start(type2 + ".parentclass");
		if (!start.getUpdate()) {
			cout << "Error: Cannot check class compatibility for " << type2
				 << "!" << endl;
			return false;
		}
	}
	parent_path = (KsStringValue &)start.getCurrProps()->value;
	KscDomain parent_dom(parent_path);
#endif
	return false;							// require full compatibility by now
} // compat_classes

//-------------------------------------------------------------------------------

// Search and compare objects in target server
bool checkObjs(class instance *node)
{
	KscDomain	*parent, *help;

	KsString domname = server + KsString(*node->ident);
	KscDomain dom(domname);
	if (!dom.getEngPropsUpdate()) {					// object not in server
		node->cr_opts = CO_CREATE;					// => create
		KsString parentname(server + KsString(dom.getPathOnly()));
		parent = new KscDomain(parentname);
		while (strcmp(parent->getName(), "") != 0) {		// parent domains in server?
			if (parent->getEngPropsUpdate()) {
//				delete parent;								// TODO: this one causes segmentation faults
				break;
			}
			KsString parentname(server + KsString(parent->getPathOnly()));
			help = new KscDomain(parentname);
			delete parent;
			parent = help;
		} // while (strcmp ...)
		_DELETE(parent);
	} else {										// object in server, check compatibility
		if (strcmp(KsString(*(node->class_ident)), dom.getEngProps()->class_identifier) == 0) {
													// classes are equal
			if (lopts & LO_OVERWRITE_EXACT) {
				node->cr_opts = CO_CHANGE;
			}
		} else if (compat_classes(*(node->class_ident), dom.getEngProps()->class_identifier)) {
													// classes are compatible
			if (lopts & LO_OVERWRITE_INHERITED) {
				node->cr_opts = CO_OVERWRITE;
			}
		} else {									// classes are incompatible
			if (lopts & LO_ERROR_ON_INCOMPATIBLE) {
				cout << "Error: Types of " << *(node->ident) << " are incompatible!" << endl;
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
	return (parse_tree->forEach(&checkObjs));
} // check_objects

//-------------------------------------------------------------------------------

bool check_vendortree()
{
	instance										*vendor;
	PltHashIterator<PltString, variable_value *>	*act_var;
	bool											ok;
	KS_VAR_TYPE										vartype;

	vendor = parse_tree->search(&LogPath("/vendor"));
	if (!vendor) {
		return true;
	}
	act_var = vendor->var_block->newIterator();

	while (*act_var) {
		KscVariable var(server + "/vendor/" + (*act_var)->a_key);
		if (!var.getEngPropsUpdate()) {					// class variable not found in server
			cout << "Error: Cannot access variable /vendor/" << (const char *)(*act_var)->a_key
				 << " in server!" << endl;
		}
		vartype = var.getEngProps()->type;
		if ((*act_var)->a_value->val->type == DB_VT_VECTOR) {
			ok = compat_vectors((*act_var)->a_value->val->v.pvector_val, vartype);
		} else {
			ok = compat_types((*act_var)->a_value->val->type, vartype);
		}
		if (!ok) {					// variable types are compatible
			cout << "Error: Conflicting types for variable "
				 << (const char*)(*act_var)->a_key << "!" << endl;
			return false;
		}
		if (strcmp ((*act_var)->a_key, "/vendor/activity_lock") == 0) {
			(*act_var)->a_value->val->v.bool_val = true;
		}
		++*act_var;
	}
	return true;

} // check_vendortree

//-------------------------------------------------------------------------------

// check class compatibility of instance "node"
bool checkClassId(instance *node) {

	int												ret;
	PltHashIterator<PltString, variable_value *>	*act_var;
	PltHashIterator<PltString, link_value *>		*act_link;
	PltHashIterator<PltString, instance *>			*act_part;
	int												i;
	bool											found = true;
	bool											ok = false;
	instance										*class_obj, *act_cobj, *var_inst;
	variable_value									*var_type, *var_props, *dummy;
	link_value										*bc;
	bool											succ;

	if (strncmp(node->ident->getHead(), "vendor", 6) == 0) {		// skip /vendor branch
		return true;
	}
	KsString domname(server + KsString(*(node->class_ident)));
	KscDomain dom(domname);
	if (!dom.getEngPropsUpdate()) {								// class not in server
		if (!(lopts & LO_ADD_CLASSES)) {
			cout << "Error: Class " << *(node->class_ident) << " not in server!" << endl;
			return false;
		} else {												// search class in parse tree
			class_obj = parse_tree->search(node->class_ident);
			if (!class_obj) {
				cout << "Error: Class " << *(node->class_ident) << " not found!" << endl;
				return false;
			}
			if (node->var_block) {
				act_var = node->var_block->newIterator();		// check variables of instance for
																// compliance to class definition
				while (*act_var) {								// for all variables of the instance
					ret = 0;

					LogPath act_class_name(*(class_obj->ident));

					while (!ret && act_class_name.isValid()) {
											// look for variable in the class definition
											// of the object first, if not found search the base classes
						act_cobj = parse_tree->search(&act_class_name);
						if (! act_cobj) {
							cout << "Error: Class " << act_class_name << " not found!" << endl;
							return false;
						}
						if (act_cobj->children) {
							if (act_cobj->children->query(act_class_name + "/" + *((*act_var)->a_value->ident),
														   var_inst)) {
								if (!var_inst->var_block->query("vartype", var_type)) {
									cout << "Error: Class " << *(node->class_ident) << "," << endl
										 << "       cannot get type of variable " << *((*act_var)->a_value->ident)
										 << "!" << endl;
									return false;
								}
								if (!var_inst->var_block->query("varprops", var_props)) {
									cout << "Error: Class " << *(node->class_ident) << "," << endl
										 << "       cannot get props of variable " << *((*act_var)->a_value->ident)
										 << "!" << endl;
									return false;
								}
								if (var_props->val->v.int_val & 0x4) {		// variable is derived, must not be set
									node->var_block->remove((*act_var)->a_key, dummy);
									ret = 1;
									break;
								}
								if ((*act_var)->a_value->val->type == DB_VT_VECTOR) {
									ok = compat_vectors((*act_var)->a_value->val->v.pvector_val, var_type->val->v.int_val);
								} else {
									ok = compat_types((*act_var)->a_value->val->type, var_type->val->v.int_val);
								}
								if (ok) {					// variable types are compatible
									ret = 1;
									break;
								} else {
									cout << "Error: Class " << *(node->class_ident) << "," << endl
										 << "       variable " << (const char*)(*act_var)->a_key << " not compatible!" << endl;
									return false;
								}
							} // if (var_inst)
						} // if (act_cobj->children)
						if (act_cobj->link_block->query("baseclass", bc)) {
							if (bc->link_paths) {
								PltListIterator<LogPath> *it = bc->link_paths->newIterator();
								act_class_name = **it;					// path of the base class
								delete it;
							} else {
								act_class_name = LogPath("");
							}
							continue;
						} else {
							cout << "Error: Cannot get base class of class " << act_class_name
								 << "!" << endl;
							return false;
						}
					} // while (!ret && act_class_name.isValid())

					if (!ret) {
						cout << "Error: Class " << *(node->class_ident) << "," << endl
							 << "       variable " << (const char*)(*act_var)->a_key << " not compatible!" << endl;
						return false;
					}
					++*act_var;
				} // while (*act_var)
				delete act_var;
			} // if (node->var_block)

		} // if (!lopts ..)
		node->cr_opts = CO_CREATE;

// TODO: check links and parts when class not in server

	} else {										// class in server, compatible?

		if (node->var_block) {
			act_var = node->var_block->newIterator();
			while (*act_var) {						// for all variables of the instance
				LogPath act_class_name = LogPath(*(node->class_ident));
				KsString domname(server + act_class_name + "/" + (*act_var)->a_key);
				KscDomain var(domname);
				if (!var.getEngPropsUpdate()) {					// class variable not found in server
					found = false;
					while (act_class_name.isValid() ) {			// search the base classes for
																// the variable definition
						KscVariable parent(server + act_class_name + ".baseclass");
						if (! parent.getUpdate() ) {
							cout << "Error: Cannot get base class of " << act_class_name << "!" << endl;
							return false;
						}
						const KsVarCurrProps *parentprops = parent.getCurrProps();
						act_class_name = (KsStringValue &) *parentprops->value;
						KscDomain var(server + act_class_name + "/" + (*act_var)->a_key);
						if (var.getEngPropsUpdate()) {
							found = true;
							break;
						}
					} // while
				}
				if (!found) {
					cout << "Error: Class " << *(node->class_ident) << "," << endl
						 << "       variable " << var.getName() << " does not exist!" << endl;
					return false;
				}
				KscVariable vartype(server + act_class_name + "/" + (*act_var)->a_key + ".vartype");
				if (! (vartype.getEngPropsUpdate() && vartype.getUpdate()) ) {
					cout << "Error: Cannot get variable type of " << endl
						 << *(node->class_ident) << "/" << (const char*)(*act_var)->a_key << "!" << endl;
					return false;
				}
				KscVariable varprops(server + act_class_name + "/" + (*act_var)->a_key + ".varprops");
				if (! (varprops.getEngPropsUpdate() && varprops.getUpdate()) ) {
					cout << "Error: Cannot get variable props of " << endl
						 << *(node->class_ident) << "/" << (const char*)(*act_var)->a_key << "!" << endl;
					return false;
				}
				const KsVarCurrProps *currprops = varprops.getCurrProps();
				if (((int)((KsIntValue &) *currprops->value)) & 0x4) {		// variable is derived, must not be set
					node->var_block->remove((*act_var)->a_key, dummy);
					++*act_var;
					continue;
				}
				currprops = vartype.getCurrProps();
				if ((*act_var)->a_value->val->type == DB_VT_VECTOR) {
					// check variable types for compatibility
					ok = compat_vectors((*act_var)->a_value->val->v.pvector_val, (int)((KsIntValue &) *currprops->value));
				} else {
					ok = compat_types((*act_var)->a_value->val->type, (int)((KsIntValue &) *currprops->value));
				}
				if (!ok) {
															// variable type not compatible
					cout << "Error: Class " << *(node->class_ident) << "," << endl
						 << "       type of ." << (const char*)(*act_var)->a_key << " is not compatible!"
						 << endl;
					return false;
				}
				++*act_var;
			} // while (*act_var)
			delete act_var;
		} // if (node->var_block);

		if (node->link_block) {
			act_link = node->link_block->newIterator();

			while (*act_link) {								// for all links of the instance
				succ = false;
				LogPath act_class_name = *(node->class_ident);
				while (!succ && act_class_name.isValid() ) {
					KscVariable childlink(server + act_class_name + ".childassociation");
					if (! childlink.getUpdate() ) {
						cout << "Error: Class " << act_class_name << "," << endl
							 << "       cannot get childlink type!" << endl
							 << endl;
						return false;
					}
					const KsVarCurrProps *childcprops = childlink.getCurrProps();
					KscVariable parentlink(server + act_class_name + ".parentassociation");
					if (! parentlink.getUpdate() ) {
						cout << "Error: Class " << act_class_name << "," << endl
							 << "       cannot get parentlink type!" << endl
							 << endl;
						return false;
					}
					const KsVarCurrProps *parentcprops = parentlink.getCurrProps();
					for(i = 0; i < ((KsStringVecValue &) *childcprops->value).size(); i++) {
						KscVariable p_role(server + ((KsStringVecValue &) *childcprops->value)[i] + ".parentrolename");
						if (! (p_role.getEngPropsUpdate() && p_role.getUpdate()) ) {
							cout << "Error: Class " << *(node->class_ident) << "," << endl
								 << "       cannot get parentrolename!" << endl
								 << endl;
							return false;
						}
						const KsVarCurrProps *p_role_props = p_role.getCurrProps();
						if (strcmp((*act_link)->a_key, (KsStringValue &) *p_role_props->value) == 0) {
							succ = true;
							break;
						}
					}
					for(i = 0; !succ && i < ((KsStringVecValue &) *parentcprops->value).size(); i++) {
						KscVariable c_role(server + ((KsStringVecValue &) *parentcprops->value)[i] + ".childrolename");
						if (! (c_role.getEngPropsUpdate() && c_role.getUpdate()) ) {
							cout << "Error: Class " << *(node->class_ident) << "," << endl
								 << "       cannot get childrolename!" << endl
								 << endl;
							return false;
						}
						const KsVarCurrProps *c_role_props = c_role.getCurrProps();
						if (strcmp((*act_link)->a_key, (KsStringValue &) *c_role_props->value) == 0) {
							succ = true;
							break;
						}
					}
					if (!succ) {
						KscVariable parent(server + act_class_name + ".baseclass");
						if (! parent.getUpdate() ) {
							cout << "Error: Cannot get base class of " << act_class_name << "!" << endl;
							return false;
						}
						const KsVarCurrProps *parentprops = parent.getCurrProps();
						act_class_name = (KsStringValue &) *parentprops->value;
					}
				} // while (!succ ...)
				if (!succ) {
					cout << "Error: Class " << *(node->class_ident) << ": link type of " << endl
						 << (const char*)(*act_link)->a_key << " is not compatible!" << endl;
					return false;
				}
				++*act_link;
			} // while (*act_link)
			delete act_link;
		} // if (node->link_block)

		if (node->part_block) {
			act_part = node->part_block->newIterator();

			while (*act_part) {									// for all parts of the instance
				LogPath act_class_name = LogPath(*(node->class_ident));

				int j;
				for (j = (*act_part)->a_key.len() - 1; j >= 0; j--) {
					if ((*act_part)->a_key[j] == '.') {
						break;
					}
				}
				KsString help = KsString(&(*act_part)->a_key[j+1]);

				KsString domname(server + act_class_name + "/" + help);
				KscDomain part(domname);
				found = true;

				if (!part.getEngPropsUpdate()) {				// class part not found in server
					found = false;
					while (act_class_name.isValid() ) {			// search the base classes for
																// the part definition
						KscVariable parent(server + act_class_name + ".baseclass");
						if (! parent.getUpdate() ) {
							cout << "Error: Cannot get base class of " << act_class_name << "!" << endl;
							return false;
						}
						const KsVarCurrProps *parentprops = parent.getCurrProps();
						act_class_name = (KsStringValue &) *parentprops->value;
						KscDomain part(server + act_class_name + "/" + help);
						if (part.getEngPropsUpdate()) {
							found = true;
							break;
						}
					} // while
				}
				if (!found) {
					cout << "Error: Class " << *(node->class_ident) << "," << endl
						 << "       part " << part.getName() << " does not exist!" << endl;
					return false;
				}

				KscVariable clid(server + act_class_name + "/" + help + ".partclass");
				if (! clid.getUpdate() ) {
					cout << "Error: Cannot get part class of " << act_class_name << "/"
						 << help << "!" << endl;
					return false;
				}
				const KsVarCurrProps *clidprops = clid.getCurrProps();
				KsString clname = KsString((KsStringValue &) *clidprops->value);

				if (strcmp(clname, (PltString)*((*act_part)->a_value->class_ident)) != 0) {
					cout << "Error: Class " << *(node->class_ident) << ": part class " << endl
						 << help << " is not compatible!" << endl;
					return false;
				}
				++*act_part;
			} // while (*act_part)
			delete act_part;
		} // if (node->part_block)

	} // if (!dom.get ...

	return true;
} // checkClassId

//-------------------------------------------------------------------------------

// Are all necessary classes available in target server?
bool check_class_tree()
{
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
	vector					*act_vec;
	int						vec_sz;
	enum value_types		vec_t;
	int						i;

	if (node->cr_opts == CO_OVERWRITE) {
		KsArray<KsString> del_path(1);
		del_path[0] = KsString(*(node->ident));
		delete_params.paths = del_path;
		if (!server_base->requestByOpcode(KS_DELETEOBJECT,
										  0,
										  delete_params,
										  delete_result)) {
			cout << "Error deleting object " << *(node->ident) << "!" << endl;
			return false;
		}
		if (delete_result.result != KS_ERR_OK) {
			cout << "Error " << delete_result.result << " deleting object " << *(node->ident)
				 << "!" << endl;
			return false;
		}
		if (delete_result.results.size() != 1) {
			cout << "Error: Out of memory in write_instance!" << endl;
			return false;
		}
		node->cr_opts = CO_CREATE;
	} // if (node->cr_opts == CO_OVERWRITE)

	if (node->cr_opts == CO_CREATE) {
		create_params.items = KsArray<KsCreateObjItem>(1);
		if (create_params.items.size() != 1) {
			cout << "Error: Out of memory in write_instance!" << endl;
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
			cout << "Error creating object " << *(node->ident) << "!" << endl;
			return false;
		}

		if (create_result.result != KS_ERR_OK && create_result.result != KS_ERR_ALREADYEXISTS) {
			cout << "Error " << create_result.result << " creating object " << *(node->ident)
				 << "!" << endl;
			return false;
		}
		if (verbose && create_result.result == KS_ERR_ALREADYEXISTS) {
			cout << "Warning: Object " << *(node->ident) << " already exists!" << endl;
		}
		if (create_result.obj_results.size() != 1) {
			cout << "Error: Out of memory!" << endl;
			return false;
		}
		if (create_result.obj_results[0].result != KS_ERR_OK &&
				create_result.obj_results[0].result != KS_ERR_ALREADYEXISTS) {
			cout << "Error " << create_result.obj_results[0].result << " creating object " << *(node->ident)
				 << "!" << endl;
			return false;
		}
		if (verbose) {
			cout << "Created object " << *(node->ident) << " successfully!" << endl;
		}
		node->cr_opts = CO_CHANGE;
	} // if (node->cr_opts == CO_CREATE)

	// iterate over all variables of the object
	if (node->cr_opts == CO_CHANGE && node->var_block) {
		PltHashIterator<PltString, variable_value *> *it = node->var_block->newIterator();
		int nr_v = 0;
		while (*it) {
			PltString delimiter;
			if (strncmp(node->ident->getHead(), "vendor", 6) == 0) {
				delimiter = "/";
			} else {
				delimiter = ".";
			}
			KscVariable help(server + *(node->ident) + delimiter + (*it)->a_key);
			if (!help.getEngPropsUpdate()) {
				cout << "Error: Variable " << *(node->ident) << delimiter << (const char*)(*it)->a_key
					 << " does not exist!" << endl;
				delete it;
				return false;
			}
			const KsVarEngProps *helpprops = help.getEngProps();
			if (helpprops->access_mode & KS_AC_WRITE) {
				nr_v++;
			} else {
				(*it)->a_value->val->type = DB_VT_NONE;
			}
			++*it;
		} // while (*it)

		KsArray<KsSetVarItem> params(nr_v);
		it->toStart();
		i = 0;
		while (*it) {
			KsValue *val;
			switch((*it)->a_value->val->type) {
				case DB_VT_NONE:					// skip read-only variables
					++*it;
					continue;
				case DB_VT_BOOL:
					val = new KsBoolValue((*it)->a_value->val->v.bool_val);
					break;
				case DB_VT_INT:
					val = new KsIntValue((*it)->a_value->val->v.int_val);
					break;
				case DB_VT_DOUBLE:
					val = new KsDoubleValue((*it)->a_value->val->v.double_val);
					break;
				case DB_VT_INT_TO_DOUBLE:
					val = new KsDoubleValue((double)(*it)->a_value->val->v.int_val);
					break;
				case DB_VT_STRING:
					val = new KsStringValue(*((*it)->a_value->val->v.pstring_val));
					break;
				case DB_VT_TIME:
					val = new KsTimeValue(*((*it)->a_value->val->v.ptime_val));
					break;
				case DB_VT_TIME_SPAN:
					val = new KsTimeSpanValue(*((*it)->a_value->val->v.ptime_span_val));
					break;
				case DB_VT_UINT:
					val = new KsUIntValue((u_int)(*it)->a_value->val->v.int_val);
					break;
				case DB_VT_INT_TO_SINGLE:
					val = new KsSingleValue((float)(*it)->a_value->val->v.int_val);
					break;
				case DB_VT_DOUBLE_TO_SINGLE:
					val = new KsSingleValue((float)(*it)->a_value->val->v.double_val);
					break;
				case DB_VT_TIME_TO_TIME_SPAN:
					val = new KsTimeSpanValue((*it)->a_value->val->v.ptime_val->tv_sec,
											  (*it)->a_value->val->v.ptime_val->tv_usec);
					break;
				case DB_VT_TIME_SPAN_TO_TIME:
					val = new KsTimeValue((*it)->a_value->val->v.ptime_span_val->tv_sec,
										  (*it)->a_value->val->v.ptime_span_val->tv_usec);
					break;
				case DB_VT_VECTOR:
					act_vec = (*it)->a_value->val->v.pvector_val;
					vec_t = act_vec->type;
					vec_sz = 0;
					while(act_vec && act_vec->content) {
						vec_sz++;
						act_vec = act_vec->next;
					}
					act_vec = (*it)->a_value->val->v.pvector_val;
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
								(*stvec)[j] = *(act_vec->content->v.pstring_val);
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
								} else {
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
							cout << "Error: nested vectors are not allowed! (" << (const char*)(*it)->a_key
								 << ")" << endl;
							delete it;
							return false;
						}
						case DB_VT_STRUCTURE: {
							cout << "Error: variables of type structure are not yet supported! ("
								 << (const char*)(*it)->a_key << ")" << endl;
							delete it;
							return false;
						}
						default: {
							cout << "Error: " << (const char*)(*it)->a_key << " has unknown variable type!"
								 << endl;
							delete it;
							return false;
						}
					}
					break;
				case DB_VT_STRUCTURE:
					cout << "Error: variables of type structure are not yet supported! ("
						 << (const char*)(*it)->a_key << ")" << endl;
					delete it;
					return false;
				case DB_VT_VOID:
					val = new KsVoidValue();
					break;
				default:
					cout << "Error: " << (const char*)(*it)->a_key << " has unknown variable type!"
						 << endl;
					delete it;
					return false;
			} // switch

			KsValueHandle hvalue(val, KsOsNew);
			KsVarCurrProps *cprops = new KsVarCurrProps(hvalue, *((*it)->a_value->var_time),
														(*it)->a_value->var_state);
			KsCurrPropsHandle hcprops(cprops, KsOsNew);

			PltString delimiter;
			if (strncmp(node->ident->getHead(), "vendor", 6) == 0) {
				delimiter = "/";
			} else {
				delimiter = ".";
			}
			params[i].path_and_name = *(node->ident) + delimiter + (*it)->a_key;
			params[i].curr_props = hcprops;

			++*it;
			++i;
		} // while (*it)
		delete it;

		if (nr_v != 0) {
			KsSetVarParams set_params(nr_v);
			KsSetVarResult set_result(nr_v);
			set_params.items = params;
			if (!server_base->setVar(NULL, set_params, set_result)) {
				cout << "Error setting variables of object " << *(node->ident) << "!" << endl;
				return false;
			}
			if (set_result.results.size() != nr_v) {
				cout << "Error: Out of memory in write_instance!" << endl;
				return false;
			}
			for (i = 0; i < nr_v; i++) {
				if (set_result.results[i].result == KS_ERR_NOACCESS) {
					if (verbose) {
						cout << "Warning: No access to variable " << params[i].path_and_name
							 << " of object " << *(node->ident) << "!" << endl;
					}
				} else {
					if (set_result.results[i].result == KS_ERR_BADVALUE) {
						if (verbose) {
							cout << "Warning: Bad value for variable " << params[i].path_and_name
								 << " of object " << *(node->ident) << "! Value not set." << endl;
						}
					} else {
						if (set_result.results[i].result != KS_ERR_OK) {
							cout << "Error " << set_result.results[i].result << " setting variable "
								 << params[i].path_and_name << " of object " << *(node->ident) << "!" << endl;
							return false;
						}
					}
				}
			}
			if (verbose) {
				cout << "Created variables of " << *(node->ident) << " successfully!" << endl;
			}
		}

	} // if (node->cr_opts == CO_CHANGE ...
	return true;

} // write_instance

//-------------------------------------------------------------------------------

// Find and create all libraries before the other objects
bool find_libraries(instance *node)
{
	if (*(node->class_ident) == LogPath("/acplt/ov/library")) {
		if (strcmp((PltString)*(node->ident), "/acplt/ov") == 0) {		// skip /acplt/ov branch
			delete node->children;			// skip contents of library,
			node->children = NULL;
			delete node->var_block;			// which is created automatically
			node->var_block = NULL;
			delete node->link_block;
			node->link_block = NULL;
			delete node->part_block;
			node->part_block = NULL;
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
		delete node->var_block;			// which is created automatically
		node->var_block = NULL;
		delete node->link_block;
		node->link_block = NULL;
		delete node->part_block;
		node->part_block = NULL;
		if (liblist.addLast(node)) {
			return true;
		} else {
			return false;
		}
	} else {
		return true;
	}
} // find_libraries

//-------------------------------------------------------------------------------

bool write_links(instance *node)
{
	int					i, j;
	KsPlacementHint		pmh;
	KsLinkParams		link_params;
	KsLinkResult		link_result;

	if (! (node->cr_opts == CO_CHANGE && node->link_block)) {
		return true;
	}

	PltHashIterator<PltString, link_value *> *it = node->link_block->newIterator();

	for ( ; *it; ++*it) {
		KscAnyCommObject help(server + *(node->ident) + "." + (*it)->a_key);
		if (!help.getEngPropsUpdate()) {
			cout << "Error: Link " << *(node->ident) << "." << (const char*)(*it)->a_key
				 << " does not exist!" << endl;
			delete it;
			return false;
		}
		const KsLinkEngProps *helpprops = (const KsLinkEngProps *)help.getEngProps();
		if (! (helpprops->access_mode & KS_AC_LINKABLE)) {
			continue;
		}
		if (! (*it)->a_value->link_paths) {			// empty link
			continue;
		}
		i = (*it)->a_value->link_paths->size();
		KsArray<KsLinkItem> l_items(i);

		PltListIterator<LogPath> *link_it = (*it)->a_value->link_paths->newIterator();
		for (j = 0; j < i; j++) {
			l_items[j].link_path = *(node->ident) + "." + (*it)->a_key;
			l_items[j].element_path = PltString(**link_it);
			l_items[j].place = pmh;
			l_items[j].opposite_place = pmh;
			++*link_it;
		}
		link_params.items = l_items;
		if (!server_base->requestByOpcode(KS_LINK,
										  0,
										  link_params,
										  link_result)) {
			cout << "Error setting links of object " << *(node->ident) << "!" << endl;
			return false;
		}
		if (link_result.results.size() != i) {
			cout << "Error: Out of memory in write_links!" << endl;
			return false;
		}
		link_it->toStart();
		for (j = 0; j < i; j++) {
			if (link_result.results[j] == KS_ERR_OK || link_result.results[j] == KS_ERR_ALREADYEXISTS) {
				++*link_it;
				continue;
			}
			if (link_result.results[j] == KS_ERR_BADPATH) {
				if (verbose) {
					cout << "Warning: Link " << (const char*)(*it)->a_key << " of object " << *(node->ident)
						 << " could not be set," << endl << "    target " << (PltString)**link_it
						 << " does not exist!" << endl;
				}
			} else {
				cout << "Error " << link_result.results[j] << " setting link "
					 << (const char*)(*it)->a_key << " of object " << *(node->ident)
					 << "!" << endl;
				return false;
			}
			++*link_it;
		} //for
		delete link_it;

	} // for
	delete it;
	return true;
} // write_links

//-------------------------------------------------------------------------------

bool write_database()
{
	bool ok = true;

	if (!parse_tree->forEach(&find_libraries)) {
		cout << "Error: In function find_libraries!" << endl;
		return false;
	}
	KsArray<KsSetVarItem> params(1);
	KsBoolValue *val = new KsBoolValue(1);
	KsValueHandle hvalue(val, KsOsNew);
	KsVarCurrProps *cprops = new KsVarCurrProps(hvalue);
	KsCurrPropsHandle hcprops(cprops, KsOsNew);

	params[0].path_and_name = "/vendor/activity_lock";
	params[0].curr_props = hcprops;

	KsSetVarParams set_params(1);
	KsSetVarResult set_result(1);
	set_params.items = params;
	if (!server_base->setVar(NULL, set_params, set_result)) {
		cout << "Error setting activity_lock!" << endl;
		return false;
	}
	if (set_result.results.size() != 1) {
		cout << "Error: Out of memory in write_database!" << endl;
		return false;
	}
	if (set_result.results[0].result != KS_ERR_OK) {
		cout << "Error setting activity_lock!" << endl;
		return false;
	}

	PltListIterator<instance *> *lib_it = liblist.newIterator();
	for (int i = 0; ok && i < liblist.size(); i++) {
		lib_it->toStart();
		while (ok && *lib_it) {
			(**lib_it)->cr_opts = CO_CREATE;
			if (!write_instance(**lib_it)) {
				ok = false;
				cout << "Error: Could not load all libraries needed!" << endl;
			}
			++*lib_it;
		}
	}

	sleep(lib_wait_time);

	if (ok && !parse_tree->forEach(&write_instance)) {
		cout << "Error: Could not write objects in server!" << endl;
		ok = false;
	}

	// set actual link values
	if (ok && !parse_tree->forEach(&write_links)) {
		cout << "Error: Could not write links in server!" << endl;
		ok = false;
	}

	*val = KsBoolValue(0);
	if (!server_base->setVar(NULL, set_params, set_result)) {
		cout << "Error unsetting activity_lock!" << endl;
		return false;
	}
	if (set_result.results.size() != 1) {
		cout << "Error: Out of memory in write_database!" << endl;
		return false;
	}
	if (set_result.results[0].result != KS_ERR_OK) {
		cout << "Error unsetting activity_lock!" << endl;
		return false;
	}
	return ok;

} // write_database

//-------------------------------------------------------------------------------

// Get dump options from command line parameter
int GetLoadOpts(char *arg)
{
	int		ret;
	int		i, j, k;
	char	*constant[7];
	char	x;

	i = 2;
	j = 1;
	do {
		constant[j] = (char *)malloc(30);
		k = sscanf(&arg[i], "%[A-Za-z_]%1[-]", constant[j], &x);	// verify parameter format
		if (k == 0) {
			cout << "Error: Invalid load options parameter." << endl;
//			free(constant);											// TODO: crashes why?
			exit(-1);
		}
		if (k == 1) {
			break;
		} else {
			i = i + strlen(constant[j]) + 1;
			j++;
		}
	} while (j < 7);

	for (i=1; i<=j; i++) {
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
		cout << "Error: Invalid load options parameter." << endl;
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
	KsString				infile;
	KscCommObjectHandle		hrootdomain;
	char					*x;
	int						i;
//	extern int				yydebug;					// enable parser trace facilities
//	yydebug = 1;

	cout << "** ACPLT/OV text file parser, version " << version << " **" << endl
		 << "(c) 2002 Lehrstuhl fuer Prozessleittechnik, RWTH Aachen" << endl
		 << endl;

	if (argc < 2) {
		cout << "Error: Missing parameter \"//host/server\"." << endl
			 << "Usage: ./dbparse //host/server [-finput_file]" << endl
			 << "[-lload_option[-load_option][-..]]" << endl
			 << "[-wlib_wait_time]" << endl
			 << "[-v]" << endl
			 << endl << "For more help type ./dbparse -?" << endl;
		return -1;
	}

	if ((argc == 2 && (strcmp(argv[1], "-?") == 0))) {							// print help
		cout << "Usage: ./dbparse //host/server [-finput_file]" << endl
			 << "[-lload_option[-load_option][-..]]" << endl
			 << "[-wlib_wait_time]" << endl
			 << "[-v]" << endl
			 << endl
			 << "Options:" << endl
			 << "//host/server : name of the OV host and server" << endl
			 << "-finput_file : name of the input file, default: dbdump.txt" << endl
			 << "-lload_option[-load_option][-..] :" << endl
			 << "   valid options:" << endl
			 << "      LO_ADD_CLASSES              add missing classes to server" << endl
			 << "      LO_OVERWRITE_EXACT          overwrite objects of exactly matching type" << endl
			 << "      LO_OVERWRITE_INHERITED      overwrite objects of inheritance compatible" << endl
			 << "                                  type" << endl
			 << "      LO_OVERWRITE_INCOMPATIBLE   overwrite objects of incompatible type" << endl
			 << "      LO_ERROR_ON_INCOMPATIBLE    generate error when encountering incompatible" << endl
			 << "                                  objects" << endl
			 << "      LO_ADD_DOMAINS              add missing parent domains" << endl
			 << "      LO_DEFAULT                  = LO_ADD_CLASSES | LO_OVERWRITE_EXACT |" << endl
			 << "                                    LO_ADD_DOMAINS" << endl
			 << "      Options may be combined, separated by \"-\"" << endl
			 << "-wlib_wait_time : time to wait after loading the libraries in s, default: 0 s" << endl
			 << "-v : print verbose status information" << endl
			 << endl;
		return 0;
	}

	if (argc > 6) {
		cout << "Error: Too many parameters." << endl;
		return -1;
	}

	x = (char *)malloc(strlen(argv[1])+1);
															// Check host and server name
	if (sscanf(argv[1], "%1[/]%1[/]%[a-zA-Z0-9_.:-]%1[/]%[a-zA-Z0-9_]%s", x, x, x, x, x, x) != 5) {
		cout << "Error: Invalid host and server name." << endl;
		free(x);
		return -1;
	} else {
		free(x);
	}

	infile = KsString("dbdump.txt");
	lopts = LO_DEFAULT;
	server = KsString(argv[1]);
	lib_wait_time = 0;
	verbose = false;

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
						  cout << "Error: Invalid parameter for lib_wait_time." << endl;
						  return -1;
					  }
					  break;
			case 'v':
			case 'V': verbose = true;
					  break;
			default	: cout << "Invalid parameter: " << argv[i] << "." << endl;
					  return -1;
			}
		} else {
			cout << "Invalid parameter: " << argv[i] << "." << endl;
			return -1;
		}
	}

	yyin = fopen(infile, "r");					// open the input file read-only
	if(!yyin) {
		cout << "Error: File not found!" << endl;
		return -1;
	}
	ret = yyparse();									// call parser

	if (ret != 0) {										// parse error
		ok = false;
	}

	if (verbose) {
		parselog.open("dbparse.log");						// dump parse tree ..
		parselog << parse_tree->getRoot();					// .. to "dbparse.log"
		parselog.close();
	}

	KsString rootname(server + KsString("/"));
	if (ok) {
		lopts = LO_ADD_CLASSES | LO_OVERWRITE_EXACT | LO_ADD_DOMAINS;
		KscDomain rootdom(rootname);
		if (!rootdom.getEngPropsUpdate()) {
			cout << "Error: Unable to open server " << server << "!" << endl;
			ok = false;
		} else {
			ok = check_class_tree();
		}
	}
	if (ok) {
		ok = check_objects() && check_vendortree();
	}
	if (verbose) {
		parselog.open("dbparse.log");						// dump parse tree ..
		parselog << parse_tree->getRoot();					// .. to "dbparse.log"
		parselog.close();
	}
	if (ok) {
		hrootdomain.bindTo(new KscDomain(rootname), KsOsNew);
		if (hrootdomain && hrootdomain->hasValidPath()) {
			server_base = hrootdomain->getServer();
		} else {
			cout << "Error: Cannot find server " << server << "!" << endl;
			return false;
		}
		ok = write_database();
	}

	if (ok) {
		cout << "Success." << endl;
	} else {
		cout << "Error." << endl;
	}
	delete(parse_tree);									// clean up
	return ret;
} // main
