### -*-makefile-*-
### 
### Compiler & platform specific part for
### Watcom C++ / Watcom make / Windows NT

### Filename conventions
O=.obj
A=.lib
EXE=.exe
SRCDIR = ..\..\src\\
TESTSRCDIR = ..\..\tests\\

### Compiler
CXX = wcl386
CXX_FLAGS = -DNDEBUG -zq -zm -w4 -od- -otexan
CXX_EXTRA_FLAGS = -I=.;..\..\include;E:\devel\oncrpc

.cpp: ..\..\src

.cpp: ..\..\tests

.cpp.obj:
	wcl386 $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -fo=$@ $< 

### Include generic part

!INCLUDE ../generic.mk

### Include auto dependencies (built with gcc)

!INCLUDE ../depend.mk

### How to build things
.obj.exe :
	wcl386 $< $(LIBPLT)

$(LIBPLT) : $(LIBPLT_OBJECTS)
	..\w_ar r $@ $?

clean : .SYMBOLIC
	del *.obj
	del *.exe

mrproper : clean
	del *.lib
	del *.err
	del *.sym
	del *.mbr


