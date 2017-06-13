/*
 * $Id: dbdump.h,v 1.7 2007-04-25 13:59:03 martin Exp $
 *
 * Copyright (c) 1996
 * Lehrstuhl fuer Prozessleittechnik
 * D-52064 Aachen, Germany.
 * All rights reserved.
 *
 * This file is part of the ACPLT/KS Package 
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

// Includes, defines and typedefs for ACPLT/OV database dumper
// Author : Christian Poensgen <chris@plt.rwth-aachen.de>
// dbdump.h
// last change: Jan 25, 2002

/////////////////////////////////////////////////////////////////////////////

#ifndef KS_DBDUMP_H_INCLUDED
#define KS_DBDUMP_H_INCLUDED

#include <time.h>

//---------------------------------------------------------------------------------

#include "ks/client.h"
#include "ks/clntpath.h"
#include "ks/commobject.h"
//#include "ks/stdconnectionmgr.h"
#include "ks/path.h"

#if PLT_COMPILER_GCC || PLT_COMPILER_DECCXX
#include "plt/hashtable_impl.h"
#include "plt/handle_impl.h"
#include "plt/list_impl.h"
#include "plt/priorityqueue_impl.h"
#include "plt/container_impl.h"
#endif

#if PLT_COMPILER_BORLAND
#include "ks/array.h"
#include "ks/array_builtins.h"
#include "ks/handle.h"
#include "ks/list.h"
#endif

#if PLT_INSTANTIATE_TEMPLATES
//#include "dbdump_inst.h"
#endif

#if PLT_COMPILER_MSVC
#include <ctype.h>
#include <io.h>
#endif


//---------------------------------------------------------------------------------

// Indent output INDENTATION spaces per level
#define INDENTATION				3

// Dump options constants
#define KS_DO_CLASS_IDENT		0x1			// Dump the class identifier
#define KS_DO_CREATION_TIME		0x2			// Dump the creation time
#define KS_DO_SEM_FLAGS			0x4			// Dump the semantic flags
#define KS_DO_COMMENT			0x8			// Dump the comment

#define KS_DO_VAR_TIME			0x10		// Dump the variable timestamp
#define KS_DO_VAR_STATE			0x20		// Dump the variable "reliability" state
#define KS_DO_VAR_SEM_FLAGS		0x40		// Dump the variable semantic flags
#define KS_DO_VAR_TECH_UNIT		0x80		// Dump the technical unit of the variable
#define KS_DO_VAR_COMMENT		0x100		// Dump the variable comment

#define KS_DO_ALL				0x1FF		// Dump all information

// Structure specifying the search parameters
typedef struct search_params {
	KsString			host_and_server;
	KsString			path;
	KscPath				*kscpath;
	KsString			identifier;			// identifier of the comm. object, wildcards allowed
	KsTime				from_creation_time;
	KsTime				to_creation_time;
	KsString			comment;
	bool				relative;
	KS_ACCESS			access;
	KS_SEMANTIC_FLAGS	semantic_flags;
} search_t;

typedef search_t* search_p;

// Bit field for the dump options
typedef u_long dump_opt_t;

typedef dump_opt_t* dump_opt_p;

#endif //KS_DBDUMP_H_INCLUDED

// End of dbdump.h
