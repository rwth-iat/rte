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
    os << "Path and Name: " << path << endl;
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
    PltHashIterator<KscAbsPath, KscCommObject *> it(child_table);

    while(it) {
        it->a_value->debugPrint(os);
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
    PLT_PRECONDITION(KscClient::getClient() != 0);

    return KscClient::getClient()->
        getServer(path.getHostAndServer());

}

//////////////////////////////////////////////////////////////////////

KscServer *
KscCommObject::getServer() const
{
    return server;
}

//////////////////////////////////////////////////////////////////////

KscNegotiator *
KscCommObject::getNegotiator() 
{
    KscNegotiator *neg;
 
    // first try to get private negotiator
    //
    if(av_module) {
        neg = av_module->getNegotiator();
        if(neg) {
            return neg;
        }
    }

    // now request the server for negotiator,
    // the server should ensure that we get
    // a valid object 
    //
    neg = getServer()->getNegotiator();

    PLT_ASSERT(neg);

    return neg;
}

//////////////////////////////////////////////////////////////////////
// class KscDomain
//////////////////////////////////////////////////////////////////////

bool
KscDomain::getProjPropsUpdate() 
{
    KscServer *myServer = getServer();
    PLT_ASSERT(myServer);

    KsGetPPParams params;
    KsGetPPResult result;

    params.path = KsString(path.getPathOnly());
    params.type_mask = KS_OT_DOMAIN;
    params.name_mask = KsString(path.getNameOnly());

    bool ok = myServer->getPP(getNegotiator(),
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
KscDomain::updateChilds(KS_OBJ_TYPE typeMask)
{
    // locate server
    //
    KscServer *myServer = getServer();
    PLT_ASSERT(myServer);

    // create and fill data structures
    //
    KsGetPPParams params;
    params.path = PltString(path.getVarPath());
    params.type_mask = typeMask;
    params.name_mask = KsString("*");
    KsGetPPResult result;

    // request service
    //
    bool ok = myServer->getPP(params, result);

    if( !(ok && result.result == KS_ERR_OK) ) {
        return false;
    }

    // delete old childs and insert the new one
    //
    flushChilds(typeMask);

    ok = true;

    while(!result.items.isEmpty() && ok) {
        KsProjPropsHandle hpp = result.items.removeFirst();
        if(hpp) {
            KscAbsPath new_path =
                path + hpp->identifier;
            PLT_ASSERT(new_path.isValid());
            
            KscCommObject *new_var = 0;

            switch(hpp->xdrTypeCode()) 
            {
            case KS_OT_VARIABLE:
                new_var = new KscVariable(new_path);
                break;
            case KS_OT_DOMAIN:
                new_var = new KscDomain(new_path);
                break;
            default:
                PLT_DMSG("Unknown objectype in KscDomain::updateChilds" << endl);
            }
            
            if(new_var) {
                new_var->setProjProps(hpp);
                ok = child_table.add(new_path, new_var);
            }
            else {
                ok = false;
            }
        }
        else {
            ok = false;
        }
    } // while
            
    return ok;
}        

//////////////////////////////////////////////////////////////////////

bool
KscDomain::flushChilds(KS_OBJ_TYPE typeMask)
{
    PltHashIterator<KscAbsPath,KscCommObject *> it(child_table);
    KscCommObject *temp;

    while(it) {
        if( it->a_value->typeCode() & typeMask ) {
            temp = 0;
            child_table.remove(it->a_key, temp);
            PLT_ASSERT(temp);
            // delete temp;
        }
        ++it;
    }

    return true;
}

//////////////////////////////////////////////////////////////////////

KscChildIterator *
KscDomain::newChildIterator(KS_OBJ_TYPE typeMask,
                            bool update)
{
    if( update ) {
        if( !updateChilds(typeMask) ) {
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
{}

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
          !(it->a_value->typeCode() & type_mask));
    
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

const KsProjProps *
KscDomain::ChildIterator::operator -> () const
{
    PLT_PRECONDITION(*this);

    return it->a_value->getProjProps();
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

    params.path = KsString(path.getPathOnly());
    params.type_mask = KS_OT_VARIABLE;
    params.name_mask = KsString(path.getNameOnly());

    bool ok = myServer->getPP(params, result);

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
    params.identifiers[0] = getName();

    KsGetVarResult result(1);

    bool ok = myServer->getVar(getNegotiator(),
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
    params.items[0].path_and_name = PltString(path.getVarPath());
    params.items[0].curr_props = 
        KsCurrPropsHandle( &curr_props, KsOsUnmanaged);

    KsSetVarResult result(1);

    bool ok = myServer->setVar(getNegotiator(),
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



        












