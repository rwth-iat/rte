/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/examples/ttree1.cpp,v 1.12 1999-01-29 12:38:43 harald Exp $ */
/*
 * Copyright (c) 1996, 1997, 1998, 1999
 * Chair of Process Control Engineering,
 * Aachen University of Technology.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must print or display the above
 *    copyright notice either during startup or must have a means for
 *    the user to view the copyright notice.
 * 3. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the Chair of Process Control Engineering nor the
 *    name of the Aachen University of Technology may be used to endorse or
 *    promote products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE CHAIR OF PROCESS CONTROL
 * ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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


void DumpProjProps(const KsProjProps &proj_props, int indent)
{
    PltString info;
    time_t creation_time;

    //
    // Get our hands on the object's name...
    //
    Indent(indent);
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

            const KsLinkProjProps *link_proj_props =
                (const KsLinkProjProps *) &proj_props; // for msvc

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
            
            const KsVarProjProps *var_proj_props =
                (const KsVarProjProps *) &proj_props; // for msvc
// replaced:            PLT_DYNAMIC_PCAST(KsVarProjProps, &proj_props);
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
} // DumpProjProps


void DumpProjProps(KscCommObject &obj, int indent)
{
    const KsProjProps *proj_props;

    if ( !obj.getProjPropsUpdate() ) {
        cout << "Can't retrieve proj. props for \"" 
             << obj.getName() 
             << "\""
             << endl;
        return;
    }
    proj_props = obj.getProjProps();
    if ( !proj_props ) {
        cout << "Can't get hands on \"" 
             << obj.getName() 
             << "\" for proj. props"
             << endl;
        return;
    }
    DumpProjProps(*proj_props, indent);
} // DumpProjProps


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
        KsProjPropsHandle current(**children);
        DumpProjProps(*current, indent + INDENTATION);

        if ( current->xdrTypeCode() == KS_OT_DOMAIN ) {
            KscAnyCommObject child_domain(PltString(branch.getFullPath()) + 
                                          PltString(indent ? "/" : "") +
                                          current->identifier);
            DumpBranch(child_domain, indent + INDENTATION);

        } else if ( current->xdrTypeCode() == KS_OT_LINK ) {
            KscAnyCommObject child_domain(PltString(branch.getFullPath()) + 
                                          PltString(indent ? "/" : "") +
                                          current->identifier);
            KscVariable child_var(PltString(branch.getFullPath()) + 
                                   PltString(indent ? "/" : "") +
                                   current->identifier);
            DumpVar(child_var, indent + 2 * INDENTATION);
            DumpBranch(child_domain, indent + INDENTATION);

        } else if ( current->xdrTypeCode() == KS_OT_HISTORY ) {
            KscAnyCommObject child_domain(PltString(branch.getFullPath()) + 
                                          PltString(indent ? "/" : "") +
                                          current->identifier);
            DumpBranch(child_domain, indent + INDENTATION);

        } else if ( current->xdrTypeCode() == KS_OT_VARIABLE ) {
            KscVariable var(PltString(branch.getFullPath()) + 
                                   PltString(indent ? "/" : "") +
                                   current->identifier);
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

    if ( argc != 2 ) {
        cerr << "usage: ttree <host>/<server>" << endl;
        return 1;
    }

    host_and_server = KsString("//") + argv[1];
    host_and_server += "/";
    KscAnyCommObject root(host_and_server);
    if ( !root.getProjPropsUpdate() ) {
        cerr << "Can't open /" << argv[1] << endl;
        return 42;
    }
    
    DumpProjProps(root, 0);
    DumpBranch(root, 0);

    return 0;
} // main

/* End of ttree1.cpp */
