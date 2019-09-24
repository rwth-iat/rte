
add_ov_library(UDPbind TRUE)
ov_library_includes(UDPbind ov ksbase)


if (OV_LIBRARY_UDPbind)
    if (WIN32)
        target_link_libraries(UDPbind -lws2_32)
    endif()
endif()