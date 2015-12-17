### -*-makefile-*-
###
### Compiler & platform specific part for
### Borland C++ / make / Windows NT

### Filename conventions
O=.obj
A=.lib
EXE=.exe
ONCDIR = ..\..\..\..\oncrpc
LIBRPC = $(ONCDIR)\bin\oncrpc.lib
PLTDIR = ..\..\..\plt
LIBPLT = $(PLTDIR)\build\nt\libplt.lib
KSDIR = ..\..
LIBKS = libks.lib
LIBKSSVR = libkssvr.lib
LIBKSCLN = libkscln.lib

SRCDIR = $(KSDIR)\src\\
EXAMPLESSRCDIR = $(KSDIR)\examples\\


### Compiler
CXX = bcc32 -v-
# The following set of flags compiles with optimization and no PLT debugging
# helpers (no alloc tracker, no debugging messages)
#CXX_FLAGS = -D_BORLANDC=1 -w -DNDEBUG

# Enable next set of flags when using the new server-side dynamic XDR
# streams, which buffer client i/o. (Prevents servers from blocking in i/o).
CXX_FLAGS = -D_BORLANDC=1 -w -DNDEBUG -DPLT_USE_BUFFERED_STREAMS=1

#CXX_FLAGS = -D_BORLANDC=1 -w -DPLT_USE_BUFFERED_STREAMS=1 -v

CXX_EXTRA_FLAGS = -a8 -I. -I$(EXAMPLESSRCDIR) -I$(PLTDIR)\include -I$(KSDIR)\include -I$(ONCDIR) -DPLT_SYSTEM_NT=1
# -DFD_SETSIZE=128

RC = brc32

LIBKS_NT_OBJECTS = ntservice$(O) w95service$(O)

.SUFFIXES:

.SUFFIXES:      .cpp .obj .lib .exe

all: $(LIBKS) $(LIBKSSVR) $(LIBKSCLN) examples

# FIXME -Jgx for 5.0, -Jg for 5.3
{$(SRCDIR)}.cpp{}.obj:
	@echo Compiling $<
	@$(CXX) @&&!
		-Jgx $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -o$@ $<
!

histparams.obj: $(SRCDIR)histparams.cpp
	@echo Compiling histparams.cpp with -Vs
	@$(CXX) @&&!
		-Jgx -Vs $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c $(SRCDIR)histparams.cpp
!

history.obj: $(SRCDIR)history.cpp
	@echo Compiling history.cpp with -Vs
	@$(CXX) @&&!
		-Jgx -Vs $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c $(SRCDIR)history.cpp
!

histdomain.obj: $(SRCDIR)histdomain.cpp
	@echo Compiling histdomain.cpp with -Vs
	@$(CXX) @&&!
		-Jgx -Vs $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c $(SRCDIR)histdomain.cpp
!

# FIXME -Jgx
{$(EXAMPLESSRCDIR)}.cpp{}.obj:
	@echo Compiling $<
	@$(CXX) @&&!
		-Jg $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -o$@ $<
!

w95ksmanager.obj: $(EXAMPLESSRCDIR)ntksmanager.cpp
	@echo Compiling ntksmanager.cpp/W95
	@$(CXX) @&&!
		-DPLT_W95SERVICE
		-Jgx $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -o$@
		$(EXAMPLESSRCDIR)ntksmanager.cpp
!

################################################################################
# template instantiation files
# the following targets are compiled with template instantiation activated(-Jgd)
################################################################################

templates.obj:  $(SRCDIR)templates.cpp
	@echo Compiling $<
	@$(CXX) @&&!
		-Jgd $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -o$@ $(SRCDIR)templates.cpp
!

value.obj:  $(SRCDIR)value.cpp
	@echo Compiling $<
	@$(CXX) @&&!
		-Jgd $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -o$@ $(SRCDIR)value.cpp
!

tmanager.obj:   $(EXAMPLESSRCDIR)tmanager.cpp
	@echo Compiling $<
	@$(CXX) @&&!
		-Jgd $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -o$@ $(EXAMPLESSRCDIR)tmanager.cpp
!

tserver.obj:    $(EXAMPLESSRCDIR)tserver.cpp
	@echo Compiling $<
	@$(CXX) @&&!
		-Jgd $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -o$@ $(EXAMPLESSRCDIR)tserver.cpp
!

tsclient.obj:   $(EXAMPLESSRCDIR)tsclient.cpp
	@echo Compiling $<
	@$(CXX) @&&!
		-Jgd $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -o$@ $(EXAMPLESSRCDIR)tsclient.cpp
!

ntksmanager_templates.obj:      $(EXAMPLESSRCDIR)ntksmanager_templates.cpp
	@echo Compiling $<
	@$(CXX) @&&!
		-Jgd $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -o$@ $(EXAMPLESSRCDIR)ntksmanager_templates.cpp
!

ttree.obj:    $(EXAMPLESSRCDIR)ttree.cpp
	@echo Compiling $<
	@$(CXX) @&&!
		-Jgd $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -o$@ $(EXAMPLESSRCDIR)ttree.cpp
!

tclient.obj:    $(EXAMPLESSRCDIR)tclient.cpp
	@echo Compiling $<
	@$(CXX) @&&!
		-Jgd $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -o$@ $(EXAMPLESSRCDIR)tclient.cpp
!

tshell.obj:    $(EXAMPLESSRCDIR)tshell.cpp
	@echo Compiling $<
	@$(CXX) @&&!
		-Jgd $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -o$@ $(EXAMPLESSRCDIR)tshell.cpp
!


pmobile.obj:    $(EXAMPLESSRCDIR)pmobile.cpp
	@echo Compiling $<
	@$(CXX) @&&!
		-Jgd $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -o$@ $(EXAMPLESSRCDIR)pmobile.cpp
!

###############################################################################
# end of template instantiation files
###############################################################################

examples:   ntksmanager.exe tmanager.exe
	tdstrp32 ntksmanager.exe
	tdstrp32 tmanager.exe

#     ntksmanager.exe w95ksmanager.exe tmanager.exe tserver.exe ttree.exe
!

### Include generic part

!INCLUDE ..\generic.mk

### Include auto dependencies (built with gcc)

!INCLUDE ..\depend.nt

### How to build things
.obj.exe :
	@echo Linking $@
	$(CXX) @&&!
		$< $(LIBKS) $(LIBKSCLN) $(LIBKSSVR) $(LIBPLT) $(LIBRPC)
!

#
# tslcient.exe and tshell.exe aren't supported.
#examples:       tclient.exe ntksmanager.exe tmanager.exe tserver.exe tsclient.exe ttree.exe

tmanager.res: $(EXAMPLESSRCDIR)tmanager.rc
	$(RC) -r -fotmanager.res $(EXAMPLESSRCDIR)tmanager.rc

tmanager.exe: tmanager.obj tmanager1.obj $(LIBKSSVR) $(LIBKS) tmanager.res
	@echo Linking $@
	$(CXX) @&&!
		tmanager.obj tmanager1.obj $(LIBKSSVR) $(LIBKS) $(LIBPLT) $(LIBRPC)
!
	$(RC) -fetmanager.exe tmanager.res

tserver.exe: tserver.obj tserver1.obj $(LIBKSSVR) $(LIBKS)
	@echo Linking $@
	$(CXX) @&&!
		-v tserver.obj tserver1.obj ext_sp.obj $(LIBKSSVR) $(LIBKS) $(LIBPLT) $(LIBRPC)
!

tsclient.exe: tsclient.obj tsclient1.obj $(LIBKS)
	@echo Linking $@
	$(CXX) @&&!
		tsclient.obj tsclient1.obj $(LIBKS) $(LIBPLT) $(LIBRPC)
!

pmobile.exe: pmobile.obj pmobile_code.obj $(LIBKS) $(LIBKSCLN)
	@echo Linking $@
	$(CXX) @&&!
		pmobile.obj pmobile_code.obj $(LIBKSCLN) $(LIBKS) $(LIBPLT) $(LIBRPC)
!

ntksmanager.res: $(EXAMPLESSRCDIR)ntksmanager.rc
	$(RC) -r -fontksmanager.res $(EXAMPLESSRCDIR)ntksmanager.rc

ntksmanager.exe: ntksmanager.obj ntksmanager_templates.obj $(LIBKSSVR) $(LIBKS) ntksmanager.res
	@echo Linking $@
	$(CXX) @&&!
		-tWM ntksmanager.obj ntksmanager_templates.obj $(LIBKSSVR) $(LIBKS) $(LIBPLT) $(LIBRPC)
!
	$(RC) -fentksmanager.exe ntksmanager.res

w95ksmanager.res: $(EXAMPLESSRCDIR)ntksmanager.rc
	$(RC) -DPLT_W95SERVICE -r -fow95ksmanager.res $(EXAMPLESSRCDIR)ntksmanager.rc

w95ksmanager.exe: w95ksmanager.obj ntksmanager_templates.obj $(LIBKSSVR) $(LIBKS) w95ksmanager.res
	@echo Linking $@
	$(CXX) @&&!
		-laa -tWM w95ksmanager.obj ntksmanager_templates.obj $(LIBKSSVR) $(LIBKS) $(LIBPLT) $(LIBRPC)
!
	$(RC) -few95ksmanager.exe w95ksmanager.res

ttree.exe: ttree.obj ttree1.obj $(LIBKSCLN) $(LIBKS)
	@echo Linking $@
	$(CXX) @&&!
		ttree.obj ttree1.obj $(LIBKSCLN) $(LIBKS) $(LIBPLT) $(LIBRPC)
!

tshell.exe: tshell.obj $(LIBKSCLN) $(LIBKS)
	@echo Linking $@
	$(CXX) @&&!
		tshell.obj $(LIBKSCLN) $(LIBKS) $(LIBPLT) $(LIBRPC)
!

tclient.exe: tclient.obj tclient1.obj $(LIBKSCLN) $(LIBKS) 
	@echo Linking $@
	$(CXX) @&&!
		tclient.obj tclient1.obj $(LIBKSCLN) $(LIBKS) $(LIBPLT) $(LIBRPC)
!

### explicit dependencies not covered by platform-dependend depent mechanism
ntservice$(O): $(SRCDIR)ntservice.cpp $(KSDIR)\include\ks\ntservice.h
	@echo Compiling ntservice.cpp
	@$(CXX) @&&!
		-Jgx $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -o$@ $(SRCDIR)ntservice.cpp
!

w95service$(O): $(SRCDIR)w95service.cpp $(KSDIR)\include\ks\w95service.h
	@echo Compiling w95service.cpp
	@$(CXX) @&&!
		-Jgx $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -o$@ $(SRCDIR)w95service.cpp
!

$(LIBKS) : $(LIBKS_OBJECTS)
	$(PLTDIR)\build\nt\plt_ar tlib /P128 $@ $(LIBKS_OBJECTS1)
	$(PLTDIR)\build\nt\plt_ar tlib /P128 $@ $(LIBKS_OBJECTS2)
	$(PLTDIR)\build\nt\plt_ar tlib /P128 $@ $(LIBKS_OBJECTS3)
	$(PLTDIR)\build\nt\plt_ar tlib /P128 $@ $(LIBKS_OBJECTS4)
!

$(LIBKSSVR) : $(LIBKSSVR_OBJECTS) $(LIBKS_NT_OBJECTS)
	$(PLTDIR)\build\nt\plt_ar tlib /P256 $@ $(LIBKSSVR_OBJECTS1)
	$(PLTDIR)\build\nt\plt_ar tlib /P256 $@ $(LIBKSSVR_OBJECTS2)
	$(PLTDIR)\build\nt\plt_ar tlib /P256 $@ $(LIBKSSVR_OBJECTS3)
	$(PLTDIR)\build\nt\plt_ar tlib /P256 $@ $(LIBKSSVR_OBJECTS4)
	$(PLTDIR)\build\nt\plt_ar tlib /P256 $@ $(LIBKS_NT_OBJECTS)
!

$(LIBKSCLN) : $(LIBKSCLN_OBJECTS)
	$(PLTDIR)\build\nt\plt_ar tlib /P128 $@ $(LIBKSCLN_OBJECTS1)
	$(PLTDIR)\build\nt\plt_ar tlib /P128 $@ $(LIBKSCLN_OBJECTS2)
!



clean :
	del *.obj
	del *.exe
	del *.map
	del *.bak

mrproper : clean
	del *.lib
	del *.err
	del *.sym
	del *.mbr
