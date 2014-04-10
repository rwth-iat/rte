
#include "ov_ksserver_backend.h"

/**
 * The read functions defined herein read data conforming to an xdr-stream from a KS_DATAPACKET
 * They all take a pointer to the datapacket as first parameter. The second parameter is a pointer to the variable where the data shall be stored
 * the read_string function takes the maximum length of the string to read (including the terminating '\0') as a third parameter.
 * The read functions for dynamic length types allocate memory using ov_malloc(n). Use ov_free() to free it. If no memory can be allocated the function returns OV_ERR_HEAPOUTOFMEMORY.
 * The check whether all pointers are set and whether the read pointer of the datapacket points inside its data. If everything is fine the functions return OV_ERR_OV otherwise OV_ERR_GENERIC.
 * The read functions for dynamic length types return OV_ERR_BADVALUE if the data they should read is longer than the maximum length.
 * If maximum length is 0 no upper bound is assumed.
 * If no data is read, the read pointer is not modified. Otherwise it is shifted to the next data to be read.
 */

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_uint(KS_DATAPACKET* datapacket, OV_UINT* value);
OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_uint(KS_DATAPACKET* datapacket, const OV_UINT* value);

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_int(KS_DATAPACKET* datapacket, OV_INT* value);
OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_int(KS_DATAPACKET* datapacket, const OV_INT* value);

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_single(KS_DATAPACKET* datapacket, OV_SINGLE* value);
OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_single(KS_DATAPACKET* datapacket, const OV_SINGLE* value);

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_double(KS_DATAPACKET* datapacket, OV_DOUBLE* value);
OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_double(KS_DATAPACKET* datapacket, const OV_DOUBLE* value);

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_string(KS_DATAPACKET* datapacket, OV_STRING* value, OV_UINT maxlen);
OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_string_wolength(KS_DATAPACKET* datapacket, OV_STRING* value);
OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_string_tomemstack(KS_DATAPACKET* datapacket, OV_STRING* value, OV_UINT maxlen);
OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_string_tomemstack_wolength(KS_DATAPACKET* datapacket, OV_STRING* value);
OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_string(KS_DATAPACKET* datapacket, const OV_STRING* value);

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_opaque(KS_DATAPACKET* datapacket, char** value, OV_UINT* decLength, OV_UINT maxlen);
OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_opaque_tomemstack(KS_DATAPACKET* datapacket, char** value, OV_UINT* decLength, OV_UINT maxlen);
OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_opaque(KS_DATAPACKET* datapacket, const char* value, OV_UINT length);

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_opaque_fixedlength(KS_DATAPACKET* datapacket, char** value, OV_UINT length);
OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_opaque_fixedlength(KS_DATAPACKET* datapacket, const char* value, OV_UINT length);

/*
 * XDR routines for arrays
 */

typedef OV_RESULT (*xdr_readfncptr)(KS_DATAPACKET* datapacket, void* value);
typedef OV_RESULT (*xdr_writefncptr)(KS_DATAPACKET* datapacket, void* value);

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_array_tomemstack(KS_DATAPACKET* datapacket, void** value, OV_UINT elementSize,
		OV_UINT* length, xdr_readfncptr);

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_array(KS_DATAPACKET* datapacket, void** value, OV_UINT elementSize,
		OV_UINT* length, xdr_writefncptr);

/*
 * OV-specific XDR routines that are used for more than one service
 * The read functions allocate space on the memstack for vectors, structs and strings. memory for the parameters themselves is NOT allocated within the read functions
 */
OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_OV_TIME(KS_DATAPACKET* datapacket, OV_TIME *ptime);
OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_OV_TIME(KS_DATAPACKET* datapacket, const OV_TIME* ptime);

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_OV_TIME_SPAN(KS_DATAPACKET* datapacket, OV_TIME_SPAN *ptimespan);
OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_OV_TIME_SPAN(KS_DATAPACKET* datapacket, const OV_TIME_SPAN* ptimespan);

OV_RESULT xdr_read_OV_VAR_CURRENT_PROPS(KS_DATAPACKET* dataReceived, OV_VAR_CURRENT_PROPS* currprops);
OV_RESULT xdr_write_OV_VAR_CURRENT_PROPS(KS_DATAPACKET* serviceAnswer, OV_VAR_CURRENT_PROPS* pProps);

OV_RESULT xdr_read_OV_GETVAR_ITEM (KS_DATAPACKET* dataPacket, OV_GETVAR_ITEM *pItem);
OV_RESULT xdr_write_OV_GETVAR_ITEM (KS_DATAPACKET* serviceAnswer, OV_GETVAR_ITEM *pItem);

OV_RESULT xdr_read_OV_SETVAR_ITEM (KS_DATAPACKET* dataReceived, OV_SETVAR_ITEM* item);
OV_RESULT xdr_write_OV_SETVAR_ITEM (KS_DATAPACKET* dataPacket, OV_SETVAR_ITEM* item);

OV_RESULT xdr_read_OV_PLACEMENT (KS_DATAPACKET* dataReceived, OV_PLACEMENT* pplacement);
OV_RESULT xdr_write_OV_PLACEMENT (KS_DATAPACKET* dataPacket, OV_PLACEMENT* pplacement);

OV_RESULT xdr_read_OV_LINK_ITEM (KS_DATAPACKET* dataReceived, OV_LINK_ITEM*	pitem);
OV_RESULT xdr_write_OV_LINK_ITEM (KS_DATAPACKET* dataPacket, OV_LINK_ITEM*	pitem);

/*
 * XDR functions for OV_VAR_VALUE (ANY variable)
 * The read function allocates space on the memstack for vectors and strings. the space for the OV_VAR_VALUE-object is NOT allocated within the read function
 */

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_OV_VAR_VALUE(KS_DATAPACKET* datapacket, OV_VAR_VALUE* value);
OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_OV_VAR_VALUE(KS_DATAPACKET* datapacket, OV_VAR_VALUE* value);

/*
 * OV-Specific routines that are used for only one service
 * the functions' definitions are located in the respective files (e.g. ksxdr_getPP.c)
 */

/*	getPP	*/
OV_RESULT xdr_read_OV_VAR_PROJECTED_PROPS(KS_DATAPACKET* dataPacket, OV_VAR_PROJECTED_PROPS *objp);
OV_RESULT xdr_write_OV_VAR_PROJECTED_PROPS(KS_DATAPACKET* serviceAnswer, OV_VAR_PROJECTED_PROPS *objp);

OV_RESULT xdr_read_OV_LINK_PROJECTED_PROPS(KS_DATAPACKET* dataPacket, OV_LINK_PROJECTED_PROPS *objp);
OV_RESULT xdr_write_OV_LINK_PROJECTED_PROPS(KS_DATAPACKET* serviceAnswer, OV_LINK_PROJECTED_PROPS *objp);

OV_RESULT xdr_read_OV_OBJ_PROJECTED_PROPS(KS_DATAPACKET* dataPacket, OV_OBJ_PROJECTED_PROPS *objp);
OV_RESULT xdr_write_OV_OBJ_PROJECTED_PROPS(KS_DATAPACKET* serviceAnswer, OV_OBJ_PROJECTED_PROPS *objp);

/*	getEP	*/
OV_RESULT xdr_read_OV_HISTORY_ENGINEERED_PROPS(KS_DATAPACKET* dataPacket, OV_HISTORY_ENGINEERED_PROPS *objp);
OV_RESULT xdr_write_OV_HISTORY_ENGINEERED_PROPS(KS_DATAPACKET* serviceAnswer, OV_HISTORY_ENGINEERED_PROPS *objp);

OV_RESULT xdr_read_OV_DOMAIN_ENGINEERED_PROPS(KS_DATAPACKET* dataPacket, OV_DOMAIN_ENGINEERED_PROPS *objp);
OV_RESULT xdr_write_OV_DOMAIN_ENGINEERED_PROPS(KS_DATAPACKET* serviceAnswer, OV_DOMAIN_ENGINEERED_PROPS *objp);

OV_RESULT xdr_read_OV_VAR_ENGINEERED_PROPS(KS_DATAPACKET* dataPacket, OV_VAR_ENGINEERED_PROPS *objp);
OV_RESULT xdr_write_OV_VAR_ENGINEERED_PROPS(KS_DATAPACKET* serviceAnswer, OV_VAR_ENGINEERED_PROPS *objp);

OV_RESULT xdr_read_OV_LINK_ENGINEERED_PROPS(KS_DATAPACKET* dataPacket, OV_LINK_ENGINEERED_PROPS *objp);
OV_RESULT xdr_write_OV_LINK_ENGINEERED_PROPS(KS_DATAPACKET* serviceAnswer, OV_LINK_ENGINEERED_PROPS *objp);

OV_RESULT xdr_read_OV_OBJ_ENGINEERED_PROPS(KS_DATAPACKET* dataPacket, OV_OBJ_ENGINEERED_PROPS *objp);
OV_RESULT xdr_write_OV_OBJ_ENGINEERED_PROPS(KS_DATAPACKET* serviceAnswer, OV_OBJ_ENGINEERED_PROPS *objp);

/*	CreateObject	*/
OV_RESULT xdr_read_OV_CREATEOBJ_ITEM (KS_DATAPACKET* dataReceived, OV_CREATEOBJ_ITEM* pitem);
OV_RESULT xdr_write_OV_CREATEOBJ_ITEM (KS_DATAPACKET* dataPacket, OV_CREATEOBJ_ITEM* pitem);
OV_RESULT xdr_read_OV_CREATEOBJECTITEM_RES (KS_DATAPACKET* datapacket, OV_CREATEOBJECTITEM_RES* pitem);
OV_RESULT xdr_write_OV_CREATEOBJECTITEM_RES (KS_DATAPACKET* datapacket, OV_CREATEOBJECTITEM_RES* pitem);

/*	RenameObject	*/
OV_RESULT xdr_read_OV_RENAMEOBJECT_ITEM (KS_DATAPACKET* datapacket, OV_RENAMEOBJECT_ITEM* pitem);
OV_RESULT xdr_write_OV_RENAMEOBJECT_ITEM (KS_DATAPACKET* datapacket, OV_RENAMEOBJECT_ITEM* pitem);

/*	UnLink	*/
OV_RESULT xdr_read_OV_UNLINK_ITEM (KS_DATAPACKET* dataReceived,	OV_UNLINK_ITEM* pitem);
OV_RESULT xdr_write_OV_UNLINK_ITEM (KS_DATAPACKET* dataPacket,	OV_UNLINK_ITEM* pitem);

/*
*	XDR routines for enumeration values
*/
#define KS_DATAPACKET_read_xdr_OV_BOOL(datapacket, bool)					KS_DATAPACKET_read_xdr_int(datapacket, (OV_INT*) bool)
#define KS_DATAPACKET_read_xdr_OV_ENUM(datapacket, enumeration)					KS_DATAPACKET_read_xdr_int(datapacket, (OV_INT*) enumeration)
#define KS_DATAPACKET_read_xdr_OV_TICKET_TYPE			KS_DATAPACKET_read_xdr_OV_ENUM
#define KS_DATAPACKET_read_xdr_OV_VAR_TYPE				KS_DATAPACKET_read_xdr_OV_ENUM
#define KS_DATAPACKET_read_xdr_OV_STATE					KS_DATAPACKET_read_xdr_OV_ENUM
#define KS_DATAPACKET_read_xdr_OV_OBJ_TYPE				KS_DATAPACKET_read_xdr_OV_ENUM
#define KS_DATAPACKET_read_xdr_OV_ACCESS				KS_DATAPACKET_read_xdr_OV_ENUM
#define KS_DATAPACKET_read_xdr_OV_LINK_TYPE				KS_DATAPACKET_read_xdr_OV_ENUM
#define KS_DATAPACKET_read_xdr_OV_HIST_TYPE				KS_DATAPACKET_read_xdr_OV_ENUM
#define KS_DATAPACKET_read_xdr_OV_PLACEMENT_HINT		KS_DATAPACKET_read_xdr_OV_ENUM
#define KS_DATAPACKET_read_xdr_OV_EP_FLAGS				KS_DATAPACKET_read_xdr_OV_ENUM
#define KS_DATAPACKET_read_xdr_OV_TIME_TYPE				KS_DATAPACKET_read_xdr_OV_ENUM
#define KS_DATAPACKET_read_xdr_OV_INTERPOLATION_MODE	KS_DATAPACKET_read_xdr_OV_ENUM
#define KS_DATAPACKET_read_xdr_OV_HSEL_TYPE				KS_DATAPACKET_read_xdr_OV_ENUM
#define KS_DATAPACKET_read_xdr_OV_RESULT				KS_DATAPACKET_read_xdr_OV_ENUM

#define KS_DATAPACKET_write_xdr_OV_BOOL(datapacket, bool)					KS_DATAPACKET_write_xdr_int(datapacket, (OV_INT*) bool)
#define KS_DATAPACKET_write_xdr_OV_ENUM(datapacket, enumeration)					KS_DATAPACKET_write_xdr_int(datapacket, (OV_INT*) enumeration)
#define KS_DATAPACKET_write_xdr_OV_TICKET_TYPE			KS_DATAPACKET_write_xdr_OV_ENUM
#define KS_DATAPACKET_write_xdr_OV_VAR_TYPE				KS_DATAPACKET_write_xdr_OV_ENUM
#define KS_DATAPACKET_write_xdr_OV_STATE					KS_DATAPACKET_write_xdr_OV_ENUM
#define KS_DATAPACKET_write_xdr_OV_OBJ_TYPE				KS_DATAPACKET_write_xdr_OV_ENUM
#define KS_DATAPACKET_write_xdr_OV_ACCESS				KS_DATAPACKET_write_xdr_OV_ENUM
#define KS_DATAPACKET_write_xdr_OV_LINK_TYPE				KS_DATAPACKET_write_xdr_OV_ENUM
#define KS_DATAPACKET_write_xdr_OV_HIST_TYPE				KS_DATAPACKET_write_xdr_OV_ENUM
#define KS_DATAPACKET_write_xdr_OV_PLACEMENT_HINT		KS_DATAPACKET_write_xdr_OV_ENUM
#define KS_DATAPACKET_write_xdr_OV_EP_FLAGS				KS_DATAPACKET_write_xdr_OV_ENUM
#define KS_DATAPACKET_write_xdr_OV_TIME_TYPE				KS_DATAPACKET_write_xdr_OV_ENUM
#define KS_DATAPACKET_write_xdr_OV_INTERPOLATION_MODE	KS_DATAPACKET_write_xdr_OV_ENUM
#define KS_DATAPACKET_write_xdr_OV_HSEL_TYPE				KS_DATAPACKET_write_xdr_OV_ENUM
#define KS_DATAPACKET_write_xdr_OV_RESULT				KS_DATAPACKET_write_xdr_OV_ENUM

/*
 * More XDR routine defines
 */

#define KS_DATAPACKET_read_xdr_OV_SEMANTIC_FLAGS				KS_DATAPACKET_read_xdr_uint
#define KS_DATAPACKET_write_xdr_OV_SEMANTIC_FLAGS				KS_DATAPACKET_write_xdr_uint



