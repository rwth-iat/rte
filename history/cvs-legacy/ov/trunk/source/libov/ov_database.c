/*
*   $Id: ov_database.c,v 1.16 2002-06-20 08:47:36 ansgar Exp $
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
*	13-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	07-Jun-2001 J.Nagelmann <nagelmann@ltsoft.de>: Changes for Sun Solaris.
*/

#define OV_COMPILE_LIBOV

#include "libov/ov_database.h"
#include "libov/ov_object.h"
#include "libov/ov_library.h"
#include "libov/ov_string.h"
#include "libov/ov_time.h"
#include "libov/ov_macros.h"



#if OV_SYSTEM_SOLARIS
#include "/usr/ucbinclude/sys/file.h"
#include <sys/file.h>
#include <fcntl.h>
int flock (int filedes, int oper)
{
  struct flock the_lock;
  switch (oper) {
  case LOCK_EX:
    the_lock.l_type = F_WRLCK;
    break;
  case LOCK_UN:
    the_lock.l_type = F_UNLCK;
    break;
  default:
    return (-1);
    break;
  }
  the_lock.l_start=0;
  the_lock.l_whence=SEEK_SET;
  the_lock.l_len=0;
  return (fcntl(filedes,F_SETLKW, &the_lock));
}

#endif

#if OV_SYSTEM_UNIX
#include <sys/mman.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#if OV_SYSTEM_NT || OV_SYSTEM_RMOS
#include <fcntl.h>
#include <io.h>
#endif

#if OV_SYSTEM_OPENVMS
#include <rms.h>
#include <rmsdef.h>
#include <secdef.h>
#include <ssdef.h>
#include <starlet.h>
#endif

#ifndef __STDC__
#define __STDC__ 1
#include "mp_malloc.h"
#undef __STDC__
#else
#include "mp_malloc.h"
#endif

#if OV_SYSTEM_MC164
#define memset xmemset
#define memcpy xmemcpy
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Global variable: database pointer, file handles and stuff
*/
OV_DLLVAREXPORT OV_DATABASE_INFO OV_MEMSPEC	*pdb = NULL;

#if OV_SYSTEM_UNIX
static int			fd;
#endif

#if OV_SYSTEM_NT
static HANDLE		hfile;
static HANDLE		hmap;
#endif

#if OV_SYSTEM_RMOS
FILE				*file;
#endif

#if OV_SYSTEM_OPENVMS
unsigned short		channel;
#endif

#if OV_SYSTEM_MC164
OV_DATABASE_INFO OV_MEMSPEC	*pdbmem;
#endif

/*
*	VTable of any object in case we dont start up the database
*/
static OV_VTBL_ov_object nostartupvtable = {
	ov_object_constructor_nostartup,
	ov_object_checkinit,
	ov_object_destructor,
	ov_object_startup,
	ov_object_shutdown,
	ov_object_getaccess_nostartup,
	ov_object_getflags,
	ov_object_getcomment,
	ov_object_gettechunit,
	ov_object_getvar,
	ov_object_setvar
};

/*	----------------------------------------------------------------------	*/

/*
*	Helper macro: Round up the size to the next n*BLOCKSIZE
*/
#define Ov_Roundup(size) (((long)(size)+BLOCKSIZE-1)&~(BLOCKSIZE-1))

/*	----------------------------------------------------------------------	*/

/*
*	Macro: pointer to memory pool info structure
*/
#define pmpinfo ((mp_info OV_MEMSPEC *)(pdb+1))

/*	----------------------------------------------------------------------	*/

/*
*	Get more core memory from the memory pool (subroutine)
*/
__ptr_t ov_database_morecore(
	__malloc_ptrdiff_t	size
) {
	/*
	*	local variables
	*/
	__ptr_t	pmem = (__ptr_t)(pdb->pcurr);
	OV_BYTE	OV_MEMSPEC *psoon = pdb->pcurr+size;
#if OV_SYSTEM_UNIX
	char	null = 0;
#endif
	/*
	*	test if we can get/release memory
	*/
	if (psoon<pdb->pstart) {
		return NULL;
	}
	if(psoon > pdb->pend) {
#if OV_DYNAMIC_DATABASE
		/*
		*	check database file size
		*/
		if(pdb->size+Ov_Roundup(size) > OV_DATABASE_MAXSIZE) {
			return NULL;
		}
#if OV_SYSTEM_UNIX
		/*
		*	extend the database file in size
		*/
		if(lseek(fd, pdb->size+Ov_Roundup(size)-sizeof(char), SEEK_SET) < 0) {
			return NULL;
		}
		if(write(fd, (void*)&null, sizeof(char)) == -1) {
			return NULL;
		}
#endif
#if OV_SYSTEM_NT
		/*
		*	extend the database file in size
		*/
		if(SetFilePointer(hfile, pdb->size+Ov_Roundup(size), NULL, FILE_BEGIN) 
			== 0xFFFFFFFF
		) {
			return NULL;
		}
		if(!SetEndOfFile(hfile)) {
			return NULL;
		}
#endif
		pdb->pend += Ov_Roundup(size);
		pdb->size += Ov_Roundup(size);
#else
		return NULL;
#endif
	}
	/*
	*	yes we can. get/release it.
	*/
	pdb->pcurr = psoon;
	return (__ptr_t)pmem;
}

/*	----------------------------------------------------------------------	*/

/*
*	Create a new database
*/
OV_DLLFNCEXPORT OV_RESULT ov_database_create(
	OV_STRING	filename,
	OV_UINT		size
) {
	/*
	*	local variables
	*/
	OV_RESULT		result;
#if !OV_SYSTEM_MC164
	FILE			*fp;
#endif
#if OV_SYSTEM_UNIX
	static char		null = 0;
#endif
#if OV_SYSTEM_OPENVMS
	struct FAB		fab;
	OV_BYTE			*inaddr[2], *retaddr[2];
	unsigned long	status;
#endif
	/*
	*	check parameters
	*/
	if(pdb) {
		return OV_ERR_GENERIC;
	}
	if(!size || (size > OV_DATABASE_MAXSIZE) || !filename) {
		return OV_ERR_BADPARAM;
	}
#if !OV_SYSTEM_MC164
	/*
	*	check, if file already exists
	*/
	fp = fopen(filename, "r");
	if(fp) {
		fclose(fp); 
		return OV_ERR_FILEALREADYEXISTS;
	}
#endif
#if OV_SYSTEM_UNIX
	/*
	*	create the new database file
	*/
	fd = open(filename, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP);
	if(fd == -1) {
		return OV_ERR_CANTCREATEFILE;
	}
	/*
	*	lock the database file
	*/
#if OV_SYSTEM_SOLARIS
	if(flock(fd, LOCK_EX)) {
		close(fd);
		return OV_ERR_CANTLOCKFILE;
	}
#else 
	if(flock(fd, LOCK_EX | LOCK_NB)) {
		close(fd);
		return OV_ERR_CANTLOCKFILE;
	}
#endif
	/*
	*	make the file "size" bytes long (rounded up)
	*/
	if(lseek(fd, Ov_Roundup(size)-sizeof(char), SEEK_SET) < 0) {
		close(fd);
		return OV_ERR_CANTWRITETOFILE;
	}
	if(write(fd, (void*)&null, sizeof(char)) == -1) {
		close(fd);
		return OV_ERR_CANTWRITETOFILE;
	}
	/*
	*	map the file to memory
	*/
	pdb = (OV_DATABASE_INFO*)mmap(0, OV_DATABASE_MAXSIZE,
		PROT_READ | PROT_WRITE,	MAP_SHARED, fd, 0);
	if(pdb == (OV_DATABASE_INFO*)-1) {
		pdb = NULL;
		close(fd);
		return OV_ERR_CANTMAPFILE;
	}
#endif
#if OV_SYSTEM_NT
	/*
	*	check file size
	*/
	if(Ov_Roundup(size) > OV_DATABASE_MAXSIZE) {
		return OV_ERR_CANTCREATEFILE;
	}
	/*
	*	create the new database file
	*/
	hfile = CreateFile(filename, GENERIC_READ | GENERIC_WRITE,
		0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hfile == INVALID_HANDLE_VALUE) {
		return OV_ERR_CANTCREATEFILE;
	}
	/*
	*	make the file size approriate (rounded up size)
	*/
	if(SetFilePointer(hfile, Ov_Roundup(size), NULL, FILE_BEGIN) 
		== 0xFFFFFFFF
	) {
		CloseHandle(hfile);
		return OV_ERR_CANTWRITETOFILE;
	}
	if(!SetEndOfFile(hfile)) {
		CloseHandle(hfile);
		return OV_ERR_CANTWRITETOFILE;
	}
	/*
	*	create a file mapping object
	*/
#if OV_DYNAMIC_DATABASE
	hmap = CreateFileMapping(hfile, NULL, PAGE_READWRITE,
		0, OV_DATABASE_MAXSIZE, NULL);
#else
	hmap = CreateFileMapping(hfile, NULL, PAGE_READWRITE,
		0, Ov_Roundup(size), NULL);
#endif
	if(!hmap) {
		CloseHandle(hfile);
		return OV_ERR_CANTMAPFILE;
	}
	/*
	*	map the file to memory
	*/
	pdb = (OV_DATABASE_INFO*)MapViewOfFile(hmap,
		FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if(!pdb) {
		CloseHandle(hmap);
		CloseHandle(hfile);
		return OV_ERR_CANTMAPFILE;
	}
#endif
#if OV_SYSTEM_MC164
	pdb = pdbmem;
#endif
#if OV_SYSTEM_RMOS
	/*
	*	get database memory
	*/
	pdb = (OV_DATABASE_INFO*)Ov_HeapMalloc(Ov_Roundup(size));
	if(!pdb) {
		return OV_ERR_HEAPOUTOFMEMORY;
	}
	file = fopen(filename, "wb+");
	if(!file) {
		Ov_HeapFree(pdb);
		return OV_ERR_CANTCREATEFILE;
	}
#endif
#if OV_SYSTEM_OPENVMS
	/*
	*	initialize file access block (FAB)
	*/
	fab = cc$rms_fab;
	fab.fab$v_ufo = 1;						/* user file open */
	fab.fab$l_fna = filename;				/* filename address */
	fab.fab$b_fns = strlen(filename);		/* filename size */
	fab.fab$l_alq = Ov_Roundup(size) >> 9;	/* Allocation quantity (file size) */
	/*
	*	create database file if none exists
	*/
	status = sys$create(&fab);
	if(!(status & 1)) {
		return OV_ERR_CANTCREATEFILE;
	}
	/*
	*	get channel from the FAB
	*/
	channel = fab.fab$l_stv;
	/*
	*	initialize address hint: just an address in P0 space
	*/
	inaddr[0] = inaddr[1] = NULL;
	/*
	*	map the file to memory
	*/
	status = sys$crmpsc(inaddr, retaddr, 0, SEC$M_WRT | SEC$M_DZRO | SEC$M_EXPREG,
		0, 0, 0, channel, 0, 0, 0, 0);
	if(!(status & 1)) {
		sys$dassgn(channel);
		return OV_ERR_CANTMAPFILE;
	}
	/*
	*	ensure that we are the first opener of the database file
	*/
	if(status != SS$_CREATED) {
		ov_database_unmap();
		return OV_ERR_CANTCREATEFILE;
	}
	/*
	*	assign database pointer
	*/
	Ov_AbortIfNot(Ov_Roundup(size) == retaddr[1]-retaddr[0]+1);
	pdb = (OV_DATABASE_INFO*)retaddr[0];
#endif
	/*
	*	initialize database structure
	*/
	Ov_AbortIfNot(pdb);
	memset(pdb, 0, sizeof(OV_DATABASE_INFO));
	pdb->baseaddr = (OV_POINTER)pdb;
	pdb->size = Ov_Roundup(size);
	pdb->pstart = pdb->pcurr = (OV_BYTE*)Ov_Roundup(pmpinfo+1);
	pdb->pend = (OV_BYTE*)((long)pdb+Ov_Roundup(size));
	pdb->started = FALSE;
	/*
	*	initialize the database memory pool
	*/
	if(!mp_initialize(pmpinfo, ov_database_morecore)) {
		ov_database_unmap();
		return OV_ERR_DBOUTOFMEMORY;
	}
	/*
	*	initialize the root domain object
	*/
	Ov_AbortIfNot(Ov_OK(ov_string_setvalue(&pdb->root.v_identifier, 
		OV_OBJNAME_ROOT)));
	ov_time_gettime(&pdb->root.v_creationtime);
	/*
	*	initialize the acplt domain object
	*/
	Ov_AbortIfNot(Ov_OK(ov_string_setvalue(&pdb->acplt.v_identifier, 
		OV_OBJNAME_ACPLT)));
	ov_time_gettime(&pdb->acplt.v_creationtime);
	/*
	*	initialize the OV library object
	*/
	Ov_AbortIfNot(Ov_OK(ov_string_setvalue(&pdb->ov.v_identifier, 
		OV_OBJNAME_OV)));
	ov_time_gettime(&pdb->ov.v_creationtime);
	/*
	*	load the OV library
	*/
	result = ov_library_load(&pdb->ov, &OV_LIBRARY_DEF_ov);
	if(Ov_Fail(result)) {
		ov_database_unmap();
		return result;
	}
	/*
	*	initialize the vendor tree
	*/
	ov_vendortree_setdatabasename(filename);
	Ov_AbortIfNot(Ov_OK(ov_vendortree_init()));
#if OV_SYSTEM_RMOS
	/*
	*	flush the database file
	*/
	ov_database_flush();
#endif
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Map an existing database
*/
OV_DLLFNCEXPORT OV_RESULT ov_database_map(
	OV_STRING	filename
) {
	/*
	*	local variables
	*/
	OV_RESULT	result;
	OV_POINTER	baseaddr = NULL;
	OV_UINT		size = 0;
#if OV_SYSTEM_NT
	DWORD		bytesread;
#endif
#if OV_SYSTEM_OPENVMS
	struct FAB		fab;
	OV_BYTE			*inaddr[2], *retaddr[2];
	unsigned long	status;
#endif
	/*
	*	check parameters
	*/
	if(pdb) {
		return OV_ERR_GENERIC;
	}
	if(!filename) {
		return OV_ERR_BADPARAM;
	}
#if OV_SYSTEM_UNIX
	/*
	*	open the database file
	*/
	fd = open(filename, O_RDWR);
	if(fd == -1) {
		return OV_ERR_CANTOPENFILE;
	}
	/*
	*	lock the database file
	*/
#if OV_SYSTEM_SOLARIS
	if(flock(fd, LOCK_EX)) {
		close(fd);
		return OV_ERR_CANTLOCKFILE;
	}
#else 
	if(flock(fd, LOCK_EX | LOCK_NB)) {
		close(fd);
		return OV_ERR_CANTLOCKFILE;
	}
#endif
	/*
	*	read the base address
	*/
	if(read(fd, (void*)&baseaddr, sizeof(OV_POINTER)) == -1) {
		close(fd);
		return OV_ERR_CANTREADFROMFILE;
	}
	/*
	*	read the filemapping size
	*/
	if(read(fd, (void*)&size, sizeof(OV_UINT)) == -1) {
		close(fd);
		return OV_ERR_CANTREADFROMFILE;
	}
	/*
	*	map the file to memory
	*/
	pdb = (OV_DATABASE_INFO*)mmap(baseaddr, OV_DATABASE_MAXSIZE,
		PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(pdb == (OV_DATABASE_INFO*)-1) {
		pdb = NULL;
		close(fd);
		return OV_ERR_CANTMAPFILE;
	}
#endif
#if OV_SYSTEM_NT
	/*
	*	open the database file
	*/
	hfile = CreateFile(filename, GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hfile == INVALID_HANDLE_VALUE) {
		return OV_ERR_CANTOPENFILE;
	}
	/*
	*	read the base address
	*/
	if(!ReadFile(hfile, (LPVOID)&baseaddr, sizeof(OV_POINTER),
		&bytesread, NULL)
	) {
		CloseHandle(hfile);
		return OV_ERR_CANTREADFROMFILE;
	}
	/*
	*	read the filemapping size
	*/
	if(!ReadFile(hfile, (LPVOID)&size, sizeof(OV_UINT),
		&bytesread, NULL)
	) {
		CloseHandle(hfile);
		return OV_ERR_CANTREADFROMFILE;
	}
	/*
	*	check file size
	*/
	if(GetFileSize(hfile, NULL) != size) {
		CloseHandle(hfile);
		return OV_ERR_BADDATABASE;
	}
	/*
	*	create a file mapping object
	*/
#if OV_DYNAMIC_DATABASE
	hmap = CreateFileMapping(hfile, NULL, PAGE_READWRITE,
		0, OV_DATABASE_MAXSIZE, NULL);
#else
	hmap = CreateFileMapping(hfile, NULL, PAGE_READWRITE,
		0, Ov_Roundup(size), NULL);
#endif
	if(!hmap) {
		CloseHandle(hfile);
		return OV_ERR_CANTMAPFILE;
	}
	/*
	*	map the file to memory
	*/
	pdb = (OV_DATABASE_INFO*)MapViewOfFileEx(hmap, 
		FILE_MAP_ALL_ACCESS, 0, 0, 0, (LPVOID)baseaddr);
	if(!pdb) {
		pdb = (OV_DATABASE_INFO*)MapViewOfFile(hmap,
			FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if(!pdb) {
			CloseHandle(hmap);
			CloseHandle(hfile);
			return OV_ERR_CANTMAPFILE;
		}
	}
#endif
#if OV_SYSTEM_MC164
	pdb = pdbmem;
#endif
#if OV_SYSTEM_RMOS
	/*
	*	open the database file
	*/
	file = fopen(filename, "rb+");
	if(!file) {
		return OV_ERR_CANTOPENFILE;
	}
	/*
	*	read the base address
	*/
	if(fread((char*)&baseaddr, sizeof(OV_POINTER), 1, file) != 1) {
		fclose(file);
		return OV_ERR_CANTREADFROMFILE;
	}
	/*
	*	read the filemapping size
	*/
	if(fread((char*)&size, sizeof(OV_UINT), 1, file) != 1) {
		fclose(file);
		return OV_ERR_CANTREADFROMFILE;
	}
	/*
	*	get database memory
	*/
	pdb = (OV_DATABASE_INFO*)Ov_HeapMalloc(Ov_Roundup(size));
	if(!pdb) {
		return OV_ERR_HEAPOUTOFMEMORY;
	}
	/*
	*	read the file to memory
	*/
	if(fseek(file, 0, SEEK_SET) < 0) {
		Ov_HeapFree(pdb);
		fclose(file);
		return OV_ERR_CANTREADFROMFILE;
	}
	if(fread((char*)pdb, size, 1, file) != 1) {
		Ov_HeapFree(pdb);
		fclose(file);
		return OV_ERR_CANTREADFROMFILE;
	}
#endif
#if OV_SYSTEM_OPENVMS
	/*
	*	initialize FAB
	*/
	fab = cc$rms_fab;
	fab.fab$v_ufo = 1;					/* user file open */
	fab.fab$l_fna = filename;			/* filename address */
	fab.fab$b_fns = strlen(filename);	/* filename size */
	fab.fab$v_upd = 1;					/* update access */
	/*
	*	open database file
	*/
	status = sys$open(&fab);
	if(!(status & 1)) {
		return OV_ERR_CANTOPENFILE;
	}
	/*
	*	get channel from the FAB
	*/
	channel = fab.fab$l_stv;
	/*
	*	initialize address hint: just an address in P0 space
	*/
	inaddr[0] = inaddr[1] = NULL;
	/*
	*	map the file to memory
	*/
	status = sys$crmpsc(inaddr, retaddr, 0, SEC$M_WRT | SEC$M_EXPREG,
		0, 0, 0, channel, 0, 0, 0, 0);
	if(!(status & 1)) {
		sys$dassgn(channel);
		return OV_ERR_CANTMAPFILE;
	}
#if 0	/* how??? */
	/*
	*	ensure that we are the first opener of the database file
	*/
	if(status != SS$_CREATED) {
		ov_database_unmap();
		return OV_ERR_CANTOPENFILE;
	}
#endif
	/*
	*	assign database pointer and database size
	*/
	pdb = (OV_DATABASE_INFO*)retaddr[0];
	size = retaddr[1]-retaddr[0]+1;
#endif
	/*
	*	test if we got the same filemapping size
	*/
	if (!pdb) {
		ov_database_unmap();
		return OV_ERR_BADDATABASE;
	}
#if OV_SYSTEM_MC164
	/* nothing to do */
#else
	if(size != pdb->size) {
		ov_database_unmap();
		return OV_ERR_BADDATABASE;
	}
#endif
	/*
	*	test if we got the same base address
	*/
	if((OV_POINTER)pdb != pdb->baseaddr) {
		result = ov_database_move((OV_BYTE*)pdb-(OV_BYTE*)(pdb->baseaddr));
		if(Ov_Fail(result)) {
			ov_database_unmap();
			return result;
		}
	}
	/*
	*	restart the database memory pool
	*/
	if(!mp_restart(pmpinfo, ov_database_morecore)) {
		ov_database_unmap();
		return OV_ERR_BADDATABASE;
	}
	/*
	*	load the OV library
	*/
	result = ov_library_compare(&pdb->ov, &OV_LIBRARY_DEF_ov);
	if(Ov_Fail(result)) {
		return result;
	}
	/*
	*	initialize the database
	*/
	ov_database_init();
	/*
	*	finally initialize the vendor tree
	*/
	ov_vendortree_setdatabasename(filename);
	return result;
}

/*	----------------------------------------------------------------------	*/

/*
*	Unmap the database
*/
OV_DLLFNCEXPORT void ov_database_unmap(void) {
	/*
	*	local variables
	*/
#if OV_SYSTEM_OPENVMS
	OV_BYTE	*inaddr[2];
#endif
	/*
	*	instructions
	*/
	/*
	*	shut down database if not already done
	*/
	if (pdb)	ov_database_shutdown();
#if OV_SYSTEM_UNIX
	/*
	*	unmap the file and close it
	*/
#if OV_SYSTEM_SOLARIS
	if (pdb)	munmap((caddr_t )pdb, pdb->size);
#else
	if (pdb) munmap(pdb, pdb->size);
#endif
	if (fd) close(fd);
#endif
#if OV_SYSTEM_NT
	/*
	*	unmap the file and close all handles
	*/
	if (pdb)   UnmapViewOfFile(pdb);
	if (hmap)  CloseHandle(hmap);
	if (hfile) CloseHandle(hfile);
#endif
#if OV_SYSTEM_RMOS
	/*
	*	flush the database file, close it and free the database memory
	*/
	if (pdb)  ov_database_flush();
	if (file) fclose(file);
	if (pdb)  Ov_HeapFree(pdb);
#endif
#if OV_SYSTEM_OPENVMS
	/*
	*	delete virtual address space and close the channel
	*/
	if (pdb) {
		inaddr[0] = (OV_BYTE*)pdb;
		inaddr[1] = inaddr[0]+pdb->size-1;
		sys$deltva(inaddr, 0, 0);
	}
	if (channel) sys$dassgn(channel);
#endif
	pdb = NULL;
	ov_vendortree_setdatabasename(NULL);
}

/*	----------------------------------------------------------------------	*/

/*
*	Flush the contents of a database
*/
OV_DLLFNCEXPORT void ov_database_flush(void) {
	/*
	*	local variables
	*/
#if OV_SYSTEM_OPENVMS
	OV_BYTE	*inaddr[2];
#endif
	/*
	*	instructions
	*/
	if(pdb) {
#if OV_SYSTEM_UNIX
		msync((void*)pdb, pdb->size, MS_SYNC);
#endif
#if OV_SYSTEM_NT
		FlushViewOfFile((LPCVOID)pdb, 0);
#endif
#if OV_SYSTEM_RMOS
		/*
		*	write the database memory to the file
		*/
		if(fseek(file, 0, SEEK_SET) < 0) {
			Ov_Warning("can't flush database");
			return;
		}
		if(fwrite((char*)pdb, pdb->size, 1, file) != 1) {
			Ov_Warning("can't flush database");
			return;
		}
		if(fflush(file)) {
			Ov_Warning("can't flush database");
			return;
		}
#endif
#if OV_SYSTEM_OPENVMS
		/*
		*	synchronously flush the virtual address space
		*/
		inaddr[0] = (OV_BYTE*)pdb;
		inaddr[1] = inaddr[0]+pdb->size-1;
		sys$updsecw(inaddr, 0, 0, 0, 0, 0, 0, 0);
#endif
	}
}
/*	----------------------------------------------------------------------	*/

/*
*	Write database to backupfile
*/
OV_DLLFNCEXPORT OV_RESULT ov_database_write(OV_STRING dbname) {

	FILE				*backupfile;
	
	backupfile = fopen(dbname, "wb+");
	if(!backupfile) {
		return OV_ERR_CANTOPENFILE;
	}
	fseek(backupfile, 0, SEEK_SET);
	fwrite((char*)pdb, pdb->size, 1, backupfile);
	fclose(backupfile);
	return OV_ERR_OK;
}
/*	----------------------------------------------------------------------	*/

/*
*	Initialize the database (subroutine)
*/
void ov_database_init(void) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_library		plib;
	OV_INSTPTR_ov_association	passoc;
	OV_INSTPTR_ov_class			pclass;
	OV_INSTPTR_ov_variable		pvar;
	/*
	*	iterate over all libraries except the OV library
	*/
	Ov_ForEachChildEx(ov_instantiation, pclass_ov_library, plib, ov_library) {
		if(plib != &pdb->ov) {
			/*
			*	Set the DLL/shared library handle to zero
			*/
			plib->v_handle = 0;
			/*
			*	Iterate over all associations of the libray
			*/
			Ov_ForEachChildEx(ov_containment, plib, passoc, ov_association) {
				/*
				*	Set function pointers to NULL
				*/
				passoc->v_linkfnc = NULL;
				passoc->v_unlinkfnc = NULL;
				passoc->v_getaccessfnc = NULL;
			}	/* associations */
			/*
			*	Iterate over all classes of the libray
			*/
			Ov_ForEachChildEx(ov_containment, plib, pclass, ov_class) {
				/*
				*	Set the vtable pointer to the "nostartup" vtable pointer
				*/
				pclass->v_pvtable = &nostartupvtable;
				/*
				*	Iterate for all variables of the class
				*/
				Ov_ForEachChildEx(ov_containment, pclass, pvar, ov_variable) {
					/*
					*	Set function pointers to NULL
					*/
					pvar->v_setfnc = NULL;
					pvar->v_getfnc = NULL;
				}	/* variables */
			}	/* classes */
		}	/* if */
	}	/* libraries */
	/*
	*	library is not started up
	*/
	pdb->started = FALSE;
}

/*	----------------------------------------------------------------------	*/

/*
*	Start up the database
*/
OV_DLLFNCEXPORT OV_RESULT ov_database_startup(void) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_library	plib;
	OV_LIBRARY_DEF			*plibdef;
	OV_RESULT				result;
	/*
	*	don't start if already started
	*/
	if(pdb->started) {
		return OV_ERR_GENERIC;
	}
	/*
	*	open and compare all libraries 
	*/
	Ov_ForEachChildEx(ov_instantiation, pclass_ov_library, plib, ov_library) {
		plibdef = ov_library_open(plib);
		if(!plibdef) {
			return OV_ERR_CANTOPENLIBRARY;
		}
		result = ov_library_compare(plib, plibdef);
		if(Ov_Fail(result)) {
			ov_database_init();
			return result;
		}
	}
	/*
	*	call startup method of root object
	*/
	ov_object_startup(Ov_PtrUpCast(ov_object, &pdb->root));
	/*
	*	library is started
	*/
	pdb->started = TRUE;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Shut down the database
*/
OV_DLLFNCEXPORT void ov_database_shutdown(void) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_library	plib;
	/*
	*	shut down database
	*/
	if(pdb->started) {
		/*
		*	call shutdown method of root object
		*/
		ov_object_shutdown(Ov_PtrUpCast(ov_object, &pdb->root));
		/*
		*	close all libraries 
		*/
		Ov_ForEachChildEx(ov_instantiation, pclass_ov_library, plib, ov_library) {
			ov_library_close(plib);
		}
		pdb->started = FALSE;
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	Allocate memory in the database
*/
OV_DLLFNCEXPORT OV_POINTER ov_database_malloc(
	OV_UINT		size
) {
	if(pdb) {
		return mp_malloc(size);
	}
	return NULL;
}

/*	----------------------------------------------------------------------	*/

/*
*	Reallocate memory in the database
*/
OV_DLLFNCEXPORT OV_POINTER ov_database_realloc(
	OV_POINTER	ptr,
	OV_UINT		size
) {
	if(pdb) {
		return mp_realloc(ptr, size);
	}
	return NULL;
}

/*	----------------------------------------------------------------------	*/

/*
*	Free memory in the database
*/
OV_DLLFNCEXPORT void ov_database_free(
	OV_POINTER	ptr
) {
	if(pdb) {
		mp_free(ptr);
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	Get size of the database
*/
OV_DLLFNCEXPORT OV_UINT ov_database_getsize(void) {
	if(pdb) {
		return pdb->size;
	}
	return 0;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get free storage in the database
*/
OV_DLLFNCEXPORT OV_UINT ov_database_getfree(void) {
	if(pdb) {
		return pdb->pend-pdb->pcurr+pmpinfo->bytes_free;
	}
	return 0;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get used storage in the database
*/
OV_DLLFNCEXPORT OV_UINT ov_database_getused(void) {
	if(pdb) {
		return pdb->pstart-(OV_BYTE OV_MEMSPEC*)pdb+pmpinfo->bytes_used;
	}
	return 0;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get fragmentation of the database
*/
OV_DLLFNCEXPORT OV_UINT ov_database_getfrag(void) {
	/*
	*	local variables
	*/
	OV_UINT					i, num, sumsize, avgsize, logavgsize, sum;
	OV_INSTPTR_ov_class		pclass;
	struct list OV_MEMSPEC	*pnext;
	/*
	*	calculate average instance size
	*/
	if(pdb) {
		num = sumsize = 0;
		Ov_ForEachChildEx(ov_instantiation, pclass_ov_class, pclass, ov_class) {
			num++;
			sumsize += pclass->v_size;
		}
		avgsize = sumsize/num;
		/*
		*	calculate the log (base of 2) of the average instance size
		*/
		for(logavgsize=0; avgsize; avgsize >>= 1) {
			logavgsize++;
		}
		avgsize = sumsize/num;
		/*
		*	find out how many blocks of the average size there are
		*/
		sum = 0;
		for(i=logavgsize; i<BLOCKLOG; i++) {
			num = 0;
			for(pnext = pmpinfo->fraghead[i].next; pnext; pnext=pnext->next) {
				num++;
			}
			sum += num*(1<<(i-logavgsize));
		}
		sum += (pdb->pend-pdb->pcurr)/avgsize;
		return ((ov_database_getfree()-sum*avgsize)*100)/ov_database_getfree();
	}
	return 100;
}

/*	----------------------------------------------------------------------	*/

/*
*	Helper macros for moving the database to a new base address
*/
#define Ov_Adjust(type, ptr) 											\
	ptr = (type)(((ptr)?(((OV_BYTE*)(ptr))+distance):(NULL)))

/*	----------------------------------------------------------------------	*/

/*
*	Move the database to a new base address (subroutine)
*
*	When using linktables, you can't access the links without adjusting the linktablepointer before.
*	Also the association offsets in this linktable may have been changed (in comparison to the standard
*	offsets you get by using 'ov_association_getparentoffset' and 'ov_association_getchildoffset')
*	in a mapped database. So at least we need the knowledge about the associations 'containment' and
*	'instantiation' which are stored in the database info object . We will search the 'ov' domain
*	for a contained object 'association' and will then look for it's instances, hence the other ov-associations.
*	After this we can use the standard ov_object_move, which uses the knowledge about the associations in the ov-model.
*
*/
OV_RESULT ov_database_move(
	const OV_INT	distance
) {
	/*
	*	local variables
	*/
	OV_DATABASE_INFO	*pdbcopy;
	int					i;
	struct list			*phead;
	struct alignlist	*pblock;
	OV_RESULT			result;
	OV_INSTPTR_ov_object 		pobj;
	OV_INST_ov_association 		assoc_inheritance;
	OV_INST_ov_association 		assoc_childrelationship;
	OV_INST_ov_association 		assoc_parentrelationship;
	/*
	*	search inheritance, childrelationship, parentrelationship association
	*/
	passoc_ov_containment = &pdb->containment;
	passoc_ov_instantiation = &pdb->instantiation;

	assoc_inheritance.v_assoctype = OV_ASSOCIATION_DEF_ov_inheritance.assoctype;
	assoc_inheritance.v_assocprops = OV_ASSOCIATION_DEF_ov_inheritance.assocprops;
	assoc_childrelationship.v_assoctype = OV_ASSOCIATION_DEF_ov_childrelationship.assoctype;
	assoc_childrelationship.v_assocprops = OV_ASSOCIATION_DEF_ov_childrelationship.assocprops;
	assoc_parentrelationship.v_assoctype = OV_ASSOCIATION_DEF_ov_parentrelationship.assoctype;
	assoc_parentrelationship.v_assocprops = OV_ASSOCIATION_DEF_ov_parentrelationship.assocprops;

	pobj = (OV_INSTPTR_ov_object) (((OV_HEAD*) (pdb->ov.v_linktable + distance + passoc_ov_containment->v_parentoffset))->pfirst);
	while (pobj) { 
		pobj = (OV_INSTPTR_ov_object) ((char*)pobj + distance);
		if (!strcmp(pobj->v_identifier+distance, "inheritance")) {
			assoc_inheritance.v_parentoffset = ((OV_INSTPTR_ov_association) pobj)->v_parentoffset;
			assoc_inheritance.v_childoffset = ((OV_INSTPTR_ov_association) pobj)->v_childoffset;
		}
		if (!strcmp(pobj->v_identifier+distance, "childrelationship")) {
			assoc_childrelationship.v_parentoffset = ((OV_INSTPTR_ov_association) pobj)->v_parentoffset;
			assoc_childrelationship.v_childoffset = ((OV_INSTPTR_ov_association) pobj)->v_childoffset;
		}
		if (!strcmp(pobj->v_identifier+distance, "parentrelationship")) {
			assoc_parentrelationship.v_parentoffset = ((OV_INSTPTR_ov_association) pobj)->v_parentoffset;
			assoc_parentrelationship.v_childoffset = ((OV_INSTPTR_ov_association) pobj)->v_childoffset;
		}
		pobj = (OV_INSTPTR_ov_object) (((OV_ANCHOR*) (pobj->v_linktable + distance + passoc_ov_containment->v_childoffset))->pnext);
	}

	passoc_ov_inheritance = &assoc_inheritance;
	passoc_ov_childrelationship = &assoc_childrelationship;
	passoc_ov_parentrelationship = &assoc_parentrelationship;
	/*
	*	make a copy of the original database
	*/
	pdbcopy = (OV_DATABASE_INFO*)Ov_HeapMalloc(pdb->size);
	if(!pdbcopy) {
		return OV_ERR_HEAPOUTOFMEMORY;
	}
	memcpy(pdbcopy, pdb, pdb->size);
	/*
	*	adjust database info pointers
	*/
	Ov_Adjust(OV_POINTER, pdb->baseaddr);
	Ov_Adjust(OV_BYTE*, pdb->pstart);
	Ov_Adjust(OV_BYTE*, pdb->pend);
	Ov_Adjust(OV_BYTE*, pdb->pcurr);
	Ov_Adjust(OV_STRING, pdb->serverpassword);
	/*
	*	adjust pointers of the memory pool
	*/
	Ov_Adjust(char*, pmpinfo->heapbase);
	Ov_Adjust(malloc_info*, pmpinfo->heapinfo);
	for(i=0; i<BLOCKLOG; i++) {
		for(phead=&pmpinfo->fraghead[i]; phead; phead=phead->next) {
			Ov_Adjust(struct list*, phead->next);
			Ov_Adjust(struct list*, phead->prev);
		}
	}
	Ov_Adjust(struct alignlist*, pmpinfo->aligned_blocks);
	for(pblock=pmpinfo->aligned_blocks; pblock; pblock=pblock->next) {
		Ov_Adjust(struct alignlist*, pblock->next);
		Ov_Adjust(__ptr_t, pblock->aligned);
		Ov_Adjust(__ptr_t, pblock->exact);
	}
	/*
	*	adjust pointers of the ACPLT/OV objects
	*/
	result = ov_object_move(Ov_PtrUpCast(ov_object, &pdb->root),
		Ov_PtrUpCast(ov_object, &pdbcopy->root), distance);
	if(Ov_Fail(result)) {
		/*
		*	copy back old database contents
		*/
		memcpy(pdb, pdbcopy, pdb->size);
	}
	/*
	*	free the database copy memory
	*/
	Ov_HeapFree(pdbcopy);
	/*
	*	we are through
	*/
	return result;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

