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

#include <iostream.h>
#include <plt/debug.h>
#include <plt/list.h>
#include <plt/hashtable.h>
#include "ks/commobject.h"
#include "ks/path.h"

#if PLT_COMPILER_GCC
#include "plt/priorityqueue_impl.h"
#include "plt/hashtable_impl.h"
#include "ks/array_impl.h"
#include "ks/handle_impl.h"
#include "ks/list_impl.h"
#endif

#if PLT_COMPILER_BORLAND
#include "plt/priorityqueue.h"
#include "plt/hashtable.h"
#include "ks/array.h"
#include "ks/array_builtins.h"
#include "ks/handle.h"
#include "ks/list.h"
#endif

//////////////////////////////////////////////////////////////////////

typedef void (*ExecuteFunction)();
PltHashTable<PltString,ExecuteFunction> dispatcher_table;

PltString host_name, server_name, current_path;
KscDomain *current_domain = 0;

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

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void
execute_nothing()
{}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void 
execute_open()
{
    if(current_domain) {
        delete current_domain;
    }
    
    char buf[256];
    cin >> buf;
    PltString temp("//");
    temp += buf;
    temp += "/";
    cin >> buf;
    temp += buf;
    temp += "/";
    
    current_domain = new KscDomain(temp);
    
    check_pointer(current_domain);
    
    if(!current_domain->getProjPropsUpdate()) {
        err_msg("cannot open root domain");
        delete current_domain;
        current_domain = 0;
    } else {
        current_path = "/";
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void 
execute_cd() 
{
    if(!current_domain) {
        err_msg("first open a server");
        return;
    }

    char buf[256];
    cin >> buf;

    PltString cdTo(buf);

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
            cout << "VAR\t" 
                 << sz_access_modes[vpp->access_mode]
                 << sz_var_type(vpp->type)
                 << vpp->identifier 
                 << endl;
        }
        break;
    case KS_OT_DOMAIN:
        {
            cout << "DOMAIN\t" 
                 << sz_access_modes[hpp->access_mode]
                 << hpp->identifier << endl;
        }
        break;
    default:
        {
            err_msg("Unknown type of projected properties");
        }
    }
}
 
void
execute_ls()
{
    if(!current_domain) {
        err_msg("no current directory - first open a server");
        return;
    }

    KscChildIterator *child_it = 
        current_domain->newChildIterator(KS_OT_ANY);

    check_pointer(child_it);

    while(*child_it) {
        print_pp(**child_it);
        ++(*child_it);
    }

    delete child_it;
}

//////////////////////////////////////////////////////////////////////

void
print_value(KsValueHandle hv) {
    hv->debugPrint(cout);
    cout << endl;
}

void
execute_get_var()
{
    char buf[256];
    cin >> buf;
    PltString var_name(buf);

    if( !current_domain ) {
        err_msg("first open a server");
        return;
    }

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

    PltString var_path(current_domain->getFullPath());
    var_path += "/";
    var_path += var_name;

    KscVariable var(var_path);

    if(!var.getProjPropsUpdate()) {
        err_msg("cannot read PP update");
        return;
    }

    if(!(var.getProjProps()->access_mode & KS_AC_READ)) {
        err_msg("var is not readable");
        return;
    }

    if(!var.getUpdate()) {
        err_msg("cannot read CP update");
        return;
    }

    print_value(var.getValue());
}

    
//////////////////////////////////////////////////////////////////////

void
init_dispatcher_table()
{
    dispatcher_table.add("", execute_nothing);
    dispatcher_table.add("open", execute_open);
    dispatcher_table.add("ls", execute_ls);
    dispatcher_table.add("cd", execute_cd);
    dispatcher_table.add("getvar", execute_get_var);
}

//////////////////////////////////////////////////////////////////////

void 
start_shell()
{
    bool fContinue = true;
    char buf[256];

    init_dispatcher_table();

    while(fContinue) {
        if(current_domain) {
            cout << current_domain->getName() << ">";
        } else {
            cout << "(nowhere)>";
        }
        cin >> buf;
        
        PltString cmd(buf);

        if(cmd == "quit") {
            fContinue = false;
        } else {
            ExecuteFunction func;
            if(dispatcher_table.query(cmd,func)) {
                func();
            } else {
                err_msg("unknown command");
            }
        }


    }
}
 
//////////////////////////////////////////////////////////////////////

int 
main(int, char **)
{
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

#include "plt/hashtable_impl.h"
#include "plt/handle_impl.h"
#include "tshell_inst.h"









