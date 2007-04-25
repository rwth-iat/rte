/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/simpleserver.cpp,v 1.34 2007-04-25 12:57:21 martin Exp $ */
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
/* Author: Martin Kneissl <martin@plt.rwth-aachen.de> */
/* v1+ stuff: Harald Albrecht <harald@plt.rwth-aachen.de> */


#include "ks/simpleserver.h"
#include "ks/path.h"
#include "ks/conversions.h"
#include "plt/log.h"

#include "ks/svrsimpleobjects.h"


#if PLT_USE_BUFFERED_STREAMS

#include "ks/xdrmemstream.h"

// ---------------------------------------------------------------------------
// When a server uses dynamic XDR memory streams, then you´ll find in
// its /vendor/modules/acpltks domain a few variables with statistics
// information about the communication layer -- like number of open
// transports, memory footprint,...  All these variables are
// represented by instances of the class KssIoStatisticsVariable below
// which handles getting always up-to-date data...
//
class KssIoStatisticsVariable : public KssSimpleVariable
{
public:
    enum StatisticType {
    	DLAS_CONNECTION_COUNT = 0,
	DLAS_IO_ERROR_COUNT,
	DLAS_IO_RX_ERROR_COUNT,
	DLAS_IO_TX_ERROR_COUNT,
	DLAS_POOL_SIZE,
	DLAS_POOL_USED,
	DLAS_THIS_IS_THE_END
    };
	
    KssIoStatisticsVariable(StatisticType type);
    
    virtual KsValueHandle getValue() const;
    virtual KsTime        getTime() const;

protected:
    StatisticType _stat_type;
}; // class KssIoStatisticsVariable


// ---------------------------------------------------------------------------
// List of statistical variables: name, a comment and the technical unit.
//
static struct {
    const char *_name;
    const char *_comment;
    const char *_unit;
} _statisticsVariables[] = {
    { "transport_count", "number of open transports", "" },
    { "io_error_count", "number of generic I/O errors during communication", "" },
    { "io_rx_error_count", "number of receiving errors during communication", "" },
    { "io_tx_error_count", "number of sending errors during communication", "" },
    { "transport_pool_size", "size of memory pool for transports", "bytes" },
    { "transport_pool_used", "memory used for active transports", "bytes" }
}; // _statisticsVariables


// ---------------------------------------------------------------------------
// Construct a statistical variable and connect it to a particular statistics
// parameter of the communication layer.
//
KssIoStatisticsVariable::KssIoStatisticsVariable(StatisticType type)
    : KssSimpleVariable(_statisticsVariables[type]._name,
	                KsTime::now(),
	                _statisticsVariables[type]._comment),
      _stat_type(type)
{
    setState(KS_ST_GOOD);
    setTechUnit(KsString(_statisticsVariables[type]._unit));
    lock();
} // KssIoStatisticsVariable::KssIoStatisticsVariable


// ---------------------------------------------------------------------------
// Retrieve the current value for a statistical variable. This eventually maps
// to calling the appropriate information function.
//
KsValueHandle KssIoStatisticsVariable::getValue() const
{
    KsValue *pVal = 0;
    
    switch ( _stat_type ) {
    case DLAS_CONNECTION_COUNT:
	pVal = new KsIntValue(
	    KsServerBase::getServerObject().getConnectionManager()->
	        getConnectionCount());
	break;
    case DLAS_IO_ERROR_COUNT:
	pVal = new KsIntValue(
	    KsServerBase::getServerObject().getConnectionManager()->
	        getIoErrorCount());
	break;
    case DLAS_IO_RX_ERROR_COUNT:
	pVal = new KsIntValue(
	    KsServerBase::getServerObject().getConnectionManager()->
	        getIoRxErrorCount());
	break;
    case DLAS_IO_TX_ERROR_COUNT:
	pVal = new KsIntValue(
	    KsServerBase::getServerObject().getConnectionManager()->
	        getIoTxErrorCount());
	break;
    case DLAS_POOL_SIZE:
	{
	    u_int total, freepool;
	    xdrmemstream_getusage(&total, &freepool);
	    pVal = new KsIntValue(total);
    	}
	break;
    case DLAS_POOL_USED:
	{
	    u_int total, freepool;
	    xdrmemstream_getusage(&total, &freepool);
	    pVal = new KsIntValue(total - freepool);
    	}
	break;
    default:
	break;
    }
    return KsValueHandle(pVal, KsOsNew);
} // KssIoStatisticsVariable::getValue


// ---------------------------------------------------------------------------
// Always make a statistical variable using the current time for the timestamp
// of its value.
//
KsTime KssIoStatisticsVariable::getTime() const
{
    return KsTime::now();
} // KssIoStatisticsVariable::getTime


// ---------------------------------------------------------------------------
// Set up the statistical variables below the /vendor
// domain. Currently, we only populate the C++ Communication Library
// module branch in /vendor/modules/acpltks
//
bool KsSimpleServer::initStatistics()
{
    KsPath modks("/vendor/modules/acpltks");
    int i;

    //
    // Now create all the information variables below the "acpltks"
    // module info domain. Here we watch for creation problem when
    // constructing all the variables, and bail out in case of error.
    //
    for ( i = 0; i < KssIoStatisticsVariable::DLAS_THIS_IS_THE_END; ++i ) {
    	KssCommObjectHandle h(
		new KssIoStatisticsVariable(
		    (KssIoStatisticsVariable::StatisticType) i), KsOsNew);
	if ( !h || !addCommObject(modks, h) ) {
	    return false;
	}
    }
    return true;
} // KsSimpleServer::initStatistics

#endif


// ---------------------------------------------------------------------------
// Note that the virtual base class KsServerBase will first set the
// member variable _sock_port to KS_ANYPORT before we'll here reset
// it to whatever the programmer wants. We do it this way because then
// the compiler can fall back to the default constructor from
// KsServerBase without breaking existing code.
//
KsSimpleServer::KsSimpleServer(int port)
: _root_domain("/")
{
    if ( port != KS_ANYPORT ) {
        //
        // Some day I'll be after those who'd invented virtual base
        // classes without defining the order of constructors called
        // for derived classes. But on the other side, why did I ever
        // relied on that?! Okay -- I'm stupid, yeah. =:)
        //
        _sock_port = port;
    }
} // KsSimpleServer::KsSimpleServer


// ---------------------------------------------------------------------------
// An ACPLT/KS client requests to read several communication variables.
//
void 
KsSimpleServer::getVar(KsAvTicket &ticket,
                       const KsGetVarParams &params,
                       KsGetVarResult &result)
{
    size_t reqsz = params.identifiers.size();
    PltArray<KsPath> paths(reqsz);
    PltArray<KS_RESULT> pathres(reqsz);
    
    if (    (paths.size() == reqsz)
         && (pathres.size() == reqsz) ) {
	//
        // Allocation ok. So we can now resolve relative paths into
	// absolute ones, decoding encoded characters on the fly.
	//
        KsPath::resolvePaths(params.identifiers, paths, pathres);
	//
	// Simply retrieve the value of each variable separately, without
	// any optimization. If this is necessary, override getVar() in
	// your derived server class.
	//
        for ( size_t i = 0; i < reqsz; ++i ) {
	    //
	    // Only try to retrieve the variable if we have a syntactically
	    // valid path for it.
	    //
            if ( pathres[i] == KS_ERR_OK ) {
#if 0
		cerr << "[" << i << "]: " 
		     << (const char *) params.identifiers[i] << endl;
		cerr << "    ";
		for ( int ii = 0; ii < paths[i].size(); ++ii ) {
		    cerr << "[" << (const char *) (paths[i])[ii] << "] ";
		}
		cerr << endl;
#endif
                getVarItem(ticket, paths[i], result.items[i]);
#if 0
		cerr << "   Result: " << result.items[i].result << endl;
#endif
            } else {
                result.items[i].result = pathres[i];
            }
        }
	//
	// The service could be satisfied (this does not mean that we
	// could successfully do our job on the individual variables)
	//
        result.result = KS_ERR_OK;
    } else {
        //
	// Allocation failed. So bail out with a more or less appropriate
	// error indication: the complete service could not be worked on.
	//
        result.result = KS_ERR_GENERIC;
    }
} // KsSimpleServer::getVar


// ---------------------------------------------------------------------------
// An ACPLT/KS client requests to write several communication variables.
//
void 
KsSimpleServer::setVar(KsAvTicket &ticket,
                       const KsSetVarParams &params,
                       KsSetVarResult &result)
{
    size_t reqsz=params.items.size();
    PltArray<KsString> ids(reqsz);
    PltArray<KsPath> paths(reqsz);
    PltArray<KS_RESULT> pathres(reqsz);
    
    if (    (paths.size() == reqsz)
         && (ids.size() == reqsz)
         && (pathres.size() == reqsz) ) {
	//
        // Allocation ok. So we can now resolve relative paths into
	// absolute ones, decoding encoded characters on the fly.
	//
        for ( size_t j = 0; j < reqsz; ++j ) {
            ids[j] = params.items[j].path_and_name;
        }
        KsPath::resolvePaths(ids, paths, pathres);
	//
	// Simply set the value of each variable separately, without
	// any optimization. If this is necessary, override setVar() in
	// your derived server class.
	//
        for ( size_t i = 0; i < reqsz; ++i ) {
	    //
	    // Only try to set the variable if we have a syntactically
	    // valid path for it.
	    //
            if ( pathres[i] == KS_ERR_OK ) {
                setVarItem(ticket, 
                           paths[i], 
                           params.items[i].curr_props,
                           result.results[i]);
            } else {
                result.results[i].result = pathres[i];
            }
        }
	//
	// The service could be satisfied (this does not mean that we
	// could successfully do our job on the individual variables)
	//
        result.result = KS_ERR_OK;
    } else {
        //
	// Allocation failed. So bail out with a more or less appropriate
	// error indication: the complete service could not be worked on.
	//
        result.result = KS_ERR_GENERIC;
    }
} // KsSimpleServer::setVar


// ---------------------------------------------------------------------------
// Retrieve the value for exactly one variable. This assumes that it does not
// matter whether one or several variables should be read and performance
// can not be optimized.
//
void
KsSimpleServer::getVarItem(KsAvTicket &ticket, 
                           const KsPath &path,
                           KsGetVarItemResult &result)
{
    PLT_PRECONDITION(path.isValid() && path.isAbsolute());
    
    if ( ticket.canReadVar(KsString(PltString(path))) ) {
	//
        // Access granted. Proceed. Now we try to get our hands on the
	// communication object addressed by the path. Now this is
	// simple, just ask the root domain for the descendant.
	//
        KssCommObjectHandle hobj(_root_domain.getChildByPath(path));
        if ( hobj ) {
            //
	    // Unfortunately, some C++ compilers don't support true RTTI,
            // so downcasting to a particular class isn't always working
            // properly when multiple inheritance comes into play. So we
	    // need to give compilers a helping hand. Sigh.
	    //
	    KssCurrPropsService *pobj = 0;
	    switch ( hobj->typeCode() ) {
	    case KS_OT_VARIABLE:
		pobj = (KssVariable*) hobj.getPtr();
		break;
	    case KS_OT_LINK:
		pobj = (KssLink*) hobj.getPtr();
		break;
	    default:
		break; // all other object classes are invalid.
	    }
            if ( pobj ) {
                //
                // Hey, we have found something that might give us some
		// current properties.
		//
		result.result = pobj->getCurrProps(result.item);
//##FIXME                result.item = pobj->getCurrProps();
//                result.result = result.item ? KS_ERR_OK : KS_ERR_GENERIC;
            } else {
		//
                // An object that do not want to give us its current
		// properties -- mainly because it doesn't have any.
		//
                result.result = KS_ERR_BADTYPE;
            }
        } else {
	    //
            // No such object with this path.
	    //
            result.result = KS_ERR_BADPATH;
        }
    } else {
	//
        // Access denied -- for whatever reason, which we do not want
	// to leak to the client. No, this is meant seriously.
	//
        result.result = KS_ERR_NOACCESS;
    }
} // KsSimpleServer::getVarItem


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void
KsSimpleServer::setVarItem(KsAvTicket &ticket, 
                           const KsPath & path,
                           const KsCurrPropsHandle & curr_props,
                           KsResult & result)
{
    PLT_PRECONDITION(path.isValid() && path.isAbsolute());
    if ( ticket.canWriteVar(KsString(PltString(path))) ) {
        // Access okay.
        KssCommObjectHandle hobj(_root_domain.getChildByPath(path));
        if ( hobj ) {
            //
	    // Unfortunately, some C++ compilers don't support true RTTI,
            // so downcasting to a particular class isn't always working
            // properly when multiple inheritance comes into play. So we
	    // need to give compilers a helping hand. Sigh.
	    //
	    KssCurrPropsService *pobj = 0;
	    switch ( hobj->typeCode() ) {
	    case KS_OT_VARIABLE:
		pobj = (KssVariable*) hobj.getPtr();
		break;
	    case KS_OT_LINK:
		//
		// Although you can't write new values to links, we allow
                // the attempt to do so here. It will be denied through the
                // setCurrProps() method nevertheless lateron.
                //
		pobj = (KssLink*) hobj.getPtr();
		break;
	    default:
		break; // all other object classes are invalid.
	    }
            if ( pobj ) {
                //
                // Hey, we have found something that might want take some
		// current properties from us.
		//
                result.result = pobj->setCurrProps(curr_props);
            } else {
		//
                // Nothing that wants to get some current properties.
		//
                result.result = KS_ERR_BADTYPE;
            }
        } else {
            // No such object.
            result.result = KS_ERR_BADPATH;
        }
    } else {
        // Access denied.
        result.result = KS_ERR_NOACCESS;
    }
}


// ---------------------------------------------------------------------------
// Retrieve the engineered properties of either a communication object itself
// or its children, depending on the path and mask specified.
//
void
KsSimpleServer::getEPOfObject(KssCommObject *pobj,
                              const PltString &prefix,
                              KsAvTicket &ticket,
                              const KsGetEPParams &params,
                              KsGetEPResult &result)
{
    PLT_PRECONDITION( pobj );
 
    if ( params.name_mask == "" ) {
	//
        // We are being asked about the engineered props of the object itself.
	// Just add them to the end of the list of engineered props the result
	// already has. Well, the list should be empty when we came here.
        //
        KsEngPropsHandle h(pobj->getEP());
        if ( h && result.items.addLast(h) ) {
            result.result = KS_ERR_OK;
        } else {
            result.result = KS_ERR_GENERIC;
        }
    } else {
	//
        // Make sure that the object for which the children are to be
	// queried can have children. For instance, you can't ask a variable
        // for children, because it'll never can have children. For objects
	// which can be queried for its children, get the pointer to the
        // apropriate interface. Once again, we would need true RTTI support
        // with all C++ compilers, but... so we need to do the downcast
        // ourselves very carefully -- or bang!
        //
        KssChildrenService *pcs = 0;
	switch ( pobj->typeCode() ) {
	case KS_OT_DOMAIN:
	    pcs = (KssDomain *) pobj;
	    break;
	case KS_OT_LINK:
	    pcs = (KssLink *) pobj;
	    break;
        case KS_OT_HISTORY:
	    pcs = (KssHistory *) pobj;
	    break;
	default:
	    result.result = KS_ERR_BADPATH;
	    return;
	}
	//
        // Convert mask. FIXME
	//
        KsString mask;
        result.result = ksStringFromPercent(params.name_mask, mask);
        if ( result.result != KS_ERR_OK ) {
	    return;
	}
	//
        // Iterate over the children of a communication object and add
	// the engineered properties of each child to the result list.
	//
        KssChildIterator *pit =
	       PLT_RETTYPE_CAST((KssChildIterator *))
	       pcs->newMaskedIterator(mask, params.type_mask);
	if ( pit ) {
	    //
	    // We got an iterator. Just use it(tm).
	    //
	    for ( KssChildIterator &it = *pit; it; ++it ) {
		if ( *it ) {
		    //
		    // Check that the child is visible before adding it
		    // to the child list returned as the service's result.
		    //
                    PltString childname(prefix,
					(*it)->getIdentifier());
		    if ( ticket.isVisible(childname) ) {
			//
			// Ask for the child's engineered properties. Make
			// sure that the access mode can't be better than
			// what the A/V ticket allows at this point. Note
			// that we must preserve the other flags here.
			//
                        KsEngPropsHandle hep = (*it)->getEP();
			if ( hep ) {
			    hep->access_mode &= 
				ticket.getAccess(childname) |
				~KS_AC_READWRITE;
			    hep->identifier = 
				ksStringToPercent(hep->identifier);
			    result.items.addLast(hep);
			} else {
			    //
			    // Ignore any error, thus making the child
			    // effectively invisible.
			    //
			}
		    } else {
			//
			// Not visible, so do nothing(tm).
			//
                    }
		} else {
		    // null handle, log error
                    PltLog::Error("Child iterator returned null handle");
		}
	    } // for
            delete pit; // get rid of iterator...
        } else {
	    // no iterator: ignore error
        }
    }               
} // KsSimpleServer::getEPOfObject


/////////////////////////////////////////////////////////////////////////////

void
KsSimpleServer::getEP(KsAvTicket &ticket, 
                      const KsGetEPParams & params,
                      KsGetEPResult & result) 
{
    KsPath path(params.path);
    PltString prefix;

    if( path.isValid() && path.isAbsolute() ) {
        KS_RESULT convres = path.decodePercents();
        if( convres == KS_ERR_OK ) {
	    //
            // Path is okay, so we can now proceed -- if the object is
	    // at least visible.
	    //
            if ( ticket.isVisible(KsString(path)) ) {
                KssCommObjectHandle hc;
                KssCommObject *pd = 0;
                if ( params.path == "/" ) {
                    prefix = params.path;
		    //
                    // The root must be handled specially...
		    //
                    pd = &_root_domain;
                } else {
		    //
                    //
                    //
                    prefix = PltString(PltString(path), "/");
                    hc = _root_domain.getChildByPath(path);
                    if ( hc ) {
			pd = hc.getPtr();
                    }
                }
                if ( pd ) {
                    getEPOfObject(pd, prefix, ticket, params, result);
                } else {
                    // not a domain or no such child
                    result.result = KS_ERR_BADPATH;
                }
            } else {
                // domain invisible
                result.result = KS_ERR_NOACCESS; // TODO or BADPATH?
            }
        } else {
            // failed to convert path
            result.result = convres;
        }
    } else {
        // Path syntax error
        result.result = KS_ERR_BADNAME;
    }
}

//////////////////////////////////////////////////////////////////////

bool
KsSimpleServer::addCommObject(const KsPath & dompath,
                              const KssCommObjectHandle & ho)
{
    PLT_PRECONDITION(dompath.isValid() && dompath.isAbsolute());
    KssCommObjectHandle hd;
    if (dompath.isSingle() && dompath[0] == "") {
        hd.bindTo(&_root_domain, KsOsUnmanaged);
    } else {
        hd = _root_domain.getChildByPath(dompath);
    }
    if (hd) {
        //
        // found an object, is it a simple domain?
        //
        KssSimpleDomain * pd = 
            PLT_DYNAMIC_PCAST(KssSimpleDomain, hd.getPtr());
        if (pd) {
            //
            // It is a simple domain. Add the child. But only add it
            // if a child with the same name does not exist yet.
            //
            if ( !pd->getChildById(ho->getIdentifier()) ) {
                return pd->addChild(ho);
            } else {
                return false;
            }
        } else {
            //
            // Not a simple domain.
            //
            return false;
        }
    } else {
        //
        // object not found or other error.
        //
        return false;
    }
}

/////////////////////////////////////////////////////////////////////////////

bool
KsSimpleServer::removeCommObject(const KsPath &dompath,
                                 const KsString &id)
{
    PLT_PRECONDITION(dompath.isValid() && dompath.isAbsolute());

    KssCommObjectHandle hd;
    //
    // Search for object at dompath, 
    // root domain needs special treatment.
    //
    if(dompath.isSingle() && dompath[0] == "") {
        hd.bindTo(&_root_domain, KsOsUnmanaged);
    } else {
        hd = _root_domain.getChildByPath(dompath);
    }
 
    //
    // If a domain was found try to remove the child id.
    //
    if( hd ) {
        KssSimpleDomain *dom = 
            PLT_DYNAMIC_PCAST(KssSimpleDomain, hd.getPtr());

        if( dom ) {
            return dom->removeChild(id) ? true : false;
        } 
    }

    return false;
}

//////////////////////////////////////////////////////////////////////
        
bool
KsSimpleServer::addDomain(const KsPath & dompath,
                                const KsString & id,
                                const KsString & comment)
{
    PLT_PRECONDITION(dompath.isValid() && dompath.isAbsolute());
    KssSimpleDomain * pd = new KssSimpleDomain(id);
    if (pd) {
        pd->setComment(comment);
        KssCommObjectHandle ho(pd, KsOsNew);
        if (ho) {
            return addCommObject(dompath, ho);
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
        
bool
KsSimpleServer::addStringVar(const KsPath & dompath,
                                   const KsString & id,
                                   const KsString & str,
                                   const KsString & comment,
                                   bool lock)
{
    PLT_PRECONDITION(dompath.isValid() && dompath.isAbsolute());
    KsValueHandle hv(new KsStringValue(str), KsOsNew);
    if (hv) {
        KssSimpleVariable *po = new KssSimpleVariable(id);
        if (po) {
            po->setValue(hv);
            po->setState(KS_ST_GOOD);
            po->setComment(comment);
            if (lock) po->lock();
            KssCommObjectHandle ho(po, KsOsNew);
            if (ho) {
                return addCommObject(dompath, ho);
            }
        }
    }
    return false;
}


#if PLT_SYSTEM_LINUX || PLT_SYSTEM_HPUX || PLT_SYSTEM_SOLARIS \
    || PLT_SYSTEM_FREEBSD || PLT_SYSTEM_IRIX
#include <sys/utsname.h>
#endif

// ---------------------------------------------------------------------------
// Determine the operating system
//
KsString
KsSimpleServer::getOsIdent()
{
#if PLT_SYSTEM_NT

    OSVERSIONINFO osinfo;

    osinfo.dwOSVersionInfoSize = sizeof(osinfo);
    if ( !GetVersionEx(&osinfo) ) {
    	return "unidentified Win32 \"operating system\"";
    }
    KsString name;
    switch ( osinfo.dwPlatformId ) {
    case VER_PLATFORM_WIN32_NT:
    	name = "WinNT";
    	break;
    case VER_PLATFORM_WIN32_WINDOWS:
    	name = "Windows 95, 98, ME, YOU, WHATEVER";
    	break;
    default:
        name = "unidentified Windows \"operating system\"";
    }
    return name + " " + KsString::fromInt(osinfo.dwMajorVersion) + "."
           + KsString::fromInt(osinfo.dwMinorVersion) + "."
           + KsString::fromInt(osinfo.dwBuildNumber)
           + " " + osinfo.szCSDVersion;
	
#elif PLT_SYSTEM_OPENVMS
    return "OpenVMS";
#elif PLT_SYSTEM_LINUX || PLT_SYSTEM_HPUX || PLT_SYSTEM_SOLARIS \
    || PLT_SYSTEM_FREEBSD || PLT_SYSTEM_IRIX

    struct utsname id;
    if ( uname(&id) < 0 ) {
    	return "unidentified Un*x-like operating system";
    }
    return KsString(id.sysname) + " " + id.release + " " + id.version + " "
           + id.machine
           + " (on " + id.nodename + ")";
    
#else
    return "unidentified operating system";
#endif
}

// ---------------------------------------------------------------------------
// Setup the "/vendor" branch of the ACPLT/KS server and populate it with
// some variables and domains containing more or less useful information about
// the server and other things in the universe.
//
bool
KsSimpleServer::initVendorTree()
{
    static KssTimeNowVariable server_time("server_time",
                                          KsTime::now(),
                                          "current server time");
    KssCommObjectHandle server_time_handle(&server_time, KsOsUnmanaged);
    
    static KssSimpleVariable startup_time("startup_time",
	                                  KsTime::now(),
	                                  "server startup time");
    startup_time.setValue(new KsTimeValue(KsTime::now()));
    startup_time.setTechUnit("UTC");
    startup_time.setState(KS_ST_GOOD);
    startup_time.lock();
    KssCommObjectHandle startup_time_handle(&startup_time, KsOsUnmanaged);
    //
    // "Man soll dem Esel der da drischt nicht das Maul verbinden."
    //
    static KssSimpleVariable fame("ks_comm_lib_fame", KsTime::now(),
				  "the people behind the ACPLT/KS "
	                          "C++ Communication Library");
    KsStringVecValue *fame_val = new KsStringVecValue(3);
    (*fame_val)[0] = "Harald \"Harry Hirsch\" Albrecht";
    (*fame_val)[1] = "Markus Juergens";
    (*fame_val)[2] = "Martin \"Graph Grammar\" Kneissl";
    fame.setValue(fame_val);
    fame.lock();
    KssCommObjectHandle fame_handle(&fame, KsOsUnmanaged);
    //
    // Create the "/vendor" domain but allow that it already exists.
    //
    KsPath vendor("/vendor");
    KsPath modules("/vendor/modules");
    KsPath modks("/vendor/modules/acpltks");
    
    //
    // Add some domains but ignore errors for now, as they might have already
    // been created earlier by someone, so we can just use them.
    //
    addDomain(KsPath("/"), "vendor",
              "vendor and server-specific information");
    addDomain(vendor, "extensions", "protocol extension information");
    addDomain(vendor, "modules", "information about server modules");
    addDomain(vendor, "specific", "additional vendor-specific information");
    
    //
    // But from here on we can not ignore any errors.
    //
    return
           addCommObject(vendor, server_time_handle)

        && addStringVar(vendor, "server_name",
                        getServerName(),
                        "name of server as registered with the ACPLT/KS Manager")

        && addStringVar(vendor, "server_version",
                        getServerVersion(), "server version string")

        && addStringVar(vendor, "server_description",
                        getServerDescription(), "short description of this ACPLT/KS server")

        && addStringVar(vendor, "name",
                        getVendorName(), "name of vendor of this ACPLT/KS server")

        && addStringVar(vendor, "server_os",
                        getOsIdent(), "operating system underlaying ACPLT/KS server")

        && addDomain(modules, "acpltks", "ACPLT/KS C++ Communication Library module")

        && addStringVar(modks, "ks_comm_lib_version",
                        KS_VERSION_STRING, "C++ Communication Library version string")

	&& addCommObject(modks, fame_handle)

#if PLT_USE_BUFFERED_STREAMS
    	&& initStatistics()
#endif

        && addCommObject(vendor, startup_time_handle);
} // KsSimpleServer::initVendorTree


/* End of simpleserver.cpp */
