
#ifndef ksbase_helper_INCLUDED
#define ksbase_helper_INCLUDED

#include "libov/ov_ov.h"
#include "ksbase.h"

OV_DLLFNCEXPORT OV_BOOL ks_isvalidname(OV_STRING name);

OV_DLLFNCEXPORT void ksbase_free_KSDATAPACKET(KS_DATAPACKET* packet);

#endif
