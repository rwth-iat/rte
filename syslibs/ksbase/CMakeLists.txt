
add_ov_library(ksbase)
ov_library_includes(ksbase ov)

if (WIN32)
    target_link_libraries(ksbase -lwinmm -lws2_32)
endif()
target_compile_definitions(ksbase PRIVATE -DLOG_KS_TO_OV=1)
target_compile_definitions(ksbase PUBLIC -DLOG_KS_WARNING=1 -DLOG_KS_ERROR=1)

if (OV_STATIC_LIBRARIES)
    target_link_libraries(ksbase kshist)
endif()
