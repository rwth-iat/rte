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
    KsString str(path);
    os << "Path and Name: " << str << endl;
    os << "Last result:" << getLastResult() << endl;
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
}

#endif // PLT_DEBUG

//////////////////////////////////////////////////////////////////////
// end of debbugging section
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Implementation of RTTI
//////////////////////////////////////////////////////////////////////

PLT_IMPL_RTTI0(KscCommObject);
PLT_IMPL_RTTI1(KscVariable, KscCommObject);
PLT_IMPL_RTTI1(KscDomain, KscCommObject);

//////////////////////////////////////////////////////////////////////
// class KscCommObject
//////////////////////////////////////////////////////////////////////

KscCommObject::KscCommObject(const char *object_path)
: path(object_path),
  av_module(0),
  last_result(-1)
{
    if(hasValidPath()) {
        server = findServer();
        PLT_ASSERT(server);
        server->incRefcount();
    } else {
        server = 0;
    }
}

//////////////////////////////////////////////////////////////////////

KscCommObject::~KscCommObject()
{
    KscServerBase *myServer = getServer();

    if(hasValidPath() && myServer) {
        myServer->decRefcount();
    }
}

//////////////////////////////////////////////////////////////////////
   
KscServerBase *
KscCommObject::findServer()
{
    return KscClient::getClient()->
        createServer(path.getHostAndServer());

}

//////////////////////////////////////////////////////////////////////

KscServerBase *
KscCommObject::getServer() const
{
    return server;
}

//////////////////////////////////////////////////////////////////////
// class KscDomain
//////////////////////////////////////////////////////////////////////

KscDomain::~KscDomain()
{
}

//////////////////////////////////////////////////////////////////////

bool
KscDomain::getProjPropsUpdate() 
{
    if(!hasValidPath()) {
        last_result = KS_ERR_MALFORMEDPATH;
        return false;
    }

    KscServerBase *myServer = getServer();
    if(!myServer) {
        // this could only happen  due to insufficient memory
        //
        last_result = KS_ERR_GENERIC;
        return false;
    }

    KsGetPPParams params;
    KsGetPPResult result;

    params.path = path.getPathOnly();
    params.type_mask = KS_OT_DOMAIN;
    params.name_mask = path.getName();

    bool ok = myServer->getPP(av_module,
                              params, 
                              result);
    //
    // TODO: extend KS_RESULT for more specific failure indication
    //
    if(ok) {
        last_result = result.result;
        if(result.result == KS_ERR_OK) {
            if(result.items.size() == 1) { 
                KsProjPropsHandle hpp = result.items.removeFirst();
                if( hpp ) {
                    if(hpp->xdrTypeCode() == KS_OT_DOMAIN ) {
                        proj_props = *(KsDomainProjProps *)hpp.getPtr();
                        return true;
                    } else {
                        // type mismatch
                        last_result = KS_ERR_TYPEMISMATCH;
                    }
                } else {
                    // out of mem
                    last_result = KS_ERR_GENERIC;
                }
            } else {
                // more than one item in response, 
                // it is more likely an internal error
                last_result = KS_ERR_GENERIC;
            }
        }
    } else {
        // failed to do getPP
        // result.result should be -1 indicating that no meaningful
        // result was returned
        //
        last_result = result.result;
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
KscDomain::getChildPPUpdate(KS_OBJ_TYPE typeMask,
                            KsString nameMask,
                            PltList<KsProjPropsHandle> &pp_list)
{
    if( !hasValidPath() ) {
        last_result = KS_ERR_MALFORMEDPATH;
        return false;
    }

    // locate server
    //
    KscServerBase *myServer = getServer();
    if(!myServer) {
        // this could only happen  due to insufficient memory
        //
        last_result = KS_ERR_GENERIC;
        return false;
    }

    // create and fill data structures
    //
    KsGetPPParams params;
    params.path = path.getPathAndName();
    params.type_mask = typeMask;
    params.name_mask = nameMask;
    KsGetPPResult result;

    // request service
    //
    bool ok = myServer->getPP(av_module,
                              params, result);

    //
    // TODO: extend KS_RESULT for more specific error indication
    //
    if(!ok) {
        last_result = KS_ERR_NETWORK;
        return false;
    } else {
        last_result = result.result;
    }

    if(last_result != KS_ERR_OK) ) {
        return false;
    }

    // 
    // copy PP
    //
    ok = true;

    while(!result.items.isEmpty() && ok) {
        KsProjPropsHandle hpp = result.items.removeFirst();
        if(hpp) {
            ok = pp_list.addLast(hpp);
            if(!ok) {            
                PLT_DMSG("KscDomain::getChildPPUpdate() : cannot add new commobject to child table" << endl);
            }
        }
        else {
            ok = false;
            PLT_DMSG("KscDomain::getChildPPUpdate() : unbound handle returned" << endl);
        }
    } // while

    if(!ok) {
        last_result = KS_ERR_GENERIC;  // TODO : change to more specific code
    }
            
    return ok;
}        

//////////////////////////////////////////////////////////////////////

KscChildIterator *
KscDomain::newChildIterator(KS_OBJ_TYPE typeMask, KsString nameMask)
{
    PltList<KsProjPropsHandle> *pp_list =
        new PltList<KsProjPropsHandle>;

    if(pp_list) {
        if(getChildPPUpdate(typeMask, nameMask, *pp_list)) {
            // success, now create iterator
            //
            ChildIterator *it = new ChildIterator(pp_list);
            if(it) {
                return it;
            } else {
                // clean up list
                //
                delete pp_list;
                return 0;
            }
        } else {
            delete pp_list;
            return 0;
        }
    } else {
        last_result = KS_ERR_GENERIC;
        return 0;
    }
}

//////////////////////////////////////////////////////////////////////
// class KscDomain::ChildIterator
//////////////////////////////////////////////////////////////////////

KscDomain::ChildIterator::ChildIterator(PltList<KsProjPropsHandle> *lst)
: PltListIterator<KsProjPropsHandle>(*lst),
  pp_list(lst)
{}

//////////////////////////////////////////////////////////////////////

KscDomain::ChildIterator::~ChildIterator()
{
    delete pp_list;
}

//////////////////////////////////////////////////////////////////////
// class KscVariable
//////////////////////////////////////////////////////////////////////

bool
KscVariable::getProjPropsUpdate()
{
    if( !hasValidPath() ) {
        last_result = KS_ERR_MALFORMEDPATH;
        return false;
    }

    KscServerBase *myServer = getServer();


    KsGetPPParams params;
    KsGetPPResult result;

    params.path = path.getPathOnly();
    params.type_mask = KS_OT_VARIABLE;
    params.name_mask = path.getName();

    bool ok = myServer->getPP(av_module,
                              params, result);

    //
    // TODO: extend KS_RESULT for more specific error indication
    //
    if(ok) {
        last_result = result.result;
        if(result.result == KS_ERR_OK) {
            if(result.items.size() == 1) {
                KsProjPropsHandle hpp = result.items.removeFirst();
                if( hpp ) {
                    if( hpp->xdrTypeCode() == KS_OT_VARIABLE ) {
                        proj_props = *(KsVarProjProps *)hpp.getPtr();
                        return true;
                    } else {
                        // type mismatch
                        last_result = KS_ERR_TYPEMISMATCH;
                    }
                } else {
                    // out of mem
                    last_result = KS_ERR_GENERIC;
                }
            } else {
                // too much items,
                // maybe an internal error
                last_result = KS_ERR_GENERIC;
            }
        } // result.result == KS_ERR_OK
    } else {
        // getPP failed
        last_result = result.result;  // == -1
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
    if( !hasValidPath() ) {
        last_result = KS_ERR_MALFORMEDPATH;
        return false;
    }

    KscServerBase *myServer = getServer();
    if(!myServer) {
        // this could only happen  due to insufficient memory
        //
        last_result = KS_ERR_GENERIC;
        return false;
    }

    KsGetVarParams params(1);
    params.identifiers[0] = path.getPathAndName();

    KsGetVarResult result(1);

    bool ok = myServer->getVar(av_module,
                               params, 
                               result);
    //
    // TODO: extend KS_RESULT for more specific error indication
    //
    last_result = result.result;

    if( ok ) {
        if( result.result == KS_ERR_OK )  {
            // check wether typecode is ok
            //
            KsGetVarItemResult *pitem = &(result.items[0]);
            
            if(pitem->result == KS_ERR_OK) {
                if(pitem->item->xdrTypeCode() == KS_OT_VARIABLE) {
                    // everything went ok
                    curr_props = *(KsVarCurrProps *)pitem->item.getPtr(); 
                    fDirty = false;
                    return true;
                } else {
                    // type mismatch
                    last_result = KS_ERR_TYPEMISMATCH;
                }
            } else {
                last_result = pitem->result;
            }
        }
    } else {
        // failed to do getVar
        last_result = KS_ERR_NETWORKERROR;
    }

    return false;
}

//////////////////////////////////////////////////////////////////////

bool
KscVariable::setUpdate()
{
    if( !hasValidPath() ) {
        last_result = KS_ERR_MALFORMEDPATH;
        return false;
    }

    KscServerBase *myServer = getServer();
    if(!myServer) {
        // this could only happen  due to insufficient memory
        //
        last_result = KS_ERR_GENERIC;
        return false;
    }

    KsSetVarParams params(1);
    params.items[0].path_and_name = path.getPathAndName();
    params.items[0].curr_props = 
        KsCurrPropsHandle( &curr_props, KsOsUnmanaged);

    KsSetVarResult result(1);

    bool ok = myServer->setVar(av_module,
                               params,
                               result);

    //
    // TODO: extend KS_RESULT for more specific error indication
    //
    last_result = result.result;

    if(ok) { 
        if(result.result == KS_ERR_OK) {
            if(result.results[0].result == KS_ERR_OK) {
                // successs
                //
                fDirty = false;
                return true;
            } else {
                // error in writing item
                last_result = result.results[0].result;
            }
        } else {
            // error in whole request
            // last_result already set
        }
    } else {
        // setVar failed
        last_result = KS_ERR_NETWORKERROR;
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
// EOF commobject.cpp
//////////////////////////////////////////////////////////////////////



        













