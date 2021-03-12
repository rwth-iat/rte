
add_executable(acplt_builder
    acplt_builder.c
    common.c)
target_include_directories(acplt_builder PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(acplt_builder PRIVATE ov_codegen_lib)

# FIXME development install does currently not work due to the issue with OV_MODEL_INCLUDE_DIRECTORIES (see below)
# install(TARGETS acplt_builder
#     EXPORT ${PROJECT_NAME}
#     RUNTIME
#         DESTINATION bin
#     COMPONENT development
# )
