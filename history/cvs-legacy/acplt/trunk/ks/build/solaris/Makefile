# Platform specific part for solaris

RANLIB = ranlib
CXX_FLAGS = -g -Wall
CXX_PLATFORM_FLAGS = -DPLT_SYSTEM_SOLARIS=1
CXX_PLATFORM_LIBS = -lrpcsvc -lsocket
PLATFORM = solaris

include ../unix.mk

tfoxia$(EXE) : tfoxia$(O) tfoxia1$(O) $(LIBKS)
	$(CXX_LINK) -o $@ $^ $(LIBPLT) $(CXX_PLATFORM_LIBS) -lfox -lnsl $(CXX_LIBS)

depend: tfoxia.cpp tfoxia1.cpp








