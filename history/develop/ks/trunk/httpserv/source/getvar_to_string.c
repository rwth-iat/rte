#include "libov/ov_ov.h"
#include "libov/ov_string.h"
#include "libov/ov_macros.h"
#include "libov/ov_vendortree.h"
#include "config.h"

#define GETVAR_TO_STRING_RETURN \
	ov_string_setvalue(&temp, NULL);\
	return

OV_RESULT getvar_to_string(OV_INSTPTR_ov_object pObj, OV_STRING* varname, OV_STRING* message){
	//	Local Variables
	//
	OV_VTBLPTR_ov_object pOvVTable = NULL;

	//	Local Variables
	//
	OV_ELEMENT	ParentElement;
	OV_ELEMENT	PartElement;
	OV_ANY		Variable;
	OV_RESULT	fr = OV_ERR_OK;
	OV_UINT i = 0;
	OV_STRING temp = NULL;

	Ov_GetVTablePtr(ov_object, pOvVTable, pObj);
	ParentElement.elemtype	= OV_ET_OBJECT;
	ParentElement.pobj		= Ov_PtrUpCast(ov_object, pObj);

	if(pObj == NULL){
		ov_string_append(message, "Object does not exist\n");
		GETVAR_TO_STRING_RETURN OV_ERR_BADPATH; //404
	}

	if(ov_string_compare(pObj->v_identifier, "vendor") == OV_STRCMP_EQUAL && Ov_GetParent(ov_containment, pObj)==&pdb->root){
		if(ov_string_compare(*varname, "classes") == OV_STRCMP_EQUAL){
			//TODO: tut minimum nicht bei classes/libraries
			fr = ov_vendortree_getclasses(&Variable, NULL);
		}else{
			pObj = Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, pObj), *varname);
			fr = ov_vendortree_getvar(pObj, &Variable, NULL);
		}
		if (Ov_Fail(fr))
		{
			ov_string_append(message, "Vendor object exists, but variable does not\n");
			GETVAR_TO_STRING_RETURN OV_ERR_BADPATH; //404
		}
	}else{
		//	search Variable-Element with corresponding name in OV-Object
		//
		fr = ov_element_searchpart(&ParentElement, &PartElement, OV_ET_VARIABLE, *varname);
		if (Ov_Fail(fr))
		{
			GETVAR_TO_STRING_RETURN OV_ERR_GENERIC; //503
		};

		//	fill Variable with active Content of OV-Object
		//
		ov_memstack_lock();
			fr = pOvVTable->m_getvar(pObj, &PartElement, &Variable);
		ov_memstack_unlock();
		if (Ov_Fail(fr))
		{
			ov_string_append(message, "Object exists, but variable does not\n");
			GETVAR_TO_STRING_RETURN OV_ERR_BADPATH; //404
		};
	}
	//	We are checking actual content and not the definition of Variable, so we have no ANY, but VOID
	//	OV_VT_BOOL_PV is an ANY which is set to BOOL
	//
	switch (Variable.value.vartype)
	{
		case OV_VT_BOOL:
		case OV_VT_BOOL_PV:
			if (Variable.value.valueunion.val_bool == TRUE){
				ov_string_append(&temp, "TRUE");
			}else{
				ov_string_append(&temp, "FALSE");
			}
			break;

		case OV_VT_BYTE:
		case (OV_VT_BYTE | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):
			ov_string_print(&temp, "%d", Variable.value.valueunion.val_byte);
			break;

		case OV_VT_INT:
		case OV_VT_INT_PV:
			ov_string_print(&temp, "%i", Variable.value.valueunion.val_int);
			break;

		case OV_VT_UINT:
		case OV_VT_UINT_PV:
			ov_string_print(&temp, "%u", Variable.value.valueunion.val_uint);
			break;

		case OV_VT_SINGLE:
		case OV_VT_SINGLE_PV:
			ov_string_print(&temp, "%g", Variable.value.valueunion.val_single);
			break;

		case OV_VT_DOUBLE:
		case OV_VT_DOUBLE_PV:
			ov_string_print(&temp, "%g", Variable.value.valueunion.val_double);
			break;

		case OV_VT_STRING:
		case OV_VT_STRING_PV:
			if (ov_string_compare(Variable.value.valueunion.val_string, NULL) == OV_STRCMP_EQUAL){
				ov_string_setvalue(&temp, "\"\"");
			}else{
				ov_string_print(&temp, "\"%s\"", Variable.value.valueunion.val_string);
			}
			break;

		case OV_VT_TIME:
		case OV_VT_TIME_PV:
			ov_string_print(&temp, "%s", ov_time_timetoascii(&Variable.value.valueunion.val_time));
			break;

		case OV_VT_TIME_SPAN:
		case OV_VT_TIME_SPAN_PV:
			ov_string_print(&temp, "%s", ov_time_timespantoascii(&Variable.value.valueunion.val_time_span));
			break;

		case OV_VT_STATE:
		case (OV_VT_STATE | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):
			ov_string_print(&temp, "%s", "unknown");
			break;

		case OV_VT_STRUCT:
		case (OV_VT_STRUCT | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):
			ov_string_print(&temp, "%s", "unknown");
			break;

		case OV_VT_VOID:															//unused ANY with explicit no content
		case (OV_VT_VOID | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):	//  used ANY with explicit no content
			ov_string_print(&temp, "%s", "VOID");
			break;

		//****************** VEC: *******************
		case OV_VT_BOOL_VEC:
		case OV_VT_BOOL_PV_VEC:
			ov_string_setvalue(&temp, "Vector has no entries");
			for ( i = 0; i < Variable.value.valueunion.val_bool_vec.veclen;i++){
				if (i == 0){
					if (Variable.value.valueunion.val_bool_vec.value[i] == TRUE){
						ov_string_setvalue(&temp, "TRUE");
					}else{
						ov_string_setvalue(&temp, "FALSE");
					}
				}else{
					if (Variable.value.valueunion.val_bool_vec.value[i] == TRUE){
						ov_string_append(&temp, ";TRUE");
					}else{
						ov_string_append(&temp, ";FALSE");
					}
				}
			}
			break;

		case OV_VT_BYTE_VEC:
		case (OV_VT_BYTE_VEC | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):
			ov_string_setvalue(&temp, "Vector has no entries");
			for ( i = 0; i < Variable.value.valueunion.val_byte_vec.veclen;i++){
				if (i == 0){
					ov_string_print(&temp, "%d", Variable.value.valueunion.val_byte_vec.value[i]);
				}else{
					ov_string_print(&temp, "%s;%d", temp, Variable.value.valueunion.val_byte_vec.value[i]);
				}
			}
			break;

		case OV_VT_INT_VEC:
		case OV_VT_INT_PV_VEC:
			ov_string_setvalue(&temp, "Vector has no entries");
			for ( i = 0; i < Variable.value.valueunion.val_int_vec.veclen;i++){
				if (i == 0){
					ov_string_print(&temp, "%i", Variable.value.valueunion.val_int_vec.value[i]);
				}else{
					ov_string_print(&temp, "%s;%i", temp, Variable.value.valueunion.val_int_vec.value[i]);
				}
			}
			break;

		case OV_VT_UINT_VEC:
		case OV_VT_UINT_PV_VEC:
			ov_string_setvalue(&temp, "Vector has no entries");
			for ( i = 0; i < Variable.value.valueunion.val_uint_vec.veclen;i++){
				if (i == 0){
					ov_string_print(&temp, "%i", Variable.value.valueunion.val_uint_vec.value[i]);
				}else{
					ov_string_print(&temp, "%s;%i", temp, Variable.value.valueunion.val_uint_vec.value[i]);
				}
			}
			break;

		case OV_VT_SINGLE_VEC:
		case OV_VT_SINGLE_PV_VEC:
			ov_string_setvalue(&temp, "Vector has no entries");
			for ( i = 0; i < Variable.value.valueunion.val_single_vec.veclen;i++){
				if (i == 0){
					ov_string_print(&temp, "%g", Variable.value.valueunion.val_single_vec.value[i]);
				}else{
					ov_string_print(&temp, "%s;%g", temp, Variable.value.valueunion.val_single_vec.value[i]);
				}
			}
			break;

		case OV_VT_DOUBLE_VEC:
		case OV_VT_DOUBLE_PV_VEC:
			ov_string_setvalue(&temp, "Vector has no entries");
			for ( i = 0; i < Variable.value.valueunion.val_double_vec.veclen;i++){
				if (i == 0){
					ov_string_print(&temp, "%g", Variable.value.valueunion.val_double_vec.value[i]);
				}else{
					ov_string_print(&temp, "%s;%g", temp, Variable.value.valueunion.val_double_vec.value[i]);
				}
			}
			break;

		case OV_VT_STRING_VEC:
		case OV_VT_STRING_PV_VEC:
			ov_string_setvalue(&temp, "Vector has no entries");
			for ( i = 0; i < Variable.value.valueunion.val_string_vec.veclen;i++){
				if (i == 0){
					if (ov_string_compare(Variable.value.valueunion.val_string_vec.value[i], NULL) == OV_STRCMP_EQUAL){
						ov_string_setvalue(&temp, "\"\"");
					}else{
						ov_string_print(&temp, "\"%s\"", Variable.value.valueunion.val_string_vec.value[i]);
					}
				}else{
					if (ov_string_compare(Variable.value.valueunion.val_string_vec.value[i], NULL) == OV_STRCMP_EQUAL){
						ov_string_print(&temp, "%s;%s", temp, "\"\"");
					}else{
						ov_string_print(&temp, "%s;\"%s\"", temp, Variable.value.valueunion.val_string_vec.value[i]);
					}
				}
			}
			break;

		case OV_VT_TIME_VEC:
		case OV_VT_TIME_PV_VEC:
			ov_string_setvalue(&temp, "Vector has no entries");
			for ( i = 0; i < Variable.value.valueunion.val_time_vec.veclen;i++){
				if (i == 0){
					ov_string_print(&temp, "%s", ov_time_timetoascii(&Variable.value.valueunion.val_time_vec.value[i]));
				}else{
					ov_string_print(&temp, "%s;%s", temp, ov_time_timetoascii(&Variable.value.valueunion.val_time_vec.value[i]));
				}
			}
			break;

		case OV_VT_TIME_SPAN_VEC:
		case OV_VT_TIME_SPAN_PV_VEC:
			ov_string_setvalue(&temp, "Vector has no entries");
			for ( i = 0; i < Variable.value.valueunion.val_time_span_vec.veclen;i++){
				if (i == 0){
					ov_string_print(&temp, "%s", ov_time_timespantoascii(&Variable.value.valueunion.val_time_span_vec.value[i]));
				}else{
					ov_string_print(&temp, "%s;%s", temp, ov_time_timespantoascii(&Variable.value.valueunion.val_time_span_vec.value[i]));
				}
			}
			break;

		case OV_VT_STATE_VEC:
		case (OV_VT_STATE_VEC | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):
			ov_string_print(&temp, "%s", "unknown");
			break;

		case OV_VT_STRUCT_VEC:
		case (OV_VT_STRUCT_VEC | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):
			ov_string_print(&temp, "%s", "unknown");
			break;

		default:
			ov_string_print(&temp, "unknown value, VarType: %#X", Variable.value.vartype);
			break;
	}

    ov_string_setvalue(message, temp);
    GETVAR_TO_STRING_RETURN OV_ERR_OK;
}
