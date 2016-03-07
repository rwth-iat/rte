/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/connectionmgr.cpp,v 1.15 2008-09-22 08:26:09 henning Exp $ */
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

#include "ks/connectionmgr.h"


//#define CNXDEBUG 1


//
// Ah, yes... compile the whole stuff only if we want to use the buffered
// stream magic. Otherwise we can skip this source module completely.
//
#if PLT_USE_BUFFERED_STREAMS

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


typedef _KssConnectionItem * _pKssConnectionItem;

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
// Create and initialize (aka "construct") a connection manager object. This
// involves creating a lookup-table so we can easily find the connection
// object for a given file descriptor (this is what the select() call will
// give us as information). Unfortunately, Microsoft's file descriptor sets
// are different once again, so in that case we have to use a hash table...
//
// Check _is_ok to make sure the connection manager object could be ctor�ed
// successfully.
//
KssConnectionManager::KssConnectionManager()
    : _is_ok(true),
      _connection_count(0), _serviceable_count(0),
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
    _connections = new _KssConnectionItem[_fdset_size];
    FD_ZERO(&_readable_fdset);
    FD_ZERO(&_writeable_fdset);
    if ( !_connections ) {
	_is_ok = false;
    }
#if PLT_CNX_MGR_USE_HT
    //
    // For some "new technology" os(?) we�ll need to allocate a hash table
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
    _hash_table = new _KssCnxHashTableItem[_hash_table_size];
    if ( !_hash_table ) {
	_is_ok = false;
    }
#endif
} // KssConnectionManager::KssConnectionManager


// ---------------------------------------------------------------------------
// Destroy a connection manager object. We just delete the connection table
// and every auto-shutdownable connection in it.
//
KssConnectionManager::~KssConnectionManager()
{
    if ( _connections ) {
    	int i;
	for ( i = 0; i < _fdset_size; ++i ) {
	    KssConnection *con = _connections[i]._connection;
	    if ( con && con->isAutoDestroyable() ) {
		removeConnection(*con);
		con->shutdown();
	        delete con;
	    }
	}
    	delete [] _connections;
	_connections = 0;
    }
#if PLT_CNX_MGR_USE_HT
    if ( _hash_table ) {
	delete [] _hash_table;
	_hash_table = 0;
    }
#endif
} // KssConnectionManager::~KssConnectionManager


// ---------------------------------------------------------------------------
// Track state information for a particular connection. Whenever a connection
// might change its io mode, than call this function, so the fdsets are
// updated, etc.
//
void KssConnectionManager::trackCnxIoMode(_KssConnectionItem &item, 
                                          KssConnection::ConnectionIoMode ioMode)
{
    int  fd        = item._connection->getFd();
    bool fastwrite = false;

#ifdef CNXDEBUG
    cout << "trackCnxIoMode: " << fd << ": ";
#endif
    //
    // Set the flags in the fdsets for reading or writing data accordingly
    // to what the connection indicates. This way the connection manager does
    // not need any knowledge about the meaning of the different states a
    // connection can be in. All it is interested in is whether it must
    // deliver data to the connection or has to send out data.
    //
    if ( (ioMode & KssConnection::CNX_IO_READABLE) !=
    	 (item._last_io_mode & KssConnection::CNX_IO_READABLE) ) {
	if ( ioMode & KssConnection::CNX_IO_READABLE ) {
    	    FD_SET(fd, &_readable_fdset);
#ifdef CNXDEBUG
	    cout << "[readable]";
#endif
    	} else {
    	    FD_CLR(fd, &_readable_fdset);
#ifdef CNXDEBUG
	    cout << "[!readable]";
#endif
	}
    }
    if ( (ioMode & KssConnection::CNX_IO_WRITEABLE) !=
    	 (item._last_io_mode & KssConnection::CNX_IO_WRITEABLE) ) {
    	if ( ioMode & KssConnection::CNX_IO_WRITEABLE ) {
    	    FD_SET(fd, &_writeable_fdset);
    	    
            if ( !(ioMode & KssConnection::CNX_IO_NO_FASTWRITE) ) {
                fastwrite = true;
            }
#ifdef CNXDEBUG
	    cout << "[writeable]";
#endif
    	} else {
    	    FD_CLR(fd, &_writeable_fdset);
#ifdef CNXDEBUG
	    cout << "[!writeable]";
#endif
    	}
    }
    
    if ( ioMode & KssConnection::CNX_IO_ATTENTION ) {
    	//
    	// This connection needs attention.
    	//
	item.remove(); // just in case it was in the timeout list
	item.addAfter(_serviceable_connections);
	++_serviceable_count;
#ifdef CNXDEBUG
	cout << "[attention]";
#endif
    }

    if ( (ioMode & KssConnection::CNX_IO_NEED_TIMEOUT) !=
	 (item._last_io_mode & KssConnection::CNX_IO_NEED_TIMEOUT) ) {
        if ( ioMode & KssConnection::CNX_IO_NEED_TIMEOUT ) {
    	    //
    	    // This connection needs a timeout supervision. So calculate when
    	    // the connection will be due and remove it from any list it may be
    	    // currently linked into. Then put this connection on the timeout
    	    // list. Start searching for the insert point from the end of the
	    // list.
    	    //
	    _KssConnectionItem *it = (_KssConnectionItem *) 
	                        	 _active_connections._prev;
    	    item._best_before = PltTime::now(item._connection->getTimeout());
	    item.remove();
	    while ( (it != (_KssConnectionItem *) &_active_connections) &&
	            (it->_best_before > item._best_before) ) {
		it = (_KssConnectionItem *) it->_prev;
	    }
	    item.addAfter(*it);
#ifdef CNXDEBUG
	    cout << "[timeout]";
#endif
	} else if ( !(ioMode & KssConnection::CNX_IO_ATTENTION) ) {
	    item.remove();
#ifdef CNXDEBUG
	    cout << "[!timeout]";
#endif
	}
    }
    //
    // For the damned lies, pardon, statistics...
    //
    if ( ioMode & KssConnection::CNX_IO_HAD_ERROR ) {
	++_io_errors;
#ifdef CNXDEBUG
	cout << "[io error]";
#endif
    }
    if ( ioMode & KssConnection::CNX_IO_HAD_RX_ERROR ) {
	++_io_rx_errors;
#ifdef CNXDEBUG
	cout << "[io rx error]";
#endif
    }
    if ( ioMode & KssConnection::CNX_IO_HAD_TX_ERROR ) {
	++_io_tx_errors;
#ifdef CNXDEBUG
	cout << "[io tx error]";
#endif
    }

#ifdef CNXDEBUG
    cout << endl;
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
} // KssConnectionManager::trackCnxIoMode


// ---------------------------------------------------------------------------
//
int KssConnectionManager::getFdSets(fd_set &readables, fd_set &writeables)
{
    readables = _readable_fdset;
    writeables = _writeable_fdset;
    return _fdset_size;
} // KssConnectionManager::getFdSets


// ---------------------------------------------------------------------------
// Get the timestamp for the point in time when the first connection will
// time out. You should first call hasTimeout() to make sure that there are
// any active connections at the moment which could time out. If you call
// this function with no active connection, then it'll return a point in
// time far far away (actually around 2038, but that's another story).
//
PltTime KssConnectionManager::getEarliestTimeout()
{
    if ( mayHaveTimeout() ) {
    	return ((_KssConnectionItem *) _active_connections._next)->
	    _best_before;
    } else {
    	return PltTime(MAXINT);
    }
} // KssConnectionManager::getEarliestTimeout


// ---------------------------------------------------------------------------
// Nearly same as above, but this time we�ll return a time span instead of a
// point somewhere in time.
//
PltTime KssConnectionManager::getEarliestTimeoutSpan()
{
    if ( mayHaveTimeout() ) {
	PltTime jetzat(PltTime::now()); // Swabian for "now"
	PltTime cnx_timeout = ((_KssConnectionItem *) 
		_active_connections._next)->_best_before;
	if ( cnx_timeout > jetzat ) {
	    return cnx_timeout - jetzat;
	} else {
	    return PltTime();
	}
    } else {
    	return PltTime(MAXINT);
    }
} // KssConnectionManager::getEarliestTimeout


// ---------------------------------------------------------------------------
// This function should only be called if a timeout state has been detected.
// In this case the function closes and destroys auto-destroyable connections
// or resets non-auto-destroyable connections.
//
void KssConnectionManager::processTimeout()
{
    _KssConnectionItem *item, *next;
    PltTime             now = PltTime::now();
    
    item = (_KssConnectionItem *) _active_connections._next;
    while ( item != &_active_connections ) {
	next = (_KssConnectionItem *) item->_next;
	if ( item->_best_before <= now ) {
	    //
	    // A connection has timed out...
	    //
	    KssConnection                   *con = item->_connection;
	    KssConnection::ConnectionIoMode  iomode = con->timedOut();

	    trackCnxIoMode(*item, iomode);
	    if ( iomode & KssConnection::CNX_IO_DEAD ) {
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
} // KssConnectionManager::processTimeout


// ---------------------------------------------------------------------------
// Handle incomming and outgoing data on connections which are ready for i/o.
// If a connection fails (not timeouts, they're handled in processTimeout)
// and it is auto-destroyable, then it will be killed. Otherwise it is put
// back into the idle state.
//
int KssConnectionManager::processConnections(fd_set &readables, 
	    	    	    	    	     fd_set &writeables)
{
    _KssConnectionItem              *item;
    KssConnection                   *con;
    KssConnection::ConnectionIoMode  ioMode;
#if !PLT_SYSTEM_NT
    int                              fd_idx;
#else
    int                              i;
    SOCKET                          *sock;
#endif

#ifdef CNXDEBUG
    cout << "processConnections" << endl;
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
	    cout << "processConnections: " << con->getFd() 
		 << ": [send]" << endl;
#endif
	    ioMode = con->send();
	    trackCnxIoMode(*item, ioMode);
	    if ( ioMode & KssConnection::CNX_IO_DEAD ) {
    	    	//
	    	// The connection has failed miserabely, so if it can be auto-
	    	// matically destroyed, do so. This is used with TCP/IP client
	    	// connections that have been closed or become broken. If we're
	    	// not allowed to doom the connection, then we'll just reset it.
	    	//
#ifdef CNXDEBUG
	    cout << "processConnections: " << con->getFd() 
		 << ": [dead after send]" << endl;
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
	    cout << "processConnections: " << con->getFd() 
		 << ": [receive]" << endl;
#endif
	    ioMode = con->receive();
	    trackCnxIoMode(*item, ioMode);
	    if ( ioMode & KssConnection::CNX_IO_DEAD ) {
    	    	//
	    	// The connection has failed miserabely, so if it can be auto-
	    	// matically destroyed, do so. This is used with TCP/IP client
	    	// connections that have been closed or become broken. If we're
	    	// not allowed to doom the connection, then we'll just reset it.
	    	//
#ifdef CNXDEBUG
	    cout << "processConnections: " << con->getFd() 
		 << ": [dead after receive]" << endl;
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
} // KssConnectionManager::processConnections


// ---------------------------------------------------------------------------
// Shut down all auto-destroyable connections but first try to flush all
// sending connections. This way, the server can flush its streams. The
// parameter "secs" specifies how long to wait for the streams to get fully
// flushed.
//
// Note that we don�t obey timeouts here -- we�re just trying to get rid of
// data to be sent within the time span the caller has specified as "secs".
//
bool KssConnectionManager::shutdownConnections(long secs)
{
    int     pending = 0;
    int     i;

    if ( !_connections ) {
	return true; // nothing to shut down, so it�s okay.
    }
    //
    // First close all auto-destroyable connections which are currently
    // not sending. Count those that want to send, so we can later keep
    // track of those pesky streams.
    //
    for ( i = 0; i < _fdset_size; ++i ) {
	KssConnection *con = _connections[i]._connection;
	if ( con ) {
	    if ( (con->getIoMode() & KssConnection::CNX_IO_WRITEABLE)
		 && (con->getState() != KssConnection::CNX_STATE_CONNECTING) ) {
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
    _KssConnectionItem              *item;
    KssConnection                   *con;
    KssConnection::ConnectionIoMode  ioMode;
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
	    // let the caller know that we�re closing connections...
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
		if ( ioMode & KssConnection::CNX_IO_DEAD ||
	             !(ioMode & KssConnection::CNX_IO_WRITEABLE) ) {
		    //
		    // The connection failed for some reason, so destroy it
		    // if we�re allowed to do so. In any case, we won�t serve
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
} // KssConnectionManager::shutdownConnections


// ---------------------------------------------------------------------------
// Returns the next connection that is waiting to be served. In addition this
// connection is removed from the "serviceable" list.
//
KssConnection *KssConnectionManager::getNextServiceableConnection()
{
    if ( _serviceable_connections._next == &_serviceable_connections ) {
    	return 0;
    } else {
    	_KssConnectionItem *item =
		(_KssConnectionItem *) _serviceable_connections._next;
	item->remove();
	--_serviceable_count;
	return item->_connection;
    }
} // KssConnectionManager::getServiceableReadyConnection


// ---------------------------------------------------------------------------
// Brings a connection back into the play after it has moved into the service-
// able list.
//
bool KssConnectionManager::trackConnection(KssConnection &con)
{
    int                 fd   = con.getFd();
    _KssConnectionItem *item = getConnectionItem(fd);
    if ( item && item->_connection ) {
	if ( con.getState() != KssConnection::CNX_STATE_DEAD ) {
    	    trackCnxIoMode(*item, con.getIoMode());
	    /*
	     * As we take advantage of "fast writes" (or early writes),
	     * we need to check the state of the connection after we have
	     * tracked its io mode. This is necessary, because trackCnx-
	     * IoMode() does fast writes, that it, is tries immediately to
	     * write data down to a connection which just has switched into
	     * sending state. Then it can happen that the connections runs
	     * into trouble (broken connection, etc.), so that we need to
	     * kill it immediately. Otherwise it would just stall and never
	     * be closed.
	     */
	    if ( con.getState() != KssConnection::CNX_STATE_DEAD ) {
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
} // KssConnectionManager::trackConnection


// ---------------------------------------------------------------------------
// Allow someone outside the connection manager and a connection itself to
// reset that particular connection.  Puts the connection back into its
// default mode. After certain i/o errors we might want to do this
// (especially with udp connections).
//
bool KssConnectionManager::resetConnection(KssConnection &con)
{
    int                 fd   = con.getFd();
    _KssConnectionItem *item = getConnectionItem(fd);
    if ( item && item->_connection ) {
	if ( con.getState() != KssConnection::CNX_STATE_DEAD ) {
    	    trackCnxIoMode(*item, con.reset());
	    if ( con.getState() != KssConnection::CNX_STATE_DEAD ) {
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
} // KssConnectionManager::resetConnection


// ---------------------------------------------------------------------------
// Helper function to retrieve a pointer to the connection item within the
// fd-to-connection table. If the given file descriptor is invalid, then a
// null pointer is returned.
//
_KssConnectionItem *KssConnectionManager::getConnectionItem(int fd)
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
    _KssCnxHashTableItem  *htItem = _hash_table + hidx;
    _KssConnectionItem   **cnx    = htItem->_overflow_table;
    int                    i      = htItem->_used;
    // TODO: binary search
    for ( ; i ; --i, ++cnx ) {
	if ( (*cnx)->_fd == fd ) {
	    return *cnx;
	}
    }
    return 0;
#endif
} // KssConnectionManager::getConnectionItem


// ---------------------------------------------------------------------------
// Now for the connection management functions. The next two suckers add or
// remove respectively a connection object from the domain of power of the
// connection manager.
//
// In the case of adding a new connection, the connection will be asked for
// its current io mode. This way, server connections will automagically start
// being readable with no timeout.
//
bool KssConnectionManager::addConnection(KssConnection &con)
{
    int fd = con.getFd();
#if !PLT_CNX_MGR_USE_HT
    _KssConnectionItem *item = getConnectionItem(fd);
#else
    unsigned hidx = getHash(fd) & _hash_table_mask;
    _KssCnxHashTableItem  *htItem = _hash_table + hidx;
    _KssConnectionItem   **cnx    = htItem->_overflow_table;
    int                    i      = htItem->_used;
    _KssConnectionItem    *item;
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
	cnx = new _pKssConnectionItem[htItem->_size];
	if ( !cnx ) {
	    return false;
	}
	if ( htItem->_overflow_table && htItem->_used ) {
	    memcpy(cnx, htItem->_overflow_table, 
	           sizeof(_KssConnectionItem *) * htItem->_used);
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
    item = (_KssConnectionItem *) _free_entries._next;
    item->remove();
    htItem->_overflow_table[htItem->_used] = item;
    ++(htItem->_used);
#endif
	    
    if ( item && !item->_connection ) {
    	item->_connection   = &con;
	item->_last_io_mode = KssConnection::CNX_IO_DORMANT;
	item->_fd           = fd;
	con.thisIsMyConnectionManager(this);
	trackCnxIoMode(*item, con.getIoMode());
    	++_connection_count;
	return true;
    }
    return false;
} // KssConnectionManager::addConnection


// ---------------------------------------------------------------------------
// Bring a connection out of the control of the connection manager. This will
// automatically deselect the connection from reading and writing (otherwise
// this would be going bad...)
//
bool KssConnectionManager::removeConnection(KssConnection &con)
{
    int                 fd   = con.getFd();
#if !PLT_CNX_MGR_USE_HT
    _KssConnectionItem *item = getConnectionItem(fd);
#else
    unsigned hidx = getHash(fd) & _hash_table_mask;
    _KssCnxHashTableItem  *htItem = _hash_table + hidx;
    _KssConnectionItem   **cnx    = htItem->_overflow_table;
    int                    idx    = htItem->_used;
    _KssConnectionItem    *item   = 0;
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
	           sizeof(_KssConnectionItem *) *
		     (htItem->_used - idx - 1));
	}
	--(htItem->_used);
    	item->addAfter(_free_entries);
#endif
    	return true;
    }
    return false;
} // KssConnectionManager::removeConnection


// ---------------------------------------------------------------------------
// Return the connection object associated with a particular file descriptor.
// If the file descriptor is invalid, then a null pointer is returned.
//
KssConnection *KssConnectionManager::lookupConnection(int fd)
{
    _KssConnectionItem *item = getConnectionItem(fd);
    if ( item ) {
    	return item->_connection;
    }
    return 0;
} // KssConnectionManager::lookupConnection

#endif /* PLT_USE_BUFFERED_STREAMS */

/* End of connectionmgr.cpp */
