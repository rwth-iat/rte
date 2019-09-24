/*
*   The Framework was generated by the ACPLT/OV Builder.
*
*   Builder version: 1.0.1
*   Date of file generation:  15-Apr-2002 (12:30:51)
*/

#ifndef OV_COMPILE_LIBRARY_dynov
#define OV_COMPILE_LIBRARY_dynov
#endif


#include "dynov.h"
#include "ov_association.h"

#include "ov_macros.h"

OV_DLLFNCEXPORT OV_RESULT dynov_assocaccess_link(
        const OV_PPT_dynov_assocaccess                 pparent,
        const OV_CPT_dynov_assocaccess                 pchild,
        const OV_PLACEMENT_HINT            parenthint,
        const OV_CPT_dynov_assocaccess                 prelparent,
        const OV_PLACEMENT_HINT            childhint,
        const OV_CPT_dynov_assocaccess                 prelchild
) {
        return ov_association_link(passoc_dynov_assocaccess, Ov_PtrUpCast
                       (ov_object, pparent), Ov_PtrUpCast(ov_object, pchild),
                      parenthint, Ov_PtrUpCast(ov_object, prelparent),
                      childhint, Ov_PtrUpCast(ov_object, prelchild));
}

OV_DLLFNCEXPORT void dynov_assocaccess_unlink(
        const OV_PPT_dynov_assocaccess                 pparent,
        const OV_CPT_dynov_assocaccess                 pchild
) {
        ov_association_unlink(passoc_dynov_assocaccess, Ov_PtrUpCast
                      (ov_object, pparent), Ov_PtrUpCast(ov_object, pchild));
}

OV_DLLFNCEXPORT OV_ACCESS dynov_assocaccess_getaccess(
        const OV_PPT_dynov_assocaccess                 pparent,
        const OV_CPT_dynov_assocaccess                 pchild,
        const OV_TICKET                    *pticket
) {
	if (Ov_GetParent(dynov_islinkfnc, pchild)) return OV_AC_READ;
	if (Ov_GetParent(dynov_isunlinkfnc, pchild)) return OV_AC_READ;
	if (Ov_GetParent(dynov_isgetaccessor, pchild)) return OV_AC_READ;
	if (Ov_GetParent(dynov_issetaccessor, pchild)) return OV_AC_READ;
        return OV_AC_READ | OV_AC_LINKABLE | OV_AC_UNLINKABLE;
}

