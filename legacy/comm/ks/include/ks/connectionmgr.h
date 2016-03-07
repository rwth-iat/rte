/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/connectionmgr.h,v 1.10 2007-04-25 12:57:20 martin Exp $ */
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
 * connectionmgr.h -- Implements a connection manager which can handle
 *                    connection objects which in turn encapsulate XDR
 *                    streams. The connection manager makes it possible to
 *                    use so-called buffered XDR streams, which decouple
 *                    RPC I/O from service handling and thus serialization
 *                    and deserialization. In addition, the connection
 *                    manager can also handle other kinds of connections.
 *
 * Written by Harald Albrecht <harald@plt.rwth-aachen.de>
 */

#ifndef KS_CONNECTIONMGR_H_INCLUDED
#define KS_CONNECTIONMGR_H_INCLUDED

#if PLT_USE_BUFFERED_STREAMS

#include "plt/time.h"

class KssConnection;
class KssConnectionManager;

#ifndef KS_CONNECTION_H_INCLUDED
#include "ks/connection.h"
#endif


// ---------------------------------------------------------------------------
// This one was surely missing for long ;-) This is a basic implementation of
// a doubly-linked list.
//
struct _PltDLinkedListNode {
public: // oh, M$ is sooooo dumb...
    _PltDLinkedListNode *_next; // next node in list
    _PltDLinkedListNode *_prev; // previous node in list

    _PltDLinkedListNode() { _next = _prev = this; }
    
    void addBefore(_PltDLinkedListNode &n);
    void addAfter(_PltDLinkedListNode &n);
    void remove();
}; // struct _PltDLinkedListNode


// ---------------------------------------------------------------------------
// The connection manager manages a lookup-table to quickly find the
// connection object associated with a particular file descriptor. In
// addition, each item in the table can also be linked into a list of
// active connections (timeout) or connections waiting to be served.
//
struct _KssConnectionItem : public _PltDLinkedListNode {
public: // oh, M$ is sooooo dumb...
    KssConnection                   *_connection;
    PltTime                          _best_before;
    KssConnection::ConnectionIoMode  _last_io_mode;
    int                              _fd; // optimization...

    _KssConnectionItem() : _connection(0),
                           _last_io_mode(KssConnection::CNX_IO_DORMANT),
                           _fd(-1) { }
}; // struct _KssConnectionItem


#if PLT_CNX_MGR_USE_HT
// ---------------------------------------------------------------------------
// As some "new technology" uses crude handles instead of file descriptors,
// there's no easy mapping between fds and connection items. So we're forced
// to use a hash table instead, which is made of the following class.
//
struct _KssCnxHashTableItem {
public: // oh, M$ is sooooo dumb...
    _KssConnectionItem **_overflow_table; /* sorted table w/ references      */
    int                  _size;           /* # of allocated entries in table */
    int                  _used;           /* # of used entries               */
    
    _KssCnxHashTableItem() : _overflow_table(0), _size(0), _used(0) { }
}; // class _KssCnxHashTableItem
#endif


// ---------------------------------------------------------------------------
// The Connection Manager Itself(sm). It manages the whole mess of glory
// connections.
//
class KssConnectionManager {
public:
    KssConnectionManager();
    virtual ~KssConnectionManager();
    
    void shutdown(int deadline); // TODO
    
    bool isOk() const { return _is_ok; }
    
    // management of timeouts...
    bool mayHaveTimeout()
    	{ return _active_connections._next != &_active_connections; }
    PltTime getEarliestTimeout();
    PltTime getEarliestTimeoutSpan();
    void processTimeout();
    
    int getFdSets(fd_set &readables, fd_set &writeables); // OpenVMS: do not inline!!!
    int getFdSetSize() { return _fdset_size; }

    // process incomming and outgoing data...
    int processConnections(fd_set &readables, fd_set &writeables);
    
    // connections waiting to be served...or being served...
    KssConnection *getNextServiceableConnection();
    bool trackConnection(KssConnection &con);

    // connection management
    bool addConnection(KssConnection &con);
    bool removeConnection(KssConnection &con);
    KssConnection *lookupConnection(int fd);
    bool resetConnection(KssConnection &con);

    // send out to the world your last bits...
    bool shutdownConnections(long secs);

    // damned lies & statistics
    inline unsigned int getConnectionCount() { return _connection_count; }
    inline unsigned int getIoErrorCount() { return _io_errors; }
    inline unsigned int getIoRxErrorCount() { return _io_rx_errors; }
    inline unsigned int getIoTxErrorCount() { return _io_tx_errors; }
    
protected:
    friend class KssConnection;
    void trackCnxIoMode(_KssConnectionItem &item, 
                        KssConnection::ConnectionIoMode ioMode);
    
    // TODO: incomming notifications from dying connections
    virtual void connectionShutdownNotification(KssConnection &) { }
    
private:
    _KssConnectionItem *getConnectionItem(int fd);

    bool                _is_ok;
    
    int                 _fdset_size;  // size of fd_sets on this host
    _KssConnectionItem *_connections; // the connections per fd
    unsigned int        _connection_count;

    fd_set              _readable_fdset;
    fd_set              _writeable_fdset;
    int                 _serviceable_count;
    
    unsigned int        _io_errors;
    unsigned int        _io_rx_errors;
    unsigned int        _io_tx_errors;
        
    _PltDLinkedListNode _active_connections; // for timeouts
    _PltDLinkedListNode _serviceable_connections;  // waiting to be served

#if PLT_CNX_MGR_USE_HT
    _PltDLinkedListNode   _free_entries;
    _KssCnxHashTableItem *_hash_table;
    int                   _hash_table_size;
    int                   _hash_table_mask;
    
    unsigned int getHash(int fd) const { return fd * 625842579; } // TODO!
#endif    
}; // class KssConnectionManager


#endif /* PLT_USE_BUFFERED_STREAMS */
#endif

/* End of connectionmgr.h */
