/*
*   The Framework was generated by the ACPLT/OV Builder.
*
*   Builder version: 1.0.1
*   Date of file generation:  10-Apr-2002 (17:18:25)
*/

#ifndef OV_COMPILE_LIBRARY_dynov
#define OV_COMPILE_LIBRARY_dynov
#endif


#include "dynov.h"
#include "libov/ov_macros.h"
#include "libov/ov_association.h"

OV_RESULT dynov_association_linktable_allocate(
	OV_INSTPTR_ov_class		pclass,
	OV_INT				addsize
);

void dynov_association_linktable_insert(
	OV_INSTPTR_ov_class		passocclass,
	OV_INSTPTR_ov_class		pclass,
	OV_INT				addsize,
	OV_UINT				offset
);

OV_DLLFNCEXPORT void dynov_dynassociation_uncheck(
        OV_INSTPTR_ov_object          pobj
) {
        OV_INSTPTR_ov_class      	pparentclass;
        OV_INSTPTR_ov_class      	pchildclass;
        OV_INSTPTR_ov_association 	passoc;

	passoc = Ov_DynamicPtrCast(ov_association, pobj);
	pparentclass = Ov_GetParent(ov_parentrelationship, passoc);
	pchildclass = Ov_GetParent(ov_childrelationship, passoc);

        ov_association_unload(passoc, pparentclass, pchildclass);

	passoc->v_linkfnc = 0;
	passoc->v_unlinkfnc = 0;
	passoc->v_getaccessfnc = 0;

	Ov_Unlink(ov_parentrelationship, pparentclass, passoc);
	Ov_Unlink(ov_childrelationship, pchildclass, passoc);
}

OV_DLLFNCEXPORT OV_BOOL dynov_dynassociation_check(
        OV_INSTPTR_ov_object          pobj
) {
        OV_INSTPTR_ov_class      	pparentclass;
        OV_INSTPTR_ov_class      	pchildclass;
        OV_INSTPTR_dynov_dynassociation pdynassoc;
        OV_INSTPTR_dynov_dynoperation	pdynop;
	OV_ELEMENT	 		baseelement;
	OV_ELEMENT	 		searchedelement;
	OV_RESULT			result;
        OV_VTBLPTR_dynov_dynoperation   pvtableop;

	// test of association's parameters

	pdynassoc = Ov_DynamicPtrCast(dynov_dynassociation, pobj);
	if (!pdynassoc) return FALSE;
	if (Ov_GetParent(ov_parentrelationship, pdynassoc) && Ov_GetParent(ov_childrelationship, pdynassoc)) return TRUE;
	pparentclass = Ov_GetParent(dynov_dynparentrelationship, pdynassoc);
	pchildclass = Ov_GetParent(dynov_dynchildrelationship, pdynassoc);
	if ((!pparentclass) || (!pchildclass)) return FALSE;
	if ((!(pparentclass->v_classprops & OV_CP_INSTANTIABLE)) || (!(pchildclass->v_classprops & OV_CP_INSTANTIABLE))) return FALSE;
	baseelement.elemtype = OV_ET_OBJECT;
	baseelement.pobj = Ov_PtrUpCast(ov_object, pparentclass);
	baseelement.elemunion.pobj = Ov_PtrUpCast(ov_object, pparentclass);
	ov_element_searchchild(&baseelement, &searchedelement, pdynassoc->v_parentrolename);
	if (searchedelement.elemtype != OV_ET_NONE) return FALSE;
	baseelement.elemtype = OV_ET_OBJECT;
	baseelement.pobj = Ov_PtrUpCast(ov_object, pchildclass);
	baseelement.elemunion.pobj = Ov_PtrUpCast(ov_object, pchildclass);
	ov_element_searchchild(&baseelement, &searchedelement, pdynassoc->v_childrolename);
	if (searchedelement.elemtype != OV_ET_NONE) return FALSE;
	if (!(pdynassoc->v_assoctype & (OV_AT_ONE_TO_ONE | OV_AT_ONE_TO_MANY | OV_AT_MANY_TO_MANY))) return FALSE;
	if ((pdynassoc->v_assocprops) && (!(pdynassoc->v_assocprops & OV_AP_LOCAL))) return FALSE;

	// get the association's functions

	pdynop = Ov_GetChild(dynov_islinkfnc, pdynassoc);
	if (pdynop) if (pdynop->v_executeable) {
		Ov_GetVTablePtr(dynov_dynoperation, pvtableop, pdynop);
		if (pvtableop)
			pdynassoc->v_linkfnc = (OV_FNCPTR_LINK) pvtableop->m_fnclink;
	}
	pdynop = Ov_GetChild(dynov_isunlinkfnc, pdynassoc);
	if (pdynop) if (pdynop->v_executeable)  {
		Ov_GetVTablePtr(dynov_dynoperation, pvtableop, pdynop);
		if (pvtableop)
			pdynassoc->v_unlinkfnc = (OV_FNCPTR_UNLINK) pvtableop->m_fncunlink;
	}
	pdynop = Ov_GetChild(dynov_assocaccess, pdynassoc);
	if (pdynop) if (pdynop->v_executeable)  {
		Ov_GetVTablePtr(dynov_dynoperation, pvtableop, pdynop);
		if (pvtableop)
			pdynassoc->v_getaccessfnc = (OV_FNCPTR_GETACCESS) pvtableop->m_fncaccess;
	}

	// insert the association's link space into linktables of parent and child class' instances

	if (Ov_Fail(ov_association_linktable_calculate(pdynassoc, pparentclass, pchildclass))) return FALSE;
	return TRUE;
}

OV_DLLFNCEXPORT OV_BOOL dynov_dynassociation_islinkable_get(
             OV_INSTPTR_dynov_dynassociation          pobj
) {
             return pobj->v_islinkable;
}

OV_DLLFNCEXPORT OV_RESULT dynov_dynassociation_islinkable_set(
             OV_INSTPTR_dynov_dynassociation          pobj,
             const OV_BOOL           value
) {
             OV_INSTPTR_ov_class          	pclass;
             OV_INSTPTR_ov_object          	pchild;

	     if ((value) && (!pobj->v_islinkable)) {
	             pobj->v_islinkable = dynov_dynassociation_check(Ov_PtrUpCast(ov_object, pobj));
		     if (!pobj->v_islinkable) return OV_ERR_ASSOCDEFMISMATCH;
	     }
	     else if ((!value) && (pobj->v_islinkable)) {
		     pclass = Ov_GetParent(ov_parentrelationship, pobj);
		     Ov_ForEachChild(ov_instantiation, pclass, pchild) {
			if (ov_association_isusedparentlink(Ov_PtrUpCast(ov_association, pobj), pchild)) return OV_ERR_NOACCESS;
		     }
		     pclass = Ov_GetParent(ov_childrelationship, pobj);
		     Ov_ForEachChild(ov_instantiation, pclass, pchild) {
			if (ov_association_isusedchildlink(Ov_PtrUpCast(ov_association, pobj), pchild)) return OV_ERR_NOACCESS;
		     }
		     dynov_dynassociation_uncheck(Ov_PtrUpCast(ov_object, pobj));
		     pobj->v_islinkable = FALSE;
	     }

             return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_ACCESS dynov_dynassociation_getaccess(
	OV_INSTPTR_ov_object	pobj,
	const OV_ELEMENT		*pelem,
	const OV_TICKET			*pticket
) {
	OV_INSTPTR_dynov_dynassociation	pdynassoc;
	OV_ACCESS			access;
	OV_ACCESS			access2;
		
	pdynassoc = Ov_StaticPtrCast(dynov_dynassociation, pobj);
	switch(pelem->elemtype) {
		case OV_ET_VARIABLE:
			if (!ov_string_compare(pelem->elemunion.pvar->v_identifier, "islinkable")) return OV_AC_READ | OV_AC_WRITE;
			access = OV_AC_NONE;
			if (pdynassoc->v_islinkable) access2 = OV_AC_READ;
			else access2 = OV_AC_READ | OV_AC_WRITE;
			if (!ov_string_compare(pelem->elemunion.pvar->v_identifier, "assoctype"))
				access = access2;
			if (!ov_string_compare(pelem->elemunion.pvar->v_identifier, "assocprops"))
				access = access2;
			if (!ov_string_compare(pelem->elemunion.pvar->v_identifier, "childrolename"))
				access = access2;
			if (!ov_string_compare(pelem->elemunion.pvar->v_identifier, "parentrolename"))
				access = access2;
			if (!ov_string_compare(pelem->elemunion.pvar->v_identifier, "childcomment"))
				access = access2;
			if (!ov_string_compare(pelem->elemunion.pvar->v_identifier, "parentcomment"))
				access = access2;
			if (!ov_string_compare(pelem->elemunion.pvar->v_identifier, "childflags"))
				access = access2;
			if (!ov_string_compare(pelem->elemunion.pvar->v_identifier, "parentflags"))
				access = access2;
			if (!ov_string_compare(pelem->elemunion.pvar->v_identifier, "parentoffset"))
				access = OV_AC_READ;
			if (!ov_string_compare(pelem->elemunion.pvar->v_identifier, "childoffset"))
				access = OV_AC_READ;
			return access;
		case OV_ET_CHILDLINK:
			if (pdynassoc->v_islinkable) {
				access = OV_AC_READ;
				access2 = OV_AC_READ;
			}
			else  {
				access = OV_AC_READ | OV_AC_WRITE;
				access2 = OV_AC_READ;
			}
			if (pelem->elemunion.passoc == passoc_ov_childrelationship)
				return access2;
			if (pelem->elemunion.passoc == passoc_ov_parentrelationship)
				return access2;
			if (pelem->elemunion.passoc == passoc_dynov_dynchildrelationship)
				return access;
			if (pelem->elemunion.passoc == passoc_dynov_dynparentrelationship)
				return access;
			return OV_AC_NONE;
		case OV_ET_PARENTLINK:
			if (pdynassoc->v_islinkable) {
				access = OV_AC_READ;
			}
			else  {
				access = OV_AC_READ | OV_AC_WRITE;
			}
			if (pelem->elemunion.passoc == passoc_dynov_islinkfnc)
				return access;
			if (pelem->elemunion.passoc == passoc_dynov_isunlinkfnc)
				return access;
			if (pelem->elemunion.passoc == passoc_dynov_assocaccess)
				return access;
			return OV_AC_NONE;
		case OV_ET_OPERATION:
			return OV_AC_NONE;
	}
	if (pdynassoc->v_islinkable) return OV_AC_READ;
	else return OV_AC_READ | OV_AC_RENAMEABLE | OV_AC_DELETEABLE;
}

/*	----------------------------------------------------------------------	*/


