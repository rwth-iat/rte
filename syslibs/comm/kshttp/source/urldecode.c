#include "config.h"
#include <ctype.h>

/* Converts a hex character to its integer value */
static char from_hex(char ch) {
	return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}

/* Returns a url-decoded version of str Public Domain code from http://www.geekhideout.com/urlcode.shtml*/
/* IMPORTANT: be sure to ov_memstack_lock/unlock() arround */
OV_STRING url_decode(OV_STRING str) {
	OV_STRING pstr = str;
	OV_STRING buf = ov_memstack_alloc(strlen(str) + 1);
	OV_STRING pbuf = buf;
	while (*pstr) {
		if (*pstr == '%') {
			if (pstr[1] && pstr[2]) {
				*pbuf++ = from_hex(pstr[1]) << 4 | from_hex(pstr[2]);
				pstr += 2;
			}
		} else if (*pstr == '+') {
			*pbuf++ = ' ';
		} else {
			*pbuf++ = *pstr;
		}
		pstr++;
	}
	*pbuf = '\0';
	return buf;
}

