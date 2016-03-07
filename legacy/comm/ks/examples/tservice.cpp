/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Markus Juergens <markusj@plt.rwth-aachen.de> 
 *
 *  Test file for KscServer::requestService() and related functions.
 *
 */

//////////////////////////////////////////////////////////////////////

#include <iostream.h>
#include <plt/debug.h>
#include <plt/list.h>
#include <plt/hashtable.h>
#include "ks/serviceparams.h"
#include "ks/commobject.h"
#include "ks/path.h"
#include "ks/client.h"
#include "ks/list.h"

#include "ext_sp.h"


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

void
err_msg(const char *msg)
{
    cout << msg << endl;
}

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

/////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////

int 
main(int, char **)
{
  KsString serverloc("//terra/tserver");
  KsString path1("/vendor");
  
  KsGetPPParams gpp;
  gpp.path = path1;
  gpp.type_mask = KS_OT_ANY;
  gpp.name_mask = "*";

  KsGetPPResult gpr;

  // connect to server in order to create a proxy object
  //
  KscVariable var(serverloc + "/vendor/server_name");
  if(!var.getProjPropsUpdate()) {
    cout << "cannot read variable" << endl;
  }
  KscServerBase *pserver = var.getServer();

  // now test requestService() with getPP
  //
  if( pserver->requestService("ks_core", 1, 0, &gpp, &gpr) ) {
    cout << "request succeeded" << endl;
    cout << "results: " << gpr.result << endl;
    PltListIterator<KsProjPropsHandle> *pit = gpr.items.newIterator();
    while(*pit) {
      KsProjPropsHandle temp(**pit);
      print_pp(temp);
      ++(*pit);
    }
    delete pit;
  } else {
    cout << "request failed" << endl;
    cout << "error code : " << pserver->getLastResult() << endl;
  }
  
  // test requestService() with extension "ks_test"
  //
  KsCreateParams cop;
  cop.name = "new_var";
  KsCreateResult cor;

  if( pserver->requestService("ks_test", KS_CREATEOBJECT_MINOR_OPCODE,
                              0, &cop, &cor) ) {
    cout << "request succeeded" << endl;
    cout << "result: " << cor.result << endl;
  } else {
    cout << "request failed" << endl;
    cout << "error code : " << pserver->getLastResult() << endl;
  }

  return 0;
}

//////////////////////////////////////////////////////////////////////

#if PLT_COMPILER_BORLAND
#include "ks/array_builtins.h"
#endif

#if PLT_INSTANTIATE_TEMPLATES
#include "tservice_inst.h"
#endif









