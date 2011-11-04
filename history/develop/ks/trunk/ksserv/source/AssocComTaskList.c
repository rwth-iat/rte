#ifndef OV_COMPILE_LIBRARY_ksserv
#define OV_COMPILE_LIBRARY_ksserv
#endif


/**
 * Assoc from the RootComTask to the various ComTask implementation instances.
*/

#include "ksserv.h"
#include "libov/ov_association.h"

#include "libov/ov_macros.h"

OV_DECL_LINK(ksserv_AssocComTaskList) {
        return ov_association_link(passoc_ksserv_AssocComTaskList, Ov_PtrUpCast
                       (ov_object, pparent), Ov_PtrUpCast(ov_object, pchild),
                      parenthint, Ov_PtrUpCast(ov_object, prelparent),
                      childhint, Ov_PtrUpCast(ov_object, prelchild));
}

OV_DECL_UNLINK(ksserv_AssocComTaskList) {
        ov_association_unlink(passoc_ksserv_AssocComTaskList, Ov_PtrUpCast
                      (ov_object, pparent), Ov_PtrUpCast(ov_object, pchild));
}

OV_DECL_GETACCESS(ksserv_AssocComTaskList) {
        return OV_AC_READ | OV_AC_LINKABLE | OV_AC_UNLINKABLE;
}

