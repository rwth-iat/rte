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
CXX_FLAGS =
CXX_EXTRA_FLAGS = -w -I. -I$(PLTDIR)\include -DPLT_SYSTEM_NT=1

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

$(LIBPLT) : $(LIBPLT_OBJECTS)
	$(PLTDIR)\build\plt_ar tlib $@ $(LIBPLT_OBJECTS1)
	$(PLTDIR)\build\plt_ar tlib $@ $(LIBPLT_OBJECTS2)

clean :
	del *.obj
	del *.exe

mrproper : clean
	del *.lib
	del *.err
	del *.sym
	del *.mbr


