/*
*	KS-Includes
*	-----------
*/
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

#include "ks/server.h"
#include "ks/simpleserver.h"
#include "plt/list_impl.h"
#include "plt/log.h"
#include "ks/histdomain.h"
#include "ks/objmgrparams.h"


extern "C" void sighandler(int) {
    KsServerBase::getServerObject().downServer();
}


/* --------------------------------------------------------------------- */
class testDomIterator
: public KssDomainIterator
{
public:
#if PLT_RETTYPE_OVERLOADABLE
    typedef testDomIterator THISTYPE;
#define testDomIterator_THISTYPE \
    testDomIterator
#else
#define testDomIterator_THISTYPE \
        testDomIterator_THISTYPE
#endif
            
    testDomIterator() {
fprintf(stderr, " Constructor testDomIterator\n");
        toStart();
    }
    virtual ~testDomIterator() { }
    
    virtual operator bool () const
        { return _count < _lid.size(); }
    
    virtual THISTYPE & operator ++ ()
        { ++_count; return *this; }
    
    virtual void toStart() {
    
fprintf(stderr, " toStart() testDomIterator\n");

        // Alles auf 0.
        _count = 0;
        while(_lid.size()) {
            _lid.removeFirst();
        }
        _lid.addLast(PltString("dom1"));
        
    }
    virtual KssCommObjectHandle operator * () const;
    
    PltList<PltString> _lid;    // Liste aller Tabellen oder Sichten
    unsigned int      _count;  // Zaehler
};

/* --------------------------------------------------------------------- */
class itemDomIterator
: public KssDomainIterator
{
public:
#if PLT_RETTYPE_OVERLOADABLE
    typedef itemDomIterator THISTYPE;
#define itemDomIterator_THISTYPE \
    itemDomIterator
#else
#define itemDomIterator_THISTYPE \
        itemDomIterator_THISTYPE
#endif
            
    itemDomIterator() {
fprintf(stderr, "  Constructor itemDomIterator\n");
        toStart();
    }
    virtual ~itemDomIterator() { }
    
    virtual operator bool () const
        { return FALSE; }
    
    virtual THISTYPE & operator ++ ()
        { ++_count; return *this; }
    
    virtual void toStart() {
    
fprintf(stderr, "  toStart() itemDomIterator\n");

        // Alles auf 0.
        _count = 0;
    }
    virtual KssCommObjectHandle operator * () const;
    
    unsigned int      _count;  // Zaehler
};

/* --------------------------------------------------------------------- */
class domRoot : public KssDomain
{
public:
    domRoot() {};
    ~domRoot() { }
    
    KsString getIdentifier() const
        { return KsString("testRoot") ; }
    
    KsTime getCreationTime() const
        { return KsTime::now(); }
    
    KsString getClassIdentifier() const
        { return KsString(); }
    
    KsString getComment() const
        { return KsString("Test Root-Domain"); }
    
    virtual KS_SEMANTIC_FLAGS getSemanticFlags() const
        { return 0; }
    
    virtual KssCommObjectHandle getChildById(const KsString &id) const;
    
    KssDomainIterator_THISTYPE * newIterator() const
    {
        return new testDomIterator();
    }
};

/* --------------------------------------------------------------------- */
class itemDom : public KssDomain
{
public:
    itemDom(KsString _id) {
fprintf(stderr, "    Constructor itemDom\n");
        _name = _id;
    }
    ~itemDom() { }
    
    KsString getIdentifier() const
        { return _name ; }
    
    KsTime getCreationTime() const
        { return KsTime::now(); }
    
    KsString getClassIdentifier() const
        { return KsString(); }
    
    KsString getComment() const
        { return KsString("Test Item-Domain"); }
    
    virtual KS_SEMANTIC_FLAGS getSemanticFlags() const
        { return 0; }
    
    virtual KssCommObjectHandle getChildById(const KsString &id) const {
        return KssCommObjectHandle(0, KsOsNew);
    }
    
    KssDomainIterator_THISTYPE * newIterator() const
    {
        return new itemDomIterator();
    }
    
    KsString _name;
};


/* --------------------------------------------------------------------- */
KssCommObjectHandle
domRoot::getChildById(const KsString &id) const
{
    // Ident id suchen

    int  i;
    PltList<PltString> _lid;
    
fprintf(stderr, "   domRoot: getChildById(%s)\n", (const char*)id);

    _lid.addLast(PltString("dom1"));

    while( _lid.size() ) {
        if( id == _lid.removeFirst() ) {
            itemDom *pRT = new itemDom(id);
            return KssCommObjectHandle(pRT, KsOsNew);
        }
    }
    
    return KssCommObjectHandle(0, KsOsNew);
}


KssCommObjectHandle
testDomIterator::operator * () const
{
    unsigned int i;

fprintf(stderr, "  testDomIterator: Operator *\n");
    
    PltListIterator<PltString> *id =
        PLT_RETTYPE_CAST((PltListIterator<PltString> *)) _lid.newIterator() ;
    
    for(i=0; i<_count; i++) {
        ++(*id);
    }
    
    KsString ident(**id);
    delete id;
    
    itemDom * pRT = new itemDom(ident);

    return KssCommObjectHandle(pRT, KsOsNew);
}
KssCommObjectHandle
itemDomIterator::operator * () const
{
    return KssCommObjectHandle(0, KsOsNew);
}

/* --------------------------------------------------------------------- */
class tstServer
: public KsServer,
  public KsSimpleServer
{	
//    friend void handler(int);
public:
    tstServer();
    ~tstServer() {};

    //// accessors
    virtual KsString getServerName() const
        { return KsString("tstSrv"); }
    virtual KsString getServerDescription() const {
        KsString lst("Test server");
        
        return lst;
    }
    virtual KsString getServerVersion() const { 
        return KsString("0.1"); 
    }
    virtual KsString getVendorName () const {
        return KsString("Tst"); 
    }

};

tstServer::tstServer()
: KsServer(30)
{
    KsAvNoneTicket::setDefaultAccess(KS_AC_READ | KS_AC_WRITE);
    initVendorTree();

    // "Test"-Domain erzeugen
    KssCommObject * pL = new domRoot();
    KssCommObjectHandle hL(pL, KsOsNew);
    addCommObject(KsPath("/"), hL);
}



/*****************************************************************************/
int main()
/*****************************************************************************/
{


    signal(SIGTERM, sighandler);
    signal(SIGINT, sighandler);

    tstServer Server;
    
    
    Server.startServer();
    Server.run();
    Server.stopServer();
   
    return 0;                             
} /* main() */
