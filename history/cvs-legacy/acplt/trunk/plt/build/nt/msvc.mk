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
TESTDIR = $(PLTDIR)\tests\\

### Compiler
CXX = cl
CXX_FLAGS = /Zi /MTd
CXX_EXTRA_FLAGS = -I. -GR -I$(PLTDIR)\include -DPLT_SYSTEM_NT=1

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

$(LIBPLT) : plt_ar.exe $(LIBPLT_OBJECTS)
	lib /OUT:$@ $(LIBPLT_OBJECTS)
#	plt_ar tlib $@ $(LIBPLT_OBJECTS1)
#	plt_ar tlib $@ $(LIBPLT_OBJECTS2)

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


