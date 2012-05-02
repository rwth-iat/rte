/** @file 
* Assertion from a CommunicationComponent to a Message
* Communication component iterates over this 1:N Link for retrieval of its sending
* Messages
******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_msgsys
#define OV_COMPILE_LIBRARY_msgsys
#endif


#include "msgsys.h"
#include "libov/ov_association.h"

#include "libov/ov_macros.h"

OV_IMPL_LINK(msgsys_AssertionMsgSys2Message)
OV_IMPL_UNLINK(msgsys_AssertionMsgSys2Message)
OV_IMPL_GETACCESS(msgsys_AssertionMsgSys2Message)

