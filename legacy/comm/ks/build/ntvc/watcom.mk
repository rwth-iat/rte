### -*-makefile-*-
### 
### Compiler & platform specific part for
### Watcom C++ / Watcom make / Windows NT

### DON'T EXPECT ANYTHING FROM THIS MAKEFILE EXCEPT CREATING A BUNCH
### OF ERROR MESSAGES...

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

### Compiler
CXX = wcl386
CXX_FLAGS = -zq -zm -w4
# -od- -otexan

CXX_EXTRA_FLAGS = -I=.;$(KSDIR)\include;$(PLTDIR)\include;D:\devel\watcom\h;d:\devel\watcom\h\nt;$(ONCDIR)

.cpp: $(KSDIR)\src

.cpp: $(KSDIR)\examples

.cpp.obj:
	wcl386 $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -fo=$@ $< 

### Include generic part

!INCLUDE ..\generic.mk

### Include auto dependencies (built with gcc)

!INCLUDE ..\depend.nt

### How to build things
.obj.exe :
	wcl386 $< $(LIBKS) $(LIBPLT) $(LIBRPC) 

$(LIBKS) : $(LIBKS_OBJECTS)
	$(PLTDIR)\build\plt_ar wlib /p=64 $@ $?

tmanager.exe :	tmanager.obj tmanager1.obj $(LIBKS)
	wcl386 tmanager.obj tmanager1.obj $(LIBPLT) $(LIBRPC) 


clean : .SYMBOLIC
	del *.obj
	del *.exe

mrproper : clean
	del *.lib
	del *.err
	del *.sym
	del *.mbr


