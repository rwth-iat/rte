# General OV preprocessor definitions for OV and all libs
if (WIN32)
    add_definitions(-DOV_SYSTEM_NT=1 -DPLT_SYSTEM_NT=1 -DFD_SETSIZE=128 -DWIN32)
elseif(APPLE)
    message(FATAL_ERROR "Building ACPLT/OV on Mac OS is not supported.")
elseif(CYGWIN)
    add_definitions(-DOV_SYSTEM_CYGWIN=1)
else()
    add_definitions(-DOV_SYSTEM_LINUX=1)
endif()
set_directory_properties(PROPERTIES COMPILE_DEFINITIONS_DEBUG -DOV_DEBUG)

# General compiler options for OV and all libs
add_compile_options(-std=c99 -Wall)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-Wno-unused-parameter -Winit-self  -Wno-attributes)
endif()