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

#include "ks/commobject.h"
#include "ks/client.h"

//////////////////////////////////////////////////////////////////////
// printing functions for debbugging
//////////////////////////////////////////////////////////////////////
#if PLT_DEBUG

void
KscCommObject::debugPrint(ostream &os) const
{
    os << "Path and Name: " << PltString(path) << endl;
}

//////////////////////////////////////////////////////////////////////

void
KscVariable::debugPrint(ostream &os) const
{
    os << "KscVariable object :" << endl;
    KscCommObject::debugPrint(os);
    os << "Dirty Flag : " << fDirty << endl;
    proj_props.debugPrint(os);
    curr_props.debugPrint(os);
}

//////////////////////////////////////////////////////////////////////

void
KscDomain::debugPrint(ostream &os) const
{
    os << "KscDomain object :" << endl;
    KscCommObject::debugPrint(os);
    proj_props.debugPrint(os);
    os << child_table.size() << " childs :" << endl;

    // iterate over childs and print them
    //
    PltListIterator<KsProjPropsHandle> it(child_table);

    while(it) {
        (*it)->debugPrint(os);
        ++it;
    }
}

#endif // PLT_DEBUG

//////////////////////////////////////////////////////////////////////
// end of debbugging section
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Implementation of RTTI
//
PLT_IMPL_RTTI0(KscCommObject);
PLT_IMPL_RTTI1(KscVariable, KscCommObject);
PLT_IMPL_RTTI1(KscDomain, KscCommObject);


//////////////////////////////////////////////////////////////////////
// class KscCommObject
//////////////////////////////////////////////////////////////////////

KscCommObject::KscCommObject(const char *object_path)
: path(object_path),
  av_module(0)
{
    PLT_PRECONDITION(path.isValid());

    server = findServer();
    PLT_ASSERT(server);
    server->incRefcount();
}

//////////////////////////////////////////////////////////////////////

#if 0
KscCommObject::KscCommObject(const KscAbsPath &object_path)
: path(object_path),
  av_module(0)
{
    PLT_PRECONDITION(path.isValid());

    server = findServer();
    PLT_ASSERT(server);
    server->incRefcount(this);
}

//////////////////////////////////////////////////////////////////////

KscCommObject::KscCommObject(const KscCommObject &other)
: path(other.path),
  server(other.server),
  av_module(other.av_module)
{
    PLT_ASSERT(myServer);
    server->incRefcount();
}

//////////////////////////////////////////////////////////////////////

KscCommObject &
KscCommObject::operator = (const KscCommObject &other)
{
    // deregister at old server
    //
    server->decRefcount();

    // copy data
    //
    path = other.path;
    server = other.server;
    av_module = other.av_module;

    // register at new server
    //
    server->incRefcount();

    return *this;
}

#endif

//////////////////////////////////////////////////////////////////////

KscCommObject::~KscCommObject()
{
    KscServer *myServer = getServer();
    PLT_ASSERT(myServer);
    myServer->decRefcount();
}

//////////////////////////////////////////////////////////////////////
   
KscServer *
KscCommObject::findServer()
{
    return KscClient::getClient()->
        createServer(path.getHostAndServer());

}

//////////////////////////////////////////////////////////////////////

KscServer *
KscCommObject::getServer() const
{
    return server;
}

//////////////////////////////////////////////////////////////////////
// class KscDomain
//////////////////////////////////////////////////////////////////////

KscDomain::~KscDomain()
{
    flushChilds();
    PLT_POSTCONDITION(child_table.size() == 0);
}

//////////////////////////////////////////////////////////////////////

bool
KscDomain::getProjPropsUpdate() 
{
    KscServer *myServer = getServer();
    PLT_ASSERT(myServer);

    KsGetPPParams params;
    KsGetPPResult result;

    params.path = path.getPathOnly();
    params.type_mask = KS_OT_DOMAIN;
    params.name_mask = path.getName();

    bool ok = myServer->getPP(av_module,
                              params, 
                              result);

    if(ok &&
       result.result == KS_ERR_OK &&
       result.items.size() == 1) 
    {
        KsProjPropsHandle hpp = result.items.removeFirst();
        if( hpp->xdrTypeCode() == KS_OT_DOMAIN ) {
            proj_props = *(KsDomainProjProps *)hpp.getPtr();
            return true;
        }
    }

    return false;
}

//////////////////////////////////////////////////////////////////////

bool
KscDomain::setProjProps(KsProjPropsHandle hpp) 
{
    if(hpp &&
       hpp->xdrTypeCode() == typeCode()) 
    {
        proj_props = *(KsDomainProjProps *)hpp.getPtr();
        return true;
    }

    return false;
}

//////////////////////////////////////////////////////////////////////

bool
KscDomain::getChildPPUpdate()
{
    // locate server
    //
    KscServer *myServer = getServer();
    PLT_ASSERT(myServer);

    // create and fill data structures
    //
    KsGetPPParams params;
    params.path = path.getPathAndName();
    params.type_mask = KS_OT_ANY;
    params.name_mask = KsString("*");
    KsGetPPResult result;

    // request service
    //
    bool ok = myServer->getPP(av_module,
                              params, result);

    if( !(ok && result.result == KS_ERR_OK) ) {
        return false;
    }

    // delete old childs and insert the new one
    //
    flushChilds();

    ok = true;

    while(!result.items.isEmpty() && ok) {
        KsProjPropsHandle hpp = result.items.removeFirst();
        if(hpp) {
            ok = child_table.addLast(hpp);
            if(!ok) {            
                PLT_DMSG("KscDomain::getChildPPUpdate() : cannot add new commobject to child table" << endl);
            }
        }
        else {
            ok = false;
            PLT_DMSG("KscDomain::getChildPPUpdate() : unbound handle returned" << endl);
        }
    } // while

    fChildPPValid = ok;
            
    return ok;
}        

//////////////////////////////////////////////////////////////////////

bool
KscDomain::flushChilds()
{
    while(!child_table.isEmpty()) {
        child_table.removeFirst();
    }

    fChildPPValid = false;

    return true;
}

//////////////////////////////////////////////////////////////////////

KscChildIterator *
KscDomain::newChildIterator(KS_OBJ_TYPE typeMask,
                            bool update)
{
    if( !fChildPPValid || update ) {
        if( !getChildPPUpdate() ) {
            PLT_DMSG("Cannot update childs in KscDomain::newChildIterator" << endl);
            return 0;
        }
    }

    KscChildIterator *pit = new ChildIterator(*this, typeMask);

    return pit;
}

//////////////////////////////////////////////////////////////////////
// class KscDomain::ChildIterator
//
KscDomain::ChildIterator::ChildIterator(const KscDomain &domain,
                                        enum_t typeMask)
: it(domain.child_table),
  type_mask(typeMask)
{
    // move forward to the first element
    // matching with type_mask
    //
    while( it && !((*it)->xdrTypeCode() & type_mask)) {
        ++it;
    }
}

//////////////////////////////////////////////////////////////////////

KscDomain::ChildIterator::operator
const void * () const 
{
    return it.operator const void * ();
}

//////////////////////////////////////////////////////////////////////

KscDomain::ChildIterator &
KscDomain::ChildIterator::operator ++ ()
{
    PLT_PRECONDITION(*this);

    do {
        ++it;
    }
    while(it && 
          !((*it)->xdrTypeCode() & type_mask));
    
    return *this;
}

//////////////////////////////////////////////////////////////////////
        
void
KscDomain::ChildIterator::operator ++ (int)
{
    ++it;
}

//////////////////////////////////////////////////////////////////////

void
KscDomain::ChildIterator::toStart()
{
    it.toStart();
}

//////////////////////////////////////////////////////////////////////

const KsProjPropsHandle &
KscDomain::ChildIterator::operator * () const
{
    PLT_PRECONDITION(*this);

    return *it;
}


//////////////////////////////////////////////////////////////////////
// class KscVariable
//////////////////////////////////////////////////////////////////////

bool
KscVariable::getProjPropsUpdate()
{
    KscServer *myServer = getServer();
    PLT_ASSERT(myServer);

    KsGetPPParams params;
    KsGetPPResult result;

    params.path = path.getPathOnly();
    params.type_mask = KS_OT_VARIABLE;
    params.name_mask = path.getName();

    bool ok = myServer->getPP(av_module,
                              params, result);

    if(ok &&
       result.result == KS_ERR_OK &&
       result.items.size() == 1) 
    {
        KsProjPropsHandle hpp = result.items.removeFirst();
        if( hpp->xdrTypeCode() == KS_OT_VARIABLE ) {
            proj_props = *(KsVarProjProps *)hpp.getPtr();
            return true;
        }
    }

    return false;
}

//////////////////////////////////////////////////////////////////////

bool
KscVariable::setProjProps(KsProjPropsHandle hpp)
{
    if(hpp
       && hpp->xdrTypeCode() == typeCode())
    {
        proj_props = *(KsVarProjProps *)hpp.getPtr();
        return true;
    }

    return false;
}

//////////////////////////////////////////////////////////////////////

bool
KscVariable::getUpdate() 
{
    KscServer *myServer = getServer();
    PLT_ASSERT(myServer);

    KsGetVarParams params(1);
    params.identifiers[0] = path.getPathAndName();

    KsGetVarResult result(1);

    bool ok = myServer->getVar(av_module,
                               params, 
                               result);

    if( ok && result.result == KS_ERR_OK )  {
        // check wether typecode is ok
        //
        KsGetVarItemResult *pitem = &(result.items[0]);

        if(pitem->result != KS_ERR_OK ||
           pitem->item->xdrTypeCode() != KS_OT_VARIABLE) {
            return false;
        }

        curr_props = *(KsVarCurrProps *)pitem->item.getPtr(); 
        
        fDirty = false;

        return true;
    }

    return false;
}

//////////////////////////////////////////////////////////////////////

bool
KscVariable::setUpdate()
{
    KscServer *myServer = getServer();
    PLT_ASSERT(myServer);

    KsSetVarParams params(1);
    params.items[0].path_and_name = path.getPathAndName();
    params.items[0].curr_props = 
        KsCurrPropsHandle( &curr_props, KsOsUnmanaged);

    KsSetVarResult result(1);

    bool ok = myServer->setVar(av_module,
                               params,
                               result);

    if(ok && 
       result.result == KS_ERR_OK &&
       result.results[0].result == KS_ERR_OK) {
        // successs
        //
        fDirty = false;
        return true;
    }

    return false;
}

//////////////////////////////////////////////////////////////////////

KsValueHandle
KscVariable::getValue() const
{
    return curr_props.value; 
}

//////////////////////////////////////////////////////////////////////

bool
KscVariable::setCurrProps(KsVarCurrProps &cp)
{
    curr_props = cp;

    fDirty = true;

    return true;
}

//////////////////////////////////////////////////////////////////////



        












