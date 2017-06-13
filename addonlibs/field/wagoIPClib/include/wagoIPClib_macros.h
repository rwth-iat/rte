/*
*   Copyright (C) 2005
*   Chair of Process Control Engineering,
*   D-52056 Aachen, Germany.
*   All rights reserved.
*
*
*   This file is part of the ACPLT/OV Package 
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*/
/***********************************************************************
*
*	File
*	-----
*	wagolib_macros.h
*
*	Autoren:
*	--------
*	LE						Lars Evertz		<l.evertz@plt.rwth-aachen.de>
*
*	CVS:
*	----
*	$Revision: 
*
*	Historie:
*	--------
*	25th-February-2011	LE
*		-	file created
*
***********************************************************************/

#ifndef WAGOLIB_MACROS_H_INCLUDED
#define WAGOLIB_MACROS_H_INCLUDED

#define kbus_checkerr(var)	if(res)	\
	{	\
		pinst->v_Error = FALSE;	\
		switch(var)	\
		{	\
			case KBUS_ERROR_BUSY:	\
				ov_logfile_error("%s: error: kbus busy\n", pinst->v_identifier);	\
				pinst->v_ErrorCode = KBUS_ERROR_BUSY;	\
				ov_string_setvalue(&pinst->v_ErrorString, KBUS_ERRORSTR_BUSY);	\
				break;	\
			case KBUS_ERROR_NODEV:	\
				ov_logfile_error("%s: error: DP-Ram not initialized\n", pinst->v_identifier);	\
				pinst->v_ErrorCode = KBUS_ERROR_NODEV;	\
				ov_string_setvalue(&pinst->v_ErrorString, KBUS_ERRORSTR_NODEV);	\
				break;	\
			case KBUS_ERROR_IO:	\
				ov_logfile_error("%s: error: kbus failed to answer\n", pinst->v_identifier);	\
				pinst->v_ErrorCode = KBUS_ERROR_IO;	\
				ov_string_setvalue(&pinst->v_ErrorString, KBUS_ERRORSTR_IO);	\
				break;	\
			case KBUS_ERROR_TIME:	\
				ov_logfile_error("%s: error: communication timed out\n", pinst->v_identifier);	\
				pinst->v_ErrorCode = KBUS_ERROR_TIME;	\
				ov_string_setvalue(&pinst->v_ErrorString, KBUS_ERRORSTR_TIME);	\
				break;	\
		}	\
		return;	\
	}

#endif
