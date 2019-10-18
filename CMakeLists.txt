cmake_minimum_required(VERSION 3.10)

project(rte C)
include(cmake/ov_functions.cmake)
include(cmake/ov_definitions.cmake)

# OV Core system
add_subdirectory(core/ovheaders)
add_subdirectory(core/codegen)
add_subdirectory(core/ov)
add_subdirectory(core/runtimeserver)

# OV core tools
add_subdirectory(core/acplt_builder)
add_subdirectory(core/dbutil)

# Core Communication Libraries
add_subdirectory(syslibs/ksbase)
SET(BUILD_OV_LIBRARY_kshist OFF CACHE BOOL "")
add_subdirectory(syslibs/kshist)
add_subdirectory(syslibs/kshttp)
add_subdirectory(syslibs/ksxdr)
add_subdirectory(syslibs/TCPbind)
add_subdirectory(syslibs/UDPbind)
add_subdirectory(syslibs/opcua)
add_subdirectory(syslibs/ksapi)

# Function Block Libraries
add_subdirectory(syslibs/fb)
add_subdirectory(syslibs/ssc)

# HMI
add_subdirectory(syslibs/cshmi)


# Prevent cmake projects including this via add_subdirectory() from searching for OV elsewhere
set(CMAKE_DISABLE_FIND_PACKAGE_RTE TRUE CACHE BOOL "")
set(RTE_FOUND ON
    CACHE BOOL "" FORCE)

# Add RTE_CMAKE_DIR property to ov library target and export it to allow other projects to find our cmake files
set_property(TARGET ov
    PROPERTY RTE_CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set_property(TARGET ov
    APPEND
    PROPERTY EXPORT_PROPERTIES "RTE_CMAKE_DIR")


# Export all targets to make build tree importable via find_package(OV)
export(EXPORT ${PROJECT_NAME}
    FILE RTEConfig.cmake)

# Install export to make install tree importable via find_package(OV)
# FIXME development install does currently not work due to the issue with OV_MODEL_INCLUDE_DIRECTORIES (see below)
# install(EXPORT ${PROJECT_NAME}
#     DESTINATION cmake
#     COMPONENT development
#     FILE OVConfig.cmake)
