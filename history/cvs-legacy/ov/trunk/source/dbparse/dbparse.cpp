/*
 * Copyright (c) 1996, 1997, 1998, 1999, 2000
 * Lehrstuhl fuer Prozessleittechnik, RWTH Aachen
 * D-52064 Aachen, Germany.
 * All rights reserved.
 *
 * This file is part of the ACPLT/KS Package which is licensed as open
 * source under the Artistic License; you can use, redistribute and/or
 * modify it under the terms of that license.
 *
 * You should have received a copy of the Artistic License along with
 * this Package; see the file ARTISTIC-LICENSE. If not, write to the
 * Copyright Holder.
 *
 * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
 * OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

// Author : Christian Poensgen <chris@plt.rwth-aachen.de>

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
