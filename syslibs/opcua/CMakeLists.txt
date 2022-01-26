add_ov_library(opcua)
ov_library_includes(opcua ov ksbase)

if(BUILD_OV_LIBRARY_opcua)
# Custom CMake options for opcua library:
option(OV_UA_USE_INSTALLED_OPEN62541 "Use find_package to include an installed open62541 library instead of including the submodule." OFF)
option(OV_UA_ENABLE_UA_LOG "Print UA log output." ON)
option(OV_UA_USE_OV_LOGGER "Redirect UA log output to OV logger (OV_UA_USE_OV_LOGGER has to be ON)." ON)

# Set CMake options for open62541 library:
set(UA_NAMESPACE_ZERO "REDUCED" CACHE STRING "Enable reduced NS0 (default) to allow for faster server startup." FORCE)
set(UA_ENABLE_IMMUTABLE_NODES "ON" CACHE BOOL "Use getNodeCopy and replaceNode instead of modifying nodes directly." FORCE)
set(UA_ENABLE_NODEMANAGEMENT "OFF" CACHE BOOL "Nodemanagement is currently not implemented, as no backtransformations are implemented." FORCE)

# Include installed open62541 library or add submodule
if(OV_UA_USE_INSTALLED_OPEN62541)
    find_package(open62541 1.3) #TODO add with correct component name: REQUIRED COMPONENTS ImmutableNodes
else()
    add_subdirectory(open62541)
endif()

if(WIN32)
    target_compile_definitions(opcua PRIVATE -D_WINSOCKAPI_)
endif()

target_link_libraries(opcua PUBLIC open62541)
endif()