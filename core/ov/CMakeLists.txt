
add_ov_library(ov)
ov_library_includes(ov)
target_link_libraries(ov ovheaders)

if(UNIX)
    target_link_libraries(ov -ldl)
endif()

option(OV_VALGRIND "Compile OV with valgrind support" OFF)
if(OV_VALGRIND)
    target_compile_definitions(ov PRIVATE -DOV_VALGRIND=1)
endif()
