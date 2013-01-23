/*
 * servregids.h -- preprocessor defines for resources.
 *
 * (c) 1999 Harald Albrecht <harald@plt.rwth-aachen.de>
 * Chair of Process Control Engineering,
 * Aachen University of Technology, Germany.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must print or display the above
 *    copyright notice either during startup or must have a means for
 *    the user to view the copyright notice.
 * 3. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the Chair of Process Control Engineering nor the
 *    name of the Aachen University of Technology may be used to endorse or
 *    promote products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE CHAIR OF PROCESS CONTROL
 * ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef __SERVREGIDS_H
#define __SERVREGIDS_H


#define IDS_SERVICEID 10
#define IDS_SERVICEDISPLAY 11
#define IDS_SERVICEDEPS 12

#define IDS_REALLYDEINSTALL 59
#define IDS_OTHERSERVICE 58
#define IDS_NODEINSTALL 57
#define IDS_INSUFFICENTDEINSTACCESS 56
#define IDS_SHUTERR 55
#define IDS_DEPENDENTSHUTERR 54
#define IDS_DEPENDENTRETRERR 53
#define IDS_REALLYINSTALL 52
#define IDS_CANTRECONFIGURE	51
#define IDS_CANTREGISTER	50
#define IDS_STOPSERVICE	49
#define IDS_STARTSERVICE	48
#define IDS_CANTSTART	47
#define IDS_NOINSTALL	46
#define IDS_INSUFFICENTACCESS	45
#define IDS_NOPATH	44
#define IDS_DEINSTALLTITLE	43
#define IDS_INSTALLTITLE	42

#define DLG_STARTSERVICE	42
#define DLG_STOPSERVICE	    43

#define ICON_STARTSERVICE   42
#define ICON_STOPSERVICE    43


#endif
/* End of servregids.h */

