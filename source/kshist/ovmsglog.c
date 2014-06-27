#ifndef OV_COMPILE_LIBRARY_kshist
#define OV_COMPILE_LIBRARY_kshist
#endif

#include "kshist.h"
#include "libov/ov_macros.h"


/*
*	Operations
*/

OV_RESULT OV_DLLFNCEXPORT kshist_OvMsgLog_getephist (
	OV_INSTPTR_ov_object 	pobj,
	OV_STRING*		type_identifier,
	OV_HIST_TYPE*		historytype,
	OV_INTERPOLATION_MODE*  default_interpolation,
	OV_INTERPOLATION_MODE*	supported_interpolation
)
{
	*type_identifier = NULL;
	*historytype = KS_HT_CHANGE_DRIVEN | KS_HT_LOG;
	*default_interpolation = KS_IPM_NONE;
	*supported_interpolation = KS_IPM_NONE;
	return OV_ERR_OK;
}

OV_RESULT OV_DLLFNCEXPORT kshist_OvMsgLog_gethist(
  OV_INSTPTR_ov_object pobj,
  OV_UINT items_len,	
  OV_GETHIST_ITEM* pitem,
  OV_GETHISTRESULT_ITEM* presultitem,
  OV_UINT max_answers	
)
{
	OV_UINT			i;
	OV_TIME			from, to;

	/*
	*	This history is a message log, which contains two tracks, "t" and "value".
	*/

   	for(i=0; i<items_len; i++, presultitem++, pitem++) {
		presultitem->result = OV_ERR_BADPARAM;
		if (strcmp(pitem->part, "t") && strcmp(pitem->part, "value")) return OV_ERR_BADPARAM;
		switch(pitem->selector.hseltype) {
		case OV_HSELT_NONE:
			/*
			*	get all messages available
			*/
			from.secs = 0;
			from.usecs = 0;
			ov_time_gettime(&to);
			break;
		case OV_HSELT_TIME:
			/*
			*	only get selected messages
			*/
			if(pitem->selector.OV_HISTSELECTOR_u.ths.ip_mode != OV_IPM_NONE) {
				/* messages are change driven; no interpolation available */
				presultitem->result = OV_ERR_BADSELECTOR;
				continue;
			}
			switch(pitem->selector.OV_HISTSELECTOR_u.ths.from.timetype) {
			case OV_TT_ABSOLUTE:
				from = pitem->selector.OV_HISTSELECTOR_u.ths.from.OV_ABSRELTIME_u.abstime;
				break;
			case OV_TT_RELATIVE:
				ov_time_gettime(&from);
				ov_time_add(&from, &from,
					&pitem->selector.OV_HISTSELECTOR_u.ths.from.OV_ABSRELTIME_u.reltime);
				break;
			default:
				presultitem->result = OV_ERR_BADSELECTOR;
				continue;
			}
			switch(pitem->selector.OV_HISTSELECTOR_u.ths.to.timetype) {
			case OV_TT_ABSOLUTE:
				to = pitem->selector.OV_HISTSELECTOR_u.ths.to.OV_ABSRELTIME_u.abstime;
				break;
			case OV_TT_RELATIVE:
				ov_time_gettime(&to);
				ov_time_add(&to, &to,
					&pitem->selector.OV_HISTSELECTOR_u.ths.to.OV_ABSRELTIME_u.reltime);
				break;
			default:
				presultitem->result = OV_ERR_BADSELECTOR;
				continue;
			}
			break;
		case OV_HSELT_STRING:
			/*
			*	not supported
			*/
			/* fall through... */
		default:
			presultitem->result = OV_ERR_BADSELECTOR;
			continue;
		}	/* switch() */

		/*
		*	we now know the start and the end time
		*/
		if(*pitem->part == 't') {
			/*
			*	retreive time stamps
			*/
			presultitem->value.vartype = OV_VT_TIME_VEC;
			presultitem->result = ov_logfile_getmessages(&from, &to,
				max_answers, NULL,
				&presultitem->value.valueunion.val_time_vec.value,
				&presultitem->value.valueunion.val_time_vec.veclen);
			if(Ov_Fail(presultitem->result)) {
				presultitem->result = OV_ERR_TARGETGENERIC;	/* out of memory */
			}
		} else {
			/*
			*	retreive message values
			*/
			presultitem->value.vartype = OV_VT_STRING_VEC;
			presultitem->result = ov_logfile_getmessages(&from, &to,
				max_answers,
				&presultitem->value.valueunion.val_string_vec.value, NULL,
				&presultitem->value.valueunion.val_string_vec.veclen);
			if(Ov_Fail(presultitem->result)) {
				presultitem->result = OV_ERR_TARGETGENERIC;	/* out of memory */
			}
		}
	}	/* for */

	/*
	*	we are finished.
	*/
	return OV_ERR_OK;
}

OV_RESULT OV_DLLFNCEXPORT kshist_OvMsgLog_getephistelem(
	OV_INSTPTR_ov_object 	pobj,
	const OV_TICKET		*pticket,
	OV_GETEP_RES		*result,
	KS_EP_FLAGS		scope_flags,
	const OV_OBJ_TYPE	type_mask,
	const OV_STRING		name_mask
) 
{
	OV_OBJ_ENGINEERED_PROPS	*pprops;
	OV_OBJ_ENGINEERED_PROPS	*pprops1;
	OV_VTBLPTR_kshist_OvMsgLog pvtable;
	OV_INSTPTR_kshist_OvMsgLog pmsglog;
	OV_ACCESS	access;
	OV_ELEMENT	elem;

	pmsglog = Ov_StaticPtrCast(kshist_OvMsgLog, pobj);
	Ov_GetVTablePtr(kshist_OvMsgLog, pvtable, pmsglog);
	elem.elemtype = OV_ET_OBJECT;
	elem.pobj = pobj;
	access = pvtable->m_getaccess(pobj, &elem, pticket);
   	if((scope_flags & KS_EPF_CHILDREN) && ((type_mask & (KS_OT_DOMAIN | KS_OT_VARIABLE | KS_OT_HISTORY)))) {

		/*
		*	create new result item and insert into result item list
		*/

		pprops = Ov_MemStackAlloc(OV_OBJ_ENGINEERED_PROPS);
		if(!pprops) {
			return OV_ERR_TARGETGENERIC;	/* TODO! out of heap memory */
		}
		pprops1 = Ov_MemStackAlloc(OV_OBJ_ENGINEERED_PROPS);
		if(!pprops1) {
			return OV_ERR_TARGETGENERIC;	/* TODO! out of heap memory */
		}
		pprops->pnext = pprops1;
		if(result->plast) {
			result->plast->pnext = pprops;
			result->plast = pprops;
		} else {
			result->pfirst = result->plast = pprops;
		}
		result->items_len++;
		if(result->plast) {
			result->plast->pnext = pprops1;
			result->plast = pprops1;
		} else {
			result->pfirst = result->plast = pprops1;
		}
		result->items_len++;

		/*
		*	set engineered properties of t Track
		*/

		pprops->objtype = KS_OT_VARIABLE;
		pprops->identifier = (OV_STRING) ov_memstack_alloc(2);
		strcpy(pprops->identifier, "t");
		pprops->creation_time = pobj->v_creationtime;
		pprops->comment = NULL;
		pprops->access = access;
		pprops->semantic_flags = 0;

		pprops->OV_OBJ_ENGINEERED_PROPS_u.var_engineered_props.tech_unit
			= NULL;
		pprops->OV_OBJ_ENGINEERED_PROPS_u.var_engineered_props.vartype
			= OV_VT_TIME;

		/*
		*	set engineered properties of value Track
		*/

		pprops1->objtype = KS_OT_VARIABLE;
		pprops1->identifier = (OV_STRING) ov_memstack_alloc(6);
		strcpy(pprops1->identifier, "value");
		pprops1->creation_time = pobj->v_creationtime;
		pprops1->comment = NULL;
		pprops1->access = access;
		pprops1->semantic_flags = 0;

		pprops1->OV_OBJ_ENGINEERED_PROPS_u.var_engineered_props.tech_unit
			= NULL;
		pprops1->OV_OBJ_ENGINEERED_PROPS_u.var_engineered_props.vartype
			= OV_VT_STRING;
    	}
	return OV_ERR_OK;
}
