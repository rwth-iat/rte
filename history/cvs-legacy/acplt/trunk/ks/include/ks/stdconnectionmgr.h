/* -*-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/stdconnectionmgr.h,v 1.3 2007-09-25 12:07:06 wolfram Exp $ */
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
 * stdconnectionmgr.h -- Implements a "standard" connection manager which
 *   can handle non-blocking "background" I/O using the bread-and-butter
 *   select() mechanism.
 *
 * Written by Harald Albrecht <harald@plt.rwth-aachen.de>
 */

#ifndef KS_STDCONNECTIONMGR_H_INCLUDED
#define KS_STDCONNECTIONMGR_H_INCLUDED

#include "ks/connectionmgr.h"

#include "plt/log.h"
#include "plt/comparable.h"
#include "plt/priorityqueue.h"
#include "ks/xdrmemstream.h"


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
struct _KsConnectionItem : public _PltDLinkedListNode {
public: // oh, M$ is sooooo dumb...
    KsConnection                   *_connection;
    PltTime                         _best_before;
    KsConnection::ConnectionIoMode  _last_io_mode;
    int                             _fd; // optimization...

    _KsConnectionItem() : _connection(0),
                          _last_io_mode(KsConnection::CNX_IO_DORMANT),
                          _fd(-1) { }
}; // struct _KsConnectionItem


#if PLT_CNX_MGR_USE_HT
// ---------------------------------------------------------------------------
// As some "new technology" uses crude handles instead of file descriptors,
// there's no easy mapping between fds and connection items. So we're forced
// to use a hash table instead, which is made of the following class.
//
struct _KsCnxHashTableItem {
public: // oh, M$ is sooooo dumb...
    _KsConnectionItem **_overflow_table; /* sorted table w/ references      */
    int                 _size;           /* # of allocated entries in table */
    int                 _used;           /* # of used entries               */
    
    _KsCnxHashTableItem() : _overflow_table(0), _size(0), _used(0) { }
}; // class _KsCnxHashTableItem
#endif


// ---------------------------------------------------------------------------
// The "Standard" Connection Manager Itself(sm). It manages the whole mess
// of glory connections.
//
class KsStdConnectionManager : public KsConnectionManager {
public:
    KsStdConnectionManager();
    virtual ~KsStdConnectionManager();

    // I/O and event management
    virtual bool hasPendingEvents() const;
    virtual bool servePendingEvents(KsTime timeout = KsTime());
    virtual bool servePendingEvents(KsTime *pTimeout);

    // Timeout event management
    virtual bool addTimerEvent(KsTimerEvent *event);
    virtual bool removeTimerEvent(KsTimerEvent *event);
    virtual KsTimerEvent *removeNextTimerEvent();
    virtual const KsTimerEvent *peekNextTimerEvent() const;
    
    //
    virtual bool shutdownConnections(long secs);

    // connection management
    virtual bool addConnection(KsConnection &con);
    virtual bool removeConnection(KsConnection &con);
    virtual bool resetConnection(KsConnection &con);
    virtual bool trackConnection(KsConnection &con);

    // damned lies & statistics
    virtual unsigned int getConnectionCount();
    virtual unsigned int getIoErrorCount();
    virtual unsigned int getIoRxErrorCount();
    virtual unsigned int getIoTxErrorCount();



protected:

    // internal timeout handling
    PltTime getEarliestTimeout() const;
    PltTime getEarliestTimeoutSpan() const;
    void processTimeout();
    bool mayHaveTimeout() const;

    //
    int serveRequests(const KsTime *pTimeout);

    // internal file descriptor sets handling
    int getFdSets(fd_set &readables, fd_set &writeables) const;
    int getReadyFds(fd_set &read_fds, fd_set &write_fds, 
		    size_t numfds, const KsTime *pto) const;

    // process incomming and outgoing data...
    int processConnections(fd_set &readables, fd_set &writeables);

    // connections waiting to be served...or being served...
    KsConnection *getNextServiceableConnection();

    // connection management
    KsConnection *lookupConnection(int fd);


    friend class KsConnection; // TODO!!!
    void trackCnxIoMode(_KsConnectionItem &item, 
                        KsConnection::ConnectionIoMode ioMode);
        
    PltPriorityQueue< PltPtrComparable<KsTimerEvent> > _timer_queue;    

    _KsConnectionItem *getConnectionItem(int fd);

    bool                _is_ok;
    
    int                 _fdset_size;  // size of fd_sets on this host
    _KsConnectionItem  *_connections; // the connections per fd
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
    _KsCnxHashTableItem  *_hash_table;
    int                   _hash_table_size;
    int                   _hash_table_mask;
    
    unsigned int getHash(int fd) const { return fd * 625842579; } // TODO!
#endif

    static KsConnectionManager *the_cnx_manager;

}; // class KsConnectionManager


#endif

/* End of stdconnectionmgr.h */
