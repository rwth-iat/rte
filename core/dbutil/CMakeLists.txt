
add_executable(ov_dbutil
    ov_dbutil.c
    dump.c
    fbddump.c)
target_include_directories(ov_dbutil PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ov_dbutil ov)

install(TARGETS ov_dbutil
        EXPORT ${PROJECT_NAME}
        RUNTIME
        DESTINATION bin
        COMPONENT runtime)
