/* -*-plt-c++-*- */

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

/* Author: Markus Juergens <markusj@plt.rwth-aachen.de> */

//////////////////////////////////////////////////////////////////////

#include <ks/package.h>
#include <ks/client.h>

//////////////////////////////////////////////////////////////////////
// printing functions for debbugging
//////////////////////////////////////////////////////////////////////
#if PLT_DEBUG

void
KscPackage::debugPrint(ostream &os, bool printAll) const
{
    os << "KscPackage object :" << endl;
    os << "Related server : ";
    if(server_set) {
        os << related_server << endl;
    }
    else {
        os << "none" << endl;
    }
    os << "\tVariables : " << sizeVariables() << endl;
    os << "\tSubpackages : " << sizeSubpackages() << endl;

    if(printAll) {
        // print variables
        //
        PltIterator<KscVariable> *varit =
            newVariableIterator(false);
        if(varit) {
            while(*varit) {
                // os << (*varit)->getFullPath() << endl;
                (*varit)->debugPrint(os);
                ++(*varit);
            }
            delete varit;
        }
        // print subpackages
        //
        PltIterator<KscPackage> *pkgit =
            newSubpackageIterator();
        if(pkgit) {
            while(*pkgit) {
                (*pkgit)->debugPrint(os,true);
                ++(*pkgit);
            }
            delete pkgit;
        }
    }
}

//////////////////////////////////////////////////////////////////////

void
KscExchangePackage::debugPrint(ostream &os, bool printAll) const
{
    os << "KscExchangePackage object :" << endl;
    os << "package to get :" << endl;
    get_pkg->debugPrint(os,printAll);
    os << "package to set :" << endl;
    set_pkg->debugPrint(os,printAll);
}

#endif
//////////////////////////////////////////////////////////////////////
// end of debbugging section               
//////////////////////////////////////////////////////////////////////

KscPackage::KscPackage()
: server_set(false),
  num_vars(0),
  num_pkgs(0),
  av_module(0),
  fDirty(false)
{}

//////////////////////////////////////////////////////////////////////

KscPackage::~KscPackage()
{}

//////////////////////////////////////////////////////////////////////

bool
KscPackage::add(const KscVariable &var)
{
    if( !server_set ) {
        // first variable, set related server object
        //
        related_server = var.getHostAndServer();
        server_set = true;
    }
    else {
        // check wether variable belongs to the correct server
        //
        if( related_server != var.getHostAndServer() ) {
            return false;
        }
    }

#ifdef PLT_DEBUG
    // check for duplicated pointers to variables
    //
    PltListIterator<const KscVariable *> it(vars);
    while(it) {
        if( *it == &var ) {
            return true;
        }
        ++it;
    }
#endif
        
    bool ok = vars.addLast(&var);
    
    if(ok) num_vars++;

    return ok;
}

//////////////////////////////////////////////////////////////////////

bool
KscPackage::add(const KscPackage &pkg) 
{
    if( !server_set ) {
        // if the package has a related server take it
        //
        if( pkg.server_set ) {
            related_server = pkg.related_server;
            server_set = true;
        }
    }
    else {
        // check wether related servers match
        //
        if( pkg.related_server != related_server ) {
            return false;
        }
    }

#ifdef PLT_DEBUG
    // check for duplicated pointers to variables
    //
    PltListIterator<const KscPackage *> it(pkgs);
    while(it) {
        if( *it == &pkg ) {
            return true;
        }
        ++it;
    }
#endif
        
    bool ok = pkgs.addLast(&pkg);
    
    if(ok) num_pkgs++;

    return ok;
}
    
//////////////////////////////////////////////////////////////////////

bool
KscPackage::remove(const KscVariable &var) 
{
    bool ok = vars.remove(&var);
    
    if(ok) num_vars--;

    return ok;
}

//////////////////////////////////////////////////////////////////////

bool
KscPackage::remove(const KscPackage &pkg)
{
    bool ok = pkgs.remove(&pkg);

    if(ok) num_pkgs--;

    return ok;
}

//////////////////////////////////////////////////////////////////////

size_t 
KscPackage::sizeVariables(bool deep) const
{
    if(deep) {
        size_t count = 0;
        // iterate over subpackages
        //
        PltListIterator<const KscPackage *> it(pkgs);
        while(it) {
            count += (*it)->sizeVariables(true);
            ++it;
        }
        return count + num_vars;
    }

    return num_vars;
}

//////////////////////////////////////////////////////////////////////

PltIterator<KscVariable> *
KscPackage::newVariableIterator(bool deep) const
{
    if(deep) {
        return new DeepIterator(*this);
    } 
    else {
        return new KscDirectIterator<KscVariable>(vars);
    }
}

//////////////////////////////////////////////////////////////////////

PltIterator<KscPackage> *
KscPackage::newSubpackageIterator() const 
{
    return new KscDirectIterator<KscPackage>(pkgs);
}

//////////////////////////////////////////////////////////////////////
// TODO:
//   change absolute path names to relative path names 
//   in order to save space
//
bool
KscPackage::getUpdate() 
{
    int count, size;

    // create objects for getVar service
    //
    size = sizeVariables(true); // count all variables including this
                                // in subpackages

    if( !size ) {
        // empty packages need no update 
        return true;
    }

    KsGetVarParams params(size);
    KsGetVarResult result(0);

    // locate the server
    //
    if( !server_set ) {
        // if we have no related server, 
        // the package is empty anyway
        return true;
    }
    KscServer *myServer = 
        KscClient::getClient()->getServer(related_server);

    PLT_ASSERT(myServer);

    // put all variables in params
    //
    DeepIterator it(*this);
    count = 0;
    while(it) {
        params.identifiers[count++] = it->getName();
        it++;
    }

    PLT_ASSERT(count == size);

    // request updated data from the server
    //
    bool ok = myServer->getVar(params, result);

    // check errors
    //
    if( !ok || result.result != KS_ERR_OK ) {
        return false;
    }

    // copy new data
    //
    it.toStart();
    count = 0;
    ok = true;
    while(it) {
        KsCurrPropsHandle *hcp =
            &(result.items[count].item);
        if(result.items[count].result == KS_ERR_OK
           && *hcp ) 
        {
            PLT_ASSERT((*hcp)->xdrTypeCode() == KS_OT_VARIABLE);

            it->setCurrProps(*(KsVarCurrProps *)(hcp->getPtr()));
            it->fDirty = false;
        }
        else {
            ok = false;
        }
        ++count;
        ++it;
    }

    PLT_ASSERT( size == count );

    fDirty = ok;
    return ok;
}

//////////////////////////////////////////////////////////////////////
// class KscPackage::DeepIterator
//////////////////////////////////////////////////////////////////////

KscPackage::DeepIterator::DeepIterator(const KscPackage &itOver)
: pkg(itOver),
  rek_it(0),
  vars_it(itOver.vars),
  pkgs_it(itOver.pkgs)
{}

//////////////////////////////////////////////////////////////////////

KscPackage::DeepIterator::~DeepIterator()
{
    if(rek_it) {
        delete rek_it;
    }
}

//////////////////////////////////////////////////////////////////////

KscPackage::DeepIterator::operator 
const void *() const
{
    if(vars_it) {
        return this;
    }

    if(pkgs_it) {
        return this;
    }

    if(rek_it && *rek_it) {
        return this;
    }

    return NULL;
}

//////////////////////////////////////////////////////////////////////

const KscVariable &
KscPackage::DeepIterator::operator * () const
{
    PLT_PRECONDITION(*this);

    // check for elements in this package
    //
    if(vars_it) {
        return **vars_it;
    }

    // check for elements in the subpackages
    //
    if(rek_it && *rek_it) {
        return **rek_it;
    }

    // this is an error condition
    //
    PLT_ASSERT(0);
}
    
//////////////////////////////////////////////////////////////////////
    
KscVariable *
KscPackage::DeepIterator::operator -> () const
{
    PLT_PRECONDITION(*this);

    // check for elements in this package
    //
    if(vars_it) {
        return *vars_it;
    }

    // check for elements in the subpackages
    //
    if(rek_it && *rek_it) {
        return &(**rek_it);
    }

    // this is an error condition
    //
    PLT_ASSERT(0);
}

//////////////////////////////////////////////////////////////////////

KscPackage::DeepIterator &
KscPackage::DeepIterator::operator ++()
{
    PLT_PRECONDITION(*this);
    // check for variables in this package left
    //
    if(vars_it) {
        ++vars_it;
        // if we have more variables
        // there is no need to check the packages
        //
        if(vars_it) {
            return *this;
        }
    }
    
    // check for variables in the current subpackage
    //
    if(rek_it && *rek_it) {
        ++(*rek_it);
        // if this iterator holds more variables 
        // we are finished
        if( *rek_it ) {
            return *this;
        }
    }

    // check wether there are more subpackages left
    //
    while(pkgs_it) {
        if(rek_it) {
            delete rek_it;
        }
        rek_it = new DeepIterator(**pkgs_it);
        PLT_ASSERT(rek_it);
        ++pkgs_it;
        if(*rek_it) {
            return *this;
        }
    }

    // we ran out of variables
    //
    return *this;
}

//////////////////////////////////////////////////////////////////////

void
KscPackage::DeepIterator::operator ++ (int)
{
  PLT_PRECONDITION(*this);
    // check for variables in this package left
    //
    if(vars_it) {
        ++vars_it;
        // if we have more variables
        // there is no need to check the packages
        //
        if(vars_it) {
            return;
        }
    }
    
    // check for variables in the current subpackage
    //
    if(rek_it && *rek_it) {
        ++(*rek_it);
        // if this iterator holds more variables 
        // we are finished
        if( *rek_it ) {
            return;
        }
    }

    // check wether there are more subpackages left
    //
    while(pkgs_it) {
        if(rek_it) {
            delete rek_it;
        }
        rek_it = new DeepIterator(**pkgs_it);
        PLT_ASSERT(rek_it);
        ++pkgs_it;
        if(*rek_it) {
            return;
        }
    }
  
    // no more variables left
    //
    return;
}

//////////////////////////////////////////////////////////////////////

void
KscPackage::DeepIterator::toStart()
{
    if(rek_it) {
        delete rek_it;
        rek_it = 0;
    }
    pkgs_it.toStart();
    vars_it.toStart();
}
    















