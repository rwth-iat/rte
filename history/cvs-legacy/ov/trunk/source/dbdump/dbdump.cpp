/*
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

// ACPLT/OV database dumper:
// This programme writes the contents of an ACPLT/OV server into a text file.
//
// Author : Christian Poensgen <chris@plt.rwth-aachen.de>
// dbdump.cpp
// Version : 1.33
// last change: Oct 29, 2002

/////////////////////////////////////////////////////////////////////////////

#include "dbdump.h"
#include "fnmatch.h"					// for string matching routines
#include <iostream.h>
#include <iomanip.h>
#include <fstream.h>
#include <ctype.h>
#include <time.h>

#if PLT_SYSTEM_NT
#include <stdio.h>
#define uint unsigned int
#endif

#define version "1.33"

// Global variables
search_p				sp;				// search parameters
dump_opt_p				dop;			// dump options
ofstream				db_file;		// output stream
PltList<PltString>		inst_list;		// list of instances which are to be
										// dumped later (which have the pattern "/x.y/z")

//---------------------------------------------------------------------------------

// Forward declaration
void DumpEngProps(const KsEngProps &eng_props, KscAnyCommObject &obj, int indent);

//---------------------------------------------------------------------------------

// Inserts formatting space
void Indent(int ind)
{
	while ( --ind >= 0 ) {
		db_file << ' ';
	}
} // Indent

//---------------------------------------------------------------------------------

// Check if "mask" matches "name", "mask" may contain *, ?, [char_set] and [^char_set]
int CompareKsStrings(KsString mask, KsString name, int no_case)
{
	char		*x, *y;
	error_t		*err_c;					// Error code for Acplt_FNMatch()
	int			ret;

	err_c = new error_t;
	x = (char *)malloc((uint)mask.len() + 1);
	y = (char *)malloc((uint)name.len() + 1);
	strcpy(x, mask);					// Acplt_FNMatch requires char* arguments
	strcpy(y, name);
	ret = Acplt_FNMatch(x, y, no_case, err_c);
	delete err_c;
	free(x);
	free(y);
	return ret;
} // CompareKsStrings

//---------------------------------------------------------------------------------

//Convert a KsTime object into an ASCII string
KsString TimeToAscii(const KsTime ptime)
{
	// local variables
	static char		timestring[] = "YYYY/MM/DD hh:mm:ss.uuuuuu";
	struct tm		*ptm;
	time_t			secs = ptime.tv_sec;

	//convert the time to a string
	ptm = gmtime(&secs);
	sprintf(timestring, "%04d/%02d/%02d %02d:%02d:%02d.%06u",
		ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour,
		ptm->tm_min, ptm->tm_sec, ptime.tv_usec);
	return (KsString)timestring;
} // TimeToAscii

//---------------------------------------------------------------------------------

// Convert a KsTimeSpan object into an ASCII string
// in the format hours:minutes:seconds.microseconds
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

//---------------------------------------------------------------------------------

// Dump variables of current domain communication object (branch)
int DumpVars(KscAnyCommObject &branch, int indent)
{
	KscChildIterator		*children;
	KsTime					time;
	KsTimeSpan				time_span;
	size_t					size, i;
	const KsVarCurrProps	*curr_props;
	PltString				delimiter;

	children = branch.newChildIterator(KS_OT_VARIABLE, KS_EPF_DEFAULT);

	if ( !children ) {
		Indent(indent);
		db_file << "Can't allocate child iterator." << endl;
		return -1;
	}

	Indent(indent);
	db_file << "VARIABLE_VALUES" << endl;

	//
	// Now iterate over all the child variables of the current level, we're in.
	// Take care that you increment the iterator and not the pointer to
	// it.
	//
	for ( ; *children; ++*children ) {
		KsEngPropsHandle current(**children);

		if (strncmp("/vendor", branch.getPathAndName(), 7) == 0) {
			delimiter = "/";	// variables in vendor branch are separated by slashes
		} else {
			delimiter = ".";
		}
		KscVariable child_var(PltString(branch.getFullPath()) +		// create child variable object
										delimiter +
										current->identifier);

		const KsVarEngProps *var_eng_props = (const KsVarEngProps *) current.getPtr();

		if(!var_eng_props) {
			Indent(indent);
			db_file << "<unknown engineered properties>";
		} else {													// check search restrictions
			if ((CompareKsStrings(sp->identifier, var_eng_props->identifier, 1)) &&
				(sp->from_creation_time <= var_eng_props->creation_time) &&
				(sp->to_creation_time >= var_eng_props->creation_time) &&
				(CompareKsStrings(sp->comment, var_eng_props->comment, 1)) &&
				(sp->access & var_eng_props->access_mode) &&
				((sp->semantic_flags & var_eng_props->semantic_flags) == sp->semantic_flags)
			   )
			{

				if (!child_var.getUpdate() ) {
					Indent(indent + INDENTATION);
					db_file << "Can't retrieve current properties for \""
							<< child_var.getName()
							<< "\"."
							<< endl;
				} else {
					curr_props = child_var.getCurrProps();
					if (!curr_props) {
						Indent(indent + INDENTATION);
						db_file << "Can't get hands on \""
								<< child_var.getName()
								<< "\" for current properties."
								<< endl;
					} else {							// dump variable value depending on type
						Indent(indent + INDENTATION);
                    	db_file << child_var.getName() << " = ";
						switch ( curr_props->value->xdrTypeCode() ) {
						case KS_VT_VOID:
							db_file << "<void>" << endl;
							break;
						case KS_VT_BOOL:
							if (strcmp("/vendor/activity_lock", child_var.getPathAndName() ) == 0) {
								// always enable activity lock flag in output file
								db_file << "TRUE" << endl;
							} else {
								db_file << ((bool) ((KsBoolValue &) *curr_props->value) ? "TRUE" : "FALSE")
										<< endl;
							}
							break;
						case KS_VT_INT:
							db_file << (long) ((KsIntValue &) *curr_props->value)
									<< endl;
							break;
						case KS_VT_UINT:
            	        	db_file << (u_long) ((KsUIntValue &) *curr_props->value)
									<< endl;
							break;
						case KS_VT_SINGLE:
							db_file << (float) ((KsSingleValue &) *curr_props->value)
									<< endl;
							break;
						case KS_VT_DOUBLE:
							db_file << (double) ((KsDoubleValue &) *curr_props->value)
									<< endl;
           	            	break;
						case KS_VT_STRING:
							db_file << "\""
									<< (const char *) ((KsStringValue &) *curr_props->value)
									<< "\"" << endl;
							break;
						case KS_VT_TIME:
							time = ((KsTimeValue &) *curr_props->value);
							db_file << TimeToAscii(time)
									<< endl;
							break;
						case KS_VT_TIME_SPAN:
							time_span = ((KsTimeSpanValue &) *curr_props->value);
							db_file << TimeSpanToAscii(time_span)
									<< endl;
							break;
                	    case KS_VT_BYTE_VEC:
							size = ((KsByteVecValue &) *curr_props->value).size();
                        	db_file << "{";
							for ( i = 0; i < size; ++i ) {
								db_file <<  hex << setfill('0') << setw(2)
										<< (unsigned int) ((KsByteVecValue &) *curr_props->value)[i]
										<< dec;
								if (i < size-1) {
									db_file << ", ";
								}
							}
							db_file << "}" << endl;
							break;
						case KS_VT_BOOL_VEC:
							size = ((KsByteVecValue &) *curr_props->value).size();
							db_file << "{";
							for ( i = 0; i < size; ++i ) {
								db_file <<  hex << setfill('0') << setw(2)
										<< ((bool) ((KsBoolVecValue &) *curr_props->value)[i] ? "TRUE" : "FALSE")
										<< dec;
								if (i < size-1) {
									db_file << ", ";
								}
							}
							db_file << "}" << endl;
                    	    break;
						case KS_VT_INT_VEC:
							size = ((KsIntVecValue &) *curr_props->value).size();
							db_file << "{";
            	            for ( i = 0; i < size; ++i ) {
								db_file << ((KsIntVecValue &) *curr_props->value)[i];
								if (i < size-1) {
									db_file << ", ";
								}
							}
	                    	db_file << "}" << endl;
    	                    break;
						case KS_VT_UINT_VEC:
							size = ((KsUIntVecValue &) *curr_props->value).size();
							db_file << "{";
							for ( i = 0; i < size; ++i ) {
								db_file << ((KsUIntVecValue &) *curr_props->value)[i];
								if (i < size-1) {
									db_file << ", ";
								}
	                        }
							db_file << "}" << endl;
							break;
						case KS_VT_SINGLE_VEC:
							size = ((KsSingleVecValue &) *curr_props->value).size();
							db_file << "{";
							for ( i = 0; i < size; ++i ) {
								db_file << ((KsSingleVecValue &) *curr_props->value)[i];
								if (i < size-1) {
									db_file << ", ";
								}
							}
							db_file << "}" << endl;
							break;
						case KS_VT_DOUBLE_VEC:
							size = ((KsDoubleVecValue &) *curr_props->value).size();
							db_file << "{";
							for ( i = 0; i < size; ++i ) {
								db_file << ((KsDoubleVecValue &) *curr_props->value)[i];
								if (i < size-1) {
									db_file << ", ";
								}
							}
							db_file << "}" << endl;
							break;
						case KS_VT_STRING_VEC:
							size = ((KsStringVecValue &) *curr_props->value).size();
							db_file << "{";
							for ( i = 0; i < size; ++i ) {
								db_file << "\""
										<< (const char *) ((KsStringVecValue &) *curr_props->value)[i]
										<< "\"";
								if (i < size-1) {
									db_file << ", ";
								}
							}
							db_file << "}" << endl;
							break;
						case KS_VT_TIME_VEC:
							size = ((KsTimeVecValue &) *curr_props->value).size();
							db_file << "{";
							for ( i = 0; i < size; ++i ) {
								time = ((KsTimeVecValue &) *curr_props->value)[i];
								db_file << TimeToAscii(time);
								if (i < size-1) {
									db_file << ", ";
								}
	                    	}
    	                    db_file << "}" << endl;
        	                break;
						case KS_VT_TIME_SPAN_VEC:
							size = ((KsTimeSpanVecValue &) *curr_props->value).size();
							db_file << "{";
							for ( i = 0; i < size; ++i ) {
								time_span = ((KsTimeSpanVecValue &) *curr_props->value)[i];
								db_file << TimeSpanToAscii(time_span);
								if (i < size-1) {
									db_file << ", ";
								}
							}
							db_file << "}" << endl;
							break;
						default:	// unknown variable type
							db_file << "<???>" << endl;
						} // switch

						// evaluate dump options
						if (*dop & KS_DO_VAR_TIME) {
							Indent(indent + 2 * INDENTATION);
							db_file << "TIME = ";
							db_file << TimeToAscii(curr_props->time)
									<< endl;
						}

						if (*dop & KS_DO_VAR_STATE) {
							Indent(indent + 2 * INDENTATION);
							db_file << "STATE = ";
							switch ( curr_props->state ) {
							case KS_ST_NOTSUPPORTED:
								db_file << "NOT_SUPPORTED" << endl;
								break;
							case KS_ST_UNKNOWN:
								db_file << "UNKNOWN" << endl;
								break;
							case KS_ST_BAD:
								db_file << "BAD" << endl;
								break;
							case KS_ST_QUESTIONABLE:
								db_file << "QUESTIONABLE" << endl;
								break;
							case KS_ST_GOOD:
								db_file << "GOOD" << endl;
								break;
							default:
								db_file << "<???>" << endl;
							}
						}

						if (*dop & KS_DO_VAR_SEM_FLAGS) {
							Indent(indent + 2 * INDENTATION);
							db_file << "FLAGS = \"";
							// 'A': lowest bit set, ..., 'Z', 'a', ..., 'f': highest bit set
							if (var_eng_props->semantic_flags) {
								for (i = 0; i <= 25; i++) {
									if (var_eng_props->semantic_flags & (1 << i)) {
										db_file << char('A' + i);
									}
								}
								for (i = 26; i <= 31; i++) {
									if (var_eng_props->semantic_flags & (1 << i)) {
										db_file << char('a' + i - 26);
									}
								}
							}
							db_file << "\"" << endl;
						}

						if (*dop & KS_DO_VAR_TECH_UNIT) {
							Indent(indent + 2 * INDENTATION);
							db_file << "UNIT = \""
									<< var_eng_props->tech_unit
									<< "\"" << endl;
						}

						if (*dop & KS_DO_VAR_COMMENT) {
							Indent(indent + 2 * INDENTATION);
    	    	            db_file << "COMMENT = \""
									<< var_eng_props->comment
									<< "\";" << endl;
						}
					}
				}
			}
		}
	} // for

	Indent(indent);
	db_file << "END_VARIABLE_VALUES;" << endl;

	//
	// finally, clean up everything we've allocated in this level
	//
	delete children;
	return 0;

} // DumpVars


//---------------------------------------------------------------------------------

// Dump links of current domain communication object
int DumpLinks(KscAnyCommObject &branch, int indent)
{
	KscChildIterator		*children;
	PltString				delimiter;
	const KsVarCurrProps	*curr_props;
	int						i, size;

	children = branch.newChildIterator(KS_OT_LINK, KS_EPF_DEFAULT, "*");
	if ( !children ) {
		Indent(indent);
		db_file << "Can't allocate child iterator." << endl;
		return -1;
	}

	Indent(indent);
	db_file << "LINK_VALUES" << endl;

	//
	// Now iterate over all the child links of the current level, we're in.
	// Take care that you increment the iterator, and not the pointer to
	// it.
	//
	for ( ; *children; ++*children ) {
		KsEngPropsHandle current(**children);

		delimiter = ".";
		KscVariable child_link(PltString(branch.getFullPath()) +		// create child link object
										 delimiter +
										 current->identifier);

		const KsLinkEngProps *link_eng_props = (const KsLinkEngProps *) current.getPtr();

		if(!link_eng_props) {
			Indent(indent + INDENTATION);
			db_file << "<unknown engineered properties>" << endl;;
		} else { 													// check search restrictions
			if ((CompareKsStrings(sp->identifier, link_eng_props->identifier, 1)) &&
				(sp->from_creation_time <= link_eng_props->creation_time) &&
				(sp->to_creation_time >= link_eng_props->creation_time) &&
				(CompareKsStrings(sp->comment, link_eng_props->comment, 1)) &&
				(sp->access & link_eng_props->access_mode) &&
				((sp->semantic_flags & link_eng_props->semantic_flags) == sp->semantic_flags)
	   		   )
			{
				if (!child_link.getUpdate() ) {
					Indent(indent + INDENTATION);
					db_file << "Can't retrieve current properties for \""
							<< child_link.getName()
							<< "\"."
							<< endl;
				} else {
					curr_props = child_link.getCurrProps();
					if (!curr_props) {
						Indent(indent + INDENTATION);
						db_file << "Can't get hands on \""
								<< child_link.getName()
								<< "\" for current properties."
								<< endl;
					} else {									// dump link value depending on type
						Indent(indent + INDENTATION);
						db_file << link_eng_props->identifier << " = {";
						switch (curr_props->value->xdrTypeCode()) {					// link target(s)
						case KS_VT_STRING :
							db_file << (const char *) ((KsStringValue &) *curr_props->value);
							break;
						case KS_VT_STRING_VEC :
							size = ((KsStringVecValue &) *curr_props->value).size();
							for (i = 0; i < size; ++i) {
								db_file << (const char *) ((KsStringVecValue &) *curr_props->value)[i];
								if (i < size-1) {
									db_file << "," << endl;
									Indent(indent + 2 * INDENTATION);
								}
							}
							break;
						default : cout << "Error: Invalid value type "
									   << curr_props->value->xdrTypeCode()
									   << " of link " << child_link.getName() << "." << endl;
						}
						db_file << "}" << endl;
					}
				}
			}
		}
	} // for

	Indent(indent);
	db_file << "END_LINK_VALUES;" << endl;

	//
	// finally, clean up everything we've allocated in this level
	//
	delete children;
	return 0;
} // DumpLinks

//---------------------------------------------------------------------------------

// Dump part domains of current domain communication objects
int DumpParts(KscAnyCommObject &branch, int indent)
{
	KscChildIterator	*children;
	PltString			delimiter;

	children = branch.newChildIterator(KS_OT_DOMAIN, KS_EPF_PARTS, "*");

	if ( !children ) {
		Indent(indent);
		db_file << "Can't allocate child iterator." << endl;
		return -1;
	}

	Indent(indent);
	db_file << "PART_INSTANCES" << endl;

	//
	// Now iterate over all the part domains of the current level, we're in.
	// Take care that you increment the iterator, and not the pointer to
	// it.
	//
	for ( ; *children; ++*children ) {
		KsEngPropsHandle current(**children);

														// check search restrictions
		if ((CompareKsStrings(sp->identifier, current->identifier, 1)) &&
			(sp->from_creation_time <= current->creation_time) &&
			(sp->to_creation_time >= current->creation_time) &&
			(CompareKsStrings(sp->comment, current->comment, 1)) &&
			(sp->access & current->access_mode) &&
			((sp->semantic_flags & current->semantic_flags) == sp->semantic_flags)
		   )
		{

			delimiter = ".";
			KscAnyCommObject child_domain(PltString(branch.getFullPath()) +
													delimiter +
													current->identifier);
			DumpEngProps(*current, child_domain, indent + INDENTATION);
		}
	}
	Indent(indent);
	db_file << "END_PART_INSTANCES;" << endl;

	//
	// finally, clean up everything we've allocated in this level
	//
	delete children;
	return 0;

} // DumpParts

//---------------------------------------------------------------------------------

// Dump child domains of current domain communication object
int DumpChildren(KscAnyCommObject &branch, int indent)
{
	KscChildIterator	*children;
	PltString			delimiter;

	children = branch.newChildIterator(KS_OT_DOMAIN, KS_EPF_CHILDREN, "*");

	if ( !children ) {
		Indent(indent);
		db_file << "Can't allocate child iterator." << endl;
		return -1;
	}
	if (branch.getName() == "") {			// root level: "/" already included in path
		delimiter = "";
	} else {
		delimiter = "/";
	}

	//
	// Now iterate over all the child domains of the current level, we're in.
	// Take care that you increment the iterator, and not the pointer to
	// it.
	//
	for ( ; *children; ++*children ) {
		KsEngPropsHandle current(**children);
		KscAnyCommObject child_domain(PltString(branch.getFullPath()) +
										delimiter +
										current->identifier);
		DumpEngProps(*current, child_domain, indent);
	}

	//
	// finally, clean up everything we've allocated in this level...
	//
	delete children;
	return 0;
} // DumpChildren


//---------------------------------------------------------------------------------

// Dump engineered properties of current domain communication object and iterate over
// children and parts
void DumpEngProps(const KsEngProps &eng_props, KscAnyCommObject &obj, int indent)
{
	int	i;

													// check search restrictions
	if ((CompareKsStrings(sp->identifier, eng_props.identifier, 1)) &&
		(sp->from_creation_time <= eng_props.creation_time) &&
		(sp->to_creation_time >= eng_props.creation_time) &&
		(CompareKsStrings(sp->comment, eng_props.comment, 1)) &&
		(sp->access & eng_props.access_mode) &&
		((sp->semantic_flags & eng_props.semantic_flags) == sp->semantic_flags)
	   )
	{

		if (eng_props.access_mode & KS_AC_PART) {
			Indent(indent);
			db_file << "PART_INSTANCE " << obj.getPathAndName();
		} else {
			if (indent > 0) {	// if indent is not 0 the parent of obj has to be
								// a part, e.g. "/x.parent/obj", thus instance obj
								// must be dumped later on level 0
				inst_list.addLast(obj.getFullPath() );
				return;
			}
			Indent(indent);
			db_file << "INSTANCE " << obj.getPathAndName();
		}

		// evaluate dump options
		if (*dop & KS_DO_CLASS_IDENT) {
			db_file << " : CLASS "
				<< ((KsDomainEngProps &)eng_props).class_identifier;
		}
		db_file << endl;

		if (*dop & KS_DO_CREATION_TIME) {
			Indent(indent + INDENTATION);
			db_file << "CREATION_TIME = "
					<< TimeToAscii(eng_props.creation_time)
					<< ";" << endl;
		}

		if (*dop & KS_DO_SEM_FLAGS) {			// Semantic Flags: lowest bit set = "A",
			Indent(indent + INDENTATION);		// previous bit set = "B", ...
			db_file << "FLAGS = \"";
			if (eng_props.semantic_flags) {
				for (i = 0; i <= 25; i++) {
					if (eng_props.semantic_flags & (1 << i)) {
						db_file << char('A' + i);
					}
				}
				for (i = 26; i <= 31; i++) {	// ... highest bit set = "f"
					if (eng_props.semantic_flags & (1 << i)) {
						db_file << char('a' + i - 26);
					}
				}
			}
			db_file << "\";" << endl;
		}

		if (*dop & KS_DO_COMMENT) {
			Indent(indent + INDENTATION);
			db_file << "COMMENT = \""
					<< eng_props.comment
					<< "\";" << endl;
		}

		DumpVars(obj, indent + INDENTATION);			// Dump variables of current domain

		DumpLinks(obj, indent + INDENTATION);			// Dump links of current domain

		DumpParts(obj, indent + INDENTATION);			// Dump parts of current domain

		Indent(indent);
		if (eng_props.access_mode & KS_AC_PART) {
			db_file << "END_PART_INSTANCE;" << endl;
		} else {
			db_file << "END_INSTANCE;" << endl << endl;
		}
	}

	DumpChildren(obj, indent);							// Dump child objects recursively
} // DumpEngProps

//---------------------------------------------------------------------------------

// Retrieve engineered properties of root (start) communication object
int DumpEngProps(KscAnyCommObject &obj, int indent)
{
	const KsEngProps	*eng_props;

	if ( !obj.getEngPropsUpdate() ) {
		Indent(indent);
		db_file << "Can't retrieve engineered properties for \""
				<< obj.getName()
				<< "\"."
				<< endl;
		return -1;
	}
	eng_props = obj.getEngProps();				// get root domain engineered props
	if ( !eng_props ) {
		Indent(indent);
		db_file << "Can't get hands on \""
				<< obj.getName()
				<< "\" for engineered properties."
				<< endl;
		return -1;
	}
	DumpEngProps(*eng_props, obj, indent);	// dump engineered properties of root domain
	db_file << endl;
	return 0;
} // DumpEngProps

//---------------------------------------------------------------------------------

// Get creation time from command line parameter
KsTime GetTime(char *arg)
{
	// local variables
	static char		format[] = "0000/00/00 00:00:00.000000";
	struct tm		tm;
	time_t			secs;
	u_int			usecs;
	char			*pc1, *pc2;
	KsTime			ptime;
	int				i;

	i = 0;
	while (arg[i] != '\0') {				// change "," used in parameter format to SPACE
		if (arg[i] == ',') {
			arg[i] = ' ';
		}
		i++;
	}
	// check the format of the given string, which must be either
	// YYYY/MM/DD, YYYY/MM/DD hh:mm:ss or YYYY/MM/DD hh:mm:ss.uuuuuu
	if(!arg) {
		cout << "Error: Bad time format." << endl;
		exit (-1);
	}
	for(pc1=format, pc2=arg; *pc1; pc1++, pc2++) {
		if(!*pc2) {
			if((*pc1 == ' ') || (*pc1 == '.')) {
				break;
			}
			cout << "Error: Bad time format." << endl;
			exit (-1);
		}
		switch(*pc1) {
		case '0':
			if(!((*pc2 >= '0') && (*pc2 <= '9'))) {
				cout << "Error: Bad time format." << endl;
				exit (-1);
			}
			break;
		case '/':
		case ':':
		case ' ':
		case '.':
			if(*pc2 != *pc1) {
				cout << "Error: Bad time format." << endl;
				exit (-1);
			}
			break;
		default:
			cout << "Error: Bad time format." << endl;
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
	if(secs == -1) {
		cout << "Error: Time conversion failed." << endl;
		exit (-1);
	}
	// time is OK, return value
	ptime.tv_sec = secs;
	ptime.tv_usec = usecs;
	return ptime;
} // GetTime

//---------------------------------------------------------------------------------

// Get access rights from command line parameter
KS_ACCESS GetAccRights(char *arg)
{
	KS_OBJ_TYPE	ret;
	int			i, j, k;
	char		*constant[11];	// at most 11 access rights parameters possible
	char		x;

	ret = 0;
	i = 2;
	j = 1;
	do {
		constant[j] = (char *)malloc(30);
		k = sscanf(&arg[i], "%[A-Za-z_]%1[-]", constant[j], &x);	// verify parameter format
		if (k == 0) {
			cout << "Error: Invalid access rights parameter." << endl;
//			free(constant);											// ??? error! why?
			exit(-1);
		}
		if (k == 1) {
			break;
		} else {
			i = i + strlen(constant[j]) + 1;
			j++;
		}
	} while (j < 11);

	for (i=1; i <= j; i++) {
		if (strcmp(constant[j],"KS_AC_NONE") == 0) {
			ret |= KS_AC_NONE;
			continue;
		}
		if (strcmp(constant[j],"KS_AC_READ") == 0) {
			ret |= KS_AC_READ;
			continue;
		}
		if (strcmp(constant[j],"KS_AC_WRITE") == 0) {
			ret |= KS_AC_WRITE;
			continue;
		}
		if (strcmp(constant[j],"KS_AC_READWRITE") == 0) {
			ret |= KS_AC_READWRITE;
			continue;
		}
		if (strcmp(constant[j],"KS_AC_DELETABLE") == 0) {
			ret |= KS_AC_DELETEABLE;
			continue;
		}
		if (strcmp(constant[j],"KS_AC_RENAMABLE") == 0) {
			ret |= KS_AC_RENAMEABLE;
			continue;
		}
		if (strcmp(constant[j],"KS_AC_LINKABLE") == 0) {
			ret |= KS_AC_LINKABLE;
			continue;
		}
		if (strcmp(constant[j],"KS_AC_UNLINKABLE") == 0) {
			ret |= KS_AC_UNLINKABLE;
			continue;
		}
		if (strcmp(constant[j],"KS_AC_INSTANTIABLE") == 0) {
			ret |= KS_AC_INSTANTIABLE;
			continue;
		}
		if (strcmp(constant[j],"KS_AC_PART") == 0) {
			ret |= KS_AC_PART;
			continue;
		}
		cout << "Error: Invalid access rights parameter." << endl;
		exit(-1);
	}
//	free(constant);											// ??? error! why?
	return ret;
} // GetAccRights

//---------------------------------------------------------------------------------

// Get semantic flags from command line parameter
KS_SEMANTIC_FLAGS GetSemFlags(char *arg)
{
	KS_SEMANTIC_FLAGS	ret;
	uint				i;

	if (arg[2] == '\0') {
		return 0;
	}
	i = 2;
	ret = 0;
	while ((arg[i] != '\0') &&								// verify parameter format
			(isupper(arg[i])  ||
			(arg[i] >= 'a' && arg[i] <= 'f')) &&
			(arg[i+1] == '\0' || arg[i+1] > arg[i])) {

		if (isupper(arg[i])) {
			ret |= (1 << (arg[i] - 'A'));					// build bit field
		} else {
			ret |= (1 << (26 + arg[i] - 'a'));
		}
		i++;
	}
	if (i == strlen(arg)) {
		return ret;
	} else {
		cout << "Error: Invalid semantic flags." << endl;
		exit(-1);
	}
} // GetSemFlags

//---------------------------------------------------------------------------------

// Get dump options from command line parameter
dump_opt_t GetDumpOpts(char *arg)
{
	dump_opt_t	ret;
	int			i, j, k;
	char		*constant[11];	// at most 11 dump options parameters possible
	char		x;

	i = 2;
	j = 1;
	do {
		constant[j] = (char *)malloc(30);
		k = sscanf(&arg[i], "%[A-Za-z_]%1[-]", constant[j], &x);	// verify parameter format
		if (k == 0) {
			cout << "Error: Invalid dump options parameter." << endl;
//			free(constant);										// ??? error! why?
			exit(-1);
		}
		if (k == 1) {
			break;
		} else {
			i = i + strlen(constant[j]) + 1;
			j++;
		}
	} while (j < 11);

	for (i=1; i<=j; i++) {
		if (strcmp(constant[i],"KS_DO_CLASS_IDENT") == 0) {
			ret |= KS_DO_CLASS_IDENT;
			continue;
		}
		if (strcmp(constant[i],"KS_DO_CREATION_TIME") == 0) {
			ret |= KS_DO_CREATION_TIME;
			continue;
		}
		if (strcmp(constant[i],"KS_DO_SEM_FLAGS") == 0) {
			ret |= KS_DO_SEM_FLAGS;
			continue;
		}
		if (strcmp(constant[i],"KS_DO_COMMENT") == 0) {
			ret |= KS_DO_COMMENT;
			continue;
		}
		if (strcmp(constant[i],"KS_DO_VAR_TIME") == 0) {
			ret |= KS_DO_VAR_TIME;
			continue;
		}
		if (strcmp(constant[i],"KS_DO_VAR_STATE") == 0) {
			ret |= KS_DO_VAR_STATE;
			continue;
		}
		if (strcmp(constant[i],"KS_DO_VAR_SEM_FLAGS") == 0) {
			ret |= KS_DO_VAR_SEM_FLAGS;
			continue;
		}
		if (strcmp(constant[i],"KS_DO_VAR_TECH_UNIT") == 0) {
			ret |= KS_DO_VAR_TECH_UNIT;
			continue;
		}
		if (strcmp(constant[i],"KS_DO_VAR_COMMENT") == 0) {
			ret |= KS_DO_VAR_COMMENT;
			continue;
		}
		if (strcmp(constant[i],"KS_DO_ALL") == 0) {
			ret |= KS_DO_ALL;
			continue;
		}
		cout << "Error: Invalid dump options parameter." << endl;
		exit(-1);
	}
//	free (constant);									// ??? error! why?
	return ret;
} // GetDumpOpts

//---------------------------------------------------------------------------------

// Dump the database objects matching "sp" to file "outfile",
// "dop" specifies which information to dump
int main(int argc, char **argv)						// command line arguments
{
	KsString	outfile;			// name of output file
	int 		i;
	KsString	start_path;			// domain specified as root object
	bool		overwrite_output = false;	// overwrite output file without prompting

	cout << "** ACPLT/OV database dumper, version " << version << " **" << endl
		 << "(c) 2002 Lehrstuhl fuer Prozessleittechnik, RWTH Aachen" << endl
		 << endl;

	if (argc < 2) {
		cout << "Error: Missing parameter \"//host/server\"." << endl
			 << "Usage: " << argv[0] << " //host/server [-foutput_file] [-Ppath] [-Iidentifier]" << endl
			 << "[-Ffrom_creation_time] [-Tto_creation_time] [-Ccomment] [-y]" << endl
			 << "[-Aaccess[-access][-..]] [-Ssemantic_flags] [-Ddump_option[-dump_option][-..]]" << endl
			 << endl << "For more help type \"" << argv[0] << " -?\"" << endl;
		return -1;
	}

	if ((argc == 2 && (strcmp(argv[1], "-?") == 0))) {							// print help
		cout << "Usage: " << argv[0] << " //host/server [-foutput_file] [-Ppath] [-Iidentifier]" << endl
			 << "[-Ffrom_creation_time] [-Tto_creation_time] [-Ccomment] [-y]" << endl
			 << "[-Aaccess[-access][-..]] [-Ssemantic_flags] [-Ddump_option[-dump_option][-..]]" << endl
			 << endl
			 << "Options:" << endl
			 << "//host/server : name of the OV host and server" << endl
			 << "-foutput_file : name of the desired output file, default: dbdump.txt" << endl
			 << "-Ppath : object path to start the search with, default: /" << endl
			 << "-Iidentifier : object identifiers to search for, may contain wildcards" << endl
			 << "-Ffrom_creation_time : search for objects newer than from_creation_time" << endl
			 << "      format: YYYY/MM/DD, YYYY/MM/DD,hh:mm:ss or YYYY/MM/DD,hh:mm:ss.uuuuuu" << endl
			 << "-Tto_creation_time : search for objects older than to_creation_time" << endl
			 << "      format: YYYY/MM/DD, YYYY/MM/DD,hh:mm:ss or YYYY/MM/DD,hh:mm:ss.uuuuuu" << endl
			 << "-Ccomment : search for objects containing that comment, wildcards allowed," << endl
			 << "            no white space" << endl
			 << "-y : overwrite output file without prompt" << endl
			 << "-Aaccess[-access][-..] : search for objects matching this/these access right(s)" << endl
			 << "      valid options: KS_AC_READ           read access" << endl
			 << "                     KS_AC_WRITE          write access" << endl
			 << "                     KS_AC_READWRITE      read or write access [default]" << endl
			 << "                     KS_AC_DELETABLE      object is deletable" << endl
			 << "                     KS_AC_RENAMABLE      object is renamable" << endl
			 << "                     KS_AC_LINKABLE       object is linkable" << endl
			 << "                     KS_AC_UNLINKABLE     object is unlinkable" << endl
			 << "                     KS_AC_INSTANTIABLE   object can act as a factory" << endl
			 << "                     KS_AC_PART           object is part of another object" << endl
			 << "                     Options may be combined, separated by \"-\"" << endl
			 << "-Ssemantic_flags : search for objects matching these semantic flags" << endl
			 << "      format: \"ABC\"..\"Zabcdef\" with \"A\" = lowest bit set, .., \"f\" = highest bit" << endl
			 << "              set" << endl
			 << "-Ddump_option[-dump_option][-..] : dump only info specified by dump_options" << endl
			 << "      valid options: KS_DO_CLASS_IDENT     dump class identifier" << endl
			 << "                     KS_DO_CREATION_TIME   dump creation time" << endl
			 << "                     KS_DO_SEM_FLAGS       dump semantic flags" << endl
			 << "                     KS_DO_COMMENT         dump comment" << endl
			 << "                     KS_DO_VAR_TIME        dump variable timestamp" << endl
			 << "                     KS_DO_VAR_STATE       dump variable \"reliability\" state" << endl
			 << "                     KS_DO_VAR_SEM_FLAGS   dump variable semantic flags" << endl
			 << "                     KS_DO_VAR_TECH_UNIT   dump technical unit of variable" << endl
			 << "                     KS_DO_VAR_COMMENT     dump variable comment" << endl
			 << "                     KS_DO_ALL             dump all information [default]" << endl
			 << "                     Options may be combined, separated by \"-\"" << endl;
		return 0;
	}

	if (argc > 13) {
		cout << "Error: Too many parameters." << endl;
		return -1;
	}
	char *x = (char *)malloc(strlen(argv[1])+1);	// dummy variable
															// Check host and server name
	if (sscanf(argv[1], "%1[/]%1[/]%[a-zA-Z0-9_.:-]%1[/]%[a-zA-Z0-9_]%s", x, x, x, x, x, x) != 5) {
		cout << "Error: Invalid host and server name." << endl;
		free(x);
		return -1;
	} else {
		free(x);
	}

	sp = new search_t;								// set search parameters to default values
	sp->host_and_server = KsString(argv[1]);
	sp->path = KsString("/");
	sp->identifier = KsString("*");
	sp->from_creation_time = KsTime((long)0, (long)0);
	sp->to_creation_time = GetTime("2030/12/31");	// ???
	sp->comment = KsString("*");
	sp->access = KS_AC_READWRITE;
	sp->semantic_flags = (KS_SEMANTIC_FLAGS) 0;

	dop = new dump_opt_t;							// set dump options to default value
	*dop = 0;
	*dop |= KS_DO_ALL;

	outfile = KsString("dbdump.txt");				// default output file

	for (i=2; i<argc; i++) {						// read command line parameters
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
			case 'f': outfile = KsString(&argv[i][2]);
					  break;
			case 'P': sp->path = KsString(&argv[i][2]);
					  break;
			case 'I': sp->identifier = KsString(&argv[i][2]);
					  break;
			case 'F': sp->from_creation_time = GetTime(&argv[i][2]);
					  break;
			case 'T': sp->to_creation_time = GetTime(&argv[i][2]);
					  break;
			case 'C': sp->comment = KsString(&argv[i][2]);
					  break;
			case 'A': sp->access = GetAccRights(argv[i]);
					  break;
			case 'S': sp->semantic_flags = GetSemFlags(argv[i]);
					  break;
			case 'D': *dop = GetDumpOpts(argv[i]);
					  break;
			case 'y': overwrite_output = true;
					  break;
			default	: cout << "Invalid parameter: " << argv[i] << "." << endl;
					  delete sp;
					  delete dop;
					  return -1;
			}
		} else {
			cout << "Invalid parameter: " << argv[i] << "." << endl;
			delete sp;
			delete dop;
			return -1;
		}
	}

	db_file.open(outfile, ios::noreplace);	// open output file
	char inp;

	if (! (overwrite_output || db_file.good() )) {
		cout << "Output file " << outfile << " already exists!" << endl
			 << "If you want to replace it type \"y\": ";
		cin >> inp;
		if (inp == 'y' || inp == 'Y') {
			db_file.open(outfile);
		} else {
			cout << "Output file " << outfile << " already exists:" << endl
				 << "choose another file name!" << endl;
			delete sp;
			delete dop;
			return -1;
		}
	}
	start_path = sp->host_and_server + sp->path;
	KscAnyCommObject start_object(start_path);		// create comm. object to start with
	if (!start_object.getEngPropsUpdate()) {
		db_file << "Can't open \"" << sp->host_and_server << "\"." << endl;
		cout << "Can't open \"" << sp->host_and_server << "\"." << endl;
		delete sp;
		delete dop;
		db_file.close();
		return -1;
	}

	// check if server is OV server
	KscAnyCommObject lib_version(PltString(sp->host_and_server + "/vendor/libov_version"));
	if (!lib_version.getEngPropsUpdate()) {
		db_file << "Error: Server is not an ACPLT/OV server." << endl;
		cout << "Error: Server is not an ACPLT/OV server." << endl;
		delete sp;
		delete dop;
		db_file.close();
		return -1;
	}

	DumpEngProps(start_object, 0);		// start recursive descent with root domain
	
	// dump the objects remaining in inst_list (i.e. "children of parts") on level 0
	while (! inst_list.isEmpty() ) {
		KscAnyCommObject obj(inst_list.removeFirst() );
		DumpEngProps(obj, 0);
	}
	db_file.close();

	delete sp;
	delete dop;
	cout << "Success." << endl;
	return 0;																// no error
}

// End of dbdump.cpp
