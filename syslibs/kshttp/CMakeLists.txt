
add_ov_library(kshttp)
ov_library_includes(kshttp ov ksbase)

# Generate generated_static_index.c from staticfiles/ using prebuild.tcl
# (requires tclsh)
file(GLOB KSHTTP_STATIC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/staticfiles/*)
add_custom_command (
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/generated_static_index.c
    COMMAND tclsh ${CMAKE_CURRENT_SOURCE_DIR}/prebuild.tcl ${CMAKE_CURRENT_SOURCE_DIR}/staticfiles ${CMAKE_CURRENT_BINARY_DIR}/generated_static_index.c
    DEPENDS ${KSHTTP_STATIC_FILES}
    COMMENT "Generating kshttp static files index"
    )
target_sources(kshttp PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/generated_static_index.c)
