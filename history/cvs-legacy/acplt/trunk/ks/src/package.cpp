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

//////////////////////////////////////////////////////////////////////

KscPackage::KscPackage()
: num_vars(0),
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
    PltListIterator<const KscVariable *> it(vars);
    
    bool ok = vars.remove(&var);
    
    if(ok) num_vars--;

    return ok;
}

//////////////////////////////////////////////////////////////////////

bool
KscPackage::remove(const KscPackage &pkg)
{
    PltListIterator<const KscPackage *> it(pkgs);

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
    
const KscVariable *
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
    






