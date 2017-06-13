
#   $Id: linker.cmd,v 1.1 1999-07-19 15:01:56 dirk Exp $
#
#   Copyright (C) 1998
#   Lehrstuhl fuer Prozessleittechnik,
#   D-52056 Aachen, Germany.
#   All rights reserved.
#   
#   Author: Stefan Heinkel <stefan@plt.rwth-aachen.de>
#   
#   This file is part of the ACPLT/OV Package 
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.

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
  
