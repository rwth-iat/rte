/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Markus Juergens <markusj@plt.rwth-aachen.de> 
 *
 *  This file provides an example for the use of the classes 
 *  KscVariable and KscDomain. A simple shell is implemented which allows
 *  browsing through a server.
 *
 */

//////////////////////////////////////////////////////////////////////

#include <ctype.h>
#include <iostream.h>
#include <iomanip.h>
#include <plt/debug.h>
#include <plt/list.h>
#include <plt/hashtable.h>
#include <plt/dictionary.h>
#include <plt/key.h>
#include "ks/commobject.h"
#include "ks/client.h"
#include "ks/path.h"


#if PLT_SEE_ALL_TEMPLATES
#include "plt/priorityqueue.h"
#include "plt/hashtable.h"
#include "ks/array.h"
#include "ks/handle.h"
#include "ks/list.h"
#else
#include "plt/priorityqueue_impl.h"
#include "plt/hashtable_impl.h"
#include "ks/array_impl.h"
#include "ks/handle_impl.h"
#include "ks/list_impl.h"
#endif

#if PLT_SYSTEM_NT
#include <time.h>
#endif

//////////////////////////////////////////////////////////////////////

const PltTime default_timeout(60, 0);
const size_t default_tries = 1;

typedef void (*ExecuteFunction)(PltList<PltString> &args);
PltHashTable<PltString,ExecuteFunction> dispatcher_table;

PltString host_name, server_name, current_path;
KscDomain *current_domain = 0;

/////////////////////////////////////////////////////////////////////////////

class PltIntKey
{
public:
    PltIntKey(KS_RESULT key) : k(key) {}

    unsigned long hash() const { return k; }
    bool operator == (const PltIntKey &rhs) const { return k == rhs.k; }

    KS_RESULT k;
};

PltHashTable< PltIntKey, PltString > error_table;

//////////////////////////////////////////////////////////////////////

void
err_msg(const char *msg)
{
    cout << msg << endl;
}

//////////////////////////////////////////////////////////////////////

void 
check_pointer(void *p) 
{
    if(!p) {
        err_msg("out of memory");
        exit(-2);
    }
}

/////////////////////////////////////////////////////////////////////////////

PltString
get_error_str(KS_RESULT err) 
{
    PltString str;

    if( error_table.query(err, str) ) {
        return str;
    } else {
        return PltString("unknown error code");
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void
execute_nothing(PltList<PltString> &)
{}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void 
execute_open(PltList<PltString> &args)
{
    if(current_domain) {
        delete current_domain;
    }
    
    if( args.size() != 2 ) {
        err_msg("Syntax: open host server");
        return;
    }

    PltString temp("//");
    temp += args.removeFirst();
    temp += "/";
    temp += args.removeFirst();
    temp += "/";

    current_domain = new KscDomain(temp);
    
    check_pointer(current_domain);

    if( !current_domain->hasValidPath() ) {
        err_msg("Syntax: open host server");
        return;
    }
    
    if(!current_domain->getProjPropsUpdate()) {
        PltString msg("cannot open root domain - ",
                      get_error_str(current_domain->getLastResult()));
        err_msg(msg);
        delete current_domain;
        current_domain = 0;
    } else {
        current_path = "/";
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void 
execute_cd(PltList<PltString> &args) 
{
    if(!current_domain) {
        err_msg("first open a server");
        return;
    }

    if( args.size() != 1 ) {
        err_msg("Syntax: cd <relative path>");
        return;
    }
    
    PltString cdTo = args.removeFirst();

    if( cdTo == ".." ) {
        // handle this case special
        //
        KsString new_path(current_domain->getHostAndServer(),
                          current_domain->getPathOnly());
        KscDomain *new_domain = new KscDomain(new_path);
        check_pointer(new_domain);
        if(new_domain->hasValidPath()) {
            delete current_domain;
            current_domain = new_domain;            
        } else {
            err_msg("error in changing path");
        }
        return;
    }
        
    KscChildIterator *child_it =
        current_domain->newChildIterator(KS_OT_DOMAIN);

    check_pointer(child_it);

    // check wether desired domain exists
    //
    bool fExists = false;
    while(*child_it && !fExists) {
        if( (**child_it)->identifier == cdTo ) {
            fExists = true;
        }
        ++(*child_it);
    }

 
    if(fExists) {
        PltString temp(current_domain->getFullPath());
        if(temp[temp.len()-1] != '/') {
            temp += "/";
        }
        temp += cdTo;
        KscDomain *new_domain = new KscDomain(temp);
        if(new_domain) {
            delete current_domain;
            current_domain = new_domain;
        }
    } else {
        err_msg("path does not exist");
    }

    delete child_it;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

const char *
sz_var_type(KS_VAR_TYPE vt)
{
    switch(vt) {
    case KS_VT_VOID :
        return "void\t\t";
        break;
    
    case KS_VT_BOOL :
        return "bool\t\t";
        break;

    case KS_VT_INT :
        return "int\t\t";
        break;

    case KS_VT_UINT :
        return "u_int\t\t";
        break;

    case KS_VT_SINGLE :
        return "single\t\t";
        break;

    case KS_VT_DOUBLE :
        return "double\t\t";
        break;

    case KS_VT_STRING :
        return "string\t\t";
        break;

    case KS_VT_TIME :
        return "time\t\t";
        break;

    case KS_VT_BYTE_VEC :
        return "byte vec\t";
        break;

    case KS_VT_BOOL_VEC :
        return "bool vec\t";
        break;

    case KS_VT_INT_VEC :
        return "int vec\t\t";
        break;

    case KS_VT_UINT_VEC :
        return "u_int vec\t";
        break;

    case KS_VT_SINGLE_VEC :
        return "single vec\t";
        break;

    case KS_VT_DOUBLE_VEC :
        return "double vec\t";
        break;

    case KS_VT_STRING_VEC :
        return "string vec\t";
        break;

    case KS_VT_TIME_VEC :
        return "time vec\t";
        break;

    default :
        return "unknown\t\t";
        break;
    }
}

void 
print_pp(KsProjPropsHandle hpp)
{
    static char sz_access_modes[][7] = {
        "None\t",
        "Read\t",
        "Write\t",
        "RW\t"
    };


    switch(hpp->xdrTypeCode()) {
    case KS_OT_VARIABLE: 
        {
            const KsVarProjProps *vpp =
                PLT_DYNAMIC_PCAST(KsVarProjProps,hpp.getPtr());
            if(!vpp) {
                err_msg("internal type error");
                return;
            }
            cout << setw(8) << "VAR" 
                 << setw(15) << vpp->identifier       
                 << setw(8) << sz_access_modes[vpp->access_mode]
                 << setw(10) << sz_var_type(vpp->type)
                 << vpp->comment
                 << endl;
        }
        break;
    case KS_OT_DOMAIN:
        {
            cout << setw(8) << "DOMAIN" 
                 << setw(15) << hpp->identifier 
                 << setw(8) << sz_access_modes[hpp->access_mode]
                 << hpp->comment
                 << endl;
        }
        break;
    default:
        {
            err_msg("Unknown type of projected properties");
        }
    }
}
 
void
execute_ls( PltList<PltString> &args )
{
    if(!current_domain) {
        err_msg("no current directory - first open a server");
        return;
    }

    KscChildIterator *child_it;

    if( args.isEmpty() ) {
        child_it = 
            current_domain->newChildIterator(KS_OT_ANY);
    } else {
        child_it = 
            current_domain->newChildIterator(KS_OT_ANY,
                                             args.removeFirst());
    }

    if( !child_it ) {
        if( current_domain->getLastResult() == KS_ERR_OK ) {
            cout << "Empty domain" << endl;
        } else {
            cout << "Communication error : " 
                 << get_error_str(current_domain->getLastResult())
                 << endl;
        }
        return;
    }

    while(*child_it) {
        print_pp(**child_it);
        ++(*child_it);
    }

    delete child_it;
}

//////////////////////////////////////////////////////////////////////

void
print_value(KsValueHandle hv) {
    if(!hv) {
        cout << "no value" << endl;
        return;
    }

    switch(hv->xdrTypeCode()) {
    case KS_VT_VOID : 
        {
            cout << "void" << endl;
        } 
        break;
    case KS_VT_BOOL :
        {
            KsBoolValue *pBool =
                PLT_DYNAMIC_PCAST(KsBoolValue, hv.getPtr());
            check_pointer(pBool);
            cout << "Bool " << ((bool)(*pBool) ? "true" : "false") << endl;
        }
        break;
    case KS_VT_INT : 
        {
            KsIntValue *pInt =
                PLT_DYNAMIC_PCAST(KsIntValue, hv.getPtr());
            check_pointer(pInt);
            cout << "Int " << (long)(*pInt) << endl;
        }
        break;
    case KS_VT_UINT :
        {
            KsUIntValue *pUInt =
                PLT_DYNAMIC_PCAST(KsUIntValue, hv.getPtr());
            check_pointer(pUInt);
            cout << "UInt " << (u_long)(*pUInt) << endl;
        }
        break;
    case KS_VT_SINGLE :
        {
            KsSingleValue *pSingle =
                PLT_DYNAMIC_PCAST(KsSingleValue, hv.getPtr());
            check_pointer(pSingle);
            cout << "Single " << (float)(*pSingle) << endl;
        }
        break;
    case KS_VT_DOUBLE :
        {
            KsDoubleValue *pDouble =
                PLT_DYNAMIC_PCAST(KsDoubleValue, hv.getPtr());
            check_pointer(pDouble);
            cout << "Double " << (double)(*pDouble) << endl;
        }
        break;
    case KS_VT_STRING :
        {
            KsStringValue *pStr =
                PLT_DYNAMIC_PCAST(KsStringValue, hv.getPtr());
            check_pointer(pStr);
            cout << "String " << (const char *)(*pStr) << endl;
        }
        break;
    case KS_VT_TIME :
        {
            KsTimeValue *pTime =
                PLT_DYNAMIC_PCAST(KsTimeValue, hv.getPtr());
            check_pointer(pTime);
#if PLT_SYSTEM_OPENVMS
            unsigned long sec = pTime->tv_sec;
#else
            long sec = pTime->tv_sec;
#endif
            cout << "Time " << ctime(&sec) << endl;
        }
        break;
    case KS_VT_BYTE_VEC :
    case KS_VT_BOOL_VEC :
    case KS_VT_INT_VEC :
    case KS_VT_UINT_VEC :
    case KS_VT_SINGLE_VEC :
    case KS_VT_DOUBLE_VEC :
    case KS_VT_STRING_VEC :
    case KS_VT_TIME_VEC :
        {
            cout << "Vector value" << endl;
        }
        break;
    default:
        {
            cout << "unknown typecode" << endl;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////

void
execute_get_var( PltList<PltString> &args )
{
    if( args.size() != 1 ) {
        err_msg("Syntax: getvar <name>");
        return;
    }

    PltString var_name = args.removeFirst();

    if( !current_domain ) {
        err_msg("first open a server");
        return;
    }

#if 0
    // check if child exists
    //
    KscChildIterator *pit = 
        current_domain->newChildIterator(KS_OT_VARIABLE);

    check_pointer(pit);

    bool searching = true;

    while(searching && *pit) {
        if((**pit)->identifier == var_name) {
            searching = false;
        }
        ++*pit;
    }

    delete pit;
    pit = 0;

    if(searching) {
        err_msg("var does not exist");
        return;
    }
#endif

    PltString var_path(current_domain->getFullPath());
    var_path += "/";
    var_path += var_name;

    KscVariable var(var_path);
#if 0
    if(!var.getProjPropsUpdate()) {
        err_msg("cannot read PP update");
        return;
    }

    if(!(var.getProjProps()->access_mode & KS_AC_READ)) {
        err_msg("var is not readable");
        return;
    }
#endif
    if(!var.getUpdate()) {
        PltString msg("cannot read CP update - ",
                      get_error_str(var.getLastResult()));
        err_msg(msg);
        return;
    }

    print_value(var.getValue());
}


/////////////////////////////////////////////////////////////////////////////

void
execute_get_pp( PltList<PltString> &args )
{
    if( !current_domain ) {
        err_msg("first open a server");
        return;
    }

    if( args.size() == 0 ) {
        if( current_domain->getProjPropsUpdate() ) {
            KsDomainProjProps pp(*(current_domain->getProjProps()));
            print_pp( KsProjPropsHandle(&pp, KsOsUnmanaged) );
        } else {
            err_msg("Cannot read projected props");
        }
    } else if( args.size() == 1 ) {
        execute_ls(args);
    } else {
        err_msg("Syntax: get_pp\n        get_pp name");
    }
}

/////////////////////////////////////////////////////////////////////////////

void
execute_set_timeout( PltList<PltString> &args ) 
{
    PltTime timeout = default_timeout;
    size_t tries = default_tries;
 
    if( ! args.isEmpty() ) {
        PltString s = args.removeFirst();
        char *end;
        size_t temp = strtoul(s, &end, 0);
        if( end == (const char *)s || temp == 0 ) {
            err_msg("Syntax: set_timeout [time [tries]]");
            return;
        } else {
            timeout = PltTime(temp,0);

            if( ! args.isEmpty() ) {
                s = args.removeFirst();
                temp = strtoul(s, &end, 0);
                if( end == (const char *)s || temp == 0 ) {
                    err_msg("Syntax: settimeout [time_span [tries]]");
                    return;
                } else {
                    tries = temp;
                }
            }
        }
    }

    KscClient::getClient()->
        setTimeouts(timeout, timeout, tries);
    cout << "Timeout set to " << timeout.tv_sec
         << " seconds, " << tries
         << " tries will be done" 
         << endl;
}

//////////////////////////////////////////////////////////////////////

void
init_dispatcher_table()
{
    dispatcher_table.add("", &execute_nothing);
    dispatcher_table.add("open", &execute_open);
    dispatcher_table.add("ls", &execute_ls);
    dispatcher_table.add("cd", &execute_cd);
    dispatcher_table.add("getvar", &execute_get_var);
    dispatcher_table.add("getpp", &execute_get_pp);
    dispatcher_table.add("settimeout", &execute_set_timeout);
}

/////////////////////////////////////////////////////////////////////////////

void
init_error_table() 
{
    error_table.add( KS_ERR_OK, "ok" );
    error_table.add( KS_ERR_GENERIC, "generic" );
    error_table.add( KS_ERR_BADAUTH, "bad auth" );
    error_table.add( KS_ERR_UNKNOWNAUTH, "unknown auth" );
    error_table.add( KS_ERR_NOTIMPLEMENTED, "not implemented" );
    error_table.add( KS_ERR_BADPARAM, "bad param" );
    error_table.add( KS_ERR_BADNAME, "bad name" );
    error_table.add( KS_ERR_BADPATH, "bad path" );
    error_table.add( KS_ERR_BADMASK, "bad mask" );
    error_table.add( KS_ERR_NOACCESS, "no access" );
    error_table.add( KS_ERR_BADTYPE, "bad type" );
    error_table.add( KS_ERR_CANTSYNC, "cannot sync" );
    error_table.add( KS_ERR_NOREMOTE, "no remote" );
    error_table.add( KS_ERR_SERVERUNKNOWN, "server unknown" );
    error_table.add( KS_ERR_MALFORMEDPATH, "malformed path" );
    error_table.add( KS_ERR_NETWORKERROR, "network error" );
    error_table.add( KS_ERR_TYPEMISMATCH, "type mismatch" );
    error_table.add( KS_ERR_HOSTUNKNOWN, "host unknown" );
    error_table.add( KS_ERR_CANTCONTACT, "cannot contact" );
    error_table.add( KS_ERR_TIMEOUT, "timed out" );
    error_table.add( KS_ERR_NOMANAGER, "no manager" );
}

//////////////////////////////////////////////////////////////////////

void
split_line(char *line, PltList<PltString> &items)
{
    char *curr = line;
    char *last_break = line;

    while( *curr ) {
        while( *curr && !isspace(*curr) ) curr++;
        items.addLast(PltString(last_break, curr - last_break));
        while( isspace(*curr) ) curr++;
        last_break = curr;
    }
}
        
/////////////////////////////////////////////////////////////////////////////

void 
start_shell()
{
    bool fContinue = true;
    char buf[256];

    init_dispatcher_table();

    while(fContinue) {
        if(current_domain) {
            cout << current_domain->getPathAndName() << ">";
        } else {
            cout << "(nowhere)>";
        }

        cin.getline(buf, sizeof(buf));
        
        PltList<PltString> args;

        split_line(buf, args);

        if( !args.isEmpty() ) {

            PltString cmd = args.removeFirst();

            if(cmd == "quit") {
                fContinue = false;
            } else {
                ExecuteFunction func;
                if(dispatcher_table.query(cmd,func)) {
                    func(args);
                } else {
                    err_msg("unknown command");
                }
            }
        }
    }
}
 
//////////////////////////////////////////////////////////////////////

int 
main(int, char **)
{
    KscClient::getClient()->
        setTimeouts(default_timeout, default_timeout, default_tries);

    init_error_table();

    // enter main loop
    //
    start_shell();

    if(current_domain) {
        delete current_domain;
    }

    // finished
    //
    return 0;

}

//////////////////////////////////////////////////////////////////////

#if PLT_COMPILER_BORLAND
#include "ks/array_builtins.h"
#endif

#if PLT_INSTANTIATE_TEMPLATES
#include "tshell_inst.h"
#endif









