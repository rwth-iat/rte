CMAKE_MINIMUM_REQUIRED(VERSION 3.11)

MESSAGE("Processing ov_runtimeserver Source directory...")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR} ${OV_INCLUDE_DIR} ${OV_MODEL_DIR} ${CMAKE_CURRENT_BINARY_DIR})

SET(OV_OVM ${OV_MODEL_DIR}/ov.ovm)
SET(LIBOV_MOD_SRC ${CMAKE_CURRENT_BINARY_DIR}/ov.c)

SET (    OV_SERVER_SRC      ov_runtimeserver.c                            
                            ov_serverlibs.c
                            ovksxdr.c
    )

ADD_CUSTOM_COMMAND(OUTPUT ${LIBOV_MOD_SRC}
	COMMAND $<TARGET_FILE:ov_codegen> -I ${OV_MODEL_DIR} -f ${OV_OVM} -l ov
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	DEPENDS ov_codegen)

SET_SOURCE_FILES_PROPERTIES(${LIBOV_MOD_SRC} GENERATED)    

ADD_EXECUTABLE(ov_runtimeserver ${OV_SERVER_SRC} ${LIBOV_MOD_SRC})
TARGET_LINK_LIBRARIES(ov_runtimeserver libml libov)
TARGET_COMPILE_DEFINITIONS(ov_runtimeserver PRIVATE ${OV_DEFINES})

INSTALL(TARGETS ov_runtimeserver
    RUNTIME DESTINATION ${BIN_INSTALL_DIR})