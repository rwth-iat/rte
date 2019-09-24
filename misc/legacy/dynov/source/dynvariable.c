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
#include "ov_macros.h"

OV_DLLFNCEXPORT void dynov_dynvariable_uncheck(
        OV_INSTPTR_ov_object          pobj
) {
	OV_INSTPTR_ov_structure		pstruct;
	OV_INSTPTR_dynov_dynvariable    pdynvar;

	pdynvar = Ov_DynamicPtrCast(dynov_dynvariable, pobj);
	if (pdynvar) {
		pstruct = Ov_GetParent(ov_construction, pdynvar);
		if (pstruct) {
			Ov_Unlink(ov_construction, pstruct, pdynvar);
		}
		pdynvar->v_getfnc = 0;
		pdynvar->v_setfnc = 0;
	}
}

OV_UINT dynov_dynvariable_getvectype(
	OV_UINT vartype,
	OV_UINT veclen
) {
	switch(vartype & OV_VT_KSMASK) {
	case OV_VT_BYTE:
		return OV_VT_BYTE_VEC;
	case OV_VT_BOOL:
		return OV_VT_BOOL_VEC;
	case OV_VT_INT:
		return OV_VT_INT_VEC;
	case OV_VT_UINT:
		return OV_VT_UINT_VEC;
	case OV_VT_SINGLE:
		return OV_VT_SINGLE_VEC;
	case OV_VT_DOUBLE:
		return OV_VT_DOUBLE_VEC;
	case OV_VT_STRING:
		return OV_VT_STRING_VEC;
	case OV_VT_TIME:
		return OV_VT_TIME_VEC;
	case OV_VT_TIME_SPAN:
		return OV_VT_TIME_SPAN_VEC;
	}
	return 0;
}

OV_UINT dynov_dynvariable_getsize(
	OV_UINT vartype,
	OV_UINT veclen
) {
	OV_UINT size;

	switch(vartype & OV_VT_KSMASK) {
	case OV_VT_BYTE_VEC:
	case OV_VT_BYTE:
		size = sizeof(OV_BYTE)*veclen;
		break;
	case OV_VT_BOOL_VEC:
	case OV_VT_BOOL:
		size = sizeof(OV_BOOL)*veclen;
		break;
	case OV_VT_INT_VEC:
	case OV_VT_INT:
		size = sizeof(OV_INT)*veclen;
		break;
	case OV_VT_UINT_VEC:
	case OV_VT_UINT:
		size = sizeof(OV_UINT)*veclen;
		break;
	case OV_VT_SINGLE_VEC:
	case OV_VT_SINGLE:
		size = sizeof(OV_SINGLE)*veclen;
		break;
	case OV_VT_DOUBLE_VEC:
	case OV_VT_DOUBLE:
		size = sizeof(OV_DOUBLE)*veclen;
		break;
	case OV_VT_STRING_VEC:
	case OV_VT_STRING:
		size = sizeof(OV_STRING)*veclen;
		break;
	case OV_VT_TIME_VEC:
	case OV_VT_TIME:
		size = sizeof(OV_TIME)*veclen;
		break;
	case OV_VT_TIME_SPAN_VEC:
	case OV_VT_TIME_SPAN:
		size = sizeof(OV_TIME_SPAN)*veclen;
		break;
	default:
		size = 0;
	}
	if (veclen == 0) size = sizeof(OV_GENERIC_VEC);
	if (vartype & OV_VT_HAS_STATE) size += sizeof(OV_STATE);
	if (vartype & OV_VT_HAS_TIMESTAMP) size += sizeof(OV_TIME);

	return size;
}

OV_DLLFNCEXPORT OV_BOOL dynov_dynvariable_check(
        OV_INSTPTR_ov_object          pobj
) {
	OV_INSTPTR_ov_structure          	pstruct;
	OV_INSTPTR_dynov_dynstructure          	pdynstruct;
	OV_INSTPTR_dynov_dynvariable          	pdynvar;
	OV_INSTPTR_dynov_dynoperation          	pdynop;
	OV_VTBLPTR_dynov_dynstructure          	pvtable;
	OV_BOOL					result;
        OV_VTBLPTR_dynov_dynoperation           pvtableop;

	pdynvar = Ov_DynamicPtrCast(dynov_dynvariable, pobj);
	if (!pdynvar) return FALSE;
	pstruct = Ov_GetParent(dynov_dynconstruction, pdynvar);
	if (pstruct) {
	        pdynvar->v_vartype = OV_VT_STRUCT;
	        pdynvar->v_veclen = 0;
		pdynstruct = Ov_DynamicPtrCast(dynov_dynstructure, pstruct);
		if (pdynstruct) {
			if (Ov_GetParent(ov_containment, pdynvar)==Ov_PtrUpCast(ov_domain, pdynstruct)) return FALSE;
			Ov_GetVTablePtr(dynov_dynstructure, pvtable, pdynstruct);
			result = pvtable->m_check(Ov_PtrUpCast(ov_object, pdynstruct));
			if (!pdynstruct->v_isinstantiable) return FALSE;
			pdynvar->v_size = pdynstruct->v_size;
		}
		else if (pstruct) pdynvar->v_size = pstruct->v_size;
		     else return FALSE;
	}
	else {
		if ((pdynvar->v_vartype < OV_VT_STRUCT) && (pdynvar->v_veclen!=1))
			pdynvar->v_vartype = dynov_dynvariable_getvectype(pdynvar->v_vartype, pdynvar->v_veclen);
		pdynvar->v_size = dynov_dynvariable_getsize(pdynvar->v_vartype, pdynvar->v_veclen);
		if (!pdynvar->v_size) return FALSE;
	}
	if (pdynvar->v_ctypename) return FALSE;
	if ((pdynvar->v_initialvalue.value.vartype!=0) &&
	   ((pdynvar->v_vartype & OV_VT_KSMASK) != (pdynvar->v_initialvalue.value.vartype & OV_VT_KSMASK))) return FALSE;
	if((pdynvar->v_varprops & OV_VP_STATIC) && (pdynvar->v_varprops & OV_VP_DERIVED)) return FALSE;

	pdynop = Ov_GetChild(dynov_isgetaccessor, pdynvar);
	pdynvar->v_varprops = pdynvar->v_varprops & (~OV_VP_GETACCESSOR);
	if (pdynop) if (pdynop->v_executeable) {
		Ov_GetVTablePtr(dynov_dynoperation, pvtableop, pdynop);
		if (pvtableop) {
			pdynvar->v_getfnc = (OV_FNCPTR_GET) pvtableop->m_fncget;
			pdynvar->v_varprops = pdynvar->v_varprops & OV_VP_GETACCESSOR;
		}
	}
	pdynvar->v_varprops = pdynvar->v_varprops & (~OV_VP_SETACCESSOR);
	pdynop = Ov_GetChild(dynov_issetaccessor, pdynvar);
	if (pdynop) if (pdynop->v_executeable)  {
		Ov_GetVTablePtr(dynov_dynoperation, pvtableop, pdynop);
		if (pvtableop) {
			pdynvar->v_setfnc = (OV_FNCPTR_SET) pvtableop->m_fncset;
			pdynvar->v_varprops = pdynvar->v_varprops & OV_VP_SETACCESSOR;
		}
	}

	if((pdynvar->v_varprops & OV_VP_DERIVED) && !(pdynvar->v_varprops & OV_VP_ACCESSORS)) return FALSE;

	pstruct = Ov_GetParent(dynov_dynconstruction, pdynvar);
	if (pstruct) {
		if (Ov_Fail(Ov_Link(ov_construction, pstruct, pdynvar))) return FALSE;
	}
	return TRUE;
}


OV_DLLFNCEXPORT OV_UINT dynov_dynvariable_accessrights_get(
             OV_INSTPTR_dynov_dynvariable          pobj
) {
             return pobj->v_accessrights;
}

OV_DLLFNCEXPORT OV_RESULT dynov_dynvariable_accessrights_set(
             OV_INSTPTR_dynov_dynvariable          pobj,
             const OV_UINT           value
) {

	     if (value & (OV_AC_READ | OV_AC_WRITE))
		     pobj->v_accessrights = value;
	     else return OV_ERR_BADVALUE;
       	     return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT dynov_dynvariable_dynvartype_get(
             OV_INSTPTR_dynov_dynvariable          pobj
) {
             return pobj->v_dynvartype;
}

OV_DLLFNCEXPORT OV_RESULT dynov_dynvariable_dynvartype_set(
             OV_INSTPTR_dynov_dynvariable          pobj,
             const OV_UINT           value
) {

	     switch (pobj->v_initialvalue.value.vartype & OV_VT_KSMASK) {
		case OV_VT_STRING: 
			ov_string_setvalue(&pobj->v_initialvalue.value.valueunion.val_string, NULL);
			break;
		case OV_VT_STRING_VEC:
			ov_string_setvecvalue(pobj->v_initialvalue.value.valueunion.val_string_vec.value, NULL, pobj->v_initialvalue.value.valueunion.val_string_vec.veclen);
			break;
		case OV_VT_BOOL_VEC:
			Ov_SetDynamicVectorValue(&pobj->v_initialvalue.value.valueunion.val_bool_vec, NULL, 0, BOOL);
			break;
		case OV_VT_INT_VEC:
			Ov_SetDynamicVectorValue(&pobj->v_initialvalue.value.valueunion.val_int_vec, NULL, 0, INT);
			break;
		case OV_VT_UINT_VEC:
			Ov_SetDynamicVectorValue(&pobj->v_initialvalue.value.valueunion.val_uint_vec, NULL, 0, UINT);
			break;
		case OV_VT_SINGLE_VEC:
			Ov_SetDynamicVectorValue(&pobj->v_initialvalue.value.valueunion.val_single_vec, NULL, 0, SINGLE);
			break;
		case OV_VT_DOUBLE_VEC:
			Ov_SetDynamicVectorValue(&pobj->v_initialvalue.value.valueunion.val_double_vec, NULL, 0, DOUBLE);
			break;
		case OV_VT_TIME_VEC:
			Ov_SetDynamicVectorValue(&pobj->v_initialvalue.value.valueunion.val_time_vec, NULL, 0, TIME);
			break;
		case OV_VT_TIME_SPAN_VEC:
			Ov_SetDynamicVectorValue(&pobj->v_initialvalue.value.valueunion.val_time_span_vec, NULL, 0, TIME_SPAN);
			break;
	     }	
	     pobj->v_initialvalue.value.valueunion.val_time.secs = 0;
	     pobj->v_initialvalue.value.valueunion.val_time.usecs = 0;
	     if ((value < OV_VT_STRUCT) && (pobj->v_veclen!=1)) 
			pobj->v_dynvartype = dynov_dynvariable_getvectype(value, pobj->v_veclen);
	     else pobj->v_dynvartype = value;
	     pobj->v_vartype = pobj->v_dynvartype;
	     pobj->v_initialvalue.value.vartype = pobj->v_dynvartype;
       	     return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT dynov_dynvariable_dynveclen_get(
             OV_INSTPTR_dynov_dynvariable          pobj
) {
             return pobj->v_dynveclen;
}

OV_DLLFNCEXPORT OV_RESULT dynov_dynvariable_dynveclen_set(
             OV_INSTPTR_dynov_dynvariable          pobj,
             const OV_UINT           value
) {
	     pobj->v_dynveclen = value;
	     pobj->v_veclen = value;
	     return dynov_dynvariable_dynvartype_set(pobj, (pobj->v_vartype & 0x7f));
}

OV_BOOL dynov_dynvariable_isinstantiable(
	OV_INSTPTR_ov_object	pobj
) {
	OV_INSTPTR_dynov_dynclass	pdynclass;
	OV_INSTPTR_dynov_dynstructure	pdynstruct;
	OV_INSTPTR_ov_variable		pvar;

	pdynclass = Ov_DynamicPtrCast(dynov_dynclass, Ov_GetParent(ov_containment, pobj));
	if (pdynclass) if (pdynclass->v_isinstantiable) return TRUE;
	pdynstruct = Ov_DynamicPtrCast(dynov_dynstructure, Ov_GetParent(ov_containment, pobj));
	if (pdynstruct) {
		Ov_ForEachChild(ov_construction, Ov_PtrUpCast(ov_structure, pdynstruct), pvar) {
			pdynclass = Ov_DynamicPtrCast(dynov_dynclass, Ov_GetParent(ov_containment, pvar));
			pdynstruct = Ov_DynamicPtrCast(dynov_dynstructure, Ov_GetParent(ov_containment, pvar));
			if (pdynstruct) return dynov_dynvariable_isinstantiable(Ov_PtrUpCast(ov_object, pdynstruct));
			if (pdynclass) if (pdynclass->v_isinstantiable) return TRUE;
		}
        }
	return FALSE;
}

OV_DLLFNCEXPORT OV_ACCESS dynov_dynvariable_getaccess(
	OV_INSTPTR_ov_object	pobj,
	const OV_ELEMENT		*pelem,
	const OV_TICKET			*pticket
) {
	OV_ACCESS			access;
	OV_ACCESS			access2;

	switch(pelem->elemtype) {
		case OV_ET_VARIABLE:
			access = OV_AC_NONE;
			if (dynov_dynvariable_isinstantiable(pobj)) access = OV_AC_READ;
			else access = OV_AC_READ | OV_AC_WRITE;
			if (!ov_string_compare(pelem->elemunion.pvar->v_identifier, "vartype")) {
				if (dynov_dynvariable_isinstantiable(pobj)) return OV_AC_READ;
				else return OV_AC_READ;
			}
			if (!ov_string_compare(pelem->elemunion.pvar->v_identifier, "dynvartype")) {
				if (dynov_dynvariable_isinstantiable(pobj)) return OV_AC_READ;
				else return  OV_AC_READ | OV_AC_WRITE;
			}
			if (!ov_string_compare(pelem->elemunion.pvar->v_identifier, "veclen")) {
				if (dynov_dynvariable_isinstantiable(pobj)) return OV_AC_READ;
				else return OV_AC_READ;
			}
			if (!ov_string_compare(pelem->elemunion.pvar->v_identifier, "dynveclen")) {
				if (dynov_dynvariable_isinstantiable(pobj)) return OV_AC_READ;
				else return  OV_AC_READ | OV_AC_WRITE;
			}
			if (!ov_string_compare(pelem->elemunion.pvar->v_identifier, "varprops"))
				return access;
			if (!ov_string_compare(pelem->elemunion.pvar->v_identifier, "comment"))
				return access;
			if (!ov_string_compare(pelem->elemunion.pvar->v_identifier, "flags"))
				return access;
			if (!ov_string_compare(pelem->elemunion.pvar->v_identifier, "techunit"))
				return access;
			if (!ov_string_compare(pelem->elemunion.pvar->v_identifier, "size"))
				return access;
			if (!ov_string_compare(pelem->elemunion.pvar->v_identifier, "initialvalue"))
				return access;
			if (!ov_string_compare(pelem->elemunion.pvar->v_identifier, "accessrights"))
				return access;
			return OV_AC_NONE;
		case OV_ET_CHILDLINK:
			if (pelem->elemunion.passoc == passoc_ov_instantiation)
				return OV_AC_NONE;
			if (pelem->elemunion.passoc == passoc_ov_containment)
				return OV_AC_NONE;
			if (dynov_dynvariable_isinstantiable(pobj)) {
				access = OV_AC_READ;
				access2 = OV_AC_READ;
			}
			else  {
				access = OV_AC_READ | OV_AC_WRITE;
				access2 = OV_AC_READ;
			}
			if (pelem->elemunion.passoc == passoc_ov_construction)
				return access2;
			if (pelem->elemunion.passoc == passoc_dynov_dynconstruction)
				return access;
			return OV_AC_NONE;
		case OV_ET_PARENTLINK:
			if (dynov_dynvariable_isinstantiable(pobj)) {
				access = OV_AC_READ;
			}
			else  {
				access = OV_AC_READ | OV_AC_WRITE;
			}
			if (pelem->elemunion.passoc == passoc_dynov_isgetaccessor)
				return access;
			if (pelem->elemunion.passoc == passoc_dynov_issetaccessor)
				return access;
			return OV_AC_NONE;
    	}
	if ((dynov_dynvariable_isinstantiable(pobj)) || (pobj->v_pouterobject)) return OV_AC_READ;
	return OV_AC_READ | OV_AC_DELETEABLE | OV_AC_RENAMEABLE;
}

