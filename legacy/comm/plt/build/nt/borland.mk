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

### Compiler
CXX = bcc32 -v-
CXX_FLAGS = -DNDEBUG -O2 -w
#CXX_FLAGS = 
CXX_EXTRA_FLAGS = -I. -a8 -I$(PLTDIR)\include -DPLT_SYSTEM_NT=1

.SUFFIXES:
.SUFFIXES:      .cpp .obj .lib .exe

{$(SRCDIR)}.cpp{}.obj:
	$(CXX) -Jg $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -o$@ $< 

### Include generic part
!INCLUDE ..\generic.mk

### Include auto dependencies (built with gcc)
!INCLUDE ..\depend.nt

### How to build things
.obj.exe :
	$(CXX) $< $(LIBPLT)

$(LIBPLT) : plt_ar.exe $(LIBPLT_OBJECTS)
	plt_ar tlib $@ $(LIBPLT_OBJECTS1)
	plt_ar tlib $@ $(LIBPLT_OBJECTS2)

#	tdstrp32 $(LIBPLT)



### PLT Archiver Wrapper
plt_ar.exe:     ..\plt_ar.cpp
	$(CXX) ..\plt_ar.cpp

clean :
	del *.obj
	del *.exe
	del *.lib
	del *.bak

mrproper : clean
	del *.lib
	del *.err
	del *.sym
	del *.mbr


