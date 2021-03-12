
add_ov_library(opcua)
ov_library_includes(opcua ov ksbase)

target_compile_definitions(opcua
        PUBLIC -DUA_EXTERNAL_NAMESPACES=1 -DSTATIC_LINKING=1 -DLOG_UA=1 -DLOG_UA_TO_OV=1 -DUA_DYNAMIC_LINKING -DUA_DYNAMIC_LINKING_EXPORT)
if(UNIX)
    target_compile_definitions(opcua
        PRIVATE -D_XOPEN_SOURCE=500 -D_GNU_SOURCE)
endif()
if (WIN32)
    target_link_libraries(opcua PRIVATE -lwinmm -lws2_32)
endif()
