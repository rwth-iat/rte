/*
*   $Id: ov_ksserver_gethist.c,v 1.4 2000-04-04 15:12:50 dirk Exp $
*
*   Copyright (C) 1998-1999
*   Lehrstuhl fuer Prozessleittechnik,
*   RWTH Aachen, D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
*
*   This file is part of the ACPLT/OV Package which is licensed as open
*   source under the Artistic License; you can use, redistribute and/or
*   modify it under the terms of that license.
*
*   You should have received a copy of the Artistic License along with
*   this Package; see the file ARTISTIC-LICENSE. If not, write to the
*   Copyright Holder.
*
*   THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
*   WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
*   OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/
/*
*	History:
*	--------
*	20-Jul-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

#define OV_COMPILE_LIBOVKS

#include "libovks/ov_ksserver.h"
#include "libov/ov_path.h"
#include "libov/ov_string.h"
#include "libov/ov_time.h"
#include "libov/ov_macros.h"

/*	----------------------------------------------------------------------	*/

/*
*	Execute the GetHist service (subroutine)
*/
void ov_ksserver_gethist(
	const OV_UINT			version,
	const OV_TICKET			*pticket,
	const OV_GETHIST_PAR	*params,
	OV_GETHIST_RES			*result
) {
	/*
	*	local variables
	*/
	OV_UINT					len = params->items_len;
	OV_GETHIST_ITEM			*pitem = params->items_val;
	OV_GETHISTSINGLERESULT	*psingleresult;
	OV_GETHISTRESULT_ITEM	*presultitem;
	OV_UINT					i;
	OV_TIME					from, to;
	/*
	*	check access rights
	*/
	if(!(pticket->vtbl->getaccess(pticket) & OV_AC_READ)) {
		result->result = OV_ERR_NOACCESS;
		return;
	}
	/*
	*	for now, we only support one path: "/messages". This history is
	*	a message log, which contains two tracks, "t" and "value".
	*/
	result->result = OV_ERR_BADPARAM;
	if(params->paths_len != 1) {
		return;
	}
	if(strcmp(params->paths_val[0], "/messages")) {
		return;
	}
	for(i=0; i<len; i++) {
		if(strcmp(params->items_val[i].part, "t")
			&& strcmp(params->items_val[i].part, "value")
		) {
			return;
		}
	}
	/*
	*	allocate memory for the one and only single result item
	*/
	psingleresult = (OV_GETHISTSINGLERESULT*)ov_memstack_alloc(sizeof(OV_GETHISTSINGLERESULT));
	if(!psingleresult) {
		result->result = OV_ERR_TARGETGENERIC;
		return;
	}
	result->results_val = psingleresult;
	result->result = OV_ERR_OK;
	result->results_len = 1;
	/*
	*	further initialization
	*/
	psingleresult->items_len = 0;
	psingleresult->items_val = NULL;
	/*
	*	if there are no items, we are done
	*/
	if(!len) {
		return;
	}
	/*
	*	allocate memory for the result items
	*/
	presultitem = (OV_GETHISTRESULT_ITEM*)ov_memstack_alloc(len*sizeof(OV_GETHISTRESULT_ITEM));
	if(!presultitem) {
		result->result = OV_ERR_TARGETGENERIC;
		return;
	}
	psingleresult->result = OV_ERR_OK;
	psingleresult->items_val = presultitem;
	psingleresult->items_len = len;
	/*
	*	iterate over the items given
	*/
	for(i=0; i<len; i++, pitem++, presultitem++) {
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
				params->max_answers, NULL,
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
				params->max_answers,
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
	return;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

