#
# Makefile -- toplevel makefile which can be used on Un*x boxes to compile
#             the C++ communication library.
#
# Copyright (c) 1996, 1997
# Chair of Process Control Engineering,
# Aachen University of Technology.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must print or display the above
#    copyright notice either during startup or must have a means for
#    the user to view the copyright notice.
# 3. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 4. Neither the name of the Chair of Process Control Engineering nor the
#    name of the Aachen University of Technology may be used to endorse or
#    promote products derived from this software without specific prior
#    written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
# ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
# TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE CHAIR OF PROCESS CONTROL
# ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# $Header: /home/david/cvs/acplt/Makefile,v 1.7 1997-12-12 07:34:22 harald Exp $
#

UNIXES	= linux hpux solaris
SOME_OTHER_OS = nt

nothing:
	@echo "Usage:"
	@echo "  make clean       -- remove all objects, executables, libraries, etc."
	@echo "  make XXX.all     -- build libraries and examples"
	@echo "  make XXX.acplt   -- build the libraries only"
	@echo
	@echo "  Where \"XXX\" is either:"
	@echo "    \"hpux\", \"irix\", \"linux\", or \"solaris\"."
	@echo
	@echo "  If you're on NT, do instead a:"
	@echo "     make -f makefile.nt nt.all    (etc.)"

clean:
	@echo "-------------------------------------------------------------"
	@echo "Cleaning up......."
	@echo "-------------------------------------------------------------"
#	@for i in $(UNIXES) $(SOME_OTHER_OS) ;
	@for i in $(UNIXES) ; \
	    do \
                (cd plt/build/$$i; $(MAKE) clean;) ; \
		(cd ks/build/$$i; $(MAKE) clean;)  ; \
	    done


linux.acplt:
	@echo "-------------------------------------------------------------"
	@echo "Building the libPLT..."
	@echo "-------------------------------------------------------------"
	(cd plt/build/linux; $(MAKE) depend; $(MAKE);)
	@echo "-------------------------------------------------------------"
	@echo "Building the libKS..."
	@echo "-------------------------------------------------------------"
	(cd ks/build/linux; $(MAKE) depend; $(MAKE);)

linux.all:	linux.acplt
	@echo "-------------------------------------------------------------"
	@echo "Building examples..."
	@echo "-------------------------------------------------------------"
	(cd ks/build/linux; $(MAKE) examples;)


hpux.acplt:
	@echo "-------------------------------------------------------------"
	@echo "Building the libPLT..."
	@echo "-------------------------------------------------------------"
	(cd plt/build/hpux; $(MAKE) depend; $(MAKE);)
	@echo "-------------------------------------------------------------"
	@echo "Building the libKS..."
	@echo "-------------------------------------------------------------"
	(cd ks/build/hpux; $(MAKE) depend; $(MAKE);)

hpux.all:	hpux.acplt
	@echo "-------------------------------------------------------------"
	@echo "Building examples..."
	@echo "-------------------------------------------------------------"
	(cd ks/build/hpux; $(MAKE) examples;)


irix.acplt:
	@echo "-------------------------------------------------------------"
	@echo "Building the libPLT..."
	@echo "-------------------------------------------------------------"
	(cd plt/build/irix; $(MAKE) depend; $(MAKE);)
	@echo "-------------------------------------------------------------"
	@echo "Building the libKS..."
	@echo "-------------------------------------------------------------"
	(cd ks/build/irix; $(MAKE) depend; $(MAKE);)

irix.all:	irix.acplt
	@echo "-------------------------------------------------------------"
	@echo "Building examples..."
	@echo "-------------------------------------------------------------"
	(cd ks/build/irix; $(MAKE) examples;)


solaris.acplt:
	@echo "-------------------------------------------------------------"
	@echo "Building the libPLT..."
	@echo "-------------------------------------------------------------"
	(cd plt/build/solaris; $(MAKE) depend; $(MAKE);)
	@echo "-------------------------------------------------------------"
	@echo "Building the libKS..."
	@echo "-------------------------------------------------------------"
	(cd ks/build/solaris; $(MAKE) depend; $(MAKE);)

solaris.all:	solaris.acplt
	@echo "-------------------------------------------------------------"
	@echo "Building examples..."
	@echo "-------------------------------------------------------------"
	(cd ks/build/solaris; $(MAKE) examples;)


#
# Special Target -- for CVS EXPORT ACTION only -- which is used whenever
# we -- the PLT -- release a new version of the C++ Communication Library.
#
export:		clean
	@echo "-------------------------------------------------------------"
	@echo "WARNING: EXPORTING FROM linux BUILD DIRECTORIES!!!"
	@echo "-------------------------------------------------------------"
	(cd plt/build/linux; $(MAKE) depend;)
	(cd ks/build/linux; $(MAKE) depend;)
	(cd ks/build/linux; cp *_inst.h ../nt; cp *_inst.h ../solaris; cp *_inst.h ../hpux; cp *_inst.h ../irix; cp *_inst.h ../openvms;)

# End of Makefile
