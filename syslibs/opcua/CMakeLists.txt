add_ov_library(opcua)
ov_library_includes(opcua ov ksbase)

#alternative syntax:
set(UA_NAMESPACE_ZERO "REDUCED" CACHE STRING "Enable reduced NS0 (default) to allow for faster server startup." FORCE)
set(UA_ENABLE_AMALGAMATION "ON" CACHE BOOL "Merge als .c and .h files from open62541 in one open62541.c/.h" FORCE)
set(UA_AMALGAMATION_ARCHITECTURES "posix;win32" CACHE STRING "Include posix and win32 architecture." FORCE)
set(UA_ENABLE_IMMUTABLE_NODES "ON" CACHE BOOL "Use getNodeCopy and replaceNode instead of modifying nodes directly." FORCE)
set(UA_ENABLE_NODEMANAGEMENT "OFF" CACHE BOOL "Nodemanagement is currently not implemented, as no backtransformations are implemented." FORCE)
add_subdirectory(open62541)

target_compile_definitions(opcua
        PRIVATE -DLOG_UA=1 -DLOG_UA_TO_OV=1 -DUA_DYNAMIC_LINKING_EXPORT)
if(UNIX)
    target_compile_definitions(opcua PRIVATE -D_XOPEN_SOURCE=500 -D_GNU_SOURCE)
endif()
if (WIN32)
    target_link_libraries(opcua PRIVATE winmm.lib ws2_32.lib)
endif()

target_link_libraries(opcua PRIVATE open62541)