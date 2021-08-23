
add_library(ovheaders INTERFACE)
target_include_directories(ovheaders INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>)

install(TARGETS ovheaders  # only to make cmake's install dependency check happy
    EXPORT ${PROJECT_NAME})
# FIXME development install does currently not work due to the issue with OV_MODEL_INCLUDE_DIRECTORIES (see below)
# install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
#     DESTINATION include
#     COMPONENT development
#     FILES_MATCHING PATTERN *.h
# )
