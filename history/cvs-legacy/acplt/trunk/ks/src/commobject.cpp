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

#include <ks/commobject.h>
#include <ks/client.h>

//////////////////////////////////////////////////////////////////////
// Inline Implementation
// These functions should be inline, but there seems to be a compiler
// or linker error.
//////////////////////////////////////////////////////////////////////

KscCommObject::KscCommObject(const KscAbsPath &object_path)
: path(object_path),
  av_module(0)
{
    PLT_PRECONDITION(path.isValid());

    KscServer *myServer = getServer();
    PLT_ASSERT(myServer);
    myServer->registerVar(this);
}

//////////////////////////////////////////////////////////////////////

KscCommObject::~KscCommObject()
{
    KscServer *myServer = getServer();
    PLT_ASSERT(myServer);
    myServer->deregisterVar(this);
}

//////////////////////////////////////////////////////////////////////

PltString
KscCommObject::getName() const
{
    return path.getVarPath();
}

//////////////////////////////////////////////////////////////////////

KscAbsPath
KscCommObject::getHostAndServer() const
{
    return path.getHostAndServer();
}

//////////////////////////////////////////////////////////////////////

const KscAbsPath &
KscCommObject::getFullPath() const
{
    return path;
}

//////////////////////////////////////////////////////////////////////

void
KscCommObject::setAvModule(const KscAvModule *avm) 
{
    // don't delete old AvModule since we are not owner
    // of it
    //
    av_module = avm;
}

//////////////////////////////////////////////////////////////////////


const KscAvModule *
KscCommObject::getAvModule() const
{
    return av_module;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


KscDomain::KscDomain(const KscAbsPath &domain_path)
: KscCommObject(domain_path)
{}

//////////////////////////////////////////////////////////////////////

KS_OBJ_TYPE
KscDomain::typeCode() const
{
    return KS_OT_DOMAIN;
}

//////////////////////////////////////////////////////////////////////

const KsDomainProjProps *
KscDomain::getProjProps() const
{
    return &proj_props;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


KscVariable::KscVariable(const KscAbsPath &var_path)
: KscCommObject(var_path),
  fDirty(false)
{}

//////////////////////////////////////////////////////////////////////

KS_OBJ_TYPE
KscVariable::typeCode() const
{
    return KS_OT_VARIABLE;
}

//////////////////////////////////////////////////////////////////////


const KsVarProjProps *
KscVariable::getProjProps() const
{
    return &proj_props;
}

//////////////////////////////////////////////////////////////////////


const KsVarCurrProps *
KscVariable::getCurrProps() const
{
    return &curr_props;
}
 
//////////////////////////////////////////////////////////////////////


bool
KscVariable::isDirty() const
{
    return fDirty;
}

//////////////////////////////////////////////////////////////////////
// End of  Inline Implementation
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// class KscCommObject
//////////////////////////////////////////////////////////////////////

KscServer *
KscCommObject::getServer() const
{
    PLT_PRECONDITION(KscClient::getClient() != 0);

    return KscClient::getClient()->
        getServer(path.getHostAndServer());

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

    bool ok = myServer->getPP(params, result);

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

#if 0

bool
KscDomain::updateChilds(KS_OBJ_TYPE typeMask)
{
    // locate server
    //
    KscServer *myServer = getServer();
    PLT_ASSERT(myServer);

    // create and fill data structures
    //
    KsGetPPParams params(path, typeMask, "*");
    // params.path = path;
    // params.type_mask = typeMask;
    // params.name_mask = KsString("*");
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

    bool ok = true;

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
                new_var = new KscVariable(KscAbsPath);
                break;
            case KS_OT_DOMAIN:
                new_var = new KscDomain(KscAbsPath);
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
        if( it->a_value.typeCode() & typeMask ) {
            temp = 0;
            child_table.remove(*it, temp);
            PLT_ASSERT(temp);
            delete temp;
        }
        ++it;
    }
}

//////////////////////////////////////////////////////////////////////

KscChildIterator *
KscDomain::newChildIterator(KS_OBJ_TYPE typeMask,
                            bool update)
{
    if( update ) {
        if( !updateChildren() ) {
            return 0;
        }
    }

    ChildIterator *pit = new ChildIterator(*this, typeMask);

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
    return it.operator const void ();
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
          !(it->typeCode() & type_mask));
    
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

    return &(*it);
}

#endif

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

    bool ok = 
        myServer->getVar(params, result);

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

    bool ok = myServer->setVar(params, result);

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



        












