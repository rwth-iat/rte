add_executable(ov_runtimeserver
    ov_runtimeserver.c
    ovksxdr.c)
target_include_directories(ov_runtimeserver PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ov_runtimeserver ov)

# Special treatment for static linkin of ov libraries
option(OV_STATIC_LIBRARIES OFF "Link all ov libraries static to ov_runtimeserver")
if(OV_STATIC_LIBRARIES)
    set(OV_SERVERLIBS_FILE "${CMAKE_CURRENT_BINARY_DIR}/ov_serverlibs.c")
    set(OV_SERVERLIBS_H_FILE "${CMAKE_CURRENT_BINARY_DIR}/ov_serverlibs.h")
    target_include_directories(ov_runtimeserver PRIVATE ${CMAKE_CURRENT_BINARY_DIR})  # for ov_serverlibs.h
    set_target_properties(ov_runtimeserver
        PROPERTIES
            STATIC_LIBRARY_DEFINE_FILE "${OV_SERVERLIBS_FILE}"
            STATIC_LIBRARY_INCLUDE_FILE "${OV_SERVERLIBS_H_FILE}")
    target_sources(ov_runtimeserver PRIVATE ${OV_SERVERLIBS_FILE})
    file(WRITE ${OV_SERVERLIBS_FILE}
       "#include \"ov_config.h\"\n"
       "#include \"ov_database.h\"\n"
       "#include \"ov_result.h\"\n"
       "#include \"ov_logfile.h\"\n"
       "#include \"ov_macros.h\"\n"
       "#include \"ov_serverlibs.h\"\n"
       "\n"
       "Ov_BeginStaticLibraryTable\n"
    )
    file(WRITE ${OV_SERVERLIBS_H_FILE} "")
endif()


install(TARGETS ov_runtimeserver
    EXPORT ${PROJECT_NAME}
    RUNTIME
      DESTINATION bin
    COMPONENT runtime)
