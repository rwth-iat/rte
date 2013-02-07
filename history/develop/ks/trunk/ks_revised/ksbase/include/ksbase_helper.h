
#ifndef ksbase_helper_INCLUDED
#define ksbase_helper_INCLUDED

#include "libov/ov_ov.h"
#include "ksbase.h"

OV_DLLFNCEXPORT OV_BOOL ks_isvalidname(OV_STRING name);

OV_DLLFNCEXPORT void ksbase_free_KSDATAPACKET(KS_DATAPACKET* packet);

OV_DLLFNCEXPORT OV_RESULT ksbase_KSDATAPACKET_append(KS_DATAPACKET* packet, OV_BYTE* data, OV_UINT addlength);

OV_DLLFNCEXPORT OV_RESULT ksbase_KSDATAPACKET_set(KS_DATAPACKET* packet, OV_BYTE* data, OV_UINT addlength);

//association of ClientHandler
#define KSBASE_CH_NOTNEEDED			0
#define KSBASE_CH_ASSOCIATED			1
#define KSBASE_CH_NOTASSOCATIED		2
#define KSBASE_CH_NOTFOUND			252

#endif
