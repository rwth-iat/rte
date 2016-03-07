### -*-makefile-*-
### 
### Compiler & platform specific part for
### Watcom C++ / Watcom make / Windows NT

### Filename conventions
O=.obj
A=.lib
EXE=.exe
PLTDIR = ..\..
SRCDIR = $(PLTDIR)\src\\

### Compiler
CXX = wcl386
CXX_FLAGS = -zq -zm -w4
# -od- -otexan

CXX_EXTRA_FLAGS = -I=.;$(PLTDIR)\include;D:\devel\watcom\h;d:\devel\watcom\h\nt;E:\devel\oncrpc

.cpp: $(PLTDIR)\src

.cpp.obj:
	wcl386 $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -fo=$@ $< 

### Include generic part

!INCLUDE ..\generic.mk

### Include auto dependencies (built with gcc)

!INCLUDE ..\depend.nt

### How to build things
.obj.exe :
	wcl386 $< $(LIBPLT)

$(LIBPLT) : $(LIBPLT_OBJECTS)
	$(PLTDIR)\build\plt_ar wlib $@ $?

clean : .SYMBOLIC
	del *.obj
	del *.exe

mrproper : clean
	del *.lib
	del *.err
	del *.sym
	del *.mbr


