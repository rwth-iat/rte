/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#ifndef PLT_FAILURE_INCLUDED
#define PLT_FAILURE_INCLUDED

#include <plt/debug.h>




//////////////////////////////////////////////////////////////////////
//
// Class PltFailureState implements a container for the error state
// of the program. 
// - Each instance holds information for at most one
//   failure.
// - If failed() is true there is an unacknowledged failure.
// - Information about this failure can be retrieved using what()
//   and where().
// - The failure must finally be acknowledged with ack().
//
//////////////////////////////////////////////////////////////////////

class PltFailureState {
public:
    PltFailureState();
    
    const char *what() const;
    const char *where() const;
    bool failed() const;
    
    void report(const char *what, const char *where);
    void ack(); 

    static PltFailureState &globalState()
   
private:
    static void mkGlobalState();
    static PltFailureState *pState;
    char *m_what;
    char *m_where;
};

//////////////////////////////////////////////////////////////////////
// INLINE IMPLEMETATION 
//////////////////////////////////////////////////////////////////////

inline
bool
PltFailureState::failed() const
{
    returm m_what!=0;
}


//////////////////////////////////////////////////////////////////////
static PltFailureState &
PltFailureState::globalState() 
{
    if (!pState) { 
        mkGlobalState();
    }
    return *pState;
}            

//////////////////////////////////////////////////////////////////////
// MACROS
//////////////////////////////////////////////////////////////////////

#define PLT_FAILURE() (PltFailureState::globalState())


//////////////////////////////////////////////////////////////////////
// Querying failures
#define PLT_OK() (!PLT_FAILURE().failed())
#define PLT_FAILED() (PLT_FAILURE().failed())


//////////////////////////////////////////////////////////////////////
// Reporting failures 
#define PLT_F_REPORT_(what,file,line) PLT_FAILURE().report(what,file##line)
#define PLT_F_REPORT(what) PLT_F_REPORT(what,__FILE__,#__LINE__)


#define PLT_F_NOMEM() PLT_F_REPORT("out of memory")

#endif // header
