
add_executable(ov_dbutil
    ov_dbutil.c
    dump.c
    fbddump.c)
target_include_directories(ov_dbutil PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ov_dbutil ov)

# FIXME development install does currently not work due to the issue with OV_MODEL_INCLUDE_DIRECTORIES (see below)
# install(TARGETS acplt_builder
#     EXPORT ${PROJECT_NAME}
#     RUNTIME
#         DESTINATION bin
#     COMPONENT development
# )
