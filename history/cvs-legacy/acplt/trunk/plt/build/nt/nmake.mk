### -*-makefile-*-
### 
### Compiler & platform specific part for
### Microsoft Visual C++ / nmake / Windows NT

### Filename conventions
O=.obj
A=.lib
EXE=.exe
SRCDIR = ..\..\src\\
TESTSRCDIR = ..\..\tests\\

### Compiler
CXX = cl
CXX_FLAGS =
CXX_EXTRA_FLAGS = /W3 /GR /Zi -I. -I..\..\include -DPLT_SYSTEM_NT

.SUFFIXES:	

.SUFFIXES:	.cpp .obj .lib .exe

{$(SRCDIR)}.cpp{}.obj:
	$(CXX) $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c /Fo$@ $< 

{$(TESTSRCDIR)}.cpp{}.obj:
	$(CXX) $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c /Fo$@ $< 

### Include generic part

!INCLUDE ../generic.mk

### Include auto dependencies (built with gcc)

!INCLUDE ../depend.mk

### How to build things
.obj.exe :
	link $< $(LIBPLT)

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


