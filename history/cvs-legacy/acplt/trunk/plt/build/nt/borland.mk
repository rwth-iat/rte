### -*-makefile-*-
### 
### Compiler & platform specific part for
### Borland C++ / make / Windows NT

### Filename conventions
O=.obj
A=.lib
EXE=.exe
PLTDIR = ..\..
SRCDIR = $(PLTDIR)\src\\
TESTSRCDIR = $(PLTDIR)\tests\\

### Compiler
CXX = bcc32
CXX_FLAGS = -DNDEBUG -O2 -w
CXX_EXTRA_FLAGS = -I. -a8 -I$(PLTDIR)\include -DPLT_SYSTEM_NT=1

.SUFFIXES:	

.SUFFIXES:	.cpp .obj .lib .exe

{$(SRCDIR)}.cpp{}.obj:
	$(CXX) -Jgx $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -o$@ $< 

{$(TESTSRCDIR)}.cpp{}.obj:
	$(CXX) -Jgd $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -o$@ $< 

### Include generic part

!INCLUDE ..\generic.mk

### Include auto dependencies (built with gcc)

!INCLUDE ..\depend.mk

### How to build things
.obj.exe :
	$(CXX) $< $(LIBPLT)

$(LIBPLT) : plt_ar.exe $(LIBPLT_OBJECTS)
	plt_ar tlib $@ $(LIBPLT_OBJECTS1)
	plt_ar tlib $@ $(LIBPLT_OBJECTS2)

### PLT Archiver Wrapper
plt_ar.exe:	..\plt_ar.cpp
	$(CXX) ..\plt_ar.cpp

clean :
	del *.obj
	del *.exe

mrproper : clean
	del *.lib
	del *.err
	del *.sym
	del *.mbr


