/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/examples/ttree1.cpp,v 1.17 2003-09-23 15:36:23 harald Exp $ */
/*
 * Copyright (c) 1996, 1997, 1998, 1999
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
/* Author: Harald Albrecht <harald@plt.rwth-aachen.de> */

/*
 * This is a simple demonstration of how to use the basic c++ communication
 * objects on the client's side. You supply the host/server on the command
 * line, and this little gem (or is it just a dust ball?) prints out the
 * tree of communication objects it can find within the KS server.
 * NEW: now it spits out the variables' values as well...
 */

#include "ks/commobject.h"
#if PLT_USE_DEPRECIATED_HEADER
#include <iostream.h>
#include <iomanip.h>
#else
#include <iostream>
#include <iomanip>
#endif
#include <time.h>

//##
#include "ks/avmodule.h"
#include "ks/avsimplemodule.h"

#define INDENTATION 3


void Indent(int ind)
{
    while ( --ind >= 0 ) {
	STDNS::cout << ' ';
    }
} // Indent


void DumpVarType(const int var_type)
{
    switch ( var_type ) {
    case KS_VT_VOID:       STDNS::cout << "void "; break;
    case KS_VT_BOOL:       STDNS::cout << "bool "; break;
    case KS_VT_INT:        STDNS::cout << "int "; break;
    case KS_VT_UINT:       STDNS::cout << "uint "; break;
    case KS_VT_SINGLE:     STDNS::cout << "single "; break;
    case KS_VT_DOUBLE:     STDNS::cout << "double "; break;
    case KS_VT_STRING:     STDNS::cout << "string "; break;
    case KS_VT_TIME:       STDNS::cout << "time "; break;
    case KS_VT_BYTE_VEC:   STDNS::cout << "byte vector "; break;
    case KS_VT_BOOL_VEC:   STDNS::cout << "bool vector "; break;
    case KS_VT_INT_VEC:    STDNS::cout << "int vector "; break;
    case KS_VT_UINT_VEC:   STDNS::cout << "uint vector "; break;
    case KS_VT_SINGLE_VEC: STDNS::cout << "single vector "; break;
    case KS_VT_DOUBLE_VEC: STDNS::cout << "double vector "; break;
    case KS_VT_STRING_VEC: STDNS::cout << "string vector "; break;
    case KS_VT_TIME_VEC:   STDNS::cout << "time vector "; break;
    default:               STDNS::cout << "<unknown type> "; break;
    }
} // DumpVarType


void DumpLinkType(const int link_type)
{
    switch ( link_type ) {
    case KS_LT_LOCAL_1_1:     STDNS::cout << "local 1:1 "; break;
    case KS_LT_LOCAL_1_MANY:  STDNS::cout << "local 1:m "; break;
    case KS_LT_GLOBAL_1_1:    STDNS::cout << "global 1:1 "; break;
    case KS_LT_GLOBAL_1_MANY: STDNS::cout << "global 1:m "; break;
    default:                  STDNS::cout << "<unknown type> "; break;
    }
} // DumpLinkType


void DumpEngProps(const KsEngProps &proj_props, int indent)
{
    PltString info;
    time_t creation_time;

    //
    // Get our hands on the object's name...
    //
    Indent(indent);
    if ( proj_props.access_mode & KS_AC_PART ) {
	STDNS::cout << ".";
    }
    STDNS::cout << proj_props.identifier << " ";

    //
    // Set up some info, which consists of the creation time and the access
    // mode(s). This info is used lateron...
    //
    creation_time = proj_props.creation_time.tv_sec;
    info += PltString(ctime(&creation_time)).substr(4, 15);
    info += " ";

    if ( proj_props.access_mode & KS_AC_READ ) {
        info += "readable ";
    }
    if ( proj_props.access_mode & KS_AC_WRITE ) {
        info += "writeable ";
    }

    //
    // Look! Who's there... what, a communication object?!
    //
    switch ( proj_props.xdrTypeCode() ) {
    case KS_OT_DOMAIN:
	STDNS::cout << "<DOM> " << info;
	break;
        
    case KS_OT_LINK:
        {
	    STDNS::cout << "<LNK> " << info;

            const KsLinkEngProps *link_proj_props =
                (const KsLinkEngProps *) &proj_props; // for msvc

            if( !link_proj_props ) {
                STDNS::cout << "<unknown projected properties>";
            } else {
		DumpLinkType(link_proj_props->type);
	    }
	    break;
	}

    case KS_OT_HISTORY:
	STDNS::cout << "<HST> " << info;
	break;

    case KS_OT_VARIABLE:
        {
            STDNS::cout << "<VAR> " << info;
            
            const KsVarEngProps *var_proj_props =
                (const KsVarEngProps *) &proj_props; // for msvc
// replaced:            PLT_DYNAMIC_PCAST(KsVarEngProps, &proj_props);
            if( !var_proj_props ) {
                STDNS::cout << "<unknown projected properties>";
            } else {
                DumpVarType(var_proj_props->type);
            }
            break;
        }
    default:
        STDNS::cout << "<-?-> " << info;
    }

    STDNS::cout << STDNS::endl;
} // DumpEngProps


void DumpEngProps(KscCommObject &obj, int indent)
{
    const KsEngProps *proj_props;

    if ( !obj.getEngPropsUpdate() ) {
        STDNS::cout << "Can't retrieve proj. props for \"" 
             << obj.getName() 
             << "\""
             << STDNS::endl;
        return;
    }
    proj_props = obj.getEngProps();
    if ( !proj_props ) {
        STDNS::cout << "Can't get hands on \"" 
             << obj.getName() 
             << "\" for proj. props"
             << STDNS::endl;
        return;
    }
    DumpEngProps(*proj_props, indent);
} // DumpEngProps


void DumpVar(KscVariable &var, int indent)
{
    KsTime             time;
    time_t             value_time;
    size_t             size, i;
    const KsVarCurrProps *curr_props;

    if ( !var.getUpdate() ) {
        STDNS::cout << "Can't retrieve curr. props for \""
             << var.getName()
             << "\""
             << STDNS::endl;
        return;
    }
    curr_props = var.getCurrProps();
    if ( !curr_props ) {
        STDNS::cout << "Can't get hands on \""
             << var.getName()
             << "\" for curr. props"
             << STDNS::endl;
    }

    Indent(indent);
    STDNS::cout << "** ";

    value_time = curr_props->time.tv_sec;
    STDNS::cout << PltString(ctime(&value_time)).substr(4, 15) << " ";
    switch ( curr_props->state ) {
    case KS_ST_NOTSUPPORTED:
        STDNS::cout << "(no state supported) ";
        break;
    case KS_ST_UNKNOWN:
        STDNS::cout << "(state unknown) ";
        break;
    case KS_ST_BAD:
        STDNS::cout << "(state bad) ";
        break;
    case KS_ST_QUESTIONABLE:
        STDNS::cout << "(state questionable) ";
        break;
    case KS_ST_GOOD:
        STDNS::cout << "(state good) ";
        break;
    default:
        STDNS::cout << "(state -?-) ";
    }

    switch ( curr_props->value->xdrTypeCode() ) {
    case KS_VT_VOID:
        STDNS::cout << "void" << STDNS::endl;
        break;
    case KS_VT_BOOL:
        STDNS::cout << "bool "
             << ((bool) ((KsBoolValue &) *curr_props->value) ? "true" : "false")
             << STDNS::endl;
	break;
    case KS_VT_INT:
        STDNS::cout << "integer " 
             << (long) ((KsIntValue &) *curr_props->value)
             << STDNS::endl;
        break;
    case KS_VT_UINT:
        STDNS::cout << "unsigned integer " 
             << (u_long) ((KsUIntValue &) *curr_props->value)
             << STDNS::endl;
        break;
    case KS_VT_SINGLE:
        STDNS::cout << "single " 
             << (float) ((KsSingleValue &) *curr_props->value)
             << STDNS::endl;
        break;
    case KS_VT_DOUBLE:
        STDNS::cout << "double " 
             << (double) ((KsDoubleValue &) *curr_props->value)
             << STDNS::endl;
        break;
    case KS_VT_STRING:
        STDNS::cout << "string \"";
        if ( ((KsStringValue &) *curr_props->value).len() > 40 ) {
            STDNS::cout << ((KsStringValue &) *curr_props->value).substr(0, 40)
                 << "...";
        } else {
            STDNS::cout << (const char *) ((KsStringValue &) *curr_props->value);
        }
        STDNS::cout << "\"" << STDNS::endl;
        break;
    case KS_VT_TIME:
        time = ((KsTimeValue &) *curr_props->value);
        value_time = time.tv_sec;
        STDNS::cout << "time " 
             << PltString(ctime(&value_time)).substr(4, 15)
             << STDNS::endl;
        break;
    case KS_VT_BYTE_VEC:
        size = ((KsByteVecValue &) *curr_props->value).size();
        STDNS::cout << "byte vector <" << size << "> {";
        if ( size > 5 ) {
            size = 5;
        }
        for ( i = 0; i < size; ++i ) {
            STDNS::cout <<  STDNS::hex << STDNS::setfill('0') << STDNS::setw(2)
                 << (unsigned int) ((KsByteVecValue &) *curr_props->value)[i]
                 << STDNS::dec << ",";
        }
        STDNS::cout << "...}" << STDNS::endl;
        break;
    case KS_VT_BOOL_VEC:
        size = ((KsByteVecValue &) *curr_props->value).size();
        STDNS::cout << "bool vector <" << size << "> {";
        if ( size > 5 ) {
            size = 5;
        }
        for ( i = 0; i < size; ++i ) {
            STDNS::cout <<  STDNS::hex << STDNS::setfill('0') << STDNS::setw(2)
                 << ((bool) ((KsBoolVecValue &) *curr_props->value)[i] ? "true" : "false")
                 << STDNS::dec << ",";
        }
        STDNS::cout << "...}" << STDNS::endl;
        break;
    case KS_VT_INT_VEC:
        size = ((KsIntVecValue &) *curr_props->value).size();
        STDNS::cout << "integer vector <" << size << "> {";
        if ( size > 5 ) {
            size = 5;
        }
        for ( i = 0; i < size; ++i ) {
            STDNS::cout << ((KsIntVecValue &) *curr_props->value)[i]
                 << ",";
        }
        STDNS::cout << "...}" << STDNS::endl;
        break;
    case KS_VT_UINT_VEC:
        size = ((KsUIntVecValue &) *curr_props->value).size();
        STDNS::cout << "unsigned integer vector <" << size << "> {";
        if ( size > 5 ) {
            size = 5;
        }
        for ( i = 0; i < size; ++i ) {
            STDNS::cout << ((KsUIntVecValue &) *curr_props->value)[i]
                 << ",";
        }
        STDNS::cout << "...}" << STDNS::endl;
        break;
    case KS_VT_SINGLE_VEC:
        size = ((KsSingleVecValue &) *curr_props->value).size();
        STDNS::cout << "single vector <" << size << "> {";
        if ( size > 5 ) {
            size = 5;
        }
        for ( i = 0; i < size; ++i ) {
            STDNS::cout << ((KsSingleVecValue &) *curr_props->value)[i]
                 << ",";
        }
        STDNS::cout << "...}" << STDNS::endl;
        break;
    case KS_VT_DOUBLE_VEC:
        size = ((KsDoubleVecValue &) *curr_props->value).size();
        STDNS::cout << "double vector <" << size << "> {";
        if ( size > 5 ) {
            size = 5;
        }
        for ( i = 0; i < size; ++i ) {
            STDNS::cout << ((KsDoubleVecValue &) *curr_props->value)[i]
                 << ",";
        }
        STDNS::cout << "...}" << STDNS::endl;
        break;
    case KS_VT_STRING_VEC:
        size = ((KsStringVecValue &) *curr_props->value).size();
        STDNS::cout << "string vector <" << size << "> {";
        if ( size > 5 ) {
            size = 5;
        }
        for ( i = 0; i < size; ++i ) {
            if ( ((KsStringVecValue &) *curr_props->value)[i].len() > 40 ) {
                STDNS::cout << ((KsStringVecValue &) *curr_props->value)[i].substr(0, 40)
                     << "...";
            } else {
                STDNS::cout << (const char *) ((KsStringVecValue &) *curr_props->value)[i];
            }
            STDNS::cout << ",";
        }
        STDNS::cout << "...}" << STDNS::endl;

        break;
    case KS_VT_TIME_VEC:
        size = ((KsTimeVecValue &) *curr_props->value).size();
        STDNS::cout << "time vector <" << size << "> {";
        if ( size > 5 ) {
            size = 5;
        }
        for ( i = 0; i < size; ++i ) {
            time = ((KsTimeVecValue &) *curr_props->value)[i];
            value_time = time.tv_sec;
            STDNS::cout << PltString(ctime(&value_time)).substr(4, 15)
                 << ",";
        }
        STDNS::cout << "...}" << STDNS::endl;

        break;
    default:
        STDNS::cout << "-?-" << STDNS::endl;
    }
} // DumpVar


void DumpBranch(KscAnyCommObject &branch, int indent)
{
    KscChildIterator *children;

    children = branch.newChildIterator(KS_OT_ANY);
    if ( !children ) {
        STDNS::cout << "Can't allocate child iterator" << STDNS::endl;
        return;
    }
    //
    // Now iterate over all the children of the current level, we're in.
    // Take care that you increment the iterator, and not the pointer to
    // it... (whistle, whistle...)
    //
    for ( ; *children; ++*children ) {
        KsEngPropsHandle current(**children);
        DumpEngProps(*current, indent + INDENTATION);
	PltString delemiter(!indent ? "" :
			    (current->access_mode & KS_AC_PART ? "." : "/"));

        if ( current->xdrTypeCode() == KS_OT_DOMAIN ) {
            KscAnyCommObject child_domain(PltString(branch.getFullPath()) + 
                                          delemiter +
                                          current->identifier);
            DumpBranch(child_domain, indent + INDENTATION);

        } else if ( current->xdrTypeCode() == KS_OT_LINK ) {
            KscAnyCommObject child_domain(PltString(branch.getFullPath()) + 
                                          delemiter +
                                          current->identifier);
            KscVariable child_var(PltString(branch.getFullPath()) + 
				  delemiter +
				  current->identifier);
            DumpVar(child_var, indent + 2 * INDENTATION);
            DumpBranch(child_domain, indent + INDENTATION);

        } else if ( current->xdrTypeCode() == KS_OT_HISTORY ) {
            KscAnyCommObject child_domain(PltString(branch.getFullPath()) + 
                                          delemiter +
                                          current->identifier);
            DumpBranch(child_domain, indent + INDENTATION);

        } else if ( current->xdrTypeCode() == KS_OT_VARIABLE ) {
            KscVariable var(PltString(branch.getFullPath()) + 
                                   delemiter +
                                   current->identifier);
#if 0
	    Indent(indent);
	    STDNS::cout << "##" << var.getFullPath() << "##" << STDNS::endl;
#endif
            DumpVar(var, indent + 2 * INDENTATION);
        }
    }

    //
    // finally, clean up everything we've allocated in this level...
    //
    delete children;
} // DumpBranch


int main(int argc, char **argv)
{
    KsString host_and_server;

#if 0
    KscPathParser pp1("//host");
    KscPathParser pp2("//host/server");
    KscPathParser pp3("//host/server/");
    KscPathParser pp4("//host/server/path/to/object");
    KscPathParser pp5("//host/server/path/to.object");
    KscPathParser pp6("/path/to.object");

    cerr << (const char *)(KsString) pp1 << ": " << pp1.getType() << endl;
    cerr << (const char *)(KsString) pp2 << ": " << pp2.getType() << endl;
    cerr << (const char *)(KsString) pp3 << ": " << pp3.getType() << endl;
    cerr << (const char *)(KsString) pp4 << ": " << pp4.getType() << endl;
    cerr << (const char *)(KsString) pp5 << ": " << pp5.getType() << endl;
    cerr << (const char *)(KsString) pp6 << ": " << pp6.getType() << endl;

    cerr << pp1.getParent(true) << endl;
    cerr << pp2.getParent(true) << endl;
    cerr << pp2.getParent(false) << endl;
    cerr << pp3.getParent(true) << endl;
    cerr << pp3.getParent(false) << endl;
    cerr << pp4.getParent(true) << endl;
    cerr << pp5.getParent(true) << endl;
    cerr << pp6.getParent(true) << endl;

    cerr << pp5.resolve(".part") << endl;
    cerr << pp5.resolve("child") << endl;
    cerr << pp5.resolve("./.part") << endl;
    cerr << pp5.resolve("./child") << endl;
    cerr << pp5.resolve("../.part") << endl;
    cerr << pp5.resolve("../../.part") << endl;
    cerr << pp5.resolve("../../../../../.part") << endl;
    cerr << pp5.resolve("./../this/is/./../.part") << endl;
    cerr << pp5.resolve("./this.part/is/far/away") << endl;

    return 0;
#endif


    if ( argc != 2 ) {
        STDNS::cerr << "usage: ttree <host>/<server>" << STDNS::endl;
        return 1;
    }

    host_and_server = KsString("//") + argv[1];
    host_and_server += "/";
    KscAnyCommObject root(host_and_server);

    if ( !root.getEngPropsUpdate() ) {
        STDNS::cerr << "Can't open /" << argv[1] << STDNS::endl;
        return 42;
    }


#if 0
    KscAnyCommObject test1(host_and_server + ".X");
    if ( !test1.hasValidPath() ) {
	STDNS::cerr << "Invalid path: " << (const char *) test1.getFullPath()
	     << STDNS::endl;
	return 43;
    }
    cout << (const char *) test1.getPathOnly() << " # "
	 << (const char *) test1.getName() << " # "
	 << (test1.isNamePart() ? "part" : "child") << endl;
    KscAnyCommObject test2(host_and_server + "A.B/C.X");
    if ( !test2.hasValidPath() ) {
	STDNS::cerr << "Invalid path: " << (const char *) test2.getFullPath()
	     << STDNS::endl;
	return 43;
    }
    STDNS::cout << (const char *) test2.getPathOnly() << " # "
	 << (const char *) test2.getName() << " # "
	 << (test2.isNamePart() ? "part" : "child") << STDNS::endl;
    KscAnyCommObject test3(host_and_server + "A.B/C/X");
    if ( !test3.hasValidPath() ) {
	STDNS::cerr << "Invalid path: " << (const char *) test3.getFullPath()
	     << STDNS::endl;
	return 43;
    }
    STDNS::cout << (const char *) test3.getPathOnly() << " # "
	 << (const char *) test3.getName() << " # "
	 << (test3.isNamePart() ? "part" : "child") << STDNS::endl;

    return 0;
#endif

    
    DumpEngProps(root, 0);
    DumpBranch(root, 0);

    return 0;
} // main

/* End of ttree1.cpp */
