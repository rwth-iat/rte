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
SRCDIR = $(KSDIR)\src\\
EXAMPLESSRCDIR = $(KSDIR)\examples\\

### Compiler
CXX = bcc32
CXX_FLAGS = -D_BORLANDC=1
#CXX_FLAGS = -DNDEBUG -w -v
#CXX_FLAGS =
CXX_EXTRA_FLAGS = -a8 -I. -I$(PLTDIR)\include -I$(KSDIR)\include -I$(ONCDIR) -I\interface -DPLT_SYSTEM_NT=1

RC = brc32

LIBKS_NT_OBJECTS = ntservice$(O)

.SUFFIXES:

.SUFFIXES:      .cpp .obj .lib .exe

all: $(LIBKS)

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

### Include generic part

!INCLUDE ..\generic.mk

### Include auto dependencies (built with gcc)

!INCLUDE ..\depend.nt

### How to build things
.obj.exe :
	@echo Linking $@
	$(CXX) @&&!
		$< $(LIBKS) $(LIBPLT) $(LIBRPC)
!

examples:       ntksmanager.exe tmanager.exe tserver.exe tsclient.exe ttree.exe

tmanager.exe: tmanager.obj tmanager1.obj $(LIBKS)
	@echo Linking $@
	$(CXX) @&&!
		tmanager.obj tmanager1.obj $(LIBKS) $(LIBPLT) $(LIBRPC)
!

tserver.exe: tserver.obj tserver1.obj $(LIBKS)
	@echo Linking $@
	$(CXX) @&&!
		tserver.obj tserver1.obj $(LIBKS) $(LIBPLT) $(LIBRPC)
!

tsclient.exe: tsclient.obj tsclient1.obj $(LIBKS)
	@echo Linking $@
	$(CXX) @&&!
		tsclient.obj tsclient1.obj $(LIBKS) $(LIBPLT) $(LIBRPC)
!

ntksmanager.res: $(EXAMPLESSRCDIR)ntksmanager.rc
	$(RC) -r -fontksmanager.res $(EXAMPLESSRCDIR)ntksmanager.rc

ntksmanager.exe: ntksmanager.obj ntksmanager_templates.obj $(LIBKS) ntksmanager.res
	@echo Linking $@
	$(CXX) @&&!
		-tWM ntksmanager.obj ntksmanager_templates.obj $(LIBKS) $(LIBPLT) $(LIBRPC)
!
	$(RC) ntksmanager.res ntksmanager.exe

ttree.exe: ttree.obj ttree1.obj $(LIBKS)
	@echo Linking $@
	$(CXX) @&&!
		ttree.obj ttree1.obj $(LIBKS) $(LIBPLT) $(LIBRPC)
!

### explicit dependencies not covered by platform-dependend depent mechanism
ntservice$(O): $(SRCDIR)ntservice.cpp $(KSDIR)\include\ks\ntservice.h
	@echo Compiling ntservice.cpp
	@$(CXX) @&&!
		-Jgx $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -o$@ $(SRCDIR)ntservice.cpp
!

$(LIBKS) : $(LIBKS_OBJECTS) $(LIBKS_NT_OBJECTS)
	$(PLTDIR)\build\nt\plt_ar tlib /P32 $@ $(LIBKS_OBJECTS1)
	$(PLTDIR)\build\nt\plt_ar tlib /P32 $@ $(LIBKS_OBJECTS2)
	$(PLTDIR)\build\nt\plt_ar tlib /P32 $@ $(LIBKS_OBJECTS3)
	$(PLTDIR)\build\nt\plt_ar tlib /P32 $@ $(LIBKS_OBJECTS4)
	$(PLTDIR)\build\nt\plt_ar tlib /P32 $@ $(LIBKS_OBJECTS5)
	$(PLTDIR)\build\nt\plt_ar tlib /P32 $@ $(LIBKS_NT_OBJECTS)

clean :
	del *.obj
	del *.exe

mrproper : clean
	del *.lib
	del *.err
	del *.sym
	del *.mbr


