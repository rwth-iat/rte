### -*-makefile-*-
###
### Compiler & platform specific part for
### Borland C++ / make / Windows NT

### Filename conventions
O=.obj
A=.lib
EXE=.exe
ONCDIR = \oncrpc
LIBRPC = \oncrpc\bin\oncrpc.lib
PLTDIR = ..\..\..\plt
LIBPLT = $(PLTDIR)\build\nt\libplt.lib
KSDIR = ..\..
LIBKS = libks.lib
SRCDIR = $(KSDIR)\src\\
EXAMPLESSRCDIR = $(KSDIR)\examples\\

### Compiler
CXX = cl
CXX_FLAGS = -Zi -MTd
#CXX_FLAGS = -DNDEBUG -w -v
#CXX_FLAGS =

### LINKER
LD = link
LD_FLAGS = /DEBUG /NODEFAULTLIB:libc

CXX_EXTRA_FLAGS = -GR -I. -I$(PLTDIR)\include -I$(KSDIR)\include -I$(ONCDIR) -DPLT_SYSTEM_NT=1

RC = echo

LIBKS_NT_OBJECTS = ntservice$(O)

.SUFFIXES:

.SUFFIXES:      .cpp .obj .lib .exe .i

all: $(LIBKS)

examples:       ntksmanager.exe tmanager.exe tserver.exe tsclient.exe ttree.exe

{$(SRCDIR)}.cpp{}.obj:
	@echo Compiling $<
	@$(CXX)	$(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c  $<

{$(SRCDIR)}.cpp{}.i:
	@echo Compiling $<
	@$(CXX)	$(CXX_EXTRA_FLAGS) $(CXX_FLAGS) /P $<


{$(EXAMPLESSRCDIR)}.cpp{}.obj:
	@echo Compiling $<
	@$(CXX) $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c  $<


### Include generic part

!INCLUDE ..\generic.mk

### Include auto dependencies (built with gcc)

!INCLUDE ..\depend.nt

### How to build things
.obj.exe :
	@echo Linking $@
	$(LD) $(LD_FLAGS) $< \
		$(LIBKS) $(LIBPLT) $(LIBRPC) \
		wsock32.lib advapi32.lib


### explicit dependencies not covered by platform-dependend depent mechanism
ntservice$(O): $(SRCDIR)ntservice.cpp $(KSDIR)\include\ks\ntservice.h
	$(CXX)	$(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c $(SRCDIR)ntservice.cpp

ntksmanager.res: $(EXAMPLESSRCDIR)ntksmanager.rc
	$(RC) -r -fontksmanager.res $(EXAMPLESSRCDIR)ntksmanager.rc

ntksmanager.exe: ntksmanager.obj ntksmanager_templates.obj $(LIBKS) ntksmanager.res
	@echo Linking $@
	$(LD) $(LD_FLAGS) ntksmanager.obj ntksmanager_templates.obj \
		$(LIBKS) $(LIBPLT) $(LIBRPC) \
		wsock32.lib advapi32.lib
	$(RC) ntksmanager.res ntksmanager.exe


$(LIBKS) : $(LIBKS_OBJECTS) ntservice$(O)
	lib /OUT:$@ $(LIBKS_OBJECTS) ntservice$(O)

# $(LIBKS_NT_OBJECTS)

tmanager$(EXE): tmanager$(O) tmanager1$(O)
	$(LD) $(LD_FLAGS) tmanager$(O) tmanager1$(O) \
		$(LIBKS) $(LIBPLT) $(LIBRPC) \
		wsock32.lib advapi32.lib

tserver$(EXE): tserver$(O) tserver1$(O)
	$(LD) $(LD_FLAGS) tserver$(O) tserver1$(O) \
		$(LIBKS) $(LIBPLT) $(LIBRPC) \
		wsock32.lib advapi32.lib

tsclient$(EXE): tsclient$(O) tsclient1$(O)
	$(LD) $(LD_FLAGS) tsclient$(O) tsclient1$(O) \
		$(LIBKS) $(LIBPLT) $(LIBRPC) \
		wsock32.lib advapi32.lib

tmanager$(EXE): tmanager$(O) tmanager1$(O)
	$(LD) $(LD_FLAGS) tmanager$(O) tmanager1$(O) \
		$(LIBKS) $(LIBPLT) $(LIBRPC) \
		wsock32.lib advapi32.lib

ttree$(EXE): ttree$(O) ttree1$(O)
	$(LD) $(LD_FLAGS) ttree$(O) ttree1$(O) \
		$(LIBKS) $(LIBPLT) $(LIBRPC) \
		wsock32.lib advapi32.lib

clean :
	del *.obj
	del *.exe

mrproper : clean
	del *.lib
	del *.err
	del *.sym
	del *.mbr


