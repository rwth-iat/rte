/*
*   $Id: ov_version.h,v 1.3 1999-08-06 09:37:21 dirk Exp $
*
*   Copyright (C) 1998-1999
*   Lehrstuhl fuer Prozessleittechnik,
*   RWTH Aachen, D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
*
*   This file is part of the ACPLT/OV Package which is licensed as open
*   source under the Artistic License; you can use, redistribute and/or
*   modify it under the terms of that license.
*
*   You should have received a copy of the Artistic License along with
*   this Package; see the file ARTISTIC-LICENSE. If not, write to the
*   Copyright Holder.
*
*   THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
*   WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
*   OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/
/*
*	History:
*	--------
*	13-Jan-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	08-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/

#ifndef OV_VERSION_H_INCLUDED
#define OV_VERSION_H_INCLUDED

/*
*	Version information for different modules of ACPLT/OV
*	-----------------------------------------------------
*/
#define OV_VER_CODEGEN					"0.1.0pre6"
#define OV_VER_LIBOV					"0.1.0pre6"
#define OV_VER_DBUTIL 					"0.1.0pre6"
#define OV_VER_LIBOVKS 					"0.1.0pre6"
#define OV_VER_SERVER					"0.1.0pre6"
#define OV_VER_NTSERVICE				"0.1.0pre6"

#define OV_VER_FILEVERSION_CODEGEN		0, 1, 0, 6
#define OV_VER_FILEVERSION_LIBOV		0, 1, 0, 6
#define OV_VER_FILEVERSION_DBUTIL 		0, 1, 0, 6
#define OV_VER_FILEVERSION_LIBOVKS 		0, 1, 0, 6
#define OV_VER_FILEVERSION_SERVER		0, 1, 0, 6
#define OV_VER_FILEVERSION_NTSERVICE	0, 1, 0, 6

#define OV_VER_PRODUCTVERSION_CODEGEN	0, 1, 0, 6
#define OV_VER_PRODUCTVERSION_LIBOV		0, 1, 0, 6
#define OV_VER_PRODUCTVERSION_DBUTIL 	0, 1, 0, 6
#define OV_VER_PRODUCTVERSION_LIBOVKS 	0, 1, 0, 6
#define OV_VER_PRODUCTVERSION_SERVER	0, 1, 0, 6
#define OV_VER_PRODUCTVERSION_NTSERVICE	0, 1, 0, 6

#define OV_VER_LICENSE															\
	"Copyright (C) 1998-1999 "													\
	"Lehrstuhl fuer Prozessleittechnik, "										\
	"RWTH Aachen, D-52056 Aachen, Germany. "									\
	"All rights reserved.\r\n"													\
	"\r\n"																		\
	"This file is part of the ACPLT/OV Package which is licensed as open "		\
	"source under the Artistic License; you can use, redistribute and/or "		\
	"modify it under the terms of that license.\r\n"							\
	"\r\n"																		\
	"You should have received a copy of the Artistic License along with "		\
	"this Package; see the file ARTISTIC-LICENSE. If not, write to the "		\
	"Copyright Holder.\r\n"														\
	"\r\n"																		\
	"THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED "	\
	"WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES "		\
	"OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.\r\n"

#ifdef OV_USE_WINDRES
#define VOS_UNKNOWN			(0L)
#define VOS_DOS				(0x10000L)
#define VOS_OS216			(0x20000L)
#define VOS_OS232			(0x30000L)
#define VOS_NT				(0x40000L)
#define VOS_DOS_WINDOWS16	(0x10001L)
#define VOS_DOS_WINDOWS32	(0x10004L)
#define VOS_OS216_PM16		(0x20002L)
#define VOS_OS232_PM32		(0x30003L)
#define VOS_NT_WINDOWS32	(0x40004L)
#define VFT_UNKNOWN			(0L)
#define VFT_APP				(0x1L)
#define VFT_DLL				(0x2L)
#define VFT_DRV				(0x3L)
#define VFT_FONT			(0x4L)
#define VFT_VXD				(0x5L)
#define VFT_STATIC_LIB		(0x7L)
#endif

#endif
/*
*	End of file
*/

