
#   $Id: linker.cmd,v 1.1 1999-07-19 15:01:56 dirk Exp $
#
#   Copyright (C) 1998-1999
#   Lehrstuhl fuer Prozessleittechnik,
#   RWTH Aachen, D-52056 Aachen, Germany.
#   All rights reserved.
#   
#   Author: Stefan Heinkel <stefan@plt.rwth-aachen.de>
#   
#   This file is part of the ACPLT/OV Package which is licensed as open
#   source under the Artistic License; you can use, redistribute and/or
#   modify it under the terms of that license.
#   
#   You should have received a copy of the Artistic License along with
#   this Package; see the file ARTISTIC-LICENSE. If not, write to the
#   Copyright Holder.
#   
#   THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
#   WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
#   OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.

ovserver.obj, &
G:\plt\S7\BUILD\CANLIB.LIB, &
G:\plt\acplt\ov\build\mc164\libovks.lib, &
G:\plt\acplt\ov\build\mc164\libov.lib, &
G:\plt\libmpm\libmpm.lib &
TO MC &
RE (0x4-0x8B, 0x90-0xFF,0x104-0x209, 0x4000-0xBFFF, 0x10000-0x1FFFF) &
CLASSES(NDATA0 (0x20A-0x3FFF),&
		NDATA  (0x20A-0x3FFF),&
		NCONST (0x20A-0x3FFF),&
		NCODE  (0x20A-0x3FFF),&
		HDATA0 (0x20000-0x40000),&
		HDATA  (0x20000-0x40000),&
		FCODE  (0x20000-0x40000)) 
  
