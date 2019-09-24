
add_ov_library(TCPbind TRUE)
ov_library_includes(TCPbind ov ksbase)

if (OV_LIBRARY_TCPbind)
    if (WIN32)
        target_link_libraries(TCPbind -lws2_32)
    else()
        target_link_libraries(TCPbind -lpthread)
    endif()
endif()