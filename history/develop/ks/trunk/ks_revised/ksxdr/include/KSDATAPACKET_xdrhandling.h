
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

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_u_long(KS_DATAPACKET* datapacket, OV_UINT* value);
OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_u_long(KS_DATAPACKET* datapacket, const OV_UINT* value);

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_long(KS_DATAPACKET* datapacket, OV_INT* value);
OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_long(KS_DATAPACKET* datapacket, const OV_INT* value);

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

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_OV_TIME(KS_DATAPACKET* datapacket, OV_TIME *ptime);
OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_OV_TIME(KS_DATAPACKET* datapacket, const OV_TIME* ptime);

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_OV_TIME_SPAN(KS_DATAPACKET* datapacket, OV_TIME_SPAN *ptimespan);
OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_OV_TIME_SPAN(KS_DATAPACKET* datapacket, const OV_TIME_SPAN* ptimespan);


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
 * XDR functions for OV_VAR_VALUE (ANY variable)
 * The read function allocates space on the memstack for vectors and strings. the space for the OV_VAR_VAALUE-object is NOT allocated within the read function
 */

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_OV_VAR_VALUE(KS_DATAPACKET* datapacket, OV_VAR_VALUE* value);
OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_OV_VAR_VALUE(KS_DATAPACKET* datapacket, OV_VAR_VALUE* value);

/*
*	XDR routines for enumeration values
*/
#define KS_DATAPACKET_read_xdr_OV_BOOL(datapacket, bool)					KS_DATAPACKET_read_xdr_long(datapacket, (OV_INT*) bool)
#define KS_DATAPACKET_read_xdr_OV_ENUM(datapacket, enumeration)					KS_DATAPACKET_read_xdr_long(datapacket, (OV_INT*) enumeration)
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

#define KS_DATAPACKET_write_xdr_OV_BOOL(datapacket, bool)					KS_DATAPACKET_write_xdr_long(datapacket, (OV_INT*) bool)
#define KS_DATAPACKET_write_xdr_OV_ENUM(datapacket, enumeration)					KS_DATAPACKET_write_xdr_long(datapacket, (OV_INT*) enumeration)
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

#define KS_DATAPACKET_read_xdr_OV_SEMANTIC_FLAGS				KS_DATAPACKET_read_xdr_u_long
#define KS_DATAPACKET_write_xdr_OV_SEMANTIC_FLAGS				KS_DATAPACKET_write_xdr_u_long



