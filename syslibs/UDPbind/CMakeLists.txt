
add_ov_library(UDPbind)
ov_library_includes(UDPbind ov ksbase)

target_compile_definitions(UDPbind PRIVATE -DPLT_USE_WINSOCK2=1)
if (WIN32)
    target_link_libraries(UDPbind -lws2_32)
elseif (CYGWIN)
    target_compile_definitions(UDPbind PRIVATE -D_GNU_SOURCE=1)
endif()
