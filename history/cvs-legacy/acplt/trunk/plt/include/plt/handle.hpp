/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */
#ifndef PLT_HANDLE_INCLUDED
#define PLT_HANDLE_INCLUDED

#include <limits.h>

#include <plt/debug.h>

template<class T>
class PltHandle : virtual public PltDebuggable {
public:
    PltHandle();
    PltHandle(const PltHandle &);
    ~PltHandle();
    PltHandle& operator=(PltHandle &);

    // accessors
    bool isBound() const;
    T& operator*() const;
    T* operator->() const;

    // modifiers
    bool bindTo(T *&);

#if PLT_DEBUG_INVARIANTS
protected:
    virtual bool invariant() const;
#endif
private:
    void addRef();
    void removeRef();

    unsigned *pcount;
    T *prep;
};


//////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////

template<class T>
bool
PltHandle<T>::isBound() const
{
    return prep != 0; // and pcount == 0
}


//////////////////////////////////////////////////////////////////////

#if PLT_DEBUG_INVARIANTS
template<class T>
bool 
PltHandle<T>::invariant() const 
{
    return (prep==0 && pcount==0) 
        || (prep && pcount && *pcount>0);
}
#endif

//////////////////////////////////////////////////////////////////////
// Add one reference to the handled object

template<class T>
void PltHandle<T>::addRef()
{
    PLT_PRECONDITION(isBound() && *pcount<UINT_MAX);
    (*pcount)++;
    PLT_CHECK_INVARIANT();
}


//////////////////////////////////////////////////////////////////////
// Remove one reference to the handled object.

template<class T>
void PltHandle<T>::removeRef()
{
    PLT_PRECONDITION( isBound() );
    if ( --(*pcount) == 0) {
        delete prep; prep = 0;
        delete pcount; pcount = 0;
    }
}


//////////////////////////////////////////////////////////////////////
// Bind to a new object. You can bind only once and only to valid
// objects! If binding succeeds p is set to 0, because it is not
// longer valid now. Returns true on success.

template <class T>
bool
PltHandle<T>::bindTo(T* &p)
{
    bool ok;
    PLT_PRECONDITION( p && !isBound() );
    assert(prep == 0 && pcount == 0);

    pcount = new unsigned(1);

    if (pcount) {
        prep = p;
        p = 0;
        ok = true;
    } else {
// TODO        PLT_LOG_NOMEM("PltHandle::bindTo");
        ok = false;
    }
    PLT_CHECK_INVARIANT();
    return ok;
}
        
//////////////////////////////////////////////////////////////////////
// Create a PltHandle

template<class T>
PltHandle<T>::PltHandle() 
: prep(0), 
  pcount(0)
{
    PLT_CHECK_INVARIANT();
}

//////////////////////////////////////////////////////////////////////
// Copy constructor. The handled object -- if any -- gets an 
// additional reference.

template<class T>
PltHandle<T>::PltHandle(const PltHandle &h)
: prep(h.prep), pcount(h.pcount)
{
    if (h.isBound()) {
        addRef();
    }
    PLT_CHECK_INVARIANT();
}

//////////////////////////////////////////////////////////////////////
// Destructor. If the handle is bound it removes one reference 
// from the handled object.

template<class T>
PltHandle<T>::~PltHandle()
{
    if (isBound()) {
        removeRef();
    }
}

//////////////////////////////////////////////////////////////////////
//  Assignment
template<class T>
PltHandle<T> & PltHandle<T>::operator=(PltHandle &rhs) 
{
    // order is important if called as 'handle = handle;'
    if (rhs.isBound()) {
        rhs.addRef();
    }
    if (isBound()) {
        removeRef();
    }

    pcount = rhs.pcount;
    prep = rhs.prep;

    PLT_CHECK_INVARIANT();
    return *this;
}


//////////////////////////////////////////////////////////////////////
// Derefencing. Note that it is ILLEGAL to hold a pointer/reference
// to the representation!

template<class T>
T * PltHandle<T>::operator->() const
{
    return isBound() ? prep : 0;
}

//////////////////////////////////////////////////////////////////////
// Derefencing. Note that it is ILLEGAL to hold a pointer/reference
// to the representation!

template<class T>
T & PltHandle<T>::operator*() const
{
    PLT_PRECONDITION( isBound() );
    return *(operator->());
}

#endif // Header
