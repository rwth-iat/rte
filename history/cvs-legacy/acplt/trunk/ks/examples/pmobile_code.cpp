// -*-plt-c++-*-
/* $Header: /home/david/cvs/acplt/ks/examples/pmobile_code.cpp,v 1.1 1997-09-09 15:32:15 martin Exp $ */
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
/* Author: Harald Albrecht <harald@plt.rwth-aachen.de>,
           Martin Kneissl <martin@plt.rwth-aachen.de>
 */

#include <stdlib.h>
#include <unistd.h>
#include <iostream.h>

#include "ks/commobject.h"
#include "ks/package.h"
#include "plt/log.h"
#include "plt/logstream.h"

// ----------------------------------------------------------------------------
//
PltCerrLog logger("perpetuum mobile");
PltLogStream ls;


// ----------------------------------------------------------------------------
//
void buildPackage(istream & istr, KscPackage &pkg)
{
    while (istr) {
        char name[1024];
        // read variable name
        istr.getline(name, sizeof name);
        if (*name) { // at least one char
            // construct variable
            KscVariable *p = new KscVariable(name);
            if ( !p ) {
                ls << "Can't create KscVariable for \"" << name << "\".";
                ls.error();
                exit(2);
            }
            if (p->hasValidPath()) {
                // add variable to package
                if (!pkg.add(KscVariableHandle(p, PltOsNew))) {
                    ls << "Can't add Variable \"" << name << "\" to package.";
                    ls.error();
                    exit(3);
                }
            } else {
                ls << "Illegal path: \"" << name << "\"";
                ls.warning();
                delete p;
            }
        }
    }
} // buildPackage


// ----------------------------------------------------------------------------
//
void keepSpinning(KscPackage &pkg, int secs)
{
    for ( ;; ) {
        if ( !pkg.getUpdate() ) {
            ls << "Oops: couldn't update the package.";
            ls.error();
        }
        ls << "package updated.";
        ls.debug();

        PltIterator<KscVariableHandle> *pit = pkg.newVariableIterator();
        if ( pit ) {
            ls << "My shopping bag contains...";
            ls.debug();
            for (PltIterator<KscVariableHandle> & it = *pit;
                 it;
                 ++it) {
                ls << "  " << (*it)->getFullPath();
                ls.debug();
            }
            delete pit;
        } else {
            ls << "Can't create iterator for package.\n";
            ls.debug();
        }
        sleep(secs);
    }
} // keepSpinning


// ----------------------------------------------------------------------------
// Now for the main program, which keeps the wheel spinning...
//
int main(int argc, char **argv)
{
    int sleepsecs = 1;
    if ( argc > 1 ) {
        int tmp = atoi(argv[1]);
        if (tmp >= 0) {
            sleepsecs = tmp;
        }
    }
    KscPackage pkg;
    buildPackage(cin, pkg);
    ls << "Package created.";
    ls.debug();
    
    keepSpinning(pkg, sleepsecs);

    return 0;
} // main

// End of pmobile_code.cpp
