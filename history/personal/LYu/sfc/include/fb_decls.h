
/**
 * Declarations of the FB library required by the FC library.
 */

int fb_vartype_implemented (OV_VAR_TYPE typ);

OV_RESULT fb_varvalue_changed
(const OV_ELEMENT * pelem, const OV_ANY * pvarcurrprops, OV_BOOL * changed);

OV_BOOL fb_connection_getelements (OV_INSTPTR_fb_connection pconn);

int fb_connectiontype_implemented (OV_VAR_TYPE typ);
