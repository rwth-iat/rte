/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/array.cpp,v 1.9 1999-09-16 10:54:46 harald Exp $ */
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

//////////////////////////////////////////////////////////////////////
// Author: Martin Kneissl <martin@plt.rwth-aachen.de>
//////////////////////////////////////////////////////////////////////

#include "ks/array.h"

#if !PLT_SEE_ALL_TEMPLATES
#include "ks/array_impl.h"
#endif

#if !PLT_COMPILER_BORLAND  && !PLT_COMPILER_DECCXX
#include "ks/array_builtins.h"
#endif

//#if PLT_COMPILER_BORLAND >= 0x0530
//#include "ks/array_builtins.h"
//#endif

//////////////////////////////////////////////////////////////////////
// explicit instantiation

#if PLT_INSTANTIATE_TEMPLATES && !PLT_COMPILER_BORLAND && !PLT_COMPILER_DECCXX && !PLT_COMPILER_CYGWIN

template class KsArray<bool>;
template class KsArray<char>;
template class KsArray<u_long>;
template class KsArray<long>;
template class KsArray<u_int>;
template class KsArray<int>;
template class KsArray<short>;
template class KsArray<u_short>;
template class KsArray<float>;
template class KsArray<double>;

#endif // PLT_INSTANTIATE_TEMPLATES

//////////////////////////////////////////////////////////////////////


// ks/array.cpp

