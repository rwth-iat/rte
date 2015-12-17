### -*-makefile-*-
### 
### Compiler & platform specific part for
### Microsoft VisualC++ / nmake / Windows NT
### $id
###

### Filename conventions
O=.obj
A=.lib
EXE=.exe

PLTDIR = ..\..
SRCDIR = $(PLTDIR)\src\\
TESTDIR = $(PLTDIR)\tests\\

### Compiler
CXX = cl /nologo
#CXX_FLAGS = /Zi /MTd
CXX_FLAGS = -DNDEBUG -MT
CXX_EXTRA_FLAGS = -I. -I$(PLTDIR)\include \
	-GR -DPLT_SYSTEM_NT=1  -DPLT_DEBUG_NEW=0

.SUFFIXES:	
.SUFFIXES:	.cpp .obj .lib .exe

{$(SRCDIR)}.cpp{}.obj:
	$(CXX) $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -o$@ $< 

{$(TESTDIR)}.cpp{}.obj:
	$(CXX) $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -o$@ $< 

### Include generic part
!INCLUDE ..\generic.mk

### Include auto dependencies (built with gcc)
!INCLUDE ..\depend.nt

### How to build things
.obj.exe :
	$(CXX) $< $(LIBPLT)

$(LIBPLT) : $(LIBPLT_OBJECTS)
	lib /OUT:$@ $(LIBPLT_OBJECTS)

clean :
	del *.obj
	del *.exe



mrproper : clean
	del *.lib
	del *.err
	del *.sym
	del *.mbr
