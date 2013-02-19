/*
 * ksxdr_getserver.c
 *
 *  Created on: 12.02.2013
 *      Author: lars
 */


/*	the following defines reduce the loglevel to warning and error for this file
 * As soon as the development phase is over these lines should be deleted and the loglevel should be reduced to warning and error globally in the Makefile
 * */
#if LOG_KS
#undef LOG_KS
#define LOG_KS 0
#endif

#if LOG_KS_DEBUG
#undef LOG_KS_DBUG
#define LOG_KS_DEBUG 0
#endif

#if LOG_KS_INFO
#undef LOG_KS_INFO
#define LOG_KS_INFO 0
#endif

#if !LOG_KS_WARNING
#ifdef LOG_KS_WARNING
#undef LOG_KS_WARNING
#endif
#define LOG_KS_WARNING 1
#endif

#if !LOG_KS_ERROR
#ifdef LOG_KS_ERROR
#undef LOG_KS_ERROR
#endif
#define LOG_KS_ERROR 1
#endif

#include "ksxdr.h"
#include "ksxdr_config.h"
#include "ks_logfile.h"
#include "libov/ov_result.h"
#include "libov/ov_macros.h"
#include "KSDATAPACKET_xdrhandling.h"
#include "ov_ksserver_backend.h"
#include "ksbase_helper.h"


/*
 * Nobody seems to use get-EP
 */


/*
 * xdr routine for decoding getpp parameters
 */

OV_RESULT ksxdr_getEP_decodeparams(KS_DATAPACKET* dataReceived, OV_GETEP_PAR* params)
{
	OV_RESULT result;

	result = KS_DATAPACKET_read_xdr_string_tomemstack(dataReceived, &params->path, ~0);
	if(Ov_Fail(result))
		return result;
KS_logfile_debug(("getEP_decodeparams: path: \n\t\t%s", params->path));

result = KS_DATAPACKET_read_xdr_OV_OBJ_TYPE(dataReceived, &params->type_mask);
	if(Ov_Fail(result))
		return result;

	result = KS_DATAPACKET_read_xdr_string_tomemstack(dataReceived, &params->name_mask, ~0);
	if(Ov_Fail(result))
		return result;
KS_logfile_debug(("getEP_decodeparams: name_mask: \n\t\t%s", params->name_mask));

return KS_DATAPACKET_read_xdr_OV_EP_FLAGS(dataReceived, &params->scope_flags);
}

/*
*	XDR routine for OV_HISTORY_ENGINEERED_PROPS
*/
OV_RESULT xdr_write_OV_HISTORY_ENGINEERED_PROPS(KS_DATAPACKET* serviceAnswer, OV_HISTORY_ENGINEERED_PROPS *objp) {
	OV_RESULT result;

	result = KS_DATAPACKET_write_xdr_OV_HIST_TYPE(serviceAnswer, &objp->historytype);
	if(Ov_Fail(result))
		return result;

	result = KS_DATAPACKET_write_xdr_OV_INTERPOLATION_MODE(serviceAnswer, &objp->default_interpolation);
	if(Ov_Fail(result))
		return result;

	result = KS_DATAPACKET_write_xdr_OV_INTERPOLATION_MODE(serviceAnswer, &objp->supported_interpolation);
	if(Ov_Fail(result))
		return result;

	return KS_DATAPACKET_write_xdr_string(serviceAnswer, &objp->type_identifier);
}

/*
*	XDR routine for OV_DOMAIN_ENGINEERED_PROPS
*/
OV_RESULT xdr_write_OV_DOMAIN_ENGINEERED_PROPS(KS_DATAPACKET* serviceAnswer, OV_DOMAIN_ENGINEERED_PROPS *objp) {

	return KS_DATAPACKET_write_xdr_string(serviceAnswer, &objp->class_identifier);
}

/*
*	XDR routine for OV_VAR_ENGINEERED_PROPS
*/
OV_RESULT xdr_write_OV_VAR_ENGINEERED_PROPS(KS_DATAPACKET* serviceAnswer, OV_VAR_ENGINEERED_PROPS *objp) {
	OV_RESULT result;

	if(objp->tech_unit && strlen(objp->tech_unit) > KS_TECHUNIT_MAXLEN)
		return OV_ERR_BADVALUE;
	result = KS_DATAPACKET_write_xdr_string(serviceAnswer, &objp->tech_unit);
	if(Ov_Fail(result))
		return result;

	return KS_DATAPACKET_write_xdr_OV_VAR_TYPE(serviceAnswer, &objp->vartype);
}

/*
*	XDR routine for OV_LINK_ENGINEERED_PROPS
*/
OV_RESULT xdr_write_OV_LINK_ENGINEERED_PROPS(KS_DATAPACKET* serviceAnswer, OV_LINK_ENGINEERED_PROPS *objp) {
	OV_RESULT result;

	result = KS_DATAPACKET_write_xdr_OV_LINK_TYPE(serviceAnswer, &objp->linktype);
	if(Ov_Fail(result))
		return result;

	result = KS_DATAPACKET_write_xdr_string(serviceAnswer, &objp->opposite_role_identifier);
	if(Ov_Fail(result))
		return result;

	return KS_DATAPACKET_write_xdr_string(serviceAnswer, &objp->association_identifier);
}


/*
 * XDR routine for OBJ engineered props
 */

OV_RESULT xdr_write_OV_OBJ_ENGINEERED_PROPS(KS_DATAPACKET* serviceAnswer, OV_OBJ_ENGINEERED_PROPS *objp) {
	OV_RESULT result;

	result = KS_DATAPACKET_write_xdr_OV_OBJ_TYPE(serviceAnswer, &objp->objtype);
	if(Ov_Fail(result))
		return result;
KS_logfile_debug(("xdr_OV_OBJ_ENGINEERED_PROPS: objecttype encoded"));
	switch(objp->objtype) {
	case KS_OT_HISTORY:
		result = xdr_write_OV_HISTORY_ENGINEERED_PROPS(serviceAnswer, &objp->OV_OBJ_ENGINEERED_PROPS_u.history_engineered_props);
		if(Ov_Fail(result))
			return result;
		break;

	case KS_OT_DOMAIN:
		result = xdr_write_OV_DOMAIN_ENGINEERED_PROPS(serviceAnswer, &objp->OV_OBJ_ENGINEERED_PROPS_u.domain_engineered_props);
		if(Ov_Fail(result))
			return result;
		break;

	case KS_OT_VARIABLE:
		result = xdr_write_OV_VAR_ENGINEERED_PROPS(serviceAnswer, &objp->OV_OBJ_ENGINEERED_PROPS_u.var_engineered_props);
		if(Ov_Fail(result))
			return result;
		break;

	case KS_OT_LINK:
		result = xdr_write_OV_LINK_ENGINEERED_PROPS(serviceAnswer, &objp->OV_OBJ_ENGINEERED_PROPS_u.link_engineered_props);
		if(Ov_Fail(result))
			return result;
		break;
	default:
		break;
	}
KS_logfile_debug(("xdr_OV_OBJ_ENGINEERED_PROPS: special properties encoded"));
	if(objp->identifier && strlen(objp->identifier) > KS_NAME_MAXLEN)
		return OV_ERR_BADVALUE;
	result = KS_DATAPACKET_write_xdr_string(serviceAnswer, &objp->identifier);
	if(Ov_Fail(result))
		return result;

	result = KS_DATAPACKET_write_xdr_OV_TIME(serviceAnswer, &objp->creation_time);
	if(Ov_Fail(result))
		return result;

	if(objp->comment && strlen(objp->comment) > KS_COMMENT_MAXLEN)
		return OV_ERR_BADVALUE;
	result = KS_DATAPACKET_write_xdr_string(serviceAnswer, &objp->comment);
	if(Ov_Fail(result))
		return result;

	result = KS_DATAPACKET_write_xdr_OV_ACCESS(serviceAnswer, &objp->access);
	if(Ov_Fail(result))
		return result;
KS_logfile_debug(("xdr_OV_OBJ_ENGINEERED_PROPS: only semantic flags missing"));
	return KS_DATAPACKET_write_xdr_OV_SEMANTIC_FLAGS(serviceAnswer, &objp->semantic_flags);
}



/*
 * routine to encode the results
 */

OV_RESULT ksxdr_getEP_encoderesults(KS_DATAPACKET* serviceAnswer, OV_GETEP_RES* results)
{
	/*
	 *	local variables
	 */
	OV_UINT					i;
	OV_UINT					items_len = results->items_len;
	OV_OBJ_ENGINEERED_PROPS*	pprops;
	OV_RESULT fncresult;
	/*
	 *	Note, that we do not have an array of items, but a linked list;
	 *	this routine may be used for encoding only!
	 */

	switch(results->result) {
	case OV_ERR_OK:
		/*
		 *	serialize the array size first
		 */
		fncresult = KS_DATAPACKET_write_xdr_u_long(serviceAnswer, &items_len);
		if(Ov_Fail(fncresult)) {
			return fncresult;
		}
KS_logfile_debug(("getEP_encoderesults: %d elements found", items_len));
		/*
		 *	now serialize the array elements
		 */
		pprops = results->pfirst;
		for(i=0; i<items_len; i++) {
			if(!pprops) {
				return OV_ERR_GENERIC;
			}
KS_logfile_debug(("getEP_encoderesults: encoding element %d,\n\tidentifier: %s", i, pprops->identifier));
			fncresult = xdr_write_OV_OBJ_ENGINEERED_PROPS(serviceAnswer, pprops);
			if(Ov_Fail(fncresult)) {
				return fncresult;
			}
			pprops = pprops->pnext;
		}
		break;
	default:
		break;
	}
	return OV_ERR_OK;
}


/*
 * this routine decodes the params, executes the service and encodes the results
 */

OV_RESULT ksxdr_getEP(const OV_UINT version, const OV_TICKET* pticket, KS_DATAPACKET* dataReceived, KS_DATAPACKET* serviceAnswer, OV_UINT* msgState, OV_UINT* ksErrCode)
{
	/*
	 *	parameter and result objects
	 */
	OV_GETEP_PAR	params;
	OV_GETEP_RES	results;
	OV_RESULT		fncresult;
	/*
	 *	decode the parameters
	 */
	ov_memstack_lock();
	fncresult = ksxdr_getEP_decodeparams(dataReceived, &params);
	if(Ov_OK(fncresult))
	{
		/*
		 *	properly decoded, call service function and send reply
		 */
		ov_ksserver_getep(version, pticket, &params, &results);
		*ksErrCode = results.result;
		KS_logfile_debug(("getEP: getEP issued: %s", ov_result_getresulttext(*ksErrCode)));
		fncresult = ksxdr_getEP_encoderesults(serviceAnswer, &results);
		KS_logfile_debug(("getEP: results encoded: %s", ov_result_getresulttext(fncresult)));
		ov_memstack_unlock();
		return OV_ERR_OK;
	}
	ov_memstack_unlock();

	return fncresult;
}
