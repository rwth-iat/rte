/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#ifndef PLT_COMPARABLE_INCLUDED
#define PLT_COMPARABLE_INCLUDED

#include <plt/debug.h>


//////////////////////////////////////////////////////////////////////

class PltComparable {
public:
	virtual bool operator <  (const PltComparable &) const = 0;
    virtual bool operator == (const PltComparable &) const;
    virtual bool operator != (const PltComparable &) const;
    virtual bool operator >  (const PltComparable &) const;
    virtual bool operator <= (const PltComparable &) const;
    virtual bool operator >= (const PltComparable &) const;
};

//////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

inline bool
PltComparable::operator == (const PltComparable &r) const
{
    return ! (*this != r);
}

//////////////////////////////////////////////////////////////////////

inline bool
PltComparable::operator != (const PltComparable &r) const
{
    return (*this < r) || (r < *this);
}

//////////////////////////////////////////////////////////////////////

inline bool
PltComparable::operator > (const PltComparable &r) const
{
    return (r < *this);
}

//////////////////////////////////////////////////////////////////////

inline bool
PltComparable::operator <= (const PltComparable &r) const
{
    return !(r < *this);
}

//////////////////////////////////////////////////////////////////////

inline bool
PltComparable::operator >= (const PltComparable &r) const
{
    return !(*this < r);
}

//////////////////////////////////////////////////////////////////////

#endif
