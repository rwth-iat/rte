
add_executable(ov_dbutil
    ov_dbutil.c
    dump.c
    fbddump.c)
target_include_directories(ov_dbutil PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ov_dbutil PRIVATE ov)

# Define option to let the user disable default build of ov_dbutil
option(BUILD_OV_DBUTIL
        "Compile dbutil executable"
        ON)
# Remove targets from default build, if user wished
if(NOT BUILD_OV_DBUTIL)
    set_target_properties(ov_dbutil
            PROPERTIES
            EXCLUDE_FROM_ALL 1
            EXCLUDE_FROM_DEFAULT_BUILD 1)
endif()

if(BUILD_OV_DBUTIL)
    install(TARGETS ov_dbutil
            EXPORT ${PROJECT_NAME}
            RUNTIME
            DESTINATION bin
            COMPONENT runtime)
endif()
