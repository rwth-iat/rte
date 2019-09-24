add_executable(ov_runtimeserver
    ov_runtimeserver.c
    ov_serverlibs.c
    ovksxdr.c)
target_include_directories(ov_runtimeserver PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ov_runtimeserver ov)

install(TARGETS ov_runtimeserver
    EXPORT ${PROJECT_NAME}
    RUNTIME
      DESTINATION bin
    COMPONENT runtime)
