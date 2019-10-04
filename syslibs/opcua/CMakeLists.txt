
add_ov_library(opcua FALSE)
ov_library_includes(opcua ov ksbase)

target_compile_definitions(opcua PRIVATE -DUA_EXTERNAL_NAMESPACES=1 -DSTATIC_LINKING=1 -DLOG_UA=1 -DLOG_UA_TO_OV=1 )
if(WIN32)
    target_compile_definitions(opcua
        PRIVATE -DUA_DYNAMIC_LINKING_EXPORT -DUA_DYNAMIC_LINKING)
else()
    target_compile_definitions(opcua
        PRIVATE -D_XOPEN_SOURCE=500 -D_GNU_SOURCE)
endif()