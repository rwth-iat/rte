/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/examples/ttree1.cpp,v 1.5 1997-07-18 14:09:10 martin Exp $ */
/*
 * Copyright (c) 1996, 1997
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
 */

#include "ks/commobject.h"
#include <iostream.h>
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
    case KS_VT_INT:        cout << "int "; break;
    case KS_VT_UINT:       cout << "uint "; break;
    case KS_VT_SINGLE:     cout << "single "; break;
    case KS_VT_DOUBLE:     cout << "double "; break;
    case KS_VT_STRING:     cout << "string "; break;
    case KS_VT_TIME:       cout << "time "; break;
    case KS_VT_BYTE_VEC:   cout << "byte vector "; break;
    case KS_VT_INT_VEC:    cout << "int vector "; break;
    case KS_VT_UINT_VEC:   cout << "uint vector "; break;
    case KS_VT_SINGLE_VEC: cout << "single vector "; break;
    case KS_VT_DOUBLE_VEC: cout << "double vector "; break;
    case KS_VT_STRING_VEC: cout << "string vector "; break;
    case KS_VT_TIME_VEC:   cout << "time vector "; break;
    default:               cout << "<unknown type> "; break;
    }
} // DumpVarType


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
        cout << "Can't retrieve \"" 
	     << obj.getName() 
	     << "\" for proj. props"
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


void DumpBranch(KscDomain &branch, int indent)
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
            KscDomain child_domain(PltString(branch.getFullPath()) + 
                                   PltString(indent ? "/" : "") +
                                   current->identifier);
            DumpBranch(child_domain, indent + INDENTATION);
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
    KscDomain root(host_and_server);
    if ( !root.getProjPropsUpdate() ) {
	cerr << "Can't open /" << argv[1] << endl;
	return 42;
    }
    
    DumpProjProps(root, 0);
    DumpBranch(root, 0);

    return 0;
} // main

/* End of ttree1.cpp */
