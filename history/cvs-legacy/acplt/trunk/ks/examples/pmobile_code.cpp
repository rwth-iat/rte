// -*-plt-c++-*-
/* $Header: /home/david/cvs/acplt/ks/examples/pmobile_code.cpp,v 1.14 2003-09-23 15:36:23 harald Exp $ */
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
#if PLT_USE_DEPRECIATED_HEADER
#include <iostream.h>
#include <iomanip.h>
#else
#include <iostream>
#include <iomanip>
#endif

#if PLT_SYSTEM_NT
#include <strstrea.h>
#include <time.h>
#else
#if PLT_USE_DEPRECIATED_HEADER
#include <strstream.h>
#else
#include <sstream>
#endif
#endif

#if PLT_USE_DEPRECIATED_HEADER
#include <fstream.h>
#else
#include <fstream>
#endif

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
        STDNS::cout << "no value" << STDNS::endl;
        return;
    }

    switch(hv->xdrTypeCode()) {
    case KS_VT_VOID : 
        {
            STDNS::cout << "void" << STDNS::endl;
        } 
        break;
    case KS_VT_INT : 
        {
            KsIntValue *pInt =
                PLT_DYNAMIC_PCAST(KsIntValue, hv.getPtr());
            check_pointer(pInt);
            STDNS::cout << "Int " << (long)(*pInt) << STDNS::endl;
        }
        break;
    case KS_VT_UINT :
        {
            KsUIntValue *pUInt =
                PLT_DYNAMIC_PCAST(KsUIntValue, hv.getPtr());
            check_pointer(pUInt);
            STDNS::cout << "UInt " << (u_long)(*pUInt) << STDNS::endl;
        }
        break;
    case KS_VT_SINGLE :
        {
            KsSingleValue *pSingle =
                PLT_DYNAMIC_PCAST(KsSingleValue, hv.getPtr());
            check_pointer(pSingle);
            STDNS::cout << "Single " << (float)(*pSingle) << STDNS::endl;
        }
        break;
    case KS_VT_DOUBLE :
        {
            KsDoubleValue *pDouble =
                PLT_DYNAMIC_PCAST(KsDoubleValue, hv.getPtr());
            check_pointer(pDouble);
            STDNS::cout << "Double " << (double)(*pDouble) << STDNS::endl;
        }
        break;
    case KS_VT_STRING :
        {
            KsStringValue *pStr =
                PLT_DYNAMIC_PCAST(KsStringValue, hv.getPtr());
            check_pointer(pStr);
            STDNS::cout << "String " << (const char *)(*pStr) << STDNS::endl;
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
            STDNS::cout << "Time " << ctime(&sec) << STDNS::endl;
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
            STDNS::cout << "Vector value" << STDNS::endl;
        }
        break;
    default:
        {
            STDNS::cout << "unknown typecode" << STDNS::endl;
        }
    }
}

// ----------------------------------------------------------------------------
//
PltCerrLog logger("perpetuum mobile");
PltLogStream ls;


// ----------------------------------------------------------------------------
//
PltArray<PltString> readNames(STDNS::istream & istr)
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
        STDNS::cout << "Building package with " << size << " variables..." ;
        STDNS::cout.flush();
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
        STDNS::cout << " done." << STDNS::endl;
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
            STDNS::cerr << "starting getUpdate() ..." << STDNS::endl;
        }
        PltTime start(PltTime::now());
        if ( !ppkg->getUpdate() ) {
            ls << "Oops: couldn't update the package, reason 0x"
	       << STDNS::hex << STDNS::setfill('0') << STDNS::setw(4)
	       << ppkg->getLastResult() << STDNS::dec;
            ls.error();
        }
        PltTime elapsed = PltTime::now() - start;
        if (verbosity>1) {
            STDNS::cerr << "getUpdate() finished" << STDNS::endl;
        }
        if (verbosity>0) {
            double secs = elapsed.tv_sec + elapsed.tv_usec / 1.0e6;
            size_t size = ppkg->sizeVariables();
            STDNS::cout << "Elapsed: " << secs << "s "
                 << "Variables: " << size
                 << " Avg: "    << secs / size << " s/var "
                 <<                size / secs << " var/s "
                 << STDNS::endl;
        }

        if (verbosity>2) {
            PltIterator<KscVariableHandle> *pit = 
                PLT_RETTYPE_CAST((PltIterator<KscVariableHandle> *))
                    ppkg->newVariableIterator();
            if ( pit ) {
                STDNS::cout << "My shopping bag contains..." << STDNS::endl;
                for (PltIterator<KscVariableHandle> & it = *pit;
                     it;
                     ++it) {
                    STDNS::cout << "  " << (*it)->getFullPath();
                    if (verbosity>3) {
                        STDNS::cout << " ";
                        print_value((*it)->getValue());
                    }
                    STDNS::cout << STDNS::endl;
                }
                delete pit;
            } else {
                ls << "Can't create iterator for package.\n";
                ls.warning();
            }
        }
        if (verbosity>1) {
            STDNS::cout << "Sleeping...";
            STDNS::cout.flush();
        }
        delay.sleep();
        if (verbosity>1) {
            STDNS::cout << STDNS::endl;
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
    STDNS::ifstream fstr;
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
#if PLT_USE_DEPRECIATED_HEADER
		    istrstream
#else
                    STDNS::istringstream
#endif
			str(argv[argi]);
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
        names = readNames(STDNS::cin);
    }
    if (verbosity > 0) {
        ls << names.size() << "names  read.";
        ls.debug();
    }
    
    keepSpinning(names, updates, PltTime(sleepsecs,0),
                 maxsize, randomperc);

    return 0;

 usage:
    STDNS::cerr << argv[0] << " [-v verbosity] [-c updates] [-s secs_between_updates] [-r random_perc [-m max_pkg_size]] [filename]"
        << STDNS::endl;
    return 1;

} // main

// End of pmobile_code.cpp



