/* -*-plt-c++-*- 
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#include <stdlib.h>
#include <iostream.h>

#include <plt/debug.h>
#include <plt/failure.hpp>



//////////////////////////////////////////////////////////////////////

static PltFailureState *
PltFailureState::pState = 0;

static void
PltFailureState::mkGlobalState() 
{
    pState = new PltFailureState;
    if (!pState) {
        cerr << "FATAL: No memory to make failure state object." << endl;
        exit(EXIT_FAILURE);
    }
}

//////////////////////////////////////////////////////////////////////

PltFailureState::PltFailureState()
: m_what(0),
  m_where(0)
{
}

//////////////////////////////////////////////////////////////////////

const char *
PltFailureState::what() const
{
    PLT_PRECONDITION( failed() );
    return m_what;
}

//////////////////////////////////////////////////////////////////////

const char *
PltFailureState::where() const
{
    PLT_PRECONDITION( failed() );
    return m_where;
}

//////////////////////////////////////////////////////////////////////

void
PltFailureState::report(const char *what, const char *where) 
{
    if (failed()) {
        // Double fault
        clog << "Double failure" << endl;
        abort();
    }
    clog 
        << "Failure reported: "
        << what 
        << " at: " 
        << where
        << endl;
    m_what = what;
    m_where = where;
}

//////////////////////////////////////////////////////////////////////

void 
PltFailureState::ack()
{
    m_what = 0;
    m_where = 0;
}
        
    
