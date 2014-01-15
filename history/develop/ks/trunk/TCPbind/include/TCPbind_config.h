#ifndef TCPbind_confih_INCLUDED
#define TCPbind_confih_INCLUDED

#include "ksbase_helper.h"

//Size of data received per chunk
#define IPbind_CHUNKSIZE			1460	/*	maximum size of tcp segment over ethernet IP 	*/

//timeouts for destroying inactive Channels
#define IPbind_TTL_AFTER_ASSOC		1200
#define IPbind_TTL_BEFORE_ASSOC	60

//timeout after receiving an incomplete package (erase of data)
#define IPbind_TIMEOUT_PARTIALPACKAGE	300

#endif
