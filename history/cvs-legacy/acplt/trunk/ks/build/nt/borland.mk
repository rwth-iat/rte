### -*-makefile-*-
###
### Compiler & platform specific part for
### Borland C++ / make / Windows NT

### Filename conventions
O=.obj
A=.lib
EXE=.exe
ONCDIR = \oncrpc
LIBRPC = \oncrpc\librpc\oncrpc.lib
PLTDIR = ..\..\..\plt
LIBPLT = $(PLTDIR)\build\nt\libplt.lib
KSDIR = ..\..
LIBKS = libks.lib
LIBKSSVR = libkssvr.lib
LIBKSCLN = libkscln.lib

SRCDIR = $(KSDIR)\src\\
EXAMPLESSRCDIR = $(KSDIR)\examples\\

### Compiler
CXX = bcc32
CXX_FLAGS = -D_BORLANDC -w
#CXX_FLAGS = -D_BORLANDC=1 -DNDEBUG -w
#CXX_FLAGS =
CXX_EXTRA_FLAGS = -a8 -I. -I$(EXAMPLESSRCDIR) -I$(PLTDIR)\include -I$(KSDIR)\include -I$(ONCDIR) -DPLT_SYSTEM_NT=1 -DFD_SETSIZE=128

RC = brc32

LIBKS_NT_OBJECTS = ntservice$(O)

.SUFFIXES:

.SUFFIXES:      .cpp .obj .lib .exe

all: $(LIBKS) $(LIBKSSVR) $(LIBKSCLN)

{$(SRCDIR)}.cpp{}.obj:
	@echo Compiling $<
	@$(CXX) @&&!
		-Jgx $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -o$@ $<
!

{$(EXAMPLESSRCDIR)}.cpp{}.obj:
	@echo Compiling $<
	@$(CXX) @&&!
		-Jgx $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -o$@ $<
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

examples:       tclient.exe ntksmanager.exe tmanager.exe tserver.exe tsclient.exe ttree.exe

tmanager.exe: tmanager.obj tmanager1.obj $(LIBKSSVR) $(LIBKS)
	@echo Linking $@
	$(CXX) @&&!
		tmanager.obj tmanager1.obj $(LIBKSSVR) $(LIBKS) $(LIBPLT) $(LIBRPC)
!

tserver.exe: tserver.obj tserver1.obj $(LIBKSSVR) $(LIBKS)
	@echo Linking $@
	$(CXX) @&&!
		tserver.obj tserver1.obj $(LIBKSSVR) $(LIBKS) $(LIBPLT) $(LIBRPC)
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
	$(RC) ntksmanager.res ntksmanager.exe

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

$(LIBKS) : $(LIBKS_OBJECTS)
	$(PLTDIR)\build\nt\plt_ar tlib /P64 $@ $(LIBKS_OBJECTS1)
	$(PLTDIR)\build\nt\plt_ar tlib /P64 $@ $(LIBKS_OBJECTS2)


$(LIBKSSVR) : $(LIBKSSVR_OBJECTS) $(LIBKS_NT_OBJECTS)
	$(PLTDIR)\build\nt\plt_ar tlib /P64 $@ $(LIBKSSVR_OBJECTS1)
	$(PLTDIR)\build\nt\plt_ar tlib /P64 $@ $(LIBKSSVR_OBJECTS2)
	$(PLTDIR)\build\nt\plt_ar tlib /P64 $@ $(LIBKS_NT_OBJECTS)

$(LIBKSCLN) : $(LIBKSCLN_OBJECTS)
	$(PLTDIR)\build\nt\plt_ar tlib /P64 $@ $(LIBKSCLN_OBJECTS1)
	$(PLTDIR)\build\nt\plt_ar tlib /P64 $@ $(LIBKSCLN_OBJECTS2)



clean :
	del *.obj
	del *.exe

mrproper : clean
	del *.lib
	del *.err
	del *.sym
	del *.mbr





