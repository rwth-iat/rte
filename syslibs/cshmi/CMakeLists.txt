
add_ov_library(cshmi)
ov_library_includes(cshmi ov)

# Generate StaticDisplayComponent.fbd from staticfiles/ using generateStaticFBD.tcl
# (requires tclsh)
file(GLOB CSHMI_STATIC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/staticfiles/*.{xhtml,xml,js,css,svg})
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/StaticDisplayComponent.fbd
    COMMAND tclsh ${CMAKE_CURRENT_SOURCE_DIR}/generateStaticFBD.tcl ${CMAKE_CURRENT_SOURCE_DIR}/staticfiles ${CMAKE_CURRENT_BINARY_DIR}/StaticDisplayComponent.fbd
    DEPENDS ${CSHMI_STATIC_FILES}
    COMMENT "Generating cshmi static files FBD template"
    )
add_custom_target(cshmi_static_fbd
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/StaticDisplayComponent.fbd)
add_dependencies(cshmi cshmi_static_fbd)

# install rules for .fbd template files
INSTALL(DIRECTORY templates/
    DESTINATION templates/cshmi)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/StaticDisplayComponent.fbd
    DESTINATION templates/cshmi)
