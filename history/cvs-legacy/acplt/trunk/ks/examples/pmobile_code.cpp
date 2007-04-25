// -*-plt-c++-*-
/* $Header: /home/david/cvs/acplt/ks/examples/pmobile_code.cpp,v 1.17 2007-04-25 12:57:20 martin Exp $ */
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
/* Author: Harald Albrecht <harald@plt.rwth-aachen.de>,
           Martin Kneissl <martin@plt.rwth-aachen.de>
 */

#include "plt/config.h"

#include <stdlib.h>
#include <iostream.h>
#include <iomanip.h>

#if PLT_SYSTEM_NT
#include <strstrea.h>
#include <time.h>
#else
#include <strstream.h>
#endif

#include <fstream.h>

#include "ks/commobject.h"
#include "ks/package.h"
#include "plt/log.h"
#include "plt/logstream.h"
#include "plt/time.h"

static int verbosity = 1;
inline void check_pointer(void *) { }

//////////////////////////////////////////////////////////////////////

inline float frand() {
    float r = rand();
    return r/RAND_MAX;
}

//////////////////////////////////////////////////////////////////////

void
print_value(KsValueHandle hv) {
/// Stolen from Markus' tshell.cpp ....
    if(!hv) {
        cout << "no value" << endl;
        return;
    }

    switch(hv->xdrTypeCode()) {
    case KS_VT_VOID : 
        {
            cout << "void" << endl;
        } 
        break;
    case KS_VT_INT : 
        {
            KsIntValue *pInt =
                PLT_DYNAMIC_PCAST(KsIntValue, hv.getPtr());
            check_pointer(pInt);
            cout << "Int " << (long)(*pInt) << endl;
        }
        break;
    case KS_VT_UINT :
        {
            KsUIntValue *pUInt =
                PLT_DYNAMIC_PCAST(KsUIntValue, hv.getPtr());
            check_pointer(pUInt);
            cout << "UInt " << (u_long)(*pUInt) << endl;
        }
        break;
    case KS_VT_SINGLE :
        {
            KsSingleValue *pSingle =
                PLT_DYNAMIC_PCAST(KsSingleValue, hv.getPtr());
            check_pointer(pSingle);
            cout << "Single " << (float)(*pSingle) << endl;
        }
        break;
    case KS_VT_DOUBLE :
        {
            KsDoubleValue *pDouble =
                PLT_DYNAMIC_PCAST(KsDoubleValue, hv.getPtr());
            check_pointer(pDouble);
            cout << "Double " << (double)(*pDouble) << endl;
        }
        break;
    case KS_VT_STRING :
        {
            KsStringValue *pStr =
                PLT_DYNAMIC_PCAST(KsStringValue, hv.getPtr());
            check_pointer(pStr);
            cout << "String " << (const char *)(*pStr) << endl;
        }
        break;
    case KS_VT_TIME :
        {
            KsTimeValue *pTime =
                PLT_DYNAMIC_PCAST(KsTimeValue, hv.getPtr());
            check_pointer(pTime);
#if PLT_SYSTEM_OPENVMS
            unsigned long sec = pTime->tv_sec;
#else
            time_t sec = pTime->tv_sec;
#endif
            cout << "Time " << ctime(&sec) << endl;
        }
        break;
    case KS_VT_BYTE_VEC :
    case KS_VT_INT_VEC :
    case KS_VT_UINT_VEC :
    case KS_VT_SINGLE_VEC :
    case KS_VT_DOUBLE_VEC :
    case KS_VT_STRING_VEC :
    case KS_VT_TIME_VEC :
        {
            cout << "Vector value" << endl;
        }
        break;
    default:
        {
            cout << "unknown typecode" << endl;
        }
    }
}

// ----------------------------------------------------------------------------
//
PltCerrLog logger("perpetuum mobile");
PltLogStream ls;


// ----------------------------------------------------------------------------
//
PltArray<PltString> readNames(istream & istr)
{
    PltList<PltString> list;
    while (istr) {
        char name[1024];
        // read variable name
        istr.getline(name, sizeof name);
        if (*name) { // at least one char
            // construct variable
            list.addLast(PltString(name));
        }
    }
    PltArray<PltString> array(list.size());
    for (size_t i=0; ! list.isEmpty(); ++i) {
        array[i] = list.removeFirst();
    }
    return array;
}

//////////////////////////////////////////////////////////////////////

KscPackage * buildPackage(const PltArray<PltString> & array, 
                  size_t maxsize = 0)
{
    KscPackage &pkg = *(new KscPackage);
    bool random = (maxsize!=0);
    size_t size = random ? (size_t)(frand()*maxsize+1) : array.size();
    if (verbosity > 1) {
        cout << "Building package with " << size << " variables..." ;
        cout.flush();
    }
    for (size_t i=0; i < size; ++i) {
        size_t index = random ? (size_t)(frand()*array.size()) : i; 
        const PltString & name = array[index];

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
    if (verbosity>1) {
        cout << " done." << endl;
    }
    return &pkg;
} // buildPackage


// ----------------------------------------------------------------------------
//
void keepSpinning(const PltArray<PltString> & names, 
                  int updates, 
                  const PltTime & delay, 
                  size_t maxsize,
                  int randperc)
{
    float prand = (maxsize>0) ? randperc / 100.0 : 0.0;
    KscPackage *ppkg = buildPackage(names, maxsize);
    int i=0;
    while (updates == -1 || i < updates) {
        ++i;
        if (frand() < prand) {
            // new package
            delete ppkg;
            ppkg = buildPackage(names, maxsize);
        }
        if (verbosity>1) {
            cerr << "starting getUpdate() ..." << endl;
        }
        PltTime start(PltTime::now());
        if ( !ppkg->getUpdate() ) {
            ls << "Oops: couldn't update the package, reason 0x"
	       << hex << setfill('0') << setw(4)
	       << ppkg->getLastResult() << dec;
            ls.error();
        }
        PltTime elapsed = PltTime::now() - start;
        if (verbosity>1) {
            cerr << "getUpdate() finished" << endl;
        }
        if (verbosity>0) {
            double secs = elapsed.tv_sec + elapsed.tv_usec / 1.0e6;
            size_t size = ppkg->sizeVariables();
            cout << "Elapsed: " << secs << "s "
                 << "Variables: " << size
                 << " Avg: "    << secs / size << " s/var "
                 <<                size / secs << " var/s "
                 << endl;
        }

        if (verbosity>2) {
            PltIterator<KscVariableHandle> *pit = 
                PLT_RETTYPE_CAST((PltIterator<KscVariableHandle> *))
                    ppkg->newVariableIterator();
            if ( pit ) {
                cout << "My shopping bag contains..." << endl;
                for (PltIterator<KscVariableHandle> & it = *pit;
                     it;
                     ++it) {
                    cout << "  " << (*it)->getFullPath();
                    if (verbosity>3) {
                        cout << " ";
                        print_value((*it)->getValue());
                    }
                    cout << endl;
                }
                delete pit;
            } else {
                ls << "Can't create iterator for package.\n";
                ls.warning();
            }
        }
        if (verbosity>1) {
            cout << "Sleeping...";
            cout.flush();
        }
        delay.sleep();
        if (verbosity>1) {
            cout << endl;
        }
    }
    delete ppkg;
} // keepSpinning


// ----------------------------------------------------------------------------
// Now for the main program, which keeps the wheel spinning...
//
int main(int argc, char **argv)
{
    int sleepsecs = 1;
    char * filename = 0;
    int updates = -1;
    int randomperc = 0;
    size_t maxsize = 0;
    ifstream fstr;
    PltArray<PltString> names;

    for (int argi=1; argi < argc; ++argi) {
        char * arg = argv[argi];
        if (arg[0] == '-') {
            // it is a switch
            int val = -1;
            switch (arg[1]) {
            case 'c' : 
            case 's' :
            case 'v' :
            case 'r' :
            case 'm' : 
                if (++argi < argc) {
                    istrstream str(argv[argi]);
                    str >> val;
                    if (!str) goto usage;
                    if (-1 != str.get()) goto usage;
                }
                break;
            default:
                // does not require an argument
                // do nothing
                {}
            }
            switch(arg[1]) {
            case 's' :
                if (val < 0) goto usage;
                sleepsecs = val;
                break;
            case 'c' :
                updates = val;
                break;
            case 'v':
                verbosity = val;
                break;
            case 'm':
                maxsize = val;
                break;
            case 'r':
                if (0 <= val && val <= 100 ) {
                    randomperc = val;
                } else {
                    goto usage;
                }
                break;
            default:
                goto usage;
            }
        } else {
            // it is the name of a file containing variable paths
            filename = arg;
            if (argi+1 != argc) goto usage;
        }
    }
    
    if (randomperc == 0) {
        maxsize = 0;
    }

    if (filename) {
        fstr.open(filename);
        names = readNames(fstr);
    } else {
        names = readNames(cin);
    }
    if (verbosity > 0) {
        ls << names.size() << "names  read.";
        ls.debug();
    }
    
    keepSpinning(names, updates, PltTime(sleepsecs,0),
                 maxsize, randomperc);

    return 0;

 usage:
    cerr << argv[0] << " [-v verbosity] [-c updates] [-s secs_between_updates] [-r random_perc [-m max_pkg_size]] [filename]"
        << endl;
    return 1;

} // main

// End of pmobile_code.cpp



