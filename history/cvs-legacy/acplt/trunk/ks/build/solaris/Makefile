#
# Platform specific makefile for the Solaris operating system.
#
# Make sure that you always define the symbol PLT_SYSTEM_SOLARIS
# when compiling for this operating system.
# Adjust the CXX_FLAGS according to your needs.
#

CXX_FLAGS = -DNDEBUG -O2 -Wall
#CXX_FLAGS = -g -Wall

CXX_PLATFORM_FLAGS = -DPLT_SYSTEM_SOLARIS=1
CXX_PLATFORM_LIBS = -lrpcsvc -lnsl -lsocket

PLATFORM = solaris
MAKE=make
RANLIB = ranlib

include ../unix.mk

# End of solaris/Makefile






