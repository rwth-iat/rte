// -*-plt-c++-*-
/* $Header: /home/david/cvs/acplt/ks/examples/pmobile.cpp,v 1.5 2007-04-25 12:57:20 martin Exp $ */
/*
 * Copyright (c) 1996, 1997, 1998, 1999
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
/* Author: Harald Albrecht <harald@plt.rwth-aachen.de> */

#include "ks/client.h"
#include "ks/clntpath.h"
#include "ks/commobject.h"
#include "ks/package.h"

#if PLT_SEE_ALL_TEMPLATES
#include "plt/hashtable.h"
#include "plt/priorityqueue.h"
#include "ks/array.h"
#include "ks/list.h"
#include "ks/handle.h"
#include "plt/sort.h"
#else
#include "plt/hashtable_impl.h"
#include "plt/priorityqueue_impl.h"
#include "ks/array_impl.h"
#include "ks/list_impl.h"
#include "ks/handle_impl.h"
#include "plt/sort_impl.h"
#endif

#if PLT_COMPILER_BORLAND
#include "ks/array_builtins.h"
#endif

#if PLT_INSTANTIATE_TEMPLATES
#include "pmobile_inst.h"
#endif


// End of pmobile.cpp



