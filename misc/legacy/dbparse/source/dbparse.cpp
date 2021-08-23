/*
 * $Id: dbparse.cpp,v 1.7 2007-04-25 13:59:03 martin Exp $
 *
 * Copyright (c) 1996
 * Lehrstuhl fuer Prozessleittechnik
 * D-52064 Aachen, Germany.
 * All rights reserved.
 *
 * This file is part of the ACPLT/KS 
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

// ACPLT/OV text file parser includes
//
// Author : Christian Poensgen <chris@plt.rwth-aachen.de>
// dbparse.cpp
// last change: Jul 20, 2001

//-------------------------------------------------------------------------------

#include "ks/array.h"
#include "ks/client.h"
#include "ks/clntpath.h"
#include "ks/commobject.h"
#include "ks/objmgrparams.h"
#include "ks/path.h"
#include "ks/string.h"
#include "dbparse.h"

#if PLT_COMPILER_GCC || PLT_COMPILER_DECCXX || PLT_COMPILER_MSVC

#include "plt/hashtable_impl.h"
#include "plt/handle_impl.h"
#include "ks/array_impl.h"
#include "plt/list_impl.h"
//#include "plt/priorityqueue_impl.h"
#include "plt/container_impl.h"

#endif

#if PLT_COMPILER_BORLAND
#include "ks/array.h"
#include "ks/array_builtins.h"
#include "ks/handle.h"
#include "ks/list.h"
#endif

#if PLT_INSTANTIATE_TEMPLATES
#include "dbparse_inst.h"
#endif
