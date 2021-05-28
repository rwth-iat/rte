
add_ov_library(TCPbind)
ov_library_includes(TCPbind ov ksbase)

if (WIN32)
    target_compile_definitions(TCPbind PUBLIC -DPLT_USE_WINSOCK2=1)
    target_link_libraries(TCPbind PRIVATE ws2_32.lib)
else()
    if (CYGWIN)
        target_compile_definitions(TCPbind PRIVATE -D_GNU_SOURCE=1)
    endif()
    target_link_libraries(TCPbind PRIVATE -lpthread)
endif()
