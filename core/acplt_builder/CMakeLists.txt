
add_executable(acplt_builder
    acplt_builder.c
    common.c)
target_include_directories(acplt_builder PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(acplt_builder PRIVATE ov_codegen_lib)

set_property(TARGET acplt_builder PROPERTY C_STANDARD 99)

# Define option to let the user disable default build of acplt_builder
option(BUILD_ACPLT_BUILDER
        "Compile acplt builder executable"
        ON)
# Remove targets from default build, if user wished
if(NOT BUILD_ACPLT_BUILDER)
    set_target_properties(acplt_builder
            PROPERTIES
            EXCLUDE_FROM_ALL 1
            EXCLUDE_FROM_DEFAULT_BUILD 1)
endif()

# FIXME development install does currently not work due to the issue with OV_MODEL_INCLUDE_DIRECTORIES (see below)
# install(TARGETS acplt_builder
#     EXPORT ${PROJECT_NAME}
#     RUNTIME
#         DESTINATION bin
#     COMPONENT development
# )
