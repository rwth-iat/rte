/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/commobject.cpp,v 1.21 1998-01-12 07:49:26 harald Exp $ */
/*
 * Copyright (c) 1996, 1997, 1998
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
/* Dumb comments, redesigned child iterator and improved error handling
   by: Harald Albrecht <harald@plt.rwth-aachen.de> */

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
  _last_result(KS_ERR_OK)
{
    if ( hasValidPath() ) {
	//
	// As soon as this comm. object is created, we try to get our
	// hands on a server object, which we will use lateron to
	// send requests to an ACPLT/KS server. If we can't get our
	// hands on a server object, then findServer() will already
	// have set the result code of this communication object, so
	// we don't have to do this here ourselves.
	//
        server = findServer();
	if ( server ) {
	    server->incRefcount();
	}
    } else {
	//
	// If already the name was malformed, we don't even try to find
	// a suitable server object and just set the last result to
	// reflect the malformed resource locator.
	//
        server = 0;
	_last_result = KS_ERR_MALFORMEDPATH;
    }
} // KscCommObject::KscCommObject


//////////////////////////////////////////////////////////////////////
// Clean up a communication object.
//
KscCommObject::~KscCommObject()
{
    KscServerBase *myServer = getServer();

    // FIXME markusj: is hasValidPath() necessary here??
    if ( hasValidPath() && myServer ) {
        myServer->decRefcount();
    }
} // KscCommObject::~KscCommObject


//////////////////////////////////////////////////////////////////////
// Creates a new server object for this communication object and
// returns the pointer to this server object. In case it fails, then
// _last_result will be set accordingly to the error reason.
// Note that is function is only called *ONCE* from the constructor.
// If you loose at this point, then you must throw away such a
// communication object.
//
KscServerBase *
KscCommObject::findServer()
{
    KscServerBase *pServer;
    _last_result = KscClient::getClient()->
        createServer(path.getHostAndServer(), pServer);
    return pServer;
} // KscCommObject::findServer


//////////////////////////////////////////////////////////////////////
// TODO: markusj, isn't this a good candidate for inlining, is it?
KscServerBase *
KscCommObject::getServer() const
{
    return server;
} // KscCommObject::getServer


//////////////////////////////////////////////////////////////////////
// class KscDomain
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Destruct a domain object. Not much to do here...
//
KscDomain::~KscDomain()
{
} // KscDomain::~KscDomain


//////////////////////////////////////////////////////////////////////
// Query the projected properties of the domain object (communication
// object of type variable) within the ACPLT/KS server this object
// refers to.
//
bool
KscDomain::getProjPropsUpdate() 
{
    //
    // Just to make sure the caller is not trying (once) again to read
    // a variable with a bad resource locator.
    //
    if ( !hasValidPath() ) {
        _last_result = KS_ERR_MALFORMEDPATH;
        return false;
    }

    //
    // Get your hands on the server object which is responsible for the
    // communication. If we get back a null pointer instead, then for
    // some reason during construction of this object, no server object
    // could be created. Because we can't yet determine what went wrong
    // we just return a generic error indication. If you want to know
    // exactly what happened, then you must immediately call getLastResult()
    // after you've created a communication object.
    //
    KscServerBase *myServer = getServer();
    if ( !myServer ) {
        _last_result = KS_ERR_GENERIC;
        return false;
    }

    //
    // Set up the service parameters. We ask exactly for this domain
    // communication object within the server. Note that not all ACPLT/KS
    // might support the GetPP service on all the domains they provide.
    //
    KsGetPPParams params;
    KsGetPPResult result;

    params.path = path.getPathOnly();
    params.type_mask = KS_OT_DOMAIN;
    params.name_mask = path.getName();

    //
    // Now query the ACPLT/KS server with the help of the server object. The
    // server object will handle the request accordingly to whatever kind
    // of ACPLT/KS server it is connected with.
    //
    bool ok = myServer->getPP(av_module,
                              params, 
                              result);
    if ( ok ) {
	//
	// The request succeeded at least at the communication level. So
	// check whether the request itself was granted by the ACPLT/KS
	// server.
	//
        if ( result.result == KS_ERR_OK ) {
	    //
            // Yes, the request was granted. Now check whether we've got
	    // back the projected properties of exactly *ONE* domain.
	    // Otherwise something got really mixed up. Maybe some joker
	    // included a wildcard character in the resource locator...
            //
            if ( result.items.size() == 1 ) { 
                KsProjPropsHandle hpp = result.items.removeFirst();
                if ( hpp ) {
                    if ( hpp->xdrTypeCode() == KS_OT_DOMAIN ) {
                        proj_props = *(KsDomainProjProps *)hpp.getPtr();
			_last_result = KS_ERR_OK;
                        return true;
                    } else {
			//
                        // The resource locator of this variable does not
			// refer to a variable object but instead to some
			// other communication object within the server.
			// Dear programmer, you've messed it up!
			//
                        _last_result = KS_ERR_TYPEMISMATCH;
                    }
                } else {
		    //
                    // An unbound handle, so I presume: out of memory...
		    //
                    _last_result = KS_ERR_GENERIC;
                }
            } else {
		//
		// Either the ACPLT/KS server does not support the GetPP
		// on this particular domain, or there was a wildcard
		// in the domains's name. So bail out with an error.
		//
                _last_result = KS_ERR_GENERIC;
            }
        } else {
	    //
	    // The request was *NOT* granted by the ACPLT/KS server. The
	    // service reply contains the reason for the failure, so we
	    // return this through the "last result" member variable. One
	    // prominent reason for failing here is that the ACPLT/KS does
	    // not support the GetPP service on this particular domain.
	    //
	    _last_result = result.result;
        }
    } else {
	//
        // The GetPP service request failed on the communication level.
	// In this case, the server object supplies more precise information
	// about the cause of the failure.
	//
        _last_result = myServer->getLastResult();
    }

    return false;
} // KscDomain::getProjPropsUpdate


//////////////////////////////////////////////////////////////////////
// Set the projected properties of this domain object. This one is
// definetly *UNSUPPORTED*, so never, NEVER, **NEVER** use it! Also
// it's of no use (yet).
//
bool
KscDomain::setProjProps(KsProjPropsHandle hpp) 
{
    if ( hpp && (hpp->xdrTypeCode() == typeCode()) ) {
        proj_props = *(KsDomainProjProps *) hpp.getPtr();
        return true;
    }

    return false;
} // KscDomain::setProjProps


//////////////////////////////////////////////////////////////////////
// Query all the children of this domain object, which fit into the
// name mask and type mask.
// --aldi: the function now takes as its third argument a reference to
// a result object pointer. The result object of a GetPP service
// request contains already the list of the projected properties of
// the children, so why should we build a second list? So we just
// return the result object if we succeed with our query. The caller
// is then responsible to destroy that result object later, when he's
// not interested in it anymore. In fact, the ChildIterator local class
// takes over the ownership of the result object and destroys it when
// it gets destroyed itself.
//
bool
KscDomain::getChildPPUpdate(KS_OBJ_TYPE typeMask,
                            KsString nameMask,
                            KsGetPPResult *&result)
{
    //
    // Just to make sure the caller is not trying (once) again to read
    // a variable with a bad resource locator.
    //
    if ( !hasValidPath() ) {
        _last_result = KS_ERR_MALFORMEDPATH;
        return false;
    }

    //
    // Get your hands on the server object which is responsible for the
    // communication. If we get back a null pointer instead, then for
    // some reason during construction of this object, no server object
    // could be created. Because we can't yet determine what went wrong
    // we just return a generic error indication. If you want to know
    // exactly what happened, then you must immediately call getLastResult()
    // after you've created a communication object.
    //
    KscServerBase *myServer = getServer();
    if ( !myServer ) {
        _last_result = KS_ERR_GENERIC;
        return false;
    }

    //
    // Set up the service parameters. Note that not all ACPLT/KS servers
    // might support the GetPP service on all the variables they provide.
    //
    KsGetPPParams params;

    result = new KsGetPPResult;
    if ( !result ) {
	_last_result = KS_ERR_GENERIC;
	return false;
    }

    params.path = path.getPathAndName();
    params.type_mask = typeMask;
    params.name_mask = nameMask;

    //
    // Now query the ACPLT/KS server with the help of the server object. The
    // server object will handle the request accordingly to whatever kind
    // of ACPLT/KS server it is connected with.
    //
    bool ok = myServer->getPP(av_module,
                              params,
			      *result);

    if ( !ok ) {
	//
        // The GetPP service request failed on the communication level.
	// In this case, the server object supplies more precise information
	// about the cause of the failure.
	//
	delete result;
        _last_result = myServer->getLastResult();
        return false;
    }
    //
    // The request succeeded at least at the communication level. So
    // check whether the request itself was granted by the ACPLT/KS
    // server, otherwise bail out with the error code returned by the
    // ACPLT/KS server.
    //
    _last_result = result->result;
    if ( _last_result != KS_ERR_OK ) {
	delete result;
        return false;
    }
    return true;
} // KscDomain::getChildPPUpdate


//////////////////////////////////////////////////////////////////////
// Return an iterator for iterating over the children of this domain
// communication object which adhere to the given type and name masks.
//
KscChildIterator *
KscDomain::newChildIterator(KS_OBJ_TYPE typeMask, KsString nameMask)
{
    KsGetPPResult *result = 0;

    if ( getChildPPUpdate(typeMask, nameMask, result) ) {
	if ( result ) {
	    //
	    // I'm really overchecking here, but you should already
	    // know by now: defensive programming! It has saved my day
	    // more than once. Yes!
	    //
	    ChildIterator *it = new ChildIterator(*result);
	    if ( it ) {
		return it;
	    }
	    delete result;
	}
	_last_result = KS_ERR_GENERIC;
	return 0;
    }
    //
    // _last_result has already been set by getChildPPUpdate(), so
    // we can't set any better indication here. The result object has
    // also been destroyed automatically by getChildPPUpdate(), if this
    // function had created one before it failed.
    //
    return 0;
} // KscDomain::newChildIterator


//////////////////////////////////////////////////////////////////////
// class KscDomain::ChildIterator
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// This child iterator is an ordinary list iterator, which iterates
// over handles for projected properties. But in addition to such an
// ordinary PltListIterator, it also takes the ownership of a GetPP
// result object. It will be freed lateron together with this
// iterator object. The advantage is, that we can save an (possibly
// expensive) list duplication only to get ownership of the list
// of the projected properties of the children.
//
KscDomain::ChildIterator::ChildIterator(KsGetPPResult &getPPResult)
: PltListIterator<KsProjPropsHandle>(getPPResult.items),
  _getPP_result(getPPResult)
{} // KscDomain::ChildIterator::ChildIterator


//////////////////////////////////////////////////////////////////////
// If a child iterator is destroyed, then the result object must be
// destroyed too. Remember that the result object contained the list
// we have been iterating over.
//
KscDomain::ChildIterator::~ChildIterator()
{
    delete &_getPP_result;
} // KscDomain::ChildIterator::~ChildIterator


//////////////////////////////////////////////////////////////////////
// class KscVariable
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Query the projected properties of the variable object (communica-
// tion object of type variable) within the ACPLT/KS server this
// object refers to.
//
bool
KscVariable::getProjPropsUpdate()
{
    //
    // Just to make sure the caller is not trying (once) again to read
    // a variable with a bad resource locator.
    //
    if ( !hasValidPath() ) {
        _last_result = KS_ERR_MALFORMEDPATH;
        return false;
    }

    //
    // Get your hands on the server object which is responsible for the
    // communication. If we get back a null pointer instead, then for
    // some reason during construction of this object, no server object
    // could be created. Because we can't yet determine what went wrong
    // we just return a generic error indication. If you want to know
    // exactly what happened, then you must immediately call getLastResult()
    // after you've created a communication object.
    //
    KscServerBase *myServer = getServer();
    if( !myServer ) {
        _last_result = KS_ERR_GENERIC;
        return false;
    }

    //
    // Set up the service parameters. We ask exactly for this variable
    // communication object within the server. Note that not all ACPLT/KS
    // server might support the GetPP service on all the variables they
    // provide.
    //
    KsGetPPParams params;
    KsGetPPResult result;

    params.path = path.getPathOnly();
    params.type_mask = KS_OT_VARIABLE;
    params.name_mask = path.getName();

    //
    // Now query the ACPLT/KS server with the help of the server object. The
    // server object will handle the request accordingly to whatever kind
    // of ACPLT/KS server it is connected with.
    //
    bool ok = myServer->getPP(av_module,
                              params,
			      result);

    if ( ok ) {
	//
	// The request succeeded at least at the communication level. So
	// check whether the request itself was granted by the ACPLT/KS
	// server.
	//
        if ( result.result == KS_ERR_OK ) {
	    //
            // Yes, the request was granted. Now check whether we've got
	    // back the projected properties of exactly *ONE* variable.
	    // Otherwise something got really mixed up. Maybe some joker
	    // included a wildcard character in the resource locator...
            //
            if ( result.items.size() == 1 ) {
                KsProjPropsHandle hpp = result.items.removeFirst();
                if ( hpp ) {
                    if ( hpp->xdrTypeCode() == KS_OT_VARIABLE ) {
                        proj_props = *(KsVarProjProps *)hpp.getPtr();
			_last_result = KS_ERR_OK;
                        return true;
                    } else {
			//
                        // The resource locator of this variable does not
			// refer to a variable object but instead to some
			// other communication object within the server.
			// Dear programmer, you've messed it up!
			//
                        _last_result = KS_ERR_TYPEMISMATCH;
                    }
                } else {
		    //
                    // An unbound handle, so I presume: out of memory...
		    //
                    _last_result = KS_ERR_GENERIC;
                }
            } else {
		//
		// Either the ACPLT/KS server does not support the GetPP
		// on this particular variable, or there was a wildcard
		// in the variable's name. So bail out with an error.
		//
                _last_result = KS_ERR_GENERIC;
            }
        } else {
	    //
	    // The request was *NOT* granted by the ACPLT/KS server. The
	    // service reply contains the reason for the failure, so we
	    // return this through the "last result" member variable. One
	    // prominent reason for failing here is that the ACPLT/KS does
	    // not support the GetPP service on this particular variable.
	    //
	    _last_result = result.result;
	}
    } else {
	//
        // The GetPP service request failed on the communication level.
	// In this case, the server object supplies more precise information
	// about the cause of the failure.
	//
        _last_result = myServer->getLastResult();
    }

    return false;
} // KscVariable::getProjPropsUpdate


//////////////////////////////////////////////////////////////////////
// Set the projected properties of this variable object. This one is
// definetly *UNSUPPORTED*, so never, NEVER, **NEVER** use it! Also
// it's of no use (yet).
//
bool
KscVariable::setProjProps(KsProjPropsHandle hpp)
{
    if ( hpp && (hpp->xdrTypeCode() == typeCode()) ) {
        proj_props = *(KsVarProjProps *) hpp.getPtr();
        return true;
    }
    return false;
} // KscVariable::setProjProps


//////////////////////////////////////////////////////////////////////
// Query the variable object (communication object of type variable)
// within the ACPLT/KS server this object refers to.
//
bool
KscVariable::getUpdate() 
{
    //
    // Just to make sure the caller is not trying (once) again to read
    // a variable with a bad resource locator.
    //
    if ( !hasValidPath() ) {
        _last_result = KS_ERR_MALFORMEDPATH;
        return false;
    }

    //
    // Get your hands on the server object which is responsible for the
    // communication. If we get back a null pointer instead, then for
    // some reason during construction of this object, no server object
    // could be created. Because we can't yet determine what went wrong
    // we just return a generic error indication. If you want to know
    // exactly what happened, then you must immediately call getLastResult()
    // after you've created a communication object.
    //
    KscServerBase *myServer = getServer();
    if( !myServer ) {
        _last_result = KS_ERR_GENERIC;
        return false;
    }

    //
    // Set up the service parameters. We ask only for exactly one variable,
    // so we expect only one result.
    //
    KsGetVarParams params(1);
    params.identifiers[0] = path.getPathAndName();

    KsGetVarResult result(1);

    //
    // Now query the ACPLT/KS server with the help of the server object. The
    // server object will handle the request accordingly to whatever kind
    // of ACPLT/KS server it is connected with.
    //
    bool ok = myServer->getVar(av_module,
                               params, 
                               result);

    if ( ok ) {
	//
	// The request succeeded at least at the communication level. So
	// check whether the request itself was granted by the ACPLT/KS
	// server.
	//
        if( result.result == KS_ERR_OK )  {
	    //
            // Yes, the request was granted. Now check whether we've got
	    // back a value for this variable or whether the variable
	    // could not be queried for some reason.
            //
            KsGetVarItemResult *pitem = &(result.items[0]);
            
            if ( pitem->result == KS_ERR_OK ) {
                if ( pitem->item->xdrTypeCode() == KS_OT_VARIABLE ) {
		    //
                    // Everything went okay and we've got a value. Now
		    // it's up to the caller what to do with it...
		    //
                    curr_props = *(KsVarCurrProps *) pitem->item.getPtr(); 
                    fDirty = false;
		    _last_result = KS_ERR_OK;
                    return true;
                } else {
		    //
                    // Type mismatch: asked for variable but got some other
		    // value for another kind of communication object. Note
		    // that this is pure defensive programming. Per
		    // definition, the GetVar service can't return values
		    // for communication objects other than variables.
		    //
                    _last_result = KS_ERR_TYPEMISMATCH;
                }
            } else {
		//
		// No, the variable could not be queried. Return the reason
		// as given in the service reply.
		//
                _last_result = pitem->result;
            }
        } else {
	    //
	    // The request was *NOT* granted by the ACPLT/KS server. The
	    // service reply contains the reason for the failure, so we
	    // return this through the "last result" member variable.
	    //
	    _last_result = result.result;
	}
    } else {
	//
        // The GetVar service request failed on the communication level.
	// In this case, the server object supplies more precise information
	// about the cause of the failure.
	//
        _last_result = myServer->getLastResult();
    }

    return false;
} // KscVariable::getUpdate


//////////////////////////////////////////////////////////////////////
// Write the value (aka current properties) of this variable object
// back into the communication object (variable) within the ACPLT/KS
// server this object refers to.
//
bool
KscVariable::setUpdate()
{
    //
    // Just to make sure the caller is not trying (once) again to write
    // a variable with a bad resource locator.
    //
    if ( !hasValidPath() ) {
        _last_result = KS_ERR_MALFORMEDPATH;
        return false;
    }

    //
    // Get your hands on the server object which is responsible for the
    // communication. If we get back a null pointer instead, then for
    // some reason during construction of this object, no server object
    // could be created. Because we can't yet determine what went wrong
    // we just return a generic error indication. If you want to know
    // exactly what happened, then you must immediately call getLastResult()
    // after you've created a communication object.
    //
    KscServerBase *myServer = getServer();
    if( !myServer ) {
        _last_result = KS_ERR_GENERIC;
        return false;
    }

    //
    // Set up the service parameters. We deal only with exactly one variable,
    // so we expect only one result.
    //
    KsSetVarParams params(1);
    params.items[0].path_and_name = path.getPathAndName();
    params.items[0].curr_props = 
        KsCurrPropsHandle(&curr_props, KsOsUnmanaged);

    KsSetVarResult result(1);

    //
    // Now query the ACPLT/KS server with the help of the server object. The
    // server object will handle the request accordingly to whatever kind
    // of ACPLT/KS server it is connected with.
    //
    bool ok = myServer->setVar(av_module,
                               params,
                               result);

    if ( ok ) { 
	//
	// The request succeeded at least at the communication level. So
	// check whether the request itself was granted by the ACPLT/KS
	// server.
	//
        if ( result.result == KS_ERR_OK ) {
	    //
            // Yes, the request was granted. Now check whether we could
	    // write the value into the (real) variable object inside the
	    // ACPLT/KS server.
            //
            if ( result.results[0].result == KS_ERR_OK ) {
                fDirty = false;
		_last_result = KS_ERR_OK;
                return true;
            } else {
		//
		// No, the variable could not be set. Return the reason
		// as given in the service reply.
		//
                _last_result = result.results[0].result;
            }
        } else {
	    //
	    // The request was *NOT* granted by the ACPLT/KS server. The
	    // service reply contains the reason for the failure, so we
	    // return this through the "last result" member variable.
	    //
	    _last_result = result.result;
        }
    } else {
	//
        // The SetVar service request failed on the communication level.
	// In this case, the server object supplies more precise information
	// about the cause of the failure.
	//
        _last_result = myServer->getLastResult();
    }

    return false;
} // KscVariable::setUpdate


//////////////////////////////////////////////////////////////////////
// Okay, retrieving the value handle is easy. Just return the handle
// contained in the current properties object.
// TODO: markusj, isn't this a candidate for inlining, is it?
//
KsValueHandle
KscVariable::getValue() const
{
    return curr_props.value; 
} // KscVariable::getValue


//////////////////////////////////////////////////////////////////////
// Really, setting the current properties has not much ado about it.
// Just copy the structure, er class, and set the dirty flag, so we
// know that the "value" of the variable object has been changed.
//
bool
KscVariable::setCurrProps(KsVarCurrProps &cp)
{
    curr_props = cp;

    fDirty = true;

    return true;
} // KscVariable::setCurrProps

//////////////////////////////////////////////////////////////////////
// EOF commobject.cpp
//////////////////////////////////////////////////////////////////////
