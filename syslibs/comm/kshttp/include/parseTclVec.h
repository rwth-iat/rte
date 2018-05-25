/* returns a list of tcl vector elements with escape sequences*/
/* IMPORTANT: be sure to ov_memstack_lock/unlock() arround */
OV_RESULT parseTclVec(OV_STRING** list, const OV_STRING str, char open, char close, OV_UINT* len);
