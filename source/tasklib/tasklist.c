/*
*   The Framework was generated by the ACPLT/OV Builder.
*
*   Builder version: 1.0.0
*   Date of file generation:  21-Nov-2001 (10:25:25)
*/

#ifndef OV_COMPILE_LIBRARY_tasklib
#define OV_COMPILE_LIBRARY_tasklib
#endif


#include "tasklib.h"
#include "libov/ov_association.h"

#include "libov/ov_macros.h"

OV_DLLFNCEXPORT OV_RESULT tasklib_tasklist_link(
        const OV_PPT_tasklib_tasklist                 pparent,
        const OV_CPT_tasklib_tasklist                 pchild,
        const OV_PLACEMENT_HINT            parenthint,
        const OV_CPT_tasklib_tasklist                 prelparent,
        const OV_PLACEMENT_HINT            childhint,
        const OV_CPT_tasklib_tasklist                 prelchild
) {
        if (!ov_association_testpath(passoc_tasklib_tasklist, (OV_INSTPTR_ov_object)pchild, (OV_INSTPTR_ov_object)pparent))
	        return ov_association_link(passoc_tasklib_tasklist, Ov_PtrUpCast
                       (ov_object, pparent), Ov_PtrUpCast(ov_object, pchild),
                      parenthint, Ov_PtrUpCast(ov_object, prelparent),
                      childhint, Ov_PtrUpCast(ov_object, prelchild));
        return OV_ERR_GENERIC;
}

OV_DLLFNCEXPORT void tasklib_tasklist_unlink(
        const OV_PPT_tasklib_tasklist                 pparent,
        const OV_CPT_tasklib_tasklist                 pchild
) {
        ov_association_unlink(passoc_tasklib_tasklist, Ov_PtrUpCast
                      (ov_object, pparent), Ov_PtrUpCast(ov_object, pchild));
}

OV_DLLFNCEXPORT OV_ACCESS tasklib_tasklist_getaccess(
        const OV_PPT_tasklib_tasklist                 pparent,
        const OV_CPT_tasklib_tasklist                 pchild,
        const OV_TICKET                    *pticket
) {
        if (!ov_association_testpath(passoc_tasklib_tasklist, (OV_INSTPTR_ov_object)pchild, (OV_INSTPTR_ov_object)pparent))
		return OV_AC_READ | OV_AC_LINKABLE | OV_AC_UNLINKABLE;
	return OV_AC_READ | OV_AC_UNLINKABLE;
}

