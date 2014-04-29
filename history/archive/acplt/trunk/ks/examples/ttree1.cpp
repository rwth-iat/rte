/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/examples/ttree1.cpp,v 1.20 2007-04-25 12:57:20 martin Exp $ */
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
#include <iostream.h>
#include <iomanip.h>
#include <time.h>

//##
#include "ks/avmodule.h"
#include "ks/avsimplemodule.h"

#define INDENTATION 3


void Indent(int ind)
{
    while ( --ind >= 0 ) {
	cout << ' ';
    }
} // Indent


void DumpVarType(const int var_type)
{
    switch ( var_type ) {
    case KS_VT_VOID:       cout << "void "; break;
    case KS_VT_BOOL:       cout << "bool "; break;
    case KS_VT_INT:        cout << "int "; break;
    case KS_VT_UINT:       cout << "uint "; break;
    case KS_VT_SINGLE:     cout << "single "; break;
    case KS_VT_DOUBLE:     cout << "double "; break;
    case KS_VT_STRING:     cout << "string "; break;
    case KS_VT_TIME:       cout << "time "; break;
    case KS_VT_BYTE_VEC:   cout << "byte vector "; break;
    case KS_VT_BOOL_VEC:   cout << "bool vector "; break;
    case KS_VT_INT_VEC:    cout << "int vector "; break;
    case KS_VT_UINT_VEC:   cout << "uint vector "; break;
    case KS_VT_SINGLE_VEC: cout << "single vector "; break;
    case KS_VT_DOUBLE_VEC: cout << "double vector "; break;
    case KS_VT_STRING_VEC: cout << "string vector "; break;
    case KS_VT_TIME_VEC:   cout << "time vector "; break;
    default:               cout << "<unknown type> "; break;
    }
} // DumpVarType


void DumpLinkType(const int link_type)
{
    switch ( link_type ) {
    case KS_LT_LOCAL_1_1:     cout << "local 1:1 "; break;
    case KS_LT_LOCAL_1_MANY:  cout << "local 1:m "; break;
    case KS_LT_GLOBAL_1_1:    cout << "global 1:1 "; break;
    case KS_LT_GLOBAL_1_MANY: cout << "global 1:m "; break;
    default:                  cout << "<unknown type> "; break;
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
	cout << ".";
    }
    cout << proj_props.identifier << " ";

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
	cout << "<DOM> " << info;
	break;
        
    case KS_OT_LINK:
        {
	    cout << "<LNK> " << info;

            const KsLinkEngProps *link_proj_props =
                (const KsLinkEngProps *) &proj_props; // for msvc

            if( !link_proj_props ) {
                cout << "<unknown projected properties>";
            } else {
		DumpLinkType(link_proj_props->type);
	    }
	    break;
	}

    case KS_OT_HISTORY:
	cout << "<HST> " << info;
	break;

    case KS_OT_VARIABLE:
        {
            cout << "<VAR> " << info;
            
            const KsVarEngProps *var_proj_props =
                (const KsVarEngProps *) &proj_props; // for msvc
// replaced:            PLT_DYNAMIC_PCAST(KsVarEngProps, &proj_props);
            if( !var_proj_props ) {
                cout << "<unknown projected properties>";
            } else {
                DumpVarType(var_proj_props->type);
            }
            break;
        }
    default:
        cout << "<???> " << info;
    }

    cout << endl;
} // DumpEngProps


void DumpEngProps(KscCommObject &obj, int indent)
{
    const KsEngProps *proj_props;

    if ( !obj.getEngPropsUpdate() ) {
        cout << "Can't retrieve proj. props for \"" 
             << obj.getName() 
             << "\""
             << endl;
        return;
    }
    proj_props = obj.getEngProps();
    if ( !proj_props ) {
        cout << "Can't get hands on \"" 
             << obj.getName() 
             << "\" for proj. props"
             << endl;
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
        cout << "Can't retrieve curr. props for \""
             << var.getName()
             << "\""
             << endl;
        return;
    }
    curr_props = var.getCurrProps();
    if ( !curr_props ) {
        cout << "Can't get hands on \""
             << var.getName()
             << "\" for curr. props"
             << endl;
        return;
    }

    Indent(indent);
    cout << "** ";

    value_time = curr_props->time.tv_sec;
    cout << PltString(ctime(&value_time)).substr(4, 15) << " ";
    switch ( curr_props->state ) {
    case KS_ST_NOTSUPPORTED:
        cout << "(no state supported) ";
        break;
    case KS_ST_UNKNOWN:
        cout << "(state unknown) ";
        break;
    case KS_ST_BAD:
        cout << "(state bad) ";
        break;
    case KS_ST_QUESTIONABLE:
        cout << "(state questionable) ";
        break;
    case KS_ST_GOOD:
        cout << "(state good) ";
        break;
    default:
        cout << "(state ???) ";
    }

    switch ( curr_props->value->xdrTypeCode() ) {
    case KS_VT_VOID:
        cout << "void" << endl;
        break;
    case KS_VT_BOOL:
        cout << "bool "
             << ((bool) ((KsBoolValue &) *curr_props->value) ? "true" : "false")
             << endl;
	break;
    case KS_VT_INT:
        cout << "integer " 
             << (long) ((KsIntValue &) *curr_props->value)
             << endl;
        break;
    case KS_VT_UINT:
        cout << "unsigned integer " 
             << (u_long) ((KsUIntValue &) *curr_props->value)
             << endl;
        break;
    case KS_VT_SINGLE:
        cout << "single " 
             << (float) ((KsSingleValue &) *curr_props->value)
             << endl;
        break;
    case KS_VT_DOUBLE:
        cout << "double " 
             << (double) ((KsDoubleValue &) *curr_props->value)
             << endl;
        break;
    case KS_VT_STRING:
        cout << "string \"";
        if ( ((KsStringValue &) *curr_props->value).len() > 40 ) {
            cout << ((KsStringValue &) *curr_props->value).substr(0, 40)
                 << "...";
        } else {
            cout << (const char *) ((KsStringValue &) *curr_props->value);
        }
        cout << "\"" << endl;
        break;
    case KS_VT_TIME:
        time = ((KsTimeValue &) *curr_props->value);
        value_time = time.tv_sec;
        cout << "time " 
             << PltString(ctime(&value_time)).substr(4, 15)
             << endl;
        break;
    case KS_VT_BYTE_VEC:
        size = ((KsByteVecValue &) *curr_props->value).size();
        cout << "byte vector <" << size << "> {";
        if ( size > 5 ) {
            size = 5;
        }
        for ( i = 0; i < size; ++i ) {
            cout <<  hex << setfill('0') << setw(2)
                 << (unsigned int) ((KsByteVecValue &) *curr_props->value)[i]
                 << dec << ",";
        }
        cout << "...}" << endl;
        break;
    case KS_VT_BOOL_VEC:
        size = ((KsByteVecValue &) *curr_props->value).size();
        cout << "bool vector <" << size << "> {";
        if ( size > 5 ) {
            size = 5;
        }
        for ( i = 0; i < size; ++i ) {
            cout <<  hex << setfill('0') << setw(2)
                 << ((bool) ((KsBoolVecValue &) *curr_props->value)[i] ? "true" : "false")
                 << dec << ",";
        }
        cout << "...}" << endl;
        break;
    case KS_VT_INT_VEC:
        size = ((KsIntVecValue &) *curr_props->value).size();
        cout << "integer vector <" << size << "> {";
        if ( size > 5 ) {
            size = 5;
        }
        for ( i = 0; i < size; ++i ) {
            cout << ((KsIntVecValue &) *curr_props->value)[i]
                 << ",";
        }
        cout << "...}" << endl;
        break;
    case KS_VT_UINT_VEC:
        size = ((KsUIntVecValue &) *curr_props->value).size();
        cout << "unsigned integer vector <" << size << "> {";
        if ( size > 5 ) {
            size = 5;
        }
        for ( i = 0; i < size; ++i ) {
            cout << ((KsUIntVecValue &) *curr_props->value)[i]
                 << ",";
        }
        cout << "...}" << endl;
        break;
    case KS_VT_SINGLE_VEC:
        size = ((KsSingleVecValue &) *curr_props->value).size();
        cout << "single vector <" << size << "> {";
        if ( size > 5 ) {
            size = 5;
        }
        for ( i = 0; i < size; ++i ) {
            cout << ((KsSingleVecValue &) *curr_props->value)[i]
                 << ",";
        }
        cout << "...}" << endl;
        break;
    case KS_VT_DOUBLE_VEC:
        size = ((KsDoubleVecValue &) *curr_props->value).size();
        cout << "double vector <" << size << "> {";
        if ( size > 5 ) {
            size = 5;
        }
        for ( i = 0; i < size; ++i ) {
            cout << ((KsDoubleVecValue &) *curr_props->value)[i]
                 << ",";
        }
        cout << "...}" << endl;
        break;
    case KS_VT_STRING_VEC:
        size = ((KsStringVecValue &) *curr_props->value).size();
        cout << "string vector <" << size << "> {";
        if ( size > 5 ) {
            size = 5;
        }
        for ( i = 0; i < size; ++i ) {
            if ( ((KsStringVecValue &) *curr_props->value)[i].len() > 40 ) {
                cout << ((KsStringVecValue &) *curr_props->value)[i].substr(0, 40)
                     << "...";
            } else {
                cout << (const char *) ((KsStringVecValue &) *curr_props->value)[i];
            }
            cout << ",";
        }
        cout << "...}" << endl;

        break;
    case KS_VT_TIME_VEC:
        size = ((KsTimeVecValue &) *curr_props->value).size();
        cout << "time vector <" << size << "> {";
        if ( size > 5 ) {
            size = 5;
        }
        for ( i = 0; i < size; ++i ) {
            time = ((KsTimeVecValue &) *curr_props->value)[i];
            value_time = time.tv_sec;
            cout << PltString(ctime(&value_time)).substr(4, 15)
                 << ",";
        }
        cout << "...}" << endl;

        break;
    default:
        cout << "???" << endl;
    }
} // DumpVar


void DumpBranch(KscAnyCommObject &branch, int indent)
{
    KscChildIterator *children;

    children = branch.newChildIterator(KS_OT_ANY);
    if ( !children ) {
        cout << "Can't allocate child iterator" << endl;
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
	    cout << "##" << var.getFullPath() << "##" << endl;
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
        cerr << "usage: ttree <host>/<server>" << endl;
        return 1;
    }

    host_and_server = KsString("//") + argv[1];
    host_and_server += "/";
    KscAnyCommObject root(host_and_server);

    if ( !root.getEngPropsUpdate() ) {
        cerr << "Can't open /" << argv[1] << endl;
        return 42;
    }


#if 0
    KscAnyCommObject test1(host_and_server + ".X");
    if ( !test1.hasValidPath() ) {
	cerr << "Invalid path: " << (const char *) test1.getFullPath()
	     << endl;
	return 43;
    }
    cout << (const char *) test1.getPathOnly() << " # "
	 << (const char *) test1.getName() << " # "
	 << (test1.isNamePart() ? "part" : "child") << endl;
    KscAnyCommObject test2(host_and_server + "A.B/C.X");
    if ( !test2.hasValidPath() ) {
	cerr << "Invalid path: " << (const char *) test2.getFullPath()
	     << endl;
	return 43;
    }
    cout << (const char *) test2.getPathOnly() << " # "
	 << (const char *) test2.getName() << " # "
	 << (test2.isNamePart() ? "part" : "child") << endl;
    KscAnyCommObject test3(host_and_server + "A.B/C/X");
    if ( !test3.hasValidPath() ) {
	cerr << "Invalid path: " << (const char *) test3.getFullPath()
	     << endl;
	return 43;
    }
    cout << (const char *) test3.getPathOnly() << " # "
	 << (const char *) test3.getName() << " # "
	 << (test3.isNamePart() ? "part" : "child") << endl;

    return 0;
#endif

    
    DumpEngProps(root, 0);
    DumpBranch(root, 0);

    return 0;
} // main

/* End of ttree1.cpp */
