### -*-makefile-*-
###
### Compiler & platform specific part for
### MS VC++ / nmake / Windows NT

### Filename conventions
O=.obj
A=.lib
EXE=.exe
ONCDIR = ..\..\..\..\oncrpc
LIBRPC = $(ONCDIR)\bin\oncrpcms.lib
PLTDIR = ..\..\..\plt
LIBPLT = $(PLTDIR)\build\ntvc\libplt.lib
KSDIR = ..\..
LIBKS = libks.lib
LIBKSSVR = libkssvr.lib
LIBKSCLN = libkscln.lib
SRCDIR = $(KSDIR)\src\\
EXAMPLESSRCDIR = $(KSDIR)\examples\\

### Compiler
CXX = cl /nologo
#CXX_FLAGS = -Zi -MTd
CXX_FLAGS = -DNDEBUG -MT
#CXX_FLAGS =

### LINKER
LD = link /nologo /debug
# LD_FLAGS = /DEBUG
CXX_EXTRA_FLAGS = -I. -I$(PLTDIR)\include -I$(KSDIR)\include -I$(ONCDIR) \
	-GR -GX -DPLT_SYSTEM_NT=1 -DPLT_USE_BUFFERED_STREAMS=1 -DPLT_DEBUG_NEW=0 -DFD_SETSIZE=128
CXX_LIBS = $(LIBKS) $(LIBPLT) $(LIBRPC) wsock32.lib advapi32.lib user32.lib

RC = rc
RC_OPTS = /dPLT_RCMSVC=1

LIBKS_NT_OBJECTS = ntservice$(O) w95service$(O)

.SUFFIXES:
.SUFFIXES:      .cpp .obj .lib .exe .i

all: $(LIBKS) $(LIBKSSVR) $(LIBKSCLN)

# the tsclient.exe is not supported
examples:       ntksmanager.exe tmanager.exe tserver.exe

# tsclient.exe ttree.exe

{$(SRCDIR)}.cpp{}.obj:
	$(CXX)  $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c  $<

{$(SRCDIR)}.cpp{}.i:
	$(CXX)  $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) /P $<

{$(EXAMPLESSRCDIR)}.cpp{}.obj:
	$(CXX) $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c  $<

### Include generic part
!INCLUDE ..\generic.mk

### Include auto dependencies (built with gcc)
!INCLUDE ..\depend.nt

###

w95service$(O): $(KSDIR)\include\ks\w95service.h $(KSDIR)\src\w95service.cpp

ntservice$(O): $(KSDIR)\include\ks\ntservice.h $(KSDIR)\src\ntservice.cpp

### explicit dependencies not covered by platform-dependend depent mechanism

ntservice$(O): $(SRCDIR)ntservice.cpp $(KSDIR)\include\ks\ntservice.h
	$(CXX)  $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c $(SRCDIR)ntservice.cpp

ntksmanager.res: $(EXAMPLESSRCDIR)ntksmanager.rc
	$(RC) $(RC_OPTS) /fo ntksmanager.res $(EXAMPLESSRCDIR)ntksmanager.rc

ntksmanager.exe: ntksmanager.obj ntksmanager_templates.obj $(LIBKS) ntksmanager.res
	@echo Linking $@
	$(LD) $(LD_FLAGS) /NODEFAULTLIB:libc \
		 ntksmanager.obj ntksmanager_templates.obj ntksmanager.res \
		$(LIBKSSVR) $(CXX_LIBS)

w95ksmanager$(O): $(EXAMPLESSRCDIR)ntksmanager.cpp $(KSDIR)\include\ks\ntservice.h
	$(CXX) -DPLT_W95SERVICE $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -Fo$@ $(EXAMPLESSRCDIR)ntksmanager.cpp

w95ksmanager.res: $(EXAMPLESSRCDIR)ntksmanager.rc
	$(RC) $(RC_OPTS) /dPLT_W95SERVICE /fo w95ksmanager.res $(EXAMPLESSRCDIR)ntksmanager.rc

w95ksmanager.exe: w95ksmanager.obj ntksmanager_templates.obj $(LIBKS) w95ksmanager.res
	@echo Linking $@
	$(LD) $(LD_FLAGS) /NODEFAULTLIB:libc \
		/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup \
		 w95ksmanager.obj ntksmanager_templates.obj w95ksmanager.res \
		$(LIBKSSVR) $(CXX_LIBS)

$(LIBKS) : $(LIBKS_OBJECTS)
	lib /OUT:$@ $(LIBKS_OBJECTS)

$(LIBKSSVR) : $(LIBKSSVR_OBJECTS) $(LIBKS_NT_OBJECTS)
	lib /OUT:$@ $(LIBKSSVR_OBJECTS) $(LIBKS_NT_OBJECTS)

$(LIBKSCLN) : $(LIBKSCLN_OBJECTS)
	lib /OUT:$@ $(LIBKSCLN_OBJECTS)

# $(LIBKS_NT_OBJECTS)

pmobile$(EXE): pmobile$(O) pmobile_code$(O) $(LIBKS) $(LIBKSCLN)
	$(LD) $(LD_FLAGS) /NODEFAULTLIB:libc \
		pmobile$(O) pmobile_code$(O) \
		$(LIBKSCLN) $(CXX_LIBS)

tclient$(EXE): tclient$(O) tclient1$(O) $(LIBKS) $(LIBKSCLN)
	$(LD) $(LD_FLAGS) /NODEFAULTLIB:libc \
		tclient$(O) tclient1$(O) \
		$(LIBKSCLN) $(CXX_LIBS)

tmanager$(EXE): tmanager$(O) tmanager1$(O) $(LIBKSSVR) $(LIBKS)
	$(LD) $(LD_FLAGS) /NODEFAULTLIB:libc \
		tmanager$(O) tmanager1$(O) \
		$(LIBKSSVR) $(CXX_LIBS)

tserver$(EXE): tserver$(O) tserver1$(O) $(LIBKSSVR) $(LIBKS)
	$(LD) $(LD_FLAGS) /NODEFAULTLIB:libc \
		tserver$(O) tserver1$(O) ext_sp$(O) \
		$(LIBKSSVR) $(CXX_LIBS)

tsclient$(EXE): tsclient$(O) tsclient1$(O) svrrpcctx$(O) $(LIBKSSVR) $(LIBKS)
	$(LD) $(LD_FLAGS) /NODEFAULTLIB:libc \
		tsclient$(O) tsclient1$(O) \
		svrrpcctx$(O) \
		$(LIBKSSVR) $(CXX_LIBS)

tshell$(EXE): tshell$(O) $(LIBKS) $(LIBKSCLN)
	$(LD) $(LD_FLAGS) /NODEFAULTLIB:libc \
		tshell$(O) \
		$(LIBKSCLN) $(CXX_LIBS)

ttree$(EXE): ttree$(O) ttree1$(O) $(LIBKS) $(LIBKSCLN)
	$(LD) $(LD_FLAGS) /NODEFAULTLIB:libc \
		ttree$(O) ttree1$(O) \
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

