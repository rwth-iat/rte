
add_ov_library(ov FALSE)
ov_library_includes(ov)
target_link_libraries(ov ovheaders)

if(UNIX)
    target_link_libraries(ov -ldl)
endif()
