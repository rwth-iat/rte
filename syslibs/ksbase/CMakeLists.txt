
add_ov_library(ksbase TRUE)
ov_library_includes(ksbase ov)

if (WIN32)
    target_link_libraries(ksbase -lwinmm -lws2_32)
endif()