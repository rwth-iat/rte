/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/stdconnectionmgr.cpp,v 1.1 2003-10-13 12:00:05 harald Exp $ */
/*
 * Copyright (c) 1996, 2003
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

/*
 * connectionmgr.cpp -- Implements a connection manager which can handle
 *                      connection objects which in turn encapsulate XDR
 *                      streams. The connection manager makes it possible to
 *                      use so-called buffered XDR streams, which decouple
 *                      RPC I/O from service handling and thus serialization
 *                      and deserialization. In addition, the connection
 *                      manager can also handle other kinds of connections.
 *
 * Written by Harald Albrecht <harald@plt.rwth-aachen.de>
 */

#include "ks/stdconnectionmgr.h"


//#define CNXDEBUG 1


#include <errno.h>

#if !PLT_SYSTEM_NT
#include <unistd.h>
#endif

#if !PLT_SYSTEM_OPENVMS && !PLT_COMPILER_CYGWIN && !PLT_COMPILER_MSVC
#include <values.h>
#else
#define MAXINT (((unsigned int) -1) >> 1)
#endif

#ifdef CNXDEBUG
#include <iostream.h>
#endif


typedef _KsConnectionItem * _pKsConnectionItem;

// ---------------------------------------------------------------------------
// Insert "this" node before the node given as a parameter to this method.
// Somehow the time has come for a doubly linked list to enter this project...
// This is gonna make someone really happy...!
//
void _PltDLinkedListNode::addBefore(_PltDLinkedListNode &n)
{
    _prev = n._prev;      n._prev      = this;
    _next = _prev->_next; _prev->_next = this;
} // _PltDLinkedListNode::addBefore


// ---------------------------------------------------------------------------
// Insert "this" node after the node given as a parameter to this method.
//
void _PltDLinkedListNode::addAfter(_PltDLinkedListNode &n)
{
    _next = n._next;      n._next      = this;
    _prev = _next->_prev; _next->_prev = this;
} // _PltDLinkedListNode::addBAfter


// ---------------------------------------------------------------------------
// Remove "this" node from the list it is currently put in.
//
void _PltDLinkedListNode::remove()
{
    _next->_prev = _prev; _prev->_next = _next;
    _next = _prev = this; // I'm pendantic...
} // _PltDLinkedListNode::remove


// ---------------------------------------------------------------------------
// When working with buffered streams, we're using a timer event to free up
// unused fragments from time to time. This way, memory from large requests or
// replies can be reclaimed not only by the transports but also by the C++
// objects. Typically, not all currently unused fragments (buffers) will be
// given back at once but rather we leave some spare for client requests to
// come. Someone should implement a sorted list of free fragments, so
// fragmentation could be minimized. But then, who's gonna do it?
//
class KsGarbageTimerEvent : public KsTimerEvent {
public:
    KsGarbageTimerEvent(unsigned long interval);
    virtual void trigger();

protected:
    unsigned long _interval;
}; // class KsGarbageTimerEvent


KsGarbageTimerEvent::KsGarbageTimerEvent(unsigned long interval)
    : KsTimerEvent(KsTime::now(interval)),
      _interval(interval)
{
} // KsGarbageTimerEvent::KsGarbageTimerEvent


void KsGarbageTimerEvent::trigger()
{
    xdrmemstream_freegarbage();
    _trigger_at = KsTime::now(_interval);
    KsConnectionManager::getConnectionManagerObject()->addTimerEvent(this);
} // KsGarbageTimerEvent::trigger


// ---------------------------------------------------------------------------
// Create and initialize (aka "construct") a connection manager object. This
// involves creating a lookup-table so we can easily find the connection
// object for a given file descriptor (this is what the select() call will
// give us as information). Unfortunately, Microsoft's file descriptor sets
// are different once again, so in that case we have to use a hash table...
//
// Check _is_ok to make sure the connection manager object could be ctor´ed
// successfully.
//
KsStdConnectionManager::KsStdConnectionManager()
    : _connection_count(0), _serviceable_count(0),
      _io_errors(0), _io_rx_errors(0), _io_tx_errors(0)
#if PLT_CNX_MGR_USE_HT
      , _hash_table(0), _hash_table_size(0), _hash_table_mask(0)
#endif
{
    //
    // Allocate the lookup-table, which is indexed by fds and contains
    // references to connection objects as well as some state stuff...
    //
    _fdset_size = FD_SETSIZE;
    if ( _fdset_size == 0 ) {
    	_fdset_size = 32; // I'm paranoid once more again...
    }
    _connections = new _KsConnectionItem[_fdset_size];
    FD_ZERO(&_readable_fdset);
    FD_ZERO(&_writeable_fdset);
    if ( !_connections ) {
	_is_ok = false;
    }
#if PLT_CNX_MGR_USE_HT
    //
    // For some "new technology" os(?) we´ll need to allocate a hash table
    // in order to speed up mapping fds to connection items. In addition,
    // all entries of the _connections table are linked into a list of
    // free entries. That speeds up finding a free slot quite a bit.
    //
    int i;
    for ( i = 0; i < _fdset_size; ++i ) {
    	_connections[i].addAfter(_free_entries);
    }
    //
    // Calculate the hash table size depending on the size of a fdset.
    // We always use the next number to the power of two which is larger
    // than the size of a fdset.
    //
    for ( i = 32; i < _fdset_size; i <<= 1 )
	;
    _hash_table_size = i;
    _hash_table_mask = i - 1; // this has always the lower bits set...
    _hash_table = new _KsCnxHashTableItem[_hash_table_size];
    if ( !_hash_table ) {
	_is_ok = false;
    }
#endif
    //
    // Register a timer event which will fire from time to time and
    // triggers fragment garbage collection for the dynamic XDR memory
    // streams.
    //
    KsGarbageTimerEvent *pGarbageEvent = 
	new KsGarbageTimerEvent(30 /* seconds */);
    if ( !pGarbageEvent ) {
	PltLog::Error("KsStdConnectionManager::KsStdConnectionManager(): "
		      "could not create garbage timer event.");
	_is_ok = false;
    }
    if ( !addTimerEvent(pGarbageEvent) ) {
	PltLog::Error("KsStdConnectionManager::KsStdConnectionManager(): "
		      "could not register garbage timer event.");
	_is_ok = false;
    }
} // KsStdConnectionManager::KsStdConnectionManager


// ---------------------------------------------------------------------------
// Destroy a connection manager object. We just delete the connection table
// and every auto-shutdownable connection in it.
//
KsStdConnectionManager::~KsStdConnectionManager()
{
    if ( _connections ) {
    	int i;
	for ( i = 0; i < _fdset_size; ++i ) {
	    KsConnection *con = _connections[i]._connection;
	    if ( con && con->isAutoDestroyable() ) {
		removeConnection(*con);
		con->shutdown();
	        delete con;
	    }
	}
    	delete _connections;
	_connections = 0;
    }
#if PLT_CNX_MGR_USE_HT
    if ( _hash_table ) {
	delete _hash_table;
	_hash_table = 0;
    }
#endif
} // KsStdConnectionManager::~KsStdConnectionManager


// ---------------------------------------------------------------------------
// statistics stuff...
unsigned int KsStdConnectionManager::getConnectionCount()
{ return _connection_count; }
unsigned int KsStdConnectionManager::getIoErrorCount()
{ return _io_errors; }
unsigned int KsStdConnectionManager::getIoRxErrorCount()
{ return _io_rx_errors; }
unsigned int KsStdConnectionManager::getIoTxErrorCount()
{ return _io_tx_errors; }


// ---------------------------------------------------------------------------
// Checks for pending events and returns true in this case.
//
// Here, we check for 1) pending timer events, 2) for pending I/O timeout
// events, and finally 3) for pending I/O itself.
//
bool KsStdConnectionManager::hasPendingEvents() const
{
    // Check for pending timer events...
    if ( !_timer_queue.isEmpty()
         && !_timer_queue.peek()->remainingTime().isZero() ) {
	return true;
    }
    // Check for pending I/O timeouts...
    if ( mayHaveTimeout()
         && getEarliestTimeoutSpan().isZero() ) {
	return true;
    }
    // Check for pending I/O via a non-blocking select() call
    fd_set read_fds, write_fds;
    size_t numfds = getFdSets(read_fds, write_fds);
    KsTime zerotimeout;
    if ( getReadyFds(read_fds, write_fds, numfds, &zerotimeout) > 0 ) {
	return true;
    }
    return false;
} // KsStdConnectionManager::hasPendingEvents


// ---------------------------------------------------------------------------
// Convenience function, which falls back on servePendingEvents(), which
// expects a pointer to a time object.
//
bool
KsStdConnectionManager::servePendingEvents(KsTime timeout) 
{
    return servePendingEvents(&timeout);
} // KsStdConnectionManager::servePendingEvents


// ---------------------------------------------------------------------------
// Handles pending events or waits at most until timeout. In this case it
// returns false, otherwise -- when events have been served -- it returns
// true.
//
bool
KsStdConnectionManager::servePendingEvents(KsTime * pTimeout)
{
    const KsTimerEvent *pEvent = peekNextTimerEvent();
    if ( pEvent ) {
        //
        // The timerqueue is not empty, so peek at the first entry
	// which is the event that will trigger soon...
        //
        KsTime timeleft( pEvent->remainingTime() );
        if ( !timeleft.isZero() ) {
            // 
            // The next timer event is still not pending.
            //
            // Choose the shorter time from
            // external timeout and the time left until the
            // timer event occurs
            //
            if ( pTimeout && *pTimeout < timeleft ) {
                timeleft = *pTimeout;
            }
            int servedRequests = serveRequests(&timeleft);
            return servedRequests > 0;
        } else {
            //
            // pending timer event, execute it immediately
            // 
            removeNextTimerEvent()->trigger();
            return true;
        }
    } else {
        // 
        // The timer queue is empty: use external timeout
        // 
        int servedRequests = serveRequests(pTimeout);
        return servedRequests > 0;
    }
} // KsStdConnectionManager::servePendingEvents


// ---------------------------------------------------------------------------
// Serve RPC requests with timeout. Return -1 on error or the number
// of served fds.
//
int
KsStdConnectionManager::serveRequests(const KsTime *pTimeout)
{
    KsTime cnx_timeout;
    bool   has_cnx_timeout = false;
    fd_set read_fds, write_fds;
    
// FIXME!!!
    for ( ; ; ) {
	KsConnection *con = this->getNextServiceableConnection();
	if ( !con ) {
	    break;
	}
	KsConnectionAttentionInterface *attn = con->getAttentionPartner();
	bool reactivate = true;
	if ( attn ) {
	    reactivate = attn->attention(*con);
	} else {
	    PltLog::Error("KsServerBase::serveRequests(): "
			  "connection without attention partner.");
	    // FIXME: Should we reset the connection here?
	}
	if ( reactivate ) {
	    this->trackConnection(*con);
	}
    }

    if ( this->mayHaveTimeout() ) {
	//
	// There is a timeout in the connection manager´s queue, so let´s
	// check when this timeout will happen. If it would happen earlier
	// than a possible other timeout, then we´ll take the timeout from
	// the connection manager.
	//
	cnx_timeout = getEarliestTimeoutSpan();
	if ( pTimeout ) {
	    if ( cnx_timeout < *pTimeout ) {
		pTimeout = &cnx_timeout;
		has_cnx_timeout = true;
	    }
	} else {
	    pTimeout = &cnx_timeout;
	    has_cnx_timeout = true;
	}
    }
    
    size_t numfds = this->getFdSets(read_fds, write_fds);
    int res = getReadyFds(read_fds, write_fds, numfds, pTimeout);

    if (res > 0) {
        //
        // Now as there are requests pending on some connections, try to
        // serve them. The RPC level is responsible for this. Eventually
        // the RPC level calls a wrapper which in turn redirects the
        // request handling back to our server object.
        // Each connection with waiting request(s) will be served exactly
        // once, so after a round through all input handles we'll come
        // back -- no matter how many requests are still waiting on the
        // connections just served.
        //

        // When you write to a pipe or a socket and there is no listener
        // any more, you will get a SIGPIPE, which by default terminates
        // the application. There are two problems here: The code of the
        // underlying system might depend on delivery of SIGPIPE and
        // there are multiple interfaces for setting signal handlers.

#ifdef KS_IGNORE_SIGPIPE
        // TODO: This is a temporary fix. We leave it
        // to the user what to do providing a hook.
        signal(SIGPIPE, SIG_IGN);
#endif

	int serviceables = this->processConnections(read_fds,
						    write_fds);
	while ( serviceables-- ) {
	    KsConnection *con = this->getNextServiceableConnection();
	    if ( !con ) {
		break;
	    }
	    KsConnectionAttentionInterface *attn = con->getAttentionPartner();
	    bool reactivate = true;
	    if ( attn ) {
		reactivate = attn->attention(*con);
	    } else {
		PltLog::Error("KsServerBase::serveRequests(): "
			      "connection without attention partner.");
		// FIXME: Should we reset the connection here?
	    }
	    if ( reactivate ) {
		this->trackConnection(*con);
	    }
	}
    } else {
	if ( has_cnx_timeout ) {
	    //
	    // We had a connection timeout. So let the connection manager do
	    // its dirty duty.
	    //
	    this->processTimeout();
	}
    }
    return res;
} // KsStdConnectionManager::serveRequests


// ---------------------------------------------------------------------------
// Return the next timer event that will trigger or 0, if there's none in
// the timer event queue.
//
const KsTimerEvent *
KsStdConnectionManager::peekNextTimerEvent() const
{
    if ( _timer_queue.isEmpty() ) {
        return 0;
    } else {
        return _timer_queue.peek();
    }
} // KsStdConnectionManager::peekNextTimerEvent


// ---------------------------------------------------------------------------
// Add a timer event object to the timer event queue. If its time has come
// the timer event object will be activated by calling its trigger method.
//
bool 
KsStdConnectionManager::addTimerEvent(KsTimerEvent *event)
{
    PLT_PRECONDITION(event);
    return _timer_queue.add(event);
} // KsStdConnectionManager::addTimerEvent


// ---------------------------------------------------------------------------
// Remove a specific timer event object -- at least if it's in the timer event
// queue. In this case the method returns "true". Otherwise if the timer
// event isn't in the queue then you'll get a "false" result.
//
bool 
KsStdConnectionManager::removeTimerEvent(KsTimerEvent *event)
{
    return _timer_queue.remove(event);
}  // KsStdConnectionManager::removeTimerEvent


// ---------------------------------------------------------------------------
//  Remove the next timer event from the queue
//
KsTimerEvent *
KsStdConnectionManager::removeNextTimerEvent()
{
    return _timer_queue.removeFirst();
} // KsStdConnectionManager::removeNextTimerEvent







// ---------------------------------------------------------------------------
// Track state information for a particular connection. Whenever a connection
// might change its io mode, than call this function, so the fdsets are
// updated, etc.
//
void KsStdConnectionManager::trackCnxIoMode(_KsConnectionItem &item, 
					    KsConnection::ConnectionIoMode ioMode)
{
    int  fd        = item._connection->getFd();
    bool fastwrite = false;

#ifdef CNXDEBUG
    STDNS::cerr << "trackCnxIoMode: " << fd << ": ";
#endif
    //
    // Set the flags in the fdsets for reading or writing data accordingly
    // to what the connection indicates. This way the connection manager does
    // not need any knowledge about the meaning of the different states a
    // connection can be in. All it is interested in is whether it must
    // deliver data to the connection or has to send out data.
    //
    if ( (ioMode & KsConnection::CNX_IO_READABLE) !=
    	 (item._last_io_mode & KsConnection::CNX_IO_READABLE) ) {
	if ( ioMode & KsConnection::CNX_IO_READABLE ) {
    	    FD_SET(fd, &_readable_fdset);
#ifdef CNXDEBUG
	    STDNS::cerr << "[readable]";
#endif
    	} else {
    	    FD_CLR(fd, &_readable_fdset);
#ifdef CNXDEBUG
	    STDNS::cerr << "[!readable]";
#endif
	}
    }
    if ( (ioMode & KsConnection::CNX_IO_WRITEABLE) !=
    	 (item._last_io_mode & KsConnection::CNX_IO_WRITEABLE) ) {
    	if ( ioMode & KsConnection::CNX_IO_WRITEABLE ) {
    	    FD_SET(fd, &_writeable_fdset);
	    if ( !ioMode & KsConnection::CNX_IO_NO_FASTWRITE ) {
		fastwrite = true;
	    }
#ifdef CNXDEBUG
	    STDNS::cerr << "[writeable]";
#endif
    	} else {
    	    FD_CLR(fd, &_writeable_fdset);
#ifdef CNXDEBUG
	    STDNS::cerr << "[!writeable]";
#endif
    	}
    }
    
    if ( ioMode & KsConnection::CNX_IO_ATTENTION ) {
    	//
    	// This connection needs attention.
    	//
	item.remove(); // just in case it was in the timeout list
	item.addAfter(_serviceable_connections);
	++_serviceable_count;
#ifdef CNXDEBUG
	STDNS::cerr << "[attention]";
#endif
    }

    if ( (ioMode & KsConnection::CNX_IO_NEED_TIMEOUT) !=
	 (item._last_io_mode & KsConnection::CNX_IO_NEED_TIMEOUT) ) {
        if ( ioMode & KsConnection::CNX_IO_NEED_TIMEOUT ) {
    	    //
    	    // This connection needs a timeout supervision. So calculate when
    	    // the connection will be due and remove it from any list it may be
    	    // currently linked into. Then put this connection on the timeout
    	    // list. Start searching for the insert point from the end of the
	    // list.
    	    //
	    _KsConnectionItem *it = (_KsConnectionItem *) 
	                        	 _active_connections._prev;
    	    item._best_before = PltTime::now(item._connection->getTimeout());
	    item.remove();
	    while ( (it != (_KsConnectionItem *) &_active_connections) &&
	            (it->_best_before > item._best_before) ) {
		it = (_KsConnectionItem *) it->_prev;
	    }
	    item.addAfter(*it);
#ifdef CNXDEBUG
	    STDNS::cerr << "[timeout]";
#endif
	} else if ( !(ioMode & KsConnection::CNX_IO_ATTENTION) ) {
	    item.remove();
#ifdef CNXDEBUG
	    STDNS::cerr << "[!timeout]";
#endif
	}
    }
    //
    // For the damned lies, pardon, statistics...
    //
    if ( ioMode & KsConnection::CNX_IO_HAD_ERROR ) {
	++_io_errors;
#ifdef CNXDEBUG
	STDNS::cerr << "[io error]";
#endif
    }
    if ( ioMode & KsConnection::CNX_IO_HAD_RX_ERROR ) {
	++_io_rx_errors;
#ifdef CNXDEBUG
	STDNS::cerr << "[io rx error]";
#endif
    }
    if ( ioMode & KsConnection::CNX_IO_HAD_TX_ERROR ) {
	++_io_tx_errors;
#ifdef CNXDEBUG
	STDNS::cerr << "[io tx error]";
#endif
    }

#ifdef CNXDEBUG
    STDNS::cerr << STDNS::endl;
#endif
    //
    // Finally remember this new state so we can optimize the fdset access
    // functions the next time we're called for this particular connection.
    //
    item._last_io_mode = ioMode;
    //
    // This looks somehow like a dirty hack but it does speed up things in
    // some situations by quite some ten miliseconds, because we do fill
    // an outgoing pipe very early without having to wait for the select()
    // call to notifiy us to send our data for the first time.
    //
    if ( fastwrite ) {
	trackCnxIoMode(item, item._connection->send());
    }
} // KsStdConnectionManager::trackCnxIoMode


// ---------------------------------------------------------------------------
// Do not inline this function on OpenVMS; the code generated by certain
// versions of the HP C++ compiler is buggy.
//
int KsStdConnectionManager::getFdSets(fd_set &readables, 
				      fd_set &writeables) const
{
    readables = _readable_fdset;
    writeables = _writeable_fdset;
    return _fdset_size;
} // KsStdConnectionManager::getFdSets


// ---------------------------------------------------------------------------
// Get the timestamp for the point in time when the first connection will
// time out. You should first call hasTimeout() to make sure that there are
// any active connections at the moment which could time out. If you call
// this function with no active connection, then it'll return a point in
// time far far away (actually around 2038, but that's another story).
//
PltTime KsStdConnectionManager::getEarliestTimeout() const
{
    if ( mayHaveTimeout() ) {
    	return ((_KsConnectionItem *) _active_connections._next)->
	    _best_before;
    } else {
    	return PltTime(MAXINT);
    }
} // KsStdConnectionManager::getEarliestTimeout


// ---------------------------------------------------------------------------
// Nearly same as above, but this time we´ll return a time span instead of a
// point somewhere in time.
//
PltTime KsStdConnectionManager::getEarliestTimeoutSpan() const
{
    if ( mayHaveTimeout() ) {
	PltTime jetzat(PltTime::now()); // Swabian for "now"
	PltTime cnx_timeout = ((_KsConnectionItem *) 
		_active_connections._next)->_best_before;
	if ( cnx_timeout > jetzat ) {
	    return cnx_timeout - jetzat;
	} else {
	    return PltTime();
	}
    } else {
    	return PltTime(MAXINT);
    }
} // KsStdConnectionManager::getEarliestTimeout


// ---------------------------------------------------------------------------
//
bool KsStdConnectionManager::mayHaveTimeout() const
{
    return _active_connections._next != &_active_connections;
} // KsStdConnectionManager::mayHaveTimeout


// ---------------------------------------------------------------------------
// This function should only be called if a timeout state has been detected.
// In this case the function closes and destroys auto-destroyable connections
// or resets non-auto-destroyable connections.
//
void KsStdConnectionManager::processTimeout()
{
    _KsConnectionItem *item, *next;
    PltTime             now = PltTime::now();
    
    item = (_KsConnectionItem *) _active_connections._next;
    while ( item != &_active_connections ) {
	next = (_KsConnectionItem *) item->_next;
	if ( item->_best_before <= now ) {
	    //
	    // A connection has timed out...
	    //
	    KsConnection                   *con = item->_connection;
	    KsConnection::ConnectionIoMode  iomode = con->timedOut();

	    trackCnxIoMode(*item, iomode);
	    if ( iomode & KsConnection::CNX_IO_DEAD ) {
		if ( con->isAutoDestroyable() ) {
		    //
		    // Force a close on the connection. By deleting the
		    // connection object, this is responsible for shutting
		    // down the connection as soon as possible.
		    //
		    removeConnection(*con);
		    con->shutdown();
		    delete con;
		} else {
		    //
		    // We're not allowed to doom this particular connection,
		    // although it had a timeout. So we're just resetting it,
		    // so it'll tell us what io mode it is now in.
		    //
		    trackCnxIoMode(*item, con->reset());
		}
	    }
	    item = next;
	} else {
	    break; // all further items will expire in the future...
	}
    }
} // KsStdConnectionManager::processTimeout



// ---------------------------------------------------------------------------
// Helper function. Get ready readable fds with optional timeout. When using
// a connection manager we´re watching for writeable fds too.
// Returns number of ready fds or -1 on error, and the fdsets are updated to
// reflect file descriptors willing to do i/o. Note that this helper function
// eventually boils down to a select() with an optional PltTime timeout.
//
int
KsStdConnectionManager::getReadyFds(fd_set &read_fds, 
				    fd_set &write_fds, 
				    size_t numfds, 
				    const KsTime *pto) const
{
    // copy timeout, bbsts
    KsTime timeout;
    KsTime *pTimeout;
    if (pto) {
        timeout = *pto;
        pTimeout = &timeout;
    } else {
        pTimeout = 0;
    }

#if PLT_SYSTEM_HPUX && PLT_SYSTEM_HPUX_MAJOR<10
    int res = select(numfds,
	             (int *) &read_fds, (int *) &write_fds, 0,
	             pTimeout);
#else
    int res = select(numfds, &read_fds, &write_fds, 0, pTimeout);
#endif

    if ( res == -1 ) {
        // select reports an error
#if PLT_SYSTEM_NT
        int myerrno = WSAGetLastError();
#else
        int myerrno = errno;
#endif
        PLT_DMSG_ADD("select returned -1: " << strerror(myerrno));
        PLT_DMSG_END;
        if ( myerrno == EINTR ) {
            //
            // While waiting for something to happen on the sockets, we
            // were interrupted by a signal. So let's interpret this as
            // a timeout...
            //
            FD_ZERO(&read_fds);
	    FD_ZERO(&write_fds);
            res = 0;
        } else {
            // something unexpected happened
            PltLog::Error("Select failed unexpectedly");
        }
    }
    return res;
} // KsStdConnectionManager::getReadyFds



// ---------------------------------------------------------------------------
// Handle incomming and outgoing data on connections which are ready for i/o.
// If a connection fails (not timeouts, they're handled in processTimeout)
// and it is auto-destroyable, then it will be killed. Otherwise it is put
// back into the idle state.
//
int
KsStdConnectionManager::processConnections(fd_set &readables, 
					   fd_set &writeables)
{
    _KsConnectionItem              *item;
    KsConnection                   *con;
    KsConnection::ConnectionIoMode  ioMode;
#if !PLT_SYSTEM_NT
    int                              fd_idx;
#else
    int                              i;
    SOCKET                          *sock;
#endif

#ifdef CNXDEBUG
    STDNS::cerr << "processConnections" << STDNS::endl;
#endif
    
#if !PLT_SYSTEM_NT
    for ( fd_idx = 0; fd_idx < _fdset_size; ++fd_idx ) {
    	//
	// Check whether this file descriptor is writeable. Then send
	// data from the appropriate connection object...
	//
	if ( FD_ISSET(fd_idx, &writeables) ) {
#if !PLT_CNX_MGR_USE_HT
	    item = _connections + fd_idx;
#else
	    item = getConnectionItem(fd_idx);
#endif
#else
    sock = writeables.fd_array;
    for ( i = writeables.fd_count; i; --i, ++sock ) {
    	{
	    item = getConnectionItem(*sock);
#endif
	    con  = item->_connection;
#ifdef CNXDEBUG
	    STDNS::cerr << "processConnections: " << con->getFd() 
		 << ": [send]" << STDNS::endl;
#endif
	    ioMode = con->send();
	    trackCnxIoMode(*item, ioMode);
	    if ( ioMode & KsConnection::CNX_IO_DEAD ) {
    	    	//
	    	// The connection has failed miserabely, so if it can be auto-
	    	// matically destroyed, do so. This is used with TCP/IP client
	    	// connections that have been closed or become broken. If we're
	    	// not allowed to doom the connection, then we'll just reset it.
	    	//
#ifdef CNXDEBUG
	    STDNS::cerr << "processConnections: " << con->getFd() 
		 << ": [dead after send]" << STDNS::endl;
#endif
	    	if ( con->isAutoDestroyable() ) {
		    removeConnection(*con);
		    con->shutdown();
		    delete con;
		} else {
		    trackCnxIoMode(*item, con->reset());
		}
	    }
	}
    }

#if !PLT_SYSTEM_NT
    for ( fd_idx = 0; fd_idx < _fdset_size; ++fd_idx ) {
    	//
	// Check whether this file descriptor is readable. Then put
	// the new data into the appropriate connection object...
	//
	if ( FD_ISSET(fd_idx, &readables) ) {
#if !PLT_CNX_MGR_USE_HT
	    item = _connections + fd_idx;
#else
	    item = getConnectionItem(fd_idx);
#endif
#else
    sock = readables.fd_array;
    for ( i = readables.fd_count; i; --i, ++sock ) {
    	{
	    item = getConnectionItem(*sock);
#endif
	    con    = item->_connection;
#ifdef CNXDEBUG
	    STDNS::cerr << "processConnections: " << con->getFd() 
		 << ": [receive]" << STDNS::endl;
#endif
	    ioMode = con->receive();
	    trackCnxIoMode(*item, ioMode);
	    if ( ioMode & KsConnection::CNX_IO_DEAD ) {
    	    	//
	    	// The connection has failed miserabely, so if it can be auto-
	    	// matically destroyed, do so. This is used with TCP/IP client
	    	// connections that have been closed or become broken. If we're
	    	// not allowed to doom the connection, then we'll just reset it.
	    	//
#ifdef CNXDEBUG
	    STDNS::cerr << "processConnections: " << con->getFd() 
		 << ": [dead after receive]" << STDNS::endl;
#endif
	    	if ( con->isAutoDestroyable() ) {
		    removeConnection(*con);
		    con->shutdown();
		    delete con;
		} else {
		    trackCnxIoMode(*item, con->reset());
		}
	    }
	}
    }
    return _serviceable_count;
} // KsStdConnectionManager::processConnections


// ---------------------------------------------------------------------------
// Shut down all auto-destroyable connections but first try to flush all
// sending connections. This way, the server can flush its streams. The
// parameter "secs" specifies how long to wait for the streams to get fully
// flushed.
//
// Note that we don´t obey timeouts here -- we´re just trying to get rid of
// data to be sent within the time span the caller has specified as "secs".
//
bool
KsStdConnectionManager::shutdownConnections(long secs)
{
    int     pending = 0;
    int     i;

    if ( !_connections ) {
	return true; // nothing to shut down, so it´s okay.
    }
    //
    // First close all auto-destroyable connections which are currently
    // not sending. Count those that want to send, so we can later keep
    // track of those pesky streams.
    //
    for ( i = 0; i < _fdset_size; ++i ) {
	KsConnection *con = _connections[i]._connection;
	if ( con ) {
	    if ( (con->getIoMode() & KsConnection::CNX_IO_WRITEABLE)
		 && (con->getState() != KsConnection::CNX_STATE_CONNECTING) ) {
		// count connections that want to send data
		++pending;
	    } else {
	    	if ( con->isAutoDestroyable() ) {
	    	    removeConnection(*con);
	    	    con->shutdown();
	    	    delete con;
		}
	    }
	}
    }
    //
    // Now try to get rid of the data the connections want to send...
    //
    PltTime                          doomsday = PltTime::now(secs);
    PltTime                          sleep;
    fd_set                           writeables;
    _KsConnectionItem              *item;
    KsConnection                   *con;
    KsConnection::ConnectionIoMode  ioMode;
#if !PLT_SYSTEM_NT
    int                              fd_idx;
#else
    SOCKET                          *sock;
#endif

    for ( ; pending > 0 ; ) {
    	sleep = PltTime::now();
	if ( doomsday < sleep ) {
	    //
	    // timed out with connections still trying to send data, so
	    // let the caller know that we´re closing connections...
	    //
	    return false;
	}
    	sleep = doomsday - sleep;
    	writeables = _writeable_fdset;
#if PLT_SYSTEM_HPUX && PLT_SYSTEM_HPUX_MAJOR<10
	int res = select(_fdset_size,
	        	 0, (int *) &writeables, 0,
	        	 &sleep);
#else
    	int res = select(_fdset_size, 0, &writeables, 0, &sleep);
#endif

        if ( res == -1 ) {
    	    // select reports an error
#if PLT_SYSTEM_NT
            if ( WSAGetLastError() == EINTR ) {
		continue;
	    }
#else
	    if ( errno == EINTR ) {
		continue;
	    }
#endif
	    return false;
	}
	
	//
	// serve pending connections...
	//
#if !PLT_SYSTEM_NT
	for ( fd_idx = 0; fd_idx < _fdset_size; ++fd_idx ) {
    	    //
	    // Check whether this file descriptor is writeable. Then send
	    // data from the appropriate connection object...
	    //
	    if ( FD_ISSET(fd_idx, &writeables) ) {
#if !PLT_CNX_MGR_USE_HT
	    item = _connections + fd_idx;
#else
	    item = getConnectionItem(fd_idx);
#endif
#else
	sock = writeables.fd_array;
	for ( i = writeables.fd_count; i; --i, ++sock ) {
    	    {
		item = getConnectionItem(*sock);
#endif
		con    = item->_connection;
		ioMode = con->send();
		trackCnxIoMode(*item, ioMode);
		if ( ioMode & KsConnection::CNX_IO_DEAD ||
	             !(ioMode & KsConnection::CNX_IO_WRITEABLE) ) {
		    //
		    // The connection failed for some reason, so destroy it
		    // if we´re allowed to do so. In any case, we won´t serve
		    // it anymore. In case the connection has sent all its
		    // data, the same applies too.
		    //
		    --pending;
	    	    if ( con->isAutoDestroyable() ) {
			removeConnection(*con);
			con->shutdown();
			delete con;
		    } else {
			trackCnxIoMode(*item, con->reset());
		    }
		}
	    }
	}
    }
    //
    // All pending connections have been served. Fine.
    //
    return true;
} // KsStdConnectionManager::shutdownConnections


// ---------------------------------------------------------------------------
// Returns the next connection that is waiting to be served. In addition this
// connection is removed from the "serviceable" list.
//
KsConnection *
KsStdConnectionManager::getNextServiceableConnection()
{
    if ( _serviceable_connections._next == &_serviceable_connections ) {
    	return 0;
    } else {
    	_KsConnectionItem *item =
		(_KsConnectionItem *) _serviceable_connections._next;
	item->remove();
	--_serviceable_count;
	return item->_connection;
    }
} // KsStdConnectionManager::getNextServiceableReadyConnection


// ---------------------------------------------------------------------------
// Brings a connection back into the play after it has moved into the service-
// able list.
//
bool KsStdConnectionManager::trackConnection(KsConnection &con)
{
    int                 fd   = con.getFd();
    _KsConnectionItem *item = getConnectionItem(fd);
    if ( item && item->_connection ) {
	if ( con.getState() != KsConnection::CNX_STATE_DEAD ) {
    	    trackCnxIoMode(*item, con.getIoMode());
	    //
	    // As we take advantage of "fast writes" (or early writes),
	    // we need to check the state of the connection after we have
	    // tracked its io mode. This is necessary, because trackCnx-
	    // IoMode() does fast writes, that it, is tries immediately to
	    // write data down to a connection which just has switched into
	    // sending state. Then it can happen that the connections runs
	    // into trouble (broken connection, etc.), so that we need to
	    // kill it immediately. Otherwise it would just stall and never
	    // be closed.
	    //
	    if ( con.getState() != KsConnection::CNX_STATE_DEAD ) {
		return true;
	    }
	}
	if ( con.isAutoDestroyable() ) {
	    con.shutdown();
	    removeConnection(con);
	    delete &con;
	    return true;
	}
	con.reset(); // Try to reset nevertheless...
    }
    return false;
} // KsStdConnectionManager::trackConnection


// ---------------------------------------------------------------------------
// Allow someone outside the connection manager and a connection itself to
// reset that particular connection.  Puts the connection back into its
// default mode. After certain i/o errors we might want to do this
// (especially with udp connections).
//
bool KsStdConnectionManager::resetConnection(KsConnection &con)
{
    int                 fd   = con.getFd();
    _KsConnectionItem *item = getConnectionItem(fd);
    if ( item && item->_connection ) {
	if ( con.getState() != KsConnection::CNX_STATE_DEAD ) {
    	    trackCnxIoMode(*item, con.reset());
	    if ( con.getState() != KsConnection::CNX_STATE_DEAD ) {
		return true;
	    }
	}
	if ( con.isAutoDestroyable() ) {
	    con.shutdown();
	    removeConnection(con);
	    delete &con;
	    return true;
	}
    }
    return false;
} // KsStdConnectionManager::resetConnection


// ---------------------------------------------------------------------------
// Helper function to retrieve a pointer to the connection item within the
// fd-to-connection table. If the given file descriptor is invalid, then a
// null pointer is returned.
//
_KsConnectionItem *
KsStdConnectionManager::getConnectionItem(int fd)
{
#if !PLT_CNX_MGR_USE_HT
    //
    // Make sure that the given file descriptor makes sense and that this
    // connection hasn't been added yet.
    //
    if ( (fd < 0) || (fd >= _fdset_size) || !_connections ) {
    	return 0;
    }
    return _connections + fd;
#else
    //
    // With NT we don't know easily whether the fd is or is not under the
    // control of the connection manager.
    //
    unsigned hidx = getHash(fd) & _hash_table_mask;
    _KsCnxHashTableItem  *htItem = _hash_table + hidx;
    _KsConnectionItem   **cnx    = htItem->_overflow_table;
    int                    i      = htItem->_used;
    // TODO: binary search
    for ( ; i ; --i, ++cnx ) {
	if ( (*cnx)->_fd == fd ) {
	    return *cnx;
	}
    }
    return 0;
#endif
} // KsStdConnectionManager::getConnectionItem


// ---------------------------------------------------------------------------
// Now for the connection management functions. The next two suckers add or
// remove respectively a connection object from the domain of power of the
// connection manager.
//
// In the case of adding a new connection, the connection will be asked for
// its current io mode. This way, server connections will automagically start
// being readable with no timeout.
//
bool
KsStdConnectionManager::addConnection(KsConnection &con)
{
    int fd = con.getFd();
#if !PLT_CNX_MGR_USE_HT
    _KsConnectionItem *item = getConnectionItem(fd);
#else
    unsigned hidx = getHash(fd) & _hash_table_mask;
    _KsCnxHashTableItem  *htItem = _hash_table + hidx;
    _KsConnectionItem   **cnx    = htItem->_overflow_table;
    int                    i      = htItem->_used;
    _KsConnectionItem    *item;
    //
    // First make sure that this connection (or its fd) hasn't already been
    // put under the control of the connection manager. If it has, then fail.
    //    
    for ( ; i ; --i, ++cnx ) {
	if ( (*cnx)->_fd == fd ) {
	    return false;
	}
    }
    if ( htItem->_used == htItem->_size ) {
	//
	// Need to allocate more room for the overflow table, which holds
	// the references to the individual connection items.
	//
	htItem->_size += 8;
	cnx = new _pKsConnectionItem[htItem->_size];
	if ( !cnx ) {
	    return false;
	}
	if ( htItem->_overflow_table && htItem->_used ) {
	    memcpy(cnx, htItem->_overflow_table, 
	           sizeof(_KsConnectionItem *) * htItem->_used);
	}
	if ( htItem->_overflow_table ) {
	    delete htItem->_overflow_table;
	}
	htItem->_overflow_table = cnx;
    }
    //
    // Now pull of a fresh connection item from the free list and fill in
    // the reference to the connection object and put the item into the
    // hash table.
    //
    if ( _free_entries._next == &_free_entries ) {
    	return false; // Run out of free entries. This means that the size of
	              // a fd set isn't large enough.
    }
    item = (_KsConnectionItem *) _free_entries._next;
    item->remove();
    htItem->_overflow_table[htItem->_used] = item;
    ++(htItem->_used);
#endif
	    
    if ( item && !item->_connection ) {
    	item->_connection   = &con;
	item->_last_io_mode = KsConnection::CNX_IO_DORMANT;
	item->_fd           = fd;
	con.thisIsMyConnectionManager(this);
	trackCnxIoMode(*item, con.getIoMode());
    	++_connection_count;
	return true;
    }
    return false;
} // KsStdConnectionManager::addConnection


// ---------------------------------------------------------------------------
// Bring a connection out of the control of the connection manager. This will
// automatically deselect the connection from reading and writing (otherwise
// this would be going bad...)
//
bool
KsStdConnectionManager::removeConnection(KsConnection &con)
{
    int                 fd   = con.getFd();
#if !PLT_CNX_MGR_USE_HT
    _KsConnectionItem *item = getConnectionItem(fd);
#else
    unsigned hidx = getHash(fd) & _hash_table_mask;
    _KsCnxHashTableItem  *htItem = _hash_table + hidx;
    _KsConnectionItem   **cnx    = htItem->_overflow_table;
    int                    idx    = htItem->_used;
    _KsConnectionItem    *item   = 0;
    // TODO: binary search
    for ( ; idx ; --idx, ++cnx ) {
	if ( (*cnx)->_fd == fd ) {
	    item = *cnx;
	    idx = htItem->_used - idx; // make idx point to this entry
	    break;
	}
    }
#endif
    if ( item && item->_connection ) {
    	//
    	// Remove this connection from the list (timeout, ready) it is
    	// currently in. And don't forget to remove it from the file
	// descriptor sets.
	//
	item->_connection->thisIsMyConnectionManager(0);
    	item->remove();
    	FD_CLR(fd, &_readable_fdset);
	FD_CLR(fd, &_writeable_fdset);
    	--_connection_count;
	item->_connection = 0;
#if PLT_CNX_MGR_USE_HT
    	//
	// Remove this entry from the overflow table of a hash table entry
	// and put it into the list of empty entries.
	//
    	if ( idx < htItem->_used - 1 ) {
	    memmove(htItem->_overflow_table + idx,
	           htItem->_overflow_table + idx + 1,
	           sizeof(_KsConnectionItem *) *
		     (htItem->_used - idx - 1));
	}
	--(htItem->_used);
    	item->addAfter(_free_entries);
#endif
    	return true;
    }
    return false;
} // KsStdConnectionManager::removeConnection


// ---------------------------------------------------------------------------
// Return the connection object associated with a particular file descriptor.
// If the file descriptor is invalid, then a null pointer is returned.
//
KsConnection *
KsStdConnectionManager::lookupConnection(int fd)
{
    _KsConnectionItem *item = getConnectionItem(fd);
    if ( item ) {
    	return item->_connection;
    }
    return 0;
} // KsStdConnectionManager::lookupConnection


/* End of stdconnectionmgr.cpp */
