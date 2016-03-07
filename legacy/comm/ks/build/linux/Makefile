# Platform specific part for linux

RANLIB = touch

#CXX_FLAGS = -g -Wall -DPLT_DEBUG_INVARIANTS=0 -DPLT_DEBUG_NEW=1 -DPLT_DEBUG_NEW_REPORT=1

# The following set of flags compiles with optimization and no PLT debugging
# helpers (no alloc tracker, no debugging messages)
#CXX_FLAGS = -O2 -Wall -DNDEBUG

# Enable next set of flags when using the new server-side dynamic XDR
# streams, which buffer client i/o. (Prevents servers from blocking in i/o).
CXX_FLAGS = -O2 -Wall -DNDEBUG -DPLT_USE_BUFFERED_STREAMS=1

# Enable next set for usual debugging turn-around.
#CXX_FLAGS = -g -Wall -DNDEBUG -DPLT_USE_BUFFERED_STREAMS=1

CXX_PLATFORM_FLAGS = -DPLT_SYSTEM_LINUX=1
PLATFORM = linux

include ../unix.mk









