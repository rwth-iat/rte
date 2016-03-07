/*
*	Copyright (C) 2004
*	Chair of Process Control Engineering,
*	Aachen University of Technology.
*	All rights reserved.
*
*	Redistribution and use in source and binary forms, with or without
*	modification, are permitted provided that the following conditions
*	are met:
*	1. Redistributions of source code must retain the above copyright
*	   notice, this list of conditions and the following disclaimer.
*	2. Redistributions in binary form must print or display the above
*	   copyright notice either during startup or must have a means for
*	   the user to view the copyright notice.
*	3. Redistributions in binary form must reproduce the above copyright
*	   notice, this list of conditions and the following disclaimer in
*		the documentation and/or other materials provided with the
*		distribution.
*	4. Neither the name of the Chair of Process Control Engineering nor
*		the name of the Aachen University of Technology may be used to
*		endorse or promote products derived from this software without
*		specific prior written permission.
*
*	THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
*	``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE CHAIR OF
*	PROCESS CONTROL ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT,
*	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
*	OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
*	AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
*	WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*	POSSIBILITY OF SUCH DAMAGE.
*/
/***********************************************************************
*
*	File
*	-----
*	com_macros.h
*
*	Autoren:
*	--------
*	MF							Murat Fedai <Murat@plt.rwth-aachen.de>
*	St							Stefan Schmitz <StefanS@plt.rwth-aachen.de>
*
*	CVS:
*	----
*	$Revision: 1.2 $
*	$Date: 2005-08-30 14:24:16 $
*
*	Historie:
*	--------
*	15-August-2000			MF
*		-	Erstellung
*
*	23-Oktober-2000		MF
*		-	Einfuehrung von Vektorstrukturen
*
*	31-Oktober-2000		MF
*		-	Einfuehrung von Time und Timespan
*
*	29-April-2004			St
*		-	Neustrukturierung
*
*	18-Juli-2005			St
*		-	Belegungskommandos in cmdlib (cmd.h) überführt
*
*	30-August-2005			St
*		-	CVS-CheckIn
*
***********************************************************************/

#ifndef _COM_LIB_H
#define _COM_LIB_H




/***********************************************************************
	Variablentypen
***********************************************************************/

#define VAR_UINT        	10
#define VAR_INT         	11
#define VAR_SINGLE      	12
#define VAR_DOUBLE      	13
#define VAR_STRING      	14
#define VAR_BOOL        	15
#define VAR_TIME        	16
#define VAR_TIME_SPAN   	17

#define VAR_UINT_VEC    	18
#define VAR_INT_VEC     	19
#define VAR_SINGLE_VEC  	20
#define VAR_DOUBLE_VEC  	21
#define VAR_STRING_VEC  	22
#define VAR_BOOL_VEC    	23
#define VAR_TIME_VEC    	24
#define VAR_TIME_SPAN_VEC	25

#define VAR_COM         	29

#define VAR_INT_PV      	30
#define VAR_BOOL_PV     	31
#define VAR_SINGLE_PV   	32

#define GET_VAR         	80
#define SET_VAR         	90
#define SET_VAR_PV      	91

#define SET_COMMAND    		190

/***********************************************************************
	Stati für Flag: connstate
		-	Reihenfolge ist wichtig: Zahl fuer READY muss groesser als
			'Startstati=NOT_CREATED;CREATED;OPENING' sein
			(siehe beginn setrequest) 
***********************************************************************/

#define CONN_NOT_CREATED			0
#define CONN_CREATED					1
#define CONN_OPENING					2
#define CONN_ERR_HOST_UNKNOWN		3
#define CONN_ERR_SERVER_UNKNOWN	4
#define CONN_ERR_CANT_CONTACT		5
#define CONN_ERR_TIME_OUT			6
#define CONN_ERR_NO_MANAGER		7
#define CONN_ERR_DEFAULT			8
#define CONN_ERR_OK					9
#define CONN_READY					10
#define CONN_BUSY						11

#define CONN_TEXT_NOTCREATED		"Not created"
#define CONN_TEXT_CREATED			"Created"
#define CONN_TEXT_OPENING			"Opening"

#define CONN_TEXT_SERVERUNKNOWN	"Server unknown"
#define CONN_TEXT_HOSTUNKNOWN		"Host unknown"
#define CONN_TEXT_CANTCONTACT		"Can't contact"
#define CONN_TEXT_TIMEOUT			"Timeout"
#define CONN_TEXT_NOMANAGER		"No manager"
#define CONN_TEXT_GENERIC			"Generic error"

#define CONN_TEXT_READY				"Ready"
#define CONN_TEXT_BUSY				"Busy"

/***********************************************************************
	Definition von Errorcodes fuer die Komunikation mit dem Manager
***********************************************************************/

#define COM_ERR_OK						0
#define COM_ERR_GENERIC					1

#define COM_ERR_BADPARAM				100 
#define COM_ERR_ALLREADYEXISTS		101
#define COM_ERR_BADDATATYPE			110 

#define COM_ERR_BADPATHMANAGER		102
#define COM_ERR_BADOBJTYPEMANAGER	103
#define COM_ERR_NOACCESSMANAGER		104
 
#define COM_ERR_BADPATHLOCALINST		105
#define COM_ERR_BADOBJTYPELOCALINST	106

#define COM_ERR_BADPATHSTATUS			107
#define COM_ERR_BADOBJTYPESTATUS		108
#define COM_ERR_BADDATATYPESTATUS	109

#define COM_STATUS_OK					0
#define COM_STATUS_BUSY					1 
#define COM_STATUS_ERR					300 

#define COM_ERR_BADRESULT				200 
#define COM_ERR_NEWSTATUS				201


/***********************************************************************
	Errorcodes die von ACPLT/KS zurueckgeliefert werden koennen:
***********************************************************************/

/* 
KS_ERR_OK              0 (int)
KS_ERR_GENERIC         1 (int)
KS_ERR_TARGETGENERIC   6 (int)
KS_ERR_BADAUTH         2 (int)
KS_ERR_UNKNOWNAUTH     5 (int)
KS_ERR_NOTIMPLEMENTED  3 (int)
KS_ERR_BADPARAM        4 (int)
KS_ERR_BADOBJTYPE      7 (int)
KS_ERR_BADNAME        16 (int)
KS_ERR_BADPATH        17 (int)
KS_ERR_BADMASK        18 (int)
KS_ERR_NOACCESS       19 (int)
KS_ERR_BADTYPE        20 (int)
. . .
*/

/***********************************************************************
	Definition von TEXT Errorcodes fuer die Komunikation mit dem Manager
***********************************************************************/

#define COM_TEXT_OK							"OK"
#define COM_TEXT_GENERIC					"Generic Error"

#define COM_TEXT_BADPARAM					"Bad Parameter" 
#define COM_TEXT_ALLREADYEXISTS			"Already exists"
#define COM_TEXT_BADDATATYPE				"Bad Datatype" 

#define COM_TEXT_BADPATHMANAGER			"Bad path to manager"
#define COM_TEXT_BADOBJTYPEMANAGER		"Bad objecttype manager"
#define COM_TEXT_NOACCESSMANAGER			"No access to manager"
 
#define COM_TEXT_BADPATHLOCALINST		"Bad path to inst.var"
#define COM_TEXT_BADOBJTYPELOCALINST	"Bad objecttype inst"

#define COM_TEXT_BADPATHSTATUS			"Bad path to status"
#define COM_TEXT_BADOBJTYPESTATUS		"Bad objecttye status"
#define COM_TEXT_BADDATATYPESTATUS		"Bad datatype status"

#define COM_TEXT_STATUS_OK					"Status Ok"
#define COM_TEXT_STATUS_BUSY				"Status Busy"
#define COM_TEXT_STATUS_ERR				"Status Error"

#define COM_TEXT_BADRESULT					"Bad Result"
#define COM_TEXT_NEWSTATUS					"New Status"

	
#endif
