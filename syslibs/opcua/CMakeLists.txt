
add_ov_library(opcua)
ov_library_includes(opcua ov ksbase)

target_compile_definitions(opcua
        PRIVATE -DUA_EXTERNAL_NAMESPACES=1 -DLOG_UA=1 -DLOG_UA_TO_OV=1 -DUA_DYNAMIC_LINKING_EXPORT)
target_compile_definitions(opcua
		PUBLIC -DUA_DYNAMIC_LINKING )
if(UNIX)
    target_compile_definitions(opcua
        PRIVATE -D_XOPEN_SOURCE=500 -D_GNU_SOURCE)
endif()
if (WIN32)
    target_link_libraries(opcua PRIVATE winmm.lib ws2_32.lib)
endif()
