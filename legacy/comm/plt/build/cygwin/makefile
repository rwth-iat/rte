### -*-makefile-*-

###

### Cygwin/gcc specific part

###



MINGWINCLUDEPATH = /cygdrive/c/MinGW32/include



MAKE = make

RANLIB = ranlib

CXX_PLATFORM_FLAGS = -DPLT_SYSTEM_NT=1 -DCYGWIN  -I$(MINGWINCLUDEPATH)

#CXX_PLATFORM_FLAGS = -DPLT_SYSTEM_NT=1 -DCYGWIN

#CXX_FLAGS = -g -Wall -DPLT_DEBUG_INVARIANTS=0 -DPLT_DEBUG_NEW=1 -DPLT_DEBUG_NEW_REPORT=1

#CXX_FLAGS = -g -Wall -DNDEBUG

CXX_FLAGS = -O2 -Wall -DNDEBUG 



include ../unix.mk



