
add_ov_library(UDPbind TRUE)
ov_library_includes(UDPbind ov ksbase)

target_compile_definitions(UDPbind PRIVATE -DPLT_USE_WINSOCK2=1)
if (OV_LIBRARY_UDPbind)
    if (WIN32)
        target_link_libraries(UDPbind -lws2_32)
    endif()
endif()