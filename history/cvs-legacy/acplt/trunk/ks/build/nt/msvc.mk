### -*-makefile-*-
###
### Compiler & platform specific part for
### MS VC++ 4.2 / nmake / Windows NT

### Filename conventions
O=.obj
A=.lib
EXE=.exe
ONCDIR = \oncrpc
LIBRPC = \oncrpc\bin\oncrpcms.lib
PLTDIR = ..\..\..\plt
LIBPLT = $(PLTDIR)\build\nt\libplt.lib
KSDIR = ..\..
LIBKS = libks.lib
LIBKSSVR = libkssvr.lib
LIBKSCLN = libkscln.lib
SRCDIR = $(KSDIR)\src\\
EXAMPLESSRCDIR = $(KSDIR)\examples\\

### Compiler
CXX = cl
#CXX_FLAGS = -Zi -MTd
CXX_FLAGS = -DNDEBUG -MT
#CXX_FLAGS =

### LINKER
LD = link
# LD_FLAGS = /DEBUG
CXX_EXTRA_FLAGS = -I. -I$(PLTDIR)\include -I$(KSDIR)\include -I$(ONCDIR) \
	-GR -DPLT_SYSTEM_NT=1 -DPLT_DEBUG_NEW=0
CXX_LIBS = $(LIBKS) $(LIBPLT) $(LIBRPC) wsock32.lib advapi32.lib

RC = echo

LIBKS_NT_OBJECTS = ntservice$(O)

.SUFFIXES:

.SUFFIXES:      .cpp .obj .lib .exe .i

all: $(LIBKS) $(LIBKSSVR) $(LIBKSCLN)

examples:       ntksmanager.exe tmanager.exe tserver.exe tsclient.exe ttree.exe

{$(SRCDIR)}.cpp{}.obj:
	$(CXX)	$(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c  $<

{$(SRCDIR)}.cpp{}.i:
	$(CXX)	$(CXX_EXTRA_FLAGS) $(CXX_FLAGS) /P $<


{$(EXAMPLESSRCDIR)}.cpp{}.obj:
	$(CXX) $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c  $<


### Include generic part

!INCLUDE ..\generic.mk

### Include auto dependencies (built with gcc)

!INCLUDE ..\depend.nt

###

### explicit dependencies not covered by platform-dependend depent mechanism
ntservice$(O): $(SRCDIR)ntservice.cpp $(KSDIR)\include\ks\ntservice.h
	$(CXX)	$(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c $(SRCDIR)ntservice.cpp

ntksmanager.res: $(EXAMPLESSRCDIR)ntksmanager.rc
	echo " "> $@

ntksmanager.exe: ntksmanager.obj ntksmanager_templates.obj $(LIBKS) ntksmanager.res
	@echo Linking $@
	$(LD) $(LD_FLAGS) /NODEFAULTLIB:libc \
		 ntksmanager.obj ntksmanager_templates.obj \
		$(LIBKSSVR) $(CXX_LIBS)
	$(RC) ntksmanager.res ntksmanager.exe


$(LIBKS) : $(LIBKS_OBJECTS)
	lib /OUT:$@ $(LIBKS_OBJECTS)

$(LIBKSSVR) : $(LIBKSSVR_OBJECTS) ntservice$(O)
	lib /OUT:$@ $(LIBKSSVR_OBJECTS) ntservice$(O)

$(LIBKSCLN) : $(LIBKSCLN_OBJECTS)
	lib /OUT:$@ $(LIBKSCLN_OBJECTS)

# $(LIBKS_NT_OBJECTS)

tmanager$(EXE): tmanager$(O) tmanager1$(O) $(LIBKSSVR) $(LIBKS)
	$(LD) $(LD_FLAGS) /NODEFAULTLIB:libc \
		tmanager$(O) tmanager1$(O) \
		$(LIBKSSVR) $(CXX_LIBS)

tserver$(EXE): tserver$(O) tserver1$(O) $(LIBKSSVR) $(LIBKS)
	$(LD) $(LD_FLAGS) /NODEFAULTLIB:libc \
		tserver$(O) tserver1$(O) \
		$(LIBKSSVR) $(CXX_LIBS)

tsclient$(EXE): tsclient$(O) tsclient1$(O) svrrpcctx$(O) $(LIBKS)
	$(LD) $(LD_FLAGS) /NODEFAULTLIB:libc \
		tsclient$(O) tsclient1$(O) \
		svrrpcctx$(O) $(CXX_LIBS)

ttree$(EXE): ttree$(O) ttree1$(O) $(LIBKS) $(LIBKSCLN)
	$(LD) $(LD_FLAGS) /NODEFAULTLIB:libc \
		ttree$(O) ttree1$(O) \
		$(LIBKSCLN) $(CXX_LIBS)

tshell$(EXE): tshell$(O) $(LIBKS) $(LIBKSCLN)
	$(LD) $(LD_FLAGS) /NODEFAULTLIB:libc \
		tshell$(O) \
		$(LIBKSCLN) $(CXX_LIBS)

pmobile$(EXE): pmobile$(O) pmobile_code$(O) $(LIBKS) $(LIBKSCLN)
	$(LD) $(LD_FLAGS) /NODEFAULTLIB:libc \
		pmobile$(O) pmobile_code$(O) \
		$(LIBKSCLN) $(CXX_LIBS)

clean :
	del *.obj
	del *.exe
	del *.pdb
	del *.ilk

mrproper : clean
	del *.lib
	del *.err
	del *.sym
	del *.mbr
