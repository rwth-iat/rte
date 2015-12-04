/* 	Created by Sten Grüner on 08.06.11 			*/

/* 	This file is processed by the cdt in conjunction
	with org.eclipse.cdt.core.prefs file to fix 
	static-analysis errors. Workaround for the bug 
	https://bugs.eclipse.org/bugs/show_bug.cgi?id=197989 	*/

#define OV_INSTBODY_ov_object

struct {												\
}	__classinfo;

#include "libov\ov_association.h"
#include "ov.h"
#include "fb.h"
