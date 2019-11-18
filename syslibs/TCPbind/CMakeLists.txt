
add_ov_library(TCPbind)
ov_library_includes(TCPbind ov ksbase)

target_compile_definitions(TCPbind PRIVATE -DPLT_USE_WINSOCK2=1)
if (WIN32)
    target_link_libraries(TCPbind -lws2_32)
else()
    if (CYGWIN)
        target_compile_definitions(TCPbind PRIVATE -D_GNU_SOURCE=1)
    endif()
    target_link_libraries(TCPbind -lpthread)
endif()
